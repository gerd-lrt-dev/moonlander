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
    // Initial state and model
    // -----------------------------
    /// Initial state of the spacecraft
    OptimizationState x0;

    /// Physical model parameters (gravity, Isp, max/min thrust)
    OptimizationModelParams params;

    // -----------------------------
    // Optimization horizon
    // -----------------------------
    /// Number of time steps in the optimization horizon
    unsigned N;

    /// Time step length [s]
    double dt;

    // -----------------------------
    // Running cost weights
    // -----------------------------
    /// Weight for height deviation in running cost
    double w_h;

    /// Weight for velocity deviation in running cost
    double w_v;

    /// Weight for fuel consumption (mass change) in running cost
    double w_m;

    /// Weight for thrust usage in running cost
    double w_T;

    /// Weight encouraging continuous descent
    double w_d;

    /// Weight penalizing violations of min/max velocity
    double w_v_constraint;

    /// Weight penalizing deviation from height-dependent velocity profile
    double w_v_profile;

    // -----------------------------
    // Terminal cost weights
    // -----------------------------
    /// Weight for final height penalty at the end of horizon
    double w_hf;

    /// Weight for final velocity penalty at the end of horizon
    double w_vf;

    // -----------------------------
    // Reference values for normalization
    // -----------------------------
    /// Reference height for normalization
    double h_ref;

    /// Reference velocity for normalization
    double v_ref;

    /// Safe landing velocity
    double v_safe;

    /// Reference mass for normalization
    double m_ref;

    /// Reference thrust for normalization
    double T_ref;

    // -----------------------------
    // Physical constraints
    // -----------------------------
    /// Minimum allowed spacecraft mass
    double m_dry;

    /// Minimum allowed velocity (maximum descent speed)
    double v_min;

    /// Maximum allowed velocity (hover / ascent limit)
    double v_max;

    /// Gain for height-dependent velocity reference (v_ref(h) = -k_v * sqrt(h))
    double k_v;
};
