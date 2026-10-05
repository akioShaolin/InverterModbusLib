#pragma once

#include <stdint.h>

typedef uint32_t SerialConfig;
static const SerialConfig SERIAL_8N1 = 0;
extern uint32_t hostMillis;
inline uint32_t millis() { return hostMillis; }

class HardwareSerial {
public:
    void begin(uint32_t, SerialConfig) {}
};
