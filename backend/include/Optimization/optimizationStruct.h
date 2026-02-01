#pragma once

/**
 * @brief Represents the dynamic state of the spacecraft at a given time.
 *
 * This struct holds the essential variables that define the system's current
 * state for integration and optimization.
 */
struct OptimizationState
{
    /// Height of the spacecraft relative to the moon's center [m]
    double h;

    /// Vertical velocity of the spacecraft (positive upwards) [m/s]
    double v;

    /// Total mass of the spacecraft [kg]
    double m;
};
