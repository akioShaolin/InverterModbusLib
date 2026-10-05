#pragma once

#include <Arduino.h>
#include <ModbusRTU.h>

#ifndef INV_ASYNC_BUFFER_REGS
#define INV_ASYNC_BUFFER_REGS 16
#endif

#ifndef INV_MAX_MODBUS_BUSES
#define INV_MAX_MODBUS_BUSES 2
#endif

class Inverter;

enum InverterRequestStatus : uint8_t {
    INV_IDLE     = 0x00,
    INV_BUSY     = 0x01,
    INV_DONE     = 0x02,
    INV_ERROR    = 0x03,
    INV_REJECTED = 0x04
};

enum InverterRequestId : uint8_t {
    REQ_NONE = 0,
    REQ_GRID_FREQUENCY,
    REQ_ACTIVE_POWER,
    REQ_RATED_POWER,
    REQ_SERIAL_NUMBER,
    REQ_SET_POWER_LIMIT,
    REQ_SET_POWER_LIMIT_PERCENT
};

enum InverterModbusStatus : uint8_t {
    INV_MB_NONE,
    INV_MB_SUCCESS,
    INV_MB_TIMEOUT,
    INV_MB_ILLEGAL_FUNCTION,
    INV_MB_ILLEGAL_ADDRESS,
    INV_MB_ILLEGAL_VALUE,
    INV_MB_SLAVE_FAILURE,
    INV_MB_UNEXPECTED_RESPONSE,
    INV_MB_GENERAL_FAILURE
};

enum BusTransactionState : uint8_t {
    BUS_IDLE,
    BUS_WAITING,
    BUS_DONE,
    BUS_ERROR
};

// A terminal transaction diagnostic, not a capture of the UART frame or CRC.
// words contains write values (including failed writes), or successful read data.
// Failed read data is zeroed and marked invalid. An enqueue refusal has no
// Modbus response code: accepted and resultCodeValid are both false.
struct InverterTransactionTrace {
    InverterRequestId requestId;
    uint8_t slaveId;
    uint8_t functionCode;
    uint16_t address;
    uint16_t registerCount;
    uint16_t words[INV_ASYNC_BUFFER_REGS];
    InverterModbusStatus status;
    uint16_t resultCode;
    uint32_t startedMs;
    uint32_t durationMs;
    bool accepted;
    bool resultCodeValid;
    bool payloadValid;
};

typedef void (*InverterTraceCallback)(const InverterTransactionTrace&, void*);

class InverterModbusBus {
public:
    explicit InverterModbusBus(ModbusRTU& mb);
    ~InverterModbusBus();

    bool begin(HardwareSerial& serial, uint32_t baud, SerialConfig serialConfig, int8_t deRePin);
    void task();

    // Optional, disabled by default. Called synchronously from task(), or from
    // startRead/startWrite when Modbus refuses to enqueue a valid request.
    // Copy to RAM only: no flash/network I/O, no bus re-entry, and do not retain
    // the reference. The owner/request remain held until normal consumption.
    void setTraceCallback(InverterTraceCallback callback, void* context = nullptr);

    bool isBusy() const;
    bool belongsTo(const Inverter* owner, InverterRequestId request) const;
    bool isCompletedFor(const Inverter* owner, InverterRequestId request) const;
    bool isFailedFor(const Inverter* owner, InverterRequestId request) const;

    const uint16_t* buffer() const;
    uint16_t registerCount() const;
    InverterModbusStatus transactionStatus() const;

private:
    friend class Inverter;

    bool startRead(Inverter* owner, InverterRequestId request, uint8_t slaveId,
                   uint16_t address, uint16_t registerCount);
    bool startWrite(Inverter* owner, InverterRequestId request, uint8_t slaveId,
                    uint16_t address, const uint16_t* values, uint16_t registerCount);
    void release();
    bool complete(Modbus::ResultCode result);
    void emitTrace(bool accepted);

    static bool callback0(Modbus::ResultCode result, uint16_t transactionId, void* data);
    static bool callback1(Modbus::ResultCode result, uint16_t transactionId, void* data);
    cbTransaction callback() const;

    InverterModbusBus(const InverterModbusBus&) = delete;
    InverterModbusBus& operator=(const InverterModbusBus&) = delete;

    ModbusRTU* _mb;
    HardwareSerial* _serial;
    Inverter* _owner;
    InverterRequestId _activeRequest;
    BusTransactionState _state;
    Modbus::ResultCode _resultCode;
    InverterModbusStatus _status;
    uint16_t _buffer[INV_ASYNC_BUFFER_REGS];
    uint16_t _registerCount;
    bool _callbackDone;
    bool _initialized;
    uint8_t _slot;
    uint32_t _startedAt;
    uint32_t _timeoutMs;
    uint32_t _baud;
    SerialConfig _serialConfig;
    int8_t _deRePin;
    InverterTraceCallback _traceCallback;
    void* _traceContext;
    uint8_t _slaveId;
    uint8_t _functionCode;
    uint16_t _address;
};
