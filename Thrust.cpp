#include "Thrust.h"
#include "spacemath.h"
#include "environmentConfig.h"

// ---Private-------------------------------------

const double Thrust::calcFuelReduction(double fuelMass, double massFlowFuel, double dt)
{
    fuelMass -= massFlowFuel * dt;
    return fuelMass;
}

void Thrust::calcFuelConsumption()
{
    liveConsumption = fuelMass0 - fuelMass1;
}

// ---Public--------------------------------------
Thrust::~Thrust()
{

}

void Thrust::setTarget(double t)
{
    targetThrust = t;
}

void Thrust::updateSimpleThrust(double dt)
{
    currentThrust += (targetThrust - currentThrust) * rate * dt;
}

double Thrust::updateThrust(double dt, double fuelMass)
{
    // Transfer old fuel mass for calculating live consumption
    fuelMass0 = fuelMass;

    // Initate vars
    double newFuelMass(0.0), massFlow(0.0);

    // Calculate thrust
    currentThrust += (dt / timeConstant) * (targetThrust - currentThrust);

    // Calculate massFlow for fuel consumption calculation
    massFlow = math.calcMassFlowBasedOnThrust(currentThrust, Isp, envConfig.earthGravity);

    // Calculate fuel mass based on fuel consumption
    newFuelMass = calcFuelReduction(fuelMass, massFlow, dt);
    fuelMass1   = newFuelMass; 

    return newFuelMass;
}

double Thrust::getTargetThrust() const
{
    return targetThrust;
}

double Thrust::getCurrentThrust() const
{
    return currentThrust;
}

double Thrust::getFuelConsumption() const
{
    return liveConsumption;
}

