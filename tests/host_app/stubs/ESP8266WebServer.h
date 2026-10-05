#pragma once
#include "Arduino.h"
#include <functional>
#include <map>

enum HTTPMethod { HTTP_GET, HTTP_POST };
class ESP8266WebServer {
public:
    explicit ESP8266WebServer(int) {}
    std::map<std::string, String> args;
    int responseCode = 0;
    String response;
    uint32_t handled = 0;
    bool hasArg(const char* key) const { return args.count(key); }
    String arg(const char* key) const { auto it = args.find(key); return it == args.end() ? String("") : it->second; }
    void sendHeader(const char*, const String&) {}
    void send(int status, const char*, const String& body) { responseCode = status; response = body; }
    void send_P(int status, const char*, const char* body) { responseCode = status; response = body; }
    template<class T> void streamFile(T&, const char*) { responseCode = 200; }
    template<class F> void on(const char*, HTTPMethod, F) {}
    template<class F> void onNotFound(F) {}
    void begin() {}
    void handleClient() { ++handled; }
};
