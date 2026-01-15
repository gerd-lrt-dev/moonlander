#ifndef SPACECRAFT_H
#define SPACECRAFT_H

#include "Thrust.h"
#include "vector3.h"
#include "environmentConfig.h"

/**
 * @struct customSpacecraft
 * @brief Defines all physical and configuration parameters of a spacecraft.
 *
 * All parameters are expressed in the spacecraft's body reference frame (B-frame),
 * unless explicitly stated otherwise. The struct contains mass properties,
 * propulsion characteristics, attitude-related inertial properties, and 
 * frame-initialization parameters.
 */
struct customSpacecraft
{
    // -------------------------------------------------------------------------
    // Mass Properties
    // -------------------------------------------------------------------------

    double emptyMass;              
    ///< [kg] Dry mass of the spacecraft (structure + engines, no fuel).

    double fuelM;          
    ///< [kg] Initial fuel mass at simulation start.

    double maxFuelM;       
    ///< [kg] Maximum fuel tank capacity (optional; used for validation or HUD).

    // -------------------------------------------------------------------------
    // Propulsion System
    // -------------------------------------------------------------------------

    double maxT;           
    ///< [N] Maximum thrust produced by the main engine at full throttle.

    double Isp;            
    ///< [s] Specific impulse of the main engine (constant exhaust efficiency).

    double timeConstant;   
    ///< [s] Engine throttle response time (first-order system time constant).

    double responseRate;
    ///< [s] Maximum rate of thrust change.

    Vector3 B_mainThrustDirection;
    ///< [unit] Normalized thrust direction vector in body frame (B-frame).
    ///< Convention: +Z axis typically points "down" along the engine nozzle.

    Vector3 B_mainThrustPosition;
    ///< [m] Position of the main engine's thrust application point in B-frame.
    ///< This produces torque if offset from the spacecraft's center of mass.


    // -------------------------------------------------------------------------
    // Attitude Dynamics (Rigid Body Inertia)
    // -------------------------------------------------------------------------

    double Ixx;            
    ///< [kg·m²] Moment of inertia about the spacecraft’s body X-axis.

    double Iyy;            
    ///< [kg·m²] Moment of inertia about the body Y-axis.

    double Izz;            
    ///< [kg·m²] Moment of inertia about the body Z-axis.
    ///< Assumed diagonal inertia tensor (symmetric spacecraft).


    // -------------------------------------------------------------------------
    // State (Body Frame Coordinates)
    // -------------------------------------------------------------------------

    Vector3 B_initialPos;  
    ///< [m] Initial spacecraft position expressed in body coordinates.
    ///< Typically initial (0, 0, 0) because physics handles world/Moon coordinates.

    Vector3 B_initialRot;  
    ///< [rad] Initial orientation (pitch, yaw, roll) in body coordinates.

    Vector3 B_initialCenterOfMass;
    ///< [m] Center of mass location in body frame at t=0.
    ///< Should be updated dynamically if fuel mass distribution changes.

    Vector3 initialVelocity;
    ///< [m/s] Velocity in three spatial directions

    // -------------------------------------------------------------------------
    // Integrity & Operational Status
    // Attributes controlling spacecraft health, integrity, and operational status.
    // -------------------------------------------------------------------------

    double       structuralIntegrity;       ///< [%] Threshold at which the spacecraft becomes non-operational
    double       safeVelocity;              ///< [m/s] Velocity in which is the spacecraft able to land safely
};

