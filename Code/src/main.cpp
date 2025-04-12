#include <Arduino.h>

#include "Display/LED.h"
#include "Display/OLED.h"
#include "Sensors/LightSensor.h"
#include "Sensors/TempAndHumidSensor.h"
#include "Devices/ServoControl.h"

LED built_in_led = LED(LED_BUILTIN);
OLED oled = OLED();
TempAndHumidSensor temp_and_humid_sensor = TempAndHumidSensor();
LightSensor light_sensor = LightSensor();
// ServoControl servo = ServoControl(PC14);

void setup()
{
  built_in_led.Init();
  oled.Init();
  // servo.Init();
  temp_and_humid_sensor.Init();
  light_sensor.Init();
}

int loop_index = 1;

void loop()
{
  TempAndHumidSensor::Data tempAndHumid_data = temp_and_humid_sensor.Get_mean_data();
  float light_data = light_sensor.Get_mean_data();

  oled.Clear();

  String text[5];
  text[0] = "Loop number: " + String(loop_index);
  text[1] = "";
  text[2] = "Light: " + String(light_data);
  text[3] = "Temp: " + String(tempAndHumid_data.temperature);
  text[4] = "Humid: " + String(tempAndHumid_data.humidity);
  oled.Print(text, 5);

  built_in_led.blink(50, 50, 3);

  loop_index++;

  delay(200);
}