#include "Integrators/Dynamics.h"
#include <cmath>
#include <algorithm>
#include "spacemath.h"
/*
OptimizationState integrateEuler(
    const OptimizationState& x,           // current state
    double T,                             // thrust
    double dt,                            // time step
    const ThrustOptimizationProblem& problem
    )
{
    OptimizationState xn = x;            // start from current state
    const auto& p = problem.params;      // grab model parameters

    // Safety: prevent mass dropping below dry mass
    if (xn.m <= problem.m_dry)
    {
        xn.m = problem.m_dry;
        xn.v = 0.0;  // optional: "out of fuel" stop
        return xn;
    }

    // Position update
    xn.h += xn.v * dt;

    // Velocity update: simple vertical dynamics
    xn.v += (T / xn.m - p.g) * dt;

    // Mass update
    double mdot = T / (p.Isp * p.g0);
    xn.m = std::max(xn.m - mdot * dt, problem.m_dry);

    return xn;
}
*/

OptimizationState integrateEuler1D(const OptimizationState& x, double T, double dt, const ThrustOptimizationProblem& problem)
{
    OptimizationState xn = x;

    // --- Gravity depends on height ---
    double g = problem.params.mu_moon / ((problem.params.R_moon + x.h) * (problem.params.R_moon + x.h));

    // --- Total acceleration ---
    double a = (T / x.m) - g;

    // --- Position & velocity update ---
    xn.h += x.v * dt + 0.5 * a * dt * dt;
    xn.v += a * dt;

    // --- Mass flow ---
    double mdot = spacemath::calcMassFlowBasedOnThrust(T, problem.params.Isp, problem.params.g0);
    xn.m = std::max(x.m - mdot * dt, problem.m_dry);

    // --- Safety ---
    if (!std::isfinite(xn.h) || !std::isfinite(xn.v) || !std::isfinite(xn.m))
    {
        xn.h = 0.0;
        xn.v = 0.0;
        xn.m = problem.m_dry;
    }

    return xn;
}




