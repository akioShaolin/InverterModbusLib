# Validação dos Mapas

Este documento descreve como os mapas Modbus da InverterModbusLib são validados.

A validação é feita preferencialmente com equipamentos reais em bancada ou campo.

## Status de Validação

| Status | Significado |
|---|---|
| OK | Testado e funcionando corretamente |
| NOK | Testado, mas não funcionou corretamente |
| Parcial | Parte das funções funcionou |
| Não testado | Existe mapa, mas ainda não foi testado em equipamento real |
| Não suportado | O modelo não possui essa função |
| Desabilitado | O registrador existe, mas está bloqueado ou desabilitado no firmware/configuração |
| Fallback pendente | A função depende de tratamento especial ainda não implementado |
| Mapa pendente | Há indícios do mapa, mas ele ainda não foi revisado |

## Critérios de Teste

Cada função deve ser validada considerando:

- endereço do registrador;
- tipo de dado;
- tamanho;
- stride;
- escala;
- permissão de leitura;
- permissão de escrita;
- offset 0-based ou 1-based;
- ordem dos words em valores de 32 bits;
- comportamento especial por fabricante;
- compatibilidade com firmware do inversor.

## Funções Prioritárias

Para o primeiro estágio do projeto, as funções mais importantes são:

- `getSerialNumber()`
- `getActivePower()`
- `getGridVoltage()`
- `getGridCurrent()`
- `getGridFrequency()`
- `getTotalEnergy()`
- `getDailyEnergy()`
- `getStringVoltage()`
- `getStringCurrent()`
- `getTemperature()`
- `setPowerLimit()`
- `setPowerLimitPercent()`
- `setExportLimit()`
- `setExportLimitPercent()`

## Observações Importantes

Nem todo registrador presente em um mapa Modbus oficial funciona necessariamente em todos os modelos ou firmwares.

Alguns fabricantes usam:

- offsets diferentes;
- registradores compartilhados;
- enums de controle;
- campos compactados;
- formatos especiais de data/hora;
- escalas inconsistentes;
- registradores de leitura e escrita separados;
- funções bloqueadas por senha, configuração ou firmware.

## Resultado por Modelo

Os resultados detalhados por modelo devem ser registrados na documentação de compatibilidade ou em arquivos separados por modelo.

Exemplo futuro:

```text
documentation/models/SIW500H_ST030_M3.md
documentation/models/GW3000N_XS.md
documentation/models/GW10K_MS.md