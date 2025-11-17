#include "jsonConfigReader.h"

void from_json(const nlohmann::json& j, Vector3& v) 
{
    if (j.is_array() && j.size() == 3) {
        v.x = j[0].get<double>();
        v.y = j[1].get<double>();
        v.z = j[2].get<double>();
    } 
    else {
        v.x = j.at("x").get<double>();
        v.y = j.at("y").get<double>();
        v.z = j.at("z").get<double>();
    }
}


void to_json(nlohmann::json& j, const Vector3& v) {
    j = nlohmann::json{{"x", v.x}, {"y", v.y}, {"z", v.z}};
}

// Loads json Config file and return nlohmann object
nlohmann::json jsonConfigReader::loadConfig(const std::string& filename) 
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open config file: " + filename);
    }

    nlohmann::json j;
    file >> j;
    return j;
}

customSpacecraft jsonConfigReader::parseLander(const nlohmann::json& j)
{
    customSpacecraft lander;

    lander.m            = j.at("m").get<double>();
    lander.fuelM        = j.at("fuelM").get<double>();
    lander.maxFuelM     = j.at("maxFuelM").get<double>();
    lander.maxT         = j.at("maxT").get<double>();
    lander.Isp          = j.at("Isp").get<double>();
    lander.timeConstant = j.at("timeConstant").get<double>();

    lander.B_mainThrustDirection    = j.at("B_mainThrustDirection").get<Vector3>();
    lander.B_mainThrustPosition     = j.at("B_mainThrustPosition").get<Vector3>();

    lander.Ixx          = j.at("Ixx").get<double>();
    lander.Iyy          = j.at("Iyy").get<double>();
    lander.Izz          = j.at("Izz").get<double>();
    
    lander.B_initialPos = j.at("B_initialPos").get<Vector3>();
    lander.B_initialRot = j.at("B_initialRot").get<Vector3>();
    lander.B_centerOfMass = j.at("B_centerOfMass").get<Vector3>();

    return lander;
}


