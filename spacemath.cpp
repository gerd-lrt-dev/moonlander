#include "spacemath.h"

double spacemath::kineticEnergy(double mass, double velocity)
{
    return 0.5 * mass * velocity * velocity;
}

double spacemath::accelerationBasedOnThrust(double thrust, double mass)
{
    return thrust / mass;
}

double spacemath::accelerationComplex(double currentThrust, double totalMass, double gravityConstant)
{
    return currentThrust / totalMass - gravityConstant;
}

double spacemath::calcMassFlowBasedOnThrust(double currenThrust, double Isp, double earthGravity)
{
    return currenThrust / (Isp * earthGravity); 
}