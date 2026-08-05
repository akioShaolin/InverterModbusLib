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

static const ModbusInverterMap map_SIW400G_T PROGMEM = {
    // identification
    {
        { 0x9C74, ASCII, 16, 1, 1.0f, true, false },    // Serial. Scale é ignorado para ASCII; usar 1.0f apenas como valor neutro
        { 0x9C42, U16, 1, 1, 1.0f, true, false },       // Model ID
        { 0x9C54, ASCII, 16, 1, 1.0f, true, false },    // Model Name
        { 0x9C6C, ASCII, 8, 1, 1.0f, true, false },     // Firmware Version
        { 0x9D23, U16, 1, 1, 100.0f, true, false },     // Rated Power
        INVALID_FIELD,                                  // Number os PV Strings
        INVALID_FIELD                                   // Number of MPP trackers
    }, {
        { 0xA716, U16, 1, 1, 1.0f, true, true },        // Boot
        INVALID_FIELD,                                  // Shutdown (compartilhado com o Boot)
        0xCF,                                           // bootValue
        0xCE,                                           // shutdownValue
        true                                            // sharedBootRegister
    }, {
        { 0x9D6B, U16, 1, 1, 1.0f, true, true },        // Enable Power Limit (0 - Disable, 1 - Enable)
        INVALID_FIELD,                                  // Mode
        INVALID_FIELD,                                  // PowerLimit (W)
        { 0x9D6C, U16, 1, 1, 0.1f, true, true },        // Set Power Limit Percent (%)
        0x01,                                           // enableValue
        0x00,                                           // disableValue
        FEATURE_VALUE_NONE,                             // wattsModeValue
        FEATURE_VALUE_NONE,                             // percentModeValue
        true,                                           // supportsEnable
        false,                                          // supportsMode
        false,                                          // implicitEnable
        false,                                          // supportsWatts
        true,                                           // supportsPercent
        true,                                           // requiresEnableBeforeWrite
        false                                           // requiresModeBeforeWrite
    }, {
        { 0xA886, U16, 1, 1, 1.0f, true, true },        // Enable Export Limit (55 - Disable, AA - Enable)
        INVALID_FIELD,                                  // Mode
        INVALID_FIELD,                                  // Set Export Limit (W) (não disponível nesse modelo)
        { 0xA888, U16, 1, 1, 0.1f, true, true },        // Set Export Limit Percent(%)
        0xAA,                                           // enableValue
        0x55,                                           // disableValue
        FEATURE_VALUE_NONE,                             // wattsModeValue
        FEATURE_VALUE_NONE,                             // percentModeValue
        true,                                           // supportsEnable
        false,                                          // supportsMode
        false,                                          // implicitEnable
        false,                                          // supportsWatts
        true,                                           // supportsPercent
        true,                                           // requiresEnableBeforeWrite
        false                                           // requiresModeBeforeWrite
    }, {
        { 0x9D6A, U16, 1, 1, 1.0f, true, true },        // Enable Power Factor (0 - Disable, 1 - Enable)
        { 0x9D6D, U16, 1, 1, 1.0f, true, true },        // Enable Fixed Reactive ()
        INVALID_FIELD,                                  // Mode
        { 0x9D73, U16, 1, 1, 0.001f, true, true },      // powerFactorSetpoint
        { 0x9D74, U16, 1, 1, 1.0f, true, true },        // Power Factor Excitation Mode (0 - Inductive, 1 - Capacitive)
        { 0x9D6E, I32, 1, 2, 0.1f, true, true },        // Fixed Reactive
        0x01,                                           // enablePfValue
        0x00,                                           // disablePfValue
        0x01,                                           // enableFixedReactivePowerValue
        0x00,                                           // disableFixedReactivePowerValue
        true,                                           // supportsEnablePf
        false,                                          // supportsControlModePf
        true,                                           // supportPfSp
        false,                                          // implicitPfSp
        false,                                          // invertedRanges
        true,                                           // supportsEnableFixedReactive
        true,                                           // supportsControlModeFixedReactive
        true,                                           // supportsFixedReactiveSetpoint
        false,                                          // implicitFixedReactiveSetpoint
        true,                                           // supportsExcitationMode
        true,                                           // requiresEnableBeforeWrite
        true                                            // requiresModeBeforeWrite
    }, {
        { 0xA710, U16, 1, 1, 1.0f, true, true },        // Time Year
        { 0xA711, U16, 1, 1, 1.0f, true, true },        // Time Month
        { 0xA712, U16, 1, 1, 1.0f, true, true },        // Time Day
        { 0xA713, U16, 1, 1, 1.0f, true, true },        // Time Hour
        { 0xA714, U16, 1, 1, 1.0f, true, true },        // Time Minute
        { 0xA715, U16, 1, 1, 1.0f, true, true },        // Time Second
        INVALID_FIELD,                                  // Time Epoch (não disponível nesse modelo)
        false,                                          // usesSharedDateTimeRegisters
        false,                                          // supportsEpoch
        false                                           // yearIsOffsetFrom2000
    }, {
        { 0x9C8E, I16, 1, 1, 100.0f, true, false },     // Active Power (kW)
        { 0x9C90, I16, 1, 1, 100.0f, true, false },     // Reactive Power (kVAr)
        { 0x9C8F, I16, 1, 1, 100.0f, true, false },     // Apparent Power (kVA)
        { 0x9C91, I16, 1, 1, 0.001f, true, false },     // Power Factor
    }, {
        { 0x9C9B, U16, 3, 3, 0.1f, true, false },       // Phase Voltage R, S, T (V)
        { 0x9C9A, U16, 3, 3, 0.1f, true, false },       // Line Voltage R, S, T (V)
        { 0x9C99, I16, 3, 3, 0.1f, true, false },       // Grid Current R, S, T (A)
        { 0x9C92, U32, 1, 2, 0.01f, true, false },      // Frequency (Hz)
        3,                                              // phaseCount
        true,                                           // supportsPhaseVoltage
        true                                            // supportsLineVoltage                                          
    }, {
        { 0xA2B6, U32, 1, 2, 0.1f, true, false },       // Total Energy (kWh)
        { 0xA2B5, U16, 1, 1, 0.1f, true, false },       // Daily Energy (kWh)  
    }, {
        { 0xA018, U16, 24, 2, 0.1f, true, false },      // String Voltage (V)
        { 0xA019, U16, 24, 2, 0.01f, true, false },     // String Current (A)
        INVALID_FIELD,                                  // String Power (W) (não disponível nesse modelo)
    }, 
    noBattery,
    noEps, {
        { 0x9C98, I16, 1, 1, 0.1f, true, false },       // Temperature (°C)
        { 0xA00D, U16, 1, 1, 1.0f, true, false },       // Insulation Resistance (kΩ)
    }, {
        { 0x9C8A, U16, 1, 1, 1.0f, true, false },       // Inverter Status
        { 0x9C8C, U32, 1, 2, 1.0f, true, false },       // Alarm
    }
};

