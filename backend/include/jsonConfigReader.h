#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

#include "spacecraft.h"
#include "vector3.h"

/**
 * @class JsonConfigReader
 * @brief Utility class for loading and parsing JSON configuration files for Moonlander.
 *
 * This class provides static helper functions to load JSON files from disk and
 * convert them into fully initialized @ref customSpacecraft structures.  
 * It acts purely as a utility class and therefore holds no internal state.
 *
 * The constructor and destructor are intentionally empty and not meant to be used.
 */
class jsonConfigReader {
public:

    /**
     * @brief Default constructor.
     *
     * This class is designed as a purely static utility helper.  
     * The constructor performs no action and should generally not be used.
     */
    jsonConfigReader() = default;

    /**
     * @brief Default destructor.
     *
     * JsonConfigReader manages no resources;  
     * therefore, the destructor performs no cleanup.
     */
    ~jsonConfigReader() = default;

    /**
     * @brief Loads a JSON configuration file from disk.
     *
     * Opens the file at the provided path and reads its full JSON content.  
     * The result is returned as an @ref nlohmann::json object.
     *
     * @param filename Path to the JSON file (e.g. `"configs/lander.json"`).
     *
     * @return Parsed JSON object containing all data from the file.
     *
     * @throws std::runtime_error If the file cannot be opened or read.
     *
     * @note This function only loads the raw JSON document.  
     *       It does not interpret or validate its contents.
     */
    static nlohmann::json loadConfig(const std::string& filename);

    /**
     * @brief Parses a JSON object containing spacecraft configuration data.
     *
     * This function extracts all required lander parameters from the JSON object
     * and constructs a fully initialized @ref customSpacecraft instance.
     *
     * Example of the expected JSON structure:
     * @code
     * {
     *   "m": 1100.0,
     *   "fuelM": 800.0,
     *   "maxFuelM": 800.0,
     *   "maxT": 15000.0,
     *   "Isp": 300.0,
     *   "timeConstant": 0.15,
     *   "B_mainThrustDirection": { "x": 0, "y": 0, "z": -1 },
     *   "B_mainThrustPosition":  { "x": 0, "y": 0, "z": 0.5 },
     *   "Ixx": 250.0,
     *   "Iyy": 250.0,
     *   "Izz": 450.0,
     *   "B_initialPos": { "x": 0, "y": 0, "z": 1200 },
     *   "B_initialRot": { "x": 0, "y": 0, "z": 0   },
     *   "B_centerOfMass": { "x": 0, "y": 0, "z": -0.2 }
     * }
     * @endcode
     *
     * @param j JSON object containing spacecraft configuration data.
     *
     * @return A fully initialized @ref customSpacecraft instance.
     *
     * @throws nlohmann::json::exception  
     *         If required fields are missing or have incorrect types.
     */
    static customSpacecraft parseLander(const nlohmann::json& j);
};
