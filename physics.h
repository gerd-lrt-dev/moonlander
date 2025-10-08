#ifndef PHYSICS_H
#define PHYSICS_H

class physics
{
private:
    // Constants
    const double gravityMoon = 1.635;

    // Calculate
    double calcHeight(double t, double v0,  double h) const; // t = sec, v = m/s, h = m
    double calcVel(double t, double v) const; // t = sec, v0 = m/s
public:
    physics(/* args */);
    ~physics();

    // Getter
    double getHeight(double t, double v0 = 0.0, double h0 = 4000.00) const;
    double getVel(double t, double v0 = 0.0) const;
};

#endif