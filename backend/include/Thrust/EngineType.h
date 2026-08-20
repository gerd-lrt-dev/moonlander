#ifndef ENGINETYPE_H
#define ENGINETYPE_H

/**
 * @brief Enumeration of propulsion system types used for thrust queries.
 *
 * Defines the selectable propulsion subsystems within the simulation framework.
 * This enumeration is used to specify whether thrust-related functions should
 * return the contribution of a specific propulsion system or the combined
 * effect of all available systems.
 */
enum class EngineType
{
    /**
     * @brief Main engine propulsion system.
     *
     * Refers to the primary propulsion unit responsible for generating the
     * dominant thrust along the vehicle's main thrust axis.
     */
    MainEngine,

    /**
     * @brief Reaction Control System (RCS) for translational motion.
     *
     * Refers to RCS functionality associated with linear motion along the
     * spacecraft body-fixed axes.
     */
    RCS_translation,

    /**
     * @brief Reaction Control System (RCS) for rotational motion.
     *
     * Refers to RCS functionality associated with rotational motion about the
     * spacecraft body-fixed axes.
     */
    RCS_rotation
};

/**
 * @brief Converts an engine type to its human-readable string representation.
 *
 * @param type Engine type to convert.
 * @return String representation of the specified engine type.
 */
inline const char* engineTypeToString(EngineType type)
{
    switch (type)
    {
    case EngineType::MainEngine:
        return "MainEngine";

    case EngineType::RCS_translation:
        return "RCS_translation";

    case EngineType::RCS_rotation:
        return "RCS_rotation";

    default:
        return "Unknown";
    }
}
#endif // ENGINETYPE_H
