#ifndef CONTROLLERS_LIST_FACTORY_H
#define CONTROLLERS_LIST_FACTORY_H

#include <memory>
#include "../Models/Structs.h"
#include "../Models/Templates.h"
#include "../Controllers/LED_Controller.h"
#include "../Controllers/SensorsController.h"
#include "../Drivers/PCA9548A.h"
#include "../Drivers/SSD1306.h"
#include "../Drivers/I2C_DeviceBase.h"
#include "../Drivers/MG90s_180.h"

using namespace std;

class ControllersFactory
{
public:
    struct ControllersList
    {
    public:
        shared_ptr<LED_Controller> LED{nullptr};

        shared_ptr<SensorsController> Sensors{nullptr};

        shared_ptr<PCA9548A> Multiplexer{nullptr};

        shared_ptr<SSD1306> Display{nullptr};

        shared_ptr<MG90S_180> Servo{nullptr};
    };

    ControllersList Build(AppConfig config);
};

#endif // CONTROLLERS_LIST_FACTORY_H