#include "APDS9930.h"

APDS9930::APDS9930(Config config) : I2C_DeviceBase(config)
{
    this->_init();
}

bool APDS9930::_init()
{
    if (!this->_is_ready_to_use())
        return false;

    this->_set_mode(APDS9930_MODE_ALL, APDS9930_OFF);
    if (this->_status != DeviceStatus::Ok)
        return false;

    this->_I2C_write_byte(APDS9930_REG_ATIME, APDS9930_DEFAULT_ATIME);
    if (this->_status != DeviceStatus::Ok)
        return false;

    this->_I2C_write_byte(APDS9930_REG_WTIME, APDS9930_DEFAULT_WTIME);
    if (this->_status != DeviceStatus::Ok)
        return false;

    this->_I2C_write_byte(APDS9930_REG_PPULSE, APDS9930_DEFAULT_PPULSE);
    if (this->_status != DeviceStatus::Ok)
        return false;

    this->_I2C_write_byte(APDS9930_REG_POFFSET, APDS9930_DEFAULT_POFFSET);
    if (this->_status != DeviceStatus::Ok)
        return false;

    this->_I2C_write_byte(APDS9930_REG_CONFIG, APDS9930_DEFAULT_CONFIG);
    if (this->_status != DeviceStatus::Ok)
        return false;

    this->_set_LED_drive(APDS9930_DEFAULT_PDRIVE);
    if (this->_status != DeviceStatus::Ok)
        return false;

    this->_set_proximity_gain(APDS9930_DEFAULT_PGAIN);
    if (this->_status != DeviceStatus::Ok)
        return false;

    this->_set_ambient_light_gain(APDS9930_DEFAULT_AGAIN);
    if (this->_status != DeviceStatus::Ok)
        return false;

    this->_set_proximity_diode(APDS9930_DEFAULT_PDIODE);
    if (this->_status != DeviceStatus::Ok)
        return false;

    this->_set_proximity_int_low_threshold(APDS9930_DEFAULT_PILT);
    if (this->_status != DeviceStatus::Ok)
        return false;

    this->_set_proximity_int_high_threshold(APDS9930_DEFAULT_PIHT);
    if (this->_status != DeviceStatus::Ok)
        return false;

    this->_set_light_int_low_threshold(APDS9930_DEFAULT_AILT);
    if (this->_status != DeviceStatus::Ok)
        return false;

    this->_set_light_int_high_threshold(APDS9930_DEFAULT_AIHT);
    if (this->_status != DeviceStatus::Ok)
        return false;

    this->_I2C_write_byte(APDS9930_REG_PERS, APDS9930_DEFAULT_PERS);
    if (this->_status != DeviceStatus::Ok)
        return false;

    return true;
}

HAL_Response<uint8_t> APDS9930::ID()
{
    HAL_Response<uint8_t> response;

    if (!this->_is_ready_to_use())
    {
        response.Status = HAL_StatusTypeDef::HAL_ERROR;
        return response;
    }

    response.Status = this->_I2C_read_byte(APDS9930_REG_ID, &response.Value);

    return response;
}

HAL_Response<uint8_t> APDS9930::Mode()
{
    HAL_Response<uint8_t> response;

    if (!this->_is_ready_to_use())
    {
        response.Status = HAL_StatusTypeDef::HAL_ERROR;
        return response;
    }

    response.Status = this->_I2C_read_byte(APDS9930_REG_ENABLE, &response.Value);

    if (response.Value == 0xFF)
    {
        response.Status = HAL_StatusTypeDef::HAL_ERROR;
        this->_update_device_status(response.Status);
    }

    return response;
}

HAL_StatusTypeDef APDS9930::SetPower(bool enable)
{
    if (!this->_is_ready_to_use())
    {
        return HAL_StatusTypeDef::HAL_ERROR;
    }

    return this->_set_mode(APDS9930_MODE_POWER, enable ? APDS9930_ON : APDS9930_OFF);
}

HAL_Response<uint16_t> APDS9930::Proximity()
{
    HAL_Response<uint16_t> response;

    if (!this->_is_ready_to_use())
    {
        response.Status = HAL_StatusTypeDef::HAL_ERROR;
        return response;
    }

    response.Value = 0;

    uint8_t byte;

    response.Status = this->_I2C_read_byte(APDS9930_REG_PDATAL, &byte);
    if (this->_status != DeviceStatus::Ok)
        return response;

    response.Value = byte;

    response.Status = this->_I2C_read_byte(APDS9930_REG_PDATAH, &byte);
    if (this->_status != DeviceStatus::Ok)
        return response;

    response.Value += ((uint16_t)byte << 8);

    return response;
}

