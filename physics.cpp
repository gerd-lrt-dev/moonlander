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
    return -0.5 * gravityMoon * t * t + v0 * t + h0 + thrustSpacecraft * t * t;
}

double physics::calcVel(double t, double v0, double thrust) const
{
    return - gravityMoon * t + v0 + thrust * t;
}

// public  ---------------------------------------------------------
double physics::getHeight(double t, double v, double h, double thrustSpacecraft) const
{
    return calcHeight(t, v, h, thrustSpacecraft);
}

double physics::getVel(double t, double v) const 
{
    return calcVel(t, v);
}