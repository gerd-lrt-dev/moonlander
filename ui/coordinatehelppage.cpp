#include "coordinatehelppage.h"

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPolygonF>
#include <QScrollArea>
#include <QVBoxLayout>

#include <cmath>

namespace
{

class CoordinateFrameDiagram : public QWidget
{
public:
    explicit CoordinateFrameDiagram(
        const QString& frameName,
        QWidget *parent = nullptr)
        : QWidget(parent),
          frameName_(frameName)
    {
        setMinimumSize(220, 145);
        setMaximumHeight(165);
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);

        const QRectF panel = rect().adjusted(1.0, 1.0, -1.0, -1.0);

        painter.setPen(QPen(QColor("#223A5E"), 1.0));
        painter.setBrush(QColor("#091523"));
        painter.drawRoundedRect(panel, 8.0, 8.0);

        if (frameName_ == "MCI")
        {
            drawMCI(painter);
        }
        else if (frameName_ == "MCMF")
        {
            drawMCMF(painter);
        }
        else if (frameName_ == "MSC")
        {
            drawMSC(painter);
        }
        else if (frameName_ == "ENU")
        {
            drawENU(painter);
        }
        else if (frameName_ == "LVLH")
        {
            drawLVLH(painter);
        }
        else if (frameName_ == "SBF")
        {
            drawSBF(painter);
        }
    }

private:
    QString frameName_;

    const QColor accent_{"#38BDF8"};
    const QColor primary_{"#D6E1F0"};
    const QColor secondary_{"#8FA4BA"};
    const QColor surface_{"#3B506B"};

