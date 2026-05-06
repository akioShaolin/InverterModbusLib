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
│   ├── isBooted()
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

bool Inverter::getSerial(String& serial) {
    if (_map.serial.address == 0xFFFF) return false;

    switch (_map.serial.mode) {

        case FIELD_SIMPLE:
            if (!_map.serial.readable) return false;
            if (_map.serial.type == ASCII) {
                char buffer[INV_MAX_STRING_CHARS + 1];
                // Supondo que o número serial tenha no máximo 32 caracteres, mais um para o terminador nulo
                if (!readField(_map.serial, buffer)) return false;
                
                serial = String(buffer);
                return true;                
            }
            
            if (_map.serial.type == U32) {
                uint32_t buffer; // Ler um valor de 32 bits
                if (!readField(_map.serial, &buffer)) return false;

                serial = String(buffer); // Transforma o número em string
                return true;
            }
            return false;

        default:
            return false;
    }
}

// ======================================================
// Limits and Control State
// ======================================================

bool Inverter::isBooted(bool& isBooted) {
    if (_map.serial.address == 0xFFFF) return false;
    if (_descriptor.bootMode == nullptr) return false;

    switch (_map.boot.mode) {

        case FIELD_SIMPLE: {
            if (!_map.boot.readable) return false;
            uint16_t v;
            if (!readField(_map.boot, &v)) return false;

            uint16_t bootValue = pgm_read_word(&_descriptor.bootMode->bootValue);
            uint16_t shutdownValue = pgm_read_word(&_descriptor.bootMode->shutdownValue);

            if (v == bootValue) {
                isBooted = true;
                return true;
            }

            if (v == shutdownValue) {
                isBooted = false;
                return true;
            }

            return false;
        }

        case FIELD_SPECIAL:
            return false; // TODO: inversor com leitura em campo diferente da escrita

        default:
            return false;
    }
}

bool Inverter::isPowerLimitEnabled(bool& enabled) {
    if (_map.serial.address == 0xFFFF) return false;
    
    switch (_map.enablePowerLimit.mode) {

        case FIELD_SIMPLE:{
            if (!_map.enablePowerLimit.readable) return false;
            uint16_t e;
            if (!readField(_map.enablePowerLimit, &e)) return false;

            enabled = (e != 0);
            return true;
        }

        default:
            return false;
    }
}

bool Inverter::getPowerLimit(float& watts) {
    if (_map.serial.address == 0xFFFF) return false;
    if (_descriptor.nominalPowerW == 0) return false;

    switch (_map.PowerLimit.mode) {

        case FIELD_SIMPLE:
            if(_map.PowerLimit.readable) {
                return readScaledFloat(_map.PowerLimit, watts);
            }

            if (_map.PowerLimitPercent.readable) {
                float percent;
                if (!readScaledFloat(_map.PowerLimitPercent, percent)) return false;

                watts = ((float)_descriptor.nominalPowerW * percent) / 100.0f;
                return true;
            }
            return false;

        default:
            return false;
    }
}

bool Inverter::getPowerLimitPercent(float& percent) {
    if (_map.serial.address == 0xFFFF) return false;
    if (_descriptor.nominalPowerW == 0) return false;

    switch (_map.PowerLimitPercent.mode) {

        case FIELD_SIMPLE:
            if (_map.PowerLimitPercent.readable) {
                return readScaledFloat(_map.PowerLimitPercent, percent);
            }

            if (_map.PowerLimit.readable) {
                float watts;
                if (!readScaledFloat(_map.PowerLimit, watts)) return false;

                percent = (watts / (float)_descriptor.nominalPowerW) * 100.0f;
                return true;
            }
            
            return false;

        default:
            return false;
    }
}

bool Inverter::isExportLimitEnabled(bool& enabled) {
    if (_map.serial.address == 0xFFFF) return false;
    if (_descriptor.exportLimitMode == nullptr) return false;

    switch (_map.enableExportLimit.mode) {

        case FIELD_SIMPLE: {
            if (!_map.enableExportLimit.readable) return false;
            uint16_t v;
            if (!readField(_map.enableExportLimit, &v)) return false;

            uint16_t enabledValue = pgm_read_word(&_descriptor.exportLimitMode->exportLimitEnable);
            uint16_t disabledValue = pgm_read_word(&_descriptor.exportLimitMode->exportLimitDisable);

            if (v == enabledValue) {
                enabled = true;
                return true;
            }

            if (v == disabledValue) {
                enabled = false;
                return true;
            }

            return false;
        }

        default:
            return false;
    }
}

