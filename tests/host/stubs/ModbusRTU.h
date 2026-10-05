#pragma once
#include "Arduino.h"
#include <vector>

namespace Modbus {
enum ResultCode {
    EX_SUCCESS = 0,
    EX_ILLEGAL_FUNCTION = 1,
    EX_ILLEGAL_ADDRESS = 2,
    EX_ILLEGAL_VALUE = 3,
    EX_SLAVE_FAILURE = 4,
    EX_TIMEOUT = 0xE4,
    EX_UNEXPECTED_RESPONSE = 0xE1
};
}
using cbTransaction = bool (*)(Modbus::ResultCode, uint16_t, void*);

class ModbusRTU {
public:
    struct Transaction {
        uint8_t function;
        uint8_t id;
        uint16_t address;
        uint16_t count;
        std::vector<uint16_t> values;
    };
    std::vector<Transaction> sent;
    bool failNextStart = false;

    void begin(HardwareSerial*, int8_t = -1) {}
    void master() {}
    uint8_t slave() const { return activeId; }

    bool readHreg(uint8_t id, uint16_t address, uint16_t* output, uint16_t count,
                  cbTransaction cb) {
        if (!accept(id, cb)) return false;
        destination = output;
        destinationCount = count;
        sent.push_back({3, id, address, count, {}});
        return true;
    }
    bool writeHreg(uint8_t id, uint16_t address, uint16_t value, cbTransaction cb) {
        if (!accept(id, cb)) return false;
        sent.push_back({6, id, address, 1, {value}});
        return true;
    }
    bool writeHreg(uint8_t id, uint16_t address, uint16_t* values, uint16_t count,
                   cbTransaction cb) {
        if (!accept(id, cb)) return false;
        sent.push_back({16, id, address, count, std::vector<uint16_t>(values, values + count)});
        return true;
    }

    void respond(Modbus::ResultCode result, const std::vector<uint16_t>& values = {}) {
        pendingResult = result;
        pendingValues = values;
        responseReady = true;
    }
    void task() {
        if (!responseReady || callback == nullptr) return;
        if (pendingResult == Modbus::EX_SUCCESS && destination != nullptr) {
            for (uint16_t i = 0; i < destinationCount && i < pendingValues.size(); ++i) {
                destination[i] = pendingValues[i];
            }
        }
        cbTransaction cb = callback;
        callback = nullptr;
        activeId = 0;
        responseReady = false;
        cb(pendingResult, 1, nullptr);
    }

private:
    uint8_t activeId = 0;
    cbTransaction callback = nullptr;
    uint16_t* destination = nullptr;
    uint16_t destinationCount = 0;
    bool responseReady = false;
    Modbus::ResultCode pendingResult = Modbus::EX_SUCCESS;
    std::vector<uint16_t> pendingValues;

    bool accept(uint8_t id, cbTransaction cb) {
        if (failNextStart) { failNextStart = false; return false; }
        if (activeId != 0 || id == 0) return false;
        activeId = id;
        callback = cb;
        destination = nullptr;
        destinationCount = 0;
        return true;
    }
};
