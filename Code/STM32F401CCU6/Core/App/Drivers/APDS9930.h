#ifndef APDS9930_H
#define APDS9930_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"

using namespace std;

/* Command register modes */
#define APDS9930_REPEATED_BYTE 0x80
#define APDS9930_AUTO_INCREMENT 0xA0
#define APDS9930_SPECIAL_FN 0xE0

/* Error code for returned values */
#define APDS9930_ERROR 0xFF

/* Misc parameters */
#define APDS9930_FIFO_PAUSE_TIME 30 // Wait period (ms) between FIFO reads

/* APDS-9930 register addresses */
#define APDS9930_ENABLE 0x00
#define APDS9930_ATIME 0x01
#define APDS9930_PTIME 0x02
#define APDS9930_WTIME 0x03
#define APDS9930_AILTL 0x04
#define APDS9930_AILTH 0x05
#define APDS9930_AIHTL 0x06
#define APDS9930_AIHTH 0x07
#define APDS9930_PILTL 0x08
#define APDS9930_PILTH 0x09
#define APDS9930_PIHTL 0x0A
#define APDS9930_PIHTH 0x0B
#define APDS9930_PERS 0x0C
#define APDS9930_CONFIG 0x0D
#define APDS9930_PPULSE 0x0E
#define APDS9930_CONTROL 0x0F
#define APDS9930_ID_1 0x12
#define APDS9930_ID_2 0x39
#define APDS9930_STATUS 0x13
#define APDS9930_Ch0DATAL 0x14
#define APDS9930_Ch0DATAH 0x15
#define APDS9930_Ch1DATAL 0x16
#define APDS9930_Ch1DATAH 0x17
#define APDS9930_PDATAL 0x18
#define APDS9930_PDATAH 0x19
#define APDS9930_POFFSET 0x1E

/* Bit fields */
#define APDS9930_PON 0b00000001
#define APDS9930_AEN 0b00000010
#define APDS9930_PEN 0b00000100
#define APDS9930_WEN 0b00001000
#define APSD9930_AIEN 0b00010000
#define APDS9930_PIEN 0b00100000
#define APDS9930_SAI 0b01000000

/* On/Off definitions */
#define APDS9930_OFF 0
#define APDS9930_ON 1

/* Acceptable parameters for setMode */
#define APDS9930_POWER 0
#define APDS9930_AMBIENT_LIGHT 1
#define APDS9930_PROXIMITY 2
#define APDS9930_WAIT 3
#define APDS9930_AMBIENT_LIGHT_INT 4
#define APDS9930_PROXIMITY_INT 5
#define APDS9930_SLEEP_AFTER_INT 6
#define APDS9930_ALL 7

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

/* ALS coefficients */
#define APDS9930_DF 52
#define APDS9930_GA 0.49
#define APDS9930_ALS_B 1.862
#define APDS9930_ALS_C 0.746
#define APDS9930_ALS_D 1.291

class APDS9930
{
public:
    struct Config
    {
    public:
        uint16_t I2C_Address{(uint16_t)(0x39 << 1)};
        uint32_t Timeout{1000};
    };

    APDS9930() = delete;
    APDS9930(I2C_HandleTypeDef *hi2c1, Config config);

    int16_t init();
    uint8_t getMode();
    bool setMode(uint8_t mode, uint8_t enable);

    /* Turn the APDS-9930 on and off */
    bool enablePower();
    bool disablePower();

    /* Enable or disable specific sensors */
    bool enableLightSensor(bool interrupts = false);
    bool disableLightSensor();
    bool enableProximitySensor(bool interrupts = false);
    bool disableProximitySensor();

    /* LED drive strength control */
    uint8_t getLEDDrive();
    bool setLEDDrive(uint8_t drive);
    // uint8_t getGestureLEDDrive();
    // bool setGestureLEDDrive(uint8_t drive);

    /* Gain control */
    uint8_t getAmbientLightGain();
    bool setAmbientLightGain(uint8_t gain);
    uint8_t getProximityGain();
    bool setProximityGain(uint8_t gain);
    bool setProximityDiode(uint8_t drive);
    uint8_t getProximityDiode();

    /* Get and set light interrupt thresholds */
    bool getLightIntLowThreshold(uint16_t &threshold);
    bool setLightIntLowThreshold(uint16_t threshold);
    bool getLightIntHighThreshold(uint16_t &threshold);
    bool setLightIntHighThreshold(uint16_t threshold);

    /* Get and set interrupt enables */
    uint8_t getAmbientLightIntEnable();
    bool setAmbientLightIntEnable(uint8_t enable);
    uint8_t getProximityIntEnable();
    bool setProximityIntEnable(uint8_t enable);

    /* Clear interrupts */
    bool clearAmbientLightInt();
    bool clearProximityInt();
    bool clearAllInts();

    /* Proximity methods */
    bool readProximity(uint16_t &val);

    /* Ambient light methods */
    bool readAmbientLightLux(float &val);
    bool readAmbientLightLux(unsigned long &val);
    float floatAmbientToLux(uint16_t Ch0, uint16_t Ch1);
    unsigned long ulongAmbientToLux(uint16_t Ch0, uint16_t Ch1);
    bool readCh0Light(uint16_t &val);
    bool readCh1Light(uint16_t &val);

private:
    I2C_HandleTypeDef *_hi2c1;
    Config _config;

    /* Proximity Interrupt Threshold */
    uint16_t getProximityIntLowThreshold();
    bool setProximityIntLowThreshold(uint16_t threshold);
    uint16_t getProximityIntHighThreshold();
    bool setProximityIntHighThreshold(uint16_t threshold);

    /* Raw I2C Commands */
    bool wireWriteByte(uint8_t val);
    bool wireWriteDataByte(uint8_t reg, uint8_t val);
    bool wireWriteDataBlock(uint8_t reg, uint8_t *val, unsigned int len);
    bool wireReadDataByte(uint8_t reg, uint8_t &val);
    int wireReadDataBlock(uint8_t reg, uint8_t *val, unsigned int len);
};

#endif // APDS9930_H