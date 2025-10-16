#include "physics.h"
#include <cmath>

// constructor - destructor ---------------------------------------
physics::physics(/* args */)
{
}

physics::~physics()
{
}

// private ---------------------------------------------------------
double physics::calcHeight(double t, double v0, double h0, double thrustSpacecraft) const
{
    return h0 + v0 * t + 0.5 * (thrustSpacecraft - gravityMoon) * t * t;
}

double physics::calcVel(double t, double v0, double thrust) const
{
    return v0 + (thrust - gravityMoon) * t;
}

// public  ---------------------------------------------------------
double physics::getHeight(double t, double v, double h, double thrustSpacecraft) const
{
    return calcHeight(t, v, h, thrustSpacecraft);
}

double physics::getVel(double t, double v, double thrust) const 
{
    return calcVel(t, v, thrust);
}