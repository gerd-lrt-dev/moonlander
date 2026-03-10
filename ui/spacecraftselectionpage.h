#pragma once

#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>

#include "configmanager.h"

class SpacecraftSelectionPage : public QWidget
{
    Q_OBJECT

public:
    explicit SpacecraftSelectionPage(ConfigManager& configManager,
                                     QWidget *parent = nullptr);

signals:
    void spacecraftSelected(QString spacecraft);
    void backRequested();

public slots:
    void receiveJsonConfigStr();

private slots:
    void onSelectClicked();
    void onRowChanged(int row);

private:

    // ==========================================
    // Member Variables
    // ==========================================

    ConfigManager& configManager_;

    // QT UI Objects
    QListWidget *listWidget;
    QLabel      *nameLabel;
    QPushButton *selectButton;
    QPushButton *backButton;
    QTextEdit   *detailsView;

    // ==========================================
    // Member Functions
    // ==========================================

    void setupUI();
    void setupConnections();
    void populateList();
};
