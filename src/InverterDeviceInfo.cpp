/*
 * InverterModbusLib - Solar Inverter Library for Arduino
 * ------------------------------------------------
 * Modbus RTU (RS485)communication layer for inverter integration
 *
 * Copyright (c) 2026, Pedro Akio Sakuma
 * Licensed under BSD 3-Clause License
 */
 /*
InverterDeviceInfo.cpp
├── Identificação
│   └── getSerial()
│
├── Comandos/Limites
│   ├── isPowerLimitEnabled()
│   ├── getPowerLimit()
│   ├── getPowerLimitPercent()
│   ├── isExportLimitEnabled()
│   ├── getExportLimit()
│   ├── getExportLimitPercent()
│   ├── isPowerFactorEnabled()
│   └── getPowerFactorSetpoint()
│
├── Medições AC
│   ├── getActivePower()
│   ├── getReactivePower()
│   ├── getApparentPower()
│   ├── getPowerFactor()
│   ├── getGridVoltage()
│   ├── getGridCurrent()
│   └── getGridFrequency()
│
├── Energia
│   ├── getTotalEnergy()
│   └── getDailyEnergy()
│
├── Strings FV
│   ├── getStringVoltage()
│   ├── getStringCurrent()
│   └── getStringPower()
│
├── Bateria
│   ├── getBatteryVoltage()
│   ├── getBatteryCurrent()
│   ├── getBatteryPower()
│   ├── getBatterySoC()
│   └── getBatterySoH()
│
├── EPS
│   ├── getEPSVoltage()
│   ├── getEPSCurrent()
│   └── getEPSActivePower()
│
├── Diagnóstico e Saúde
│   ├── getTemperature()
│   └── getInsulationResistance()
│
└── Status/Alarmes
    ├── getInverterStatus()
    └── getAlarm()
    */

 #include "Inverter.h"

// ======================================================
// Identification
// ======================================================

bool Inverter::hasValidMap() const {
    return _map.identification.serialNumber.address != 0xFFFF;
}

bool Inverter::getSerialNumber(String& serialNumber) {
    if (!hasValidMap()) return false;    
    const ModbusField& field = _map.identification.serialNumber;

    switch (field.mode) {

        case FIELD_SIMPLE:
            if (!field.readable) return false;
            if (field.type == ASCII) {
                char buffer[INV_MAX_STRING_CHARS + 1];
                // Buffer limitado pelo tamanho máximo definido pela lib, mais um para o terminador nulo
                if (!readField(field, buffer)) return false;
                
                serialNumber = String(buffer);
                return true;                
            }
            
            if (field.type == U32) {
                uint32_t buffer; // Ler um valor de 32 bits
                if (!readField(field, &buffer)) return false;

                serialNumber = String(buffer); // Transforma o número em string
                return true;
            }

            // if (field.type == U64) {
            //     uint64_t buffer;
            //     if (!readField(field, &buffer)) return false;

            //     serialNumber = String(buffer); // Transforma o número em string
            //     return true;
            // }
            return false;

        default:
            return false;
    }
}

// ======================================================
// Limits and Control State
// ======================================================

bool Inverter::isPowerLimitEnabled(bool& enabled) {
    if (!hasValidMap()) return false;
    const ActivePowerFeature& feature = _map.activePower;
    
    if (!feature.supportsEnable) {
        if (feature.implicitEnable) {
            enabled = true; // Mapa do Huawei não tem reg de habilitar a limitação
            return true;
        }
        return false;
    }

    const ModbusField& field = feature.enable;

    switch (field.mode) {

        case FIELD_SIMPLE:{
            if (!field.readable) return false;

            uint16_t e;
            if (!readField(field, &e)) return false;

            // Em alguns inversores, o registrador pode ser compartilhado
            // entre diferentes modos. Portanto, somente o valor expecífico
            // de power limit deve retornar true
            enabled = (e == feature.enableValue);
            return true;
        }

        default:
            return false;
    }
}

