#ifndef CONTROLLERS_LIST_FACTORY_H
#define CONTROLLERS_LIST_FACTORY_H

#include <memory>
#include "../Models/Structs.h"
#include "../Models/Templates.h"
#include "../Controllers/I2C_ControllerBase.h"
#include "../Controllers/LED_Controller.h"
#include "../Controllers/AmbientLightController.h"
#include "../Controllers/HumidityAndTemperatureController.h"
#include "../Drivers/PCA9548A.h"
#include "../Drivers/SSD1306.h"
#include "../Drivers/I2C_DeviceBase.h"

using namespace std;

class ControllersFactory
{
public:
    struct ControllersList
    {
    public:
        shared_ptr<LED_Controller> LED{nullptr};

        shared_ptr<AmbientLightController> AmbientLight{nullptr};
        I2C_ControllerConfig AmbientLightConfig;

        shared_ptr<HumidityAndTemperatureController> HumidityAndTemperature{nullptr};
        I2C_ControllerConfig HumidityAndTemperatureConfig;        

        shared_ptr<PCA9548A> Multiplexer{nullptr};

        shared_ptr<SSD1306> Display{nullptr};
    };

    ControllersList Build(PCA9548A::Config config);
};

#endif // CONTROLLERS_LIST_FACTORY_H