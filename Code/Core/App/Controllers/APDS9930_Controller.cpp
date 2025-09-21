#include "APDS9930_Controller.h"

APDS9930_Controller::APDS9930_Controller(I2C_HandleTypeDef *hi2c1)
{
    APDS9930::Config config;
    this->_sensor = shared_ptr<APDS9930>(new APDS9930(hi2c1, config));

    this->_errorCode = this->_sensor->init();

    this->_status = this->_errorCode < 0 ? Status::Error : Status::Ok;
}

Validated<APDS9930_Controller::AmbientLight> APDS9930_Controller::ReadAmbientLight()
{
    AmbientLight light;

    bool valid = this->_sensor->readAmbientLightLux(light.Lux) &&
                 this->_sensor->readCh0Light(light.Ch0) &&
                 this->_sensor->readCh1Light(light.Ch1);

    return Validated<AmbientLight>(valid, light);
}
