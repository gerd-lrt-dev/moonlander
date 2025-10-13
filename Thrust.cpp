#include "Thrust.h"



Thrust::~Thrust()
{

}

void Thrust::setTarget(double t)
{
    target = t;
}

void Thrust::update(double dt)
{
    current += (target - current) * rate * dt;
}

double Thrust::getTargetThrust() const
{
    return target;
}

double Thrust::getCurrentThrust() const
{
    return current;
}
