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

// ======================================================
// Date/Time Read
// ======================================================

bool Inverter::getYear(uint16_t& year) {
    if (!hasValidMap()) return false;

    const TimeFeature& feature = _map.time;
    const ModbusField& fieldYear = feature.year;

    // Verifica se o campo existe, senão cai em fallback
    if (!isInvalidField(fieldYear)) {
        
        switch (fieldYear.mode) {
            case FIELD_SIMPLE: {
                if (!fieldYear.readable) return false;

                uint16_t raw = 0;

                if (!readField(fieldYear, &raw)) return false;

                if (feature.usesSharedDateTimeRegisters) {
                    raw = raw >> 8;
                }

                if (feature.yearIsOffsetFrom2000) {
                    raw = raw + 2000;
                }

                if (raw < 1970 || raw > 2100) return false;

                year = raw;

                return true;
            }

            default:
                return false;
        }
    }

    // Fallback: Tenta obter ano a partir do epoch
    if (!feature.supportsEpoch) return false;

    const ModbusField& fieldEpoch = feature.epoch;

    if (isInvalidField(fieldEpoch)) return false;

    switch (fieldEpoch.mode)
    {
    case FIELD_SIMPLE: {
        if (!fieldEpoch.readable) return false;

        uint32_t epoch = 0;
        Datetime dtTmp;

        if (!readField(fieldEpoch, &epoch)) return false;

        dtTmp = epochToDatetime(epoch);

        if (dtTmp.year < 1970 || dtTmp.year > 2100) return false;

        year = dtTmp.year;
        return true;
    }  
    
    default:
        return false;
    }
}

bool Inverter::getMonth(uint16_t& month) {
    if (!hasValidMap()) return false;

    const TimeFeature& feature = _map.time;
    const ModbusField& fieldMonth = feature.month;

    // Verifica se o campo existe, senão cai em fallback
    if (!isInvalidField(fieldMonth)) {
        
        switch (fieldMonth.mode) {
            case FIELD_SIMPLE: {
                if (!fieldMonth.readable) return false;

                uint16_t raw = 0;

                if (!readField(fieldMonth, &raw)) return false;

                if (feature.usesSharedDateTimeRegisters) {
                    raw = raw & 0xFF;
                }

                if (raw < 1 || raw > 12) return false;

                month = raw;

                return true;
            }

            default:
                return false;
        }
    }

    // Fallback: Tenta obter mês a partir do epoch
    if (!feature.supportsEpoch) return false;

    const ModbusField& fieldEpoch = feature.epoch;

    if (isInvalidField(fieldEpoch)) return false;

    switch (fieldEpoch.mode)
    {
    case FIELD_SIMPLE: {
        if (!fieldEpoch.readable) return false;

        uint32_t epoch = 0;
        Datetime dtTmp;

        if (!readField(fieldEpoch, &epoch)) return false;

        dtTmp = epochToDatetime(epoch);

        if (dtTmp.month < 1 || dtTmp.month > 12) return false;

        month = dtTmp.month;
        return true;
    }  
    
    default:
        return false;
    }
}

bool Inverter::getDay(uint16_t& day) {
    if (!hasValidMap()) return false;

    const TimeFeature& feature = _map.time;
    const ModbusField& fieldDay = feature.day;

    // Verifica se o campo existe, senão cai em fallback
    if (!isInvalidField(fieldDay)) {
        
        switch (fieldDay.mode) {
            case FIELD_SIMPLE: {
                if (!fieldDay.readable) return false;

                uint16_t raw = 0;

                if (!readField(fieldDay, &raw)) return false;

                if (feature.usesSharedDateTimeRegisters) {
                    raw = raw >> 8;
                }

                if (raw < 1 || raw > 31) return false;

                day = raw;

                return true;
            }

            default:
                return false;
        }
    }

    // Fallback: Tenta obter dia a partir do epoch
    if (!feature.supportsEpoch) return false;

    const ModbusField& fieldEpoch = feature.epoch;

    if (isInvalidField(fieldEpoch)) return false;

    switch (fieldEpoch.mode)
    {
    case FIELD_SIMPLE: {
        if (!fieldEpoch.readable) return false;

        uint32_t epoch = 0;
        Datetime dtTmp;

        if (!readField(fieldEpoch, &epoch)) return false;

        dtTmp = epochToDatetime(epoch);

        if (dtTmp.day < 1 || dtTmp.day > 31) return false;

        day = dtTmp.day;
        return true;
    }  
    
    default:
        return false;
    }
}

