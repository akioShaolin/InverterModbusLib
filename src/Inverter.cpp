/*
 * inverterLib - Solar Inverter Library for Arduino
 * ------------------------------------------------
 * Modbus RTU (RS485)communication layer for inverter integration
 *
 * Copyright (c) 2026, Pedro Akio Sakuma
 * Licensed under BSD 3-Clause License
 */

#include "Inverter.h"
#include "ModbusRTU.h"

static bool _mb_done = false;
static bool _mb_success = false;
Datetime datetime;

/*
    Entrevista com a Copilot sobre a implementação da classe Inverter:

    • Por que verificar o mapa antes de cada leitura/escrita?
        Porque nem todos os modelos de inversores terão todos os campos disponíveis.
        O mapa do inversor indica quais campos estão disponíveis para leitura e escrita,
        e quais registradores Modbus correspondem a cada campo. Verificar o mapa antes
        de tentar acessar um campo garante que o código não tente ler ou escrever em um
        registrador que não existe ou que não é aplicável para aquele modelo de inversor,
        o que poderia causar erros ou comportamento inesperado.

    • Devo implementar a lógica de leitura e escrita de campos usando o mapa do inversor
    dentro de cada método público (por exemplo, getSerial, boot, setBoot, etc.) ou criar
    métodos privados auxiliares para lidar com a leitura e escrita de campos usando o mapa?
        Criar métodos privados auxiliares para leitura e escrita de campos usando o mapa
        do inversor é uma abordagem mais eficiente e organizada. Isso evita a repetição de
        código em cada método público, centraliza a lógica de acesso aos registradores
        Modbus e torna o código mais fácil de manter. Os métodos públicos podem então
        simplesmente chamar esses métodos privados, passando o campo relevante do mapa do
        inversor, e os métodos privados cuidam dos detalhes da leitura/escrita, aplicação
        da escala, combinação de registradores quando necessário, etc.

    • Ok, então primeiro vou criar as funções e depois eu me preocupo com a maneira como
        vou obter os dados do Modbus. A ideia é que os métodos públicos sejam simples e
        diretos, enquanto a complexidade de lidar com o Modbus e o mapa do inversor fique
        encapsulada nos métodos privados. Dessa forma, se no futuro eu precisar mudar a
        forma como acesso o Modbus ou como interpreto os dados, posso fazer isso em um só
        lugar, sem precisar modificar cada método público individualmente.


    ::Alguns dados são obtidos lendo diretamente o registrador correspondente, enquanto
    outros podem exigir leitura de múltiplos registradores e combinação dos valores (por
    exemplo, para valores de 32 bits ou floats). Para escrita, o processo é similar, mas
    no sentido inverso: o valor a ser escrito pode precisar ser convertido para o formato
    bruto esperado pelo registrador (por exemplo, aplicando a escala e convertendo para
    inteiro).::

    ::Outros precisam ser lidos em cambos distantes (2 regs aqui, 2 regs ali)::

    ::Para campos que não estão disponíveis nesse modelo, os métodos públicos devem retornar
    false ou um valor de erro apropriado, e não tentar acessar o Modbus.::
*/

bool Inverter::readScaledFloat(const ModbusField& field, float& value) {
    switch(field.type) {
        case U16: {
            uint16_t raw;
            if (!readField(field, &raw)) return false;
            value = (float)raw * field.scale;
            break;
        }

        case U32: {
            uint32_t raw;
            if (!readField(field, &raw)) return false;
            value = (float)raw * field.scale;
            break;
        }
        
        case FLOAT32: {
            float raw;
            if (!readField(field, &raw)) return false;
            value = raw * field.scale;
            break;
        }

        case I16: {
            int16_t raw;
            if (!readField(field, &raw)) return false;
            value = (float)raw * field.scale;
            break;
        }

        case I32: {
            int32_t raw;
            if (!readField(field, &raw)) return false;
            value = (float)raw * field.scale;
            break;
        }

        default:
            return false; // Tipo de dado não suportado para este campo

    }
    return true;
}

