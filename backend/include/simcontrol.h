#ifndef SIMCONTROL_H
#define SIMCONTROL_H

#include "spacecraft.h"
#include "simDataStruct.h"
#include "jsonConfigReader.h"
#include "Control/inputArbiter.h"

#include <memory>
#include <nlopt.h>

/**
 * @class simcontrol
 * @brief Central simulation orchestrator.
 *
 * SimControl is responsible for coordinating all high-level control flow of the simulation.
 * It does not perform physics calculations itself, but manages the interaction between
 * the major subsystems.
 *
 * Responsibilities:
 * - Collect and forward user input commands
 * - Query and apply automation / autopilot commands
 * - Resolve command priority (user vs. automation)
 * - Trigger and sequence simulation time steps
 * - Notify and update the UI / frontend state
 *
 * In short, SimControl decides *who controls the spacecraft* and *when the simulation advances*,
 * while the actual physics and state changes are handled by the spacecraft and its subsystems.
 *
 */
class simcontrol
{
private:
    //***********************************************************
    //*************        Members                   ************
    //***********************************************************
    std::unique_ptr<spacecraft> landerSpacecraft;   ///< Spacecraft with specs and integrity
    std::unique_ptr<InputArbiter> inputArbiter_;    ///< Arbiter for input commands
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
    void setTargetThrust(const double& thrustPercent = 0, const double& thrustInNewton = 0);

    /**
     * @brief Sets reset Boolean to true
     */
    void setResetBoolean();
};

#endif
