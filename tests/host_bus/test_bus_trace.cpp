#include "InverterModbusBus.h"

#include <assert.h>
#include <stdio.h>
#include <vector>

uint32_t hostMillis = 0;

// The production bus exposes transaction starts only to its Inverter friend.
// This harness isolates the bus instead of compiling unrelated inverter code.
class Inverter {
public:
    bool read(InverterModbusBus& bus, InverterRequestId request = REQ_ACTIVE_POWER,
              uint8_t id = 3, uint16_t address = 0x1000, uint16_t count = 2) {
        return bus.startRead(this, request, id, address, count);
    }
    bool write(InverterModbusBus& bus, const uint16_t* words, uint16_t count,
               InverterRequestId request = REQ_SET_POWER_LIMIT_PERCENT,
               uint8_t id = 2, uint16_t address = 0x9D6C) {
        return bus.startWrite(this, request, id, address, words, count);
    }
    void release(InverterModbusBus& bus) { bus.release(); }
};

struct TraceSink {
    std::vector<InverterTransactionTrace> traces;
    InverterModbusBus* bus;
    Inverter* owner;
};

static void collect(const InverterTransactionTrace& trace, void* context) {
    TraceSink& sink = *static_cast<TraceSink*>(context);
    // The lease must remain valid even while diagnostics are being delivered.
    assert(sink.bus->belongsTo(sink.owner, trace.requestId));
    assert(sink.bus->isBusy());
    if (trace.status == INV_MB_SUCCESS) {
        assert(sink.bus->isCompletedFor(sink.owner, trace.requestId));
    } else {
        assert(sink.bus->isFailedFor(sink.owner, trace.requestId));
    }
    sink.traces.push_back(trace);
}

struct Fixture {
    ModbusRTU mb;
    HardwareSerial serial;
    InverterModbusBus bus;
    Inverter owner;
    TraceSink sink;
    Fixture() : bus(mb), sink{{}, &bus, &owner} {
        assert(bus.begin(serial, 9600, SERIAL_8N1, 12));
        bus.setTraceCallback(collect, &sink);
    }
    void finish(Modbus::ResultCode result, uint32_t elapsed = 7) {
        hostMillis += elapsed;
        mb.result = result;
        mb.ready = true;
        bus.task();
    }
};

static void read_success_and_lease() {
    Fixture f;
    hostMillis = 100;
    assert(f.owner.read(f.bus));
    assert(f.sink.traces.empty());
    f.bus.task();
    assert(f.sink.traces.empty());
    assert(f.bus.belongsTo(&f.owner, REQ_ACTIVE_POWER));
    assert(!f.bus.belongsTo(&f.owner, REQ_GRID_FREQUENCY));
    f.mb.response[0] = 0x1234;
    f.mb.response[1] = 0xABCD;
    f.finish(Modbus::EX_SUCCESS, 20);
    assert(f.sink.traces.size() == 1);
    const auto& t = f.sink.traces[0];
    assert(t.requestId == REQ_ACTIVE_POWER && t.slaveId == 3);
    assert(t.functionCode == 3 && t.address == 0x1000 && t.registerCount == 2);
    assert(t.words[0] == 0x1234 && t.words[1] == 0xABCD && t.words[2] == 0);
    assert(t.status == INV_MB_SUCCESS && t.resultCode == Modbus::EX_SUCCESS);
    assert(t.startedMs == 100 && t.durationMs == 20);
    assert(t.accepted && t.resultCodeValid && t.payloadValid);
    assert(f.bus.buffer()[0] == 0x1234 && f.bus.registerCount() == 2);
    Inverter other;
    assert(!other.read(f.bus));
    assert(!f.owner.read(f.bus, REQ_GRID_FREQUENCY));
    f.bus.task();
    f.bus.task();
    assert(f.sink.traces.size() == 1);
    assert(f.bus.isCompletedFor(&f.owner, REQ_ACTIVE_POWER));
    f.owner.release(f.bus);
    assert(!f.bus.isBusy());
    assert(!f.bus.belongsTo(&f.owner, REQ_ACTIVE_POWER));
    assert(f.bus.transactionStatus() == INV_MB_NONE);
}

static void write_function_selection_and_error_payload() {
    Fixture f;
    uint16_t single[] = {900};
    assert(f.owner.write(f.bus, single, 1));
    single[0] = 1;
    assert(f.mb.lastFunction == 6 && f.mb.sent[0] == 900);
    f.finish(Modbus::EX_SUCCESS);
    assert(f.sink.traces[0].functionCode == 6);
    assert(f.sink.traces[0].words[0] == 900);
    f.owner.release(f.bus);

    uint16_t multiple[] = {0x1234, 0x5678};
    assert(f.owner.write(f.bus, multiple, 2, REQ_SET_POWER_LIMIT, 1, 0x2000));
    assert(f.mb.lastFunction == 16 && f.mb.sent[1] == 0x5678);
    f.finish(Modbus::EX_ILLEGAL_ADDRESS);
    const auto& t = f.sink.traces[1];
    assert(t.functionCode == 16 && t.slaveId == 1 && t.address == 0x2000);
    assert(t.requestId == REQ_SET_POWER_LIMIT && t.registerCount == 2);
    assert(t.words[0] == 0x1234 && t.words[1] == 0x5678 && t.words[2] == 0);
    assert(t.accepted && t.payloadValid && t.resultCodeValid);
    assert(t.status == INV_MB_ILLEGAL_ADDRESS && t.resultCode == Modbus::EX_ILLEGAL_ADDRESS);
    assert(f.bus.isFailedFor(&f.owner, REQ_SET_POWER_LIMIT));
    assert(!f.owner.write(f.bus, single, 1));
    f.bus.task();
    assert(f.sink.traces.size() == 2);
    f.owner.release(f.bus);
}

