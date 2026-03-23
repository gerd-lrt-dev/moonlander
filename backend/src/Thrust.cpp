#include "Thrust.h"
#include <iostream>
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
void Thrust::setTargetThrust(const double &tThrust, const size_t &engineNr)
{
    if (engineNr >= models_.size())
    {
        std::cerr << "Engine index out of range!" << std::endl;
        return;
    }

    models_[engineNr]->setTarget(tThrust);
}

void Thrust::initializeEngines(const double &Isp, const double &timeConstant, const double &responseRate, const double &maxThrust, const Vector3 &thrustDirection, const std::vector<double> &tanks)
{
    EngineConfig tmpConfig;
    FuelState tmpState;

    for (const auto& tank : tanks)
    {// TODO: Add tank not only with information about capacity but also with information about who is authorized to use the tank
        addFuelTank(tank);
    }
    tmpFullFuelMass = getFuelMassOfAllTanks(); //TODO: ELIMINATE WHEN USING DIFFERENT TANKS IS IMPLEMENTED!!!
    std::cout << "[Thrust] Added -" << tanks.size() << "- tanks to spacecraft structure" << std::endl;

    tmpConfig.Isp           = Isp;
    tmpConfig.timeConstant  = timeConstant;
    tmpConfig.responseRate  = responseRate;
    tmpConfig.direction     = thrustDirection;
    fuelState_.massStart    = getFuelMassOfAllTanks();
    tmpState.consumptionRate= 0.0;

    addModel(std::make_unique<basicMainEngineModel>(tmpConfig, tmpState));
}

void Thrust::updateThrust(double dt)
{
    if (getFuelMassOfAllTanks() > 0.0)
    {
        // Update thrust for all engines
        for (auto& model : models_)
        {

            model->updateThrust(dt);
            tmpFullFuelMass = model->calcFuelReduction(tmpFullFuelMass, model->getFuelConsumption(), dt);
        }
    }
    else
    {
        std::cout << "[Thrust]: Update thrust called..." << std::endl;
        return;
    }
}

// --- Getter functions ---------------------------------------------

double Thrust::getTargetThrust() const
{
    double sum = 0.0;

    for (const auto& model : models_)
    {
        sum += model->getTargetThrust();
    }

    return sum;
}

Vector3 Thrust::getCurrentThrust() const
{
    Vector3 total{0.0, 0.0, 0.0};

    for (const auto& model : models_)
    {
        Vector3 dir = model->getDirectionOfThrust();
        double thrust = model->getCurrentThrust();

        total += dir * thrust;
    }

    return total;
}

Vector3 Thrust::getCurrentThrustInPercentage() const
{
    for (const auto& model: models_)
    {
        //model->
    }

}

double Thrust::getFuelConsumption() const
{
    double sum = 0.0;

    for (const auto& model : models_)
    {
        sum += model->getFuelConsumption();
    }

    return sum;
}

double Thrust::getCurrentFuelMass() const
{
    return tmpFullFuelMass;
}

void Thrust::addModel(std::unique_ptr<IThrustModel> model)
{
    models_.push_back(std::move(model));
}

void Thrust::addFuelTank(double tank)
{
    tanks_.push_back(tank);
}

double Thrust::getFuelMassOfAllTanks() const
{
    double fuelMass = std::accumulate(tanks_.begin(), tanks_.end(), 0.0);
    return fuelMass;
}


