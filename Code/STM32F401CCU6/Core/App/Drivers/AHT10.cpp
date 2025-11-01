#include "AHT10.h"

AHT10::AHT10(Config config) : I2C_DeviceBase(config)
{
    this->_init();
}

bool AHT10::ReInit()
{
    if (!this->_reset())
        return false;

    return this->_init();
}

bool AHT10::_init()
{
    if (!this->_is_ready_to_use())
        return false;

    uint8_t cmds[3] = {0};
    cmds[0] = AHT10_CMD_INIT;
    cmds[1] = 0x08;
    cmds[2] = 0x00;

    this->_update_device_status(HAL_I2C_Master_Transmit(this->_config.I2C, this->_config.DevAddress, cmds, 3, this->_config.Timeout));

    return this->_status == DeviceStatus::Ok;
}

bool AHT10::_reset()
{
    if (!this->_is_ready_to_use())
        return false;

    uint8_t cmd = AHT10_CMD_SOFT_RESET;
    this->_update_device_status(HAL_I2C_Master_Transmit(this->_config.I2C, this->_config.DevAddress, &cmd, 1, this->_config.Timeout));

    return this->_status == DeviceStatus::Ok;
}

HAL_Response<AHT10::Data> AHT10::Measure()
{
    HAL_Response<AHT10::Data> response;

    if (!this->_is_ready_to_use())
    {
        response.Status = HAL_StatusTypeDef::HAL_ERROR;
        return response;
    }

    this->_trigger_measure();

    if (this->_status != DeviceStatus::Ok)
    {
        response.Status = HAL_StatusTypeDef::HAL_ERROR;
        return response;
    }

    HAL_Delay(100);

    this->_read_data();

    if (this->_status != DeviceStatus::Ok)
    {
        response.Status = HAL_StatusTypeDef::HAL_ERROR;
        return response;
    }

    response.Value.Humidity = this->_humidity();
    response.Value.Temperature = this->_temperature();

    response.Status = HAL_StatusTypeDef::HAL_OK;

    return response;
}

HAL_StatusTypeDef AHT10::_read_data()
{
    HAL_StatusTypeDef status = HAL_I2C_Master_Receive(this->_config.I2C, this->_config.DevAddress, this->_data, 6, this->_config.Timeout);

    this->_update_device_status(status);

    return status;
}

float AHT10::_humidity()
{
    uint8_t humiTemp[3] = {0};
    uint32_t humi = 0;
    float tempHumidity = 0;

    humiTemp[0] = this->_data[1];
    humiTemp[1] = this->_data[2];
    humiTemp[2] = this->_data[3] & 0xF0;

    for (int i = 0; i < 3; i++)
    {
        humi += humiTemp[i];
        humi <<= 8;
    }
    humi >>= 12;
    tempHumidity = (humi / 1048576.0f) * 100.0f;

    for (int i = 0; i < 3; i++)
    {
        humiTemp[i] = 0;
    }
    humi = 0;

    return tempHumidity;
}

float AHT10::_temperature()
{
    uint8_t tempTemp[3] = {0};
    uint32_t temp = 0;
    float tempTemperature = 0;

    tempTemp[0] = (this->_data[3] & 0x0F) << 4;
    tempTemp[0] += (this->_data[4] & 0xF0) >> 4;
    tempTemp[1] = (this->_data[4] & 0x0F) << 4;
    tempTemp[1] += (this->_data[5] & 0xF0) >> 4;
    tempTemp[2] = (this->_data[5] & 0x0F) << 4;
    tempTemp[2] += (this->_data[6] & 0xF0) >> 4;

    for (int i = 0; i < 3; i++)
    {
        temp += tempTemp[i];
        temp <<= 8;
    }
    temp >>= 12;
    tempTemperature = ((temp / 1048576.0f) * 200.0f) - 50.0f;

    for (int i = 0; i < 3; i++)
    {
        tempTemp[i] = 0;
    }
    temp = 0;

    return tempTemperature;
}

void AHT10::_trigger_measure()
{
    uint8_t cmds[3] = {0};
    cmds[0] = AHT10_CMD_MEASURE_TRIG;
    cmds[1] = 0x33;
    cmds[2] = 0x00;
    this->_update_device_status(HAL_I2C_Master_Transmit(this->_config.I2C, this->_config.DevAddress, cmds, 3, this->_config.Timeout));
}
