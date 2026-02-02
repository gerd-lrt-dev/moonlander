#include "Integrators/Dynamics.h"
#include <cmath>
#include <algorithm>

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
    double T_clamped = std::max(0.0, T);
    double mdot = T_clamped / (problem.params.Isp * problem.params.g0);

    xn.m = x.m - mdot * dt;

    // Massensicherheitslimit
    if (xn.m < problem.m_dry) xn.m = problem.m_dry;

    // --- Safety ---
    if (!std::isfinite(xn.h) || !std::isfinite(xn.v) || !std::isfinite(xn.m))
    {
        xn.h = 0.0;
        xn.v = 0.0;
        xn.m = problem.m_dry;
    }

    return xn;
}




