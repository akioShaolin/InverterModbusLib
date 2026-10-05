// Compile the real firmware implementation, not a second copy of its scheduler.
#include "../../src/main.cpp"
#include <cassert>
#include <iostream>

HardwareSerial Serial;
uint32_t hostMillis = 0;
FakeEsp ESP;
FakeWiFi WiFi;
FakeFS LittleFS;
const InverterDescriptor getDescriptor(InverterModel model) { return getDescriptor_Weg(model); }
bool getInverterMap(InverterModel model, ModbusInverterMap& out) { return getMap_Weg(model, out); }

static void args(std::initializer_list<std::pair<const std::string, String>> values) {
    webServer.args = values; webServer.args["token"] = bootToken; webServer.responseCode = 0;
}
static void drain() { for (int i = 0; i < 12 && testLog.queued(); ++i) testLog.service(true); }
static void start() {
    args({{"label", "host,\"case\""}, {"utc", "2026-10-05T18:00:00.000Z"}});
    handleStart(); assert(webServer.responseCode == 201); assert(!armed);
    assert(testLog.queued() == 7); assert(!paused); drain();
    args({{"enable", "1"}}); handleArm(); assert(armed); drain();
    devices[0].hasRated = true; devices[0].rated = 30000;
    devices[1].hasRated = true; devices[1].rated = 100000;
}
static void queue(uint8_t index, const char* value, uint32_t nonce) {
    args({{"inv", String(index)}, {"percent", value}, {"nonce", String(nonce)}});
    handleCommand(OP_PERCENT);
}
static void finishStop() {
    requestSessionStop("host_stop");
    for (int i = 0; i < 20; ++i) { stoppingTask(); testLog.service(true); stoppingTask(); }
    assert(stoppedAndQuiet());
}

static void csvAndNumberParsing() {
    float value;
    const char* invalid[] = {"", "nan", "inf", "-inf", "90junk", "90,1", " 90", "90 ", "1e99"};
    for (const char* input : invalid) assert(!parseNumber(String(input), value));
    assert(parseNumber("90.5", value) && value == 90.5f);
    uint32_t integer;
    assert(!parseUnsigned("-1", integer)); assert(!parseUnsigned("4294967296", integer));
    CsvRow row; row.text(" =1+1"); row.text("a,\"b\"\nline"); row.real(-2.5);
    assert(std::string(row.buffer) == "\"' =1+1\",\"a,\"\"b\"\" line\",-2.5");
    CsvRow huge; huge.text(std::string(700, 'a').c_str()); assert(!huge.valid);
}
static void bootAndDuplicateAndGracefulStop() {
    assert(paused && quiet() && !armed && mb.sent.empty());
    queue(0, "90", nextCommandId); assert(webServer.responseCode == 409 && mb.sent.empty());
    start();
    // Prevent new telemetry jobs while selecting explicit read for concurrency.
    activeJob = {}; activeJob.operation = OP_FREQUENCY; activeJob.device = 0;
    activeJob.startedAt = millis(); advanceActiveJob(); assert(modbusBus.isBusy());
    const uint32_t calls = LittleFS.ioCalls;
    testLog.service(false); assert(LittleFS.ioCalls == calls);
    const uint32_t nonce = nextCommandId;
    queue(0, "90", nonce); assert(webServer.responseCode == 202);
    queue(0, "10", nonce); assert(webServer.responseCode == 409);
    assert(pendingCommand.value == 90 && pendingCommand.id == nonce);
    args({}); handleStop(); assert(paused && stopping && !armed && pendingCommand.valid);
    handleDownload(); assert(webServer.responseCode == 409);
    mb.respond(Modbus::EX_SUCCESS, {6000}); modbusBus.task(); schedulerTask();
    assert(activeJob.operation == OP_NONE && pendingCommand.valid);
    drain(); hostMillis += 60; schedulerTask(); assert(activeJob.operation == OP_PERCENT);
    assert(mb.sent.back().values == std::vector<uint16_t>({900}));
    mb.respond(Modbus::EX_SUCCESS); modbusBus.task(); schedulerTask();
    assert(std::string(lastCommandStatus) == "DONE"); finishStop();
    const std::string csv = LittleFS.files.at(testLog.currentName())->data;
    assert(csv.find("CMD_QUEUED") < csv.find("CMD_START"));
    assert(csv.find("CMD_START") < csv.find("CMD_RESULT"));
    assert(csv.find("CMD_RESULT") < csv.find("SESSION_STOP"));
    assert(csv.find("0384") != std::string::npos);
    handleStatus(); assert(webServer.responseCode == 200);
    std::cout << "STATUS_JSON=" << webServer.response << '\n';
    std::cout << "CSV_BEGIN\n" << csv << "CSV_END\n";
}
static void metadataQueueIsNotDiskFull() {
    args({{"label", "metadata"}, {"utc", "2026-10-05T18:01:00.000Z"}});
    handleStart(); assert(webServer.responseCode == 201 && testLog.queued() == 7);
    loop(); assert(!paused && !stopping); drain(); finishStop();
}
static void ioFailureCancelsUnstartedWrite() {
    start();
    const size_t before = mb.sent.size();
    queue(0, "90", nextCommandId); assert(webServer.responseCode == 202);
    LittleFS.writeLimit = 0;
    testLog.service(true); assert(testLog.state() == FieldTestLog::FAULT);
    hostMillis += 60; schedulerTask();
    assert(!pendingCommand.valid && activeJob.operation == OP_NONE);
    assert(std::string(lastCommandStatus) == "CANCELLED_NO_LOG");
    assert(lastCommandModbus == INV_MB_NONE && mb.sent.size() == before);
    LittleFS.writeLimit = std::numeric_limits<size_t>::max(); finishStop();
}
static void noteStormReservesCommandResultSpace() {
    start(); queue(1, "90", nextCommandId); assert(webServer.responseCode == 202);
    drain(); hostMillis += 60; schedulerTask(); assert(activeJob.operation == OP_PERCENT);
    for (int i = 0; i < 20; ++i) {
        args({{"note", "storm"}}); handleNote();
        args({{"enable", "1"}}); handleArm();
    }
    assert(testLog.queued() <= 4);
    const uint32_t fsBefore = LittleFS.ioCalls;
    mb.respond(Modbus::EX_SUCCESS); modbusBus.task(); schedulerTask();
    assert(activeJob.operation == OP_PERCENT); // enable completed; setpoint running.
    mb.respond(Modbus::EX_SUCCESS); modbusBus.task(); schedulerTask();
    assert(LittleFS.ioCalls == fsBefore); // callbacks and result only enqueue RAM.
    assert(std::string(lastCommandStatus) == "DONE" && testLog.dropped() == 0);
    finishStop();
}
int main() {
    setup(); csvAndNumberParsing(); bootAndDuplicateAndGracefulStop();
    metadataQueueIsNotDiskFull(); ioFailureCancelsUnstartedWrite(); noteStormReservesCommandResultSpace();
    std::puts("PASS: application parsers, CSV, queue/nonce, graceful stop, no-log cancellation and queue-pressure scenarios.");
}
