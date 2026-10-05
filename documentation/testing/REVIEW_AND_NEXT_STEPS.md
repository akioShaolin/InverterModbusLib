# Revisão e sequência de migração

Revisão de 2026-10-05. Base: `refactor/nonblocking-modbus`, commit
`eaa6006f`, comparada ao `main.cpp` enviado pelo usuário. A campanha atual é
[SolarView — teste 01](SOLARVIEW_TEST_01.md). Nenhum ensaio físico foi realizado
durante esta revisão. Compilação, testes no computador e teste em inversor são
evidências diferentes.

## O que foi retomado

O anexo, preservado em `solarview-main-received.cpp.txt`, estava à frente do
`src/main.cpp` versionado: separava `/config` do dashboard, retornava para essa
página após comandos, mostrava `field.length` e calculava o diagnóstico W → %
com a nominal obtida. Era Markdown escapado; a cópia preservada foi normalizada
para texto C++, sem transformar o arquivo recebido em uma validação de campo.

O relatório do usuário registrava `setPowerLimitPercent(100)` com
`DONE/SUCCESS`, endereço `0x9D6C` e raw 1000. Isso sustenta um resultado pontual
de comunicação relatado, não a validação de todas as escritas, modelos ou da
redução de geração. As quatro leituras foram informadas como já validadas pelo
usuário; esta campanha recolhe nova evidência da combinação atual de firmware,
barramento e equipamentos.

| Área examinada | Arquivos e conclusão para esta campanha |
| --- | --- |
| Aplicação | `src/main.cpp`, `src/WebPage.h`, anexo preservado: painel de bancada e interface anterior; aproveitar o fluxo de configuração separado. |
| Transporte e conversões | `src/InverterModbusBus.*`, `src/InverterCore.cpp`, `src/ModbusConfig.*`: barramento compartilhado assíncrono convive com helpers bloqueantes legados. |
| API e comandos | `src/Inverter.h`, `src/InverterControl.cpp`, `src/InverterDeviceInfo.cpp`, `src/InverterTime.cpp`, `src/InverterInternalHelpers.cpp`, `src/InverterModbusLib.h`: seis operações públicas têm retorno assíncrono; o restante exige inventário e migração gradual. |
| Modelos e mapas | `src/InverterMaps*`, `src/InverterDescriptor*`, `src/InverterModels.h`, `src/InverterFeatures.h`, `src/ModbusField.h`: descriptors não garantem mapa ativo; a base contém mapas históricos comentados e seletores incompletos. |
| Exemplos e histórico | `examples/FullWebPanel/*`, `examples/ReadSerialTwoInverters/*`, `resources/*`, `functions.xlsx`: referência do uso anterior; as assinaturas, capturas e marcas de suporte não comprovam o refactor assíncrono. |
| Projeto e documentação | `platformio.ini`, `library.properties`, `keywords.txt`, `README*`, `LICENSE.txt`, `documentation/{API,COMPATIBILITY,KNOWN_BEHAVIORS,ROADMAP,VALIDATION,README}.md`: parte descreve a API anterior. O header e esta matriz são a referência desta campanha. |

O observador acrescentado em `InverterModbusBus` registra transações reais da
biblioteca: ID, request, FC03/06/16, endereço, quantidade de registradores,
palavras, duração e resultado. Não é captura elétrica da UART, frame completo
ou CRC. O CSV de teste tem 20 colunas; seu contrato e uso estão no roteiro.
`FieldTestLog.*` copia eventos para uma fila limitada em RAM e grava com o
barramento ocioso. Flash e HTTP continuam tendo custos síncronos mensuráveis;
não se deve chamar toda a aplicação de tempo real por ter Modbus assíncrono.

## Correção necessária antes do ensaio

