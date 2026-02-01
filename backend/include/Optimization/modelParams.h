#pragma once

/**
 * @brief Contains the physical parameters of the system used for integration and dynamics.
 *
 * This struct encapsulates constants like gravity, specific impulse, and thrust limits.
 * It is meant to be passed to integrators (e.g., Euler, Runge-Kutta) to compute state evolution.
 */
struct OptimizationModelParams
{
    /// Gravitational acceleration [m/s²]
    double g;

    /// Standard gravity used for specific impulse calculations [m/s²]
    double g0;

    /// Specific impulse of the thruster [s]
    double Isp;

    /// Maximum allowed thrust [N]
    double T_max;

    /// Minimum allowed thrust [N] (possibly 0)
    double T_min;
};
