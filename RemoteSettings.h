#ifndef RemoteSettings_h
#define RemoteSettings_h

#include "Arduino.h"
#include "Signal.h"

struct RemoteSettings {
  Signal signals[10];
};

#endif
