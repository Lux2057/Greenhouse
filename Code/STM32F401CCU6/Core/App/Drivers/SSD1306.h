#ifndef SSD1306_H
#define SSD1306_H

#include <stddef.h>
#include <stdint.h>
#include <_ansi.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "I2C_DeviceBase.h"
#include "SSD1306_Fonts.h"

class SSD1306 : public I2C_DeviceBase
{
#define SSD1306_DEFAULT_I2C_ADDRESS 0x78
#define SSD1306_X_OFFSET_LOWER 0
#define SSD1306_X_OFFSET_UPPER 0
#define SSD1306_HEIGHT 64
#define SSD1306_WIDTH 128
#define SSD1306_BUFFER_SIZE SSD1306_WIDTH *SSD1306_HEIGHT / 8

public:
    enum class Color
    {
        Black = 0x00,
        White = 0x01
    };

    struct State
    {
    public:
        uint16_t CurrentX;
        uint16_t CurrentY;
        bool Initialized;
        uint8_t DisplayOn;
    };

    struct VERTEX
    {
    public:
        uint8_t x;
        uint8_t y;
    };

    struct Font
    {
    public:
        const uint8_t width;
        const uint8_t height;
        const uint16_t *const data;
        const uint8_t *const char_width;
    };

    SSD1306() = delete;
    SSD1306(Config config);

    bool ReInit() override;

    void Fill(Color color);
    void UpdateScreen();
    void DrawPixel(uint8_t x, uint8_t y, Color color);
    char WriteChar(char ch, Font Font, Color color);
    char WriteString(char *str, Font Font, Color color);
    void SetCursor(uint8_t x, uint8_t y);
    void Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Color color);
    void DrawCircle(uint8_t x, uint8_t y, uint8_t r, Color color);
    void FillCircle(uint8_t x, uint8_t y, uint8_t r, Color color);
    void DrawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Color color);
    void FillRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Color color);

    void SetContrast(const uint8_t value);
    void SetDisplayOn(const bool on);
    bool GetDisplayOn();

    const Font Font_6x8 = {6, 8, Font6x8, NULL};
    const Font Font_7x10 = {7, 10, Font7x10, NULL};
    const Font Font_11x18 = {11, 18, Font11x18, NULL};
    const Font Font_16x26 = {16, 26, Font16x26, NULL};
    const Font Font_16x24 = {16, 24, Font16x24, NULL};
    const Font Font_16x15 = {16, 15, Font16x15, char_width};

private:
    bool _init() override;

    void _write_command(uint8_t byte);
    void _write_data(uint8_t *buffer, size_t buff_size);
    void _fill_buffer(uint8_t *buf, uint32_t len);

    uint8_t _buffer[SSD1306_BUFFER_SIZE];
    State _state;
};

#endif // SSD1306_H