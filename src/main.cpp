/*
 * SolarView field test 01.1: the six migrated asynchronous APIs only.
 * Writes are operator-queued, never sent at boot or automatically restored.
 * RS485 is exclusively Serial 9600 8N1, DE/RE GPIO12, switch GPIO13 LOW.
 * HTTP and LittleFS remain synchronous services; their impact is measured.
 */
#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ModbusRTU.h>
#include <InverterModbusLib.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
extern "C" {
#include <user_interface.h>
}
#include "FieldTestLog.h"
#include "FieldTestPage.h"

static constexpr uint8_t LED_PIN = 2;
static constexpr uint8_t DE_RE_PIN = 12;
static constexpr uint8_t PIN_RS485_SWITCH = 13;
static constexpr uint32_t MODBUS_BAUD = 9600;
static constexpr uint32_t READ_INTERVAL_MS = 3000;
static constexpr uint32_t STALE_MS = 15000;
static constexpr uint32_t INTER_REQUEST_GAP_MS = 50;
static const char BUILD_ID[] = "solarview-async-01.1";
static const char BASE_COMMIT[] = "eaa6006f";
static const char AP_SSID[] = "InverterModbus-Test";
static const char AP_PASSWORD[] = "modbus123";
static const char CSV_HEADER[] =
    "schema,session,seq,uptime_ms,event,command_id,slave_id,model,operation,status,"
    "modbus_status,function_code,address,register_count,words_hex,value,unit,elapsed_ms,source,detail";

ModbusRTU mb;
InverterModbusBus modbusBus(mb);
Inverter inverter1(SIW500H_ST030_M3);
Inverter inverter2(SIW400G_T100_W0);
Inverter inverter3(SIW400G_T100_W0);
ESP8266WebServer webServer(80);
FieldTestLog testLog;

enum Operation : uint8_t { OP_NONE, OP_RATED, OP_SERIAL, OP_FREQUENCY, OP_POWER, OP_WATTS, OP_PERCENT };
static const Operation READ_OPERATIONS[] = {OP_RATED, OP_SERIAL, OP_FREQUENCY, OP_POWER};

struct Device {
    Inverter* inverter;
    const char* model;
    uint8_t id;
    bool enabled = true;
    bool ready = false;
    float frequency = 0, power = 0, rated = 0;
    bool hasFrequency = false, hasPower = false, hasRated = false, hasSerial = false;
    bool ratedFromSpec = false;
    char serial[33]{};
    uint32_t frequencyAt = 0, powerAt = 0;
    uint32_t triedAt[4]{};
    bool tried[4]{};
    uint32_t ok = 0, errors = 0, timeouts = 0, rejected = 0;
    InverterRequestStatus status = INV_IDLE;
    InverterModbusStatus modbus = INV_MB_NONE;
};
Device devices[] = {
    {&inverter1, "SIW500H_ST030_M3", 1},
    {&inverter2, "SIW400G_T100_W0", 2},
    {&inverter3, "SIW400G_T100_W0", 3}
};
static constexpr uint8_t DEVICE_COUNT = sizeof(devices) / sizeof(devices[0]);

struct Command {
    bool valid = false;
    uint32_t id = 0;
    uint8_t device = 0;
    Operation operation = OP_NONE;
    float value = 0;
};
struct Job {
    Operation operation = OP_NONE;
    uint8_t device = 0;
    uint32_t commandId = 0, startedAt = 0;
    float requested = 0, value = 0;
    char serial[33]{};
    bool hadTransaction = false;
    InverterModbusStatus lastTransaction = INV_MB_NONE;
};
Command pendingCommand;
Job activeJob;
Command lastCommand;
const char* lastCommandStatus = "NONE";
InverterModbusStatus lastCommandModbus = INV_MB_NONE;

bool busReady = false, paused = true, stopping = false, armed = false;
bool stopLogged = false, flashSafe = false;
char stopReason[96]{}, bootToken[33]{}, lastTrace[224]{};
uint32_t nextCommandId = 1, logSequence = 0;
uint32_t lastBlinkMs = 0, nextRequestAt = 0, lastHealthMs = 0;
uint32_t lastLoopUs = 0, lastBusUs = 0, maxLoopGapUs = 0, maxBusGapUs = 0, maxApiUs = 0;
uint32_t flashReal = 0, flashConfig = 0;
uint8_t readCursor = 0;
bool ledOn = false;

static const char* operationName(Operation op) {
    switch (op) {
        case OP_RATED: return "getRatedPower";
        case OP_SERIAL: return "getSerialNumber";
        case OP_FREQUENCY: return "getGridFrequency";
        case OP_POWER: return "getActivePower";
        case OP_WATTS: return "setPowerLimit";
        case OP_PERCENT: return "setPowerLimitPercent";
        default: return "NONE";
    }
}
static const char* requestName(InverterRequestStatus status) {
    switch (status) {
        case INV_BUSY: return "BUSY";
        case INV_DONE: return "DONE";
        case INV_ERROR: return "ERROR";
        case INV_REJECTED: return "REJECTED";
        default: return "IDLE";
    }
}
static const char* modbusName(InverterModbusStatus status) {
    switch (status) {
        case INV_MB_SUCCESS: return "SUCCESS";
        case INV_MB_TIMEOUT: return "TIMEOUT";
        case INV_MB_ILLEGAL_FUNCTION: return "ILLEGAL_FUNCTION";
        case INV_MB_ILLEGAL_ADDRESS: return "ILLEGAL_ADDRESS";
        case INV_MB_ILLEGAL_VALUE: return "ILLEGAL_VALUE";
        case INV_MB_SLAVE_FAILURE: return "SLAVE_FAILURE";
        case INV_MB_UNEXPECTED_RESPONSE: return "UNEXPECTED_RESPONSE";
        case INV_MB_GENERAL_FAILURE: return "GENERAL_FAILURE";
        default: return "NONE";
    }
}
static bool isCommand(Operation op) { return op == OP_WATTS || op == OP_PERCENT; }
static bool quiet() {
    return activeJob.operation == OP_NONE && !pendingCommand.valid && !modbusBus.isBusy();
}
static bool stoppedAndQuiet() { return paused && !stopping && quiet() && !testLog.active(); }
static void requestSessionStop(const char* reason) {
    armed = false;
    paused = true;
    if (!stopping) {
        stopping = true;
        stopLogged = false;
        snprintf(stopReason, sizeof(stopReason), "%s", reason);
    }
}

