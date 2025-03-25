#include <Arduino.h>

#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
  #include <ESPmDNS.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <ESP8266mDNS.h>
#endif

#include <ESPAsyncWebServer.h>
#include <ESPAsyncHTTPUpdateServer.h>

#include <LittleFS.h>

#define DEBUG

//#define STATION

#ifdef STATION
  const char *ssid = "wifi-ssid";
  const char *password = "wifi-password";
#else
  const char *ssid = "esp-test";
  const char *password = "esp-test321";
#endif

const char *otaUsername = "esp-test";
const char *otaPassword = "esp-test";
const char *hostname = "test";
const char *mdns = "test";

// Definitions
void listFiles();

void configRoutes(AsyncWebServer *server);
void notFound(AsyncWebServerRequest *request);

AsyncWebServer _server(80);
ESPAsyncHTTPUpdateServer _updateServer;