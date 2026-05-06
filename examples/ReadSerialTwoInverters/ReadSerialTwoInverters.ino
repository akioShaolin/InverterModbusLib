/*
  Exemplo: lendo o serial de 2 inversores no mesmo barramento RS485

  Placa: ESP8266 / ESP-07
  Biblioteca: inverterLib

  Um inversor está com Endereço 1 e o outro inversor está com o Endereço 2
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ModbusRTU.h>
#include <InverterModbusLib.h>

#define DE_RE_PIN 12  // Defina o pino DE_RE

const char* AP_SSID = "ESP07-Inverter";
const char* AP_PASS = "12345678";

ESP8266WebServer server(80);
ModbusRTU mb;
ModbusConfig modbusCfg;

// Inversores no mesmo barramento RS485
Inverter inverter400G(SIW400G_T100_W0);
Inverter inverter500H(SIW500H_ST030_M3);

Inverter* getSelectedInverter(const String& model) {
  if (model == "SIW400G") return &inverter400G;
  if (model == "SIW500H") return &inverter500H;
  return nullptr;
}

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Lendo serial de 2 inversores</title>
</head>
<body>
  <h2>Lendo serial de 2 inversores</h2>

  <form action="/read" method="GET">
    <label>Modelo:</label>
    <select name="model">
      <option value="SIW400G">SIW400G T100 W0</option>
      <option value="SIW500H">SIW500H ST030 M3</option>
    </select>

    <br><br>

    <label>ID Modbus:</label>
    <input type="number" name="id" min="1" max="247" value="1">

    <br><br>

    <button type="submit">Ler Serial</button>
    
  </form>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}

void handleRead() {
  if (!server.hasArg("model") || !server.hasArg("id")) {
    server.send(400, "text/plain", "Informe o modelo e o ID.");
    return;
  }
  
  String model = server.arg("model");
  uint8_t slaveId = server.arg("id").toInt();

  if (slaveId < 1 || slaveId > 247) {
    server.send(400, "text/plain", "ID invalido.");
    return;
  }

  Inverter* selectedInverter = getSelectedInverter(model);

  if (selectedInverter == nullptr) {
    server.send(400, "text/plain", "Modelo inválido");
    return;
  }

  selectedInverter->setSlaveId(slaveId);

  String serial;
  bool ok = selectedInverter->getSerial(serial);

  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<title>Resultado</title></head><body>";
  html += "<h2>Resultado</h2>";
  html += "<p><b>ID:</b> " + String(slaveId) + "</p>";

  if (ok) {
    html += "<p><b>Serial:</b> " + serial + "</p>";
  } else {
    html += "<p><b>Falha:</b> não foi possivel ler o serial.</p>";
  }

  html += "<br><a href='/'>Voltar</a>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void setup() {
  modbusCfg.setConfig(1, 9600, SERIAL_8N1, DE_RE_PIN);

  modbusCfg.applyTo(mb, Serial);
  mb.master();

  inverter400G.attachModbus(mb);
  inverter400G.attachConfig(modbusCfg);

  inverter500H.attachModbus(mb);
  inverter500H.attachConfig(modbusCfg);

  if (!inverter400G.begin() || !inverter500H.begin()) {
    return;
  }

  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);

  server.on("/", handleRoot);
  server.on("/read", handleRead);
  server.begin();
}

void loop() {
  server.handleClient();
  mb.task();
}