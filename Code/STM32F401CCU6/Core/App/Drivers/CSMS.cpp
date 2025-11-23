#include "CSMS.h"

CSMS::CSMS(ADC_HandleTypeDef *adc)
{
    this->_adc = adc;
}

HAL_Response<uint32_t> CSMS::Value()
{
    HAL_Response<uint32_t> response;

    if (this->_adc == nullptr)
    {
        response.Status = HAL_StatusTypeDef::HAL_ERROR;
        return response;
    }

    response.Status = HAL_ADC_Start(this->_adc);

    if (response.Status != HAL_StatusTypeDef::HAL_OK)
    {
        return response;
    }

    response.Status = HAL_ADC_PollForConversion(this->_adc, 1);

    if (response.Status != HAL_StatusTypeDef::HAL_OK)
    {
        return response;
    }

    response.Value = HAL_ADC_GetValue(this->_adc);

    response.Status = HAL_ADC_Stop(this->_adc);

    return response;
}
