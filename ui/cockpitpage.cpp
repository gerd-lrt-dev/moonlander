#include "cockpitpage.h"

#include <QGridLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>

// ------------------------------------------------
// Constructor
// ------------------------------------------------
cockpitPage::cockpitPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupConnections();
}

// ------------------------------------------------
// UI Setup
// ------------------------------------------------
void cockpitPage::setupUI()
{
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // Equal vertical split
    mainLayout->setRowStretch(0, 1);
    mainLayout->setRowStretch(1, 1);

    // Column balance
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(1, 1);
    mainLayout->setColumnStretch(2, 1);

    // ===== Global Style =====
    setStyleSheet(
        "QWidget { background-color: #0E1624; color: #D6E1F0; }"

        "QGroupBox { "
        "border: 1px solid #2F4A72; "
        "border-radius: 8px; "
        "margin-top: 22px; "
        "padding-top: 18px; "
        "font-weight: bold; "
        "}"

        "QGroupBox::title { "
        "subcontrol-origin: margin; "
        "subcontrol-position: top left; "
        "left: 12px; "
        "top: 6px; "
        "padding: 0 6px; "
        "color: #4FC3F7; "
        "}"

        "QLCDNumber { "
        "background-color: #000000; "
        "color: #4FC3F7; "
        "border: 2px solid #2F4A72; "
        "border-radius: 4px; "
        "min-height: 26px; "
        "}"
        );

    // ================= NAV =================
    QGroupBox *navBox = new QGroupBox("NAV");
    QGridLayout *navLayout = new QGridLayout(navBox);

    lcdTime     = new QLCDNumber();
    lcdAltitude = new QLCDNumber();
    lcdVSpeed   = new QLCDNumber();
    lcdHSpeed   = new QLCDNumber();

    navLayout->addWidget(new QLabel("Time [s]"),      0, 0);
    navLayout->addWidget(lcdTime,                     0, 1);
    navLayout->addWidget(new QLabel("Altitude [m]"),  1, 0);
    navLayout->addWidget(lcdAltitude,                 1, 1);
    navLayout->addWidget(new QLabel("V-Speed [m/s]"), 2, 0);
    navLayout->addWidget(lcdVSpeed,                   2, 1);
    navLayout->addWidget(new QLabel("H-Speed [m/s]"), 3, 0);
    navLayout->addWidget(lcdHSpeed,                   3, 1);

    // ================= ENGINE =================
    QGroupBox *engineBox = new QGroupBox("ENGINE");
    QGridLayout *engineLayout = new QGridLayout(engineBox);

    lcdThrust       = new QLCDNumber();
    lcdTargetThrust = new QLCDNumber();
    lcdAcceleration = new QLCDNumber();

    engineLayout->addWidget(new QLabel("Thrust [%]"),        0, 0);
    engineLayout->addWidget(lcdThrust,                       0, 1);
    engineLayout->addWidget(new QLabel("Target Thrust [%]"), 1, 0);
    engineLayout->addWidget(lcdTargetThrust,                 1, 1);
    engineLayout->addWidget(new QLabel("Accel [m/s²]"),      2, 0);
    engineLayout->addWidget(lcdAcceleration,                 2, 1);

    // ================= FUEL =================
    QGroupBox *fuelBox = new QGroupBox("FUEL");
    QGridLayout *fuelLayout = new QGridLayout(fuelBox);

    lcdFuelMass = new QLCDNumber();
    lcdFuelFlow = new QLCDNumber();

    fuelLayout->addWidget(new QLabel("Fuel Mass [kg]"),   0, 0);
    fuelLayout->addWidget(lcdFuelMass,                    0, 1);
    fuelLayout->addWidget(new QLabel("Fuel Flow [kg/s]"), 1, 0);
    fuelLayout->addWidget(lcdFuelFlow,                    1, 1);

    // ================= STATUS =================
    QGroupBox *statusBox = new QGroupBox("STATUS");
    QVBoxLayout *statusLayout = new QVBoxLayout(statusBox);

    lblHullStatus = new QLabel("HULL: OK");
    lblHullStatus->setStyleSheet("color: lime; font-weight: bold;");
    statusLayout->addWidget(lblHullStatus);
    statusLayout->addStretch();

    // ================= LANDING VIEW =================
    QGroupBox *landingBox = new QGroupBox("LANDING VIEW");
    QVBoxLayout *landingLayout = new QVBoxLayout(landingBox);
    landingLayout->setSpacing(8);

    // --- Landing View ---
    landingView = new LandingView(this);
    landingView->setMinimumSize(240, 180);
    landingView->setStyleSheet(
        "background-color: #060B14;"
        "border: 1px solid #2F4A72;"
        "border-radius: 4px;"
        );

    landingLayout->addWidget(landingView, 1);

    // --- Simulation Controls ---
    QHBoxLayout *simControlLayout = new QHBoxLayout();
    simControlLayout->setSpacing(6);

    btnSimStart = new QPushButton("START");
    btnSimPause = new QPushButton("PAUSE");
    btnSimStop  = new QPushButton("STOP");

    btnSimStart->setStyleSheet(
        "QPushButton { background-color: #1B5E20; color: white; "
        "border-radius: 4px; padding: 6px; font-weight: bold; }"
        "QPushButton:hover { background-color: #2E7D32; }"
        );

    btnSimPause->setStyleSheet(
        "QPushButton { background-color: #37474F; color: white; "
        "border-radius: 4px; padding: 6px; font-weight: bold; }"
        "QPushButton:hover { background-color: #455A64; }"
        );

    btnSimStop->setStyleSheet(
        "QPushButton { background-color: #7F1D1D; color: white; "
        "border-radius: 4px; padding: 6px; font-weight: bold; }"
        "QPushButton:hover { background-color: #B71C1C; }"
        );

    simControlLayout->addWidget(btnSimStart);
    simControlLayout->addWidget(btnSimPause);
    simControlLayout->addWidget(btnSimStop);

    landingLayout->addLayout(simControlLayout);

    // ================= GRID ASSEMBLY =================
    mainLayout->addWidget(navBox,     0, 0);
    mainLayout->addWidget(fuelBox,    1, 0);

    mainLayout->addWidget(landingBox, 0, 1, 2, 1); // spans two rows

    mainLayout->addWidget(engineBox,  0, 2);
    mainLayout->addWidget(statusBox,  1, 2);

    setLayout(mainLayout);
}

