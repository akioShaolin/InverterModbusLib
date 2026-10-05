#pragma once
#include <cstddef>
#include <cstdint>
#include <string>

extern uint32_t fakeMillis;
uint32_t millis();
uint32_t micros();

class String {
public:
    String(const std::string& value = "") : _value(value) {}
    bool startsWith(const char* prefix) const { return _value.rfind(prefix, 0) == 0; }
    const char* c_str() const { return _value.c_str(); }
private:
    std::string _value;
};
