#include "Integrators/eulerIntegrator.h"

Eigen::Vector3d EulerIntegrator::integrateFirstOrder(const Eigen::Vector3d& vel, const Eigen::Vector3d& acc, double dt) const
{
    return vel + acc * dt;
}

Eigen::Vector3d EulerIntegrator::integrateSecondOrder(const Eigen::Vector3d& pos, const Eigen::Vector3d& vel, const Eigen::Vector3d& acc, double dt) const
{
    return pos + vel * dt + acc * 0.5 * dt * dt;
}

Eigen::Quaterniond EulerIntegrator::integrateQuaternion(const Eigen::Quaterniond& quaternion, const Eigen::Vector3d& angularVelocity, double dt) const
{
    const Eigen::Quaterniond omegaQuaternion(0.0, angularVelocity.x(), angularVelocity.y(), angularVelocity.z());

    Eigen::Quaterniond qDot = quaternion * omegaQuaternion;

    qDot.coeffs() *= 0.5;

    Eigen::Quaterniond result = quaternion;
    result.coeffs() += qDot.coeffs() * dt;

    result.normalize();

    return result;
}
