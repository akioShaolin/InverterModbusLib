#pragma once

#include <Arduino.h>
#include <assert.h>
#include <string.h>

namespace Modbus {
enum ResultCode {
    EX_SUCCESS = 0x00,
    EX_ILLEGAL_FUNCTION = 0x01,
    EX_ILLEGAL_ADDRESS = 0x02,
    EX_ILLEGAL_VALUE = 0x03,
    EX_SLAVE_FAILURE = 0x04,
    EX_UNEXPECTED_RESPONSE = 0xE3,
    EX_TIMEOUT = 0xE4
};
}

typedef bool (*cbTransaction)(Modbus::ResultCode, uint16_t, void*);

// Models enqueue and completion, not serial bytes/timing or the Modbus stack.
class ModbusRTU {
public:
    bool accept = true;
    bool ready = false;
    uint8_t activeSlave = 0;
    uint8_t lastFunction = 0;
    uint16_t lastAddress = 0;
    uint16_t lastCount = 0;
    uint16_t sent[16] = {};
    uint16_t response[16] = {};
    uint16_t* destination = nullptr;
    Modbus::ResultCode result = Modbus::EX_SUCCESS;
    cbTransaction completion = nullptr;
    uint32_t calls = 0;

    void begin(HardwareSerial*, int8_t = -1) {}
    void master() {}
    uint8_t slave() const { return activeSlave; }

    bool readHreg(uint8_t id, uint16_t address, uint16_t* values,
                  uint16_t count, cbTransaction cb) {
        destination = values;
        return enqueue(id, address, count, 3, cb);
    }
    bool writeHreg(uint8_t id, uint16_t address, uint16_t value, cbTransaction cb) {
        destination = nullptr;
        sent[0] = value;
        return enqueue(id, address, 1, 6, cb);
    }
    bool writeHreg(uint8_t id, uint16_t address, uint16_t* values,
                   uint16_t count, cbTransaction cb) {
        destination = nullptr;
        memcpy(sent, values, count * sizeof(uint16_t));
        return enqueue(id, address, count, 16, cb);
    }
    void task() {
        if (!ready || completion == nullptr) return;
        if (destination != nullptr) {
            // Deliberately copies even on an error to test invalid RX removal.
            memcpy(destination, response, lastCount * sizeof(uint16_t));
        }
        cbTransaction cb = completion;
        completion = nullptr;
        ready = false;
        activeSlave = 0;
        cb(result, 1, nullptr);
    }

private:
    bool enqueue(uint8_t id, uint16_t address, uint16_t count,
                 uint8_t function, cbTransaction cb) {
        assert(count <= 16);
        ++calls;
        lastFunction = function;
        lastAddress = address;
        lastCount = count;
        if (!accept) return false;
        activeSlave = id;
        completion = cb;
        return true;
    }
};
