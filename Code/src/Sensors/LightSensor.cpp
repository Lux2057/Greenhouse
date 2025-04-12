#include "Sensors/LightSensor.h"

LightSensor::LightSensor(int measurementCount, int measurementDelay)
{
    this->measurementCount = measurementCount;
    this->measurementDelay = measurementDelay;
}

void LightSensor::Init()
{
    this->apds9930.init();
    this->apds9930.enableLightSensor();
    delay(500);
}

float LightSensor::Get_mean_data()
{
    float mean_light = 0.0;
    float current_light = 0.0;

    for (int i = 0; i < this->measurementCount; i++)
    {
        this->apds9930.readAmbientLightLux(current_light);

        mean_light += current_light;

        delay(this->measurementDelay);
    }

    return mean_light / this->measurementCount;
}