bool Inverter::getPowerLimit(float& watts) {
    if (!hasValidMap()) return false;
    
    const ActivePowerFeature& feature = _map.activePower;
    
    if (feature.supportsWatts) {
        const ModbusField& field = feature.watts;

        switch (field.mode) {
            case FIELD_SIMPLE:
                return readScaledFloat(field, watts);
                
            default:
                return false;
        }
    }

    if (feature.supportsPercent) {
        const ModbusField& field = feature.percent;

        switch (field.mode) {
            case FIELD_SIMPLE: {
                if (!field.readable) return false;
                if (_descriptor.nominalPowerW == 0) return false;
                // Trocar por getNominalPower. Ler registrador de potencia nominal e adicionar fallback para o descriptor

                float percent;
                if (!readScaledFloat(field, percent)) return false;

                watts = ((float)_descriptor.nominalPowerW * percent) / 100.0f;
                return true;
            }

            default:
                return false;
        }
    }

    return false;
}

bool Inverter::getPowerLimitPercent(float& percent) {
    if (!hasValidMap()) return false;

    const ActivePowerFeature& feature = _map.activePower;

    if (feature.supportsPercent) {
        const ModbusField& field = feature.percent;

        switch (field.mode) {
            case FIELD_SIMPLE:
                return readScaledFloat(field, percent);

            default:
                return false;
        }
    }

    if (feature.supportsWatts) {
        const ModbusField& field = feature.watts;

        switch (field.mode) {
            case FIELD_SIMPLE: {
                if (!field.readable) return false;
                if (_descriptor.nominalPowerW == 0) return false;
                // Trocar por getNominalPower. Ler registrador de potencia nominal e adicionar fallback para o descriptor

                float watts;
                if (!readScaledFloat(field, watts)) return false;

                percent = (watts / (float)_descriptor.nominalPowerW) * 100.0f;
                return true;
            }

            default:
                return false;
        }
    }

    return false;
}

bool Inverter::isExportLimitEnabled(bool& enabled) {
    if (!hasValidMap()) return false;
    const ExportLimitFeature& feature = _map.exportLimit;

    if (!feature.supportsEnable) {
        if (feature.implicitEnable) {
            enabled = true;
            return true;
        }
        return false;
    }

    const ModbusField& field = feature.enable;

    switch (field.mode) {

        case FIELD_SIMPLE: {
            if (!field.readable) return false;

            uint16_t e;
            if (!readField(field, &e)) return false;

            // Em alguns inversores, o registrador pode ser compartilhado
            // entre diferentes modos. Portanto, somente o valor expecífico
            // de export limit deve retornar true
            enabled = (e == feature.enableValue);
            return true;
        }

        default:
            return false;
    }
}

bool Inverter::getExportLimit(float& watts) {
    if (!hasValidMap()) return false;

    const ExportLimitFeature& feature = _map.exportLimit;

    if (feature.supportsWatts) {
        const ModbusField& field = feature.watts;

        switch (field.mode) {

        case FIELD_SIMPLE:
            return readScaledFloat(field, watts);

        default:
            return false;
        }
    }

    if (feature.supportsPercent) {
        const ModbusField& field = feature.percent;
        switch (field.mode) {
            case FIELD_SIMPLE: {
                if (!field.readable) return false;
                if (_descriptor.nominalPowerW == 0) return false;
                // Trocar por getNominalPower. Ler registrador de potencia nominal e adicionar fallback para o descriptor

                float percent;
                if (!readScaledFloat(field, percent)) return false;

                watts = ((float)_descriptor.nominalPowerW * percent) / 100.0f;
                return true;
            }

            default:
                return false;
        }
        
    }

    return false;
}

bool Inverter::getExportLimitPercent(float& percent) {
    if (!hasValidMap()) return false;

    const ExportLimitFeature& feature = _map.exportLimit;

    if (feature.supportsPercent) {
        const ModbusField& field = feature.percent;

        switch (field.mode) {
            case FIELD_SIMPLE:
                return readScaledFloat(field, percent);

            default:
                return false;
        }
    }

    if (feature.supportsWatts) {
        const ModbusField& field = feature.watts;

        switch (field.mode) {
            case FIELD_SIMPLE: {
                if (!field.readable) return false;
                if (_descriptor.nominalPowerW == 0) return false;
                // Trocar por getNominalPower. Ler registrador de potencia nominal e adicionar fallback para o descriptor

                float watts;
                if (!readScaledFloat(field, watts)) return false;

                percent = (watts / (float)_descriptor.nominalPowerW) * 100.0f;
                return true;
            }

            default:
                return false;
        }
    }

    return false;
}