    void drawArrow(
        QPainter& painter,
        const QPointF& start,
        const QPointF& end,
        const QString& label,
        const QColor& color) const
    {
        painter.setPen(QPen(color, 2.0, Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(start, end);

        const QLineF line(start, end);
        const double angle = std::atan2(-line.dy(), line.dx());
        constexpr double arrowSize = 7.0;
        constexpr double pi = 3.14159265358979323846;

        const QPointF arrowP1 =
            end - QPointF(
                      std::cos(angle + pi / 6.0) * arrowSize,
                     -std::sin(angle + pi / 6.0) * arrowSize);

        const QPointF arrowP2 =
            end - QPointF(
                      std::cos(angle - pi / 6.0) * arrowSize,
                     -std::sin(angle - pi / 6.0) * arrowSize);

        QPolygonF head;
        head << end << arrowP1 << arrowP2;

        painter.setBrush(color);
        painter.drawPolygon(head);

        painter.setPen(color);
        painter.setFont(QFont("Sans Serif", 8, QFont::DemiBold));
        painter.drawText(
            end + QPointF(5.0, -4.0),
            label);
    }

    void drawMoon(
        QPainter& painter,
        const QPointF& center,
        double radius) const
    {
        painter.setPen(QPen(surface_, 1.5));
        painter.setBrush(QColor("#111F31"));
        painter.drawEllipse(center, radius, radius);

        painter.setPen(QPen(QColor("#2A405D"), 1.0));
        painter.drawArc(
            QRectF(
                center.x() - radius * 0.55,
                center.y() - radius * 0.45,
                radius * 0.8,
                radius * 0.55),
            20 * 16,
            210 * 16);

        painter.drawEllipse(
            center + QPointF(radius * 0.25, radius * 0.18),
            radius * 0.12,
            radius * 0.07);
    }

    void drawAxes(
        QPainter& painter,
        const QPointF& origin,
        const QString& xLabel = "+X",
        const QString& yLabel = "+Y",
        const QString& zLabel = "+Z") const
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(primary_);
        painter.drawEllipse(origin, 3.5, 3.5);

        drawArrow(
            painter,
            origin,
            origin + QPointF(48.0, 0.0),
            xLabel,
            accent_);

        drawArrow(
            painter,
            origin,
            origin + QPointF(0.0, -43.0),
            zLabel,
            QColor("#72A5FF"));

        drawArrow(
            painter,
            origin,
            origin + QPointF(-32.0, 28.0),
            yLabel,
            QColor("#89D4E8"));
    }

    void drawRotationArrow(
        QPainter& painter,
        const QPointF& center,
        double radius) const
    {
        painter.setPen(QPen(accent_, 1.8));
        painter.setBrush(Qt::NoBrush);

        const QRectF arcRect(
            center.x() - radius,
            center.y() - radius,
            radius * 2.0,
            radius * 2.0);

        painter.drawArc(arcRect, 35 * 16, 215 * 16);

        const QPointF end(
            center.x() - radius * 0.80,
            center.y() - radius * 0.60);

        QPolygonF head;
        head
            << end
            << end + QPointF(9.0, -1.0)
            << end + QPointF(4.0, 7.0);

        painter.setBrush(accent_);
        painter.drawPolygon(head);
    }

    void drawLander(
        QPainter& painter,
        const QPointF& center) const
    {
        painter.setPen(QPen(primary_, 1.5));
        painter.setBrush(QColor("#14263B"));

        QPolygonF body;
        body
            << center + QPointF(-15.0, -11.0)
            << center + QPointF(15.0, -11.0)
            << center + QPointF(11.0, 10.0)
            << center + QPointF(-11.0, 10.0);

        painter.drawPolygon(body);

        painter.drawLine(
            center + QPointF(-9.0, 10.0),
            center + QPointF(-20.0, 23.0));

        painter.drawLine(
            center + QPointF(9.0, 10.0),
            center + QPointF(20.0, 23.0));

        painter.drawLine(
            center + QPointF(-25.0, 23.0),
            center + QPointF(-15.0, 23.0));

        painter.drawLine(
            center + QPointF(15.0, 23.0),
            center + QPointF(25.0, 23.0));
    }

    void drawMCI(QPainter& painter) const
    {
        const QPointF moonCenter(72.0, 77.0);

        drawMoon(painter, moonCenter, 31.0);
        drawAxes(painter, moonCenter);

        painter.setPen(secondary_);
        painter.setFont(QFont("Sans Serif", 8));
        painter.drawText(
            QRectF(128.0, 87.0, 80.0, 30.0),
            Qt::AlignLeft | Qt::AlignTop,
            "inertial\nnon-rotating");
    }

    void drawMCMF(QPainter& painter) const
    {
        const QPointF moonCenter(72.0, 77.0);

        drawMoon(painter, moonCenter, 31.0);
        drawAxes(painter, moonCenter);
        drawRotationArrow(painter, moonCenter, 42.0);

        painter.setPen(secondary_);
        painter.setFont(QFont("Sans Serif", 8));
        painter.drawText(
            QRectF(128.0, 87.0, 80.0, 30.0),
            Qt::AlignLeft | Qt::AlignTop,
            "Moon-fixed\nrotating");
    }

    void drawMSC(QPainter& painter) const
    {
        painter.setPen(QPen(surface_, 2.0));
        painter.setBrush(QColor("#111F31"));

        const QRectF moonRect(24.0, 69.0, 168.0, 105.0);
        painter.drawEllipse(moonRect);

        const QPointF site(108.0, 72.0);

        painter.setPen(Qt::NoPen);
        painter.setBrush(accent_);
        painter.drawEllipse(site, 5.0, 5.0);

        painter.setPen(QPen(accent_, 1.5, Qt::DashLine));
        painter.drawLine(site, QPointF(108.0, 36.0));

        painter.setPen(primary_);
        painter.setFont(QFont("Sans Serif", 8, QFont::DemiBold));
        painter.drawText(QPointF(116.0, 40.0), "alt");

        painter.setPen(secondary_);
        painter.setFont(QFont("Sans Serif", 8));
        painter.drawText(QPointF(37.0, 55.0), "lat / lon");
        painter.drawText(QPointF(119.0, 76.0), "surface point");
    }

    void drawENU(QPainter& painter) const
    {
        const QPointF site(105.0, 91.0);

        painter.setPen(QPen(surface_, 2.0));
        painter.drawArc(
            QRectF(22.0, 77.0, 170.0, 100.0),
            15 * 16,
            150 * 16);

        painter.setPen(Qt::NoPen);
        painter.setBrush(primary_);
        painter.drawEllipse(site, 4.0, 4.0);

        drawArrow(
            painter,
            site,
            site + QPointF(55.0, 0.0),
            "East",
            accent_);

        drawArrow(
            painter,
            site,
            site + QPointF(0.0, -52.0),
            "Up",
            QColor("#72A5FF"));

        drawArrow(
            painter,
            site,
            site + QPointF(-39.0, 25.0),
            "North",
            QColor("#89D4E8"));

        painter.setPen(secondary_);
        painter.setFont(QFont("Sans Serif", 8));
        painter.setPen(secondary_);
        painter.setFont(QFont("Sans Serif", 8));

        painter.drawText(
            QRectF(118.0, 112.0, 90.0, 22.0),
            Qt::AlignLeft | Qt::AlignVCenter,
            "landing site");
    }

    void drawLVLH(QPainter& painter) const
    {
        painter.setPen(QPen(surface_, 2.0));
        painter.drawArc(
            QRectF(13.0, 95.0, 205.0, 112.0),
            15 * 16,
            150 * 16);

        const QPointF vehicle(91.0, 58.0);
        drawLander(painter, vehicle);

        drawArrow(
            painter,
            vehicle,
            vehicle + QPointF(55.0, -18.0),
            "+X Fwd",
            accent_);

        drawArrow(
            painter,
            vehicle,
            vehicle + QPointF(46.0, 24.0),
            "+Y Right",
            QColor("#89D4E8"));

        drawArrow(
            painter,
            vehicle,
            vehicle + QPointF(0.0, 58.0),
            "+Z Down",
            QColor("#72A5FF"));
    }

    void drawSBF(QPainter& painter) const
    {
        const QPointF vehicle(91.0, 68.0);
        drawLander(painter, vehicle);

        painter.setPen(Qt::NoPen);
        painter.setBrush(primary_);
        painter.drawEllipse(vehicle, 3.5, 3.5);

        drawArrow(
            painter,
            vehicle,
            vehicle + QPointF(57.0, 0.0),
            "+X Fwd",
            accent_);

        drawArrow(
            painter,
            vehicle,
            vehicle + QPointF(38.0, -32.0),
            "+Y Right",
            QColor("#89D4E8"));

        drawArrow(
            painter,
            vehicle,
            vehicle + QPointF(0.0, 55.0),
            "+Z Down",
            QColor("#72A5FF"));

        painter.setPen(secondary_);
        painter.setFont(QFont("Sans Serif", 8));
        painter.setPen(secondary_);
        painter.setFont(QFont("Sans Serif", 8));

        painter.drawText(
            QRectF(12.0, 124.0, 155.0, 22.0),
            Qt::AlignRight | Qt::AlignVCenter,
            "rigidly attached to spacecraft");
    }
};

} // namespace

