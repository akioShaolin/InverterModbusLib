/*
 * InverterModbusLib - Solar Inverter Library for Arduino
 * ------------------------------------------------
 * Modbus RTU (RS485)communication layer for inverter integration
 *
 * Copyright (c) 2026, Pedro Akio Sakuma
 * Licensed under BSD 3-Clause License
 */

#include "ModbusConfig.h"

#define DEFAULT_TIMEOUT 1000

static uint32_t _activeBaud = 0;
static SerialConfig _activeSerialConfig = SERIAL_8N1;
static int8_t _activeDeRePin = -1;
static bool _initialized = false;

ModbusConfig::ModbusConfig(uint8_t id, uint32_t baud, SerialConfig config) 
    :_id(id),
    _baud(baud),
    _serialConfig(config),
    _deRePin(-1),
    _timeout(DEFAULT_TIMEOUT)
{}

void ModbusConfig::setConfig(uint8_t id, uint32_t baud) {
    _id = id;
    _baud = baud;
}

void ModbusConfig::setConfig(uint8_t id, uint32_t baud, SerialConfig config) {
    ModbusConfig::setConfig(id, baud);
    _serialConfig = config;
}

void ModbusConfig::setConfig(uint8_t id, uint32_t baud, SerialConfig config, int8_t deRePin) {
    ModbusConfig::setConfig(id, baud, config);
    _deRePin = deRePin;
}

void ModbusConfig::setId(uint8_t id) {
    _id = id;
}

void ModbusConfig::setBaud(uint32_t baud) {
    _baud = baud;
}

void ModbusConfig::setSerialConfig(SerialConfig config) {
    _serialConfig = config;
}

void ModbusConfig::setDeRePin(int8_t pin) {
    _deRePin = pin;
}

void ModbusConfig::setTimeout(uint16_t timeout) {
    _timeout = timeout;
}

uint8_t ModbusConfig::getId() const {
    return _id;
}

uint32_t ModbusConfig::getBaud() const {
    return _baud;
}

SerialConfig ModbusConfig::getSerialConfig() const {
    return _serialConfig;
}

int8_t ModbusConfig::getDeRePin() const {
    return _deRePin;
}

uint16_t ModbusConfig::getTimeout() const {
    return _timeout;
}

bool ModbusConfig::ensureApplied(ModbusRTU& mb, HardwareSerial& serial) {

    bool changed =
        !_initialized ||
        _baud != _activeBaud ||
        _serialConfig != _activeSerialConfig ||
        _deRePin != _activeDeRePin;

    if (!changed) {
        return true;
    }
    
    serial.flush();

    serial.begin(_baud, _serialConfig);

    if (_deRePin >= 0) {
        mb.begin(&serial, _deRePin);
    } else {
        mb.begin(&serial);
    }

    mb.master();

    _activeBaud = _baud;
    _activeSerialConfig = _serialConfig;
    _activeDeRePin = _deRePin;
    _initialized = true;

    delay(2);

    return true;
 }