#include "Display/OLED.h"

void OLED::Init()
{
    this->display = Adafruit_SSD1306(128, 64, &Wire, -1);
    this->display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void OLED::Clear()
{
    this->display.clearDisplay();    
}

void OLED::Print(String lines[], int linesCount)
{
    this->display.setTextSize(1);
    this->display.setTextColor(WHITE);
    this->display.setCursor(0, 5);

    for (int i = 0; i < linesCount; ++i)
    {
        this->display.println(lines[i]);
    }

    this->display.display();
}