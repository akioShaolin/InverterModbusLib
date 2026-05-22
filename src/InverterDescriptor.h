/*
 * InverterModbusLib - Solar Inverter Library for Arduino
 * ------------------------------------------------
 * Modbus RTU (RS485)communication layer for inverter integration
 *
 * Copyright (c) 2026, Pedro Akio Sakuma
 * Licensed under BSD 3-Clause License
 */

#ifndef INVERTER_DESCRIPTOR_H
#define INVERTER_DESCRIPTOR_H

#include <Arduino.h>
#include "InverterModels.h"
#include "ModbusConfig.h"

constexpr uint8_t MAX_STRINGS = 28;
constexpr uint8_t MAX_BATTERIES = 12;

struct ModbusConfigData {
    uint8_t id;
    uint32_t baud;
    SerialConfig serialConfig;
    int8_t deRePin = -1;
};

enum InverterPhaseType {
    NO_PHASED = 0x00,
    SINGLE_PHASE = 0x01,
    THREE_PHASE = 0x03
};

enum EpsPhaseType {
    NO_EPS = 0x00,
    EPS_SINGLE_PHASE = 0x01,
    EPS_THREE_PHASE = 0x03
};

enum InverterTopology {
    STRING,
    CENTRAL,
    MICRO
};

enum InverterGridConnection {
    ON_GRID,
    OFF_GRID,
    HYBRID
};

struct PVInfo {
    uint8_t stringCount;    // Número de strings, relevante para inversores do tipo STRING, MICRO, pode ser 0 para outros tipos
    uint8_t mpptCount;      // Número de mppts, relevante para inversores do tipo STRING, MICRO, pode ser 0 para outros tipos
    uint8_t stringToMppt[MAX_STRINGS];  // Distribuição das Strings pelos MPPTs
};

struct BatteryInfo {
    uint8_t batteryCount;    // Número de baterias, relevante para inversores do tipo Hibrido e Offgrid, pode ser 0 para outros tipos
    uint8_t portCount;      // Número de entradas, relevante para inversores do tipo Hibrido e Offgrid, pode ser 0 para outros tipos
    uint8_t batteryToPort[MAX_BATTERIES];  // Distribuição das Baterias pelas Entradas
};
    
struct BootMode {
    uint16_t bootValue;
    uint16_t shutdownValue;
};

struct PowerLimitMode {
    uint16_t powerLimitEnable;
    uint16_t powerLimitDisable;
};

struct ExportLimitMode {
    uint16_t exportLimitEnable;
    uint16_t exportLimitDisable;
};

enum class AlarmFormat {
    UNKNOWN,
    ENUM_CODE,
    BITFIELD16,
    BITFIELD32
};

enum class StatusFormat {
    UNKNOWN,
    ENUM_CODE,
    BITFIELD16,
    BITFIELD32
};

enum ProtocolFamily {
    PROTOCOL_DEFAULT,
    PROTOCOL_GOODWE_COMPAT,
    PROTOCOL_HUAWEI_COMPAT,
    PROTOCOL_FOXESS_COMPAT,
    PROTOCOL_CHINT_COMPAT,
    PROTOCOL_WEG_COMPAT
};

constexpr ModbusConfigData defaultModbusConfig PROGMEM {1, 9600, SERIAL_8N1};
constexpr PowerLimitMode defaultPowerLimitMode PROGMEM = {0x00, 0x01};
constexpr ExportLimitMode defaultExportLimitMode PROGMEM = {0x01, 0x00};
constexpr BootMode defaultBootMode PROGMEM = {0x01, 0x00};
constexpr BatteryInfo noBattery PROGMEM = {0, 0, {0}};

struct InverterDescriptor {
    // Identificação elétrica do equipamento
    InverterTopology topology;
    InverterGridConnection gridConnection;
    InverterPhaseType inverterPhaseType;
    EpsPhaseType epsPhaseType;
    uint32_t nominalPowerW; // Potência máxima em watts, pode ser 0 se não for aplicável ou desconhecida

    // Comunicação
    const ModbusConfigData* config;
    ProtocolFamily protocolFamily;

    // Recursos disponíveis
    PVInfo pvInfo;
    BatteryInfo batteryInfo;
    const BootMode* bootMode;    
    const PowerLimitMode* powerLimitMode;
    const ExportLimitMode* exportLimitMode;
    
    // Formatos de interpretação de alarmes e status
    AlarmFormat alarmFormat;
    StatusFormat statusFormat;
};

const InverterDescriptor getDescriptor(InverterModel model);
const InverterDescriptor getDescriptor_Weg(InverterModel model);
const InverterDescriptor getDescriptor_FoxEss(InverterModel model);
const InverterDescriptor getDescriptor_GoodWe(InverterModel model);
const InverterDescriptor getDescriptor_Huawei(InverterModel model);

#endif