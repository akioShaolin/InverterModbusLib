/*
 * InverterModbusLib - Solar Inverter Library for Arduino
 * ------------------------------------------------
 * Modbus RTU (RS485)communication layer for inverter integration
 *
 * Copyright (c) 2026, Pedro Akio Sakuma
 * Licensed under BSD 3-Clause License
 */

#include "InverterDescriptor.h"
#include "InverterModels.h"

constexpr ModbusConfigData gwCompModbusConfig PROGMEM {247, 9600, SERIAL_8N1};
// Weg
constexpr InverterDescriptor desc_SIW200_M030 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    3000U,  
    &gwCompModbusConfig, 
    {1, 1, {0}}, 
    noBattery, 
    AlarmFormat::BITFIELD32, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW200_M050 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    5000U,  
    &gwCompModbusConfig, 
    {2, 2, {0, 1}}, 
    noBattery, 
    AlarmFormat::BITFIELD32, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW200_M085 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    8500U,  
    &gwCompModbusConfig, 
    {3, 3, {0, 1, 2}}, 
    noBattery, 
    AlarmFormat::BITFIELD32, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW200_M100 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    10000U, 
    &gwCompModbusConfig, 
    {3, 3, {0, 1, 2}}, 
    noBattery, 
    AlarmFormat::BITFIELD32, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW200H_M050_W00 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    5000U,  
    &defaultModbusConfig, 
    {2, 2, {0, 1}}, 
    {4, 1, {0, 0, 0, 0}}, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW200H_M075_W10 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    7500U,  
    &defaultModbusConfig, 
    {3, 3, {0, 1, 2}}, 
    {4, 1, {0, 0, 0, 0}}, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW200H_M105_W10 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    10500U, 
    &defaultModbusConfig, 
    {4, 4, {0, 1, 2, 3}}, 
    {4, 1, {0, 0, 0, 0}}, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW200G_M030_W0 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    3000U,  
    &defaultModbusConfig, 
    {2, 2, {0, 1}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW200G_M030_W1 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    3000U,  
    &defaultModbusConfig, 
    {2, 2, {0, 1}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW200G_M050_W0 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    5000U,  
    &defaultModbusConfig, 
    {2, 2, {0, 1}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW200G_M050_W1 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    5000U,  
    &defaultModbusConfig, 
    {2, 2, {0, 1}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW200G_M060_W0 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    6000U,  
    &defaultModbusConfig, 
    {2, 2, {0, 1}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW200G_M060_W1 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    6000U,  
    &defaultModbusConfig, 
    {2, 2, {0, 1}},
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW200G_M070_W1 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    7000U,  
    &defaultModbusConfig, 
    {3, 3, {0, 1, 2}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW200G_M075_W1 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    7500U,  
    &defaultModbusConfig, 
    {3, 3, {0, 1, 2}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW200G_M080_W1 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    8000U,  
    &defaultModbusConfig, 
    {3, 3, {0, 1, 2}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW200G_M090_W1 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    9000U,  
    &defaultModbusConfig, 
    {3, 3, {0, 1, 2}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW200G_M105_W1 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    10500U, 
    &defaultModbusConfig, 
    {3, 3, {0, 1, 2}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW300H_M030_W00 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    3000U,  
    &defaultModbusConfig, 
    {2, 2, {0, 1}}, 
    {2, 1, {0, 0}}, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16  
};

constexpr InverterDescriptor desc_SIW300H_M050_W00 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    5000U,  
    &defaultModbusConfig, 
    {2, 2, {0, 1}}, 
    {2, 1, {0, 0}}, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16  
};

constexpr InverterDescriptor desc_SIW300H_M060_W00 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    6000U,  
    &defaultModbusConfig, 
    {2, 2, {0, 1}}, 
    {2, 1, {0, 0}}, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16  
};

constexpr InverterDescriptor desc_SIW300H_M075_W00 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    7500U,  
    &defaultModbusConfig, 
    {3, 3, {0, 1, 2}}, 
    {2, 1, {0, 0}}, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16  
};

constexpr InverterDescriptor desc_SIW300H_M080_W00 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    8000U,  
    &defaultModbusConfig, 
    {3, 3, {0, 1, 2}}, 
    {2, 1, {0, 0}}, 
    AlarmFormat::BITFIELD16,
    StatusFormat::BITFIELD16  
};

constexpr InverterDescriptor desc_SIW300H_M100_W00 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    10000U, 
    &defaultModbusConfig, 
    {3, 3, {0, 1, 2}}, 
    {2, 1, {0, 0}}, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16  
};

constexpr InverterDescriptor desc_SIW400_ST075 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    75000U,  
    &gwCompModbusConfig, 
    {16, 4, {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW400G_T012_W0 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    12000U,  
    &defaultModbusConfig, 
    {2, 2, {0, 1}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW400G_T012_W1 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    12000U,  
    &defaultModbusConfig, 
    {2, 2, {0, 1}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW400G_T015_W0 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    15000U,  
    &defaultModbusConfig, 
    {4, 2, {0, 0, 1, 1}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW400G_T015_W1 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    15000U,  
    &defaultModbusConfig, 
    {4, 2, {0, 0, 1, 1}}, 
    noBattery, 
    AlarmFormat::BITFIELD16,
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW400G_T020_W0 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    20000U,  
    &defaultModbusConfig, 
    {4, 2, {0, 0, 1, 1}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW400G_T020_W1 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    20000U,  
    &defaultModbusConfig, 
    {4, 2, {0, 0, 1, 1}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW400G_T025_W0 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    25000U,  
    &defaultModbusConfig, 
    {4, 2, {0, 0, 1, 1}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW400G_T025_W1 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    25000U,  
    &defaultModbusConfig, 
    {4, 2, {0, 0, 1, 1}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW400G_T075_W0 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    75000U,  
    &defaultModbusConfig, 
    {18, 9, {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8}}, 
    noBattery,
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW400G_T100_W0 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    100000U, 
    &defaultModbusConfig, 
    {18, 9, {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW400G_T050_W00 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    50000U,  
    &defaultModbusConfig, 
    {12, 6, {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW400G_T060_W00 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    50000U,  
    &defaultModbusConfig, 
    {12, 6, {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW400G_T075_W01 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    50000U,  
    &defaultModbusConfig, 
    {12, 6, {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}}, 
    noBattery, 
    AlarmFormat::BITFIELD16,
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW400G_K015_W00 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    15000U,  
    &defaultModbusConfig, 
    {8, 4, {0, 0, 1, 1, 2, 2, 3, 3}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW400G_K020_W00 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    20000U,  
    &defaultModbusConfig, 
    {8, 4, {0, 0, 1, 1, 2, 2, 3, 3}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW400G_K025_W00 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    25000U,  
    &defaultModbusConfig, 
    {8, 4, {0, 0, 1, 1, 2, 2, 3, 3}}, 
    noBattery,
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW400G_K030_W00 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    30000U,  
    &defaultModbusConfig, 
    {8, 4, {0, 0, 1, 1, 2, 2, 3, 3}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW400G_K037_W00 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    37000U,  
    &defaultModbusConfig, 
    {8, 4, {0, 0, 1, 1, 2, 2, 3, 3}}, 
    noBattery,
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW400H_T015_W10 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    15000U,  
    &defaultModbusConfig, 
    {6, 3, {0, 0, 1, 1, 2, 2,}}, 
    {8, 2, {0, 0, 0, 0, 1, 1, 1, 1}}, 
     AlarmFormat::BITFIELD16, 
     StatusFormat::ENUM_CODE  
    };
constexpr InverterDescriptor desc_SIW400H_T030_W10 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    30000U,  
    &defaultModbusConfig, 
    {6, 3, {0, 0, 1, 1, 2, 2,}}, 
    {8, 2, {0, 0, 0, 0, 1, 1, 1, 1}}, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW420G_K025_W00 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    25000U,  
    &defaultModbusConfig, 
    {4, 2,   {0, 0, 1, 1}}, 
    noBattery, 
    AlarmFormat::UNKNOWN, 
    StatusFormat::UNKNOWN    
};

constexpr InverterDescriptor desc_SIW420G_K075_W00 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    75000U,  
    &defaultModbusConfig, 
    {18, 9,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8}}, 
    noBattery, 
    AlarmFormat::UNKNOWN, 
    StatusFormat::UNKNOWN    
};

constexpr InverterDescriptor desc_SIW500G_T075_W0 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    75000U,  
    &defaultModbusConfig, 
    {20, 10, {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16
};

constexpr InverterDescriptor desc_SIW500G_T100_W0 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    100000U,  
    &defaultModbusConfig, 
    {20, 10, {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16 
};

constexpr InverterDescriptor desc_SIW500G_K050_W00 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    50000U,  
    &defaultModbusConfig, 
    {21, 7,  {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16 
};

constexpr InverterDescriptor desc_SIW500G_K075_W00 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    75000U,  
    &defaultModbusConfig, 
    {21, 7,  {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16 
};

constexpr InverterDescriptor desc_SIW500G_H250_W0 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    250000U,  
    &defaultModbusConfig, 
    {28, 6,  {0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16 
};

constexpr InverterDescriptor desc_SIW500H_ST012_M2 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    12000U,  
    &defaultModbusConfig, 
    {4, 2,   {0, 0, 1, 1}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16 
};

constexpr InverterDescriptor desc_SIW500H_ST015_M2 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    15000U,  
    &defaultModbusConfig, 
    {4, 2,   {0, 0, 1, 1}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16 
};

constexpr InverterDescriptor desc_SIW500H_ST020_M2 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    20000U,  
    &defaultModbusConfig, 
    {4, 2,   {0, 0, 1, 1}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16 
};

constexpr InverterDescriptor desc_SIW500H_ST030_M3 PROGMEM = {
    STRING,
    ON_GRID,
    THREE_PHASE,
    30000U,
    &defaultModbusConfig,
    { 8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},
    noBattery,
    AlarmFormat::BITFIELD16,
    StatusFormat::BITFIELD16
};

constexpr InverterDescriptor desc_SIW500H_ST036_M3 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    36000U,  
    &defaultModbusConfig, 
    {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16 
};

constexpr InverterDescriptor desc_SIW500H_ST040_M3 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    40000U, 
    &defaultModbusConfig, 
    {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, 
    noBattery,
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16 
};

constexpr InverterDescriptor desc_SIW500H_SK020 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    20000U,  
    &defaultModbusConfig, 
    {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16 
};

constexpr InverterDescriptor desc_SIW500H_ST030 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    30000U, 
     &defaultModbusConfig, 
     {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, 
     noBattery, 
     AlarmFormat::BITFIELD16, 
     StatusFormat::BITFIELD16 
    };

constexpr InverterDescriptor desc_SIW500H_ST036 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    36000U,  
    &defaultModbusConfig, 
    {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16 
};

constexpr InverterDescriptor desc_SIW500H_ST040 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    40000U,  
    &defaultModbusConfig, 
    {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16 
};

constexpr InverterDescriptor desc_SIW500H_ST060 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    60000U,  
    &defaultModbusConfig, 
    {12, 6,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16 
};

constexpr InverterDescriptor desc_SIW500H_ST100 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    100000U,  
    &defaultModbusConfig, 
    {20, 10, {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16 
};

constexpr InverterDescriptor desc_SIW500H_ST200_H3 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    200000U,  
    &defaultModbusConfig, 
    {18, 9,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16 
};

constexpr InverterDescriptor desc_SIW500H_ST200_H0 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    200000U,  
    &defaultModbusConfig, 
    {14, 3,  {0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16 
};

constexpr InverterDescriptor desc_SIW500H_ST060_HV PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    600000U,  
    &defaultModbusConfig, 
    {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16 
};

constexpr InverterDescriptor desc_SIW500H_ST100_HV PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    100000U,  
    &defaultModbusConfig, 
    {12, 6,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16 
};

constexpr InverterDescriptor desc_SIW500H_T012_W00 PROGMEM = {
    STRING, 
    HYBRID, 
    THREE_PHASE, 
    12000U,  
    &defaultModbusConfig, 
    {4, 2,   {0, 0, 1, 1}}, 
    {4, 2, {0, 0, 1, 1}}, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16 
};

constexpr InverterDescriptor desc_SIW500H_T015_W00 PROGMEM = {
    STRING, 
    HYBRID, 
    THREE_PHASE, 
    15000U,  
    &defaultModbusConfig, 
    {4, 2,   {0, 0, 1, 1}}, 
    {4, 2, {0, 0, 1, 1}}, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16 
};

constexpr InverterDescriptor desc_SIW500H_T017_W00 PROGMEM = {
    STRING, 
    HYBRID, 
    THREE_PHASE, 
    17000U,  
    &defaultModbusConfig, 
    {4, 2,   {0, 0, 1, 1}}, 
    {4, 2, {0, 0, 1, 1}}, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16 
};

constexpr InverterDescriptor desc_SIW500H_T020_W00 PROGMEM = {
    STRING, 
    HYBRID, 
    THREE_PHASE, 
    20000U,  
    &defaultModbusConfig, 
    {4, 2,   {0, 0, 1, 1}}, 
    {4, 2, {0, 0, 1, 1}}, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16 
};

constexpr InverterDescriptor desc_SIW500H_T025_W00 PROGMEM = {
    STRING, 
    HYBRID, 
    THREE_PHASE, 
    25000U,  
    &defaultModbusConfig, 
    {4, 2,   {0, 0, 1, 1}}, 
    {4, 2, {0, 0, 1, 1}}, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::BITFIELD16 
};

constexpr InverterDescriptor desc_SIW600_T020_44 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    20000U,  
    &defaultModbusConfig, 
    {4, 2,   {0, 0, 1, 1}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW610_T018_W0 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    18000U,  
    &defaultModbusConfig, 
    {4, 2,   {0, 0, 1, 1}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_SIW610_T075_W0 PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    75000U,  
    &defaultModbusConfig, 
    {12, 6,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}}, 
    noBattery, 
    AlarmFormat::BITFIELD16, 
    StatusFormat::ENUM_CODE  
};

const InverterDescriptor getDescriptor_Weg(InverterModel model) {
    InverterDescriptor desc;
    switch (model) {
        case SIW200_M030:
            memcpy_P(&desc, &desc_SIW200_M030, sizeof(InverterDescriptor));
            return desc;
        case SIW200_M050:
            memcpy_P(&desc, &desc_SIW200_M050, sizeof(InverterDescriptor));
            return desc;
        case SIW200_M085:
            memcpy_P(&desc, &desc_SIW200_M085, sizeof(InverterDescriptor));
            return desc;
        case SIW200_M100:
            memcpy_P(&desc, &desc_SIW200_M100, sizeof(InverterDescriptor));
            return desc;
        case SIW200H_M050_W00:
            memcpy_P(&desc, &desc_SIW200H_M050_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW200H_M075_W10:
            memcpy_P(&desc, &desc_SIW200H_M075_W10, sizeof(InverterDescriptor));
            return desc;
        case SIW200H_M105_W10:
            memcpy_P(&desc, &desc_SIW200H_M105_W10, sizeof(InverterDescriptor));
            return desc;
        case SIW200G_M030_W0:
            memcpy_P(&desc, &desc_SIW200G_M030_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW200G_M030_W1:
            memcpy_P(&desc, &desc_SIW200G_M030_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW200G_M050_W0:
            memcpy_P(&desc, &desc_SIW200G_M050_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW200G_M050_W1:
            memcpy_P(&desc, &desc_SIW200G_M050_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW200G_M060_W0:
            memcpy_P(&desc, &desc_SIW200G_M060_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW200G_M060_W1:
            memcpy_P(&desc, &desc_SIW200G_M060_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW200G_M070_W1:
            memcpy_P(&desc, &desc_SIW200G_M070_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW200G_M075_W1:
            memcpy_P(&desc, &desc_SIW200G_M075_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW200G_M080_W1:
            memcpy_P(&desc, &desc_SIW200G_M080_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW200G_M090_W1:
            memcpy_P(&desc, &desc_SIW200G_M090_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW200G_M105_W1:
            memcpy_P(&desc, &desc_SIW200G_M105_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW300H_M030_W00:
            memcpy_P(&desc, &desc_SIW300H_M030_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW300H_M050_W00:
            memcpy_P(&desc, &desc_SIW300H_M050_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW300H_M060_W00:
            memcpy_P(&desc, &desc_SIW300H_M060_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW300H_M075_W00:
            memcpy_P(&desc, &desc_SIW300H_M075_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW300H_M080_W00:
            memcpy_P(&desc, &desc_SIW300H_M080_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW300H_M100_W00:
            memcpy_P(&desc, &desc_SIW300H_M100_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW400_ST075:
            memcpy_P(&desc, &desc_SIW400_ST075, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T012_W0:
            memcpy_P(&desc, &desc_SIW400G_T012_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T012_W1:
            memcpy_P(&desc, &desc_SIW400G_T012_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T015_W0:
            memcpy_P(&desc, &desc_SIW400G_T015_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T015_W1:
            memcpy_P(&desc, &desc_SIW400G_T015_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T020_W0:
            memcpy_P(&desc, &desc_SIW400G_T020_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T020_W1:
            memcpy_P(&desc, &desc_SIW400G_T020_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T025_W0:
            memcpy_P(&desc, &desc_SIW400G_T025_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T025_W1:
            memcpy_P(&desc, &desc_SIW400G_T025_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T075_W0:
            memcpy_P(&desc, &desc_SIW400G_T075_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T100_W0:
            memcpy_P(&desc, &desc_SIW400G_T100_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T050_W00:
            memcpy_P(&desc, &desc_SIW400G_T050_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T060_W00:
            memcpy_P(&desc, &desc_SIW400G_T060_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T075_W01:
            memcpy_P(&desc, &desc_SIW400G_T075_W01, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_K015_W00:
            memcpy_P(&desc, &desc_SIW400G_K015_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_K020_W00:
            memcpy_P(&desc, &desc_SIW400G_K020_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_K025_W00:
            memcpy_P(&desc, &desc_SIW400G_K025_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_K030_W00:
            memcpy_P(&desc, &desc_SIW400G_K030_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_K037_W00:
            memcpy_P(&desc, &desc_SIW400G_K037_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW400H_T015_W10:
            memcpy_P(&desc, &desc_SIW400H_T015_W10, sizeof(InverterDescriptor));
            return desc;
        case SIW400H_T030_W10:
            memcpy_P(&desc, &desc_SIW400H_T030_W10, sizeof(InverterDescriptor));
            return desc;
        case SIW420G_K025_W00:
            memcpy_P(&desc, &desc_SIW420G_K025_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW420G_K075_W00:
            memcpy_P(&desc, &desc_SIW420G_K075_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW500G_T075_W0:
            memcpy_P(&desc, &desc_SIW500G_T075_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW500G_T100_W0:
            memcpy_P(&desc, &desc_SIW500G_T100_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW500G_K050_W00:
            memcpy_P(&desc, &desc_SIW500G_K050_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW500G_K075_W00:
            memcpy_P(&desc, &desc_SIW500G_K075_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW500G_H250_W0:
            memcpy_P(&desc, &desc_SIW500G_H250_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST012_M2:
            memcpy_P(&desc, &desc_SIW500H_ST012_M2, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST015_M2:
            memcpy_P(&desc, &desc_SIW500H_ST015_M2, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST020_M2:
            memcpy_P(&desc, &desc_SIW500H_ST020_M2, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST030_M3:
            memcpy_P(&desc, &desc_SIW500H_ST030_M3, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST036_M3:
            memcpy_P(&desc, &desc_SIW500H_ST036_M3, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST040_M3:
            memcpy_P(&desc, &desc_SIW500H_ST040_M3, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_SK020:
            memcpy_P(&desc, &desc_SIW500H_SK020, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST030:
            memcpy_P(&desc, &desc_SIW500H_ST030, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST036:
            memcpy_P(&desc, &desc_SIW500H_ST036, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST040:
            memcpy_P(&desc, &desc_SIW500H_ST040, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST060:
            memcpy_P(&desc, &desc_SIW500H_ST060, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST100:
            memcpy_P(&desc, &desc_SIW500H_ST100, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST060_HV:
            memcpy_P(&desc, &desc_SIW500H_ST060_HV, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST100_HV:
            memcpy_P(&desc, &desc_SIW500H_ST100_HV, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST200_H0:
            memcpy_P(&desc, &desc_SIW500H_ST200_H0, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST200_H3:
            memcpy_P(&desc, &desc_SIW500H_ST200_H3, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_T012_W00:
            memcpy_P(&desc, &desc_SIW500H_T012_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_T015_W00:
            memcpy_P(&desc, &desc_SIW500H_T015_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_T017_W00:
            memcpy_P(&desc, &desc_SIW500H_T017_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_T020_W00:
            memcpy_P(&desc, &desc_SIW500H_T020_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_T025_W00:
            memcpy_P(&desc, &desc_SIW500H_T025_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW600_T020_44:
            memcpy_P(&desc, &desc_SIW600_T020_44, sizeof(InverterDescriptor));
            return desc;
        case SIW610_T018_W0:
            memcpy_P(&desc, &desc_SIW610_T018_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW610_T075_W0:
            memcpy_P(&desc, &desc_SIW610_T075_W0, sizeof(InverterDescriptor));
            return desc;
        
        default:
            desc = InverterDescriptor{};
            return desc;
    }
    desc = InverterDescriptor{};
    return desc;
}