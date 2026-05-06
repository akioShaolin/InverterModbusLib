/*
 * InverterModbusLib - Solar Inverter Library for Arduino
 * ------------------------------------------------
 * Modbus RTU (RS485)communication layer for inverter integration
 *
 * Copyright (c) 2026, Pedro Akio Sakuma
 * Licensed under BSD 3-Clause License
 */
 /*
InverterTime.cpp
├── Leitura de data/hora
│   ├── getDatetime()
│   ├── getYear()
│   ├── getMonth()
│   ├── getDay()
│   ├── getHour()
│   ├── getMinute()
│   ├── getSecond()
│   └── getEpochTime()
│
├── Escrita de data/hora
│   ├── setDatetime()
│   ├── setYear()
│   ├── setMonth()
│   ├── setDay()
│   ├── setHour()
│   ├── setMinute()
│   ├── setSecond()
│   └── setEpochTime()
│
└── Conversões internas
    ├── datetimeToEpoch()
    └── epochToDatetime()
*/

#include "Inverter.h"

Datetime datetime;

// ======================================================
// Date/Time Read
// ======================================================

bool Inverter::getDatetime(Datetime& dt) {
    if (_map.serial.address == 0xFFFF) return false;

    if (_map.time_epoch.readable) {
        uint32_t epoch;
        if (!readField(_map.time_epoch, &epoch)) return false;

        dt = epochToDatetime(epoch);
        return true;
    }
    // Inserir leitura por leitura e Goodwe
    // fallback futuro: campos separados
    return false;
}

bool Inverter::getYear(uint16_t& year) {
    if (_map.serial.address == 0xFFFF) return false;

    if (_map.time_year.readable) {
        uint16_t y;
        if (!readField(_map.time_year, &y)) return false;

        if (_map.time_year.handlerId == GOODWE_HANDLER) {
            y = y >> 8;
        }

        if (y < 1970 || y > 2100) return false;

        year = y;
        return true;
    }

    if (_map.time_epoch.readable) {
        uint32_t epoch;
        if (!readField(_map.time_epoch, &epoch)) return false;

        datetime = epochToDatetime(epoch);
        year = datetime.year;
        return true;
    }
    return false;
}

bool Inverter::getMonth(uint16_t& month) {
    if (_map.serial.address == 0xFFFF) return false;

    if (_map.time_month.readable) {
        uint16_t m;
        if (!readField(_map.time_month, &m)) return false;

        if (_map.time_month.handlerId == GOODWE_HANDLER) {
            m = m & 0xFF;
        }
        
        if (m < 1 || m > 12) return false;

        month = m;
        return true;
        }

    if (_map.time_epoch.readable) {
        uint32_t epoch;
        if (!readField(_map.time_epoch, &epoch)) return false;

        datetime = epochToDatetime(epoch);
        month = datetime.month;
        return true;
    }
    return false;
}

bool Inverter::getDay(uint16_t& day) {
    if (_map.serial.address == 0xFFFF) return false;

    if (_map.time_day.readable) {
        uint16_t d;
        if (!readField(_map.time_day, &d)) return false;

        if (_map.time_day.handlerId == GOODWE_HANDLER) {
            d = d >> 8;
        }

        if (d < 1 || d > 31) return false;

        day = d;
        return true;
    }

    if (_map.time_epoch.readable) {
        uint32_t epoch;
        if (!readField(_map.time_epoch, &epoch)) return false;

        datetime = epochToDatetime(epoch);
        day = datetime.day;
        return true;
    }
    return false;
}

bool Inverter::getHour(uint16_t& hour) {
    if (_map.serial.address == 0xFFFF) return false;

    if (_map.time_hour.readable) {
        uint16_t h;
        if (!readField(_map.time_hour, &h)) return false;

        if (_map.time_hour.handlerId == GOODWE_HANDLER) {
            h = h & 0xFF;
        }
        
        if (h > 23) return false;

        hour = h;
        return true;
        }

    if (_map.time_epoch.readable) {
        uint32_t epoch;
        if (!readField(_map.time_epoch, &epoch)) return false;

        datetime = epochToDatetime(epoch);
        hour = datetime.hour;
        return true;
    }
    return false;
}

