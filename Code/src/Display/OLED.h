#ifndef OLED_H
#define OLED_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class OLED
{
private:
    Adafruit_SSD1306 display;

public:
    void Print(String lines[], int linesCount);    
    void Clear();
    void Init();
};

#endif