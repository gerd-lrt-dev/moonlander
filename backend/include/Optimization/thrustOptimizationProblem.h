#pragma once
#include "Optimization/optimizationStruct.h"
#include "Optimization/modelParams.h"

struct ThrustOptimizationProblem
{
    OptimizationState x0;
    OptimizationModelParams params;

    double dt;
    int N;

    // cost weights
    double w_h;
    double w_v;
    double w_m;
    double w_T;
};
