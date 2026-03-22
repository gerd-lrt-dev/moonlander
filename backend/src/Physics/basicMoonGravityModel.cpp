#include "Physics/basicMoonGravityModel.h"

//******************************************************
//************* PUBLIC *********************************
//******************************************************
Vector3 BasicMoonGravityModel::computeAcceleration(const Vector3& pos, const Vector3& vel, double mass, const Vector3& thrust) const
{
    Vector3 gravity = calcAccelerationAlignedToCenterOfMoon(pos);
    Vector3 thrustAcc = thrust/mass;
    return gravity + thrustAcc;
}

//******************************************************
//************* PRIVATE ********************************
//******************************************************
Vector3 BasicMoonGravityModel::calcAccelerationAlignedToCenterOfMoon(const Vector3& pos) const
{
    //Vector3 dir = pos.normalized();
    //double r = pos.norm();

    Vector3 r       = pos;
    double rNorm    = r.norm();
    double r3       = rNorm * rNorm * rNorm;

    return - r * (configData.muMoon / (r3));
}
