#include <nlopt.h>
#include "Optimization/thrustOptimizationProblem.h"
#include "Integrators/Dynamics.h"
#include <iostream>
#include <cmath>

/// @brief Cost function for vertical thrust optimization
double thrustCostFunction(unsigned n, const double* u, double*, void* user_data)
{
    static int evalCounter = 0;
    evalCounter++;

    auto* problem = static_cast<ThrustOptimizationProblem*>(user_data);
    OptimizationState x = problem->x0;

    double cost_u_smooth = 0.0;

    // -----------------------------
    // Forward simulation
    // -----------------------------
    for (unsigned k = 0; k < n; ++k)
    {
        x = integrateEuler1D(x, u[k], problem->dt, *problem);

        if (!std::isfinite(x.h) || !std::isfinite(x.v) || !std::isfinite(x.m))
        {
            if (evalCounter % 20 == 0)
            {
                std::cerr << "[Cost][NaN] h=" << x.h
                          << " v=" << x.v
                          << " m=" << x.m << "\n";
            }
            return 1e8;
        }

        double uN = u[k] / problem->T_ref;
        cost_u_smooth += uN * uN;
    }

    // -----------------------------
    // Fuel
    // -----------------------------
    double fuelUsed = (problem->x0.m - x.m) / problem->m_ref;

    // -----------------------------
    // Terminal
    // -----------------------------
    double hTf = x.h / problem->h_ref;
    double vTf = x.v / problem->v_safe;

    double cost_terminal =
        problem->w_hf * hTf * hTf +
        problem->w_vf * vTf * vTf;

    // -----------------------------
    // Velocity constraint
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
    // Descent
    // -----------------------------
    double cost_descent = x.h / problem->h_ref;

    // -----------------------------
    // Total
    // -----------------------------
    double J =
        problem->w_fuel * fuelUsed +
        problem->w_terminal * cost_terminal +
        problem->w_v_constraint * cost_v_constraint +
        problem->w_smooth * cost_u_smooth * problem->dt +
        problem->w_descent * cost_descent;

    // -----------------------------
    // DEBUG OUTPUT
    // -----------------------------
    if (evalCounter % 20 == 0)
    {
        std::cerr
            << "\n[Cost Eval #" << evalCounter << "]\n"
            << " EndState: h=" << x.h
            << " v=" << x.v
            << " m=" << x.m << "\n"
            << " FuelUsed      : " << fuelUsed << "\n"
            << " TerminalCost  : " << cost_terminal << "\n"
            << " VelConstraint : " << cost_v_constraint << "\n"
            << " SmoothCost    : " << cost_u_smooth * problem->dt << "\n"
            << " DescentCost   : " << cost_descent << "\n"
            << " ---- TOTAL J  : " << J << "\n";
    }

    return J;
}


