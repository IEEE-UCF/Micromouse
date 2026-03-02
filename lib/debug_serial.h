#ifdef SERIAL_COM

#include "include/config.h"
#include <Arduino.h>

namespace Serial {
void start() { Serial.begin(BAUD_RATE); }
} // namespace Serial

#endif
