/*
 * InverterModbusLib - Solar Inverter Library for Arduino
 * ------------------------------------------------
 * Modbus RTU (RS485)communication layer for inverter integration
 *
 * Copyright (c) 2026, Pedro Akio Sakuma
 * Licensed under BSD 3-Clause License
 */

#ifndef INVERTER_FEATURES_H
#define INVERTER_FEATURES_H

#include <Arduino.h>
#include "ModbusField.h"

// Organização das Features

struct IdentificationFeature {
    ModbusField serialNumber;
    ModbusField modelId;
    ModbusField modelName;
    ModbusField firmwareVersion;
};

struct ControlFeature {
    ModbusField boot;
    ModbusField shutdown;

    uint16_t bootValue;
    uint16_t shutdownValue;

    bool sharedBootRegister;
};

struct ActivePowerFeature {
    ModbusField enable;
    ModbusField mode;
    ModbusField watts;
    ModbusField percent;

    uint16_t disableValue;
    uint16_t enableValue;
    uint16_t wattsModeValue;
    uint16_t percentModeValue;

    bool supportsEnable;
    bool implicitEnable;
    bool supportsWatts;
    bool supportsPercent;
    bool requiresEnableBeforeWrite;
    bool requiresModeBeforeWrite;
};

struct ExportLimitFeature {
    ModbusField enable;
    ModbusField mode;
    ModbusField watts;
    ModbusField percent;

    uint16_t disableValue;
    uint16_t enableValue;
    uint16_t wattsModeValue;
    uint16_t percentModeValue;

    bool supportsEnable;
    bool implicitEnable;
    bool supportsWatts;
    bool supportsPercent;
    bool requiresEnableBeforeWrite;
    bool requiresModeBeforeWrite;
};

struct ReactivePowerFeature {
    // Enable simples, quando existir
    ModbusField enablePf;
    ModbusField enableFixedReactive;

    // Modo enum compartilhado, quando existir
    ModbusField controlMode;

    // Setpoints
    ModbusField pfSetpoint;
    ModbusField excitationMode;
    ModbusField fixedReactiveSp;

    // Valores para PF
    uint16_t disablePfValue;
    uint16_t enablePfValue;             // usado no enablePf ou no controlMode

    // Valores para reativo fixo
    uint16_t disableFixedReactiveValue;    
    uint16_t enableFixedReactiveValue;  // usado no enableFixedReactive ou no controlMode

    // Suporte a fator de potencia
    bool supportsEnablePf;
    bool supportsControlModePf;
    bool supportsPfSp;
    bool implicitPfSp;

    // Suporte a reativo fixo
    bool supportsEnableFixedReactive;
    bool supportsControlModeFixedReactive;
    bool supportsFixedReactiveSp;
    bool implicitFixedReactiveSp;

    // Excitação
    bool supportsExcitationMode;

    // Comportamento de escrita
    bool requiresEnableBeforeWrite;
    bool requiresModeBeforeWrite;

    // Sp = Setpoint
};

struct TimeFeature {
    ModbusField year;
    ModbusField month;
    ModbusField day;
    ModbusField hour;
    ModbusField minute;
    ModbusField second;
    ModbusField epoch;

    bool supportsSeparatedFields;
    bool supportsEpoch;
    bool yearIsOffsetFrom2000;
    bool usesBCD;
};

struct PowerMeasurementFeature {
    ModbusField activePower;
    ModbusField reactivePower;
    ModbusField apparentPower;
    ModbusField powerFactor;
};

struct GridFeature {
    ModbusField phaseVoltage;
    ModbusField lineVoltage;
    ModbusField current;
    ModbusField frequency;

    uint8_t phaseCount;

    bool supportsPhaseVoltage;
    bool supportsLineVoltage;
};

struct EnergyFeature {
    ModbusField total;
    ModbusField daily;
};

struct PvStringFeature {
    ModbusField voltage;
    ModbusField current;
    ModbusField power;

    ModbusField stringCount;
    ModbusField MpptCount;

    bool stringCountFromDescriptor;
    bool mpptCountFromDescriptor;
};

struct BatteryFeature {
    ModbusField voltage;
    ModbusField current;
    ModbusField power;
    ModbusField soc;
    ModbusField soh;

    uint8_t batteryCount;
    uint8_t portCount;
};

struct EpsFeature {
    ModbusField voltage;
    ModbusField current;
    ModbusField activePower;

    uint8_t phaseCount;
};

struct DiagnosticFeature {
    ModbusField temperature;
    ModbusField insulationResistance;
};

struct StatusFeature {
    ModbusField inverterStatus;
    ModbusField alarm;

    //AlarmFormat alarmFormat;
    //StatusFormat statusFormat;
};

#endif