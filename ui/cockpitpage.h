/**
 * @file cockpitpage.h
 * @brief Cockpit / simulation view page of the Lunar Lander simulator.
 *
 * The cockpitPage class contains control panels, LCD displays for spacecraft
 * data, and a 2D landing simulation viewer.
 */

#ifndef COCKPITPAGE_H
#define COCKPITPAGE_H

#include <QWidget>
#include <QLCDNumber>

/**
 * @class cockpitPage
 * @brief Main simulation interface page.
 *
 * Inherits from QWidget. Responsible for displaying the spacecraft cockpit,
 * including all control panels, status displays, and the 2D landing visualization.
 */
class cockpitPage : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for cockpitPage.
     * @param parent Optional parent widget.
     *
     * Initializes UI elements and layouts for the simulation cockpit.
     */
    explicit cockpitPage(QWidget *parent = nullptr);

private:
    // ==========================================
    // Member Functions
    // ==========================================
    void setupUI();           ///< Sets up cockpit UI elements and layouts
    void setupConnections();  ///< Connects signals and slots for cockpit controls
};

#endif // COCKPITPAGE_H