bool Inverter::getHour(uint16_t& hour) {
    if (!hasValidMap()) return false;

    const TimeFeature& feature = _map.time;
    const ModbusField& fieldHour = feature.hour;

    // Verifica se o campo existe, senão cai em fallback
    if (!isInvalidField(fieldHour)) {
        
        switch (fieldHour.mode) {
            case FIELD_SIMPLE: {
                if (!fieldHour.readable) return false;

                uint16_t raw = 0;

                if (!readField(fieldHour, &raw)) return false;

                if (feature.usesSharedDateTimeRegisters) {
                    raw = raw & 0xFF;
                }

                if (raw < 0 || raw > 23) return false;

                hour = raw;

                return true;
            }

            default:
                return false;
        }
    }

    // Fallback: Tenta obter hora a partir do epoch
    if (!feature.supportsEpoch) return false;

    const ModbusField& fieldEpoch = feature.epoch;

    if (isInvalidField(fieldEpoch)) return false;

    switch (fieldEpoch.mode)
    {
    case FIELD_SIMPLE: {
        if (!fieldEpoch.readable) return false;

        uint32_t epoch = 0;
        Datetime dtTmp;

        if (!readField(fieldEpoch, &epoch)) return false;

        dtTmp = epochToDatetime(epoch);

        if (dtTmp.hour < 0 || dtTmp.hour > 23) return false;

        hour = dtTmp.hour;
        return true;
    }  
    
    default:
        return false;
    }
}

bool Inverter::getMinute(uint16_t& minute) {
    if (!hasValidMap()) return false;

    const TimeFeature& feature = _map.time;
    const ModbusField& fieldMin = feature.minute;

    // Verifica se o campo existe, senão cai em fallback
    if (!isInvalidField(fieldMin)) {
        
        switch (fieldMin.mode) {
            case FIELD_SIMPLE: {
                if (!fieldMin.readable) return false;

                uint16_t raw = 0;

                if (!readField(fieldMin, &raw)) return false;

                if (feature.usesSharedDateTimeRegisters) {
                    raw = raw >> 8;
                }

                if (raw > 59) return false;

                minute = raw;

                return true;
            }

            default:
                return false;
        }
    }

    // Fallback: Tenta obter minuto a partir do epoch
    if (!feature.supportsEpoch) return false;

    const ModbusField& fieldEpoch = feature.epoch;

    if (isInvalidField(fieldEpoch)) return false;

    switch (fieldEpoch.mode)
    {
    case FIELD_SIMPLE: {
        if (!fieldEpoch.readable) return false;

        uint32_t epoch = 0;
        Datetime dtTmp;

        if (!readField(fieldEpoch, &epoch)) return false;

        dtTmp = epochToDatetime(epoch);

        if (dtTmp.minute > 59) return false;

        minute = dtTmp.minute;
        return true;
    }  
    
    default:
        return false;
    }
}

bool Inverter::getSecond(uint16_t& second) {
    if (!hasValidMap()) return false;

    const TimeFeature& feature = _map.time;
    const ModbusField& fieldSec = feature.second;

    // Verifica se o campo existe, senão cai em fallback
    if (!isInvalidField(fieldSec)) {
        
        switch (fieldSec.mode) {
            case FIELD_SIMPLE: {
                if (!fieldSec.readable) return false;

                uint16_t raw = 0;

                if (!readField(fieldSec, &raw)) return false;

                if (feature.usesSharedDateTimeRegisters) {
                    raw = raw & 0xFF;
                }

                if (raw > 59) return false;

                second = raw;

                return true;
            }

            default:
                return false;
        }
    }

    // Fallback: Tenta obter segundo a partir do epoch
    if (!feature.supportsEpoch) return false;

    const ModbusField& fieldEpoch = feature.epoch;

    if (isInvalidField(fieldEpoch)) return false;

    switch (fieldEpoch.mode) {
    case FIELD_SIMPLE: {
        if (!fieldEpoch.readable) return false;

        uint32_t epoch = 0;
        Datetime dtTmp;

        if (!readField(fieldEpoch, &epoch)) return false;

        dtTmp = epochToDatetime(epoch);

        if (dtTmp.second > 59) return false;

        second = dtTmp.second;
        return true;
    }  
    
    default:
        return false;
    }
}