bool Inverter::getExportLimit(float& watts) {
    if (_map.serial.address == 0xFFFF) return false;
    if (_descriptor.nominalPowerW == 0) return false;

    switch (_map.ExportLimit.mode) {

        case FIELD_SIMPLE:
            if(_map.ExportLimit.readable) {
                return readScaledFloat(_map.ExportLimit, watts);
            }

            if (_map.ExportLimitPercent.readable) {
                float percent;
                if (!readScaledFloat(_map.ExportLimitPercent, percent)) return false;

                watts = (_descriptor.nominalPowerW * percent) / 100.0f;
                return true;
            }
            return false;
            
        default:
            return false;
    }
}

bool Inverter::getExportLimitPercent(float& percent) {
    if (_map.serial.address == 0xFFFF) return false;
    if (_descriptor.nominalPowerW == 0) return false;

    switch (_map.ExportLimitPercent.mode) {

        case FIELD_SIMPLE:
            if (_map.ExportLimitPercent.readable) {
                return readScaledFloat(_map.ExportLimitPercent, percent);
            }

            if(_map.ExportLimit.readable) {
                float watts;
                if (!readScaledFloat(_map.ExportLimit, watts)) return false;

                percent = (watts / (float)_descriptor.nominalPowerW) * 100.0f;
                return true;
            }

            return false;
            
        default:
            return false;
    }
}

bool Inverter::isPowerFactorEnabled(bool& enabled) {
    if (_map.serial.address == 0xFFFF) return false;
    
    switch (_map.enablePowerFactor.mode) {

        case FIELD_SIMPLE: {
            if (!_map.enablePowerFactor.readable) return false;
            uint16_t e;
            if (!readField(_map.enablePowerFactor, &e)) return false;

            enabled = (e != 0);
            return true;
        }
            
        default:
            return false;
    }
}

bool Inverter::getPowerFactorSetpoint(float& pf) {
    if (_map.serial.address == 0xFFFF) return false;

    switch (_map.PowerFactorSetpoint.mode) {

        case FIELD_SIMPLE:{
            uint16_t signal = 0;
            // 0 - mantém o sinal; 1 - troca o sinal
            if (!isInvalidField(_map.powerFactorExcitationMode) && _map.powerFactorExcitationMode.readable) {
                if (!readField(_map.powerFactorExcitationMode, &signal)) return false;
            }

            if (_map.PowerFactorSetpoint.readable) {
                float scaledPf;
                if (!readScaledFloat(_map.PowerFactorSetpoint, scaledPf)) return false;

                pf = (signal != 0) ? -scaledPf : scaledPf;
                return true;
            }
            return false;
        }
            
        default:
            return false;
    }
}

// ======================================================
// AC Measurements
// ======================================================

bool Inverter::getActivePower(float& watts) {
    if (_map.serial.address == 0xFFFF) return false;
    
    switch (_map.activePower.mode) {

        case FIELD_SIMPLE:
            if (!_map.activePower.readable) return false;
            return readScaledFloat(_map.activePower, watts);
            
        default:
            return false;
    }
}
   
bool Inverter::getReactivePower(float& voltAmperReactive) {
    if (_map.serial.address == 0xFFFF) return false;
    
    switch (_map.reactivePower.mode) {

        case FIELD_SIMPLE:
            if (_map.reactivePower.readable) return false;
            return readScaledFloat(_map.reactivePower, voltAmperReactive);
            
        default:
            return false;
    }
}

bool Inverter::getApparentPower(float& voltAmper) {
    if (_map.serial.address == 0xFFFF) return false;
    
    switch (_map.apparentPower.mode) {

        case FIELD_SIMPLE:
            if (!_map.apparentPower.readable) return false;
            return readScaledFloat(_map.reactivePower, voltAmper);
            
        default:
            return false;
    }
}

