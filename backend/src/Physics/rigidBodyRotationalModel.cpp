#include "Physics/rigidBodyRotationalModel.h"

Eigen::Vector3d RigidBodyRotationalModel::computeAngularAcceleration(const Eigen::Vector3d& SBF_angularVelocity, const Eigen::Matrix3d& SBF_inertia, const Eigen::Vector3d& SBF_torque) const
{
    Eigen::Vector3d SBF_angularMomentum = SBF_inertia * SBF_angularVelocity;

    Eigen::Vector3d SBF_gyroTerm        = SBF_angularVelocity.cross(SBF_angularMomentum);

    return SBF_inertia.inverse() * (SBF_torque - SBF_gyroTerm);
}
