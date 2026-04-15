#include "Thrust/BasicMainEngineModel.h"

// -------------------------------------------------------------------------
// Public class methods
// -------------------------------------------------------------------------
basicMainEngineModel::basicMainEngineModel(const EngineConfig& eConfig, FuelState fState)
    : engineConfig_(EngineConfig::Create(eConfig.engineActivated,
                                         eConfig.id,
                                         eConfig.name,
                                         eConfig.type,
                                         eConfig.tankID,
                                         eConfig.Isp,
                                         eConfig.timeConstant,
                                         eConfig.responseRate,
                                         eConfig.maxThrust,
                                         eConfig.direction,
                                         eConfig.position)
                    ),
    fuelstate_(fState)
{
    engineConfig_.engineActivated = true;
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
void basicMainEngineModel::setEnginePowerSwitch(bool activateEngine)
{
    engineConfig_.engineActivated = activateEngine;
}

void basicMainEngineModel::setTarget(const double &tThrust)
{
    engineConfig_.engineActivated ? thrustState_.target = tThrust : thrustState_.target = 0.0;
}

void basicMainEngineModel::setTargetInPercentage(const double &tThrustInPercentage)
{
    engineConfig_.engineActivated ? thrustState_.target = tThrustInPercentage * engineConfig_.maxThrust : thrustState_.target = 0.0;
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

double basicMainEngineModel::getTankID() const
{
    return engineConfig_.tankID;
}

Vector3 basicMainEngineModel::getDirectionOfThrust() const
{
    return engineConfig_.direction;
}

double basicMainEngineModel::getMaxThrust() const
{
    return engineConfig_.maxThrust;
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
