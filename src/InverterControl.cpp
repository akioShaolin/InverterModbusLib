/*
 * InverterModbusLib - Solar Inverter Library for Arduino
 * ------------------------------------------------
 * Modbus RTU (RS485)communication layer for inverter integration
 *
 * Copyright (c) 2026, Pedro Akio Sakuma
 * Licensed under BSD 3-Clause License
 */
/*
InverterControl.cpp
├── Inicialização e configuração
│   ├── Inverter()
│   ├── attachModbus()
│   ├── attachConfig()
│   ├── attachSerial()
│   ├── begin()
│   └── setSlaveId()
│
├── Controle operacional
│   ├── boot()
│   ├── setBoot()
│   ├── shutdown()
│   ├── setPowerLimitEnabled()
│   ├── setPowerLimit()
│   ├── setPowerLimitPercent()
│   ├── setExportLimitEnabled()
│   ├── setExportLimit()
│   ├── setExportLimitPercent()
│   ├── setPowerFactorEnabled()
│   ├── setPowerFactor()
│   └── setPowerFactorExcitationMode()
│
└── Helpers internos
    └── isInvalidField()
*/

#include "Inverter.h"

// ======================================================
// Begin and Setup
// ======================================================

Inverter::Inverter(InverterModel model)
  : _modbus(1, 9600, SERIAL_8N1),
    _model(model),
    _mb(nullptr),
    _serialPort(&Serial) {

    _descriptor = getDescriptor(model);
    _map = getInverterMap(model);

    if (_descriptor.config != nullptr) {
        _cfg.id = pgm_read_byte(&_descriptor.config->id);
        _cfg.baud = pgm_read_dword(&_descriptor.config->baud);
        _cfg.serialConfig = (SerialConfig)pgm_read_dword(&_descriptor.config->serialConfig);
        _cfg.deRePin = (int8_t)pgm_read_byte(&_descriptor.config->deRePin);
    } else {
        _cfg = {1, 9600, SERIAL_8N1, -1};
    }
}

void Inverter::attachModbus(ModbusRTU& mb) {
    _mb = &mb;
}

void Inverter::attachConfig(const ModbusConfigData& config) {
    memcpy(&_cfg, &config, sizeof(ModbusConfigData));
}

void Inverter::attachSerial(HardwareSerial& serial) {
    _serialPort = &serial;
}

bool Inverter::begin() {
    if (_mb == nullptr) return false;
    if (!hasValidMap()) return false;
    if (_descriptor.ratedPowerW == 0) return false;  //São campos obrigatórios. A falta deles invalida a struct

    _modbus.setConfig(
        _cfg.id,
        _cfg.baud,
        _cfg.serialConfig,
        _cfg.deRePin
    );
    
    return true;
}

void Inverter::setSlaveId(uint8_t id) {
    _cfg.id = id;
    _modbus.setId(id);
}

// ======================================================
// Operational Control
// ======================================================

// Caso o inversor possua apenas um registrador para controle (boot/shutdown),
// este deve ser mapeado em _map.boot, e não em _map.shutdown.
bool Inverter::setBoot(bool enable) {
    return enable ? Inverter::boot() : shutdown();
}

bool Inverter::boot() {
    if (!hasValidMap()) return false;

    const ControlFeature& feature = _map.control;
    const ModbusField& field = feature.boot;
    const uint16_t bootValue = feature.bootValue;

    if (bootValue == FEATURE_VALUE_NONE) return false;
    if (isInvalidField(field)) return false;
    
    switch (field.mode) {

        case FIELD_SIMPLE:
            if (!field.writable) return false;
            return writeField(field, bootValue);

        default:
            return false;
    }
}

bool Inverter::shutdown() {
    if (!hasValidMap()) return false;

    const ControlFeature& feature = _map.control;
    const ModbusField& field = feature.shutdown;
    const uint16_t shutdownValue = feature.shutdownValue;

    if (shutdownValue == FEATURE_VALUE_NONE) return false;
    if (isInvalidField(field)) return false;
    
    switch (field.mode) {

        case FIELD_SIMPLE:
            if (!field.writable) return false;
            return writeField(field, shutdownValue);

        default:
            return false;
    }
}

