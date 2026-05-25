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

constexpr uint16_t FEATURE_VALUE_NONE = 0xFFFF;

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

    bool supportsWatts;
    bool supportsPercent;
    bool requiresEnableBeforeWrite;
    bool requiresModeBeforeWrite;
};

struct ReactivePowerFeature {
    ModbusField enable;
    ModbusField controlMode;

    ModbusField powerFactorSetpoint;
    ModbusField excitationMode;
    ModbusField fixedReactivePowerSetpoint;

    uint16_t disableValue;
    uint16_t enablePowerFactorValue;
    uint16_t enableFixedReactivePowerValue;

    bool supportsPowerFactorSetpoint;
    bool supportsExcitationMode;
    bool supportsFixedReactivePower;
    bool requiresEnableBeforeWrite;
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
    ModbusField voltage;
    ModbusField current;
    ModbusField frequency;

    uint8_t phaseCount;
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

// Declaração de pequenas constantes

#endif