// All CSV rows have exactly 20 columns. Text is quoted, multiline is flattened,
// and spreadsheet-formula prefixes are neutralized; numeric fields stay numeric.
class CsvRow {
public:
    char buffer[FieldTestLog::MAX_LINE_BYTES]{};
    bool valid = true;
    void empty() { separator(); }
    void number(uint32_t n) {
        separator(); char value[16]; snprintf(value, sizeof(value), "%lu", (unsigned long)n); raw(value);
    }
    void real(float n) {
        if (!isfinite(n)) { empty(); return; }
        separator(); char value[32]; snprintf(value, sizeof(value), "%.7g", (double)n); raw(value);
    }
    void text(const char* value) {
        separator(); put('"');
        if (value) {
            const char* significant = value;
            while (*significant == ' ' || *significant == '\t' || *significant == '\r' || *significant == '\n') ++significant;
            if (*significant == '=' || *significant == '+' || *significant == '-' || *significant == '@') put('\'');
            for (const unsigned char* p = reinterpret_cast<const unsigned char*>(value); *p; ++p) {
                char ch = *p < 32 || *p == 127 ? ' ' : static_cast<char>(*p);
                if (ch == '"') put('"');
                put(ch);
            }
        }
        put('"');
    }
private:
    size_t length = 0;
    uint8_t columns = 0;
    void put(char ch) {
        if (length >= sizeof(buffer) - 2) { valid = false; return; }
        buffer[length++] = ch; buffer[length] = '\0';
    }
    void raw(const char* value) { while (*value) put(*value++); }
    void separator() { if (columns++) put(','); }
};
struct Event {
    const char* name;
    int8_t device = -1;
    Operation operation = OP_NONE;
    const char* status = "";
    InverterModbusStatus modbus = INV_MB_NONE;
    uint32_t commandId = 0;
    const InverterTransactionTrace* trace = nullptr;
    bool hasValue = false;
    float value = 0;
    const char* unit = "";
    bool hasElapsed = false;
    uint32_t elapsed = 0;
    const char* source = "application";
    const char* detail = "";
};
static bool emitEvent(const Event& event, bool critical = false) {
    // Preserve the logger's original I/O error instead of replacing it with
    // hundreds of follow-up "no session" errors after an SD/flash failure.
    if (testLog.state() != FieldTestLog::RECORDING && testLog.state() != FieldTestLog::STOPPING) return false;
    CsvRow row;
    row.text("solarview.test.v1"); row.text(testLog.currentName()); row.number(++logSequence);
    row.number(millis()); row.text(event.name);
    if (event.commandId) row.number(event.commandId); else row.empty();
    if (event.device >= 0 && event.device < DEVICE_COUNT) {
        row.number(devices[event.device].id); row.text(devices[event.device].model);
    } else { row.empty(); row.empty(); }
    row.text(operationName(event.operation)); row.text(event.status); row.text(modbusName(event.modbus));
    if (event.trace) {
        const InverterTransactionTrace& trace = *event.trace;
        row.number(trace.functionCode);
        char address[12]; snprintf(address, sizeof(address), "0x%04X", trace.address); row.text(address);
        row.number(trace.registerCount);
        char words[INV_ASYNC_BUFFER_REGS * 5 + 1]{};
        if (trace.payloadValid) {
            size_t used = 0;
            for (uint16_t i = 0; i < trace.registerCount && i < INV_ASYNC_BUFFER_REGS; ++i)
                used += snprintf(words + used, sizeof(words) - used, "%s%04X", i ? " " : "", trace.words[i]);
            row.text(words);
        } else row.empty();
    } else { row.empty(); row.empty(); row.empty(); row.empty(); }
    if (event.hasValue) row.real(event.value); else row.empty();
    row.text(event.unit);
    if (event.hasElapsed) row.number(event.elapsed); else row.empty();
    row.text(event.source); row.text(event.detail);
    const bool accepted = testLog.appendCsvLine(row.valid ? row.buffer : nullptr, critical);
    if (!accepted) requestSessionStop("log_row_not_recorded");
    return accepted;
}
static int8_t deviceForSlave(uint8_t id) {
    for (uint8_t i = 0; i < DEVICE_COUNT; ++i) if (devices[i].id == id) return i;
    return -1;
}
static void onTransaction(const InverterTransactionTrace& trace, void*) {
    // No filesystem, network I/O, bus re-entry or dynamic allocation here.
    activeJob.hadTransaction = true;
    activeJob.lastTransaction = trace.status;
    char detail[176];
    snprintf(detail, sizeof(detail),
        "accepted=%u;result_code_valid=%u;result_code=0x%02X;payload_valid=%u;words=%s;started_ms=%lu",
        trace.accepted, trace.resultCodeValid, trace.resultCode, trace.payloadValid,
        trace.functionCode == 3 ? "RX_registers" : "TX_requested_registers", (unsigned long)trace.startedMs);
    Event event{"TRANSACTION"};
    event.device = deviceForSlave(trace.slaveId); event.operation = activeJob.operation;
    event.commandId = activeJob.commandId; event.trace = &trace;
    event.status = !trace.accepted ? "START_REFUSED" : trace.status == INV_MB_SUCCESS ? "DONE" : "ERROR";
    event.modbus = trace.status; event.hasElapsed = true; event.elapsed = trace.durationMs;
    event.source = "modbus_callback"; event.detail = detail;
    emitEvent(event, isCommand(activeJob.operation));
    snprintf(lastTrace, sizeof(lastTrace), "ID %u | %s | FC%02u | 0x%04X | %u regs | %s | %lu ms",
        trace.slaveId, operationName(activeJob.operation), trace.functionCode, trace.address,
        trace.registerCount, modbusName(trace.status), (unsigned long)trace.durationMs);
}

