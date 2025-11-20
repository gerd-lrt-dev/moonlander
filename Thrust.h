#ifndef THRUST_H
#define THRUST_H

#include "spacemath.h"
#include "environmentConfig.h"
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

    Vector3 direction;       
    /**<
     * @brief Normalized thrust direction in the spacecraft body frame.
     * @unit unit vector [-]
     * Specifies the direction in which thrust is applied.
     */

    static EngineConfig Create(double Isp,
                               double timeConstant,
                               double responseRate,
                               Vector3 direction)
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
            timeConstant = 0.5;
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

        if (direction.norm() == 0)
        {
            std::cerr << "[EngineConfig] Warning: Thrust direction cannot be zero! Resetting to (0,0,1)";
            direction = {0.0, 0.0, 1.0};
        }
        else
        { 
            direction = direction.normalized(); 
        }

        return {Isp, timeConstant, responseRate, direction};
    }
};


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

/**
 * @brief Fuel-related state variables for the engine.
 *
 * Tracks the mass of the remaining fuel and instantaneous consumption rate.
 * All values evolve over simulation time.
 */
struct FuelState
{
    double massStart;
    /**<
     * @brief Fuel mass at the previous simulation timestep.
     * @unit kilograms [kg]
     * Used to compute per-step consumption rates.
     */

    double massCurrent;
    /**<
     * @brief Current remaining fuel mass.
     * @unit kilograms [kg]
     * Updated each timestep based on thrust and Isp.
     */

    double consumptionRate;
    /**<
     * @brief Instantaneous propellant mass flow rate.
     * @unit kilograms per second [kg/s]
     * Calculated from thrust and specific impulse:
     *  \f$ \dot{m} = \frac{T}{I_{sp} g_0} \f$
     */
};

/**
 * @class Thrust
 * @brief Simulates the thrust behavior of a spacecraft engine.
 *
 * The class models the current thrust value approaching a target thrust
 * over time, considering a reaction speed (rate). This allows for a more
 * realistic simulation of engine dynamics instead of instantaneous changes.
 */
class Thrust
{
private:
    // Structs
    EngineConfig engineConfig;      ///< [-] Configuration parameters for a spacecraft engine.
    ThrustState thrustState;        ///< [-] Dynamic state of the engine thrust.
    FuelState fuelstate;            ///< [-] Fuel-related state variables for the engine.
    EnvironmentConfig envConfig;    ///< [-] Environment parameters

    // Classes
    spacemath math;                 ///< Math helper class
    
    /**
     * @brief Calculate fuel cunsomption
     * @param fuelMass      ///< [kg] Mass of fuel
     * @param massflowFuel  ///< [kg/s] Mass flow of fuel
     * @param dt            ///< [s] discrete time step and update parameter
     * 
     * Reduces fuel supply depending on fuel consumption through the thrust provision process
     */
    double calcFuelReduction(double fuelMass, double massFlowFuel, double dt);

    /**
     * @brief Updates the current thrust value using an exponential smoothing approach.
     *
     * This function adjusts the current thrust toward the target thrust based on
     * a continuous-time first-order response model. The differential equation
     *
     *     d(current) / dt = (target - current) / τ
     *
     * describes the convergence of the current thrust toward the target with
     * time constant τ. Solving this yields the discrete update rule:
     *
     *     current += (1 - exp(-dt / τ)) * (target - current)
     *
     * This provides framerate-independent, smooth convergence of the thrust,
     * where a smaller τ results in faster response.
     *
     * @param dt  Time step in seconds.
     */
    void calculateThrust(double dt);

    /**
     * @brief Sets direction of thrust
     * 
     * TODO: Build algorithm to simulate tvc deviation
     */
    void setThrustDirection();

public:
    
    /**
     * @brief Constructor
     * @param eConfig Configuration parameters for a spacecraft engine
     * @param fState Fuel-related state variables for the engine
     */
    Thrust(const EngineConfig& eConfig, FuelState fState);
    
    /**
     * @brief Destructor
     *
     * Currently trivial, no dynamic resources.
     */
    ~Thrust();

    /**
     * @brief Set a new target thrust
     * @param t ///<  [N] Target thrust
     */
    void setTarget(double tThrust);

    /**
     * @brief Set default values & start conditions
     */
    void setDefaultValues();

    /**
     * @brief Update the current thrust based on the time delta
     * @param dt    ///< [s] Time step since last update 
     * @param fuelMass
     * @return Returns fuel mass after combustion step
     * 
     * This function updates the complex thrust calculation with 
     * mass reduction and solution of the differential equation 
     * in discrete time steps.
     * 
     * Approximation of the first-order differential equation for thrust control
     * T(t + delta t) = T(t) + (delta t)/ teta * (T{target} - T(t))
     * T: thrust, t: time, delta t: discrete time step, teta: time conmstant 
     * 
     * This function also uses the transfer parameter to check whether fuel is 
     * available to provide thrust. In addition, it returns the fuel supply 
     * remaining after the combustion process in the respective time increment.
     * 
     * Steps to calculate thrust:
     * 1. updateThrust - function initializes the thrust calculation. From now on, the thrust is determined based on the target thrust. Even if thrust = 0. Later with idle mode
     */
    void updateThrust(double dt);

    /**
     * @brief Get the target thrust
     * @return ///< [N] target thrust 
     */
    double getTargetThrust() const;

    /**
     * @brief Get the current thrust
     * @return ///< [N] Current thrust 
     */
    double getCurrentThrust() const;

    /**
     * @brief Getter function for fuel live fuel consumption
     * @return ///< [kg / dt] Real-time fuel consumption
     */
    double getFuelConsumption() const;

    /**
     * @brief Getter function for current fuel mass
     * @return ///< [kg] fuel mass
     */
    double getCurrentFuelMass() const;

    /**
     * @brief Getter function for thrust direction
     * @return ///< [-] Vector with direction of thrust
     * 
     * The vector is aligned with the static coordinate system of the spacecraft
     */
    Vector3 getDirectionOfThrust() const;
};

#endif
