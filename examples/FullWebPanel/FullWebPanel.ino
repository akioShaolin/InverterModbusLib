/*
  InverterModbusLib - Full Web Panel Example
  ------------------------------------------------------------
  Example for ESP8266 / ESP-07 using WiFi AP + WebServer + RS485/Modbus RTU.

  What this example does:
  - Creates a WiFi access point.
  - Shows a web page with one section for each public function of the library.
  - Provides GET buttons for read-only functions.
  - Provides input fields or toggle buttons for write functions.
  - Provides a separate configuration page for inverter model and Modbus/UART settings.

  Important notes:
  - Write functions change real inverter parameters. Use carefully.
  - On ESP8266, the main UART is often used for RS485/Modbus. Avoid Serial.print()
    while Modbus is active, because it can interfere with the RS485 bus.
  - The library is expected to apply Serial.begin(), mb.begin() and mb.master()
    internally through ModbusConfig / ensureApplied() before Modbus transactions.
  - This example uses one active inverter object at a time. Reconfiguring the web page
    deletes the previous object and creates a new one with the selected model.
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ModbusRTU.h>
#include <InverterModbusLib.h>
#include "WebPage.h"

// ====================================================== 
// WiFi AP
// ====================================================== 

const char* AP_SSID = "ESP07-Inverter";
const char* AP_PASS = "12345678";

// ====================================================== 
// Modbus / RS485 defaults
// ====================================================== 

#define DEFAULT_DE_RE_PIN 12

ESP8266WebServer server(80);
ModbusRTU mb;

// Active inverter selected in the /config page.
Inverter* inverter = nullptr;


struct ModelOption {
  const char* label;
  InverterModel model;
};

ModelOption modelOptions[] = {
  {"SIW200_M030", SIW200_M030},
  {"SIW200_M050", SIW200_M050},
  {"SIW200_M085", SIW200_M085},
  {"SIW200_M100", SIW200_M100},
  {"SIW200H_M050_W00", SIW200H_M050_W00},
  {"SIW200H_M075_W10", SIW200H_M075_W10},
  {"SIW200H_M105_W10", SIW200H_M105_W10},
  {"SIW200G_M030_W0", SIW200G_M030_W0},
  {"SIW200G_M030_W1", SIW200G_M030_W1},
  {"SIW200G_M050_W0", SIW200G_M050_W0},
  {"SIW200G_M050_W1", SIW200G_M050_W1},
  {"SIW200G_M060_W0", SIW200G_M060_W0},
  {"SIW200G_M060_W1", SIW200G_M060_W1},
  {"SIW200G_M070_W1", SIW200G_M070_W1},
  {"SIW200G_M075_W1", SIW200G_M075_W1},
  {"SIW200G_M080_W1", SIW200G_M080_W1},
  {"SIW200G_M090_W1", SIW200G_M090_W1},
  {"SIW200G_M105_W1", SIW200G_M105_W1},
  {"SIW300H_M030_W00", SIW300H_M030_W00},
  {"SIW300H_M050_W00", SIW300H_M050_W00},
  {"SIW300H_M060_W00", SIW300H_M060_W00},
  {"SIW300H_M075_W00", SIW300H_M075_W00},
  {"SIW300H_M080_W00", SIW300H_M080_W00},
  {"SIW300H_M100_W00", SIW300H_M100_W00},
  {"SIW400_ST075", SIW400_ST075},
  {"SIW400G_T012_W0", SIW400G_T012_W0},
  {"SIW400G_T012_W1", SIW400G_T012_W1},
  {"SIW400G_T015_W0", SIW400G_T015_W0},
  {"SIW400G_T015_W1", SIW400G_T015_W1},
  {"SIW400G_T020_W0", SIW400G_T020_W0},
  {"SIW400G_T020_W1", SIW400G_T020_W1},
  {"SIW400G_T025_W0", SIW400G_T025_W0},
  {"SIW400G_T025_W1", SIW400G_T025_W1},
  {"SIW400G_T075_W0", SIW400G_T075_W0},
  {"SIW400G_T100_W0", SIW400G_T100_W0},
  {"SIW400G_T050_W00", SIW400G_T050_W00},
  {"SIW400G_T060_W00", SIW400G_T060_W00},
  {"SIW400G_T075_W01", SIW400G_T075_W01},
  {"SIW400G_K015_W00", SIW400G_K015_W00},
  {"SIW400G_K020_W00", SIW400G_K020_W00},
  {"SIW400G_K025_W00", SIW400G_K025_W00},
  {"SIW400G_K030_W00", SIW400G_K030_W00},
  {"SIW400G_K037_W00", SIW400G_K037_W00},
  {"SIW400H_T015_W10", SIW400H_T015_W10},
  {"SIW400H_T030_W10", SIW400H_T030_W10},
  {"SIW420G_K025_W00", SIW420G_K025_W00},
  {"SIW420G_K075_W00", SIW420G_K075_W00},
  {"SIW500G_T075_W0", SIW500G_T075_W0},
  {"SIW500G_T100_W0", SIW500G_T100_W0},
  {"SIW500G_K050_W00", SIW500G_K050_W00},
  {"SIW500G_K075_W00", SIW500G_K075_W00},
  {"SIW500G_H250_W0", SIW500G_H250_W0},
  {"SIW500H_ST012_M2", SIW500H_ST012_M2},
  {"SIW500H_ST015_M2", SIW500H_ST015_M2},
  {"SIW500H_ST020_M2", SIW500H_ST020_M2},
  {"SIW500H_ST030_M3", SIW500H_ST030_M3},
  {"SIW500H_ST036_M3", SIW500H_ST036_M3},
  {"SIW500H_ST040_M3", SIW500H_ST040_M3},
  {"SIW500H_SK020", SIW500H_SK020},
  {"SIW500H_ST030", SIW500H_ST030},
  {"SIW500H_ST036", SIW500H_ST036},
  {"SIW500H_ST040", SIW500H_ST040},
  {"SIW500H_ST060", SIW500H_ST060},
  {"SIW500H_ST100", SIW500H_ST100},
  {"SIW500H_ST060_HV", SIW500H_ST060_HV},
  {"SIW500H_ST100_HV", SIW500H_ST100_HV},
  {"SIW500H_ST200_H0", SIW500H_ST200_H0},
  {"SIW500H_ST200_H3", SIW500H_ST200_H3},
  {"SIW500H_T012_W00", SIW500H_T012_W00},
  {"SIW500H_T015_W00", SIW500H_T015_W00},
  {"SIW500H_T017_W00", SIW500H_T017_W00},
  {"SIW500H_T020_W00", SIW500H_T020_W00},
  {"SIW500H_T025_W00", SIW500H_T025_W00},
  {"SIW600_T020_44", SIW600_T020_44},
  {"SIW610_T018_W0", SIW610_T018_W0},
  {"SIW610_T075_W0", SIW610_T075_W0},
  {"S700_G2", S700_G2},
  {"S1000_G2", S1000_G2},
  {"S1500_G2", S1500_G2},
  {"S2000_G2", S2000_G2},
  {"S2500_G2", S2500_G2},
  {"S3000_G2", S3000_G2},
  {"S3300_G2", S3300_G2},
  {"F3000_G2", F3000_G2},
  {"F3600_G2", F3600_G2},
  {"F4600_G2", F4600_G2},
  {"F5000_G2", F5000_G2},
  {"F5300_G2", F5300_G2},
  {"F6000_G2", F6000_G2},
  {"G7", G7},
  {"G7_5", G7_5},
  {"G8_", G8_},
  {"G9", G9},
  {"G10", G10},
  {"G10_5", G10_5},
  {"T3_G3", T3_G3},
  {"T4_G3", T4_G3},
  {"T5_G3", T5_G3},
  {"T6_G3", T6_G3},
  {"T8_G3", T8_G3},
  {"T10_G3", T10_G3},
  {"T12_G3", T12_G3},
  {"T15_G3", T15_G3},
  {"T17_G3", T17_G3},
  {"T20_G3", T20_G3},
  {"T23_G3", T23_G3},
  {"T25_G3", T25_G3},
  {"V30", V30},
  {"V33", V33},
  {"V36", V36},
  {"V40", V40},
  {"V50", V50},
  {"V60", V60},
  {"V75", V75},
  {"VL15", VL15},
  {"VL20", VL20},
  {"VL25", VL25},
  {"VL30", VL30},
  {"VL37_5", VL37_5},
  {"R75", R75},
  {"R100", R100},
  {"R110", R110},
  {"GW3000_XS", GW3000_XS},
  {"GW3300_XS_30", GW3300_XS_30},
  {"GW5000_DNS", GW5000_DNS},
  {"GW5K_DNS_G40", GW5K_DNS_G40},
  {"GW8500_MS", GW8500_MS},
  {"GW10K_MS", GW10K_MS},
  {"SUN2000_2KTL", SUN2000_2KTL},
  {"SUN2000_3KTL", SUN2000_3KTL},
  {"SUN2000_4KTL", SUN2000_4KTL},
  {"SUN2000_5KTL", SUN2000_5KTL},
  {"SUN2000_6KTL", SUN2000_6KTL},
  {"SUN2000_12KTL_M0", SUN2000_12KTL_M0},
  {"SUN2000_12KTL_M2", SUN2000_12KTL_M2},
  {"SUN2000_15KTL_M0", SUN2000_15KTL_M0},
  {"SUN2000_15KTL_M2", SUN2000_15KTL_M2},
  {"SUN2000_20KTL_M0", SUN2000_20KTL_M0},
  {"SUN2000_20KTL_M2", SUN2000_20KTL_M2},
  {"SUN2000_30KTL", SUN2000_30KTL},
  {"SUN2000_36KTL_A", SUN2000_36KTL_A},
  {"SUN2000_30KTL_M3", SUN2000_30KTL_M3},
  {"SUN2000_36KTL_M3", SUN2000_36KTL_M3},
  {"SUN2000_40KTL_M3", SUN2000_40KTL_M3},
  {"SUN2000_60KTL_M0", SUN2000_60KTL_M0},
  {"SUN2000_75KTL_M1", SUN2000_75KTL_M1},
  {"SUN2000_100KTL_M2", SUN2000_100KTL_M2},
  {"SUN2000_100KTL_H1", SUN2000_100KTL_H1},
  {"SUN2000_215KTL_H0", SUN2000_215KTL_H0},
  {"SUN2000_215KTL_H3", SUN2000_215KTL_H3},
  {"SUN2000_250KTL_H1", SUN2000_250KTL_H1},
};

const uint16_t MODEL_COUNT = sizeof(modelOptions) / sizeof(modelOptions[0]);

uint16_t activeModelIndex = 0;
ModbusConfigData activeCfg = {1, 9600, SERIAL_8N1, DEFAULT_DE_RE_PIN};

// ====================================================== 
// Formatting helpers
// ====================================================== 

String escapeHtml(const String& input) {
  String out;
  for (uint16_t i = 0; i < input.length(); i++) {
    char c = input[i];
    if (c == '&') out += "&amp;";
    else if (c == '<') out += "&lt;";
    else if (c == '>') out += "&gt;";
    else if (c == '"') out += "&quot;";
    else out += c;
  }
  return out;
}

String boolText(bool v) {
  return v ? "true" : "false";
}

String phaseText(const PhaseData& p, const char* unit) {
  String s;
  s += "R: " + String(p.r, 3) + " " + unit + "<br>";
  s += "S: " + String(p.s, 3) + " " + unit + "<br>";
  s += "T: " + String(p.t, 3) + " " + unit;
  return s;
}

String stringValuesText(const StringValues& values, const char* unit) {
  String s = "Count: " + String(values.count) + "<br>";
  for (uint8_t i = 0; i < values.count; i++) {
    s += "String " + String(i + 1) + ": " + String(values.values[i], 3) + " " + unit + "<br>";
  }
  return s;
}

String batteryValuesText(const BatteryValues& values, const char* unit) {
  String s = "Count: " + String(values.count) + "<br>";
  for (uint8_t i = 0; i < values.count; i++) {
    s += "Battery " + String(i + 1) + ": " + String(values.values[i], 3) + " " + unit + "<br>";
  }
  return s;
}

String datetimeText(const Datetime& dt) {
  String s;
  s += String(dt.year) + "-";
  if (dt.month < 10) s += "0";
  s += String(dt.month) + "-";
  if (dt.day < 10) s += "0";
  s += String(dt.day) + " ";
  if (dt.hour < 10) s += "0";
  s += String(dt.hour) + ":";
  if (dt.minute < 10) s += "0";
  s += String(dt.minute) + ":";
  if (dt.second < 10) s += "0";
  s += String(dt.second);
  return s;
}

String serialConfigToText(uint32_t config) {
  if (config == SERIAL_8N1) return "SERIAL_8N1";
  if (config == SERIAL_8N2) return "SERIAL_8N2";
  if (config == SERIAL_8E1) return "SERIAL_8E1";
  if (config == SERIAL_8E2) return "SERIAL_8E2";
  if (config == SERIAL_8O1) return "SERIAL_8O1";
  if (config == SERIAL_8O2) return "SERIAL_8O2";
  return "UNKNOWN";
}

uint32_t makeSerialConfig(char parity, uint8_t stopBits) {
  if (parity == 'N' && stopBits == 1) return SERIAL_8N1;
  if (parity == 'N' && stopBits == 2) return SERIAL_8N2;
  if (parity == 'E' && stopBits == 1) return SERIAL_8E1;
  if (parity == 'E' && stopBits == 2) return SERIAL_8E2;
  if (parity == 'O' && stopBits == 1) return SERIAL_8O1;
  if (parity == 'O' && stopBits == 2) return SERIAL_8O2;
  return SERIAL_8N1;
}

// ====================================================== 
// Inverter configuration
// ====================================================== 

bool configureActiveInverter(uint16_t modelIndex, const ModbusConfigData& cfg) {
  if (modelIndex >= MODEL_COUNT) return false;

  if (inverter != nullptr) {
    delete inverter;
    inverter = nullptr;
  }

  inverter = new Inverter(modelOptions[modelIndex].model);
  if (inverter == nullptr) return false;

  activeModelIndex = modelIndex;
  activeCfg = cfg;

  inverter->attachModbus(mb);
  inverter->attachSerial(Serial);
  inverter->attachConfig(activeCfg);

  return inverter->begin();
}

bool ensureInverterConfigured() {
  if (inverter != nullptr) return true;
  return configureActiveInverter(activeModelIndex, activeCfg);
}

// ====================================================== 
// HTTP handlers
// ====================================================== 

void handleRoot() {
  server.send_P(200, "text/html", rootPage);
}

void handleConfig() {
  server.send_P(200, "text/html", configPage);
}

void handleConfigApply() {
  uint16_t modelIndex = server.hasArg("model") ? server.arg("model").toInt() : activeModelIndex;

  if (modelIndex >= MODEL_COUNT) {
    server.send(400, "text/plain", "Modelo invalido.");
    return;
  }

  int idRaw = server.hasArg("id") ? server.arg("id").toInt() : activeCfg.id;
  uint8_t id = constrain(idRaw, 1, 247);

  uint32_t baud = server.hasArg("baud") ? server.arg("baud").toInt() : activeCfg.baud;

  char parity = server.hasArg("parity") ? server.arg("parity")[0] : 'N';
  uint8_t stopBits = server.hasArg("stopBits") ? server.arg("stopBits").toInt() : 1;

  int8_t deRePin = server.hasArg("dere") ? server.arg("dere").toInt() : activeCfg.deRePin;

  ModbusConfigData cfg = {
    id,
    baud,
    (SerialConfig)makeSerialConfig(parity, stopBits),
    deRePin
  };

  bool ok = configureActiveInverter(modelIndex, cfg);

  if (!ok) {
    server.send(500, "text/plain", "Falha ao aplicar configuracao.");
    return;
  }

  server.sendHeader("Location", "/");
  server.send(303);
}

void handleApiGet() {

  if (!server.hasArg("fn")){
    server.send(400, "text/plain", "Missing fn");
    return;
  }

  String fn = server.arg("fn");
  String result = "Unknown function";

  bool ok = false;

  if (fn == "getSerialNumber") { String v; ok = inverter->getSerialNumber(v); result = ok ? escapeHtml(v) : "Falha"; }
  else if (fn == "isBooted") { bool v; ok = inverter->isBooted(v); result = ok ? boolText(v) : "Falha"; }
  else if (fn == "isPowerLimitEnabled") { bool v; ok = inverter->isPowerLimitEnabled(v); result = ok ? boolText(v) : "Falha"; }
  else if (fn == "getPowerLimit") { float v; ok = inverter->getPowerLimit(v); result = ok ? String(v, 3) + " W" : "Falha"; }
  else if (fn == "getPowerLimitPercent") { float v; ok = inverter->getPowerLimitPercent(v); result = ok ? String(v, 3) + " %" : "Falha"; }
  else if (fn == "isExportLimitEnabled") { bool v; ok = inverter->isExportLimitEnabled(v); result = ok ? boolText(v) : "Falha"; }
  else if (fn == "getExportLimit") { float v; ok = inverter->getExportLimit(v); result = ok ? String(v, 3) + " W" : "Falha"; }
  else if (fn == "getExportLimitPercent") { float v; ok = inverter->getExportLimitPercent(v); result = ok ? String(v, 3) + " %" : "Falha"; }
  else if (fn == "isPowerFactorEnabled") { bool v; ok = inverter->isPowerFactorEnabled(v); result = ok ? boolText(v) : "Falha"; }
  else if (fn == "getPowerFactorSetpoint") { float v; ok = inverter->getPowerFactorSetpoint(v); result = ok ? String(v, 4) : "Falha"; }
  else if (fn == "getDatetime") { Datetime v; ok = inverter->getDatetime(v); result = ok ? datetimeText(v) : "Falha"; }
  else if (fn == "getYear") { uint16_t v; ok = inverter->getYear(v); result = ok ? String(v) : "Falha"; }
  else if (fn == "getMonth") { uint16_t v; ok = inverter->getMonth(v); result = ok ? String(v) : "Falha"; }
  else if (fn == "getDay") { uint16_t v; ok = inverter->getDay(v); result = ok ? String(v) : "Falha"; }
  else if (fn == "getHour") { uint16_t v; ok = inverter->getHour(v); result = ok ? String(v) : "Falha"; }
  else if (fn == "getMinute") { uint16_t v; ok = inverter->getMinute(v); result = ok ? String(v) : "Falha"; }
  else if (fn == "getSecond") { uint16_t v; ok = inverter->getSecond(v); result = ok ? String(v) : "Falha"; }
  else if (fn == "getEpochTime") { uint32_t v; ok = inverter->getEpochTime(v); result = ok ? String(v) : "Falha"; }
  else if (fn == "getActivePower") { float v; ok = inverter->getActivePower(v); result = ok ? String(v, 3) + " W" : "Falha"; }
  else if (fn == "getReactivePower") { float v; ok = inverter->getReactivePower(v); result = ok ? String(v, 3) + " VAr" : "Falha"; }
  else if (fn == "getApparentPower") { float v; ok = inverter->getApparentPower(v); result = ok ? String(v, 3) + " VA" : "Falha"; }
  else if (fn == "getPowerFactor") { float v; ok = inverter->getPowerFactor(v); result = ok ? String(v, 4) : "Falha"; }
  else if (fn == "getGridVoltage") { PhaseData v; ok = inverter->getGridVoltage(v); result = ok ? phaseText(v, "V") : "Falha"; }
  else if (fn == "getGridCurrent") { PhaseData v; ok = inverter->getGridCurrent(v); result = ok ? phaseText(v, "A") : "Falha"; }
  else if (fn == "getGridFrequency") { PhaseData v; ok = inverter->getGridFrequency(v); result = ok ? phaseText(v, "Hz") : "Falha"; }
  else if (fn == "getTotalEnergy") { float v; ok = inverter->getTotalEnergy(v); result = ok ? String(v, 3) + " kWh" : "Falha"; }
  else if (fn == "getDailyEnergy") { float v; ok = inverter->getDailyEnergy(v); result = ok ? String(v, 3) + " kWh" : "Falha"; }
  else if (fn == "getStringVoltage") { StringValues v; ok = inverter->getStringVoltage(v); result = ok ? stringValuesText(v, "V") : "Falha"; }
  else if (fn == "getStringCurrent") { StringValues v; ok = inverter->getStringCurrent(v); result = ok ? stringValuesText(v, "A") : "Falha"; }
  else if (fn == "getStringPower") { StringValues v; ok = inverter->getStringPower(v); result = ok ? stringValuesText(v, "W") : "Falha"; }
  else if (fn == "getBatteryVoltage") { BatteryValues v; ok = inverter->getBatteryVoltage(v); result = ok ? batteryValuesText(v, "V") : "Falha"; }
  else if (fn == "getBatteryCurrent") { BatteryValues v; ok = inverter->getBatteryCurrent(v); result = ok ? batteryValuesText(v, "A") : "Falha"; }
  else if (fn == "getBatteryPower") { BatteryValues v; ok = inverter->getBatteryPower(v); result = ok ? batteryValuesText(v, "W") : "Falha"; }
  else if (fn == "getBatterySoC") { BatteryValues v; ok = inverter->getBatterySoC(v); result = ok ? batteryValuesText(v, "%") : "Falha"; }
  else if (fn == "getBatterySoH") { BatteryValues v; ok = inverter->getBatterySoH(v); result = ok ? batteryValuesText(v, "%") : "Falha"; }
  else if (fn == "getEPSVoltage") { PhaseData v; ok = inverter->getEPSVoltage(v); result = ok ? phaseText(v, "V") : "Falha"; }
  else if (fn == "getEPSCurrent") { PhaseData v; ok = inverter->getEPSCurrent(v); result = ok ? phaseText(v, "A") : "Falha"; }
  else if (fn == "getEPSActivePower") { PhaseData v; ok = inverter->getEPSActivePower(v); result = ok ? phaseText(v, "W") : "Falha"; }
  else if (fn == "getTemperature") { float v; ok = inverter->getTemperature(v); result = ok ? String(v, 3) + " °C" : "Falha"; }
  else if (fn == "getInsulationResistance") { float v; ok = inverter->getInsulationResistance(v); result = ok ? String(v, 3) + " kΩ" : "Falha"; }
  else if (fn == "getInverterStatus") { uint16_t v; ok = inverter->getInverterStatus(v); result = ok ? String(v) : "Falha"; }
  else if (fn == "getAlarm") { uint16_t v; ok = inverter->getAlarm(v); result = ok ? String(v) : "Falha"; }
  else { result = "Função desconhecida."; }

  server.send(200, "text/html", result);
}

void handleApiSet() {
  if (!server.hasArg("fn")) {
    server.send(400, "text/plain", "Missing fn");
    return;
  }

  String fn = server.arg("fn");

  float f = server.hasArg("value") ? server.arg("value").toFloat() : 0.0f;
  uint32_t u = server.hasArg("value") ? (uint32_t)server.arg("value").toInt() : 0;
  bool b = server.hasArg("value") ? (server.arg("value").toInt() != 0) : false;
  bool ok = false;
  String group = "set";

  if (fn == "boot") ok = inverter->boot();
  else if (fn == "shutdown") ok = inverter->shutdown();
  else if (fn == "setBoot") ok = inverter->setBoot(b);
  else if (fn == "setPowerLimitEnabled") ok = inverter->setPowerLimitEnabled(b);
  else if (fn == "setPowerLimit") ok = inverter->setPowerLimit(f);
  else if (fn == "setPowerLimitPercent") ok = inverter->setPowerLimitPercent(f);
  else if (fn == "setExportLimitEnabled") ok = inverter->setExportLimitEnabled(b);
  else if (fn == "setExportLimit") ok = inverter->setExportLimit(f);
  else if (fn == "setExportLimitPercent") ok = inverter->setExportLimitPercent(f);
  else if (fn == "setPowerFactorEnabled") ok = inverter->setPowerFactorEnabled(b);
  else if (fn == "setPowerFactor") ok = inverter->setPowerFactor(f);
  else if (fn == "setPowerFactorExcitationMode") ok = inverter->setPowerFactorExcitationMode((PfExcitationMode)u);
  else if (fn == "setDatetime") {
    group = "setTime";
    Datetime dt;
    dt.year = server.arg("year").toInt();
    dt.month = server.arg("month").toInt();
    dt.day = server.arg("day").toInt();
    dt.hour = server.arg("hour").toInt();
    dt.minute = server.arg("minute").toInt();
    dt.second = server.arg("second").toInt();
    ok = inverter->setDatetime(dt);
  }
  else if (fn == "setYear") { group = "setTime"; ok = inverter->setYear((uint16_t)u); }
  else if (fn == "setMonth") { group = "setTime"; ok = inverter->setMonth((uint16_t)u); }
  else if (fn == "setDay") { group = "setTime"; ok = inverter->setDay((uint16_t)u); }
  else if (fn == "setHour") { group = "setTime"; ok = inverter->setHour((uint16_t)u); }
  else if (fn == "setMinute") { group = "setTime"; ok = inverter->setMinute((uint16_t)u); }
  else if (fn == "setSecond") { group = "setTime"; ok = inverter->setSecond((uint16_t)u); }
  else if (fn == "setEpochTime") { group = "setTime"; ok = inverter->setEpochTime(u); }
  else { server.send(400, "text/plain", "Unknown function"); return;}

  server.send(200, "text/html", ok ? "OK" : "Falha");
  return;
}

void handleApiConfig() {
  String html;
  html.reserve(256);

  html += "<b>Modelo ativo:</b> ";
  html += modelOptions[activeModelIndex].label;
  html += "<br>";

  html += "<b>Slave ID:</b> ";
  html += String(activeCfg.id);
  html += " | <b>Baud:</b> ";
  html += String(activeCfg.baud);
  html += " | <b>Serial:</b> ";
  html += serialConfigToText(activeCfg.serialConfig);
  html += " | <b>DE/RE:</b> GPIO ";
  html += String(activeCfg.deRePin);

  html += "<div class='small'>";
  html += "Use a página de configuração para alterar modelo, ID e parâmetros Modbus/UART.";
  html += "</div>";

  server.send(200, "text/html", html);
}

// ====================================================== 

void setup() {
  // Do not call Serial.begin() for debug here when using Serial as RS485.
  // The library applies the UART configuration before Modbus transactions.

  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);

  ensureInverterConfigured();

  server.on("/", handleRoot);
  server.on("/config", handleConfig);
  server.on("/config/apply", handleConfigApply);
  server.on("/api/config", handleApiConfig);
  server.on("/api/get", handleApiGet);
  server.on("/api/set", handleApiSet);
  server.begin();
}

void loop() {
  server.handleClient();
  mb.task();
  yield();
}