/*
 * InverterModbusLib - Solar Inverter Library for Arduino
 * ------------------------------------------------
 * Modbus RTU (RS485)communication layer for inverter integration
 *
 * Copyright (c) 2026, Pedro Akio Sakuma
 * Licensed under BSD 3-Clause License
 */

#include "InverterMaps.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------- WEG ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------

static const ModbusInverterMap map_SIW200 PROGMEM = {
    // Identificação
    { 0x0200, ASCII, 8, 1, 1.0f, true, false },   // Serial. Scale é ignorado para ASCII; usar 1.0f apenas como valor neutro
    // Controle
    { 0x0120, U16, 1, 1, 1.0f, false, true },     // Boot. Write 0
    { 0x0121, U16, 1, 1, 1.0f, false, true },     // Shutdown. Write 0
    INVALID_FIELD,                                // Enable Power Limit (Não disponível nesse modelo)
    INVALID_FIELD,                                // Set Power Limit (W) (não disponível nesse modelo)
    { 0x0100, U16, 1, 1, 1.0f, true, true },      // Set Power Limit Percent (%)
    { 0x0123, U16, 1, 1, 1.0f, true, true },      // Enable Export Limit (0-Off, 1-On)
    INVALID_FIELD,                                // Set Export Limit (W) (não disponível nesse modelo)
    { 0x0124, U16, 1, 1, 0.1f, true, true },      // Set Export Limit Percent(%)
    INVALID_FIELD,                                // Enable Power Factor (Não disponível nesse modelo)
    { 0x0101, U16, 1, 1, 1.0f, true, true, FIELD_SPECIAL, GOODWE_HANDLER },      // Set Power Factor % [1, 20] LAGGING, [80, 100] LEADING
    INVALID_FIELD,                                // Power Factor Excitation Mode (Não disponível nesse modelo)
    // Tempo
    { 0x0010, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Year MSB
    { 0x0010, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Month LSB
    { 0x0011, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Day MSB
    { 0x0011, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Hour LSB
    { 0x0012, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Minute MSB
    { 0x0012, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Second LSB
    INVALID_FIELD,                                // Time Epoch (não disponível nesse modelo)
    // Status
    { 0x0222, U16, 2, 1, 0.1f, true, false },     // Total Energy (kWh) - MSB em 0x0222 e LSB em 0x0223 
    { 0x0236, U16, 1, 1, 0.1f, true, false },     // Daily Energy (kWh)  

    { 0x0233, U16, 1, 1, 1.0f, true, false },     // Active Power (W)
    INVALID_FIELD,                                // Apparent Power (kVA) (não disponível nesse modelo)
    INVALID_FIELD,                                // Reactive Power (kVAr) (não disponível nesse modelo)
    INVALID_FIELD,                                // Power Factor (não disponível nesse modelo)

    { 0x022A, U16, 3, 1, 0.1f, true, false },     // Grid Voltage R, S, T (V)
    { 0x022D, U16, 3, 1, 0.1f, true, false },     // Grid Current R, S, T (A)
    { 0x022F, U16, 3, 1, 0.01f, true, false },    // Frequency (Hz)

    { 0x0235, U16, 1, 1, 0.1f, true, false },     // Temperature (°C)
    INVALID_FIELD,                                // Insulation Resistance (kΩ)
    { 0x0234, U16, 1, 1, 1.0f, true, false },     // Inverter Status
    { 0x0220, U32, 1, 2, 1.0f, true, false },     // Alarm

    { 0x0226, U16, 2, 1, 0.1f, true, false },    // String Voltage (V)
    { 0x0228, U16, 2, 1, 0.1f, true, false },    // String Current (A)
    INVALID_FIELD,                               // String Power (W) (não disponível nesse modelo)

    INVALID_FIELD,                               // Battery Voltage (V) (não disponível nesse modelo)
    INVALID_FIELD,                               // Battery Current (A) (não disponível nesse modelo)
    INVALID_FIELD,                               // Battery Power (W) (não disponível nesse modelo)
    INVALID_FIELD,                               // Battery SoC (%) (não disponível nesse modelo)
    INVALID_FIELD,                               // Battery SoH (%) (não disponível nesse modelo)
    INVALID_FIELD,                               // EPS Voltage R, S, T (V) (não disponível nesse modelo)
    INVALID_FIELD,                               // EPS Current R, S, T (A) (não disponível nesse modelo)
    INVALID_FIELD                                // EPS Active Power (W) (não disponível nesse modelo)
};

static const ModbusInverterMap map_SIW200H PROGMEM = {};
static const ModbusInverterMap map_SIW200G PROGMEM = {};

static const ModbusInverterMap map_SIW300H PROGMEM = {
    // Identificação
    { 0x753F, ASCII, 10, 1, 1.0f, true, false },  // Serial. Scale é ignorado para ASCII; usar 1.0f apenas como valor neutro
    // Controle
    { 0x9D08, U16, 1, 1, 1.0f, false, true },     // Boot. Write 1
    { 0x9D09, U16, 1, 1, 1.0f, false, true },     // Shutdown. Write 1
    INVALID_FIELD,                                // Enable Power Limit (Não disponível nesse modelo)
    INVALID_FIELD,                                // Set Power Limit (W) (não disponível nesse modelo)
    INVALID_FIELD,                                // Set Power Limit Percent (%)
    { 0xB937, U16, 1, 1, 1.0f, true, true  },     // Enable Export Limit (0-Off, 1-DI, 5-Zero, 6-Power limited, 7-Power limited percent)
    { 0xB938, U32, 1, 2, 0.001f, true, true  },   // Set Export Limit (kW)
    { 0xB93A, U16, 1, 1, 0.1f, true, true  },     // Set Export Limit Percent(%)
    { 0x89E8, U16, 1, 1, 1.0f, true, true },      // Enable Power Factor (Não disponível nesse modelo)
    { 0x9CBA, U16, 1, 1, 0.001f, true, true },    // Set Power Factor
    INVALID_FIELD,                                // Power Factor Excitation Mode (Não disponível nesse modelo)
    // Tempo
    INVALID_FIELD,                                // Time Year (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Month (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Day (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Hour (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Minute (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Second (não disponível nesse modelo)
    { 0x9C40, U32, 1, 2, 1.0f, true, false },     // Time Epoch
    // Status
    { 0x7D6A, U32, 1, 2, 0.01f, true, false },    // Total Energy (kWh) - MSB em 0x0222 e LSB em 0x0223 
    { 0x7D72, U32, 1, 2, 0.01f, true, false },    // Daily Energy (kWh)  

    { 0x7D50, I32, 1, 2, 0.001f, true, false },   // Active Power (W)
    INVALID_FIELD,                                // Apparent Power (kVA) (não disponível nesse modelo)
    { 0x7D52, I32, 1, 2, 0.001f, true, false },   // Reactive Power (kVAr)
    { 0x7D54, I16, 1, 1, 0.001f, true, false },   // Power Factor

    { 0x7D45, U16, 3, 1, 0.1f, true, false },     // Grid Voltage R, S, T (V)
    { 0x7D48, I16, 3, 2, 0.001f, true, false },   // Grid Current R, S, T (A)
    { 0x7D55, U16, 1, 1, 0.01f, true, false },    // Frequency (Hz)

    { 0x7D57, I16, 1, 1, 0.1f, true, false },     // Temperature (°C)
    { 0x7D58, U16, 1, 1, 0.001f, true, false },   // Insulation Resistance (kΩ)
    { 0x7D59, U16, 1, 1, 1.0f, true, false },     // Inverter Status
    { 0x7D08, U16, 5, 1, 1.0f, true, false },     // Alarm

    { 0x7D10, I16, 24, 2, 0.1f, true, false },    // String Voltage (V)
    { 0x7D11, I16, 24, 2, 0.01f, true, false},    // String Current (A)
    INVALID_FIELD,                                // String Power (W) (não disponível nesse modelo)
                             
    { 0x908B, I16, 1, 1, 0.1f, true, false},      // Battery Voltage (V)                
    { 0x909D, I16, 1, 1, 0.1, true, false },      // Battery Current (A) (não disponível nesse modelo)
    { 0x9089, I32, 1, 2, 1.0f, true, false},      // Battery Power (W)
    { 0x908C, I16, 1, 1, 0.1f, true, false},      // Battery SoC (%)
    { 0x9423, U16, 1, 1, 0.1f, true, false},      // Battery SoH (%) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Voltage R, S, T (V) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Current R, S, T (A) (não disponível nesse modelo)
    INVALID_FIELD                                 // EPS Active Power (W) (não disponível nesse modelo)
};

static const ModbusInverterMap map_SIW400 PROGMEM = {
    // Identificação
    { 0x0200, ASCII, 8, 1, 1.0f, true, false },   // Serial. Scale é ignorado para ASCII; usar 1.0f apenas como valor neutro
    // Controle
    { 0x0120, U16, 1, 1, 1.0f, false, true },     // Boot. Write 0
    { 0x0121, U16, 1, 1, 1.0f, false, true },     // Shutdown. Write 0
    INVALID_FIELD,                                // Enable Power Limit (Não disponível nesse modelo)
    INVALID_FIELD,                                // Set Power Limit (W) (não disponível nesse modelo)
    { 0x0100, U16, 1, 1, 1.0f, true, true },      // Set Power Limit Percent (%)
    { 0x0123, U16, 1, 1, 1.0f, true, true },      // Enable Export Limit (0-Off, 1-On)
    INVALID_FIELD,                                // Set Export Limit (W) (não disponível nesse modelo)
    { 0x0124, U16, 1, 1, 0.1f, true, true },      // Set Export Limit Percent(%)
    INVALID_FIELD,                                // Enable Power Factor (Não disponível nesse modelo)
    { 0x0101, U16, 1, 1, 1.0f, true, true, FIELD_SPECIAL, GOODWE_HANDLER },      // Set Power Factor % [1, 20] LAGGING, [80, 100] LEADING
    INVALID_FIELD,                                // Power Factor Excitation Mode (Não disponível nesse modelo)
    // Tempo
    { 0x033B, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Year MSB
    { 0x033B, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Month LSB
    { 0x033C, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Day MSB
    { 0x033C, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Hour LSB
    { 0x033D, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Minute MSB
    { 0x033D, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Second LSB
    INVALID_FIELD,                                // Time Epoch (não disponível nesse modelo)
    // Status
    { 0x0312, U32, 1, 1, 0.1f, true, false },     // Total Energy (kWh) - MSB em 0x0222 e LSB em 0x0223 
    { 0x0320, U16, 1, 1, 0.1f, true, false },     // Daily Energy (kWh)  

    { 0x0352, U32, 1, 1, 1.0f, true, false },     // Active Power (W)
    INVALID_FIELD,                                // Apparent Power (kVA) (não disponível nesse modelo)
    { 0x037D, U32, 1, 1, 0.001f, true, false },   // Reactive Power (kVAr)
    { 0x0376, U16, 1, 1, 0.001f, true, false },   // Power Factor

    { 0x0304, U16, 3, 1, 0.1f, true, false },     // Grid Voltage R, S, T (V)
    { 0x0307, U16, 3, 1, 0.1f, true, false },     // Grid Current R, S, T (A)
    { 0x030A, U16, 3, 1, 0.01f, true, false },    // Frequency (Hz)

    { 0x030F, U16, 1, 1, 0.1f, true, false },     // Temperature (°C)
    { 0x037F, U16, 1, 1, 1.0f, true, false },     // Insulation Resistance (kΩ)
    { 0x030E, U16, 1, 1, 1.0f, true, false },     // Inverter Status
    { 0x0310, U16, 2, 1, 1.0f, true, false },     // Alarm

    { 0x0300, U16, 1, 2, 0.1f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },    // String Voltage (V) Reg 0x0357 e 0x0358 para MPPTs 3 e 4
    { 0x035B, U16, 20, 1, 0.1f, true, false},     // String Current (A) Reg strings 1 a 20
    INVALID_FIELD,                                // String Power (W) (não disponível nesse modelo)

    INVALID_FIELD,                                // Battery Voltage (V) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery Current (A) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery Power (W) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery SoC (%) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery SoH (%) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Voltage R, S, T (V) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Current R, S, T (A) (não disponível nesse modelo)
    INVALID_FIELD                                 // EPS Active Power (W) (não disponível nesse modelo)
};

static const ModbusInverterMap map_SIW400G_T012_T025 PROGMEM = {};

static const ModbusInverterMap map_SIW400G_T050_T100 PROGMEM = {
    // Identificação
    { 0x9C74, ASCII, 16, 1, 1.0f, true, false }, // Serial. Scale é ignorado para ASCII; usar 1.0f apenas como valor neutro
    // Controle
    { 0xA717, U16, 1, 1, 1.0f, true, true },     // Boot. CE - Shutdown, CF - Boot
    INVALID_FIELD,                               // Shutdown (Não disponível nesse modelo)
    { 0x9D6C, U16, 1, 1, 1.0f, true, true },     // Enable Power Limit (0 - Disable, 1 - Enable)
    INVALID_FIELD,                               // Set Power Limit (W) (não disponível nesse modelo)
    { 0x9D6D, U16, 1, 1, 0.1f, true, true },     // Set Power Limit Percent (%)
    { 0xA887, U16, 1, 1, 1.0f, true, true },     // Enable Export Limit (55 - Disable, AA - Enable)
    INVALID_FIELD,                               // Set Export Limit (W) (não disponível nesse modelo)
    { 0xA889, U16, 1, 1, 0.1f, true, true },     // Set Export Limit Percent(%)
    { 0x9D68, U16, 1, 1, 1.0f, true, true },     // Enable Power Factor (0 - Disable, 1 - Enable)
    { 0x9D74, U16, 1, 1, 0.001f, true, true },   // Set Power Factor
    { 0x9D75, U16, 1, 1, 1.0f, true, true },     // Power Factor Excitation Mode (0 - Inductive, 1 - Capacitive)
    // Tempo
    { 0xA710, U16, 1, 1, 1.0f, true, false },     // Time Year
    { 0xA711, U16, 1, 1, 1.0f, true, false },     // Time Month
    { 0xA712, U16, 1, 1, 1.0f, true, false },     // Time Day
    { 0xA713, U16, 1, 1, 1.0f, true, false },     // Time Hour
    { 0xA714, U16, 1, 1, 1.0f, true, false },     // Time Minute
    { 0xA715, U16, 1, 1, 1.0f, true, false },     // Time Second
    INVALID_FIELD,                                // Time Epoch (não disponível nesse modelo)
    // Status
    { 0xA2B7, U32, 1, 2, 0.1f, true, false },     // Total Energy (kWh)
    { 0xA2B6, U16, 1, 1, 0.1f, true, false },     // Daily Energy (kWh)    

    { 0x9C8F, I16, 1, 1, 100.0f, true, false },   // Active Power (kW)
    { 0x9C90, I16, 1, 1, 100.0f, true, false },   // Apparent Power (kVA)
    { 0x9C91, I16, 1, 1, 100.0f, true, false },   // Reactive Power (kVAr)
    { 0x9C92, I16, 1, 1, 0.001f, true, false },   // Power Factor

    { 0x9C9B, U16, 3, 3, 0.1f, true, false },     // Grid Voltage R, S, T (V)
    { 0x9C9A, I16, 3, 3, 0.1f, true, false },     // Grid Current R, S, T (A)
    { 0x9C93, U32, 1, 2, 0.01f, true, false },    // Frequency (Hz)

    { 0x9C99, I16, 1, 1, 0.1f, true, false },     // Temperature (°C)
    { 0xA00E, U16, 1, 1, 1.0f, true, false },     // Insulation Resistance (kΩ)
    { 0x9C8B, U16, 1, 1, 1.0f, true, false },     // Inverter Status
    { 0x9C8D, U32, 1, 2, 1.0f, true, false },     // Alarm

    { 0xA019, U16, 24, 2, 1.0f, true, false },    // String Voltage (V)
    { 0xA01A, U16, 24, 2, 0.01f, true, false },   // String Current (A)
    INVALID_FIELD,                                // String Power (W) (não disponível nesse modelo)

    INVALID_FIELD,                                // Battery Voltage (V) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery Current (A) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery Power (W) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery SoC (%) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery SoH (%) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Voltage R, S, T (V) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Current R, S, T (A) (não disponível nesse modelo)
    INVALID_FIELD                                 // EPS Active Power (W) (não disponível nesse modelo)
};

static const ModbusInverterMap map_SIW400H_W10 PROGMEM = {
    // Identificação
    { 0x9C74, ASCII, 16, 1, 1.0f, true, false }, // Serial. Scale é ignorado para ASCII; usar 1.0f apenas como valor neutro
    // Controle
    { 0xA717, U16, 1, 1, 1.0f, true, true },     // Boot. CE - Shutdown, CF - Boot
    INVALID_FIELD,                               // Shutdown (Não disponível nesse modelo)
    { 0x9D6C, U16, 1, 1, 1.0f, true, true },     // Enable Power Limit (0 - Disable, 1 - Enable)
    INVALID_FIELD,                               // Set Power Limit (W) (não disponível nesse modelo)
    { 0x9D6D, U16, 1, 1, 0.1f, true, true },     // Set Power Limit Percent (%)
    { 0xA887, U16, 1, 1, 1.0f, true, true },     // Enable Export Limit (55 - Disable, AA - Enable)
    { 0xA889, U16, 1, 1, 1.0f, true, true },     // Set Export Limit (kW)
    INVALID_FIELD,                               // Set Export Limit Percent(%) (não disponível nesse modelo)
    { 0x9D68, U16, 1, 1, 1.0f, true, true },     // Enable Power Factor (0 - Disable, 1 - Enable)
    { 0x9D74, U16, 1, 1, 0.001f, true, true },   // Set Power Factor
    { 0x9D75, U16, 1, 1, 1.0f, true, true },     // Power Factor Excitation Mode (0 - Inductive, 1 - Capacitive)
    // Tempo
    { 0xA710, U16, 1, 1, 1.0f, true, false },     // Time Year
    { 0xA711, U16, 1, 1, 1.0f, true, false },     // Time Month
    { 0xA712, U16, 1, 1, 1.0f, true, false },     // Time Day
    { 0xA713, U16, 1, 1, 1.0f, true, false },     // Time Hour
    { 0xA714, U16, 1, 1, 1.0f, true, false },     // Time Minute
    { 0xA715, U16, 1, 1, 1.0f, true, false },     // Time Second
    INVALID_FIELD,                                // Time Epoch (não disponível nesse modelo)
    // Status
    { 0xA2BB, U32, 1, 2, 0.1f, true, false },     // Total Energy (Wh)
    { 0xA2BA, U16, 1, 1, 0.1f, true, false },     // Daily Energy (Wh) (não disponível nesse modelo)    

    { 0xAA6D, I32, 1, 2, 0.001f, true, false },   // Active Power (kW)
    { 0x9C90, I16, 1, 1, 100.0f, true, false },   // Apparent Power (kVA)
    { 0x9C91, I16, 1, 1, 100.0f, true, false },   // Reactive Power (kVAr)
    { 0x9C92, I16, 1, 1, 0.001f, true, false },   // Power Factor

    { 0x9C9B, U16, 3, 3, 0.1f, true, false },     // Grid Voltage R, S, T (V)
    { 0x9C9A, I16, 3, 3, 0.1f, true, false },     // Grid Current R, S, T (A)
    { 0x9C93, U32, 1, 2, 0.01f, true, false },    // Frequency (Hz)

    { 0x9C99, I16, 1, 1, 0.1f, true, false },     // Temperature (°C)
    { 0xA00E, U16, 1, 1, 1.0f, true, false },     // Insulation Resistance (kΩ)
    { 0x9C8B, U16, 1, 1, 1.0f, true, false },     // Inverter Status
    { 0x9C8D, U32, 1, 2, 1.0f, true, false },     // Alarm

    { 0xA019, U16, 24, 2, 1.0f, true, false },    // String Voltage (V)
    { 0xA01A, U16, 24, 2, 0.01f, true, false },   // String Current (A)
    INVALID_FIELD,                                // String Power (W) (não disponível nesse modelo)

    { 0xA3AA, U16, 2, 2671, 0.01f, true, false }, // Battery Voltage (V) (não disponível nesse modelo)
    { 0xA3AB, U16, 2, 2671, 0.01f, true, false }, // Battery Current (A) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery Power (W) (não disponível nesse modelo)
    { 0xA3AC, U16, 2, 2671, 0.01f, true, false }, // Battery SoC (%) (não disponível nesse modelo)
    { 0xA3CB, U16, 2, 2671, 0.01f, true, false }, // Battery SoH (%) (não disponível nesse modelo)
    { 0xB1B5, U16, 3, 1, 0.1f, true, false },     // EPS Voltage R, S, T (V) (não disponível nesse modelo)
    { 0xB1B8, U16, 3, 1, 0.1f, true, false },     // EPS Current R, S, T (A) (não disponível nesse modelo)
    { 0xB1BE, U32, 1, 2, 0.01f, true, false }     // EPS Active Power (W) (não disponível nesse modelo)
};

static const ModbusInverterMap map_SIW420G PROGMEM = {};

static const ModbusInverterMap map_SIW500G PROGMEM = {
    // Identificação
    { 0x753F, ASCII, 10, 1, 1.0f, true, false },   // Serial. Scale é ignorado para ASCII; usar 1.0f apenas como valor neutro
    // Controle
    { 0x9D08, U16, 1, 1, 1.0f, false, true },     // Boot. Write 1
    { 0x9D09, U16, 1, 1, 1.0f, false, true },     // Shutdown. Write 1
    INVALID_FIELD,                                // Enable Power Limit (Não disponível nesse modelo)
    INVALID_FIELD,                                // Set Power Limit (W) (não disponível nesse modelo)
    INVALID_FIELD,                                // Set Power Limit Percent (%)
    { 0xB937, U16, 1, 1, 1.0f, true, true  },     // Enable Export Limit (0-Off, 1-DI, 5-Zero, 6-Power limited, 7-Power limited percent)
    { 0xB938, U32, 1, 2, 0.001f, true, true  },   // Set Export Limit (kW)
    { 0xB93A, U16, 1, 1, 0.1f, true, true  },     // Set Export Limit Percent(%)
    { 0x89E8, U16, 1, 1, 1.0f, true, true },      // Enable Power Factor (Não disponível nesse modelo)
    { 0x9CBA, U16, 1, 1, 0.001f, true, true },    // Set Power Factor
    INVALID_FIELD,                                // Power Factor Excitation Mode (Não disponível nesse modelo)
    // Tempo
    INVALID_FIELD,                                // Time Year (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Month (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Day (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Hour (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Minute (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Second (não disponível nesse modelo)
    { 0x9C40, U32, 1, 2, 1.0f, true, false },     // Time Epoch
    // Status
    { 0x7D6A, U32, 1, 2, 0.01f, true, false },    // Total Energy (kWh) - MSB em 0x0222 e LSB em 0x0223 
    { 0x7D72, U32, 1, 2, 0.01f, true, false },    // Daily Energy (kWh)  

    { 0x7D50, I32, 1, 2, 0.001f, true, false },   // Active Power (W)
    INVALID_FIELD,                                // Apparent Power (kVA) (não disponível nesse modelo)
    { 0x7D52, I32, 1, 2, 0.001f, true, false },   // Reactive Power (kVAr)
    { 0x7D54, I16, 1, 1, 0.001f, true, false },   // Power Factor

    { 0x7D45, U16, 3, 1, 0.1f, true, false },     // Grid Voltage R, S, T (V)
    { 0x7D48, I16, 3, 2, 0.001f, true, false },   // Grid Current R, S, T (A)
    { 0x7D55, U16, 1, 1, 0.01f, true, false },    // Frequency (Hz)

    { 0x7D57, I16, 1, 1, 0.1f, true, false },     // Temperature (°C)
    { 0x7D58, U16, 1, 1, 0.001f, true, false },   // Insulation Resistance (kΩ)
    { 0x7D59, U16, 1, 1, 1.0f, true, false },     // Inverter Status
    { 0x7D08, U16, 5, 1, 1.0f, true, false },     // Alarm

    { 0x7D10, I16, 24, 2, 0.1f, true, false },    // String Voltage (V)
    { 0x7D11, I16, 24, 2, 0.01f, true, false},    // String Current (A)
    INVALID_FIELD,                                // String Power (W) (não disponível nesse modelo)
                             
    INVALID_FIELD,                                // Battery Voltage (V)                
    INVALID_FIELD,                                // Battery Current (A) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery Power (W)
    INVALID_FIELD,                                // Battery SoC (%)
    INVALID_FIELD,                                // Battery SoH (%) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Voltage R, S, T (V) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Current R, S, T (A) (não disponível nesse modelo)
    INVALID_FIELD                                 // EPS Active Power (W) (não disponível nesse modelo)
};

static const ModbusInverterMap map_SIW500H PROGMEM = {
    // Identificação
    { 0x753F, ASCII, 10, 1, 1.0f, true, false },   // Serial. Scale é ignorado para ASCII; usar 1.0f apenas como valor neutro
    // Controle
    { 0x9D08, U16, 1, 1, 1.0f, false, true },     // Boot. Write 1
    { 0x9D09, U16, 1, 1, 1.0f, false, true },     // Shutdown. Write 1
    INVALID_FIELD,                                // Enable Power Limit (Não disponível nesse modelo)
    INVALID_FIELD,                                // Set Power Limit (W) (não disponível nesse modelo)
    INVALID_FIELD,                                // Set Power Limit Percent (%)
    { 0xB937, U16, 1, 1, 1.0f, true, true  },     // Enable Export Limit (0-Off, 1-DI, 5-Zero, 6-Power limited, 7-Power limited percent)
    { 0xB938, U32, 1, 2, 0.001f, true, true  },   // Set Export Limit (kW)
    { 0xB93A, U16, 1, 1, 0.1f, true, true  },     // Set Export Limit Percent(%)
    { 0x89E8, U16, 1, 1, 1.0f, true, true },      // Enable Power Factor (Não disponível nesse modelo)
    { 0x9CBA, U16, 1, 1, 0.001f, true, true },    // Set Power Factor
    INVALID_FIELD,                                // Power Factor Excitation Mode (Não disponível nesse modelo)
    // Tempo
    INVALID_FIELD,                                // Time Year (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Month (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Day (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Hour (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Minute (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Second (não disponível nesse modelo)
    { 0x9C40, U32, 1, 2, 1.0f, true, false },     // Time Epoch
    // Status
    { 0x7D6A, U32, 1, 2, 0.01f, true, false },    // Total Energy (kWh) - MSB em 0x0222 e LSB em 0x0223 
    { 0x7D72, U32, 1, 2, 0.01f, true, false },    // Daily Energy (kWh)  

    { 0x7D50, I32, 1, 2, 0.001f, true, false },   // Active Power (W)
    INVALID_FIELD,                                // Apparent Power (kVA) (não disponível nesse modelo)
    { 0x7D52, I32, 1, 2, 0.001f, true, false },   // Reactive Power (kVAr)
    { 0x7D54, I16, 1, 1, 0.001f, true, false },   // Power Factor

    { 0x7D45, U16, 3, 1, 0.1f, true, false },     // Grid Voltage R, S, T (V)
    { 0x7D48, I16, 3, 2, 0.001f, true, false },   // Grid Current R, S, T (A)
    { 0x7D55, U16, 1, 1, 0.01f, true, false },    // Frequency (Hz)

    { 0x7D57, I16, 1, 1, 0.1f, true, false },     // Temperature (°C)
    { 0x7D58, U16, 1, 1, 0.001f, true, false },   // Insulation Resistance (kΩ)
    { 0x7D59, U16, 1, 1, 1.0f, true, false },     // Inverter Status
    { 0x7D08, U16, 5, 1, 1.0f, true, false },     // Alarm

    { 0x7D10, I16, 24, 2, 0.1f, true, false },    // String Voltage (V)
    { 0x7D11, I16, 24, 2, 0.01f, true, false},    // String Current (A)
    INVALID_FIELD,                                // String Power (W) (não disponível nesse modelo)
                             
    { 0x908B, I16, 1, 1, 0.1f, true, false},      // Battery Voltage (V)                
    { 0x909D, I16, 1, 1, 0.1, true, false },      // Battery Current (A) (não disponível nesse modelo)
    { 0x9089, I32, 1, 2, 1.0f, true, false},      // Battery Power (W)
    { 0x908C, I16, 1, 1, 0.1f, true, false},      // Battery SoC (%)
    { 0x9423, U16, 1, 1, 0.1f, true, false},                                // Battery SoH (%) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Voltage R, S, T (V) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Current R, S, T (A) (não disponível nesse modelo)
    INVALID_FIELD                                 // EPS Active Power (W) (não disponível nesse modelo)
};

static const ModbusInverterMap map_SIW600 PROGMEM = {
    // Identificação
    { 0x072E, U32, 1, 1, 1.0f, true, false },    // Serial. Este modelo tem o serial em número
    // Controle
    { 0x0074, U16, 1, 1, 1.0f, true, true },     // Boot. 0 - Shutdown, 1 - Boot
    INVALID_FIELD,                               // Shutdown (Não disponível nesse modelo)
    INVALID_FIELD,                               // Enable Power Limit (Não disponível nesse modelo)
    INVALID_FIELD,                               // Set Power Limit (W) (Não disponível nesse modelo)
    { 0x0086, FLOAT32, 1, 1, 1.0f, true, true }, // Set Power Limit Percent (%)
    INVALID_FIELD,                               // Enable Export Limit (Não disponível nesse modelo)
    INVALID_FIELD,                               // Set Export Limit (W) (não disponível nesse modelo)
    INVALID_FIELD,                               // Set Export Limit Percent(%) (Não disponível nesse modelo)
    { 0x006E, U16, 1, 1, 1.0f, true, true },     // Enable Power Factor (0 - Disable, 1 - Enable)
    { 0x0070, FLOAT32, 1, 1, 1.0f, true, true }, // Set Power Factor
    { 0x0072, U16, 1, 1, 1.0f, true, true },     // Power Factor Excitation Mode (0 - Inductive, 1 - Capacitive)
    // Tempo
    { 0x0036, U16, 1, 1, 1.0f, true, false },     // Time Year (não disponível nesse modelo)
    { 0x0034, U16, 1, 1, 1.0f, true, false },     // Time Month (não disponível nesse modelo)
    { 0x0032, U16, 1, 1, 1.0f, true, false },     // Time Day (não disponível nesse modelo)
    { 0x0038, U16, 1, 1, 1.0f, true, false },     // Time Hour (não disponível nesse modelo)
    { 0x003A, U16, 1, 1, 1.0f, true, false },     // Time Minute (não disponível nesse modelo)
    { 0x003C, U16, 1, 1, 1.0f, true, false },     // Time Second (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Epoch
    // Status
    { 0x001E, U32, 1, 1, 1.0f, true, false }, // Total Energy (kWh)
    INVALID_FIELD,                                // Daily Energy (Wh) (não disponível nesse modelo)    

    { 0x0008, FLOAT32, 1, 1, 1.0f, true, false }, // Active Power (kW)
    INVALID_FIELD,                                // Apparent Power (kVA)
    { 0x000A, FLOAT32, 1, 1, 1.0f, true, false }, // Reactive Power (kVAr)
    { 0x000C, FLOAT32, 1, 1, 1.0f, true, false }, // Power Factor

    { 0x0050, FLOAT32, 3, 2, 1.0f, true, false }, // Grid Voltage R, S, T (V)
    { 0x0062, FLOAT32, 3, 2, 1.0f, true, false }, // Grid Current R, S, T (A)
    { 0x0006, FLOAT32, 1, 1, 1.0f, true, false }, // Frequency (Hz)

    { 0x0018, FLOAT32, 1, 1, 1.0f, true, false }, // Temperature (°C)
    INVALID_FIELD,                                // Insulation Resistance (kΩ)
    { 0x0022, U16, 1, 1, 1.0f, true, false },     // Inverter Status
    { 0x0026, U16, 1, 1, 1.0f, true, false },     // Alarm

    { 0x005E, FLOAT32, 2, 6, 1.0f, true, false }, // String Voltage (V)
    { 0x005A, FLOAT32, 2, 6, 1.0f, true, false }, // String Current (A)
    { 0x0056, FLOAT32, 2, 6, 1.0f, true, false }, // String Power (W) (não disponível nesse modelo)

    INVALID_FIELD,                                // Battery Voltage (V) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery Current (A) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery Power (W) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery SoC (%) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery SoH (%) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Voltage R, S, T (V) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Current R, S, T (A) (não disponível nesse modelo)
    INVALID_FIELD                                 // EPS Active Power (W) (não disponível nesse modelo)
};

static const ModbusInverterMap map_SIW610 PROGMEM = {
    // Identificação
    { 0x0808, U32, 1, 1, 1.0f, true, false },    // Serial. Este modelo tem o serial em número
    // Controle
    { 0x013D, U16, 1, 1, 1.0f, true, true },     // Boot. 0 - Shutdown, 1 - Boot
    INVALID_FIELD,                               // Shutdown (Não disponível nesse modelo)
    INVALID_FIELD,                               // Enable Power Limit (Não disponível nesse modelo)
    INVALID_FIELD,                               // Set Power Limit (W) (Não disponível nesse modelo)
    { 0x0132, U16, 1, 1, 0.1f, true, true },     // Set Power Limit Percent (%)
    INVALID_FIELD,                               // Enable Export Limit (Não disponível nesse modelo)
    INVALID_FIELD,                               // Set Export Limit (W) (não disponível nesse modelo)
    INVALID_FIELD,                               // Set Export Limit Percent(%) (Não disponível nesse modelo)
    { 0x0134, U16, 1, 1, 1.0f, true, true },     // Enable Power Factor (0 - Disable, 1 - Enable)
    { 0x0136, U16, 1, 1, 0.001f, true, true },   // Set Power Factor
    { 0x0138, U16, 1, 1, 1.0f, true, true },     // Power Factor Excitation Mode (0 - Inductive, 1 - Capacitive)
    // Tempo
    INVALID_FIELD,                                // Time Year (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Month (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Day (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Hour (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Minute (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Second (não disponível nesse modelo)
    { 0x03F0, U32, 1, 1, 1.0f, true, false },     // Time Epoch
    // Status
    { 0x0BE6, U32, 1, 1, 0.00001f, true, false }, // Total Energy (kWh)
    INVALID_FIELD,                                // Daily Energy (Wh) (não disponível nesse modelo)    

    { 0x0026, I32, 1, 1, 0.1f, true, false },     // Active Power (kW)
    INVALID_FIELD,                                // Apparent Power (kVA)
    { 0x0028, I16, 1, 1, 0.1f, true, false },     // Reactive Power (kVAr)
    { 0x002A, I16, 1, 1, 0.001f, true, false },   // Power Factor

    { 0x000C, U32, 3, 4, 0.1f, true, false },     // Grid Voltage R, S, T (V)
    { 0x0018, I32, 3, 4, 0.1f, true, false },     // Grid Current R, S, T (A)
    { 0x0014, U32, 1, 1, 0.01f, true, false },    // Frequency (Hz)

    { 0x07EE, I16, 1, 1, 1.0f, true, false },     // Temperature (°C)
    INVALID_FIELD,                                // Insulation Resistance (kΩ)
    { 0x0016, U16, 1, 1, 1.0f, true, false },     // Inverter Status
    { 0x0898, U16, 2, 1, 1.0f, true, false },     // Alarm

    { 0x0C1E, U16, 6, 100, 1.0f, true, false },   // String Voltage (V)
    { 0x0C20, U16, 6, 100, 1.0f, true, false },   // String Current (A)
    { 0x0C1C, U16, 6, 100, 1.0f, true, false },   // String Power (W) (não disponível nesse modelo)

    INVALID_FIELD,                                // Battery Voltage (V) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery Current (A) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery Power (W) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery SoC (%) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery SoH (%) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Voltage R, S, T (V) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Current R, S, T (A) (não disponível nesse modelo)
    INVALID_FIELD                                 // EPS Active Power (W) (não disponível nesse modelo)
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------- FOXESS -----------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------

static const ModbusInverterMap map_S_G2_series PROGMEM = {};

static const ModbusInverterMap map_F_G2_series PROGMEM = {};

static const ModbusInverterMap map_G_series PROGMEM = {};

static const ModbusInverterMap map_T_G3_series PROGMEM = {};

static const ModbusInverterMap map_H3P_series PROGMEM = {
    // Identificação
    { 0x9C74, ASCII, 16, 1, 1.0f, true, false }, // Serial. Scale é ignorado para ASCII; usar 1.0f apenas como valor neutro
    // Controle
    { 0xA717, U16, 1, 1, 1.0f, true, true },     // Boot. CE - Shutdown, CF - Boot
    INVALID_FIELD,                               // Shutdown (Não disponível nesse modelo)
    { 0x9D6C, U16, 1, 1, 1.0f, true, true },     // Enable Power Limit (0 - Disable, 1 - Enable)
    INVALID_FIELD,                               // Set Power Limit (W) (não disponível nesse modelo)
    { 0x9D6D, U16, 1, 1, 0.1f, true, true },     // Set Power Limit Percent (%)
    { 0xA887, U16, 1, 1, 1.0f, true, true },     // Enable Export Limit (55 - Disable, AA - Enable)
    { 0xA889, U16, 1, 1, 1.0f, true, true },     // Set Export Limit (kW)
    INVALID_FIELD,                               // Set Export Limit Percent(%) (não disponível nesse modelo)
    { 0x9D68, U16, 1, 1, 1.0f, true, true },     // Enable Power Factor (0 - Disable, 1 - Enable)
    { 0x9D74, U16, 1, 1, 0.001f, true, true },   // Set Power Factor
    { 0x9D75, U16, 1, 1, 1.0f, true, true },     // Power Factor Excitation Mode (0 - Inductive, 1 - Capacitive)
    // Tempo
    { 0xA710, U16, 1, 1, 1.0f, true, false },     // Time Year
    { 0xA711, U16, 1, 1, 1.0f, true, false },     // Time Month
    { 0xA712, U16, 1, 1, 1.0f, true, false },     // Time Day
    { 0xA713, U16, 1, 1, 1.0f, true, false },     // Time Hour
    { 0xA714, U16, 1, 1, 1.0f, true, false },     // Time Minute
    { 0xA715, U16, 1, 1, 1.0f, true, false },     // Time Second
    INVALID_FIELD,                                // Time Epoch (não disponível nesse modelo)
    // Status
    { 0xA2BB, U32, 1, 2, 0.1f, true, false },     // Total Energy (Wh)
    { 0xA2BA, U16, 1, 1, 0.1f, true, false },     // Daily Energy (Wh) (não disponível nesse modelo)    

    { 0xAA6D, I32, 1, 2, 0.001f, true, false },   // Active Power (kW)
    { 0x9C90, I16, 1, 1, 100.0f, true, false },   // Apparent Power (kVA)
    { 0x9C91, I16, 1, 1, 100.0f, true, false },   // Reactive Power (kVAr)
    { 0x9C92, I16, 1, 1, 0.001f, true, false },   // Power Factor

    { 0x9C9B, U16, 3, 3, 0.1f, true, false },     // Grid Voltage R, S, T (V)
    { 0x9C9A, I16, 3, 3, 0.1f, true, false },     // Grid Current R, S, T (A)
    { 0x9C93, U32, 1, 2, 0.01f, true, false },    // Frequency (Hz)

    { 0x9C99, I16, 1, 1, 0.1f, true, false },     // Temperature (°C)
    { 0xA00E, U16, 1, 1, 1.0f, true, false },     // Insulation Resistance (kΩ)
    { 0x9C8B, U16, 1, 1, 1.0f, true, false },     // Inverter Status
    { 0x9C8D, U32, 1, 2, 1.0f, true, false },     // Alarm

    { 0xA019, U16, 24, 2, 1.0f, true, false },    // String Voltage (V)
    { 0xA01A, U16, 24, 2, 0.01f, true, false },   // String Current (A)
    INVALID_FIELD,                                // String Power (W) (não disponível nesse modelo)

    { 0xA3AA, U16, 2, 2671, 0.01f, true, false }, // Battery Voltage (V) (não disponível nesse modelo)
    { 0xA3AB, U16, 2, 2671, 0.01f, true, false }, // Battery Current (A) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery Power (W) (não disponível nesse modelo)
    { 0xA3AC, U16, 2, 2671, 0.01f, true, false }, // Battery SoC (%) (não disponível nesse modelo)
    { 0xA3CB, U16, 2, 2671, 0.01f, true, false }, // Battery SoH (%) (não disponível nesse modelo)
    { 0xB1B5, U16, 3, 1, 0.1f, true, false },     // EPS Voltage R, S, T (V) (não disponível nesse modelo)
    { 0xB1B8, U16, 3, 1, 0.1f, true, false },     // EPS Current R, S, T (A) (não disponível nesse modelo)
    { 0xB1BE, U32, 1, 2, 0.01f, true, false }     // EPS Active Power (W) (não disponível nesse modelo)
};

static const ModbusInverterMap map_V_series PROGMEM = {
    // Identificação
    { 0x9C74, ASCII, 16, 1, 1.0f, true, false }, // Serial. Scale é ignorado para ASCII; usar 1.0f apenas como valor neutro
    // Controle
    { 0xA717, U16, 1, 1, 1.0f, true, true },     // Boot. CE - Shutdown, CF - Boot
    INVALID_FIELD,                               // Shutdown (Não disponível nesse modelo)
    { 0x9D6C, U16, 1, 1, 1.0f, true, true },     // Enable Power Limit (0 - Disable, 1 - Enable)
    INVALID_FIELD,                               // Set Power Limit (W) (não disponível nesse modelo)
    { 0x9D6D, U16, 1, 1, 0.1f, true, true },     // Set Power Limit Percent (%)
    { 0xA887, U16, 1, 1, 1.0f, true, true },     // Enable Export Limit (55 - Disable, AA - Enable)
    INVALID_FIELD,                               // Set Export Limit (W) (não disponível nesse modelo)
    { 0xA889, U16, 1, 1, 0.1f, true, true },     // Set Export Limit Percent(%)
    { 0x9D68, U16, 1, 1, 1.0f, true, true },     // Enable Power Factor (0 - Disable, 1 - Enable)
    { 0x9D74, U16, 1, 1, 0.001f, true, true },   // Set Power Factor
    { 0x9D75, U16, 1, 1, 1.0f, true, true },     // Power Factor Excitation Mode (0 - Inductive, 1 - Capacitive)
    // Tempo
    { 0xA710, U16, 1, 1, 1.0f, true, false },     // Time Year
    { 0xA711, U16, 1, 1, 1.0f, true, false },     // Time Month
    { 0xA712, U16, 1, 1, 1.0f, true, false },     // Time Day
    { 0xA713, U16, 1, 1, 1.0f, true, false },     // Time Hour
    { 0xA714, U16, 1, 1, 1.0f, true, false },     // Time Minute
    { 0xA715, U16, 1, 1, 1.0f, true, false },     // Time Second
    INVALID_FIELD,                                // Time Epoch (não disponível nesse modelo)
    // Status
    { 0x9C95, U64, 1, 4, 0.1f, true, false },     // Total Energy (kWh)
    INVALID_FIELD,                                // Daily Energy (Wh) (não disponível nesse modelo)    

    { 0x9C8F, I16, 1, 1, 100.0f, true, false },   // Active Power (kW)
    { 0x9C90, I16, 1, 1, 100.0f, true, false },   // Apparent Power (kVA)
    { 0x9C91, I16, 1, 1, 100.0f, true, false },   // Reactive Power (kVAr)
    { 0x9C92, I16, 1, 1, 0.001f, true, false },   // Power Factor

    { 0x9C9B, U16, 3, 3, 0.1f, true, false },     // Grid Voltage R, S, T (V)
    { 0x9C9A, I16, 3, 3, 0.1f, true, false },     // Grid Current R, S, T (A)
    { 0x9C93, U32, 1, 2, 0.01f, true, false },    // Frequency (Hz)

    { 0x9C99, I16, 1, 1, 0.1f, true, false },     // Temperature (°C)
    { 0xA00E, U16, 1, 1, 1.0f, true, false },     // Insulation Resistance (kΩ)
    { 0x9C8B, U16, 1, 1, 1.0f, true, false },     // Inverter Status
    { 0x9C8D, U32, 1, 2, 1.0f, true, false },     // Alarm

    { 0xA019, U16, 18, 2, 1.0f, true, false },    // String Voltage (V)
    { 0xA01A, U16, 18, 2, 0.01f, true, false },   // String Current (A)
    INVALID_FIELD,                                // String Power (W) (não disponível nesse modelo)

    INVALID_FIELD,                                // Battery Voltage (V) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery Current (A) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery Power (W) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery SoC (%) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery SoH (%) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Voltage R, S, T (V) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Current R, S, T (A) (não disponível nesse modelo)
    INVALID_FIELD                                 // EPS Active Power (W) (não disponível nesse modelo)
};

static const ModbusInverterMap map_R_series PROGMEM = {
    // Identificação
    { 0x9C74, ASCII, 16, 1, 1.0f, true, false }, // Serial. Scale é ignorado para ASCII; usar 1.0f apenas como valor neutro
    // Controle
    { 0xA717, U16, 1, 1, 1.0f, true, true },     // Boot. CE - Shutdown, CF - Boot
    INVALID_FIELD,                               // Shutdown (Não disponível nesse modelo)
    { 0x9D6C, U16, 1, 1, 1.0f, true, true },     // Enable Power Limit (0 - Disable, 1 - Enable)
    INVALID_FIELD,                               // Set Power Limit (W) (não disponível nesse modelo)
    { 0x9D6D, U16, 1, 1, 0.1f, true, true },     // Set Power Limit Percent (%)
    { 0xA887, U16, 1, 1, 1.0f, true, true },     // Enable Export Limit (55 - Disable, AA - Enable)
    INVALID_FIELD,                               // Set Export Limit (W) (não disponível nesse modelo)
    { 0xA889, U16, 1, 1, 0.1f, true, true },     // Set Export Limit Percent(%)
    { 0x9D68, U16, 1, 1, 1.0f, true, true },     // Enable Power Factor (0 - Disable, 1 - Enable)
    { 0x9D74, U16, 1, 1, 0.001f, true, true },   // Set Power Factor
    { 0x9D75, U16, 1, 1, 1.0f, true, true },     // Power Factor Excitation Mode (0 - Inductive, 1 - Capacitive)
    // Tempo
    { 0xA710, U16, 1, 1, 1.0f, true, false },     // Time Year
    { 0xA711, U16, 1, 1, 1.0f, true, false },     // Time Month
    { 0xA712, U16, 1, 1, 1.0f, true, false },     // Time Day
    { 0xA713, U16, 1, 1, 1.0f, true, false },     // Time Hour
    { 0xA714, U16, 1, 1, 1.0f, true, false },     // Time Minute
    { 0xA715, U16, 1, 1, 1.0f, true, false },     // Time Second
    INVALID_FIELD,                                // Time Epoch (não disponível nesse modelo)
    // Status
    { 0x9C95, U64, 1, 4, 0.1f, true, false },     // Total Energy (kWh)
    INVALID_FIELD,                                // Daily Energy (Wh) (não disponível nesse modelo)    

    { 0x9C8F, I16, 1, 1, 100.0f, true, false },   // Active Power (kW)
    { 0x9C90, I16, 1, 1, 100.0f, true, false },   // Apparent Power (kVA)
    { 0x9C91, I16, 1, 1, 100.0f, true, false },   // Reactive Power (kVAr)
    { 0x9C92, I16, 1, 1, 0.001f, true, false },   // Power Factor

    { 0x9C9B, U16, 3, 3, 0.1f, true, false },     // Grid Voltage R, S, T (V)
    { 0x9C9A, I16, 3, 3, 0.1f, true, false },     // Grid Current R, S, T (A)
    { 0x9C93, U32, 1, 2, 0.01f, true, false },    // Frequency (Hz)

    { 0x9C99, I16, 1, 1, 0.1f, true, false },     // Temperature (°C)
    { 0xA00E, U16, 1, 1, 1.0f, true, false },     // Insulation Resistance (kΩ)
    { 0x9C8B, U16, 1, 1, 1.0f, true, false },     // Inverter Status
    { 0x9C8D, U32, 1, 2, 1.0f, true, false },     // Alarm

    { 0xA019, U16, 18, 2, 1.0f, true, false },    // String Voltage (V)
    { 0xA01A, U16, 18, 2, 0.01f, true, false },   // String Current (A)
    INVALID_FIELD,                                // String Power (W) (não disponível nesse modelo)

    INVALID_FIELD,                                // Battery Voltage (V) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery Current (A) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery Power (W) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery SoC (%) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery SoH (%) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Voltage R, S, T (V) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Current R, S, T (A) (não disponível nesse modelo)
    INVALID_FIELD                                 // EPS Active Power (W) (não disponível nesse modelo)
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------- GOODWE -----------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------

static const ModbusInverterMap map_GOODWE_NON_MT PROGMEM = {
    // Identificação
    { 0x0200, ASCII, 8, 1, 1.0f, true, false }, // Serial. Scale é ignorado para ASCII; usar 1.0f apenas como valor neutro
    // Controle
    { 0x0120, U16, 1, 1, 1.0f, false, true },     // Boot. Write 0
    { 0x0121, U16, 1, 1, 1.0f, false, true },     // Shutdown. Write 0
    INVALID_FIELD,                                // Enable Power Limit (Não disponível nesse modelo)
    INVALID_FIELD,                                // Set Power Limit (W) (não disponível nesse modelo)
    { 0x0100, U16, 1, 1, 1.0f, true, true },      // Set Power Limit Percent (%)
    { 0x0123, U16, 1, 1, 1.0f, true, true },      // Enable Export Limit (0-Off, 1-On)
    INVALID_FIELD,                                // Set Export Limit (W) (não disponível nesse modelo)
    { 0x0124, U16, 1, 1, 0.1f, true, true },      // Set Export Limit Percent(%)
    INVALID_FIELD,                                // Enable Power Factor (Não disponível nesse modelo)
    { 0x0101, U16, 1, 1, 1.0f, true, true, FIELD_SPECIAL, GOODWE_HANDLER },      // Set Power Factor % [1, 20] LAGGING, [80, 100] LEADING
    INVALID_FIELD,                                // Power Factor Excitation Mode (Não disponível nesse modelo)
    // Tempo
    { 0x0010, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Year MSB
    { 0x0010, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Month LSB
    { 0x0011, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Day MSB
    { 0x0011, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Hour LSB
    { 0x0012, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Minute MSB
    { 0x0012, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Second LSB
    INVALID_FIELD,                                // Time Epoch (não disponível nesse modelo)
    // Status
    { 0x0222, U16, 2, 1, 0.1f, true, false },     // Total Energy (kWh) - MSB em 0x0222 e LSB em 0x0223 
    { 0x0236, U16, 1, 1, 0.1f, true, false },     // Daily Energy (kWh)  

    { 0x0233, U16, 1, 1, 1.0f, true, false },     // Active Power (W)
    INVALID_FIELD,                                // Apparent Power (kVA) (não disponível nesse modelo)
    INVALID_FIELD,                                // Reactive Power (kVAr) (não disponível nesse modelo)
    INVALID_FIELD,                                // Power Factor (não disponível nesse modelo)

    { 0x022A, U16, 3, 1, 0.1f, true, false },     // Grid Voltage R, S, T (V)
    { 0x022D, U16, 3, 1, 0.1f, true, false },     // Grid Current R, S, T (A)
    { 0x022F, U16, 3, 1, 0.01f, true, false },    // Frequency (Hz)

    { 0x0235, U16, 1, 1, 0.1f, true, false },     // Temperature (°C)
    INVALID_FIELD,                                // Insulation Resistance (kΩ)
    { 0x0234, U16, 1, 1, 1.0f, true, false },     // Inverter Status
    { 0x0220, U32, 1, 2, 1.0f, true, false },     // Alarm

    { 0x0226, U16, 2, 1, 0.1f, true, false },    // String Voltage (V)
    { 0x0228, U16, 2, 1, 0.1f, true, false },    // String Current (A)
    INVALID_FIELD,                               // String Power (W) (não disponível nesse modelo)

    INVALID_FIELD,                               // Battery Voltage (V) (não disponível nesse modelo)
    INVALID_FIELD,                               // Battery Current (A) (não disponível nesse modelo)
    INVALID_FIELD,                               // Battery Power (W) (não disponível nesse modelo)
    INVALID_FIELD,                               // Battery SoC (%) (não disponível nesse modelo)
    INVALID_FIELD,                               // Battery SoH (%) (não disponível nesse modelo)
    INVALID_FIELD,                               // EPS Voltage R, S, T (V) (não disponível nesse modelo)
    INVALID_FIELD,                               // EPS Current R, S, T (A) (não disponível nesse modelo)
    INVALID_FIELD                                // EPS Active Power (W) (não disponível nesse modelo)
};

static const ModbusInverterMap map_GOODWE_MT_ONLY PROGMEM = {
    // Identificação
    { 0x0200, ASCII, 8, 1, 1.0f, true, false },   // Serial. Scale é ignorado para ASCII; usar 1.0f apenas como valor neutro
    // Controle
    { 0x0120, U16, 1, 1, 1.0f, false, true },     // Boot. Write 0
    { 0x0121, U16, 1, 1, 1.0f, false, true },     // Shutdown. Write 0
    INVALID_FIELD,                                // Enable Power Limit (Não disponível nesse modelo)
    INVALID_FIELD,                                // Set Power Limit (W) (não disponível nesse modelo)
    { 0x0100, U16, 1, 1, 1.0f, true, true },      // Set Power Limit Percent (%)
    { 0x0123, U16, 1, 1, 1.0f, true, true },      // Enable Export Limit (0-Off, 1-On)
    INVALID_FIELD,                                // Set Export Limit (W) (não disponível nesse modelo)
    { 0x0124, U16, 1, 1, 0.1f, true, true },      // Set Export Limit Percent(%)
    INVALID_FIELD,                                // Enable Power Factor (Não disponível nesse modelo)
    { 0x0101, U16, 1, 1, 1.0f, true, true, FIELD_SPECIAL, GOODWE_HANDLER },      // Set Power Factor % [1, 20] LAGGING, [80, 100] LEADING
    INVALID_FIELD,                                // Power Factor Excitation Mode (Não disponível nesse modelo)
    // Tempo
    { 0x033B, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Year MSB
    { 0x033B, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Month LSB
    { 0x033C, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Day MSB
    { 0x033C, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Hour LSB
    { 0x033D, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Minute MSB
    { 0x033D, U16, 1, 1, 1.0f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },     // Time Second LSB
    INVALID_FIELD,                                // Time Epoch (não disponível nesse modelo)
    // Status
    { 0x0312, U32, 1, 1, 0.1f, true, false },     // Total Energy (kWh) - MSB em 0x0222 e LSB em 0x0223 
    { 0x0320, U16, 1, 1, 0.1f, true, false },     // Daily Energy (kWh)  

    { 0x0352, U32, 1, 1, 1.0f, true, false },     // Active Power (W)
    INVALID_FIELD,                                // Apparent Power (kVA) (não disponível nesse modelo)
    { 0x037D, U32, 1, 1, 0.001f, true, false },   // Reactive Power (kVAr)
    { 0x0376, U16, 1, 1, 0.001f, true, false },   // Power Factor

    { 0x0304, U16, 3, 1, 0.1f, true, false },     // Grid Voltage R, S, T (V)
    { 0x0307, U16, 3, 1, 0.1f, true, false },     // Grid Current R, S, T (A)
    { 0x030A, U16, 3, 1, 0.01f, true, false },    // Frequency (Hz)

    { 0x030F, U16, 1, 1, 0.1f, true, false },     // Temperature (°C)
    { 0x037F, U16, 1, 1, 1.0f, true, false },     // Insulation Resistance (kΩ)
    { 0x030E, U16, 1, 1, 1.0f, true, false },     // Inverter Status
    { 0x0310, U16, 2, 1, 1.0f, true, false },     // Alarm

    { 0x0300, U16, 1, 2, 0.1f, true, false, FIELD_SPECIAL, GOODWE_HANDLER },    // String Voltage (V) Reg 0x0357 e 0x0358 para MPPTs 3 e 4
    { 0x035B, U16, 20, 1, 0.1f, true, false},     // String Current (A) Reg strings 1 a 20
    INVALID_FIELD,                                // String Power (W) (não disponível nesse modelo)

    INVALID_FIELD,                                // Battery Voltage (V) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery Current (A) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery Power (W) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery SoC (%) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery SoH (%) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Voltage R, S, T (V) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Current R, S, T (A) (não disponível nesse modelo)
    INVALID_FIELD                                 // EPS Active Power (W) (não disponível nesse modelo)
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------- HUAWEI -----------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------

static const ModbusInverterMap map_SUN2000_MONO PROGMEM = {
    // Identificação
    { 0x753F, ASCII, 10, 1, 1.0f, true, false },   // Serial. Scale é ignorado para ASCII; usar 1.0f apenas como valor neutro
    // Controle
    { 0x9D08, U16, 1, 1, 1.0f, false, true },     // Boot. Write 1
    { 0x9D09, U16, 1, 1, 1.0f, false, true },     // Shutdown. Write 1
    INVALID_FIELD,                                // Enable Power Limit (Não disponível nesse modelo)
    INVALID_FIELD,                                // Set Power Limit (W) (não disponível nesse modelo)
    INVALID_FIELD,                                // Set Power Limit Percent (%)
    { 0xB937, U16, 1, 1, 1.0f, true, true  },     // Enable Export Limit (0-Off, 1-DI, 5-Zero, 6-Power limited, 7-Power limited percent)
    { 0xB938, U32, 1, 2, 0.001f, true, true  },   // Set Export Limit (kW)
    { 0xB93A, U16, 1, 1, 0.1f, true, true  },     // Set Export Limit Percent(%)
    { 0x89E8, U16, 1, 1, 1.0f, true, true },      // Enable Power Factor (Não disponível nesse modelo)
    { 0x9CBA, U16, 1, 1, 0.001f, true, true },    // Set Power Factor
    INVALID_FIELD,                                // Power Factor Excitation Mode (Não disponível nesse modelo)
    // Tempo
    INVALID_FIELD,                                // Time Year (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Month (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Day (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Hour (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Minute (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Second (não disponível nesse modelo)
    { 0x9C40, U32, 1, 2, 1.0f, true, false },     // Time Epoch
    // Status
    { 0x7D6A, U32, 1, 2, 0.01f, true, false },    // Total Energy (kWh) - MSB em 0x0222 e LSB em 0x0223 
    { 0x7D72, U32, 1, 2, 0.01f, true, false },    // Daily Energy (kWh)  

    { 0x7D50, I32, 1, 2, 0.001f, true, false },   // Active Power (W)
    INVALID_FIELD,                                // Apparent Power (kVA) (não disponível nesse modelo)
    { 0x7D52, I32, 1, 2, 0.001f, true, false },   // Reactive Power (kVAr)
    { 0x7D54, I16, 1, 1, 0.001f, true, false },   // Power Factor

    { 0x7D45, U16, 3, 1, 0.1f, true, false },     // Grid Voltage R, S, T (V)
    { 0x7D48, I16, 3, 2, 0.001f, true, false },   // Grid Current R, S, T (A)
    { 0x7D55, U16, 1, 1, 0.01f, true, false },    // Frequency (Hz)

    { 0x7D57, I16, 1, 1, 0.1f, true, false },     // Temperature (°C)
    { 0x7D58, U16, 1, 1, 0.001f, true, false },   // Insulation Resistance (kΩ)
    { 0x7D59, U16, 1, 1, 1.0f, true, false },     // Inverter Status
    { 0x7D08, U16, 5, 1, 1.0f, true, false },     // Alarm

    { 0x7D10, I16, 24, 2, 0.1f, true, false },    // String Voltage (V)
    { 0x7D11, I16, 24, 2, 0.01f, true, false},    // String Current (A)
    INVALID_FIELD,                                // String Power (W) (não disponível nesse modelo)
                             
    INVALID_FIELD,                                // Battery Voltage (V)                
    INVALID_FIELD,                                // Battery Current (A) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery Power (W)
    INVALID_FIELD,                                // Battery SoC (%)
    INVALID_FIELD,                                // Battery SoH (%) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Voltage R, S, T (V) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Current R, S, T (A) (não disponível nesse modelo)
    INVALID_FIELD                                 // EPS Active Power (W) (não disponível nesse modelo)
};

static const ModbusInverterMap map_SUN2000_TRIF PROGMEM = {
    // Identificação
    { 0x753F, ASCII, 10, 1, 1.0f, true, false },   // Serial. Scale é ignorado para ASCII; usar 1.0f apenas como valor neutro
    // Controle
    { 0x9D08, U16, 1, 1, 1.0f, false, true },     // Boot. Write 1
    { 0x9D09, U16, 1, 1, 1.0f, false, true },     // Shutdown. Write 1
    INVALID_FIELD,                                // Enable Power Limit (Não disponível nesse modelo)
    INVALID_FIELD,                                // Set Power Limit (W) (não disponível nesse modelo)
    INVALID_FIELD,                                // Set Power Limit Percent (%)
    { 0xB937, U16, 1, 1, 1.0f, true, true  },     // Enable Export Limit (0-Off, 1-DI, 5-Zero, 6-Power limited, 7-Power limited percent)
    { 0xB938, U32, 1, 2, 0.001f, true, true  },   // Set Export Limit (kW)
    { 0xB93A, U16, 1, 1, 0.1f, true, true  },     // Set Export Limit Percent(%)
    { 0x89E8, U16, 1, 1, 1.0f, true, true },      // Enable Power Factor (Não disponível nesse modelo)
    { 0x9CBA, U16, 1, 1, 0.001f, true, true },    // Set Power Factor
    INVALID_FIELD,                                // Power Factor Excitation Mode (Não disponível nesse modelo)
    // Tempo
    INVALID_FIELD,                                // Time Year (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Month (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Day (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Hour (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Minute (não disponível nesse modelo)
    INVALID_FIELD,                                // Time Second (não disponível nesse modelo)
    { 0x9C40, U32, 1, 2, 1.0f, true, false },     // Time Epoch
    // Status
    { 0x7D6A, U32, 1, 2, 0.01f, true, false },    // Total Energy (kWh) - MSB em 0x0222 e LSB em 0x0223 
    { 0x7D72, U32, 1, 2, 0.01f, true, false },    // Daily Energy (kWh)  

    { 0x7D50, I32, 1, 2, 0.001f, true, false },   // Active Power (W)
    INVALID_FIELD,                                // Apparent Power (kVA) (não disponível nesse modelo)
    { 0x7D52, I32, 1, 2, 0.001f, true, false },   // Reactive Power (kVAr)
    { 0x7D54, I16, 1, 1, 0.001f, true, false },   // Power Factor

    { 0x7D45, U16, 3, 1, 0.1f, true, false },     // Grid Voltage R, S, T (V)
    { 0x7D48, I16, 3, 2, 0.001f, true, false },   // Grid Current R, S, T (A)
    { 0x7D55, U16, 1, 1, 0.01f, true, false },    // Frequency (Hz)

    { 0x7D57, I16, 1, 1, 0.1f, true, false },     // Temperature (°C)
    { 0x7D58, U16, 1, 1, 0.001f, true, false },   // Insulation Resistance (kΩ)
    { 0x7D59, U16, 1, 1, 1.0f, true, false },     // Inverter Status
    { 0x7D08, U16, 5, 1, 1.0f, true, false },     // Alarm

    { 0x7D10, I16, 24, 2, 0.1f, true, false },    // String Voltage (V)
    { 0x7D11, I16, 24, 2, 0.01f, true, false},    // String Current (A)
    INVALID_FIELD,                                // String Power (W) (não disponível nesse modelo)
                             
    INVALID_FIELD,                                // Battery Voltage (V)                
    INVALID_FIELD,                                // Battery Current (A) (não disponível nesse modelo)
    INVALID_FIELD,                                // Battery Power (W)
    INVALID_FIELD,                                // Battery SoC (%)
    INVALID_FIELD,                                // Battery SoH (%) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Voltage R, S, T (V) (não disponível nesse modelo)
    INVALID_FIELD,                                // EPS Current R, S, T (A) (não disponível nesse modelo)
    INVALID_FIELD                                 // EPS Active Power (W) (não disponível nesse modelo)
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------- Escolha ----------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------

ModbusInverterMap getInverterMap(InverterModel model) {
    ModbusInverterMap map; // Variável temporária para copiar o mapa da PROGMEM
    switch (model) {
        // Weg
        case SIW200_M030:
            memcpy_P(&map, &map_SIW200, sizeof(ModbusInverterMap));
            return map;
        case SIW200_M050:
            memcpy_P(&map, &map_SIW200, sizeof(ModbusInverterMap));
            return map;
        case SIW200_M085:
            memcpy_P(&map, &map_SIW200, sizeof(ModbusInverterMap));
            return map;
        case SIW200_M100:
            memcpy_P(&map, &map_SIW200, sizeof(ModbusInverterMap));
            return map;
// #
        case SIW200H_M050_W00:
            memcpy_P(&map, &map_SIW200H, sizeof(ModbusInverterMap));
            return map;
        case SIW200H_M075_W10:
            memcpy_P(&map, &map_SIW200H, sizeof(ModbusInverterMap));
            return map;
        case SIW200H_M105_W10:
            memcpy_P(&map, &map_SIW200H, sizeof(ModbusInverterMap));
            return map;
        case SIW200G_M030_W0:
            memcpy_P(&map, &map_SIW200G, sizeof(ModbusInverterMap));
            return map;
        case SIW200G_M030_W1:
            memcpy_P(&map, &map_SIW200G, sizeof(ModbusInverterMap));
            return map;
        case SIW200G_M050_W0:
            memcpy_P(&map, &map_SIW200G, sizeof(ModbusInverterMap));
            return map;
        case SIW200G_M050_W1:
            memcpy_P(&map, &map_SIW200G, sizeof(ModbusInverterMap));
            return map;
        case SIW200G_M060_W0:
            memcpy_P(&map, &map_SIW200G, sizeof(ModbusInverterMap));
            return map;
        case SIW200G_M060_W1:
            memcpy_P(&map, &map_SIW200G, sizeof(ModbusInverterMap));
            return map;
        case SIW200G_M070_W1:
            memcpy_P(&map, &map_SIW200G, sizeof(ModbusInverterMap));
            return map;
        case SIW200G_M075_W1:
            memcpy_P(&map, &map_SIW200G, sizeof(ModbusInverterMap));
            return map;
        case SIW200G_M080_W1:
            memcpy_P(&map, &map_SIW200G, sizeof(ModbusInverterMap));
            return map;
        case SIW200G_M090_W1:
            memcpy_P(&map, &map_SIW200G, sizeof(ModbusInverterMap));
            return map;
        case SIW200G_M105_W1:
            memcpy_P(&map, &map_SIW200G, sizeof(ModbusInverterMap));
            return map;
// #
        case SIW300H_M030_W00:
            memcpy_P(&map, &map_SIW300H, sizeof(ModbusInverterMap));
            return map;
        case SIW300H_M050_W00:
            memcpy_P(&map, &map_SIW300H, sizeof(ModbusInverterMap));
            return map;
        case SIW300H_M060_W00:
            memcpy_P(&map, &map_SIW300H, sizeof(ModbusInverterMap));
            return map;
        case SIW300H_M075_W00:
            memcpy_P(&map, &map_SIW300H, sizeof(ModbusInverterMap));
            return map;
        case SIW300H_M080_W00:
            memcpy_P(&map, &map_SIW300H, sizeof(ModbusInverterMap));
            return map;
        case SIW300H_M100_W00:
            memcpy_P(&map, &map_SIW300H, sizeof(ModbusInverterMap));
            return map;
        case SIW400_ST075:
            memcpy_P(&map, &map_SIW400, sizeof(ModbusInverterMap));
            return map;
// #
        case SIW400G_T012_W0:
            memcpy_P(&map, &map_SIW400G_T012_T025, sizeof(ModbusInverterMap));
            return map;
        case SIW400G_T012_W1:
            memcpy_P(&map, &map_SIW400G_T012_T025, sizeof(ModbusInverterMap));
            return map;
        case SIW400G_T015_W0:
            memcpy_P(&map, &map_SIW400G_T012_T025, sizeof(ModbusInverterMap));
            return map;
        case SIW400G_T015_W1:
            memcpy_P(&map, &map_SIW400G_T012_T025, sizeof(ModbusInverterMap));
            return map;
        case SIW400G_T020_W0:
            memcpy_P(&map, &map_SIW400G_T012_T025, sizeof(ModbusInverterMap));
            return map;
        case SIW400G_T020_W1:
            memcpy_P(&map, &map_SIW400G_T012_T025, sizeof(ModbusInverterMap));
            return map;
        case SIW400G_T025_W0:
            memcpy_P(&map, &map_SIW400G_T012_T025, sizeof(ModbusInverterMap));
            return map;
        case SIW400G_T025_W1:
            memcpy_P(&map, &map_SIW400G_T012_T025, sizeof(ModbusInverterMap));
            return map;
// #
        case SIW400G_K015_W00:
            memcpy_P(&map, &map_SIW400G_T050_T100, sizeof(ModbusInverterMap));
            return map;
        case SIW400G_K020_W00:
            memcpy_P(&map, &map_SIW400G_T050_T100, sizeof(ModbusInverterMap));
            return map;
        case SIW400G_K025_W00:
            memcpy_P(&map, &map_SIW400G_T050_T100, sizeof(ModbusInverterMap));
            return map;
        case SIW400G_K030_W00:
            memcpy_P(&map, &map_SIW400G_T050_T100, sizeof(ModbusInverterMap));
            return map;          
        case SIW400G_K037_W00:
            memcpy_P(&map, &map_SIW400G_T050_T100, sizeof(ModbusInverterMap));
            return map;  
        case SIW400G_T050_W00:
            memcpy_P(&map, &map_SIW400G_T050_T100, sizeof(ModbusInverterMap));
            return map;
        case SIW400G_T060_W00:
            memcpy_P(&map, &map_SIW400G_T050_T100, sizeof(ModbusInverterMap));
            return map;
        case SIW400G_T075_W0:
            memcpy_P(&map, &map_SIW400G_T050_T100, sizeof(ModbusInverterMap));
            return map;
        case SIW400G_T075_W01:
            memcpy_P(&map, &map_SIW400G_T050_T100, sizeof(ModbusInverterMap));
            return map;
        case SIW400G_T100_W0:
            memcpy_P(&map, &map_SIW400G_T050_T100, sizeof(ModbusInverterMap));
            return map;
        case SIW400H_T015_W10:
            memcpy_P(&map, &map_SIW400H_W10, sizeof(ModbusInverterMap));
            return map;
        case SIW400H_T030_W10:
            memcpy_P(&map, &map_SIW400H_W10, sizeof(ModbusInverterMap));
            return map;
        case SIW420G_K025_W00:
            memcpy_P(&map, &map_SIW420G, sizeof(ModbusInverterMap));
            return map;
        case SIW420G_K075_W00:
            memcpy_P(&map, &map_SIW420G, sizeof(ModbusInverterMap));
            return map;
        case SIW500G_T100_W0:
            memcpy_P(&map, &map_SIW500G, sizeof(ModbusInverterMap));
            return map;
        case SIW500G_K050_W00:
            memcpy_P(&map, &map_SIW500G, sizeof(ModbusInverterMap));
            return map;
        case SIW500G_K075_W00:
            memcpy_P(&map, &map_SIW500G, sizeof(ModbusInverterMap));
            return map;
        case SIW500G_H250_W0:
            memcpy_P(&map, &map_SIW500G, sizeof(ModbusInverterMap));
            return map;
        case SIW500H_ST012_M2:
            memcpy_P(&map, &map_SIW500H, sizeof(ModbusInverterMap));
            return map;
        case SIW500H_ST015_M2:
            memcpy_P(&map, &map_SIW500H, sizeof(ModbusInverterMap));
            return map;
        case SIW500H_ST020_M2:
            memcpy_P(&map, &map_SIW500H, sizeof(ModbusInverterMap));
            return map;
        case SIW500H_ST030_M3:
            memcpy_P(&map, &map_SIW500H, sizeof(ModbusInverterMap));
            return map;
        case SIW500H_ST036_M3:
            memcpy_P(&map, &map_SIW500H, sizeof(ModbusInverterMap));
            return map;
        case SIW500H_ST040_M3:
            memcpy_P(&map, &map_SIW500H, sizeof(ModbusInverterMap));
            return map;
        case SIW500H_SK020:
            memcpy_P(&map, &map_SIW500H, sizeof(ModbusInverterMap));
            return map;
        case SIW500H_ST030:
            memcpy_P(&map, &map_SIW500H, sizeof(ModbusInverterMap));
            return map;
        case SIW500H_ST036:
            memcpy_P(&map, &map_SIW500H, sizeof(ModbusInverterMap));
            return map;
        case SIW500H_ST040:
            memcpy_P(&map, &map_SIW500H, sizeof(ModbusInverterMap));
            return map;
        case SIW500H_ST060:
            memcpy_P(&map, &map_SIW500H, sizeof(ModbusInverterMap));
            return map;
        case SIW500H_ST100:
            memcpy_P(&map, &map_SIW500H, sizeof(ModbusInverterMap));
            return map;
        case SIW500H_ST060_HV:
            memcpy_P(&map, &map_SIW500H, sizeof(ModbusInverterMap));
            return map;
        case SIW500H_ST100_HV:
            memcpy_P(&map, &map_SIW500H, sizeof(ModbusInverterMap));
            return map;
        case SIW500H_ST200_H0:
            memcpy_P(&map, &map_SIW500H, sizeof(ModbusInverterMap));
            return map;
        case SIW500H_ST200_H3:
            memcpy_P(&map, &map_SIW500H, sizeof(ModbusInverterMap));
            return map;
        case SIW500H_T012_W00:
            memcpy_P(&map, &map_SIW500H, sizeof(ModbusInverterMap));
            return map;
        case SIW500H_T015_W00:
            memcpy_P(&map, &map_SIW500H, sizeof(ModbusInverterMap));
            return map;
        case SIW500H_T017_W00:
            memcpy_P(&map, &map_SIW500H, sizeof(ModbusInverterMap));
            return map;
        case SIW500H_T020_W00:
            memcpy_P(&map, &map_SIW500H, sizeof(ModbusInverterMap));
            return map;
        case SIW500H_T025_W00:
            memcpy_P(&map, &map_SIW500H, sizeof(ModbusInverterMap));
            return map;
        case SIW600_T020_44:
            memcpy_P(&map, &map_SIW600, sizeof(ModbusInverterMap));
            return map;
        case SIW610_T018_W0:
            memcpy_P(&map, &map_SIW610, sizeof(ModbusInverterMap));
            return map;
        case SIW610_T075_W0:
            memcpy_P(&map, &map_SIW610, sizeof(ModbusInverterMap));
            return map;

        // Goodwe
        case GW3000_XS:
            memcpy_P(&map, &map_GOODWE_NON_MT, sizeof(ModbusInverterMap));
            return map;
        case GW3300_XS_30:
            memcpy_P(&map, &map_GOODWE_NON_MT, sizeof(ModbusInverterMap));
            return map;
        case GW5000_DNS:
            memcpy_P(&map, &map_GOODWE_NON_MT, sizeof(ModbusInverterMap));
            return map;
        case GW5K_DNS_G40:
            memcpy_P(&map, &map_GOODWE_NON_MT, sizeof(ModbusInverterMap));
            return map;

        // Foxess
// #
        case S700_G2:
            memcpy_P(&map, &map_S_G2_series, sizeof(ModbusInverterMap));
            return map;
        case S1000_G2:
            memcpy_P(&map, &map_S_G2_series, sizeof(ModbusInverterMap));
            return map;
        case S1500_G2:
            memcpy_P(&map, &map_S_G2_series, sizeof(ModbusInverterMap));
            return map;
        case S2000_G2:
            memcpy_P(&map, &map_S_G2_series, sizeof(ModbusInverterMap));
            return map;
        case S2500_G2:
            memcpy_P(&map, &map_S_G2_series, sizeof(ModbusInverterMap));
            return map;
        case S3000_G2:
            memcpy_P(&map, &map_S_G2_series, sizeof(ModbusInverterMap));
            return map;
        case S3300_G2:
            memcpy_P(&map, &map_S_G2_series, sizeof(ModbusInverterMap));
            return map;
        case F3000_G2:
            memcpy_P(&map, &map_F_G2_series, sizeof(ModbusInverterMap));
            return map;
        case F3600_G2:
            memcpy_P(&map, &map_F_G2_series, sizeof(ModbusInverterMap));
            return map;
        case F4600_G2:
            memcpy_P(&map, &map_F_G2_series, sizeof(ModbusInverterMap));
            return map;
        case F5000_G2:
            memcpy_P(&map, &map_F_G2_series, sizeof(ModbusInverterMap));
            return map;
        case F5300_G2:
            memcpy_P(&map, &map_F_G2_series, sizeof(ModbusInverterMap));
            return map;
        case F6000_G2:
            memcpy_P(&map, &map_F_G2_series, sizeof(ModbusInverterMap));
            return map;
        case G7:
            memcpy_P(&map, &map_G_series, sizeof(ModbusInverterMap));
            return map;
        case G7_5:
            memcpy_P(&map, &map_G_series, sizeof(ModbusInverterMap));
            return map;
        case G8_:
            memcpy_P(&map, &map_G_series, sizeof(ModbusInverterMap));
            return map;
        case G9:
            memcpy_P(&map, &map_G_series, sizeof(ModbusInverterMap));
            return map;
        case G10:
            memcpy_P(&map, &map_G_series, sizeof(ModbusInverterMap));
            return map;
        case G10_5:
            memcpy_P(&map, &map_G_series, sizeof(ModbusInverterMap));
            return map;
        case T3_G3:
            memcpy_P(&map, &map_T_G3_series, sizeof(ModbusInverterMap));
            return map;
        case T4_G3:
            memcpy_P(&map, &map_T_G3_series, sizeof(ModbusInverterMap));
            return map;
        case T5_G3:
            memcpy_P(&map, &map_T_G3_series, sizeof(ModbusInverterMap));
            return map;
        case T6_G3:
            memcpy_P(&map, &map_T_G3_series, sizeof(ModbusInverterMap));
            return map;
        case T8_G3:
            memcpy_P(&map, &map_T_G3_series, sizeof(ModbusInverterMap));
            return map;
        case T10_G3:
            memcpy_P(&map, &map_T_G3_series, sizeof(ModbusInverterMap));
            return map;
        case T12_G3:
            memcpy_P(&map, &map_T_G3_series, sizeof(ModbusInverterMap));
            return map;
        case T15_G3:
            memcpy_P(&map, &map_T_G3_series, sizeof(ModbusInverterMap));
            return map;
        case T17_G3:
            memcpy_P(&map, &map_T_G3_series, sizeof(ModbusInverterMap));
            return map;
        case T20_G3:
            memcpy_P(&map, &map_T_G3_series, sizeof(ModbusInverterMap));
            return map;
        case T23_G3:
            memcpy_P(&map, &map_T_G3_series, sizeof(ModbusInverterMap));
            return map;
        case T25_G3:
            memcpy_P(&map, &map_T_G3_series, sizeof(ModbusInverterMap));
            return map;
// #
        case V30:
            memcpy_P(&map, &map_V_series, sizeof(ModbusInverterMap));
            return map;
        case V33:
            memcpy_P(&map, &map_V_series, sizeof(ModbusInverterMap));
            return map;
        case V36:
            memcpy_P(&map, &map_V_series, sizeof(ModbusInverterMap));
            return map;
        case V40:
            memcpy_P(&map, &map_V_series, sizeof(ModbusInverterMap));
            return map;
        case V50:
            memcpy_P(&map, &map_V_series, sizeof(ModbusInverterMap));
            return map;
        case V60:
            memcpy_P(&map, &map_V_series, sizeof(ModbusInverterMap));
            return map;
        case V75:
            memcpy_P(&map, &map_V_series, sizeof(ModbusInverterMap));
            return map;
        case VL15:
            memcpy_P(&map, &map_V_series, sizeof(ModbusInverterMap));
            return map;
        case VL20:
            memcpy_P(&map, &map_V_series, sizeof(ModbusInverterMap));
            return map;
        case VL25:
            memcpy_P(&map, &map_V_series, sizeof(ModbusInverterMap));
            return map;
        case VL30:
            memcpy_P(&map, &map_V_series, sizeof(ModbusInverterMap));
            return map;
        case VL37_5:
            memcpy_P(&map, &map_V_series, sizeof(ModbusInverterMap));
            return map;
        case R75:
            memcpy_P(&map, &map_R_series, sizeof(ModbusInverterMap));
            return map;
        case R100:
            memcpy_P(&map, &map_R_series, sizeof(ModbusInverterMap));
            return map;
        case R110:
            memcpy_P(&map, &map_R_series, sizeof(ModbusInverterMap));
            return map;

        // Huawei
        case SUN2000_2KTL:
            memcpy_P(&map, &map_SUN2000_MONO, sizeof(ModbusInverterMap));
            return map;
        case SUN2000_3KTL:
            memcpy_P(&map, &map_SUN2000_MONO, sizeof(ModbusInverterMap));
            return map;
        case SUN2000_4KTL:
            memcpy_P(&map, &map_SUN2000_MONO, sizeof(ModbusInverterMap));
            return map;
        case SUN2000_5KTL:
            memcpy_P(&map, &map_SUN2000_MONO, sizeof(ModbusInverterMap));
            return map;
        case SUN2000_6KTL:
            memcpy_P(&map, &map_SUN2000_MONO, sizeof(ModbusInverterMap));
            return map;
        case SUN2000_12KTL_M0:
            memcpy_P(&map, &map_SUN2000_TRIF, sizeof(ModbusInverterMap));
            return map;
        case SUN2000_12KTL_M2:
            memcpy_P(&map, &map_SUN2000_TRIF, sizeof(ModbusInverterMap));
            return map;
        case SUN2000_15KTL_M0:
            memcpy_P(&map, &map_SUN2000_TRIF, sizeof(ModbusInverterMap));
            return map;
        case SUN2000_15KTL_M2:
            memcpy_P(&map, &map_SUN2000_TRIF, sizeof(ModbusInverterMap));
            return map;
        case SUN2000_20KTL_M0:
            memcpy_P(&map, &map_SUN2000_TRIF, sizeof(ModbusInverterMap));
            return map;
        case SUN2000_20KTL_M2:
            memcpy_P(&map, &map_SUN2000_TRIF, sizeof(ModbusInverterMap));
            return map;
        case SUN2000_30KTL:
            memcpy_P(&map, &map_SUN2000_TRIF, sizeof(ModbusInverterMap));
            return map;
        case SUN2000_36KTL_A:
            memcpy_P(&map, &map_SUN2000_TRIF, sizeof(ModbusInverterMap));
            return map;
        case SUN2000_30KTL_M3:
            memcpy_P(&map, &map_SUN2000_TRIF, sizeof(ModbusInverterMap));
            return map;
        case SUN2000_36KTL_M3:
            memcpy_P(&map, &map_SUN2000_TRIF, sizeof(ModbusInverterMap));
            return map;
        case SUN2000_40KTL_M3:
            memcpy_P(&map, &map_SUN2000_TRIF, sizeof(ModbusInverterMap));
            return map;
        case SUN2000_60KTL_M0:
            memcpy_P(&map, &map_SUN2000_TRIF, sizeof(ModbusInverterMap));
            return map;
        case SUN2000_75KTL_M1:
            memcpy_P(&map, &map_SUN2000_TRIF, sizeof(ModbusInverterMap));
            return map;
        case SUN2000_100KTL_M2:
            memcpy_P(&map, &map_SUN2000_TRIF, sizeof(ModbusInverterMap));
            return map;
        case SUN2000_100KTL_H1:
            memcpy_P(&map, &map_SUN2000_TRIF, sizeof(ModbusInverterMap));
            return map;
        case SUN2000_215KTL_H0:
            memcpy_P(&map, &map_SUN2000_TRIF, sizeof(ModbusInverterMap));
            return map;
        case SUN2000_215KTL_H3:
            memcpy_P(&map, &map_SUN2000_TRIF, sizeof(ModbusInverterMap));
            return map;
        case SUN2000_250KTL_H1:
            memcpy_P(&map, &map_SUN2000_TRIF, sizeof(ModbusInverterMap));
            return map;
            
        case UNKNOWN_INVERTER:
            memset(&map, 0, sizeof(ModbusInverterMap)); // Retorna um mapa vazio para o modelo desconhecido
            return map;
        default:
            memset(&map, 0, sizeof(ModbusInverterMap)); // Retorna um mapa vazio para modelos não mapeados
            return map;
    }
}