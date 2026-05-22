# InverterModbusLib

[Português](README.pt-BR.md)

Arduino library for Modbus RTU communication with photovoltaic inverters using ESP8266 and ESP32.

## About the Project

Parts of the architecture, documentation and examples of this library were developed with the assistance of artificial intelligence tools to speed up prototyping, review and code organization.

This library aims to standardize Modbus communication with photovoltaic inverters from different manufacturers by providing a single, consistent and reusable interface.

The goal is to allow developers to interact with inverters without needing to know the specific details of each Modbus map.

---

## Project Status

This project is currently in **alpha** stage.

The Modbus communication core is already functional, but inverter maps are still being validated in practice with real equipment.

### 🎯 Goals

- Provide a simple API for reading data and basic power control with real equipment
- Standardize data reading and writing through Modbus
- Support multiple manufacturers and models
- Make integration with embedded systems easier
- Reduce rework when implementing support for new inverters

---

## Features

- Modbus RTU communication over RS485
- ESP8266 and ESP32 compatibility
- Dynamic UART/Modbus configuration
- Support for multiple inverters on the same bus
- Modbus maps separated by model/family
- Object-oriented API
- Arduino IDE examples
- Web panel example for testing and validation
- Structure prepared for future expansion

---

## Compatibility

| Model | Status | Notes |
|---|---|---|
| SIW200 M030 | Partially validated | Time data uses GoodWe-compatible handling |
| SIW200 M050 | Partially validated | Time data uses GoodWe-compatible handling |
| SIW200 M100 | Partially validated | Time data uses GoodWe-compatible handling |
| SIW400G T100 W0 | Partially validated | Most readings are working |
| SIW500H ST030 M3 | Partially validated | Power/export limitation working |
| SIW500H ST015 M0 | Partially validated | Power/export limitation working |
| SIW420G K075 W00 | Map found | Validation pending |
| SIW200H M050 W00 | Pending | Map may be different from SIW400G |
| SIW400H T030 W10 | Pending | Inconclusive test |
| GW3000N-XS | Partially validated | Time data uses GoodWe special handling, total energy register reading |
| GW5000-DNS | Partially validated | Time data uses GoodWe special handling, total energy register reading |
| GW10K-MS | Partially validated | Time data uses GoodWe special handling, total energy register reading |
| R100 | Partially validated | Most readings are working |
| H3-PRO | Pending | Inconclusive test |
| SUN2000-30KTL-M3 | Partially validated | Power/export limitation working |
| SUN2000-15KTL-M0 | Partially validated | Power/export limitation working |

Models from the same family may share the same Modbus map, but only physically tested models should be considered validated.

---

## Documentation

Detailed documentation is available in the `documentation` folder.

- [Public API](documentation/API.md)
- [Compatibility and validation](documentation/COMPATIBILITY.md)
- [Roadmap](documentation/ROADMAP.md)

> Some documents may still be incomplete during the alpha stage.

---

## Installation

### Arduino IDE

Copy the library folder to the Arduino libraries directory:

```text
Documents/Arduino/libraries/InverterModbusLib
```

Then restart Arduino IDE and include the library in your sketch:

```cpp
#include <InverterModbusLib.h>
```

---

## Known Limitations and Behaviors

Some inverters have manufacturer-specific behaviors, such as different offsets, special fields, control enums, custom date/time formats and partially implemented registers.

See:

- [Known behaviors](documentation/KNOWN_BEHAVIORS.md)
- [Compatibility](documentation/COMPATIBILITY.md)
- [Validation status](documentation/VALIDATION.md)

---

## 🤝 Collaboration with Manufacturers

This project seeks direct collaboration with inverter manufacturers.

If you are a manufacturer or technical representative, your contribution is extremely valuable.

We are interested in:

- Official Modbus map documentation
- Differences between models and firmware versions
- Best practices for reading/writing data
- Scale and encoding specifications

---

## 📩 Contact

If you want to collaborate or provide documentation:

- Open an issue
- Contact directly

---

## 📦 Dependencies

This library uses the ModbusRTU library developed by Emelianov, licensed under the BSD 3-Clause License.

---