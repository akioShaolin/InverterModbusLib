/*
 * InverterModbusLib - Solar Inverter Library for Arduino
 * ------------------------------------------------
 * Modbus RTU (RS485)communication layer for inverter integration
 *
 * Copyright (c) 2026, Pedro Akio Sakuma
 * Licensed under BSD 3-Clause License
 */

#include "Inverter.h"
// ======================================================
// Internal Helpers
// ======================================================

// ======================================================
// Internal Conversions
// ======================================================

Datetime Inverter::epochToDatetime(uint32_t epoch) {
    Datetime dt;

    uint32_t seconds = epoch;

    dt.second = seconds % 60;
    seconds /= 60;

    dt.minute = seconds % 60;
    seconds /= 60;

    dt.hour = seconds % 24;
    uint32_t days = seconds / 24;

    uint16_t year = 1970;

    while (true) {
        uint16_t dy = isLeap(year) ? 366 : 365;
        if (days >= dy) {
            days -= dy;
            year++;
        } else {
            break;
        }
    }

    dt.year = year;

    static const uint8_t daysInMonth[] = {
        31,28,31,30,31,30,31,31,30,31,30,31
    };

    uint8_t month = 0;

    while (true) {
        uint8_t dim = daysInMonth[month];

        if (month == 1 && isLeap(year)) {
            dim = 29;
        }

        if (days >= dim) {
            days -= dim;
            month++;
        } else {
            break;
        }
    }

    dt.month = month + 1;
    dt.day = days + 1;

    return dt;
}

bool Inverter::isValidDatetime(const Datetime& dt) {
    if (dt.year < 1970 || dt.year > 2100) return false;
    if (dt.month < 1 || dt.month > 12) return false;
    if (dt.hour > 23) return false;
    if (dt.minute > 59) return false;
    if (dt.second > 59) return false;

    static const uint8_t daysInMonth[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    uint8_t maxDay = daysInMonth[dt.month - 1];

    if (dt.month == 2 && isLeap(dt.year)) {
        maxDay = 29;
    }

    if (dt.day < 1 || dt.day > maxDay) return false;

    return true;
}

uint32_t Inverter::datetimeToEpoch(const Datetime& dt) {
    uint32_t days = 0;

    // Anos completos desde 1970
    for (uint16_t y = 1970; y < dt.year; y++) {
        days += isLeap(y) ? 366UL : 365UL;
    }

    // Meses completos do ano atual
    static const uint8_t daysInMonth[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    for (uint8_t m = 1; m < dt.month; m++) {
        if (m == 2 && isLeap(dt.year)) {
            days += 29;
        } else {
            days += daysInMonth[m - 1];
        }
    }

    // Dias completos do mês atual
    days += (dt.day - 1);

    uint32_t epoch = days * 86400UL;
    epoch += (uint32_t)dt.hour * 3600UL;
    epoch += (uint32_t)dt.minute * 60UL;
    epoch += dt.second;

    return epoch;
}

bool Inverter::hasValidMap() const {
    return _map.identification.serialNumber.address != 0xFFFF;
}

bool Inverter::isInvalidField(const ModbusField& field) {
    return field.address == 0xFFFF;
}

bool Inverter::isLeap(uint16_t y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

