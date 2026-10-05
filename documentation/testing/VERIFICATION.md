# Verificação da etapa SolarView 01

Executada em 05/10/2026, para `solarview-async-01.1`. Código base `eaa6006f`.
Não houve acesso físico ao SolarView ou aos inversores durante esta preparação.

## Compilação real para ESP8266

```sh
PLATFORMIO_SETTING_ENABLE_TELEMETRY=No python -m platformio run -e esp07
```

Resultado: **SUCCESS**.

| Item | Resultado |
| --- | --- |
| Plataforma | `espressif8266@4.2.1` |
| Core Arduino | ESP8266 3.1.2 |
| Transporte | `modbus-esp8266@4.1.0` |
| RAM estática | 47.556 / 81.920 bytes — 58,1% |
| Flash de programa | 400.051 / 761.840 bytes — 52,5% |
| Layout | 1 MiB flash, LittleFS 256 KiB |

A RAM informada pelo linker não inclui o pico dinâmico de Wi-Fi, HTTP e Strings.
O ensaio registra heap em operação. Permanecem avisos anteriores sobre ordem de
inicialização/memset em `InverterControl.cpp` e avisos de escape no script
`elf2bin.py` do toolchain; não foram introduzidos para o logger.

SHA-256 do binário produzido neste ambiente:
`0151a29ea28064345bf9272af305b948a516425755aeb43c4fd8b641d697b4f6`.
O binário não está versionado. Outro build pode ter SHA diferente, pois a
identificação inclui `__DATE__`/`__TIME__`. O firmware identificável e o commit
usado devem acompanhar o teste; não use esse SHA como identidade de todo rebuild.

## Regressões com código de produção e dependências simuladas

| Verificação | Resultado e alcance |
| --- | --- |
| `bash tests/host/run.sh` | PASS, 6 cenários. Compila implementações/mapas reais; reproduz comando antigo após rejeição, ownership, FC06/16, W→% pela nominal lida, timeout/falha local e limites numéricos; UBSan/float-cast-overflow. |
| `bash tests/host_bus/run.sh` | PASS, 7 cenários. RX/TX do observador, validade de dados/resultado, owner/request até consumo, exceção/timeout, recusa local, dois buses e tempo com wrap. |
| Teste `FieldTestLog` abaixo | PASS. Montagem sem autoformat, preservação, numeração, fila limitada, ausência de I/O no callback/ocupado, capacidade, conclusão crítica, escrita parcial e erros expostos pela API simulada. |
| `bash tests/host_app/run.sh` | PASS. Compila `main.cpp` real. Parsers, CSV, token/nonce, recusa sem substituir comando, parada com operação/comando aceito, pressão na fila, falha de armazenamento cancelando escrita ainda não iniciada. |
| Parser independente Python em `host_app/run.sh` | Status JSON válido e 16 linhas CSV com exatamente 20 colunas; ordem pedido → início → resultado → fechamento. |
| Interface DOM/API abaixo | PASS. Script HTML real, criação das três fichas, preenchimento preservado durante polling, habilitação, POST/nonce, link de download, reinício e desconexão. API simulada, sem afirmar renderização/latência física. |
| `git diff --check` | PASS. |

Os testes de regressão dos limites também foram executados contra o arquivo
`InverterControl.cpp` original da base: o cenário “pedido 10% recusado, seguido
de pedido 90%” falhou, demonstrando a retenção do valor antigo corrigida aqui.

```sh
g++ -std=c++17 -Wall -Wextra -Werror -Itests/host_log -Isrc \
  tests/host_log/test.cpp src/FieldTestLog.cpp -o /tmp/field-test-log
/tmp/field-test-log
```

Teste opcional de interface, requer Node.js e jsdom. Nesta preparação:
Node 24.19.0 e dependência instalada em diretório temporário, fora do projeto.

```sh
npm install --prefix /tmp/solarview-ui jsdom --no-audit --no-fund
NODE_PATH=/tmp/solarview-ui/node_modules node tests/ui/check.cjs
```

## O que ainda depende da bancada

- UART, transceptor, polaridade/conexões físicas, CRC e comportamento dos firmwares
  dos inversores não são simulados com fidelidade elétrica pelos testes host.
- O observador vê transações da biblioteca; não é captura de frame completo.
- O core ESP8266 não expõe todo erro de `LittleFS.flush()`. Um teste com erro
  simulado verifica a reação quando o erro é visível, não garante que o hardware
  detectará toda falha de sync. Faça download e verifique o CSV preservado.
- HTTP e flash são síncronos; frequência de atendimento, heap e recuperação
  dependem do ESP real, do navegador e do barramento. Os indicadores do CSV
  permitem avaliar isso, sem certificação automática de tempo real.
- `DONE/SUCCESS` não demonstra o efeito operacional, a precedência W/% no M3
  nem a restituição do estado original de enable no SIW400G.

Use [SOLARVIEW_TEST_01.md](SOLARVIEW_TEST_01.md) para o ensaio e devolva os CSVs
antes de executar a migração seguinte.
