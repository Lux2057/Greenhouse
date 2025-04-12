#ifndef SERVOCONTROL_H
#define SERVOCONTROL_H

#include <Arduino.h>
#include <Servo.h>

#define STEP_DELAY 10

class ServoControl
{
public:
    ServoControl(int pin,                 
                 int default_angle = 0);
    void Init();

    int Angle();
    void SetAngle(int value);
    
    int DefaultAngle() const;
    void SetDefaultAngle(int value);
    
    void Reset();

private:
    Servo _servo;
    int _pin;
    int _default_angle;    
};

#endif