// ------------------------------------------------
// Connections
// ------------------------------------------------
void cockpitPage::setupConnections()
{
    connect(btnSimStart, &QPushButton::clicked,
            this, &cockpitPage::startRequested);

    connect(btnSimPause, &QPushButton::clicked,
            this, &cockpitPage::pauseRequested);

    connect(btnSimStop, &QPushButton::clicked,
            this, &cockpitPage::stopRequested);

}

// ------------------------------------------------
// Update Interface
// ------------------------------------------------
void cockpitPage::updateTime(double t)                { lcdTime->display(t); }
void cockpitPage::updateAltitude(double altitude)     { lcdAltitude->display(altitude); }
void cockpitPage::updateVerticalVelocity(double v)    { lcdVSpeed->display(v); }
void cockpitPage::updateHorizontalVelocity(double h)  { lcdHSpeed->display(h); }
void cockpitPage::updateAcceleration(double a)        { lcdAcceleration->display(a); }
void cockpitPage::updateThrust(double t)              { lcdThrust->display(t); }
void cockpitPage::updateTargetThrust(double t)        { lcdTargetThrust->display(t); }
void cockpitPage::updateFuelMass(double f)            { lcdFuelMass->display(f); }
void cockpitPage::updateFuelFlow(double f)            { lcdFuelFlow->display(f); }

void cockpitPage::updateHullStatus(bool intact)
{
    lblHullStatus->setText(intact ? "HULL: OK" : "HULL: DAMAGED");
    lblHullStatus->setStyleSheet(
        intact
            ? "color: lime; font-weight: bold;"
            : "color: red; font-weight: bold;"
        );
}
