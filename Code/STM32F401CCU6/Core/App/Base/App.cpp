#include "App.h"

App::App(ControllersFactory::Config config)
{
    ControllersFactory controllersFactory;
    this->_controllers = controllersFactory.Build(config);

    this->_config = config;
}

void App::Start()
{
    Validated<APDS9930_Controller::AmbientLight> ambient(false, APDS9930_Controller::AmbientLight());

    while (true)
    {
        ambient = this->_controllers.LightSensor->ReadAmbientLight();

        if (ambient.Valid)
        {
            this->_controllers.LED->Blink(this->_controllers.LED->OK);
        }

        HAL_Delay(1000);
    }
}