#include "I2C_DeviceBase.h"

I2C_DeviceBase::I2C_DeviceBase(Config config)
{
    this->_config = config;

    if (config.I2C == nullptr)
    {
        this->_status = DeviceStatus::Incorrect_Config;
    }

    HAL_StatusTypeDef availability = HAL_I2C_IsDeviceReady(this->_config.I2C, this->_config.DevAddress, 3, this->_config.Timeout);
    this->_status = availability == HAL_StatusTypeDef::HAL_OK ? DeviceStatus::Ok : DeviceStatus::Unavailable;
}

DeviceStatus I2C_DeviceBase::Status() const
{
    return this->_status;
}

void I2C_DeviceBase::_update_device_status(HAL_StatusTypeDef status)
{
    if (this->_status == DeviceStatus::Incorrect_Config)
        return;

    if (status == HAL_StatusTypeDef::HAL_OK)
    {
        this->_status = DeviceStatus::Ok;

        return;
    }

    HAL_StatusTypeDef availability = HAL_I2C_IsDeviceReady(this->_config.I2C, this->_config.DevAddress << 1, 3, this->_config.Timeout);

    this->_status = availability == HAL_StatusTypeDef::HAL_OK ? DeviceStatus::Error : DeviceStatus::Unavailable;
}

bool I2C_DeviceBase::_is_ready_to_use()
{
    switch (this->_status)
    {
    case DeviceStatus::Ok:
        return true;

    case DeviceStatus::Unavailable:
    {
        if (HAL_I2C_IsDeviceReady(this->_config.I2C, this->_config.DevAddress, 3, this->_config.Timeout) == HAL_StatusTypeDef::HAL_OK)
        {
            this->_status = DeviceStatus::Ok;
            return true;
        }

        return false;
    }

    default:
        return false;
    }
}
