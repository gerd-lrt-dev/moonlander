#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Build central Widget
    QWidget *centralWidget = new QWidget(this);

    // Build vertical layout box
    vLayout = new QVBoxLayout(centralWidget);

    // Build stackedWidget
    QStackedWidget *stackedWidget = new QStackedWidget(this);

    //Create pages
    QWidget *homepageWidget = createHomepage(stackedWidget);

    // Add pages to stackedWidget
    stackedWidget->addWidget(homepageWidget);

    // Add StackedWidget to layout
    vLayout->addWidget(stackedWidget);

    // Contribute centralWidget to MainWIndow
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow()
{

}

QWidget* MainWindow::createHomepage(QStackedWidget *stackedWidget)
{
    // Build widget object (return value)
    QWidget *homepage = new QWidget(this);

    // Add title
    QLabel *title = new QLabel("Lunar Lander Simulation", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 32px;"
                         "font-weight: bold;");

    // Add logo
    QLabel *logoLabel   = new QLabel(this);
    QPixmap logoPixmap(":/new/logo/Resources/Images/logo.png");
    logoPixmap = logoPixmap.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logoLabel->setPixmap(logoPixmap);
    logoLabel->setAlignment(Qt::AlignCenter);

    // Add widgets to Layout

    vLayout->addWidget(title);
    vLayout->addSpacing(15);
    vLayout->addWidget(logoLabel);
    vLayout->setAlignment(Qt::AlignTop);
    vLayout->setAlignment(Qt::AlignVCenter);

    return homepage;
}