bool Inverter::isPowerFactorEnabled(bool& enabled) {
    if (!hasValidMap()) return false;

    const ReactivePowerFeature& feature = _map.reactivePowerControl;

    if (!feature.supportsEnablePf) {
        const ModbusField& field = feature.enablePf;

        switch (field.mode) {

            case FIELD_SIMPLE: {
                if (!field.readable) return false;

                uint16_t e;
                if (!readField(field, &e)) return false;

                // Em alguns inversores, o registrador pode ser compartilhado
                // entre diferentes modos. Portanto, somente o valor expecífico
                // de export limit deve retornar true
                enabled = (e == feature.enablePfValue);
                return true;
            }

            default:
                return false;
        }
    }

    if (feature.supportsControlModePf) {
        const ModbusField& field = feature.controlMode;

        switch (field.mode) {
            case FIELD_SIMPLE: {
                if (!field.readable) return false;

                uint16_t mode;
                if (!readField(field, &mode)) return false;

                enabled = (mode == feature.enablePfValue);
                return true;
            }

            default:
                return false;
        }
    }

    if (feature.implicitPfSp) {
        enabled = true;
        return true;
    }

    return false;
}

bool Inverter::getPowerFactorSetpoint(float& pf) {
    if (!hasValidMap()) return false;

    const ReactivePowerFeature feature = _map.reactivePowerControl;

    if (!feature.supportsPfSp) return false;

    const ModbusField& field = feature.pfSetpoint;

    switch (field.mode) {

        case FIELD_SIMPLE:
            if (!field.readable) return false;
            return readScaledFloat(field, pf);

        default:
            return false;
    }

    return false;
}

// ======================================================
// AC Measurements
// ======================================================

bool Inverter::getActivePower(float& watts) {
    if (!hasValidMap()) return false;

    const ModbusField& field = _map.power.activePower;
    
    switch (field.mode) {

        case FIELD_SIMPLE:
            if (!field.readable) return false;
            return readScaledFloat(field, watts);
            
        default:
            return false;
    }
}
   
bool Inverter::getReactivePower(float& var) {
    if (!hasValidMap()) return false;

    const ModbusField& field = _map.power.reactivePower;
    
    switch (field.mode) {

        case FIELD_SIMPLE:
            if (!field.readable) return false;
            return readScaledFloat(field, var);
            
        default:
            return false;
    }
}

bool Inverter::getApparentPower(float& va) {
    if (!hasValidMap()) return false;

    const ModbusField& field = _map.power.apparentPower;
    
    switch (field.mode) {

        case FIELD_SIMPLE:
            if (!field.readable) return false;
            return readScaledFloat(field, va);
            
        default:
            return false;
    }
}

bool Inverter::getPowerFactor(float &pf) {
    if (!hasValidMap()) return false;

    const ModbusField& fieldPf = _map.power.powerFactor;
    
    switch (fieldPf.mode) {

        case FIELD_SIMPLE: {

            if (!fieldPf.readable) return false;
            
            float scaledPf;
            if (!readScaledFloat(fieldPf, scaledPf)) return false;

            const ReactivePowerFeature& featureReactive = _map.reactivePowerControl;

            if (featureReactive.supportsExcitationMode) {
                const ModbusField& fieldExcitation = featureReactive.excitationMode;

                if (!isInvalidField(fieldExcitation) && fieldExcitation.readable) {
                    uint16_t signal = 0;

                    if (!readField(fieldExcitation, &signal)) return false;

                    // 0 - mantém o sinal; 1 - troca o sinal
                    pf = (signal != 0) ? -scaledPf : scaledPf;
                    return true;
                }                
            }
               
            // Sem excitationMode, assume que o valor lido está correto
            pf = scaledPf;
            return true;
        }

        default:
            return false;
    
    }
}

bool Inverter::getGridVoltage(float& voltage) {
    if (!hasValidMap()) return false;

    const GridFeature& feature = _map.grid;

    if (!feature.supportsPhaseVoltage) return false;
    if (feature.phaseCount != 1) return false;

    const ModbusField& field = feature.phaseVoltage;

    if (field.length != 1) return false;

    switch (field.mode) {
        case FIELD_SIMPLE:
            if (!field.readable) return false;
            return readScaledFloat(field, voltage);

        default:
            return false;
    }
}

