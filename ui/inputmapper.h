#pragma once

#include <QLabel>
#include <QObject>
#include <QSlider>

class QKeyEvent;

/**
 * @file inputmapper.h
 * @brief Declares the inputmapper class used to map keyboard input to UI-based thrust control.
 */

/**
 * @class inputmapper
 * @brief Maps keyboard input to the main engine thrust slider and emits thrust commands.
 *
 * This class provides a small Qt-based input adapter that augments an existing
 * main engine throttle slider with keyboard interaction.
 *
 * The current use case is limited to manual control of the main engine command:
 * - Arrow Up increases the commanded thrust
 * - Arrow Down decreases the commanded thrust
 *
 * Internally, the keyboard does not emit thrust values directly. Instead, it modifies
 * the associated QSlider value. This ensures that all UI updates remain consistent,
 * because the slider stays the single source of truth for the commanded main engine thrust.
 *
 * Whenever the slider value changes:
 * - the command label is updated
 * - the signal ME_thrustTargetRequested() is emitted
 *
 * This class is intended to be owned and used by the UI layer, e.g. the cockpit page.
 *
 * @note The widget receiving the keyboard events must have keyboard focus.
 * @note This class currently handles only main engine throttle input.
 *       It can later be extended for additional control axes such as RCS commands.
 */
class inputmapper : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the input mapper for main engine throttle control.
     *
     * Creates an input helper that connects a given slider and label to keyboard-based
     * throttle changes.
     *
     * The slider represents the current commanded main engine thrust.
     * The label displays the currently commanded thrust as text.
     *
     * @param mainEngineSlider Pointer to the slider representing the main engine thrust command.
     * @param label Pointer to the label displaying the current commanded thrust.
     * @param parent Optional Qt parent object.
     */
    explicit inputmapper(QSlider* mainEngineSlider, QLabel* label, QObject* parent = nullptr);

    /**
     * @brief Handles a key press event for thrust input.
     *
     * Processes relevant keyboard input and updates the main engine slider accordingly.
     *
     * Current mapping:
     * - Qt::Key_Up   -> increase main engine thrust
     * - Qt::Key_Down -> decrease main engine thrust
     *
     * @param event Pointer to the Qt key event.
     *
     * @note Auto-repeat key press events are ignored.
     */
    void handleKeyPress(QKeyEvent* event);

    /**
     * @brief Handles a key release event for thrust input.
     *
     * This method is currently present for completeness and future extensibility.
     * At the moment, no persistent pressed/released state is stored for main engine
     * throttle control, so key releases do not trigger any action.
     *
     * @param event Pointer to the Qt key event.
     *
     * @note Auto-repeat key release events are ignored.
     */
    void handleKeyRelease(QKeyEvent* event);

signals:
    /**
     * @brief Emitted when a new main engine thrust target is requested.
     *
     * The emitted value corresponds to the current slider value after user input.
     *
     * @param value Commanded main engine thrust in percent.
     */
    void ME_thrustTargetRequested(double value);

private:
    /**
     * @brief Updates the thrust command label text.
     *
     * Formats and writes the current commanded thrust value to the associated label.
     *
     * @param value Current main engine thrust command in percent.
     */
    void updateLabel(int value);

    /**
     * @brief Pointer to the main engine thrust slider.
     *
     * This slider acts as the central UI state for the commanded thrust value.
     */
    QSlider* mE_Slider = nullptr;

    /**
     * @brief Pointer to the label showing the commanded thrust.
     */
    QLabel* m_label = nullptr;
};
