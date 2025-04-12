#ifndef LIGHT_H
#define LIGHT_H

#include <Arduino.h>
#include <APDS9930.h>

class LightSensor
{
private:
    APDS9930 apds9930;
    int measurementCount;
    int measurementDelay;

public:
    LightSensor(int measurementCount = 10, int measurementDelay = 50);

    float Get_mean_data();
    void Init();
};

#endif