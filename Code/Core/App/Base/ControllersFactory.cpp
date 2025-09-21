#include "ControllersFactory.h"

ControllersFactory::ControllersList ControllersFactory::Build(ControllersFactory::Config config)
{
    ControllersList controllers;

    LED_Controller::Config LED_config;
    controllers.LED = shared_ptr<LED_Controller>(new LED_Controller(LED_config));

    controllers.LightSensor = shared_ptr<APDS9930_Controller>(new APDS9930_Controller(config.I2C));

    return controllers;
}
