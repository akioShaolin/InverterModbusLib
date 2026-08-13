#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ModbusRTU.h>
#include <InverterModbusLib.h>

// =======================
// Configurações do teste
// =======================

#define LED_PIN 2

// Ajuste conforme seu hardware
#define DE_RE_PIN 12
#define PIN_RS485_SWITCH 13

#define MODBUS_BAUD 9600
#define READ_INTERVAL_MS 3000UL
#define INTER_REQUEST_GAP_MS 50UL

static const char* AP_SSID = "InverterModbus-Test";
static const char* AP_PASSWORD = "modbus123";

// =======================
// Modbus e inversores
// =======================

ModbusRTU mb;
InverterModbusBus modbusBus(mb);

Inverter inverter1(SIW500H_ST030_M3);
Inverter inverter2(SIW400G_T100_W0);
Inverter inverter3(SIW400G_T100_W0);

struct InverterReading {
    Inverter* inverter;
    const char* modelName;
    uint8_t slaveId;
    uint16_t frequencyAddress;
    uint16_t fieldLength;
    float frequencyScale;
    float frequency;
    uint32_t lastRaw;
    uint32_t successCount;
    uint32_t failCount;
    uint32_t rejectedCount;
    InverterRequestStatus lastRequestStatus;
    InverterModbusStatus lastModbusStatus;
    bool ready;
    bool hasValue;
};

InverterReading readings[] = {
    { &inverter1, "SIW500H_ST030_M3", 1, 0x7D55, 1, 0.01f, 0.0f, 0, 0, 0, 0, INV_IDLE, INV_MB_NONE, false, false },
    { &inverter2, "SIW400G_T100_W0",  2, 0x9C92, 1, 0.01f, 0.0f, 0, 0, 0, 0, INV_IDLE, INV_MB_NONE, false, false },
    { &inverter3, "SIW400G_T100_W0",  3, 0x9C92, 1, 0.01f, 0.0f, 0, 0, 0, 0, INV_IDLE, INV_MB_NONE, false, false }
};

constexpr uint8_t INVERTER_COUNT = sizeof(readings) / sizeof(readings[0]);

// =======================
// Estado cooperativo
// =======================

ESP8266WebServer webServer(80);

bool systemReady = false;
bool readCycleActive = false;
bool ledState = false;

uint8_t currentInverter = 0;
uint32_t lastReadCycleMs = 0;
uint32_t lastBlinkMs = 0;
uint32_t nextRequestMs = 0;

// =======================
// LED: prova de vida
// =======================

void blinkAliveTask() {
    if (millis() - lastBlinkMs >= 250UL) {
        lastBlinkMs = millis();
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
    }
}

// =======================
// Leitura assíncrona
// =======================

void finishCurrentReading() {
    currentInverter++;
    nextRequestMs = millis() + INTER_REQUEST_GAP_MS;

    if (currentInverter >= INVERTER_COUNT) {
        currentInverter = 0;
        readCycleActive = false;
        lastReadCycleMs = millis();
    }
}

void asyncFrequencyTask() {
    if (!systemReady) return;

    if (!readCycleActive) {
        if (millis() - lastReadCycleMs < READ_INTERVAL_MS) return;

        readCycleActive = true;
        currentInverter = 0;
    }

    if ((int32_t)(millis() - nextRequestMs) < 0) return;

    InverterReading& reading = readings[currentInverter];
    if (!reading.ready) {
        reading.failCount++;
        finishCurrentReading();
        return;
    }

    InverterRequestStatus status = reading.inverter->getGridFrequency(reading.frequency);
    reading.lastRequestStatus = status;

    switch (status) {
        case INV_BUSY:
            break;

        case INV_DONE:
            reading.successCount++;
            reading.hasValue = true;
            reading.lastRaw = (uint32_t)(reading.frequency / reading.frequencyScale + 0.5f);
            reading.lastModbusStatus = reading.inverter->getLastModbusStatus();
            finishCurrentReading();
            break;

        case INV_REJECTED:
            reading.rejectedCount++;
            break;

        case INV_ERROR:
            reading.failCount++;
            reading.lastModbusStatus = reading.inverter->getLastModbusStatus();
            finishCurrentReading();
            break;

        case INV_IDLE:
            break;
    }
}

// =======================
// Página web
// =======================

