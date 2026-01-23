#include "Integrators/Dynamics.h"

OptimizationState integrateEuler(const OptimizationState& x, double T, double dt, const OptimizationModelParams& p)
{
    OptimizationState xn = x;

    xn.h += x.v * dt;
    xn.v += (T / x.m - p.g) * dt;
    xn.m -= p.alpha * T * dt;

    return xn;
}
