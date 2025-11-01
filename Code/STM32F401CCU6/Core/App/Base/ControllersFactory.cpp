#include "ControllersFactory.h"

ControllersFactory::ControllersList ControllersFactory::Build(PCA9548A::Config config)
{
    ControllersList controllers;

    LED_Controller::Config LED_config;
    controllers.LED = shared_ptr<LED_Controller>(new LED_Controller(LED_config));

    I2C_DeviceBase::Config display_config;
    display_config.I2C = config.I2C;
    display_config.DevAddress = SSD1306_DEFAULT_I2C_ADDRESS;    
    controllers.Display = shared_ptr<SSD1306>(new SSD1306(display_config));

    controllers.Multiplexer = shared_ptr<PCA9548A>(new PCA9548A(config));

    controllers.AmbientLightConfig.I2C = config.I2C;
    controllers.AmbientLightConfig.MultiplexerChannelsCount = 1;
    controllers.AmbientLightConfig.MultiplexerChannels = new uint8_t[1]{2};
    controllers.AmbientLight = shared_ptr<AmbientLightController>(new AmbientLightController(controllers.AmbientLightConfig, controllers.Multiplexer));

    controllers.HumidityAndTemperatureConfig.MultiplexerChannelsCount = 2;
    controllers.HumidityAndTemperatureConfig.MultiplexerChannels = new uint8_t[2]{0, 1};
    controllers.HumidityAndTemperature = shared_ptr<HumidityAndTemperatureController>(new HumidityAndTemperatureController(controllers.HumidityAndTemperatureConfig, controllers.Multiplexer));    

    return controllers;
}
