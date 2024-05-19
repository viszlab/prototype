/*

Title: Wi-fi Gateway for Bluebird data physicaliztaion
Description: Connects to Wi-Fi to fetch CO2 data from the building API and
sends data to Arduino Controller over UART

Hardware: ESP (NodeMCU v3)

---
Master: Master Information Studies: Information Systems (track)
University: University of Amsterdam (UvA)
Institute: Informatics Institute
Faculty: Faculty of Science (FNWI)
Research Group: Digital Interactions Lab (DIL)
Student: BSc Danny de Vries (14495643)
Supervisor: Dr. H. (Hamed) Seiied Alavi PhD
---
*/

/* Include Arduino Framework (built-in) */
#include <Arduino.h>

/* Include Wi-Fi capabilities, HTTP fetches and JSON parsing*/
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

/* Stores the wifi-credentials in .env*/
#include <config.h>

/* Coordinates for Amsterdam */
const char *latitude = "52.3676";
const char *longitude = "4.9041";

/* OpenWeatherMap API endpoint build up string */
String apiEndpoint = "http://api.openweathermap.org/data/2.5/weather?lat=" + String(latitude) + "&lon=" + String(longitude) + "&appid=" + String(API_KEY) + "&units=metric";

WiFiClient wifiClient;


/* Set-up of the Wi-Fi connection login if succesful and logigng the IP */
void setup()
{
  delay(5000);
  Serial.begin(115200);

  Serial.println();
  Serial.println("Starting Wi-Fi connection...");

  Serial.print("Connecting to: ");
  Serial.println(WIFI_SSID);

  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long startAttemptTime = millis();

  /* Keep attempting to connect until timeout */
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000)
  {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Connected to Wi-Fi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.print("Wi-Fi connection status: ");
    Serial.println(WiFi.status());
    Serial.println("Failed to connect to Wi-Fi.");
  }
}

/* Fetches weather data for specified city, converts the json and stores in variables*/
void fetchWeatherData()
{
  HTTPClient http;
  http.begin(wifiClient, apiEndpoint);

  int httpCode = http.GET();
  if (httpCode > 0)
  {
    if (httpCode == HTTP_CODE_OK)
    {
      String payload = http.getString();
      Serial.println(payload);

      // Parse JSON
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, payload);
      if (!error)
      {
        float temperature = doc["main"]["temp"];
        Serial.print("Temperature in Amsterdam: ");
        Serial.print(temperature);
        Serial.println(" °C");
      }
      else
      {
        Serial.print("Failed to parse JSON: ");
        Serial.println(error.c_str());
      }
    }
  }
  else
  {
    Serial.print("HTTP GET request failed, error: ");
    Serial.println(http.errorToString(httpCode).c_str());
  }

  http.end();
}

/* Fetches the weather data each 5 second*/
void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    fetchWeatherData();
  }
  else
  {
    Serial.println("Wi-Fi not connected");
  }
  delay(5000);
}