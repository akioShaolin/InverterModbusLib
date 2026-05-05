/*
 * InverterModbusLib - Solar Inverter Library for Arduino
 * ------------------------------------------------
 * Modbus RTU (RS485)communication layer for inverter integration
 *
 * Copyright (c) 2026, Pedro Akio Sakuma
 * Licensed under BSD 3-Clause License
 */

 #include "Inverter.h"

 bool Inverter::getSerial(String& serial) {
    if (_map.serial.address == 0xFFFF) return false;
    if (!_map.serial.readable) return false;

    switch (_map.serial.mode) {

        case FIELD_SIMPLE:
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

bool Inverter::isBooted(bool& isBooted) {
    if (_map.serial.address == 0xFFFF) return false;
    if (!_map.boot.readable) return false;
    if (_descriptor.bootMode == nullptr) return false;

    switch (_map.boot.mode) {

        case FIELD_SIMPLE: {
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
    if (!_map.enablePowerLimit.readable) return false;
    
    switch (_map.enablePowerLimit.mode) {

        case FIELD_SIMPLE:{
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
    if (!_map.enableExportLimit.readable) return false;
    if (_descriptor.exportLimitMode == nullptr) return false;

    switch (_map.enableExportLimit.mode) {

        case FIELD_SIMPLE: {
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

// #############################################################################################################

bool Inverter::getExportLimit(float& watts) {
    if (_map.serial.address == 0xFFFF) return false;
    if (!_map.ExportLimit.readable) return false;

    switch (_map.ExportLimit.mode) {

        case FIELD_SIMPLE:
            if(_map.ExportLimit.readable) {
                return readScaledFloat(_map.ExportLimit, watts);
            }

            if (_map.ExportLimitPercent.readable) {
                float percent;
                if (!getExportLimitPercent(percent)) return false;
                watts = (_descriptor.nominalPowerW * percent) / 100.0f;
                return true;
            }
            
        default:
            return false;
    }
}

bool Inverter::getExportLimitPercent(float& percent) {
    if (_map.serial.address == 0xFFFF) return false;
    if (!_map.ExportLimitPercent.readable) return false;

    switch (_map.ExportLimitPercent.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}

bool Inverter::isPowerFactorEnabled(bool& enabled) {
    if (_map.serial.address == 0xFFFF) return false;
    if (!_map.powerFactor.readable) return false;
    
    switch (_map.enablePowerFactor.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}

bool Inverter::getPowerFactorSetpoint(float& pf) {
    if (_map.serial.address == 0xFFFF) return false;
    if (!_map.PowerFactorSetpoint.readable) return false;

    switch (_map.PowerFactorSetpoint.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}
  
bool Inverter::getTotalEnergy(float& kWh) {
    if (_map.serial.address == 0xFFFF) return false;
    if (!_map.totalEnergy.readable) return false;
    
    switch (_map.totalEnergy.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}

bool Inverter::getDailyEnergy(float& kWh) {
    if (_map.serial.address == 0xFFFF) return false;
    if (!_map.dailyEnergy.readable) return false;
    
    switch (_map.dailyEnergy.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}


bool Inverter::getActivePower(float& watts) {
    if (_map.serial.address == 0xFFFF) return false;
    if (!_map.activePower.readable) return false;
    
    switch (_map.activePower.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}
   
bool Inverter::getReactivePower(float& voltAmperReactive) {
    if (_map.serial.address == 0xFFFF) return false;
    if (_map.reactivePower.readable) return false;
    
    switch (_map.reactivePower.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}

bool Inverter::getApparentPower(float& voltAmper) {
    if (_map.serial.address == 0xFFFF) return false;
    if (!_map.apparentPower.readable) return false;
    
    switch (_map.apparentPower.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}

bool Inverter::getPowerFactor(float &pf) {
    if (_map.serial.address == 0xFFFF) return false;
    if (!_map.powerFactor.readable) return false;
    
    switch (_map.powerFactor.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}


bool Inverter::getGridVoltage(PhaseData& phase) {
    if (_map.serial.address == 0xFFFF) return false;
    
    switch (_map.gridVoltage.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}

bool Inverter::getGridCurrent(PhaseData& phase) {
    if (_map.serial.address == 0xFFFF) return false;
    
    switch (_map.gridCurrent.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}

bool Inverter::getGridFrequency(PhaseData& phase) {
    if (_map.serial.address == 0xFFFF) return false;
    
    switch (_map.gridFrequency.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}

bool Inverter::getTemperature(float& temperature) {
    if (_map.serial.address == 0xFFFF) return false;
    if (!_map.temperature.readable) return false;
    
    switch (_map.temperature.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}

bool Inverter::getInsulationResistance(float& kiloOhms) {
    if (_map.serial.address == 0xFFFF) return false;
    if (!_map.insulationResistance.readable) return false;
    
    switch (_map.insulationResistance.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}

bool Inverter::getInverterStatus(InverterStatus& status) {
    if (_map.serial.address == 0xFFFF) return false;
    if (!_map.inverterStatus.readable) return false;
    
    switch (_map.inverterStatus.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}

bool Inverter::getAlarm(Alarm& alarm) {
    if (_map.serial.address == 0xFFFF) return false;
    if (!_map.alarm.readable) return false;
    
    switch (_map.alarm.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}

bool Inverter::getStringVoltage(StringValues& voltage) {
    if (_map.serial.address == 0xFFFF) return false;
    if (!_map.stringVoltage.readable) return false;
    
    switch (_map.stringVoltage.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}

bool Inverter::getStringCurrent(StringValues& current) {
    if (_map.serial.address == 0xFFFF) return false;
    if (!_map.stringCurrent.readable) return false;
    
    switch (_map.stringCurrent.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}

bool Inverter::getStringPower(StringValues& power) {
    if (_map.serial.address == 0xFFFF) return false;
    if (!_map.stringPower.readable) return false;
    
    switch (_map.stringPower.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}

bool Inverter::getBatteryVoltage(BatteryValues& voltage) {
    if (_map.serial.address == 0xFFFF) return false;
    if (_descriptor.batteryInfo.batteryCount == 0) return false;
    if (!_map.batteryVoltage.readable) return false;
    
    switch (_map.batteryVoltage.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}

bool Inverter::getBatteryCurrent(BatteryValues& current) {
    if (_map.serial.address == 0xFFFF) return false;
    if (_descriptor.batteryInfo.batteryCount == 0) return false;
    if (!_map.batteryCurrent.readable) return false;
    
    switch (_map.batteryCurrent.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar

        default:
            return false;
    }
}

bool Inverter::getBatteryPower(BatteryValues& power) {
    if (_map.serial.address == 0xFFFF) return false;
    if (_descriptor.batteryInfo.batteryCount == 0) return false;
    if (!_map.batteryPower.readable) return false;
    
    switch (_map.batteryPower.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}

bool Inverter::getBatterySoC(BatteryValues& soc) {
    if (_map.serial.address == 0xFFFF) return false;
    if (_descriptor.batteryInfo.batteryCount == 0) return false;
    if (!_map.batterySoC.readable) return false;
    
    switch (_map.batterySoC.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}

bool Inverter::getBatterySoH(BatteryValues& soh) {
    if (_map.serial.address == 0xFFFF) return false;
    if (_descriptor.batteryInfo.batteryCount == 0) return false;
    if (!_map.batterySoH.readable) return false;
    
    switch (_map.batterySoH.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}

bool Inverter::getEPSVoltage(PhaseData& phase) {
    if (_map.serial.address == 0xFFFF) return false;
    if (_descriptor.epsPhaseType == NO_EPS) return false;
    if (!_map.epsVoltage.readable) return false;
    
    switch (_map.epsVoltage.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}

bool Inverter::getEPSCurrent(PhaseData& phase) {
    if (_map.serial.address == 0xFFFF) return false;
    if (_descriptor.epsPhaseType == NO_EPS) return false;
    if (!_map.epsCurrent.readable) return false;
    
    switch (_map.epsCurrent.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
}

bool Inverter::getEPSActivePower(PhaseData& phase) {
    if (_map.serial.address == 0xFFFF) return false;
    if (_descriptor.epsPhaseType == NO_EPS) return false;
    if (!_map.epsActivePower.readable) return false;
    
    
    switch (_map.epsActivePower.mode) {

        case FIELD_SIMPLE:
            return false; // TODO: implementar
            
        default:
            return false;
    }
} 