CoordinateHelpPage::CoordinateHelpPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

void CoordinateHelpPage::setupUI()
{
    setStyleSheet(
        "QWidget { "
        "background-color: #07111F; "
        "color: #E6EEF8; "
        "}"

        "QScrollArea { "
        "border: none; "
        "background-color: #07111F; "
        "}"

        "QFrame#ReferenceFrame { "
        "background-color: #0E1A2B; "
        "border: 1px solid #223A5E; "
        "border-radius: 10px; "
        "}"

        "QLabel#TitleLabel { "
        "font-size: 32px; "
        "font-weight: 700; "
        "color: #E6EEF8; "
        "}"

        "QLabel#SubtitleLabel { "
        "font-size: 15px; "
        "color: #A8BACD; "
        "}"

        "QLabel#FrameName { "
        "font-size: 20px; "
        "font-weight: 700; "
        "color: #38BDF8; "
        "}"

        "QLabel#FrameTitle { "
        "font-size: 14px; "
        "font-weight: 600; "
        "color: #E6EEF8; "
        "}"

        "QLabel#FrameText { "
        "font-size: 13px; "
        "color: #C7D3E0; "
        "}"

        "QLabel#NoteLabel { "
        "font-size: 13px; "
        "color: #8FA4BA; "
        "}"
        );

    auto *pageLayout = new QVBoxLayout(this);
    pageLayout->setContentsMargins(0, 0, 0, 0);

    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    auto *content = new QWidget(scrollArea);
    auto *layout = new QVBoxLayout(content);

    layout->setContentsMargins(40, 30, 40, 40);
    layout->setSpacing(22);

    auto *title = new QLabel("COORDINATE FRAME REFERENCE", content);
    title->setObjectName("TitleLabel");

    auto *subtitle = new QLabel(
        "Compact reference for the coordinate systems used by SDF navigation, "
        "physics, guidance, telemetry and spacecraft dynamics.",
        content);
    subtitle->setObjectName("SubtitleLabel");
    subtitle->setWordWrap(true);

    layout->addWidget(title);
    layout->addWidget(subtitle);

    // =====================================================
    // Frame definitions
    // =====================================================

    auto *definitionsTitle = new QLabel("FRAME DEFINITIONS", content);
    definitionsTitle->setObjectName("FrameName");
    layout->addWidget(definitionsTitle);

    auto *grid = new QGridLayout();
    grid->setHorizontalSpacing(16);
    grid->setVerticalSpacing(16);
    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(1, 1);

    grid->addWidget(
        createFrameCard(
            "MCI",
            "Moon-Centered Inertial Frame",
            "Authoritative translational physics and integration frame. "
            "It is centered on the Moon and does not rotate with the lunar surface.",
            "Cartesian: x, y, z",
            "Moon center"),
        0, 0);

    grid->addWidget(
        createFrameCard(
            "MCMF",
            "Moon-Centered Moon-Fixed Frame",
            "Global Cartesian lunar frame that rotates with the Moon and remains "
            "fixed relative to the lunar surface.",
            "Cartesian: x, y, z",
            "Moon center"),
        0, 1);

    grid->addWidget(
        createFrameCard(
            "MSC",
            "Moon Surface Coordinates",
            "Surface / geodetic representation used to describe fixed lunar "
            "locations such as landing sites and mission targets.",
            "Latitude, longitude, altitude",
            "Surface location representation; not a Cartesian frame"),
        1, 0);

    grid->addWidget(
        createFrameCard(
            "ENU",
            "East-North-Up Frame",
            "Local tangent frame used for landing guidance, local navigation "
            "and surface-relative telemetry.",
            "+X East, +Y North, +Z Up",
            "Configured local reference point, typically the landing site"),
        1, 1);

    grid->addWidget(
        createFrameCard(
            "LVLH",
            "Local Vertical Local Horizontal Frame",
            "Orbital-derived spacecraft-local frame constructed from the MCI "
            "position and velocity. SDF uses the Forward-Right-Down convention.",
            "+X Forward, +Y Right, +Z Down",
            "Spacecraft center of mass"),
        2, 0);

    grid->addWidget(
        createFrameCard(
            "SBF",
            "Spacecraft Body Frame",
            "Rigidly attached spacecraft frame used by propulsion, RCS, attitude "
            "dynamics, inertia, torque calculations and onboard sensors.",
            "+X Forward, +Y Right, +Z Down",
            "Spacecraft body reference point"),
        2, 1);

    layout->addLayout(grid);

    auto *architectureNote = new QLabel(
        "Architecture note: MCI remains the simulation truth frame. Other "
        "representations are derived through the backend CoordinateTransformer. "
        "The frontend performs no independent coordinate transformation.",
        content);
    architectureNote->setObjectName("NoteLabel");
    architectureNote->setWordWrap(true);

    layout->addWidget(architectureNote);
    layout->addStretch();

    scrollArea->setWidget(content);
    pageLayout->addWidget(scrollArea);
}

