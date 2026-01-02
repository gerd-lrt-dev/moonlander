#include "landingview.h"

#include <QPainter>
#include <QPaintEvent>

LandingView::LandingView(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(300, 300);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
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

void LandingView::setHullIntact(bool intact)
{
    hullIntact = intact;
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

void LandingView::drawLander(QPainter &p, const QRect &landerRect)
{
    QColor hullColor = hullIntact ? QColor("#CFD8DC") : QColor("#E53935");

    p.setPen(Qt::NoPen);
    p.setBrush(hullColor);
    p.drawRoundedRect(landerRect, 4, 4);
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
