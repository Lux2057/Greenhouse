#ifndef APP_H
#define APP_H

#include "stm32f4xx_hal.h"
#include "ControllersFactory.h"
#include "../Drivers/PCA9548A.h"

using namespace std;

class App
{
public:
    App() = delete;
    App(PCA9548A::Config config);

    void Start();

private:
    ControllersFactory::ControllersList _controllers;    
};

#endif // APP_H