HAL_Response<float> APDS9930::AmbientLight()
{

    HAL_Response<float> response;
    if (!this->_is_ready_to_use())
    {
        response.Status = HAL_StatusTypeDef::HAL_ERROR;
        return response;
    }

    response.Value = 0.0;

    HAL_Response<uint16_t> ch0;
    HAL_Response<uint16_t> ch1;

    ch0 = this->_ch0_light();
    if (this->_status != DeviceStatus::Ok)
    {
        response.Status = ch0.Status;
        return response;
    }

    ch1 = this->_ch1_light();
    if (this->_status != DeviceStatus::Ok)
    {
        response.Status = ch1.Status;
        return response;
    }

    return this->_float_ambient_to_lux(ch0.Value, ch1.Value);
}

bool APDS9930::ReInit()
{
    return this->_init();
}

HAL_StatusTypeDef APDS9930::EnableLightSensor(bool enable, bool interrupts)
{
    if (!this->_is_ready_to_use())
    {
        return HAL_StatusTypeDef::HAL_ERROR;
    }

    HAL_StatusTypeDef status;
    if (enable)
    {
        status = this->_set_ambient_light_gain(APDS9930_DEFAULT_AGAIN);
        if (this->_status != DeviceStatus::Ok)
        {
            return status;
        }

        status = this->_set_ambient_light_int_enabled(interrupts ? APDS9930_ON : APDS9930_OFF);
        if (this->_status != DeviceStatus::Ok)
        {
            return status;
        }

        return this->_set_mode(APDS9930_MODE_AMBIENT_LIGHT, APDS9930_ON);
    }
    else
    {
        status = this->_set_ambient_light_int_enabled(APDS9930_OFF);
        if (this->_status != DeviceStatus::Ok)
        {
            return status;
        }

        return this->_set_mode(APDS9930_MODE_AMBIENT_LIGHT, APDS9930_OFF);
    }
}

HAL_StatusTypeDef APDS9930::EnableProximitySensor(bool enable, bool interrupts)
{
    if (!this->_is_ready_to_use())
    {
        return HAL_StatusTypeDef::HAL_ERROR;
    }

    HAL_StatusTypeDef status;
    if (enable)
    {
        status = this->_set_proximity_gain(APDS9930_DEFAULT_PGAIN);
        if (this->_status != DeviceStatus::Ok)
        {
            return status;
        }

        status = this->_set_LED_drive(APDS9930_DEFAULT_PDRIVE);
        if (this->_status != DeviceStatus::Ok)
        {
            return status;
        }

        status = this->_set_proximity_int_enabled(interrupts ? APDS9930_ON : APDS9930_OFF);
        if (this->_status != DeviceStatus::Ok)
        {
            return status;
        }

        return this->_set_mode(APDS9930_MODE_PROXIMITY, APDS9930_ON);
    }
    else
    {
        status = this->_set_proximity_int_enabled(APDS9930_OFF);
        if (this->_status != DeviceStatus::Ok)
        {
            return status;
        }

        return this->_set_mode(APDS9930_MODE_PROXIMITY, APDS9930_OFF);
    }
}

HAL_StatusTypeDef APDS9930::_I2C_set_cmd(uint8_t cmd)
{
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(this->_config.I2C, this->_config.DevAddress << 1, &cmd, 1, this->_config.Timeout);

    this->_update_device_status(status);

    return status;
}

HAL_StatusTypeDef APDS9930::_I2C_set_reg(uint8_t const &reg)
{
    uint8_t cmd = reg | APDS9930_CMD_AUTO_INCREMENT_TYPE << 5 | 1 << 7;

    return this->_I2C_set_cmd(cmd);
}

HAL_StatusTypeDef APDS9930::_I2C_read_byte(uint8_t const &reg, uint8_t *val)
{
    HAL_StatusTypeDef status = this->_I2C_set_reg(reg);

    if (status != HAL_StatusTypeDef::HAL_OK)
        return status;

    status = HAL_I2C_Master_Receive(this->_config.I2C, this->_config.DevAddress << 1, val, 1, this->_config.Timeout);

    this->_update_device_status(status);

    return status;
}

