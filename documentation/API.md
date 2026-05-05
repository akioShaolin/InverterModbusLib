## API

### Beggining

Settings

```cpp
Inverter(InverterModel model);

void attachModbus(ModbusRTU& mb);
void attachConfig(const ModbusConfig& config);
void setSlaveId(uint8_t id);

bool begin();
```

Inverter Control

```cpp
bool boot();
bool setBoot(bool boot);
bool shutdown();
bool setPowerLimitEnabled(bool enabled);
bool setPowerLimit(float watts);
bool setPowerLimitPercent(float percent);
bool setExportLimitEnabled(bool enabled);
bool setExportLimit(float watts);
bool setExportLimitPercent(float percent);
bool setPowerFactorEnabled(bool enabled);
bool setPowerFactor(float pf);
bool setPowerFactorExcitationMode(PfExcitationMode excitationMode);
```

Get information from inverter

```cpp
bool getSerial(String& serial);
bool isBooted(bool& isBooted);
bool isPowerLimitEnabled(bool& enabled);
bool getPowerLimit(float& watts);
bool getPowerLimitPercent(float& percent);
bool isExportLimitEnabled(bool& enabled);
bool getExportLimit(float& watts);
bool getExportLimitPercent(float& percent);
bool isPowerFactorEnabled(bool& enabled);
bool getPowerFactorSetpoint(float& pf); 
bool getTotalEnergy(float& kWh);
bool getDailyEnergy(float& kWh);

bool getActivePower(float& watts);
bool getReactivePower(float& voltAmperReactive);
bool getApparentPower(float& voltAmper);
bool getPowerFactor(float &pf);

// Grid
bool getGridVoltage(PhaseData& phase);
bool getGridCurrent(PhaseData& phase);
bool getGridFrequency(PhaseData& phase);

// Internal
bool getTemperature(float& temperature);
bool getInsulationResistance(float& kiloOhms);

// Status and Alarm
bool getInverterStatus(InverterStatus& status);
bool getAlarm(Alarm& alarm);

// Strings
bool getStringVoltage(StringValues& voltage);
bool getStringCurrent(StringValues& current);
bool getStringPower(StringValues& power);

// Battery
bool getBatteryVoltage(BatteryValues& voltage);
bool getBatteryCurrent(BatteryValues& current);
bool getBatteryPower(BatteryValues& power);
bool getBatterySoC(BatteryValues& soc);
bool getBatterySoH(BatteryValues& soh);

// EPS
bool getEPSVoltage(PhaseData& phase);
bool getEPSCurrent(PhaseData& phase);
bool getEPSActivePower(PhaseData& phase);
```

Get and Set time

```cpp
bool getDatetime(Datetime& dt);
bool getYear(uint16_t& year);
bool getMonth(uint16_t& month);
bool getDay(uint16_t& day);
bool getHour(uint16_t& hour);
bool getMinute(uint16_t& minute);
bool getSecond(uint16_t& second);
bool getEpochTime(uint32_t& epoch);

bool setDatetime(Datetime dt);
bool setYear(uint16_t year);
bool setMonth(uint16_t month);
bool setDay(uint16_t day);
bool setHour(uint16_t hour);
bool setMinute(uint16_t minute);
bool setSecond(uint16_t second);
bool setEpochTime(uint32_t epoch);
```

## Data Information
---

```cpp

struct PhaseData {
    float grid;
    float r;
    float s;
    float t;
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
    LAGGING,        // Same as Inductive and Over-excited
    LEADING,        // Same as Capacitive and Under-excited
    INDUCTIVE,
    CAPACITIVE,
    OVER_EXCITED,
    UNDER_EXCITED
};

struct StringValues {
    uint8_t count;
    float values[MAX_STRINGS];
    bool isEstimated;
};

struct BatteryValues {
    uint8_t count;
    float values[MAX_BATTERIES];
};

struct Datetime {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

// Alarm and InverterStatus
```

## SUPORTED MODELS