bool Inverter::getPowerFactor(float &pf) {
    if (_map.serial.address == 0xFFFF) return false;
    
    switch (_map.powerFactor.mode) {

        case FIELD_SIMPLE: {
            uint16_t signal = 0;
            // 0 - mantém o sinal; 1 - troca o sinal
            if (!isInvalidField(_map.powerFactorExcitationMode) && _map.powerFactorExcitationMode.readable) {
                if (!readField(_map.powerFactorExcitationMode, &signal)) return false;
            }

            if (_map.powerFactor.readable) {
                float scaledPf;
                if (!readScaledFloat(_map.powerFactor, scaledPf)) return false;

                pf = (signal != 0) ? -scaledPf : scaledPf;
                return true;
            }
            return false;
        }

        default:
            return false;
    
    }
}

bool Inverter::getGridVoltage(PhaseData& phase) {
    if (_map.serial.address == 0xFFFF) return false;
    
    switch (_map.gridVoltage.mode) {

        case FIELD_SIMPLE: {
            if (!_map.gridVoltage.readable) return false;
            if (!_map.gridVoltage.length == 0 || _map.gridVoltage.length > 3) return false;

            float v[INV_MAX_FLOAT_VALUES];

            if (!readScaledFloat(_map.gridVoltage, v, _map.gridVoltage.length)) return false;

            phase.r = 0.0f;
            phase.s = 0.0f;
            phase.t = 0.0f;

            if (_map.gridVoltage.length >= 1) phase.r = v[0];
            if (_map.gridVoltage.length >= 2) phase.s = v[1];
            if (_map.gridVoltage.length >= 3) phase.t = v[2];

            return true;
        }

        default:
            return false;
    }
}

bool Inverter::getGridCurrent(PhaseData& phase) {
    if (_map.serial.address == 0xFFFF) return false;
    
    switch (_map.gridCurrent.mode) {

        case FIELD_SIMPLE: {
            if (!_map.gridCurrent.readable) return false;
            if (_map.gridCurrent.length || _map.gridCurrent.length > 3) return false;

            float v[INV_MAX_FLOAT_VALUES];

            if (!readScaledFloat(_map.gridCurrent, v, _map.gridCurrent.length)) return false;

            phase.r = 0.0f;
            phase.s = 0.0f;
            phase.t = 0.0f;

            if (_map.gridCurrent.length >= 1) phase.r = v[0];
            if (_map.gridCurrent.length >= 2) phase.s = v[1];
            if (_map.gridCurrent.length >= 3) phase.t = v[2];

            return true;
        }

        default:
            return false;
    }
}

bool Inverter::getGridFrequency(PhaseData& phase) {
    if (_map.serial.address == 0xFFFF) return false;
    
    switch (_map.gridFrequency.mode) {

        case FIELD_SIMPLE: {
            if (!_map.gridFrequency.readable) return false;
            if (_map.gridFrequency.length == 0 || _map.gridFrequency.length > 3) return false;

            float v[INV_MAX_FLOAT_VALUES];

            if (!readScaledFloat(_map.gridFrequency, v, _map.gridFrequency.length)) return false;

            phase.r = 0.0f;
            phase.s = 0.0f;
            phase.t = 0.0f;

            if (_map.gridFrequency.length >= 1) phase.r = v[0];
            if (_map.gridFrequency.length >= 2) phase.s = v[1];
            if (_map.gridFrequency.length >= 3) phase.t = v[2];

            return true;
        }

        default:
            return false;
    }
}

// ======================================================
// Energy
// ======================================================

bool Inverter::getTotalEnergy(float& kWh) {
    if (_map.serial.address == 0xFFFF) return false;
    
    switch (_map.totalEnergy.mode) {

        case FIELD_SIMPLE:
            if (!_map.totalEnergy.readable) return false;
            return readScaledFloat(_map.totalEnergy, kWh);
            
        default:
            return false;
    }
}

bool Inverter::getDailyEnergy(float& kWh) {
    if (_map.serial.address == 0xFFFF) return false;
    
    switch (_map.dailyEnergy.mode) {

        case FIELD_SIMPLE:
            if (!_map.dailyEnergy.readable) return false;
            return readScaledFloat(_map.dailyEnergy, kWh);
            
        default:
            return false;
    }
}

