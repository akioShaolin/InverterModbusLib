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

constexpr BootMode hwCompBootMode PROGMEM =      {0x00, 0x00};
constexpr BootMode gwCompBootMode PROGMEM =      {0x00, 0x00};
constexpr BootMode fxCompBootMode PROGMEM = {0xCF, 0xCE};
constexpr PowerLimitMode hwCompPowerLimitMode PROGMEM = {0x00, 0x01};
constexpr ExportLimitMode fxCompExportLimitMode PROGMEM = {0xAA, 0x55};
constexpr ModbusConfigData gwCompModbusConfig PROGMEM {247, 9600, SERIAL_8N1};
// Weg
constexpr InverterDescriptor desc_SIW200_M030 PROGMEM =            {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 3000U,  &gwCompModbusConfig, PROTOCOL_GOODWE_COMPAT, {1, 1, {0}}, noBattery, &gwCompBootMode, &defaultPowerLimitMode,nullptr, AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200_M050 PROGMEM =            {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 5000U,  &gwCompModbusConfig, PROTOCOL_GOODWE_COMPAT, {2, 2, {0, 1}}, noBattery, &gwCompBootMode, &defaultPowerLimitMode, nullptr, AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200_M085 PROGMEM =            {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 8500U,  &gwCompModbusConfig, PROTOCOL_GOODWE_COMPAT, {3, 3, {0, 1, 2}}, noBattery, &gwCompBootMode, &defaultPowerLimitMode, nullptr, AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200_M100 PROGMEM =            {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 10000U, &gwCompModbusConfig, PROTOCOL_GOODWE_COMPAT, {3, 3, {0, 1, 2}}, noBattery, &gwCompBootMode, &defaultPowerLimitMode, nullptr, AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200H_M050_W00 PROGMEM =       {STRING, ON_GRID, SINGLE_PHASE, EPS_SINGLE_PHASE, 5000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {2, 2, {0, 1}}, {4, 1, {0, 0, 0, 0}}, &defaultBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200H_M075_W10 PROGMEM =       {STRING, ON_GRID, SINGLE_PHASE, EPS_SINGLE_PHASE, 7500U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {3, 3, {0, 1, 2}}, {4, 1, {0, 0, 0, 0}}, &defaultBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200H_M105_W10 PROGMEM =       {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 10500U, &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {4, 4, {0, 1, 2, 3}}, {4, 1, {0, 0, 0, 0}}, &defaultBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200G_M030_W0 PROGMEM =        {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 3000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {2, 2, {0, 1}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200G_M030_W1 PROGMEM =        {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 3000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {2, 2, {0, 1}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200G_M050_W0 PROGMEM =        {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 5000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {2, 2, {0, 1}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200G_M050_W1 PROGMEM =        {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 5000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {2, 2, {0, 1}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200G_M060_W0 PROGMEM =        {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 6000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {2, 2, {0, 1}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200G_M060_W1 PROGMEM =        {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 6000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {2, 2, {0, 1}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200G_M070_W1 PROGMEM =        {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 7000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {3, 3, {0, 1, 2}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200G_M075_W1 PROGMEM =        {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 7500U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {3, 3, {0, 1, 2}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200G_M080_W1 PROGMEM =        {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 8000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {3, 3, {0, 1, 2}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200G_M090_W1 PROGMEM =        {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 9000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {3, 3, {0, 1, 2}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200G_M105_W1 PROGMEM =        {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 10500U, &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {3, 3, {0, 1, 2}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW300H_M030_W00 PROGMEM =       {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 3000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {2, 2, {0, 1}}, {2, 1, {0, 0}}, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16  };
constexpr InverterDescriptor desc_SIW300H_M050_W00 PROGMEM =       {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 5000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {2, 2, {0, 1}}, {2, 1, {0, 0}}, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16  };
constexpr InverterDescriptor desc_SIW300H_M060_W00 PROGMEM =       {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 6000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {2, 2, {0, 1}}, {2, 1, {0, 0}}, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16  };
constexpr InverterDescriptor desc_SIW300H_M075_W00 PROGMEM =       {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 7500U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {3, 3, {0, 1, 2}}, {2, 1, {0, 0}}, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16  };
constexpr InverterDescriptor desc_SIW300H_M080_W00 PROGMEM =       {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 8000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {3, 3, {0, 1, 2}}, {2, 1, {0, 0}}, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16  };
constexpr InverterDescriptor desc_SIW300H_M100_W00 PROGMEM =       {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 10000U, &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {3, 3, {0, 1, 2}}, {2, 1, {0, 0}}, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16  };
constexpr InverterDescriptor desc_SIW400_ST075 PROGMEM =           {STRING, ON_GRID, THREE_PHASE, NO_EPS, 75000U,  &gwCompModbusConfig, PROTOCOL_GOODWE_COMPAT, {16, 4, {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3}}, noBattery, &gwCompBootMode, &defaultPowerLimitMode, nullptr, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T012_W0 PROGMEM =        {STRING, ON_GRID, THREE_PHASE, NO_EPS, 12000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {2, 2, {0, 1}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T012_W1 PROGMEM =        {STRING, ON_GRID, THREE_PHASE, NO_EPS, 12000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {2, 2, {0, 1}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T015_W0 PROGMEM =        {STRING, ON_GRID, THREE_PHASE, NO_EPS, 15000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {4, 2, {0, 0, 1, 1}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T015_W1 PROGMEM =        {STRING, ON_GRID, THREE_PHASE, NO_EPS, 15000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {4, 2, {0, 0, 1, 1}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T020_W0 PROGMEM =        {STRING, ON_GRID, THREE_PHASE, NO_EPS, 20000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {4, 2, {0, 0, 1, 1}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T020_W1 PROGMEM =        {STRING, ON_GRID, THREE_PHASE, NO_EPS, 20000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {4, 2, {0, 0, 1, 1}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T025_W0 PROGMEM =        {STRING, ON_GRID, THREE_PHASE, NO_EPS, 25000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {4, 2, {0, 0, 1, 1}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T025_W1 PROGMEM =        {STRING, ON_GRID, THREE_PHASE, NO_EPS, 25000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {4, 2, {0, 0, 1, 1}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T075_W0 PROGMEM =        {STRING, ON_GRID, THREE_PHASE, NO_EPS, 75000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {18, 9, {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T100_W0 PROGMEM =        {STRING, ON_GRID, THREE_PHASE, NO_EPS, 100000U, &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {18, 9, {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T050_W00 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS, 50000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {12, 6, {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T060_W00 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS, 50000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {12, 6, {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T075_W01 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS, 50000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {12, 6, {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_K015_W00 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS, 15000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {8, 4, {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_K020_W00 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS, 20000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {8, 4, {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_K025_W00 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS, 25000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {8, 4, {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_K030_W00 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS, 30000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {8, 4, {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_K037_W00 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS, 37000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {8, 4, {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400H_T015_W10 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, EPS_THREE_PHASE, 15000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {6, 3, {0, 0, 1, 1, 2, 2,}}, {8, 2, {0, 0, 0, 0, 1, 1, 1, 1}}, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400H_T030_W10 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, EPS_THREE_PHASE, 30000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {6, 3, {0, 0, 1, 1, 2, 2,}}, {8, 2, {0, 0, 0, 0, 1, 1, 1, 1}}, &fxCompBootMode, &defaultPowerLimitMode, &fxCompExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW420G_K025_W00 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS, 25000U,  &defaultModbusConfig, PROTOCOL_CHINT_COMPAT, {4, 2,   {0, 0, 1, 1}}, noBattery, &defaultBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::UNKNOWN, StatusFormat::UNKNOWN    };
constexpr InverterDescriptor desc_SIW420G_K075_W00 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS, 75000U,  &defaultModbusConfig, PROTOCOL_CHINT_COMPAT, {18, 9,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8}}, noBattery, &defaultBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::UNKNOWN, StatusFormat::UNKNOWN    };
constexpr InverterDescriptor desc_SIW500G_T075_W0 PROGMEM =        {STRING, ON_GRID, THREE_PHASE, NO_EPS, 75000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {20, 10, {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9}}, noBattery, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500G_T100_W0 PROGMEM =        {STRING, ON_GRID, THREE_PHASE, NO_EPS, 100000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {20, 10, {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9}}, noBattery, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500G_K050_W00 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS, 50000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {21, 7,  {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6}}, noBattery, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500G_K075_W00 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS, 75000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {21, 7,  {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6}}, noBattery, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500G_H250_W0 PROGMEM =        {STRING, ON_GRID, THREE_PHASE, NO_EPS, 250000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {28, 6,  {0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5}}, noBattery, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST012_M2 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS, 12000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {4, 2,   {0, 0, 1, 1}}, noBattery, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST015_M2 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS, 15000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {4, 2,   {0, 0, 1, 1}}, noBattery, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST020_M2 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS, 20000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {4, 2,   {0, 0, 1, 1}}, noBattery, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST030_M3 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS, 30000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST036_M3 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS, 36000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST040_M3 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS, 40000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_SK020 PROGMEM =          {STRING, ON_GRID, THREE_PHASE, NO_EPS, 20000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST030 PROGMEM =          {STRING, ON_GRID, THREE_PHASE, NO_EPS, 30000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST036 PROGMEM =          {STRING, ON_GRID, THREE_PHASE, NO_EPS, 36000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST040 PROGMEM =          {STRING, ON_GRID, THREE_PHASE, NO_EPS, 40000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST060 PROGMEM =          {STRING, ON_GRID, THREE_PHASE, NO_EPS, 60000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {12, 6,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}}, noBattery, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST100 PROGMEM =          {STRING, ON_GRID, THREE_PHASE, NO_EPS, 100000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {20, 10, {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9}}, noBattery, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST200_H3 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS, 200000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {18, 9,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8}}, noBattery, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST200_H0 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS, 200000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {14, 3,  {0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2}}, noBattery, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST060_HV PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS, 600000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST100_HV PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS, 100000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {12, 6,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}}, noBattery, &hwCompBootMode, &hwCompPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_T012_W00 PROGMEM =       {STRING, HYBRID, THREE_PHASE, NO_EPS,  12000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {4, 2,   {0, 0, 1, 1}}, {4, 2, {0, 0, 1, 1}}, &defaultBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_T015_W00 PROGMEM =       {STRING, HYBRID, THREE_PHASE, NO_EPS,  15000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {4, 2,   {0, 0, 1, 1}}, {4, 2, {0, 0, 1, 1}}, &defaultBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_T017_W00 PROGMEM =       {STRING, HYBRID, THREE_PHASE, NO_EPS,  17000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {4, 2,   {0, 0, 1, 1}}, {4, 2, {0, 0, 1, 1}}, &defaultBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_T020_W00 PROGMEM =       {STRING, HYBRID, THREE_PHASE, NO_EPS,  20000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {4, 2,   {0, 0, 1, 1}}, {4, 2, {0, 0, 1, 1}}, &defaultBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_T025_W00 PROGMEM =       {STRING, HYBRID, THREE_PHASE, NO_EPS,  25000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {4, 2,   {0, 0, 1, 1}}, {4, 2, {0, 0, 1, 1}}, &defaultBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW600_T020_44 PROGMEM =         {STRING, ON_GRID, THREE_PHASE, NO_EPS, 20000U,  &defaultModbusConfig, PROTOCOL_WEG_COMPAT, {4, 2,   {0, 0, 1, 1}}, noBattery, &defaultBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW610_T018_W0 PROGMEM =         {STRING, ON_GRID, THREE_PHASE, NO_EPS, 18000U,  &defaultModbusConfig, PROTOCOL_WEG_COMPAT, {4, 2,   {0, 0, 1, 1}}, noBattery, &defaultBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW610_T075_W0 PROGMEM =         {STRING, ON_GRID, THREE_PHASE, NO_EPS, 75000U,  &defaultModbusConfig, PROTOCOL_WEG_COMPAT, {12, 6,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}}, noBattery, &defaultBootMode, &defaultPowerLimitMode, &defaultExportLimitMode,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };

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
            memset(&desc, 0, sizeof(InverterDescriptor));
            return desc;
    }
    memset(&desc, 0, sizeof(InverterDescriptor));
    return desc;
}