static String jsonText(const char* value) {
    String out; out.reserve(value ? strlen(value) + 4 : 4); out += '"';
    if (value) for (const unsigned char* p = reinterpret_cast<const unsigned char*>(value); *p; ++p) {
        if (*p == '"' || *p == '\\') { out += '\\'; out += static_cast<char>(*p); }
        else if (*p < 32) { char escape[7]; snprintf(escape, sizeof(escape), "\\u%04x", *p); out += escape; }
        else out += static_cast<char>(*p);
    }
    out += '"'; return out;
}
static String jsonFloat(float value, bool present) { return present && isfinite(value) ? String(value, 3) : String("null"); }
static const char* jsonBool(bool value) { return value ? "true" : "false"; }
static void reply(int status, const char* message) {
    webServer.sendHeader("Cache-Control", "no-store");
    webServer.send(status, "application/json; charset=utf-8", String("{\"message\":") + jsonText(message) + "}");
}
static bool authorized() {
    if (!webServer.hasArg("token") || webServer.arg("token") != bootToken) {
        reply(403, "Token invalido. Atualize a pagina deste dispositivo."); return false;
    }
    return true;
}
static bool parseUnsigned(const String& text, uint32_t& value) {
    if (!text.length() || text.length() > 10) return false;
    uint32_t result = 0;
    for (size_t i = 0; i < text.length(); ++i) {
        const char c = text[i];
        if (c < '0' || c > '9') return false;
        const uint32_t digit = c - '0';
        if (result > (UINT32_MAX - digit) / 10U) return false;
        result = result * 10U + digit;
    }
    value = result; return true;
}
static bool parseNumber(const String& text, float& value) {
    if (!text.length() || text.length() > 32) return false;
    // Do not silently accept empty input, trailing junk, commas or whitespace.
    for (size_t i = 0; i < text.length(); ++i) if (static_cast<unsigned char>(text[i]) <= 32) return false;
    errno = 0; char* end = nullptr;
    const float result = strtof(text.c_str(), &end);
    if (errno == ERANGE || end == text.c_str() || *end || !isfinite(result)) return false;
    value = result; return true;
}
static bool parseDevice(uint8_t& index) {
    uint32_t value;
    if (!parseUnsigned(webServer.arg("inv"), value) || value >= DEVICE_COUNT) {
        reply(400, "Inversor invalido: use indice 0, 1 ou 2."); return false;
    }
    index = value; return true;
}
static void noteQueueRejection(uint8_t index, Operation op, float value, const char* why) {
    ++devices[index].rejected;
    if (testLog.state() != FieldTestLog::RECORDING || testLog.queued() > 3) return;
    Event event{"QUEUE_REJECTED"}; event.device = index; event.operation = op;
    event.status = "REJECTED"; event.hasValue = isfinite(value); event.value = value;
    event.unit = op == OP_PERCENT ? "%" : "W"; event.detail = why;
    emitEvent(event);
}
static void handleCommand(Operation operation) {
    if (!authorized()) return;
    uint8_t index = 0;
    if (!parseDevice(index)) return;
    float value = NAN;
    if (!parseNumber(webServer.arg(operation == OP_WATTS ? "watts" : "percent"), value)) {
        noteQueueRejection(index, operation, NAN, "invalid_number");
        reply(400, "Valor invalido. Informe um numero finito completo usando ponto decimal."); return;
    }
    uint32_t nonce;
    if (!parseUnsigned(webServer.arg("nonce"), nonce) || nonce != nextCommandId) {
        noteQueueRejection(index, operation, value, "duplicate_or_stale_nonce");
        reply(409, "Comando repetido ou nonce antigo. Atualize o status antes de enviar outro."); return;
    }
    if (pendingCommand.valid || isCommand(activeJob.operation)) {
        noteQueueRejection(index, operation, value, "command_slot_occupied");
        reply(409, "Ja existe comando aceito. Aguarde seu resultado."); return;
    }
    if (paused || stopping || !armed || !busReady || testLog.state() != FieldTestLog::RECORDING) {
        noteQueueRejection(index, operation, value, "session_not_armed_or_recording");
        reply(409, "Inicie uma sessao gravada e habilite os comandos antes de escrever."); return;
    }
    Device& device = devices[index];
    if (!device.enabled || !device.ready || !device.hasRated || !isfinite(device.rated) || device.rated <= 0) {
        noteQueueRejection(index, operation, value, "device_or_rated_power_unavailable");
        reply(409, "Aguarde a potencia nominal valida do inversor habilitado nesta sessao."); return;
    }
    if (value < 0 || (operation == OP_PERCENT ? value > 100 : value > device.rated)) {
        noteQueueRejection(index, operation, value, "value_out_of_range");
        reply(400, "Limite fora da faixa: 0 a 100% ou 0 ate a potencia nominal em W."); return;
    }
    if (nextCommandId == UINT32_MAX) { reply(503, "Contador de comandos esgotado; reinicie o dispositivo."); return; }
    if (!testLog.canAcceptCommand()) {
        // Queue pressure is temporary; it is not a full filesystem.
        reply(testLog.queued() ? 409 : 507,
              testLog.queued() ? "Aguarde a fila de logs ser gravada." : "Sem reserva de log para aceitar o comando.");
        return;
    }
    Event event{"CMD_QUEUED"}; event.device = index; event.operation = operation;
    event.commandId = nextCommandId; event.status = "QUEUED"; event.hasValue = true;
    event.value = value; event.unit = operation == OP_PERCENT ? "%" : "W";
    event.detail = "operator_request;no_automatic_retry_or_restore";
    if (!emitEvent(event, true)) { reply(507, "Nao foi possivel registrar o comando; nenhuma escrita foi agendada."); return; }
    pendingCommand = {true, nextCommandId++, index, operation, value};
    lastCommand = pendingCommand; lastCommandStatus = "QUEUED"; lastCommandModbus = INV_MB_NONE;
    reply(202, "Comando registrado e agendado. Aguarde DONE ou ERROR no painel.");
}

