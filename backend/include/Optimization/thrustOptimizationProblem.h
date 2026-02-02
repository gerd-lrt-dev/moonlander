#pragma once
#include "Optimization/optimizationStruct.h"
#include "Optimization/modelParams.h"

/**
 * @brief Struct defining the thrust optimization problem for lunar landing.
 *
 * This struct contains all parameters necessary for the NLopt-based vertical
 * thrust optimization of the lunar lander. It includes initial state, physical
 * model parameters, optimization horizon, cost weights, reference values,
 * physical constraints, and the target altitude (lunar surface).
 *
 * Adjusting the weights allows tuning for:
 * - Aggressiveness of descent
 * - Terminal safety (velocity & height at touchdown)
 * - Fuel efficiency
 * - Thrust smoothing
 *
 * The optimization currently assumes a 1D vertical descent model.
 */
struct ThrustOptimizationProblem
{
    // -----------------------------
    // Initial state & model
    // -----------------------------

    OptimizationState x0;           ///< Initial state of the spacecraft (height, velocity, mass)
    OptimizationModelParams params; ///< Physical model parameters (gravity, Isp, etc.)

    // -----------------------------
    // Optimization horizon
    // -----------------------------

    unsigned N;  ///< Number of time steps in the optimization horizon
    double dt;   ///< Duration of each time step [s]

    // -----------------------------
    // Cost weights
    // -----------------------------

    double w_fuel;         ///< Weight for fuel usage. Higher -> optimizer prefers low fuel consumption.
    double w_terminal;     ///< Global multiplier for terminal cost. Higher -> final state more important.
    double w_hf;           ///< Weight for final height at end of horizon. Higher -> optimizer penalizes high touchdown altitude.
    double w_vf;           ///< Weight for final velocity at end of horizon. Higher -> optimizer penalizes exceeding safe touchdown speed.
    double w_v_constraint; ///< Weight for velocity constraints (v_min / v_max). Higher -> stronger enforcement of limits.
    double w_smooth;       ///< Weight for thrust smoothing. Higher -> optimizer prefers gradual thrust changes.
    double w_descent;      ///< Weight encouraging descent. Higher -> optimizer prefers early falling phase instead of immediate hovering.

    // -----------------------------
    // Reference values
    // -----------------------------

    double h_ref;   ///< Reference height [m] for normalization in cost function.
    double v_safe;  ///< Maximum safe landing velocity [m/s].
    double m_ref;   ///< Reference mass [kg] for normalization in cost function.
    double T_ref;   ///< Reference thrust [N] for normalization in cost function.

    // -----------------------------
    // Physical constraints
    // -----------------------------

    double m_dry;  ///< Minimum allowed mass of the spacecraft (empty mass) [kg].
    double v_min;  ///< Minimum allowed vertical velocity (max descent speed) [m/s].
    double v_max;  ///< Maximum allowed vertical velocity (hover/ascent limit) [m/s].

    // -----------------------------
    // Target properties
    // -----------------------------

    double r_target; ///< Target height / lunar surface radius [m]. Usually R_moon.
};


