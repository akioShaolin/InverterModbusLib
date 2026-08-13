#include <Arduino.h>
#include <ModbusRTU.h>
#include <InverterModbusLib.h>

// =======================
// Configurações do teste
// =======================

#define LED_PIN 2

// Ajuste conforme seu hardware
#define DE_RE_PIN 12
#define PIN_RS485_SWITCH 13

// Ajuste conforme seu inversor
#define MODBUS_ID 1
#define MODBUS_BAUD 9600

// Ajuste conforme seu modelo
Inverter inverter(SIW500H_ST030_M3);   // troque pelo enum correto do seu modelo

ModbusRTU mb;
const ModbusConfigData modbusConfig = {
    MODBUS_ID,
    MODBUS_BAUD,
    SERIAL_8N1,
    DE_RE_PIN
};

// =======================
// Estados do teste
// =======================

bool inverterReady = false;
bool waitingFrequency = false;

uint32_t lastRequestMs = 0;
uint32_t lastBlinkMs = 0;

bool ledState = false;
float gridFrequency = 0.0f;

// Contadores sem Serial
uint32_t requestCount = 0;
uint32_t successCount = 0;
uint32_t failCount = 0;
uint32_t rejectedCount = 0;

// =======================
// Blink base: prova de vida
// =======================

void blinkAliveTask() {
    // Pisca a cada 250 ms
    // Se travar, o loop está bloqueado
    if (millis() - lastBlinkMs >= 250) {
        lastBlinkMs = millis();
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
    }
}

// =======================
// Tarefa de leitura assíncrona
// =======================

void asyncFrequencyTask() {
    if (!inverterReady) return;

    // A task da lib precisa rodar sempre
    inverter.task();

    // Inicia uma nova leitura a cada 3 segundos e continua
    // consultando a mesma requisição até ela terminar.
    if (!waitingFrequency && millis() - lastRequestMs < 3000) return;

    if (!waitingFrequency) {
        lastRequestMs = millis();
    }

    InverterRequestStatus status = inverter.getGridFrequency(gridFrequency);

    switch (status) {
        case INV_BUSY:
            if (!waitingFrequency) {
                waitingFrequency = true;
                requestCount++;
            }
            break;

        case INV_DONE:
            successCount++;
            waitingFrequency = false;
            break;

        case INV_REJECTED:
            rejectedCount++;
            waitingFrequency = false;
            break;

        case INV_ERROR:
            failCount++;
            waitingFrequency = false;
            break;

        case INV_IDLE:
            break;
    }
}

// =======================
// Setup
// =======================

void setup() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH); // LED_BUILTIN geralmente é invertido no ESP8266

    pinMode(DE_RE_PIN, OUTPUT);
    digitalWrite(DE_RE_PIN, LOW);

    pinMode(PIN_RS485_SWITCH, OUTPUT);
    digitalWrite(PIN_RS485_SWITCH, LOW); // Half Duplex

    Serial.begin(MODBUS_BAUD, SERIAL_8N1);

    mb.begin(&Serial, DE_RE_PIN);
    mb.master();

    inverter.attachModbus(mb);
    inverter.attachSerial(Serial);
    inverter.attachConfig(modbusConfig);

    inverter.setSlaveId(MODBUS_ID);

    inverterReady = inverter.begin();

    // Se falhar no begin, deixa LED piscando mesmo assim
}

// =======================
// Loop
// =======================

void loop() {
    blinkAliveTask();
    asyncFrequencyTask();

    yield();
}
