#include "Physics/basicMoonGravityModel.h"
#include <iostream>
//******************************************************
//************* PUBLIC *********************************
//******************************************************
Vector3 BasicMoonGravityModel::computeAcceleration(const Vector3& pos, const Vector3& vel, double mass, double thrust, const Vector3& thrustDir) const
{
    Vector3 gravity = calcAccelerationAlignedToCenterOfMoon(pos);
    Vector3 thrustAcc = thrustDir *(thrust/mass);
    return gravity + thrustAcc;
}

//******************************************************
//************* PRIVATE ********************************
//******************************************************
Vector3 BasicMoonGravityModel::calcAccelerationAlignedToCenterOfMoon(const Vector3& pos) const
{
    Vector3 dir = pos.normalized();
    double r = pos.norm() + configData.radiusMoon;

    std::cout << "position.z: " << r << std::endl;

    return -dir * (configData.gravitationalFactorMoon / (r*r));
}
