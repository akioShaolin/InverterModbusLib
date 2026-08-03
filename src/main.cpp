#include <Arduino.h>
#include <ModbusRTU.h>
#include <InverterModbusLib.h>

// =======================
// Configurações do teste
// =======================

#define LED_PIN LED_BUILTIN

// Ajuste conforme seu hardware
#define DE_RE_PIN 12

// Ajuste conforme seu inversor
#define MODBUS_ID 1
#define MODBUS_BAUD 9600

// Ajuste conforme seu modelo
Inverter inverter(SIW500H_ST030_M3);   // troque pelo enum correto do seu modelo

ModbusRTU mb;

// =======================
// Estados do teste
// =======================

bool inverterReady = false;
bool waitingFrequency = false;

uint32_t lastRequestMs = 0;
uint32_t lastBlinkMs = 0;
uint32_t lastStatusPulseMs = 0;

bool ledState = false;

// Contadores sem Serial
uint32_t requestCount = 0;
uint32_t successCount = 0;
uint32_t failCount = 0;
uint32_t timeoutCount = 0;

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

    // Inicia uma leitura a cada 3 segundos
    if (!waitingFrequency && millis() - lastRequestMs >= 3000) {
        lastRequestMs = millis();

        if (inverter.requestGridFrequency()) {
            waitingFrequency = true;
            requestCount++;
        } else {
            failCount++;
        }
    }

    // A task da lib precisa rodar sempre
    inverter.task();

    // Verifica se terminou
    if (waitingFrequency && inverter.isDone()) {
        uint32_t raw = 0;

        if (inverter.getGridFrequencyResult(raw)) {
            successCount++;
        } else {
            failCount++;
        }

        waitingFrequency = false;
    }

    // Verifica erro/timeout
    if (waitingFrequency && inverter.hasError()) {
        timeoutCount++;
        waitingFrequency = false;
    }
}

// =======================
// Setup
// =======================

void setup() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH); // LED_BUILTIN geralmente é invertido no ESP8266

    Serial.begin(MODBUS_BAUD, SERIAL_8N1);

    inverter.attachModbus(mb);
    inverter.attachSerial(Serial);
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