#include "spacecraft.h"

#include "spacemath.h"

spacecraft::spacecraft(customSpacecraft lMoon)
    : landerMoon(lMoon),
    mainEngine(
        EngineConfig::Create(
            lMoon.Isp,
            lMoon.timeConstant,
            lMoon.responseRate,
            lMoon.B_mainThrustDirection
        ),
        FuelState(lMoon.fuelM, lMoon.fuelM, 0.0)
    )
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
    spacecraftIsOperational = true;
    totalMass = landerMoon.emptyMass + landerMoon.fuelM;
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
    spacecraftIsOperational = spacecraftIntegrity >= landerMoon.structuralIntegrity;
}

void spacecraft::applyLandingDamage(double impactVelocity)
{
    double KE(0), KEref(0), damageInPercent(0);

    KEref   = spacemath::kineticEnergy(totalMass, landerMoon.safeVelocity);
    KE      = spacemath::kineticEnergy(totalMass, impactVelocity);

    damageInPercent = KE / KEref;

    spacecraftIntegrity += -damageInPercent;
}

void spacecraft::setThrust(double targetThrustInPercentage)
{
    // thrust in percentage = target thrust / maxiumum thrust <=>
    double targetThrust = targetThrustInPercentage * landerMoon.maxT; // [m/s²]

    mainEngine.setTarget(targetThrust);
}

void spacecraft::updateTime(double dt)
{
    time += dt;

    // Start updating time for main engine 
    landerMoon.fuelM = mainEngine.updateThrust(dt, landerMoon.fuelM);
}

double spacecraft::requestTargetThrust() const
{
    
    return mainEngine.getTargetThrust();
}

double spacecraft::requestThrust() const
{
    
    return mainEngine.getCurrentThrust();
}

Vector3 spacecraft::requestThrustDirection() const
{
    return mainEngine.getDirectionOfThrust();
}

double spacecraft::requestLiveFuelConsumption() const
{
    return mainEngine.getFuelConsumption();
}

void spacecraft::setPos(Vector3 pos)
{
    B_Pos = pos; 
}

void spacecraft::setRot(Vector3 rot)
{
    B_Rot = rot;
}

void spacecraft::setVel(Vector3 vel)
{
    B_Vel = vel;
}

Vector3 spacecraft::getPos()
{
    return B_Pos;
}

Vector3 spacecraft::getRot()
{
    return B_Rot;
}

Vector3 spacecraft::getVel()
{
    return B_Vel;
}

double spacecraft::getTotalMass()
{
    return totalMass;
}