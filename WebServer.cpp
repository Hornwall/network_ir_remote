#include "Arduino.h"
#include "WebServer.h"

WebServer::WebServer(int port) : server(port) {
}

void WebServer::startServer() {
  Serial.println("WEBSERVER NOT IMPLEMENTED");
}

void WebServer::receivedData(uint16_t length, uint16_t *rawData) {
}

boolean WebServer::readerActive() {
  return false;
}
