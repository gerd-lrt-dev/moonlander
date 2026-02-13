#pragma once
#include <optional>

struct ControlCommand{
    double thrustInPercentage = 0.0;
    double thrustInNewton = 0.0;
    bool autopilotActive = false;
};

class InputArbiter{
public:
    ControlCommand chooseCommand(const std::optional<ControlCommand>& userCmd, const std::optional<ControlCommand>& autoCmd);

private:
    void setAutomationActiveFlag(bool on);
    bool automationActive = false;
};
