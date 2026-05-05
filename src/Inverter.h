/*
 * InverterModbusLib - Solar Inverter Library for Arduino
 * ------------------------------------------------
 * Modbus RTU (RS485)communication layer for inverter integration
 *
 * Copyright (c) 2026, Pedro Akio Sakuma
 * Licensed under BSD 3-Clause License
 */

#ifndef INVERTER_H
#define INVERTER_H

#include <Arduino.h>
#include "ModbusConfig.h"
#include "InverterMaps.h"
#include "InverterDescriptor.h"
#include "InverterModels.h"

constexpr uint8_t INV_MAX_U16_VALUES =   28;
constexpr uint8_t INV_MAX_U32_VALUES =   3;
constexpr uint8_t INV_MAX_U64_VALUES =   1;
constexpr uint8_t INV_MAX_FLOAT_VALUES = 3;
constexpr uint8_t INV_MAX_STRING_CHARS = 32;

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

/*enum A{
    AVERAGE,
    ESTIMATED,
    SUM,
};*/

struct PhaseData {
    float grid = NAN;
    float r = NAN;
    float s = NAN;
    float t = NAN;
    bool isEstimated;
};

enum InverterStatus {
    INVERTER_STATUS_OK,
    INVERTER_STATUS_WARNING,
    INVERTER_STATUS_ERROR,
    INVERTER_STATUS_UNKNOWN
};

enum Alarm {
    ALARM_NONE,
    ALARM_OVERVOLTAGE,
    ALARM_UNDERVOLTAGE,
    ALARM_OVERCURRENT,
    ALARM_OVERPOWER,
    ALARM_TEMPERATURE,
    ALARM_COMMUNICATION,
    ALARM_UNKNOWN
};

enum PfExcitationMode {
    LAGGING,            // +j
    LEADING,            // -j
    INDUCTIVE,          // +j
    CAPACITIVE,         // -j
    OVER_EXCITED,       // +j
    UNDER_EXCITED       // -j
};

struct StringValues {
    uint8_t count; // Número de strings ativas
    float values[MAX_STRINGS];
    bool isEstimated;
};

struct BatteryValues {
    uint8_t count; // Número de baterias ativas
    float values[MAX_BATTERIES];
};

class Inverter {
public:
    void attachModbus(ModbusRTU& mb);               // ✓
    void attachConfig(const ModbusConfig& config);        // ✓

    Inverter(InverterModel model);                  // ✓

    bool begin();                                   // ✓

    void setSlaveId(uint8_t id);                    // ✓

    // Identificação
    bool getSerial(String& serial);                  // RO retorna uma string contendo o numero serial  ✓
    // Controle
    bool boot();                                     // WO ✓
    bool setBoot(bool boot);                         // WO ✓
    bool shutdown();                                 // WO ✓
    bool setPowerLimitEnabled(bool enabled);         // WO ✓
    bool setPowerLimit(float watts);                 // WO ✓
    bool setPowerLimitPercent(float percent);        // WO ✓
    bool setExportLimitEnabled(bool enabled);        // WO ✓
    bool setExportLimit(float watts);                // WO ✓
    bool setExportLimitPercent(float percent);       // WO ✓
    bool setPowerFactorEnabled(bool enabled);        // WO ✓
    bool setPowerFactor(float pf);                   // WO ✓
    bool setPowerFactorExcitationMode(PfExcitationMode excitationMode); // WO ✓

    bool isBooted(bool& isBooted);                   // RO - retorna true se o inversor estiver ligado, false caso contrário   
    bool isPowerLimitEnabled(bool& enabled);         // RO
    bool getPowerLimit(float& watts);                // RO    ✓ 
    bool getPowerLimitPercent(float& percent);       // RO    ✓ 
    bool isExportLimitEnabled(bool& enabled);        // RO
    bool getExportLimit(float& watts);               // RO
    bool getExportLimitPercent(float& percent);      // RO
    bool isPowerFactorEnabled(bool& enabled);        // RO
    bool getPowerFactorSetpoint(float& pf);          // RO    

    // Tempo
    // Arquivo InverterTime.cpp
    bool getDatetime(Datetime& dt);                  // RO  ✓
    bool getYear(uint16_t& year);                    // RO  ✓
    bool getMonth(uint16_t& month);                  // RO  ✓
    bool getDay(uint16_t& day);                      // RO  ✓
    bool getHour(uint16_t& hour);                    // RO  ✓
    bool getMinute(uint16_t& minute);                // RO  ✓
    bool getSecond(uint16_t& second);                // RO  ✓
    bool getEpochTime(uint32_t& epoch);              // RO  ✓

    bool setDatetime(Datetime dt);                   // WO  
    bool setYear(uint16_t year);                     // WO  ✓
    bool setMonth(uint16_t month);                   // WO  ✓
    bool setDay(uint16_t day);                       // WO  ✓
    bool setHour(uint16_t hour);                     // WO  ✓
    bool setMinute(uint16_t minute);                 // WO  ✓
    bool setSecond(uint16_t second);                 // WO  ✓
    bool setEpochTime(uint32_t epoch);               // WO  ✓

