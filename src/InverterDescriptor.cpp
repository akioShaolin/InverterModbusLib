/*
 * InverterModbusLib - Solar Inverter Library for Arduino
 * ------------------------------------------------
 * Modbus RTU (RS485)communication layer for inverter integration
 *
 * Copyright (c) 2026, Pedro Akio Sakuma
 * Licensed under BSD 3-Clause License
 */

#include "InverterDescriptor.h"
#include "InverterModels.h"

const InverterDescriptor getDescriptor(InverterModel model) {
    InverterDescriptor desc;

    switch (model) {
        case SIW200_M030:
        case SIW200_M050:
        case SIW200_M085:
        case SIW200_M100:
        case SIW200H_M050_W00:
        case SIW200H_M075_W10:
        case SIW200H_M105_W10:
        case SIW200G_M030_W0:
        case SIW200G_M030_W1:
        case SIW200G_M050_W0:
        case SIW200G_M050_W1:
        case SIW200G_M060_W0:
        case SIW200G_M060_W1:
        case SIW200G_M070_W1:
        case SIW200G_M075_W1:
        case SIW200G_M080_W1:
        case SIW200G_M090_W1:
        case SIW200G_M105_W1:
        case SIW300H_M030_W00:
        case SIW300H_M050_W00:
        case SIW300H_M060_W00:
        case SIW300H_M075_W00:
        case SIW300H_M080_W00:
        case SIW300H_M100_W00:
        case SIW400_ST075:
        case SIW400G_T012_W0:
        case SIW400G_T012_W1:
        case SIW400G_T015_W0:
        case SIW400G_T015_W1:
        case SIW400G_T020_W0:
        case SIW400G_T020_W1:
        case SIW400G_T025_W0:
        case SIW400G_T025_W1:
        case SIW400G_T075_W0:
        case SIW400G_T100_W0:
        case SIW400G_T050_W00:
        case SIW400G_T060_W00:
        case SIW400G_T075_W01:
        case SIW400G_K015_W00:
        case SIW400G_K020_W00:
        case SIW400G_K025_W00:
        case SIW400G_K030_W00:
        case SIW400G_K037_W00:
        case SIW400H_T015_W10:
        case SIW400H_T030_W10:
        case SIW420G_K025_W00:
        case SIW420G_K075_W00:
        case SIW500G_T075_W0:
        case SIW500G_T100_W0:
        case SIW500G_K050_W00:
        case SIW500G_K075_W00:
        case SIW500G_H250_W0:
        case SIW500H_ST012_M2:
        case SIW500H_ST015_M2:
        case SIW500H_ST020_M2:
        case SIW500H_ST030_M3:
        case SIW500H_ST036_M3:
        case SIW500H_ST040_M3:
        case SIW500H_SK020:
        case SIW500H_ST030:
        case SIW500H_ST036:
        case SIW500H_ST040:
        case SIW500H_ST060:
        case SIW500H_ST100:
        case SIW500H_ST060_HV:
        case SIW500H_ST100_HV:
        case SIW500H_ST200_H0:
        case SIW500H_ST200_H3:
        case SIW500H_T012_W00:
        case SIW500H_T015_W00:
        case SIW500H_T017_W00:
        case SIW500H_T020_W00:
        case SIW500H_T025_W00:
        case SIW600_T020_44:
        case SIW610_T018_W0:
        case SIW610_T075_W0:
            return getDescriptor_Weg(model);

        case S700_G2:
        case S1000_G2:
        case S1500_G2:
        case S2000_G2:
        case S2500_G2:
        case S3000_G2:
        case S3300_G2:
        case F3000_G2:
        case F3600_G2:
        case F4600_G2:
        case F5000_G2:
        case F5300_G2:
        case F6000_G2:
        case G7:
        case G7_5:
        case G8_:
        case G9:
        case G10:
        case G10_5:
        case T3_G3:
        case T4_G3:
        case T5_G3:
        case T6_G3:
        case T8_G3:
        case T10_G3:
        case T12_G3:
        case T15_G3:
        case T17_G3:
        case T20_G3:
        case T23_G3:
        case T25_G3:
        case V30:
        case V33:
        case V36:
        case V40:
        case V50:
        case V60:
        case V75:
        case VL15:
        case VL20:
        case VL25:
        case VL30:
        case VL37_5:
        case R75:
        case R100:
        case R110:
            return getDescriptor_FoxEss(model);

        case GW3000_XS:
        case GW3300_XS_30:
        case GW5000_DNS:
        case GW5K_DNS_G40:
        case GW8500_MS:
        case GW10K_MS:
        case GW75K_MT:
            return getDescriptor_GoodWe(model);

        case SUN2000_2KTL:
        case SUN2000_3KTL:
        case SUN2000_4KTL:
        case SUN2000_5KTL:
        case SUN2000_6KTL:
        case SUN2000_12KTL_M0:
        case SUN2000_12KTL_M2:
        case SUN2000_15KTL_M0:
        case SUN2000_15KTL_M2:
        case SUN2000_20KTL_M0:
        case SUN2000_20KTL_M2:
        case SUN2000_30KTL:
        case SUN2000_36KTL_A:
        case SUN2000_30KTL_M3:
        case SUN2000_36KTL_M3:
        case SUN2000_40KTL_M3:
        case SUN2000_60KTL_M0:
        case SUN2000_75KTL_M1:
        case SUN2000_100KTL_M2:
        case SUN2000_100KTL_H1:
        case SUN2000_215KTL_H0:
        case SUN2000_215KTL_H3:
        case SUN2000_250KTL_H1:
            return getDescriptor_Huawei(model);
            
        default:
            desc = InverterDescriptor{};
            return desc;
    }
    desc = InverterDescriptor{};
    return desc;
}