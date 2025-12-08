#include "homepage.h"
#include "uibuilder.h"

Homepage::Homepage(QWidget *parent) :
    QWidget(parent)
{
    setupUI();
}

void Homepage::setupUI()
{
    // Initialize Layout
    vHomepageLayout = new QVBoxLayout(this);

    // Build instance for ui building class
    UIBuilder uiBuilder;

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
}

void Homepage::setupConnections()
{
    connect(startSimulationButton, &QPushButton::clicked, this, [this]
    {
        qDebug() << "Homepage[setupUI]: Emitting signal requestStartOfSimulation...";
        emit requestStartOfSimulation();
    });
}

