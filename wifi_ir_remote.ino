#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include "FS.h"
#include "WebServer.h"
#include "SetupServer.h"
#include "RemoteServer.h"
#include "WifiSettings.h"
#include "Signal.h"
#include "RemoteSettings.h"
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>

int addr = 0;

const char* ssid = "WIFI Remote";
const char* password = "password";

WifiSettings wifiSettings;

WebServer* webServer;

const uint16_t irReceiverPin = D3;
const uint16_t bufferSize = 1024;
const uint8_t timeout = 15;

IRrecv irrecv(irReceiverPin, bufferSize, timeout, true);
decode_results results; 

void setupReceiver() {
  assert(irutils::lowLevelSanityCheck() == 0);
  irrecv.setTolerance(kTolerance);
  irrecv.enableIRIn();
}

void eepromSetup() {
  EEPROM.begin(4096);
  EEPROM.get(addr, wifiSettings);
}

void wifiSetup() {
  WiFi.hostname("ir-remote");
  if(wifiSettings.setupComplete) {
    WiFi.begin(wifiSettings.ssid, wifiSettings.password);
  } else {
    WiFi.softAP(ssid, password);
  }
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
}

void setup() {
  Serial.begin(115200);
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  eepromSetup();

  if(wifiSettings.setupComplete) {
    Serial.println("WIFI SETUP COMPLETE");
    Serial.println("Booting into regular mode");
    setupReceiver();
    webServer = new RemoteServer(80, D3);
  } else {
    Serial.println("WIFI SETUP MODE");
    webServer = new SetupServer(80);
  }

  wifiSetup();


  webServer->startServer();

}

void loop() {
  // Check if the IR code has been received.
  if(wifiSettings.setupComplete && webServer->readerActive()) {
    if (irrecv.decode(&results)) {
      webServer->receivedData(getCorrectedRawLength(&results), resultToRawArray(&results));

      yield();
    }
  }
}
