#ifndef SPACECRAFT_H
#define SPACECRAFT_H

#include "Thrust.h"
#include "vector3.h"
#include "environmentConfig.h"

/**
 * @class spacecraft
 * @brief Represents a spacecraft with a main engine and fuel.
 *
 * The class encapsulates the physical properties of a spacecraft,
 * such as mass (empty and fuel), maximum thrust, and its main engine.
 * This allows for physics-based simulations including thrust updates
 * and mass-dependent calculations.
 * This class calculates the damage a spacecraft takes by specific manouvers.
 * 
 * TODO: Spacecraft should get an point for center of gravity, which moves, when
 * fuel is gettin lower
 */
class spacecraft
{
private:
    /**
     * @name Position, rotation and velocity data of spacecraft
     * An initial assignment of position, rotation and rotation
     * is performeed via the constructor
     */
    ///@{
    Vector3 position; ///< [m] Position in cartesian coordinates 
    Vector3 rotation; ///< [rad] Rotation in euler angles
    Vector3 velocity; ///< [m/s] Velocity in three spatial directions
    ///@}


    /**
     * @name Simulation start conditions 
     * physical conditions passed via constructor
     * 
     */
    ///@{
    const double emptyMass;   ///< [kg] Mass of the spacecraft without fuel
    const double maxThrust;   ///< [N] Maximum thrust the main engine can produce
    Thrust       mainEngine;  ///< Main engine responsible for generating thrust
    double       fuelMass;    ///< [kg] Mass of the fuel currently onboard
    ///@}
    
    /**
     * @name Physical properties
     * Physical and mechanical properties of the spacecraft.
     * TODO: When it comes to many more vars organize them in structs
     */
    ///@{
    double       dt = 0;      ///< [s] Time steps. Provided by updateTime 
    double       time = 0;    ///< [s] Absolute time. Will be added by dt from udpateTime
    double       totalMass;   ///< [kg] Total mass of spacecraft - is composed of empty mass and fuel mass in setDefaultValues
    EnvironmentConfig config;
    ///@}

    /**
     * @name Integrity & Operational Status
     * Attributes controlling spacecraft health, integrity, and operational status.
     */
    ///@{
    double       spacecraftIntegrity;       ///< [%] Current integrity of the spacecraft
    double       structuralIntegrity;       ///< [%] Threshold at which the spacecraft becomes non-operational
    bool         spacecraftIsOperational;   ///< [true/false] Whether the spacecraft is still operational
    double       safeVelocity;              ///< [m/s] Velocity in which is the spacecraft able to land safely
    ///@}

    /**
     * @brief Sets default values required to run the simulation
     * 
     * This function initializes all spacecraft parameters that are not explicitly
     * set via the constructor. It ensures that the spacecraft has a valid initial
     * state for simulation.
     *
     * Specifically, it sets default values for:
     * - @ref spacecraftIntegrity
     * - @ref structuralIntegrity
     * - @ref spacecraftIsOperational
     * - @ref totalMass
     * - @ref safeVelocity
     *
     * Called automatically in the constructor.
     */
    void setDefaultValues();

    /**
     * @brief Updates total mass
     * Updates the total mass of the spacecraft in connection with fuel consumption due to the combustion process.
     * Updates the total mass of the class directly without return.
     */
    void updateTotalMassOnFuelReduction(double emptyMass, double fuelMass);
    
    
public:
    /**
     * @brief Constructor
     * @param m Empty mass of the spacecraft [kg]
     * @param maxT Maximum thrust of the main engine [N]
     * @param rate Reaction speed / responsiveness of the engine [Hz]
     * @param fuelM Initial fuel mass [kg]
     *
     * Initializes the spacecraft with its physical parameters and engine.
     */
    spacecraft(double m, double maxT, double rate, double fuelM, double timeConstant, Vector3 initialPos, Vector3 initialRot);

    /**
     * @brief Destructor
     *
     * Cleans up resources if needed. Currently trivial.
     */
    ~spacecraft();

    /**
     * @brief Checks whether the spacecraft is still operational.
     * @return Returns true if the spacecraft is operational (airworthy), false otherwise.
     *
     * This function evaluates the spacecraft's integrity (0–100%) and updates the
     * operational status. If the integrity falls below 50%, the spacecraft is
     * considered no longer operational.
     *
     * Example usage:
     * @code
     * if(spacecraft.isIntact()) {
     *     // proceed with simulation
     * } else {
     *     // handle failure / crash
     * }
     * @endcode
     */
    bool isIntact();

