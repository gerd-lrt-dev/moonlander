#include "Control/inputArbiter.h"

ControlCommand InputArbiter::chooseCommand(const ControlCommand* userInput, const ControlCommand* automation)
{
    return automationActive ? *automation : *userInput;
}

void InputArbiter::setAutomationActiveFlag(bool on)
{
    automationActive = on;
}
