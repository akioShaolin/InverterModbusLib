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
│   └── getSerialNumber()
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
│   ├── getGridPhaseVoltage()
│   ├── getGridLineVoltage()
│   ├── getGridCurrent()
│   └── getGridFrequency()
│
├── Energia
│   ├── getTotalEnergy()
│   └── getDailyEnergy()
│
├── Strings FV
│   ├── getStringCount()
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

InverterRequestStatus Inverter::getSerialNumber(char* output, size_t outputSize) {
    if (output == nullptr || outputSize == 0) return INV_ERROR;
    output[0] = '\0';
    if (_bus == nullptr || !hasValidMap()) return INV_ERROR;

    const ModbusField& field = _map.identification.serialNumber;
    if (isInvalidField(field) || !field.readable || field.mode != FIELD_SIMPLE || field.length == 0) {
        return INV_ERROR;
    }
    uint16_t registerCount = field.length;
    if (field.type == U32 || field.type == I32) registerCount *= 2;
    else if (field.type == U64 || field.type == I64) registerCount *= 4;
    else if (field.type != ASCII && field.type != U16 && field.type != I16) return INV_ERROR;
    const size_t requiredSize = field.type == ASCII ? (size_t)registerCount * 2U + 1U : 24U;
    if (outputSize < requiredSize || registerCount > INV_ASYNC_BUFFER_REGS) return INV_ERROR;

    if (_bus->isCompletedFor(this, REQ_SERIAL_NUMBER)) {
        const uint16_t* regs = _bus->buffer();
        size_t used = 0;
        if (field.type == ASCII) {
            for (uint16_t i = 0; i < registerCount; ++i) {
                const char high = (char)(regs[i] >> 8);
                const char low = (char)(regs[i] & 0xFF);
                if (high >= 32 && high <= 126) output[used++] = high;
                if (low >= 32 && low <= 126) output[used++] = low;
            }
            while (used > 0 && output[used - 1] == ' ') --used;
            output[used] = '\0';
        } else {
            uint64_t raw = 0;
            for (uint16_t i = 0; i < registerCount; ++i) raw = (raw << 16) | regs[i];
            if (field.type == I16) snprintf(output, outputSize, "%d", (int16_t)raw);
            else if (field.type == I32) snprintf(output, outputSize, "%ld", (long)(int32_t)raw);
            else if (field.type == I64) snprintf(output, outputSize, "%lld", (long long)(int64_t)raw);
            else snprintf(output, outputSize, "%llu", (unsigned long long)raw);
            used = strlen(output);
        }
        _lastModbusStatus = _bus->transactionStatus();
        _bus->release();
        if (used == 0) {
            _lastModbusStatus = INV_MB_UNEXPECTED_RESPONSE;
            return INV_ERROR;
        }
        return INV_DONE;
    }
    if (_bus->isFailedFor(this, REQ_SERIAL_NUMBER)) {
        _lastModbusStatus = _bus->transactionStatus();
        _bus->release();
        return INV_ERROR;
    }
    if (_bus->belongsTo(this, REQ_SERIAL_NUMBER)) return INV_BUSY;
    if (_bus->isBusy()) return INV_REJECTED;
    if (!_bus->startRead(this, REQ_SERIAL_NUMBER, _cfg.id, field.address, registerCount)) {
        if (_bus->belongsTo(this, REQ_SERIAL_NUMBER)) {
            _lastModbusStatus = _bus->transactionStatus();
            _bus->release();
        }
        return INV_ERROR;
    }
    return INV_BUSY;
}

bool Inverter::getModelId(uint16_t& modelId) {
    if (!hasValidMap()) return false;

    const ModbusField& field = _map.identification.modelId;

    if (isInvalidField(field)) return false;
    
    switch (field.mode) {

        case FIELD_SIMPLE: 
            if (!field.readable) return false;
            
            return readField(field, &modelId);

        default:
            return false;
    }

    return false;
}

