# SolarView — teste 01: limites de potência e logs

Firmware `solarview-async-01.1`, preparado em 05/10/2026 sobre
`refactor/nonblocking-modbus` (`eaa6006f`). Branch de ensaio:
`test/solarview-async-validation-01` (local; publicação remota bloqueada por HTTP
403 da integração GitHub nesta entrega). **O firmware está preparado para bancada;
esta documentação não declara um teste físico concluído.**

## O que esta rodada valida

1. Regressão das quatro leituras já informadas como funcionais: frequência,
   potência ativa, potência nominal e serial, com identificação de cada inversor.
2. `setPowerLimitPercent()` e `setPowerLimit()`: valor solicitado, FC06/FC16,
   endereço, palavras realmente entregues ao transporte, resposta e duração.
3. Sequência enable → setpoint nos SIW400G; conversão W → % pela nominal
   efetivamente obtida, registrando se veio do equipamento ou do descriptor.
4. Um comando por vez, prioridade após consumir a leitura atual, continuidade
   do loop/HTTP, comportamento com timeout e recuperação sem reboot.
5. Geração, fechamento, download e integridade dos arquivos de teste.

O firmware não migra os outros getters/setters nesta rodada. Não chama APIs
legadas bloqueantes para tentar confirmar a escrita. Readback do setpoint será
a próxima etapa: [plano completo](REVIEW_AND_NEXT_STEPS.md) e
[prompt de continuação](CODEX_NEXT_COMMAND.md).

## Hardware e configuração preservados

| Item | Configuração |
| --- | --- |
| Placa | SolarView adaptado, ESP-07 / ESP8266 |
| UART | `Serial` / UART0: TX GPIO1, RX GPIO3 |
| Direção RS485 | GPIO12: HIGH transmissão; LOW recepção |
| Seleção RS485 | GPIO13 mantido LOW |
| LED de atividade | GPIO2, alternando a cada 250 ms |
| Modbus | 9600, 8N1 |
| ID 1 | `SIW500H_ST030_M3` |
| IDs 2 e 3 | `SIW400G_T100_W0` |
| Rede Wi-Fi | `InverterModbus-Test` / senha `modbus123` |
| Painel | `http://192.168.4.1/` |
| Sessão/comandos/arquivos | `http://192.168.4.1/config` |

O firmware mantém o circuito adaptado; não utiliza o RTC nem a EEPROM externa
do datalogger. Os logs usam LittleFS na flash do ESP. Não há `Serial.print()`
porque a UART está reservada para os inversores. O LED sozinho não comprova
conclusão de uma transação.

O perfil `esp07` usa flash configurada de 1 MiB e LittleFS de 256 KiB
(`eagle.flash.1m256.ld`). Isso é a configuração de compilação, não uma medição
prévia da sua placa. O painel mostra a flash física e a configurada; o acesso ao
filesystem é recusado se a flash física não cobrir o perfil. O layout já é o
padrão do perfil ESP-07 da plataforma fixada. Não altere o layout entre ensaios
sem baixar os arquivos anteriores.

## Obter e gravar

Como o último relatório mencionava mudanças locais sem commit, prefira uma
pasta separada. Assim o projeto antigo permanece disponível para comparação.
O anexo recebido está guardado como `solarview-main-received.cpp.txt`, com
escapes Markdown removidos; não é compilado.

Baixe e extraia `InverterModbusLib-SolarView-Teste01.zip`. Abra a pasta
`InverterModbusLib` extraída no PlatformIO e execute no terminal dessa pasta:

```powershell
pio run -e esp07
pio run -e esp07 -t upload
```

O ZIP contém o projeto completo, o binário compilado, um patch contra a base
`eaa6006f` e instruções. Não é necessário esperar a publicação no GitHub para
compilar/gravar. Para reservar a versão no repositório usando seu Codex local,
consulte [CODEX_PUBLISH_TEST_01.md](CODEX_PUBLISH_TEST_01.md).

Abra essa pasta no PlatformIO. O comando de upload usa a porta detectada; caso necessário,
acrescente `--upload-port COMx`, substituindo pela porta real. Use o mesmo
procedimento de gravação serial/BOOT já utilizado nesse SolarView.

Não execute `uploadfs`: a interface está no firmware e a área de logs será
montada/inicializada pelo painel. Um upload de filesystem substituiria os logs.

As dependências foram fixadas em Espressif8266 PlatformIO 4.2.1,
Arduino ESP8266 3.1.2 (fornecido pela plataforma) e modbus-esp8266 4.1.0.

## Antes de aplicar limites

1. Conecte o SolarView ao barramento dos três equipamentos, mantendo os IDs
   acima e apenas um mestre controlando esse barramento durante o ensaio.
