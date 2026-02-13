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
     * @param spacecraftState_ Current spacecraft state enum.
     */
    void updateHullStatus(SpacecraftState spacecraftState_);

    /**
     * @brief Updates the autopilot status indicator.
     *
     * Changes the visual label and/or indicator that shows
     * whether the autopilot is currently active.
     *
     * @param active True if autopilot is enabled, false otherwise.
     */
    void updateAutopilotStatus(bool active);

signals:
    /**
     * @brief Emitted when the user requests simulation start.
     */
    void startRequested();

    /**
     * @brief Emitted when the user requests simulation pause.
     */
    void pauseRequested();

    /**
     * @brief Emitted after the user confirmed a simulation stop/reset.
     */
    void stopConfirmed();

    /**
     * @brief Emitted when the user requests a full simulation reset.
     */
    void resetSimulationRequested();

    /**
     * @brief Emitted when the user changes the thrust slider.
     * @param percent Target thrust in percent.
     */
    void thrustTargetRequested(double percent);

    /**
     * @brief Emitted when the autopilot toggle button is pressed.
     * @param acitve True if autopilot should be enabled.
     */
    void autopilotToggled(bool acitve);

public slots:
    /**
     * @brief Slot receiving updated simulation state.
     *
     * Called by the simulation backend or controller to update
     * all cockpit instruments and visualization elements.
     *
     * @param time Current simulation time in seconds.
     * @param pos Current spacecraft position vector.
     * @param vel Current spacecraft velocity vector.
     * @param GLoad Current acceleration magnitude.
     * @param spacecraftState_ Current spacecraft state enum.
     * @param thrust Current thrust in Newton.
     * @param targetThrust Target thrust in Newton.
     * @param fuelMass Remaining fuel mass in kilograms.
     * @param fuelFlow Current fuel flow in kilograms per second.
     */
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
    /**
     * @brief Handles stop button click including confirmation dialog.
     */
    void onStopClicked(); ///< Combining stop button with prompt

    /**
     * @brief Handles autopilot toggle button click.
     *
     * Toggles internal autopilot state and emits corresponding signal.
     */
    void onAutopilotClicked();

private:
    // Members
    double lastTimeDisplay; ///< Intermediate storage of time to calm the display down

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
     * @brief Builds navigation elements.
     * @return Navigation Box as QGroupBox.
     */
    QGroupBox *setupNavBox();

    /**
     * @brief Builds engine box elements.
     * @return Engine Box as QGroupBox.
     */
    QGroupBox *setupEngineBox();

    /**
     * @brief Builds fuel elements.
     * @return Fuel Box as QGroupBox.
     */
    QGroupBox *setupFuelBox();

    /**
     * @brief Builds status elements.
     * @return Status Box as QGroupBox.
     */
    QGroupBox *setupStatusBox();

    /**
     * @brief Builds landing elements.
     * @return Landing Box as QGroupBox.
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

    QLabel *lblHullStatus;       ///< Hull integrity indicator
    QLabel *lblAutopilotStatus;  ///< Status update when autopilot becomes activated

    // =====================================================
    // Visualization
    // =====================================================

    LandingView *landingView; ///< 2D landing visualization widget

    // =====================================================
    // Buttons
    // =====================================================

    QPushButton *btnSimStart; ///< Simulation start button
    QPushButton *btnSimPause; ///< Simulation pause button
    QPushButton *btnSimStop;  ///< Simulation stop button

    // =====================================================
    // Thrust Controle Console
    // =====================================================

    QSlider* thrustSlider = nullptr; ///< Thrust control slider
    QLabel* lblThrustCmd  = nullptr; ///< Displays commanded thrust percentage

    // =====================================================
    // Autopilot
    // =====================================================

    QPushButton* btnAutopilot; ///< Autopilot toggle button
    bool autopilotActive = false; ///< Internal autopilot state flag
};

#endif // COCKPITPAGE_H
