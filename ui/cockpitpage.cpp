#include "cockpitpage.h"

#include <QGridLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QSlider>
#include <QtMath>

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
// Helper
// ------------------------------------------------
static void configureLCD(QLCDNumber* lcd, int digits)
{
    lcd->setDigitCount(digits);
    lcd->setSegmentStyle(QLCDNumber::Flat);
    lcd->setSmallDecimalPoint(false);
}

// ------------------------------------------------
// UI Setup
// ------------------------------------------------
void cockpitPage::setupUI()
{
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    mainLayout->setRowStretch(0, 1);
    mainLayout->setRowStretch(1, 1);

    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(1, 1);
    mainLayout->setColumnStretch(2, 1);

    setStyleSheet(
        "QWidget { background-color: #0E1624; color: #D6E1F0; }"
        "QGroupBox { border: 1px solid #2F4A72; border-radius: 8px; margin-top: 22px; padding-top: 18px; font-weight: bold; }"
        "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; left: 12px; top: 6px; padding: 0 6px; color: #4FC3F7; }"
        "QLCDNumber { background-color: #000000; color: #4FC3F7; border: 2px solid #2F4A72; border-radius: 4px; min-height: 26px; }"
        "QSlider::groove:horizontal { height: 6px; background: #2F4A72; }"
        "QSlider::handle:horizontal { width: 14px; background: #4FC3F7; margin: -4px 0; border-radius: 3px; }"
        );

    // ================= NAV =================
    QGroupBox *navBox = new QGroupBox("NAV");
    QGridLayout *navLayout = new QGridLayout(navBox);

    lcdTime     = new QLCDNumber();
    lcdAltitude = new QLCDNumber();
    lcdVSpeed   = new QLCDNumber();
    lcdHSpeed   = new QLCDNumber();

    configureLCD(lcdTime,     7);
    configureLCD(lcdAltitude, 7);
    configureLCD(lcdVSpeed,   6);
    configureLCD(lcdHSpeed,   6);

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

    configureLCD(lcdThrust,       7);
    configureLCD(lcdTargetThrust, 7);
    configureLCD(lcdAcceleration, 6);

    engineLayout->addWidget(new QLabel("Thrust [N]"),        0, 0);
    engineLayout->addWidget(lcdThrust,                       0, 1);
    engineLayout->addWidget(new QLabel("Target Thrust [N]"), 1, 0);
    engineLayout->addWidget(lcdTargetThrust,                 1, 1);
    engineLayout->addWidget(new QLabel("Accel [m/s²]"),      2, 0);
    engineLayout->addWidget(lcdAcceleration,                 2, 1);

    // ================= FUEL =================
    QGroupBox *fuelBox = new QGroupBox("FUEL");
    QGridLayout *fuelLayout = new QGridLayout(fuelBox);

    lcdFuelMass = new QLCDNumber();
    lcdFuelFlow = new QLCDNumber();

    configureLCD(lcdFuelMass, 7);
    configureLCD(lcdFuelFlow, 6);

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

    landingView = new LandingView(this);
    landingView->setMinimumSize(240, 180);
    landingLayout->addWidget(landingView, 1);

    // === Thrust Control Console ===
    QGroupBox *thrustBox = new QGroupBox("THRUST CONTROL");
    QVBoxLayout *thrustLayout = new QVBoxLayout(thrustBox);

    thrustSlider = new QSlider(Qt::Horizontal);
    thrustSlider->setRange(0, 100);
    thrustSlider->setValue(0);

    lblThrustCmd = new QLabel("Commanded Thrust: 0 %");
    lblThrustCmd->setAlignment(Qt::AlignCenter);

    thrustLayout->addWidget(thrustSlider);
    thrustLayout->addWidget(lblThrustCmd);

    landingLayout->addWidget(thrustBox);

    // === Simulation Controls ===
    QHBoxLayout *simControlLayout = new QHBoxLayout();
    btnSimStart = new QPushButton("START");
    btnSimPause = new QPushButton("PAUSE");
    btnSimStop  = new QPushButton("STOP");

    simControlLayout->addWidget(btnSimStart);
    simControlLayout->addWidget(btnSimPause);
    simControlLayout->addWidget(btnSimStop);

    landingLayout->addLayout(simControlLayout);

    // ================= GRID ASSEMBLY =================
    mainLayout->addWidget(navBox,     0, 0);
    mainLayout->addWidget(fuelBox,    1, 0);
    mainLayout->addWidget(landingBox, 0, 1, 2, 1);
    mainLayout->addWidget(engineBox,  0, 2);
    mainLayout->addWidget(statusBox,  1, 2);

    setLayout(mainLayout);
}

