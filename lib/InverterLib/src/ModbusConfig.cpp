#include "ModbusConfig.h"

ModbusConfig::ModbusConfig() : _id(1), _baud(9600), _serialConfig(SERIAL_8N1) {}

void ModbusConfig::applyTo(ModbusRTU& mb, HardwareSerial& serial) {
    // Não iniciar o modbus e o serial varias vezes. Ajustar isso aqui depois
    serial.begin(_baud, _serialConfig);
    mb.begin(&serial);

}

void ModbusConfig::setConfig(uint8_t id, uint32_t baud) {
    _id = id;
    _baud = baud;
}

void ModbusConfig::setConfig(uint8_t id, uint32_t baud, SerialConfig config) {
    _id = id;
    _baud = baud;
    _serialConfig = config;
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

uint8_t ModbusConfig::getId() const {
    return _id;
}

uint32_t ModbusConfig::getBaud() const {
    return _baud;
}

SerialConfig ModbusConfig::getSerialConfig() const {
    return _serialConfig;
}