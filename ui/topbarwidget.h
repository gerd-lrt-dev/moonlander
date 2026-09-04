#pragma once

#include <QWidget>

class QPushButton;

class TopBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TopBarWidget(QWidget *parent = nullptr);

signals:
    void homeRequested();
    void selectSpacecraftRequested();
    void startSimulationRequested();
    void controlsHelpRequested();
    void coordinateHelpRequested();
    void settingsRequested();

private:
    QPushButton *homeButton = nullptr;
    QPushButton *selectSpacecraftButton = nullptr;
    QPushButton *startSimulationButton = nullptr;
    QPushButton *controlsHelpButton = nullptr;
    QPushButton *coordinateHelpButton = nullptr;
    QPushButton *settingsButton = nullptr;
};
