#include "inputmapper.h"

#include <QKeyEvent>
#include <QString>

inputmapper::inputmapper(QSlider* mainEngineSlider, QLabel* label, QObject* parent)
    : QObject(parent),
    mE_Slider(mainEngineSlider),
    m_label(label)
{
    Q_ASSERT(mE_Slider != nullptr);
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
        return;

    case Qt::Key_Down:
        mE_Slider->setValue(mE_Slider->value() - 1);
        return;

    // Translation
    case Qt::Key_D:
        ENU_RCS_PosX = true;
        break;

    case Qt::Key_A:
        ENU_RCS_NegX = true;
        break;

    case Qt::Key_W:
        ENU_RCS_PosY = true;
        break;

    case Qt::Key_S:
        ENU_RCS_NegY = true;
        break;

    case Qt::Key_E:
        ENU_RCS_PosZ = true;
        break;

    case Qt::Key_Q:
        ENU_RCS_NegZ = true;
        break;

    // Rotation - Roll
    case Qt::Key_6:
        ENU_RCS_PosRoll = true;
        break;

    case Qt::Key_4:
        ENU_RCS_NegRoll = true;
        break;

    // Rotation - Pitch
    case Qt::Key_8:
        ENU_RCS_PosPitch = true;
        break;

    case Qt::Key_2:
        ENU_RCS_NegPitch = true;
        break;

    // Rotation - Yaw
    case Qt::Key_9:
        ENU_RCS_PosYaw = true;
        break;

    case Qt::Key_7:
        ENU_RCS_NegYaw = true;
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
    // Translation
    case Qt::Key_D:
        ENU_RCS_PosX = false;
        break;

    case Qt::Key_A:
        ENU_RCS_NegX = false;
        break;

    case Qt::Key_W:
        ENU_RCS_PosY = false;
        break;

    case Qt::Key_S:
        ENU_RCS_NegY = false;
        break;

    case Qt::Key_E:
        ENU_RCS_PosZ = false;
        break;

    case Qt::Key_Q:
        ENU_RCS_NegZ = false;
        break;

    // Rotation - Roll
    case Qt::Key_6:
        ENU_RCS_PosRoll = false;
        break;

    case Qt::Key_4:
        ENU_RCS_NegRoll = false;
        break;

    // Rotation - Pitch
    case Qt::Key_8:
        ENU_RCS_PosPitch = false;
        break;

    case Qt::Key_2:
        ENU_RCS_NegPitch = false;
        break;

    // Rotation - Yaw
    case Qt::Key_9:
        ENU_RCS_PosYaw = false;
        break;

    case Qt::Key_7:
        ENU_RCS_NegYaw = false;
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
    FlightCommandDTO cmd{};

    // Translation
    cmd.translation.x() = resolveAxisCommand(ENU_RCS_PosX, ENU_RCS_NegX);
    cmd.translation.y() = resolveAxisCommand(ENU_RCS_PosY, ENU_RCS_NegY);
    cmd.translation.z() = resolveAxisCommand(ENU_RCS_PosZ, ENU_RCS_NegZ);

    // Rotation
    cmd.rotation.x() = resolveAxisCommand(ENU_RCS_PosRoll, ENU_RCS_NegRoll);
    cmd.rotation.y() = resolveAxisCommand(ENU_RCS_PosPitch, ENU_RCS_NegPitch);
    cmd.rotation.z() = resolveAxisCommand(ENU_RCS_PosYaw, ENU_RCS_NegYaw);

    emit RCS_cmdRequested(cmd);
}

double inputmapper::resolveAxisCommand(bool positive, bool negative)
{
    if (positive && !negative)
    {
        return 1.0;
    }

    if (negative && !positive)
    {
        return -1.0;
    }

    return 0.0;
}
