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
    running = false;
    simulationTimer->stop();
}

void SimulationWorker::stop()
{
    running = false;
    simulationTimer->stop();
    currentTime = 0.0;

    emit stateUpdated(currentTime,
                      {0.0, 0.0, 0.0},
                      {0.0, 0.0, 0.0},
                      {0.0, 0.0, 0.0},
                      1.0,
                      0.0,
                      0.0,
                      0.0,
                      0.0
                      );

    controller->setResetBoolean();
}

void SimulationWorker::receiveJsonConfig(const QString &json)
{
    this->jsonConfig = json.toStdString();

    qDebug() << "SimulationWorker received JSON config, size:"
             << jsonConfig.size();
}

void SimulationWorker::setTargetThrust(double percent)
{
    QMutexLocker locker(&mutex);  // protect access
    requestedThrustPercent = percent;
}

void SimulationWorker::stepSimulation()
{
    double dt = 0.00;   ///< Initialized discrete timestep

    // Return if not running
    if(!running)
        return;

    // times
    dt = 0.05; // TODO: should specified in json as well
    currentTime += dt;

    // Calling backend simulator
    spacecraftData = controller->runSimulation(dt);

    // Withdraw user input due to thrust
    controller->setTargetThrust(requestedThrustPercent);

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


