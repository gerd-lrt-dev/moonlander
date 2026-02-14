#include "Thrust.h"

// ---Private-------------------------------------

double Thrust::calcFuelReduction(double fuelMass, double massFlowFuel, double dt)
{
    fuelMass -= massFlowFuel * dt;

    return fuelMass;
}

void Thrust::calculateThrust(double dt)
{
    if(engineConfig.timeConstant != 0)
    {
        thrustState.current += (1 - exp(-dt / engineConfig.timeConstant)) * (thrustState.target - thrustState.current);
    }
    else
    {
        throw std::runtime_error("time constant tau is zero!");
    }
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

void Thrust::updateThrust(double dt)
{
    // TODO: 
    if (fuelstate.massCurrent > 0.0 && thrustState.target != 0)
    {
        // Initiate vars
        double newFuelMass(0.0), massFlow(0.0);

        // Calculate & Update thrust
        calculateThrust(dt);

        // Calculate massFlow for fuel consumption calculation
        massFlow = math.calcMassFlowBasedOnThrust(thrustState.current, engineConfig.Isp, envConfig.earthGravity);
        
        // Withdraw massflow to liveconsumption to display the current fuel consumption
        fuelstate.consumptionRate = massFlow;

        // Calculate fuel mass based on fuel consumption
        fuelstate.massCurrent = calcFuelReduction(fuelstate.massCurrent, massFlow, dt);
    }
    else
    {
        thrustState.current = 0.0;
    }
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

double Thrust::getCurrentFuelMass() const
{
    return fuelstate.massCurrent;
}

Vector3 Thrust::getDirectionOfThrust() const
{
    return engineConfig.direction;
}

