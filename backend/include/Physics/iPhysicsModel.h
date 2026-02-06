#pragma once
#include "vector3.h"

/**
 * @class IPhysicsModel
 * @brief Interface for physical acceleration models.
 *
 * IPhysicsModel defines the contract for all physics models that compute
 * the resulting acceleration acting on an object (e.g. a spacecraft)
 * based on its current state and external forces.
 *
 * Implementations may include gravity models, atmospheric drag,
 * multi-body gravity, or simplified test models. The interface is
 * intentionally independent of any numerical integration scheme.
 */
class IPhysicsModel {
public:

    /**
     * @brief Virtual destructor to ensure proper cleanup of derived models.
     */
    virtual ~IPhysicsModel() = default;

    /**
     * @brief Computes the total acceleration acting on an object.
     *
     * The returned vector typically combines all relevant forces such as
     * gravity, thrust, or other environmental effects, converted to
     * acceleration.
     *
     * @param pos       Current position vector in world space.
     * @param vel       Current velocity vector in world space.
     * @param mass      Current mass of the object.
     * @param thrust    Current thrust force magnitude.
     * @param thrustDir Normalized thrust direction vector.
     * @return Resulting acceleration vector.
     */
    virtual Vector3 computeAcceleration(const Vector3& pos, const Vector3& vel, double mass, double thrust, const Vector3& thrustDir) const = 0;
};
