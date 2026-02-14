#pragma once

#include "Automation/iautopilot.h"

/**
 * @brief Adaptive descent controller for lunar/planetary landers.
 *
 * Implements an energy-guided descent strategy with multiple phases (modes)
 * that dynamically adjust controller parameters based on brake ratio.
 *
 * Uses a PD-based velocity controller to compute thrust commands while
 * ensuring safe and efficient landing.
 */
class AdaptiveDescentController : public IAutopilot
{
public:
    /**
     * @brief Constructor.
     * @param v_safe Safe descent velocity (used to initialize v_safe_ and related conservative value)
     */
    AdaptiveDescentController(double v_safe)
        : v_safe_(v_safe > 0 ? -v_safe : v_safe),
        v_conservative_(v_safe_ * 0.8)
    {
    }

    /**
     * @brief Computes the required thrust in Newtons for the next timestep.
     *
     * Calculates the appropriate thrust using the selected controller,
     * taking into account the current velocity, height, mass, and gravity.
     * Adjusts controller parameters according to the brake ratio and mode.
     *
     * @param useController Controller object (e.g., PD controller) to compute thrust
     * @param T_max Maximum thrust available [N]
     * @param vel Current vertical velocity [m/s]
     * @param h Current altitude [m]
     * @param dt Timestep duration [s]
     * @param m Lander mass [kg]
     * @param g Local gravity [m/s²]
     * @return Thrust command in Newtons for this timestep
     */
    double setAutoThrustInNewton(IController *useController, const double &T_max, const double &vel, const double &h, const double &dt, const double &m, const double &g) const override;

    /**
     * @brief Normalizes thrust into a 0..1 range based on maximum thrust.
     *
     * @param thrustInNewton Thrust command in Newtons
     * @return Normalized thrust (0.0 = no thrust, 1.0 = T_max)
     */
    double normalizAutoThrust(const double &thrustInNewton, const double &T_max) const override;

private:
    //***********************************************************
    //*************        Members                   ************
    //***********************************************************

    /**
     * @brief Safe descent velocity [m/s]
     */
    double v_safe_;

    /**
     * @brief Conservative descent velocity used internally
     */
    double v_conservative_;

    /**
     * @brief Current descent mode determined from brake ratio
     */
    DescentMode descentMode_;

    //***********************************************************
    //*************    Memberfunctions                ************
    //***********************************************************

    /**
     * @brief Calculates the maximum acceleration achievable by the lander.
     *
     * @param T_max Maximum thrust [N]
     * @param m Current mass [kg]
     * @param g Gravity [m/s²]
     * @return Maximum acceleration [m/s²], clamped to avoid division by zero
     */
    double calcMaxAcc(const double &T_max, const double &m, const double &g) const;

    /**
     * @brief Calculates the minimum braking distance for a given velocity and max acceleration.
     *
     * @param vel Current vertical velocity [m/s]
     * @param a_max Maximum available acceleration [m/s²]
     * @return Braking distance [m]
     */
    double calcBrakingDistance(const double &vel, const double &a_max) const;

    /**
     * @brief Computes the brake ratio (h / d_brake) used to select the descent mode.
     *
     * @param h Current altitude [m]
     * @param d_brake Required braking distance [m]
     * @return Brake ratio (unitless)
     */
    double calcBrakeRatio(const double &h, const double &d_brake) const;

    /**
     * @brief Calculates the target velocity for the next timestep.
     *
     * Takes into account maximum acceleration, current height, and reserve factor k_r.
     *
     * @param a_max Maximum acceleration [m/s²]
     * @param h Current altitude [m]
     * @param k_r Reserve factor for safe descent
     * @return Target vertical velocity [m/s]
     */
    double calcTargetVelocity(const double &a_max, const double &h, const double k_r, const double &vel, const double &dt) const;

    /**
     * @brief Calculates the hover thrust required to balance gravity.
     *
     * @param m Lander mass [kg]
     * @param g Gravity [m/s²]
     * @return Hover thrust [N]
     */
    double calcHoverThrust(const double &m, const double &g) const;

    /**
     * @brief Calculates normalized hover throttle (0..1) based on T_max.
     *
     * @param m Lander mass [kg]
     * @param g Gravity [m/s²]
     * @param T_max Maximum thrust [N]
     * @return Normalized throttle for hovering
     */
    double calcHoverThrottle(const double &m, const double &g, const double &T_max) const;

    /**
     * @brief Saturates thrust command to allowed range [0, T_max].
     *
     * @param T_cmd Thrust command [N]
     * @param T_max Maximum thrust [N]
     * @return Clamped thrust [N]
     */
    double calcSaturation(const double &T_cmd, const double &T_max) const;

    /**
     * @brief Normalizes thrust command to 0..1 range.
     *
     * @param T_cmd Thrust command [N]
     * @param T_max Maximum thrust [N]
     * @return Normalized thrust (0..1)
     */
    double calcNormalizedThrust(const double &T_cmd, const double &T_max) const;

    /**
     * @brief Interpolates reserve factor k_r based on brake ratio.
     *
     * @param R_brake Current brake ratio
     * @return Interpolated k_r for target velocity computation
     */
    double interpolate_k_r(double R_brake) const;

    /**
     * @brief Interpolates proportional gain K_p for PD controller based on brake ratio.
     *
     * @param R_brake Current brake ratio
     * @return Interpolated K_p
     */
    double interpolate_Kp(double R_brake) const;

    /**
     * @brief Interpolates derivative gain K_d for PD controller based on brake ratio.
     *
     * @param R_brake Current brake ratio
     * @return Interpolated K_d
     */
    double interpolate_Kd(double R_brake) const;
};
