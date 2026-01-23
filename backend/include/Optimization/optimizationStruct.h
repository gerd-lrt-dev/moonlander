#pragma once

struct OptimizationState
{
    double h;   ///< [m] Height vertical to moon center
    double v;   ///< [m/s²] Velocity vertical to moon center
    double m;   ///< [kg] Total mass spacecraft
};
