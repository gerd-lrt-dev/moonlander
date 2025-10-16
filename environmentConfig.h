#ifndef ENVIRONMENTCONFIG_H
#define ENVIRONMENTCONFIG_H

struct EnvironmentConfig {
    double initialHeight    = 4000.0;  ///< [m], starting altitude
    double initialVelocity  = 0.0;     ///< [m/s], starting speed
    double moonGravity      = 1.635;   ///< [m/s²], Moon gravity
    double earthGravity     = 9.81;    ///< [m/s2], earth gravity 
    int maxTimeStep         = 16;      ///< [ms], maximum simulation step (~60 Hz)
};



#endif