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

    // Initialize the controller with:
    // v0 = 0 m/s, h0 = 3200 m, t0 = 0 s
    try {
            controller = std::make_unique<simcontrol>(Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, 3200.0), 0);
            //controller->instanceLoggingAction();
            controller->initialize(jsonConfig);
        }
    catch (const std::exception& e)
    {
        qCritical() << "Simulation start failed: " << e.what();
            emit simulationError(QString::fromStdString(e.what()));
        return;
    }

    running = true;
    simulationTimer->start();

    qDebug("simulationworker[constructor]: Simulation started successfully");
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

void SimulationWorker::receiveJsonConfig(const QString &json)
{
    this->jsonConfig = json.toStdString();

    qDebug() << "SimulationWorker received JSON config, size:"
             << jsonConfig.size();
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

    // times
    double dt = 0.05;
    currentTime += dt;

    // simulator
    simData spacecraftData = controller->runSimulation(dt);

    // signals
    emit stateUpdated(currentTime,
                      spacecraftData.pos,
                      spacecraftData.vel,
                      spacecraftData.acc,
                      spacecraftData.spacecraftIntegrity,
                      spacecraftData.thrust,
                      spacecraftData.targetThrust,
                      spacecraftData.fuelMass,
                      spacecraftData.fuelFlow
                      );
}
