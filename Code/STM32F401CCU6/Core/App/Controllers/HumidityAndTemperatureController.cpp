#include "HumidityAndTemperatureController.h"

HumidityAndTemperatureController::HumidityAndTemperatureController(I2C_ControllerConfig config, shared_ptr<PCA9548A> multiplexer) : I2C_ControllerBase(config, multiplexer)
{
    AHT10::Config sensor_config;
    sensor_config.I2C = config.I2C;
    sensor_config.DevAddress = AHT10_DEFAULT_I2C_DEV_ADDRESS;
    sensor_config.Timeout = 100;
    this->_sensor_config = sensor_config;

    this->InitDevices();
}

void HumidityAndTemperatureController::InitDevices()
{
    if (this->_devices == nullptr)
    {
        this->_devices = new I2C_Device<AHT10>[this->_config.MultiplexerChannelsCount] {};

        I2C_Device<AHT10> device;
        for (int i = 0; i < this->_config.MultiplexerChannelsCount; i++)
        {
            this->_activate_channel(this->_config.MultiplexerChannels[i]);

            device.Sensor = shared_ptr<AHT10>(new AHT10(this->_sensor_config));
            device.MultiplexerChannel = this->_config.MultiplexerChannels[i];
            this->_devices[i] = device;
        }
    }
    else
    {
        for (int i = 0; i < this->_devices_count; i++)
        {
            this->_activate_channel(this->_devices[i].MultiplexerChannel);
            this->_devices[i].Sensor->ReInit();
        }
    }
}

ValidatedValue<AHT10::Data> HumidityAndTemperatureController::Value()
{
    ValidatedValue<AHT10::Data> response;

    if (this->_devices == nullptr)
    {
        response.Valid = false;
        return response;
    }

    HAL_Response<AHT10::Data> data;
    uint8_t responses_count = 0;
    for (int i = 0; i < this->_devices_count; i++)
    {
        this->_activate_channel(this->_devices[i].MultiplexerChannel);

        data = this->_devices[i].Sensor->Measure();

        if (data.Status != HAL_StatusTypeDef::HAL_OK)
            continue;

        response.Value.Humidity += data.Value.Humidity;
        response.Value.Temperature += data.Value.Temperature;
        responses_count++;
    }

    if (responses_count == 0)
    {
        response.Valid = false;
        return response;
    }

    response.Valid = true;

    if (responses_count > 1)
    {
        response.Value.Humidity /= responses_count;
        response.Value.Temperature /= responses_count;
    }

    return response;
}

DeviceStatus HumidityAndTemperatureController::Status(uint8_t device_index)
{
    if (this->_devices == nullptr || device_index >= this->_devices_count)
    {
        return DeviceStatus::Unavailable;
    }

    this->_activate_channel(this->_devices[device_index].MultiplexerChannel);

    return this->_devices[device_index].Sensor->Status();
}
