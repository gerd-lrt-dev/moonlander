#pragma once
#include <vector>
#include <nlopt.h>
#include "Optimization/thrustOptimizationProblem.h"

class ThrustOptimizer
{
private:
    static const char* nloptResultToString(nlopt_result r);
public:
    std::vector<double> optimize(ThrustOptimizationProblem& problem, double T_Max);
};