static void resetSessionReadings() {
    for (uint8_t i = 0; i < DEVICE_COUNT; ++i) {
        Device& d = devices[i];
        d.hasFrequency = d.hasPower = d.hasRated = d.hasSerial = false;
        d.ratedFromSpec = false; d.serial[0] = '\0';
        memset(d.triedAt, 0, sizeof(d.triedAt)); memset(d.tried, 0, sizeof(d.tried));
        d.ok = d.errors = d.timeouts = d.rejected = 0;
        d.status = INV_IDLE; d.modbus = INV_MB_NONE;
    }
    readCursor = 0; lastTrace[0] = '\0';
    lastCommand = {}; lastCommandStatus = "NONE"; lastCommandModbus = INV_MB_NONE;
    maxLoopGapUs = maxBusGapUs = maxApiUs = 0;
    lastLoopUs = lastBusUs = micros();
}
static void handleStart() {
    if (!authorized()) return;
    if (!stoppedAndQuiet()) { reply(409, "Pare e conclua a sessao anterior antes de iniciar outra."); return; }
    if (!busReady) { reply(503, "Barramento nao inicializado."); return; }
    const String label = webServer.arg("label"), utc = webServer.arg("utc");
    if (label.length() > 48 || utc.length() > 32) { reply(400, "Rotulo: ate 48 bytes; horario UTC: ate 32 bytes."); return; }
    if (!testLog.start(CSV_HEADER)) { reply(507, testLog.lastError()); return; }
    resetSessionReadings(); logSequence = 0;
    armed = false; paused = false; stopping = false; stopLogged = false;
    nextRequestAt = millis() + INTER_REQUEST_GAP_MS; lastHealthMs = millis();
    Event begin{"SESSION_START"}; begin.status = "DISARMED"; begin.source = "operator"; begin.detail = label.c_str(); emitEvent(begin, true);
    char detail[220];
    snprintf(detail, sizeof(detail), "build=%s;base=%s;compiled=%s %s;flash_real=%lu;flash_config=%lu;time=uptime_ms",
        BUILD_ID, BASE_COMMIT, __DATE__, __TIME__, (unsigned long)flashReal, (unsigned long)flashConfig);
    Event build{"BUILD"}; build.detail = detail; emitEvent(build, true);
    Event anchor{"BROWSER_UTC_ANCHOR"}; anchor.source = "browser_clock_unverified"; anchor.detail = utc.c_str(); emitEvent(anchor, true);
    for (uint8_t i = 0; i < DEVICE_COUNT; ++i) {
        snprintf(detail, sizeof(detail), "enabled=%u;ready=%u;uart=Serial;baud=9600;format=8N1;de_re=12;switch=13_LOW;led=2",
            devices[i].enabled, devices[i].ready);
        Event config{"DEVICE_CONFIG"}; config.device = i; config.detail = detail; emitEvent(config, true);
    }
    if (stopping) { reply(507, "Falha ao registrar contexto; sessao interrompida sem comandos."); return; }
    reply(201, "Sessao iniciada com leituras. Comandos permanecem desabilitados.");
}
static void handleStop() {
    if (!authorized()) return;
    if (paused && !stopping && !testLog.active()) { reply(200, "Dispositivo ja pausado."); return; }
    requestSessionStop("operator_stop");
    reply(202, "Parando: comando ja aceito e leitura em curso terminarao; depois os logs serao fechados.");
}
static void handleArm() {
    if (!authorized()) return;
    const String enable = webServer.arg("enable");
    if (enable != "0" && enable != "1") { reply(400, "enable deve ser 0 ou 1."); return; }
    if (paused || stopping || testLog.state() != FieldTestLog::RECORDING) { reply(409, "Inicie uma sessao antes de habilitar comandos."); return; }
    if (!testLog.canAcceptCommand() || testLog.queued() > 3) { reply(409, "Aguarde a gravacao dos logs."); return; }
    Event event{"COMMANDS_ARMED"}; event.status = enable == "1" ? "ARMED" : "DISARMED";
    event.source = "operator"; event.detail = "affects_new_commands_only";
    if (!emitEvent(event, true)) { reply(507, "Falha ao registrar habilitacao."); return; }
    armed = enable == "1";
    reply(200, armed ? "Comandos habilitados para esta sessao." : "Novos comandos desabilitados; comando aceito sera concluido.");
}
static void handleNote() {
    if (!authorized()) return;
    const String note = webServer.arg("note");
    if (!note.length() || note.length() > 100) { reply(400, "Observacao: de 1 a 100 bytes."); return; }
    if (paused || stopping || !testLog.canAcceptCommand() || testLog.queued() > 3) { reply(409, "Sessao nao aceita observacoes agora; aguarde os logs ou inicie uma sessao."); return; }
    Event event{"OPERATOR_NOTE"}; event.source = "operator"; event.detail = note.c_str();
    if (!emitEvent(event)) { reply(507, "Falha ao gravar observacao."); return; }
    reply(200, "Observacao registrada.");
}
static void handleDeviceConfig() {
    if (!authorized()) return;
    if (!stoppedAndQuiet()) { reply(409, "Pause a sessao antes de mudar os inversores habilitados."); return; }
    uint8_t index; if (!parseDevice(index)) return;
    const String enable = webServer.arg("enable");
    if (enable != "0" && enable != "1") { reply(400, "enable deve ser 0 ou 1."); return; }
    devices[index].enabled = enable == "1";
    reply(200, "Configuracao aplicada em RAM; ao reiniciar os tres inversores voltam habilitados.");
}

