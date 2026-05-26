/*
 * InverterModbusLib - Solar Inverter Library for Arduino
 * ------------------------------------------------
 * Modbus RTU (RS485)communication layer for inverter integration
 *
 * Copyright (c) 2026, Pedro Akio Sakuma
 * Licensed under BSD 3-Clause License
 */

#include "InverterMaps.h"

constexpr uint16_t FEATURE_VALUE_NONE = 0xFFFF;

constexpr BatteryFeature noBattery = {
    INVALID_FIELD,
    INVALID_FIELD,
    INVALID_FIELD,
    INVALID_FIELD,
    INVALID_FIELD,
    0,
    0
};

constexpr EpsFeature noEps = {
    INVALID_FIELD,
    INVALID_FIELD,
    INVALID_FIELD,
    0
};

static const ModbusInverterMap map_SIW500H_M3 PROGMEM = {
    // identification
    {
        { 0x753F, ASCII, 10, 1, 1.0f, true, false },    // Serial. Scale é ignorado para ASCII; usar 1.0f apenas como valor neutro
        { 0x7576, U16, 1, 1, 1.0f, true, false },       // Model ID
        { 0x7530, ASCII, 15, 1, 1.0f, true, false },    // Model Name
        INVALID_FIELD                                   // O firmware é obtido pelo Model ID
    }, {
        { 0x9D08, U16, 1, 1, 1.0f, false, true },       // Boot. Write 1
        { 0x9D09, U16, 1, 1, 1.0f, false, true },       // Shutdown. Write 1
        0x00,                                           // bootValue
        0x00,                                           // shutdownValue
        false                                           // sharedBootRegister
    }, {
        INVALID_FIELD,                                  // Enable Power Limit (É habilitado. Somente alterar o power limit)
        INVALID_FIELD,                                  // mode
        { 0x9CB8, U16, 1, 1, 0.1f, true, true },        // Set Power Limit (W)
        { 0x9CBD, U16, 1, 1, 0.1f, true, true },        // Set Power Limit Percent (%)
        FEATURE_VALUE_NONE,                             // disableValue
        FEATURE_VALUE_NONE,                             // enableValue
        FEATURE_VALUE_NONE,                             // wattsModeValue
        FEATURE_VALUE_NONE,                             // percentModeValue
        false,                                          // supportsEnable
        true,                                           // implicitEnable
        true,                                           // supportsWatts
        true,                                           // supportsPercent
        false,                                          // requiresEnableBeforeWrite
        false                                           // requiresModeBeforeWrite
    }, {
        INVALID_FIELD,                                  // Enable Export Limit (0-Off, 1-DI, 5-Zero, 6-Power limited, 7-Power limited percent)
        { 0xB937, U16, 1, 1, 1.0f, true, true },        // mode
        { 0xB938, U32, 1, 2, 1.0f, true, true },        // Set Export Limit (W)
        { 0xB93A, U16, 1, 1, 0.1f, true, true },        // Set Export Limit Percent(%)
        0x00,                                           // disableValue
        FEATURE_VALUE_NONE,                             // enableValue
        0x06,                                           // wattsModeValue
        0x07,                                           // percentModeValue
        true,                                           // supportsWatts
        true,                                           // supportsPercent
        false,                                          // requiresEnableBeforeWrite
        true,                                           // requiresModeBeforeWrite
    }, {
        INVALID_FIELD,                                  // enable
        INVALID_FIELD,                                  // controlMode
        { 0x9CBA, U16, 1, 1, 0.001f, true, true },      // powerFactorSetpoint
        INVALID_FIELD,                                  // excitationMode
        { 0x9D04, U16, 1, 1, 1.0f, true, true },        // fixedReactivePower
        FEATURE_VALUE_NONE,                             // disableValue
        FEATURE_VALUE_NONE,                             // enablePowerFactorValue
        FEATURE_VALUE_NONE,                             // enableFixedReactivePowerValue
        true,                                           // supportsPowerFactorSetpoint
        false,                                          // supportsExcitationMode
        true,                                           // supportsFixedReactivePower
        false                                           // requiresEnableBeforeWrite
    }, {
        INVALID_FIELD,                                  // Time Year (não disponível nesse modelo)
        INVALID_FIELD,                                  // Time Month (não disponível nesse modelo)
        INVALID_FIELD,                                  // Time Day (não disponível nesse modelo)
        INVALID_FIELD,                                  // Time Hour (não disponível nesse modelo)
        INVALID_FIELD,                                  // Time Minute (não disponível nesse modelo)
        INVALID_FIELD,                                  // Time Second (não disponível nesse modelo)
        { 0x9C40, U32, 1, 2, 1.0f, true, true },        // Time Epoch
        false,                                          // supportsSeparatedFields
        true,                                           // supportsEpoch
        false,                                          // yearIsOffsetFrom2000
        false                                           // usesBCD
    }, {
        { 0x7D50, I32, 1, 2, 1.0f, true, false },       // Active Power (W)
        INVALID_FIELD,                                  // Apparent Power (kVA) (não disponível nesse modelo)
        { 0x7D52, I32, 1, 2, 1.0f, true, false },       // Reactive Power (VAr)
        { 0x7D54, I16, 1, 1, 0.001f, true, false },     // Power Factor
    }, {
        { 0x7D45, U16, 3, 1, 0.1f, true, false },       // Grid Voltage R, S, T (V)
        { 0x7D48, I32, 3, 2, 0.001f, true, false },     // Grid Current R, S, T (A)
        { 0x7D55, U16, 1, 1, 0.01f, true, false },      // Frequency (Hz)
        3                                               // phaseCount
    }, {
        { 0x7D6A, U32, 1, 2, 0.01f, true, false },      // Total Energy (kWh)
        { 0x7D72, U32, 1, 2, 0.01f, true, false },      // Daily Energy (kWh)  
    }, {
        { 0x7D10, I16, 24, 2, 0.1f, true, false },      // String Voltage (V)
        { 0x7D11, I16, 24, 2, 0.01f, true, false},      // String Current (A)
        INVALID_FIELD,                                  // String Power (W) (não disponível nesse modelo)
        { 0x7576, U16, 1, 1, 1.0f, true, false },       // Number of PV Strings
        { 0x7577, U16, 1, 1, 1.0f, true, false },       // Number of MPP trackers
        false,
        false
    }, 
    noBattery,
    noEps, {
        { 0x7D57, I16, 1, 1, 0.1f, true, false },       // Temperature (°C)
        { 0x7D58, U16, 1, 1, 0.001f, true, false },     // Insulation Resistance (kΩ)
    }, {
        { 0x7D59, U16, 1, 1, 1.0f, true, false },       // Inverter Status
        { 0x7D08, U16, 5, 1, 1.0f, true, false },       // Alarm

    }
};

