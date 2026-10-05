# Host regressions: asynchronous power limits

Run from the repository root with a C++17 compiler:

```sh
bash tests/host/run.sh
```

The tests compile the production bus, control, device-info code and the actual
WEG maps/descriptors. Only Arduino and the Modbus transport are substitutes;
callbacks are delivered explicitly by each scenario. Undefined-behavior and
float-to-integer overflow sanitizers are enabled.

They cover rejection without stale command retention, ownership until the result
is consumed, function 06/16 encoding, SIW400G enable-before-setpoint, conversion
using measured rated power, timeout/start-failure recovery and invalid numeric
inputs. An isolated malformed-map fixture exercises numeric encoding boundaries.

These checks do not establish RS485 timing, serial-driver behavior, response echo
validation, flash/Wi-Fi responsiveness or actual inverter power reduction. Those
remain field tests using the SolarView firmware and its downloadable log.
