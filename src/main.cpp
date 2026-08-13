/*
 * Teste de bancada da arquitetura assíncrona da InverterModbusLib no ESP8266.
 *
 * Este programa cria um ponto de acesso Wi-Fi e uma página web em 192.168.4.1,
 * mantém três objetos Inverter no mesmo barramento RS485 e lê, sequencialmente,
 * frequência e potência ativa de cada equipamento. Cada operação possui seus
 * próprios valores, contadores e diagnósticos. O LED pisca a cada 250 ms para
 * demonstrar que Modbus, Wi-Fi e servidor web são atendidos sem espera bloqueante.
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

enum MetricId : uint8_t {
    METRIC_FREQUENCY,
    METRIC_ACTIVE_POWER,
    METRIC_COUNT
};

struct MetricReading {
    const char* name;
    const char* unit;
    uint16_t address;
    uint16_t fieldLength;
    float scale;
    float value;
    int32_t lastRaw;
    uint32_t successCount;
    uint32_t failCount;
    uint32_t timeoutCount;
    uint32_t rejectedCount;
    InverterRequestStatus lastRequestStatus;
    InverterModbusStatus lastModbusStatus;
    InverterModbusStatus lastModbusErrorStatus;
    bool hasValue;
};

struct InverterReading {
    Inverter* inverter;
    const char* modelName;
    uint8_t slaveId;
    MetricReading metrics[METRIC_COUNT];
    bool ready;
};

#define METRIC_INIT(label, unitText, reg, length, factor) \
    { label, unitText, reg, length, factor, 0.0f, 0, 0, 0, 0, 0, \
      INV_IDLE, INV_MB_NONE, INV_MB_NONE, false }

InverterReading readings[] = {
    { &inverter1, "SIW500H_ST030_M3", 1, {
        METRIC_INIT("Frequência", "Hz", 0x7D55, 1, 0.01f),
        METRIC_INIT("Potência ativa", "W", 0x7D50, 1, 1.0f)
    }, false },
    { &inverter2, "SIW400G_T100_W0", 2, {
        METRIC_INIT("Frequência", "Hz", 0x9C92, 1, 0.01f),
        METRIC_INIT("Potência ativa", "W", 0x9C8E, 1, 100.0f)
    }, false },
    { &inverter3, "SIW400G_T100_W0", 3, {
        METRIC_INIT("Frequência", "Hz", 0x9C92, 1, 0.01f),
        METRIC_INIT("Potência ativa", "W", 0x9C8E, 1, 100.0f)
    }, false }
};

#undef METRIC_INIT

constexpr uint8_t INVERTER_COUNT = sizeof(readings) / sizeof(readings[0]);

ESP8266WebServer webServer(80);
bool systemReady = false;
bool readCycleActive = false;
bool ledState = false;
uint8_t currentInverter = 0;
uint8_t currentMetric = 0;
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

void finishCurrentMetric() {
    currentMetric++;
    nextRequestMs = millis() + INTER_REQUEST_GAP_MS;

    if (currentMetric >= METRIC_COUNT) {
        currentMetric = 0;
        currentInverter++;
    }

    if (currentInverter >= INVERTER_COUNT) {
        currentInverter = 0;
        readCycleActive = false;
        lastReadCycleMs = millis();
    }
}

void recordError(InverterReading& reading, MetricReading& metric) {
    metric.failCount++;
    metric.lastModbusStatus = reading.inverter->getLastModbusStatus();
    metric.lastModbusErrorStatus = metric.lastModbusStatus;
    if (metric.lastModbusStatus == INV_MB_TIMEOUT) metric.timeoutCount++;
}

void asyncMeasurementsTask() {
    if (!systemReady) return;

    if (!readCycleActive) {
        if (millis() - lastReadCycleMs < READ_INTERVAL_MS) return;
        readCycleActive = true;
        currentInverter = 0;
        currentMetric = 0;
    }

    if ((int32_t)(millis() - nextRequestMs) < 0) return;

    InverterReading& reading = readings[currentInverter];
    MetricReading& metric = reading.metrics[currentMetric];

    if (!reading.ready) {
        metric.lastRequestStatus = INV_ERROR;
        metric.failCount++;
        metric.lastModbusErrorStatus = INV_MB_GENERAL_FAILURE;
        finishCurrentMetric();
        return;
    }

    InverterRequestStatus status = currentMetric == METRIC_FREQUENCY
        ? reading.inverter->getGridFrequency(metric.value)
        : reading.inverter->getActivePower(metric.value);
    metric.lastRequestStatus = status;

    switch (status) {
        case INV_BUSY:
            break;

        case INV_DONE:
            metric.successCount++;
            metric.hasValue = true;
            metric.lastRaw = (int32_t)(metric.value / metric.scale);
            metric.lastModbusStatus = reading.inverter->getLastModbusStatus();
            finishCurrentMetric();
            break;

        case INV_REJECTED:
            metric.rejectedCount++;
            break;

        case INV_ERROR:
            recordError(reading, metric);
            finishCurrentMetric();
            break;

        case INV_IDLE:
            break;
    }
}

const char* requestStatusText(InverterRequestStatus status) {
    switch (status) {
        case INV_IDLE: return "IDLE";
        case INV_BUSY: return "BUSY";
        case INV_DONE: return "DONE";
        case INV_ERROR: return "ERROR";
        case INV_REJECTED: return "REJECTED";
        default: return "UNKNOWN";
    }
}

const char* modbusStatusText(InverterModbusStatus status) {
    switch (status) {
        case INV_MB_NONE: return "NONE";
        case INV_MB_SUCCESS: return "SUCCESS";
        case INV_MB_TIMEOUT: return "TIMEOUT";
        case INV_MB_ILLEGAL_FUNCTION: return "ILLEGAL_FUNCTION";
        case INV_MB_ILLEGAL_ADDRESS: return "ILLEGAL_ADDRESS";
        case INV_MB_ILLEGAL_VALUE: return "ILLEGAL_VALUE";
        case INV_MB_SLAVE_FAILURE: return "SLAVE_FAILURE";
        case INV_MB_UNEXPECTED_RESPONSE: return "UNEXPECTED_RESPONSE";
        case INV_MB_GENERAL_FAILURE: return "GENERAL_FAILURE";
        default: return "UNKNOWN";
    }
}

void appendMetricHtml(String& html, const MetricReading& metric) {
    html += F("<div class='metric'><h3>");
    html += metric.name;
    html += F("</h3><div class='value'>");
    if (metric.hasValue) {
        html += String(metric.value, metric.unit[0] == 'H' ? 2 : 0);
        html += ' ';
        html += metric.unit;
    } else {
        html += F("Aguardando leitura");
    }
    html += F("</div><div class='counts'>Sucessos: ");
    html += metric.successCount;
    html += F(" · Falhas: ");
    html += metric.failCount;
    html += F(" · Timeouts: ");
    html += metric.timeoutCount;
    html += F(" · Rejeições: ");
    html += metric.rejectedCount;
    html += F("</div><div class='diagnostics'>Request: ");
    html += requestStatusText(metric.lastRequestStatus);
    html += F(" · Modbus atual: ");
    html += modbusStatusText(metric.lastModbusStatus);
    html += F("<br>Último erro Modbus: ");
    html += modbusStatusText(metric.lastModbusErrorStatus);
    html += F("<br>Endereço: 0x");
    if (metric.address < 0x1000) html += '0';
    html += String(metric.address, HEX);
    html += F(" · field.length: ");
    html += metric.fieldLength;
    html += F(" · raw: ");
    if (metric.hasValue) html += metric.lastRaw;
    else html += '-';
    html += F("</div></div>");
}

void appendInverterHtml(String& html, const InverterReading& reading) {
    html += F("<section class='card'><h2>");
    html += reading.modelName;
    html += F("</h2><p class='id'>Modbus ID ");
    html += reading.slaveId;
    html += reading.ready ? F(" · <strong class='ready'>ready</strong>")
                          : F(" · <strong class='error'>not ready</strong>");
    html += F("</p>");
    for (uint8_t i = 0; i < METRIC_COUNT; ++i) appendMetricHtml(html, reading.metrics[i]);
    html += F("</section>");
}

void handleRoot() {
    String html;
    html.reserve(5200);
    html += F("<!doctype html><html lang='pt-BR'><head><meta charset='utf-8'>"
              "<meta name='viewport' content='width=device-width,initial-scale=1'>"
              "<meta http-equiv='refresh' content='2'><title>InverterModbusLib</title><style>"
              "body{margin:0;background:#0d131a;color:#e8eef4;font-family:Arial,sans-serif}"
              "main{max-width:1050px;margin:auto;padding:24px}.subtitle,.id{color:#91a4b7}"
              ".grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(300px,1fr));gap:16px;margin-top:24px}"
              ".card{background:#17212b;border:1px solid #2d4052;border-radius:12px;padding:18px}"
              ".card h2{font-size:18px;margin:0}.metric{padding:14px 0;border-top:1px solid #2d4052}"
              ".metric h3{font-size:14px;color:#b9c7d4;margin:0 0 8px}.value{font-size:28px;font-weight:bold;color:#35d39a}"
              ".counts{margin-top:12px;color:#b9c7d4;line-height:1.6}.diagnostics{margin-top:12px;color:#91a4b7;font:12px monospace;line-height:1.6}"
              ".ready{color:#35d39a}.error{color:#ff8c94}</style></head><body><main>"
              "<h1>Medições dos inversores</h1><p class='subtitle'>Frequência e potência ativa, sequenciais e não bloqueantes.</p><p class='");
    html += systemReady ? F("ready'>Barramento pronto") : F("error'>Falha no barramento");
    html += F(" · AP: ");
    html += WiFi.softAPIP().toString();
    html += F("</p><div class='grid'>");
    for (uint8_t i = 0; i < INVERTER_COUNT; ++i) appendInverterHtml(html, readings[i]);
    html += F("</div></main></body></html>");
    webServer.sendHeader(F("Cache-Control"), F("no-store"));
    webServer.send(200, F("text/html; charset=utf-8"), html);
}

void startWifiAccessPoint() {
    WiFi.persistent(false);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    webServer.on("/", HTTP_GET, handleRoot);
    webServer.begin();
}

void setup() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
    pinMode(DE_RE_PIN, OUTPUT);
    digitalWrite(DE_RE_PIN, LOW);
    pinMode(PIN_RS485_SWITCH, OUTPUT);
    digitalWrite(PIN_RS485_SWITCH, LOW); // Half duplex

    bool busReady = modbusBus.begin(Serial, MODBUS_BAUD, SERIAL_8N1, DE_RE_PIN);
    for (uint8_t i = 0; i < INVERTER_COUNT; ++i) {
        readings[i].inverter->attachBus(modbusBus);
        readings[i].inverter->setSlaveId(readings[i].slaveId);
        readings[i].ready = readings[i].inverter->begin();
    }
    systemReady = busReady;
    startWifiAccessPoint();
}

void loop() {
    blinkAliveTask();
    modbusBus.task();
    webServer.handleClient();
    asyncMeasurementsTask();
    yield();
}
