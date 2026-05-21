# InverterModbusLib

[English](README.md)

Biblioteca Arduino para comunicação Modbus RTU com inversores fotovoltaicos utilizando ESP8266 e ESP32

### Sobre o projeto

Partes da arquitetura, documentação e exemplos desta biblioteca foram desenvolvidas com auxílio de inteligência artificial para acelerar prototipagem, revisão e organização do código.

Esta biblioteca tem como objetivo padronizar a comunicação Modbus com inversores fotovoltaicos de diferentes fabricantes, criando uma interface única, consistente e reutilizável.

A proposta é permitir que desenvolvedores interajam com inversores sem precisar conhecer os detalhes específicos de cada mapa Modbus.

---

## Status do Projeto

Este projeto está atualmente em fase **alpha**.

O núcleo de comunicação Modbus já está funcional, mas os mapas dos inversores ainda estão em validação prática.

### 🎯 Objetivos

* Fornecer uma API simples para leitura de dados e controle básico de potência com equipamentos reais
* Padronizar leitura e escrita de dados via Modbus
* Suportar múltiplos fabricantes e modelos
* Facilitar integração com sistemas embarcados (ESP8266, ESP32, Arduino)
* Reduzir retrabalho na implementação de novos inversores

---

## Recursos

- Comunicação Modbus RTU via RS485
- Compatibilidade com ESP8266 e ESP32
- Configuração dinâmica de UART/Modbus
- Suporte a múltiplos inversores no mesmo barramento
- Mapas Modbus separados por modelo/família
- API orientada a objetos
- Exemplos para Arduino IDE
- Exemplo com painel web para testes e validação
- Estrutura preparada para expansão futura

---

## Compatibilidade

| Modelo | Status | Observações |
|---|---|---|
| SIW200 M030 | Parcialmente validado | Dados de tempo usam tratamento especial GoodWe, leitura do reg de Energia total |
| SIW200 M050 | Parcialmente validado | Dados de tempo usam tratamento especial GoodWe, leitura do reg de Energia total |
| SIW200 M100 | Parcialmente validado | Dados de tempo usam tratamento especial GoodWe, leitura do reg de Energia total |
| SIW400G T100 W0 | Parcialmente validado | Grande parte das leituras funcionando |
| SIW500H ST030 M3 | Parcialmente validado | Limitação de potência/exportação funcionando |
| SIW500H ST015 M0 | Parcialmente validado | Limitação de potência/exportação funcionando |
| SIW420G K075 W00 |  Mapa encontrado | Validação pendente |
| SIW200H M050 W00 | Pendente | Mapa possivelmente diferente do SIW400G |
| SIW400H T030 W10 | Pendente | Teste inconclusivo |
| GW3000N-XS | Parcialmente validado | Dados de tempo usam tratamento especial GoodWe, leitura do reg de Energia total |
| GW5000-DNS | Parcialmente validado | Dados de tempo usam tratamento especial GoodWe, leitura do reg de Energia total |
| GW10K-MS | Parcialmente validado | Dados de tempo usam tratamento especial GoodWe, leitura do reg de Energia total |
| R100 | Parcialmente validado | Grande parte das leituras funcionando |
| H3-PRO | Pendente | Teste inconclusivo |
| SUN2000-30KTL-M3 | Parcialmente validado | Limitação de potência/exportação funcionando |
| SUN2000-15KTL-M0 | Parcialmente validado | Limitação de potência/exportação funcionando |

Modelos da mesma família podem compartilhar o mesmo mapa Modbus, mas somente os modelos testados fisicamente devem ser considerados validados.

---

## Documentação

A documentação detalhada está disponível na pasta `documentation`.

- [API pública](documentation/API.md)
- [Compatibilidade e validação](documentation/COMPATIBILITY.md)
- [Roadmap](documentation/ROADMAP.md)

> Alguns documentos ainda podem estar incompletos durante a fase alpha.

---

## Instalação

### Arduino IDE

Copie a pasta da biblioteca para o diretório de bibliotecas do Arduino:

```text
Documentos/Arduino/libraries/InverterModbusLib
```

Depois inicie a Arduino IDE e inclua a biblioteca no sketch

```cpp
#include <InverterModbusLib.h>
```

---

## Limitações e Comportamentos Conhecidos

Alguns inversores possuem comportamentos específicos por fabricante, como offsets diferentes, campos especiais, enums de controle, formatos próprios de data/hora e registradores parcialmente implementados.

Consulte:

- [Comportamentos conhecidos](documentation/KNOWN_BEHAVIORS.md)
- [Compatibilidade](documentation/COMPATIBILITY.md)
- [Status de validação](documentation/VALIDATION.md)

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

Esta biblioteca utiliza a biblioteca ModbusRTU desenvolvida por Emelianov, licenciada sob a BSD 3-Clause License.

---