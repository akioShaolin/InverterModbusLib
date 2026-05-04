/*
 * inverterLib - Solar Inverter Library for Arduino
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

struct InverterDescriptor {
    // Identificação elétrica do equipamento
    InverterTopology topology;
    InverterGridConnection gridConnection;
    InverterPhaseType inverterPhaseType;
    EpsPhaseType epsPhaseType;
    uint32_t nominalPowerW; // Potência máxima em watts, pode ser 0 se não for aplicável ou desconhecida

    // Comunicação
    const ModbusConfig* config;

    // Recursos disponíveis
    PVInfo pvInfo;
    BatteryInfo batteryInfo;
    const BootMode* bootMode;    
    const ExportLimitMode* exportLimitMode;
    
    // Formatos de interpretação de alarmes e status
    AlarmFormat alarmFormat;
    StatusFormat statusFormat;
};

InverterDescriptor getDescriptor(InverterModel model);

#endif