#include "physics.h"
#include <cmath>
#include <iostream>

#include "environmentConfig.h"

// constructor - destructor ---------------------------------------
physics::physics(/* args */)
{
}

physics::~physics()
{
}

// private ---------------------------------------------------------
Vector3 physics::calcGravityRadialToMoonCenter(Vector3 pos) const 
{
    // Direction from center Of moon (points to the outside)
    Vector3 dir = pos.normalized();

    // Distance to center of moon radius + height
    double r    = configData.radiusMoon + pos.norm(); 

    return - dir * (configData.gravitationalFactorMoon / (r * r));
}

Vector3 physics::calcAccelerationAlignedToCenterOfMoon(Vector3 accelerationSpacecraft, Vector3 gravityRadialToMoonCenter, double totalMassSpacecraft) const
{
    return (accelerationSpacecraft / totalMassSpacecraft) + gravityRadialToMoonCenter;
}

// public  ---------------------------------------------------------
Vector3 physics::computePos(Vector3 vel, Vector3 pos, Vector3 accelerationSpacecraft, double dt) const
{
    return pos + vel * dt + accelerationSpacecraft * 0.5 * dt * dt;
}

Vector3 physics::computeVel(Vector3 vel, Vector3 accelerationSpacecraft, double dt) const
{
    /*
    std::cout << "Current Velocity in z: " << vel.z << std::endl;
    std::cout << "Current acceleration in z: " << accelerationSpacecraft.z << std::endl;
    std::cout << "dt: " << dt << std::endl;
    */
    return vel + accelerationSpacecraft * dt;
}

Vector3 physics::computeAcc(double currentThrust, double totalMass, Vector3 directionOfThrust, const Vector3 moonGravityVec) const
{
    return spacemath::accelerationComplex(currentThrust, totalMass, directionOfThrust, moonGravityVec);
}

double physics::computeGLoad(const Vector3& totalAcceleration, const Vector3& gravityAcceleration)
{
    constexpr double g0 = 9.80665; // [m/s^2]

    // Berechne Proper Acceleration
    Vector3 properAcceleration = totalAcceleration - gravityAcceleration;

    double gLoad = properAcceleration.norm() / g0;

    return gLoad;
}