bool Inverter::getEpochTime(uint32_t& epoch) {
    if (!hasValidMap()) return false;

    const TimeFeature& feature = _map.time;
    const ModbusField& field = feature.epoch;

    if (isInvalidField(field)) return false;

    switch (field.mode) {
    case FIELD_SIMPLE:
        if (!field.readable) return false;
        return readField(field, &epoch);
    
    default:
        return false;
    }

    // Sem fallback
}

bool Inverter::getDateTime(Datetime& dt) {

}
// ======================================================
// Date/Time Write
// ======================================================

bool Inverter::setYear(uint16_t year) {
    if (!hasValidMap()) return false;
    if (year < 1970 || year > 2100) return false;

    const TimeFeature& feature = _map.time;
    const ModbusField& fieldYear = feature.year;

    // Verifica se o campo existe, senão cai em fallback
    if (!isInvalidField(fieldYear)) {
        
        switch (fieldYear.mode) {
            case FIELD_SIMPLE: {
                if (!fieldYear.writable) return false;
                if (feature.yearIsOffsetFrom2000) {
                    if (year < 2000 && year > 2099) return false;
                    year -= 2000;
                }

                if (feature.usesSharedDateTimeRegisters) {
                    uint16_t reg;

                    // ler valor atual (contém month junto)
                    if (!readField(fieldYear, &reg)) return false;

                    // preservar LSB (month)
                    uint16_t lsb = reg & 0x00FF;

                    // colocar ano no MSB
                    uint16_t newVal = (year << 8) | lsb;

                    return writeField(fieldYear, newVal);
                }

                return writeField(fieldYear, year);
            }

            default:
                return false;
        }
    }

    // Fallback: Tentaria escrever ano a partir do epoch, mas não é necessário por enquanto
    return false;
}

bool Inverter::setMonth(uint16_t month) {
    if (!hasValidMap()) return false;
    if (month < 1 || month > 12) return false;

    const TimeFeature& feature = _map.time;
    const ModbusField& fieldMonth = feature.month;

    // Verifica se o campo existe, senão cai em fallback
    if (!isInvalidField(fieldMonth)) {
        
        switch (fieldMonth.mode) {
            case FIELD_SIMPLE: {
                if (!fieldMonth.writable) return false;

                if (feature.usesSharedDateTimeRegisters) {
                    uint16_t reg;

                    // ler valor atual (contém year junto)
                    if (!readField(fieldMonth, &reg)) return false;

                    // preservar MSB (year)
                    uint16_t msb = reg & 0xFF00;

                    // colocar mes no LSB
                    uint16_t newVal = msb | (month & 0x00FF);

                    return writeField(fieldMonth, newVal);
                }

                return writeField(fieldMonth, month);
            }

            default:
                return false;
        }
    }

    // Fallback: Tentaria escrever mes a partir do epoch, mas não é necessário por enquanto
    return false;
}

bool Inverter::setDay(uint16_t day) {
    if (!hasValidMap()) return false;
    if (day < 1 || day > 31) return false;

    const TimeFeature& feature = _map.time;
    const ModbusField& fieldDay = feature.day;

    // Verifica se o campo existe, senão cai em fallback
    if (!isInvalidField(fieldDay)) {
        
        switch (fieldDay.mode) {
            case FIELD_SIMPLE: {
                if (!fieldDay.writable) return false;

                if (feature.usesSharedDateTimeRegisters) {
                    uint16_t reg;

                    // ler valor atual (contém hour junto)
                    if (!readField(fieldDay, &reg)) return false;

                    // preservar LSB (hour)
                    uint16_t lsb = reg & 0x00FF;

                    // colocar dia no MSB
                    uint16_t newVal = (day << 8) | lsb;

                    return writeField(fieldDay, newVal);
                }

                return writeField(fieldDay, day);
            }

            default:
                return false;
        }
    }

    // Fallback: Tentaria escrever dia a partir do epoch, mas não é necessário por enquanto
    return false;
}

