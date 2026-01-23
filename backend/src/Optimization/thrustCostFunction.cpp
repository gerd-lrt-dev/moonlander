#include <nlopt.h>
#include "Optimization/thrustOptimizationProblem.h"
#include "Integrators/Dynamics.h"

double thrust_cost_function(
    unsigned n,
    const double* u,
    double* /*grad*/,
    void* user_data
    )
{
    auto* problem = static_cast<ThrustOptimizationProblem*>(user_data);

    OptimizationState x = problem->x0;

    // Forward integration
    for (int k = 0; k < problem->N; ++k)
    {
        x = integrateEuler(
            x,
            u[k],
            problem->dt,
            problem->params
            );
    }

    // end costs
    double J =
        problem->w_h * x.h * x.h +
        problem->w_v * x.v * x.v +
        problem->w_m * (problem->x0.m - x.m);

    // running costs
    for (int k = 0; k < problem->N; ++k)
    {
        J += problem->w_T * u[k] * u[k];
    }

    return J;
}
