#ifndef I2C_DEVICE_H
#define I2C_DEVICE_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"
#include "../Models/Enums.h"

class I2C_DeviceBase
{
public:
    struct Config
    {
    public:
        I2C_HandleTypeDef *I2C{nullptr};
        uint8_t DevAddress;
        uint32_t Timeout{30};
    };

    I2C_DeviceBase() = delete;
    I2C_DeviceBase(Config config);

    virtual bool ReInit() = 0;

    DeviceStatus Status() const;

protected:
    DeviceStatus _status{DeviceStatus::Unknown};
    Config _config;

    virtual bool _init() = 0;

    void _update_device_status(HAL_StatusTypeDef status);
    bool _is_ready_to_use();
};

#endif // I2C_DEVICE_H