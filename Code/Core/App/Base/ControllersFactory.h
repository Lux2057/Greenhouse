#ifndef CONTROLLERS_LIST_FACTORY_H
#define CONTROLLERS_LIST_FACTORY_H

#include <memory>
#include "../Controllers/LED_Controller.h"
#include "../Controllers/APDS9930_Controller.h"

class ControllersFactory
{
public:
    struct Config
    {
        I2C_HandleTypeDef *I2C{nullptr};
    };

    struct ControllersList
    {
    public:
        shared_ptr<LED_Controller> LED{nullptr};
        shared_ptr<APDS9930_Controller> LightSensor{nullptr};
    };

    ControllersList Build(Config config);
};

#endif // CONTROLLERS_LIST_FACTORY_H