bool Inverter::setPowerLimitEnabled(bool enabled) {
    if (!hasValidMap()) return false;

    const ActivePowerFeature& feature = _map.activePower;   
    
    // Fallback cai em enable por enum ou por implicit enable
    if (feature.supportsEnable) {

        const ModbusField& field = feature.enable;

        if (isInvalidField(field)) {
            if (feature.implicitEnable == true) {
                return enabled;
            }

            return false;
        }

        const uint16_t enableValue = feature.enableValue;
        const uint16_t disableValue = feature.disableValue;

        if (enableValue == FEATURE_VALUE_NONE || disableValue == FEATURE_VALUE_NONE) return false;

        switch (field.mode) {

            case FIELD_SIMPLE: {
                if (!field.writable) return false;
                uint16_t v = enabled
                    ? enableValue
                    : disableValue;

                return writeField(field, v);
            }

            default:
                return false;
        }
    }
    // Fallback do Mode
    if (feature.supportsMode) {
        const ModbusField& field = feature.mode;

        if (isInvalidField(field)) return false;

        const uint16_t enableMode = feature.enableValue;
        const uint16_t disableMode = feature.disableValue;

        if (enableMode == FEATURE_VALUE_NONE || disableMode == FEATURE_VALUE_NONE) return false;

        switch (field.mode) {

            case FIELD_SIMPLE: {
                if (!field.writable) return false;
                uint16_t v = enabled
                    ? enableMode
                    : disableMode;

                return writeField (field, v);
            }

            default:
                return false;
        }
    }
    // Fallback do implicit
    if (feature.implicitEnable) {
        return enabled;
    }

    return false;
}

bool Inverter::setPowerLimit(float watts) {
    if (!hasValidMap()) return false;
    
    const ActivePowerFeature& feature = _map.activePower;

    if (!feature.supportsWatts && !feature.supportsPercent) return false;

    if (feature.requiresEnableBeforeWrite) {
        if (!setPowerLimitEnabled(true)) return false;
    }    
    
    // 1) Tenta escrever diretamente em watts
    if (feature.supportsWatts) {
        bool wattsWriteOk = false;
    
        const ModbusField& field = feature.watts;

        if (!isInvalidField(field)) {
            switch (field.mode) {

                case FIELD_SIMPLE:
                    if (field.writable && field.scale != 0.0f) {

                        bool modeOk = true;

                        if (feature.requiresModeBeforeWrite) {
                            const ModbusField& modeField = feature.mode;

                            // Condição lógica para validar a escrita em watts. Se falhar, cai na fallback de percent
                            modeOk =    !isInvalidField(modeField) &&
                                        modeField.writable &&
                                        feature.wattsModeValue != FEATURE_VALUE_NONE &&
                                        writeField(modeField, feature.wattsModeValue);
                        }                    

                        if (modeOk) {
                            wattsWriteOk = writeField(field, watts / field.scale);
                        }
                    }
                    break;

                default:
                    break;
            }
        }

        if (wattsWriteOk) return true;
    }

    // 2) Fallback: converte watts para porcentagem
    if (feature.supportsPercent) {
        const ModbusField& field = feature.percent;

        if (isInvalidField(field)) return false;

        switch (field.mode) {
            case FIELD_SIMPLE: {
                if (!field.writable) return false;
                if (field.scale == 0.0f)  return false;

                uint32_t ratedPower = 0;

                if (!getRatedPower(ratedPower)) return false;
                if (ratedPower == 0) return false;

                if (feature.requiresModeBeforeWrite) {
                    const ModbusField& modeField = feature.mode;

                    if (isInvalidField(modeField)) return false;
                    if (!modeField.writable) return false;
                    if (feature.percentModeValue == FEATURE_VALUE_NONE) return false;

                    if (!writeField(modeField, feature.percentModeValue)) return false;
                }

                float percent = (watts * 100.0f) / ((float)ratedPower);

                return writeField(field, percent / field.scale);
            }

            default:
                return false;
        }
    }

    return false;
}

