# Comportamentos Conhecidos

## GoodWe

### RTC

Alguns modelos GoodWe armazenam os dados de tempo combinados em um mesmo registrador.

| Reg | Tempo | Posição |
| 0x0010 | Ano | MSB |
| 0x0010 | Mês | LSB |
| 0x0011 | Dia | MSB |
| 0x0011 | Hora | LSB |
| 0x0012 | Minuto | MSB |
| 0x0012 | Segundo | LSB |

A biblioteca trata esse comportamento usando `GOODWE_HANDLER`.

### Ano com dois dígitos

O ano é armazenado como dois dígitos.

Exemplo:

| Valor real | Valor Modbus |
|---|---:|
| 2020 | 20 |
| 2021 | 21 |
| 2026 | 26 |

A biblioteca trata esse comportamento usando `GOODWE_HANDLER`.

## Huawei / derivados

### Controle de limitação por enum

Alguns modelos utilizam registradores de controle em formato de enum, em vez de simples enable/disable.

Isso significa que o mesmo registrador pode representar diferentes modos de operação, como:

- desabilitado;
- limite por potência;
- limite por percentual.

## Campos não implementados

Alguns inversores retornam valores especiais para campos não suportados, como:

| Tipo | Valor |
|---|---:|
| uint16 | `0xFFFF` |
| int16 | `0x8000` |
| uint32 | `0xFFFFFFFF` |
| int32 | `0x80000000` |

---

## Comportamento da biblioteca

#### Boot e Shutdown

Alguns inversores utilizam apenas um único registrador para controle de estado (ligar/desligar).

Nestes casos:

O registrador deve ser mapeado em *_map.boot*
Não deve ser mapeado em *_map.shutdown*

A biblioteca trata automaticamente as operações de boot e shutdown utilizando os valores definidos em `BootMode`.

#### 

- Para os inversores já testados, os mapas realizam automaticamente a normalização das unidades utilizadas pela biblioteca. Exemplo: um inversor pode reportar potência em kW e outro em W; os campos scale realizam a conversão necessária, mesmo que a documentação original utilize unidades diferentes.
- Os endereços dos registradores são tratados como 0-based internamente. Dependendo da documentação do fabricante, podem existir divergências de endereçamento (0-based vs 1-based).
- switch no campo principal, mas o fallback depende de outro campo. (em ExportLimit, o fallback leva a ExportLimitPercent. Não é feita a verificação de FIELD_SIMPLE ou SPECIAL)
- Valores de energia podem ser expostos como float para conveniência da API. (getTotalEnergy)
- Para operações acumulativas de longo prazo, recomenda-se utilizar os valores brutos inteiros (uint32_t/int32_t) para evitar perda progressiva de precisão.
- Em inversores monofásicos, apenas a fase r possui valor válido para leitura de tensão da rede. As fases s e t permanecem zeradas. Em inversores trifásicos que disponibilizam apenas um registrador de frequência, o valor válido será atribuído somente à fase r.
- Alguns registradores podem variar entre firmwares/modelos.
- Alguns fabricantes utilizam escalas e endianness diferentes.
- O mapa Modbus ainda está em evolução e validação prática.
- Algumas funções de escrita ainda estão em validação e podem variar bastante entre fabricantes. Em certos modelos, diferentes recursos podem compartilhar o mesmo registrador de controle com valores enumerados distintos. Essas funções podem existir internamente, mas ainda não fazem parte da API pública recomendada.
- Alguns mapas ainda estão em validação prática. Certos fabricantes utilizam registradores diferentes para leitura e escrita, offsets 0-based/1-based, escalas diferentes ou campos parcialmente disponíveis.
- Current Modbus transaction layer is not thread-safe.