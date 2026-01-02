/**
 * @file homepage.h
 * @brief First page of the Lunar Lander simulator.
 *
 * The Homepage class provides the initial navigation interface of the
 * application. Users can select spacecraft, access settings, or start
 * the simulation from here.
 */

#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QThread>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>

#include "cockpitpage.h"
#include "simulationworker.h"

/**
 * @class Homepage
 * @brief Main landing page / starting interface of the application.
 *
 * Inherits from QWidget. Contains buttons to navigate to the simulation
 * or other setup/configuration pages.
 */
class Homepage : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for the Homepage.
     * @param parent Optional parent widget.
     *
     * Initializes UI elements and layouts for the landing page.
     */
    explicit Homepage(QWidget *parent = nullptr);
    ~Homepage();

signals:
    /**
     * @brief Signal emitted to request the start of the simulation.
     */
    void requestStartOfSimulation();

private:
    // ==========================================
    // Member Variables
    // ==========================================
    // Pages
    cockpitPage *cockpit;

    // Widgets
    QWidget         *centralWidget;
    QStackedWidget  *stackedWidget;

    // Layouts
    QVBoxLayout *vHomepageLayout;        ///< Layout for arranging homepage widgets vertically

    // Buttons
    QPushButton *selectSpacecraftButton; ///< Opens spacecraft selection dialog
    QPushButton *startSimulationButton;  ///< Starts the simulation

    // Threads
    QThread *simulationThread;  ///< Thread running the simulation
    SimulationWorker *simulationWorker; ///< Worker object for simlation

    // ==========================================
    // Member Functions
    // ==========================================
    void setupStackedWidget(); ///< Sets up stacked widget and build homepage as central widget
    void setupConnections(); ///< Connects signals and slots for the GUI
    void setupThread();

    /**
    * @brief createHomePage & Sets up UI elements and layouts for the homepage
        * @param stackedWidget
        * @return homepageWidget
    */
    QWidget* createHomePage(QStackedWidget *stackedWidget);
};

#endif // HOMEPAGE_H