bool Inverter::setPowerLimitPercent(float percent) {
    if (!hasValidMap()) return false;
    
    const ActivePowerFeature& feature = _map.activePower;

    if (!feature.supportsPercent && !feature.supportsWatts) return false;

    if (feature.requiresEnableBeforeWrite) {
        if (!setPowerLimitEnabled(true)) return false;
    }
    
    // 1) Tenta escrever diretamente em percent
    if (feature.supportsPercent) {
        bool percentWriteOk = false;
    
        const ModbusField& field = feature.percent;

        if (!isInvalidField(field)) {
            switch (field.mode) {

                case FIELD_SIMPLE:
                    if (field.writable && field.scale != 0.0f) {

                        bool modeOk = true;

                        if (feature.requiresModeBeforeWrite) {
                            const ModbusField& modeField = feature.mode;

                            // Condição lógica para validar a escrita em percent. Se falhar, cai na fallback de watts
                            modeOk =    !isInvalidField(modeField) &&
                                        modeField.writable &&
                                        feature.percentModeValue != FEATURE_VALUE_NONE &&
                                        writeField(modeField, feature.percentModeValue);
                        }                    

                        if (modeOk) {
                            percentWriteOk = writeField(field, percent / field.scale);
                        }
                    }
                    break;

                default:
                    break;
            }
        }

        if (percentWriteOk) return true;
    }

    // 2) Fallback: converte percent para watts
    if (feature.supportsWatts) {
        const ModbusField& field = feature.watts;

        if (isInvalidField(field)) return false;

        switch (field.mode) {
            case FIELD_SIMPLE: {
                if (!field.writable) return false;
                if (field.scale == 0.0f)  return false;

                uint32_t ratedPower = 0;

                if (!getRatedPower(ratedPower)) return false;
                if (ratedPower == 0) return false;

                if (feature.requiresModeBeforeWrite) {
                    const ModbusField& modeField = feature.mode;

                    if (isInvalidField(modeField)) return false;
                    if (!modeField.writable) return false;
                    if (feature.wattsModeValue == FEATURE_VALUE_NONE) return false;

                    if (!writeField(modeField, feature.wattsModeValue)) return false;
                }

                float watts = percent * ((float)ratedPower) / 100.0f;

                return writeField(field, watts / field.scale);
            }

            default:
                return false;
        }
    }

    return false;
}

bool Inverter::setExportLimitEnabled(bool enabled) {
    if (!hasValidMap()) return false;

    const ExportLimitFeature& feature = _map.exportLimit;   
    
    // Fallback cai em enable por enum ou por implicit enable
    if (feature.supportsEnable) {

        const ModbusField& field = feature.enable;

        if (isInvalidField(field)) {
            if (feature.implicitEnable == true) {
                return enabled;
            }

            return false;
        }

        const uint16_t enableValue = feature.enableValue;
        const uint16_t disableValue = feature.disableValue;

        if (enableValue == FEATURE_VALUE_NONE || disableValue == FEATURE_VALUE_NONE) return false;

        switch (field.mode) {

            case FIELD_SIMPLE: {
                if (!field.writable) return false;
                uint16_t v = enabled
                    ? enableValue
                    : disableValue;

                return writeField(field, v);
            }

            default:
                return false;
        }
    }
    // Fallback do Mode
    if (feature.supportsMode) {
        const ModbusField& field = feature.mode;

        if (isInvalidField(field)) return false;

        const uint16_t enableMode = feature.enableValue;
        const uint16_t disableMode = feature.disableValue;

        if (enableMode == FEATURE_VALUE_NONE || disableMode == FEATURE_VALUE_NONE) return false;

        switch (field.mode) {

            case FIELD_SIMPLE: {
                if (!field.writable) return false;
                uint16_t v = enabled
                    ? enableMode
                    : disableMode;

                return writeField (field, v);
            }

            default:
                return false;
        }
    }
    // Fallback do implicit
    if (feature.implicitEnable) {
        return enabled;
    }

    return false;
}

