#include <Display\LED.h>

LED::LED(int pin)
{
    this->_pin = pin;
}

void LED::Init()
{
    pinMode(this->_pin, OUTPUT);
}

void LED::blink(int litDuration, int litInterval, int loopCount)
{
    digitalWrite(LED_BUILTIN, HIGH);

    for (int i = 0; i < loopCount; i++)
    {
        digitalWrite(LED_BUILTIN, LOW);
        delay(litDuration);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(litInterval);
    }

    digitalWrite(LED_BUILTIN, HIGH);
}