bool Inverter::getModelName(String& modelName) {
    if (!hasValidMap()) return false;    
    const ModbusField& field = _map.identification.modelName;

    switch (field.mode) {

        case FIELD_SIMPLE:
            if (!field.readable) return false;
            if (field.type == ASCII) {
                char buffer[INV_MAX_STRING_CHARS + 1];
                // Buffer limitado pelo tamanho máximo definido pela lib, mais um para o terminador nulo
                if (!readField(field, buffer)) return false;
                
                modelName = String(buffer);
                return true;                
            }
            
            return false;

        default:
            return false;
    }
}

bool Inverter::getFirmwareVersion(String& firmwareVersion) {
    if (!hasValidMap()) return false;    
    const ModbusField& field = _map.identification.firmwareVersion;

    switch (field.mode) {

        case FIELD_SIMPLE:
            if (!field.readable) return false;
            if (field.type == ASCII) {
                char buffer[INV_MAX_STRING_CHARS + 1];
                // Buffer limitado pelo tamanho máximo definido pela lib, mais um para o terminador nulo
                if (!readField(field, buffer)) return false;
                
                firmwareVersion = String(buffer);
                return true;                
            }
            
            return false;

        default:
            return false;
    }
}

bool Inverter::getRatedPower(uint32_t& power) {
    if (!hasValidMap()) return false;

    power = 0;

    const ModbusField& field = _map.identification.ratedPower;
    
    if(!isInvalidField(field) && field.readable) {
        switch (field.mode) {
            case FIELD_SIMPLE: {
                if (field.scale == 0.0f) return false;

                uint32_t raw = 0;

                if (readField(field, &raw)) {
                    power = (uint32_t)((float)raw * field.scale);
                    return true;
                }

                break;
            }

        default:
            break;
        }
    }

    // Fallback: descriptor
    if (_descriptor.ratedPowerW != 0) {
        power = _descriptor.ratedPowerW;
        return true;
    } 
    
    return false;
}

bool Inverter::getRatedPowerSpec(float& ratedPower) const {
    if (_descriptor.ratedPowerW == 0) return false;
    ratedPower = (float)_descriptor.ratedPowerW;
    return true;
}

bool Inverter::wasLastRatedPowerFallback() const {
    return _ratedPowerFromSpec;
}

InverterRequestStatus Inverter::getRatedPower(float& power) {
    if (!hasValidMap()) return INV_ERROR;
    const ModbusField& field = _map.identification.ratedPower;

    if (isInvalidField(field) || !field.readable || field.mode != FIELD_SIMPLE) {
        if (!getRatedPowerSpec(power)) return INV_ERROR;
        _ratedPowerCache = power;
        _hasRatedPowerCache = true;
        _ratedPowerFromSpec = true;
        return INV_DONE;
    }
    if (_bus == nullptr || field.length != 1 || field.scale == 0.0f) return INV_ERROR;

    if (_bus->isCompletedFor(this, REQ_RATED_POWER)) {
        const uint16_t* regs = _bus->buffer();
        const uint32_t raw32 = ((uint32_t)regs[0] << 16) | regs[1];
        switch (field.type) {
            case U16: power = (float)regs[0] * field.scale; break;
            case I16: power = (float)(int16_t)regs[0] * field.scale; break;
            case U32: power = (float)raw32 * field.scale; break;
            case I32: power = (float)(int32_t)raw32 * field.scale; break;
            case FLOAT32: memcpy(&power, &raw32, sizeof(power)); power *= field.scale; break;
            default:
                _lastModbusStatus = INV_MB_GENERAL_FAILURE;
                _bus->release();
                return INV_ERROR;
        }
        _lastModbusStatus = _bus->transactionStatus();
        _bus->release();
        _ratedPowerCache = power;
        _hasRatedPowerCache = true;
        _ratedPowerFromSpec = false;
        return INV_DONE;
    }
    if (_bus->isFailedFor(this, REQ_RATED_POWER)) {
        _lastModbusStatus = _bus->transactionStatus();
        _bus->release();
        return INV_ERROR;
    }
    if (_bus->belongsTo(this, REQ_RATED_POWER)) return INV_BUSY;
    if (_bus->isBusy()) return INV_REJECTED;

    uint16_t count = 0;
    if (field.type == U16 || field.type == I16) count = 1;
    else if (field.type == U32 || field.type == I32 || field.type == FLOAT32) count = 2;
    else return INV_ERROR;

    if (!_bus->startRead(this, REQ_RATED_POWER, _cfg.id, field.address, count)) {
        if (_bus->belongsTo(this, REQ_RATED_POWER)) {
            _lastModbusStatus = _bus->transactionStatus();
            _bus->release();
        }
        return INV_ERROR;
    }
    return INV_BUSY;
}

