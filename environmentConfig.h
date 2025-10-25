#ifndef ENVIRONMENTCONFIG_H
#define ENVIRONMENTCONFIG_H

struct EnvironmentConfig {
    // Adjustable parameters/ inital parameters
    const double initialHeight    = 4000.0;  ///< [m], starting altitude
    const double initialVelocity  = 0.0;     ///< [m/s], starting speed

    // Constants
    const double moonGravity      = 1.635;   ///< [m/s²], Moon gravity
    const double earthGravity     = 9.81;    ///< [m/s2], earth gravity 

    // Simulation parameters
    int maxTimeStep         = 16;      ///< [ms], maximum simulation step (~60 Hz)
};



#endif