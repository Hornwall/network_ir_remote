#ifndef Signal_h
#define Signal_h

#include "Arduino.h"

struct Signal {
  uint16_t length;
  uint16_t rawData[130];
  char name[20];
  bool ready;
};

#endif