bool Inverter::setExportLimit(float watts) {
    if (!hasValidMap()) return false;
    
    const ExportLimitFeature& feature = _map.exportLimit;

    if (!feature.supportsWatts && !feature.supportsPercent) return false;

    if (feature.requiresEnableBeforeWrite) {
        if (!setExportLimitEnabled(true)) return false;
    }
    
    // 1) Tenta escrever diretamente em watts
    if (feature.supportsWatts) {
        bool wattsWriteOk = false;
    
        const ModbusField& field = feature.watts;

        if (!isInvalidField(field)) {
            switch (field.mode) {

                case FIELD_SIMPLE:
                    if (field.writable && field.scale != 0.0f) {

                        bool modeOk = true;

                        if (feature.requiresModeBeforeWrite) {
                            const ModbusField& modeField = feature.mode;

                            // Condição lógica para validar a escrita em watts. Se falhar, cai na fallback de percent
                            modeOk =    !isInvalidField(modeField) &&
                                        modeField.writable &&
                                        feature.wattsModeValue != FEATURE_VALUE_NONE &&
                                        writeField(modeField, feature.wattsModeValue);
                        }                    

                        if (modeOk) {
                            wattsWriteOk = writeField(field, watts / field.scale);
                        }
                    }
                    break;

                default:
                    break;
            }
        }

        if (wattsWriteOk) return true;
    }

    // 2) Fallback: converte watts para percent
    if (feature.supportsPercent) {
        const ModbusField& field = feature.percent;

        if (isInvalidField(field)) return false;

        switch (field.mode) {
            case FIELD_SIMPLE: {
                if (!field.writable) return false;
                if (field.scale == 0.0f)  return false;

                uint32_t ratedPower = 0;

                if (!getRatedPower(ratedPower)) return false;
                if (ratedPower == 0) return false;

                if (feature.requiresModeBeforeWrite) {
                    const ModbusField& modeField = feature.mode;

                    if (isInvalidField(modeField)) return false;
                    if (!modeField.writable) return false;
                    if (feature.percentModeValue == FEATURE_VALUE_NONE) return false;

                    if (!writeField(modeField, feature.percentModeValue)) return false;
                }

                float percent = watts * 100.0f / ((float)ratedPower);

                return writeField(field, percent / field.scale);
            }

            default:
                return false;
        }
    }

    return false;
}

bool Inverter::setExportLimitPercent(float percent) {
    if (!hasValidMap()) return false;
    
    const ExportLimitFeature& feature = _map.exportLimit;

    if (!feature.supportsPercent && !feature.supportsWatts) return false;

    if (feature.requiresEnableBeforeWrite) {
        if (!setExportLimitEnabled(true)) return false;
    }
    
    // 1) Tenta escrever diretamente em percent
    if (feature.supportsPercent) {
        bool percentWriteOk = false;
    
        const ModbusField& field = feature.percent;

        if (!isInvalidField(field)) {
            switch (field.mode) {

                case FIELD_SIMPLE:
                    if (field.writable && field.scale != 0.0f) {

                        bool modeOk = true;

                        if (feature.requiresModeBeforeWrite) {
                            const ModbusField& modeField = feature.mode;

                            // Condição lógica para validar a escrita em percent. Se falhar, cai na fallback de watts
                            modeOk =    !isInvalidField(modeField) &&
                                        modeField.writable &&
                                        feature.percentModeValue != FEATURE_VALUE_NONE &&
                                        writeField(modeField, feature.percentModeValue);
                        }                    

                        if (modeOk) {
                            percentWriteOk = writeField(field, percent / field.scale);
                        }
                    }
                    break;

                default:
                    break;
            }
        }

        if (percentWriteOk) return true;
    }

    // 2) Fallback: converte percent para watts
    if (feature.supportsWatts) {
        const ModbusField& field = feature.watts;

        if (isInvalidField(field)) return false;

        switch (field.mode) {
            case FIELD_SIMPLE: {
                if (!field.writable) return false;
                if (field.scale == 0.0f)  return false;

                uint32_t ratedPower = 0;

                if (!getRatedPower(ratedPower)) return false;
                if (ratedPower == 0) return false;

                if (feature.requiresModeBeforeWrite) {
                    const ModbusField& modeField = feature.mode;

                    if (isInvalidField(modeField)) return false;
                    if (!modeField.writable) return false;
                    if (feature.wattsModeValue == FEATURE_VALUE_NONE) return false;

                    if (!writeField(modeField, feature.wattsModeValue)) return false;
                }

                float watts = percent * ((float)ratedPower) / 100.0f;

                return writeField(field, watts / field.scale);
            }

            default:
                return false;
        }
    }

    return false;
}

