#ifndef PCA9548A_H
#define PCA9548A_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32f4xx_hal_gpio.h"
#include "I2C_DeviceBase.h"
#include "../Models/Enums.h"
#include "../Models/Templates.h"

class PCA9548A : public I2C_DeviceBase
{
#define PCA9548A_BASE_I2C_ADDR 112
#define PCA9548A_RESET_TIME_LOW 1
#define PCA9548A_RESET_TIME_HIGH 0

public:
    struct Config
    {
    public:
        I2C_HandleTypeDef *I2C{nullptr};
        uint16_t ResetPin;
        GPIO_TypeDef *ResetPinPort;
        uint16_t A0_Pin;
        GPIO_TypeDef *A0_PinPort;
        uint16_t A1_Pin;
        GPIO_TypeDef *A1_PinPort;
        uint16_t A2_Pin;
        GPIO_TypeDef *A2_PinPort;
        uint8_t HardwareAddress{0};
        uint32_t Timeout{10};
    };

    PCA9548A() = delete;
    PCA9548A(Config mux_config);

    HAL_StatusTypeDef SelectChannel(uint8_t channel, bool val);

    bool ReInit() override;

private:
    Config _mux_config;

    bool _init() override;

    HAL_StatusTypeDef _set_hardware_address();

    HAL_StatusTypeDef _set_channels(uint8_t mask);
    HAL_StatusTypeDef _reset();

    static I2C_DeviceBase::Config _base_config(Config mux_config);
    static uint8_t _normalize_hardware_address(uint8_t val);
    static uint8_t _set_bit(uint8_t initial, uint8_t position, bool set);
};

#endif // PCA9548A_H