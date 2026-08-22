#include "coordinatehelppage.h"

#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>
#include <QScrollArea>
#include <QVBoxLayout>

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
        "Reference for the coordinate systems used by SDF navigation, "
        "physics, guidance, telemetry and spacecraft dynamics.",
        content);
    subtitle->setObjectName("SubtitleLabel");
    subtitle->setWordWrap(true);

    layout->addWidget(title);
    layout->addWidget(subtitle);

    // =====================================================
    // Master frame overview
    // =====================================================

    auto *overviewFrame = new QFrame(content);
    overviewFrame->setObjectName("ReferenceFrame");

    auto *overviewLayout = new QVBoxLayout(overviewFrame);
    overviewLayout->setContentsMargins(18, 18, 18, 18);
    overviewLayout->setSpacing(12);

    auto *overviewTitle = new QLabel("SDF FRAME OVERVIEW", overviewFrame);
    overviewTitle->setObjectName("FrameName");

    auto *overviewText = new QLabel(
        "The diagram distinguishes the inertial physics frame, the rotating "
        "Moon-fixed frame, lunar surface coordinates, local landing and "
        "orbital frames, and the spacecraft body frame. Transformation and "
        "frame-construction paths are shown separately where applicable.",
        overviewFrame);
    overviewText->setObjectName("FrameText");
    overviewText->setWordWrap(true);

    auto *imageLabel = new QLabel(overviewFrame);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet(
        "background-color: #FFFFFF; "
        "border: 1px solid #223A5E; "
        "border-radius: 6px;"
        );

    const QPixmap overviewPixmap(":/help/Frames_convention.png");

    if (!overviewPixmap.isNull())
    {
        imageLabel->setPixmap(
            overviewPixmap.scaledToWidth(
                1300,
                Qt::SmoothTransformation));
    }
    else
    {
        imageLabel->setText("Coordinate frame overview image could not be loaded.");
        imageLabel->setMinimumHeight(120);
    }

    overviewLayout->addWidget(overviewTitle);
    overviewLayout->addWidget(overviewText);
    overviewLayout->addWidget(imageLabel, 0, Qt::AlignHCenter);

    layout->addWidget(overviewFrame);

    // =====================================================
    // Frame definitions
    // =====================================================

    auto *definitionsTitle = new QLabel("FRAME DEFINITIONS", content);
    definitionsTitle->setObjectName("FrameName");
    layout->addWidget(definitionsTitle);

    auto *grid = new QGridLayout();
    grid->setHorizontalSpacing(16);
    grid->setVerticalSpacing(16);

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
            "Spacecraft / orbital state"),
        2, 0);

    grid->addWidget(
        createFrameCard(
            "SBF",
            "Spacecraft Body Frame",
            "Rigidly attached spacecraft frame used by propulsion, RCS, attitude "
            "dynamics, inertia, torque calculations and onboard sensors.",
            "+X Forward, +Y Right, +Z Down",
            "Spacecraft center of mass"),
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

    auto *layout = new QVBoxLayout(frame);
    layout->setContentsMargins(18, 16, 18, 16);
    layout->setSpacing(7);

    auto *nameLabel = new QLabel(frameName, frame);
    nameLabel->setObjectName("FrameName");

    auto *titleLabel = new QLabel(title, frame);
    titleLabel->setObjectName("FrameTitle");
    titleLabel->setWordWrap(true);

    auto *descriptionLabel = new QLabel(description, frame);
    descriptionLabel->setObjectName("FrameText");
    descriptionLabel->setWordWrap(true);

    auto *coordinatesLabel = new QLabel(
        QString("Coordinates: %1").arg(coordinates),
        frame);
    coordinatesLabel->setObjectName("FrameText");
    coordinatesLabel->setWordWrap(true);

    auto *originLabel = new QLabel(
        QString("Origin / reference: %1").arg(origin),
        frame);
    originLabel->setObjectName("FrameText");
    originLabel->setWordWrap(true);

    layout->addWidget(nameLabel);
    layout->addWidget(titleLabel);
    layout->addWidget(descriptionLabel);
    layout->addWidget(coordinatesLabel);
    layout->addWidget(originLabel);
    layout->addStretch();

    return frame;
}