static void timeout_invalidates_read_payload() {
    Fixture f;
    hostMillis = 1000;
    assert(f.owner.read(f.bus));
    f.mb.response[0] = 0xCAFE;
    f.mb.response[1] = 0xBEEF;
    f.finish(Modbus::EX_TIMEOUT, 1000);
    const auto& t = f.sink.traces[0];
    assert(t.status == INV_MB_TIMEOUT && t.resultCode == Modbus::EX_TIMEOUT);
    assert(t.accepted && t.resultCodeValid && !t.payloadValid);
    for (uint16_t word : t.words) assert(word == 0);
    assert(t.durationMs == 1000);
    assert(f.bus.isFailedFor(&f.owner, REQ_ACTIVE_POWER));
    f.owner.release(f.bus);
}

static void enqueue_refusal_is_not_a_modbus_response() {
    Fixture f;
    f.mb.accept = false;
    assert(!f.owner.read(f.bus));
    assert(f.sink.traces.size() == 1);
    auto t = f.sink.traces.back();
    assert(!t.accepted && !t.resultCodeValid && !t.payloadValid);
    assert(t.status == INV_MB_GENERAL_FAILURE && t.durationMs == 0);
    assert(f.bus.isFailedFor(&f.owner, REQ_ACTIVE_POWER));
    f.bus.task();
    assert(f.sink.traces.size() == 1);
    f.owner.release(f.bus);

    uint16_t words[] = {1000};
    assert(!f.owner.write(f.bus, words, 1));
    assert(f.sink.traces.size() == 2);
    t = f.sink.traces.back();
    assert(!t.accepted && !t.resultCodeValid && t.payloadValid);
    assert(t.functionCode == 6 && t.words[0] == 1000);
    assert(f.bus.isFailedFor(&f.owner, REQ_SET_POWER_LIMIT_PERCENT));
    f.owner.release(f.bus);
    f.mb.accept = true;
    assert(f.owner.read(f.bus));
    f.finish(Modbus::EX_SUCCESS);
    assert(f.sink.traces.size() == 3);
    f.owner.release(f.bus);
}

static void independent_buses() {
    Fixture a;
    Fixture b;
    uint16_t words[] = {750};
    assert(a.owner.read(a.bus, REQ_GRID_FREQUENCY, 1, 0x3000, 1));
    assert(b.owner.write(b.bus, words, 1));
    b.finish(Modbus::EX_SLAVE_FAILURE);
    assert(a.sink.traces.empty() && b.sink.traces.size() == 1);
    assert(!a.bus.isCompletedFor(&a.owner, REQ_GRID_FREQUENCY));
    assert(b.sink.traces[0].status == INV_MB_SLAVE_FAILURE);
    a.mb.response[0] = 6000;
    a.finish(Modbus::EX_SUCCESS);
    assert(a.sink.traces.size() == 1 && a.sink.traces[0].words[0] == 6000);
    assert(b.bus.isFailedFor(&b.owner, REQ_SET_POWER_LIMIT_PERCENT));
    a.owner.release(a.bus);
    b.owner.release(b.bus);
}

static void rejected_preconditions_do_not_trace() {
    Fixture f;
    uint16_t words[] = {1, 2};
    assert(!f.owner.write(f.bus, nullptr, 1));
    assert(!f.owner.write(f.bus, words, 0));
    assert(!f.owner.write(f.bus, words, INV_ASYNC_BUFFER_REGS + 1));
    assert(!f.owner.write(f.bus, words, 2, REQ_SET_POWER_LIMIT, 1, 0xFFFF));
    assert(!f.owner.read(f.bus, REQ_NONE));
    assert(!f.owner.read(f.bus, REQ_ACTIVE_POWER, 1, 1, 0));
    assert(!f.owner.read(f.bus, REQ_ACTIVE_POWER, 1, 1, INV_ASYNC_BUFFER_REGS + 1));
    assert(f.sink.traces.empty() && f.mb.calls == 0 && !f.bus.isBusy());
    assert(f.owner.write(f.bus, words, 2));
    assert(!f.owner.read(f.bus));
    assert(f.mb.calls == 1);
    f.finish(Modbus::EX_SUCCESS);
    assert(f.sink.traces.size() == 1);
    f.owner.release(f.bus);
}

static void optional_callback_and_clock_wrap() {
    Fixture f;
    f.bus.setTraceCallback(nullptr);
    assert(f.owner.read(f.bus));
    f.finish(Modbus::EX_SUCCESS);
    assert(f.sink.traces.empty());
    f.owner.release(f.bus);

    f.bus.setTraceCallback(collect, &f.sink);
    hostMillis = 0xFFFFFFF0u;
    assert(f.owner.read(f.bus));
    f.finish(Modbus::EX_SUCCESS, 32);
    assert(f.sink.traces[0].startedMs == 0xFFFFFFF0u);
    assert(f.sink.traces[0].durationMs == 32);
    f.owner.release(f.bus);
}

int main() {
    read_success_and_lease();
    write_function_selection_and_error_payload();
    timeout_invalidates_read_payload();
    enqueue_refusal_is_not_a_modbus_response();
    independent_buses();
    rejected_preconditions_do_not_trace();
    optional_callback_and_clock_wrap();
    puts("PASS: 7 bus trace scenarios (C++11 host; mocked ModbusRTU)");
}
