#pragma once

#include "Automation/iautopilot.h"

class FlightController : public IAutopilot
{
public:
    double setThrust(const double& thrust) const override;
};
