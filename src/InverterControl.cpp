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

Inverter::Inverter(InverterModel model) : _model(model), _modbus(1, 9600, SERIAL_8N1) {
    _descriptor = getDescriptor(model);
    _map = getInverterMap(model);
}

void Inverter::attachModbus(ModbusRTU& mb) {
    _mb = &mb;
}

void Inverter::attachConfig(const ModbusConfig& config) {
    _modbus = config;
    _customConfigSet = true;
}

bool Inverter::begin() {
    if (_mb == nullptr) return false;
    if (_map.serial.address == 0xFFFF) return false;
    if (_descriptor.nominalPowerW == 0) return false;  //São campos obrigatórios. A falta deles invalida a struct

    if (!_customConfigSet) {
        if (_descriptor.config == nullptr) return false;
        memcpy_P(&_modbus, _descriptor.config, sizeof(ModbusConfig));
    }

    getSerial(_serial);
    
    return true;
}

void Inverter::setSlaveId(uint8_t id) {
    _modbus.setId(id);
}

// ======================================================
// Operational Control
// ======================================================

// Caso o inversor possua apenas um registrador para controle (boot/shutdown),
// este deve ser mapeado em _map.boot, e não em _map.shutdown.
bool Inverter::boot() {
    if (_map.serial.address == 0xFFFF) return false;
    if (_descriptor.bootMode == nullptr) return false;

    uint16_t v = pgm_read_word(&_descriptor.bootMode->bootValue);
    
    switch (_map.boot.mode) {

        case FIELD_SIMPLE:
            if (!_map.boot.writable) return false;
            return writeField(_map.boot, v);

        default:
            return false;
    }    
}

bool Inverter::setBoot(bool enable) {
    return enable ? Inverter::boot() : shutdown();
}

bool Inverter::shutdown() {
    if (_map.serial.address == 0xFFFF) return false;
    if (_descriptor.bootMode == nullptr) return false;
    
    uint16_t v = pgm_read_word(&_descriptor.bootMode->shutdownValue);

    switch (_map.shutdown.mode) {

        case FIELD_SIMPLE:
            if(_map.shutdown.writable) {
                return writeField(_map.shutdown, v);
            }
            
            if (_map.boot.writable) {
                return writeField(_map.boot, v);
            }
            return false;

        default:
            return false;
    }
}

bool Inverter::setPowerLimitEnabled(bool enabled) {
    if (_map.serial.address == 0xFFFF) return false;
    
    switch (_map.enablePowerLimit.mode) {

        case FIELD_SIMPLE:
            if (!_map.enablePowerLimit.writable) return false;
            return writeField(_map.enablePowerLimit, (uint16_t)enabled);

        default:
            return false;
    }
}

bool Inverter::setPowerLimit(float watts) {
    if (_map.serial.address == 0xFFFF) return false;
    if (_descriptor.nominalPowerW == 0) return false;

    switch (_map.PowerLimit.mode) {

        case FIELD_SIMPLE:
            if (_map.PowerLimit.writable && _map.PowerLimit.scale == 0.0f) {
                return writeField(_map.PowerLimit, watts / _map.PowerLimit.scale);
            }

            if(_map.PowerLimitPercent.writable && _map.PowerLimitPercent.scale == 0.0f) {
                float percent = (watts / (float)_descriptor.nominalPowerW) * 100.0f;
                return writeField(_map.PowerLimitPercent, percent / _map.PowerLimitPercent.scale);
            }
            return false;

        default:
            return false;
    }
}

bool Inverter::setPowerLimitPercent(float percent) {
    if (_map.serial.address == 0xFFFF) return false;
    if (_descriptor.nominalPowerW == 0) return false;

    switch (_map.PowerLimitPercent.mode) {
        
        case FIELD_SIMPLE:
            if (_map.PowerLimitPercent.writable && _map.PowerLimitPercent.scale == 0.0f) {
                return writeField(_map.PowerLimitPercent, percent / _map.PowerLimitPercent.scale);
            }

            if (_map.PowerLimit.writable && _map.PowerLimit.scale == 0.0f) {
                float watts = ((float)_descriptor.nominalPowerW * percent) / 100.0f;
                return writeField(_map.PowerLimit, watts / _map.PowerLimit.scale);
            }
            return false;

        default:
            return false;
    }
}

bool Inverter::setExportLimitEnabled(bool enabled) {
    if (_map.serial.address == 0xFFFF) return false;
    if (!_map.enableExportLimit.writable) return false;
    if (_descriptor.exportLimitMode == nullptr) return false;
    
    switch (_map.enableExportLimit.mode) {

        case FIELD_SIMPLE: {
            uint16_t v = enabled
                ? pgm_read_word(&_descriptor.exportLimitMode->exportLimitEnable)
                : pgm_read_word(&_descriptor.exportLimitMode->exportLimitDisable);
            
            return writeField(_map.enableExportLimit, v);
        }
        
        default:

            // Special Field daqui para baixo
            return false;
    }
}

