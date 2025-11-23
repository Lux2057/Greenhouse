#include "MG90s_180.h"

MG90S_180::MG90S_180(Config config)
{
    this->_config = config;

    this->Rotate(this->_config.InitAngle);
}

uint8_t MG90S_180::CurrentAngle() const
{
    return this->_current_angle;
}

HAL_StatusTypeDef MG90S_180::Rotate(uint8_t angle)
{
    if (angle > 180)
        angle = 180;

    this->_write_angle(angle);

    HAL_StatusTypeDef status = HAL_TIM_PWM_Start(this->_config.TIM_Handle, this->_config.TIM_HandleChannel);

    if (status != HAL_StatusTypeDef::HAL_OK)
    {
        return HAL_TIM_PWM_Stop(this->_config.TIM_Handle, this->_config.TIM_HandleChannel);
    }

    this->_current_angle = angle;

    return HAL_TIM_PWM_Stop(this->_config.TIM_Handle, this->_config.TIM_HandleChannel);
}

HAL_StatusTypeDef MG90S_180::Rotate(uint8_t angle, uint8_t step, uint8_t step_delay)
{
    if (angle > 180)
        angle = 180;

    if (this->_current_angle == angle)
    {
        return HAL_StatusTypeDef::HAL_OK;
    }

    HAL_StatusTypeDef status = HAL_TIM_PWM_Start(this->_config.TIM_Handle, this->_config.TIM_HandleChannel);

    if (status != HAL_StatusTypeDef::HAL_OK)
    {
        return HAL_TIM_PWM_Stop(this->_config.TIM_Handle, this->_config.TIM_HandleChannel);
    }

    int temp_angle;

    if (this->_current_angle < angle)
    {
        for (temp_angle = this->_current_angle; temp_angle <= angle; temp_angle += step)
        {
            this->_write_angle(temp_angle);

            HAL_Delay(step_delay);
        }
    }
    else
    {
        for (temp_angle = this->_current_angle; temp_angle >= angle; temp_angle -= step)
        {
            this->_write_angle(temp_angle);

            HAL_Delay(step_delay);
        }
    }

    if (temp_angle != angle)
    {
        this->_write_angle(angle);

        HAL_Delay(step_delay);
    }

    this->_current_angle = angle;

    return HAL_TIM_PWM_Stop(this->_config.TIM_Handle, this->_config.TIM_HandleChannel);
}

void MG90S_180::_write_angle(uint8_t angle)
{
    uint16_t pulse_length = angle > 0 && angle <= 180 ? MG90S_PWM_MIN_LENGTH + MG90S_PWM_LENGTH_COEFFICIENT * angle : MG90S_PWM_MIN_LENGTH;

    switch (this->_config.TIM_RegisterName)
    {
    case RegisterName::CCR1:
    default:
        this->_config.TIM->CCR1 = pulse_length;
        break;

    case RegisterName::CCR2:
        this->_config.TIM->CCR2 = pulse_length;
        break;

    case RegisterName::CCR3:
        this->_config.TIM->CCR3 = pulse_length;
        break;

    case RegisterName::CCR4:
        this->_config.TIM->CCR4 = pulse_length;
        break;
    }
}
