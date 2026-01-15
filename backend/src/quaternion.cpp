#include "quaternion.h"
#include <cmath>

// public
Quaternion::Quaternion()
    : q0_(1.0), q1_(0.0), q2_(0.0), q3_(0.0)
{}

Quaternion::Quaternion(double q0, double q1, double q2, double q3)
{
    double qN = norm(q0, q1, q2, q3);

    if (qN == 0.0) qN = 1.0;
    q0_ = q0 / qN;
    q1_ = q1 / qN;
    q2_ = q2 / qN;
    q3_ = q3 / qN;
}

double Quaternion::getQ0() const
{
    return q0_;
}

double Quaternion::getQ1() const
{
    return q1_;
}

double Quaternion::getQ2() const
{
    return q2_;
}

double Quaternion::getQ3() const
{
    return q3_;
}

//private
double Quaternion::norm(double q0, double q1, double q2, double q3) const
{
    return sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
}

void Quaternion::normalize()
{
    double qN = norm(q0_, q1_, q2_, q3_);
    q0_ /= qN;
    q1_ /= qN;
    q2_ /= qN;
    q3_ /= qN;
}
