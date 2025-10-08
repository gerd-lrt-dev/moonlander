#ifndef SIMCONTROL_H
#define SIMCONTROL_H

#include "physics.h"
#include "output.h"
#include <memory>

/**
 * @class simcontrol
 * @brief Main controller for the lunar lander simulation.
 *
 * This class coordinates the simulation loop, updating the physics
 * of the lander and handling output/visualization. It owns the
 * Physics and Output objects via smart pointers to ensure automatic
 * memory management.
 */
class simcontrol
{
private:
    std::unique_ptr<physics> lander; ///< Physics engine handling lander motion
    std::unique_ptr<output> drawer;  ///< Output handler for cockpit display

    /**
     * @brief Runs the simulation until the lander reaches the surface.
     * @param v1 Initial velocity [m/s]
     * @param h1 Initial height [m]
     * @param t Initial simulation time [s]
     *
     * Updates lander physics, calculates velocity and position, and
     * triggers output drawing for each time step.
     */
    void runSimulator(double v1, double h1, double t);

public:
    /**
     * @brief Constructor
     * @param v1 Initial velocity [m/s]
     * @param h1 Initial height [m]
     * @param t Initial simulation time [s]
     *
     * Checks input parameters and starts the simulation.
     */
    simcontrol(double v1, double h1, double t);

    /**
     * @brief Destructor
     *
     * Cleans up resources automatically via smart pointers.
     */
    ~simcontrol();
};

#endif
