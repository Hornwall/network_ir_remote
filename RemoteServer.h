#ifndef RemoteServer_h
#define RemoteServer_h

#include <EEPROM.h>
#include "WebServer.h"
#include "WifiSettings.h"
#include "RemoteSettings.h"
#include "Signal.h"

class RemoteServer : public WebServer
{
  public:
    RemoteServer(int port, uint16_t receivePin) : WebServer(port) {
      //Setup EEPROM
      _addr = sizeof(WifiSettings) + 1;
      _readerActive = false;
      EEPROM.get(_addr, _remoteSettings);
    };
    void startServer() override;
    void receivedData(uint16_t length, uint16_t *rawData) override;
    boolean readerActive() override;
  private:
    RemoteSettings _remoteSettings;
    int _addr;
    boolean _readerActive;
    Signal  _rawSignal;
};
#endif
