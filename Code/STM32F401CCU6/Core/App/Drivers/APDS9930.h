#ifndef APDS9930_H
#define APDS9930_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"
#include "I2C_DeviceBase.h"
#include "../Models/Enums.h"
#include "../Models/Templates.h"

using namespace std;

class APDS9930 : public I2C_DeviceBase
{

/* APDS-9930 register addresses */
#define APDS9930_REG_ENABLE 0x00   // Enable of states and interrupts
#define APDS9930_REG_ATIME 0x01    //  ALS ADC time
#define APDS9930_REG_PTIME 0x02    //  Proximity ADC time
#define APDS9930_REG_WTIME 0x03    //  Wait time
#define APDS9930_REG_AILTL 0x04    //  ALS interrupt low threshold low byte
#define APDS9930_REG_AILTH 0x05    //  ALS interrupt low threshold hi byte
#define APDS9930_REG_AIHTL 0x06    //  ALS interrupt hi threshold low byte
#define APDS9930_REG_AIHTH 0x07    //  ALS interrupt hi threshold hi byte
#define APDS9930_REG_PILTL 0x08    //  Proximity interrupt low threshold low byte
#define APDS9930_REG_PILTH 0x09    //  Proximity interrupt low threshold hi byte
#define APDS9930_REG_PIHTL 0x0A    //  Proximity interrupt hi threshold low byte
#define APDS9930_REG_PIHTH 0x0B    //  Proximity interrupt hi threshold hi byte
#define APDS9930_REG_PERS 0x0C     //  Interrupt persistence filters
#define APDS9930_REG_CONFIG 0x0D   // Configuration
#define APDS9930_REG_PPULSE 0x0E   //  Proximity pulse count
#define APDS9930_REG_CONTROL 0x0F  //  Gain control register
#define APDS9930_REG_ID 0x12       // Device ID
#define APDS9930_REG_STATUS 0x13   // Device status
#define APDS9930_REG_Ch0DATAL 0x14 //  Ch0 ADC low data register
#define APDS9930_REG_Ch0DATAH 0x15 // Ch0 ADC high data register
#define APDS9930_REG_Ch1DATAL 0x16 //  Ch1 ADC low data register
#define APDS9930_REG_Ch1DATAH 0x17 // Ch1 ADC high data register
#define APDS9930_REG_PDATAL 0x18   // Proximity ADC low data register
#define APDS9930_REG_PDATAH 0x19   //  Proximity ADC high data register
#define APDS9930_REG_POFFSET 0x1E  //  Proximity offset register

/* Command register modes */
#define APDS9930_CMD_REPEATED_BYTE_TYPE 0
#define APDS9930_CMD_AUTO_INCREMENT_TYPE 1
#define APDS9930_CMD_SPECIAL_FUNCTION_TYPE 3

/* On/Off definitions */
#define APDS9930_OFF 0
#define APDS9930_ON 1

/* Bit fields */
#define APDS9930_PON 0b00000001
#define APDS9930_AEN 0b00000010
#define APDS9930_PEN 0b00000100
#define APDS9930_WEN 0b00001000
#define APSD9930_AIEN 0b00010000
#define APDS9930_PIEN 0b00100000
#define APDS9930_SAI 0b01000000

/* Acceptable parameters for Mode */
#define APDS9930_MODE_POWER 0
#define APDS9930_MODE_AMBIENT_LIGHT 1
#define APDS9930_MODE_PROXIMITY 2
#define APDS9930_MODE_WAIT 3
#define APDS9930_MODE_AMBIENT_LIGHT_INT 4
#define APDS9930_MODE_PROXIMITY_INT 5
#define APDS9930_MODE_SLEEP_AFTER_INT 6
#define APDS9930_MODE_ALL 7

/* ALS coefficients */
#define APDS9930_DF 52
#define APDS9930_GA 0.49
#define APDS9930_ALS_B 1.862
#define APDS9930_ALS_C 0.746
#define APDS9930_ALS_D 1.291

/* LED Drive values */
#define APDS9930_LED_DRIVE_100MA 0
#define APDS9930_LED_DRIVE_50MA 1
#define APDS9930_LED_DRIVE_25MA 2
#define APDS9930_LED_DRIVE_12_5MA 3

/* Proximity Gain (PGAIN) values */
#define APDS9930_PGAIN_1X 0
#define APDS9930_PGAIN_2X 1
#define APDS9930_PGAIN_4X 2
#define APDS9930_PGAIN_8X 3

/* ALS Gain (AGAIN) values */
#define APDS9930_AGAIN_1X 0
#define APDS9930_AGAIN_8X 1
#define APDS9930_AGAIN_16X 2
#define APDS9930_AGAIN_120X 3

/* Interrupt clear values */
#define APDS9930_CLEAR_PROX_INT 0xE5
#define APDS9930_CLEAR_ALS_INT 0xE6
#define APDS9930_CLEAR_ALL_INTS 0xE7

/* Default values */
#define APDS9930_DEFAULT_I2C_DEV_ADDRESS 0x39
#define APDS9930_DEFAULT_ATIME 0xED
#define APDS9930_DEFAULT_WTIME 0xFF
#define APDS9930_DEFAULT_PTIME 0xFF
#define APDS9930_DEFAULT_PPULSE 0x08
#define APDS9930_DEFAULT_POFFSET 0 // 0 offset
#define APDS9930_DEFAULT_CONFIG 0
#define APDS9930_DEFAULT_PDRIVE APDS9930_LED_DRIVE_100MA
#define APDS9930_DEFAULT_PDIODE 2
#define APDS9930_DEFAULT_PGAIN APDS9930_PGAIN_8X
#define APDS9930_DEFAULT_AGAIN APDS9930_AGAIN_1X
#define APDS9930_DEFAULT_PILT 0      // Low proximity threshold
#define APDS9930_DEFAULT_PIHT 50     // High proximity threshold
#define APDS9930_DEFAULT_AILT 0xFFFF // Force interrupt for calibration
#define APDS9930_DEFAULT_AIHT 0
#define APDS9930_DEFAULT_PERS 0x22 // 2 consecutive prox or ALS for int.

public:
    APDS9930() = delete;
    APDS9930(Config config);