bool Inverter::getGridPhaseVoltage(PhaseData& phase) {
    if (!hasValidMap()) return false;

    const GridFeature& feature = _map.grid;

    if (!feature.supportsPhaseVoltage) return false;
    if (feature.phaseCount != 3) return false;

    const ModbusField& field = feature.phaseVoltage;
    
    if (field.length != 3) return false;

    switch (field.mode) {

        case FIELD_SIMPLE: {
            if (!field.readable) return false;

            float v[3];

            if (!readScaledFloat(field, v, 3)) return false;

            phase.r = v[0];
            phase.s = v[1];
            phase.t = v[2];

            return true;
        }

        default:
            return false;
    }
}

bool Inverter::getGridLineVoltage(PhaseData& phase) {
    if (!hasValidMap()) return false;

    const GridFeature& feature = _map.grid;

    if (!feature.supportsLineVoltage) return false;
    if (feature.phaseCount != 3) return false;

    const ModbusField& field = feature.lineVoltage;
    
    if (field.length != 3) return false;

    switch (field.mode) {

        case FIELD_SIMPLE: {
            if (!field.readable) return false;

            float v[3];

            if (!readScaledFloat(field, v, 3)) return false;

            phase.r = v[0];
            phase.s = v[1];
            phase.t = v[2];

            return true;
        }

        default:
            return false;
    }
}

bool Inverter::getGridCurrent(float& current) {
    if (!hasValidMap()) return false;

    const GridFeature& feature = _map.grid;

    if (feature.phaseCount != 1) return false;

    const ModbusField& field = feature.current;

    if (field.length != 1) return false;

    switch (field.mode) {
        case FIELD_SIMPLE:
            if (!field.readable) return false;
            return readScaledFloat(field, current);

        default:
            return false;
    }
}

bool Inverter::getGridCurrent(PhaseData& phase) {
    if (!hasValidMap()) return false;

    const GridFeature& feature = _map.grid;

    if (feature.phaseCount != 3) return false;

    const ModbusField& field = feature.current;
    
    if (field.length != 3) return false;

    switch (field.mode) {

        case FIELD_SIMPLE: {
            if (!field.readable) return false;

            float v[3];

            if (!readScaledFloat(field, v, 3)) return false;

            phase.r = v[0];
            phase.s = v[1];
            phase.t = v[2];

            return true;
        }

        default:
            return false;
    }
}

bool Inverter::getGridFrequency(float& freq) {
    if (!hasValidMap()) return false;

    const ModbusField& field = _map.grid.frequency;

    if (!field.length != 1) return false;

    switch (field.mode) {
        case FIELD_SIMPLE:
            if (!field.readable) return false;
            return readScaledFloat(field, freq);

        default:
            return false;
    }
}

// ======================================================
// Energy
// ======================================================

bool Inverter::getTotalEnergy(float& kWh) {
    if (!hasValidMap()) return false;

    const ModbusField& field = _map.energy.total;
    
    switch (field.mode) {

        case FIELD_SIMPLE:
            if (!field.readable) return false;
            return readScaledFloat(field, kWh);
            
        default:
            return false;
    }
}

bool Inverter::getDailyEnergy(float& kWh) {
    if (!hasValidMap()) return false;
    
    const ModbusField& field = _map.energy.daily;

    switch (field.mode) {

        case FIELD_SIMPLE:
            if (!field.readable) return false;
            return readScaledFloat(field, kWh);
            
        default:
            return false;
    }
}

// ======================================================
// PV Strings
// ======================================================
bool Inverter::getPVStringCount(uint16_t& count) {
    if (!hasValidMap()) return false;

    const PvStringFeature& feature = _map.pvString;
    const ModbusField& field = feature.stringCount;

    count = 0;

    // Primeiro tentar ler o registrador do inversor
    if (!isInvalidField(field) && field.readable) {
        switch (field.mode) {
            case FIELD_SIMPLE: {
                uint16_t raw;
                if (readField(field, &raw)) {
                    count = raw;
                    return true;
                }
                break;
            }

            default:
                break;
        }
    }

    // Fallback: usa o descriptor
    if (_descriptor.pvInfo.stringCount > 0) {
        count = _descriptor.pvInfo.stringCount;

        return true;
    }

    return false;
}

