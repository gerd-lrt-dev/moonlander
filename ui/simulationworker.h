/**
 * @file simulationworker.h
 * @brief Worker object executing the lunar lander simulation in a separate thread.
 *
 * The SimulationWorker owns the simulation backend and performs
 * time-stepped simulation updates independently of the UI thread.
 */

#ifndef SIMULATIONWORKER_H
#define SIMULATIONWORKER_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QMutexLocker>

#include "simcontrol.h"

/**
 * @class SimulationWorker
 * @brief Executes the simulation loop in a worker thread.
 *
 * This class encapsulates the simulation backend and periodically
 * advances the simulation state. Updated state data is emitted via
 * Qt signals and consumed by the UI.
 */
class SimulationWorker : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief SimulationWorker construct the simulation worker.
     * @param parent Optional QObject parent
     */
    explicit SimulationWorker(QObject *parent = nullptr);

public slots:
    /**
     * @brief Starts the simulation loop.
     */
    void start();

    /**
     * @brief Pause the simulation loop.
     */
    void pause();

    /**
     * @brief Stops the simulation loop.
     */
    void stop();

    /**
     * @brief Sets the desired thrust lvl
     * @param percent Target thrust in percent
     */
    void setTargetThrust(double percent);

    /**
     * @brief Receives QString with json config data
     */
    void receiveJsonConfig(const QString &json);

    /**
     * @brief Sets autopilot to activated
     * @param Boolean active
     */
    void setAutopilotFlag(bool active);

signals:
    /**
     * @brief Emitted after each simulation step.
     *
     * @param time Simulation time [s]
     * @param altitude Current altitude [m]
     * @param vSpeed Vertical speed [m/s]
     * @param hSpeed Horizontal speed [m/s]
     * @param thrust Current thrust [%]
     * @param fuel Fuel mass [kg]
     * @param intact Hull integrity flag
     */
    void stateUpdated(double time,
                      Vector3 pos,
                      Vector3 vel,
                      double GLoad,
                      SpacecraftState spacecraftState_,
                      double thrust,
                      double targetThrust,
                      double fuelMass,
                      double fuelFlow
                      );

    /**
     * @brief simulationError
     * @param errorMsg
     */
    void simulationError(QString errorMsg);

public slots:
    /**
     * @brief Adavnces the simulation by one timestep.
     */
    void stepSimulation();

private:
     // ==========================
     // Internal State
     // ==========================
    simData spacecraftData;
    ControlCommand FEControlCommands_;
    std::unique_ptr<simcontrol> controller;

    std::string jsonConfig;     ///< String with spacecraft config data
    QTimer *simulationTimer;    ///< Drives simulation ticks
    bool running = false;       ///< Simulation running flag

    double currentTime = 0.0;   ///< Simulation time [s]

    QMutex mutex;               ///< Thread safety
    double requestedThrustPercent = 0.0; ///< Desired thrust in percentage

    // ==========================
    // Private Methods
    // ==========================

    /**
     * @brief Collects a user-issued control command from the frontend.
     *
     * This function stores the thrust command values received from the user.
     * Both parameters are optional and default to zero.
     *
     * @param thrustInPercentage Commanded thrust as a percentage of maximum thrust.
     * @param thrustInNewton     Commanded thrust in Newtons.
     */
    void collectControlCommands(const double &thrustInPercentage = 0.0, const double &thrustInNewton = 0.0);

    /**
     * @brief Collects an autopilot command from the automation system.
     *
     * This function updates the internal flag indicating whether the autopilot
     * should be active. Only the automation system should call this.
     *
     * @param autopilotActive True if the autopilot is engaged, false otherwise.
     */
    void collectAutopilotCommand(bool autopilotActive);

    /**
     * @brief Sends the currently collected control commands to the backend.
     *
     * This function forwards the stored user and/or autopilot commands to the
     * arbiter, which decides which commands are active. Ensures frontend cannot
     * directly manipulate automation commands.
     */
    void sendControlCommands();



};

#endif // SIMULATIONWORKER_H
