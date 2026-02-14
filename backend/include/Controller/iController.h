#pragma once

/**
 * @brief Abstract base class for all controllers.
 *
 * Defines the interface for a generic controller that calculates a control signal
 * based on a target and measured value. Designed to be overridden by specific
 * controller types (e.g., PD, PID, Bang-Bang).
 */
class IController
{
public:
    /**
     * @brief Virtual destructor for safe polymorphic deletion.
     */
    virtual ~IController() = default;

    /**
     * @brief Compute the control output for a single timestep.
     *
     * @param targetValue Desired setpoint.
     * @param measuredValue Current measured value.
     * @param K_p Proportional gain.
     * @param K_d Derivative gain.
     * @param dt Time step since last update.
     * @return Control output (e.g., throttle or force command).
     *
     * @note This function is intended to be overridden in derived classes.
     */
    virtual double control(const double &targetValue, const double &measuredValue, const double &K_p, const double &K_d, const double &dt) const = 0;
};
