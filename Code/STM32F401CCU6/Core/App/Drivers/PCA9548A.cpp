#include "PCA9548A.h"

PCA9548A::PCA9548A(PCA9548A::Config mux_config) : I2C_DeviceBase(_base_config(mux_config))
{
    this->_mux_config = mux_config;
    this->_mux_config.HardwareAddress = _normalize_hardware_address(mux_config.HardwareAddress);

    this->_init();
}

HAL_StatusTypeDef PCA9548A::SelectChannel(uint8_t channel, bool enable)
{
    channel = this->_normalize_hardware_address(channel);

    uint8_t channels_value;
    HAL_StatusTypeDef status = HAL_I2C_Master_Receive(this->_config.I2C, this->_config.DevAddress, &channels_value, 1, this->_config.Timeout);

    if (status != HAL_StatusTypeDef::HAL_OK)
    {
        this->_update_device_status(status);
        return status;
    }

    return this->_set_channels(_set_bit(channels_value, channel, enable));
}

bool PCA9548A::ReInit()
{
    HAL_StatusTypeDef status = this->_reset();
    if (status != HAL_StatusTypeDef::HAL_OK)
    {
        this->_update_device_status(status);
        return status;
    }

    status = this->_set_channels(0);

    this->_update_device_status(status);

    return status;
}

bool PCA9548A::_init()
{
    HAL_GPIO_WritePin(this->_mux_config.ResetPinPort, this->_mux_config.ResetPin, GPIO_PinState::GPIO_PIN_SET);
    HAL_Delay(PCA9548A_RESET_TIME_HIGH);

    HAL_StatusTypeDef status = this->_set_hardware_address();
    if (status != HAL_StatusTypeDef::HAL_OK)
    {
        this->_update_device_status(status);
        return status;
    }

    status = this->_set_channels(0);

    this->_update_device_status(status);

    return status;
}

I2C_DeviceBase::Config PCA9548A::_base_config(PCA9548A::Config mux_config)
{
    I2C_DeviceBase::Config config;
    config.I2C = mux_config.I2C;
    config.DevAddress = (PCA9548A_BASE_I2C_ADDR + _normalize_hardware_address(mux_config.HardwareAddress));
    config.Timeout = mux_config.Timeout;

    return config;
}

uint8_t PCA9548A::_normalize_hardware_address(uint8_t val)
{
    return val > 7 ? 7 : val;
}

uint8_t PCA9548A::_set_bit(uint8_t value, uint8_t position, bool set)
{
    uint8_t mask = 1 << position;

    if (set)
    {
        value |= mask;
    }
    else
    {
        value &= ~mask;
    }

    return value;
}

HAL_StatusTypeDef PCA9548A::_set_hardware_address()
{
    uint8_t temp = this->_mux_config.HardwareAddress;

    uint8_t bit = temp & 1;
    HAL_GPIO_WritePin(this->_mux_config.A0_PinPort, this->_mux_config.A0_Pin, bit == 1 ? GPIO_PinState::GPIO_PIN_SET : GPIO_PinState::GPIO_PIN_RESET);

    temp = temp >> 1;
    bit = temp & 1;
    HAL_GPIO_WritePin(this->_mux_config.A1_PinPort, this->_mux_config.A1_Pin, bit == 1 ? GPIO_PinState::GPIO_PIN_SET : GPIO_PinState::GPIO_PIN_RESET);

    temp = temp >> 1;
    bit = temp & 1;
    HAL_GPIO_WritePin(this->_mux_config.A2_PinPort, this->_mux_config.A2_Pin, bit == 1 ? GPIO_PinState::GPIO_PIN_SET : GPIO_PinState::GPIO_PIN_RESET);

    HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(this->_config.I2C, this->_config.DevAddress, 3, this->_config.Timeout);

    this->_update_device_status(status);

    return status;
}

HAL_StatusTypeDef PCA9548A::_set_channels(uint8_t mask)
{
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(this->_config.I2C, this->_config.DevAddress, &mask, 1, this->_config.Timeout);
    if (status != HAL_StatusTypeDef::HAL_OK)
    {
        this->_update_device_status(status);
        return status;
    }

    uint8_t current_mask = 0;
    status = HAL_I2C_Master_Receive(this->_config.I2C, this->_config.DevAddress, &current_mask, 1, this->_config.Timeout);

    if (status != HAL_StatusTypeDef::HAL_OK)
    {
        this->_update_device_status(status);
        return status;
    }

    HAL_StatusTypeDef result = current_mask == mask ? status : HAL_StatusTypeDef::HAL_ERROR;

    this->_update_device_status(result);

    return result;
}

HAL_StatusTypeDef PCA9548A::_reset()
{
    HAL_GPIO_WritePin(this->_mux_config.ResetPinPort, this->_mux_config.ResetPin, GPIO_PinState::GPIO_PIN_RESET);
    HAL_Delay(PCA9548A_RESET_TIME_LOW);

    HAL_GPIO_WritePin(this->_mux_config.ResetPinPort, this->_mux_config.ResetPin, GPIO_PinState::GPIO_PIN_SET);
    HAL_Delay(PCA9548A_RESET_TIME_HIGH);

    return this->_set_channels(0);
}