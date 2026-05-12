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

struct PhaseData {
    float r;
    float s;
    float t;
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
};

struct BatteryValues {
    uint8_t count; // Número de baterias ativas
    float values[MAX_BATTERIES];
};

class Inverter {
public:

    // Setters
    // Arquivo InverterControl.cpp
    // ------------------------------------------------------
    Inverter(InverterModel model);                      //
    void attachModbus(ModbusRTU& mb);                   //
    void attachConfig(const ModbusConfigData& config);  //  
    void attachSerial(HardwareSerial& serial);          //
    bool begin();                                       //
    void setSlaveId(uint8_t id);                        //
    bool boot();                                        //
    bool setBoot(bool enable);                          //
    bool shutdown();                                    //
    bool setPowerLimitEnabled(bool enabled);            //
    bool setPowerLimit(float watts);                    //
    bool setPowerLimitPercent(float percent);           //
    bool setExportLimitEnabled(bool enabled);           //
    bool setExportLimit(float watts);                   //
    bool setExportLimitPercent(float percent);          //
    bool setPowerFactorEnabled(bool enabled);           //
    bool setPowerFactor(float pf);                      //
    bool setPowerFactorExcitationMode(PfExcitationMode excitationMode); //
    // ------------------------------------------------------
  
    // Tempo
    // Arquivo InverterTime.cpp
    // ------------------------------------------------------
    // Leitura de data/hora
    bool getDatetime(Datetime& dt);                  //
    bool getYear(uint16_t& year);                    //
    bool getMonth(uint16_t& month);                  //
    bool getDay(uint16_t& day);                      //
    bool getHour(uint16_t& hour);                    //
    bool getMinute(uint16_t& minute);                //
    bool getSecond(uint16_t& second);                //
    bool getEpochTime(uint32_t& epoch);              //
    // Escrita de data/hora
    bool setDatetime(Datetime dt);                   //
    bool setYear(uint16_t year);                     //
    bool setMonth(uint16_t month);                   //
    bool setDay(uint16_t day);                       //
    bool setHour(uint16_t hour);                     //
    bool setMinute(uint16_t minute);                 //
    bool setSecond(uint16_t second);                 //
    bool setEpochTime(uint32_t epoch);               //
    // ------------------------------------------------------

    // Getters
    // Arquivo InverterDeviceInfo.cpp
    // ------------------------------------------------------
    // Identificação
    bool getSerialNumber(String& serialNumber);                  //
    // Comandos / Limites
    bool isBooted(bool& isBooted);                   //
    bool isPowerLimitEnabled(bool& enabled);         //
    bool getPowerLimit(float& watts);                //
    bool getPowerLimitPercent(float& percent);       //
    bool isExportLimitEnabled(bool& enabled);        //
    bool getExportLimit(float& watts);               //
    bool getExportLimitPercent(float& percent);      //
    bool isPowerFactorEnabled(bool& enabled);        //
    bool getPowerFactorSetpoint(float& pf);          //
    // Medições AC
    bool getActivePower(float& watts);               // 
    bool getReactivePower(float& voltAmperReactive); //
    bool getApparentPower(float& voltAmper);         //
    bool getPowerFactor(float &pf);                  //
    bool getGridVoltage(PhaseData& phase);           //
    bool getGridCurrent(PhaseData& phase);           //
    bool getGridFrequency(PhaseData& phase);         //
    // Energia
    bool getTotalEnergy(float& kWh);                 //
    bool getDailyEnergy(float& kWh);                 //
    // Strings FV
    bool getStringVoltage(StringValues& voltage);    //
    bool getStringCurrent(StringValues& current);    //
    bool getStringPower(StringValues& power);        //
    // Bateria
    bool getBatteryVoltage(BatteryValues& voltage);  //
    bool getBatteryCurrent(BatteryValues& current);  //
    bool getBatteryPower(BatteryValues& power);      //
    bool getBatterySoC(BatteryValues& soc);          //
    bool getBatterySoH(BatteryValues& soh);          //    
    // EPS
    bool getEPSVoltage(PhaseData& phase);            //
    bool getEPSCurrent(PhaseData& phase);            //
    bool getEPSActivePower(PhaseData& phase);        //    
    // Diagnóstico e Saúde
    bool getTemperature(float& temperature);         //
    bool getInsulationResistance(float& kiloOhms);   //
    // Status/Alarmes
    bool getInverterStatus(uint16_t status);//InverterStatus& status);  //
    bool getAlarm(uint16_t alarm);//Alarm& alarm);                     //
    // ------------------------------------------------------

private:

