#ifndef APP_H
#define APP_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rtc.h"
#include "ControllersFactory.h"
#include "../Drivers/PCA9548A.h"
#include "../Drivers/MG90s_180.h"
#include "../Models/Structs.h"

using namespace std;

class App
{
public:
    App() = delete;
    App(AppConfig config);

    void Start();

private:
    ControllersFactory::ControllersList _controllers;
    AppConfig _config;
};

#endif // APP_H