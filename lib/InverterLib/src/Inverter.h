#ifndef INVERTER_H
#define INVERTER_H

#include <Arduino.h>
#include "ModbusConfig.h"
#include "InverterMaps.h"
#include "InverterDescriptor.h"
#include "InverterModels.h"

#define MAX_STRINGS 24
#define MAX_BATTERIES 12

struct PhaseData {
    float grid = NAN;
    float r = NAN;
    float s = NAN;
    float t = NAN;
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
    ModbusConfig modbus;

    Inverter(InverterModel model);

    bool begin();

    // Identificação
    bool getSerial(String& serial); // retorna uma string contendo o numero serial RO
    // Controle
    bool boot();    //WO
    bool setBoot(bool boot); // true para ligar, false para desligar WO
    bool shutdown();  //WO
    bool setPowerLimitEnabled(bool enabled); // WO
    bool setPowerLimit(float watts);    // WO
    bool setPowerLimitPercent(float percent);  // WO
    bool setExportLimitEnabled(bool enabled); // WO
    bool setExportLimit(float watts);    // WO
    bool setExportLimitPercent(float percent);  // WO
    bool setPowerFactorEnabled(bool enabled); // WO
    bool setPowerFactor(float pf); // 0~1 para indutivo, 1 para unity, >1 para capacitivo WO

    bool isBooted(bool& isBooted); // retorna true se o inversor estiver ligado, false caso contrário RO   
    bool isPowerLimitEnabled(bool& enabled); // RO
    bool getPowerLimit(float& watts);   // RO    
    bool getPowerLimitPercent(float& percent); // RO    
    bool isExportLimitEnabled(bool& enabled); // RO
    bool getExportLimit(float& watts);   // RO
    bool getExportLimitPercent(float& percent); // RO
    bool isPowerFactorEnabled(bool& enabled); // RO
    bool getPowerFactorSetpoint(float& pf); // RO    

    // Tempo
    bool getYear(uint16_t& year);   // RO
    bool getMonth(uint16_t& month); // RO
    bool getDay(uint16_t& day);     // RO
    bool getHour(uint16_t& hour);   // RO
    bool getMinute(uint16_t& minute); // RO
    bool getSecond(uint16_t& second); // RO
    bool getEpochTime(uint32_t& epoch); // RO

    bool setYear(uint16_t year);    // WO
    bool setMonth(uint16_t month);  // WO
    bool setDay(uint16_t day);      // WO
    bool setHour(uint16_t hour);    // WO
    bool setMinute(uint16_t minute);    // WO
    bool setSecond(uint16_t second);    // WO
    bool setEpochTime(uint32_t epoch);  // WO

    bool getTotalEnergy(float& kWh); // RO
    bool getDailyEnergy(float& kWh); // RO

    bool getActivePower(float& watts); // RO   
    bool getReactivePower(float& voltAmperReactive); // RO
    bool getApparentPower(float& voltAmper); // RO
    bool getPowerFactor(float &pf); // RO

    bool getGridVoltage(PhaseData& phase);    //Retorna struct {r,s,t}
    bool getGridCurrent(PhaseData& phase);    //Retorna struct {r,s,t}
    bool getGridFrequency(PhaseData& phase);  //Retorna struct {grid,r,s,t}

    bool getTemperature(float& temperature); // RO
    bool getInsulationResistance(float& kiloOhms); // RO
    bool getInverterStatus(InverterStatus& status); // Retorna item de enum de acordo com o status do inversor
    bool getAlarm(Alarm& alarm);          // Retorna item de enum de acordo com o alarme ativo no inversor

    bool getStringVoltage(StringValues& voltage); // Retorna vetor
    bool getStringCurrent(StringValues& current); // Retorna vetor
    bool getStringPower(StringValues& power);   // Retorna vetor

    bool getBatteryVoltage(BatteryValues& voltage); // Retorna vetor
    bool getBatteryCurrent(BatteryValues& current); // Retorna vetor
    bool getBatteryPower(BatteryValues& power);     // Retorna vetor
    bool getBatterySoC(BatteryValues& soc);       // Retorna vetor
    bool getBatterySoH(BatteryValues& soh);       // Retorna vetor

    bool getEPSVoltage(PhaseData& phase); // Retorna struct {grid,r,s,t}
    bool getEPSCurrent(PhaseData& phase); // Retorna struct {grid,r,s,t}
    bool getEPSActivePower(PhaseData& phase); // Retorna struct {grid,r,s,t}

    bool readHoldingRegisters(uint16_t startReg, uint16_t* buffer, uint16_t count);
    bool writeHoldingRegisters(uint16_t startReg, const uint16_t* buffer, uint16_t count);

private:
    InverterModel _model;
    const InverterDescriptor* _descriptor;
    const ModbusInverterMap* _map;

    String _serial;

    // Resolve 90% dos casos
    bool readField(const ModbusField& field, float& value);
    bool readField(const ModbusField& field, StringValues& values);
    bool readField(const ModbusField& field, PhaseData& data);

    uint16_t readHoldingRegister(uint16_t reg);
    bool writeHoldingRegister(uint16_t reg, uint16_t value);
};

#endif