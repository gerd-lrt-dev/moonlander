#pragma once
#include <optional>

struct ControlCommand{
    double thrustInPercentage = 0.0;
    double thrustInNewton = 0.0;
    bool autopilotActive = false;
};

class InputArbiter{
public:
    ControlCommand chooseCommand();
    void receiveUserControlCommand(const ControlCommand &userCmd);
    void receiveAutoControlCommand(const ControlCommand &autoCmd);

private:
    void setAutomationActiveFlag(bool on);
    bool automationActive = false;
    ControlCommand usrCmd_;
    ControlCommand autoCmd_;
};
