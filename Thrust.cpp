#include "Thrust.h"
#include <iostream>

// ---Private-------------------------------------

double Thrust::calcFuelReduction(double fuelMass, double massFlowFuel, double dt)
{
    fuelMass -= massFlowFuel * dt;

    return fuelMass;
}

void Thrust::setThrustDirection()
{
    // TODO: On construction...
}

void Thrust::setDefaultValues()
{
    // Initialize target & current thrust with 0.0 as start condition
    thrustState.current = 0.0;
    thrustState.target = 0.0;
}

// ---Public--------------------------------------
Thrust::Thrust(const EngineConfig& eConfig, FuelState fState) 
    : engineConfig(EngineConfig::Create(
        eConfig.Isp,
        eConfig.timeConstant,
        eConfig.responseRate,
        eConfig.direction)
    ),
     fuelstate(fState)
{
    setDefaultValues();
}

Thrust::~Thrust()
{

}

void Thrust::setTarget(double tThrust)
{
    thrustState.target = tThrust;
}

void Thrust::updateSimpleThrust(double dt)
{
    thrustState.current += (thrustState.target - thrustState.current) * engineConfig.responseRate * dt;
}

double Thrust::updateThrust(double dt, double fuelMass)
{
    if (fuelMass > 0.0 && thrustState.target != 0)
    {
        // Initiate vars
        double newFuelMass(0.0), massFlow(0.0);

        // Calculate thrust TODO: Auslagern und Fehler durch 0 teilen abfangen!
        thrustState.current += (dt / thrustState.target) * (thrustState.target - thrustState.current);

        // Calculate massFlow for fuel consumption calculation
        massFlow = math.calcMassFlowBasedOnThrust(thrustState.current, engineConfig.Isp, envConfig.earthGravity);
        
        // Withdraw massflow to liveconsumption to display the current fuel consumption
        fuelstate.consumptionRate = massFlow;

        // Calculate fuel mass based on fuel consumption
        newFuelMass = calcFuelReduction(fuelMass, massFlow, dt);

        return newFuelMass;
    }
    else
    {
        thrustState.current = 0.0;
    }
    
    return fuelMass;
}

// --- Getter functions ---------------------------------------------

double Thrust::getTargetThrust() const
{
    return thrustState.target;
}

double Thrust::getCurrentThrust() const
{
    return thrustState.current;
}

double Thrust::getFuelConsumption() const
{
    return fuelstate.consumptionRate;
}

Vector3 Thrust::getDirectionOfThrust() const
{
    return engineConfig.direction;
}

