#ifndef SENSORS_CONTROLLER_H
#define SENSORS_CONTROLLER_H

#include <memory>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32f4xx_hal_adc.h"
#include "../App/Drivers/PCA9548A.h"
#include "../App/Drivers/AHT10.h"
#include "../App/Drivers/APDS9930.h"
#include "../App/Drivers/CSMS.h"
#include "../App/Models/Enums.h"
#include "../App/Models/Templates.h"
#include "../App/Models/Structs.h"

using namespace std;

class SensorsController
{
public:
    SensorsController() = delete;
    SensorsController(shared_ptr<PCA9548A> multiplexer,
                      I2C_HandleTypeDef *i2c,
                      ADC_HandleTypeDef *adc);

    void InitClimaticSensors();
    void InitLightSensors();

    uint8_t ClimaticSensorsCount() const;
    uint8_t LightSensorsCount() const;

    DeviceStatus ClimaticSensorStatus(uint8_t index);
    DeviceStatus LightSensorStatus(uint8_t index);

    ValidatedValue<AHT10::Data> HumidityAndTemperature();
    ValidatedValue<float> AmbientLight();
    ValidatedValue<uint32_t> SoilMoisture();

private:
    static const uint8_t _climatic_sensors_count{2};
    static const uint8_t _light_sensors_count{2};

    I2C_HandleTypeDef *_i2c{nullptr};

    I2C_Device<AHT10> _climatic_sensors[_climatic_sensors_count]{I2C_Device<AHT10>(0), I2C_Device<AHT10>(2)};
    I2C_Device<APDS9930> _light_sensors[_light_sensors_count]{I2C_Device<APDS9930>(1), I2C_Device<APDS9930>(3)};

    shared_ptr<PCA9548A> _multiplexer{nullptr};

    shared_ptr<CSMS> _soil_moisture_sensor{nullptr};

    template <typename T_Device>
    void _activate_device(I2C_Device<T_Device> *devices, uint8_t count, uint8_t index);
};

#endif // SENSORS_CONTROLLER_H
