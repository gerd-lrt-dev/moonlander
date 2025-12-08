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
#include <QVBoxLayout>
#include <QWidget>

/**
 * @class Homepage
 * @brief Main landing page / starting interface of the application.
 *
 * Inherits from QWidget. Contains buttons to navigate to the simulation
 * or other setup/configuration pages.
 */
class Homepage : public QWidget
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

signals:
    /**
     * @brief Signal emitted to request the start of the simulation.
     */
    void requestStartOfSimulation();

private:
    // ==========================================
    // Member Variables
    // ==========================================
    QVBoxLayout *vHomepageLayout;        ///< Layout for arranging homepage widgets vertically

    // Buttons
    QPushButton *selectSpacecraftButton; ///< Opens spacecraft selection dialog
    QPushButton *startSimulationButton;  ///< Starts the simulation

    // ==========================================
    // Member Functions
    // ==========================================
    void setupUI(); ///< Sets up UI elements and layouts for the homepage
    void setupConnections(); ///< Connects signals and slots for the GUI
};

#endif // HOMEPAGE_H
