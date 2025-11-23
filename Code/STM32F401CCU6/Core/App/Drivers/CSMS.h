#ifndef CSMS_H
#define CSMS_H

#include "stm32f4xx_hal_adc.h"
#include "../Models/Templates.h"

class CSMS
{
public:
    CSMS() = delete;
    CSMS(ADC_HandleTypeDef *adc);

    HAL_Response<uint32_t> Value();

private:
    ADC_HandleTypeDef *_adc{nullptr};
};

#endif // CSMS_H