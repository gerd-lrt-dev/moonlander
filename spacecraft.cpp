#include "spacecraft.h"

#include "spacemath.h"

spacecraft::spacecraft(double m, double maxT, double rate, double fuelM)
    : emptyMass(m), maxThrust(maxT), mainEngine(rate), fuelMass(fuelM) 
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

void spacecraft::updateTime(double transferedDT)
{
    dt = transferedDT;
    time += dt;
}