bool Inverter::setExportLimit(float watts) {
    if (_map.serial.address == 0xFFFF) return false;
    if (_descriptor.nominalPowerW == 0) return false;

    switch (_map.ExportLimit.mode) {

        case FIELD_SIMPLE:
            if (_map.ExportLimit.writable && _map.ExportLimit.scale != 0.0f) {
                return writeField(_map.ExportLimit, watts / _map.ExportLimit.scale);
            }

            if(_map.ExportLimitPercent.writable && _map.ExportLimitPercent.scale != 0.0f) {
                float percent = (watts / (float)_descriptor.nominalPowerW) * 100.0f;
                return writeField(_map.ExportLimitPercent, percent / _map.ExportLimitPercent.scale);
            }
            return false;

        default:
            return false;
    }
}

bool Inverter::setExportLimitPercent(float percent) {
    if (_map.serial.address == 0xFFFF) return false;
    if (_descriptor.nominalPowerW == 0) return false;

    switch (_map.ExportLimitPercent.mode) {
        
        case FIELD_SIMPLE:
            if (_map.ExportLimitPercent.writable && _map.ExportLimitPercent.scale != 0.0f) {
                return writeField(_map.ExportLimitPercent, percent / _map.ExportLimitPercent.scale);
            }

            if (_map.ExportLimit.writable && _map.ExportLimit.scale != 0.0f) {
                float watts = ((float)_descriptor.nominalPowerW * percent) / 100.0f;
                return writeField(_map.ExportLimit, watts / _map.ExportLimit.scale);
            }
            return false;

        default:
            return false;
    }
}

bool Inverter::setPowerFactorEnabled(bool enabled) {
    if (_map.serial.address == 0xFFFF) return false;

    switch (_map.enablePowerFactor.mode) {
    
        case FIELD_SIMPLE:
            if (!_map.enablePowerFactor.writable) return false;
            return writeField(_map.enablePowerFactor, (uint16_t)enabled);

        default:
            return false;
    }
}

bool Inverter::setPowerFactor(float pf) {
    if (_map.serial.address == 0xFFFF) return false;
    if (pf <= -1.0f || pf > 1.0f || pf == 0.0f) return false;
    if (isInvalidField(_map.PowerFactorSetpoint)) return false;

    switch (_map.PowerFactorSetpoint.mode) {
    
        case FIELD_SIMPLE:
            if (!_map.PowerFactorSetpoint.writable && _map.PowerFactorSetpoint.scale == 0.0f) return false;

            if (!isInvalidField(_map.powerFactorExcitationMode) && _map.powerFactorExcitationMode.writable) {
                if(!setPowerFactorExcitationMode(pf < 0.0f ? LEADING : LAGGING)) return false;
                pf = pf < 0.0f ? -pf : pf;
            }
                            
            return writeField(_map.PowerFactorSetpoint, pf / _map.PowerFactorSetpoint.scale);

        case FIELD_SPECIAL:
            if (_map.PowerFactorSetpoint.handlerId == GOODWE_HANDLER) {
                if (!_map.PowerFactorSetpoint.writable && _map.PowerFactorSetpoint.scale == 0.0f) return false;
                // Set Power Factor % [1, 20] LAGGING, [80, 100] LEADING
                // 1-20,lagging 0.99-0.8;   80-100,leading 0.80-1
                // Goodwe usa ranges invertidos, leading e lagging são codificados em regiões diferentes
            
                pf = -pf;

                if (pf < 0.0f) {
                    pf = 1.0f + pf;
                }

                // Percent
                return writeField(_map.PowerFactorSetpoint, (pf * 100.0f) / _map.PowerFactorSetpoint.scale);
            }
            return false;

        default:
            return false;
    }
}

bool Inverter::setPowerFactorExcitationMode(PfExcitationMode excitationMode) {
    if (_map.serial.address == 0xFFFF) return false;
    if (isInvalidField(_map.powerFactorExcitationMode)) return false;
    
    uint16_t mode = 0;
    // 0 - lagging / inductive / over excited
    // 1 - leading / capacitive / under excited

    switch (excitationMode) {
        case LAGGING:
        case INDUCTIVE:
        case OVER_EXCITED:
            mode = 0;
            break;

        case LEADING:
        case CAPACITIVE:
        case UNDER_EXCITED:
            mode = 1;
            break;

        default:
            return false;
    }

    switch (_map.powerFactorExcitationMode.mode) {

        case FIELD_SIMPLE:
            if (!_map.powerFactorExcitationMode.writable) return false;
            return writeField(_map.powerFactorExcitationMode, mode);
            
        default:
            return false;
    }
}

// ======================================================
// Internal Helpers
// ======================================================

bool Inverter::isInvalidField(const ModbusField& field) {
    return field.address == 0xFFFF;
}
