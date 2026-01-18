#ifndef PHYSICS_H
#define PHYSICS_H

#include "environmentConfig.h"
#include "vector3.h"
#include "spacemath.h"

/**
 * @class physics
 * @brief Handles the physics calculations for a lunar lander.
 *
 * This class provides methods to calculate the lander's velocity and position
 * over time, considering lunar gravity. It is stateless except for constant
 * parameters like lunar gravity.
 *
 * @note All calculations are performed in a Moon-centered inertial reference
 *       frame, meaning that positions, velocities, and gravitational forces
 *       are expressed relative to the Moon's center. 
 *       This ensures a consistent and physically accurate simulation of 
 *       orbital and surface dynamics.
 *
 * @note Forces originating from the spacecraft itself, such as thrust, are
 *       defined in the spacecraft's body frame and must be transformed into
 *       the inertial frame before being combined with gravitational forces.
 */

class physics
{
private:
    // Constants & parameters
    EnvironmentConfig configData;
    spacemath math;

    /**
     * @brief Calculates three dimensional moon gravity effekt
     * @param pos Position in 3 dimensions [m]
     * @return Gravity in three dimensional vector aligned to the center of moon [m/s²]
     * 
     * Supports calculation of position & velocity
     */
    Vector3 calcGravityRadialToMoonCenter(Vector3 position) const;

    /**
     * @brief Calculates acceleration under the influence of mass and to the center of moon
     * @param accelerationSpacecraft Thrust vector provided by engines [m/s²]
     * @param totalMassSpacecraft Totalmass = emptymass + fuelmass [kg]
     * @param gravityRadialToMoonCenter Gravity in three dimensional vector aligned to the center of moon [m/s²]
     * 
     * Supports calculation of position & velocity
     */
    Vector3 calcAccelerationAlignedToCenterOfMoon(Vector3 accelerationSpacecraft, Vector3 gravityRadialToMoonCenter, double totalMassSpacecraft) const;

public:
    /**
     * @brief Constructor
     */
    physics();

    /**
     * @brief Destructor
     */
    ~physics();

    /**
     * @brief Calculates height based on time, initial velocity, and initial height
     * @param vel Velocity in 3 dimensions [m/s]
     * @param pos Position in 3 dimensions [m]
     * @param accelerationSpacecraft Thrust vector provided by engines [m/s²]
     * @param dt Time elapsed [s]
     * @param totalMassSpacecraft Totalmass = emptymass + fuelmass [kg]
     * @return Calculated height [m]
     * 
     * The resulting height is calculated based on the equation of motion.
     * h(t) = h0 ​+ v0 * ​t + 1/2 ​(accelerationSpacecraft−g) * t²
     */
    Vector3 computePos(Vector3 vel,  Vector3 pos, Vector3 accelerationSpacecraft, double dt) const;

    /**
     * @brief Calculates velocity based on time and initial velocity
     * @param vel Velocity in 3 dimensions [m/s]
     * @param pos Position in 3 dimensions [m]
     * @param accelerationSpacecraft Thrust vector provided by engines [m/s²]
     * @param dt Time elapsed [s]
     * @param totalMassSpacecraft [kg]
     * @return Calculated velocity [m/s]
     * 
     * v(t) = v0 + a{resutl} * t
     * a{result} = a{accelerationSpacecraft} - g{moon}
     */
    Vector3 computeVel(Vector3 vel, Vector3 accelerationSpacecraft, double dt) const;

    /**
     * @brief Request current acceleration based on thrust of spacecraft
     * @param currenThrust      ///< [N] current Thrust of Spacecraft provided by thrust class
     * @param totalMass         ///< [kg] Total mass of spacecraft taking fuel consumption into account
     * @param directionOfThrust ///< [-] Vector with direction of thrust
     * @param moonGravityVec    ///< [m/s²] Acceleration of moon ~1,64
     * @return  ///< [m/s²] Vector acceleration of spacecraft
    *
    * The total acceleration is defined as the sum of thrust acceleration and
    * gravitational acceleration:
    *
    *      a_total = a_thrust + a_gravity
    *
    * where
    *
    *      a_thrust  = F_thrust / m_total
    *      a_gravity = - (μ / r²) * r̂
    *
    * with:
    *      F_thrust  : Thrust vector produced by the main engine [N]
    *      m_total   : Current total spacecraft mass (dry mass + fuel) [kg]
    *      μ         : Gravitational parameter of the Moon (μ = g_surface * r_moon²) [m³/s²]
    *      r         : Distance from the lunar center to the spacecraft [m]
    *      r̂         : Normalized vector from the lunar center to the spacecraft
    *
    * The negative sign ensures that the gravitational acceleration vector
    * always points toward the Moon's center.
    * The calculation is made by the helper class spacemath
     */
    Vector3 computeAcc(double currentThrust, double totalMass, Vector3 directionOfThrust, const Vector3 moonGravityVec) const;

    /**
     * @brief Computes the proper G-load experienced by the spacecraft.
     *
     * The G-load represents the acceleration felt by the lander, excluding
     * the effect of gravity. It is expressed in units of Earth's standard
     * gravity (g0 = 9.80665 m/s²). This function is suitable for both 1D
     * and 3D acceleration vectors.
     *
     * @param totalAcceleration The total acceleration acting on the spacecraft [m/s²].
     *                          Typically includes thrust and gravity.
     * @param gravityAcceleration The acceleration due to gravity [m/s²] at the
     *                            spacecraft's current location (e.g., Moon gravity vector).
     *
     * @return Proper G-load experienced by the spacecraft in units of g0.
     *
     * @note This function calculates proper acceleration by subtracting the
     *       gravitational component from the total acceleration. Only the
     *       remaining acceleration contributes to the G-load.
     * @note The function uses the Euclidean norm for vector inputs:
     *       G = ||totalAcceleration - gravityAcceleration|| / g0
     * @see Vector3
     */
    double computeGLoad(const Vector3& totalAcceleration, const Vector3& gravityAcceleration);

};

#endif
