#ifndef SIMDATASTRUCT_H
#define SIMDATASTRUCT_H

#include "stateVectorStruct.h"
#include "spacecraftStateStruct.h"
#include "Thrust/FueltankStruct.h"

#include <string>
#include <vector>

/**
 * @brief This data will be return to UI by runSimulation
     * @param pos                   ///< [m] positions in three dimensions
     * @param vel                   ///< [m/s] velocity in three dimensions
     * @param acc                   ///< [s/s²] acceleration in three dimensions
     * @param spacecraftIntegrity   ///< [0/1] flag that represents spacecraft integrity
     * @param thrust                ///< [%] current thrust of spacecraft
     * @param targetThrust          ///< [%] target thrust of spacecraft
     * @param fuelMass              ///< [kg] current fuel mass of spacecraft
     * @param fuelFlow              ///< [kg/s] mass flow of fuel
 */
struct simData
{
    // Navigation data
    StateVector statevector_;

    // Spacecraft Integrity
    bool spacecraftIntegrity;
    SpacecraftState spacecraftState_;

    // Console output
    std::string output;

    // Engine data
    Vector3 thrust;
    Vector3 targetThrust;
    Vector3 thrustInPercentage;

    // Fuel data
    std::vector<FuelTank> tanks;
    double fuelMass; ///< Fuel Mass summed by all installed tanks[kg]
    double fuelFlow;

    // Forces
    double GLoad;

};

#endif // SIMDATASTRUCT_H
