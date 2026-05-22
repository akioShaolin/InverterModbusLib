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

constexpr BootMode fx_RVH3BootMode PROGMEM = {0xCF, 0xCE};
constexpr PowerLimitMode foxEssPowerLimitMode PROGMEM = {0x00, 0x01};
constexpr ExportLimitMode fx_RVH3ExportLimitMode PROGMEM = {0xAA, 0x55};

// Foxess
constexpr InverterDescriptor desc_S700_G2 PROGMEM =                {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 700U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {1, 1,   {0}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_S1000_G2 PROGMEM =               {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 1000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {1, 1,   {0}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_S1500_G2 PROGMEM =               {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 1500U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {1, 1,   {0}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_S2000_G2 PROGMEM =               {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 2000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {1, 1,   {0}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_S2500_G2 PROGMEM =               {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 2500U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {1, 1,   {0}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };;
constexpr InverterDescriptor desc_S3000_G2 PROGMEM =               {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 3000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {1, 1,   {0}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_S3300_G2 PROGMEM =               {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 3300U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {1, 1,   {0}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_F3000_G2 PROGMEM =               {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 3000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {2, 2,   {0, 1}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_F3600_G2 PROGMEM =               {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 3600U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {2, 2,   {0, 1}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_F4600_G2 PROGMEM =               {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 4600U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {2, 2,   {0, 1}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_F5000_G2 PROGMEM =               {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 5000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {2, 2,   {0, 1}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_F5300_G2 PROGMEM =               {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 5300U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {2, 2,   {0, 1}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_F6000_G2 PROGMEM =               {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 6000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {2, 2,   {0, 1}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_G7 PROGMEM =                     {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 7000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {3, 3,   {0, 1, 2}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_G7_5 PROGMEM =                   {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 7500U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {3, 3,   {0, 1, 2}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_G8 PROGMEM =                     {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 8000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {3, 3,   {0, 1, 2}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_G9 PROGMEM =                     {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 9000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {3, 3,   {0, 1, 2}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_G10 PROGMEM =                    {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 10000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {3, 3,   {0, 1, 2}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_G10_5 PROGMEM =                  {STRING, ON_GRID, SINGLE_PHASE, NO_EPS, 10500U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {3, 3,   {0, 1, 2}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T3_G3 PROGMEM =                  {STRING, ON_GRID, THREE_PHASE, NO_EPS, 3000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {2, 2,   {0, 1}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T4_G3 PROGMEM =                  {STRING, ON_GRID, THREE_PHASE, NO_EPS, 4000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {2, 2,   {0, 1}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T5_G3 PROGMEM =                  {STRING, ON_GRID, THREE_PHASE, NO_EPS, 5000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {2, 2,   {0, 1}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T6_G3 PROGMEM =                  {STRING, ON_GRID, THREE_PHASE, NO_EPS, 6000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {2, 2,   {0, 1}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T8_G3 PROGMEM =                  {STRING, ON_GRID, THREE_PHASE, NO_EPS, 8000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {2, 2,   {0, 1}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T10_G3 PROGMEM =                 {STRING, ON_GRID, THREE_PHASE, NO_EPS, 10000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {2, 2,   {0, 1}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T12_G3 PROGMEM =                 {STRING, ON_GRID, THREE_PHASE, NO_EPS, 12000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {2, 2,   {0, 1}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T15_G3 PROGMEM =                 {STRING, ON_GRID, THREE_PHASE, NO_EPS, 15000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {4, 2,   {0, 0, 1, 1}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T17_G3 PROGMEM =                 {STRING, ON_GRID, THREE_PHASE, NO_EPS, 17000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {4, 2,   {0, 0, 1, 1}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T20_G3 PROGMEM =                 {STRING, ON_GRID, THREE_PHASE, NO_EPS, 20000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {4, 2,   {0, 0, 1, 1}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T23_G3 PROGMEM =                 {STRING, ON_GRID, THREE_PHASE, NO_EPS, 23000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {4, 2,   {0, 0, 1, 1}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T25_G3 PROGMEM =                 {STRING, ON_GRID, THREE_PHASE, NO_EPS, 25000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {4, 2,   {0, 0, 1, 1}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_V30 PROGMEM =                    {STRING, ON_GRID, THREE_PHASE, NO_EPS, 30000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_V33 PROGMEM =                    {STRING, ON_GRID, THREE_PHASE, NO_EPS, 33000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_V36 PROGMEM =                    {STRING, ON_GRID, THREE_PHASE, NO_EPS, 36000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_V40 PROGMEM =                    {STRING, ON_GRID, THREE_PHASE, NO_EPS, 40000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_V50 PROGMEM =                    {STRING, ON_GRID, THREE_PHASE, NO_EPS, 50000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_V60 PROGMEM =                    {STRING, ON_GRID, THREE_PHASE, NO_EPS, 60000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {12, 6,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_V75 PROGMEM =                    {STRING, ON_GRID, THREE_PHASE, NO_EPS, 75000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {12, 6,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_VL15 PROGMEM =                   {STRING, ON_GRID, THREE_PHASE, NO_EPS, 15000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_VL20 PROGMEM =                   {STRING, ON_GRID, THREE_PHASE, NO_EPS, 20000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_VL25 PROGMEM =                   {STRING, ON_GRID, THREE_PHASE, NO_EPS, 25000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_VL30 PROGMEM =                   {STRING, ON_GRID, THREE_PHASE, NO_EPS, 30000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_VL37_5 PROGMEM =                 {STRING, ON_GRID, THREE_PHASE, NO_EPS, 37500U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_R75 PROGMEM =                    {STRING, ON_GRID, THREE_PHASE, NO_EPS, 75000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {18, 9,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_R100 PROGMEM =                   {STRING, ON_GRID, THREE_PHASE, NO_EPS, 100000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {18, 9,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_R110 PROGMEM =                   {STRING, ON_GRID, THREE_PHASE, NO_EPS, 110000U,  &defaultModbusConfig, PROTOCOL_FOXESS_COMPAT, {18, 9,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8}}, noBattery, &fx_RVH3BootMode, &foxEssPowerLimitMode, &fx_RVH3ExportLimitMode, AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };

const InverterDescriptor getDescriptor_FoxEss(InverterModel model) {
    InverterDescriptor desc;

    switch (model) {
        case S700_G2:
            memcpy_P(&desc, &desc_S700_G2, sizeof(InverterDescriptor));
            return desc;
        case S1000_G2:
            memcpy_P(&desc, &desc_S1000_G2, sizeof(InverterDescriptor));
            return desc;
        case S1500_G2:
            memcpy_P(&desc, &desc_S1500_G2, sizeof(InverterDescriptor));
            return desc;
        case S2000_G2:
            memcpy_P(&desc, &desc_S2000_G2, sizeof(InverterDescriptor));
            return desc;
        case S2500_G2:
            memcpy_P(&desc, &desc_S2500_G2, sizeof(InverterDescriptor));
            return desc;
        case S3000_G2:
            memcpy_P(&desc, &desc_S3000_G2, sizeof(InverterDescriptor));
            return desc;
        case S3300_G2:
            memcpy_P(&desc, &desc_S3300_G2, sizeof(InverterDescriptor));
            return desc;
        case F3000_G2:
            memcpy_P(&desc, &desc_F3000_G2, sizeof(InverterDescriptor));
            return desc;
        case F3600_G2:
            memcpy_P(&desc, &desc_F3600_G2, sizeof(InverterDescriptor));
            return desc;
        case F4600_G2:
            memcpy_P(&desc, &desc_F4600_G2, sizeof(InverterDescriptor));
            return desc;
        case F5000_G2:
            memcpy_P(&desc, &desc_F5000_G2, sizeof(InverterDescriptor));
            return desc;
        case F5300_G2:
            memcpy_P(&desc, &desc_F5300_G2, sizeof(InverterDescriptor));
            return desc;
        case F6000_G2:
            memcpy_P(&desc, &desc_F6000_G2, sizeof(InverterDescriptor));
            return desc;
        case G7:
            memcpy_P(&desc, &desc_G7, sizeof(InverterDescriptor));
            return desc;
        case G7_5:
            memcpy_P(&desc, &desc_G7_5, sizeof(InverterDescriptor));
            return desc;
        case G8_:
            memcpy_P(&desc, &desc_G8, sizeof(InverterDescriptor));
            return desc;
        case G9:
            memcpy_P(&desc, &desc_G9, sizeof(InverterDescriptor));
            return desc;
        case G10:
            memcpy_P(&desc, &desc_G10, sizeof(InverterDescriptor));
            return desc;
        case G10_5:
            memcpy_P(&desc, &desc_G10_5, sizeof(InverterDescriptor));
            return desc;
        case T3_G3:
            memcpy_P(&desc, &desc_T3_G3, sizeof(InverterDescriptor));
            return desc;
        case T4_G3:
            memcpy_P(&desc, &desc_T4_G3, sizeof(InverterDescriptor));
            return desc;
        case T5_G3:
            memcpy_P(&desc, &desc_T5_G3, sizeof(InverterDescriptor));
            return desc;
        case T6_G3:
            memcpy_P(&desc, &desc_T6_G3, sizeof(InverterDescriptor));
            return desc;
        case T8_G3:
            memcpy_P(&desc, &desc_T8_G3, sizeof(InverterDescriptor));
            return desc;
        case T10_G3:
            memcpy_P(&desc, &desc_T10_G3, sizeof(InverterDescriptor));
            return desc;
        case T12_G3:
            memcpy_P(&desc, &desc_T12_G3, sizeof(InverterDescriptor));
            return desc;
        case T15_G3:
            memcpy_P(&desc, &desc_T15_G3, sizeof(InverterDescriptor));
            return desc;
        case T17_G3:
            memcpy_P(&desc, &desc_T17_G3, sizeof(InverterDescriptor));
            return desc;
        case T20_G3:
            memcpy_P(&desc, &desc_T20_G3, sizeof(InverterDescriptor));
            return desc;
        case T23_G3:
            memcpy_P(&desc, &desc_T23_G3, sizeof(InverterDescriptor));
            return desc;
        case T25_G3:
            memcpy_P(&desc, &desc_T25_G3, sizeof(InverterDescriptor));
            return desc;
        case V30:
            memcpy_P(&desc, &desc_V30, sizeof(InverterDescriptor));
            return desc;
        case V33:
            memcpy_P(&desc, &desc_V33, sizeof(InverterDescriptor));
            return desc;
        case V36:
            memcpy_P(&desc, &desc_V36, sizeof(InverterDescriptor));
            return desc;
        case V40:
            memcpy_P(&desc, &desc_V40, sizeof(InverterDescriptor));
            return desc;
        case V50:
            memcpy_P(&desc, &desc_V50, sizeof(InverterDescriptor));
            return desc;
        case V60:
            memcpy_P(&desc, &desc_V60, sizeof(InverterDescriptor));
            return desc;
        case V75:
            memcpy_P(&desc, &desc_V75, sizeof(InverterDescriptor));
            return desc;
        case VL15:
            memcpy_P(&desc, &desc_VL15, sizeof(InverterDescriptor));
            return desc;
        case VL20:
            memcpy_P(&desc, &desc_VL20, sizeof(InverterDescriptor));
            return desc;
        case VL25:
            memcpy_P(&desc, &desc_VL25, sizeof(InverterDescriptor));
            return desc;
        case VL30:
            memcpy_P(&desc, &desc_VL30, sizeof(InverterDescriptor));
            return desc;
        case VL37_5:
            memcpy_P(&desc, &desc_VL37_5, sizeof(InverterDescriptor));
            return desc;
        case R75:
            memcpy_P(&desc, &desc_R75, sizeof(InverterDescriptor));
            return desc;
        case R100:
            memcpy_P(&desc, &desc_R100, sizeof(InverterDescriptor));
            return desc;
        case R110:
            memcpy_P(&desc, &desc_R110, sizeof(InverterDescriptor));
            return desc;

        default:
            memset(&desc, 0, sizeof(InverterDescriptor));
            return desc;
    }

    memset(&desc, 0, sizeof(InverterDescriptor));
    return desc;

}