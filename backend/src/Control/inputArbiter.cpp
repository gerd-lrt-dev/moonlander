#include "Control/inputArbiter.h"

ControlCommand InputArbiter::chooseCommand()
{
    ControlCommand cmd;

    if (!automationActive)
    {
        return usrCmd_;
    }
    else
    {
        cmd.mainEngine          = autoCmd_.mainEngine;
        cmd.thrustInPercentage  = autoCmd_.thrustInPercentage;

        cmd.translation         = usrCmd_.translation;
        cmd.rotation            = usrCmd_.rotation;
        cmd.autopilotActive     = usrCmd_.autopilotActive;
        cmd.killRotation        = usrCmd_.killRotation;
    }

    return cmd;
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
