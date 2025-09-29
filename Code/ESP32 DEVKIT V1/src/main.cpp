#include <Arduino.h>
#include <WiFi.h>

const char *ssid = "";
const char *password = "";

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  delay(100);

  Serial.println("Setup has been completed.");
}

bool wifi_connected = false;

void loop()
{
  if (!wifi_connected)
  {
    Serial.println("WiFi scanning...");
    int n = WiFi.scanNetworks();
    Serial.println("WiFi scanning has been done.");

    if (n == 0)
    {
      Serial.println("No networks have been found.");
    }
    else
    {
      Serial.print(n);
      Serial.println(" networks have been found.");

      bool homeWiFi_found = false;

      for (int i = 0; i < n; i++)
      {
        if (WiFi.SSID(i) != ssid)
        {
          continue;
        }

        Serial.print(ssid);
        Serial.println(" has been found! Connecting...");

        WiFi.begin(ssid, password);

        int connection_count = 0;
        while (WiFi.status() != WL_CONNECTED && connection_count < 10)
        {
          Serial.print(".");
          connection_count++;
          delay(1000);
        }

        Serial.println("");

        if (WiFi.status() == WL_CONNECTED)
        {
          Serial.println("Connected!");
          Serial.print("Local IP: ");
          Serial.println(WiFi.localIP());
          wifi_connected = true;
        }
        else
        {
          Serial.println("Connection has been failed!");
          Serial.print("Current status: ");
          Serial.println(WiFi.status());
        }

        homeWiFi_found = true;

        break;
      }

      if (!homeWiFi_found)
      {
        Serial.print(ssid);
        Serial.println(" have not been found!");
      }
    }
  }
  else
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(300);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
}