#pragma once

#include "Automation/iautopilot.h"

class AdaptiveDescentController : public IAutopilot
{
public:
    AdaptiveDescentController (double v_safe) : v_safe_(v_safe) {};
    double setThrust(const double& thrust) const override;

private:
    //***********************************************************
    //*************        Members                   ************
    //***********************************************************
    double v_safe_;
    double v_conservative = v_safe_ * 0.8;

    //***********************************************************
    //*************    Memberfuctions                ************
    //***********************************************************

    double calcMaxAcc(const double &T_max, const double &m, const double &g);

    double calcBrakingDistance(const double &vel, const double &a_max);

    double calcBrakeRatio(const double &h, const double &d_brake);

    double calcTargetVelocity(const double &a_max, const double &h, const double k_r);
};
