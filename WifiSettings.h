#ifndef WifiSettings_h
#define WifiSettings_h

#include "Arduino.h"

struct WifiSettings {
  bool setupComplete = false;
  char ssid[32];
  char password[64];
};

#endif