    bool getTotalEnergy(float& kWh);                 // RO
    bool getDailyEnergy(float& kWh);                 // RO

    bool getActivePower(float& watts);               // RO   
    bool getReactivePower(float& voltAmperReactive); // RO
    bool getApparentPower(float& voltAmper);         // RO
    bool getPowerFactor(float &pf);                  // RO

    bool getGridVoltage(PhaseData& phase);           // RO - Retorna struct {r,s,t}
    bool getGridCurrent(PhaseData& phase);           // RO - Retorna struct {r,s,t}
    bool getGridFrequency(PhaseData& phase);         // RO - Retorna struct {grid,r,s,t}

    bool getTemperature(float& temperature);         // RO
    bool getInsulationResistance(float& kiloOhms);   // RO
    bool getInverterStatus(InverterStatus& status);  // RO - Retorna item de enum de acordo com o status do inversor
    bool getAlarm(Alarm& alarm);                     // RO - Retorna item de enum de acordo com o alarme ativo no inversor

    bool getStringVoltage(StringValues& voltage);    // RO - Retorna vetor
    bool getStringCurrent(StringValues& current);    // RO - Retorna vetor
    bool getStringPower(StringValues& power);        // RO - Retorna vetor

    bool getBatteryVoltage(BatteryValues& voltage);  // RO - Retorna vetor
    bool getBatteryCurrent(BatteryValues& current);  // RO - Retorna vetor
    bool getBatteryPower(BatteryValues& power);      // RO - Retorna vetor
    bool getBatterySoC(BatteryValues& soc);          // RO - Retorna vetor
    bool getBatterySoH(BatteryValues& soh);          // RO - Retorna vetor

    bool getEPSVoltage(PhaseData& phase);            // RO - Retorna struct {grid,r,s,t}
    bool getEPSCurrent(PhaseData& phase);            // RO - Retorna struct {grid,r,s,t}
    bool getEPSActivePower(PhaseData& phase);        // RO - Retorna struct {grid,r,s,t}

private:
    ModbusRTU* _mb = nullptr;
    ModbusConfig _modbus;
    bool _customConfigSet = false;

    InverterModel _model;
    InverterDescriptor _descriptor;
    ModbusInverterMap _map;

    String _serial;

    bool readScaledFloat(const ModbusField& field, float& value);           // ✓
    bool isLeap(uint16_t y);                                                // ✓
    Datetime epochToDatetime(uint32_t epoch);                               // ✓
    bool isValidDatetime(const Datetime& dt);                               // ✓
    uint32_t datetimeToEpoch(const Datetime& dt);                           // ✓
    bool isInvalidField(const ModbusField& field);                          // ✓

    // Resolve 90% dos casos
    // Arquivo InverterCore.cpp
    bool readField(const ModbusField& field, char* value);                  // ✓
    bool readField(const ModbusField& field, float* value);                 // ✓
    bool readField(const ModbusField& field, uint16_t* value);              // ✓
    bool readField(const ModbusField& field, uint32_t* value);              // ✓
    bool readField(const ModbusField& field, uint64_t* value);              // ✓
    bool readField(const ModbusField& field, int16_t* value);               // ✓
    bool readField(const ModbusField& field, int32_t* value);               // ✓
    bool readField(const ModbusField& field, int64_t* value);               // ✓

    bool readField16Raw(const ModbusField& field, uint16_t* buffer);        // ✓
    bool readField32Raw(const ModbusField& field, uint32_t* buffer);        // ✓
    bool readField64Raw(const ModbusField& field, uint64_t* buffer);        // ✓

    bool writeField(const ModbusField& field, float value); // ✓
    bool writeField(const ModbusField& field, uint16_t value);       // ✓
    bool writeField(const ModbusField& field, uint32_t value);       // ✓
    bool writeField(const ModbusField& field, int16_t value);        // ✓
    bool writeField(const ModbusField& field, int32_t value);        // ✓
    bool writeField(const ModbusField& field, float* value, uint8_t count = 1); // ✓
    bool writeField(const ModbusField& field, uint16_t* value, uint8_t count = 1);       // ✓
    bool writeField(const ModbusField& field, uint32_t* value, uint8_t count = 1);       // ✓
    bool writeField(const ModbusField& field, int16_t* value, uint8_t count = 1);        // ✓
    bool writeField(const ModbusField& field, int32_t* value, uint8_t count = 1);        // ✓

    bool writeField16Raw(const ModbusField& field, uint16_t* value, uint8_t count = 1);  // ✓
    bool writeField32Raw(const ModbusField& field, uint32_t* value, uint8_t count = 1);  // ✓

    bool readHoldingRegister(uint16_t reg, uint16_t* value, uint16_t count = 1);    // ✓
    bool writeHoldingRegister(uint16_t reg, uint16_t* value, uint16_t count = 1);   // ✓
};

#endif