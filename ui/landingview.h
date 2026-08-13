/**
 * @file landingview.h
 * @brief 2.5D landing visualization widget for lunar landing simulation.
 *
 * This widget provides a compact situational awareness view of the spacecraft
 * during descent and landing in a local ENU (East-North-Up) frame.
 *
 * The visualization is intentionally designed as a lightweight alternative
 * to full 3D rendering, focusing on interpretability and controller analysis.
 *
 * The widget is split into three main regions:
 *
 * - Side View (E-UP): Shows vertical motion, lateral offset, projected attitude,
 *   and main-engine thrust direction
 * - Top View (E-N): Shows horizontal drift, target relation, and yaw orientation
 * - State Panel: Displays key numerical telemetry values
 *
 * The widget is passive and only visualizes values provided
 * through update functions.
 *
 * Coordinate Convention:
 * - X → East
 * - Y → North
 * - Z → Up
 *
 * Info:
 * - Quaternion-based attitude is expected to be processed externally.
 * - The top view receives yaw explicitly.
 * - The side view derives the visible E-UP attitude projection from the
 *   main-engine/body-axis direction provided in the ENU frame.
 * - This widget does NOT perform simulation or control logic.
 *
 * @author
 * Generated with ChatGPT, extended by gerd-lrt-dev
 */

#ifndef LANDINGVIEW_H
#define LANDINGVIEW_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QPointF>

#include <Eigen/Dense>

/**
 * @class LandingView
 * @brief 2.5D landing visualization widget.
 *
 * LandingView renders a simplified spatial representation of the spacecraft
 * in a local ENU coordinate system. It combines multiple views to provide
 * an intuitive understanding of vehicle motion without requiring full 3D rendering.
 *
 * Displayed information includes:
 * - Position relative to landing target
 * - Velocity vectors
 * - Trajectory history
 * - Main-engine thrust direction
 * - Projected spacecraft attitude in side view
 * - Yaw orientation in top view
 * - Hull integrity status
 *
 * The widget is optimized for:
 * - Controller tuning
 * - Stability analysis
 * - Landing behavior interpretation
 * - 6DoF state visualization in a lightweight 2.5D representation
 */
class LandingView : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a LandingView widget.
     *
     * Initializes rendering settings and animation timer.
     *
     * @param parent Optional parent widget.
     */
    explicit LandingView(QWidget *parent = nullptr);

    // =====================================================
    // Public Update Interface (Simulation → UI)
    // =====================================================

    /**
     * @brief Updates spacecraft position in ENU frame.
     *
     * @param pos Position vector:
     * - x → East [m]
     * - y → North [m]
     * - z → Up (altitude) [m]
     */
    void setPositionENU(const Eigen::Vector3d& pos);

    /**
     * @brief Updates spacecraft velocity in ENU frame.
     *
     * @param vel Velocity vector:
     * - x → East velocity [m/s]
     * - y → North velocity [m/s]
     * - z → Vertical velocity [m/s]
     */
    void setVelocityENU(const Eigen::Vector3d& vel);

    /**
     * @brief Updates yaw angle for top view visualization.
     *
     * @param yawDeg Yaw angle in degrees.
     *
     * @note Expected to be derived externally from quaternion or rotation state.
     */
    void setYawDeg(double yawDeg);

    /**
     * @brief Updates the main-engine thrust direction in the ENU frame.
     *
     * The supplied direction is used to visualize the current main-engine
     * thrust direction in the side view and to derive the visible spacecraft
     * attitude projection in the East-Up plane.
     *
     * @param direction Main-engine thrust direction expressed in ENU coordinates.
     *
     * @note The vector is expected to represent a direction only. Its magnitude
     *       is not used for thrust scaling.
     */
    void setMainEngineDirectionENU(const Eigen::Vector3d& direction);

    /**
     * @brief Updates target landing position in ENU frame.
     *
     * @param target Target position vector:
     * - x → East [m]
     * - y → North [m]
     * - z → typically 0 (ground)
     */
    void setTargetENU(const Eigen::Vector3d& target);

    /**
     * @brief Updates engine thrust level.
     *
     * @param percent Thrust percentage [0–1].
     *
     * @note The value is converted internally to percent representation.
     */
    void setThrust(double percent);

    /**
     * @brief Updates the RCS activity state.
     *
     * @param active true if at least one RCS thruster is active.
     */
    void setRCSActive(bool active);

    /**
     * @brief Updates the spacecraft integrity state.
     *
     * Determines whether the spacecraft should be rendered as structurally
     * intact based on the current spacecraft status.
     *
     * @param spacecraftState Human-readable spacecraft state.
     *
     * @note Determines the visual appearance of the spacecraft.
     */
    void setHullIntact(const QString& spacecraftState);

