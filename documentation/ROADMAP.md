# Roadmap

Este documento descreve os próximos passos planejados para a InverterModbusLib.

## v0.1-alpha

Objetivo: disponibilizar uma primeira versão funcional para testes práticos, validação de mapas e desenvolvimento inicial.

### Planejado

- [x] Núcleo de comunicação Modbus RTU
- [x] Estrutura de mapas por modelo/família
- [x] API básica da classe `Inverter`
- [x] Suporte a ESP8266
- [x] Suporte inicial a ESP32
- [x] Configuração dinâmica de UART/Modbus
- [x] Exemplo de leitura de serial
- [x] Exemplo com painel web experimental
- [x] Primeiros mapas parcialmente validados
- [ ] Documentação inicial da API
- [ ] Documentação de compatibilidade
- [ ] Documentação de validação

## v0.2

Objetivo: melhorar a confiabilidade dos mapas e separar melhor funções estáveis de funções experimentais.

### Planejado

- [ ] Revisar mapas GoodWe
- [ ] Revisar mapas Huawei
- [ ] Revisar mapas WEG/FoxESS
- [ ] Adicionar mapas Chint/CPS
- [ ] Melhorar suporte a data/hora especial por fabricante
- [ ] Melhorar tratamento de registros 32 bits com ordem de words diferente
- [ ] Separar funções públicas estáveis de funções experimentais
- [ ] Adicionar mais exemplos básicos

## v0.3

Objetivo: melhorar a arquitetura interna para lidar com diferenças entre fabricantes.

### Planejado

- [ ] Avaliar arquitetura baseada em `features`
- [ ] Criar `ActivePowerFeature`
- [ ] Criar `ReactivePowerFeature`
- [ ] Criar `BatteryFeature`
- [ ] Criar `EpsFeature`
- [ ] Melhorar tratamento de status e alarmes
- [ ] Melhorar suporte a bitfields
- [ ] Adicionar handlers por fabricante

## Futuro

- [ ] Exemplos para ESP32 com múltiplas UARTs
- [ ] Suporte mais robusto a múltiplos inversores
- [ ] Decodificação de alarmes por fabricante
- [ ] Ferramentas auxiliares para validação de mapas
- [ ] Melhor documentação por modelo
- [ ] Testes automatizados quando possível