bool isLeap(uint16_t y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

Datetime epochToDatetime(uint32_t epoch) {
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

bool isValidDatetime(const Datetime& dt) {
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

uint32_t datetimeToEpoch(const Datetime& dt) {
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

bool _mb_cb(Modbus::ResultCode event, uint16_t, void*) {
    _mb_done = true;
    _mb_success = (event == Modbus::EX_SUCCESS);
    return true;
}

Inverter::Inverter(InverterModel model) : _model(model), _modbus(1, 9600, SERIAL_8N1) {
    _descriptor = getDescriptor(model);
    _map = getInverterMap(model);
    
}

void Inverter::attachModbus(ModbusRTU& mb) {
    _mb = &mb;
}

void Inverter::attachConfig(ModbusConfig& config) {
    _modbus = config;
    _customConfigSet = true;
}

bool Inverter::begin() {
    if (_mb == nullptr) return false;
    if (_map.serial.address == 0xFFFF || _descriptor.nominalPowerW == 0) return false;  //São campos obrigatórios. A falta deles invalida a struct

    if (!_customConfigSet) {
        memcpy(&_modbus, _descriptor.config, sizeof(ModbusConfig));
    }

    return true;
}

void Inverter::setSlaveId(uint8_t id) {
    _modbus.setId(id);
}

bool Inverter::getSerial(String& serial) {
    if (_map.serial.address == 0xFFFF || !_map.serial.readable) return false;

    if (_map.serial.type == ASCII) {
        char buffer[INV_MAX_STRING_CHARS + 1]; // Supondo que o número serial tenha no máximo 32 caracteres, mais um para o terminador nulo
        if (readField(_map.serial, buffer)) {
            serial = String(buffer);
            return true;
        }
    } else if (_map.serial.type == U32) {
        uint32_t buffer; // Ler um valor de 32 bits
        if (readField(_map.serial, &buffer)) {
            serial = String(buffer); // Transforma o número em string
            return true;
        }
    }
    return false;
}

bool Inverter::boot() {
    if (_map.serial.address == 0xFFFF || !_map.boot.writable) return false;
    uint16_t v = pgm_read_word(&_descriptor.bootMode->bootValue);
    return writeField(_map.boot, v);
}

bool Inverter::setBoot(bool boot) {
    if (boot) {
        return Inverter::boot(); // já existe boot em algum lugar, então é preciso especificar o Inverter::
    }
    return shutdown();
}

bool Inverter::shutdown() {
    if (_map.serial.address == 0xFFFF) return false;
    if (!_map.shutdown.writable) {
        if(!_map.boot.writable) {
            return false;
        } else {
            uint16_t v = pgm_read_word(&_descriptor.bootMode->shutdownValue);
            return writeField(_map.boot, v);
        }
    } else {
        uint16_t v = pgm_read_word(&_descriptor.bootMode->shutdownValue);
        return writeField(_map.shutdown, v);
    }
    return false;
}

bool Inverter::setPowerLimitEnabled(bool enabled) {
    if (_map.serial.address == 0xFFFF) return false;
    if (!_map.enablePowerLimit.writable) return false;
    
    uint16_t v = 1;
    return writeField(_map.enablePowerLimit, v);
}

bool Inverter::setPowerLimit(float watts) {
    if (_map.serial.address == 0xFFFF) return false;
    if (_descriptor.nominalPowerW == 0) return false;

    if (_map.PowerLimit.writable) {
        return writeField(_map.PowerLimit, roundf(watts / _map.PowerLimit.scale));
    }

    if(_map.PowerLimitPercent.writable) {
        float percent = (watts / (float)_descriptor.nominalPowerW) * 100.0f;
        if (!setPowerLimitPercent(percent)) return false;
    }
    return false;
}

bool Inverter::setPowerLimitPercent(float percent) {
    if (_map.serial.address == 0xFFFF)

    if (_map.PowerLimitPercent.writable) {
        return writeField(_map.PowerLimitPercent, roundf(percent / _map.PowerLimitPercent.scale));
    }

    if (_map.PowerLimit.writable) {
        float watts = ((float)_descriptor.nominalPowerW * percent) / 100.0f;
        return setPowerLimitPercent(watts);
    }
   
    return false;
}

bool Inverter::setExportLimitEnabled(bool enabled) {
    if (_map.serial.address == 0xFFFF || !_map.enableExportLimit.writable) return false;
    // Aqui entraria a lógica real de escrita para habilitar/desabilitar o limite de exportação usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::setExportLimit(float watts) {
    if (_map.serial.address == 0xFFFF || !_map.ExportLimit.writable) return false;
    // Aqui entraria a lógica real de escrita para definir o limite de exportação usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::setExportLimitPercent(float percent) {
    if (_map.serial.address == 0xFFFF || !_map.ExportLimitPercent.writable) return false;
    // Aqui entraria a lógica real de escrita para definir o limite de exportação como percentual usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::setPowerFactorEnabled(bool enabled) {
    if (_map.serial.address == 0xFFFF || !_map.enablePowerFactor.writable) return false;
    // Aqui entraria a lógica real de escrita para habilitar/desabilitar o controle de fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::setPowerFactor(float pf) {
    if (_map.serial.address == 0xFFFF || !_map.PowerFactorSetpoint.writable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::isBooted(bool& isBooted) {
    if (_map.serial.address == 0xFFFF || !_map.boot.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::isPowerLimitEnabled(bool& enabled) {
    if (_map.serial.address == 0xFFFF || !_map.enablePowerLimit.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getPowerLimit(float& watts) {
    if (_map.serial.address == 0xFFFF) return false;
    if(_map.PowerLimit.readable) {
        return readScaledFloat(_map.PowerLimit, watts);
    }

    if (_map.PowerLimitPercent.readable) {
        float percent;
        if (!getPowerLimitPercent(percent)) return false;
        watts = (_descriptor.nominalPowerW * percent) / 100.0f;
        return true;
    }

    return false;
}

bool Inverter::getPowerLimitPercent(float& percent) {
    if (_map.serial.address == 0xFFFF) return false;
    if (_descriptor.nominalPowerW == 0) return false;

    if (_map.PowerLimitPercent.readable) {
        return readScaledFloat(_map.PowerLimitPercent, percent);
    }

    if (_map.PowerLimit.readable) {
        float watts;
        if (!getPowerLimit(watts)) return false;
        percent = (watts / _descriptor.nominalPowerW) * 100.0f;
        return true;
    }
    
    return false;    
}

bool Inverter::isExportLimitEnabled(bool& enabled) {
    if (_map.serial.address == 0xFFFF || !_map.enableExportLimit.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getExportLimit(float& watts) {
    if (_map.serial.address == 0xFFFF || !_map.ExportLimit.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getExportLimitPercent(float& percent) {
    if (_map.serial.address == 0xFFFF || !_map.ExportLimitPercent.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::isPowerFactorEnabled(bool& enabled) {
    if (_map.serial.address == 0xFFFF || !_map.powerFactor.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getPowerFactorSetpoint(float& pf) {
    if (_map.serial.address == 0xFFFF || !_map.PowerFactorSetpoint.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}
  
// Tempo
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

        if (_map.time_year.mode == GOODWE_HANDLER) {
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

        if (_map.time_month.mode == GOODWE_HANDLER) {
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

        if (_map.time_day.mode == GOODWE_HANDLER) {
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

        if (_map.time_hour.mode == GOODWE_HANDLER) {
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

        if (_map.time_minute.mode == GOODWE_HANDLER) {
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

        if (_map.time_second.mode == GOODWE_HANDLER) {
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
        if (_map.time_year.mode == GOODWE_HANDLER) {
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
        if (_map.time_month.mode == GOODWE_HANDLER) {
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
        if (_map.time_day.mode == GOODWE_HANDLER) {
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
        if (_map.time_hour.mode == GOODWE_HANDLER) {
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
        if (_map.time_minute.mode == GOODWE_HANDLER) {
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
        if (_map.time_second.mode == GOODWE_HANDLER) {
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


bool Inverter::getTotalEnergy(float& kWh) {
    if (_map.serial.address == 0xFFFF || !_map.totalEnergy.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getDailyEnergy(float& kWh) {
    if (_map.serial.address == 0xFFFF || !_map.dailyEnergy.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}


bool Inverter::getActivePower(float& watts) {
    if (_map.serial.address == 0xFFFF || !_map.activePower.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}
   
bool Inverter::getReactivePower(float& voltAmperReactive) {
    if (_map.serial.address == 0xFFFF || !_map.reactivePower.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getApparentPower(float& voltAmper) {
    if (_map.serial.address == 0xFFFF || !_map.apparentPower.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getPowerFactor(float &pf) {
    if (_map.serial.address == 0xFFFF || !_map.powerFactor.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}


bool Inverter::getGridVoltage(PhaseData& phase) {
    if (_map.serial.address == 0xFFFF) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getGridCurrent(PhaseData& phase) {
    if (_map.serial.address == 0xFFFF) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getGridFrequency(PhaseData& phase) {
    if (_map.serial.address == 0xFFFF) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getTemperature(float& temperature) {
    if (_map.serial.address == 0xFFFF || !_map.temperature.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getInsulationResistance(float& kiloOhms) {
    if (_map.serial.address == 0xFFFF || !_map.insulationResistance.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getInverterStatus(InverterStatus& status) {
    if (_map.serial.address == 0xFFFF || !_map.inverterStatus.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getAlarm(Alarm& alarm) {
    if (_map.serial.address == 0xFFFF || !_map.alarm.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getStringVoltage(StringValues& voltage) {
    if (_map.serial.address == 0xFFFF || !_map.stringVoltage.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getStringCurrent(StringValues& current) {
    if (_map.serial.address == 0xFFFF || !_map.stringCurrent.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getStringPower(StringValues& power) {
    if (_map.serial.address == 0xFFFF || !_map.stringPower.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}


bool Inverter::getBatteryVoltage(BatteryValues& voltage) {
    if (_map.serial.address == 0xFFFF || !_map.batteryVoltage.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getBatteryCurrent(BatteryValues& current) {
    if (_map.serial.address == 0xFFFF || !_map.batteryCurrent.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getBatteryPower(BatteryValues& power) {
    if (_map.serial.address == 0xFFFF || !_map.batteryPower.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getBatterySoC(BatteryValues& soc) {
    if (_map.serial.address == 0xFFFF || !_map.batterySoC.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getBatterySoH(BatteryValues& soh) {
    if (_map.serial.address == 0xFFFF || !_map.batterySoH.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getEPSVoltage(PhaseData& phase) {
    if (_map.serial.address == 0xFFFF || !_map.epsVoltage.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getEPSCurrent(PhaseData& phase) {
    if (_map.serial.address == 0xFFFF || !_map.epsCurrent.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getEPSActivePower(PhaseData& phase) {
    if (_map.serial.address == 0xFFFF || !_map.epsActivePower.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

// Private methods para leitura e escrita de registradores Modbus, aplicando a escala correta, lidando com tipos de dados diferentes (inteiros, floats, etc.) e combinando registradores quando necessário. Esses métodos vão usar as informações do ModbusField para fazer a leitura/escrita correta.

bool Inverter::readField(const ModbusField& field, char* value) {
    if (field.type != ASCII || !field.readable) return false;
    if (value == nullptr) return false;
    if (field.length <= 0 || field.length > INV_MAX_STRING_CHARS / 2 + 1) return false;

    uint16_t buffer[INV_MAX_STRING_CHARS / 2 + 1]; // 2 Caracteres ASCII por registrador de 16 bits, mais um nulo ao final
    
    if (field.stride == 1) {
        // Caso simples: os caracteres estão em registradores consecutivos
        if (!readHoldingRegister(field.address, buffer, field.length)) {
            return false;
        }
    } else {
        for (uint16_t i = 0; i < field.length; i++) {
            if (!readHoldingRegister(field.address + i * field.stride, &buffer[i], 1)) {
                return false;
            }
        }
    }
    uint16_t charIndex = 0;

    for (uint16_t i = 0; i < field.length; i++) {
        char high = (char)(buffer[i] >> 8);
        char low  = (char)(buffer[i] & 0xFF);

        if (high >= 32 && high <= 126) {
            value[charIndex++] = high;
        }

        if (low >= 32 && low <= 126) {
            value[charIndex++] = low;
        }
    }
    value[charIndex] = '\0'; // Garantir terminação nula
    return charIndex > 0; // Retorna true se pelo menos um caractere válido foi lido
}

bool Inverter::readField(const ModbusField& field, float* value) {
    if (field.type != FLOAT32 || !field.readable) return false;
    if (value == nullptr) return false;

    uint32_t raw [INV_MAX_FLOAT_VALUES]; // Cada float de 32 bits ocupa 2 registradores de 16 bits
    if(!readField32Raw(field, raw)) return false;

    for (uint16_t i = 0; i < field.length; i++) {
        memcpy(&value[i], &raw[i], sizeof(float));
    }
    return true;
}

bool Inverter::readField(const ModbusField& field, uint16_t* value) {
    if (field.type != U16 || !field.readable) return false;
    return readField16Raw(field, value);
}

bool Inverter::readField(const ModbusField& field, uint32_t* value) {
    if (field.type != U32 || !field.readable) return false;
    return readField32Raw(field, value);
}

bool Inverter::readField(const ModbusField& field, uint64_t* value) {
    if (field.type != U64 || !field.readable) return false;
    return readField64Raw(field, value);
}

bool Inverter::readField(const ModbusField& field, int16_t* value) {
    if (field.type != I16 || !field.readable) return false;
    if (value == nullptr) return false;

    uint16_t raw [INV_MAX_U16_VALUES];
    if(!readField16Raw(field, raw)) return false;

    for (uint16_t i = 0; i < field.length; i++) {
        value[i] = (int16_t)raw[i];
    }
    return true;
}

bool Inverter::readField(const ModbusField& field, int32_t* value) {
    if (field.type != I32 || !field.readable) return false;
    if (value == nullptr) return false;

    uint32_t raw [INV_MAX_U32_VALUES];
    if(!readField32Raw(field, raw)) return false;

    for (uint16_t i = 0; i < field.length; i++) {
        value[i] = (int32_t)raw[i];
    }
    return true;
}

bool Inverter::readField(const ModbusField& field, int64_t* value) {
    if (field.type != I64 || !field.readable) return false;
    if (value == nullptr) return false;

    uint64_t raw [INV_MAX_U64_VALUES];
    if(!readField64Raw(field, raw)) return false;

    for (uint16_t i = 0; i < field.length; i++) {
        value[i] = (int64_t)raw[i];
    }
    return true;
}

bool Inverter::readField16Raw(const ModbusField& field, uint16_t* value) {
    if (!field.readable) return false;
    if (value == nullptr) return false;
    if (field.length == 0 || field.length > INV_MAX_U16_VALUES) return false;

    uint16_t buffer[INV_MAX_U16_VALUES];

    if (field.length == 1) {
        if (!readHoldingRegister(field.address, buffer, 1)) return false;
    } 
    else if (field.stride == 1) {
        if (!readHoldingRegister(field.address, buffer, field.length)) return false;
    } 
    else {
        for (uint16_t i = 0; i < field.length; i++) {
            if (!readHoldingRegister(field.address + i * field.stride, &buffer[i], 1)) {
                return false;
            }
        }
    }
    for (uint16_t i = 0; i < field.length; i++) {
        value[i] = buffer[i];
    }
    return true;
}

bool Inverter::readField32Raw(const ModbusField& field, uint32_t* value) {
    if (!field.readable) return false;
    if (value == nullptr) return false;
    if (field.length == 0 || field.length > INV_MAX_U32_VALUES) return false;

    uint16_t buffer[INV_MAX_U32_VALUES * 2];

    if (field.length == 1) {
        if (!readHoldingRegister(field.address, buffer, 2)) return false;
    } 
    else if (field.stride == 2) {
        if (!readHoldingRegister(field.address, buffer, field.length * 2)) return false;
    } 
    else {
        for (uint16_t i = 0; i < field.length; i++) {
            if (!readHoldingRegister(field.address + i * field.stride, &buffer[i * 2], 2)) {
                return false;
            }
        }
    }
    for (uint16_t i = 0; i < field.length; i++) {
        value[i] = ((uint32_t)buffer[i * 2] << 16) | buffer[i * 2 + 1];
    }
    return true;
}

bool Inverter::readField64Raw(const ModbusField& field, uint64_t* value) {
    if (!field.readable) return false;
    if (value == nullptr) return false;
    if (field.length == 0 || field.length > INV_MAX_U64_VALUES) return false;

    uint16_t buffer[INV_MAX_U64_VALUES * 4];

    if (field.length == 1) {
        if (!readHoldingRegister(field.address, buffer, 4)) return false;
    } 
    else if (field.stride == 4) {
        if (!readHoldingRegister(field.address, buffer, field.length * 4)) return false;
    } 
    else {
        for (uint16_t i = 0; i < field.length; i++) {
            if (!readHoldingRegister(field.address + i * field.stride, &buffer[i * 4], 4)) {
                return false;
            }
        }
    }
    for (uint16_t i = 0; i < field.length; i++) {
        value[i] = ((uint64_t)buffer[i * 4] << 48) | ((uint64_t)buffer[i * 4 + 1] << 32) | ((uint64_t)buffer[i * 4 + 2] << 16) | buffer[i * 4 + 3];
    }
    return true;
}

bool Inverter::writeField(const ModbusField& field, float value) {
    return writeField(field, &value, 1);
}

bool Inverter::writeField(const ModbusField& field, uint16_t value){
    return writeField(field, &value, 1);
}

bool Inverter::writeField(const ModbusField& field, uint32_t value){
    return writeField(field, &value, 1);
}

bool Inverter::writeField(const ModbusField& field, int16_t value){
    return writeField(field, &value, 1);
}

bool Inverter::writeField(const ModbusField& field, int32_t value){
    return writeField(field, &value, 1);
}

bool Inverter::writeField(const ModbusField& field, float* value, uint8_t count) {
    if (field.type != FLOAT32 || !field.writable) return false;
    if (value == nullptr) return false;
    if (count == 0 || count > INV_MAX_FLOAT_VALUES) return false;

    uint32_t raw [INV_MAX_FLOAT_VALUES];

    for (uint8_t i = 0; i < count; i++) {
        memcpy(&raw[i], &value[i], sizeof(uint32_t));
    }

    return writeField32Raw(field, raw, count);
}

bool Inverter::writeField(const ModbusField& field, uint16_t* value, uint8_t count) {
    if (field.type != U16 || !field.writable) return false;
    if (value == nullptr) return false;
    if (count == 0 || count > INV_MAX_U16_VALUES) return false;
    return writeField16Raw(field, value, count);
}   

bool Inverter::writeField(const ModbusField& field, uint32_t* value, uint8_t count) {
    if (field.type != U32 || !field.writable) return false;
    if (value == nullptr) return false;
    if (count == 0 || count > INV_MAX_U32_VALUES) return false;
    return writeField32Raw(field, value, count);
}  

bool Inverter::writeField(const ModbusField& field, int16_t* value, uint8_t count) {
    if (field.type != I16 || !field.writable) return false;
    if (value == nullptr) return false;
    if (count == 0 || count > INV_MAX_U16_VALUES) return false;

    uint16_t raw [INV_MAX_U16_VALUES];

    for (uint8_t i = 0; i < count; i++) {
        raw[i] = (uint16_t)value[i];
    }

    return writeField16Raw(field, raw, count);
}    

bool Inverter::writeField(const ModbusField& field, int32_t* value, uint8_t count) {
    if (field.type != I32 || !field.writable) return false;
    if (value == nullptr) return false;
    if (count == 0 || count > INV_MAX_U32_VALUES) return false;

    uint32_t raw [INV_MAX_U32_VALUES];

    for (uint8_t i = 0; i < count; i++) {
        raw[i] = (uint32_t)value[i];
    }

    return writeField32Raw(field, raw, count);
}

bool Inverter::writeField16Raw(const ModbusField& field, uint16_t* value, uint8_t count) {
    if (value == nullptr) return false;
    if (count == 0 || count > INV_MAX_U16_VALUES) return false;
    if (count == 1 || field.stride == 1) {
        return writeHoldingRegister(field.address, value, count);
    } 

    for (uint16_t i = 0; i < count; i++) {
        if (!writeHoldingRegister(field.address + i * field.stride, &value[i], 1)) {
            return false;
        }
    }
    return true;
}

bool Inverter::writeField32Raw(const ModbusField& field, uint32_t* value, uint8_t count) { 
    if (value == nullptr) return false;
    if (count == 0 || count > INV_MAX_U32_VALUES) return false;

    uint16_t buffer[INV_MAX_U32_VALUES * 2]; // 2 registradores de 16 bits por valor de 32 bits

    if (count == 1 || field.stride == 2) {
        for (uint16_t i = 0; i < count; i++) {
            buffer[i * 2] = (uint16_t)(value[i] >> 16);
            buffer[i * 2 + 1] = (uint16_t)(value[i] & 0xFFFF);
        }
        return writeHoldingRegister(field.address, buffer, count * 2);
    } 
    for (uint16_t i = 0; i < count; i++) {
        uint16_t high = (uint16_t)(value[i] >> 16);
        uint16_t low = (uint16_t)(value[i] & 0xFFFF);

        if (!writeHoldingRegister(field.address + i * field.stride, &high, 1)) return false;
        if (!writeHoldingRegister(field.address + i * field.stride + 1, &low, 1)) return false;
    }
    return true;
}

bool Inverter::readHoldingRegister(uint16_t startReg, uint16_t* buffer, uint16_t count) {
    if (_mb == nullptr) return false;
    // Implementar EventCode para essa função

    _mb_done = false;
    _mb_success = false;

    bool requestAccepted = _mb->readHreg(_modbus.getId(), startReg, buffer, count, _mb_cb);

    if (!requestAccepted) {
        return false; // Não conseguiu enviar
    }

    uint32_t start = millis();

    while(!_mb_done && millis() - start < 1000) {
        _mb->task();
        yield();
    }

    if (!_mb_done) {
        return false; // Timeout
    }

    if (!_mb_success) {
        return false; // Erro na resposta
    }

    return true;
}

bool Inverter::writeHoldingRegister(uint16_t startReg, uint16_t* buffer, uint16_t count) {
    if (_mb == nullptr) return false;

    _mb_done = false;
    _mb_success = false;

    bool requestAccepted;

    if (count == 1) {
        requestAccepted = _mb->writeHreg(_modbus.getId(), startReg, buffer[0], _mb_cb);
    } else {
        requestAccepted = _mb->writeHreg(_modbus.getId(), startReg, buffer, count, _mb_cb);
    }

    if (!requestAccepted) {
        return false; // Não conseguiu enviar
    }

    uint32_t start = millis();

    while (!_mb_done && millis() - start < 1000) {
        _mb->task();
        yield();
    }

    if (!_mb_done) {
        return false; // Timeout
    }

    if (!_mb_success) {
        return false; // Erro na resposta
    }

    return true;
}