    /**
     * @brief Updates spacecraft integrity in case of damage
     * 
     * If the spacecraft sustains damage, this function adds the damage to 
     * the spacecraft's integrity. Funktion updates spacecraftIntegrity automatically
     * 
     * @note Damage (delta) has to be a negative value! 
     * 
     * 
     * Damage will deduct the integrity of spacecraft
     * - @ref spacecraftIntegrity
     */
    void updateSpacecraftIntegrity(double delta);

    /**
     * @brief Updates time
     * @param dt timestep [s]
     * 
     * Single source of thruth for time is simcontrol!
     */
    void updateTime(double dt);

    /**
     * @brief Applies landing damage based on impact velocity and spacecraft mass.
     * 
     * This function calculates the percentage damage the spacecraft receives
     * during landing. The damage is proportional to the kinetic energy (KE) at impact
     * relative to a reference safe landing energy.
     * 
     * The calculation steps are as follows:
     * 1. Compute the total kinetic energy of the spacecraft:
     *      KE = 0.5 * total_mass * v^2
     *      where:
     *          - total_mass = emptyMass + fuelMass
     *          - v = vertical velocity at impact [m/s]
     *
     * 2. Define a reference kinetic energy (KE_ref) corresponding to a safe landing:
     *      KE_ref = 0.5 * total_mass * v_safe^2
     *      where:
     *          - v_safe = maximum safe landing velocity
     *
     * 3. Compute proportional damage:
     *      damage_percent = KE / KE_ref
     *
     * 4. Update spacecraft integrity:
     *      new_integrity = old_integrity - damage_percent
     *      Integrity is clamped to [0, 1].
     *
     * 5. Update operational status:
     *      spacecraftIsOperational = (new_integrity >= structuralIntegrity)
     *
     * @param impactVelocity Vertical velocity at landing [m/s]
     * @param safeVelocity Maximum safe landing velocity [m/s] (default is 2 m/s)
     */
    void applyLandingDamage(double impactVelocity);

    /**
     * @brief Set thrust up to specific level
     * @param targetThrust Thrust clamped to [0, 1] where 1 is 100% of max thrust
     * 
     * This function initiates the main engine and provide thrust for spacecraft. It 
     * provides thrust until the setted target thrust is reached.
     */
    void setThrust(double targetThrustInPercentage);

    /**
     * @brief Request target Thrust of Aircraft
     * @return target thrust
     * 
     * Function queries the thrust class for thrust what the user has setted. 
     */
    double requestTargetThrust() const;

    /**
     * @brief Request current thrust of Spacecraft
     * @return current thrust
     * 
     * Function queries the thrust class for the thrust of the spacecraft. 
     * Not to be confused with the target thrust!
     */
    double requestThrust() const;

    /**
     * @brief Request current acceleration based on thrust of spacecraft
     * @return ///< [m/s²] Vector acceleration of spacecraft
     */
    Vector3 requestAcceleration() const;

    /**
     * @brief Request current fuel level
     * @return Fuel level in percent [%]
     */

     /**
      * @brief Request live fuel consumption from thrust class
      * @return fuel consumption
      */
     double requestLiveFuelConsumption() const;

    /**
     * @brief Update position to new values
     * @param 3D Vector with cartesian-coordinates [m]
     */
    void setPos(Vector3 pos);

    /**
     * @brief Sets rotation of spacecraft
     * @param 3D Vector with euler rotation \varphi ,\theta ,\psi [rad]
     */
    void setRot(Vector3 rot);

    /**
     * @brief Sets velocity of spacecraft
     * @param 3D Vector with velocities in three dimensions [m/s]
     */
    void setVel(Vector3 vel);

    /**
     * @brief Return current position of spacecraft
     * @return Current position in cartesian-coordinates [m]
     */
    Vector3 getPos();

    /**
     * @brief Return current rotation of spacecraft
     * @return Current rotation in euler angles [rad]
     */
    Vector3 getRot();

    /**
     * @brief Return current velocity of spacecraft
    //  * @return Current velocity [m/s]
     */
    Vector3 getVel();
};

#endif
