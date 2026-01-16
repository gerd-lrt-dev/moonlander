#include "landingview.h"

#include <QPainter>
#include <QPaintEvent>

LandingView::LandingView(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(300, 300);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Animation timer (≈30 FPS)
    connect(&animationTimer, &QTimer::timeout, this, [this]() {
        motionOffset = (motionOffset + 2) % 20;
        update();
    });

    animationTimer.start(33);
}

// =====================
// Update Interface
// =====================
void LandingView::setAltitude(double meters)
{
    altitudeMeters = meters;
    update();
}

void LandingView::setThrust(double percent)
{
    thrustPercent = percent;
    update();
}

void LandingView::setHullIntact(SpacecraftState spacecraftState_)
{
    if (spacecraftState_ == SpacecraftState::Operational)
    {
        hullIntact = true;
    }
    else if (spacecraftState_ == SpacecraftState::Landed)
    {
        hullIntact = true;
    }
    else
    {
        hullIntact = false;
    }

    update();
}

// =====================
// Rendering
// =====================
void LandingView::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Background
    p.fillRect(rect(), QColor("#0E1624"));

    drawGround(p);

    // Altitude scaling
    const int groundY = height() - 40;
    const double maxDisplayAltitude = 1000.0; // meters (visual scale)

    double normAlt = qBound(0.0, altitudeMeters / maxDisplayAltitude, 1.0);
    int landerY = groundY - static_cast<int>(normAlt * (height() - 100));

    QRect landerRect(width() / 2 - 15, landerY - 30, 30, 30);

    drawMotionStripes(p, landerY);
    drawLander(p, landerRect);
    drawThrust(p, landerRect);
}

// =====================
// Helper Drawing
// =====================
void LandingView::drawGround(QPainter &p)
{
    p.setPen(QPen(QColor("#6D4C41"), 3));
    p.drawLine(20, height() - 40, width() - 20, height() - 40);
}

void LandingView::drawLander(QPainter &p, const QRect &r)
{
    QColor hullColor = hullIntact ? QColor("#CFD8DC") : QColor("#E53935");
    p.setPen(QPen(Qt::black, 1));
    p.setBrush(hullColor);

    // Body (trapezoid)
    QPolygon body;
    body << QPoint(r.left() + 5, r.top())
         << QPoint(r.right() - 5, r.top())
         << QPoint(r.right(), r.bottom())
         << QPoint(r.left(), r.bottom());
    p.drawPolygon(body);

    // Legs
    p.setPen(QPen(QColor("#90A4AE"), 2));
    p.drawLine(r.left(),  r.bottom(), r.left()  - 10, r.bottom() + 12);
    p.drawLine(r.right(), r.bottom(), r.right() + 10, r.bottom() + 12);

    // Engine nozzle
    QRect nozzle(r.center().x() - 4, r.bottom() - 2, 8, 6);
    p.setBrush(QColor("#455A64"));
    p.drawRect(nozzle);

    // Window
    p.setBrush(QColor("#4FC3F7"));
    p.drawEllipse(r.center().x() - 4, r.top() + 6, 8, 8);
}

void LandingView::drawThrust(QPainter &p, const QRect &landerRect)
{
    if (thrustPercent <= 0.0)
        return;

    int flameHeight = static_cast<int>((thrustPercent / 100.0) * 40.0);

    QRect flameRect(
        landerRect.center().x() - 5,
        landerRect.bottom(),
        10,
        flameHeight
        );

    QLinearGradient flameGrad(flameRect.topLeft(), flameRect.bottomLeft());
    flameGrad.setColorAt(0.0, QColor("#FFF176"));
    flameGrad.setColorAt(1.0, QColor("#E65100"));

    p.setBrush(flameGrad);
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(flameRect, 3, 3);
}

void LandingView::drawMotionStripes(QPainter& p, int landerY)
{
    p.setPen(QPen(QColor(255, 255, 255, 40), 2));

    const int stripeCount = 6;
    const int stripeSpacing = 18;

    for (int i = 0; i < stripeCount; ++i)
    {
        int y = landerY + 40 + (i * stripeSpacing) + motionOffset;
        if (y > height() - 40)
            continue;

        p.drawLine(
            width() / 2 - 40, y,
            width() / 2 + 40, y
            );
    }
}

