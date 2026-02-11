#pragma once

class IAutopilot{
public:
    virtual ~IAutopilot() = default;

    virtual double setThrust(const double& thrust) const = 0;
};
