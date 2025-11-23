#ifndef MG90S_H
#define MG90S_H

#include "stm32f401xc.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include <cstdint>

using namespace std;

class MG90S_180
{

#define MG90S_PWM_MIN_LENGTH 500
#define MG90S_PMW_MAX_LENGTH 2700
#define MG90S_PWM_LENGTH_COEFFICIENT (MG90S_PMW_MAX_LENGTH - MG90S_PWM_MIN_LENGTH) / 180

public:
    enum class RegisterName
    {
        CCR1 = 0,
        CCR2 = 1,
        CCR3 = 2,
        CCR4 = 3
    };
    struct Config
    {
    public:
        TIM_HandleTypeDef *TIM_Handle{nullptr};
        uint32_t TIM_HandleChannel;
        TIM_TypeDef *TIM{nullptr};
        RegisterName TIM_RegisterName;
        uint8_t InitAngle{0};
    };

    MG90S_180() = delete;
    MG90S_180(Config config);

    uint8_t CurrentAngle() const;

    HAL_StatusTypeDef Rotate(uint8_t angle);
    HAL_StatusTypeDef Rotate(uint8_t angle, uint8_t step, uint8_t step_delay);

private:
    Config _config;
    uint8_t _current_angle;
        
    void _write_angle(uint8_t angle);
};

#endif // MG90S_H