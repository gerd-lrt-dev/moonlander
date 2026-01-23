#include "Optimization/thrustOptimizer.h"
#include <nlopt.h>
#include <stdexcept>

extern double thrust_cost_function(unsigned, const double*, double*, void*);

std::vector<double> ThrustOptimizer::optimize(ThrustOptimizationProblem& problem, double T_max)
{
    nlopt_opt opt = nlopt_create(NLOPT_LN_COBYLA, problem.N);

    if (!opt)
    {
        throw std::runtime_error("NLopt create failed");
    }

    // Bounds
    std::vector<double> lb(problem.N, 0.0);
    std::vector<double> ub (problem.N, T_max);

    // Set nlopt bounds
    nlopt_set_lower_bounds(opt, lb.data());
    nlopt_set_upper_bounds(opt, ub.data());

    // Objective
    nlopt_set_min_objective(opt, thrust_cost_function, &problem);

    // Initial guess
    std::vector<double> u(problem.N, 0.5 * T_max);

    double minf;

    // optimize
    nlopt_result res = nlopt_optimize(opt, u.data(), &minf);

    nlopt_destroy(opt);

    if (res < 0)
    {
        throw std::runtime_error("Optimization failed");
    }

    return u;
}
