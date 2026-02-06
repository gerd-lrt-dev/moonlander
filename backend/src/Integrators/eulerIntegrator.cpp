#include "Integrators/eulerIntegrator.h"

Vector3 EulerIntegrator::integrateVel(const Vector3& vel, const Vector3& acc, double dt) const
{
    return vel + acc * dt;
}

Vector3 EulerIntegrator::integratePos(const Vector3& pos, const Vector3& vel, const Vector3& acc, double dt) const
{
    return pos + vel * dt + acc * 0.5 * dt * dt;
}