// ------------------------------------------------
// Connections
// ------------------------------------------------
void cockpitPage::setupConnections()
{
    connect(btnSimStart, &QPushButton::clicked, this, &cockpitPage::startRequested);    ///< Emits signal
    connect(btnSimPause, &QPushButton::clicked, this, &cockpitPage::pauseRequested);    ///< Emits signal
    connect(btnSimStop,  &QPushButton::clicked, this, &cockpitPage::onStopClicked);     ///< Combined with private slot

    connect(thrustSlider, &QSlider::valueChanged, this, [this](int value)
            {
                lblThrustCmd->setText(QString("Commanded Thrust: %1 %").arg(value));
                emit thrustTargetRequested(static_cast<double>(value));
            });
}

// ------------------------------------------------
// Update Interface
// ------------------------------------------------
void cockpitPage::updateTime(double t)               { lcdTime->display(QString::number(t, 'f', 2)); }
void cockpitPage::updateAltitude(double a)           { lcdAltitude->display(QString::number(a, 'f', 1)); }
void cockpitPage::updateVerticalVelocity(double v)   { lcdVSpeed->display(QString::number(v, 'f', 1)); }
void cockpitPage::updateHorizontalVelocity(double h) { lcdHSpeed->display(QString::number(h, 'f', 1)); }
void cockpitPage::updateAcceleration(double a)       { lcdAcceleration->display(QString::number(a, 'f', 2)); }
void cockpitPage::updateThrust(double t)             { lcdThrust->display(QString::number(t, 'f', 1)); }
void cockpitPage::updateTargetThrust(double t)       { lcdTargetThrust->display(QString::number(t, 'f', 1)); }
void cockpitPage::updateFuelMass(double f)           { lcdFuelMass->display(QString::number(f, 'f', 1)); }
void cockpitPage::updateFuelFlow(double f)           { lcdFuelFlow->display(QString::number(f, 'f', 2)); }

void cockpitPage::updateHullStatus(bool intact)
{
    lblHullStatus->setText(intact ? "HULL: OK" : "HULL: DAMAGED");
    lblHullStatus->setStyleSheet(intact
                                     ? "color: lime; font-weight: bold;"
                                     : "color: red; font-weight: bold;");
}

// ------------------------------------------------
// Slots
// ------------------------------------------------
void cockpitPage::onStateUpdated(double time,
                                 const Vector3& pos,
                                 const Vector3& vel,
                                 const Vector3& acc,
                                 bool intact,
                                 double thrust,
                                 double targetThrust,
                                 double fuelMass,
                                 double fuelFlow)
{
    updateTime(time);
    updateAltitude(qRound(pos.z * 10.0) / 10.0);
    updateVerticalVelocity(qRound(vel.z * 10.0) / 10.0);
    updateHorizontalVelocity(qRound(vel.x * 10.0) / 10.0);
    updateAcceleration(qRound(acc.z * 100.0) / 100.0);
    updateThrust(qRound(thrust * 10.0) / 10.0);
    updateTargetThrust(qRound(targetThrust * 10.0) / 10.0);
    updateFuelMass(qRound(fuelMass * 10.0) / 10.0);
    updateFuelFlow(qRound(fuelFlow * 100.0) / 100.0);
    updateHullStatus(intact);

    landingView->setAltitude(pos.z);
    landingView->setThrust(thrust);
    landingView->setHullIntact(intact);
}

void cockpitPage::onStopClicked()
{
    auto reply = QMessageBox::question(
        this,
        "Stop Simulation",
        "If you continue, the simulation will be reset.\nDo you want to proceed?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        emit stopConfirmed();
    }
}