static void handleStatus() {
    const uint32_t now = millis();
    String json; json.reserve(4300);
    json += "{\"build\":"; json += jsonText(BUILD_ID);
    json += ",\"token\":"; json += jsonText(bootToken);
    json += ",\"nonce\":"; json += String(nextCommandId);
    json += ",\"paused\":"; json += jsonBool(paused);
    json += ",\"stopping\":"; json += jsonBool(stopping);
    json += ",\"armed\":"; json += jsonBool(armed);
    json += ",\"bus_ready\":"; json += jsonBool(busReady);
    json += ",\"quiet\":"; json += jsonBool(quiet());
    json += ",\"uptime_ms\":"; json += String(now);
    json += ",\"heap\":"; json += String(ESP.getFreeHeap());
    json += ",\"max_loop_gap_us\":"; json += String(maxLoopGapUs);
    json += ",\"max_bus_gap_us\":"; json += String(maxBusGapUs);
    json += ",\"max_api_us\":"; json += String(maxApiUs);
    json += ",\"flash_real\":"; json += String(flashReal);
    json += ",\"flash_config\":"; json += String(flashConfig);
    json += ",\"log\":{\"state\":"; json += jsonText(testLog.stateName());
    json += ",\"name\":"; json += jsonText(testLog.currentName());
    json += ",\"error\":"; json += jsonText(testLog.lastError());
    json += ",\"bytes\":"; json += String(testLog.size());
    json += ",\"free\":"; json += String(testLog.availableBytes());
    json += ",\"queued\":"; json += String(testLog.queued());
    json += ",\"dropped\":"; json += String(testLog.dropped());
    json += ",\"max_write_us\":"; json += String(testLog.maxWriteMicros());
    json += "},\"active\":"; json += jsonText(operationName(activeJob.operation));
    json += ",\"pending\":"; json += String(pendingCommand.valid ? pendingCommand.id : 0);
    json += ",\"command\":{\"id\":"; json += String(lastCommand.id);
    json += ",\"operation\":"; json += jsonText(operationName(lastCommand.operation));
    json += ",\"value\":"; json += jsonFloat(lastCommand.value, lastCommand.id != 0);
    json += ",\"status\":"; json += jsonText(lastCommandStatus);
    json += ",\"modbus\":"; json += jsonText(modbusName(lastCommandModbus));
    json += "},\"trace\":"; json += lastTrace[0] ? jsonText(lastTrace) : String("null");
    json += ",\"inverters\":[";
    for (uint8_t i = 0; i < DEVICE_COUNT; ++i) {
        const Device& d = devices[i];
        if (i) json += ',';
        json += "{\"index\":"; json += String(i);
        json += ",\"id\":"; json += String(d.id);
        json += ",\"model\":"; json += jsonText(d.model);
        json += ",\"enabled\":"; json += jsonBool(d.enabled);
        json += ",\"ready\":"; json += jsonBool(d.ready);
        json += ",\"frequency\":"; json += jsonFloat(d.frequency, d.hasFrequency);
        json += ",\"power\":"; json += jsonFloat(d.power, d.hasPower);
        json += ",\"frequency_age_ms\":"; json += d.hasFrequency ? String(now - d.frequencyAt) : String("null");
        json += ",\"power_age_ms\":"; json += d.hasPower ? String(now - d.powerAt) : String("null");
        json += ",\"stale\":"; json += jsonBool(!d.hasFrequency || !d.hasPower || now - d.frequencyAt > STALE_MS || now - d.powerAt > STALE_MS);
        json += ",\"rated\":"; json += jsonFloat(d.rated, d.hasRated);
        json += ",\"rated_source\":"; json += jsonText(!d.hasRated ? "unknown" : d.ratedFromSpec ? "descriptor" : "inverter");
        json += ",\"serial\":"; json += d.hasSerial ? jsonText(d.serial) : String("null");
        json += ",\"ok\":"; json += String(d.ok);
        json += ",\"errors\":"; json += String(d.errors);
        json += ",\"timeouts\":"; json += String(d.timeouts);
        json += ",\"rejected\":"; json += String(d.rejected);
        json += ",\"status\":"; json += jsonText(requestName(d.status));
        json += ",\"modbus\":"; json += jsonText(modbusName(d.modbus)); json += '}';
    }
    json += "]}";
    webServer.sendHeader("Cache-Control", "no-store");
    webServer.send(200, "application/json; charset=utf-8", json);
}
static bool requireStoppedStorage() {
    if (!stoppedAndQuiet()) { reply(409, "Pare a sessao e aguarde o fechamento do arquivo antes desta operacao."); return false; }
    if (!testLog.mounted()) { reply(503, testLog.lastError()); return false; }
    return true;
}
static void handleFiles() {
    if (!requireStoppedStorage()) return;
    String json("[");
    bool first = true;
    Dir dir = LittleFS.openDir("/");
    while (dir.next()) {
        String name = dir.fileName(); if (!name.startsWith("/")) name = "/" + name;
        if (!FieldTestLog::validFilename(name.c_str())) continue;
        if (!first) json += ',';
        first = false;
        json += "{\"name\":"; json += jsonText(name.c_str());
        json += ",\"bytes\":"; json += String(dir.fileSize()); json += '}';
    }
    json += ']'; webServer.sendHeader("Cache-Control", "no-store");
    webServer.send(200, "application/json; charset=utf-8", json);
}
static void handleDownload() {
    if (!requireStoppedStorage()) return;
    const String name = webServer.arg("file");
    if (!FieldTestLog::validFilename(name.c_str())) { reply(400, "Nome invalido; use /test-######.csv."); return; }
    File file = LittleFS.open(name, "r");
    if (!file) { reply(404, "Arquivo nao encontrado."); return; }
    webServer.sendHeader("Cache-Control", "no-store");
    webServer.sendHeader("Content-Disposition", String("attachment; filename=\"") + name.substring(1) + "\"");
    webServer.streamFile(file, "text/csv; charset=utf-8");
    file.close();
}
static void handleDelete() {
    if (!authorized() || !requireStoppedStorage()) return;
    const String name = webServer.arg("file");
    if (!FieldTestLog::validFilename(name.c_str())) { reply(400, "Nome invalido; use /test-######.csv."); return; }
    if (!testLog.removeFile(name.c_str())) { reply(400, testLog.lastError()); return; }
    reply(200, "Arquivo removido.");
}
static void handleFormat() {
    if (!authorized()) return;
    if (!stoppedAndQuiet()) { reply(409, "Pare a sessao antes de formatar."); return; }
    if (webServer.arg("confirm") != "FORMATAR") { reply(400, "Confirmacao obrigatoria: FORMATAR. Todos os arquivos LittleFS serao apagados."); return; }
    if (!testLog.formatStorage()) { reply(503, testLog.lastError()); return; }
    reply(200, "LittleFS formatado explicitamente. Inicie uma nova sessao.");
}

