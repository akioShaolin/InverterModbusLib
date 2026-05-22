# ReadSerialTwoInverters

This example demonstrates how to read the serial number from two different inverters connected to the same RS485 Modbus RTU bus using **InverterModbusLib**.

The ESP8266 creates a WiFi Access Point and serves a simple web page. From this page, the user can select one of the available inverter models and request its serial number.

## Features

- ESP8266 / ESP-07 example
- WiFi Access Point mode
- Simple mobile-friendly web interface
- RS485 Modbus RTU communication
- Two inverters on the same bus
- Different slave IDs for each inverter
- Serial number reading through `getSerialNumber()`

## Hardware

- ESP8266 / ESP-07
- TTL to RS485 converter
- RS485 bus wiring
- Compatible inverters

## Tested Inverters

| Inverter | Slave ID | Baud Rate | Serial Config |
|---|---:|---:|---|
| SIW400G T100 W0 | 2 | 9600 | SERIAL_8N1 |
| SIW500H ST030 M3 | 1 | 9600 | SERIAL_8N1 |

## WiFi Access

After startup, connect to:

| Parameter | Value |
|---|---|
| SSID | ESP07-Inverter |
| Password | 12345678 |
| Address | http://192.168.4.1 |

## Web Interface

### Model Selection

Files ending in `_1` show the model selection menu.

![SIW400G selection](../../resources/ReadSerialTwoInverters/400G_1.jpeg)

![SIW500H selection](../../resources/ReadSerialTwoInverters/500H_1.jpeg)

### Serial Number Result

Files ending in `_2` show the serial number result.

![SIW400G serial result](../../resources/ReadSerialTwoInverters/400G_2.jpeg)

![SIW500H serial result](../../resources/ReadSerialTwoInverters/500H_2.jpeg)

## Modbus Configuration

The example uses one `ModbusRTU` object and two inverter objects on the same RS485 bus.

```cpp
ModbusRTU mb;

ModbusConfigData mbCfg400G = {2, 9600, SERIAL_8N1, DE_RE_PIN};
ModbusConfigData mbCfg500H = {1, 9600, SERIAL_8N1};
```

`ModbusConfigData` format:

```cpp
{ slaveId, baudRate, serialConfig, deRePin }
```

## Important Notes

On ESP8266, the main UART may be used for RS485 communication. Avoid using Serial.print() for debugging on the same UART while Modbus communication is active.

Some Modbus registers may vary depending on firmware version, manufacturer documentation, or inverter variant. Always validate the Modbus map with real equipment.

### Main Functions Used

```cpp
attachModbus()
attachConfig()
begin()
getSerialNumber()

```

### Folder Structure

```bash
resources/
└── ReadSerialTwoInverters/
    ├── 400G_1.jpeg
    ├── 400G_2.jpeg
    ├── 500H_1.jpeg
    └── 500H_2.jpeg
```