#ifndef PHYSICS_H
#define PHYSICS_H

/**
 * @class physics
 * @brief Handles the physics calculations for a lunar lander.
 *
 * This class provides methods to calculate the lander's velocity and height
 * over time, considering lunar gravity. It is stateless except for constant
 * parameters like lunar gravity.
 */
class physics
{
private:
    // Constants
    const double gravityMoon = 1.635; ///< [m/s²] Lunar gravitational acceleration

    /**
     * @brief Calculates height based on time, initial velocity, and initial height
     * @param t Time elapsed [s]
     * @param v0 Initial velocity [m/s]
     * @param h Initial height [m]
     * @return Calculated height [m]
     */
    double calcHeight(double t, double v0,  double h) const;

    /**
     * @brief Calculates velocity based on time and initial velocity
     * @param t Time elapsed [s]
     * @param v Initial velocity [m/s]
     * @return Calculated velocity [m/s]
     */
    double calcVel(double t, double v, double thrust = 0) const;

public:
    /**
     * @brief Constructor
     */
    physics();

    /**
     * @brief Destructor
     */
    ~physics();

    /**
     * @brief Get the current height of the lander
     * @param t Time elapsed [s]
     * @param v0 Initial velocity [m/s] (default = 0)
     * @param h0 Initial height [m] (default = 4000)
     * @return Height [m]
     */
    double getHeight(double t, double v0 = 0.0, double h0 = 4000.0) const;

    /**
     * @brief Get the current velocity of the lander
     * @param t Time elapsed [s]
     * @param v0 Initial velocity [m/s] (default = 0)
     * @return Velocity [m/s]
     */
    double getVel(double t, double v0 = 0.0) const;
};

#endif
