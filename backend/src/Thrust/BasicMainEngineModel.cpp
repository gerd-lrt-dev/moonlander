#include "Thrust/BasicMainEngineModel.h"

// -------------------------------------------------------------------------
// Public class methods
// -------------------------------------------------------------------------
basicMainEngineModel::basicMainEngineModel(const EngineConfig& eConfig, FuelState fState)
    : engineConfig_(EngineConfig::Create(eConfig.Isp,
                                         eConfig.timeConstant,
                                         eConfig.responseRate,
                                         eConfig.direction)
                    ),
    fuelstate_(fState)
{

}

void basicMainEngineModel::updateThrust(const double &dt)
{
    if(engineConfig_.timeConstant != 0)
    {
        thrustState_.current += (1 - exp(-dt / engineConfig_.timeConstant)) * (thrustState_.target - thrustState_.current);

        fuelstate_.consumptionRate = calcMassFlow(thrustState_.current, engineConfig_.Isp, 9.81);

        // Calculate fuel mass based on fuel consumption
        fuelstate_.massCurrent = calcFuelReduction(fuelstate_.massCurrent, fuelstate_.consumptionRate, dt);
    }
    else
    {
        throw std::runtime_error("time constant tau is zero!");
    }
}

// -------------------------------------------------------------------------
// Public setter override functions
// -------------------------------------------------------------------------
void basicMainEngineModel::setTarget(const double &tThrust)
{
    thrustState_.target = tThrust;
}

// -------------------------------------------------------------------------
// Public getter override functions
// -------------------------------------------------------------------------

double basicMainEngineModel::getTargetThrust() const
{
    return thrustState_.target;
}

double basicMainEngineModel::getCurrentThrust() const
{
    return thrustState_.current;
}

double basicMainEngineModel::getFuelConsumption() const
{
    return fuelstate_.consumptionRate;
}

double basicMainEngineModel::getCurrentFuelMass() const
{
    return fuelstate_.massCurrent;
}

Vector3 basicMainEngineModel::getDirectionOfThrust() const
{
    return engineConfig_.direction;
}

// -------------------------------------------------------------------------
// Private setter functions
// -------------------------------------------------------------------------
void basicMainEngineModel::setDefaultValues()
{
    thrustState_.current = 0.0;
    thrustState_.target  = 0.0;
}

// -------------------------------------------------------------------------
// Private calculation methods
// -------------------------------------------------------------------------
double basicMainEngineModel::calcFuelReduction(const double &fuelMass, const double &massFlowFuel, const double &dt)
{
    double newFuelMass = fuelMass - (massFlowFuel * dt);

    return newFuelMass;
}

double basicMainEngineModel::calcMassFlow(const double &currenThrust, const double &Isp, const double &earthGravity)
{
    return currenThrust / (Isp * earthGravity);
}
