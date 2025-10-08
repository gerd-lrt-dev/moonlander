#ifndef SPACECRAFT_H
#define SPACECRAFT_H

#include "Thrust.h"

class spacecraft
{
private:
    const double    emptyMass;     // [kg], mass when spacecraft is empty - added by fuel
    double          fuelMass;      // [kg], mass of fuel - added with emptyMass
    const double    maxThrust;     // [N], maximum of thrust
    Thrust          mainEngine;          

public:
    spacecraft(double m, double maxT, double rate, double fuelM) 
        : emptyMass(m), maxThrust(maxT), mainEngine(rate), fuelMass(fuelM) {};
    ~spacecraft();
};

#endif