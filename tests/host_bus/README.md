# Bus transaction trace host tests

Run with a C++11 compiler:

```sh
sh tests/host_bus/run.sh
```

Compiles the real `src/InverterModbusBus.cpp` with isolated Arduino/ModbusRTU
stubs. Checks terminal trace delivery, FC03 read payload, FC06/FC16 selection
and write payload, owner/request retention until consumption, error and timeout
reporting, enqueue refusal without a fabricated Modbus result, two independent
buses, invalid/busy start rejection, disabled callback, and `millis()` rollover.

The stub supplies timeout completion as the Modbus library would: these tests
do not validate actual UART timings, CRCs, hardware, or timeout behavior inside
the external Modbus library. `setTraceCallback` is an optional diagnostic hook;
the callback must copy the temporary event to RAM and must not perform flash,
network I/O, or re-enter the bus. Calls rejected before attempting to enqueue
(invalid arguments, busy bus) do not generate a transaction trace.
