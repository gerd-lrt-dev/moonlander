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
#include "Thrust/FueltankStruct.h"

/**
 * @class Thrust
 * @brief Propulsion orchestrator of the spacecraft.
 *
 * This class no longer represents a single engine. Instead, it acts as the
 * central propulsion manager for the spacecraft and coordinates:
 *
 * - multiple engine models
 * - multiple propellant tanks
 * - thrust target dispatching
 * - engine activation / deactivation
 * - aggregated thrust computation
 * - global fuel bookkeeping
 *
 * Each engine is represented by an implementation of @ref IThrustModel and is
 * stored internally in the propulsion system. The class combines the individual
 * engine outputs into a single resulting thrust vector that can be passed to
 * the physics simulation.
 *
 * The propulsion system also owns the available fuel tanks. Each engine may
 * consume propellant from an assigned tank. This enables future support for
 * multi-tank and subsystem-specific fuel architectures, e.g.:
 *
 * - main engine tank
 * - attitude control / RCS tank
 * - reserve tank
 *
 * Current scope:
 * - multi-engine support
 * - 3D thrust vector aggregation
 * - multiple fuel tanks
 *
 * Future scope:
 * - engine groups
 * - tank authorization / routing
 * - crossfeed systems
 * - engine failure simulation
 */
class Thrust
{
public:

    /**
     * @brief Default constructor.
     *
     * Creates an empty propulsion system without engines and without tanks.
     * Engines and tanks must be initialized later via @ref initializeEngines.
     */
    Thrust();

    /**
     * @brief Default destructor.
     */
    ~Thrust();

    /**
     * @brief Advances the complete propulsion system by one simulation step.
     *
     * This function updates all registered engines and applies their fuel
     * consumption to the assigned tanks.
     *
     * Responsibilities:
     * - propagate engine dynamics in discrete time
     * - update actual thrust values from target thrust commands
     * - reduce fuel in the assigned tanks
     * - keep the propulsion system state consistent
     *
     * The individual engine models typically use a first-order response model:
     *
     * \f[
     * T(t+\Delta t) = T(t) + \frac{\Delta t}{\tau}\left(T_{target} - T(t)\right)
     * \f]
     *
     * where:
     * - \f$T\f$ is the current thrust
     * - \f$T_{target}\f$ is the commanded thrust
     * - \f$\tau\f$ is the engine time constant
     * - \f$\Delta t\f$ is the simulation time step
     *
     * If no fuel is available, the propulsion system does not advance thrust
     * production.
     *
     * @param dt Simulation time step [s]
     */
    void updateThrust(double dt);

    // -------------------------------------------------------------------------
    // Public setter functions
    // -------------------------------------------------------------------------

    /**
     * @brief Initializes engines and fuel tanks of the propulsion system.
     *
     * This function creates the internal engine models from the supplied engine
     * configurations and registers the available fuel tanks.
     *
     * The engine list may contain an arbitrary number of engines. This enables
     * spacecraft configurations such as:
     * - single main engine
     * - main engine + attitude thrusters
     * - multi-engine propulsion clusters
     *
     * The tank list may also contain multiple tanks. Each value in the vector
     * represents the initial mass or capacity of one tank.
     *
     * @param engines Vector of engine configurations
     * @param tanks Vector of tank masses / capacities [kg]
     */
    void initializeEngines(std::vector<EngineConfig> &engines, const std::vector<double> &tanks);

    /**
     * @brief Activates a specific engine.
     *
     * @param engineNr Index of the engine to activate
     */
    void activateEngine(const size_t &engineNr);

    /**
     * @brief Deactivates a specific engine.
     *
     * @param engineNr Index of the engine to deactivate
     */
    void deactivateEngine(const size_t &engineNr);

    /**
     * @brief Deactivates all engines.
     */
    void turnOffAllEngines();

    /**
     * @brief Sets a new target thrust for a specific engine.
     *
     * @param tThrust Target thrust [N]
     * @param engineNr Index of the engine to command
     */
    void setTargetThrust(const double &tThrust, const size_t &engineNr);

    /**
     * @brief Sets a new target thrust for a specific engine in normalized form.
     *
     * The input value is interpreted as a fraction of the engine's maximum
     * thrust capability.
     *
     * @param tThrustInPercentage Normalized thrust command [0..1]
     * @param engineNr Index of the engine to command
     */
    void setTargetThrustInPercentage(const double &tThrustInPercentage, const size_t &engineNr);

    /**
     * @brief Sets target thrust to zero for all engines.
     *
     * Can be used after landing, in emergency situations, or as part of a
     * general propulsion shutdown.
     */
    void shutDownAllEngines() const;

    // -------------------------------------------------------------------------
    // Public getter functions
    // -------------------------------------------------------------------------

    /**
     * @brief Returns the sum of all commanded target thrusts.
     *
     * @return Total target thrust [N]
     */
    Vector3 getTargetThrust() const;

    /**
     * @brief Returns the aggregated current thrust vector of all active engines.
     *
     * This is the total propulsion force vector produced by the spacecraft in
     * the current simulation step.
     *
     * @return Resulting thrust vector [N]
     */
    Vector3 getCurrentThrust() const;

    /**
     * @brief Returns the aggregated current thrust in normalized form.
     *
     * The returned vector expresses the current thrust components relative to
     * the maximum available thrust of the active propulsion configuration.
     *
     * @return Normalized thrust vector [-]
     */
    Vector3 getCurrentThrustInPercentage() const;

    /**
     * @brief Returns the total current fuel consumption of all engines.
     *
     * @return Total propellant mass flow rate [kg/s]
     */
    double getFuelConsumption() const;

    /**
     * @brief Returns the total remaining propellant mass of all tanks.
     *
     * @return Total fuel mass [kg]
     */
    double getCurrentFuelMass() const;

private:
    /**
     * @brief Internal list of engine models managed by the propulsion system.
     *
     * Each entry represents one concrete engine model implementing
     * @ref IThrustModel.
     */
    std::vector<std::unique_ptr<IThrustModel>> models_;

    /**
     * @brief Internal list of spacecraft fuel tanks.
     *
     * Each tank acts as a propellant reservoir that may be assigned to one or
     * more engines.
     */
    std::vector<FuelTank> tanks_;

    /**
     * @brief Global fuel-related state of the propulsion system.
     *
     * Used for aggregated fuel bookkeeping and telemetry.
     */
    FuelState fuelState_;

    /**
     * @brief Legacy single-engine configuration cache.
     *
     * @note This member may become obsolete in a fully multi-engine
     *       architecture and should be reviewed during future refactoring.
     */
    EngineConfig engineConfig_;

    /**
     * @brief Global thrust-related state of the propulsion system.
     *
     * Used for aggregated propulsion telemetry and backward-compatible access.
     */
    ThrustState thrustState_;

    /**
     * @brief Adds fuel tanks to the propulsion system.
     *
     * Each entry in the vector represents one tank with a given initial fuel
     * mass or capacity.
     *
     * @param tanks Vector of tank masses [kg]
     */
    void addFuelTank(const std::vector<double> &tanks);

    /**
     * @brief Computes the total remaining fuel mass over all tanks.
     *
     * @return Sum of all tank masses [kg]
     */
    double getFuelMassOfAllTanks() const;

    /**
     * @brief Adds a single engine model to the propulsion system.
     *
     * Ownership of the engine model is transferred to the propulsion system.
     *
     * @param model Engine model instance
     */
    void addModel(std::unique_ptr<IThrustModel> model);
};

#endif
