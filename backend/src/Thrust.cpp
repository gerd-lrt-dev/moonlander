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

void Thrust::setTargetThrustInPercentage(const double &tThrustInPercentage, const size_t &engineNr)
{
    if (engineNr >= models_.size())
    {
        std::cerr << "Engine index out of range!" << std::endl;
        return;
    }

    models_[engineNr]->setTargetInPercentage(tThrustInPercentage);
}

void Thrust::shutDownAllEngines() const
{
    for (const auto& model : models_)
    {
        model->setTarget(0.0);
    }
}

void Thrust::initializeEngines(std::vector<EngineConfig> &engineConfigs, const std::vector<double> &tanks)
{
    // -----------------------------------------
    // Initialize tanks
    // -----------------------------------------
    addFuelTank(tanks);

    // -----------------------------------------
    // Initialize engine configs
    // -----------------------------------------

    for (const auto &cfg_ : engineConfigs)
    {
        FuelState state;
        state.consumptionRate = 0.0;
        addModel(std::make_unique<basicMainEngineModel>(cfg_, state));
        std::cout << "[Thrust] Added engine: " << cfg_.name << std::endl;
    }
}

void Thrust::activateEngine(const size_t &engineNr)
{
    models_[engineNr]->setEnginePowerSwitch(true);
}

void Thrust::deactivateEngine(const size_t &engineNr)
{
    models_[engineNr]->setEnginePowerSwitch(false);
}

void Thrust::turnOffAllEngines()
{
    for (auto& model : models_)
    {
        model->setEnginePowerSwitch(false);
    }
}
void Thrust::updateThrust(double dt)
{
    if (getFuelMassOfAllTanks() > 0.0)
    {
        // Update thrust for all engines
        for (int i = 0; i < models_.size(); ++i)
        {

            models_[i]->updateThrust(dt);
            tanks_[models_[i]->getTankID()].mass = models_[i]->calcFuelReduction(tmpFullFuelMass, models_[i]->getFuelConsumption(), dt);
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

void Thrust::addFuelTank(const std::vector<double> &tanks)
{
    for (size_t i = 0; i < tanks.size(); ++i)
    {
        FuelTank tank;
        tank.id = i;
        tank.mass = tanks[i];
        tank.capacity = tanks[i];

        tanks_.push_back(tank);
    }

    std::cout << "[Thrust] Added " << tanks.size() << " tanks" << std::endl;
}

double Thrust::getFuelMassOfAllTanks() const
{
    FuelTank tmpTank;
    double fuelMassOfAllTanks = 0.0;

    for (auto &tmpTank : tanks_)
    {
        fuelMassOfAllTanks += tmpTank.mass;
    }

    return fuelMassOfAllTanks;
}


