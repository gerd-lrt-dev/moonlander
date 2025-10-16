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
    double targetThrust;    ///< [m/s²] Desired thrust value
    double currentThrust;   ///< [m/s²] Current thrust value, updated over time
    double timeConstant;    ///< [s] Parameter to be modeled that describes how quickly the current thrust responds to changes in the setpoints
    double Isp;             ///< [s] Specific impulse
    double rate;            ///< [Hz] Engine response speed (how quickly current approaches target)
    double fuelMass0;       ///< [kg] Fuel mass time step 0 (required for calculating live consumption)
    double fuelMass1;       ///< [kg] Fuel mass time step 1 (required for calculating live consumption)
    double liveConsumption; ///< [kg / dt] Real-time fuel consumption
    bool newThrust;         ///< [1,0] Flag of new incoming thurst (1)

    spacemath math;                 ///< Math helper class
    EnvironmentConfig envConfig;    ///< Environment parameters
    /**
     * @brief Calculate fuel cunsomption
     * @param fuelMass      ///< [kg] Mass of fuel
     * @param massflowFuel  ///< [kg/s] Mass flow of fuel
     * @param dt            ///< [s] discrete time step and update parameter
     * 
     * Reduces fuel supply depending on fuel consumption through the thrust provision process
     */
    const double calcFuelReduction(double fuelMass, double massFlowFuel, double dt);

    /**
     * @brief Function calculates fuel consumption based on time increments
     * 
     * Calculates real-time consumption from fuelmass0 and fuelmass1 
     * and stores it in liveConsumption.
     * 
     * Consumption can be requested via getFuelConsumption.
     */
    void calcFuelConsumption();


public:
    /**
     * @brief Constructor
     * @param r ///< [Hz] Reaction speed of the engine 
     *
     * Initializes the engine with zero current and target thrust.
     */
    Thrust(double r) : targetThrust(0), currentThrust(0), Isp(r) {};

    /**
     * @brief Destructor
     *
     * Currently trivial, no dynamic resources.
     */
    ~Thrust();

    /**
     * @brief Set a new target thrust
     * @param t ///<  [m/s²] Target thrust
     */
    void setTarget(double t);

    /**
     * @brief Update the current thrust based on the time delta
     * @param dt    ///< [s] Time step since last update 
     * @return 
     * The current thrust approaches the target thrust based on the
     * engine's reaction speed.
     */
    void updateSimpleThrust(double dt);

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
    double updateThrust(double dt, double fuelMass);

    /**
     * @brief Get the target thrust
     * @return ///< [m/s²] target thrust 
     */
    double getTargetThrust() const;

    /**
     * @brief Get the current thrust
     * @return ///< [m/s²] Current thrust 
     */
    double getCurrentThrust() const;

    /**
     * @brief Getter function for fuel live fuel consumption
     * @return ///< [kg / dt] Real-time fuel consumption
     */
    double getFuelConsumption() const;

};

#endif
