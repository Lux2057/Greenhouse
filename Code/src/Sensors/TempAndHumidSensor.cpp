#include "Sensors/TempAndHumidSensor.h"

TempAndHumidSensor::TempAndHumidSensor(int measurementCount, int measurementDelay)
{
    this->measurementCount = measurementCount;
    this->measurementDelay = measurementDelay;
}

void TempAndHumidSensor::Init()
{
    this->aht.begin();
    delay(500);
}

TempAndHumidSensor::Data TempAndHumidSensor::Get_mean_data()
{
    float mean_temperature = 0.0;
    float mean_humidity = 0.0;
    float t = 0.0;

    for (int i = 0; i < this->measurementCount; i++)
    {
        sensors_event_t humidity, temp;
        aht.getEvent(&humidity, &temp);

        mean_humidity += humidity.relative_humidity;
        mean_temperature += temp.temperature;
        t = temp.temperature;

        delay(this->measurementDelay);
    }

    Data result = Data();
    result.humidity = mean_humidity / this->measurementCount;
    result.temperature = t;//mean_temperature / this->measurementCount;

    return result;
}