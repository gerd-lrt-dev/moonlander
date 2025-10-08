#ifndef THRUST_H
#define THRUST_H

class Thrust
{
private:
    double target;      // Gewünschter Schub [m/s²]
    double current;     // Aktueller Schub [m/s²]
    double rate;        // Reaktionsgeschwindigkeit [Hz]
    
public:
    Thrust(double r) : target(0), current(0), rate(r) {};
    ~Thrust();

    double setTarget(double t);
    double update(double dt);
    double getCurrentThrust() const;
};




#endif