2. Conecte-se ao AP, abra `/config` e confira o identificador do firmware.
3. No primeiro uso, o log pode mostrar `UNAVAILABLE` por filesystem não
   inicializado. Com o teste parado, abra **Inicialização do armazenamento**,
   digite `FORMATAR` e confirme a formatação. Isso apaga somente a área LittleFS
   do perfil; não há formatação automática em erro de montagem.
4. Anote/fotografe no equipamento o limite original em W, em % e o estado de
   habilitação, conforme o que cada modelo expõe. Registre modelo, ID e firmware
   do equipamento. Não presuma que todos estavam originalmente em 100%.
5. Inicie uma sessão `01-baseline`. Os comandos permanecem desabilitados.
   Observe por 60–120 s. Confira nominal, serial, frequência e potência dos três
   equipamentos com a referência disponível no inversor. Nominal com origem
   `descriptor` é fallback, não prova de leitura Modbus.
6. Registre uma observação curta, encerre, aguarde `PARADO` e log `STOPPED`,
   atualize a lista e baixe o CSV. Confirme que ele abre antes de prosseguir.

O dashboard atualiza valores sem recarregar a página. Os campos de entrada não
são reconstruídos a cada consulta. Leituras antigas mostram sua idade;
`ready` significa inicialização, não comunicação recente.

## Sequência de ensaios

Faça **uma sessão por linha**, de preferência com 2–3 minutos, e baixe cada CSV
após encerrá-la. Todos os comandos são manuais; não existe varredura automática
de potência. Se um resultado for inesperado, pare nessa etapa e envie o arquivo.

| Ordem / nome sugerido | Ação | Evidência esperada |
| --- | --- | --- |
| 01-baseline | Somente leituras por 60–120 s | ID/serial corretos, valores plausíveis, sucessos crescentes e erros identificados |
| 02-id1-percent | Habilitar comandos; aplicar 100%, esperar resultado; aplicar 90%; observar; repor o valor original pelo mesmo formulário | FC06 `0x9CBD`; raw 1000 e 900; resultado do comando e potência posterior |
| 03-id2-percent | Repetir a sequência no ID2 | FC06 `0x9D6B=1`, seguida de FC06 `0x9D6C`; outros IDs continuam sendo lidos |
| 04-id3-percent | Repetir a sequência no ID3 | Mesmo mapa, evidência separada por ID/equipamento |
| 05-id1-watts | Aplicar a nominal confirmada em W; depois 0,90 × nominal; observar; repor limite W original | FC16 `0x9CBE`, **2 registradores**; ordem das palavras |
| 06-id2-watts | Aplicar nominal confirmada; depois 0,90 × nominal; observar e repor original | Conversão W → % e as duas escritas do SIW400G |
| 07-id3-watts | Repetir no ID3 | Mesma conversão, confirmada independentemente |
| 08-timeout-readonly | Comandos desabilitados; interrupção controlada apenas da comunicação RS485 de um equipamento e posterior reconexão | Timeout isolado, outros IDs continuam, dados antigos sinalizados, recuperação sem reboot |

Antes de cada passo de escrita, confirme que aquele ajuste é compatível com a
operação do local. Nos testes de comunicação, manuseie apenas a conexão de
comunicação acessível; não é necessário abrir o inversor nem cortar potência.
Se isolar um ID na topologia existente não for viável, deixe a linha 08 pendente.

**Não confundir estas três evidências:**

- `CMD_QUEUED`/HTTP 202: a aplicação aceitou o pedido, ainda não concluiu.
- `CMD_RESULT` com `DONE/SUCCESS`: a sequência Modbus concluiu. O log de
  transações mostra as etapas, inclusive enable bem-sucedido seguido de erro.
- Efeito operacional: limite visto no equipamento e comportamento de geração.
  Depende da disponibilidade solar, firmware e demais controles presentes.

90% de uma nominal de 30.000 W é 27.000 W. Se o inversor estiver gerando
20.000 W, não há motivo para cair ao aplicar esse teto. Nesse caso marque
“comunicação confirmada; efeito não demonstrado por geração abaixo do teto”.
Não conclua que o comando falhou nem aumente a agressividade do teste sozinho.

### Valores para conferir no CSV

| Caminho | Valor humano | Payload esperado |
| --- | --- | --- |
| Percentual, ambos os modelos | 100% / 90% | `03E8` / `0384` |
| SIW500H W, nominal confirmada 30 kW | 30.000 W / 27.000 W | `0000 7530` / `0000 6978` |
| SIW400G W, nominal confirmada 100 kW | 100.000 W / 90.000 W | `03E8` / `0384`, após enable `0001` |

No SIW400G a conta deve usar a nominal registrada naquela sessão:
`percentual = watts × 100 / nominal`; `raw = round(percentual / 0,1)`.
Se a nominal lida divergir da placa, suspenda comandos e registre a diferença.

