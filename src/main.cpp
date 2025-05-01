#include "main.h"

void listFiles(AsyncWebServerRequest *request)
{
  String result = "";

#ifdef ESP32
  File dir = LittleFS.open("/");
  File file = dir.openNextFile();
  while (file) {
    result += String(file.name()) + "\n";
    file = dir.openNextFile();
  }
#else
  Dir dir = LittleFS.openDir("/");
  while (dir.next())
    result += dir.fileName() + "\n";
#endif

  request->send(200, "text/plain", "[\n" + result + "]");
}

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found.");
}

// MARK:ROUTES
void configRoutes(AsyncWebServer *server)
{
  server->on("^\\/$", HTTP_GET, [](AsyncWebServerRequest *request)
             { listFiles(request); });

  server->serveStatic("/", LittleFS, "/");

  server->onNotFound(notFound);
}

// MARK:SETUP
void setup()
{
#ifdef DEBUG
  Serial.begin(115200);
#endif
  WiFi.hostname(hostname);

#ifdef STATION
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
#ifdef DEBUG
    Serial.println("Connecting to WiFi...");
#endif
  }
#ifdef DEBUG
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
#endif
#else
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
#ifdef DEBUG
  Serial.println("Access Point started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
#endif
#endif

#ifdef DEBUG
  if (MDNS.begin(mdns))
    Serial.println("mDNS responder started");
#else
  MDNS.begin(mdns);
#endif

  LittleFS.begin();

  // listFiles();

  configRoutes(&_server);
  _updateServer.setup(&_server, otaUsername, otaPassword);
  _updateServer.onUpdateBegin = [](const UpdateType type, int &result)
  {
    if (digitalRead(KEY) == HIGH)
      result = UpdateResult::UPDATE_ABORT;
    else
      Serial.println("Update started : " + String(type));
  };
  _updateServer.onUpdateEnd = [](const UpdateType type, int &result)
  {
    Serial.println("Update finished : " + String(type) + " result: " + String(result));
  };
  _server.begin();

#ifdef DEBUG
  Serial.println("HTTP server started");
#endif

  pinMode(LED, OUTPUT);
  pinMode(KEY, INPUT_PULLUP);

  digitalWrite(LED, LOW);
}

// MARK:LOOP
void loop()
{
  digitalWrite(LED, digitalRead(KEY) == LOW);

  yield();
#ifdef ESP8266
  MDNS.update();
#endif
}
