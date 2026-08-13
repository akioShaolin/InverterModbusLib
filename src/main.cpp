/*
 * Teste de bancada assíncrono da InverterModbusLib no ESP8266.
 *
 * Cria um AP Wi-Fi e uma página em 192.168.4.1 para três inversores no mesmo
 * RS485. Frequência e potência ativa são lidas continuamente; potência nominal
 * e serial são obtidos uma vez, com nova tentativa após erro. A página também
 * agenda setPowerLimit() e setPowerLimitPercent(), mantendo somente um comando
 * pendente. Modbus, HTTP e LED (250 ms) funcionam sem delay ou espera ativa.
 */

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ModbusRTU.h>
#include <InverterModbusLib.h>

#define LED_PIN 2
#define DE_RE_PIN 12
#define PIN_RS485_SWITCH 13
#define MODBUS_BAUD 9600
#define READ_INTERVAL_MS 3000UL
#define INTER_REQUEST_GAP_MS 50UL

static const char* AP_SSID = "InverterModbus-Test";
static const char* AP_PASSWORD = "modbus123";

ModbusRTU mb;
InverterModbusBus modbusBus(mb);
Inverter inverter1(SIW500H_ST030_M3);
Inverter inverter2(SIW400G_T100_W0);
Inverter inverter3(SIW400G_T100_W0);

enum MetricId : uint8_t { METRIC_FREQUENCY, METRIC_ACTIVE_POWER, METRIC_COUNT };
enum OperationId : uint8_t { OP_FREQUENCY, OP_ACTIVE_POWER, OP_RATED_POWER, OP_SERIAL, OP_COUNT };
enum CommandType : uint8_t { COMMAND_NONE, COMMAND_WATTS, COMMAND_PERCENT };

struct OperationStats {
    uint32_t successCount;
    uint32_t failCount;
    uint32_t timeoutCount;
    uint32_t rejectedCount;
    InverterRequestStatus requestStatus;
    InverterModbusStatus modbusStatus;
    InverterModbusStatus lastModbusErrorStatus;
};

struct MetricReading {
    const char* name;
    const char* unit;
    uint16_t address;
    uint16_t fieldLength;
    float scale;
    float value;
    int32_t lastRaw;
    OperationStats stats;
    bool hasValue;
};

struct StaticInfo {
    float ratedPower;
    char serial[33];
    OperationStats ratedStats;
    OperationStats serialStats;
    bool hasRatedPower;
    bool ratedPowerFromSpec;
    bool hasSerial;
};

struct CommandInfo {
    CommandType type;
    float requestedValue;
    uint16_t address;
    int32_t raw;
    OperationStats stats;
};

struct InverterReading {
    Inverter* inverter;
    const char* modelName;
    uint8_t slaveId;
    MetricReading metrics[METRIC_COUNT];
    StaticInfo info;
    CommandInfo command;
    bool ready;
};

#define STATS_INIT {0, 0, 0, 0, INV_IDLE, INV_MB_NONE, INV_MB_NONE}
#define METRIC_INIT(label, unitText, reg, length, factor) \
    {label, unitText, reg, length, factor, 0.0f, 0, STATS_INIT, false}
#define STATIC_INIT {0.0f, "", STATS_INIT, STATS_INIT, false, false, false}
#define COMMAND_INIT {COMMAND_NONE, 0.0f, 0xFFFF, 0, STATS_INIT}

InverterReading readings[] = {
    {&inverter1, "SIW500H_ST030_M3", 1, {
        METRIC_INIT("Frequência", "Hz", 0x7D55, 1, 0.01f),
        METRIC_INIT("Potência ativa", "W", 0x7D50, 1, 1.0f)}, STATIC_INIT, COMMAND_INIT, false},
    {&inverter2, "SIW400G_T100_W0", 2, {
        METRIC_INIT("Frequência", "Hz", 0x9C92, 1, 0.01f),
        METRIC_INIT("Potência ativa", "W", 0x9C8E, 1, 100.0f)}, STATIC_INIT, COMMAND_INIT, false},
    {&inverter3, "SIW400G_T100_W0", 3, {
        METRIC_INIT("Frequência", "Hz", 0x9C92, 1, 0.01f),
        METRIC_INIT("Potência ativa", "W", 0x9C8E, 1, 100.0f)}, STATIC_INIT, COMMAND_INIT, false}
};

