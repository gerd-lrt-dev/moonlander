#include "Thrust.h"
#include <iostream>

// ---Private-------------------------------------

double Thrust::calcFuelReduction(double fuelMass, double massFlowFuel, double dt)
{
    std::cout << "fuelMass" << fuelMass << std::endl;
    std::cout << "massFlowFuel" << massFlowFuel << std::endl;
    std::cout << "dt" << dt << std::endl;

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
    /*
    std::cout << Thrust[updateThrust] << std::endl;
    std::cout << "dt: " << dt << std::endl;
    std::cout << "FuelMass: " << fuelMass << std::endl;
    */
    if (fuelMass > 0.0 && targetThrust != 0)
    {
        // Transfer old fuel mass for calculating live consumption
        fuelMass0 = fuelMass;

        // Initate vars
        double newFuelMass(0.0), massFlow(0.0);

        std::cout << "Thrust[updateThrust]" << std::endl;
        std::cout << "currentThrust: " << currentThrust << std::endl;
        std::cout << "dt: " << dt << std::endl;
        std::cout << "timeConstant: " << timeConstant << std::endl;
        std::cout << "targetThrust: " << targetThrust << std::endl;
    
        // Calculate thrust
        currentThrust += (dt / timeConstant) * (targetThrust - currentThrust);

        // Calculate massFlow for fuel consumption calculation
        massFlow = math.calcMassFlowBasedOnThrust(currentThrust, Isp, envConfig.earthGravity);

        // Calculate fuel mass based on fuel consumption
        newFuelMass = calcFuelReduction(fuelMass, massFlow, dt);
        fuelMass1   = newFuelMass; 

        return newFuelMass;
    }
    else
    {
        currentThrust = 0.0;
        return 0;
    }
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

