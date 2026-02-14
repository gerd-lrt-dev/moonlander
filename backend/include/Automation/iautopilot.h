#pragma once

#include "Controller/iController.h"

/**
 * @brief Descent control phases of the Energy-Guided Descent Controller.
 *
 * The modes are selected based on the brake ratio R_brake = h / d_brake.
 * They do not change the physics, only controller aggressiveness,
 * damping, and velocity limits.
 */
enum class DescentMode
{
    /**
     * @brief MODE_A – Energy Dissipation
     *
     * High altitude / large braking reserve.
     * Primary goal is reducing kinetic energy while allowing
     * relatively high descent velocities.
     *
     * Typical condition: R_brake > 3
     */
    MODE_A,

    /**
     * @brief MODE_B – Controlled Descent
     *
     * Mid altitude with moderate braking reserve.
     * Descent is actively shaped; velocity targets are reduced
     * and damping is increased.
     *
     * Typical condition: 1.5 < R_brake <= 3
     */
    MODE_B,

    /**
     * @brief MODE_C – Terminal Approach
     *
     * Low altitude / small braking reserve.
     * Strict velocity limits and strong damping to avoid oscillation
     * and prepare for touchdown.
     *
     * Typical condition: 1.0 < R_brake <= 1.5
     */
    MODE_C,

    /**
     * @brief MODE_D – Critical Braking
     *
     * Minimal or insufficient braking reserve.
     * Maximum control authority; may apply near-full thrust.
     * Represents last-chance recovery or emergency braking.
     *
     * Typical condition: R_brake <= 1.0
     */
    MODE_D
};

class IAutopilot{
public:
    virtual ~IAutopilot() = default;

    virtual double setAutoThrustInNewton(IController *useController, const double &T_max, const double &vel, const double &h, const double &dt, const double &m, const double &g) const = 0;

    virtual double normalizAutoThrust(const double &thrustInNewton, const double &T_max) const = 0;
};
