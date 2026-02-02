#include <nlopt.h>
#include "Optimization/thrustOptimizationProblem.h"
#include "Integrators/Dynamics.h"
#include <iostream>
#include <cmath>
#include <iomanip>

/// @brief Cost function for vertical thrust optimization
/// @brief Optimierte Kostenfunktion für 1D Lunar Lander
double thrustCostFunction(unsigned n, const double* u, double*, void* user_data)
{
    static int evalCounter = 0;
    evalCounter++;

    auto* problem = static_cast<ThrustOptimizationProblem*>(user_data);
    OptimizationState x = problem->x0;

    double cost_u_smooth = 0.0;


    // -----------------------------
    // Vorwärtssimulation
    // -----------------------------
    for (unsigned k = 0; k < n; ++k)
    {
        x = integrateEuler1D(x, u[k], problem->dt, *problem);

        if (!std::isfinite(x.h) || !std::isfinite(x.v) || !std::isfinite(x.m))
        {
            // extrem hoher Cost für NaN / Divergenz
            return 1e8;
        }

        // Quadratische Glättung des Schubs
        double uN = u[k] / problem->T_ref;
        cost_u_smooth += uN * uN;
    }

    // -----------------------------
    // Terminalkosten (entscheidend!)
    // -----------------------------
    double h_error = x.h - problem->r_target;   // Höhe vs Ziel
    double v_error = x.v;                       // Geschwindigkeit am Ende

    double cost_terminal = problem->w_hf * h_error * h_error
                           + problem->w_vf * v_error * v_error;

    // -----------------------------
    // Treibstoffverbrauch (sekundär)
    // -----------------------------
    double fuelUsed = (problem->x0.m - x.m) / problem->m_ref;
    double cost_fuel = problem->w_fuel * fuelUsed;

    // -----------------------------
    // Glättungskosten (optional)
    // -----------------------------
    double cost_smoothness = problem->w_smooth * cost_u_smooth * problem->dt;

    // -----------------------------
    // Gesamtkosten
    // -----------------------------
    double J = cost_terminal + cost_fuel + cost_smoothness;

    // -----------------------------
    // Debug-Ausgabe alle 20 Bewertungen
    // -----------------------------
    if (evalCounter % 20 == 0)
    {
        std::cerr
            << "\n[Cost Eval #" << evalCounter << "]\n"
                << " EndState: h=" << std::setprecision(12) << x.h
            << " v=" << x.v
            << " m=" << x.m << "\n"
            << " FuelUsed      : " << fuelUsed << "\n"
            << " TerminalCost  : " << cost_terminal << "\n"
            << " SmoothCost    : " << cost_smoothness << "\n"
            << " ---- TOTAL J  : " << J << "\n";
    }

    return J;
}