bool Inverter::getMinute(uint16_t& minute) {
    if (_map.serial.address == 0xFFFF) return false;

    if (_map.time_minute.readable) {
        uint16_t m;
        if (!readField(_map.time_minute, &m)) return false;

        if (_map.time_minute.handlerId == GOODWE_HANDLER) {
            m = m >> 8;
        }

        if (m > 59) return false;

        minute = m;
        return true;
    }

    if (_map.time_epoch.readable) {
        uint32_t epoch;
        if (!readField(_map.time_epoch, &epoch)) return false;

        datetime = epochToDatetime(epoch);
        minute = datetime.minute;
        return true;
    }
    return false;
}

bool Inverter::getSecond(uint16_t& second) {
    if (_map.serial.address == 0xFFFF) return false;

    if (_map.time_second.readable) {
        uint16_t s;
        if (!readField(_map.time_second, &s)) return false;

        if (_map.time_second.handlerId == GOODWE_HANDLER) {
            s = s & 0xFF;
        }
        
        if (s > 59) return false;

        second = s;
        return true;
        }

    if (_map.time_epoch.readable) {
        uint32_t epoch;
        if (!readField(_map.time_epoch, &epoch)) return false;

        datetime = epochToDatetime(epoch);
        second = datetime.second;
        return true;
    }
    return false;
}

bool Inverter::getEpochTime(uint32_t& epoch) {
    if (_map.serial.address == 0xFFFF) return false;

    if (!_map.time_epoch.readable) return false;

    return readField(_map.time_epoch, &epoch);
}

// ======================================================
// Date/Time Write
// ======================================================

bool Inverter::setDatetime(Datetime dt) {
    if (_map.serial.address == 0xFFFF) return false;
    if (!isValidDatetime(dt)) return false;

    if (_map.time_epoch.writable) {
        uint32_t epoch = datetimeToEpoch(dt);
        return writeField(_map.time_epoch, epoch);
    }

    // fallback futuro: escrever campos separados
    return false;
}

bool Inverter::setYear(uint16_t year) {
    if (_map.serial.address == 0xFFFF) return false;
    if (year < 1970 || year > 2100) return false;

    // Caso 1: registrador direto
    if (_map.time_year.writable) {
        if (_map.time_year.handlerId == GOODWE_HANDLER) {
            uint16_t reg;

            // ler valor atual (contém month junto)
            if (!readField(_map.time_year, &reg)) return false;

            // preservar LSB (month)
            uint16_t lsb = reg & 0x00FF;

            // colocar ano no MSB
            uint16_t newVal = (year << 8) | lsb;

            return writeField(_map.time_year, newVal);
        }

        return writeField(_map.time_year, year);
    }

    // Caso 2: fallback para epoch
    if (_map.time_epoch.writable) {
        Datetime dt;

        if (!getDatetime(dt)) return false;

        dt.year = year;
        if (!isValidDatetime(dt)) return false;

        uint32_t epoch = datetimeToEpoch(dt);
        return writeField(_map.time_epoch, epoch);
    }

    return false;
}

bool Inverter::setMonth(uint16_t month) {
    if (_map.serial.address == 0xFFFF) return false;
    if (month < 1 || month > 12) return false;

    // Caso 1: registrador direto
    if (_map.time_month.writable) {
        if (_map.time_month.handlerId == GOODWE_HANDLER) {
            uint16_t reg;

            // ler valor atual (contém year junto)
            if (!readField(_map.time_month, &reg)) return false;

            // preservar MSB (year)
            uint16_t msb = reg & 0xFF00;

            // colocar mês no LSB
            uint16_t newVal = msb | (month & 0x00FF);

            return writeField(_map.time_month, newVal);
        }

        return writeField(_map.time_month, month);
    }

    // Caso 2: fallback para epoch
    if (_map.time_epoch.writable) {
        Datetime dt;

        if (!getDatetime(dt)) return false;

        dt.month = month;
        if (!isValidDatetime(dt)) return false;

        uint32_t epoch = datetimeToEpoch(dt);
        return writeField(_map.time_epoch, epoch);
    }

    return false;
}

