#include "Sensory_Perception/sensorModel.h"

double SensorModel::computeGLoad(const Vector3& totalAcceleration, const Vector3& gravityAcceleration) const
{
    // Berechne Proper Acceleration
    Vector3 properAcceleration = totalAcceleration - gravityAcceleration;

    double gLoad = properAcceleration.norm() / configData.earthGravity;

    return gLoad;
}
