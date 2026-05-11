#include "Thrust/BasicRCSModel.h"

// -------------------------------------------------------------------------
// Public class methods
// -------------------------------------------------------------------------

void basicRCSModel::updateThrust(const double &dt)
{
    totalEngineTime += dt;

    // Simulate the time required to adjust the valve
    const double delayedTime = totalEngineTime - rcsConfig_.commandDelay;

    while(!commandBuffer.empty() && commandBuffer.front().time <= delayedTime)
    {
        cmdInputDelayed = commandBuffer.front().cmd;
        commandBuffer.pop_front();
    }

    calcThrust(cmdInputDelayed, rcsConfig_.tauOn, rcsConfig_.tauOff, dt, rcsConfig_.maxThrust);

}

// -------------------------------------------------------------------------
// Public setter
// -------------------------------------------------------------------------
void basicRCSModel::setTargetInPercentage(const double &tThrustInPercentage)
{
    int target = convertToBinaryCommand(tThrustInPercentage);

    cmdInput = target;

    commandBuffer.push_back({RCSCommandSample{.time = totalEngineTime, .cmd = cmdInput}});
}

// -------------------------------------------------------------------------
// Private calculation methods
// -------------------------------------------------------------------------
double basicRCSModel::calcThrust(const int cmdInputDelayed, const double &tauOn, const double &tauOff, const double &dt, const double &nominalThrust)
{
    double thrustState = 0.0;

    if (cmdInputDelayed == 1)
    {
        thrustState += (1.0 - thrustState) / (dt / tauOn);
    }
    else
    {
        thrustState += (0.0 - thrustState) / (dt / tauOff);
    }
    return ;
}

int basicRCSModel::convertToBinaryCommand(double input)
{
    int target = 0;

    // Check whether the user input is binary
    if (input != 0.0 && input != 1.0)
    {
        std::cerr << "[basicRCSModel]-setTargetInPercentage- BasicRCSModel only accept binary input. Input is going to round up" << std::endl;

        if (input > 0.0 && input <= 0.49)
        {
            std::cerr << "[basicRCSModel]-setTargetInPercentage- Command Input is rounded down to zero " << std::endl;
            target = 0;
        }
        else if (input >0.49 && input < 1.0)
        {
            std::cerr << "[basicRCSModel]-setTargetInPercentage- Command Input is rounded up to one" << std::endl;
            target = 1;
        }
        else
        {
            std::cerr << "[basicRCSModel]-setTargetInPercentage- Command Input is invalid, because the value is ether smaller than zero or higher than one." << std::endl;
            std::cerr << "[basicRCSModel]-setTargetInPercentage- Command Input can not be processed!" << std::endl;
        }
    }
    else
    {
        target = static_cast<int>(input);
    }

    return target;
}