bool Inverter::getStringVoltage(StringValues& voltage) {
    if (!hasValidMap()) return false;

    const PvStringFeature& feature = _map.pvString;
    const ModbusField& originalField = feature.voltage;

    if (!originalField.readable) return false;
    if (originalField.length == 0) return false;

    uint16_t count = 0;

    if (!getPVStringCount(count)) return false;

    if (count == 0) return false;
    if (count > MAX_STRINGS) count = MAX_STRINGS;

    uint16_t effectiveLength = originalField.length;

    if (count < effectiveLength) {
        effectiveLength = count;
    }

    if (effectiveLength == 0) return false;
    if (effectiveLength > MAX_STRINGS) effectiveLength = MAX_STRINGS;

    ModbusField field = originalField;
    field.length = effectiveLength;

    float values[MAX_STRINGS];

    switch (field.mode) {

        case FIELD_SIMPLE: {
            if (!readScaledFloat(field, values, field.length)) return false;

            voltage.count = field.length;

            for(uint8_t i = 0; i < voltage.count; i++) {
                voltage.values[i] = values[i];
            }

            return true;
        }
            
        default:
            return false;
    }
}

bool Inverter::getStringCurrent(StringValues& current) {
    if (!hasValidMap()) return false;

    const PvStringFeature& feature = _map.pvString;
    const ModbusField& originalField = feature.current;

    if (!originalField.readable) return false;
    if (originalField.length == 0) return false;

    uint16_t count = 0;

    if (!getPVStringCount(count)) return false;

    if (count == 0) return false;
    if (count > MAX_STRINGS) count = MAX_STRINGS;

    uint16_t effectiveLength = originalField.length;

    if (count < effectiveLength) {
        effectiveLength = count;
    }

    if (effectiveLength == 0) return false;
    if (effectiveLength > MAX_STRINGS) effectiveLength = MAX_STRINGS;

    ModbusField field = originalField;
    field.length = effectiveLength;

    float values[MAX_STRINGS];

    switch (field.mode) {

        case FIELD_SIMPLE: {
            if (!readScaledFloat(field, values, field.length)) return false;

            current.count = field.length;

            // Zera o array antes de atribuir os valores
            for (uint8_t i = 0; i < MAX_STRINGS; i++) {
                current.values[i] = 0.0f;
            }

            for(uint8_t i = 0; i < current.count; i++) {
                current.values[i] = values[i];
            }

            return true;
        }
            
        default:
            return false;
    }
}

bool Inverter::getStringPower(StringValues& power) {
    if (!hasValidMap()) return false;
    
    switch (_map.stringPower.mode) {

        case FIELD_SIMPLE: {
            if (!_map.stringPower.readable) return false;
            if (_map.stringPower.length == 0 || _map.stringPower.length > MAX_STRINGS) return false;
            if (_descriptor.pvInfo.stringCount == 0 || _descriptor.pvInfo.stringCount > MAX_STRINGS) return false;

            float v[MAX_STRINGS];

            if (!readScaledFloat(_map.stringPower, v, _descriptor.pvInfo.stringCount)) return false;

            power.count = _descriptor.pvInfo.stringCount;

            for(uint8_t i = 0; i < power.count; i++) {
                power.values[i] = v[i];
            }

            return true;
        }
            
        default:
            return false;
    }
}

// ======================================================
// Battery
// ======================================================

bool Inverter::getBatteryVoltage(BatteryValues& voltage) {
    if (!hasValidMap()) return false;
    if (_descriptor.batteryInfo.batteryCount == 0) return false;
    
    switch (_map.batteryVoltage.mode) {

        case FIELD_SIMPLE: {
            if (!_map.batteryVoltage.readable) return false;
            if (_map.batteryVoltage.length == 0 || _map.batteryVoltage.length > MAX_BATTERIES) return false;

            float v[MAX_BATTERIES];

            if (!readScaledFloat(_map.batteryVoltage, v, _map.batteryVoltage.length)) return false;

            voltage.count = _map.batteryVoltage.length;

            for(uint8_t i = 0; i < MAX_BATTERIES; i++) {
                voltage.values[i] = (i < voltage.count) ? v[i] : 0.0f;
            }

            return true;
        }

        default:
            return false;
    }
}

