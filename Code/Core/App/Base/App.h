#ifndef APP_H
#define APP_H

#include "stm32f4xx_hal.h"
#include "ControllersFactory.h"

class App
{
public:
    App() = delete;
    App(ControllersFactory::Config config);

    void Start();

private:
    ControllersFactory::ControllersList _controllers;
    ControllersFactory::Config _config;
};

#endif // APP_H