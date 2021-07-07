#include "Arduino.h"
#include "RemoteServer.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>

void RemoteServer::startServer() {
  Serial.println("Starting remote server...");

  server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/remote/index.html", String(), false);
      });

  server.on("/ir_send", HTTP_GET, [this](AsyncWebServerRequest *request) {
        const uint16_t irLed = D5;

        Serial.println("Sending: ");
        if(!request->hasParam("slot")) {
          Serial.println("NO SLOT PARAM");
          request->redirect("/");
          return;
        }

        AsyncWebParameter* slot = request->getParam("slot");
        int slotNumber = atoi(slot->value().c_str());

        Serial.println("Sending data in slot: " + String(slotNumber));

        IRsend irsend(irLed);
        irsend.begin();
        irsend.sendRaw(_remoteSettings.signals[slotNumber].rawData, _remoteSettings.signals[slotNumber].length, 38);
        request->redirect("/");
      });

  server.on("/settings/activate_reader", HTTP_GET, [this](AsyncWebServerRequest *request){
      Serial.println("Activating reader");
      _readerActive = true;
      request->redirect("/settings");
      });

  server.on("/settings/store_button", HTTP_POST, [this](AsyncWebServerRequest *request){
      if(!request->hasParam("name", true) && !request->hasParam("slot", true)) {
        request->redirect("/settings");
        return;
      }

      AsyncWebParameter* name = request->getParam("name", true);
      AsyncWebParameter* slot = request->getParam("slot", true);
      strncpy(_rawSignal.name, name->value().c_str(), 130);
      _remoteSettings.signals[atoi(slot->value().c_str())] = _rawSignal;
      EEPROM.put(_addr, _remoteSettings);
      EEPROM.commit();

      _rawSignal.ready = false;

      request->redirect("/settings");
      });

  server.on("/settings", HTTP_GET, [this](AsyncWebServerRequest *request){
      Serial.println("Handling /settings");
      if(_rawSignal.ready) {
        request->send(SPIFFS, "/remote/store-button.html", String(), false);
      } else {
        request->send(SPIFFS, "/remote/settings.html", String(), false);
      }
      });


  server.begin();
}

void RemoteServer::receivedData(uint16_t length, uint16_t *rawData) {
  Serial.println("Got value, deactivating reader");
  _readerActive = false;
  _rawSignal.length = length;
  for(int i = 0; i < length; i++) {
    _rawSignal.rawData[i] = rawData[i];
  }
  _rawSignal.ready = true;
}

boolean RemoteServer::readerActive() {
  return _readerActive;
}
