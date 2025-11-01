#ifndef HUMIDITY_AND_TEMPERATURE_CONTROLLER_H
#define HUMIDITY_AND_TEMPERATURE_CONTROLLER_H

#include <memory>
#include "../App/Drivers/AHT10.h"
#include "../App/Models/Enums.h"
#include "../App/Models/Templates.h"
#include "../App/Models/Structs.h"
#include "I2C_ControllerBase.h"

using namespace std;

class HumidityAndTemperatureController : public I2C_ControllerBase<AHT10>
{
public:
    HumidityAndTemperatureController() = delete;
    HumidityAndTemperatureController(I2C_ControllerConfig config, shared_ptr<PCA9548A> multiplexer);

    void InitDevices() override;
    DeviceStatus Status(uint8_t device_index) override;

    ValidatedValue<AHT10::Data> Value();

private:
    AHT10::Config _sensor_config;
};

#endif // HUMIDITY_AND_TEMPERATURE_CONTROLLER_H