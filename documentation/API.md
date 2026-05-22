# API

This document describes the public API of InverterModbusLib for the current alpha release.

Some functions may exist internally in the library but are not considered stable yet.

## Initialization

```cpp
Inverter(InverterModel model);

void attachModbus(ModbusRTU& mb);
void attachSerial(HardwareSerial& serial);
void attachConfig(const ModbusConfigData& config);

bool begin();
void setSlaveId(uint8_t id);
```

### Initialization Order

Recommended initialization order:

```cpp
inverter.attachModbus(mb);
inverter.attachSerial(Serial);
inverter.attachConfig(cfg);
inverter.begin();
```

If `attachConfig()` is not used, the default Modbus configuration from the inverter descriptor is used.

## Modbus Configuration

```cpp
struct ModbusConfigData {
    uint8_t id;
    uint32_t baud;
    SerialConfig serialConfig;
    int8_t deRePin;
};
```

Example:

```cpp
ModbusConfigData cfg = {
    1,
    9600,
    SERIAL_8N1,
    12
};
```

## Control

```cpp
bool boot();
bool shutdown();
bool setBoot(bool enable);

bool setPowerLimit(float watts);
bool setPowerLimitPercent(float percent);

bool setExportLimit(float watts);
bool setExportLimitPercent(float percent);
```

## Identification

```cpp
bool getSerialNumber(String& serialNumber);
```

## Measurements

```cpp
bool getActivePower(float& watts);

bool getGridVoltage(PhaseData& phase);
bool getGridCurrent(PhaseData& phase);
bool getGridFrequency(PhaseData& phase);

bool getTemperature(float& temperature);
bool getInsulationResistance(float& kiloOhms);
```

## Energy

```cpp
bool getTotalEnergy(float& kWh);
bool getDailyEnergy(float& kWh);
```

## PV Strings

```cpp
bool getStringVoltage(StringValues& voltage);
bool getStringCurrent(StringValues& current);
```

## Date and Time

```cpp

bool getYear(uint16_t& year);
bool getMonth(uint16_t& month);
bool getDay(uint16_t& day);
bool getHour(uint16_t& hour);
bool getMinute(uint16_t& minute);
bool getSecond(uint16_t& second);

bool getEpochTime(uint32_t& epoch);

```

## Data Structures

### PhaseData

```cpp
struct PhaseData {
    float r;
    float s;
    float t;
};
```

For single-phase inverters, use `r` as the main phase value.

For three-phase inverters, use `r`, `s` and `t`.

### StringValues

```cpp
struct StringValues {
    uint8_t count;
    float values[MAX_STRINGS];
    bool isEstimated;
};
```

## Return Values

Most functions return `bool`.

```cpp
true
```

means the operation was successfully executed.

```cpp
false
```

means the operation failed, the field is not available for the selected model, or the inverter did not respond correctly.

## Experimental Functions

Some functions may exist in the library but are not considered stable for the current alpha release.

Examples:

```cpp

bool isBooted(bool& isBooted);
bool isPowerLimitEnabled(bool& enabled);
bool getPowerLimit(float& watts);
bool getPowerLimitPercent(float& percent);
bool isExportLimitEnabled(bool& enabled);
bool getExportLimit(float& watts);
bool getExportLimitPercent(float& percent);
bool isPowerFactorEnabled(bool& enabled);
bool getPowerFactorSetpoint(float& pf);

bool setPowerLimitEnabled(bool enabled);
bool setExportLimitEnabled(bool enabled);
bool setPowerFactorEnabled(bool enabled);
bool setPowerFactor(float pf);
bool setPowerFactorExcitationMode(PfExcitationMode excitationMode);

bool getReactivePower(float& voltAmperReactive);
bool getApparentPower(float& voltAmper);
bool getPowerFactor(float& pf);

bool getDatetime(Datetime& dt);

bool setDatetime(Datetime dt);
bool setYear(uint16_t year);
bool setMonth(uint16_t month);
bool setDay(uint16_t day);
bool setHour(uint16_t hour);
bool setMinute(uint16_t minute);
bool setSecond(uint16_t second);
bool setEpochTime(uint32_t epoch);

bool getStringPower(StringValues& power);

bool getInverterStatus(InverterStatus& status);
bool getAlarm(Alarm& alarm);

bool getBatteryVoltage(BatteryValues& voltage);
bool getBatteryCurrent(BatteryValues& current);
bool getBatteryPower(BatteryValues& power);
bool getBatterySoC(BatteryValues& soc);
bool getBatterySoH(BatteryValues& soh);

bool getEPSVoltage(PhaseData& phase);
bool getEPSCurrent(PhaseData& phase);
bool getEPSActivePower(PhaseData& phase);
```

These functions may change in future versions.

## Supported Models

For supported models and validation status, see:

[Compatibility](COMPATIBILITY.md)



## Contributions
---
https://github.com/emelianov/modbus-esp8266


## License
---
The code in this repo is licensed under de BSD 3-Clause License. See LICENSE.txt for more info.