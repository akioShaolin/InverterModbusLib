#include "Inverter.h"
#include "ModbusRTU.h"

static bool _mb_done = false;
static bool _mb_success = false;

bool _mb_cb(Modbus::ResultCode event, uint16_t, void*) {
    _mb_done = true;
    _mb_success = (event == Modbus::EX_SUCCESS);
    return true;
}

Inverter::Inverter(InverterModel model) : _model(model) {
    _descriptor = getDescriptor(model);
    _map = getInverterMap(model);
}

void Inverter::attachModbus(ModbusRTU& mb) {
    _mb = &mb;
}

void Inverter::attachConfig(ModbusConfig& config) {
    _modbus = &config;
}

bool Inverter::begin() {
    if (_mb == nullptr || _modbus == nullptr) return false;
    if (_map.serial.address == 0xFFFF || _descriptor.nominalPowerW == 0) return false;  //São campos obrigatórios. A falta deles invalida a struct
    return true;
}

// Por que verificar o mapa antes de cada leitura/escrita? Porque nem todos os modelos de inversores terão todos os campos disponíveis. O mapa do inversor indica quais campos estão disponíveis para leitura e escrita, e quais registradores Modbus correspondem a cada campo. Verificar o mapa antes de tentar acessar um campo garante que o código não tente ler ou escrever em um registrador que não existe ou que não é aplicável para aquele modelo de inversor, o que poderia causar erros ou comportamento inesperado.
// Devo implementar a lógica de leitura e escrita de campos usando o mapa do inversor dentro de cada método público (por exemplo, getSerial, boot, setBoot, etc.) ou criar métodos privados auxiliares para lidar com a leitura e escrita de campos usando o mapa? Criar métodos privados auxiliares para leitura e escrita de campos usando o mapa do inversor é uma abordagem mais eficiente e organizada. Isso evita a repetição de código em cada método público, centraliza a lógica de acesso aos registradores Modbus e torna o código mais fácil de manter. Os métodos públicos podem então simplesmente chamar esses métodos privados, passando o campo relevante do mapa do inversor, e os métodos privados cuidam dos detalhes da leitura/escrita, aplicação da escala, combinação de registradores quando necessário, etc.
// Ok, então primeiro vou criar as funções e depois eu me preocupo com a maneira como vou obter os dados do Modbus. A ideia é que os métodos públicos sejam simples e diretos, enquanto a complexidade de lidar com o Modbus e o mapa do inversor fique encapsulada nos métodos privados. Dessa forma, se no futuro eu precisar mudar a forma como acesso o Modbus ou como interpreto os dados, posso fazer isso em um só lugar, sem precisar modificar cada método público individualmente.

// Preciso criar métodos privados para leitura e escrita de campos usando o mapa do inversor, para evitar repetição de código em cada método público. Esses métodos privados vão usar o mapa para determinar o endereço do registrador, o tipo de dado, a escala, etc., e então chamar as funções de leitura/escrita Modbus apropriadas.
// Alguns dados são obtidos lendo diretamente o registrador correspondente, enquanto outros podem exigir leitura de múltiplos registradores e combinação dos valores (por exemplo, para valores de 32 bits ou floats). Para escrita, o processo é similar, mas no sentido inverso: o valor a ser escrito pode precisar ser convertido para o formato bruto esperado pelo registrador (por exemplo, aplicando a escala e convertendo para inteiro).
// Outros precisam ser lidos em cambos distantes (2 regs aqui, 2 regs ali, etc)
// Para campos que não estão disponíveis nesse modelo, os métodos públicos devem retornar false ou um valor de erro apropriado, e não tentar acessar o Modbus.

// Modbus field já resolve parte do problema, mas ainda preciso de métodos para ler e escrever usando o Modbus, aplicando a escala correta, lidando com tipos de dados diferentes (inteiros, floats, etc.) e combinando registradores quando necessário. Esses métodos vão usar as informações do ModbusField para fazer a leitura/escrita correta.