    // Variáveis privadas
    // ------------------------------------------------------
    ModbusRTU* _mb = nullptr;                        //
    ModbusConfigData _cfg;                           //
    ModbusConfig _modbus;                            //
    HardwareSerial* _serialPort;                     //
    InverterModel _model;                            //
    InverterDescriptor _descriptor;                  //
    ModbusInverterMap _map;                          //
    String _serialNumber;                            //
    // ------------------------------------------------------

    // Arquivo InverterControl.cpp
    // ------------------------------------------------------
    // Helpers internos
    bool isInvalidField(const ModbusField& field);   //
    // ------------------------------------------------------

    // Arquivo InverterTime.cpp
    // ------------------------------------------------------
    // Conversões e validações internas
    bool isLeap(uint16_t y);                         //
    Datetime epochToDatetime(uint32_t epoch);        //
    bool isValidDatetime(const Datetime& dt);        //
    uint32_t datetimeToEpoch(const Datetime& dt);    //
    // ------------------------------------------------------

    // Centro das operações Modbus
    // Arquivo InverterCore.cpp
    // ------------------------------------------------------
    // Conversão escalada
    bool readScaledFloat(const ModbusField& field, float& value);                       //
    bool readScaledFloat(const ModbusField& field, float* value, uint8_t count = 1);    //
    // Leitura tipada
    bool readField(const ModbusField& field, char* value);                              //
    bool readField(const ModbusField& field, float* value);                             //
    bool readField(const ModbusField& field, uint16_t* value);                          //
    bool readField(const ModbusField& field, uint32_t* value);                          //
    bool readField(const ModbusField& field, uint64_t* value);                          //
    bool readField(const ModbusField& field, int16_t* value);                           //
    bool readField(const ModbusField& field, int32_t* value);                           //
    bool readField(const ModbusField& field, int64_t* value);                           //
    // Leitura raw
    bool readField16Raw(const ModbusField& field, uint16_t* buffer);                    //
    bool readField32Raw(const ModbusField& field, uint32_t* buffer);                    //
    bool readField64Raw(const ModbusField& field, uint64_t* buffer);                    //
    // Escrita tipada
    bool writeField(const ModbusField& field, float value);                             //
    bool writeField(const ModbusField& field, uint16_t value);                          //
    bool writeField(const ModbusField& field, uint32_t value);                          //
    bool writeField(const ModbusField& field, int16_t value);                           //
    bool writeField(const ModbusField& field, int32_t value);                           //
    bool writeField(const ModbusField& field, float* value, uint8_t count = 1);         //
    bool writeField(const ModbusField& field, uint16_t* value, uint8_t count = 1);      //
    bool writeField(const ModbusField& field, uint32_t* value, uint8_t count = 1);      //
    bool writeField(const ModbusField& field, int16_t* value, uint8_t count = 1);       //
    bool writeField(const ModbusField& field, int32_t* value, uint8_t count = 1);       //
    // Escrita raw
    bool writeField16Raw(const ModbusField& field, uint16_t* value, uint8_t count = 1); //
    bool writeField32Raw(const ModbusField& field, uint32_t* value, uint8_t count = 1); //
    // Acesso Modbus direto
    bool readHoldingRegister(uint16_t reg, uint16_t* value, uint16_t count = 1);        //
    bool writeHoldingRegister(uint16_t reg, uint16_t* value, uint16_t count = 1);       //
    // ------------------------------------------------------
};

#endif