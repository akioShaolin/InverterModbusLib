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

constexpr ModbusConfigData gwModbusConfig PROGMEM {247, 9600, SERIAL_8N1};

//Goodwe
constexpr InverterDescriptor desc_GW3000_XS PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    3000U,  
    &gwModbusConfig, 
    {1, 1,   {0}}, 
    noBattery, 
    AlarmFormat::BITFIELD32, 
    StatusFormat::ENUM_CODE  

};
constexpr InverterDescriptor desc_GW3300_XS_30 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    3300U,  
    &gwModbusConfig, 
    {1, 1,   {0}}, 
    noBattery, 
    AlarmFormat::BITFIELD32, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_GW5000_DNS PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    5000U,  
    &gwModbusConfig, 
    {2, 2,   {0, 1}}, 
    noBattery, 
    AlarmFormat::BITFIELD32, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_GW5K_DNS_G40 PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    5000U,  
    &gwModbusConfig, 
    {2, 2,   {0, 1}}, 
    noBattery, 
    AlarmFormat::BITFIELD32, 
    StatusFormat::ENUM_CODE  
};  

constexpr InverterDescriptor desc_GW8500_MS PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    8500U,  
    &gwModbusConfig, 
    {2, 2,   {0, 1}}, 
    noBattery, 
    AlarmFormat::BITFIELD32, 
    StatusFormat::ENUM_CODE
};

constexpr InverterDescriptor desc_GW10K_MS PROGMEM = {
    STRING, 
    ON_GRID, 
    SINGLE_PHASE, 
    10000U,  
    &gwModbusConfig, 
    {2, 2,   {0, 1}}, 
    noBattery, 
    AlarmFormat::BITFIELD32, 
    StatusFormat::ENUM_CODE  
};

constexpr InverterDescriptor desc_GW75K_MT PROGMEM = {
    STRING, 
    ON_GRID, 
    THREE_PHASE, 
    75000U, 
    &gwModbusConfig, 
    {16, 4, {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3}}, 
    noBattery, 
    AlarmFormat::BITFIELD32, 
    StatusFormat::ENUM_CODE  
};

const InverterDescriptor getDescriptor_GoodWe(InverterModel model) {
    InverterDescriptor desc;

    switch (model) {

        case GW3000_XS:
            memcpy_P(&desc, &desc_GW3000_XS, sizeof(InverterDescriptor));
            return desc;
        case GW3300_XS_30:
            memcpy_P(&desc, &desc_GW3300_XS_30, sizeof(InverterDescriptor));
            return desc;
        case GW5000_DNS:
            memcpy_P(&desc, &desc_GW5000_DNS, sizeof(InverterDescriptor));
            return desc;
        case GW5K_DNS_G40:
            memcpy_P(&desc, &desc_GW5K_DNS_G40, sizeof(InverterDescriptor));
            return desc;
        case GW8500_MS:
            memcpy_P(&desc, &desc_GW8500_MS, sizeof(InverterDescriptor));
            return desc;
        case GW10K_MS:
            memcpy_P(&desc, &desc_GW10K_MS, sizeof(InverterDescriptor));
            return desc;

        default:
            memset(&desc, 0, sizeof(InverterDescriptor));
            return desc;
    }
    memset(&desc, 0, sizeof(InverterDescriptor));
    return desc;
}