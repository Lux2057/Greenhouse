#ifndef AMBIENT_LIGHT_CONTROLLER_H
#define AMBIENT_LIGHT_CONTROLLER_H

#include <memory>
#include "../App/Drivers/APDS9930.h"
#include "../App/Models/Enums.h"
#include "../App/Models/Templates.h"
#include "../App/Models/Structs.h"
#include "I2C_ControllerBase.h"

using namespace std;

class AmbientLightController : public I2C_ControllerBase<APDS9930>
{
public:
    AmbientLightController() = delete;
    AmbientLightController(I2C_ControllerConfig config, shared_ptr<PCA9548A> multiplexer);

    void InitDevices() override;
    
    ValidatedValue<float> Value();

    DeviceStatus Status(uint8_t device_index) override;    

private:    
    APDS9930::Config _sensor_config;
};

#endif // AMBIENT_LIGHT_CONTROLLER_H