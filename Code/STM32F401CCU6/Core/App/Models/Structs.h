#ifndef STRUCTS_H
#define STRUCTS_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rtc.h"
#include "stm32f4xx_hal_i2c.h"
#include "../App/Drivers/PCA9548A.h"
#include "../App/Drivers/MG90s_180.h"

struct AppConfig
{
    PCA9548A::Config Multiplexer;
    MG90S_180::Config Servo;
    RTC_HandleTypeDef* DtSource;
};

#endif // STRUCTS_H