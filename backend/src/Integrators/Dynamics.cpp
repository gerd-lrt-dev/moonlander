#include "Integrators/Dynamics.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <iomanip>

OptimizationState integrateEuler1D(const OptimizationState& x, double T, double dt, const ThrustOptimizationProblem& problem)
{
    static int integratorCounter = 0;  // Zähler für Debug-Ausgaben
    integratorCounter++;

    OptimizationState xn = x;

    double a_grav   = - problem.params.mu_moon / ((x.h) * (x.h));
    double a_thrust = T / x.m;
    double a  = a_grav + a_thrust;

    // --- DEBUG: Nur jeden xten Schritt ---
    if (integratorCounter % 10 == 0)
    {

        std::cerr << std::fixed << std::setprecision(6);  // feste Schreibweise, 6 Nachkommastellen
        std::cerr << "\n[Integrator Step #" << integratorCounter << "]"
                  << "\n  IN:"
                  << "\n    h         = " << x.h - problem.params.R_moon
                  << "\n    T         = " << T
                  << "\n    v         = " << x.v
                  << "\n    m         = " << x.m
                  << "\n    a_grav    = " << a_grav
                  << "\n    a_thrust  = " << a_thrust
                  << "\n    a_total   = " << a
                  << "\n    dt        = " << dt
                  << std::endl;

    }


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
