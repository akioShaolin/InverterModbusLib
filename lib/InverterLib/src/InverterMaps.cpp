#include "InverterMaps.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------- WEG ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------

static const ModbusInverterMap map_SIW_400_ST075 = {
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
    { 0x0101, U16, 1, 1, 1.0f, true, true },      // Set Power Factor % [1, 20] LAGGING, [80, 100] LEADING
    INVALID_FIELD,                                // Power Factor Excitation Mode (Não disponível nesse modelo)
    // Tempo
    { 0x0010, U16, 1, 1, 1.0f, true, false },     // Time Year MSB
    { 0x0010, U16, 1, 1, 1.0f, true, false },     // Time Month LSB
    { 0x0011, U16, 1, 1, 1.0f, true, false },     // Time Day MSB
    { 0x0011, U16, 1, 1, 1.0f, true, false },     // Time Hour LSB
    { 0x0012, U16, 1, 1, 1.0f, true, false },     // Time Minute MSB
    { 0x0012, U16, 1, 1, 1.0f, true, false },     // Time Second LSB
    INVALID_FIELD,                                // Time Epoch (não disponível nesse modelo)
    // Status
    { 0x0, U16, 2, 1, 0.1f, true, false },     // Total Energy (kWh) - MSB em 0x0222 e LSB em 0x0223 
    { 0x0, U16, 1, 1, 0.1f, true, false },     // Daily Energy (kWh)  

    { 0x0, U16, 1, 1, 1.0f, true, false },     // Active Power (W)
    INVALID_FIELD                             ,   // Apparent Power (kVA) (não disponível nesse modelo)
    INVALID_FIELD                             ,   // Reactive Power (kVAr) (não disponível nesse modelo)
    INVALID_FIELD                             ,   // Power Factor (não disponível nesse modelo)

    { 0x0, U16, 1, 1, 0.1f, true, false },     // Grid Voltage R (V)
    { 0x0, U16, 1, 1, 0.1f, true, false },     // Grid Current R (A)
    { 0x0, U16, 1, 1, 0.01f, true, false },    // Frequency (Hz)

    { 0x0, U16, 1, 1, 0.1f, true, false },     // Temperature (°C)
    INVALID_FIELD,                                // Insulation Resistance (kΩ)
    { 0x0, U16, 1, 1, 1.0f, true, false },     // Inverter Status
    { 0x0, U32, 1, 2, 1.0f, true, false },     // Alarm

    { 0x0, U16, 1, 1, 0.1f, true, false },    // String Voltage (V)
    { 0x0, U16, 1, 1, 0.1f, true, false },    // String Current (A)
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

static const ModbusInverterMap map_SIW400G_T075_W0 = {
    // Identificação
    { 0x9C75, ASCII, 16, 1, 1.0f, true, false }, // Serial. Scale é ignorado para ASCII; usar 1.0f apenas como valor neutro
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
    { 0x9C95, U64, 1, 4, 100.0f, true, false },   // Total Energy (Wh)
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

static const ModbusInverterMap map_SIW400G_T100_W0 = {
    // Identificação
    { 0x9C75, ASCII, 16, 1, 1.0f, true, false }, // Serial. Scale é ignorado para ASCII; usar 1.0f apenas como valor neutro
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
    { 0x9C95, U64, 1, 4, 100.0f, true, false },   // Total Energy (Wh)
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
// ----------------------------------------------- FOXESS -----------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------

static const ModbusInverterMap map_R_series = {
    // Identificação
    { 0x9C75, ASCII, 16, 1, 1.0f, true, false }, // Serial. Scale é ignorado para ASCII; usar 1.0f apenas como valor neutro
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
    { 0x9C95, U64, 1, 4, 100.0f, true, false },   // Total Energy (Wh)
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

static const ModbusInverterMap map_GW3000_XS = {
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
    { 0x0101, U16, 1, 1, 1.0f, true, true },      // Set Power Factor % [1, 20] LAGGING, [80, 100] LEADING
    INVALID_FIELD,                                // Power Factor Excitation Mode (Não disponível nesse modelo)
    // Tempo
    { 0x0010, U16, 1, 1, 1.0f, true, false },     // Time Year MSB
    { 0x0010, U16, 1, 1, 1.0f, true, false },     // Time Month LSB
    { 0x0011, U16, 1, 1, 1.0f, true, false },     // Time Day MSB
    { 0x0011, U16, 1, 1, 1.0f, true, false },     // Time Hour LSB
    { 0x0012, U16, 1, 1, 1.0f, true, false },     // Time Minute MSB
    { 0x0012, U16, 1, 1, 1.0f, true, false },     // Time Second LSB
    INVALID_FIELD,                                // Time Epoch (não disponível nesse modelo)
    // Status
    { 0x0222, U16, 2, 1, 0.1f, true, false },     // Total Energy (kWh) - MSB em 0x0222 e LSB em 0x0223 
    { 0x0236, U16, 1, 1, 0.1f, true, false },     // Daily Energy (kWh)  

    { 0x0233, U16, 1, 1, 1.0f, true, false },     // Active Power (W)
    INVALID_FIELD                             ,   // Apparent Power (kVA) (não disponível nesse modelo)
    INVALID_FIELD                             ,   // Reactive Power (kVAr) (não disponível nesse modelo)
    INVALID_FIELD                             ,   // Power Factor (não disponível nesse modelo)

    { 0x022A, U16, 1, 1, 0.1f, true, false },     // Grid Voltage R (V)
    { 0x022D, U16, 1, 1, 0.1f, true, false },     // Grid Current R (A)
    { 0x022F, U16, 1, 1, 0.01f, true, false },    // Frequency (Hz)

    { 0x0235, U16, 1, 1, 0.1f, true, false },     // Temperature (°C)
    INVALID_FIELD,                                // Insulation Resistance (kΩ)
    { 0x0234, U16, 1, 1, 1.0f, true, false },     // Inverter Status
    { 0x0220, U32, 1, 2, 1.0f, true, false },     // Alarm

    { 0x0226, U16, 1, 1, 0.1f, true, false },    // String Voltage (V)
    { 0x0228, U16, 1, 1, 0.1f, true, false },    // String Current (A)
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

static const ModbusInverterMap map_GW3300_XS_30 = {
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
    { 0x0101, U16, 1, 1, 1.0f, true, true },      // Set Power Factor % [1, 20] LAGGING, [80, 100] LEADING
    INVALID_FIELD,                                // Power Factor Excitation Mode (Não disponível nesse modelo)
    // Tempo
    { 0x0010, U16, 1, 1, 1.0f, true, false },     // Time Year MSB
    { 0x0010, U16, 1, 1, 1.0f, true, false },     // Time Month LSB
    { 0x0011, U16, 1, 1, 1.0f, true, false },     // Time Day MSB
    { 0x0011, U16, 1, 1, 1.0f, true, false },     // Time Hour LSB
    { 0x0012, U16, 1, 1, 1.0f, true, false },     // Time Minute MSB
    { 0x0012, U16, 1, 1, 1.0f, true, false },     // Time Second LSB
    INVALID_FIELD,                                // Time Epoch (não disponível nesse modelo)
    // Status
    { 0x0222, U16, 2, 1, 0.1f, true, false },     // Total Energy (kWh) - MSB em 0x0222 e LSB em 0x0223 
    { 0x0236, U16, 1, 1, 0.1f, true, false },     // Daily Energy (kWh)  

    { 0x0233, U16, 1, 1, 1.0f, true, false },     // Active Power (W)
    INVALID_FIELD                             ,   // Apparent Power (kVA) (não disponível nesse modelo)
    INVALID_FIELD                             ,   // Reactive Power (kVAr) (não disponível nesse modelo)
    INVALID_FIELD                             ,   // Power Factor (não disponível nesse modelo)

    { 0x022A, U16, 1, 1, 0.1f, true, false },     // Grid Voltage R (V)
    { 0x022D, U16, 1, 1, 0.1f, true, false },     // Grid Current R (A)
    { 0x022F, U16, 1, 1, 0.01f, true, false },    // Frequency (Hz)

    { 0x0235, U16, 1, 1, 0.1f, true, false },     // Temperature (°C)
    INVALID_FIELD,                                // Insulation Resistance (kΩ)
    { 0x0234, U16, 1, 1, 1.0f, true, false },     // Inverter Status
    { 0x0220, U32, 1, 2, 1.0f, true, false },     // Alarm

    { 0x0226, U16, 1, 1, 0.1f, true, false },    // String Voltage (V)
    { 0x0228, U16, 1, 1, 0.1f, true, false },    // String Current (A)
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

static const ModbusInverterMap map_GW5000_DNS = {
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
    { 0x0101, U16, 1, 1, 1.0f, true, true },      // Set Power Factor % [1, 20] LAGGING, [80, 100] LEADING
    INVALID_FIELD,                                // Power Factor Excitation Mode (Não disponível nesse modelo)
    // Tempo
    { 0x0010, U16, 1, 1, 1.0f, true, false },     // Time Year MSB
    { 0x0010, U16, 1, 1, 1.0f, true, false },     // Time Month LSB
    { 0x0011, U16, 1, 1, 1.0f, true, false },     // Time Day MSB
    { 0x0011, U16, 1, 1, 1.0f, true, false },     // Time Hour LSB
    { 0x0012, U16, 1, 1, 1.0f, true, false },     // Time Minute MSB
    { 0x0012, U16, 1, 1, 1.0f, true, false },     // Time Second LSB
    INVALID_FIELD,                                // Time Epoch (não disponível nesse modelo)
    // Status
    { 0x0222, U16, 2, 1, 0.1f, true, false },     // Total Energy (kWh) - MSB em 0x0222 e LSB em 0x0223 
    { 0x0236, U16, 1, 1, 0.1f, true, false },     // Daily Energy (kWh)  

    { 0x0233, U16, 1, 1, 1.0f, true, false },     // Active Power (W)
    INVALID_FIELD                             ,   // Apparent Power (kVA) (não disponível nesse modelo)
    INVALID_FIELD                             ,   // Reactive Power (kVAr) (não disponível nesse modelo)
    INVALID_FIELD                             ,   // Power Factor (não disponível nesse modelo)

    { 0x022A, U16, 1, 1, 0.1f, true, false },     // Grid Voltage R (V)
    { 0x022D, U16, 1, 1, 0.1f, true, false },     // Grid Current R (A)
    { 0x022F, U16, 1, 1, 0.01f, true, false },    // Frequency (Hz)

    { 0x0235, U16, 1, 1, 0.1f, true, false },     // Temperature (°C)
    INVALID_FIELD,                                // Insulation Resistance (kΩ)
    { 0x0234, U16, 1, 1, 1.0f, true, false },     // Inverter Status
    { 0x0220, U32, 1, 2, 1.0f, true, false },     // Alarm

    { 0x0226, U16, 1, 1, 0.1f, true, false },    // String Voltage (V)
    { 0x0228, U16, 1, 1, 0.1f, true, false },    // String Current (A)
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

static const ModbusInverterMap map_GW5K_DNS_G40 = {
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
    { 0x0101, U16, 1, 1, 1.0f, true, true },      // Set Power Factor % [1, 20] LAGGING, [80, 100] LEADING
    INVALID_FIELD,                                // Power Factor Excitation Mode (Não disponível nesse modelo)
    // Tempo
    { 0x0010, U16, 1, 1, 1.0f, true, false },     // Time Year MSB
    { 0x0010, U16, 1, 1, 1.0f, true, false },     // Time Month LSB
    { 0x0011, U16, 1, 1, 1.0f, true, false },     // Time Day MSB
    { 0x0011, U16, 1, 1, 1.0f, true, false },     // Time Hour LSB
    { 0x0012, U16, 1, 1, 1.0f, true, false },     // Time Minute MSB
    { 0x0012, U16, 1, 1, 1.0f, true, false },     // Time Second LSB
    INVALID_FIELD,                                // Time Epoch (não disponível nesse modelo)
    // Status
    { 0x0222, U16, 2, 1, 0.1f, true, false },     // Total Energy (kWh) - MSB em 0x0222 e LSB em 0x0223 
    { 0x0236, U16, 1, 1, 0.1f, true, false },     // Daily Energy (kWh)  

    { 0x0233, U16, 1, 1, 1.0f, true, false },     // Active Power (W)
    INVALID_FIELD                             ,   // Apparent Power (kVA) (não disponível nesse modelo)
    INVALID_FIELD                             ,   // Reactive Power (kVAr) (não disponível nesse modelo)
    INVALID_FIELD                             ,   // Power Factor (não disponível nesse modelo)

    { 0x022A, U16, 1, 1, 0.1f, true, false },     // Grid Voltage R (V)
    { 0x022D, U16, 1, 1, 0.1f, true, false },     // Grid Current R (A)
    { 0x022F, U16, 1, 1, 0.01f, true, false },    // Frequency (Hz)

    { 0x0235, U16, 1, 1, 0.1f, true, false },     // Temperature (°C)
    INVALID_FIELD,                                // Insulation Resistance (kΩ)
    { 0x0234, U16, 1, 1, 1.0f, true, false },     // Inverter Status
    { 0x0220, U32, 1, 2, 1.0f, true, false },     // Alarm

    { 0x0226, U16, 1, 1, 0.1f, true, false },    // String Voltage (V)
    { 0x0228, U16, 1, 1, 0.1f, true, false },    // String Current (A)
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

const ModbusInverterMap* getInverterMap(InverterModel model) {
    switch (model) {
        case SIW400G_T075_W0:
            return &map_SIW400G_T075_W0;
        case SIW400G_T100_W0:
            return &map_SIW400G_T100_W0;
        case R75:
            return &map_R_series;
        case R100:
            return &map_R_series;
        case R110:
            return &map_R_series;
        case GW3000_XS:
            return &map_GW3000_XS;
        case GW3300_XS_30:
            return &map_GW3300_XS_30;
        case GW5000_DNS:
            return &map_GW5000_DNS;
        case GW5K_DNS_G40:
            return &map_GW5K_DNS_G40;
            
        case UNKNOWN_INVERTER:
            return nullptr;
        default:
            return nullptr;
    }
}