    HAL_Response<uint8_t> ID();
    HAL_Response<uint8_t> Mode();
    HAL_Response<uint16_t> Proximity();
    HAL_Response<float> AmbientLight();

    bool ReInit() override;

    HAL_StatusTypeDef SetPower(bool enable);

    HAL_StatusTypeDef EnableLightSensor(bool enable, bool interrupts = false);
    HAL_StatusTypeDef EnableProximitySensor(bool enable, bool interrupts = false);

private:
    bool _init() override;

    HAL_StatusTypeDef _I2C_set_cmd(uint8_t cmd);
    HAL_StatusTypeDef _I2C_set_reg(uint8_t const &reg);
    HAL_StatusTypeDef _I2C_read_byte(uint8_t const &reg, uint8_t *val);
    HAL_StatusTypeDef _I2C_write_byte(uint8_t const &reg, uint8_t val);

    HAL_StatusTypeDef _set_mode(uint8_t mode, uint8_t enable);

    HAL_Response<uint8_t> _LED_drive();
    HAL_StatusTypeDef _set_LED_drive(uint8_t drive);

    HAL_Response<uint8_t> _proximity_gain();
    HAL_StatusTypeDef _set_proximity_gain(uint8_t gain);

    HAL_Response<uint8_t> _ambient_light_gain();
    HAL_StatusTypeDef _set_ambient_light_gain(uint8_t gain);

    HAL_Response<uint8_t> _proximity_diode();
    HAL_StatusTypeDef _set_proximity_diode(uint8_t value);

    HAL_Response<uint16_t> _proximity_int_low_threshold();
    HAL_StatusTypeDef _set_proximity_int_low_threshold(uint16_t threshold);

    HAL_Response<uint16_t> _proximity_int_high_threshold();
    HAL_StatusTypeDef _set_proximity_int_high_threshold(uint16_t threshold);

    HAL_Response<uint16_t> _light_int_low_threshold();
    HAL_StatusTypeDef _set_light_int_low_threshold(uint16_t threshold);

    HAL_Response<uint16_t> _light_int_high_threshold();
    HAL_StatusTypeDef _set_light_int_high_threshold(uint16_t threshold);

    HAL_Response<uint8_t> _ambient_light_int_enabled();
    HAL_StatusTypeDef _set_ambient_light_int_enabled(uint8_t value);

    HAL_Response<uint8_t> _proximity_int_enabled();
    HAL_StatusTypeDef _set_proximity_int_enabled(uint8_t value);

    HAL_StatusTypeDef _clear_ambient_light_int();
    HAL_StatusTypeDef _clear_proximity_int();
    HAL_StatusTypeDef _clear_all_int();

    HAL_Response<float> _float_ambient_to_lux(uint16_t Ch0, uint16_t Ch1);

    HAL_Response<uint16_t> _ch0_light();
    HAL_Response<uint16_t> _ch1_light();
};

#endif // APDS9930_H