#ifndef BASICROTATIONALDYNAMICS_H
#define BASICROTATIONALDYNAMICS_H

#include "Physics/iRotationalPhysicsModel.h"
#include "environmentConfig.h"

class RigidBodyRotationalModel : public IRotationalPhysicsModel{
public:
    /**
     * @brief Constructs the model with environment configuration data.
     *
     * @param cfg Reference to global environment configuration containing
     *            physical constants such as gravitational parameters.
     */
    RigidBodyRotationalModel(const EnvironmentConfig& cfg) : configData(cfg) {}

private:
    const EnvironmentConfig& configData;
};

#endif // BASICROTATIONALDYNAMICS_H
