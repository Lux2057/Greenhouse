#ifndef AHT10_H
#define AHT10_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"
#include "../Models/Enums.h"
#include "../Models/Templates.h"
#include "I2C_DeviceBase.h"

using namespace std;

class AHT10 : public I2C_DeviceBase
{

#define AHT10_DEFAULT_I2C_DEV_ADDRESS 0x38
#define AHT10_CMD_INIT 0xE1
#define AHT10_CMD_MEASURE_TRIG 0xAC
#define AHT10_CMD_SOFT_RESET 0xBA

public:
    struct Data
    {
    public:
        float Temperature;
        float Humidity;
    };

    AHT10() = delete;
    AHT10(Config config);
    
    bool ReInit() override;    

    HAL_Response<Data> Measure();    

private:
    HAL_StatusTypeDef _read_data();
    float _humidity();
    float _temperature();
    void _trigger_measure();

    bool _init() override;
    bool _reset();

    uint8_t _data[6] = {0};
};

#endif // AHT10_H