### Weg
---
SIW200_M030, SIW200_M050, SIW200_M085, SIW200_M100, SIW200H_M050_W00, SIW200H_M075_W10, SIW200H_M105_W10, SIW200G_M030_W0, SIW200G_M030_W1, SIW200G_M050_W0, SIW200G_M050_W1, SIW200G_M060_W0, SIW200G_M060_W1, SIW200G_M070_W1, SIW200G_M075_W1, SIW200G_M080_W1, SIW200G_M090_W1, SIW200G_M105_W1, SIW300H_M030_W00, SIW300H_M050_W00, SIW300H_M060_W00, SIW300H_M075_W00, SIW300H_M080_W00, SIW300H_M100_W00, SIW400_ST075, SIW400G_T012_W0, SIW400G_T012_W1, SIW400G_T015_W0, SIW400G_T015_W1, SIW400G_T020_W0, SIW400G_T020_W1, SIW400G_T025_W0, SIW400G_T025_W1, SIW400G_T075_W0, SIW400G_T100_W0, SIW400G_T050_W00, SIW400G_T060_W00, SIW400G_T075_W01, SIW400G_K015_W00, SIW400G_K020_W00, SIW400G_K025_W00, SIW400G_K030_W00, SIW400G_K037_W00, SIW400H_T015_W10, SIW400H_T030_W10, SIW420G_K025_W00, SIW420G_K075_W00, SIW500G_T075_W0, SIW500G_T100_W0, SIW500G_K050_W00, SIW500G_K075_W00, SIW500G_H250_W0, SIW500H_ST012_M2, SIW500H_ST015_M2, SIW500H_ST020_M2, SIW500H_ST030_M3, SIW500H_ST036_M3, SIW500H_ST040_M3, SIW500H_SK020, SIW500H_ST030, SIW500H_ST036, SIW500H_ST040, SIW500H_ST060, SIW500H_ST100, SIW500H_ST060_HV, SIW500H_ST100_HV, SIW500H_ST200_H0, SIW500H_ST200_H3, SIW500H_T012_W00, SIW500H_T015_W00, SIW500H_T017_W00, SIW500H_T020_W00, SIW500H_T025_W00, SIW600_T020_44,SIW610_T018_W0, SIW610_T075_W0
    
### FoxEss
---
S700_G2, S1000_G2, S1500_G2, S2000_G2, S2500_G2, S3000_G2, S3300_G2, F3000_G2, F3600_G2, F4600_G2, F5000_G2, F5300_G2, F6000_G2, G7, G7_5, G8_, G9, G10, G10_5, T3_G3, T4_G3, T5_G3, T6_G3, T8_G3, T10_G3, T12_G3, T15_G3, T17_G3, T20_G3, T23_G3, T25_G3, V30, V33, V36, V40, V50, V60, V75, VL15, VL20, VL25, VL30, VL37_5, R75, R100, R110

### GoodWe
---
GW3000_XS, GW3300_XS_30, GW5000_DNS, GW5K_DNS_G40, GW8500_MS, GW10K_MS,

### Huawei
---
SUN2000_2KTL, SUN2000_3KTL, SUN2000_4KTL, SUN2000_5KTL, SUN2000_6KTL, SUN2000_12KTL_M0, SUN2000_12KTL_M2, SUN2000_15KTL_M0, SUN2000_15KTL_M2, SUN2000_20KTL_M0, SUN2000_20KTL_M2, SUN2000_30KTL, SUN2000_36KTL_A, SUN2000_30KTL_M3, SUN2000_36KTL_M3, SUN2000_40KTL_M3, SUN2000_60KTL_M0, SUN2000_75KTL_M1, SUN2000_100KTL_M2, SUN2000_100KTL_H1, SUN2000_215KTL_H0, SUN2000_215KTL_H3, SUN2000_250KTL_H1

## Contributions
---
https://github.com/emelianov/modbus-esp8266


## License
---
The code in this repo is licensed under de BSD 3-Clause License. See LICENSE.txt for more info.