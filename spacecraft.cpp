#include "spacecraft.h"

#include "spacemath.h"

spacecraft::spacecraft(double m, double maxT, double rate, double fuelM, double timeConstant, Vector3 initialPos, Vector3 initialRot)
    : emptyMass(m), maxThrust(maxT), mainEngine(rate, timeConstant), fuelMass(fuelM), position(initialPos), rotation(initialRot)
    {
        setDefaultValues();
    };

spacecraft::~spacecraft()
{
}

bool spacecraft::isIntact()
{
    if (spacecraftIntegrity < 0.5)
    {
        spacecraftIsOperational = false;
    }
    else 
    {
        spacecraftIsOperational = true;
    }

    return spacecraftIsOperational;
}

void spacecraft::setDefaultValues()
{
    spacecraftIntegrity = 1.0;
    structuralIntegrity = 1.0;
    spacecraftIsOperational = true;
    totalMass = emptyMass + fuelMass;
    safeVelocity = 2.0;
}

void spacecraft::updateTotalMassOnFuelReduction(double emptyMass, double fuelMass)
{
    totalMass = emptyMass + fuelMass;
}

void spacecraft::updateSpacecraftIntegrity(double delta)
{
    spacecraftIntegrity += delta;
    if(spacecraftIntegrity > 1.0) spacecraftIntegrity = 1.0;
    if(spacecraftIntegrity < 0.0) spacecraftIntegrity = 0.0;

    // Update operational status
    spacecraftIsOperational = spacecraftIntegrity >= structuralIntegrity;
}

void spacecraft::applyLandingDamage(double impactVelocity)
{
    double KE(0), KEref(0), damageInPercent(0);

    KEref   = spacemath::kineticEnergy(totalMass, safeVelocity);
    KE      = spacemath::kineticEnergy(totalMass, impactVelocity);

    damageInPercent = KE / KEref;

    spacecraftIntegrity += -damageInPercent;
}

void spacecraft::setThrust(double targetThrustInPercentage)
{
    // thrust in percentage = target thrust / maxiumum thrust <=>
    double targetThrust = targetThrustInPercentage * maxThrust; // [m/s²]

    mainEngine.setTarget(targetThrust);
}

void spacecraft::updateTime(double dt)
{
    time += dt;

    // Start updating time for main engine 
    fuelMass = mainEngine.updateThrust(dt, fuelMass);

    //mainEngine.updateSimpleThrust(dt); // TODO: User should start engine and routine starts with that. That need fuel in idle mode. Currently just for testing implemented

}

double spacecraft::requestTargetThrust() const
{
    
    return mainEngine.getTargetThrust();
}

double spacecraft::requestThrust() const
{
    
    return mainEngine.getCurrentThrust();
}

double spacecraft::requestAcceleration() const
{
    return spacemath::accelerationBasedOnThrust(mainEngine.getCurrentThrust(), totalMass);
}

double spacecraft::requestLiveFuelConsumption() const
{
    return mainEngine.getFuelConsumption();
}

void spacecraft::setPos(Vector3 pos)
{
    position = pos; 
}

void spacecraft::setRot(Vector3 rot)
{
    rotation = rot;
}

void spacecraft::setVel(Vector3 vel)
{
    velocity = vel;
}

Vector3 spacecraft::getPos()
{
    return position;
}

Vector3 spacecraft::getRot()
{
    return rotation;
}

Vector3 spacecraft::getVel()
{
    return velocity;
}