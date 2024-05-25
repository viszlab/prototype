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

/* NTP libraries for getting the curent time */
#include <NTPClient.h>
#include <WiFiUdp.h>

/* Stores the wifi-credentials in .env*/
#include <config.h>

WiFiClient wifiClient;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600, 60000);

String currentTime; // Global variable to store the current time
String accessToken; // Global variable to store the access token
String endpointRooms;

float co2Concentration = 0;

    /* Function to send login request */
    void
    sendLoginRequest()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;

    http.begin(wifiClient, API_URL);
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<200> jsonDoc;
    jsonDoc["username"] = API_USERNAME;
    jsonDoc["password"] = API_PASSWORD;

    String requestBody;
    serializeJson(jsonDoc, requestBody);

    int httpResponseCode = http.POST(requestBody);

    if (httpResponseCode > 0)
    {
      String response = http.getString();
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Response: " + response);

      // Parse the response to extract the access token
      StaticJsonDocument<200> responseDoc;
      DeserializationError error = deserializeJson(responseDoc, response);

      if (!error)
      {
        accessToken = responseDoc["access_token"].as<String>();
        Serial.println("Access Token: " + accessToken);
      }
      else
      {
        Serial.println("Failed to parse the access token from response");
      }
    }
    else
    {
      Serial.println("Error in sending POST request");
      Serial.println("HTTP Response code: " + String(httpResponseCode));
    }

    http.end();
  }
  else
  {
    Serial.println("Wi-Fi not connected");
  }
}

/* Set-up of the Wi-Fi connection login if succesful and logigng the IP */
void setup()
{
  delay(5000);
  Serial.begin(115200);

  Serial.println();
  Serial.println("--- Start of set-up ---");

  Serial.println();
  Serial.println("Starting Wi-Fi connection...");

  Serial.print("Connecting to: ");
  Serial.println(WIFI_SSID);

  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long startAttemptTime = millis();

  /* Keep attempting to connect until timeout */
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 30000)
  {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Connected to Wi-Fi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    /* Initialize NTP client */
    timeClient.begin();

    /* Call the function to authenticate */
    sendLoginRequest();
  }
  else
  {
    Serial.print("Wi-Fi connection status: ");
    Serial.println(WiFi.status());
    Serial.println("Failed to connect to Wi-Fi.");
  }

  Serial.println("--- End of set-up ---");
}

void getCurrentTime()
{
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  time_t rawTime = epochTime;
  struct tm *ti;
  ti = localtime(&rawTime);

  // Subtract 2 hours because the API formatting is a bit weird?
  ti->tm_hour -= 2;
  if (ti->tm_hour < 0)
  {
    // If the resulting hour is negative, adjust the date
    ti->tm_mday--;     // Move to the previous day
    ti->tm_hour += 24; // Add 24 hours
  }

  // Set seconds to 00 before formatting time string since this is what the API supports
  ti->tm_sec = 0;

  char buffer[30];
  sprintf(buffer, "%02d-%02d-%02d %02d:%02d:%02d", ti->tm_year + 1900, ti->tm_mon + 1, ti->tm_mday, ti->tm_hour, ti->tm_min, ti->tm_sec);
  currentTime = String(buffer);
  endpointRooms = String(API_ENDPOINT) + String(ROOM_ID) + "/data?startTime=" + String{currentTime} + "&endTime=" + String{currentTime};
}

String encodeURL(const String &url)
{
  String encodedURL = "";
  for (int i = 0; i < url.length(); i++)
  {
    if (url[i] == ' ')
    {
      encodedURL += "%20";
    }
    else
    {
      encodedURL += url[i];
    }
  }
  return encodedURL;
}

void sendRequest(const String &endpoint)
{
  if (WiFi.status() == WL_CONNECTED && accessToken.length() > 0)
  {
    HTTPClient http;

    String encodedEndpoint = encodeURL(endpoint);
    http.begin(wifiClient, encodedEndpoint);

    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Bearer " + accessToken);

    Serial.println("Access Token: " + accessToken);
    Serial.println("Sending GET request to: " + encodedEndpoint);

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0)
    {
      String response = http.getString();
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Response: " + response);

      // Parse JSON response
      DynamicJsonDocument jsonDoc(1024); // Set the JSON document size according to your response size
      DeserializationError error = deserializeJson(jsonDoc, response);

      if (!error)
      {
        // Extract CO2 concentration from response and round it to the nearest whole value
        co2Concentration = round(jsonDoc["results"][0]["airquality"].as<float>());
      }
      else
      {
        Serial.println("Failed to parse JSON response");
      }
    }
    else
    {
      Serial.println("Error in sending GET request");
      Serial.println("HTTP Response code: " + String(httpResponseCode));
    }

    http.end();
  }
}

/* Fetches the sensor withing the API room */
void loop()
{

  Serial.println("--- Start of polling (5s intervals) ---");

  getCurrentTime();
  Serial.println("Current Time: " + currentTime);

  Serial.println("API Endpoint: " + endpointRooms);
  sendRequest(endpointRooms);

  Serial.println("CO2 concentrations: ");
  Serial.println(co2Concentration);

  Serial.println("--- End of polling (5s intervals) ---");

  delay(5000);
}