Havia retenção de comando recusado: uma chamada de limite preparava e guardava
valor/etapas antes de perceber que o barramento estava ocupado. Após
`INV_REJECTED`, uma nova solicitação poderia executar o valor antigo ou ser
recusada pelo request retido. `src/InverterControl.cpp` agora verifica ocupação
antes de preparar uma solicitação nova. A transação já aceita continua sendo
consumida pelo seu owner/request até terminar.

Também foram acrescentadas rejeição de NaN/infinito, checagem das escalas e dos
limites de conversão numérica, além de limpeza do diagnóstico anterior quando
uma nova solicitação começa. `tests/host/` cobre o comando antigo, concorrência,
FC06/FC16, enable antes do setpoint, nominal medida no fallback, falhas e
recuperação. `tests/host_bus/` cobre os eventos do observador. Essas regressões
usam código de produção com transporte simulado; não substituem o teste físico.

## Mapas que o teste 01 exercita

| ID / modelo | Percentual | Watts | Nominal |
| --- | --- | --- | --- |
| 1 — `SIW500H_ST030_M3` | `0x9CBD`, U16 × 0,1, FC06 | `0x9CBE`, U32 × 1, **2 regs**, FC16 | `0x7579`, U32 × 1; descriptor 30.000 W |
| 2 e 3 — `SIW400G_T100_W0` | Enable `0x9D6B=1`, depois `0x9D6C`, U16 × 0,1; duas FC06 | W → % usando nominal em cache; mesmo enable/setpoint | `0x9D23`, U16 × 100; descriptor 100.000 W |

`field.length=1` representa um valor nesse U32, não um registrador. O log deve
usar a contagem efetivamente enviada. Para 90%, espera-se raw 900; o teto é
90% da nominal, e não 90% da geração naquele instante. Geração já abaixo do teto
não permite concluir que houve redução por causa do comando.

W e % usam registradores distintos no M3; a precedência precisa de observação
no equipamento. Restaurar pelo mesmo caminho testado e conferir o resultado.
Nos SIW400G, escrever 100% mantém o enable em 1: não equivale a restaurar um
estado anterior desabilitado. O teste não altera automaticamente esses estados.

## Inventário completo da API pública de `Inverter.h`

Inventário por assinatura: **93 declarações**, incluindo o construtor. São
6 operações assíncronas, 14 métodos de infraestrutura/consulta local,
71 operações legadas que podem bloquear e 2 declarações sem implementação.
Uma operação legada pode retornar imediatamente em um fallback ou erro; isso
não torna seu caminho de comunicação não bloqueante.

### Já têm contrato assíncrono

Retornam `InverterRequestStatus`; comparar explicitamente com `INV_DONE`,
`INV_BUSY`, `INV_ERROR` e `INV_REJECTED`. Não converter o enum em `bool`.

| Assinatura | Situação de campo informada |
| --- | --- |
| `getGridFrequency(float&)` | Validada pelo usuário; repetir como regressão |
| `getActivePower(float&)` | Validada pelo usuário; repetir como regressão |
| `getRatedPower(float&)` | Validada pelo usuário; registrar origem real/fallback |
| `getSerialNumber(char*, size_t)` | Validada pelo usuário; repetir como regressão |
| `setPowerLimit(float)` | Em validação no teste 01 |
| `setPowerLimitPercent(float)` | Em validação no teste 01 |

### Infraestrutura e consultas locais

`Inverter(InverterModel)`, `attachBus(InverterModbusBus&)`,
`attachModbus(ModbusRTU&)`, `attachConfig(const ModbusConfigData&)`,
`attachSerial(HardwareSerial&)`, `begin()`, `setSlaveId(uint8_t)`, `task()`,
`isBusy() const`, `isDone() const`, `hasError() const`,
`getLastModbusStatus() const`, `getRatedPowerSpec(float&) const`,
`wasLastRatedPowerFallback() const`.

Não são 14 comandos remotos a converter. Devem preservar configuração,
ownership e consultas de estado, sem reinicializar UART durante transações.

### Operações legadas pendentes

Todas as assinaturas abaixo ainda retornam `bool`.

