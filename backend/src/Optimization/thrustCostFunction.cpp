#include <nlopt.h>
#include "Optimization/thrustOptimizationProblem.h"
#include "Integrators/Dynamics.h"
#include <iostream>
#include <cmath>
#include <iomanip>

/// @brief Verbesserte Kostenfunktion für 1D Lunar Lander
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
            return 1e8; // Extrem hoher Cost für Divergenz
        }

        // Glättung des Schubs
        double uN = u[k] / problem->T_ref;
        cost_u_smooth += uN * uN;
    }

    // -----------------------------
    // Terminalkosten (normiert!)
    // -----------------------------
    double h_rel = x.h - problem->r_target; // Höhe über Boden
    double h_norm = h_rel / 100.0;          // skaliert auf ~1
    double v_norm = (x.v - problem->v_safe) / 5.0; // skaliert auf ~1

    double cost_terminal = problem->w_hf * h_norm * h_norm
                           + problem->w_vf * v_norm * v_norm;

    // -----------------------------
    // Treibstoffverbrauch
    // -----------------------------
    double fuelUsed = (problem->x0.m - x.m) / problem->m_ref; // normiert
    double cost_fuel = problem->w_fuel * fuelUsed;

    // -----------------------------
    // Glättungskosten
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
        /*
        std::cerr << "\n[Cost Eval #" << evalCounter << "]\n"
                  << " EndState: h_rel=" << std::setprecision(12) << h_rel
                  << " v=" << x.v
                  << " m=" << x.m << "\n"
                  << " FuelUsed      : " << fuelUsed << "\n"
                  << " TerminalCost  : " << cost_terminal << "\n"
                  << " SmoothCost    : " << cost_smoothness << "\n"
                  << " ---- TOTAL J  : " << J << "\n";
*/
    }

    return J;
}