bool Inverter::setPowerFactorEnabled(bool enabled) {
    if (!hasValidMap()) return false;

    const ReactivePowerFeature& feature = _map.reactivePowerControl;   
    
    // Fallback cai em enable por enum ou por implicit enable
    if (feature.supportsEnablePf) {

        const ModbusField& field = feature.enablePf;

        if (isInvalidField(field)) {
            if (feature.implicitPfSp == true) {
                return enabled;
            }

            return false;
        }

        const uint16_t enableValue = feature.enablePfValue;
        const uint16_t disableValue = feature.disablePfValue;

        if (enableValue == FEATURE_VALUE_NONE || disableValue == FEATURE_VALUE_NONE) return false;

        switch (field.mode) {

            case FIELD_SIMPLE: {
                if (!field.writable) return false;
                uint16_t v = enabled
                    ? enableValue
                    : disableValue;

                return writeField(field, v);
            }

            default:
                return false;
        }
    }
    // Fallback do Mode
    if (feature.supportsControlModePf) {
        const ModbusField& field = feature.controlMode;

        if (isInvalidField(field)) return false;

        const uint16_t enableMode = feature.enablePfValue;
        const uint16_t disableMode = feature.disablePfValue;

        if (enableMode == FEATURE_VALUE_NONE || disableMode == FEATURE_VALUE_NONE) return false;

        switch (field.mode) {

            case FIELD_SIMPLE: {
                if (!field.writable) return false;
                uint16_t v = enabled
                    ? enableMode
                    : disableMode;

                return writeField(field, v);
            }

            default:
                return false;
        }
    }
    // Fallback do implicit
    if (feature.implicitPfSp) {
        return enabled;
    }

    return false;
}

bool Inverter::setPowerFactor(float pf) {
    if (!hasValidMap()) return false;
    if (pf <= -1.0f || pf > 1.0f || pf == 0.0f) return false;
    
    const ReactivePowerFeature& feature = _map.reactivePowerControl;

    if (!feature.supportsPfSp) return false;

    if (feature.requiresEnableBeforeWrite) {
        if (!setPowerFactorEnabled(true)) return false;
    }

    const ModbusField& fieldPfSp = feature.pfSetpoint;

    if (isInvalidField(fieldPfSp)) return false;

    switch (fieldPfSp.mode) {
        case FIELD_SIMPLE: {
            if (!fieldPfSp.writable) return false;
            if (fieldPfSp.scale == 0.0f)  return false;

            if (feature.usesGoodWePowerFactorRange) {
                // Set Power Factor % [1, 20] LAGGING, [80, 100] LEADING
                // 1-20,lagging 0.99-0.8;   80-100,leading 0.80-1
                // Lagging: PF 1.00 -> 0%, PF 0.80 -> 20%
                // Leading: PF -0.80 -> 80%, PF -1.00 -> 100%
                // Goodwe usa ranges invertidos, leading e lagging são codificados em regiões diferentes
            
                pf = -pf;

                if (pf < 0.0f) {
                    pf = 1.0f + pf;
                }

                // Percent
                return writeField(fieldPfSp, (pf * 100.0f) / fieldPfSp.scale);
            }
            const ModbusField& fieldExcitation = feature.excitationMode;

            if (!isInvalidField(fieldExcitation) && fieldExcitation.writable) {
                if(!setPowerFactorExcitationMode(pf < 0.0f ? LEADING : LAGGING)) return false;
                pf = pf < 0.0f ? -pf : pf;
            }
            return writeField(fieldPfSp, pf / fieldPfSp.scale);  

        }

        default:
            return false;
    }

    return false;
}

bool Inverter::setPowerFactorExcitationMode(PfExcitationMode excitationMode) {
    if (!hasValidMap()) return false;

    const ReactivePowerFeature& feature = _map.reactivePowerControl;

    if (!feature.supportsExcitationMode) return false;

    const ModbusField& field = feature.excitationMode;

    if (isInvalidField(field)) return false;

    switch (field.mode) {

        case FIELD_SIMPLE:
        // Até o momento somente inversores foxess e similares apresentam essa função
            if (!field.writable) return false;
            return writeField(field, (uint16_t)excitationMode);
            
        default:
            return false;
    }
}