#ifndef TEMPLATES_H
#define TEMPLATES_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"
#include <memory>

using namespace std;

template <typename T>
struct HAL_Response
{
public:
    HAL_StatusTypeDef Status;
    T Value;
};

template <typename T>
struct ValidatedValue
{
public:
    bool Valid{false};
    T Value;
};

template <typename T>
struct I2C_Device
{
    uint8_t MultiplexerChannel;
    shared_ptr<T> Sensor{nullptr};
};

#endif // TEMPLATES_H
