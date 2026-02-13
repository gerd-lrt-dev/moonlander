/**
 * @file cockpitpage.h
 * @brief Cockpit / simulation view page of the Lunar Lander simulator.
 *
 * This file defines the cockpitPage class which represents the main
 * simulation cockpit UI. It contains LCD-style instruments for navigation,
 * engine data, fuel state, and spacecraft status.
 */

#ifndef COCKPITPAGE_H
#define COCKPITPAGE_H

#include <QGroupBox>
#include <QWidget>
#include <QLCDNumber>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSlider>
#include <vector3.h>
#include <spacecraftStateStruct.h>

#include "landingview.h"

/**
 * @class cockpitPage
 * @brief Main simulation cockpit interface.
 *
 * The cockpitPage class represents the primary in-simulation user interface.
 * It visualizes real-time spacecraft telemetry such as altitude, velocity,
 * thrust, acceleration, fuel state, and hull integrity.
 *
 * The class itself does not perform any simulation logic. Instead, it exposes
 * a set of update functions which are intended to be called by a simulation
 * controller or worker thread via Qt signals and slots.
 *
 * Typical usage:
 * - Instantiate cockpitPage
 * - Add it to a QStackedWidget or main layout
 * - Connect simulation outputs to the update*() slots
 *
 * Info:
 * - This file was initially created by ChatGPT and then edited by gerd-lrt-dev.
 */
class cockpitPage : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the cockpit page.
     *
     * Initializes all UI elements and layouts required for the cockpit view.
     *
     * @param parent Optional parent widget.
     */
    explicit cockpitPage(QWidget *parent = nullptr);

    // =====================================================
    // Public Update Interface (Simulation → UI)
    // =====================================================

    /**
     * @brief Updates the simulation time display.
     * @param t Simulation time in seconds.
     */
    void updateTime(double t);

    /**
     * @brief Updates the current altitude display.
     * @param altitude Altitude above surface in meters.
     */
    void updateAltitude(double altitude);

    /**
     * @brief Updates the vertical velocity display.
     * @param vSpeed Vertical velocity in meters per second.
     */
    void updateVerticalVelocity(double vSpeed);

    /**
     * @brief Updates the horizontal velocity display.
     * @param hSpeed Horizontal velocity in meters per second.
     */
    void updateHorizontalVelocity(double hSpeed);

    /**
     * @brief Updates the current spacecraft acceleration.
     * @param accel Acceleration in meters per second squared.
     */
    void updateAcceleration(double accel);

    /**
     * @brief Updates the current engine thrust.
     * @param thrust Current thrust value (percentage or normalized).
     */
    void updateThrust(double thrust);

    /**
     * @brief Updates the target thrust setpoint.
     * @param target Target thrust value.
     */
    void updateTargetThrust(double target);

    /**
     * @brief Updates the remaining fuel mass.
     * @param fuel Remaining fuel mass in kilograms.
     */
    void updateFuelMass(double fuel);

    /**
     * @brief Updates the current fuel consumption rate.
     * @param flow Fuel consumption in kilograms per second.
     */
    void updateFuelFlow(double flow);

    /**
     * @brief Updates the hull integrity indicator.
     *
     * Displays visual feedback whether the spacecraft hull
     * is still intact or has been damaged.
     *
     * @param intact True if hull is intact, false if damaged.
     */
    void updateHullStatus(SpacecraftState spacecraftState_);

signals:
    void startRequested();
    void pauseRequested();
    void stopConfirmed();
    void resetSimulationRequested();
    void thrustTargetRequested(double percent);

public slots:
    void onStateUpdated(double time,
                        const Vector3& pos,
                        const Vector3& vel,
                        const double& GLoad,
                        SpacecraftState spacecraftState_,
                        double thrust,
                        double targetThrust,
                        double fuelMass,
                        double fuelFlow);

private slots:
    void onStopClicked(); ///< Combining stop button with prompt

private:
    // Members
    double lastTimeDisplay;         ///< Intermediate storage of time to calm the display down
    // =====================================================
    // Internal Setup Functions
    // =====================================================

    /**
     * @brief Builds all cockpit UI elements and layouts.
     *
     * Creates LCD instruments, group boxes, and assigns
     * them to the cockpit layout.
     */
    void setupUI();

    /**
     * @brief Builds navigation elements
     * @return Navigation Box as QGroupBox
     */
    QGroupBox *setupNavBox();

    /**
     * @brief Builds engine Box elements
     * @return Engine Box as QGroupBox
     */
    QGroupBox *setupEngineBox();

    /**
     * @brief Builds fuel elements
     * @return Fuel Box as QGroupBox
     */
    QGroupBox *setupFuelBox();

    /**
     * @brief Builds status elements
     * @return Status Box as QGroupBox
     */
    QGroupBox *setupStatusBox();

    /**
     * @brief Builds landing elements
     * @return Landing Box as QGroupBox
     */
    QGroupBox *setupLandingBox();

    /**
     * @brief Connects internal signals and slots.
     *
     * Reserved for future extensions such as local UI controls
     * or debug interactions.
     */
    void setupConnections();

    // =====================================================
    // Navigation Instruments
    // =====================================================

    QLCDNumber *lcdTime;        ///< Simulation time display [s]
    QLCDNumber *lcdAltitude;    ///< Altitude above surface [m]
    QLCDNumber *lcdVSpeed;      ///< Vertical velocity [m/s]
    QLCDNumber *lcdHSpeed;      ///< Horizontal velocity [m/s]

    // =====================================================
    // Engine Instruments
    // =====================================================

    QLCDNumber *lcdThrust;       ///< Current engine thrust
    QLCDNumber *lcdTargetThrust; ///< Target thrust setpoint
    QLCDNumber *lcdAcceleration; ///< Current acceleration [m/s²]

    // =====================================================
    // Fuel Instruments
    // =====================================================

    QLCDNumber *lcdFuelMass; ///< Remaining fuel mass [kg]
    QLCDNumber *lcdFuelFlow; ///< Fuel consumption rate [kg/s]

    // =====================================================
    // Status Indicators
    // =====================================================

    QLabel *lblHullStatus; ///< Hull integrity indicator

    // =====================================================
    // Visualization
    // =====================================================
    LandingView *landingView;

    // =====================================================
    // Buttons
    // =====================================================
    QPushButton *btnSimStart;
    QPushButton *btnSimPause;
    QPushButton *btnSimStop;

    // =====================================================
    // Thrust Controle Console
    // =====================================================
    QSlider* thrustSlider = nullptr;
    QLabel* lblThrustCmd  = nullptr;
};

#endif // COCKPITPAGE_H