HAL_StatusTypeDef APDS9930::_I2C_write_byte(uint8_t const &reg, uint8_t val)
{
    HAL_StatusTypeDef status = this->_I2C_set_reg(reg);

    if (status != HAL_StatusTypeDef::HAL_OK)
        return status;

    status = HAL_I2C_Master_Transmit(this->_config.I2C, this->_config.DevAddress << 1, &val, 1, this->_config.Timeout);

    this->_update_device_status(status);

    return status;
}

HAL_StatusTypeDef APDS9930::_set_mode(uint8_t mode, uint8_t enable)
{
    HAL_Response<uint8_t> current_mode = this->Mode();

    if (this->_status == DeviceStatus::Error || this->_status == DeviceStatus::Unavailable)
        return HAL_StatusTypeDef::HAL_ERROR;

    enable = enable & 0x01;
    if (mode <= 6)
    {
        if (enable)
        {
            current_mode.Value |= (1 << mode);
        }
        else
        {
            current_mode.Value &= ~(1 << mode);
        }
    }
    else if (mode == APDS9930_MODE_ALL)
    {
        if (enable)
        {
            current_mode.Value = 0x7F;
        }
        else
        {
            current_mode.Value = 0x00;
        }
    }

    return this->_I2C_write_byte(APDS9930_REG_ENABLE, current_mode.Value);
}

HAL_Response<uint8_t> APDS9930::_LED_drive()
{
    HAL_Response<uint8_t> response;
    response.Status = this->_I2C_read_byte(APDS9930_REG_CONTROL, &response.Value);

    if (this->_status != DeviceStatus::Ok)
        return response;

    response.Value = (response.Value >> 6) & 0b00000011;

    return response;
}

HAL_StatusTypeDef APDS9930::_set_LED_drive(uint8_t drive)
{
    HAL_Response<uint8_t> current_drive = this->_LED_drive();

    if (this->_status != DeviceStatus::Ok)
        return current_drive.Status;

    drive &= 0b00000011;
    drive = drive << 6;
    current_drive.Value &= 0b00111111;
    current_drive.Value |= drive;

    return this->_I2C_write_byte(APDS9930_REG_CONTROL, current_drive.Value);
}

HAL_Response<uint8_t> APDS9930::_proximity_gain()
{
    HAL_Response<uint8_t> response;
    response.Status = this->_I2C_read_byte(APDS9930_REG_CONTROL, &response.Value);

    if (this->_status != DeviceStatus::Ok)
        return response;

    response.Value = (response.Value >> 2) & 0b00000011;

    return response;
}

HAL_StatusTypeDef APDS9930::_set_proximity_gain(uint8_t gain)
{
    HAL_Response<uint8_t> current_gain = this->_proximity_gain();

    if (this->_status != DeviceStatus::Ok)
        return current_gain.Status;

    gain &= 0b00000011;
    gain = gain << 2;
    current_gain.Value &= 0b11110011;
    current_gain.Value |= gain;

    return this->_I2C_write_byte(APDS9930_REG_CONTROL, current_gain.Value);
}

HAL_Response<uint8_t> APDS9930::_ambient_light_gain()
{
    HAL_Response<uint8_t> response;
    response.Status = this->_I2C_read_byte(APDS9930_REG_CONTROL, &response.Value);

    if (this->_status != DeviceStatus::Ok)
        return response;

    response.Value &= 0b00000011;

    return response;
}

HAL_StatusTypeDef APDS9930::_set_ambient_light_gain(uint8_t gain)
{
    HAL_Response<uint8_t> current_gain = this->_ambient_light_gain();

    if (this->_status != DeviceStatus::Ok)
        return current_gain.Status;

    gain &= 0b00000011;
    current_gain.Value &= 0b11111100;
    current_gain.Value |= gain;

    return this->_I2C_write_byte(APDS9930_REG_CONTROL, current_gain.Value);
}

HAL_Response<uint8_t> APDS9930::_proximity_diode()
{
    HAL_Response<uint8_t> response;
    response.Status = this->_I2C_read_byte(APDS9930_REG_CONTROL, &response.Value);

    if (this->_status != DeviceStatus::Ok)
        return response;

    response.Value = (response.Value >> 4) & 0b00000011;

    return response;
}

HAL_StatusTypeDef APDS9930::_set_proximity_diode(uint8_t value)
{
    HAL_Response<uint8_t> current_value = this->_proximity_diode();

    if (this->_status != DeviceStatus::Ok)
        return current_value.Status;

    value &= 0b00000011;
    value = value << 4;
    current_value.Value &= 0b11001111;
    current_value.Value |= value;

    return this->_I2C_write_byte(APDS9930_REG_CONTROL, current_value.Value);
}

