# 🔌 InverterModbusLib

## 🇧🇷 Português

### 📌 Sobre o projeto

Esta biblioteca tem como objetivo padronizar a comunicação Modbus com inversores fotovoltaicos de diferentes fabricantes, criando uma interface única, consistente e reutilizável.

A proposta é permitir que desenvolvedores interajam com inversores sem precisar conhecer os detalhes específicos de cada mapa Modbus.

---

### 🎯 Objetivos

* Padronizar leitura e escrita de dados via Modbus
* Suportar múltiplos fabricantes e modelos
* Facilitar integração com sistemas embarcados (ESP8266, ESP32, Arduino)
* Reduzir retrabalho na implementação de novos inversores

---

### ⚙️ Como funciona

Cada inversor é descrito por um **mapa de registradores (Modbus Map)**, contendo:

* Endereço
* Tipo de dado
* Tamanho
* Escala
* Permissões (leitura/escrita)

A biblioteca utiliza esse mapa para interpretar automaticamente os dados.

```cpp
Inverter inverter(MODEL_X);

float power = inverter.getActivePower();
auto voltage = inverter.getGridVoltage();
```

---

### 🧠 Arquitetura

* `ModbusField` → descrição de cada dado
* `InverterMap` → conjunto de campos por modelo
* `ManufacturerProfile` → regras específicas por fabricante
* `Inverter` → interface principal

---

### 🚧 Status

Projeto em desenvolvimento ativo.

Já suporta:

* leitura estruturada de registradores
* múltiplos tipos de dados (U16, U32, FLOAT, ASCII)
* arrays e dados intercalados
* tratamento de campos não disponíveis

---

🚨 Comportamentos conhecidos

Boot/Shutdown em registrador único

Alguns inversores utilizam apenas um único registrador para controle de estado (ligar/desligar).

Nestes casos:

O registrador deve ser mapeado em *_map.boot*
Não deve ser mapeado em *_map.shutdown*

A biblioteca trata automaticamente as operações de boot e shutdown utilizando os valores definidos em ```bash BootMode```.

---

### 🤝 Colaboração com fabricantes

Este projeto busca colaboração direta com fabricantes de inversores.

Se você é fabricante ou representante técnico, sua contribuição é extremamente valiosa.

Estamos interessados em:

* Documentação oficial de mapas Modbus
* Diferenças entre modelos e firmwares
* Boas práticas de leitura/escrita
* Especificações de escala e encoding

---

### 📩 Contato

Caso queira colaborar ou fornecer documentação:

* Abra uma issue
* Entre em contato diretamente

---

### 📦 Dependências

Esta biblioteca utiliza a biblioteca ModbusRTU desenvolvida por Emelianov,
licenciada sob a BSD 3-Clause License.

---

### ⚠️ Observação importante

Mapas Modbus podem variar entre modelos e versões de firmware.
Este projeto busca representar essas variações de forma estruturada e segura.

---

## 🇺🇸 English

### 📌 About the project

This library aims to standardize Modbus communication with photovoltaic inverters from different manufacturers, providing a unified and reusable interface.

The goal is to allow developers to interact with inverters without dealing with vendor-specific register maps.

---

### 🎯 Goals

* Standardize Modbus read/write operations
* Support multiple manufacturers and models
* Enable embedded integrations (ESP8266, ESP32, Arduino)
* Reduce development effort when adding new devices

---

### ⚙️ How it works

Each inverter is described by a **Modbus register map**, including:

* Address
* Data type
* Length
* Scale
* Access permissions

The library uses this map to interpret data automatically.

```cpp
Inverter inverter(MODEL_X);

float power = inverter.getActivePower();
auto voltage = inverter.getGridVoltage();
```

---

### 🧠 Architecture

* `ModbusField` → describes each data point
* `InverterMap` → set of fields per model
* `ManufacturerProfile` → vendor-specific logic
* `Inverter` → main interface

---

### 🚧 Status

Work in progress.

Current features:

* structured register reading
* multiple data types (U16, U32, FLOAT, ASCII)
* support for arrays and interleaved data
* handling of unavailable fields

---

### 🚨 Known behaviors

Single register for boot/shutdown

Some inverters use a single register to control power state (start/stop).

In these cases:

The register must be mapped to *_map.boot*
It must not be mapped to *_map.shutdown*

The library automatically handles boot and shutdown operations using the values defined in ```bash BootMode```.

---

### 🤝 Collaboration with manufacturers

This project actively seeks collaboration with inverter manufacturers.

If you are a manufacturer or technical representative, your contribution is highly valuable.

We are interested in:

* Official Modbus protocol documentation
* Differences between models and firmware versions
* Recommended read/write practices
* Scaling and encoding specifications

---

### 📩 Contact

If you are willing to collaborate or share documentation:

* Open an issue
* Reach out directly

---

### 📦 Dependencies

This library depends on the ModbusRTU library by Emelianov,
licensed under the BSD 3-Clause License.

---

### ⚠️ Disclaimer

Modbus maps may vary between models and firmware versions.
This project aims to handle these variations in a structured and reliable way.

---
