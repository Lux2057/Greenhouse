#include "SensorsController.h"

SensorsController::SensorsController(shared_ptr<PCA9548A> multiplexer,
                                     I2C_HandleTypeDef *i2c,
                                     ADC_HandleTypeDef *adc)
{
    this->_multiplexer = multiplexer;
    this->_i2c = i2c;

    InitLightSensors();
    InitClimaticSensors();

    this->_soil_moisture_sensor = shared_ptr<CSMS>(new CSMS(adc));
}

void SensorsController::InitClimaticSensors()
{
    AHT10::Config config;
    config.I2C = this->_i2c;
    config.DevAddress = AHT10_DEFAULT_I2C_DEV_ADDRESS;

    for (int i = 0; i < _climatic_sensors_count; i++)
    {
        this->_activate_device(this->_climatic_sensors, _climatic_sensors_count, i);

        if (this->_climatic_sensors[i].Device == nullptr)
        {
            this->_climatic_sensors[i].Device = shared_ptr<AHT10>(new AHT10(config));
        }
        else
        {
            this->_climatic_sensors[i].Device->ReInit();
        }
    }
}

void SensorsController::InitLightSensors()
{
    APDS9930::Config config;
    config.I2C = this->_i2c;
    config.DevAddress = APDS9930_DEFAULT_I2C_DEV_ADDRESS;

    for (int i = 0; i < _light_sensors_count; i++)
    {
        this->_activate_device(this->_light_sensors, _light_sensors_count, i);

        if (this->_light_sensors[i].Device == nullptr)
        {
            this->_light_sensors[i].Device = shared_ptr<APDS9930>(new APDS9930(config));
        }
        else
        {
            this->_light_sensors[i].Device->ReInit();
        }

        this->_light_sensors[i].Device->EnableLightSensor(true, false);
        this->_light_sensors[i].Device->SetPower(true);
    }
}

uint8_t SensorsController::ClimaticSensorsCount() const
{
    return _climatic_sensors_count;
}

uint8_t SensorsController::LightSensorsCount() const
{
    return _light_sensors_count;
}

DeviceStatus SensorsController::ClimaticSensorStatus(uint8_t index)
{
    if (index >= _climatic_sensors_count || this->_climatic_sensors[index].Device == nullptr)
    {
        return DeviceStatus::Unavailable;
    }

    this->_activate_device(this->_climatic_sensors, _climatic_sensors_count, index);

    return this->_climatic_sensors[index].Device->Status();
}

DeviceStatus SensorsController::LightSensorStatus(uint8_t index)
{
    if (index >= _light_sensors_count || this->_light_sensors[index].Device == nullptr)
    {
        return DeviceStatus::Unavailable;
    }

    this->_activate_device(this->_light_sensors, _light_sensors_count, index);

    return this->_light_sensors[index].Device->Status();
}

ValidatedValue<AHT10::Data> SensorsController::HumidityAndTemperature()
{
    uint8_t measures_count = 0;
    ValidatedValue<AHT10::Data> response;
    response.Value.Humidity = 0.0;
    response.Value.Temperature = 0.0;
    HAL_Response<AHT10::Data> data;

    for (int i = 0; i < _climatic_sensors_count; i++)
    {
        this->_activate_device(_climatic_sensors, _climatic_sensors_count, i);

        data = this->_climatic_sensors[i].Device->Measure();

        if (data.Status != HAL_StatusTypeDef::HAL_OK)
        {
            continue;
        }

        response.Value.Humidity += data.Value.Humidity;
        response.Value.Temperature += data.Value.Temperature;
        measures_count++;
    }

    if (measures_count == 0)
    {
        response.Valid = false;
        return response;
    }

    response.Valid = true;
    response.Value.Humidity /= measures_count;
    response.Value.Temperature /= measures_count;

    return response;
}

ValidatedValue<float> SensorsController::AmbientLight()
{
    uint8_t measures_count = 0;
    ValidatedValue<float> response;
    response.Value = 0.0;
    HAL_Response<float> data;

    for (int i = 0; i < _light_sensors_count; i++)
    {
        this->_activate_device(_light_sensors, _light_sensors_count, i);

        data = this->_light_sensors[i].Device->AmbientLight();

        if (data.Status != HAL_StatusTypeDef::HAL_OK)
        {
            continue;
        }

        response.Value += data.Value;
        measures_count++;
    }

    if (measures_count == 0)
    {
        response.Valid = false;
        return response;
    }

    response.Valid = true;
    response.Value /= measures_count;

    return response;
}

ValidatedValue<uint32_t> SensorsController::SoilMoisture()
{
    HAL_Response<uint32_t> data = this->_soil_moisture_sensor->Value();

    ValidatedValue<uint32_t> response;

    response.Valid = data.Status == HAL_StatusTypeDef::HAL_OK;
    response.Value = data.Value;

    return response;
}

template <typename T_Device>
void SensorsController::_activate_device(I2C_Device<T_Device> *devices, uint8_t count, uint8_t index)
{
    if (index >= count)
        return;

    for (int i = 0; i < count; i++)
    {
        this->_multiplexer->SelectChannel(devices[i].MultiplexerChannel, i == index);
    }
}
