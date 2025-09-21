#include "App.h"

App::App(ControllersFactory::Config config)
{
    ControllersFactory controllersFactory;
    this->_controllers = controllersFactory.Build(config);

    this->_config = config;
}

void App::Start()
{
    bool ok = false;
    LED_Controller::BlinkConfig blinkConfig = this->_controllers.LED->OK;

    while (true)
    {
        if (ok)
        {
            blinkConfig = this->_controllers.LED->OK;
        }
        else
        {
            blinkConfig = this->_controllers.LED->ERROR;
        }

        this->_controllers.LED->Blink(blinkConfig);

        ok = !ok;

        HAL_Delay(1000);
    }
}