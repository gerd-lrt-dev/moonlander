#include "simcontrol.h"
#include "logger.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <stdexcept>

//***********************************************************
//*************        Pubblic                   ************
//***********************************************************
// Desctructor
simcontrol::~simcontrol()
{
    
}
void simcontrol::runUserInput()
{
    double input;

    while(landerSpacecraft->isIntact())
    {
        std::cin >> input;
        userThrustPercent.store(input);
        std::cout << "INPUT: " << input;
    }
}

customSpacecraft simcontrol::laodSpacecraftConfig(std::string filePath, std::string spacecraftName)
{
    // Open file
    nlohmann::json config = jsonConfigReader::loadConfig(":/configs/lander.json");

    // Check object type
    std::cout << "config type: " << config.type_name() << std::endl;

    // Get array spacecraft
    auto spacecraftArray = config.at("spacecraft");

    // Search specific element
    nlohmann::json landerJsonObject;

    for (const auto& sc : spacecraftArray)
    {
        if (sc.at("name").get<std::string>() == spacecraftName)
        {
            landerJsonObject = sc;
            break;
        }
    }

    if (landerJsonObject.is_null())
    {
        throw std::runtime_error("Spacecraft '" + spacecraftName + "' not found in JSON file: " + filePath);
    }

    return jsonConfigReader::parseLander(landerJsonObject);
}

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

void simcontrol::initialize(const std::string& jsonConfigStr)
{
    landerMoon1 = loadSpacecraftFromJsonString(jsonConfigStr, "MoonLander_Classic");

    buildSimulationEnvironment(initialVel, initialPos, initialTime);
}

void simcontrol::instanceLoggingAction()
{
    // Initialize logger once
    Logger::instance().init("/tmp/simulation.log");
}

//***********************************************************
//*************        Private                   ************
//***********************************************************
void simcontrol::buildSimulationEnvironment(Vector3 vel0, Vector3 pos0, double t)
{
    // Instance classes
    landerPhysics       = std::make_unique<physics>();
    drawer              = std::make_unique<output>();
    landerSpacecraft    = std::make_unique<spacecraft>(landerMoon1);

    // Update initial states to spacecraft
    landerSpacecraft->setPos(pos0);
    landerSpacecraft->setVel(vel0);

    // Call integerity for simulation run
    bool lander1IsIntact = landerSpacecraft->isIntact(); 

    // Start user input thread
    //< REACTIVATE//std::thread inputThread(&simcontrol::runUserInput, this);

    // Start simulation //TODO: Wird obsolet
    //runSimulationLoop(lander1IsIntact, vel0, pos0, t);

    // When simulation is ready wait for thread
    //< REACTIVATE//if (inputThread.joinable()) inputThread.join();

    if (!lander1IsIntact) drawer->drawMissionFailed();
}

simData simcontrol::runSimulation(const double dt)
{
    Logger& logger = Logger::instance();

    simData spacecraftData;

    try
    {
        logger.log("Simulation step started. dt = " + std::to_string(dt));

        // Update spacecraft time
        landerSpacecraft->updateTime(dt);

        // Compute velocity and altitude
        spacecraftData.acc = landerPhysics->updateAcc(
            landerSpacecraft->requestThrust(),
            landerSpacecraft->getTotalMass(),
            landerSpacecraft->requestThrustDirection(),
            env.moonGravityVec
            );

        spacecraftData.pos = landerPhysics->updatePos(
            landerSpacecraft->getVel(),
            landerSpacecraft->getPos(),
            spacecraftData.acc,
            dt
            );

        spacecraftData.vel = landerPhysics->updateVel(
            landerSpacecraft->getVel(),
            spacecraftData.acc,
            dt
            );

        // Landing damage
        if (spacecraftData.pos.z <= 0.0)
        {
            landerSpacecraft->applyLandingDamage(spacecraftData.vel.z);
            logger.log("Landing damage applied. Velocity Z: " + std::to_string(spacecraftData.vel.z));
        }

        // Request state
        spacecraftData.spacecraftIntegrity = landerSpacecraft->isIntact();
        spacecraftData.thrust = landerSpacecraft->requestThrust();
        spacecraftData.targetThrust = landerSpacecraft->requestTargetThrust();
        spacecraftData.fuelMass = landerSpacecraft->getfuelMass();
        spacecraftData.fuelFlow = landerSpacecraft->requestLiveFuelConsumption();

        // Log results
        logger.log("Step results - Pos: (" +
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
    }
    catch (const std::exception& e)
    {
        logger.log(std::string("Exception in runSimulation: ") + e.what());
        throw; // rethrow
    }
    catch (...)
    {
        logger.log("Unknown exception in runSimulation.");
        throw;
    }

    return spacecraftData;
}


void simcontrol::setJsonConfigStr(const std::string &jsonConfigStr)
{
    jsonConfigString = jsonConfigStr;
}

void simcontrol::runSimulationLoop(bool& lander1IsIntact, Vector3& vel0, Vector3& pos0, double& t0)
{
    // Build namespace for clock
    using clock = std::chrono::steady_clock;

    // Build time laps
    auto nextFrame = clock::now();
    auto lastTime = clock::now();

    // Start simulation loop
    while (lander1IsIntact)
    {
        // Update clock
        auto now = clock::now();
        std::chrono::duration<double> elapsed = now - lastTime;
        double dt = elapsed.count();
        lastTime = now;

        // Provide time to spacecraft
        landerSpacecraft->updateTime(dt);

        // Compute current velocity and altitude based on the time step
        Vector3 accelerationSpacecraft =    landerPhysics->updateAcc(landerSpacecraft->requestThrust(),
                                            landerSpacecraft->getTotalMass(),
                                            landerSpacecraft->requestThrustDirection(),
                                            env.moonGravityVec);
        Vector3 pos = landerPhysics->updatePos(vel0, pos0, accelerationSpacecraft, dt);
        Vector3 vel = landerPhysics->updateVel(vel0, accelerationSpacecraft, dt);


        // Update initial state variables for the next iteration
        // to ensure the next loop step uses the latest simulation data
        pos0 = pos;
        vel0 = vel;

        // Visualizing  process
        drawer->drawCockpit(t0, pos, vel, accelerationSpacecraft, pos0.z, landerSpacecraft->requestThrust(), landerSpacecraft->requestTargetThrust(), landerSpacecraft->requestLiveFuelConsumption(), landerSpacecraft->getfuelMass(), lander1IsIntact);

        // Update frame
        nextFrame += std::chrono::milliseconds(env.maxTimeStep);
        std::this_thread::sleep_until(nextFrame);

        // Count time due to timesteps
        t0+=dt;

        // TEST! Provide thrust when height is down below 3000 meters
        if (pos0.z < 3000.0)
        {
            landerSpacecraft->setThrust(1.0);
        }

        // Calculate damage when spacecraft hits the ground
        if (pos0.z <= 0.0)
        {
            landerSpacecraft->applyLandingDamage(vel0.z);
        }

        // Request spacecraft of malfunction
        lander1IsIntact = landerSpacecraft->isIntact();

        // If malfunction of spacecraft is to high break simulation
        if (!lander1IsIntact)
        {
            break;
        }
    }
}