| Grupo | Assinaturas completas a acompanhar |
| --- | --- |
| Limites de potência: leitura | `isPowerLimitEnabled(bool&)`, `getPowerLimit(float&)`, `getPowerLimitPercent(float&)` |
| Diagnóstico | `getTemperature(float&)`, `getInsulationResistance(float&)` |
| Energia | `getTotalEnergy(float&)`, `getDailyEnergy(float&)` |
| Outras potências | `getReactivePower(float&)`, `getApparentPower(float&)`, `getPowerFactor(float&)` |
| Identificação legada | `getSerialNumber(String&)`, `getModelId(uint16_t&)`, `getModelName(String&)`, `getFirmwareVersion(String&)`, `getRatedPower(uint32_t&)`, `getPVStringCount(uint16_t&)`, `getMpptCount(uint16_t&)` |
| Grandezas AC | `getGridVoltage(float&)`, `getGridPhaseVoltage(PhaseData&)`, `getGridLineVoltage(PhaseData&)`, `getGridCurrent(float&)`, `getGridCurrent(PhaseData&)` |
| Strings FV | `getStringVoltage(StringValues&)`, `getStringCurrent(StringValues&)`, `getStringPower(StringValues&)` |
| Estado e alarmes | `getInverterStatus(uint32_t&)`, `getAlarm(uint32_t&)` |
| Relógio: leitura | `getYear(uint16_t&)`, `getMonth(uint16_t&)`, `getDay(uint16_t&)`, `getHour(uint16_t&)`, `getMinute(uint16_t&)`, `getSecond(uint16_t&)`, `getEpochTime(uint32_t&)` |
| Relógio: escrita | `setYear(uint16_t)`, `setMonth(uint16_t)`, `setDay(uint16_t)`, `setHour(uint16_t)`, `setMinute(uint16_t)`, `setSecond(uint16_t)`, `setEpochTime(uint32_t)` |
| Liga/desliga e enable | `boot()`, `shutdown()`, `setBoot(bool)`, `setPowerLimitEnabled(bool)` |
| Export limit: leitura | `isExportLimitEnabled(bool&)`, `getExportLimit(float&)`, `getExportLimitPercent(float&)` |
| Export limit: escrita | `setExportLimitEnabled(bool)`, `setExportLimit(float)`, `setExportLimitPercent(float)` |
| Fator de potência: leitura | `isPowerFactorEnabled(bool&)`, `getPowerFactorSetpoint(float&)` |
| Fator de potência: escrita | `setPowerFactorEnabled(bool)`, `setPowerFactorSetpoint(float)`, `setPowerFactorExcitationMode(PfExcitationMode)` |
| Reativo fixo | `isFixedReactiveEnabled(bool&)`, `getFixedReactiveSetpoint(float&)`, `setFixedReactiveEnabled(bool)`, `setFixedReactiveSetpoint(float)` |
| Bateria | `getBatteryVoltage(BatteryValues&)`, `getBatteryCurrent(BatteryValues&)`, `getBatteryPower(BatteryValues&)`, `getBatterySoC(BatteryValues&)`, `getBatterySoH(BatteryValues&)` |
| EPS | `getEPSVoltage(float&)`, `getEPSVoltage(PhaseData&)`, `getEPSCurrent(float&)`, `getEPSCurrent(PhaseData&)`, `getEPSActivePower(float&)`, `getEPSActivePower(PhaseData&)` |

`getDateTime(Datetime&)` e `setDateTime(const Datetime&)` estão declarados no
header, mas suas definições em `InverterTime.cpp` estão comentadas. São lacunas
de implementação, não funções operantes aguardando apenas troca de retorno.

## Sequência de campanhas

Cada linha é uma prioridade; **cada função da linha terá sua própria etapa**.

1. Concluir teste 01: seis operações atuais, isolamento entre inversores,
   timeout/recuperação, limite manual e logs baixáveis.
