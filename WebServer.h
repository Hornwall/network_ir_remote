#ifndef WebServer_h
#define WebServer_h

#include "Arduino.h"
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

class WebServer
{
  public:
    WebServer(int port);
    virtual void startServer();
    virtual void receivedData(uint16_t length, uint16_t *rawData);
    virtual boolean readerActive();
  protected:
    AsyncWebServer server;
};
#endif
