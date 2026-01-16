#ifndef CUSTOMSPACECRAFTSTRUCT_H
#define CUSTOMSPACECRAFTSTRUCT_H

#include "vector3.h"

/**
 * @struct customSpacecraft
 * @brief Defines all physical and configuration parameters of a spacecraft.
 *
 * All parameters are expressed in the spacecraft's body reference frame (B-frame),
 * unless explicitly stated otherwise. The struct contains mass properties,
 * propulsion characteristics, attitude-related inertial properties, and
 * frame-initialization parameters.
 */
struct customSpacecraft
{
    // -------------------------------------------------------------------------
    // Mass Properties
    // -------------------------------------------------------------------------

    double emptyMass;
    ///< [kg] Dry mass of the spacecraft (structure + engines, no fuel).

    double fuelM;
    ///< [kg] Initial fuel mass at simulation start.

    double maxFuelM;
    ///< [kg] Maximum fuel tank capacity (optional; used for validation or HUD).

    // -------------------------------------------------------------------------
    // Propulsion System
    // -------------------------------------------------------------------------

    double maxT;
    ///< [N] Maximum thrust produced by the main engine at full throttle.

    double Isp;
    ///< [s] Specific impulse of the main engine (constant exhaust efficiency).

    double timeConstant;
    ///< [s] Engine throttle response time (first-order system time constant).

    double responseRate;
    ///< [s] Maximum rate of thrust change.

    Vector3 B_mainThrustDirection;
    ///< [unit] Normalized thrust direction vector in body frame (B-frame).
    ///< Convention: +Z axis typically points "down" along the engine nozzle.

    Vector3 B_mainThrustPosition;
    ///< [m] Position of the main engine's thrust application point in B-frame.
    ///< This produces torque if offset from the spacecraft's center of mass.


    // -------------------------------------------------------------------------
    // Attitude Dynamics (Rigid Body Inertia)
    // -------------------------------------------------------------------------

    double Ixx;
    ///< [kg·m²] Moment of inertia about the spacecraft’s body X-axis.

    double Iyy;
    ///< [kg·m²] Moment of inertia about the body Y-axis.

    double Izz;
    ///< [kg·m²] Moment of inertia about the body Z-axis.
    ///< Assumed diagonal inertia tensor (symmetric spacecraft).


    // -------------------------------------------------------------------------
    // State (Body Frame Coordinates)
    // -------------------------------------------------------------------------

    Vector3 I_initialPos;
    ///< [m] Initial spacecraft position expressed in body coordinates.
    ///< Typically initial (0, 0, 0) because physics handles world/Moon coordinates.

    Vector3 I_initialVelocity;
    ///< [m/s] Velocity in three spatial directions

    Vector3 B_initialRot;
    ///< [rad] Initial orientation (pitch, yaw, roll) in body coordinates.

    Vector3 B_initialCenterOfMass;
    ///< [m] Center of mass location in body frame at t=0.
    ///< Should be updated dynamically if fuel mass distribution changes.

    // -------------------------------------------------------------------------
    // Integrity & Operational Status
    // Attributes controlling spacecraft health, integrity, and operational status.
    // -------------------------------------------------------------------------

    double       structuralIntegrity;       ///< [%] Threshold at which the spacecraft becomes non-operational
    double       safeVelocity;              ///< [m/s] Velocity in which is the spacecraft able to land safely
};

#endif // CUSTOMSPACECRAFTSTRUCT_H
