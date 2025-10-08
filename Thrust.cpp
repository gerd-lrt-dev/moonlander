#include "Thrust.h"



Thrust::~Thrust()
{

}

double Thrust::setTarget(double t)
{
    target = t;
}

double Thrust::update(double dt)
{
    current += (target - current) * rate * dt;
}

double Thrust::getCurrentThrust() const
{
    return current;
}
