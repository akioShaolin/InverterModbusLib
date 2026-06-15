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
#include "InverterFeatures.h"
#include "ModbusField.h"

struct Datetime {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

constexpr uint16_t FEATURE_VALUE_NONE = 0xFFFF;

struct ModbusInverterMap {

    IdentificationFeature identification;
    ControlFeature control;

    ActivePowerFeature activePower;
    ExportLimitFeature exportLimit;
    ReactivePowerFeature reactivePowerControl;

    TimeFeature time;

    PowerMeasurementFeature power;
    GridFeature grid;
    EnergyFeature energy;
    PvStringFeature pvString;

    BatteryFeature battery;
    EpsFeature eps;

    DiagnosticFeature diagnostic;
    StatusFeature status;
};

bool getInverterMap(InverterModel model, ModbusInverterMap& out);
bool getMap_Weg(InverterModel model, ModbusInverterMap& out);
bool getMap_GoodWe(InverterModel model, ModbusInverterMap& out)

#endif