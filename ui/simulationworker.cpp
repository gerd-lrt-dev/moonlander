#include "simulationworker.h"

SimulationWorker::SimulationWorker(QObject *parent)
    : QObject(parent)
{
    // Build timer event
    simulationTimer = new QTimer(this);
    simulationTimer->setInterval(50); // 20 Hz

    // connect timer with worker function
    connect(simulationTimer, &QTimer::timeout, this, &SimulationWorker::stepSimulation);
}

void SimulationWorker::start()
{
    // return if simulation does not run (default of running is false)
    if (running)
        return;
    qDebug("simulationworker[constructor]: Simulation start button pushed");
}

void SimulationWorker::pause()
{
    // TODO: Build later
}

void SimulationWorker::stop()
{
    running = false;
    simulationTimer->stop();
}

void SimulationWorker::setTargetThrust(double percent)
{
    // TODO: Build later
    Q_UNUSED(percent);
}

void SimulationWorker::stepSimulation()
{
    if(!running)
        return;

    // =====================================
    // TODO: Backend anbinden
    // =====================================
    currentTime += 0.05;

    double altitude = 1000.0 - currentTime * 10.0;
    double vSpeed   = -10.0;
    double hSpeed   = 0.0;
    double thrust   = 50.0;
    double fuel     = 500.0;
    bool   intact   = true;

    emit stateUpdated(currentTime,
                      altitude,
                      vSpeed,
                      hSpeed,
                      thrust,
                      fuel,
                      intact);

}
