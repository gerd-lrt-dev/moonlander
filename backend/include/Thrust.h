#ifndef THRUST_H
#define THRUST_H

#include <memory>
#include <numeric>
#include <vector>

#include "vector3.h"
#include "Thrust/iThrust.h"
#include "Thrust/BasicMainEngineModel.h"
#include "Thrust/FuelStateStruct.h"
#include "Thrust/thrustState.h"
#include "Thrust/EngineConfig.h"

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
     * @brief Set Engine Config
     * @param Isp - Specific Impulse [s]
     * @param timeConstant tau [s]
     * @param Response Rate [Hz]
     * @param Maximum Thrust [N]
     * @param thrust Direction direction of thrust due to spacecraft in EastNorthUp (ENU Coordinates)
     */
    void initializeEngines(const double &Isp, const double &timeConstant, const double &responseRate, const double &maxThrust, const Vector3 &thrustDirection, const std::vector<double> &tanks);

    /**
     * @brief Set a new target thrust
     * @param tThrust   ///< [N] Target thrust
     * @param engineNr  ///< Number of engine, zero is always main Engine!
     */
    void setTargetThrust(const double &tThrust, const size_t &engineNr);

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
     * @return ///< [N] Current thrust vector
     */
    Vector3 getCurrentThrust() const;

    /**
     * @brief Get the current thrust in percentage
     * @return ///< [] Current thrust vector with percentage value
     */
    Vector3 getCurrentThrustInPercentage() const;

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

private:
    std::vector<std::unique_ptr<IThrustModel>> models_;
    std::vector<double> tanks_;
    double tmpFullFuelMass = 0.0;

    FuelState fuelState_;
    EngineConfig engineConfig_;
    ThrustState thrustState_;

    void addFuelTank(double tank);

    double getFuelMassOfAllTanks() const;

    void addModel(std::unique_ptr<IThrustModel> model);
};

#endif
