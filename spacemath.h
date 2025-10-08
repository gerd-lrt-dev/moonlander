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
     * @param mass Mass of the object [kg]
     * @param velocity Velocity of the object [m/s]
     * @return Kinetic energy [J]
     */
    static double kineticEnergy(double mass, double velocity);
};

#endif
