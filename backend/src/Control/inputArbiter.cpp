#include "Control/inputArbiter.h"
#include <iostream>

ControlCommand InputArbiter::chooseCommand(const std::optional<ControlCommand>& userCmd, const std::optional<ControlCommand>& autoCmd)
{
    if (userCmd.has_value())
    {
        setAutomationActiveFlag(userCmd->autopilotActive);
    }
    else
    {
        setAutomationActiveFlag(false);
    }

    std::cout << "Thrust percent: " << userCmd->thrustInPercentage << std::endl;

    if (automationActive && autoCmd.has_value())
    {
        return *autoCmd;
    }
    else if (!automationActive && userCmd.has_value())
    {
        return *userCmd;
    }
    else
    {
        return ControlCommand{0, 0}; // Default fallback
    }
}

void InputArbiter::setAutomationActiveFlag(bool on)
{
    automationActive = on;
}
