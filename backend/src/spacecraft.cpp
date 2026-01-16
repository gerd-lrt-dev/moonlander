#include "spacecraft.h"

#include "spacemath.h"

// -------------------------------------------------------------------------
// Private
// -------------------------------------------------------------------------

void spacecraft::setDefaultValues()
{
    spacecraftIntegrity = 1.0;
    spacecraftIsOperational = true;
    totalMass = landerMoon.emptyMass + landerMoon.fuelM;
    state_.I_Position = landerMoon.I_initialPos;
    state_.I_Velocity = landerMoon.I_initialVelocity;
}

void spacecraft::updateTotalMassOnFuelReduction(double emptyMass, double fuelMass)
{
    state_.totalMass = emptyMass + fuelMass;
}

#include <iostream>

void spacecraft::updateMovementData(double dt)
{
    // --- Pre-Checks ---
    if (!physics_)
    {
        std::cerr << "[CRASH] physics_ pointer is null!" << std::endl;
        return;
    }

    // --- Thrust ---
    Vector3 thrustDir = requestThrustDirection();

    // --- Compute acceleration ---
    Vector3 acceleration = physics_->computeAcc(
        requestThrust(),
        getTotalMass(),
        thrustDir,
        environmentConfig_.moonGravityVec
        );

    // --- Compute velocity ---
    Vector3 velocity = physics_->computeVel(state_.I_Velocity, acceleration, dt);

    // --- Compute position ---
    Vector3 position = physics_->computePos(velocity, state_.I_Position, acceleration, dt);

    // --- TODO: Compute orientation and angular velocity ---
    // ...

    // --- TODO: Update total mass ---
    // ...

    // --- Commit to state vector ---
    state_.I_Velocity = velocity;
    state_.I_Position = position;
}


void spacecraft::setSpacecraftState(SpacecraftState newState)
{
    spacecraftState_ = newState;
}

SpacecraftState spacecraft::getSpacecraftState() const
{
    return spacecraftState_;
}

void spacecraft::setPosition(const Vector3& pos)
{
    state_.I_Position = pos;
}

void spacecraft::setVelocity(const Vector3& vel)
{
    state_.I_Velocity = vel;
}

void spacecraft::setOrientation(const Quaternion& q)
{
    state_.IB_Orientation = q;
}

void spacecraft::setAngularVelocity(const Vector3& angVel)
{
    state_.B_AngularVelocity = angVel;
}

// -------------------------------------------------------------------------
// Public
// -------------------------------------------------------------------------
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
        // initialize physics
        physics_ = std::make_unique<physics>();
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

void spacecraft::updateStep(double dt)
{
    // Update mass data
    updateTotalMassOnFuelReduction(landerMoon.emptyMass, getfuelMass());

    // Update Movement data due to spacecraft state
    switch (spacecraftState_)
    {
    case SpacecraftState::Operational:
        updateMovementData(dt);
        break;

    case SpacecraftState::Landed:
        // Translation disabled, rotation optional
        break;

    case SpacecraftState::Crashed:
        // Freeze kinematics, allow logging
        break;

    case SpacecraftState::Destroyed:
        // Do nothing
        break;
    }

    // Apply landing damage
    if (state_.I_Velocity.z <= -0.1)
    {
        applyLandingDamage(state_.I_Velocity.z);
    }

    updateSpacecraftIntegrity();
}

void spacecraft::updateSpacecraftIntegrity()
{
    if(spacecraftIntegrity > 1.0) spacecraftIntegrity = 1.0;
    if(spacecraftIntegrity < 0.0) spacecraftIntegrity = 0.0;

    // Update operational status
    // Update operational status
    if (spacecraftIntegrity <= 0.0)
    {
        spacecraftState_ = SpacecraftState::Destroyed;
    }
    else if (spacecraftIntegrity < 1.0)
    {
        spacecraftState_ = SpacecraftState::Crashed;
    }
    else if (spacecraftIntegrity <= landerMoon.structuralIntegrity)
    {
        spacecraftState_ = SpacecraftState::Operational;
    }
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
    mainEngine.updateThrust(dt);
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

void spacecraft::setInitalPosition(const Vector3& position)
{
    landerMoon.I_initialPos = position;
}

void spacecraft::setInitalVelocity(const Vector3& velocity)
{
    landerMoon.I_initialVelocity = velocity;
}

simData spacecraft::getFullSimulationData() const
{
    simData simData_;

    simData_.statevector_ = getState();

    // Fill struct with data for emitting signal to UI
    simData_.spacecraftState_ = spacecraftState_;
    simData_.spacecraftIntegrity = true; // TODO: Change to spacecraft state

    simData_.thrust = requestThrust();
    simData_.targetThrust = requestTargetThrust();
    simData_.fuelMass = getfuelMass();
    simData_.fuelFlow = requestLiveFuelConsumption();

    return simData_;
}

double spacecraft::getIntegrity()
{
    return spacecraftIntegrity;
}

const StateVector& spacecraft::getState() const
{
    return state_;
}

Vector3 spacecraft::getPosition() const
{
    return state_.I_Position;
}

Vector3 spacecraft::getVelocity() const
{
    return state_.I_Velocity;
}

Quaternion spacecraft::getOrientation() const
{
    return state_.IB_Orientation;
}

Vector3 spacecraft::getAngularVelocity() const
{
    return state_.B_AngularVelocity;
}

double spacecraft::getTotalMass()
{
    return state_.totalMass;
}

double spacecraft::getfuelMass() const
{
    return mainEngine.getCurrentFuelMass();
}
