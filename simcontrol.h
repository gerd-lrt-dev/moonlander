#ifndef SIMCONTROL_H
#define SIMCONTROL_H

#include "physics.h"
#include "output.h"
#include "spacecraft.h"
#include "environmentConfig.h"
#include "vector3.h"
#include "jsonConfigReader.h"

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
    std::unique_ptr<spacecraft> landerSpacecraft;   ///< Spacecraft with specs and integrity
    std::unique_ptr<physics> landerPhysics;         ///< Physics engine handling lander motion
    std::unique_ptr<output> drawer;                 ///< Output handler for cockpit display

    std::atomic<double> userThrustPercent;      
    EnvironmentConfig env;                          ///< Load environment config

    /**
     * @brief Runs the simulation until the lander reaches the surface.
     * @param vel0  ///< [m/s] Initial velocity in three dimensions 
     * @param pos0  ///< [m] Initial positions in three dimensions
     * @param t     ///< [s] Initial simulation time 
     *
     * Updates lander physics, calculates velocity and position, and
     * triggers output drawing for each time step.
     */
    void runSimulator(Vector3 vel0, Vector3 pos0, double t);

    /**
     * @brief Thread 1 which runs simulation loop
     * @param lander1IsIntact Is true, if spacecraft in an operational state
     * @param vel0  ///< [m/s] Initial velocities in three dimensions 
     * @param pos0  ///< [m] Initial position in three dimensions 
     * @param t0    ///< [s] Initial simulation time 
     */
    void runSimulationLoop(bool& lander1IsIntact, Vector3& vel0, Vector3& pos0, double& t0);

    /**
     * @brief Additional thread for controlling user input 
     */
    void runUserInput();

    /**
     * @brief Load json config which defines spacecraft parameters
     * @param Filepath Path to config file
     * 
     */
    customSpacecraft laodSpacecraftConfig(std::string filePath, std::string spacecraftName);

public:
    /**
     * @brief Constructor
     * @param v0    ///< [m/s] Initial velocity in three dimensions 
     * @param h0    ///< [m] Initial position in three dimensions 
     * @param t     ///< [s] Initial simulation time 
     *
     * Checks input parameters and starts the simulation.
     */
    simcontrol(Vector3 vel0, Vector3 pos0, double t0 = 0);

    /**
     * @brief Destructor
     *
     * Cleans up resources automatically via smart pointers.
     */
    ~simcontrol();
};

#endif
