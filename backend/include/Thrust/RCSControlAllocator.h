#pragma once

#include <cmath>
#include <eigen3/Eigen/Dense>

class RCSControlAllocator
{
public:
    static double mapTranslationCommandToThrusterPercentage(const Eigen::Vector3d& rcsCommand, const Eigen::Vector3d& thrusterDirection)
    {
        const bool fire =
            (rcsCommand.x() > 0.0 && thrusterDirection.x() > 0.0) ||
            (rcsCommand.x() < 0.0 && thrusterDirection.x() < 0.0) ||
            (rcsCommand.y() > 0.0 && thrusterDirection.y() > 0.0) ||
            (rcsCommand.y() < 0.0 && thrusterDirection.y() < 0.0) ||
            (rcsCommand.z() > 0.0 && thrusterDirection.z() > 0.0) ||
            (rcsCommand.z() < 0.0 && thrusterDirection.z() < 0.0);

        return fire ? 1.0 : 0.0;
    }

    static double mapAttitudeCommandToThrusterPercentage(const Eigen::Vector3d& rcsCommand, const Eigen::Vector3d& thrusterPosition, const Eigen::Vector3d& centerOfMass, const Eigen::Vector3d& thrusterDirection)
    {
        const Eigen::Vector3d leverArm =
            thrusterPosition - centerOfMass;

        const Eigen::Vector3d torqueDirection =
            leverArm.cross(thrusterDirection);

        const bool fire =
            (rcsCommand.x() > 0.0 && torqueDirection.x() > 0.0) ||
            (rcsCommand.x() < 0.0 && torqueDirection.x() < 0.0) ||
            (rcsCommand.y() > 0.0 && torqueDirection.y() > 0.0) ||
            (rcsCommand.y() < 0.0 && torqueDirection.y() < 0.0) ||
            (rcsCommand.z() > 0.0 && torqueDirection.z() > 0.0) ||
            (rcsCommand.z() < 0.0 && torqueDirection.z() < 0.0);

        return fire ? 1.0 : 0.0;
    }

    static double mapTranslationCommandToThrusterNewton(const Eigen::Vector3d& rcsCommand, const Eigen::Vector3d& thrusterDirection)
    {
        if (rcsCommand.x() > 0.0 && thrusterDirection.x() > 0.0)
            return std::abs(rcsCommand.x());

        if (rcsCommand.x() < 0.0 && thrusterDirection.x() < 0.0)
            return std::abs(rcsCommand.x());

        if (rcsCommand.y() > 0.0 && thrusterDirection.y() > 0.0)
            return std::abs(rcsCommand.y());

        if (rcsCommand.y() < 0.0 && thrusterDirection.y() < 0.0)
            return std::abs(rcsCommand.y());

        if (rcsCommand.z() > 0.0 && thrusterDirection.z() > 0.0)
            return std::abs(rcsCommand.z());

        if (rcsCommand.z() < 0.0 && thrusterDirection.z() < 0.0)
            return std::abs(rcsCommand.z());

        return 0.0;
    }
};