static void finishJob(InverterRequestStatus status) {
    Device& d = devices[activeJob.device];
    // The trace belongs to this job, unlike getLastModbusStatus which may refer
    // to an earlier call when validation or descriptor fallback happens locally.
    InverterModbusStatus result = activeJob.hadTransaction ? activeJob.lastTransaction : INV_MB_NONE;
    const bool command = isCommand(activeJob.operation);
    const char* detail = "";
    if (status == INV_DONE && !command) {
        if (activeJob.operation == OP_RATED && (!isfinite(activeJob.value) || activeJob.value <= 0)) {
            status = INV_ERROR; detail = "invalid_rated_power;commands_remain_unavailable";
        } else if (activeJob.operation != OP_SERIAL && !isfinite(activeJob.value)) {
            status = INV_ERROR; detail = "non_finite_measurement";
        }
    }
    if (status == INV_DONE) {
        ++d.ok;
        switch (activeJob.operation) {
            case OP_RATED:
                d.rated = activeJob.value; d.hasRated = true;
                d.ratedFromSpec = d.inverter->wasLastRatedPowerFallback();
                detail = d.ratedFromSpec ? "rated_source=descriptor" : "rated_source=inverter"; break;
            case OP_SERIAL:
                snprintf(d.serial, sizeof(d.serial), "%s", activeJob.serial); d.hasSerial = true;
                detail = activeJob.serial; break;
            case OP_FREQUENCY:
                d.frequency = activeJob.value; d.frequencyAt = millis(); d.hasFrequency = true; break;
            case OP_POWER:
                d.power = activeJob.value; d.powerAt = millis(); d.hasPower = true; break;
            default: detail = "protocol_acknowledged;inverter_effect_requires_operator_observation"; break;
        }
    } else if (status == INV_REJECTED) { ++d.rejected; detail = "library_rejected;not_automatically_retried"; }
    else {
        ++d.errors;
        if (result == INV_MB_TIMEOUT) ++d.timeouts;
        if (!detail[0] && result == INV_MB_SUCCESS) detail = "library_decode_or_local_error_after_modbus_success";
        else if (!detail[0] && !activeJob.hadTransaction) detail = "library_local_error_without_modbus_transaction";
    }
    d.status = status; d.modbus = result;
    Event event{command ? "CMD_RESULT" : "READ_RESULT"};
    event.device = activeJob.device; event.operation = activeJob.operation;
    event.commandId = activeJob.commandId; event.status = requestName(status); event.modbus = result;
    event.hasElapsed = true; event.elapsed = millis() - activeJob.startedAt;
    event.hasValue = command || (status == INV_DONE && activeJob.operation != OP_SERIAL);
    event.value = command ? activeJob.requested : activeJob.value;
    event.unit = activeJob.operation == OP_PERCENT ? "%" : activeJob.operation == OP_FREQUENCY ? "Hz" : activeJob.operation == OP_SERIAL ? "" : "W";
    event.detail = detail;
    emitEvent(event, command);
    if (command) { lastCommandStatus = requestName(status); lastCommandModbus = result; }
    activeJob = {};
    nextRequestAt = millis() + INTER_REQUEST_GAP_MS;
}
static void advanceActiveJob() {
    if (activeJob.operation == OP_NONE) return;
    Inverter& inverter = *devices[activeJob.device].inverter;
    const uint32_t started = micros();
    InverterRequestStatus status = INV_ERROR;
    switch (activeJob.operation) {
        case OP_RATED: status = inverter.getRatedPower(activeJob.value); break;
        case OP_SERIAL: status = inverter.getSerialNumber(activeJob.serial, sizeof(activeJob.serial)); break;
        case OP_FREQUENCY: status = inverter.getGridFrequency(activeJob.value); break;
        case OP_POWER: status = inverter.getActivePower(activeJob.value); break;
        case OP_WATTS: status = inverter.setPowerLimit(activeJob.requested); break;
        case OP_PERCENT: status = inverter.setPowerLimitPercent(activeJob.requested); break;
        default: break;
    }
    const uint32_t elapsed = micros() - started;
    if (elapsed > maxApiUs) maxApiUs = elapsed;
    if (status == INV_BUSY) { devices[activeJob.device].status = INV_BUSY; return; }
    finishJob(status);
}
static void schedulerTask() {
    if (activeJob.operation != OP_NONE) { advanceActiveJob(); return; }
    // Acceptance is first held in RAM, then written through the filesystem
    // before transmission. Flush is periodic; power-loss durability is not
    // guaranteed. A detected write failure must prevent an unstarted command.
    // An already started command is handled above and is allowed to finish.
    if (pendingCommand.valid && !testLog.active()) {
        Device& device = devices[pendingCommand.device];
        ++device.errors; device.status = INV_ERROR; device.modbus = INV_MB_NONE;
        pendingCommand.valid = false;
        lastCommandStatus = "CANCELLED_NO_LOG"; lastCommandModbus = INV_MB_NONE;
        requestSessionStop("pending_command_cancelled_no_log");
        return;
    }
    if (!busReady || modbusBus.isBusy()) return;
    // Empty the bounded log queue before creating a new batch. The next command
    // can then fit its start, up to three transactions, and final result.
    if (testLog.queued() || static_cast<int32_t>(millis() - nextRequestAt) < 0) return;
    if (pendingCommand.valid) {
        const Command command = pendingCommand;
        pendingCommand.valid = false;
        activeJob.operation = command.operation; activeJob.device = command.device;
        activeJob.commandId = command.id; activeJob.requested = command.value; activeJob.startedAt = millis();
        lastCommandStatus = "BUSY"; lastCommandModbus = INV_MB_NONE;
        Event event{"CMD_START"}; event.device = command.device; event.operation = command.operation;
        event.commandId = command.id; event.status = "BUSY"; event.hasValue = true; event.value = command.value;
        event.unit = command.operation == OP_PERCENT ? "%" : "W";
        if (!emitEvent(event, true)) {
            Device& device = devices[command.device];
            ++device.errors; device.status = INV_ERROR; device.modbus = INV_MB_NONE;
            activeJob = {};
            lastCommandStatus = "CANCELLED_NO_LOG"; lastCommandModbus = INV_MB_NONE;
            requestSessionStop("command_start_cancelled_no_log");
            return;
        }
        advanceActiveJob(); return;
    }
    if (paused || stopping || !testLog.canRecord()) return;
    for (uint8_t inspected = 0; inspected < DEVICE_COUNT * 4; ++inspected) {
        const uint8_t slot = readCursor;
        readCursor = (readCursor + 1) % (DEVICE_COUNT * 4);
        const uint8_t index = slot / 4, metric = slot % 4;
        Device& d = devices[index];
        const Operation operation = READ_OPERATIONS[metric];
        if (!d.enabled || !d.ready || (operation == OP_RATED && d.hasRated) || (operation == OP_SERIAL && d.hasSerial)) continue;
        if (d.tried[metric] && millis() - d.triedAt[metric] < READ_INTERVAL_MS) continue;
        d.tried[metric] = true; d.triedAt[metric] = millis();
        activeJob.operation = operation; activeJob.device = index; activeJob.startedAt = millis();
        advanceActiveJob(); return;
    }
}
static void healthTask() {
    if (paused || stopping || activeJob.operation != OP_NONE || testLog.queued() > 2 || !testLog.canRecord() || millis() - lastHealthMs < 5000) return;
    lastHealthMs = millis();
    char detail[220];
    snprintf(detail, sizeof(detail),
        "heap=%lu;loop_gap_us=%lu;bus_gap_us=%lu;api_call_us=%lu;fs_write_us=%lu;dropped=%lu;io_errors=%lu;capacity_stops=%lu",
        (unsigned long)ESP.getFreeHeap(), (unsigned long)maxLoopGapUs, (unsigned long)maxBusGapUs,
        (unsigned long)maxApiUs, (unsigned long)testLog.maxWriteMicros(), (unsigned long)testLog.dropped(),
        (unsigned long)testLog.ioErrors(), (unsigned long)testLog.capacityStops());
    Event event{"HEALTH"}; event.detail = detail; emitEvent(event);
}
static void stoppingTask() {
    if (!stopping || !quiet()) return;
    if (!stopLogged) {
        // Wait for room so a normal stop cannot erase its own final summary.
        if (testLog.queued() >= FieldTestLog::QUEUE_CAPACITY) return;
        Event event{"SESSION_STOP"}; event.status = "PAUSED"; event.detail = stopReason;
        emitEvent(event, true);
        stopLogged = true; testLog.requestStop();
    }
    if (!testLog.active()) { stopping = false; armed = false; paused = true; }
}

