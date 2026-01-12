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
 * @brief This data will be return to UI by runSimulation
     * @param pos                   ///< [m] positions in three dimensions
     * @param vel                   ///< [m/s] velocity in three dimensions
     * @param acc                   ///< [s/s²] acceleration in three dimensions
     * @param spacecraftIntegrity   ///< [0/1] flag that represents spacecraft integrity
     * @param thrust                ///< [%] current thrust of spacecraft
     * @param targetThrust          ///< [%] target thrust of spacecraft
     * @param fuelMass              ///< [kg] current fuel mass of spacecraft
     * @param fuelFlow              ///< [kg/s] mass flow of fuel
 */
struct simData
{
    // Navigation data
    Vector3 pos;
    Vector3 vel;
    Vector3 acc;

    // Spacecraft Integrity
    bool spacecraftIntegrity;

    // Engine data
    double thrust;
    double targetThrust;

    // Fuel data
    double fuelMass;
    double fuelFlow;

};

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
    std::string jsonConfigString;                   ///< String with raw space config data provided by frontend
    customSpacecraft landerMoon1;                   ///< Config for used spacecraft provided by json config
    bool resetRequested;                            ///< Represents user desire to reset simulation

    // Inital data
    Vector3 initialVel;                             ///< [m/s] Initial velocity in three dimensions
    Vector3 initialPos;                             ///< [m] Initial positions in three dimensions
    Vector3 damageVec = {0.0, 0.0, 0.0};            ///< [m, m/s, m/s²] Sets every telemtry data to zero in case of hull defect
    double initialTime;                             ///< [s] Initial simulation time

    /**
     * @brief Runs the simulation until the lander reaches the surface.
     * @param vel0  ///< [m/s] Initial velocity in three dimensions 
     * @param pos0  ///< [m] Initial positions in three dimensions
     * @param t     ///< [s] Initial simulation time 
     *
     * Updates lander physics, calculates velocity and position, and
     * triggers output drawing for each time step.
     */
    void buildSimulationEnvironment(Vector3 vel0, Vector3 pos0, double t);

    /**
     * @brief Thread 1 which runs simulation loop (only used stand alone without UI)
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
     * Working jsonconfig together with backend!
     * Inactived when frontend is working!
     */
    customSpacecraft laodSpacecraftConfig(std::string filePath, std::string spacecraftName);

    /**
     * @brief Load json config out of string provided from frontend which defines spacecraft parameters
     * @param Filepath Path to config file
     *
     * Working jsonconfig together with frontend!
     */
    customSpacecraft loadSpacecraftFromJsonString(const std::string& jsonString, const std::string& spacecraftName);

public:
    /**
     * @brief Constructor
     * @param vel0  ///< [m/s] Initial velocity in three dimensions
     * @param pos0  ///< [m] Initial positions in three dimensions
     * @param t     ///< [s] Initial simulation time
     */
    simcontrol(Vector3 vel0, Vector3 pos0, double t0) : initialVel(vel0), initialPos(pos0), initialTime(t0) {};

    /**
     * @brief Initializes simulation environment and spacecraft config
     * @param jsonConfigStr ///< String with spacecraft config data
     */
    void initialize(const std::string& jsonConfigStr);

    /**
     * @brief Computes simulation due to timesteps given from simulation worker
     *
     * This function owns the states of simulation. It knows all physical, environmental and spacecraft conditions.
     * All states are calculated by given timesteps from worker.
     * @param dt
     */
    simData runSimulation(const double dt);

    /**
     * @brief Destructor
     *
     * Cleans up resources automatically via smart pointers.
     */
    ~simcontrol();

    /**
     * @brief set loaded json config str
     * @param jsonConfigStr with spacecraft configs
     *
     * transferd from qstring to std::string in frontend
     */
    void setJsonConfigStr(const std::string &jsonConfigStr);

    /**
     * @brief Sets target thrust
     * @param thrustPercent [%]
     */
    void setTargetThrust(double thrustPercent);

    /**
     * @brief Sets reset Boolean to true
     */
    void setResetBoolean();

    /**
     * @brief Instances the logging action and provides filepath for logging file
     */
    void instanceLoggingAction();
};

#endif
