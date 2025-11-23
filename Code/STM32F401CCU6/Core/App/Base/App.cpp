#include "App.h"

App::App(AppConfig config)
{
    ControllersFactory controllersFactory;
    this->_controllers = controllersFactory.Build(config);
    this->_config = config;
}

void App::Start()
{
    LED_Controller::BlinkConfig blinkConfig = this->_controllers.LED->OK;

    /*char *text = "Preved, Medved!";
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
    this->_controllers.Display->UpdateScreen();*/

    /*ValidatedValue<AHT10::Data> climatic_data;
    ValidatedValue<float> light_data;*/

    RTC_TimeTypeDef time;    
    RTC_DateTypeDef date;

    while (true)
    {
        HAL_RTC_GetTime(this->_config.DtSource, &time, RTC_FORMAT_BIN);
        HAL_RTC_GetDate(this->_config.DtSource, &date, RTC_FORMAT_BIN);        
        
        this->_controllers.LED->Blink(LED_Controller::BlinkConfig(time.Seconds * 20, 100, 1));

        //HAL_Delay(1000);

        /*climatic_data = this->_controllers.Sensors->HumidityAndTemperature();

        if (climatic_data.Valid)
        {
            blinkConfig = this->_controllers.LED->OK;
        }
        else
        {
            blinkConfig = this->_controllers.LED->ERROR;
        }

        this->_controllers.LED->Blink(blinkConfig);

        HAL_Delay(200);

        light_data = this->_controllers.Sensors->AmbientLight();

        if (light_data.Valid)
        {
            blinkConfig = this->_controllers.LED->OK;
        }
        else
        {
            blinkConfig = this->_controllers.LED->ERROR;
        }

        this->_controllers.LED->Blink(blinkConfig);*/

        /*this->_controllers.Servo->Rotate(175, 1, 20);
        this->_controllers.LED->Blink(blinkConfig);

        this->_controllers.Servo->Rotate(this->_config.Servo.InitAngle, 1, 20);
        this->_controllers.LED->Blink(blinkConfig);*/
    }
}