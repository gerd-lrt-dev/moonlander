#ifndef SIMDATASTRUCT_H
#define SIMDATASTRUCT_H

#include "stateVectorStruct.h"

#include "spacecraftStateStruct.h"

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

    // Engine data
    double thrust;
    double targetThrust;

    // Fuel data
    double fuelMass;
    double fuelFlow;

};

#endif // SIMDATASTRUCT_H
