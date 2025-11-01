#include "AmbientLightController.h"

AmbientLightController::AmbientLightController(I2C_ControllerConfig config, shared_ptr<PCA9548A> multiplexer) : I2C_ControllerBase(config, multiplexer)
{
    APDS9930::Config sensor_config;
    sensor_config.I2C = config.I2C;
    sensor_config.DevAddress = APDS9930_DEFAULT_I2C_DEV_ADDRESS;
    this->_sensor_config = sensor_config;

    this->InitDevices();
}

void AmbientLightController::InitDevices()
{
    if (this->_devices == nullptr)
    {
        I2C_Device<APDS9930> device;
        for (int i = 0; i < this->_config.MultiplexerChannelsCount; i++)
        {
            this->_activate_channel(this->_config.MultiplexerChannels[i]);

            device.Sensor = shared_ptr<APDS9930>(new APDS9930(this->_sensor_config));
            device.MultiplexerChannel = this->_config.MultiplexerChannels[i];
        }
    }
    else
    {
        for (int i = 0; i < this->_devices_count; i++)
        {
            this->_activate_channel(this->_devices[i].MultiplexerChannel);

            this->_devices[i].Sensor->ReInit();
            this->_devices[i].Sensor->EnableLightSensor(true, false);
            this->_devices[i].Sensor->SetPower(true);
        }
    }
}

ValidatedValue<float> AmbientLightController::Value()
{
    ValidatedValue<float> response;

    if (this->_devices == nullptr)
    {
        response.Valid = false;
        return response;
    }

    HAL_Response<float> data;
    uint8_t responses_count = 0;
    for (int i = 0; i < this->_devices_count; i++)
    {
        this->_activate_channel(this->_devices[i].MultiplexerChannel);

        data = this->_devices[i].Sensor->AmbientLight();

        if (data.Status != HAL_StatusTypeDef::HAL_OK)
            continue;

        response.Value += data.Value;
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
        response.Value /= responses_count;
    }

    return response;
}

DeviceStatus AmbientLightController::Status(uint8_t device_index)
{
    if (this->_devices == nullptr || device_index >= this->_devices_count)
    {
        return DeviceStatus::Unavailable;
    }

    this->_activate_channel(this->_devices[device_index].MultiplexerChannel);

    return this->_devices[device_index].Sensor->Status();
}
