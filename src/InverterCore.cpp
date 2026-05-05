/*
 * InverterModbusLib - Solar Inverter Library for Arduino
 * ------------------------------------------------
 * Modbus RTU (RS485)communication layer for inverter integration
 *
 * Copyright (c) 2026, Pedro Akio Sakuma
 * Licensed under BSD 3-Clause License
 */

#include "Inverter.h"

static bool _mb_done = false;
static bool _mb_success = false;

bool _mb_cb(Modbus::ResultCode event, uint16_t, void*) {
    _mb_done = true;
    _mb_success = (event == Modbus::EX_SUCCESS);
    return true;
}

bool Inverter::readScaledFloat(const ModbusField& field, float& value) {
    switch(field.type) {
        case U16: {
            uint16_t raw;
            if (!readField(field, &raw)) return false;
            value = (float)raw * field.scale;
            break;
        }

        case U32: {
            uint32_t raw;
            if (!readField(field, &raw)) return false;
            value = (float)raw * field.scale;
            break;
        }
        
        case FLOAT32: {
            float raw;
            if (!readField(field, &raw)) return false;
            value = raw * field.scale;
            break;
        }

        case I16: {
            int16_t raw;
            if (!readField(field, &raw)) return false;
            value = (float)raw * field.scale;
            break;
        }

        case I32: {
            int32_t raw;
            if (!readField(field, &raw)) return false;
            value = (float)raw * field.scale;
            break;
        }

        default:
            return false; // Tipo de dado não suportado para este campo

    }
    return true;
}

// Private methods para leitura e escrita de registradores Modbus, aplicando a escala correta, lidando com tipos de dados diferentes (inteiros, floats, etc.) e combinando registradores quando necessário. Esses métodos vão usar as informações do ModbusField para fazer a leitura/escrita correta.

bool Inverter::readField(const ModbusField& field, char* value) {
    if (field.type != ASCII || !field.readable) return false;
    if (value == nullptr) return false;
    if (field.length <= 0 || field.length > INV_MAX_STRING_CHARS / 2 + 1) return false;

    uint16_t buffer[INV_MAX_STRING_CHARS / 2 + 1]; // 2 Caracteres ASCII por registrador de 16 bits, mais um nulo ao final
    
    if (field.stride == 1) {
        // Caso simples: os caracteres estão em registradores consecutivos
        if (!readHoldingRegister(field.address, buffer, field.length)) {
            return false;
        }
    } else {
        for (uint16_t i = 0; i < field.length; i++) {
            if (!readHoldingRegister(field.address + i * field.stride, &buffer[i], 1)) {
                return false;
            }
        }
    }
    uint16_t charIndex = 0;

    for (uint16_t i = 0; i < field.length; i++) {
        char high = (char)(buffer[i] >> 8);
        char low  = (char)(buffer[i] & 0xFF);

        if (high >= 32 && high <= 126) {
            value[charIndex++] = high;
        }

        if (low >= 32 && low <= 126) {
            value[charIndex++] = low;
        }
    }
    value[charIndex] = '\0'; // Garantir terminação nula
    return charIndex > 0; // Retorna true se pelo menos um caractere válido foi lido
}

bool Inverter::readField(const ModbusField& field, float* value) {
    if (field.type != FLOAT32 || !field.readable) return false;
    if (value == nullptr) return false;

    uint32_t raw [INV_MAX_FLOAT_VALUES]; // Cada float de 32 bits ocupa 2 registradores de 16 bits
    if(!readField32Raw(field, raw)) return false;

    for (uint16_t i = 0; i < field.length; i++) {
        memcpy(&value[i], &raw[i], sizeof(float));
    }
    return true;
}

bool Inverter::readField(const ModbusField& field, uint16_t* value) {
    if (field.type != U16 || !field.readable) return false;
    return readField16Raw(field, value);
}

