#include "homepage.h"
#include "uibuilder.h"

Homepage::Homepage(QWidget *parent) :
    QMainWindow(parent)
{
    jsonConfigStr = loadJsonResource(":/configs/Resources/configs/lander.json");
    setupStackedWidget();
    setupThread();
    setupConnections();

}

Homepage::~Homepage()
{
    simulationThread->quit();
    simulationThread->wait();
}

QString Homepage::loadJsonResource(const QString& path)
{
    // 1) Check if file exists
    bool exists = QFile::exists(path);
    qDebug() << "JSON resource exists:" << exists << "| path:" << path;

    if (!exists)
    {
        qCritical() << "JSON resource NOT found:" << path;
        return {};
    }

    // 2) open file
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical() << "Could not open JSON resource:" << path
                    << "| error:" << file.errorString();
        return {};
    }

    // 3) read content
    QByteArray data = file.readAll();
    if (data.isEmpty())
    {
        qCritical() << "JSON resource is empty:" << path;
        return {};
    }

    qDebug() << "JSON resource loaded successfully, size:"
             << data.size() << "bytes";

    return QString::fromUtf8(data);
}


void Homepage::setupStackedWidget()
{
    // Build central QWidget
    centralWidget = new QWidget(this);

    // Build vertical box layout
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Build stackedWidget for pages
    stackedWidget = new QStackedWidget(this);

    // Create pages
    QWidget *homepage = createHomePage(stackedWidget);
    cockpit = new cockpitPage(this);

    // Add pages to stacked widget
    stackedWidget->addWidget(homepage);
    stackedWidget->addWidget(cockpit);

    // Add stackedwidget to Layout
    layout->addWidget(stackedWidget);

    // Set central page
    setCentralWidget(centralWidget);
}

QWidget* Homepage::createHomePage(QStackedWidget *stackedWidget)
{
    //Create Widget
    QWidget *homepage = new QWidget(this);

    // Build instance for ui building class
    UIBuilder uiBuilder;

    // Initialize Layout
    vHomepageLayout = new QVBoxLayout(homepage);

    // Add title
    QLabel *title = uiBuilder.createPageTitle("Lunar Lander Simulation", this);

    // Add logo
    QLabel *logoLabel   = new QLabel(this);
    QPixmap logoPixmap(":/new/logo/Resources/Images/logo.png");
    logoPixmap = logoPixmap.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logoLabel->setPixmap(logoPixmap);
    logoLabel->setAlignment(Qt::AlignCenter);

    // Add copryright
    QLabel *copyright = uiBuilder.createCopyright("© gerd-lrt-dev", this);

    // Add Buttons
    selectSpacecraftButton = uiBuilder.createButton("Select Spacecraft", this);
    startSimulationButton = uiBuilder.createButton("Start Simulation", this);

    // Add widgets to Layout
    vHomepageLayout->addSpacing(30);
    vHomepageLayout->addWidget(title);
    vHomepageLayout->addSpacing(15);
    vHomepageLayout->addWidget(logoLabel);
    vHomepageLayout->addSpacing(15);
    vHomepageLayout->addWidget(copyright);
    vHomepageLayout->addSpacing(15);
    vHomepageLayout->addWidget(selectSpacecraftButton, 0, Qt::AlignHCenter);
    vHomepageLayout->addSpacing(15);
    vHomepageLayout->addWidget(startSimulationButton, 0, Qt::AlignHCenter);
    vHomepageLayout->setAlignment(Qt::AlignTop);
    vHomepageLayout->setAlignment(Qt::AlignCenter);

    return homepage;
}

void Homepage::setupConnections()
{
    connect(startSimulationButton, &QPushButton::clicked, this, [this]
            {
                stackedWidget->setCurrentWidget(cockpit);
            });

    connect(cockpit, &cockpitPage::startRequested, simulationWorker, &SimulationWorker::start);

    // Connect thrust input from user with backend
    connect(cockpit, &cockpitPage::thrustTargetRequested, simulationWorker, &SimulationWorker::setTargetThrust);

    connect(cockpit, &cockpitPage::autopilotToggled, simulationWorker, &SimulationWorker::setAutopilotFlag);

}

void Homepage::setupThread()
{
    // Instance thread
    simulationThread = new QThread(this);

    // Instance class
    simulationWorker = new SimulationWorker();

    simulationWorker->moveToThread(simulationThread);

    // Build connections
    connect(simulationThread, &QThread::finished, simulationWorker, &QObject::deleteLater);

    connect(this, &QObject::destroyed, simulationThread, &QThread::quit);

    connect(cockpit, &cockpitPage::startRequested,
            simulationWorker, &SimulationWorker::start);

    connect(cockpit, &cockpitPage::pauseRequested,
            simulationWorker, &SimulationWorker::pause);

    connect(cockpit, &cockpitPage::stopConfirmed,
            simulationWorker, &SimulationWorker::stop);

    connect(this, &Homepage::sendJsonToWorker, simulationWorker,
            &SimulationWorker::receiveJsonConfig, Qt::QueuedConnection);

    connect(simulationWorker, &SimulationWorker::stateUpdated,
            cockpit, &cockpitPage::onStateUpdated);


    // Start simulation
    simulationThread->start();

    emit sendJsonToWorker(jsonConfigStr);
}

