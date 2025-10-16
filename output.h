#ifndef OUTPUT_H
#define OUTPUT_H

/**
 * @class output
 * @brief Handles the visualization of the lunar lander simulation.
 *
 * This class provides methods to display the lander's current state,
 * including time, height, velocity, and a simple height bar.
 * Output is currently text-based (console), but can be extended for GUI.
 */
class output
{
private:
    // Currently no private members, can be extended for GUI state

public:
    /**
     * @brief Constructor
     *
     * Initializes the output handler.
     */
    output();

    /**
     * @brief Destructor
     *
     * Cleans up resources if needed.
     */
    ~output();

    /**
     * @brief Draws the cockpit display for the lunar lander
     * @param t Current simulation time [s]
     * @param h Current height of the lander [m]
     * @param v Current velocity of the lander [m/s]
     * @param hMax Maximum height for scaling the height bar [m]
     *
     * Outputs a formatted cockpit view including a text-based height bar.
     */
    void drawCockpit(double t, double h, double v, double hMax, double thrust, double targetThrust, double fuelConsumption, bool integrity);

    /**
     * @brief Draws mission failed screen
     */
    void drawMissionFailed();
};

#endif
