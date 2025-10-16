#ifndef SIMCONTROL_H
#define SIMCONTROL_H

#include "physics.h"
#include "output.h"
#include "spacecraft.h"
#include "environmentConfig.h"
#include "spacemath.h"

#include <memory>
#include <atomic>

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
    std::unique_ptr<physics> lander;                ///< Physics engine handling lander motion
    std::unique_ptr<output> drawer;                 ///< Output handler for cockpit display
    std::unique_ptr<spacecraft> landerSpacecraft;   ///< Spacecraft with specs and integrity

    std::atomic<double> userThrustPercent;
    EnvironmentConfig env;                          ///< Load environment config

    /**
     * @brief Runs the simulation until the lander reaches the surface.
     * @param v0 Initial velocity [m/s]
     * @param h0 Initial height [m]
     * @param t Initial simulation time [s]
     *
     * Updates lander physics, calculates velocity and position, and
     * triggers output drawing for each time step.
     */
    void runSimulator(double v0, double h0, double t);

    /**
     * @brief Thread 1 which runs simulation loop
     * @param lander1IsIntact Is true, if spacecraft in an operational state
     * @param v0 Initial velocity [m/s]
     * @param h0 Initial height [m]
     * @param t Initial simulation time [s]
     */
    void runSimulationLoop(bool& lander1IsIntact, double& v0, double& h0, double&t);

    /**
     * @brief Additional thread for controlling user input 
     */
    void runUserInput();

public:
    /**
     * @brief Constructor
     * @param v0 Initial velocity [m/s]
     * @param h0 Initial height [m]
     * @param t Initial simulation time [s]
     *
     * Checks input parameters and starts the simulation.
     */
    simcontrol(double v0, double h0, double t = 0);

    /**
     * @brief Destructor
     *
     * Cleans up resources automatically via smart pointers.
     */
    ~simcontrol();
};

#endif
