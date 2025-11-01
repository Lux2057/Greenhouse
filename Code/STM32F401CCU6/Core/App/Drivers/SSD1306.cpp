#include "SSD1306.h"

SSD1306::SSD1306(Config config) : I2C_DeviceBase(config)
{
    HAL_StatusTypeDef test = HAL_I2C_IsDeviceReady(config.I2C, config.DevAddress, 3, 10);

    this->_init();
}

bool SSD1306::ReInit()
{
    return this->_init();
}

void SSD1306::Fill(Color color)
{
    memset(this->_buffer, (color == Color::Black) ? 0x00 : 0xFF, sizeof(this->_buffer));
}

void SSD1306::UpdateScreen()
{
    for (uint8_t i = 0; i < SSD1306_HEIGHT / 8; i++)
    {
        this->_write_command(0xB0 + i);
        this->_write_command(0x00 + SSD1306_X_OFFSET_LOWER);
        this->_write_command(0x10 + SSD1306_X_OFFSET_UPPER);
        this->_write_data(&this->_buffer[SSD1306_WIDTH * i], SSD1306_WIDTH);
    }
}

void SSD1306::DrawPixel(uint8_t x, uint8_t y, Color color)
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
    {
        return;
    }

    if (color == Color::White)
    {
        this->_buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    }
    else
    {
        this->_buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}

char SSD1306::WriteChar(char ch, Font Font, Color color)
{
    uint32_t i, b, j;

    if (ch < 32 || ch > 126)
        return 0;

    const uint8_t char_width = Font.char_width ? Font.char_width[ch - 32] : Font.width;

    if (SSD1306_WIDTH < (this->_state.CurrentX + char_width) ||
        SSD1306_HEIGHT < (this->_state.CurrentY + Font.height))
    {
        return 0;
    }

    for (i = 0; i < Font.height; i++)
    {
        b = Font.data[(ch - 32) * Font.height + i];
        for (j = 0; j < char_width; j++)
        {
            if ((b << j) & 0x8000)
            {
                this->DrawPixel(this->_state.CurrentX + j, (this->_state.CurrentY + i), (Color)color);
            }
            else
            {
                this->DrawPixel(this->_state.CurrentX + j, (this->_state.CurrentY + i), color == Color::White ? Color::Black : Color::White);
            }
        }
    }

    this->_state.CurrentX += char_width;

    return ch;
}

char SSD1306::WriteString(char *str, Font Font, Color color)
{
    while (*str)
    {
        if (this->WriteChar(*str, Font, color) != *str)
        {
            return *str;
        }
        str++;
    }

    return *str;
}

void SSD1306::SetCursor(uint8_t x, uint8_t y)
{
    this->_state.CurrentX = x;
    this->_state.CurrentY = y;
}

void SSD1306::Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Color color)
{
    int32_t deltaX = abs(x2 - x1);
    int32_t deltaY = abs(y2 - y1);
    int32_t signX = ((x1 < x2) ? 1 : -1);
    int32_t signY = ((y1 < y2) ? 1 : -1);
    int32_t error = deltaX - deltaY;
    int32_t error2;

    this->DrawPixel(x2, y2, color);

    while ((x1 != x2) || (y1 != y2))
    {
        this->DrawPixel(x1, y1, color);
        error2 = error * 2;
        if (error2 > -deltaY)
        {
            error -= deltaY;
            x1 += signX;
        }

        if (error2 < deltaX)
        {
            error += deltaX;
            y1 += signY;
        }
    }
    return;
}

void SSD1306::DrawCircle(uint8_t par_x, uint8_t par_y, uint8_t par_r, Color color)
{
    int32_t x = -par_r;
    int32_t y = 0;
    int32_t err = 2 - 2 * par_r;
    int32_t e2;

    if (par_x >= SSD1306_WIDTH || par_y >= SSD1306_HEIGHT)
    {
        return;
    }

    do
    {
        this->DrawPixel(par_x - x, par_y + y, color);
        this->DrawPixel(par_x + x, par_y + y, color);
        this->DrawPixel(par_x + x, par_y - y, color);
        this->DrawPixel(par_x - x, par_y - y, color);
        e2 = err;

        if (e2 <= y)
        {
            y++;
            err = err + (y * 2 + 1);
            if (-x == y && e2 <= x)
            {
                e2 = 0;
            }
        }

        if (e2 > x)
        {
            x++;
            err = err + (x * 2 + 1);
        }
    } while (x <= 0);

    return;
}

