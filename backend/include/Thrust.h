#ifndef THRUST_H
#define THRUST_H

#include <memory>
#include <numeric>
#include <vector>

#include "vector3.h"
#include "Thrust/iThrust.h"
#include "Thrust/FuelStateStruct.h"

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
public:

    Thrust();
    ~Thrust();


    /**
     * @brief Update the current thrust based on the time delta
     * @param dt    ///< [s] Time step since last update 
     * @param fuelMass
     * @return Returns fuel mass after combustion step
     * 
     * This function updates the complex thrust calculation with 
     * mass reduction and solution of the differential equation 
     * in discrete time steps.
     * 
     * Approximation of the first-order differential equation for thrust control
     * T(t + delta t) = T(t) + (delta t)/ teta * (T{target} - T(t))
     * T: thrust, t: time, delta t: discrete time step, teta: time conmstant 
     * 
     * This function also uses the transfer parameter to check whether fuel is 
     * available to provide thrust. In addition, it returns the fuel supply 
     * remaining after the combustion process in the respective time increment.
     * 
     * Steps to calculate thrust:
     * 1. updateThrust - function initializes the thrust calculation. From now on, the thrust is determined based on the target thrust. Even if thrust = 0. Later with idle mode
     */
    void updateThrust(double dt);

    // -------------------------------------------------------------------------
    // Public setter functions
    // -------------------------------------------------------------------------
    /**
     * @brief Set a new target thrust
     * @param tThrust ///<  [N] Target thrust
     */
    void setTargetThrust(const double &tThrust);

    // -------------------------------------------------------------------------
    // Public getter functions
    // -------------------------------------------------------------------------
    /**
     * @brief Get the target thrust
     * @return ///< [N] target thrust 
     */
    double getTargetThrust() const;

    /**
     * @brief Get the current thrust
     * @return ///< [N] Current thrust 
     */
    double getCurrentThrust() const;

    /**
     * @brief Getter function for fuel live fuel consumption
     * @return ///< [kg / dt] Real-time fuel consumption
     */
    double getFuelConsumption() const;

    /**
     * @brief Getter function for current fuel mass
     * @return ///< [kg] fuel mass
     */
    double getCurrentFuelMass() const;

    /**
     * @brief Getter function for thrust direction
     * @return ///< [-] Vector with direction of thrust
     * 
     * The vector is aligned with the static coordinate system of the spacecraft
     */
    Vector3 getDirectionOfThrust() const;

private:
    std::vector<std::unique_ptr<IThrustModel>> models_;
    std::vector<double> tanks_;

    FuelState fuelState_;

    void addModel(std::unique_ptr<IThrustModel> model);

    void addFuelTank(double tank);

    const double getFuelMassOfAllTanks();
};

#endif
