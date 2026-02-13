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
    try {
        controller = std::make_unique<simcontrol>(0);
        controller->initialize(jsonConfig);
    }
    catch (const std::exception& e)
    {
        qCritical() << "Simulation start failed: " << e.what();
        emit simulationError(QString::fromStdString(e.what()));
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
                      0.0,
                      SpacecraftState::Operational,
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
    collectControlCommands(percent);
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
    sendControlCommands();

    // signals
    emit stateUpdated(currentTime,
                      spacecraftData.statevector_.I_Position,
                      spacecraftData.statevector_.I_Velocity,
                      spacecraftData.GLoad,
                      spacecraftData.spacecraftState_,
                      spacecraftData.thrust,
                      spacecraftData.targetThrust,
                      spacecraftData.fuelMass,
                      spacecraftData.fuelFlow
                      );
}

void SimulationWorker::collectControlCommands(const double &thrustInPercentage, const double &thrustInNewton)
{
    qDebug() << "Thrust in Percentage: " << thrustInPercentage;
    qDebug() << "Thrust in Newton: " << thrustInNewton;
    FEControlCommands_.thrustInPercentage   = thrustInPercentage;
    FEControlCommands_.thrustInNewton       = thrustInNewton;
}

void SimulationWorker::sendControlCommands()
{
    controller->processCommands(&FEControlCommands_, nullptr);
}