// ======================================================
// PV Strings
// ======================================================

bool Inverter::getStringVoltage(StringValues& voltage) {
    if (_map.serial.address == 0xFFFF) return false;
    
    switch (_map.stringVoltage.mode) {

        case FIELD_SIMPLE: {
            if (!_map.stringVoltage.readable) return false;
            if (_map.stringVoltage.length == 0 || _map.stringVoltage.length > MAX_STRINGS) return false;

            float v[MAX_STRINGS];

            if (!readScaledFloat(_map.stringVoltage, v, _map.stringVoltage.length)) return false;

            voltage.count = _map.stringVoltage.length;

            for(uint8_t i = 0; i < MAX_STRINGS; i++) {
                voltage.values[i] = (i < voltage.count) ? v[i] : 0.0f;
            }

            return true;
        }
            
        default:
            return false;
    }
}

bool Inverter::getStringCurrent(StringValues& current) {
    if (_map.serial.address == 0xFFFF) return false;
    
    switch (_map.stringCurrent.mode) {

        case FIELD_SIMPLE: {
            if (!_map.stringCurrent.readable) return false;
            if (_map.stringCurrent.length == 0 || _map.stringCurrent.length > MAX_STRINGS) return false;

            float v[MAX_STRINGS];

            if (!readScaledFloat(_map.stringCurrent, v, _map.stringCurrent.length)) return false;

            current.count = _map.stringCurrent.length;

            for(uint8_t i = 0; i < MAX_STRINGS; i++) {
                current.values[i] = (i < current.count) ? v[i] : 0.0f;
            }

            return true;
        }

        default:
            return false;
    }
}

bool Inverter::getStringPower(StringValues& power) {
    if (_map.serial.address == 0xFFFF) return false;
    
    switch (_map.stringPower.mode) {

        case FIELD_SIMPLE: {
            if (!_map.stringPower.readable) return false;
            if (_map.stringPower.length == 0 || _map.stringPower.length > MAX_STRINGS) return false;

            float v[MAX_STRINGS];

            if (!readScaledFloat(_map.stringPower, v, _map.stringPower.length)) return false;

            power.count = _map.stringPower.length;

            for(uint8_t i = 0; i < MAX_STRINGS; i++) {
                power.values[i] = (i < power.count) ? v[i] : 0.0f;
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
    if (_map.serial.address == 0xFFFF) return false;
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
    if (_map.serial.address == 0xFFFF) return false;
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
    if (_map.serial.address == 0xFFFF) return false;
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
    if (_map.serial.address == 0xFFFF) return false;
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
    if (_map.serial.address == 0xFFFF) return false;
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
    if (_map.serial.address == 0xFFFF) return false;
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
    if (_map.serial.address == 0xFFFF) return false;
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
    if (_map.serial.address == 0xFFFF) return false;
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
    if (_map.serial.address == 0xFFFF) return false;
    
    
    switch (_map.temperature.mode) {

        case FIELD_SIMPLE:
            if (!_map.temperature.readable) return false;
            return readScaledFloat(_map.temperature, temperature);
            
        default:
            return false;
    }
}

bool Inverter::getInsulationResistance(float& kiloOhms) {
    if (_map.serial.address == 0xFFFF) return false;
    
    
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
bool Inverter::getInverterStatus(InverterStatus& status) {
    if (_map.serial.address == 0xFFFF) return false;
    
    switch (_map.inverterStatus.mode) {

        case FIELD_SIMPLE:{
            if (!_map.inverterStatus.readable) return false;

            uint32_t raw;

            if (!readField(_map.inverterStatus, &raw)) return false;

            status = (InverterStatus)raw;
            return true;
        }
            
        default:
            return false;
    }
}

bool Inverter::getAlarm(Alarm& alarm) {
    if (_map.serial.address == 0xFFFF) return false;
    
    switch (_map.alarm.mode) {

        case FIELD_SIMPLE: {
            if (!_map.alarm.readable) return false;

            uint16_t raw;
            if (!readField(_map.alarm, &raw)) return false;

            alarm = (Alarm)raw;
            return true;
        }
            
        default:
            return false;
    }
}
// #############################################################################################################