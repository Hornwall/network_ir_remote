#include "Arduino.h"
#include "SetupServer.h"

void SetupServer::startServer() {
  Serial.println("Starting setup server...");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.println("Handling /");
      request->send(SPIFFS, "/wifi_setup/index.html", String(), false);
      });

  server.on("/setup_wifi", HTTP_POST, [this](AsyncWebServerRequest *request){
      Serial.println("Handling /setup_wifi");
      handleSetupWifi(request);
      });

  server.begin();
}


void SetupServer::handleSetupWifi(AsyncWebServerRequest *request) {

  if(!request->hasParam("ssid", true) || !request-> hasParam("password", true)) {
    request->redirect("/");
    return;
  }

  AsyncWebParameter* ssid = request->getParam("ssid", true);
  AsyncWebParameter* password = request->getParam("password", true);

  Serial.println("");
  Serial.printf("SSID: %s, Password: %s", ssid->value().c_str(), password->value().c_str());
  storeWifiSettings(ssid->value().c_str(), password->value().c_str());

  request->redirect("/");
}

void SetupServer::storeWifiSettings(const char* ssid, const char* password) {
  strncpy(wifiSettings.ssid, ssid, 32);
  strncpy(wifiSettings.password, password, 64);
  wifiSettings.setupComplete = true;
  EEPROM.put(_addr, wifiSettings);
  EEPROM.commit();
}

