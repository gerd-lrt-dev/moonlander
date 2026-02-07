#ifndef PHYSICS_H
#define PHYSICS_H

#include <memory>

#include "environmentConfig.h"
#include "vector3.h"
#include "spacemath.h"
#include "Physics/iPhysicsModel.h"
#include "Integrators/iIntegrator.h"
#include "Sensory_Perception/iSensor.h"

/**
 * @class physics
 * @brief Orchestrator class coordinating physics model and numerical integration.
 *
 * The physics class acts as a façade and coordination layer between
 * the physical model (IPhysicsModel) and the numerical integrator
 * (IIntegrator). It does not implement physical laws or numerical
 * algorithms itself.
 *
 * Responsibilities:
 * - Query the active physics model for current acceleration.
 * - Delegate position and velocity updates to the configured integrator.
 * - Provide wrapper functions for external systems (e.g. spacecraft)
 *   to keep their interface stable and independent of internal changes.
 *
 * This design enables interchangeable physics models (e.g. Moon, Mars,
 * drag models) and integrators (Euler, RK4, Verlet) without modifying
 * higher-level systems.
 *
 * The class is intentionally lightweight and stateless except for
 * references to the selected model and integrator.
 */
class physics
{
private:
    // Constants & parameters
    EnvironmentConfig configData;
    spacemath math;
    std::shared_ptr<IPhysicsModel> model_;
    std::shared_ptr<IIntegrator> integrator_;
    std::shared_ptr<ISensor> sensor_;
public:
    /**
     * @brief Constructor
     */
    physics(std::shared_ptr<IPhysicsModel> model, std::shared_ptr<IIntegrator> integrator, std::shared_ptr<ISensor> sensor) : model_(model), integrator_(integrator), sensor_(sensor) {};

    /**
     * @brief Destructor
     */
    ~physics();

    /**
     * @brief Computes the current acceleration via the active physics model.
     *
     * Wrapper function delegating the calculation to the configured
     * IPhysicsModel implementation.
     *
     * @param thrust    Current thrust force magnitude.
     * @param mass      Current spacecraft mass.
     * @param thrustDir Normalized thrust direction vector.
     * @param pos       Current position vector.
     * @return Resulting acceleration vector.
     */
    Vector3 computeAcc(const Vector3& pos, const Vector3& vel, double mass, double thrust, const Vector3& thrustDir) const;

    /**
     * @brief Integrates velocity using the configured integrator.
     *
     * Wrapper function delegating numerical integration to IIntegrator.
     *
     * @param vel Current velocity vector.
     * @param acc Current acceleration vector.
     * @param dt  Time step in seconds.
     * @return Updated velocity vector.
     */
    Vector3 computeVel(const Vector3& vel, const Vector3& acc, double dt) const;

    /**
     * @brief Integrates position using the configured integrator.
     *
     * Wrapper function delegating numerical integration to IIntegrator.
     *
     * @param pos Current position vector.
     * @param vel Current velocity vector.
     * @param acc Current acceleration vector.
     * @param dt  Time step in seconds.
     * @return Updated position vector.
     */
    Vector3 computePos(const Vector3& pos, const Vector3& vel, const Vector3& acc, double dt) const;

    /**
     * @brief Computes the proper G-load experienced by the spacecraft.
     *
     * This function calculates the acceleration actually felt by the spacecraft,
     * excluding the effect of gravity. The result is expressed in multiples of
     * Earth's standard gravity (g₀ = 9.80665 m/s²).
     *
     * As physics is now an orchestrator, this method is a **derived metric**
     * based on the current total acceleration and the gravitational component
     * provided by the active physics model. It does not modify the simulation state.
     *
     * @param totalAcceleration   Total acceleration acting on the spacecraft [m/s²],
     *                            typically including thrust and gravity.
     * @param gravityAcceleration Gravitational acceleration vector [m/s²] at the
     *                            spacecraft's current position.
     *
     * @return Proper G-load in multiples of g₀.
     *
     * @note Proper acceleration is computed by subtracting gravity from the total acceleration:
     *       \f$ G = \frac{||\vec{a}_\text{total} - \vec{a}_\text{gravity}||}{g_0} \f$
     * @note This method is intended for telemetry, sensor simulation, or UI display.
     *       It does not affect position, velocity, or any other physics state.
     * @see Vector3
     */
    double computeGLoad(const Vector3& totalAcceleration, const Vector3& gravityAcceleration);
};

#endif
