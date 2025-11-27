/**
 * @file mainWindow.h
 * @brief Main GUI window for the lunar landing simulator application.
 *
 * The homepage class manages the central GUI of the application.
 * It provides navigation between different pages:
 * -
 *
 * It uses QStackedWidget to switch between pages.
 *
 * @author Schendzielorz Gerd
 * @date 2025-11-27
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>

/**
 * @class MainWindow
 * @brief Main window of the application.
 *
 * Inherits from QMainWindow. Handles GUI setup and page navigation.
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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // Member Vars
    QVBoxLayout *vLayout;

    // Member function---------------------------------

    /**
     * @brief creates homepage as a stacked widget
     * @param stackedWidget Homepage will be created with stacked widgets
    */
    QWidget* createHomepage(QStackedWidget *stackedWidget);

};
#endif // MAINWINDOW_H