bool Inverter::setDay(uint16_t day) {
    if (_map.serial.address == 0xFFFF) return false;
    if (day < 1 || day > 31) return false;

    // Caso 1: registrador direto
    if (_map.time_day.writable) {
        if (_map.time_day.handlerId == GOODWE_HANDLER) {
            uint16_t reg;

            // ler valor atual (contém hour junto)
            if (!readField(_map.time_day, &reg)) return false;

            // preservar LSB (hour)
            uint16_t lsb = reg & 0x00FF;

            // colocar dia no MSB
            uint16_t newVal = (day << 8) | lsb;

            return writeField(_map.time_day, newVal);
        }

        return writeField(_map.time_day, day);
    }

    // Caso 2: fallback para epoch
    if (_map.time_epoch.writable) {
        Datetime dt;

        if (!getDatetime(dt)) return false;

        dt.day = day;
        if (!isValidDatetime(dt)) return false;

        uint32_t epoch = datetimeToEpoch(dt);
        return writeField(_map.time_epoch, epoch);
    }

    return false;
}

bool Inverter::setHour(uint16_t hour) {
    if (_map.serial.address == 0xFFFF) return false;
    if (hour > 23) return false;

    // Caso 1: registrador direto
    if (_map.time_hour.writable) {
        if (_map.time_hour.handlerId == GOODWE_HANDLER) {
            uint16_t reg;

            // ler valor atual (contém day junto)
            if (!readField(_map.time_hour, &reg)) return false;

            // preservar MSB (day)
            uint16_t msb = reg & 0xFF00;

            // colocar hour no LSB
            uint16_t newVal = msb | (hour & 0x00FF);

            return writeField(_map.time_hour, newVal);
        }

        return writeField(_map.time_hour, hour);
    }

    // Caso 2: fallback para epoch
    if (_map.time_epoch.writable) {
        Datetime dt;

        if (!getDatetime(dt)) return false;

        dt.hour = hour;
        if (!isValidDatetime(dt)) return false;
        
        uint32_t epoch = datetimeToEpoch(dt);
        return writeField(_map.time_epoch, epoch);
    }

    return false;
}

bool Inverter::setMinute(uint16_t minute) {
    if (_map.serial.address == 0xFFFF) return false;
    if (minute > 59) return false;

    // Caso 1: registrador direto
    if (_map.time_minute.writable) {
        if (_map.time_minute.handlerId == GOODWE_HANDLER) {
            uint16_t reg;

            // ler valor atual (contém second junto)
            if (!readField(_map.time_minute, &reg)) return false;

            // preservar LSB (second)
            uint16_t lsb = reg & 0x00FF;

            // colocar minuto no MSB
            uint16_t newVal = (minute << 8) | lsb;

            return writeField(_map.time_minute, newVal);
        }

        return writeField(_map.time_minute, minute);
    }

    // Caso 2: fallback para epoch
    if (_map.time_epoch.writable) {
        Datetime dt;

        if (!getDatetime(dt)) return false;

        dt.minute = minute;
        if (!isValidDatetime(dt)) return false;

        uint32_t epoch = datetimeToEpoch(dt);
        return writeField(_map.time_epoch, epoch);
    }

    return false;
}

bool Inverter::setSecond(uint16_t second) {
    if (_map.serial.address == 0xFFFF) return false;
    if (second > 59) return false;

    // Caso 1: registrador direto
    if (_map.time_second.writable) {
        if (_map.time_second.handlerId == GOODWE_HANDLER) {
            uint16_t reg;

            // ler valor atual (contém minute junto)
            if (!readField(_map.time_second, &reg)) return false;

            // preservar MSB (minute)
            uint16_t msb = reg & 0xFF00;

            // colocar second no LSB
            uint16_t newVal = msb | (second & 0x00FF);

            return writeField(_map.time_second, newVal);
        }

        return writeField(_map.time_second, second);
    }

    // Caso 2: fallback para epoch
    if (_map.time_epoch.writable) {
        Datetime dt;

        if (!getDatetime(dt)) return false;

        dt.second = second;
        if (!isValidDatetime(dt)) return false;

        uint32_t epoch = datetimeToEpoch(dt);
        return writeField(_map.time_epoch, epoch);
    }

    return false;
}

bool Inverter::setEpochTime(uint32_t epoch) {
    if (_map.serial.address == 0xFFFF) return false;
    if (!_map.time_epoch.writable) return false;

    return writeField(_map.time_epoch, epoch);
}

// ======================================================
// Internal Conversions
// ======================================================

bool Inverter::isLeap(uint16_t y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

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
