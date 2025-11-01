#include "App.h"

App::App(PCA9548A::Config config)
{
    ControllersFactory controllersFactory;
    this->_controllers = controllersFactory.Build(config);
}

void App::Start()
{
    LED_Controller::BlinkConfig blinkConfig = this->_controllers.LED->OK;

    char *text = "Preved, Medved!";
    this->_controllers.Display->WriteString(text, this->_controllers.Display->Font_7x10, SSD1306::Color::White);
    this->_controllers.Display->SetCursor(0, 10);
    this->_controllers.Display->WriteString(text, this->_controllers.Display->Font_7x10, SSD1306::Color::White);
    this->_controllers.Display->SetCursor(0, 25);
    text = "Temp.:";
    this->_controllers.Display->WriteString(text, this->_controllers.Display->Font_7x10, SSD1306::Color::White);
    this->_controllers.Display->SetCursor(49, 25);
    text = "30";
    this->_controllers.Display->WriteString(text, this->_controllers.Display->Font_7x10, SSD1306::Color::White);
    this->_controllers.Display->SetCursor(64, 20);
    text = "o";
    this->_controllers.Display->WriteString(text, this->_controllers.Display->Font_6x8, SSD1306::Color::White);
    this->_controllers.Display->UpdateScreen();

    ValidatedValue<AHT10::Data> data;
    while (true)
    {
        data = this->_controllers.HumidityAndTemperature->Value();

        if (data.Valid)
        {
            blinkConfig = this->_controllers.LED->OK;
        }
        else
        {
            blinkConfig = this->_controllers.LED->ERROR;
        }

        this->_controllers.LED->Blink(blinkConfig);

        HAL_Delay(1000);
    }
}