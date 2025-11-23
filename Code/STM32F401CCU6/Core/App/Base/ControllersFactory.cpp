#include "ControllersFactory.h"

ControllersFactory::ControllersList ControllersFactory::Build(AppConfig config)
{
    ControllersList controllers;

    LED_Controller::Config LED_config;
    controllers.LED = shared_ptr<LED_Controller>(new LED_Controller(LED_config));

    /*I2C_DeviceBase::Config display_config;
    display_config.I2C = config.I2C;
    display_config.DevAddress = SSD1306_DEFAULT_I2C_ADDRESS;
    controllers.Display = shared_ptr<SSD1306>(new SSD1306(display_config));*/

    /*controllers.Multiplexer = shared_ptr<PCA9548A>(new PCA9548A(config.Multiplexer));

    controllers.Sensors = shared_ptr<SensorsController>(new SensorsController(controllers.Multiplexer, config.Multiplexer.I2C));*/

    controllers.Servo = shared_ptr<MG90S_180>(new MG90S_180(config.Servo));

    return controllers;
}
