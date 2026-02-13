#include "Control/inputArbiter.h"

ControlCommand InputArbiter::chooseCommand(ControlCommand* userInput, ControlCommand* automation)
{
    return automationActive ? *automation : *userInput;
}

void InputArbiter::setAutomationActiveFlag(bool on)
{
    automationActive = on;
}
