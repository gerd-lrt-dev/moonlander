#pragma once

struct ControlCommand{
    double thrustInPercentage;
    double thrustInNewton;
};

class InputArbiter{
public:
    ControlCommand chooseCommand(const ControlCommand* userInput, const ControlCommand* automation);

    void setAutomationActiveFlag(bool on);

private:
    bool automationActive;
};
