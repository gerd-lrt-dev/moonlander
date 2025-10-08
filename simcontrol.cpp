#include "simcontrol.h"
#include "environmentConfig.h"

#include <thread>
#include <chrono>
#include <stdexcept>

simcontrol::simcontrol(double v1, double h1, double t)
{
    // Überprüfe Parameter
    if (v1 >= 0 && h1 >= 0 && t>= 0)
    {
        runSimulator(v1, h1, t);
    }
    else
    {
        throw::std::invalid_argument("Ungueltige Startparameter für die Simulation!");
    }
}

simcontrol::~simcontrol()
{
    
}

void simcontrol::runSimulationLoop(bool lander1IsIntact, double v1, double h1, double t)
{
    EnvironmentConfig env; // Load environment config

    // Build namespace for clock
    using clock = std::chrono::steady_clock;
    
    // Build time laps
    auto nextFrame = clock::now();
    auto lastTime = clock::now();

    while (lander1IsIntact)
    {
        auto now = clock::now();
        std::chrono::duration<double> elapsed = now - lastTime;
        double dt = elapsed.count();
        lastTime = now;
        landerSpacecraft->updateTime(dt);

        // visualizing  process
        double v = lander->getVel(dt, v1);
        double h = lander->getHeight(dt, v1, h1);
        v1 = v;
        h1 = h;
        drawer->drawCockpit(t, h1, v1, 4000, lander1IsIntact);

        nextFrame += std::chrono::milliseconds(env.maxTimeStep);

        std::this_thread::sleep_until(nextFrame);

        t+=dt;

        // landerSpacecraft->setThrust(1);

        if (h1 <= 0.0)
        {
            landerSpacecraft->applyLandingDamage(v1);
        }

        lander1IsIntact = landerSpacecraft->isIntact(); 

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
    landerSpacecraft    = std::make_unique<spacecraft>(1500.0, 5500.0, 1.5, 1000.0); // emptymass: 1500 [kg], maxThrust: 5500[N], rate: 1.5 [Hz], fuel: 1000 [kg] 

    // Call integerity for simulation run
    bool lander1IsIntact = landerSpacecraft->isIntact(); 

    // Start simulation
    runSimulationLoop(lander1IsIntact, v1, h1, t);

    if (!lander1IsIntact) drawer->drawMissionFailed();
}