#undef STATS_INIT
#undef METRIC_INIT
#undef STATIC_INIT
#undef COMMAND_INIT

constexpr uint8_t INVERTER_COUNT = sizeof(readings) / sizeof(readings[0]);
ESP8266WebServer webServer(80);
bool systemReady = false;
bool readCycleActive = false;
bool ledState = false;
bool commandPending = false;
bool commandStarted = false;
uint8_t commandInverter = 0;
uint8_t currentInverter = 0;
uint8_t currentOperation = 0;
uint32_t lastReadCycleMs = 0;
uint32_t lastBlinkMs = 0;
uint32_t nextRequestMs = 0;

void blinkAliveTask() {
    if (millis() - lastBlinkMs >= 250UL) {
        lastBlinkMs = millis();
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
    }
}

void updateStats(OperationStats& stats, Inverter& inverter, InverterRequestStatus status) {
    stats.requestStatus = status;
    if (status == INV_DONE) {
        stats.successCount++;
        stats.modbusStatus = inverter.getLastModbusStatus();
    } else if (status == INV_ERROR) {
        stats.failCount++;
        stats.modbusStatus = inverter.getLastModbusStatus();
        stats.lastModbusErrorStatus = stats.modbusStatus;
        if (stats.modbusStatus == INV_MB_TIMEOUT) stats.timeoutCount++;
    } else if (status == INV_REJECTED) {
        stats.rejectedCount++;
    }
}

bool operationAlreadyComplete(const InverterReading& reading, OperationId operation) {
    return (operation == OP_RATED_POWER && reading.info.hasRatedPower) ||
           (operation == OP_SERIAL && reading.info.hasSerial);
}

void finishCurrentOperation() {
    do {
        currentOperation++;
        if (currentOperation >= OP_COUNT) {
            currentOperation = 0;
            currentInverter++;
        }
        if (currentInverter >= INVERTER_COUNT) {
            currentInverter = 0;
            readCycleActive = false;
            lastReadCycleMs = millis();
            break;
        }
    } while (operationAlreadyComplete(readings[currentInverter], (OperationId)currentOperation));
    nextRequestMs = millis() + INTER_REQUEST_GAP_MS;
}

void asyncMeasurementsTask() {
    if (!systemReady || commandStarted) return;
    if (!readCycleActive) {
        if (millis() - lastReadCycleMs < READ_INTERVAL_MS) return;
        readCycleActive = true;
        currentInverter = 0;
        currentOperation = 0;
    }
    if ((int32_t)(millis() - nextRequestMs) < 0) return;

    InverterReading& reading = readings[currentInverter];
    if (operationAlreadyComplete(reading, (OperationId)currentOperation)) {
        finishCurrentOperation();
        return;
    }
    if (!reading.ready) {
        OperationStats* stats = currentOperation < METRIC_COUNT
            ? &reading.metrics[currentOperation].stats
            : (currentOperation == OP_RATED_POWER ? &reading.info.ratedStats : &reading.info.serialStats);
        stats->requestStatus = INV_ERROR;
        stats->failCount++;
        stats->modbusStatus = INV_MB_GENERAL_FAILURE;
        stats->lastModbusErrorStatus = INV_MB_GENERAL_FAILURE;
        finishCurrentOperation();
        return;
    }

    InverterRequestStatus status = INV_ERROR;
    OperationStats* stats = nullptr;
    if (currentOperation == OP_FREQUENCY || currentOperation == OP_ACTIVE_POWER) {
        MetricReading& metric = reading.metrics[currentOperation];
        status = currentOperation == OP_FREQUENCY
            ? reading.inverter->getGridFrequency(metric.value)
            : reading.inverter->getActivePower(metric.value);
        stats = &metric.stats;
        stats->requestStatus = status;
        if (status == INV_DONE) {
            metric.hasValue = true;
            metric.lastRaw = (int32_t)(metric.value / metric.scale);
        }
    } else if (currentOperation == OP_RATED_POWER) {
        status = reading.inverter->getRatedPower(reading.info.ratedPower);
        stats = &reading.info.ratedStats;
        stats->requestStatus = status;
        if (status == INV_DONE) {
            reading.info.hasRatedPower = true;
            reading.info.ratedPowerFromSpec = reading.inverter->wasLastRatedPowerFallback();
        }
    } else {
        status = reading.inverter->getSerialNumber(reading.info.serial, sizeof(reading.info.serial));
        stats = &reading.info.serialStats;
        stats->requestStatus = status;
        if (status == INV_DONE) reading.info.hasSerial = true;
    }

    if (status == INV_DONE || status == INV_ERROR) {
        updateStats(*stats, *reading.inverter, status);
        if (currentOperation == OP_RATED_POWER && status == INV_DONE && reading.info.ratedPowerFromSpec) {
            stats->modbusStatus = INV_MB_NONE;
        }
        finishCurrentOperation();
    } else if (status == INV_REJECTED) {
        updateStats(*stats, *reading.inverter, status);
    }
}