void setup() {
    pinMode(LED_PIN, OUTPUT); digitalWrite(LED_PIN, HIGH);
    pinMode(PIN_RS485_SWITCH, OUTPUT); digitalWrite(PIN_RS485_SWITCH, LOW);
    busReady = modbusBus.begin(Serial, MODBUS_BAUD, SERIAL_8N1, DE_RE_PIN);
    modbusBus.setTraceCallback(onTransaction);
    for (uint8_t i = 0; i < DEVICE_COUNT; ++i) {
        const ModbusConfigData config = {devices[i].id, MODBUS_BAUD, SERIAL_8N1, DE_RE_PIN};
        devices[i].inverter->attachBus(modbusBus);
        devices[i].inverter->attachConfig(config);
        devices[i].ready = busReady && devices[i].inverter->begin();
    }
    flashReal = ESP.getFlashChipRealSize(); flashConfig = ESP.getFlashChipSize();
    flashSafe = flashConfig >= 1024UL * 1024UL && flashReal >= flashConfig;
    testLog.begin(flashSafe); // Autoformat is disabled by FieldTestLog.
    for (uint8_t i = 0; i < 4; ++i) snprintf(bootToken + i * 8, 9, "%08lx", (unsigned long)os_random());
    WiFi.persistent(false); WiFi.mode(WIFI_AP); WiFi.softAP(AP_SSID, AP_PASSWORD);
    webServer.on("/", HTTP_GET, []() { webServer.send_P(200, "text/html; charset=utf-8", FIELD_TEST_PAGE); });
    webServer.on("/config", HTTP_GET, []() { webServer.send_P(200, "text/html; charset=utf-8", FIELD_TEST_PAGE); });
    webServer.on("/api/status", HTTP_GET, handleStatus);
    webServer.on("/api/ping", HTTP_GET, []() { reply(200, "alive"); });
    webServer.on("/session/start", HTTP_POST, handleStart);
    webServer.on("/session/stop", HTTP_POST, handleStop);
    webServer.on("/session/arm", HTTP_POST, handleArm);
    webServer.on("/session/note", HTTP_POST, handleNote);
    webServer.on("/config/device", HTTP_POST, handleDeviceConfig);
    webServer.on("/setPower", HTTP_POST, []() { handleCommand(OP_WATTS); });
    webServer.on("/setPowerPercent", HTTP_POST, []() { handleCommand(OP_PERCENT); });
    webServer.on("/api/files", HTTP_GET, handleFiles);
    webServer.on("/logs/download", HTTP_GET, handleDownload);
    webServer.on("/logs/delete", HTTP_POST, handleDelete);
    webServer.on("/logs/format", HTTP_POST, handleFormat);
    webServer.onNotFound([]() { reply(404, "Rota inexistente ou metodo HTTP incorreto."); });
    webServer.begin();
    lastLoopUs = lastBusUs = micros();
}

