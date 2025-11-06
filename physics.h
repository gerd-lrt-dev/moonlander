#ifndef PHYSICS_H
#define PHYSICS_H

#include "environmentConfig.h"
#include "vector3.h"

/**
 * @class physics
 * @brief Handles the physics calculations for a lunar lander.
 *
 * This class provides methods to calculate the lander's velocity and height
 * over time, considering lunar gravity. It is stateless except for constant
 * parameters like lunar gravity.
 */
class physics
{
private:
    // Constants & parameters
    EnvironmentConfig configData;

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
    Vector3 updatePos(Vector3 vel,  Vector3 pos, Vector3 accelerationSpacecraft, double dt, double totalMassSpacecraft) const;

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
    Vector3 updateVel(Vector3 vel, Vector3 pos, Vector3 accelerationSpacecraft, double dt, double totalMassSpacecraft) const;
};

#endif
