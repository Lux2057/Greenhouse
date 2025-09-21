#ifndef APDS9930_CONTROLLER_H
#define APDS9930_CONTROLLER_H

#include <memory>
#include "../App/Drivers/APDS9930.h"
#include "../App/Models/Enums.h"
#include "../App/Models/Templates.h"

class APDS9930_Controller
{
public:
    class AmbientLight
    {
    public:
        float Lux{0.0};
        uint16_t Ch0{0};
        uint16_t Ch1{1};
    };

    APDS9930_Controller() = delete;
    APDS9930_Controller(I2C_HandleTypeDef* hi2c1);

    Validated<AmbientLight> ReadAmbientLight();

private:
    shared_ptr<APDS9930> _sensor{nullptr};
    Status _status{Status::Unknown};
    int16_t _errorCode{0};
};

#endif // APDS9930_CONTROLLER_H