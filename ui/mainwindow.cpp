#include "mainwindow.h"
#include "uibuilder.h"

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
    // Build instance for ui building class
    UIBuilder uiBuilder;

    // Build widget object (return value)
    QWidget *homepage = new QWidget(this);

    // Add title
    QLabel *title = uiBuilder.createPageTitle("Lunar Lander Simulation", this);

    // Add logo
    QLabel *logoLabel   = new QLabel(this);
    QPixmap logoPixmap(":/new/logo/Resources/Images/logo.png");
    logoPixmap = logoPixmap.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logoLabel->setPixmap(logoPixmap);
    logoLabel->setAlignment(Qt::AlignCenter);

    // Add copryright
    QLabel *copyright = uiBuilder.createCopyright("© gerd-lrt-dev", this);

    // Add Buttons
    QPushButton *selectSpacecraftButton = uiBuilder.createButton("Select Spacecraft", this);
    QPushButton *startSimulationButton = uiBuilder.createButton("Start Simulation", this);

    // Add widgets to Layout
    vLayout->addSpacing(30);
    vLayout->addWidget(title);
    vLayout->addSpacing(15);
    vLayout->addWidget(logoLabel);
    vLayout->addSpacing(15);
    vLayout->addWidget(copyright);
    vLayout->addSpacing(15);
    vLayout->addWidget(selectSpacecraftButton, 0, Qt::AlignHCenter);
    vLayout->addSpacing(15);
    vLayout->addWidget(startSimulationButton, 0, Qt::AlignHCenter);
    vLayout->setAlignment(Qt::AlignTop);
    vLayout->setAlignment(Qt::AlignCenter);

    return homepage;
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    if (!windowHandle())
    {
        return;
    }

    QScreen *screen = windowHandle()->screen();

    if(!screen)
    {
        return;
    }

    QRect screenGeometry = screen->availableGeometry();
    resize(screenGeometry.width() * 0.7, screenGeometry.height() * 0.7);
}