bool Inverter::getSerial(String& serial) {
    if (_map.serial.address == 0xFFFF || !_map.serial.readable) return false;

    if (_map.serial.type == ASCII) {
        char buffer[33]; // Supondo que o número serial tenha no máximo 32 caracteres, mais um para o terminador nulo
        if (readField(_map.serial, buffer)) {
            serial = String(buffer);
            return true;
        }
    } else if (_map.serial.type == U32) {
        uint32_t buffer; // Ler um valor de 32 bits
        if (readField(_map.serial, buffer)) {
            serial = String(buffer); // Transforma o número em string
            return true;
        }
    } else {
        return false;
        // Lógica para outros tipos de dados, se necessário
    }
    return false;
}

bool Inverter::boot() {
    if (_map.serial.address == 0xFFFF || !_map.boot.writable) return false;
    // Aqui entraria a lógica real de escrita para ligar o inversor usando Modbus
    // Por enquanto, apenas simulação
    writeHoldingRegister(_map.boot.address, 1);
    return true;
}

bool Inverter::setBoot(bool boot) {
    if (boot) {
        return Inverter::boot();
    }
    return Inverter::shutdown();
}

bool Inverter::shutdown() {
    if (_map.serial.address == 0xFFFF) return false;
    if (!_map.shutdown.writable) {
        if(!_map.boot.writable) {
            return false;
        } else {
            writeHoldingRegister(_map.boot.address, 0);
        }
    } else {
         writeHoldingRegister(_map.shutdown.address, 1);
    }
    
    
    // Aqui entraria a lógica real de escrita para desligar o inversor usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::setPowerLimitEnabled(bool enabled) {
    if (_map.serial.address == 0xFFFF || !_map.enablePowerLimit.writable) return false;
    // Aqui entraria a lógica real de escrita para habilitar/desabilitar o limite de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::setPowerLimit(float watts) {
    if (_map.serial.address == 0xFFFF || !_map.PowerLimit.writable) return false;
    // Aqui entraria a lógica real de escrita para definir o limite de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::setPowerLimitPercent(float percent) {
    if (_map.serial.address == 0xFFFF || !_map.PowerLimitPercent.writable) return false;
    // Aqui entraria a lógica real de escrita para definir o limite de potência como percentual usando Modbus
    // Por enquanto, apenas simulação
    return true;
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
    if (_map.serial.address == 0xFFFF || !_map.PowerLimit.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getPowerLimitPercent(float& percent) {
    if (_map.serial.address == 0xFFFF || !_map.PowerLimitPercent.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
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
bool Inverter::getYear(uint16_t& year) {
    if (_map.serial.address == 0xFFFF || !_map.time_year.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getMonth(uint16_t& month) {
    if (_map.serial.address == 0xFFFF || !_map.time_month.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getDay(uint16_t& day) {
    if (_map.serial.address == 0xFFFF || !_map.time_day.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getHour(uint16_t& hour) {
    if (_map.serial.address == 0xFFFF || !_map.time_hour.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getMinute(uint16_t& minute) {
    if (_map.serial.address == 0xFFFF || !_map.time_minute.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getSecond(uint16_t& second) {
    if (_map.serial.address == 0xFFFF || !_map.time_second.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::getEpochTime(uint32_t& epoch) {
    if (_map.serial.address == 0xFFFF || !_map.time_epoch.readable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}


bool Inverter::setYear(uint16_t year) {
    if (_map.serial.address == 0xFFFF || !_map.time_year.writable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::setMonth(uint16_t month) {
    if (_map.serial.address == 0xFFFF || !_map.time_month.writable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::setDay(uint16_t day) {
    if (_map.serial.address == 0xFFFF || !_map.time_day.writable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::setHour(uint16_t hour) {
    if (_map.serial.address == 0xFFFF || !_map.time_hour.writable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::setMinute(uint16_t minute) {
    if (_map.serial.address == 0xFFFF || !_map.time_minute.writable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::setSecond(uint16_t second) {
    if (_map.serial.address == 0xFFFF || !_map.time_second.writable) return false;
    // Aqui entraria a lógica real de escrita para definir o fator de potência usando Modbus
    // Por enquanto, apenas simulação
    return true;
}

bool Inverter::setEpochTime(uint32_t epoch) {
    if (_map.serial.address == 0xFFFF || !_map.time_epoch.writable) return false;
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

// Private methods para leitura e escrita de registradores Modbus, aplicando a escala correta, lidando com tipos de dados diferentes (inteiros, floats, etc.) e combinando registradores quando necessário. Esses métodos vão usar as informações do ModbusField para fazer a leitura/escrita correta.

bool Inverter::readField(const ModbusField& field, char* value) {
    if (field.type != ASCII || !field.readable) return false;
    if (value == nullptr) return false;

    if (field.stride == 1) {
        // Caso simples: os caracteres estão em registradores consecutivos
        if (!readHoldingRegister(field.address, (uint16_t*)value, field.length)) {
            return false;
        }

        value [field.length * 2] = '\0'; // Garantir terminação nula
        return true;
    }

    for (uint16_t i = 0; i < field.length; i++) {
        if (!readHoldingRegister(field.address + i * field.stride, (uint16_t*)(value + i * 2), 1)) {
            return false;
        }
    }

    value[field.length * 2] = '\0'; // Garantir terminação nula
    return true;
}

bool Inverter::readField(const ModbusField& field, float& value) {
    if (field.type != FLOAT32 || !field.readable) return false;
    /*if (!readHoldingRegister(field.address, (uint16_t*)&value, field.length)) {
        return false;
    }
    value *= field.scale;*/
    return true;
}

bool Inverter::readField(const ModbusField& field, uint16_t& value) {
    if (field.type != U16 || !field.readable) return false;
    /*if (!readHoldingRegister(field.address, &value, field.length)) {
        return false;
    }
    value *= field.scale;*/
    return true;
}

bool Inverter::readField(const ModbusField& field, uint32_t& value) {
    if (field.type != U32 || !field.readable) return false;
    /*if (!readHoldingRegister(field.address, (uint16_t*)&value, field.length)) {
        return false;
    }
    value *= field.scale;*/
    return true;
}

bool Inverter::readField(const ModbusField& field, int16_t& value) {
    if (field.type != I16 || !field.readable) return false;
    /*if (!readHoldingRegister(field.address, (uint16_t*)&value, field.length)) {
        return false;
    }
    value *= field.scale;*/
    return true;
}

bool Inverter::readField(const ModbusField& field, int32_t& value) {
    if (field.type != I32 || !field.readable) return false;
    /*if (!readHoldingRegister(field.address, (uint16_t*)&value, field.length)) {
        return false;
    }
    value *= field.scale;*/
    return true;
}

bool Inverter::readField(const ModbusField& field, StringValues& values) {
    /*if (field.type != STRING || !field.readable) return false;
    if (!readHoldingRegister(field.address, (uint16_t*)values.data(), field.length)) {
        return false;
    }
    values[field.length * 2] = '\0';*/
    return true;
}

bool Inverter::readField(const ModbusField& field, PhaseData& data) {
    /*if (field.type != PHASE_DATA || !field.readable) return false;
    if (!readHoldingRegister(field.address, (uint16_t*)data.data(), field.length)) {
        return false;
    }*/
    return true;
}


bool Inverter::readHoldingRegister(uint16_t startReg, uint16_t* buffer, uint16_t count) {
    if (_mb == nullptr || _modbus == nullptr) return false;
    if (buffer == nullptr) return false;
    if (count == 0) return false;

    _mb_done = false;
    _mb_success = false;

    if (!_mb->readHreg(_modbus->getId(), startReg, buffer, count, _mb_cb)) {
        return false;
    }

    uint32_t start = millis();

    while(!_mb_done && millis() - start < 1000) {
        _mb->task();
        yield();
    }

    return _mb_success;
}

bool Inverter::writeHoldingRegister(uint16_t startReg, uint16_t buffer, uint16_t count) {
    return false; // Implementação de escrita de registradores Modbus, similar à leitura, mas usando as funções de escrita do ModbusRTU e aplicando a escala correta, etc.
}