bool Inverter::getBatteryCurrent(BatteryValues& current) {
    if (!hasValidMap()) return false;
    if (_descriptor.batteryInfo.batteryCount == 0) return false;
    
    switch (_map.batteryCurrent.mode) {

        case FIELD_SIMPLE: {
            if (!_map.batteryCurrent.readable) return false;
            if (_map.batteryCurrent.length == 0 || _map.batteryCurrent.length > MAX_BATTERIES) return false;

            float v[MAX_BATTERIES];

            if (!readScaledFloat(_map.batteryCurrent, v, _map.batteryCurrent.length)) return false;

            current.count = _map.batteryCurrent.length;

            for(uint8_t i = 0; i < MAX_BATTERIES; i++) {
                current.values[i] = (i < current.count) ? v[i] : 0.0f;
            }

            return true;
        }

        default:
            return false;
    }
}

bool Inverter::getBatteryPower(BatteryValues& power) {
    if (!hasValidMap()) return false;
    if (_descriptor.batteryInfo.batteryCount == 0) return false;
    
    switch (_map.batteryPower.mode) {

        case FIELD_SIMPLE: {
            if (!_map.batteryPower.readable) return false;
            if (_map.batteryPower.length == 0 || _map.batteryPower.length > MAX_BATTERIES) return false;

            float v[MAX_BATTERIES];

            if (!readScaledFloat(_map.batteryPower, v, _map.batteryPower.length)) return false;

            power.count = _map.batteryPower.length;

            for(uint8_t i = 0; i < MAX_BATTERIES; i++) {
                power.values[i] = (i < power.count) ? v[i] : 0.0f;
            }

            return true;
        }
            
        default:
            return false;
    }
}

bool Inverter::getBatterySoC(BatteryValues& soc) {
    if (!hasValidMap()) return false;
    if (_descriptor.batteryInfo.batteryCount == 0) return false;
    
    switch (_map.batterySoC.mode) {

        case FIELD_SIMPLE: {
            if (!_map.batterySoC.readable) return false;
            if (_map.batterySoC.length == 0 || _map.batterySoC.length > MAX_BATTERIES) return false;

            float v[MAX_BATTERIES];

            if (!readScaledFloat(_map.batterySoC, v, _map.batterySoC.length)) return false;

            soc.count = _map.batterySoC.length;

            for(uint8_t i = 0; i < MAX_BATTERIES; i++) {
                soc.values[i] = (i < soc.count) ? v[i] : 0.0f;
            }

            return true;
        }
            
        default:
            return false;
    }
}

bool Inverter::getBatterySoH(BatteryValues& soh) {
    if (!hasValidMap()) return false;
    if (_descriptor.batteryInfo.batteryCount == 0) return false;
    
    switch (_map.batterySoH.mode) {

        case FIELD_SIMPLE: {
            if (!_map.batterySoH.readable) return false;
            if (_map.batterySoH.length == 0 || _map.batterySoH.length > MAX_BATTERIES) return false;

            float v[MAX_BATTERIES];

            if (!readScaledFloat(_map.batterySoH, v, _map.batterySoH.length)) return false;

            soh.count = _map.batterySoH.length;

            for(uint8_t i = 0; i < MAX_BATTERIES; i++) {
                soh.values[i] = (i < soh.count) ? v[i] : 0.0f;
            }

            return true;
        }
            
        default:
            return false;
    }
}

// ======================================================
// EPS
// ======================================================

bool Inverter::getEPSVoltage(PhaseData& phase) {
    if (!hasValidMap()) return false;
    if (_descriptor.epsPhaseType == NO_EPS) return false;
    
    switch (_map.epsVoltage.mode) {

        case FIELD_SIMPLE: {
            if (!_map.epsVoltage.readable) return false;
            if (_map.epsVoltage.length == 0 || _map.epsVoltage.length > 3) return false;

            float v[INV_MAX_FLOAT_VALUES];

            if (!readScaledFloat(_map.epsVoltage, v, _map.epsVoltage.length)) return false;

            phase.r = 0.0f;
            phase.s = 0.0f;
            phase.t = 0.0f;

            if (_map.epsVoltage.length >= 1) phase.r = v[0];
            if (_map.epsVoltage.length >= 2) phase.s = v[1];
            if (_map.epsVoltage.length >= 3) phase.t = v[2];

            return true;
        }
            
        default:
            return false;
    }
}

