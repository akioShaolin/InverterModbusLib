#ifndef INVERTER_DESCRIPTOR_H
#define INVERTER_DESCRIPTOR_H

#include <Arduino.h>

enum InverterPhaseType {
    SINGLE_PHASE,
    THREE_PHASE
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

struct InverterDescriptor {
    InverterPhaseType phaseType;
    InverterTopology topology;
    InverterGridConnection gridConnection;
    uint8_t stringCount; // Número de strings, relevante para inversores do tipo STRING, MICRO, pode ser 0 para outros tipos
    uint16_t maxPowerW; // Potência máxima em watts, pode ser 0 se não for aplicável ou desconhecida
};

const InverterDescriptor* getDescriptor(InverterModel model);

#endif