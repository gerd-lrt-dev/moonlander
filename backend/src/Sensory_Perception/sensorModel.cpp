#include "Sensory_Perception/sensorModel.h"

double SensorModel::computeGLoad(const Vector3& totalAcceleration, const Vector3& gravityAcceleration, bool isLanded) const
{
    // Counterforce upon ground contact
    Vector3 groundReaction = - gravityAcceleration;

    Vector3 properAcceleration = {0.0, 0.0, 0.0};

    // Berechne Proper Acceleration
    if (isLanded)
    {
        properAcceleration = totalAcceleration - gravityAcceleration - groundReaction;
    }
    else
    {
        properAcceleration = totalAcceleration - gravityAcceleration;
    }


    double properNorm = properAcceleration.norm();
    double gLoad = properNorm / configData.earthGravity;

    return gLoad;
}
