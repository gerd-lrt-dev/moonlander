#include "Optimization/thrustOptimizer.h"
#include <stdexcept>
#include <iostream>

extern double thrustCostFunction(unsigned, const double*, double*, void*);

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
    nlopt_set_min_objective(opt, thrustCostFunction, &problem);

    // Initial guess
    std::vector<double> u(problem.N, 50.0);

    double minf;

    // optimize
    nlopt_result res = nlopt_optimize(opt, u.data(), &minf);

    std::cerr << "[Optimizer] nlopt_result = " << res << "\n";
    std::cerr << "[Optimizer] minf        = " << minf << "\n";

    std::cerr << "[Optimizer] nlopt_result = "
              << nloptResultToString(res) << "\n";


    nlopt_destroy(opt);

    if (res < 0)
    {
        throw std::runtime_error("Optimization failed");
    }

    return u;
}

const char* ThrustOptimizer::nloptResultToString(nlopt_result r)
{
    switch (r) {
    case NLOPT_SUCCESS: return "SUCCESS";
    case NLOPT_STOPVAL_REACHED: return "STOPVAL_REACHED";
    case NLOPT_FTOL_REACHED: return "FTOL_REACHED";
    case NLOPT_XTOL_REACHED: return "XTOL_REACHED";
    case NLOPT_MAXEVAL_REACHED: return "MAXEVAL_REACHED";
    case NLOPT_MAXTIME_REACHED: return "MAXTIME_REACHED";
    case NLOPT_FAILURE: return "FAILURE";
    case NLOPT_INVALID_ARGS: return "INVALID_ARGS";
    case NLOPT_OUT_OF_MEMORY: return "OUT_OF_MEMORY";
    case NLOPT_ROUNDOFF_LIMITED: return "ROUNDOFF_LIMITED";
    case NLOPT_FORCED_STOP: return "FORCED_STOP";
    default: return "UNKNOWN";
    }
}
