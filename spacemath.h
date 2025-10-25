#ifndef SPACEMATH_H
#define SPACEMATH_H

/**
 * @class spacemath
 * @brief Utility class for space-related physics calculations.
 *
 * This class provides static methods to compute common physical quantities
 * used in spacecraft simulations, such as kinetic energy, potential energy,
 * or other physics-based calculations.
 *
 * All functions are static and can be called without instantiating the class.
 */
class spacemath
{
public:
    /**
     * @brief Constructor (trivial, no data members)
     */
    spacemath() = default;

    /**
     * @brief Destructor (trivial)
     */
    ~spacemath() = default;

    /**
     * @brief Computes kinetic energy.
     * 
     * Calculates the kinetic energy of a body given its mass and velocity:
     * \f$ KE = \frac{1}{2} m v^2 \f$
     *
     * @param mass      ///< [kg] Mass of the object 
     * @param velocity  ///< [m/s] Velocity of the object 
     * @return          ///< [J] Kinetic energy 
     */
    static double kineticEnergy(double mass, double velocity);

    /**
     * @brief Calculates acceleration based on thrust
     * @param thrust    ///< [N] Engine thrust spacecraft
     * @param mass      ///< [kg] Total mass of spacecraft
     * 
     * a = F{thrust} / m{total}
     */
    static double accelerationBasedOnThrust(double thrust, double mass);

    /**
     * @brief Calculates acceleration based on thrust, mass and specific impulse
     * 
     */
    static double accelerationComplex(double currentThrust, double totalMass, double gravityConstant);

    /**
     * @brief Calculates Mass flow based on thrust
     * @param currentThrust ///< [N] Thrust from the last time step
     * @param Isp           ///< [s] Specific impulse
     * 
     * g0 is always the acceleration due to gravity, regardless of whether you are 
     * on the moon or Mars. This is defined historically and physically because 
     * specific impulse is measured according to this standard.
     * 
     * \dot(m) = F / (I_{Sp} \cdot g_0)
     */
    static double calcMassFlowBasedOnThrust(double currenThrust, double Isp, double earthGravity);
};

#endif