struct stateVector
{
    Vector3 I_Position = {0.0, 0.0, 0.0};
    Vector3 I_Velocity = {0.0, 0.0, 0.0};
    Vector3 IB_Orientation = {0.0, 0.0, 0.0};


};



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
     * @name Physical properties
     * Physical and mechanical properties of the spacecraft.
     * Dynamic parameters are strictly seperated from constant parameters
     * Dynamic -> private members, static -> customSpacecraft struct
     * operation vs. configuration
     * TODO: When it comes to many more vars organize them also in structs
     */
    ///@{
    customSpacecraft landerMoon;    ///< [] Parameters which defines spacecraft. This are filled by json config data.
    Thrust mainEngine;              ///< [] Dynamic state of the engine thrust.
    double totalMass;               ///< [kg] Total mass of spacecraft.
    double dt = 0;                  ///< [s] Time steps. Provided by updateTime.
    double time = 0;                ///< [s] Absolute time. Will be added by dt from udpateTime.
    Vector3 B_Pos;                  ///< [m] Current spacecraft position expressed in body coordinates.
    Vector3 B_Rot;                  ///< [rad] Current orientation (pitch, yaw, roll) in body coordinates.
    Vector3 B_CenterOfMass;         ///< [m] Center of mass location in body frame at actual time.
    Vector3 B_Vel;                  ///< [m/s] Velocity in three spatial directions.
    Vector3 B_Acc;                  ///< [m/s²] Acceleration in three spatial directions.
    double spacecraftIntegrity;     ///< [%] Current integrity of the spacecraft.
    bool spacecraftIsOperational;   ///< [true/false] Whether the spacecraft is still operational.
    EnvironmentConfig config;       ///< [-] Environment config struct with constant parameters.
    enum class SpacecraftState      ///< State of spacecraft
    {
        Operational,                ///< Fully physics
        Landed,                     ///< no translation, time & systems running
        Crashed,                    ///< no translation, but state stable
        Destroyed                   ///< everything besides time is frozen
    };

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
 * @brief Constructs a spacecraft using parameters loaded from a configuration object.
 *
 * This constructor initializes all physical, mechanical, and inertial properties of the
 * spacecraft using the values provided in a @ref customSpacecraft configuration structure.
 * All mass properties, propulsion system parameters, inertial characteristics, initial
 * state vectors, and integrity limits are copied directly into the spacecraft instance.
 *
 * The spacecraft’s main engine, thrust model, inertia model, and initial state (position,
 * rotation, velocity, and center of mass) are fully initialized based on this configuration.
 *
 * @param landerMoon 
 *     A configuration structure containing complete physical and operational parameters
 *     of the spacecraft. Fields include:
 *       - @c emptyMass                 Dry mass of the spacecraft (kg)
 *       - @c fuelM                     Initial fuel mass (kg)
 *       - @c maxFuelM                  Maximum fuel capacity (kg)
 *
 *       - @c maxT                      Maximum main engine thrust (N)
 *       - @c Isp                       Specific impulse of the main engine (s)
 *       - @c timeConstant              Engine throttle response time constant (s)
 *       - @c responseRate              Maximum engine thrust rate-of-change (1/s)
 *       - @c B_mainThrustDirection     Direction of thrust vector in body frame (unit vector)
 *       - @c B_mainThrustPosition      Thrust application point relative to body origin (m)
 *
 *       - @c Ixx                       Moment of inertia around body X-axis (kg·m²)
 *       - @c Iyy                       Moment of inertia around body Y-axis (kg·m²)
 *       - @c Izz                       Moment of inertia around body Z-axis (kg·m²)
 *
 *       - @c B_initialPos              Initial spacecraft position in body coordinates (m)
 *       - @c B_initialRot              Initial spacecraft rotation in body coordinates (rad)
 *       - @c B_initialCenterOfMass     Initial center of mass location (m)
 *       - @c initialVelocity           Initial velocity vector in body frame (m/s)
 *
 *       - @c structuralIntegrity       Percentage threshold below which spacecraft fails (%)
 *       - @c safeVelocity              Maximum safe landing velocity (m/s)
 *
 * The constructor assumes that the provided configuration structure has been validated
 * prior to construction (e.g., via JSON loader or configuration class).
 */
spacecraft(customSpacecraft lMoon);



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
     * @brief Request direction of thrust
     * @return ///< [-] Vector with direction of thrust
     */
    Vector3 requestThrustDirection() const;

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
     * @brief Sets acceleration of spacecraft
     * @param 3D Vector with acceleration in three dimensions [m/s²]
     */
    void setAcc(Vector3 vel);

    /**
     * @brief getter for Integrity
     * @return [%] integrity
     */
    double getIntegrity();

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

    /**
     * @brief Return current Acceleration of spacecraft
    //  * @return Current Acceleration [m/s²]
     */
    Vector3 getAcc();

    /**
     * @brief Return current total mass
     * @return total mass
     * 
     * Note that total mass is constantly changing due to fuelmass 
     * which decrease in case of providing thrust
     */
    double getTotalMass();

    /**
     * @brief Return current fuel mass
     * @return fuel mass
     * 
     * Fuel mass is updated by update thrust
     */
    double getfuelMass() const;


};

#endif
