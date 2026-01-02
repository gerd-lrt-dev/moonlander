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

signals:
    /*
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
                      double altitude,
                      double vSpeed,
                      double hSpeed,
                      double thrust,
                      double fuel,
                      bool intact);

public slots:
    /**
     * @brief Adavnces the simulation by one timestep.
     */
    void stepSimulation();

private:
     // ==========================
     // Internal State
     // ==========================
    QTimer *simulationTimer;    ///< Drives simulation ticks
    bool running = false;       ///< Simulation running flag

    double currentTime = 0.0;   ///< Simulation time [s]
};

#endif // SIMULATIONWORKER_H