HAL_Response<uint16_t> APDS9930::_proximity_int_low_threshold()
{
    uint8_t byte;
    HAL_Response<uint16_t> response;

    response.Status = this->_I2C_read_byte(APDS9930_REG_PILTL, &byte);
    if (this->_status != DeviceStatus::Ok)
    {
        return response;
    }

    response.Value = byte;

    response.Status = this->_I2C_read_byte(APDS9930_REG_PILTH, &byte);
    if (this->_status != DeviceStatus::Ok)
    {
        return response;
    }

    response.Value |= ((uint16_t)byte << 8);

    return response;
}

HAL_StatusTypeDef APDS9930::_set_proximity_int_low_threshold(uint16_t threshold)
{
    uint8_t low;
    uint8_t high;
    high = threshold >> 8;
    low = threshold & 0x00FF;

    HAL_StatusTypeDef status = this->_I2C_write_byte(APDS9930_REG_PILTL, low);
    if (this->_status != DeviceStatus::Ok)
    {
        return status;
    }

    return this->_I2C_write_byte(APDS9930_REG_PILTH, high);
}

HAL_Response<uint16_t> APDS9930::_proximity_int_high_threshold()
{
    uint8_t byte;
    HAL_Response<uint16_t> response;

    response.Status = this->_I2C_read_byte(APDS9930_REG_PIHTL, &byte);
    if (this->_status != DeviceStatus::Ok)
    {
        return response;
    }

    response.Value = byte;

    response.Status = this->_I2C_read_byte(APDS9930_REG_PIHTH, &byte);
    if (this->_status != DeviceStatus::Ok)
    {
        return response;
    }

    response.Value |= ((uint16_t)byte << 8);

    return response;
}

HAL_StatusTypeDef APDS9930::_set_proximity_int_high_threshold(uint16_t threshold)
{
    uint8_t low;
    uint8_t high;
    high = threshold >> 8;
    low = threshold & 0x00FF;

    HAL_StatusTypeDef status = this->_I2C_write_byte(APDS9930_REG_PIHTL, low);
    if (this->_status != DeviceStatus::Ok)
    {
        return status;
    }

    return this->_I2C_write_byte(APDS9930_REG_PIHTH, high);
}

HAL_Response<uint16_t> APDS9930::_light_int_low_threshold()
{
    uint8_t byte;
    HAL_Response<uint16_t> response;

    response.Status = this->_I2C_read_byte(APDS9930_REG_AILTL, &byte);
    if (this->_status != DeviceStatus::Ok)
    {
        return response;
    }

    response.Value = byte;

    response.Status = this->_I2C_read_byte(APDS9930_REG_AILTH, &byte);
    if (this->_status != DeviceStatus::Ok)
    {
        return response;
    }

    response.Value |= ((uint16_t)byte << 8);

    return response;
}

HAL_StatusTypeDef APDS9930::_set_light_int_low_threshold(uint16_t threshold)
{
    uint8_t low;
    uint8_t high;
    high = threshold >> 8;
    low = threshold & 0x00FF;

    HAL_StatusTypeDef status = this->_I2C_write_byte(APDS9930_REG_AILTL, low);
    if (this->_status != DeviceStatus::Ok)
    {
        return status;
    }

    return this->_I2C_write_byte(APDS9930_REG_AILTH, high);
}

HAL_Response<uint16_t> APDS9930::_light_int_high_threshold()
{
    uint8_t byte;
    HAL_Response<uint16_t> response;

    response.Status = this->_I2C_read_byte(APDS9930_REG_AIHTL, &byte);
    if (this->_status != DeviceStatus::Ok)
    {
        return response;
    }

    response.Value = byte;

    response.Status = this->_I2C_read_byte(APDS9930_REG_AIHTH, &byte);
    if (this->_status != DeviceStatus::Ok)
    {
        return response;
    }

    response.Value |= ((uint16_t)byte << 8);

    return response;
}

HAL_StatusTypeDef APDS9930::_set_light_int_high_threshold(uint16_t threshold)
{
    uint8_t low;
    uint8_t high;
    high = threshold >> 8;
    low = threshold & 0x00FF;

    HAL_StatusTypeDef status = this->_I2C_write_byte(APDS9930_REG_AIHTL, low);
    if (this->_status != DeviceStatus::Ok)
    {
        return status;
    }

    return this->_I2C_write_byte(APDS9930_REG_AIHTH, high);
}