bool Inverter::readField(const ModbusField& field, uint32_t* value) {
    if (field.type != U32 || !field.readable) return false;
    return readField32Raw(field, value);
}

bool Inverter::readField(const ModbusField& field, uint64_t* value) {
    if (field.type != U64 || !field.readable) return false;
    return readField64Raw(field, value);
}

bool Inverter::readField(const ModbusField& field, int16_t* value) {
    if (field.type != I16 || !field.readable) return false;
    if (value == nullptr) return false;

    uint16_t raw [INV_MAX_U16_VALUES];
    if(!readField16Raw(field, raw)) return false;

    for (uint16_t i = 0; i < field.length; i++) {
        value[i] = (int16_t)raw[i];
    }
    return true;
}

bool Inverter::readField(const ModbusField& field, int32_t* value) {
    if (field.type != I32 || !field.readable) return false;
    if (value == nullptr) return false;

    uint32_t raw [INV_MAX_U32_VALUES];
    if(!readField32Raw(field, raw)) return false;

    for (uint16_t i = 0; i < field.length; i++) {
        value[i] = (int32_t)raw[i];
    }
    return true;
}

bool Inverter::readField(const ModbusField& field, int64_t* value) {
    if (field.type != I64 || !field.readable) return false;
    if (value == nullptr) return false;

    uint64_t raw [INV_MAX_U64_VALUES];
    if(!readField64Raw(field, raw)) return false;

    for (uint16_t i = 0; i < field.length; i++) {
        value[i] = (int64_t)raw[i];
    }
    return true;
}

bool Inverter::readField16Raw(const ModbusField& field, uint16_t* value) {
    if (!field.readable) return false;
    if (value == nullptr) return false;
    if (field.length == 0 || field.length > INV_MAX_U16_VALUES) return false;

    uint16_t buffer[INV_MAX_U16_VALUES];

    if (field.length == 1) {
        if (!readHoldingRegister(field.address, buffer, 1)) return false;
    } 
    else if (field.stride == 1) {
        if (!readHoldingRegister(field.address, buffer, field.length)) return false;
    } 
    else {
        for (uint16_t i = 0; i < field.length; i++) {
            if (!readHoldingRegister(field.address + i * field.stride, &buffer[i], 1)) {
                return false;
            }
        }
    }
    for (uint16_t i = 0; i < field.length; i++) {
        value[i] = buffer[i];
    }
    return true;
}

bool Inverter::readField32Raw(const ModbusField& field, uint32_t* value) {
    if (!field.readable) return false;
    if (value == nullptr) return false;
    if (field.length == 0 || field.length > INV_MAX_U32_VALUES) return false;

    uint16_t buffer[INV_MAX_U32_VALUES * 2];

    if (field.length == 1) {
        if (!readHoldingRegister(field.address, buffer, 2)) return false;
    } 
    else if (field.stride == 2) {
        if (!readHoldingRegister(field.address, buffer, field.length * 2)) return false;
    } 
    else {
        for (uint16_t i = 0; i < field.length; i++) {
            if (!readHoldingRegister(field.address + i * field.stride, &buffer[i * 2], 2)) {
                return false;
            }
        }
    }
    for (uint16_t i = 0; i < field.length; i++) {
        value[i] = ((uint32_t)buffer[i * 2] << 16) | buffer[i * 2 + 1];
    }
    return true;
}

bool Inverter::readField64Raw(const ModbusField& field, uint64_t* value) {
    if (!field.readable) return false;
    if (value == nullptr) return false;
    if (field.length == 0 || field.length > INV_MAX_U64_VALUES) return false;

    uint16_t buffer[INV_MAX_U64_VALUES * 4];

    if (field.length == 1) {
        if (!readHoldingRegister(field.address, buffer, 4)) return false;
    } 
    else if (field.stride == 4) {
        if (!readHoldingRegister(field.address, buffer, field.length * 4)) return false;
    } 
    else {
        for (uint16_t i = 0; i < field.length; i++) {
            if (!readHoldingRegister(field.address + i * field.stride, &buffer[i * 4], 4)) {
                return false;
            }
        }
    }
    for (uint16_t i = 0; i < field.length; i++) {
        value[i] = ((uint64_t)buffer[i * 4] << 48) | ((uint64_t)buffer[i * 4 + 1] << 32) | ((uint64_t)buffer[i * 4 + 2] << 16) | buffer[i * 4 + 3];
    }
    return true;
}

