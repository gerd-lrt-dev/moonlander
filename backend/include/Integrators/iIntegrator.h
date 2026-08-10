#pragma once

#include <eigen3/Eigen/Dense>

/**
 * @class IIntegrator
 * @brief Interface for numerical time integration.
 *
 * IIntegrator defines the contract for all numerical integration schemes
 * used to advance simulation states in time. Implementations are purely
 * mathematical and independent of any physical model.
 *
 * Typical implementations include explicit Euler, Semi-Implicit Euler,
 * Verlet and Runge-Kutta methods.
 */
class IIntegrator {
public:

    /**
     * @brief Virtual destructor to allow proper cleanup of derived classes.
     */
    virtual ~IIntegrator() = default;

    /**
     * @brief Integrates a first-order vector quantity over one timestep.
     *
     * Advances an arbitrary vector quantity using its first time derivative.
     * Typical applications include:
     * - linear velocity from linear acceleration
     * - angular velocity from angular acceleration
     *
     * @param value      Current vector quantity.
     * @param derivative Current first time derivative of the quantity.
     * @param dt         Simulation timestep [s].
     * @return Integrated vector quantity after one timestep.
     */
    virtual Eigen::Vector3d integrateFirstOrder(const Eigen::Vector3d& value, const Eigen::Vector3d& derivative, double dt) const = 0;

    /**
     * @brief Integrates a second-order vector quantity over one timestep.
     *
     * Advances a vector quantity using its first and second time derivatives.
     * This method is typically used for translational position integration.
     *
     * @param value            Current vector quantity.
     * @param firstDerivative  Current first time derivative.
     * @param secondDerivative Current second time derivative.
     * @param dt               Simulation timestep [s].
     * @return Integrated vector quantity after one timestep.
     */
    virtual Eigen::Vector3d integrateSecondOrder(const Eigen::Vector3d& value, const Eigen::Vector3d& firstDerivative, const Eigen::Vector3d& secondDerivative, double dt) const = 0;

    /**
     * @brief Integrates spacecraft attitude from angular velocity.
     *
     * Advances the spacecraft orientation quaternion over one simulation
     * timestep using quaternion kinematics. The angular velocity is assumed
     * to be expressed in the spacecraft body-fixed frame (SBF).
     *
     * The resulting quaternion is normalized after integration to compensate
     * for numerical drift.
     *
     * @param attitude        Current spacecraft attitude quaternion.
     * @param angularVelocity Current angular velocity in SBF [rad/s].
     * @param dt              Simulation timestep [s].
     * @return Updated normalized spacecraft attitude quaternion.
     */
    virtual Eigen::Quaterniond integrateQuaternion(const Eigen::Quaterniond& attitude, const Eigen::Vector3d& angularVelocity, double dt) const = 0;
};
