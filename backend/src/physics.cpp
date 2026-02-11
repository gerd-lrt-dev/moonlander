#include "physics.h"
#include <cmath>

// constructor - destructor ---------------------------------------

physics::~physics()
{
}

// public  ---------------------------------------------------------
Vector3 physics::computeAcc(const Vector3& pos, const Vector3& vel, double mass, double thrust, const Vector3& thrustDir) const
{
    return model_->computeAcceleration(pos, vel, mass, thrust, thrustDir);
}

Vector3 physics::computeVel(const Vector3& vel, const Vector3& acc, double dt) const
{
    return integrator_->integrateVel(vel, acc, dt);
}

Vector3 physics::computePos(const Vector3& pos, const Vector3& vel, const Vector3& acc, double dt) const
{
    return integrator_->integratePos(pos, vel, acc, dt);
}

double physics::computeGLoad(const Vector3& totalAcceleration, const Vector3& gravityAcceleration)
{
    return sensor_->computeGLoad(totalAcceleration, gravityAcceleration);
}

