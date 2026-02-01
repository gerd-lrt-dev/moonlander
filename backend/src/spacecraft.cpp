#include "spacecraft.h"

#include "spacemath.h"

#include <iomanip>

// -------------------------------------------------------------------------
// Private
// -------------------------------------------------------------------------

void spacecraft::setDefaultValues()
{
    spacecraftIntegrity = 1.0;
    spacecraftState_ = SpacecraftState::Operational;
    totalMass = landerMoon.emptyMass + landerMoon.fuelM;
    state_.I_Position = landerMoon.I_initialPos;
    state_.I_Velocity = landerMoon.I_initialVelocity;

    // TODO just testing here
    /*
    std::vector<double> thrust = compute_optimization(landerMoon.I_initialPos.z, landerMoon.I_initialVelocity.z, totalMass, 1.0);

    std::cout << "\n=== Thrust Optimization Result ===\n";
    std::cout << "Steps: " << thrust.size() << "\n";
    std::cout << "dt   : " << dt << " s\n\n";

    std::cout << " k\t time [s]\t thrust [N]\n";
    std::cout << "-------------------------------------\n";

    for (size_t k = 0; k < thrust.size(); ++k)
    {
        std::cout
            << std::setw(3) << k << "\t"
            << std::setw(8) << k * dt << "\t"
            << std::setw(12) << thrust[k] << "\n";
    }

    std::cout << "=====================================\n";
    */
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
    Vector3 acceleration = physics_->computeAcc(requestThrust(), getTotalMass(), thrustDir, getPosition());

    // --- Compute velocity ---
    Vector3 velocity = physics_->computeVel(getVelocity(), acceleration, dt);

    // --- Compute position ---
    Vector3 position = physics_->computePos(velocity, getPosition(), acceleration, dt);

    // --- TODO: Compute orientation and angular velocity ---
    // ...

    // --- TODO: Update total mass ---
    // ...

    updateGLoad(acceleration, environmentConfig_.moonGravityVec);

    // --- Commit to state vector ---
    setVelocity(velocity);
    setPosition(position);
    //setGload(GLoad);
}

void spacecraft::updateMovementDataToZero(double dt)
{
    // --- Commit to state vector ---
    setVelocity({0.0, 0.0, 0.0});
}

void spacecraft::updateGLoad(const Vector3& totalAcceleration, const Vector3& gravityAcceleration)
{
    GLoad = physics_->computeGLoad(totalAcceleration, environmentConfig_.moonGravityVec);
}

void spacecraft::setSpacecraftState(SpacecraftState newState)
{
    spacecraftState_ = newState;
}

SpacecraftState spacecraft::getSpacecraftState() const
{
    return spacecraftState_;
}

void spacecraft::applyLandingDamage(double impactVelocity)
{
    double KE(0), KEref(0), damageInPercent(0);

    KEref   = spacemath::kineticEnergy(totalMass, landerMoon.safeVelocity);
    KE      = spacemath::kineticEnergy(totalMass, impactVelocity);

    damageInPercent = KE / KEref;

    spacecraftIntegrity += -damageInPercent;
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

void spacecraft::updateStep(double dt)
{
    // Update mass data
    updateTotalMassOnFuelReduction(landerMoon.emptyMass, getfuelMass());

    // Apply landing damage
    if (state_.I_Position.z <= environmentConfig_.radiusMoon)
    {
        applyLandingDamage(state_.I_Velocity.z);
    }

    updateSpacecraftIntegrity();

    // Update Movement data due to spacecraft state
    switch (spacecraftState_)
    {
    case SpacecraftState::Operational:
        updateMovementData(dt);
        break;

    case SpacecraftState::Landed:
        // Translation disabled, rotation optional
        updateMovementDataToZero(dt);
        break;

    case SpacecraftState::Crashed:
        // Freeze kinematics, allow logging
        break;

    case SpacecraftState::Destroyed:
        // Do nothing
        break;
    }
}

void spacecraft::updateSpacecraftIntegrity()
{
    if(spacecraftIntegrity > 1.0) spacecraftIntegrity = 1.0;
    if(spacecraftIntegrity < 0.0) spacecraftIntegrity = 0.0;

    // --- Update spacecraft state ---
    //TODO: set thrust to zero when landed etc.

    // 1. Completely destroyed (terminal)
    if (spacecraftIntegrity <= 0.0)
    {
        spacecraftState_ = SpacecraftState::Destroyed;
        return;
    }

    // 2. Structural failure (terminal but stable)
    if (spacecraftIntegrity < landerMoon.structuralIntegrity)
    {
        spacecraftState_ = SpacecraftState::Crashed;
        return;
    }

    // 3. Successful touchdown
    if (getPosition().z <= environmentConfig_.radiusMoon)
    {
        spacecraftState_ = SpacecraftState::Landed;
        return;
    }

    // 4. Still operational (possibly damaged)
    spacecraftState_ = SpacecraftState::Operational;
}

void spacecraft::setThrust(double targetThrustInPercentage)
{
    // thrust in percentage = target thrust / maxiumum thrust <=>
    double targetThrust = targetThrustInPercentage * landerMoon.maxT; // [m/s²]

    mainEngine.setTarget(targetThrust);
}

std::vector<double> spacecraft::compute_optimization(double h0, double v0, double m0, double dt)
{
    ThrustOptimizationProblem problem;

    problem.x0.h = h0;
    problem.x0.v = v0;
    problem.x0.m = m0;

    problem.params.g = 1.64;
    problem.params.Isp = 300.0;

    problem.dt = dt;
    problem.N  = 40;

    problem.w_h = 1.0;
    problem.w_v = 1.0;
    problem.w_m = 0.1;
    problem.w_T = 0.01;

    problem.h_ref = std::max(1.0, std::abs(h0));
    problem.v_ref = std::max(1.0, std::abs(v0));
    problem.m_ref = m0;
    problem.T_ref = landerMoon.maxT;

    ThrustOptimizer optimizer;
    return optimizer.optimize(problem, landerMoon.maxT);
}

//TODO: Obsolete?
//TODO:
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

    // Reduce height by radius of moon
    simData_.statevector_.I_Position.z = simData_.statevector_.I_Position.z - environmentConfig_.radiusMoon;

    // Fill struct with data for emitting signal to UI
    simData_.spacecraftState_ = spacecraftState_;

    simData_.thrust = requestThrust();
    simData_.targetThrust = requestTargetThrust();

    simData_.fuelMass = getfuelMass();
    simData_.fuelFlow = requestLiveFuelConsumption();

    simData_.GLoad = getGload();

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

double spacecraft::getGload() const
{
    return GLoad;
}
