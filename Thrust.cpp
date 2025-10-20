#include "Thrust.h"
#include <iostream>

// ---Private-------------------------------------

double Thrust::calcFuelReduction(double fuelMass, double massFlowFuel, double dt)
{
    fuelMass -= massFlowFuel * dt;

    return fuelMass;
}

// ---Public--------------------------------------
Thrust::~Thrust()
{

}

void Thrust::setTarget(double tThrust)
{
    targetThrust = tThrust;
}

void Thrust::updateSimpleThrust(double dt)
{
    currentThrust += (targetThrust - currentThrust) * rate * dt;
}

double Thrust::updateThrust(double dt, double fuelMass)
{
    if (fuelMass > 0.0 && targetThrust != 0)
    {
        // Initate vars
        double newFuelMass(0.0), massFlow(0.0);

        // Calculate thrust TODO: Auslagern und Fehler durch 0 teilen abfangen!
        currentThrust += (dt / timeConstant) * (targetThrust - currentThrust);

        // Calculate massFlow for fuel consumption calculation
        massFlow = math.calcMassFlowBasedOnThrust(currentThrust, Isp, envConfig.earthGravity);
        
        // Withdraw massflow to liveconsumption to display the current fuel consumption
        liveConsumption = massFlow;

        // Calculate fuel mass based on fuel consumption
        newFuelMass = calcFuelReduction(fuelMass, massFlow, dt);
        fuelMass1   = newFuelMass; 

        return newFuelMass;
    }
    else
    {
        currentThrust = 0.0;
    }
    
    return fuelMass;
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