void loop() {
    uint32_t nowUs = micros();
    const uint32_t loopGap = nowUs - lastLoopUs; lastLoopUs = nowUs;
    if (loopGap > maxLoopGapUs) maxLoopGapUs = loopGap;
    const uint32_t busGap = nowUs - lastBusUs; lastBusUs = nowUs;
    if (busGap > maxBusGapUs) maxBusGapUs = busGap;
    modbusBus.task();
    // The UI/ping stays responsive during Modbus timeout. HTTP response sends
    // may block for a slow client; measured bus/loop gaps expose that limitation.
    webServer.handleClient();
    if (millis() - lastBlinkMs >= 250) {
        lastBlinkMs = millis(); ledOn = !ledOn; digitalWrite(LED_PIN, ledOn ? LOW : HIGH);
    }
    if ((!paused || stopping) && (testLog.state() == FieldTestLog::FAULT || testLog.state() == FieldTestLog::STOPPING))
        requestSessionStop(testLog.state() == FieldTestLog::FAULT ? "logger_io_fault" : "logger_capacity_stop");
    if (!paused && testLog.state() == FieldTestLog::RECORDING && !testLog.queued() && !testLog.canRecord())
        requestSessionStop("logger_capacity_reserve");
    schedulerTask();
    healthTask();
    stoppingTask();
    // Never write flash while a transaction is in flight or waiting consumption.
    testLog.service(activeJob.operation == OP_NONE && !modbusBus.isBusy());
    stoppingTask();
    yield();
}
