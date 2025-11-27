/**
 * @file mainWindow.h
 * @brief Main GUI window for the Lunar Lander simulator application.
 *
 * The MainWindow class manages the central GUI of the application.
 * It provides navigation between different pages of the simulator using
 * a QStackedWidget. The main pages include:
 * - Homepage
 * - Simulation view
 * - Settings and configuration
 *
 * The class also handles screen detection and layout adjustments to
 * ensure proper scaling on different monitors.
 *
 * @author Schendzielorz Gerd
 * @date 2025-11-27
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QPixmap>
#include <QRect>
#include <QScreen>
#include <QShowEvent>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QWindow>


/**
 * @class MainWindow
 * @brief Main application window for the Lunar Lander simulator.
 *
 * Inherits from QMainWindow. This class is responsible for:
 * - Creating and managing the central GUI layout
 * - Handling navigation between different pages via QStackedWidget
 * - Detecting screen size and adjusting layouts accordingly
 */

/**
 * @brief Constructor.
 * @param parent Optional parent widget.
 *
 * Currently trivial
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for MainWindow.
     * @param parent Optional parent widget. Defaults to nullptr.
     *
     * Initializes the main window and prepares the GUI layout.
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor for MainWindow.
     *
     * Cleans up allocated resources.
     */
    ~MainWindow();

private:
    // ==========================================
    // Member Variables
    // ==========================================
    QVBoxLayout *vLayout;///< Main vertical layout for the central widget

    // ==========================================
    // Member Functions
    // ==========================================

    /**
     * @brief Creates the application's homepage as part of a QStackedWidget layout.
     *
     * This function initializes and configures the homepage of the Lunar Lander
     * simulation. The homepage serves as the central navigation interface,
     * connecting to simulation views, settings, and other sections. The created
     * widget is added to the provided QStackedWidget.
     *
     * @param stackedWidget Pointer to the QStackedWidget that will contain
     *                      the homepage.
     * @return QWidget* Pointer to the newly created homepage widget.
     */
    QWidget* createHomepage(QStackedWidget *stackedWidget);

protected:
    /**
     * @brief Handles the show event to detect the current screen and adjust window size.
     *
     * This reimplementation of the QWidget/QMainWindow showEvent is automatically called
     * by Qt when the window becomes visible (after show() is invoked). It is used to
     * retrieve the screen on which the window is currently displayed and its available
     * geometry, so that the UI elements can be scaled appropriately.
     *
     * Note: windowHandle() must be valid when accessing the screen. This method is
     * typically preferred over accessing the primary screen directly, especially when
     * working with multi-monitor setups.
     *
     * In the Lunar Lander simulation, this ensures that the main window and homepage
     * layout are proportionally scaled according to the actual screen size.
     *
     * @param event Pointer to the QShowEvent triggered when the widget is shown.
     */
    void showEvent(QShowEvent *event) override;


};
#endif // MAINWINDOW_H
