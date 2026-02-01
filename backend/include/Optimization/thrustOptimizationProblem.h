#pragma once
#include "Optimization/optimizationStruct.h"
#include "Optimization/modelParams.h"

/**
 * @brief Defines the optimization problem for spacecraft thrust control.
 *
 * This struct contains all necessary parameters to formulate a trajectory
 * optimization problem, including initial state, model parameters, cost weights,
 * reference values, and physical constraints.
 */
struct ThrustOptimizationProblem
{
    // -----------------------------
    // Initial state & model
    // -----------------------------
    OptimizationState x0;
    OptimizationModelParams params;

    // -----------------------------
    // Horizon
    // -----------------------------
    unsigned N;
    double dt;

    // -----------------------------
    // Cost weights
    // -----------------------------
    double w_fuel;         // fuel usage
    double w_terminal;     // global terminal multiplier
    double w_hf;           // final height
    double w_vf;           // final velocity
    double w_v_constraint; // velocity bounds
    double w_smooth;       // thrust smoothing
    double w_descent;      // descent encouragement

    // -----------------------------
    // Reference values
    // -----------------------------
    double h_ref;
    double v_safe;
    double m_ref;
    double T_ref;

    // -----------------------------
    // Physical constraints
    // -----------------------------
    double m_dry;
    double v_min;
    double v_max;
};

