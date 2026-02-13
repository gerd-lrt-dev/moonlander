#pragma once

struct ControlCommand{
    double thrustInPercentage;
    double thrustInNewton;
};

class InputArbiter{
public:
    ControlCommand chooseCommand(ControlCommand* userInput, ControlCommand* automation);

    void setAutomationActiveFlag(bool on);

private:
    bool automationActive = false;
};
