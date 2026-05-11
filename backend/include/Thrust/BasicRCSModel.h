#ifndef BASICRCSMODEL_H
#define BASICRCSMODEL_H

#include "Thrust/iThrust.h"
#include "Thrust/RCSConfig.h".h"
#include "Thrust/FuelStateStruct.h"
#include "vector3.h"

#include <iostream>
#include <deque>

struct RCSCommandSample
{
    double time;
    int cmd;
};

class basicRCSModel : IThrustModel{
public:
    /**
     * @brief Constructor
     * @param eConfig Configuration parameters for a spacecraft engine
     * @param fState Fuel-related state variables for the engine
     */
    basicRCSModel(const RCSEngineConfig& rcsConfig, FuelState fState);

    /**
     * @brief Destructor
     *
     * Currently trivial, no synamic resources.
     */
    ~basicRCSModel() override = default;

    void updateThrust(const double &dt) override;
    // -------------------------------------------------------------------------
    // Public setter
    // -------------------------------------------------------------------------
    /**
     * @brief Setter function for target thrust in percentage
     * @param tThrustInPercentage
     *
     * RCS input is a binary firing signal u(t) ∈ [0.0, 1.0]
     * If the user input diverses from that, the input is going to round up or down
     * in case of u(t) ∈ (0...1) otherwise the user input is not going to processesed.
     *
     * Sets the commanded input "cmdInput" (private member)
     */
    void setTargetInPercentage(const double &tThrustInPercentage) override;

private:
    // -------------------------------------------------------------------------
    // Private members
    // -------------------------------------------------------------------------
    RCSEngineConfig rcsConfig_;                 ///< [-] Configuration parameters for a RCS engine.
    FuelState fuelstate_;                       ///< [-] Fuel-related state variables for the engine.
    int cmdInput = 0.0;                         ///< [binary] Commanded Input u(t) ∈ [0,1]
    int cmdInputDelayed = 0.0;                  ///< [s] u_d(t) = u (t-t_d) Delayed input cmd to simulate valve adjusting
    std::deque<RCSCommandSample> commandBuffer; ///< Bufferd command mit RCSCommandSample struct
    double totalEngineTime = 0.0;               ///< [s] Total time the engine has been running

    // -------------------------------------------------------------------------
    // Private calculation methods
    // -------------------------------------------------------------------------

    double calcThrust(const int cmdInputDelayed, const double &tauOn, const double &tauOff, const double &dt, const double &nominalThrust);

    int convertToBinaryCommand(double input);



};

#endif // BASICRCSMODEL_H
