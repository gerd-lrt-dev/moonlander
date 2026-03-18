#include "Thrust.h"

// ---Private-------------------------------------



// ---Public--------------------------------------
Thrust::Thrust()
{

}

Thrust::~Thrust()
{

}

// -------------------------------------------------------------------------
// Public setter functions
// -------------------------------------------------------------------------
void Thrust::setTargetThrust(const double &tThrust)
{

}

void Thrust::updateThrust(double dt)
{
    if (getFuelMassOfAllTanks() > 0.0)
    {
        // Update thrust for all engines
        for (auto& model : models_)
        {
            model->updateThrust(dt);
        }
    }
    else
    {
        return;
    }
}

// --- Getter functions ---------------------------------------------

double Thrust::getTargetThrust() const
{
    return 0.0;
}

double Thrust::getCurrentThrust() const
{
    return 0.0;
}

double Thrust::getFuelConsumption() const
{
    return 0.0;
}

double Thrust::getCurrentFuelMass() const
{
    return 0.0;
}

Vector3 Thrust::getDirectionOfThrust() const
{
    return {0.0, 0.0, 0.0};
}

void Thrust::addModel(std::unique_ptr<IThrustModel> model)
{
    models_.push_back(std::move(model));
}

void Thrust::addFuelTank(double tank)
{
    tanks_.push_back(tank);
}

const double Thrust::getFuelMassOfAllTanks()
{
    return std::accumulate(tanks_.begin(), tanks_.end(), 0.0);
}