bool Inverter::writeField(const ModbusField& field, float value) {
    return writeField(field, &value, 1);
}

bool Inverter::writeField(const ModbusField& field, uint16_t value){
    return writeField(field, &value, 1);
}

bool Inverter::writeField(const ModbusField& field, uint32_t value){
    return writeField(field, &value, 1);
}

bool Inverter::writeField(const ModbusField& field, int16_t value){
    return writeField(field, &value, 1);
}

bool Inverter::writeField(const ModbusField& field, int32_t value){
    return writeField(field, &value, 1);
}

bool Inverter::writeField(const ModbusField& field, float* value, uint8_t count) {
    if (!field.writable) return false;
    if (value == nullptr) return false;
    if (count == 0) return false;
    if (field.scale == 0.0f) return false;

    switch (field.type) {

        case U16: {
            if (count > INV_MAX_U16_VALUES) return false;
            uint16_t raw [INV_MAX_U16_VALUES];
            
            for (uint8_t i = 0; i < count; i++) { 
                raw[i] = (uint16_t)roundf(value[i] / field.scale);
            }

            return writeField(field, raw, count);
        }

        case U32: {
            if (count > INV_MAX_U32_VALUES) return false;
            uint32_t raw [INV_MAX_U32_VALUES];
            
            for (uint8_t i = 0; i < count; i++) { 
                raw[i] = (uint32_t)roundf(value[i] / field.scale);
            }

            return writeField(field, raw, count);
        }

        case I16: {
            if (count > INV_MAX_U16_VALUES) return false;
            int16_t raw [INV_MAX_U16_VALUES];
            
            for (uint8_t i = 0; i < count; i++) { 
                raw[i] = (int16_t)roundf(value[i] / field.scale);
            }

            return writeField(field, raw, count);
        }

        case I32: {
            if (count > INV_MAX_U32_VALUES) return false;
            int32_t raw [INV_MAX_U32_VALUES];
            
            for (uint8_t i = 0; i < count; i++) { 
                raw[i] = (int32_t)roundf(value[i] / field.scale);
            }

            return writeField(field, raw, count);
        }

        case FLOAT32: {
            if (count > INV_MAX_FLOAT_VALUES) return false;
            // Converter para 32 bits
            uint32_t raw [INV_MAX_FLOAT_VALUES];

            for (uint8_t i = 0; i < count; i++) {
                memcpy(&raw[i], &value[i], sizeof(uint32_t));
            }

            return writeField32Raw(field, raw, count);
        }
    }
}

bool Inverter::writeField(const ModbusField& field, uint16_t* value, uint8_t count) {
    if (field.type != U16 || !field.writable) return false;
    if (value == nullptr) return false;
    if (count == 0 || count > INV_MAX_U16_VALUES) return false;
    return writeField16Raw(field, value, count);
}   

bool Inverter::writeField(const ModbusField& field, uint32_t* value, uint8_t count) {
    if (field.type != U32 || !field.writable) return false;
    if (value == nullptr) return false;
    if (count == 0 || count > INV_MAX_U32_VALUES) return false;
    return writeField32Raw(field, value, count);
}  

bool Inverter::writeField(const ModbusField& field, int16_t* value, uint8_t count) {
    if (field.type != I16 || !field.writable) return false;
    if (value == nullptr) return false;
    if (count == 0 || count > INV_MAX_U16_VALUES) return false;

    uint16_t raw [INV_MAX_U16_VALUES];

    for (uint8_t i = 0; i < count; i++) {
        raw[i] = (uint16_t)value[i];
    }

    return writeField16Raw(field, raw, count);
}    

