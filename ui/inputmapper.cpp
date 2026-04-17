#include "inputmapper.h"

#include <QKeyEvent>
#include <QString>

/**
 * @brief Constructs the input mapper and connects the slider to UI/state updates.
 *
 * The slider remains the single source of truth for main engine thrust.
 * Any value change, whether caused by mouse interaction or keyboard input,
 * updates the label and emits the requested thrust target signal.
 *
 * @param mainEngineSlider Pointer to the main engine slider.
 * @param label Pointer to the UI label showing the commanded thrust.
 * @param parent Optional Qt parent object.
 */
inputmapper::inputmapper(QSlider* mainEngineSlider, QLabel* label, QObject* parent)
    : QObject(parent),
    mE_Slider(mainEngineSlider),
    m_label(label)
{
    Q_ASSERT(mE_Slider != nullptr);

    connect(mE_Slider, &QSlider::valueChanged, this, [this](int value)
            {
                updateLabel(value);
                emit ME_thrustTargetRequested(static_cast<double>(value));
            });

    updateLabel(mE_Slider->value());
}

/**
 * @brief Handles relevant key press events and updates the slider.
 *
 * Arrow Up increases the slider value by one step.
 * Arrow Down decreases the slider value by one step.
 *
 * Since QSlider::setValue() is used, the slider automatically enforces
 * its configured minimum and maximum range.
 *
 * @param event Pointer to the incoming key press event.
 */
void inputmapper::handleKeyPress(QKeyEvent* event)
{
    if (event == nullptr || mE_Slider == nullptr)
    {
        return;
    }

    if (event->isAutoRepeat())
    {
        return;
    }

    switch (event->key())
    {
    case Qt::Key_Up:
        mE_Slider->setValue(mE_Slider->value() + 1);
        break;

    case Qt::Key_Down:
        mE_Slider->setValue(mE_Slider->value() - 1);
        break;

    default:
        break;
    }
}

/**
 * @brief Handles key release events.
 *
 * This implementation currently performs no action on key release,
 * because main engine throttle is updated discretely on key press.
 *
 * The method is retained to support future extensions, for example
 * continuous throttle input or additional mapped controls.
 *
 * @param event Pointer to the incoming key release event.
 */
void inputmapper::handleKeyRelease(QKeyEvent* event)
{
    if (event == nullptr)
    {
        return;
    }

    if (event->isAutoRepeat())
    {
        return;
    }

    switch (event->key())
    {
    case Qt::Key_Up:
    case Qt::Key_Down:
        // No action required at the moment.
        break;

    default:
        break;
    }
}

/**
 * @brief Updates the thrust label with the current commanded thrust value.
 *
 * @param value Current slider value in percent.
 */
void inputmapper::updateLabel(int value)
{
    if (m_label != nullptr)
    {
        m_label->setText(QString("Commanded Thrust: %1 %").arg(value));
    }
}
