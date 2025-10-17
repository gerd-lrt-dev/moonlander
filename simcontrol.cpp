#include "simcontrol.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <stdexcept>

// Constructor
simcontrol::simcontrol(double v0, double h0, double t0)
{
    // Überprüfe Parameter
    if (h0 >= 0 && t0>= 0) // h0 = 0 -> Impact on moon, t0 = 0 (default value) -> otherwise physically illogical 
    {
        runSimulator(v0, h0, t0);
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

void simcontrol::runSimulationLoop(bool& lander1IsIntact, double& v0, double& h0, double& t0)
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

        /* CORRECT SO FAR
        std::cout << "lander1IsIntact: " << lander1IsIntact << std::endl;
        std::cout << "t: " << t0 << std::endl;
        std::cout << "v: " << v0 << std::endl;
        std::cout << "h: " << h0 << std::endl;
        */
        // Compute current velocity and altitude based on the time step
        double v = lander->getVel(dt, v0, landerSpacecraft->requestAcceleration());
        double h = lander->getHeight(dt, v0, h0, landerSpacecraft->requestAcceleration());
        
        // Update initial state variables for the next iteration
        // to ensure the next loop step uses the latest simulation data
        v0 = v;
        h0 = h;

        // Visualizing  process
        drawer->drawCockpit(t0, h0, v0, env.initialHeight, landerSpacecraft->requestThrust(), landerSpacecraft->requestTargetThrust(), landerSpacecraft->requestLiveFuelConsumption(), lander1IsIntact);

        // Update frame
        nextFrame += std::chrono::milliseconds(env.maxTimeStep);
        std::this_thread::sleep_until(nextFrame);

        // Count time due to timesteps
        t0+=dt;

        // TEST! Provide thrust when height is down below 3000 meters
        if (h0 < 3000.0)
        {
            landerSpacecraft->setThrust(1.0);
        }

        // Calculate damage when spacecraft hits the ground
        if (h0 <= 0.0)
        {
            landerSpacecraft->applyLandingDamage(v0);
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

void simcontrol::runSimulator(double v1, double h1, double t)
{
    // Instance classes
    lander              = std::make_unique<physics>();
    drawer              = std::make_unique<output>();
    landerSpacecraft    = std::make_unique<spacecraft>(1500.0, 5500.0, 0.5, 1000.0); // emptymass: 1500 [kg], maxThrust: 5500[N], specific impulse: 0.5 [s], fuel: 1000 [kg] //TODO: organize in struct

    // Call integerity for simulation run
    bool lander1IsIntact = landerSpacecraft->isIntact(); 

    // Start user input thread
    //< REACTIVATE//std::thread inputThread(&simcontrol::runUserInput, this);

    // Start simulation
    /* << CORRECT SO FAR
    std::cout << "lander is intact: " << lander1IsIntact << std::endl;
    std::cout << "v1: " << v1 << std::endl;
    std::cout << "h1: " << h1 << std::endl;
    std::cout << "t: " << t << std::endl;
    */
    runSimulationLoop(lander1IsIntact, v1, h1, t);

    // When simulation is ready wait for thread
    //< REACTIVATE//if (inputThread.joinable()) inputThread.join();

    if (!lander1IsIntact) drawer->drawMissionFailed();
}