void commandTask() {
    if (!commandPending || !systemReady) return;
    InverterReading& reading = readings[commandInverter];
    CommandInfo& command = reading.command;
    if (!reading.ready) {
        command.stats.requestStatus = INV_ERROR;
        command.stats.failCount++;
        command.stats.modbusStatus = INV_MB_GENERAL_FAILURE;
        command.stats.lastModbusErrorStatus = INV_MB_GENERAL_FAILURE;
        commandPending = commandStarted = false;
        return;
    }
    if (!commandStarted && modbusBus.isBusy()) return;

    InverterRequestStatus status = command.type == COMMAND_WATTS
        ? reading.inverter->setPowerLimit(command.requestedValue)
        : reading.inverter->setPowerLimitPercent(command.requestedValue);
    command.stats.requestStatus = status;
    if (status == INV_BUSY) commandStarted = true;
    if (status == INV_DONE || status == INV_ERROR) {
        updateStats(command.stats, *reading.inverter, status);
        commandPending = commandStarted = false;
        nextRequestMs = millis() + INTER_REQUEST_GAP_MS;
    } else if (status == INV_REJECTED) {
        updateStats(command.stats, *reading.inverter, status);
        if (commandStarted) {
            commandPending = commandStarted = false;
        }
    }
}

const char* requestStatusText(InverterRequestStatus status) {
    switch (status) {
        case INV_IDLE: return "IDLE"; case INV_BUSY: return "BUSY"; case INV_DONE: return "DONE";
        case INV_ERROR: return "ERROR"; case INV_REJECTED: return "REJECTED"; default: return "UNKNOWN";
    }
}

const char* modbusStatusText(InverterModbusStatus status) {
    switch (status) {
        case INV_MB_NONE: return "NONE"; case INV_MB_SUCCESS: return "SUCCESS"; case INV_MB_TIMEOUT: return "TIMEOUT";
        case INV_MB_ILLEGAL_FUNCTION: return "ILLEGAL_FUNCTION"; case INV_MB_ILLEGAL_ADDRESS: return "ILLEGAL_ADDRESS";
        case INV_MB_ILLEGAL_VALUE: return "ILLEGAL_VALUE"; case INV_MB_SLAVE_FAILURE: return "SLAVE_FAILURE";
        case INV_MB_UNEXPECTED_RESPONSE: return "UNEXPECTED_RESPONSE"; case INV_MB_GENERAL_FAILURE: return "GENERAL_FAILURE";
        default: return "UNKNOWN";
    }
}

void appendStatsHtml(String& html, const OperationStats& stats) {
    html += F("<div class='counts'>Sucessos: "); html += stats.successCount;
    html += F(" · Falhas: "); html += stats.failCount;
    html += F(" · Timeouts: "); html += stats.timeoutCount;
    html += F(" · Rejeições: "); html += stats.rejectedCount;
    html += F("</div><div class='diagnostics'>Request: "); html += requestStatusText(stats.requestStatus);
    html += F(" · Modbus atual: "); html += modbusStatusText(stats.modbusStatus);
    html += F("<br>Último erro Modbus: "); html += modbusStatusText(stats.lastModbusErrorStatus); html += F("</div>");
}