const char* requestStatusText(InverterRequestStatus status) {
    switch (status) {
        case INV_IDLE:     return "IDLE";
        case INV_BUSY:     return "BUSY";
        case INV_DONE:     return "DONE";
        case INV_ERROR:    return "ERROR";
        case INV_REJECTED: return "REJECTED";
        default:           return "UNKNOWN";
    }
}

const char* modbusStatusText(InverterModbusStatus status) {
    switch (status) {
        case INV_MB_NONE:                return "NONE";
        case INV_MB_SUCCESS:             return "SUCCESS";
        case INV_MB_TIMEOUT:             return "TIMEOUT";
        case INV_MB_ILLEGAL_FUNCTION:    return "ILLEGAL_FUNCTION";
        case INV_MB_ILLEGAL_ADDRESS:     return "ILLEGAL_ADDRESS";
        case INV_MB_ILLEGAL_VALUE:       return "ILLEGAL_VALUE";
        case INV_MB_SLAVE_FAILURE:       return "SLAVE_FAILURE";
        case INV_MB_UNEXPECTED_RESPONSE: return "UNEXPECTED_RESPONSE";
        case INV_MB_GENERAL_FAILURE:     return "GENERAL_FAILURE";
        default:                         return "UNKNOWN";
    }
}

void appendReadingHtml(String& html, const InverterReading& reading) {
    html += F("<section class='card'><h2>");
    html += reading.modelName;
    html += F("</h2><p class='id'>Modbus ID ");
    html += reading.slaveId;
    html += F(" · <strong class='");
    html += reading.ready ? F("ready'>ready") : F("not-ready'>not ready");
    html += F("</strong></p><div class='frequency'>");

    if (reading.hasValue) {
        html += String(reading.frequency, 2);
        html += F(" Hz");
    } else {
        html += F("Aguardando leitura");
    }

    html += F("</div><div class='counts'><span>Sucessos: ");
    html += reading.successCount;
    html += F("</span><span>Falhas: ");
    html += reading.failCount;
    html += F("</span><span>Rejeições: ");
    html += reading.rejectedCount;
    html += F("</span></div><div class='diagnostics'>Request: ");
    html += requestStatusText(reading.lastRequestStatus);
    html += F(" · Modbus: ");
    html += modbusStatusText(reading.lastModbusStatus);
    html += F("<br>Endereço: 0x");
    if (reading.frequencyAddress < 0x1000) html += '0';
    html += String(reading.frequencyAddress, HEX);
    html += F(" · field.length: ");
    html += reading.fieldLength;
    html += F(" · raw: ");
    if (reading.hasValue) html += reading.lastRaw;
    else html += '-';
    html += F("</div></section>");
}

void handleRoot() {
    String html;
    html.reserve(2200);
    html += F(
        "<!doctype html><html lang='pt-BR'><head>"
        "<meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'>"
        "<meta http-equiv='refresh' content='2'>"
        "<title>InverterModbusLib</title><style>"
        "body{margin:0;background:#0d131a;color:#e8eef4;font-family:Arial,sans-serif}"
        "main{max-width:900px;margin:auto;padding:24px}h1{margin-bottom:6px}"
        ".subtitle,.id{color:#91a4b7}.grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(240px,1fr));gap:16px;margin-top:24px}"
        ".card{background:#17212b;border:1px solid #2d4052;border-radius:12px;padding:18px}"
        ".card h2{font-size:17px;margin:0}.id{margin:6px 0 18px}.frequency{font-size:30px;font-weight:bold;color:#35d39a}"
        ".counts{display:flex;gap:14px;flex-wrap:wrap;margin-top:20px;color:#b9c7d4}"
        ".diagnostics{margin-top:16px;padding-top:12px;border-top:1px solid #2d4052;color:#91a4b7;font:13px monospace;line-height:1.6}"
        ".ready{color:#35d39a}.not-ready,.error{color:#ff8c94}</style></head><body><main>"
        "<h1>Frequência dos inversores</h1><p class='subtitle'>Atualização sequencial e não bloqueante a cada 3 segundos.</p>");

    html += F("<p class='");
    html += systemReady ? F("ready'>Barramento pronto") : F("error'>Falha no barramento");
    html += F(" · AP: ");
    html += WiFi.softAPIP().toString();
    html += F("</p>");

    html += F("<div class='grid'>");
    for (uint8_t i = 0; i < INVERTER_COUNT; ++i) {
        appendReadingHtml(html, readings[i]);
    }
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

// =======================
// Setup e loop
// =======================

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
    asyncFrequencyTask();
    yield();
}