No SIW500H, W e % são registros diferentes; sua precedência não foi demonstrada
pelo código. Reponha pelo mesmo caminho e confirme no equipamento. No SIW400G,
100% deixa enable ligado; retornar a 100% não restitui um estado original com
enable desligado. Essa reposição, se necessária, deve ser feita na interface do
equipamento, pois o setter legado de enable não foi exposto nesta campanha.

## O arquivo de testes

Os nomes são `/test-000001.csv`, `/test-000002.csv`, etc., escolhendo o próximo
número após os arquivos existentes. Não há sobrescrita/limpeza automática.
O arquivo tem 20 colunas:

```text
schema,session,seq,uptime_ms,event,command_id,slave_id,model,operation,status,modbus_status,function_code,address,register_count,words_hex,value,unit,elapsed_ms,source,detail
```

| Campos | Interpretação |
| --- | --- |
| `session`, `seq`, `uptime_ms` | Arquivo, ordem dos eventos e tempo monotônico do boot |
| `command_id` | Liga pedido, transações e resultado do comando |
| `slave_id`, `model`, `operation` | Alvo e função, sem adivinhar pelo endereço |
| `function_code`, `address`, `register_count`, `words_hex` | Transação real da biblioteca: FC em decimal (3, 6, 16), endereço/palavras em hex |
| `status`, `modbus_status` | Resultado da aplicação ou do transporte, conforme `event` |
| `value`, `unit`, `source` | Valor solicitado/lido e sua origem; nominal de descriptor identificada |
| `elapsed_ms`, `detail` | Duração, validade/aceitação da transação, diagnóstico ou nota |

O observador registra os registradores entregues/recebidos pelo transporte,
**não** captura o sinal elétrico, bytes completos da UART ou CRC. Em falha de
leitura, o payload é marcado inválido; em escrita, as palavras são o que foi
solicitado ao transporte, não prova de readback do inversor.

`HEALTH` registra heap, maior intervalo entre iterações/chamadas do bus,
duração máxima de chamadas de API e gravações, perdas de log e erros de armazenamento.
São indicadores para comparar funcionamento normal, comando e timeout; não
existe aprovação automática baseada em um limite arbitrário de latência.

O início registra a hora UTC fornecida pelo navegador, firmware/base e hardware.
O relógio do navegador não é certificado; use `uptime_ms` para ordenar a sessão.
Arquivos contêm serial dos equipamentos: envie-os neste chat para análise e não
os publique automaticamente no repositório.

## Armazenamento, falhas e limites deste ensaio

- Fila RAM limitada a oito linhas de até 510 bytes. Perdas aparecem no painel
  e invalidam a alegação de log completo. Não há criação dinâmica de filas.
- Até 192 KiB por arquivo; reserva de 16 KiB no filesystem e margem de 8 KiB
  para conclusões. O tempo disponível varia com o número de eventos; acompanhe
  o tamanho e baixe sessões curtas. Não é um logger de produção para dias.
- LittleFS grava/fecha sincronamente **entre operações**. O transporte Modbus
  é cooperativo, mas flash, HTTP e transmissão UART ainda consomem tempo.
  Nenhuma promessa de tempo real ou “zero bloqueio” é feita para a aplicação.
- Download, exclusão e formatação só com sessão encerrada e barramento ocioso.
  Download grande pode bloquear o servidor; não há teste Modbus ativo nesse período.
- Flush periódico acontece nas oportunidades ociosas. Falta de energia pode
  perder fila RAM e cauda ainda não persistida. Um arquivo sem `SESSION_STOP`
  não prova encerramento normal.
- No core ESP8266 3.1.2, `File::flush()` não expõe todo erro interno de sync.
  Escritas curtas e os erros visíveis são tratados, mas a durabilidade física
  ainda deve ser confirmada baixando/reabrindo o CSV
  após reiniciar. O firmware não formata para tentar “corrigir” um erro.
- Um erro pode ocorrer depois que o enable já foi escrito. O firmware não
  repete escritas automaticamente nem executa rollback/reposição de limites.

## O que devolver após testar

Envie os CSVs e, para cada sessão, diga: equipamento/ID, comando, valor original,
valor solicitado, valor visto no inversor, potência antes/depois, condição solar
e qualquer falha de tela, comunicação ou reinício. Foto do limite no equipamento
ajuda a separar confirmação de protocolo e comportamento real.

Depois revisaremos os logs desta etapa e escolheremos **um** próximo comando.
O primeiro candidato é `getPowerLimitPercent()` assíncrono para leitura de retorno.

## Verificação no computador

Os resultados de build e verificações executadas estão em
[VERIFICATION.md](VERIFICATION.md). Eles não substituem a campanha acima.

Proveniência do anexo original `Markdown colado.md`:
SHA-256 `3a394007352ee8fd11f9263a710cc533f31b72e0ee0936c73644f1095351183a`.
Snapshot C++ normalizado:
`ce4aa7b159cb0ff55de9af0e1a111b2d46e447d7854f5bd9df44276e7c580c8a`.
