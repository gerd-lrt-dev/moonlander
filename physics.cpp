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
    return - pos * (configData.moonGravity / pow(pos.norm(), 3));
}

Vector3 physics::calcAccelerationAlignedToCenterOfMoon(Vector3 thrust, Vector3 gravityRadialToMoonCenter, double totalMassSpacecraft) const
{
    return (thrust / totalMassSpacecraft) - gravityRadialToMoonCenter;
}

// public  ---------------------------------------------------------
Vector3 physics::updatePos(Vector3 vel, Vector3 pos, Vector3 thrust, double dt, double totalMassSpacecraft) const
{
    Vector3 gravityRadialToMoonCenter = calcGravityRadialToMoonCenter(pos);
    Vector3 acceleration = calcAccelerationAlignedToCenterOfMoon(thrust, gravityRadialToMoonCenter, totalMassSpacecraft);
    
    return pos + vel * dt + acceleration * 0.5 * dt * dt;
}

Vector3 physics::updateVel(Vector3 vel, Vector3 pos, Vector3 thrust, double dt, double totalMassSpacecraft) const
{
    Vector3 gravityRadialToMoonCenter = calcGravityRadialToMoonCenter(pos);
    Vector3 acceleration = calcAccelerationAlignedToCenterOfMoon(thrust, gravityRadialToMoonCenter, totalMassSpacecraft);
    
    return vel + acceleration * dt;
}