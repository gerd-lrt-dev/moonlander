#include "physics.h"
#include <cmath>
#include <iostream>

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
    return h0 + v0 * dt + 0.5 * (thrustSpacecraft - gravityMoon) * dt * dt;
}

double physics::calcVel(double dt, double v0, double thrust) const
{
    return v0 + (thrust - gravityMoon) * dt;
}

// public  ---------------------------------------------------------
double physics::getHeight(double dt, double v, double h, double thrustSpacecraft) const
{
    /*
    std::cout << "dt: " << dt << std::endl;
    std::cout << "v: " << v << std::endl;
    std::cout << "h: " << h << std::endl;
    std::cout << "thrustSpacecraft: " << thrustSpacecraft << std::endl;
    */
    return calcHeight(dt, v, h, thrustSpacecraft);
    
}

double physics::getVel(double dt, double v, double thrust) const 
{
    /*
    std::cout << "physics[getVel]" << std::endl;
    std::cout << "dt: " << dt << std::endl;
    std::cout << "v: " << v << std::endl;
    std::cout << "thrust: " << thrust << std::endl;
    */
    return calcVel(dt, v, thrust);
}