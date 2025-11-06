#include "simcontrol.h"
#include <memory>
#include "vector3.h"

/**
 * @brief Entry point for the Moonlander simulation.
 *
 * Initializes the main simulation controller and starts the simulation.
 * Uses smart pointers to ensure automatic memory management.
 *
 * @return Exit status (0 = success)
 */
int main(int argc, char* argv[]) // Preperation for user input
{
    // Create the simulation controller using a smart pointer
    std::unique_ptr<simcontrol> controller;

    // Initialize the controller with:
    // v0 = 0 m/s, h0 = 4000 m, t0 = 0 s
    controller = std::make_unique<simcontrol>(Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, 3200.0), 0);

    // Simulation runs inside simcontrol constructor / runSimulator

    return 0;
}
