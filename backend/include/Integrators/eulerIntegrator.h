#pragma once

#include "Integrators/iIntegrator.h"

#include <eigen3/Eigen/Dense>

/**
 * @class EulerIntegrator
 * @brief Concrete integrator implementing the explicit Euler method.
 *
 * The EulerIntegrator advances position and velocity using a first–order
 * explicit Euler scheme assuming constant acceleration within a timestep.
 *
 * This class is purely numerical and independent of any physical model.
 * It operates only on vectors and time deltas.
 */
class EulerIntegrator : public IIntegrator {
public:

    /**
     * @brief Integrates a vector quantity over one simulation timestep using
     *        explicit Euler integration.
     *
     * The method advances an arbitrary vector quantity by integrating its
     * first derivative over the given timestep:
     *
     * \f[
     * \mathbf{x}(t+\Delta t)
     * =
     * \mathbf{x}(t)
     * +
     * \dot{\mathbf{x}}(t)\Delta t
     * \f]
     *
     * Typical applications include:
     * - linear velocity from linear acceleration
     * - angular velocity from angular acceleration
     *
     * @param value      Current vector quantity.
     * @param derivative Current time derivative of the quantity.
     * @param dt         Simulation time step [s].
     * @return Integrated vector quantity after the timestep.
     */
    virtual Eigen::Vector3d integrateFirstOrder(const Eigen::Vector3d& value, const Eigen::Vector3d& derivative, double dt) const override;

    /**
     * @brief Integrates a vector quantity and its first derivative assuming
     *        constant second derivative during the timestep.
     *
     * The method performs a second-order explicit Euler update:
     *
     * \f[
     * \mathbf{x}(t+\Delta t)
     * =
     * \mathbf{x}(t)
     * +
     * \dot{\mathbf{x}}(t)\Delta t
     * +
     * \frac12
     * \ddot{\mathbf{x}}(t)\Delta t^2
     * \f]
     *
     * This formulation is typically used to integrate translational position
     * from velocity and acceleration.
     *
     * @param value            Current vector quantity.
     * @param firstDerivative  Current first derivative.
     * @param secondDerivative Current second derivative.
     * @param dt               Simulation time step [s].
     * @return Integrated vector quantity after the timestep.
     */
    virtual Eigen::Vector3d integrateSecondOrder(const Eigen::Vector3d& value, const Eigen::Vector3d& firstDerivative, const Eigen::Vector3d& secondDerivative, double dt) const override;

    /**
     * @brief Integrates spacecraft attitude from angular velocity.
     *
     * Advances the spacecraft orientation quaternion over one simulation
     * timestep using explicit Euler integration of the quaternion kinematic
     * equation.
     *
     * The angular velocity is expressed in the spacecraft body-fixed frame
     * (SBF). The quaternion represents the rotation from the spacecraft
     * body-fixed frame to the inertial frame.
     *
     * The quaternion derivative is computed as:
     *
     * \f[
     * \dot{q}_{BI}
     * =
     * \frac{1}{2}
     * q_{BI}
     * \otimes
     * \Omega_B
     * \f]
     *
     * with:
     *
     * \f[
     * \Omega_B =
     * \begin{bmatrix}
     * 0 &
     * \omega_x &
     * \omega_y &
     * \omega_z
     * \end{bmatrix}
     * \f]
     *
     * The attitude is then propagated according to:
     *
     * \f[
     * q_{BI}(t+\Delta t)
     * =
     * q_{BI}(t)
     * +
     * \dot{q}_{BI}(t)\Delta t
     * \f]
     *
     * The resulting quaternion is normalized after integration to compensate
     * for numerical drift.
     *
     * @param quaternion      Current spacecraft attitude quaternion.
     * @param angularVelocity Current angular velocity in SBF [rad/s].
     * @param dt              Simulation time step [s].
     * @return Updated and normalized spacecraft attitude quaternion.
     */
    virtual Eigen::Quaterniond integrateQuaternion(const Eigen::Quaterniond& quaternion, const Eigen::Vector3d& angularVelocity, double dt) const override;
};
