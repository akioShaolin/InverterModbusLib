#pragma once
constexpr int WIFI_AP = 1;
struct FakeWiFi {
    void persistent(bool) {}
    void mode(int) {}
    void softAP(const char*, const char*) {}
};
extern FakeWiFi WiFi;
