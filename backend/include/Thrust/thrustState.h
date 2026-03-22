#ifndef THRUSTSTATE_H
#define THRUSTSTATE_H

/**
 * @brief Dynamic state of the engine thrust.
 *
 * Contains all values that evolve over simulation time, including the
 * commanded thrust, the actual generated thrust (after dynamic response),
 * and a flag marking whether a new command was received.
 */
struct ThrustState
{
    double target;
    /**<
     * @brief Commanded thrust setpoint.
     * @unit Newtons [N]
     * Desired thrust level set by guidance or control algorithms.
     */

    double current;
    /**<
     * @brief Actual thrust after dynamic response.
     * @unit Newtons [N]
     * Represents the current thrust output of the engine after
     * applying the engine response model.
     */
};

#endif // THRUSTSTATE_H
