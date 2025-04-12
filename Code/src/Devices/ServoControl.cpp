#include "Devices/ServoControl.h"
#include "ServoControl.h"

ServoControl::ServoControl(int pin, int default_angle)
{
    this->_pin = pin;
    this->_default_angle = default_angle;
}

void ServoControl::Init()
{
    this->_servo.attach(this->_pin);
    this->SetAngle(this->_default_angle);
    delay(50);
}

int ServoControl::DefaultAngle() const
{
    return this->_default_angle;
}

void ServoControl::SetDefaultAngle(int value)
{
    this->_default_angle = value < 0 ? 0 : value > 180 ? 180
                                                       : value;
}

void ServoControl::Reset()
{
    this->SetAngle(this->_default_angle);
}

int ServoControl::Angle()
{
    return this->_servo.read();
}

void ServoControl::SetAngle(int value)
{
    if (value < 0)
        value = 0;
    else if (value > 180)
        value = 180;

    int current_angle = this->_servo.read();

    if (current_angle == value)
        return;

    if (value > current_angle)
    {
        for (int i = current_angle; i <= value; i++)
        {
            this->_servo.write(i);
            delay(STEP_DELAY);
        }
    }
    else
    {
        for (int i = current_angle; i >= value; i--)
        {
            this->_servo.write(i);
            delay(STEP_DELAY);
        }
    }
}
