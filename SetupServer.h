#ifndef SetupServer_h
#define SetupServer_h


#include "WebServer.h"
#include "WifiSettings.h"
#include <EEPROM.h>

class SetupServer : public WebServer
{
  public:
    SetupServer(int port) : WebServer(port) {
      _addr = 0;
    };
    void startServer() override;
  private:
    void handleSetupWifi(AsyncWebServerRequest *request);
    void storeWifiSettings(const char* ssid, const char* password);
    WifiSettings wifiSettings;
    int _addr;
};
#endif