static const ModbusInverterMap map_SIW500H_M3 PROGMEM = {
    // identification
    {
        { 0x753F, ASCII, 10, 1, 1.0f, true, false },    // Serial. Scale é ignorado para ASCII; usar 1.0f apenas como valor neutro
        { 0x7576, U16, 1, 1, 1.0f, true, false },       // Model ID
        { 0x7530, ASCII, 15, 1, 1.0f, true, false },    // Model Name
        INVALID_FIELD,                                  // O FirmwareVersion é obtido pelo Model ID
        { 0x7579, U32, 1, 2, 1.0f, true, false },       // Potência nominal
        { 0x7577, U16, 1, 1, 1.0f, true, false },       // Number of PV Strings
        { 0x7578, U16, 1, 1, 1.0f, true, false },       // Number of MPP trackers
    }, {
        { 0x9D08, U16, 1, 1, 1.0f, false, true },       // Boot. Write 0
        { 0x9D09, U16, 1, 1, 1.0f, false, true },       // Shutdown. Write 0
        0x00,                                           // bootValue
        0x00,                                           // shutdownValue
        false                                           // sharedBootRegister
    }, {
        INVALID_FIELD,                                  // Enable Power Limit (É habilitado. Somente alterar o power limit)
        INVALID_FIELD,                                  // mode
        { 0x9CBE, U32, 1, 1, 1.0f, true, true },        // Power Limit (W)
        { 0x9CBD, U16, 1, 1, 0.1f, true, true },        // Power Limit Percent (%)
        FEATURE_VALUE_NONE,                             // enableValue
        FEATURE_VALUE_NONE,                             // disableValue
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
        FEATURE_VALUE_NONE,                             // enableValue
        0x00,                                           // disableValue
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
        FEATURE_VALUE_NONE,                             // enablePfValue
        FEATURE_VALUE_NONE,                             // disablePfValue
        FEATURE_VALUE_NONE,                             // enableFixedReactivePowerValue
        FEATURE_VALUE_NONE,                             // disableFixedReactivePowerValue
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
        { 0x7D52, I32, 1, 2, 1.0f, true, false },       // Reactive Power (VAr)
        INVALID_FIELD,                                  // Apparent Power (VA) (não disponível nesse modelo)
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
        INVALID_FIELD                                   // String Power (W) (não disponível nesse modelo)
    }, 
    noBattery,
    noEps, {
        { 0x7D57, I16, 1, 1, 0.1f, true, false },       // Temperature (°C)
        { 0x7D58, U16, 1, 1, 0.001f, true, false }      // Insulation Resistance (kΩ)
    }, {
        { 0x7D59, U16, 1, 1, 1.0f, true, false },       // Inverter Status
        { 0x7D08, U16, 5, 1, 1.0f, true, false }        // Alarm
    }
};

