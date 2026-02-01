#pragma once
#include "Optimization/thrustOptimizationProblem.h"

/// @brief Forward Euler integrator for a simple vertical dynamics system.
OptimizationState integrateEuler1D(
    const OptimizationState& x,           // current state
    double T,                             // thrust
    double dt,                            // time step
    const ThrustOptimizationProblem& problem
    );
