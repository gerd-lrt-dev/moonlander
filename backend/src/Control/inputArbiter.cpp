#include "Control/inputArbiter.h"
#include <iostream>

ControlCommand InputArbiter::chooseCommand()
{
    if (!automationActive)
    {
        return usrCmd_;
    }
    else
    {
        return autoCmd_;
    }
}

void InputArbiter::receiveUserControlCommand(const ControlCommand &userCmd)
{
    usrCmd_ = userCmd;
    automationActive = userCmd.autopilotActive;
}
void InputArbiter::receiveAutoControlCommand(const ControlCommand &autoCmd)
{
    autoCmd_ = autoCmd;
}

void InputArbiter::setAutomationActiveFlag(bool on)
{
    automationActive = on;
}