void SSD1306::FillCircle(uint8_t par_x, uint8_t par_y, uint8_t par_r, Color par_color)
{
    int32_t x = -par_r;
    int32_t y = 0;
    int32_t err = 2 - 2 * par_r;
    int32_t e2;

    if (par_x >= SSD1306_WIDTH || par_y >= SSD1306_HEIGHT)
    {
        return;
    }

    do
    {
        for (uint8_t _y = (par_y + y); _y >= (par_y - y); _y--)
        {
            for (uint8_t _x = (par_x - x); _x >= (par_x + x); _x--)
            {
                this->DrawPixel(_x, _y, par_color);
            }
        }

        e2 = err;
        if (e2 <= y)
        {
            y++;
            err = err + (y * 2 + 1);
            if (-x == y && e2 <= x)
            {
                e2 = 0;
            }
        }

        if (e2 > x)
        {
            x++;
            err = err + (x * 2 + 1);
        }
    } while (x <= 0);

    return;
}

void SSD1306::DrawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Color color)
{
    this->Line(x1, y1, x2, y1, color);
    this->Line(x2, y1, x2, y2, color);
    this->Line(x2, y2, x1, y2, color);
    this->Line(x1, y2, x1, y1, color);

    return;
}

void SSD1306::FillRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Color color)
{
    uint8_t x_start = ((x1 <= x2) ? x1 : x2);
    uint8_t x_end = ((x1 <= x2) ? x2 : x1);
    uint8_t y_start = ((y1 <= y2) ? y1 : y2);
    uint8_t y_end = ((y1 <= y2) ? y2 : y1);

    for (uint8_t y = y_start; (y <= y_end) && (y < SSD1306_HEIGHT); y++)
    {
        for (uint8_t x = x_start; (x <= x_end) && (x < SSD1306_WIDTH); x++)
        {
            this->DrawPixel(x, y, color);
        }
    }
    return;
}

void SSD1306::SetContrast(const uint8_t value)
{
    const uint8_t kSetContrastControlRegister = 0x81;
    this->_write_command(kSetContrastControlRegister);
    this->_write_command(value);
}

void SSD1306::SetDisplayOn(const bool on)
{
    uint8_t value;
    if (on)
    {
        value = 0xAF;
        this->_state.DisplayOn = true;
    }
    else
    {
        value = 0xAE;
        this->_state.DisplayOn = false;
    }
    this->_write_command(value);
}

bool SSD1306::GetDisplayOn()
{
    return this->_state.DisplayOn;
}

bool SSD1306::_init()
{
    this->SetDisplayOn(false);
    this->_write_command(0x20);
    this->_write_command(0x00);
    this->_write_command(0xB0);
    this->_write_command(0xC8);
    this->_write_command(0x00);
    this->_write_command(0x10);
    this->_write_command(0x40);
    this->_write_command(0xFF);
    this->_write_command(0xA1);
    this->_write_command(0xA6);

#if (SSD1306_HEIGHT == 128)
    this->_write_command(0xFF);
#else
    this->_write_command(0xA8);
#endif

#if (SSD1306_HEIGHT == 32)
    this->_write_command(0x1F);
#elif (SSD1306_HEIGHT == 64)
    this->_write_command(0x3F);
#elif (SSD1306_HEIGHT == 128)
    this->_write_command(0x3F);
#endif

    this->_write_command(0xA4);
    this->_write_command(0xD3);
    this->_write_command(0x00);
    this->_write_command(0xD5);
    this->_write_command(0xF0);
    this->_write_command(0xD9);
    this->_write_command(0x22);
    this->_write_command(0xDA);

#if (SSD1306_HEIGHT == 32)
    this->_write_command(0x02);
#elif (SSD1306_HEIGHT == 64)
    this->_write_command(0x12);
#elif (SSD1306_HEIGHT == 128)
    this->_write_command(0x12);
#else
#error "Only 32, 64, or 128 lines of height are supported!"
#endif

    this->_write_command(0xDB);
    this->_write_command(0x20);
    this->_write_command(0x8D);
    this->_write_command(0x14);

    this->SetDisplayOn(true);

    this->Fill(Color::White);

    this->UpdateScreen();

    HAL_Delay(200);

    this->Fill(Color::Black);

    this->UpdateScreen();

    this->_state.CurrentX = 0;
    this->_state.CurrentY = 0;
    this->_state.Initialized = true;

    return true;
}

void SSD1306::_write_command(uint8_t byte)
{
    this->_update_device_status(HAL_I2C_Mem_Write(this->_config.I2C, this->_config.DevAddress, 0x00, 1, &byte, 1, HAL_MAX_DELAY));
}

void SSD1306::_write_data(uint8_t *buffer, size_t buff_size)
{
    this->_update_device_status(HAL_I2C_Mem_Write(this->_config.I2C, this->_config.DevAddress, 0x40, 1, buffer, buff_size, HAL_MAX_DELAY));
}

void SSD1306::_fill_buffer(uint8_t *buf, uint32_t len)
{
    if (len > SSD1306_BUFFER_SIZE)
        return;

    memcpy(this->_buffer, buf, len);
}
