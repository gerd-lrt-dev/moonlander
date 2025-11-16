#include "simcontrol.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <stdexcept>



// Constructor
simcontrol::simcontrol(Vector3 vel0, Vector3 pos0, double t0)
{
    // Überprüfe Parameter
    if (pos0.z >= 0 && t0>= 0) // pos0 = 0 -> Impact on moon, t0 = 0 (default value) -> otherwise physically illogical 
    {
        runSimulator(vel0, pos0, t0);
    }
    else
    {
        throw std::invalid_argument("unvalid parameters for simulation!");
    }
}

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

void simcontrol::runSimulator(Vector3 vel0, Vector3 pos0, double t)
{
    // Load lander config
    nlohmann::json landerJsonObect  = jsonConfigReader::loadConfig("/configs/lander.json");
    customSpacecraft landerMoon1    = jsonConfigReader::parseLander(landerJsonObect);

    // Instance classes
    landerPhysics       = std::make_unique<physics>();
    drawer              = std::make_unique<output>();
    landerSpacecraft    = std::make_unique<spacecraft>(landerMoon1); 

    // Call integerity for simulation run
    bool lander1IsIntact = landerSpacecraft->isIntact(); 

    // Start user input thread
    //< REACTIVATE//std::thread inputThread(&simcontrol::runUserInput, this);

    // Start simulation
    runSimulationLoop(lander1IsIntact, vel0, pos0, t);

    // When simulation is ready wait for thread
    //< REACTIVATE//if (inputThread.joinable()) inputThread.join();

    if (!lander1IsIntact) drawer->drawMissionFailed();
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
        drawer->drawCockpit(t0, pos, vel, accelerationSpacecraft, pos0.z, landerSpacecraft->requestThrust(), landerSpacecraft->requestTargetThrust(), landerSpacecraft->requestLiveFuelConsumption(), lander1IsIntact);

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