static const ModbusInverterMap map_SIW200 PROGMEM = {
    // identification
    {
        { 0x0200, ASCII, 8, 1, 1.0f, true, false },     // Serial. Scale é ignorado para ASCII; usar 1.0f apenas como valor neutro
        INVALID_FIELD,                                  // Model ID
        INVALID_FIELD,                                  // Model Name
        INVALID_FIELD,                                  // Firmware Version
        INVALID_FIELD,                                  // Rated Power
        INVALID_FIELD,                                  // Number os PV Strings
        INVALID_FIELD                                   // Number of MPP trackers
    }, {
        { 0x0120, U16, 1, 1, 1.0f, false, true },       // Boot
        { 0x0121, U16, 1, 1, 1.0f, false, true },       // Shutdown (compartilhado com o Boot)
        0x00,                                           // bootValue
        0x00,                                           // shutdownValue
        false                                           // sharedBootRegister
    }, {
        INVALID_FIELD,                                  // Enable Power Limit (0 - Disable, 1 - Enable)
        INVALID_FIELD,                                  // Mode
        INVALID_FIELD,                                  // PowerLimit (W)
        { 0x0100, U16, 1, 1, 1.0f, true, true },        // Set Power Limit Percent (%)
        FEATURE_VALUE_NONE,                             // enableValue
        FEATURE_VALUE_NONE,                             // disableValue
        FEATURE_VALUE_NONE,                             // wattsModeValue
        FEATURE_VALUE_NONE,                             // percentModeValue
        false,                                          // supportsEnable
        false,                                          // supportsMode
        true,                                           // implicitEnable
        false,                                          // supportsWatts
        true,                                           // supportsPercent
        false,                                          // requiresEnableBeforeWrite
        false                                           // requiresModeBeforeWrite
    }, {
        { 0x0123, U16, 1, 1, 1.0f, true, true },        // Enable Export Limit (55 - Disable, AA - Enable)
        INVALID_FIELD,                                  // Mode
        INVALID_FIELD,                                  // Set Export Limit (W) (não disponível nesse modelo)
        { 0x0125, U16, 1, 1, 0.1f, true, true },        // Set Export Limit Percent(%)
        0x01,                                           // enableValue
        0x00,                                           // disableValue
        FEATURE_VALUE_NONE,                             // wattsModeValue
        FEATURE_VALUE_NONE,                             // percentModeValue
        true,                                           // supportsEnable
        false,                                          // supportsMode
        false,                                          // implicitEnable
        false,                                          // supportsWatts
        true,                                           // supportsPercent
        true,                                           // requiresEnableBeforeWrite
        false                                           // requiresModeBeforeWrite
    }, {
        INVALID_FIELD,                                  // Enable Power Factor (0 - Disable, 1 - Enable)
        INVALID_FIELD,                                  // Enable Fixed Reactive ()
        INVALID_FIELD,                                  // Mode
        { 0x0101, U16, 1, 1, 0.001f, true, true },      // powerFactorSetpoint
        INVALID_FIELD,                                  // Power Factor Excitation Mode (0 - Inductive, 1 - Capacitive)
        INVALID_FIELD,                                  // Fixed Reactive
        FEATURE_VALUE_NONE,                             // enablePfValue
        FEATURE_VALUE_NONE,                             // disablePfValue
        FEATURE_VALUE_NONE,                             // enableFixedReactivePowerValue
        FEATURE_VALUE_NONE,                             // disableFixedReactivePowerValue
        false,                                          // supportsEnablePf
        false,                                          // supportsControlModePf
        true,                                           // supportPfSp
        true,                                           // implicitPfSp
        true,                                           // invertedRanges
        false,                                          // supportsEnableFixedReactive
        false,                                          // supportsControlModeFixedReactive
        false,                                          // supportsFixedReactiveSetpoint
        false,                                          // implicitFixedReactiveSetpoint
        false,                                          // supportsExcitationMode
        false,                                          // requiresEnableBeforeWrite
        false                                           // requiresModeBeforeWrite
    }, {
        { 0x0010, U16, 1, 1, 1.0f, true, true },        // Time Year
        { 0x0010, U16, 1, 1, 1.0f, true, true },        // Time Month
        { 0x0011, U16, 1, 1, 1.0f, true, true },        // Time Day
        { 0x0011, U16, 1, 1, 1.0f, true, true },        // Time Hour
        { 0x0012, U16, 1, 1, 1.0f, true, true },        // Time Minute
        { 0x0012, U16, 1, 1, 1.0f, true, true },        // Time Second
        INVALID_FIELD,                                  // Time Epoch (não disponível nesse modelo)
        true,                                           // usesSharedDateTimeRegisters
        false,                                          // supportsEpoch
        true                                            // yearIsOffsetFrom2000
    }, {
        { 0x0233, I16, 2, 1, 0.1f, true, false },       // Active Power (kW)
        INVALID_FIELD,                                  // Reactive Power (kVAr)
        INVALID_FIELD,                                  // Apparent Power (kVA)
        INVALID_FIELD,                                  // Power Factor
    }, {
        { 0x022A, U16, 3, 1, 0.1f, true, false },       // Phase Voltage R, S, T (V)
        INVALID_FIELD,                                  // Line Voltage R, S, T (V)
        { 0x022D, I16, 3, 1, 0.1f, true, false },       // Grid Current R, S, T (A)
        { 0x0230, U32, 1, 2, 0.01f, true, false },      // Frequency (Hz)
        3,                                              // phaseCount
        true,                                           // supportsPhaseVoltage
        false                                           // supportsLineVoltage                                          
    }, {
        { 0x0222, U32, 1, 2, 0.1f, true, false },       // Total Energy (kWh)
        { 0x0236, U16, 1, 1, 0.1f, true, false },       // Daily Energy (kWh)  
    }, {
        { 0x0226, U16, 2, 1, 0.1f, true, false },       // String Voltage (V)
        { 0x0228, U16, 2, 1, 0.1f, true, false },       // String Current (A)
        INVALID_FIELD,                                  // String Power (W) (não disponível nesse modelo)
    }, 
    noBattery,
    noEps, {
        { 0x0235, I16, 1, 1, 0.1f, true, false },       // Temperature (°C)
        INVALID_FIELD,                                  // Insulation Resistance (kΩ)
    }, {
        { 0x0234, U16, 1, 1, 1.0f, true, false },       // Inverter Status
        { 0x0220, U32, 1, 2, 1.0f, true, false },       // Alarm
    }
};

