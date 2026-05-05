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