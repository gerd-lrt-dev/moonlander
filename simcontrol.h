#ifndef SIMCONTROL_H
#define SIMCONTROL_H

#include "physics.h"
#include "output.h"


#include <memory>

class simcontrol
{
private:
    // Smart pointer zur automatisierten Speicherverwaltung
    std::unique_ptr<physics> lander;
    std::unique_ptr<output> drawer;
    
    void runSimulator(double v1, double h1, double t);
public:
    simcontrol(double v1, double h1, double t);
    ~simcontrol();
};

#endif