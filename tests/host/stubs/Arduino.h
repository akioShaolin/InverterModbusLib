#pragma once

// Host-only substitutes. Never include this directory in the ESP8266 build.
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <string>
#include <type_traits>

#define PROGMEM
#define memcpy_P memcpy
inline uint8_t pgm_read_byte(const void* p) { return *static_cast<const uint8_t*>(p); }
inline uint32_t pgm_read_dword(const void* p) {
    uint32_t value;
    std::memcpy(&value, p, sizeof(value));
    return value;
}

enum SerialConfig : uint32_t { SERIAL_8N1 };
class HardwareSerial {
public:
    void begin(uint32_t, SerialConfig) {}
    void flush() {}
};
extern HardwareSerial Serial;
extern uint32_t hostMillis;
inline uint32_t millis() { return hostMillis; }
inline uint32_t micros() { return hostMillis * 1000U; }
inline void yield() { ++hostMillis; }
inline void delay(uint32_t ms) { hostMillis += ms; }

class String : public std::string {
public:
    using std::string::string;
    using std::string::operator=;
    String(const std::string& value) : std::string(value) {}
    String(std::string&& value) : std::string(std::move(value)) {}
    template<class T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    explicit String(T value) : std::string(std::to_string(value)) {}
    String(float value, int digits) {
        char formatted[64]; std::snprintf(formatted, sizeof(formatted), "%.*f", digits, (double)value);
        assign(formatted);
    }
    bool startsWith(const char* value) const { return rfind(value, 0) == 0; }
    String substring(size_t position) const { return substr(position); }
};

constexpr int OUTPUT = 1, HIGH = 1, LOW = 0;
inline void pinMode(uint8_t, int) {}
inline void digitalWrite(uint8_t, int) {}
struct FakeEsp {
    uint32_t getFreeHeap() const { return 32768; }
    uint32_t getFlashChipRealSize() const { return 1024U * 1024U; }
    uint32_t getFlashChipSize() const { return 1024U * 1024U; }
};
extern FakeEsp ESP;
