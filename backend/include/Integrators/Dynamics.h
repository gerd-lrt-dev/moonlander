#pragma once
#include "Optimization/optimizationStruct.h"
#include "Optimization/modelParams.h"

OptimizationState integrateEuler
(
    const OptimizationState& x,
    double thrust,
    double dt,
    const OptimizationModelParams& params

);
