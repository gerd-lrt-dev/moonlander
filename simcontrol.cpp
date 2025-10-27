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
        throw std::invalid_argument("Ungueltige Startparameter für die Simulation!");
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
    // Instance classes
    landerPhysics       = std::make_unique<physics>();
    drawer              = std::make_unique<output>();
    landerSpacecraft    = std::make_unique<spacecraft>(1500.0, 5500.0, 300, 1000.0, 0.5); // emptymass: 1500 [kg], maxThrust: 5500[N], specific impulse: 300 [s], fuel: 1000 [kg], tau: 0,5 s //TODO: organize in struct

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
        double v = landerPhysics->getVel(dt, vel0, landerSpacecraft->requestAcceleration());
        double h = landerPhysics->getHeight(dt, vel0, pos0, landerSpacecraft->requestAcceleration());

        Vector3 thrustSpacecraft; //TODO: DUMMY muss in spacecraft implementiert werden
        double totalmass(0); // TODO: DUMMY muss in spacecraft implementiert werden
        Vector3 pos = landerPhysics->updatePos(vel0, pos0, thrustSpacecraft, dt, totalmass);
        Vector3 vel = landerPhysics->updateVel(vel0, pos0, thrustSpacecraft, dt, totalmass);

        
        // Update initial state variables for the next iteration
        // to ensure the next loop step uses the latest simulation data
        vel0 = v;
        pos0 = h;

        // Visualizing  process
        drawer->drawCockpit(t0, pos0, vel0, env.initialHeight, landerSpacecraft->requestThrust(), landerSpacecraft->requestTargetThrust(), landerSpacecraft->requestLiveFuelConsumption(), lander1IsIntact);

        // Update frame
        nextFrame += std::chrono::milliseconds(env.maxTimeStep);
        std::this_thread::sleep_until(nextFrame);

        // Count time due to timesteps
        t0+=dt;

        // TEST! Provide thrust when height is down below 3000 meters
        if (pos0 < 3000.0)
        {
            landerSpacecraft->setThrust(1.0);
        }

        // Calculate damage when spacecraft hits the ground
        if (pos0 <= 0.0)
        {
            landerSpacecraft->applyLandingDamage(vel0);
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