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

    std::cout << "v_target: " << v_target << std::endl;

    double T_hover = calcHoverThrust(m, g);

    std::cout << "T_hover: " << T_hover << std::endl;

    double a_cmd_ctrl = useController->control(v_target, vel, K_p, K_d, dt);

    double T_cmd_ctrl = calcThrustCommand(T_hover, a_cmd_ctrl, m);

    //std::cout << "T_cmd_ctrl: " << T_cmd_ctrl << std::endl;

    double T_cmd_Saturated = calcSaturation(T_cmd_ctrl, T_max);

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

    double R_brake = 0.0;

    if (h > 0)
    {
        R_brake = h / (d_brake + epsilon);
    }
    else
    {
        R_brake = 0.0;
    }

    //std::cout << "R_brake: " << R_brake << std::endl;

    return R_brake; // R_brake
}

double AdaptiveDescentController::calcTargetVelocity(const double &a_max, const double &h, const double k_r, const double &vel, const double &dt) const
{
    double term = 2 * k_r * a_max * h;

    if (term > 0)
    {
        return -sqrt(term);
    }
    else
    {
        return 0.0;
    }
}

double AdaptiveDescentController::calcHoverThrust(const double &m, const double &g) const
{
    return m * g;
}

double AdaptiveDescentController::calcHoverThrottle(const double &m, const double &g, const double &T_max) const
{
    return (m * g) / T_max;
}

double AdaptiveDescentController::calcThrustCommand(const double &T_hover, const double &a_controlled, const double &m) const
{
    double T_cmd = T_hover + (a_controlled * m);

    std::cout << "Tcmd ohne hover: " << a_controlled / m << std::endl;

    if (T_cmd > 0)
    {
        return T_cmd;
    }
    else
    {
        return 0.0;
    }
}

double AdaptiveDescentController::calcSaturation(const double &T_cmd, const double &T_max) const
{
    std::cout << "T_max: " << T_max << std::endl;
    double T_cmdPreSaturation = (T_cmd > 0) ? T_cmd : 0;

    return (T_cmdPreSaturation < T_max) ? T_cmdPreSaturation : T_max;
}

double AdaptiveDescentController::calcNormalizedThrust(const double &T_cmd, const double &T_max) const
{
    std::cout << "[CONTROLLER]: T_cmd - " << T_cmd << std::endl;
    std::cout << "[CONTROLLER]: T_max - " << T_max << std::endl;
    return T_cmd / T_max;
}

double AdaptiveDescentController::interpolate_k_r(double R_brake) const
{
    // Define typical k_r per mode
    constexpr double kA = 0.25; // Energy Dissipation
    constexpr double kB = 0.15; // Controlled Descent
    constexpr double kC = 0.05; // Terminal Approach
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

/**
 * @brief Interpolates the proportional gain Kp based on the brake ratio.
 *
 * The gain increases smoothly for large R_brake, ensuring sufficient control authority
 * at the start of descent while remaining moderate during terminal braking.
 *
 * @param R_brake The brake ratio (h / d_brake)
 * @return The proportional gain Kp
 */
double AdaptiveDescentController::interpolate_Kp(double R_brake) const
{
    constexpr double Kp_min = 0.8;   // minimal gain (far from ground)
    constexpr double Kp_max = 50.0;  // maximal gain (start of descent)
    constexpr double R_ref  = 3.0;   // reference ratio where min gain applies

    if (R_brake <= R_ref)
        return Kp_min;

    // Linear scaling beyond R_ref
    double scale = 0.1; // adjust to tune aggressiveness
    double Kp = Kp_min + (R_brake - R_ref) * scale;

    // Clamp to Kp_max
    if (Kp > Kp_max) Kp = Kp_max;

    return Kp;
}

/**
 * @brief Interpolates the derivative gain Kd based on the brake ratio.
 *
 * The derivative gain also scales with R_brake to damp velocity error
 * at high altitude while staying small during terminal descent.
 *
 * @param R_brake The brake ratio (h / d_brake)
 * @return The derivative gain Kd
 */
double AdaptiveDescentController::interpolate_Kd(double R_brake) const
{
    constexpr double Kd_min = 0.05;   // minimal gain (terminal phase)
    constexpr double Kd_max = 10.0;   // maximal gain (start of descent)
    constexpr double R_ref  = 3.0;    // reference ratio where min gain applies

    if (R_brake <= R_ref)
        return Kd_min;

    double scale = 0.05; // adjust to tune damping
    double Kd = Kd_min + (R_brake - R_ref) * scale;

    if (Kd > Kd_max) Kd = Kd_max;

    return Kd;
}


