#include "simcontrol.h"

#include <thread>
#include <chrono>
#include <stdexcept>

//TODO: Aktuell ist der run abhängig von der Höhe des Raumfahrtsystems. Künftig soll es abhängig sein von der Integrität der Struktur (z.B. intakt - nicht intakt)

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

void simcontrol::runSimulator(double v1, double h1, double t)
{
    // Instance classes
    lander              = std::make_unique<physics>();
    drawer              = std::make_unique<output>();
    landerSpacecraft    = std::make_unique<spacecraft>(1500.0, 5500.0, 1.5, 1000.0); // emptymass: 1500 [kg], maxThrust: 5500[N], rate: 1.5 [Hz], fuel: 1000 [kg] 

    // Build namespace for clock
    using clock = std::chrono::steady_clock;
    
    // Build time laps
    auto nextFrame = clock::now();
    auto lastTime = clock::now();

    // Call integerity for simulation run
    bool lander1IsIntact = landerSpacecraft->isIntact(); 

    // Start simulation
    while (lander1IsIntact)
    {
        auto now = clock::now();
        std::chrono::duration<double> elapsed = now - lastTime;
        double dt = elapsed.count();
        lastTime = now;

    
        if (t == 0)
        {
            drawer->drawCockpit(t, h1, v1, 2000, lander1IsIntact);
            double v = lander->getVel(dt);
            double h = lander->getHeight(dt);
            v1 = v;
            h1 = h;
        }
        else
        {
            double v = lander->getVel(dt, v1);
            double h = lander->getHeight(dt, v1, h1);
            v1 = v;
            h1 = h;
            drawer->drawCockpit(t, h1, v1, 2000, lander1IsIntact);
        }
        
        nextFrame += std::chrono::milliseconds(16);

        std::this_thread::sleep_until(nextFrame);

        t+=dt;

        if (h1 <= 0)
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