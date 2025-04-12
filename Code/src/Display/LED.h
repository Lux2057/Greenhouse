#ifndef LED_H
#define LES_H

#include <Arduino.h>

class LED
{
public:
    LED(int pin);

    void Init();

    void blink(int litDuration, int litInterval, int loopCount);

private:
    int _pin;
};

#endif