bool Inverter::setHour(uint16_t hour) {
    if (!hasValidMap()) return false;
    if (hour > 23) return false;

    const TimeFeature& feature = _map.time;
    const ModbusField& fieldHour = feature.hour;

    // Verifica se o campo existe, senão cai em fallback
    if (!isInvalidField(fieldHour)) {
        
        switch (fieldHour.mode) {
            case FIELD_SIMPLE: {
                if (!fieldHour.writable) return false;

                if (feature.usesSharedDateTimeRegisters) {
                    uint16_t reg;

                    // ler valor atual (contém day junto)
                    if (!readField(fieldHour, &reg)) return false;

                    // preservar MSB (day)
                    uint16_t msb = reg & 0xFF00;

                    // colocar hour no LSB
                    uint16_t newVal = msb | (hour & 0x00FF);

                    return writeField(fieldHour, newVal);
                }

                return writeField(fieldHour, hour);
            }

            default:
                return false;
        }
    }

    // Fallback: Tentaria escrever hora a partir do epoch, mas não é necessário por enquanto
    return false;
}

bool Inverter::setMinute(uint16_t minute) {
    if (!hasValidMap()) return false;
    if (minute > 59) return false;

    const TimeFeature& feature = _map.time;
    const ModbusField& fieldMin = feature.minute;

    // Verifica se o campo existe, senão cai em fallback
    if (!isInvalidField(fieldMin)) {
        
        switch (fieldMin.mode) {
            case FIELD_SIMPLE: {
                if (!fieldMin.writable) return false;

                if (feature.usesSharedDateTimeRegisters) {
                    uint16_t reg;

                    // ler valor atual (contém second junto)
                    if (!readField(fieldMin, &reg)) return false;

                    // preservar LSB (second)
                    uint16_t lsb = reg & 0x00FF;

                    // colocar minute no MSB
                    uint16_t newVal = (minute << 8) | lsb;

                    return writeField(fieldMin, newVal);
                }

                return writeField(fieldMin, minute);
            }

            default:
                return false;
        }
    }

    // Fallback: Tentaria escrever minuto a partir do epoch, mas não é necessário por enquanto
    return false;
}

bool Inverter::setSecond(uint16_t second) {
    if (!hasValidMap()) return false;
    if (second > 59) return false;

    const TimeFeature& feature = _map.time;
    const ModbusField& fieldSec = feature.second;

    // Verifica se o campo existe, senão cai em fallback
    if (!isInvalidField(fieldSec)) {
        
        switch (fieldSec.mode) {
            case FIELD_SIMPLE: {
                if (!fieldSec.writable) return false;

                if (feature.usesSharedDateTimeRegisters) {
                    uint16_t reg;

                    // ler valor atual (contém minute junto)
                    if (!readField(fieldSec, &reg)) return false;

                    // preservar MSB (minute)
                    uint16_t msb = reg & 0xFF00;

                    // colocar second no LSB
                    uint16_t newVal = msb | (second & 0x00FF);

                    return writeField(fieldSec, newVal);
                }

                return writeField(fieldSec, second);
            }

            default:
                return false;
        }
    }

    // Fallback: Tentaria escrever segundo a partir do epoch, mas não é necessário por enquanto
    return false;
}

bool Inverter::setEpochTime(uint32_t epoch) {
    if (!hasValidMap()) return false;
    //if (second > 59) return false;

    const TimeFeature& feature = _map.time;
    const ModbusField& fieldEpoch = feature.epoch;

    if (!isInvalidField(fieldEpoch)) {
        
        if (isInvalidField(fieldEpoch)) return false;
        switch (fieldEpoch.mode) {
            case FIELD_SIMPLE: {
                if (!fieldEpoch.writable) return false;

                return writeField(fieldEpoch, epoch);
            }

            default:
                return false;
        }
    }

    // Sem fallback
    return false;
}

bool Inverter::setDateTime(const Datetime& dt) {
    
}