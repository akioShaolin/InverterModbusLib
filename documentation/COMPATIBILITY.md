# Compatibilidade

Este documento lista os modelos e famílias de inversores considerados pela InverterModbusLib.

A compatibilidade pode estar em diferentes níveis de validação. Nem todos os modelos listados foram testados fisicamente.

## Legenda

| Status | Significado |
|---|---|
| Validado | Testado fisicamente e funcionando nas principais funções |
| Parcialmente validado | Algumas funções foram testadas e funcionaram |
| Mapa encontrado | Há mapa disponível, mas ainda sem validação prática |
| Esperado por família | Deve funcionar por compartilhar mapa com modelo testado |
| Pendente | Teste pendente ou inconclusivo |
| Não suportado | Modelo/função não suportado no momento |

## Modelos Testados

| Modelo | Família | Status | Observações |
|---|---|---|---|
| SIW400G T100 W0 | SIW400G | Parcialmente validado | Grande parte das leituras funcionando |
| SIW500H ST030 M3 | SIW500H M3 | Parcialmente validado | Limitação de potência/exportação funcionando |
| SIW500H ST015 M0 | SIW500H | Parcialmente validado | Limitação de potência/exportação funcionando |
| GW3000N-XS | GoodWe XS | Parcialmente validado | Data/hora usa tratamento especial GoodWe |
| GW5000-DNS | GoodWe DNS | Parcialmente validado | Mapa em revisão |
| GW10K-MS | GoodWe MS | Parcialmente validado | Energia total pendente de revisão |
| SUN2000-30KTL-M3 | Huawei M3 | Parcialmente validado | Limitação de potência/exportação funcionando |
| SUN2000-15KTL-M0 | Huawei M0 | Parcialmente validado | Limitação de potência/exportação funcionando |
| R100 | FoxESS/WEG | Parcialmente validado | Grande parte das leituras funcionando |

## Modelos Pendentes

| Modelo | Status | Observações |
|---|---|---|
| SIW200H M050 W00 | Pendente | Mapa possivelmente diferente do SIW400G |
| SIW400H T030 W10 | Pendente | Teste inconclusivo |
| SIW420G K075 W00 | Mapa encontrado | Validação pendente |
| H3-PRO | Pendente | Teste inconclusivo |
| GW75K-MT | Pendente | Equipamento disponível com defeito |

## Compatibilidade por Família

Alguns modelos podem compartilhar o mesmo mapa Modbus.

Exemplo:

| Família | Modelos | Status |
|---|---|---|
| SIW500H M3 | ST020 M3, ST030 M3, ST036 M3, ST040 M3 | Esperado por família |
| Huawei M3 | SUN2000-30KTL-M3, SUN2000-36KTL-M3, SUN2000-40KTL-M3 | Parcialmente validado |
| GoodWe DNS | GW5000-DNS e variantes próximas | Parcialmente validado |

Modelos marcados como “esperado por família” ainda devem ser testados fisicamente sempre que possível.

## Todos os modelos

### Weg
---
SIW200_M030, SIW200_M050, SIW200_M085, SIW200_M100, SIW200H_M050_W00, SIW200H_M075_W10, SIW200H_M105_W10, SIW200G_M030_W0, SIW200G_M030_W1, SIW200G_M050_W0, SIW200G_M050_W1, SIW200G_M060_W0, SIW200G_M060_W1, SIW200G_M070_W1, SIW200G_M075_W1, SIW200G_M080_W1, SIW200G_M090_W1, SIW200G_M105_W1, SIW300H_M030_W00, SIW300H_M050_W00, SIW300H_M060_W00, SIW300H_M075_W00, SIW300H_M080_W00, SIW300H_M100_W00, SIW400_ST075, SIW400G_T012_W0, SIW400G_T012_W1, SIW400G_T015_W0, SIW400G_T015_W1, SIW400G_T020_W0, SIW400G_T020_W1, SIW400G_T025_W0, SIW400G_T025_W1, SIW400G_T075_W0, SIW400G_T100_W0, SIW400G_T050_W00, SIW400G_T060_W00, SIW400G_T075_W01, SIW400G_K015_W00, SIW400G_K020_W00, SIW400G_K025_W00, SIW400G_K030_W00, SIW400G_K037_W00, SIW400H_T015_W10, SIW400H_T030_W10, SIW420G_K025_W00, SIW420G_K075_W00, SIW500G_T075_W0, SIW500G_T100_W0, SIW500G_K050_W00, SIW500G_K075_W00, SIW500G_H250_W0, SIW500H_ST012_M2, SIW500H_ST015_M2, SIW500H_ST020_M2, SIW500H_ST030_M3, SIW500H_ST036_M3, SIW500H_ST040_M3, SIW500H_SK020, SIW500H_ST030, SIW500H_ST036, SIW500H_ST040, SIW500H_ST060, SIW500H_ST100, SIW500H_ST060_HV, SIW500H_ST100_HV, SIW500H_ST200_H0, SIW500H_ST200_H3, SIW500H_T012_W00, SIW500H_T015_W00, SIW500H_T017_W00, SIW500H_T020_W00, SIW500H_T025_W00, SIW600_T020_44,SIW610_T018_W0, SIW610_T075_W0
    
### FoxEss
---
S700_G2, S1000_G2, S1500_G2, S2000_G2, S2500_G2, S3000_G2, S3300_G2, F3000_G2, F3600_G2, F4600_G2, F5000_G2, F5300_G2, F6000_G2, G7, G7_5, G8_, G9, G10, G10_5, T3_G3, T4_G3, T5_G3, T6_G3, T8_G3, T10_G3, T12_G3, T15_G3, T17_G3, T20_G3, T23_G3, T25_G3, V30, V33, V36, V40, V50, V60, V75, VL15, VL20, VL25, VL30, VL37_5, R75, R100, R110

### GoodWe
---
GW3000_XS, GW3300_XS_30, GW5000_DNS, GW5K_DNS_G40, GW8500_MS, GW10K_MS,

### Huawei
---
SUN2000_2KTL, SUN2000_3KTL, SUN2000_4KTL, SUN2000_5KTL, SUN2000_6KTL, SUN2000_12KTL_M0, SUN2000_12KTL_M2, SUN2000_15KTL_M0, SUN2000_15KTL_M2, SUN2000_20KTL_M0, SUN2000_20KTL_M2, SUN2000_30KTL, SUN2000_36KTL_A, SUN2000_30KTL_M3, SUN2000_36KTL_M3, SUN2000_40KTL_M3, SUN2000_60KTL_M0, SUN2000_75KTL_M1, SUN2000_100KTL_M2, SUN2000_100KTL_H1, SUN2000_215KTL_H0, SUN2000_215KTL_H3, SUN2000_250KTL_H1

## Observação

A compatibilidade pode variar conforme:

- versão de firmware;
- região;
- fabricante homologador;
- protocolo habilitado;
- permissões de escrita;
- configuração do inversor;
- tipo de datalogger/comunicação disponível.