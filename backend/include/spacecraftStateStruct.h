#ifndef SPACECRAFTSTATESTRUCT_H
#define SPACECRAFTSTATESTRUCT_H

enum class SpacecraftState      ///< State of spacecraft
{
    Operational,                ///< Fully physics
    Landed,                     ///< no translation, time & systems running
    Crashed,                    ///< no translation, but state stable
    Destroyed                   ///< everything besides time is frozen
};

#endif // SPACECRAFTSTATESTRUCT_H
