#include <nlopt.h>
#include "Optimization/thrustOptimizationProblem.h"
#include "Integrators/Dynamics.h"
#include <iostream>
#include <cmath>

/// @brief Cost function for vertical thrust optimization
double thrustCostFunction(
    unsigned n,
    const double* u,
    double* /*grad*/,
    void* user_data
    )
{
    static int evalCount = 0;
    evalCount++;

    // Cast user data to our problem struct
    auto* problem = static_cast<ThrustOptimizationProblem*>(user_data);

    OptimizationState x = problem->x0;

    // -----------------------------
    // Forward simulation over control horizon
    // -----------------------------
    for (unsigned k = 0; k < n; ++k)
    {
        x = integrateEuler(x, u[k], problem->dt, *problem);

        // Numerical safety
        if (!std::isfinite(x.h) || !std::isfinite(x.v) || !std::isfinite(x.m))
        {
            return 1e8;
        }
    }

    // -----------------------------
    // Terminal state normalization
    // -----------------------------
    double hN = x.h / problem->h_ref;
    double vN = x.v / problem->v_ref;
    double mN = (problem->x0.m - x.m) / problem->m_ref;

    double cost_h = hN * hN;
    double cost_v = vN * vN;
    double cost_m = mN * mN;

    // -----------------------------
    // Control effort (thrust usage)
    // -----------------------------
    double cost_u = 0.0;
    for (unsigned k = 0; k < n; ++k)
    {
        double uN = u[k] / problem->T_ref;
        cost_u += uN * uN;
    }
    cost_u *= problem->dt;

    // -----------------------------
    // Velocity constraints (prevent unrealistic hovering/falling)
    // -----------------------------
    double cost_v_constraint = 0.0;

    if (x.v > problem->v_max)
    {
        double dv = x.v - problem->v_max;
        cost_v_constraint += dv * dv;
    }

    if (x.v < problem->v_min)
    {
        double dv = problem->v_min - x.v;
        cost_v_constraint += dv * dv;
    }

    // -----------------------------
    // Optional height-dependent velocity reference
    // -----------------------------
    double cost_v_profile = 0.0;
    if (x.h > 0.0)
    {
        double v_ref = -problem->k_v * std::sqrt(x.h);
        double dv = x.v - v_ref;
        cost_v_profile = dv * dv;
    }

    // -----------------------------
    // Terminal costs (landing safety)
    // -----------------------------
    double hTf = x.h / problem->h_ref;
    double vTf = x.v / problem->v_safe;

    double J_terminal =
        problem->w_hf * hTf * hTf +
        problem->w_vf * vTf * vTf;

    // -----------------------------
    // Total cost
    // -----------------------------
    double J =
        problem->w_h * cost_h +
        problem->w_v * cost_v +
        problem->w_m * cost_m +
        problem->w_T * cost_u +
        problem->w_v_constraint * cost_v_constraint +
        problem->w_v_profile * cost_v_profile +
        problem->w_d * x.h +  // simple descent encouragement
        J_terminal;

    // -----------------------------
    // Sparse debug output
    // -----------------------------
    if (evalCount % 10 == 0)
    {
        std::cerr
            << "[Cost]"
            << " eval=" << evalCount
            << " h=" << x.h
            << " v=" << x.v
            << " m=" << x.m
            << " | Jh=" << cost_h
            << " Jv=" << cost_v
            << " Jm=" << cost_m
            << " Ju=" << cost_u
            << " JvC=" << cost_v_constraint
            << " JvP=" << cost_v_profile
            << " J=" << J
            << std::endl;
    }

    return J;
}
