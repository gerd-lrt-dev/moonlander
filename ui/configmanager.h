#pragma once

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

/**
 * @class ConfigManager
 * @brief Handles loading and parsing of spacecraft configuration files.
 *
 * The ConfigManager is responsible for reading spacecraft configuration
 * data from a JSON file, parsing its contents, and providing convenient
 * access methods to retrieve individual spacecraft configurations.
 *
 * The expected JSON structure is:
 *
 * @code
 * {
 *   "spacecraft": [
 *     { ... spacecraft configuration ... }
 *   ]
 * }
 * @endcode
 *
 * After successfully loading a configuration file, the manager emits
 * the signal jsonLoaded() to notify interested UI components or systems
 * that configuration data is available.
 */
class ConfigManager : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief Constructs a ConfigManager instance.
     *
     * @param parent Optional Qt parent object.
     */
    explicit ConfigManager(QObject *parent = nullptr);

    /**
     * @brief Loads and parses a spacecraft configuration file.
     *
     * The file is read from disk and parsed into internal JSON structures.
     * The spacecraft array is extracted and stored internally.
     *
     * If loading succeeds, the signal jsonLoaded() is emitted.
     *
     * @param path Path to the JSON configuration file.
     * @return true if the file was successfully loaded and parsed.
     * @return false if the file could not be opened or parsed.
     */
    bool loadConfig(const QString& path);

    /**
     * @brief Returns the number of spacecraft entries in the configuration.
     *
     * @return Number of spacecraft objects currently stored.
     */
    int spacecraftCount() const;

    /**
     * @brief Returns the name of a spacecraft at a given index.
     *
     * @param index Index of the spacecraft entry.
     * @return Spacecraft name as stored in the JSON configuration.
     */
    QString spacecraftName(int index) const;

    /**
     * @brief Returns the JSON object describing a spacecraft.
     *
     * @param index Index of the spacecraft entry.
     * @return QJsonObject containing the spacecraft configuration.
     */
    QJsonObject spacecraftObject(int index) const;

    /**
     * @brief Returns a spacecraft configuration as a compact JSON string.
     *
     * This is typically used when sending the configuration to the
     * simulation backend or worker threads.
     *
     * @param index Index of the spacecraft entry.
     * @return Compact JSON representation of the spacecraft.
     */
    QString spacecraftJson(int index) const;

    /**
     * @brief Returns the default spacecraft configuration.
     *
     * Usually the first spacecraft entry in the configuration file.
     * This is used when no explicit spacecraft selection was made.
     *
     * @return JSON string containing the default spacecraft configuration.
     */
    QString defaultSpacecraftJson() const;

signals:

    /**
     * @brief Emitted when a configuration file has been successfully loaded.
     *
     * This signal allows UI components (e.g. spacecraft selection pages)
     * to refresh their data once the configuration becomes available.
     */
    void jsonLoaded();

private:

    /**
     * @brief Raw JSON configuration string loaded from file.
     */
    QString rawJson;

    /**
     * @brief Parsed list of spacecraft configuration objects.
     *
     * Each entry represents one spacecraft configuration extracted
     * from the JSON file.
     */
    QList<QJsonObject> spacecraftList;
};
