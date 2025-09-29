#include "LED_Controller.h"

LED_Controller::LED_Controller(Config config)
{
    this->_config = config;
}

void LED_Controller::Blink(LED_Controller::BlinkConfig config)
{
    for (int i = 0; i < config.Count(); i++)
    {
        HAL_GPIO_WritePin(this->_config.Port, this->_config.Pin, GPIO_PIN_RESET);
        HAL_Delay(config.TurnOnMs());
        HAL_GPIO_WritePin(this->_config.Port, this->_config.Pin, GPIO_PIN_SET);
        HAL_Delay(config.TurnOffMs());
    }
}