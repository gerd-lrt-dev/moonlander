#ifndef THRUST_H
#define THRUST_H

/**
 * @class Thrust
 * @brief Simulates the thrust behavior of a spacecraft engine.
 *
 * The class models the current thrust value approaching a target thrust
 * over time, considering a reaction speed (rate). This allows for a more
 * realistic simulation of engine dynamics instead of instantaneous changes.
 */
class Thrust
{
private:
    double target;   ///< [m/s²] Desired thrust value
    double current;  ///< [m/s²] Current thrust value, updated over time
    double rate;     ///< [Hz] Engine response speed (how quickly current approaches target)
    bool newThrust;  ///< [1,0] Flag of new incoming thurst (1)

public:
    /**
     * @brief Constructor
     * @param r Reaction speed of the engine [Hz]
     *
     * Initializes the engine with zero current and target thrust.
     */
    Thrust(double r) : target(0), current(0), rate(r) {};

    /**
     * @brief Destructor
     *
     * Currently trivial, no dynamic resources.
     */
    ~Thrust();

    /**
     * @brief Set a new target thrust
     * @param t Target thrust [m/s²]
     * @return The target thrust value set
     */
    double setTarget(double t);

    /**
     * @brief Update the current thrust based on the time delta
     * @param dt Time step since last update [s]
     * @return Updated current thrust [m/s²]
     *
     * The current thrust approaches the target thrust based on the
     * engine's reaction speed.
     */
    double update(double dt);

    /**
     * @brief Get the current thrust
     * @return Current thrust [m/s²]
     */
    double getCurrentThrust() const;
};

#endif
