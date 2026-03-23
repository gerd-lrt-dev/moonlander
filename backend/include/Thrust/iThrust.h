#pragma once

#include "vector3.h"

class IThrustModel{
public:

    virtual ~IThrustModel() = default;

    virtual void updateThrust(const double &dt) = 0;

    virtual double calcFuelReduction(const double &fuelMass,const double &massFlowFuel,const double &dt) = 0;

    virtual void setTarget(const double &tThrust) = 0;

    virtual double getTargetThrust() const = 0;

    virtual double getCurrentThrust() const = 0;

    virtual Vector3 getDirectionOfThrust() const = 0;

    virtual double getFuelConsumption() const = 0;

    virtual double getCurrentFuelMass() const = 0;
};