QFrame *CoordinateHelpPage::createFrameCard(
    const QString& frameName,
    const QString& title,
    const QString& description,
    const QString& coordinates,
    const QString& origin)
{
    auto *frame = new QFrame(this);
    frame->setObjectName("ReferenceFrame");

    auto *cardLayout = new QHBoxLayout(frame);
    cardLayout->setContentsMargins(18, 16, 18, 16);
    cardLayout->setSpacing(18);

    auto *textWidget = new QWidget(frame);
    textWidget->setStyleSheet("background-color: transparent;");

    auto *textLayout = new QVBoxLayout(textWidget);
    textLayout->setContentsMargins(0, 0, 0, 0);
    textLayout->setSpacing(7);

    auto *nameLabel = new QLabel(frameName, textWidget);
    nameLabel->setObjectName("FrameName");

    auto *titleLabel = new QLabel(title, textWidget);
    titleLabel->setObjectName("FrameTitle");
    titleLabel->setWordWrap(true);

    auto *descriptionLabel = new QLabel(description, textWidget);
    descriptionLabel->setObjectName("FrameText");
    descriptionLabel->setWordWrap(true);

    auto *coordinatesLabel = new QLabel(
        QString("Coordinates: %1").arg(coordinates),
        textWidget);
    coordinatesLabel->setObjectName("FrameText");
    coordinatesLabel->setWordWrap(true);

    auto *originLabel = new QLabel(
        QString("Origin / reference: %1").arg(origin),
        textWidget);
    originLabel->setObjectName("FrameText");
    originLabel->setWordWrap(true);

    textLayout->addWidget(nameLabel);
    textLayout->addWidget(titleLabel);
    textLayout->addWidget(descriptionLabel);
    textLayout->addWidget(coordinatesLabel);
    textLayout->addWidget(originLabel);
    textLayout->addStretch();

    QWidget *diagram = createFrameDiagram(frameName);

    cardLayout->addWidget(textWidget, 3);
    cardLayout->addWidget(diagram, 2, Qt::AlignVCenter);

    return frame;
}

QWidget *CoordinateHelpPage::createFrameDiagram(
    const QString& frameName)
{
    return new CoordinateFrameDiagram(frameName, this);
}
