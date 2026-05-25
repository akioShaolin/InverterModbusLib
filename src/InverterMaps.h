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
    StatusFeature status; // */

    /*
    // Identificação
    ModbusField serialNumber;
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
    */
};

ModbusInverterMap getInverterMap(InverterModel model);

#endif