void appendMetricHtml(String& html, const MetricReading& metric) {
    html += F("<div class='metric'><h3>"); html += metric.name; html += F("</h3><div class='value'>");
    if (metric.hasValue) { html += String(metric.value, metric.unit[0] == 'H' ? 2 : 0); html += ' '; html += metric.unit; }
    else html += F("Aguardando leitura");
    appendStatsHtml(html, metric.stats);
    html += F("<div class='diagnostics'>Endereço: 0x"); if (metric.address < 0x1000) html += '0';
    html += String(metric.address, HEX); html += F(" · field.length: "); html += metric.fieldLength; html += F(" · raw: ");
    if (metric.hasValue) html += metric.lastRaw; else html += '-'; html += F("</div></div>");
}

void appendStaticHtml(String& html, const InverterReading& reading) {
    html += F("<div class='metric'><h3>Informações estáticas</h3><p>Potência nominal: ");
    if (reading.info.hasRatedPower) { html += String(reading.info.ratedPower, 0); html += F(" W ("); html += reading.info.ratedPowerFromSpec ? F("descriptor/spec") : F("Modbus"); html += ')'; }
    else html += F("pendente");
    html += F("</p>"); appendStatsHtml(html, reading.info.ratedStats);
    html += F("<p>Serial: "); html += reading.info.hasSerial ? reading.info.serial : "pendente"; html += F("</p>");
    appendStatsHtml(html, reading.info.serialStats); html += F("</div>");
}

void appendCommandHtml(String& html, const InverterReading& reading, uint8_t index) {
    const CommandInfo& command = reading.command;
    html += F("<div class='metric'><h3>Limite de potência</h3><form action='/setPower' method='get'><input type='hidden' name='inv' value='");
    html += index; html += F("'><input name='watts' type='number' min='0' step='1' placeholder='Limite W'><button>Aplicar W</button></form>");
    html += F("<form action='/setPowerPercent' method='get'><input type='hidden' name='inv' value='"); html += index;
    html += F("'><input name='percent' type='number' min='0' max='100' step='0.1' placeholder='Limite %'><button>Aplicar %</button></form><p>Último comando: ");
    html += command.type == COMMAND_WATTS ? F("setPowerLimit") : command.type == COMMAND_PERCENT ? F("setPowerLimitPercent") : F("nenhum");
    if (command.type != COMMAND_NONE) { html += F(" · solicitado: "); html += String(command.requestedValue, 1); html += command.type == COMMAND_WATTS ? F(" W") : F(" %"); }
    html += F("</p>"); appendStatsHtml(html, command.stats);
    html += F("<div class='diagnostics'>Endereço final: ");
    if (command.address == 0xFFFF) html += '-'; else { html += F("0x"); html += String(command.address, HEX); }
    html += F(" · raw final: "); if (command.type == COMMAND_NONE) html += '-'; else html += command.raw; html += F("</div></div>");
}

void appendInverterHtml(String& html, const InverterReading& reading, uint8_t index) {
    html += F("<section class='card'><h2>"); html += reading.modelName; html += F("</h2><p class='id'>Modbus ID "); html += reading.slaveId;
    html += reading.ready ? F(" · <strong class='ready'>ready</strong>") : F(" · <strong class='error'>not ready</strong>"); html += F("</p>");
    for (uint8_t i = 0; i < METRIC_COUNT; ++i) appendMetricHtml(html, reading.metrics[i]);
    appendStaticHtml(html, reading); appendCommandHtml(html, reading, index); html += F("</section>");
}

void handleRoot() {
    String html; html.reserve(9000);
    html += F("<!doctype html><html lang='pt-BR'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'>"
              "<meta http-equiv='refresh' content='2'><title>InverterModbusLib</title><style>body{margin:0;background:#0d131a;color:#e8eef4;font-family:Arial,sans-serif}"
              "main{max-width:1100px;margin:auto;padding:24px}.subtitle,.id{color:#91a4b7}.grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(320px,1fr));gap:16px;margin-top:24px}"
              ".card{background:#17212b;border:1px solid #2d4052;border-radius:12px;padding:18px}.card h2{font-size:18px;margin:0}.metric{padding:14px 0;border-top:1px solid #2d4052}"
              ".metric h3{color:#b9c7d4;margin:0 0 8px}.value{font-size:28px;font-weight:bold;color:#35d39a}.counts{margin-top:10px;color:#b9c7d4;line-height:1.6}"
              ".diagnostics{margin-top:8px;color:#91a4b7;font:12px monospace;line-height:1.6}.ready{color:#35d39a}.error{color:#ff8c94}form{display:flex;gap:8px;margin:8px 0}"
              "input,button{padding:9px;border-radius:6px;border:1px solid #40566b;background:#0d131a;color:#e8eef4}button{cursor:pointer}</style></head><body><main>"
              "<h1>Medições e controle dos inversores</h1><p class='subtitle'>Operações sequenciais e não bloqueantes.</p><p class='");
    html += systemReady ? F("ready'>Barramento pronto") : F("error'>Falha no barramento");
    html += F(" · AP: "); html += WiFi.softAPIP().toString(); html += F("</p><div class='grid'>");
    for (uint8_t i = 0; i < INVERTER_COUNT; ++i) appendInverterHtml(html, readings[i], i);
    html += F("</div></main></body></html>");
    webServer.sendHeader(F("Cache-Control"), F("no-store")); webServer.send(200, F("text/html; charset=utf-8"), html);
}

