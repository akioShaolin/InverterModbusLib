#include "InverterModbusBus.h"

#include <string.h>

static InverterModbusBus* busSlots[INV_MAX_MODBUS_BUSES] = {nullptr};

static InverterModbusStatus convertResultCode(Modbus::ResultCode result) {
    switch (result) {
        case Modbus::EX_SUCCESS:             return INV_MB_SUCCESS;
        case Modbus::EX_TIMEOUT:             return INV_MB_TIMEOUT;
        case Modbus::EX_ILLEGAL_FUNCTION:    return INV_MB_ILLEGAL_FUNCTION;
        case Modbus::EX_ILLEGAL_ADDRESS:     return INV_MB_ILLEGAL_ADDRESS;
        case Modbus::EX_ILLEGAL_VALUE:       return INV_MB_ILLEGAL_VALUE;
        case Modbus::EX_SLAVE_FAILURE:       return INV_MB_SLAVE_FAILURE;
        case Modbus::EX_UNEXPECTED_RESPONSE: return INV_MB_UNEXPECTED_RESPONSE;
        default:                             return INV_MB_GENERAL_FAILURE;
    }
}

InverterModbusBus::InverterModbusBus(ModbusRTU& mb)
    : _mb(&mb),
      _serial(nullptr),
      _owner(nullptr),
      _activeRequest(REQ_NONE),
      _state(BUS_IDLE),
      _resultCode(Modbus::EX_SUCCESS),
      _status(INV_MB_NONE),
      _registerCount(0),
      _callbackDone(false),
      _initialized(false),
      _slot(0xFF),
      _startedAt(0),
      _timeoutMs(1000),
      _baud(0),
      _serialConfig(SERIAL_8N1),
      _deRePin(-1) {
    memset(_buffer, 0, sizeof(_buffer));

    for (uint8_t i = 0; i < INV_MAX_MODBUS_BUSES; ++i) {
        if (busSlots[i] == nullptr) {
            busSlots[i] = this;
            _slot = i;
            break;
        }
    }
}

InverterModbusBus::~InverterModbusBus() {
    if (_slot < INV_MAX_MODBUS_BUSES && busSlots[_slot] == this) {
        busSlots[_slot] = nullptr;
    }
}

bool InverterModbusBus::begin(HardwareSerial& serial, uint32_t baud,
                              SerialConfig serialConfig, int8_t deRePin) {
    if (_slot >= INV_MAX_MODBUS_BUSES || baud == 0) return false;
    if (_state != BUS_IDLE) return false;

    _serial = &serial;
    _baud = baud;
    _serialConfig = serialConfig;
    _deRePin = deRePin;

    _serial->begin(_baud, _serialConfig);
    if (_deRePin >= 0) {
        _mb->begin(_serial, _deRePin);
    } else {
        _mb->begin(_serial);
    }
    _mb->master();
    _initialized = true;
    return true;
}

void InverterModbusBus::task() {
    if (!_initialized) return;

    _mb->task();
    if (_state == BUS_WAITING && _callbackDone) {
        _state = _resultCode == Modbus::EX_SUCCESS ? BUS_DONE : BUS_ERROR;
    }
}

bool InverterModbusBus::isBusy() const {
    return _state != BUS_IDLE || (_initialized && _mb->slave() != 0);
}

bool InverterModbusBus::belongsTo(const Inverter* owner, InverterRequestId request) const {
    return _owner == owner && _activeRequest == request;
}

bool InverterModbusBus::isCompletedFor(const Inverter* owner, InverterRequestId request) const {
    return _state == BUS_DONE && belongsTo(owner, request);
}

bool InverterModbusBus::isFailedFor(const Inverter* owner, InverterRequestId request) const {
    return _state == BUS_ERROR && belongsTo(owner, request);
}

const uint16_t* InverterModbusBus::buffer() const {
    return _buffer;
}

uint16_t InverterModbusBus::registerCount() const {
    return _registerCount;
}

InverterModbusStatus InverterModbusBus::transactionStatus() const {
    return _status;
}

bool InverterModbusBus::startRead(Inverter* owner, InverterRequestId request,
                                  uint8_t slaveId, uint16_t address,
                                  uint16_t registerCount) {
    if (!_initialized || owner == nullptr || request == REQ_NONE) return false;
    if (_state != BUS_IDLE || _mb->slave() != 0) return false;
    if (registerCount == 0 || registerCount > INV_ASYNC_BUFFER_REGS) return false;

    cbTransaction cb = callback();
    if (cb == nullptr) return false;

    _owner = owner;
    _activeRequest = request;
    _registerCount = registerCount;
    _resultCode = Modbus::EX_SUCCESS;
    _status = INV_MB_NONE;
    _callbackDone = false;
    memset(_buffer, 0, sizeof(_buffer));

    if (!_mb->readHreg(slaveId, address, _buffer, registerCount, cb)) {
        _state = BUS_ERROR;
        _status = INV_MB_GENERAL_FAILURE;
        return false;
    }

    _startedAt = millis();
    _state = BUS_WAITING;
    return true;
}

bool InverterModbusBus::startWrite(Inverter* owner, InverterRequestId request,
                                   uint8_t slaveId, uint16_t address,
                                   const uint16_t* values, uint16_t registerCount) {
    if (!_initialized || owner == nullptr || request == REQ_NONE || values == nullptr) return false;
    if (_state != BUS_IDLE || _mb->slave() != 0) return false;
    if (registerCount == 0 || registerCount > INV_ASYNC_BUFFER_REGS) return false;
    if ((uint32_t)address + registerCount > 0x10000UL) return false;

    cbTransaction cb = callback();
    if (cb == nullptr) return false;

    _owner = owner;
    _activeRequest = request;
    _registerCount = registerCount;
    _resultCode = Modbus::EX_SUCCESS;
    _status = INV_MB_NONE;
    _callbackDone = false;
    memcpy(_buffer, values, registerCount * sizeof(uint16_t));

    const bool accepted = registerCount == 1
        ? _mb->writeHreg(slaveId, address, _buffer[0], cb)
        : _mb->writeHreg(slaveId, address, _buffer, registerCount, cb);
    if (!accepted) {
        _state = BUS_ERROR;
        _status = INV_MB_GENERAL_FAILURE;
        return false;
    }

    _startedAt = millis();
    _state = BUS_WAITING;
    return true;
}

void InverterModbusBus::release() {
    _owner = nullptr;
    _activeRequest = REQ_NONE;
    _state = BUS_IDLE;
    _status = INV_MB_NONE;
    _registerCount = 0;
    _callbackDone = false;
}

bool InverterModbusBus::complete(Modbus::ResultCode result) {
    _resultCode = result;
    _status = convertResultCode(result);
    _callbackDone = true;
    return true;
}

bool InverterModbusBus::callback0(Modbus::ResultCode result, uint16_t, void*) {
    return busSlots[0] != nullptr && busSlots[0]->complete(result);
}

bool InverterModbusBus::callback1(Modbus::ResultCode result, uint16_t, void*) {
    return busSlots[1] != nullptr && busSlots[1]->complete(result);
}

cbTransaction InverterModbusBus::callback() const {
    switch (_slot) {
        case 0: return callback0;
        case 1: return callback1;
        default: return nullptr;
    }
}
