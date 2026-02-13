#include <Automation/adaptiveDescentController.h>
#include <cmath>
#include <iostream>

// ------------------------------------------------
// Public:
// ------------------------------------------------
double AdaptiveDescentController::setThrust(const double& thrust) const
{
    return thrust;
}

// ------------------------------------------------
// Private:
// ------------------------------------------------

double AdaptiveDescentController::calcMaxAcc(const double &T_max, const double &m, const double &g)
{
    const double epsilon = 1e-6;

    if (m <= 0.0)
    {
        return 0.0; // Physical illogical
    }

    double a_max = T_max / m - g;

    // In case of a_max is zero or beneath zero
    if (a_max < epsilon)
    {
        a_max = epsilon;
    }

    return a_max;
}

double AdaptiveDescentController::calcBrakingDistance(const double &vel, const double &a_max)
{
    if (a_max <= 0)
    {
        return 0.0;
    }

    return (vel *vel) / a_max; // d_brake
}

double AdaptiveDescentController::calcBrakeRatio(const double& h, const double &d_brake)
{
    // Epsilon preserves division by zero
    const double epsilon = 1e-6;

    return h / (d_brake + epsilon); // R_brake
}

double AdaptiveDescentController::calcTargetVelocity(const double &a_max, const double &h, const double k_r)
{
    if (k_r == 0)
    {
        std::cerr << "Safety factor k_r is zero! Resetted to one!" << std::endl;
        return - sqrt((2* a_max * h));
    }

    double v_target = - sqrt((2* a_max * h)/ k_r);

    return (v_target > v_conservative) ? v_target : v_conservative; // Minmal velocity is a conservative amount of safe velocity
}
