#ifndef SIMCONTROL_H
#define SIMCONTROL_H

#include "spacecraft.h"
#include "simDataStruct.h"
#include "jsonConfigReader.h"

#include <memory>
#include <nlopt.h>

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
    //***********************************************************
    //*************        Members                   ************
    //***********************************************************
    std::unique_ptr<spacecraft> landerSpacecraft;   ///< Spacecraft with specs and integrity
    std::string jsonConfigString;                   ///< String with raw space config data provided by frontend
    customSpacecraft landerMoon1;                   ///< Config for used spacecraft provided by json config
    bool resetRequested;                            ///< Represents user desire to reset simulation

    // Inital data
    double initialTime;                             ///< [s] Initial simulation time

    //***********************************************************
    //*************    Memberfuctions                ************
    //***********************************************************
    /**
     * @brief Build instances necessary for the simulation
     */
    void buildSimulationEnvironment(double t);

    /**
     * @brief Load json config out of string provided from frontend which defines spacecraft parameters
     * @param jsonString                            ///< String with config data
     * @param spacecraftName                        ///< Selected spacecraft from config data
     *
     * Working jsonconfig together with frontend!
     */
    customSpacecraft loadSpacecraftFromJsonString(const std::string& jsonString, const std::string& spacecraftName);

public:
    //***********************************************************
    //*************    Memberfuctions                ************
    //***********************************************************
    /**
     * @brief Constructor
     * @param t     ///< [s] Initial simulation time
     */
    simcontrol(double t0) : initialTime(t0) {};

    /**
     * @brief Destructor
     *
     * Cleans up resources automatically via smart pointers.
     */
    ~simcontrol();

    /**
     * @brief Initializes simulation environment and spacecraft config
     * @param jsonConfigStr                         ///< String with spacecraft config data
     */
    void initialize(const std::string& jsonConfigStr);

    /**
     * @brief Instances the logging action and provides filepath for logging file
     */
    void instanceLoggingAction();

    /**
     * @brief Computes simulation due to timesteps given from simulation worker
     *
     * This function owns the states of simulation. It knows all physical, environmental and spacecraft conditions.
     * All states are calculated by given timesteps from worker.
     * @param dt                                    ///< [s] discrete timestep
     */
    simData runSimulation(const double dt);

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
};

#endif