bool Inverter::getPVStringCount(uint16_t& count) {
    if (!hasValidMap()) return false;

    const IdentificationFeature& feature = _map.identification;
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

bool Inverter::getMpptCount(uint16_t& count) {
    if (!hasValidMap()) return false;

    const IdentificationFeature& feature = _map.identification;
    const ModbusField& field = feature.MpptCount;

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
    if (_descriptor.pvInfo.mpptCount > 0) {
        count = _descriptor.pvInfo.mpptCount;

        return true;
    }

    return false;
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
    
    // 1) Tenta ler diretamente em watts
    if (feature.supportsWatts) {
        const ModbusField& field = feature.watts;

        if (!isInvalidField(field) && field.readable) {
            switch (field.mode) {
                case FIELD_SIMPLE: {
                    if (field.scale == 0.0f) break;

                    if(readScaledFloat(field, watts)) return true;
                    break;
                }

                default:
                    break;
            }
        }
    }

    // 2) Tenta ler por porcentagem
    if (feature.supportsPercent) {
        const ModbusField& field = feature.percent;

        if (isInvalidField(field) || !field.readable) return false;
        if (field.scale == 0.0f) return false;

        switch (field.mode) {
            case FIELD_SIMPLE: {
              
                uint32_t power = 0;
                if (!getRatedPower(power)) return false;
                if (power == 0) return false;

                float percent = 0.0f;
                
                if (!readScaledFloat(field, percent)) return false;

                watts = ((float)power * percent) / 100.0f;
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
                if (_descriptor.ratedPowerW == 0) return false;
                // Trocar por getratedPower. Ler registrador de potencia nominal e adicionar fallback para o descriptor

                float watts;
                if (!readScaledFloat(field, watts)) return false;

                percent = (watts / (float)_descriptor.ratedPowerW) * 100.0f;
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
                if (_descriptor.ratedPowerW == 0) return false;
                // Trocar por getratedPower. Ler registrador de potencia nominal e adicionar fallback para o descriptor

                float percent;
                if (!readScaledFloat(field, percent)) return false;

                watts = ((float)_descriptor.ratedPowerW * percent) / 100.0f;
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
                if (_descriptor.ratedPowerW == 0) return false;
                // Trocar por getratedPower. Ler registrador de potencia nominal e adicionar fallback para o descriptor

                float watts;
                if (!readScaledFloat(field, watts)) return false;

                percent = (watts / (float)_descriptor.ratedPowerW) * 100.0f;
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

    if (feature.supportsEnablePf) {
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

bool Inverter::isFixedReactiveEnabled(bool& enabled) {
    if (!hasValidMap()) return false;

    const ReactivePowerFeature& feature = _map.reactivePowerControl;

    if (feature.supportsEnableFixedReactive) {
        const ModbusField& field = feature.enableFixedReactive;

        switch (field.mode) {

            case FIELD_SIMPLE: {
                if (!field.readable) return false;

                uint16_t e;
                if (!readField(field, &e)) return false;

                // Em alguns inversores, o registrador pode ser compartilhado
                // entre diferentes modos. Portanto, somente o valor expecífico
                // de export limit deve retornar true
                enabled = (e == feature.enableFixedReactiveValue);
                return true;
            }

            default:
                return false;
        }
    }

    if (feature.supportsControlModeFixedReactive) {
        const ModbusField& field = feature.controlMode;

        switch (field.mode) {
            case FIELD_SIMPLE: {
                if (!field.readable) return false;

                uint16_t mode;
                if (!readField(field, &mode)) return false;

                enabled = (mode == feature.enableFixedReactiveValue);
                return true;
            }

            default:
                return false;
        }
    }

    if (feature.implicitFixedReactiveSp) {
        enabled = true;
        return true;
    }

    return false;
}

bool Inverter::getFixedReactiveSetpoint(float& var) {
    if (!hasValidMap()) return false;

    const ReactivePowerFeature feature = _map.reactivePowerControl;

    if (!feature.supportsFixedReactiveSp) return false;

    const ModbusField& field = feature.fixedReactiveSp;

    switch (field.mode) {

        case FIELD_SIMPLE:
            if (!field.readable) return false;
            return readScaledFloat(field, var);

        default:
            return false;
    }

    return false;
}

// ======================================================
// AC Measurements
// ======================================================

InverterRequestStatus Inverter::getActivePower(float& watts) {
    if (_bus == nullptr || !hasValidMap()) return INV_ERROR;

    const ModbusField& field = _map.power.activePower;

    if (isInvalidField(field) || !field.readable || field.length == 0 || field.mode != FIELD_SIMPLE) {
        return INV_ERROR;
    }

    if (_bus->isCompletedFor(this, REQ_ACTIVE_POWER)) {
        const uint16_t* buffer = _bus->buffer();
        uint32_t raw = 0;

        switch (field.type) {
            case U16:
                watts = (float)buffer[0] * field.scale;
                break;

            case I16:
                watts = (float)(int16_t)buffer[0] * field.scale;
                break;

            case U32:
                raw = ((uint32_t)buffer[0] << 16) | buffer[1];
                watts = (float)raw * field.scale;
                break;

            case I32:
                raw = ((uint32_t)buffer[0] << 16) | buffer[1];
                watts = (float)(int32_t)raw * field.scale;
                break;

            case FLOAT32:
                raw = ((uint32_t)buffer[0] << 16) | buffer[1];
                memcpy(&watts, &raw, sizeof(watts));
                watts *= field.scale;
                break;

            default:
                _lastModbusStatus = INV_MB_GENERAL_FAILURE;
                _bus->release();
                return INV_ERROR;
        }

        _lastModbusStatus = _bus->transactionStatus();
        _bus->release();
        return INV_DONE;
    }

    if (_bus->isFailedFor(this, REQ_ACTIVE_POWER)) {
        _lastModbusStatus = _bus->transactionStatus();
        _bus->release();
        return INV_ERROR;
    }

    if (_bus->belongsTo(this, REQ_ACTIVE_POWER)) return INV_BUSY;
    if (_bus->isBusy()) return INV_REJECTED;

    uint16_t registerCount = field.length;
    switch (field.type) {
        case U16:
        case I16:
            break;

        case U32:
        case I32:
        case FLOAT32:
            registerCount *= 2;
            break;

        default:
            return INV_ERROR;
    }

    if (!_bus->startRead(this, REQ_ACTIVE_POWER, _cfg.id,
                         field.address, registerCount)) {
        if (_bus->belongsTo(this, REQ_ACTIVE_POWER)) {
            _lastModbusStatus = _bus->transactionStatus();
            _bus->release();
        }
        return INV_ERROR;
    }

    return INV_BUSY;
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

            phase.rs = v[0];
            phase.st = v[1];
            phase.tr = v[2];

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

InverterRequestStatus Inverter::getGridFrequency(float& freq) {
    if (_bus == nullptr || !hasValidMap()) return INV_ERROR;

    const ModbusField& field = _map.grid.frequency;

    if (isInvalidField(field) || !field.readable || field.length != 1 || field.mode != FIELD_SIMPLE) {
        return INV_ERROR;
    }

    if (_bus->isCompletedFor(this, REQ_GRID_FREQUENCY)) {
        const uint16_t* buffer = _bus->buffer();
        uint32_t raw = 0;

        switch (field.type) {
            case U16:
                freq = (float)buffer[0] * field.scale;
                break;

            case I16:
                freq = (float)(int16_t)buffer[0] * field.scale;
                break;

            case U32:
                raw = ((uint32_t)buffer[0] << 16) | buffer[1];
                freq = (float)raw * field.scale;
                break;

            case I32:
                raw = ((uint32_t)buffer[0] << 16) | buffer[1];
                freq = (float)(int32_t)raw * field.scale;
                break;

            case FLOAT32:
                raw = ((uint32_t)buffer[0] << 16) | buffer[1];
                memcpy(&freq, &raw, sizeof(freq));
                freq *= field.scale;
                break;

            default:
                _lastModbusStatus = INV_MB_GENERAL_FAILURE;
                _bus->release();
                return INV_ERROR;
        }

        _lastModbusStatus = _bus->transactionStatus();
        _bus->release();
        return INV_DONE;
    }

    if (_bus->isFailedFor(this, REQ_GRID_FREQUENCY)) {
        _lastModbusStatus = _bus->transactionStatus();
        _bus->release();
        return INV_ERROR;
    }

    if (_bus->belongsTo(this, REQ_GRID_FREQUENCY)) return INV_BUSY;
    if (_bus->isBusy()) return INV_REJECTED;

    uint16_t registerCount = field.length;
    if (field.type == U32 || field.type == I32 || field.type == FLOAT32) {
        registerCount *= 2;
    }

    if (!_bus->startRead(this, REQ_GRID_FREQUENCY, _cfg.id,
                         field.address, registerCount)) {
        if (_bus->belongsTo(this, REQ_GRID_FREQUENCY)) {
            _lastModbusStatus = _bus->transactionStatus();
            _bus->release();
        }
        return INV_ERROR;
    }

    return INV_BUSY;
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

bool Inverter::getStringVoltage(StringValues& voltage) {
    if (!hasValidMap()) return false;

    const PvStringFeature& feature = _map.pvString;
    const ModbusField& originalField = feature.voltage;

    if (!originalField.readable) return false;
    if (originalField.length == 0) return false;

    uint16_t count = 0;

    if (!getPVStringCount(count)) return false;

    if (count == 0) return false;

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

            // Zera o array antes de atribuir os valores
            for (uint8_t i = 0; i < MAX_STRINGS; i++) {
                voltage.values[i] = 0.0f;
            }

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

    const PvStringFeature& feature = _map.pvString;
    const ModbusField& originalField = feature.power;

    if (!originalField.readable) return false;
    if (originalField.length == 0) return false;

    uint16_t count = 0;

    if (!getPVStringCount(count)) return false;

    if (count == 0) return false;

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

            power.count = field.length;

            // Zera o array antes de atribuir os valores
            for (uint8_t i = 0; i < MAX_STRINGS; i++) {
                power.values[i] = 0.0f;
            }

            for(uint8_t i = 0; i < power.count; i++) {
                power.values[i] = values[i];
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

    const BatteryFeature& feature = _map.battery;
    const ModbusField& originalField = feature.voltage;

    if (!originalField.readable) return false;
    if (originalField.length == 0) return false;

    uint16_t count = feature.batteryCount;

    if (count == 0) return false;

    uint16_t effectiveLength = originalField.length;

    if (count < effectiveLength) {
        effectiveLength = count;
    }

    if (effectiveLength == 0) return false;
    if (effectiveLength > MAX_BATTERIES) effectiveLength = MAX_BATTERIES;

    ModbusField field = originalField;
    field.length = effectiveLength;

    float values[MAX_BATTERIES];

    switch (field.mode) {

        case FIELD_SIMPLE: {
            if (!readScaledFloat(field, values, field.length)) return false;

            voltage.count = field.length;

            // Zera o array antes de atribuir os valores
            for (uint8_t i = 0; i < MAX_BATTERIES; i++) {
                voltage.values[i] = 0.0f;
            }

            for(uint8_t i = 0; i < voltage.count; i++) {
                voltage.values[i] = values[i];
            }

            return true;
        }
            
        default:
            return false;
    }
}

bool Inverter::getBatteryCurrent(BatteryValues& current) {
    if (!hasValidMap()) return false;

    const BatteryFeature& feature = _map.battery;
    const ModbusField& originalField = feature.current;

    if (!originalField.readable) return false;
    if (originalField.length == 0) return false;

    uint16_t count = feature.batteryCount;

    if (count == 0) return false;

    uint16_t effectiveLength = originalField.length;

    if (count < effectiveLength) {
        effectiveLength = count;
    }

    if (effectiveLength == 0) return false;
    if (effectiveLength > MAX_BATTERIES) effectiveLength = MAX_BATTERIES;

    ModbusField field = originalField;
    field.length = effectiveLength;

    float values[MAX_BATTERIES];

    switch (field.mode) {

        case FIELD_SIMPLE: {
            if (!readScaledFloat(field, values, field.length)) return false;

            current.count = field.length;

            // Zera o array antes de atribuir os valores
            for (uint8_t i = 0; i < MAX_BATTERIES; i++) {
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

bool Inverter::getBatteryPower(BatteryValues& power) {
    if (!hasValidMap()) return false;

    const BatteryFeature& feature = _map.battery;
    const ModbusField& originalField = feature.power;

    if (!originalField.readable) return false;
    if (originalField.length == 0) return false;

    uint16_t count = feature.batteryCount;

    if (count == 0) return false;

    uint16_t effectiveLength = originalField.length;

    if (count < effectiveLength) {
        effectiveLength = count;
    }

    if (effectiveLength == 0) return false;
    if (effectiveLength > MAX_BATTERIES) effectiveLength = MAX_BATTERIES;

    ModbusField field = originalField;
    field.length = effectiveLength;

    float values[MAX_BATTERIES];

    switch (field.mode) {

        case FIELD_SIMPLE: {
            if (!readScaledFloat(field, values, field.length)) return false;

            power.count = field.length;

            // Zera o array antes de atribuir os valores
            for (uint8_t i = 0; i < MAX_BATTERIES; i++) {
                power.values[i] = 0.0f;
            }

            for(uint8_t i = 0; i < power.count; i++) {
                power.values[i] = values[i];
            }

            return true;
        }
            
        default:
            return false;
    }
}

bool Inverter::getBatterySoC(BatteryValues& soc) {
    if (!hasValidMap()) return false;

    const BatteryFeature& feature = _map.battery;
    const ModbusField& originalField = feature.soc;

    if (!originalField.readable) return false;
    if (originalField.length == 0) return false;

    uint16_t count = feature.batteryCount;

    if (count == 0) return false;

    uint16_t effectiveLength = originalField.length;

    if (count < effectiveLength) {
        effectiveLength = count;
    }

    if (effectiveLength == 0) return false;
    if (effectiveLength > MAX_BATTERIES) effectiveLength = MAX_BATTERIES;

    ModbusField field = originalField;
    field.length = effectiveLength;

    float values[MAX_BATTERIES];

    switch (field.mode) {

        case FIELD_SIMPLE: {
            if (!readScaledFloat(field, values, field.length)) return false;

            soc.count = field.length;

            // Zera o array antes de atribuir os valores
            for (uint8_t i = 0; i < MAX_BATTERIES; i++) {
                soc.values[i] = 0.0f;
            }

            for(uint8_t i = 0; i < soc.count; i++) {
                soc.values[i] = values[i];
            }

            return true;
        }
            
        default:
            return false;
    }
}

bool Inverter::getBatterySoH(BatteryValues& soh) {
    if (!hasValidMap()) return false;

    const BatteryFeature& feature = _map.battery;
    const ModbusField& originalField = feature.soh;

    if (!originalField.readable) return false;
    if (originalField.length == 0) return false;

    uint16_t count = feature.batteryCount;

    if (count == 0) return false;

    uint16_t effectiveLength = originalField.length;

    if (count < effectiveLength) {
        effectiveLength = count;
    }

    if (effectiveLength == 0) return false;
    if (effectiveLength > MAX_BATTERIES) effectiveLength = MAX_BATTERIES;

    ModbusField field = originalField;
    field.length = effectiveLength;

    float values[MAX_BATTERIES];

    switch (field.mode) {

        case FIELD_SIMPLE: {
            if (!readScaledFloat(field, values, field.length)) return false;

            soh.count = field.length;

            // Zera o array antes de atribuir os valores
            for (uint8_t i = 0; i < MAX_BATTERIES; i++) {
                soh.values[i] = 0.0f;
            }

            for(uint8_t i = 0; i < soh.count; i++) {
                soh.values[i] = values[i];
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

bool Inverter::getEPSVoltage(float& voltage) {
    if (!hasValidMap()) return false;

    const EpsFeature& feature = _map.eps;

    if (feature.phaseCount != 1) return false;

    const ModbusField& field = feature.voltage;

    if (field.length != 1) return false;

    switch (field.mode) {
        case FIELD_SIMPLE:
            if (!field.readable) return false;
            return readScaledFloat(field, voltage);

        default:
            return false;
    }
}

bool Inverter::getEPSVoltage(PhaseData& phase) {
    if (!hasValidMap()) return false;

    const EpsFeature& feature = _map.eps;

    if (feature.phaseCount != 3) return false;

    const ModbusField& field = feature.voltage;
    
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

bool Inverter::getEPSCurrent(float& current) {
    if (!hasValidMap()) return false;

    const EpsFeature& feature = _map.eps;

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

bool Inverter::getEPSCurrent(PhaseData& phase) {
    if (!hasValidMap()) return false;

    const EpsFeature& feature = _map.eps;

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

bool Inverter::getEPSActivePower(float& power) {
    if (!hasValidMap()) return false;

    const EpsFeature& feature = _map.eps;

    if (feature.phaseCount != 1) return false;

    const ModbusField& field = feature.activePower;

    if (field.length != 1) return false;

    switch (field.mode) {
        case FIELD_SIMPLE:
            if (!field.readable) return false;
            return readScaledFloat(field, power);

        default:
            return false;
    }
}

bool Inverter::getEPSActivePower(PhaseData& phase) {
    if (!hasValidMap()) return false;

    const EpsFeature& feature = _map.eps;

    if (feature.phaseCount != 3) return false;

    const ModbusField& field = feature.activePower;
    
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

// ======================================================
// Diagnostics and Health
// ======================================================

bool Inverter::getTemperature(float& temperature) {
    if (!hasValidMap()) return false;

    const DiagnosticFeature& feature = _map.diagnostic;
    const ModbusField& field = feature.temperature;    
    
    switch (field.mode) {

        case FIELD_SIMPLE:
            if (!field.readable) return false;
            return readScaledFloat(field, temperature);
            
        default:
            return false;
    }
}

bool Inverter::getInsulationResistance(float& kohm) {
    if (!hasValidMap()) return false;

    const DiagnosticFeature& feature = _map.diagnostic;
    const ModbusField& field = feature.insulationResistance;    
    
    switch (field.mode) {

        case FIELD_SIMPLE:
            if (!field.readable) return false;
            return readScaledFloat(field, kohm);
            
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
bool Inverter::getInverterStatus(uint32_t& status) {//InverterStatus& status) {
    if (!hasValidMap()) return false;

    const ModbusField& field = _map.status.inverterStatus;
    
    if (isInvalidField(field)) return false;
    if (!field.readable) return false;
    if (field.length != 1) return false;

    switch (field.mode) {

        case FIELD_SIMPLE:{
            uint16_t raw = 0;
            if (!readField(field, &raw)) return false;

            status = raw;//(InverterStatus)raw;
            return true;
        }
            
        default:
            return false;
    }
}

bool Inverter::getAlarm(uint32_t& alarm) {//Alarm& alarm) {
    if (!hasValidMap()) return false;

    const ModbusField& field = _map.status.alarm;

    if (isInvalidField(field)) return false;
    if (!field.readable) return false;
    if (field.length != 1) return false;

    switch (field.mode) {

        case FIELD_SIMPLE: {
            uint16_t raw = 0;
            if (!readField(field, &raw)) return false;

            alarm = raw; //(Alarm)raw;
            return true;
        }
            
        default:
            return false;
    }
}
// #############################################################################################################
