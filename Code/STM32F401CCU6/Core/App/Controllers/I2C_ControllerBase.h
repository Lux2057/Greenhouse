#ifndef I2C_CONTROLLER_H
#define I2C_CONTROLLER_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"
#include "../App/Models/Structs.h"
#include "../App/Models/Enums.h"
#include "../Drivers/PCA9548A.h"
#include <memory>

using namespace std;

template <typename T_Device>
class I2C_ControllerBase
{
public:
    I2C_ControllerBase(I2C_ControllerConfig config, shared_ptr<PCA9548A> multiplexer) : _config(config), _multiplexer(multiplexer) {};

    virtual void InitDevices() = 0;
    virtual DeviceStatus Status(uint8_t device_index) = 0;    

    uint8_t DevicesCount() const
    {
        return this->_devices_count;
    }

protected:
    I2C_ControllerConfig _config;
    shared_ptr<PCA9548A> _multiplexer{nullptr};

    I2C_Device<T_Device> *_devices{nullptr};
    uint8_t _devices_count{0};

    void _activate_channel(uint8_t channel)
    {
        for (int i = 0; i < this->_config.MultiplexerChannelsCount; i++)
        {
            this->_multiplexer->SelectChannel(this->_config.MultiplexerChannels[i], this->_config.MultiplexerChannels[i] == channel);
        }
    }
};

#endif // I2C_CONTROLLER_H