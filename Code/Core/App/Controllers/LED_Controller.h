#ifndef LED_BLINK_H
#define LED_BLINK_H

#include "stm32f4xx_hal.h"

using namespace std;

#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC

class LED_Controller
{
public:
    struct Config
    {
    public:
        uint16_t Pin{GPIO_PIN_13};
        GPIO_TypeDef *Port{GPIOC};
    };

    struct BlinkConfig
    {
    public:
        BlinkConfig() = delete;
        BlinkConfig(uint32_t turnOnMs, uint32_t turnOffMs, uint16_t count)
        {
            this->_turnOnMs = turnOnMs >= 10 ? turnOnMs : 10;
            this->_turnOffMs = turnOffMs >= 10 ? turnOffMs : 10;
            this->_count = count >= 1 ? count : 1;
        }

        uint32_t TurnOnMs()
        {
            return this->_turnOnMs;
        }
        uint32_t TurnOffMs()
        {
            return this->_turnOffMs;
        }
        uint16_t Count()
        {
            return this->_count;
        }

    private:
        uint32_t _turnOnMs{10};
        uint32_t _turnOffMs{10};
        uint16_t _count{1};
    };

    LED_Controller() = delete;
    LED_Controller(Config config);

    void Blink(BlinkConfig config);

    const BlinkConfig OK{BlinkConfig(350, 10, 1)};
    const BlinkConfig ERROR{BlinkConfig(100, 50, 3)};

private:
    Config _config;
};

#endif // LED_BLINK_H