HAL_Response<uint8_t> APDS9930::_ambient_light_int_enabled()
{
    HAL_Response<uint8_t> response;
    response.Status = this->_I2C_read_byte(APDS9930_REG_ENABLE, &response.Value);

    if (this->_status != DeviceStatus::Ok)
        return response;

    response.Value = (response.Value >> 4) & 0b00000001;

    return response;
}

HAL_StatusTypeDef APDS9930::_set_ambient_light_int_enabled(uint8_t value)
{

    HAL_Response<uint8_t> current_enable;
    current_enable.Status = this->_I2C_read_byte(APDS9930_REG_ENABLE, &current_enable.Value);

    if (this->_status != DeviceStatus::Ok)
        return current_enable.Status;

    value &= 0b00000001;
    value = value << 4;
    current_enable.Value &= 0b11101111;
    current_enable.Value |= value;

    return this->_I2C_write_byte(APDS9930_REG_ENABLE, current_enable.Value);
}

HAL_Response<uint8_t> APDS9930::_proximity_int_enabled()
{
    HAL_Response<uint8_t> response;
    response.Status = this->_I2C_read_byte(APDS9930_REG_ENABLE, &response.Value);

    if (this->_status != DeviceStatus::Ok)
        return response;

    response.Value = (response.Value >> 5) & 0b00000001;

    return response;
}

HAL_StatusTypeDef APDS9930::_set_proximity_int_enabled(uint8_t value)
{

    HAL_Response<uint8_t> current_enable;
    current_enable.Status = this->_I2C_read_byte(APDS9930_REG_ENABLE, &current_enable.Value);

    if (this->_status != DeviceStatus::Ok)
        return current_enable.Status;

    value &= 0b00000001;
    value = value << 5;
    current_enable.Value &= 0b11011111;
    current_enable.Value |= value;

    return this->_I2C_write_byte(APDS9930_REG_ENABLE, current_enable.Value);
}

HAL_StatusTypeDef APDS9930::_clear_ambient_light_int()
{
    return this->_I2C_set_cmd(APDS9930_CLEAR_ALS_INT);
}

HAL_StatusTypeDef APDS9930::_clear_proximity_int()
{
    return this->_I2C_set_cmd(APDS9930_CLEAR_PROX_INT);
}

HAL_StatusTypeDef APDS9930::_clear_all_int()
{
    return this->_I2C_set_cmd(APDS9930_CLEAR_ALL_INTS);
}

HAL_Response<float> APDS9930::_float_ambient_to_lux(uint16_t Ch0, uint16_t Ch1)
{
    uint8_t x[4] = {1, 8, 16, 120};
    float ALSIT = 2.73 * (256 - APDS9930_DEFAULT_ATIME);
    float t1 = Ch0 - APDS9930_ALS_B * Ch1;
    float t2 = APDS9930_ALS_C * Ch0 - APDS9930_ALS_D * Ch1;
    float iac = t1 > t2 ? t1 : t2;
    if (iac < 0)
        iac = 0;

    HAL_Response<float> response;

    HAL_Response<uint8_t> ambient_gain = this->_ambient_light_gain();
    response.Status = ambient_gain.Status;
    if (this->_status != DeviceStatus::Ok)
    {
        return response;
    }

    float lpc = APDS9930_GA * APDS9930_DF / (ALSIT * x[ambient_gain.Value]);

    response.Value = iac * lpc;

    return response;
}

HAL_Response<uint16_t> APDS9930::_ch0_light()
{
    HAL_Response<uint16_t> response;
    response.Value = 0;
    uint8_t byte;

    response.Status = this->_I2C_read_byte(APDS9930_REG_Ch0DATAL, &byte);
    if (this->_status != DeviceStatus::Ok)
        return response;

    response.Value = byte;

    response.Status = this->_I2C_read_byte(APDS9930_REG_Ch0DATAH, &byte);
    if (this->_status != DeviceStatus::Ok)
        return response;

    response.Value += ((uint16_t)byte << 8);

    return response;
}

HAL_Response<uint16_t> APDS9930::_ch1_light()
{
    HAL_Response<uint16_t> response;
    response.Value = 0;
    uint8_t byte;

    response.Status = this->_I2C_read_byte(APDS9930_REG_Ch1DATAL, &byte);
    if (this->_status != DeviceStatus::Ok)
        return response;

    response.Value = byte;

    response.Status = this->_I2C_read_byte(APDS9930_REG_Ch1DATAH, &byte);
    if (this->_status != DeviceStatus::Ok)
        return response;

    response.Value += ((uint16_t)byte << 8);

    return response;
}