bool getMap_Weg(InverterModel model, ModbusInverterMap& out) {
    
    switch (model) {
        case SIW200_M030:
        case SIW200_M050:
        case SIW200_M085:
        case SIW200_M100:
            memcpy_P(&out, &map_SIW200, sizeof(ModbusInverterMap));
            return true;
        case SIW200H_M050_W00:
        case SIW200H_M075_W10:
        case SIW200H_M105_W10:
        case SIW200G_M030_W0:
        case SIW200G_M030_W1:
        case SIW200G_M050_W0:
        case SIW200G_M050_W1:
        case SIW200G_M060_W0:
        case SIW200G_M060_W1:
        case SIW200G_M070_W1:
        case SIW200G_M075_W1:
        case SIW200G_M080_W1:
        case SIW200G_M090_W1:
        case SIW200G_M105_W1:
            return true;
        case SIW300H_M030_W00:
        case SIW300H_M050_W00:
        case SIW300H_M060_W00:
        case SIW300H_M075_W00:
        case SIW300H_M080_W00:
        case SIW300H_M100_W00:
            return true;
        case SIW400_ST075:
            return true;
        case SIW400G_T012_W0:
        case SIW400G_T012_W1:
        case SIW400G_T015_W0:
        case SIW400G_T015_W1:
        case SIW400G_T020_W0:
        case SIW400G_T020_W1:
        case SIW400G_T025_W0:
        case SIW400G_T025_W1:
        case SIW400G_K015_W00:
        case SIW400G_K020_W00:
        case SIW400G_K025_W00:
        case SIW400G_K030_W00:       
        case SIW400G_K037_W00:
        case SIW400G_T050_W00:
        case SIW400G_T060_W00:
        case SIW400G_T075_W0:
        case SIW400G_T075_W01:
        case SIW400G_T100_W0:
        case SIW400H_T015_W10:
        case SIW400H_T030_W10:
            memcpy_P(&out, &map_SIW400G_T, sizeof(ModbusInverterMap));
            return true;
        case SIW420G_K025_W00:
        case SIW420G_K075_W00:
            return true;
        case SIW500G_T100_W0:
        case SIW500G_K050_W00:
        case SIW500G_K075_W00:
        case SIW500G_H250_W0:
        case SIW500H_ST012_M2:
        case SIW500H_ST015_M2:
        case SIW500H_ST020_M2:
        case SIW500H_ST030_M3:
        case SIW500H_ST036_M3:
        case SIW500H_ST040_M3:
        case SIW500H_SK020:
        case SIW500H_ST030:
        case SIW500H_ST036:
        case SIW500H_ST040:
        case SIW500H_ST060:
        case SIW500H_ST100:
        case SIW500H_ST060_HV:
        case SIW500H_ST100_HV:
        case SIW500H_ST200_H0:
        case SIW500H_ST200_H3:
        case SIW500H_T012_W00:
        case SIW500H_T015_W00:
        case SIW500H_T017_W00:
        case SIW500H_T020_W00:
        case SIW500H_T025_W00: 
            memcpy_P(&out, &map_SIW500H_M3, sizeof(ModbusInverterMap));
            return true;

        case SIW600_T020_44:
            return true;
        case SIW610_T018_W0:
        case SIW610_T075_W0:
            return true;
            
        default: {
            out = ModbusInverterMap{}; // Retorna um mapa vazio para modelos não mapeados
            return false;
        }
    }
}

