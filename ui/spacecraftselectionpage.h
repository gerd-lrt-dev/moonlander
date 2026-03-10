#pragma once

#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>

#include "configmanager.h"

/**
 * @class SpacecraftSelectionPage
 * @brief UI page that allows the user to select a spacecraft configuration.
 *
 * This widget presents a list of available spacecraft loaded by the
 * ConfigManager. The user can browse the list, inspect the configuration
 * details, and select one spacecraft for the simulation.
 *
 * The page emits signals when:
 * - a spacecraft has been selected
 * - the user wants to return to the previous page
 *
 * The actual spacecraft data is managed by the ConfigManager, which is
 * provided to the page via dependency injection.
 */
class SpacecraftSelectionPage : public QWidget
{
    Q_OBJECT

public:

    /**
     * @brief Constructs the spacecraft selection page.
     *
     * The page receives a reference to the ConfigManager, which provides
     * access to all available spacecraft configurations.
     *
     * @param configManager Reference to the configuration manager.
     * @param parent Optional Qt parent widget.
     */
    explicit SpacecraftSelectionPage(ConfigManager& configManager,
                                     QWidget *parent = nullptr);

signals:

    /**
     * @brief Emitted when the user selects a spacecraft.
     *
     * The selected spacecraft configuration is emitted as a compact
     * JSON string so it can be forwarded to the simulation backend.
     *
     * @param spacecraft JSON string representing the selected spacecraft.
     */
    void spacecraftSelected(QString spacecraft);

    /**
     * @brief Emitted when the user requests to return to the previous page.
     */
    void backRequested();

public slots:

    /**
     * @brief Slot triggered when a JSON configuration has been loaded.
     *
     * This slot is typically connected to ConfigManager::jsonLoaded().
     * Once triggered, the spacecraft list is populated from the
     * configuration data stored in the ConfigManager.
     */
    void receiveJsonConfigStr();

private slots:

    /**
     * @brief Handles the "Select" button click.
     *
     * Retrieves the currently selected spacecraft and emits the
     * spacecraftSelected() signal.
     */
    void onSelectClicked();

    /**
     * @brief Triggered when the selected row in the spacecraft list changes.
     *
     * Updates the spacecraft name label and displays the detailed
     * configuration JSON in the details view.
     *
     * @param row Index of the newly selected spacecraft entry.
     */
    void onRowChanged(int row);

private:

    // ==========================================
    // Member Variables
    // ==========================================

    /**
     * @brief Reference to the central configuration manager.
     *
     * Provides access to spacecraft configuration data.
     */
    ConfigManager& configManager_;

    /**
     * @brief List widget displaying available spacecraft names.
     */
    QListWidget *listWidget;

    /**
     * @brief Label displaying the currently selected spacecraft name.
     */
    QLabel *nameLabel;

    /**
     * @brief Button used to confirm spacecraft selection.
     */
    QPushButton *selectButton;

    /**
     * @brief Button used to return to the previous page.
     */
    QPushButton *backButton;

    /**
     * @brief Text view displaying detailed spacecraft configuration data.
     */
    QTextEdit *detailsView;

    // ==========================================
    // Member Functions
    // ==========================================

    /**
     * @brief Creates and arranges all UI elements.
     */
    void setupUI();

    /**
     * @brief Establishes signal-slot connections for UI interaction.
     */
    void setupConnections();

    /**
     * @brief Populates the spacecraft list using data from ConfigManager.
     */
    void populateList();
};
