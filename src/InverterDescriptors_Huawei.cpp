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

constexpr BootMode hwBootMode PROGMEM =      {0x01, 0x01};

// Huawei
constexpr InverterDescriptor desc_SUN2000_2KTL PROGMEM =           {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 2000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {2, 2,   {0, 1}}, noBattery, &hwBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SUN2000_3KTL PROGMEM =           {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 3000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {2, 2,   {0, 1}}, noBattery, &hwBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SUN2000_4KTL PROGMEM =           {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 4000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {2, 2,   {0, 1}}, noBattery, &hwBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SUN2000_5KTL PROGMEM =           {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 5000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {2, 2,   {0, 1}}, noBattery, &hwBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SUN2000_6KTL PROGMEM =           {STRING, ON_GRID, SINGLE_PHASE, NO_EPS,  6000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {2, 2,   {0, 1}}, noBattery, &hwBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SUN2000_12KTL_M0 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS,   12000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {4, 2,   {0, 0, 1, 1}}, noBattery, &hwBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_12KTL_M2 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS,   12000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {4, 2,   {0, 0, 1, 1}}, noBattery, &hwBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_15KTL_M0 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS,   15000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {4, 2,   {0, 0, 1, 1}}, noBattery, &hwBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_15KTL_M2 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS,  15000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {4, 2,   {0, 0, 1, 1}}, noBattery, &hwBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_20KTL_M0 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS,  20000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {4, 2,   {0, 0, 1, 1}}, noBattery, &hwBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_20KTL_M2 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS,  20000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {4, 2,   {0, 0, 1, 1}}, noBattery, &hwBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_33KTL_A PROGMEM =        {STRING, ON_GRID, THREE_PHASE, NO_EPS,  33000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &hwBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_36KTL_A PROGMEM =        {STRING, ON_GRID, THREE_PHASE, NO_EPS,  36000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &hwBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_30KTL_M3 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS,  30000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &hwBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_36KTL_M3 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS,  36000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &hwBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_40KTL_M3 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS,  40000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &hwBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_60KTL_M0 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS,  60000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {12, 6,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}}, noBattery, &hwBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_75KTL_M1 PROGMEM =       {STRING, ON_GRID, THREE_PHASE, NO_EPS,  75000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {20, 10, {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9}}, noBattery, &hwBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_100KTL_M2 PROGMEM =      {STRING, ON_GRID, THREE_PHASE, NO_EPS,  100000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {20, 10, {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9}}, noBattery, &hwBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_100KTL_H1 PROGMEM =      {STRING, ON_GRID, THREE_PHASE, NO_EPS,  100000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {12, 6,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}}, noBattery, &hwBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_215KTL_H0 PROGMEM =      {STRING, ON_GRID, THREE_PHASE, NO_EPS,  215000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {18, 9,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8}}, noBattery, &hwBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_215KTL_H3 PROGMEM =      {STRING, ON_GRID, THREE_PHASE, NO_EPS,  215000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {14, 3,  {0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2}}, noBattery, &hwBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_250KTL_H1 PROGMEM =      {STRING, ON_GRID, THREE_PHASE, NO_EPS,  250000U,  &defaultModbusConfig, PROTOCOL_HUAWEI_COMPAT, {28, 6,  {0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5}}, noBattery, &hwBootMode, &defaultPowerLimitMode, &defaultExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };

const InverterDescriptor getDescriptor_Huawei(InverterModel model){
    InverterDescriptor desc;

    switch(model) {
        case SUN2000_2KTL:
            memcpy_P(&desc, &desc_SUN2000_2KTL, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_3KTL:
            memcpy_P(&desc, &desc_SUN2000_3KTL, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_4KTL:
            memcpy_P(&desc, &desc_SUN2000_4KTL, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_5KTL:
            memcpy_P(&desc, &desc_SUN2000_5KTL, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_6KTL:
            memcpy_P(&desc, &desc_SUN2000_6KTL, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_12KTL_M0:
            memcpy_P(&desc, &desc_SUN2000_12KTL_M0, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_12KTL_M2:
            memcpy_P(&desc, &desc_SUN2000_12KTL_M2, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_15KTL_M0:
            memcpy_P(&desc, &desc_SUN2000_15KTL_M0, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_15KTL_M2:
            memcpy_P(&desc, &desc_SUN2000_15KTL_M2, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_20KTL_M0:
            memcpy_P(&desc, &desc_SUN2000_20KTL_M0, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_20KTL_M2:
            memcpy_P(&desc, &desc_SUN2000_20KTL_M2, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_30KTL:
            memcpy_P(&desc, &desc_SUN2000_33KTL_A, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_36KTL_A:
            memcpy_P(&desc, &desc_SUN2000_36KTL_A, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_30KTL_M3:
            memcpy_P(&desc, &desc_SUN2000_30KTL_M3, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_36KTL_M3:
            memcpy_P(&desc, &desc_SUN2000_36KTL_M3, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_40KTL_M3:
            memcpy_P(&desc, &desc_SUN2000_40KTL_M3, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_60KTL_M0:
            memcpy_P(&desc, &desc_SUN2000_60KTL_M0, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_75KTL_M1:
            memcpy_P(&desc, &desc_SUN2000_75KTL_M1, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_100KTL_M2:
            memcpy_P(&desc, &desc_SUN2000_100KTL_M2, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_100KTL_H1:
            memcpy_P(&desc, &desc_SUN2000_100KTL_H1, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_215KTL_H0:
            memcpy_P(&desc, &desc_SUN2000_215KTL_H0, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_215KTL_H3:
            memcpy_P(&desc, &desc_SUN2000_215KTL_H3, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_250KTL_H1:
            memcpy_P(&desc, &desc_SUN2000_250KTL_H1, sizeof(InverterDescriptor));
            return desc;

        default:
            memset(&desc, 0, sizeof(InverterDescriptor));
            return desc;
    }
    memset(&desc, 0, sizeof(InverterDescriptor));
    return desc;
}