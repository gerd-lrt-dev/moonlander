#include "inputmapper.h"

#include <QKeyEvent>
#include <QString>

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

    case Qt::Key_D:
        ENU_PosX = true;
        break;

    case Qt::Key_A:
        ENU_NegX = true;
        break;

    case Qt::Key_W:
        ENU_PosY = true;
        break;

    case Qt::Key_S:
        ENU_NegY = true;
        break;

    case Qt::Key_E:
        ENU_PosZ = true;
        break;

    case Qt::Key_Q:
        ENU_NegZ = true;
        break;

    default:
        break;
    }
    updateFlightCommand();
}

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

    case Qt::Key_D:
        ENU_PosX = false;
        break;

    case Qt::Key_A:
        ENU_NegX = false;
        break;

    case Qt::Key_W:
        ENU_PosY = false;
        break;

    case Qt::Key_S:
        ENU_NegY = false;
        break;

    case Qt::Key_E:
        ENU_PosZ = false;
        break;

    case Qt::Key_Q:
        ENU_NegZ = false;
        break;

    default:
        break;
    }
    updateFlightCommand();
}

void inputmapper::updateLabel(int value)
{
    if (m_label != nullptr)
    {
        m_label->setText(QString("Commanded Thrust: %1 %").arg(value));
    }
}

void inputmapper::updateFlightCommand()
{
    FlightCommand cmd;

    cmd.translation.x = 0.0;
    cmd.translation.y = 0.0;
    cmd.translation.z = 0.0;

    if (ENU_PosX) cmd.translation.x += 1.0;
    if (ENU_NegX) cmd.translation.x -= 1.0;

    if (ENU_PosY) cmd.translation.y += 1.0;
    if (ENU_NegY) cmd.translation.y -= 1.0;

    if (ENU_PosZ) cmd.translation.z += 1.0;
    if (ENU_NegZ) cmd.translation.z -= 1.0;

    emit RCS_cmdRequested(cmd);
}