bool parseCommand(const char* valueName, float& value, uint8_t& inverterIndex) {
    if (!webServer.hasArg("inv") || !webServer.hasArg(valueName)) return false;
    const String invArg = webServer.arg("inv");
    const String valueArg = webServer.arg(valueName);
    if (invArg.length() == 0 || valueArg.length() == 0) return false;
    char* end = nullptr;
    const long inv = strtol(invArg.c_str(), &end, 10);
    if (*end != '\0' || inv < 0 || inv >= INVERTER_COUNT) return false;
    end = nullptr;
    value = strtof(valueArg.c_str(), &end);
    if (*end != '\0' || !isfinite(value) || value < 0.0f) return false;
    inverterIndex = (uint8_t)inv;
    return true;
}

void queueCommand(CommandType type) {
    float value = 0.0f; uint8_t index = 0;
    const char* arg = type == COMMAND_WATTS ? "watts" : "percent";
    if (!parseCommand(arg, value, index) || (type == COMMAND_PERCENT && value > 100.0f)) {
        webServer.send(400, F("text/plain"), F("Parâmetros inválidos")); return;
    }
    CommandInfo& command = readings[index].command;
    if (commandPending) {
        command.stats.rejectedCount++;
    } else {
        commandPending = true; commandStarted = false; commandInverter = index;
        command.type = type; command.requestedValue = value;
        command.stats.requestStatus = INV_IDLE; command.stats.modbusStatus = INV_MB_NONE;
        const bool siw500 = index == 0;
        if (type == COMMAND_WATTS) {
            command.address = siw500 ? 0x9CBE : 0x9D6C;
            command.raw = siw500 ? (int32_t)value : (int32_t)roundf((value * 100.0f / 100000.0f) / 0.1f);
        } else {
            command.address = siw500 ? 0x9CBD : 0x9D6C;
            command.raw = (int32_t)roundf(value / 0.1f);
        }
    }
    webServer.sendHeader(F("Location"), F("/")); webServer.send(303);
}

void startWifiAccessPoint() {
    WiFi.persistent(false); WiFi.mode(WIFI_AP); WiFi.softAP(AP_SSID, AP_PASSWORD);
    webServer.on("/", HTTP_GET, handleRoot);
    webServer.on("/setPower", HTTP_GET, []() { queueCommand(COMMAND_WATTS); });
    webServer.on("/setPowerPercent", HTTP_GET, []() { queueCommand(COMMAND_PERCENT); });
    webServer.begin();
}

void setup() {
    pinMode(LED_PIN, OUTPUT); digitalWrite(LED_PIN, HIGH);
    pinMode(DE_RE_PIN, OUTPUT); digitalWrite(DE_RE_PIN, LOW);
    pinMode(PIN_RS485_SWITCH, OUTPUT); digitalWrite(PIN_RS485_SWITCH, LOW);
    const bool busReady = modbusBus.begin(Serial, MODBUS_BAUD, SERIAL_8N1, DE_RE_PIN);
    for (uint8_t i = 0; i < INVERTER_COUNT; ++i) {
        readings[i].inverter->attachBus(modbusBus);
        readings[i].inverter->setSlaveId(readings[i].slaveId);
        readings[i].ready = readings[i].inverter->begin();
    }
    systemReady = busReady; startWifiAccessPoint();
}

void loop() {
    blinkAliveTask();
    modbusBus.task();
    webServer.handleClient();
    commandTask();
    asyncMeasurementsTask();
    yield();
}