bool Inverter::writeField(const ModbusField& field, int32_t* value, uint8_t count) {
    if (field.type != I32 || !field.writable) return false;
    if (value == nullptr) return false;
    if (count == 0 || count > INV_MAX_U32_VALUES) return false;

    uint32_t raw [INV_MAX_U32_VALUES];

    for (uint8_t i = 0; i < count; i++) {
        raw[i] = (uint32_t)value[i];
    }

    return writeField32Raw(field, raw, count);
}

bool Inverter::writeField16Raw(const ModbusField& field, uint16_t* value, uint8_t count) {
    if (value == nullptr) return false;
    if (count == 0 || count > INV_MAX_U16_VALUES) return false;
    if (count == 1 || field.stride == 1) {
        return writeHoldingRegister(field.address, value, count);
    } 

    for (uint16_t i = 0; i < count; i++) {
        if (!writeHoldingRegister(field.address + i * field.stride, &value[i], 1)) {
            return false;
        }
    }
    return true;
}

bool Inverter::writeField32Raw(const ModbusField& field, uint32_t* value, uint8_t count) { 
    if (value == nullptr) return false;
    if (count == 0 || count > INV_MAX_U32_VALUES) return false;

    uint16_t buffer[INV_MAX_U32_VALUES * 2]; // 2 registradores de 16 bits por valor de 32 bits

    if (count == 1 || field.stride == 2) {
        for (uint16_t i = 0; i < count; i++) {
            buffer[i * 2] = (uint16_t)(value[i] >> 16);
            buffer[i * 2 + 1] = (uint16_t)(value[i] & 0xFFFF);
        }
        return writeHoldingRegister(field.address, buffer, count * 2);
    } 
    for (uint16_t i = 0; i < count; i++) {
        uint16_t high = (uint16_t)(value[i] >> 16);
        uint16_t low = (uint16_t)(value[i] & 0xFFFF);

        if (!writeHoldingRegister(field.address + i * field.stride, &high, 1)) return false;
        if (!writeHoldingRegister(field.address + i * field.stride + 1, &low, 1)) return false;
    }
    return true;
}

bool Inverter::readHoldingRegister(uint16_t startReg, uint16_t* buffer, uint16_t count) {
    if (_mb == nullptr) return false;
    // Implementar EventCode para essa função

    _mb_done = false;
    _mb_success = false;

    bool requestAccepted = _mb->readHreg(_modbus.getId(), startReg, buffer, count, _mb_cb);

    if (!requestAccepted) {
        return false; // Não conseguiu enviar
    }

    uint32_t start = millis();

    while(!_mb_done && millis() - start < 1000) {
        _mb->task();
        yield();
    }

    if (!_mb_done) {
        return false; // Timeout
    }

    if (!_mb_success) {
        return false; // Erro na resposta
    }

    return true;
}

bool Inverter::writeHoldingRegister(uint16_t startReg, uint16_t* buffer, uint16_t count) {
    if (_mb == nullptr) return false;

    _mb_done = false;
    _mb_success = false;

    bool requestAccepted;

    if (count == 1) {
        requestAccepted = _mb->writeHreg(_modbus.getId(), startReg, buffer[0], _mb_cb);
    } else {
        requestAccepted = _mb->writeHreg(_modbus.getId(), startReg, buffer, count, _mb_cb);
    }

    if (!requestAccepted) {
        return false; // Não conseguiu enviar
    }

    uint32_t start = millis();

    while (!_mb_done && millis() - start < 1000) {
        _mb->task();
        yield();
    }

    if (!_mb_done) {
        return false; // Timeout
    }

    if (!_mb_success) {
        return false; // Erro na resposta
    }

    return true;
}