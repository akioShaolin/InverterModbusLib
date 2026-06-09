/*
 * InverterModbusLib - Solar Inverter Library for Arduino
 * ------------------------------------------------
 * Modbus RTU (RS485)communication layer for inverter integration
 *
 * Copyright (c) 2026, Pedro Akio Sakuma
 * Licensed under BSD 3-Clause License
 */

#include "InverterMaps.h"

constexpr ReactivePowerFeature NO_REACTIVE_POWER_FEATURE = {
    INVALID_FIELD,
    INVALID_FIELD,
    INVALID_FIELD,

    INVALID_FIELD,
    INVALID_FIELD,
    INVALID_FIELD,

    FEATURE_VALUE_NONE,
    FEATURE_VALUE_NONE,

    FEATURE_VALUE_NONE,
    FEATURE_VALUE_NONE,

    false,
    false,
    false,
    false,

    false,
    false,
    false,
    false,

    false,

    false,
    false
};

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
        INVALID_FIELD,                                  // O firmware é obtido pelo Model ID
        { 0x7579, U32, 1, 2, 1.0f, true, false }        // Potência nominal
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
        false,                                          // supportsMode
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
        true,                                           // supportsEnable
        false,                                          // supportsMode
        false,                                          // implicitEnable
        true,                                           // supportsWatts
        true,                                           // supportsPercent
        false,                                          // requiresEnableBeforeWrite
        true,                                           // requiresModeBeforeWrite
    }, {
        INVALID_FIELD,                                  // enablePf
        INVALID_FIELD,                                  // enableFixedReactive
        INVALID_FIELD,                                  // controlMode
        { 0x9CBA, U16, 1, 1, 0.001f, true, true },      // powerFactorSetpoint
        INVALID_FIELD,                                  // excitationMode
        INVALID_FIELD,                                  // fixedReactiveSp
        FEATURE_VALUE_NONE,                             // disablePfValue
        FEATURE_VALUE_NONE,                             // enablePfValue
        FEATURE_VALUE_NONE,                             // disableFixedReactivePowerValue
        FEATURE_VALUE_NONE,                             // enableFixedReactivePowerValue
        false,                                          // supportsEnablePf
        false,                                          // supportsControlModePf
        true,                                           // supportPfSp
        true,                                           // implicitPfSp
        false,                                          // invertedRanges
        false,                                          // supportsEnableFixedReactive
        false,                                          // supportsControlModeFixedReactive
        false,                                          // supportsFixedReactiveSetpoint
        false,                                          // implicitFixedReactiveSetpoint
        false,                                          // supportsExcitationMode
        false,                                          // requiresEnableBeforeWrite
        false                                           // requiresModeBeforeWrite
    }, {
        INVALID_FIELD,                                  // Time Year (não disponível nesse modelo)
        INVALID_FIELD,                                  // Time Month (não disponível nesse modelo)
        INVALID_FIELD,                                  // Time Day (não disponível nesse modelo)
        INVALID_FIELD,                                  // Time Hour (não disponível nesse modelo)
        INVALID_FIELD,                                  // Time Minute (não disponível nesse modelo)
        INVALID_FIELD,                                  // Time Second (não disponível nesse modelo)
        { 0x9C40, U32, 1, 2, 1.0f, true, true },        // Time Epoch
        false,                                          // usesSharedDateTimeRegisters
        true,                                           // supportsEpoch
        false,                                          // yearIsOffsetFrom2000
    }, {
        { 0x7D50, I32, 1, 2, 1.0f, true, false },       // Active Power (W)
        INVALID_FIELD,                                  // Apparent Power (kVA) (não disponível nesse modelo)
        { 0x7D52, I32, 1, 2, 1.0f, true, false },       // Reactive Power (VAr)
        { 0x7D54, I16, 1, 1, 0.001f, true, false },     // Power Factor
    }, {
        { 0x7D45, U16, 3, 1, 0.1f, true, false },       // PhaseVoltage R, S, T (V)
        { 0x7D42, U16, 3, 1, 0.1f, true, false },       // LineVoltage RS, ST, TR (V)
        { 0x7D48, I32, 3, 2, 0.001f, true, false },     // Grid Current R, S, T (A)
        { 0x7D55, U16, 1, 1, 0.01f, true, false },      // Frequency (Hz)
        3,                                              // phaseCount
        true,                                           // supportsPhaseVoltage
        true                                            // supportsLineVoltage
    }, {
        { 0x7D6A, U32, 1, 2, 0.01f, true, false },      // Total Energy (kWh)
        { 0x7D72, U32, 1, 2, 0.01f, true, false },      // Daily Energy (kWh)  
    }, {
        { 0x7D10, I16, 24, 2, 0.1f, true, false },      // String Voltage (V)
        { 0x7D11, I16, 24, 2, 0.01f, true, false},      // String Current (A)
        INVALID_FIELD,                                  // String Power (W) (não disponível nesse modelo)
        { 0x7577, U16, 1, 1, 1.0f, true, false },       // Number of PV Strings
        { 0x7578, U16, 1, 1, 1.0f, true, false },       // Number of MPP trackers
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

bool getMap_Weg(InverterModel model, ModbusInverterMap& out) {
    
    switch (model) {
        case SIW500H_ST030_M3:
            memcpy_P(&out, &map_SIW500H_M3, sizeof(ModbusInverterMap));
            return true;

        default:
            memset(&out, 0, sizeof(ModbusInverterMap)); // Retorna um mapa vazio para modelos não mapeados
            return false;
    }
}

