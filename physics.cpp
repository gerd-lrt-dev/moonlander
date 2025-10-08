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
double physics::calcHeight(double t, double v0, double h0) const
{
    return -0.5 * gravityMoon * t * t + v0 * t + h0;
}

double physics::calcVel(double t, double v0) const
{
    return - gravityMoon * t + v0;
}

// public  ---------------------------------------------------------
double physics::getHeight(double t, double v, double h) const
{
    return calcHeight(t, v, h);
}

double physics::getVel(double t, double v) const 
{
    return calcVel(t, v);
}