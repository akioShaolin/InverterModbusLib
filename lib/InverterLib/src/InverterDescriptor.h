#ifndef INVERTER_DESCRIPTOR_H
#define INVERTER_DESCRIPTOR_H

#include <Arduino.h>
#include "InverterModels.h"

constexpr uint8_t MAX_STRINGS = 30;
constexpr uint8_t MAX_BATTERIES = 12;

enum PhaseType {
    PHASE_NONE = 0x00,
    SINGLE_PHASE = 0x01,
    THREE_PHASE = 0x03
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
    
struct BootInfo {
    uint16_t bootValue = 0x01;        // Valores padrão para boot e shutdown
    uint16_t shutdownValue = 0x00;
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
    PhaseType inverterPhaseType;
    InverterTopology topology;
    InverterGridConnection gridConnection;

    uint32_t nominalPowerW; // Potência máxima em watts, pode ser 0 se não for aplicável ou desconhecida

    BootInfo bootInfo;    
    PVInfo pvInfo;
    BatteryInfo batteryInfo;
    PhaseType EpsPhaseType;

    AlarmFormat alarmFormat;
    StatusFormat statusFormat;
};

InverterDescriptor getDescriptor(InverterModel model);

#endif