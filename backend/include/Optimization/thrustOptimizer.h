#pragma once
#include <vector>
#include "Optimization/thrustOptimizationProblem.h"

class ThrustOptimizer
{
public:
    std::vector<double> optimize(ThrustOptimizationProblem& problem, double T_Max);
};
