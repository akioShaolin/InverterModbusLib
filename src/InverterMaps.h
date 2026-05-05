/*
 * InverterModbusLib - Solar Inverter Library for Arduino
 * ------------------------------------------------
 * Modbus RTU (RS485)communication layer for inverter integration
 *
 * Copyright (c) 2026, Pedro Akio Sakuma
 * Licensed under BSD 3-Clause License
 */

#ifndef INVERTER_MAPS_H
#define INVERTER_MAPS_H

#include <Arduino.h>
#include "InverterModels.h"

enum NaNValue {
    NAN_U16 = 0xFFFF,
    NAN_I16 = 0x8000,
    NAN_U32 = 0xFFFFFFFF,
    NAN_I32 = 0x80000000,
    NAN_FLOAT32 = 0x7FC00000, // Representação de NaN em float de 32 bits
    NAN_ASCII = 0xFF // Valor arbitrário para indicar NaN em ASCII
};

struct Datetime {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

// Perfil básico de campos Modbus para inversores, com campos comuns a vários modelos. Campos não aplicáveis ou não disponíveis em um modelo específico devem ser preenchidos com INVALID_FIELD.

enum ModbusDataType {
    NONE,       // Tipo especial para indicar que o campo não é aplicável ou não disponível
    U16,        // Unsigned 16-bit integer
    I16,        // Signed 16-bit integer
    U32,        // Unsigned 32-bit integer
    I32,        // Signed 32-bit integer
    U64,        // Unsigned 64-bit integer
    I64,        // Signed 64-bit integer
    FLOAT32,    // 32-bit floating point
    ASCII       // String ASCII (cada registrador de 16 bits representa 2 caracteres ASCII)
};

// address - endereço do primeiro registrador do primeiro elemento

// type - tipo de dado (U16, I16, U32, I32, FLOAT32, ASCII)

// length - quantidade de elementos (valores) do campo
// Ex.: 3 FLOAT32 = 3 elementos (cada um com 2 registradores)

// stride - distância, em regs, entre o inicio de um elemento e o próximo
// Ex: 
//   - Dados contínuos: stride = registradores por elemento
//   - Dados intercalados: stride > registradores por elemento

// scale - fator de escala para converter o valor lido do Modbus para a unidade correta (exceto ASCII)
enum FieldAccessMode {
    FIELD_SIMPLE,       // Address / length / stride resolvem
    FIELD_SPECIAL       // Requer lógica personalizada para leitura/escrita
};

enum HandlerId {
    DEFAULT_HANDLER,
    GOODWE_HANDLER,
};

struct ModbusField {
    uint16_t address;
    ModbusDataType type;
    uint16_t length; // Número de registradores (16 bits cada)
    uint16_t stride; // Número de registradores entre os campos (para arrays)
    float scale;
    bool readable;
    bool writable;

    FieldAccessMode mode = FIELD_SIMPLE;
    HandlerId handlerId = DEFAULT_HANDLER;
};

constexpr ModbusField INVALID_FIELD = { 0xFFFF, NONE, 0, 0, 1.0f, false, false, FIELD_SIMPLE, DEFAULT_HANDLER };

struct ModbusInverterMap {
    // Identificação
    ModbusField serial;

    // Controle
    ModbusField boot;
    ModbusField shutdown;
    ModbusField enablePowerLimit;
    ModbusField PowerLimit;
    ModbusField PowerLimitPercent;
    ModbusField enableExportLimit;
    ModbusField ExportLimit;
    ModbusField ExportLimitPercent;
    ModbusField enablePowerFactor;
    ModbusField PowerFactorSetpoint;
    ModbusField powerFactorExcitationMode;

    // Tempo
    ModbusField time_year;
    ModbusField time_month;
    ModbusField time_day;
    ModbusField time_hour;
    ModbusField time_minute;
    ModbusField time_second;
    ModbusField time_epoch;

    // Status
    ModbusField totalEnergy;
    ModbusField dailyEnergy;

    ModbusField activePower;
    ModbusField reactivePower;
    ModbusField apparentPower;
    ModbusField powerFactor;

    ModbusField gridVoltage;
    ModbusField gridCurrent;
    ModbusField gridFrequency;

    ModbusField temperature;
    ModbusField insulationResistance;
    ModbusField inverterStatus;
    ModbusField alarm;

    ModbusField stringVoltage;
    ModbusField stringCurrent;
    ModbusField stringPower;

    ModbusField batteryVoltage;
    ModbusField batteryCurrent;
    ModbusField batteryPower;
    ModbusField batterySoC;
    ModbusField batterySoH;

    ModbusField epsVoltage;
    ModbusField epsCurrent;
    ModbusField epsActivePower;
    
};

ModbusInverterMap getInverterMap(InverterModel model);

#endif