#ifndef IROTATIONALPHYSICSMODEL_H
#define IROTATIONALPHYSICSMODEL_H

#include <Eigen/Dense>

class IRotationalPhysicsModel {
public:
    virtual ~IRotationalPhysicsModel() = default;

    virtual Eigen::Vector3d computeAngularAcceleration(const Eigen::Vector3d& SBF_angularVelocity, const Eigen::Matrix3d& SBF_inertia, const Eigen::Vector3d& SBF_torque) const = 0;
};

#endif // IROTATIONALPHYSICSMODEL_H
