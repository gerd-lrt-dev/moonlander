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
Vector3 physics::updatePos(Vector3 vel, Vector3 pos, Vector3 accelerationSpacecraft, double dt, double totalMassSpacecraft) const
{
    return pos + vel * dt + accelerationSpacecraft * 0.5 * dt * dt;
}

Vector3 physics::updateVel(Vector3 vel, Vector3 pos, Vector3 accelerationSpacecraft, double dt, double totalMassSpacecraft) const
{
    return vel + accelerationSpacecraft * dt;
}

Vector3 physics::updateAcc(double currentThrust, double totalMass, Vector3 directionOfThrust, const Vector3 moonGravityVec) const
{
    return spacemath::accelerationComplex(currentThrust, totalMass, directionOfThrust, moonGravityVec);
}