protected:
    /**
     * @brief Paints the complete landing visualization.
     *
     * @param event Paint event.
     */
    void paintEvent(QPaintEvent *event) override;

private:
    // =====================================================
    // Internal State
    // =====================================================

    Eigen::Vector3d positionENU {0.0, 0.0, 0.0};          ///< Current position in ENU [m]
    Eigen::Vector3d velocityENU {0.0, 0.0, 0.0};          ///< Current velocity in ENU [m/s]
    Eigen::Vector3d targetENU   {0.0, 0.0, 0.0};          ///< Target position in ENU [m]
    Eigen::Vector3d mainEngineDirectionENU {0.0, 0.0, 1.0}; ///< Main-engine thrust direction in ENU

    double thrustPercent = 0.0; ///< Main-engine thrust [%]
    bool RCSActive = false;     ///< Indicates whether RCS thrusters are active
    int activeThruster = 0;     ///< Number of active RCS thrusters
    double yawDeg = 0.0;        ///< Yaw angle [deg]

    bool hullIntact = true;     ///< Hull integrity state

    QVector<QPointF> trajectoryEN; ///< Trajectory history in E-N plane

    // =====================================================
    // Animation
    // =====================================================

    QTimer animationTimer; ///< Timer for motion stripe animation
    int motionOffset = 0;  ///< Animation phase offset

    // =====================================================
    // Rendering Helpers
    // =====================================================

    /**
     * @brief Draws a framed panel with title.
     */
    void drawPanelFrame(QPainter& p, const QRect& r, const QString& title);

    /**
     * @brief Draws side view (East-Up plane).
     */
    void drawSideView(QPainter& p, const QRect& r);

    /**
     * @brief Draws top view (East-North plane).
     */
    void drawTopView(QPainter& p, const QRect& r);

    /**
     * @brief Draws compact state/telemetry panel.
     */
    void drawStatusBox(QPainter& p, const QRect& r);

    /**
     * @brief Maps ENU coordinates to side view screen coordinates.
     */
    QPointF mapSideView(const QRect& r,
                        double east,
                        double up,
                        double maxEastAbs,
                        double maxUp) const;

    /**
     * @brief Maps ENU coordinates to top view screen coordinates.
     */
    QPointF mapTopView(const QRect& r,
                       double east,
                       double north,
                       double maxEastAbs,
                       double maxNorthAbs) const;

    /**
     * @brief Draws lander in side view.
     *
     * The displayed attitude represents the projection of the spacecraft
     * orientation into the East-Up plane.
     *
     * @param p Painter used for rendering.
     * @param center Screen-space center position of the lander.
     * @param attitudeDeg Projected spacecraft attitude in the E-UP plane [deg].
     */
    void drawLanderSide(QPainter& p,
                        const QPointF& center,
                        double attitudeDeg);

    /**
     * @brief Draws lander in top view including yaw orientation.
     */
    void drawLanderTop(QPainter& p,
                       const QPointF& center,
                       double yawDeg);

    /**
     * @brief Draws vector (e.g. velocity or thrust).
     */
    void drawVector(QPainter& p,
                    const QPointF& start,
                    const QPointF& vec,
                    const QColor& color);
};

#endif // LANDINGVIEW_H
