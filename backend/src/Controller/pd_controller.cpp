#include "Controller/pd_controller.h"
#include <iostream>

// ------------------------------------------------
// Public:
// ------------------------------------------------
double PD_Controller::control(const double &targetValue, const double &measuredValue, const double &K_p, const double &K_d, const double &dt) const
{
    double error        = calcError(targetValue, measuredValue);

    double differential = calcDifferential(error, error_old_, dt);

    double P_term = error * K_p;

    double D_term = differential * K_d;

    double controlValue = P_term + D_term;

    std::cout << "P_term: " << P_term << std::endl;
    std::cout << "D_term: " << D_term << std::endl;

    return controlValue;
}

// ------------------------------------------------
// Private:
// ------------------------------------------------
double PD_Controller::calcError(const double &targetValue, const double &measureValue) const
{
    return targetValue - measureValue;
}

double PD_Controller::calcDifferential(const double &error, const double &error_old, const double &dt) const
{
    double differential = (error - error_old) / dt;

    error_old_ = error;

    return differential;
}
