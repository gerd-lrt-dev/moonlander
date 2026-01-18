#include "simcontrol.h"
#include "logger.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <stdexcept>

//***********************************************************
//*************        Private                   ************
//***********************************************************
void simcontrol::buildSimulationEnvironment(double t)
{
    // Instance classes
    landerSpacecraft    = std::make_unique<spacecraft>(landerMoon1);
}

// Function is needed when UI is working
customSpacecraft simcontrol::loadSpacecraftFromJsonString(const std::string& jsonString, const std::string& spacecraftName)
{
    nlohmann::json config;

    try
    {
        config = nlohmann::json::parse(jsonString);
    }
    catch (const nlohmann::json::parse_error& e)
    {
        throw std::runtime_error(
            std::string("JSON parse error: ") + e.what()
            );
    }

    // Validate root structure
    if (!config.contains("spacecraft") || !config["spacecraft"].is_array())
    {
        throw std::runtime_error("Invalid JSON: missing 'spacecraft' array");
    }

    const auto& spacecraftArray = config["spacecraft"];

    std::cout << "Spacecraft array size: " << spacecraftArray.size() << std::endl;
    for (size_t i = 0; i < spacecraftArray.size(); ++i)
    {
        std::cout << "Entry " << i << ": " << spacecraftArray[i].dump() << std::endl;
    }

    // Search spacecraft
    for (const auto& sc : spacecraftArray)
    {
        if (!sc.contains("name"))
            continue;

        if (sc["name"].get<std::string>() == spacecraftName)
        {
            std::cout << "simcontrol[loadSpacecraftFromJsonString]: Succesfully parsed config string" << std::endl;
            return jsonConfigReader::parseLander(sc);
        }
    }

    throw std::runtime_error(
        "Spacecraft '" + spacecraftName + "' not found in provided JSON"
        );

}

//***********************************************************
//*************        Pubblic                   ************
//***********************************************************

simcontrol::~simcontrol()
{

}

void simcontrol::initialize(const std::string& jsonConfigStr)
{
    landerMoon1 = loadSpacecraftFromJsonString(jsonConfigStr, "MoonLander_Classic");

    buildSimulationEnvironment(initialTime);
}

void simcontrol::instanceLoggingAction()
{
    // Initialize logger once
    Logger::instance().init("/tmp/simulation.log");
}

simData simcontrol::runSimulation(const double dt)
{
    Logger& logger = Logger::instance();
    simData simdata_;

    try
    {
        logger.log("Simulation step started. dt = " + std::to_string(dt));

        // --- Update spacecraft time ---

        landerSpacecraft->updateTime(dt);

        // --- Update spacecraft state (translation, velocity, etc.) ---
        landerSpacecraft->updateStep(dt);

        // --- Retrieve full simulation data ---
        simdata_ = landerSpacecraft->getFullSimulationData();

        // --- Retrieve G-Load ---


        // --- Log results (adapt later to new state vector) ---
        /*
        logger.log("Step results send to UI - Pos: (" +
                   std::to_string(spacecraftData.pos.x) + ", " +
                   std::to_string(spacecraftData.pos.y) + ", " +
                   std::to_string(spacecraftData.pos.z) + "), Vel: (" +
                   std::to_string(spacecraftData.vel.x) + ", " +
                   std::to_string(spacecraftData.vel.y) + ", " +
                   std::to_string(spacecraftData.vel.z) + "), Acc: (" +
                   std::to_string(spacecraftData.acc.x) + ", " +
                   std::to_string(spacecraftData.acc.y) + ", " +
                   std::to_string(spacecraftData.acc.z) + "), Fuel: " +
                   std::to_string(spacecraftData.fuelMass)
                   );
        */
    }
    catch (const std::exception& e)
    {
        std::cerr << "[EXCEPTION] in runSimulation: " << e.what() << std::endl;
        logger.log(std::string("Exception in runSimulation: ") + e.what());
        throw; // rethrow
    }
    catch (...)
    {
        std::cerr << "[EXCEPTION] Unknown exception in runSimulation." << std::endl;
        logger.log("Unknown exception in runSimulation.");
        throw;
    }
    return simdata_;
}


void simcontrol::setJsonConfigStr(const std::string &jsonConfigStr)
{
    jsonConfigString = jsonConfigStr;
}

void simcontrol::setTargetThrust(double thrustPercent)
{
    landerSpacecraft->setThrust(thrustPercent / 100.0);
}

void simcontrol::setResetBoolean()
{
    resetRequested = true;
}
