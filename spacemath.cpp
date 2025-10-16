#include "spacemath.h"

double spacemath::kineticEnergy(double mass, double velocity)
{
    return 0.5 * mass * velocity * velocity;
}

double spacemath::accelerationBasedOnThrust(double thrust, double mass)
{
    return thrust / mass;
}