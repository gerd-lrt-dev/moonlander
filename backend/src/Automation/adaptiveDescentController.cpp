#include <Automation/adaptiveDescentController.h>
#include <cmath>
#include <iostream>

#include "Controller/iController.h"

// ------------------------------------------------
// Public:
// ------------------------------------------------
double AdaptiveDescentController::setAutoThrustInNewton(IController *useController, const double &T_max, const double &vel, const double &h, const double &dt, const double &m, const double &g) const
{

    if (!useController)
    {
        std::cerr << "Controller pointer is null !" << std::endl;
        return calcHoverThrust(m, g);
    }

    double T_cmd = 0.0;

    double a_max = calcMaxAcc(T_max, m, g);

    double d_brake = calcBrakingDistance(vel, a_max);

    double R_brake = calcBrakeRatio(h,d_brake);

    double k_r = interpolate_k_r(R_brake);

    double K_p = interpolate_Kp(R_brake);

    double K_d = interpolate_Kd(R_brake);

    double v_target = calcTargetVelocity(a_max, h, k_r, vel, dt);

    double T_hover = calcHoverThrust(m, g);

    double T_cmd_ctrl = - useController->control(v_target, vel, K_p, K_d, dt);

    double T_cmd_row = T_cmd_ctrl;

    double T_cmd_Saturated = calcSaturation(T_cmd_row, T_max);

    T_cmd = T_cmd_Saturated;

    return T_cmd;
}

double AdaptiveDescentController::normalizAutoThrust(const double &thrustInNewton, const double &T_max) const
{
    return calcNormalizedThrust(thrustInNewton, T_max);
}

// ------------------------------------------------
// Private:
// ------------------------------------------------

double AdaptiveDescentController::calcMaxAcc(const double &T_max, const double &m, const double &g) const
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

double AdaptiveDescentController::calcBrakingDistance(const double &vel, const double &a_max) const
{
    if (a_max <= 0)
    {
        return 0.0;
    }

    return (vel *vel) / (2* a_max); // d_brake
}

double AdaptiveDescentController::calcBrakeRatio(const double& h, const double &d_brake) const
{
    // Epsilon preserves division by zero
    const double epsilon = 1e-6;

    return h / (d_brake + epsilon); // R_brake
}

double AdaptiveDescentController::calcTargetVelocity(
    const double &a_max,    // maximale Bremsbeschleunigung [m/s²]
    const double &h,        // Höhe über der Mondoberfläche [m]
    const double k_r,       // Reservefaktor (>0 = sanfter)
    const double &vel,      // aktuelle Geschwindigkeit (negativ = fallend)
    const double &dt        // Zeitschritt [s]
    ) const
{
    return 0.0;
}



double AdaptiveDescentController::calcHoverThrust(const double &m, const double &g) const
{
    return m * g;
}

double AdaptiveDescentController::calcHoverThrottle(const double &m, const double &g, const double &T_max) const
{
    return (m * g) / T_max;
}

double AdaptiveDescentController::calcSaturation(const double &T_cmd, const double &T_max) const
{
    double T_cmdPreSaturation = (T_cmd > 0) ? T_cmd : 0;

    return (T_cmdPreSaturation < T_max) ? T_cmdPreSaturation : T_max;
}

double AdaptiveDescentController::calcNormalizedThrust(const double &T_cmd, const double &T_max) const
{
    return T_cmd / T_max;
}

double AdaptiveDescentController::interpolate_k_r(double R_brake) const
{
    // Define typical k_r per mode
    constexpr double kA = 1.2; // Energy Dissipation
    constexpr double kB = 1.5; // Controlled Descent
    constexpr double kC = 2.0; // Terminal Approach
    constexpr double kD = 2.5; // Critical Braking

    if (R_brake >= 3.0) return kA;
    if (R_brake >= 1.5) // blend B–A
    {
        double alpha = (R_brake - 1.5) / (3.0 - 1.5); // 0..1
        return kB * (1.0 - alpha) + kA * alpha;
    }
    if (R_brake >= 1.0) // blend C–B
    {
        double alpha = (R_brake - 1.0) / (1.5 - 1.0); // 0..1
        return kC * (1.0 - alpha) + kB * alpha;
    }
    // R_brake < 1 → Critical
    return kD;
}

double AdaptiveDescentController::interpolate_Kp(double R_brake) const
{
    constexpr double KpA = 0.8;
    constexpr double KpB = 1.2;
    constexpr double KpC = 2.0;
    constexpr double KpD = 3.0;

    if (R_brake >= 3.0) return KpA;
    if (R_brake >= 1.5) {
        double alpha = (R_brake - 1.5) / (3.0 - 1.5);
        return KpB * (1.0 - alpha) + KpA * alpha;
    }
    if (R_brake >= 1.0) {
        double alpha = (R_brake - 1.0) / (1.5 - 1.0);
        return KpC * (1.0 - alpha) + KpB * alpha;
    }
    return KpD;
}

double AdaptiveDescentController::interpolate_Kd(double R_brake) const
{
    constexpr double KdA = 0.05;
    constexpr double KdB = 0.1;
    constexpr double KdC = 0.2;
    constexpr double KdD = 0.3;

    if (R_brake >= 3.0) return KdA;
    if (R_brake >= 1.5) {
        double alpha = (R_brake - 1.5) / (3.0 - 1.5);
        return KdB * (1.0 - alpha) + KdA * alpha;
    }
    if (R_brake >= 1.0) {
        double alpha = (R_brake - 1.0) / (1.5 - 1.0);
        return KdC * (1.0 - alpha) + KdB * alpha;
    }
    return KdD;
}