2. `getPowerLimitPercent()`; depois `isPowerLimitEnabled()`; depois
   `getPowerLimit()`. Primeiro obter readback assíncrono útil para as escritas.
3. `getTemperature()`, `getDailyEnergy()`, `getTotalEnergy()`, depois
   `getInsulationResistance()`, `getReactivePower()`, `getApparentPower()` e
   `getPowerFactor()`: escalas, sinal, um/dois registradores e campo indisponível.
4. Identificação restante; tensões/correntes AC; strings FV. Validar contagens,
   stride, fases e buffers com modelos realmente disponíveis.
5. Status/alarmes e leitura de relógio, após resolver formato/tamanho e leitura
   coerente da data. As escritas de relógio ficam em campanha posterior.
6. Enable, boot/shutdown, reativo, fator de potência e export limit, com plano
   operacional próprio para cada comando. Export limit também depende da
   medição externa e de sua configuração; não integra o teste 01.
7. Bateria/EPS e fabricantes restantes, quando houver mapas e hardware adequados.
   Encerrar wrappers legados e atualizar exemplos/documentação conforme a
   política de compatibilidade escolhida no chat da arquitetura.

Em **cada etapa**: revisar o campo e seu contrato; implementar uma operação;
compilar; executar regressões host relevantes; gerar um firmware identificável
com o mesmo logger; receber CSV e observações humanas; separar resposta Modbus,
readback e efeito; registrar resultado por modelo/firmware. Só avançar ao próximo
comando depois da revisão dos logs e do acordo com o usuário. Os logs do teste 01
e a autorização da próxima etapa são pré-condições do
[prompt seguinte](CODEX_NEXT_COMMAND.md).

## Backlog encontrado, sem correção ampla nesta campanha

- **Mapas incompletos:** `getMap_Weg()` retorna `true` sem copiar `out` para
  alguns grupos. `getMap_GoodWe()` retorna `false`; seleção FoxESS/Huawei está
  comentada. A existência do enum/descriptor não autoriza testar outro modelo.
- **Flags:** export limit M3 declara enable suportado com campo inválido e mode
  não suportado com campo válido/obrigatório. Reativo fixo SIW400G exige mode,
  mas `controlMode` é inválido. Revisar antes de expor esses comandos.
- **Nominal legada:** `getRatedPower(uint32_t&)` tenta `readField(uint32_t*)`,
  que só aceita U32. No SIW400G o campo é U16, então cai no descriptor; o overload
  float assíncrono lê corretamente. Não usá-lo escondido no próximo getter.
- **Tipos/formatos:** comentário de `ModbusField.length` contraditório;
  alarmes M3 são cinco U16, mas `getAlarm` aceita apenas length 1; no SIW400G o
  mapa é U32 e o getter usa buffer U16. A interpretação de status também precisa
  de confirmação por modelo. Revisar sentinelas NaN e precisão de energia.
- **Descriptors a conferir:** `SIW500H_ST060_HV` registra 600.000 W;
  `SIW400G_T060_W00` e `T075_W01` registram 50.000 W. Não são os três modelos
  desta campanha. Conferir fontes do fabricante antes de corrigir.
- **Data/hora:** `setYear()` testa `year < 2000 && year > 2099`, condição
  impossível. Revisar esse intervalo, datas inválidas, ano bissexto, registros
  compartilhados e leitura/modificação/escrita. `getEpochTime()` não oferece
  fallback de campos separados, e os setters parciais não alteram epoch.
- **Compatibilidade:** `documentation/API.md` e exemplos antigos ainda tratam
  operações como `bool` e citam assinaturas antigas. Um enum `BUSY` verdadeiro
  em contexto booleano não significa sucesso. Não misturar helpers bloqueantes
  no scheduler atual para fazer um botão antigo funcionar.

O objetivo final é eliminar esperas Modbus bloqueantes dos caminhos públicos
suportados. Isso exige completar a matriz acima e a política dos wrappers; não
é demonstrado por compilar o painel ou testar apenas os dois modelos atuais.
