#ifndef ENGINECONFIG_H
#define ENGINECONFIG_H
#include "vector3.h"
#include <iostream>

/**
 * @brief Configuration parameters for a spacecraft engine.
 *
 * This structure contains all static (time-invariant) parameters describing
 * the physical behavior of the engine, including thrust direction, response
 * dynamics, and specific impulse.
 *
 * All values are checked for physical plausibility. If there are values
 * that prevent the simulation from running correctly, default values
 * are automatically set.
 */
struct EngineConfig
{
    double Isp;
    /**<
     * @brief Specific impulse of the engine.
     * @unit seconds [s]
     * Determines how efficiently the engine converts propellant into thrust.
     */

    double timeConstant;
    /**<
     * @brief Time constant of the thrust response model.
     * @unit seconds [s]
     * Used in a first-order engine dynamics model describing how quickly
     * the current thrust approaches the target thrust.
     */

    double responseRate;
    /**<
     * @brief Maximum rate of thrust change.
     * @unit Hertz [Hz]
     * Defines the maximum frequency at which the thrust can adjust
     * to new setpoints (i.e., response speed of the engine controller).
     */

    double maxThrust;
    /**<
     * @brief Maximum amount of thrust
     * @unit Newton [N]
     */

    Vector3 direction;
    /**<
     * @brief Normalized thrust direction in the spacecraft body frame.
     * @unit unit vector [-]
     * Specifies the direction in which thrust is applied.
     */



    static EngineConfig Create(double Isp, double timeConstant, double responseRate, double maxThrust, Vector3 direction)
    {

        /**
         * @param Isp Specific impulse of the engine
         *
         * Engine Type	Isp (vacuum)
            - Cold Gas Thruster	                        40–80 [s]
            - Monopropellant (hydrazine)	            200–230 [s]
            - Bipropellant (MMH/N₂O₄)	                280–320 [s]
            - Lunar Lander Descent Engine (Apollo LM)	~311 [s]
            - Modern Cryogenic	                        350–460 [s]
            - Nuclear Thermal	                        600–900 [s]
         */
        if (Isp < 40.0 || Isp > 900.0)
        {
            std::cerr << "[EngineConfig] Warning: Isp out of range (" << Isp << "), resetting to default 300 [s]." << std::endl;
            Isp = 300.0;
        }

        /**
         * @param timeConstant Engine response time (τ)
         *
         * Engine    τ (seconds)
            - Cold gas                                      < 0.1 [s]
            - Hydrazine thruster                            0.1–0.3 [s]
            - Main lander engines (LM, Blue Moon, etc.)     0.3–1.5 [s]
            - Extremely sluggish systems                    > 2 [s]
            - Min: 0.05 (below 0.05 s it becomes numerically unstable)
            - Max: 3.0 (anything above 3s reacts too slowly for a landing)
            - Ideal for landing simulation: 0.3-0.8 [s]
         */
        if (timeConstant < 0.05 || timeConstant > 3.0)
        {
            std::cerr << "[EngineConfig] Warning: time constant tau out of range (" << timeConstant << "), resetting to default 0.5 [s]." << std::endl;
            timeConstant = 0.05;
        }

        /**
         * @param responseRate Maximum thrust change rate
         *
         * Small hydrazine thrusters: very fast (10–30 Hz)

         * Response Rate (Hz)
            - Main engines: 2–10 [Hz]
            - SpaceX Merlin at TVC: ~2–5 [Hz]
            - Apollo LM DES: ~3 [Hz] Controllability
            - Min: 0.5 Hz (any slower is completely useless)
            - Max: 50 Hz (above this, controlled flying is pointless)
            - Realistic Lander: 2-10 [Hz]
         */
        if (responseRate < 2.0 || responseRate > 10.0)
        {
            std::cerr << "[EngineConfig] Warning: response rate out of range (" << responseRate << "), resetting to default 8.0 [Hz]." << std::endl;
            responseRate = 8.0;
        }

        /**
         * @brief Maximum thrust the engine can generate.
         *
         * This parameter defines the upper bound of the thrust force that
         * the engine model is able to produce.
         *
         * @unit Newtons [N]
         *
         * The value is used by the thrust model to clamp commanded thrust levels.
         * If the configured value is invalid (≤ 0), it is automatically reset
         * to a safe default to prevent undefined simulation behaviour.
         *
         * Typical thrust ranges depending on engine type:
         *
         * Main landing engines:
         * - Small lunar lander main engine: ~4,000 – 10,000 N
         * - Medium descent engine: ~10,000 – 45,000 N
         * - Example: Apollo LM descent engine ≈ 45,000 N (throttleable)
         *
         * Attitude control thrusters (RCS):
         * - Small control jets: ~10 – 50 N
         * - Medium RCS thrusters: ~50 – 500 N
         * - Large translation thrusters: ~500 – 1000 N
         *
         * Example configuration:
         * @code
         * EngineConfig mainEngine;
         * mainEngine.maxThrust = 7000.0; // main descent engine
         *
         * EngineConfig rcsThruster;
         * rcsThruster.maxThrust = 50.0;  // attitude control thruster
         * @endcode
         *
         * Safety check:
         * If the configured value is less than or equal to zero,
         * the engine thrust will be reset to a default value of 100 N.
         */
        if (maxThrust <= 0)
        {
            std::cerr << "[EngineConfig] Warning: Maximum thrust is zero or less (" << maxThrust << "), resetting to default 100 [N]." << std::endl;
            maxThrust = 100.0;
        }

        /**
         * @param Thrust direction of engine
         */
        if (direction.norm() == 0)
        {
            std::cerr << "[EngineConfig] Warning: Thrust direction cannot be zero! Resetting to (0,0,1)";
            direction = {0.0, 0.0, 1.0};
        }
        else
        {
            direction = direction.normalized();
        }

        return {Isp, timeConstant, responseRate, maxThrust, direction};
    }
};

#endif // ENGINECONFIG_H
