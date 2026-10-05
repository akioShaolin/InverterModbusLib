# Prompt para a etapa 02 — somente `getPowerLimitPercent()`

Use este prompt **depois** de entregar e revisar os logs do teste 01, e quando o
usuário autorizar a próxima etapa. A existência deste arquivo não autoriza
migrar antecipadamente outros comandos.

---

Você está trabalhando na InverterModbusLib. A campanha anterior está em
`documentation/testing/SOLARVIEW_TEST_01.md`; o inventário e os problemas
conhecidos estão em `documentation/testing/REVIEW_AND_NEXT_STEPS.md`.

**Objetivo único:** migrar `getPowerLimitPercent(float&)` para o contrato
não bloqueante da branch e incorporá-lo ao mesmo painel/logger de campo.
Não implementar o próximo item da lista durante esta tarefa.

## Antes de alterar código

1. Leia as instruções do repositório, o commit/branch atual, os dois documentos,
   `src/main.cpp`, `src/FieldTestLog.*`, `src/InverterModbusBus.*`, `src/Inverter.h`
   e a implementação/mapas envolvidos. Preserve alterações locais do usuário.
2. Leia o CSV **real** e o relato humano fornecidos para o teste 01. Identifique
   firmware, sessão, ID/modelo, perdas de log, recusas, exceções, timeouts,
   recuperação, latência e as transações de cada comando. O CSV tem 20 colunas;
   consulte o schema atual no código/roteiro, sem inventar nomes ou significados.
3. Diferencie: pedido HTTP aceito, escrita aceita pelo transporte, resposta
   Modbus de sucesso, leitura do setpoint e efeito observado no inversor.
   `DONE/SUCCESS` isolado não comprova redução de potência. 90% é da nominal.
4. Registre o que foi demonstrado e o que ficou inconclusivo por equipamento.
   Se os logs não chegaram, falta autorização para a etapa seguinte ou há falha
   que impede uma conclusão útil do teste 01, apresente o impedimento concreto
   e não anuncie a etapa 02 como validada. Não invente resultado de campo.

## Implementação delimitada

- Siga o padrão público atual: retorno `InverterRequestStatus` e request de
  leitura próprio, distinto de `REQ_SET_POWER_LIMIT_PERCENT`. Acrescente o novo
  ID sem renumerar os IDs existentes nos logs. C++ não permite sobrecarga
  diferenciada apenas pelo retorno: trate a mudança de assinatura e seus
  consumidores explicitamente. Preserve o contrato das outras APIs.
- Inicie a leitura sem esperar; continue em `INV_BUSY`; consuma o resultado
  apenas para owner/request correspondente; atualize a saída somente após
  sucesso; libere o barramento em término/erro. Uma recusa não deve deixar estado
  ou parâmetros retidos. Não use laços de espera, `delay`, wrappers bloqueantes
  ou `getRatedPower(uint32_t&)` dentro da nova operação.
- Nos equipamentos desta campanha: M3/ID1 lê `0x9CBD`; SIW400G/IDs2/3 leem
  `0x9D6C`. São campos U16, escala 0,1, FC03, um registrador. Obtenha esses dados
  do mapa. Não escreva enable, modo ou setpoint para realizar o readback.
- Preserve a semântica de modelos que só ofereçam W mediante caminho
  assíncrono e nominal válida, se aplicável. Cubra isso com fixture própria;
  não transforme fallback local em leitura remota confirmada. Não corrija nem
  amplie mapas de outras famílias para fabricar cobertura de hardware.
- Mantenha o SolarView atual: ESP8266, LED GPIO2, DE/RE GPIO12, comutação RS485
  GPIO13, mesma UART, níveis de habilitação, AP e 9600/8N1. Preserve os três IDs,
  a configuração de flash/LittleFS e as salvaguardas existentes. Nenhum hardware
  adicional deve ser necessário.
- Preserve a fila RAM limitada do logger, reservas de espaço, contadores de
  perda, nomes/arquivos de sessão, flush com barramento ocioso, download e o
  schema CSV de 20 colunas. Faça cópia do observador para RAM; não grave flash,
  use rede nem reentre no barramento dentro de seu callback. Os diagnósticos
  vêm das palavras reais da transação, não de cálculo inverso da UI.
- Acrescente o readback ao scheduler mantendo um pedido no barramento, comando
  manual prioritário e limite pendente único. Preserve `/`, `/config`, as rotas
  e métodos HTTP atuais, o fluxo de sessão e o download. Mostre valor, idade da
  leitura e resultado, sem apresentar valor antigo como confirmação nova.
- Audite os consumidores do getter: não trate `INV_BUSY` ou `INV_ERROR` como
  `true`. Ajuste os pontos atingidos pelo novo contrato; mantenha exemplos
  legados explicitamente identificados. Não migre o FullWebPanel inteiro.
- Não alterar export limit, boot/shutdown, reativo, fator de potência,
  configurações do inversor ou as outras leituras. Não adicionar controle
  automático nem sequência automática de escritas.

## Verificação e entrega

1. Acrescente testes host relevantes com código de produção: BUSY → DONE,
   dados e escala dos dois mapas; chamada concorrente; resultado ainda não
   consumido; timeout/exceção/recusa; saída preservada em erro; recuperação;
   campo ausente e eventual fallback. Garanta FC03 sem escrita e correlação
   correta no observador. Reexecute as regressões existentes afetadas.
2. Compile o ambiente SolarView atual. Registre comando, resultado, RAM/flash e
   limitações. Testes com stubs não validam UART/CRC, Wi-Fi, flash ou inversores.
3. Produza `documentation/testing/SOLARVIEW_TEST_02.md` com o ensaio mínimo:
   baseline de leitura; um comando manual já autorizado; readback posterior
   identificado; observação no equipamento; restauração e download. Se precisar
   repetir escrita, o usuário a aciona; o firmware não escreve ao iniciar.
4. No SIW400G, compare o percentual lido após ambos os caminhos W/%; o retorno
   em W é conversão, não um registrador nativo. No M3, não presuma precedência
   entre os limites W/% nem equivalência de seus readbacks sem evidência.
5. Atualize a matriz de migração somente para esta função, preservando as
   distinções entre implementado, teste host, ensaio pendente e validado em
   equipamento específico. Entregue diff, arquivos e instruções de build/upload
   necessárias; informe claramente o que depende do próximo CSV do usuário.

Pare após preparar e verificar esta etapa. `isPowerLimitEnabled()` será uma
tarefa posterior, condicionada à revisão dos novos logs e à decisão do usuário.
