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
double physics::calcHeight(double dt, double v0, double h0, double thrustSpacecraft) const
{
    return h0 + v0 * dt + 0.5 * (thrustSpacecraft - configData.moonGravity) * dt * dt;
}

double physics::calcVel(double dt, double v0, double thrust) const
{
    return v0 + (thrust - configData.moonGravity) * dt;
}

// public  ---------------------------------------------------------
double physics::getHeight(double dt, double v, double h, double thrustSpacecraft) const
{
    return calcHeight(dt, v, h, thrustSpacecraft);
}

double physics::getVel(double dt, double v, double thrust) const 
{
    return calcVel(dt, v, thrust);
}