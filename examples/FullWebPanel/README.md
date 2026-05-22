# FullWebPanel

This example demonstrates a web-based test panel for **InverterModbusLib**.

The ESP8266 creates a WiFi Access Point and serves a mobile-friendly web interface. From this interface, the user can select an inverter model, configure Modbus parameters and test several public functions from the library.

This example is mainly intended for development, diagnostics and Modbus map validation.

## Features

- ESP8266 / ESP-07 example
- WiFi Access Point mode
- Mobile-friendly web interface
- RS485 Modbus RTU communication
- Inverter model selection
- Runtime Modbus configuration
- Runtime serial configuration
- Function testing through web buttons
- Read and write commands using HTTP requests
- JavaScript `fetch()` calls without full page reload
- HTML stored in `PROGMEM`
- Useful for validating inverter maps with real equipment

## Hardware

- ESP8266 / ESP-07
- TTL to RS485 converter
- RS485 bus wiring
- Compatible inverter
- External power supply suitable for the ESP and RS485 converter

## WiFi Access

After startup, connect to:

| Parameter | Value |
|---|---|
| SSID | ESP07-Inverter |
| Password | 12345678 |
| Address | http://192.168.4.1 |

## Web Interface

The web interface contains two main pages:

| Page | Description |
|---|---|
| Panel | Main page used to execute read/write functions |
| Configuration | Page used to select inverter model and Modbus/UART parameters |

The navigation bar at the top allows switching between the main panel and the configuration page.

## Configuration Page

The configuration page allows selecting:

- inverter model
- slave ID
- baud rate
- parity
- stop bits
- DE/RE pin

After submitting the configuration, the selected parameters are applied to the active inverter object.

The DE/RE pin can be set to `-1` when RS485 direction control is not required by a dedicated GPIO.

## Main Panel

The main panel is divided into functional sections.

### Identification

Used to read the inverter serial number.

Main function:

```cpp
getSerialNumber()
```

### Control and Limits

Used to send basic control and power limitation commands.

Main functions:

```cpp
boot()
shutdown()
setBoot()
setPowerLimit()
setPowerLimitPercent()
setExportLimit()
setExportLimitPercent()
```

### Date and Time

Used to read date/time values from the inverter.

Main functions:

```cpp
getYear()
getMonth()
getDay()
getHour()
getMinute()
getSecond()
getEpochTime()
```

### AC Measurements

Used to read grid and AC output measurements.

Main functions:

```cpp
getActivePower()
getGridVoltage()
getGridCurrent()
getGridFrequency()
```

### Energy

Used to read energy values.

Main functions:

```cpp
getTotalEnergy()
getDailyEnergy()
```

### PV Strings

Used to read PV string voltage and current values.

Main functions:

```cpp
getStringVoltage()
getStringCurrent()
```

### Diagnostics and Health

Used to read diagnostic values.

Main functions:

```cpp
getTemperature()
getInsulationResistance()
```

## Web Interface Screenshots

The screenshots below show the web panel running on a mobile browser.

### SIW400G Example

![FullWebPanel running with SIW400G](../../resources/FullWebPanel/SIW400G.jpeg)

### SIW500H Example

![FullWebPanel running with SIW500H](../../resources/FullWebPanel/SIW500H.jpeg)

## How It Works

The HTML interface is stored separately in `WebPage.h` using `PROGMEM`.

The main sketch serves the web pages and handles API routes used by JavaScript.

The interface uses JavaScript `fetch()` requests to call the ESP8266 without reloading the full page.

Typical flow:

```text
User presses a button
↓
JavaScript sends a request to the ESP8266
↓
ESP8266 calls the corresponding InverterModbusLib function
↓
ESP8266 returns a text response
↓
JavaScript updates the corresponding display field
```

## API Routes Used by the Web Panel

| Route | Purpose |
|---|---|
| `/` | Main web panel |
| `/config` | Configuration page |
| `/config/apply` | Applies selected model and Modbus/UART parameters |
| `/api/config` | Returns active inverter configuration |
| `/api/get` | Executes read functions |
| `/api/set` | Executes write/control functions |

## Modbus Configuration

The example uses one `ModbusRTU` object and one active inverter object.

The selected configuration is applied at runtime according to the form values from the configuration page.

`ModbusConfigData` format:

```cpp
{ slaveId, baudRate, serialConfig, deRePin }
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

## Important Notes

This example is intended for testing and validation.

Some functions may not work on all inverter models. A failed response does not necessarily mean that the library is broken. It may indicate:

- unsupported register
- wrong Modbus map
- different firmware behavior
- disabled inverter function
- manufacturer-specific encoding
- missing fallback implementation
- write permission not enabled in the inverter

Always validate the Modbus map with real equipment before using write commands in a production system.

## Safety Notes

This example can send write commands to real inverter equipment.

Use write functions carefully, especially:

```cpp
boot()
shutdown()
setPowerLimit()
setPowerLimitPercent()
setExportLimit()
setExportLimitPercent()
```

Incorrect Modbus writes may change inverter behavior.

## ESP8266 Notes

On ESP8266, the main UART may be used for RS485 communication.

Avoid using `Serial.print()` for debugging on the same UART while Modbus communication is active.

For larger projects, consider:

- limiting dynamic `String` usage
- storing static HTML in `PROGMEM`
- avoiding excessive polling
- monitoring available heap
- separating validation tools from production firmware

## Folder Structure

Expected resource folder:

```bash
resources/
└── FullWebPanel/
    ├── SIW400G.jpeg
    └── SIW500H.jpeg
```

Example folder structure:

```bash
examples/
└── FullWebPanel/
    ├── FullWebPanel.ino
    ├── WebPage.h
    └── README.md
```

## Main Files

| File | Description |
|---|---|
| `FullWebPanel.ino` | Main Arduino sketch |
| `WebPage.h` | HTML, CSS and JavaScript stored in `PROGMEM` |
| `README.md` | Example documentation |

## Main Functions Used

```cpp
attachModbus()
attachSerial()
attachConfig()
begin()

getSerialNumber()

boot()
shutdown()
setBoot()
setPowerLimit()
setPowerLimitPercent()
setExportLimit()
setExportLimitPercent()

getYear()
getMonth()
getDay()
getHour()
getMinute()
getSecond()
getEpochTime()

getActivePower()
getGridVoltage()
getGridCurrent()
getGridFrequency()

getTotalEnergy()
getDailyEnergy()

getStringVoltage()
getStringCurrent()

getTemperature()
getInsulationResistance()
```

## Validation Purpose

This example is especially useful for validating inverter maps.

It allows testing many public functions from the browser without changing and recompiling the sketch for each function.

Recommended validation workflow:

1. Select the inverter model.
2. Configure slave ID, baud rate and serial parameters.
3. Test serial number reading.
4. Test basic AC measurements.
5. Test energy readings.
6. Test PV string readings.
7. Test write functions carefully.
8. Record which functions returned valid results.

## Status

This example is experimental and intended for development, diagnostics and validation during the alpha stage of InverterModbusLib.