bool Inverter::getEPSCurrent(PhaseData& phase) {
    if (!hasValidMap()) return false;
    if (_descriptor.epsPhaseType == NO_EPS) return false;
    
    switch (_map.epsCurrent.mode) {

        case FIELD_SIMPLE: {
            if (!_map.epsCurrent.readable) return false;
            if (_map.epsCurrent.length == 0 || _map.epsCurrent.length > 3) return false;

            float v[INV_MAX_FLOAT_VALUES];

            if (!readScaledFloat(_map.epsCurrent, v, _map.epsCurrent.length)) return false;

            phase.r = 0.0f;
            phase.s = 0.0f;
            phase.t = 0.0f;

            if (_map.epsCurrent.length >= 1) phase.r = v[0];
            if (_map.epsCurrent.length >= 2) phase.s = v[1];
            if (_map.epsCurrent.length >= 3) phase.t = v[2];

            return true;
        }
        
        default:
            return false;
    }
}

bool Inverter::getEPSActivePower(PhaseData& phase) {
    if (!hasValidMap()) return false;
    if (_descriptor.epsPhaseType == NO_EPS) return false;
    
    switch (_map.epsActivePower.mode) {

        case FIELD_SIMPLE: {
            if (!_map.epsActivePower.readable) return false;
            if (_map.epsActivePower.length == 0 || _map.epsActivePower.length > 3) return false;

            float v[INV_MAX_FLOAT_VALUES];

            if (!readScaledFloat(_map.epsActivePower, v, _map.epsActivePower.length)) return false;

            phase.r = 0.0f;
            phase.s = 0.0f;
            phase.t = 0.0f;

            if (_map.epsActivePower.length >= 1) phase.r = v[0];
            if (_map.epsActivePower.length >= 2) phase.s = v[1];
            if (_map.epsActivePower.length >= 3) phase.t = v[2];

            return true;
        }
            
        default:
            return false;
    }
}

// ======================================================
// Diagnostics and Health
// ======================================================

bool Inverter::getTemperature(float& temperature) {
    if (!hasValidMap()) return false;
    
    
    switch (_map.temperature.mode) {

        case FIELD_SIMPLE:
            if (!_map.temperature.readable) return false;
            return readScaledFloat(_map.temperature, temperature);
            
        default:
            return false;
    }
}

bool Inverter::getInsulationResistance(float& kiloOhms) {
    if (!hasValidMap()) return false;
    
    
    switch (_map.insulationResistance.mode) {

        case FIELD_SIMPLE:
            if (!_map.insulationResistance.readable) return false;
            return readScaledFloat(_map.insulationResistance, kiloOhms);
            
        default:
            return false;
    }
}

// ======================================================
// Status and Alarms
// ======================================================

// TODO:
// Implementar tratamento padronizado de status e alarmes.
// Atualmente cada fabricante pode utilizar enums, bitfields
// ou códigos proprietários distintos.

// #############################################################################################################
bool Inverter::getInverterStatus(uint32_t status) {//InverterStatus& status) {
    if (!hasValidMap()) return false;
    
    switch (_map.inverterStatus.mode) {

        case FIELD_SIMPLE:{
            if (!_map.inverterStatus.readable) return false;

            uint32_t raw;

            if (!readField(_map.inverterStatus, &raw)) return false;

            status = raw;//(InverterStatus)raw;
            return true;
        }
            
        default:
            return false;
    }
}

bool Inverter::getAlarm(uint32_t alarm) {//Alarm& alarm) {
    if (!hasValidMap()) return false;
    
    switch (_map.alarm.mode) {

        case FIELD_SIMPLE: {
            if (!_map.alarm.readable) return false;

            uint16_t raw;
            if (!readField(_map.alarm, &raw)) return false;

            alarm = raw;//(Alarm)raw;
            return true;
        }
            
        default:
            return false;
    }
}
// #############################################################################################################