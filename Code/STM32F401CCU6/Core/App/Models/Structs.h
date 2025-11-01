#ifndef STRUCTS_H
#define STRUCTS_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"

struct I2C_ControllerConfig
{
public:
    I2C_HandleTypeDef *I2C{nullptr};
    uint8_t *MultiplexerChannels{nullptr};
    uint8_t MultiplexerChannelsCount;
};

#endif // STRUCTS_H