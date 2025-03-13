/*******************************************************************************
  Metrology Driver.

  Company:
    Microchip Technology Inc.

  File Name:
    drv_mcmetrology.c

  Summary:
    Metrology Driver source file.

  Description:
    None
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*
Copyright (C) 2025, Microchip Technology Inc., and its subsidiaries. All rights reserved.

The software and documentation is provided by microchip and its contributors
"as is" and any express, implied or statutory warranties, including, but not
limited to, the implied warranties of merchantability, fitness for a particular
purpose and non-infringement of third party intellectual property rights are
disclaimed to the fullest extent permitted by law. In no event shall microchip
or its contributors be liable for any direct, indirect, incidental, special,
exemplary, or consequential damages (including, but not limited to, procurement
of substitute goods or services; loss of use, data, or profits; or business
interruption) however caused and on any theory of liability, whether in contract,
strict liability, or tort (including negligence or otherwise) arising in any way
out of the use of the software and documentation, even if advised of the
possibility of such damage.

Except as expressly permitted hereunder and subject to the applicable license terms
for any third-party software incorporated in the software and any applicable open
source software license terms, no license or other rights, whether express or
implied, are granted under any patent or other intellectual property rights of
Microchip or any third party.
*/
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "peripheral/rstc/plib_rstc.h"
#include "system/int/sys_int.h"
#include "drv_mcmetrology_configuration.h"
#include "drv_mcmetrology.h"
#include "drv_mcmetrology_definitions.h"
#include "drv_mcmetrology_local.h"
#include "peripheral/pio/plib_pio.h"
#include "peripheral/rstc/plib_rstc.h"
#include "peripheral/clk/plib_clk.h"
#include "interrupts.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

typedef enum {
    PENERGY = 0U,
    QENERGY = 2U,
} DRV_MCMETROLOGY_ENERGY_TYPE;

/* This is the driver instance object array. */
static DRV_MCMETROLOGY_OBJ gDrvMCMetObj;

static const DRV_MCMETROLOGY_REGS_CONTROL gDrvMCMetControlDefault =
{
    STATE_CTRL_STATE_CTRL_RESET_Val,                       /* 00 Metrology state control register */
    DRV_MCMETROLOGY_CONF_FCTRL0,                           /* 01 Metrology feature setting register 0 */
    DRV_MCMETROLOGY_CONF_FCTRL1,                           /* 02 Metrology feature setting register 1 */
    0x00000000UL,                                          /* 03 Reserved */
    DRV_MCMETROLOGY_CONF_POWER_SOURCE,                     /* 04 Power source configuration */
    DRV_MCMETROLOGY_CONF_MT,                               /* 20 Meter type settings register */
    DRV_MCMETROLOGY_CONF_ST,                               /* 21 Channel type and Sensor Configuration Registers */
    0x00000000UL,                                          /* 23 M M=50->50Hz M=60->60Hz */
    0x00001130UL,                                          /* 24 N_MAX 4400=0x1130 */
    DRV_MCMETROLOGY_CONF_PULSE0_CTRL,                      /* 25 PULSE0_CTRL */
    DRV_MCMETROLOGY_CONF_PULSE1_CTRL,                      /* 26 PULSE1_CTRL */
    DRV_MCMETROLOGY_CONF_PULSE2_CTRL,                      /* 27 PULSE2_CTRL */
    0x00000000UL,                                          /* 28 Reserved */
    DRV_MCMETROLOGY_CONF_PULSE_CONT_PW,                    /* 29 PULSE_CONTRIBUTION_PW */
    DRV_MCMETROLOGY_CONF_PULSE_CONT_I,                     /* 30 PULSE_CONTRIBUTION_I */
    0x00000000UL,                                          /* 31 Reserved */
    DRV_MCMETROLOGY_CONF_PKT,                              /* 32 P_K_T */
    DRV_MCMETROLOGY_CONF_QKT,                              /* 33 Q_K_T */
    DRV_MCMETROLOGY_CONF_IKT,                              /* 34 I_K_T */
    DRV_MCMETROLOGY_CONF_CREEP_THR_P,                      /* 35 CREEP_THRESHOLD_P */
    DRV_MCMETROLOGY_CONF_CREEP_THR_Q,                      /* 36 CREEP_THRESHOLD_Q */
    DRV_MCMETROLOGY_CONF_CREEP_THR_I,                      /* 37 CREEP_THRESHOLD_I */
    DRV_MCMETROLOGY_CONF_P_POWER_OFFSET_CTRL,              /* 38 P_POWER_OFFSET_CTRL */
    DRV_MCMETROLOGY_CONF_Q_POWER_OFFSET_CTRL,              /* 39 Q_POWER_OFFSET_CTRL */
    DRV_MCMETROLOGY_CONF_POWER_OFFSET_P,                   /* 40 POWER_OFFSET_P */
    DRV_MCMETROLOGY_CONF_POWER_OFFSET_Q,                   /* 41 POWER_OFFSET_Q */
    DRV_MCMETROLOGY_CONF_SWELL_OVERC_THRS_CHx,             /* 42 SWELL_OVERCURRENT_THRESHOLD_CHx */
    DRV_MCMETROLOGY_CONF_SAG_THRS_CHx,                     /* 74 SAG_THRESHOLD_CHx */
    DRV_MCMETROLOGY_CONF_K_CHx,                            /* 106 K_CH_x */
    DRV_MCMETROLOGY_CONF_CAL_M_CHx,                        /* 138 CAL_M_CHx */
    DRV_MCMETROLOGY_CONF_CAL_PH_CHx,                       /* 170 CAL_PH_CHx */
    DRV_MCMETROLOGY_CONF_CAPTURE_CTRL,                     /* 202 CAPTURE_CTRL */
    DRV_MCMETROLOGY_CONF_CAPTURE_CH_SEL,                   /* 203 CAPTURE_CH_SEL */
    DRV_MCMETROLOGY_CAPTURE_BUF_SIZE,                      /* 204 CAPTURE_BUFF_SIZE */
    0x00000000UL,                                          /* 205 CAPTURE_ADDR */
    {0x00000000UL,                                         /* 206 RESERVED */
    0x00000000UL,                                          /* 207 RESERVED */
    0x00000000UL},                                         /* 208 RESERVED */
    DRV_MCMETROLOGY_CONF_MCP3914_GAIN,                     /* 209 MCP3914_GAIN */
    DRV_MCMETROLOGY_CONF_MCP3914_CONFIG,                   /* 213 MCP3914_CONFIG0 */
    0x00000000UL,                                          /* 217 RESERVED */
    DRV_MCMETROLOGY_CONF_POWER_OFFSET_P_x,                 /* 218 POWER_OFFSET_P_x */
    0x00000000UL,                                          /* 249 RESERVED */
    DRV_MCMETROLOGY_CONF_POWER_OFFSET_Q_x,                 /* 250 POWER_OFFSET_Q_x */
    0x00000000UL                                           /* 281 RESERVED */
};

DRV_MCMETROLOGY_AFE_TYPE gDrvAFEType = 0;

static const char gDrvChannel0Name[] = "V0";
static const char gDrvChannel1Name[] = "V1";
static const char gDrvChannel2Name[] = "V2";
static const char gDrvChannel3Name[] = "I1";
static const char gDrvChannel4Name[] = "I2";
static const char gDrvChannel5Name[] = "I3";
static const char gDrvChannel6Name[] = "I4";
static const char gDrvChannel7Name[] = "I5";
static const DRV_MCMETROLOGY_CHANNEL gDrvMCMetChannelsDefault[DRV_MCMETROLOGY_CHANNELS_NUMBER] =
{
    {(char *)&gDrvChannel0Name, GAIN_1, SENSOR_VRD},
    {(char *)&gDrvChannel1Name, GAIN_1, SENSOR_VRD},
    {(char *)&gDrvChannel2Name, GAIN_1, SENSOR_VRD},
    {(char *)&gDrvChannel3Name, GAIN_1, SENSOR_CT},
    {(char *)&gDrvChannel4Name, GAIN_1, SENSOR_CT},
    {(char *)&gDrvChannel5Name, GAIN_1, SENSOR_CT},
    {(char *)&gDrvChannel6Name, GAIN_1, SENSOR_CT},
    {(char *)&gDrvChannel7Name, GAIN_1, SENSOR_CT},
};

static const DRV_MCMETROLOGY_POWER_SOURCE gDrvMCMetPowersDefault[DRV_MCMETROLOGY_POWERS_NUMBER] =
{
    {0U, 3U},
    {1U, 4U},
    {2U, 5U},
};


static const uint32_t statusTestData[] =
{
    0x40000,
    0x8,
    0x20,
    0x5,
    0x82,
    0x0,
    0x7,
    0x0,
    0x1C7A005C,
    0xFA0,
    0xFF0A3D71,
    0x31FFE,
    0x31FFE,
    0x32022,
    0x32027,
    0x0,
    0x0,
    0x0,
    0x0,
    0x37890,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x350C4A0,
    0x1A5CF2,
    0x20F390,
    0x150C54,
    0x7E5E,
    0x6FFF,
    0x4EBF,
    0x63DE,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x807F6C00,
    0x0,
    0x0,
    0x0,
    0x1D1,
    0x1D2,
    0x0,
    0x0,
    0x0,
    0xF9F465,
    0xF8A456,
    0xF8A3A1,
    0x0,
    0x0,
    0x0,
    0x0,
    0xF4ACCD,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
};

static const uint64_t accTestData[] =
{
    0x1525D5ADA282,
    0xD41442A6,
    0x15759C18C,
    0x22CF5A30D,
    0x22D96,
    0x2255D,
    0x259DB,
    0x22F47,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x1525C54B385F,
    0x9321897D,
    0xE79AEA16,
    0x22D0EA14D,
    0x2030,
    0x2839,
    0x1FE6,
    0x19F0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x4CC00195A8,
    0x4563B,
    0x9F306,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x4CC1CE3DC8,
    0x34497,
    0x1DE655,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x4CBB06ACB1,
    0xFFFFFFFFFFFB11DE,
    0x6CFA4,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x4CBFA92743,
    0xFFFFFFFFFFF933BF,
    0x1AD16,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x14B6DE803684,
    0x13FDB92A,
    0x149556D00CEC,
    0x14B6EA309914,
    0xDA8EEAD,
    0x14955FA0F138,
    0xFFFFFFDDA0BF95A0,
    0xFFFFFFDD90791A6D,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0xFFFFFFDD8F1E22AC,
    0xFFFFFFDD90791A6D,
    0xFFFFFFDD9F63EC38,
    0xFFFFFFDDA0BF95A0,
    0x0,
};

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************
bool getMeasures = true;

void IPC1_InterruptHandler (void)
{
    uint32_t status = IPC1_REGS->IPC_ISR;
    status &= IPC1_REGS->IPC_IMR;

    if ((status & DRV_MCMETROLOGY_IPC_INIT_IRQ_MSK) != 0UL)
    {
        if (gDrvMCMetObj.metRegisters->MET_STATUS.STATUS == STATUS_STATUS_RESET)
        {
            gDrvMCMetObj.status = DRV_MCMETROLOGY_STATUS_INIT_DSP;
        }
    }

    if ((status & DRV_MCMETROLOGY_IPC_INTEGRATION_IRQ_MSK) != 0UL)
    {
        if (gDrvMCMetObj.metRegisters->MET_STATUS.STATUS == STATUS_STATUS_DSP_RUNNING)
        {
            if (getMeasures)
            {
                getMeasures = false;
                
                /* Update Accumulators Data */
                (void) memcpy(&gDrvMCMetObj.metAccData, &accTestData, sizeof(DRV_MCMETROLOGY_REGS_ACCUMULATORS));
//                (void) memcpy(&gDrvMCMetObj.metAccData, &gDrvMCMetObj.metRegisters->MET_ACCUMULATORS, sizeof(DRV_MCMETROLOGY_REGS_ACCUMULATORS));
                /* Store samples in period */
                gDrvMCMetObj.samplesInPeriod = statusTestData[9];
//                gDrvMCMetObj.samplesInPeriod = gDrvMCMetObj.metRegisters->MET_STATUS.N;

                /* Update Channel Data */
                (void) memcpy((uint8_t *)gDrvMCMetObj.freqChx, (uint8_t *)&statusTestData[12], sizeof(gDrvMCMetObj.freqChx));
                (void) memcpy((uint8_t *)gDrvMCMetObj.maxChx, (uint8_t *)&statusTestData[46], sizeof(gDrvMCMetObj.maxChx));
//                (void) memcpy((uint8_t *)gDrvMCMetObj.freqChx, (uint8_t *)&gDrvMCMetObj.metRegisters->MET_STATUS.FREQ_CHx[0], sizeof(gDrvMCMetObj.freqChx));
//                (void) memcpy((uint8_t *)gDrvMCMetObj.maxChx, (uint8_t *)&gDrvMCMetObj.metRegisters->MET_STATUS.CHx_MAX[0], sizeof(gDrvMCMetObj.maxChx));

                /* Update Frequency Data */
                gDrvMCMetObj.freq = statusTestData[11];
//                gDrvMCMetObj.freq = gDrvMCMetObj.metRegisters->MET_STATUS.FREQ;

                if (gDrvMCMetObj.harmonicAnalysisData.holdRegs == false)
                {
                    /* Update Harmonics Data */
                    (void) memcpy(&gDrvMCMetObj.metHarData, &gDrvMCMetObj.metRegisters->MET_HARMONICS, sizeof(DRV_MCMETROLOGY_REGS_HARMONICS));
                }
            }
        }

        gDrvMCMetObj.integrationFlag = true;
    }

    IPC1_REGS->IPC_ICCR = status;

    gDrvMCMetObj.ipcInterruptFlag = true;

}

static double lDRV_Mcmetrology_GetHarmonicRMS(int32_t real, int32_t imag)
{
    double rmsRe, rmsIm, rms;
    uint32_t div;

    rmsRe = (double)real;
    rmsIm = (double)imag;
    rms = 2 * (rmsRe * rmsRe + rmsIm * rmsIm);
    rms = sqrt(rms);
    div = (1UL << 6) * gDrvMCMetObj.samplesInPeriod;
    rms = rms / (double)div;

    return rms;
}

static float lDRV_Mcmetrology_GetVIRMS(uint64_t val, uint32_t k_x)
{
    double m;
    double k;

    // Convert uQ24.40 format to double
    m = (double)val / (double)(1LL << FORMAT_CONST_uQ2440);
    // Convert uQ22.10 format to double
    k = (double)k_x / (double)(1UL << FORMAT_CONST_uQ2210);

    m = m / (double)gDrvMCMetObj.samplesInPeriod;
    m = sqrt(m);
    m = m * k;

    return (float)m;
}

static float lDRV_Mcmetrology_GetPQ(int64_t val, uint32_t k_ix, uint32_t k_vx)
{
    double m;
    double mult;

    // Convert sQ23.40 format to double
    m = (double)val / (double)(1LL << FORMAT_CONST_sQ2340);

    m = m / (double)gDrvMCMetObj.samplesInPeriod;
    mult = (double)k_ix * (double)k_vx;
    m = (m * mult) / (double)(1UL << (FORMAT_CONST_uQ2210 << 1));

    return (float)m;
}

static float lDRV_Mcmetrology_GetAngle(int64_t p, int64_t q)
{
    double angle, pd, qd;

    pd = (double)p;
    qd = (double)q;
    angle = atan2(qd, pd);
    angle = 180.0 * angle;
    angle = angle / CONST_Pi;

    return (float)angle;
}

static float lDRV_Mcmetrology_GetEnergy(DRV_MCMETROLOGY_ENERGY_TYPE id)
{
    double energy;
    uint8_t index;
    
    energy = 0.0;
    for (index = 0; index < DRV_MCMETROLOGY_POWERS_NUMBER; index++)
    {
        energy += gDrvMCMetObj.metAFEData.powMeasure[index][id];
    }

    energy = energy / SECS_IN_HOUR_DOUBLE;   /* (P[Wh]; Q[Varh] */

    return (float)energy;
}

static void lDRV_Mcmetrology_IpcInitialize (void)
{
    /* Clear interrupts */
    IPC1_REGS->IPC_ICCR = 0xFFFFFFFFUL;
    /* Enable interrupts */
    IPC1_REGS->IPC_IECR = DRV_MCMETROLOGY_IPC_INIT_IRQ_MSK |
        DRV_MCMETROLOGY_IPC_INTEGRATION_IRQ_MSK;
}

static void lDRV_MCMETROLOGY_UpdateMeasurements(void)
{
    DRV_MCMETROLOGY_REGS_ACCUMULATORS *pAccData = NULL;
    DRV_MCMETROLOGY_REGS_CONTROL *pMetControl = NULL;
    DRV_MCMETROLOGY_AFE_EVENTS *pAFEEvents = NULL;
    float *pDstData = NULL;
    float pt = 0;
    float ptf = 0;
    float qt = 0;
    float qtf = 0;
    uint32_t kxData;
    uint8_t index;

    pAccData = &gDrvMCMetObj.metAccData;
    pMetControl = &gDrvMCMetObj.metRegisters->MET_CONTROL;
    pAFEEvents = &gDrvMCMetObj.metAFEData.events;

    /* Update Accumulated Voltage/Current/Frequency/Max Quantities per Channel */
    for (index = 0; index < DRV_MCMETROLOGY_CHANNELS_NUMBER; index++)
    {
        pDstData = (float *)&gDrvMCMetObj.metAFEData.chnMeasure[index];

        /* Improve accuracy and apply format values to Freq and Max values */
        *(pDstData + CHN_MEASURE_FREQ) = (float)gDrvMCMetObj.freqChx[index] / (1 << FORMAT_CONST_uQ2012);
        *(pDstData + CHN_MEASURE_MAX) = (float)gDrvMCMetObj.maxChx[index] / (1 << FORMAT_CONST_sQ229);

        kxData = pMetControl->K_CHx[index];
        *(pDstData + CHN_MEASURE_RMS) = lDRV_Mcmetrology_GetVIRMS(pAccData->CHx[index], kxData);
        *(pDstData + CHN_MEASURE_RMS_F) = lDRV_Mcmetrology_GetVIRMS(pAccData->CHx_F[index], kxData);
    }

    /* Update Accumulated Active/Reactive Quantities per Power */
    for (index = 0; index < DRV_MCMETROLOGY_POWERS_NUMBER; index++)
    {
        uint32_t kIData;
        float value;
        uint8_t channel;

        pDstData = (float *)&gDrvMCMetObj.metAFEData.powMeasure[index];

        channel = gDrvMCMetPowersDefault[index].vChannel;
        kxData = pMetControl->K_CHx[channel];

        channel = gDrvMCMetPowersDefault[index].iChannel;
        kIData = pMetControl->K_CHx[channel];

        value = lDRV_Mcmetrology_GetPQ(pAccData->P_CHx[index], kIData, kxData);
        *(pDstData + POW_MEASURE_P) = value;
        pt += value;

        value = lDRV_Mcmetrology_GetPQ(pAccData->P_CHx_F[index], kIData, kxData);
        *(pDstData + POW_MEASURE_P_F) = value;
        ptf += value;

        value = lDRV_Mcmetrology_GetPQ(pAccData->Q_CHx[index], kIData, kxData);
        *(pDstData + POW_MEASURE_Q) = value;
        qt += value;

        value = lDRV_Mcmetrology_GetPQ(pAccData->Q_CHx_F[index], kIData, kxData);
        *(pDstData + POW_MEASURE_Q_F) = value;
        qtf += value;

        *(pDstData + POW_MEASURE_ANGLE) = lDRV_Mcmetrology_GetAngle(pAccData->P_CHx[index], pAccData->Q_CHx[index]);
    }

    pDstData = gDrvMCMetObj.metAFEData.measure;

    /* Update Accumulated Voltage Quantities Between Phases. It assumes all KVx are the same */
    kxData = pMetControl->K_CHx[gDrvMCMetPowersDefault[0].vChannel];
    *(pDstData + MEASURE_V_AB) = lDRV_Mcmetrology_GetVIRMS(pAccData->V_AB, kxData);
    *(pDstData + MEASURE_V_BC) = lDRV_Mcmetrology_GetVIRMS(pAccData->V_BC, kxData);
    *(pDstData + MEASURE_V_CA) = lDRV_Mcmetrology_GetVIRMS(pAccData->V_CA, kxData);
    *(pDstData + MEASURE_V_AB_F) = lDRV_Mcmetrology_GetVIRMS(pAccData->V_AB_F, kxData);
    *(pDstData + MEASURE_V_BC_F) = lDRV_Mcmetrology_GetVIRMS(pAccData->V_BC_F, kxData);
    *(pDstData + MEASURE_V_CA_F) = lDRV_Mcmetrology_GetVIRMS(pAccData->V_CA_F, kxData);

    /* Update Frequency */
    *(pDstData + MEASURE_FREQ) = (float)gDrvMCMetObj.freq / (1 << FORMAT_CONST_uQ2012);

    /* Update Total Accumulated Quantities */
    *(pDstData + MEASURE_ACC_TO) = (float)pAccData->ACC_T0 / (1 << FORMAT_CONST_sQ3330);
    *(pDstData + MEASURE_ACC_T1) = (float)pAccData->ACC_T1 / (1 << FORMAT_CONST_sQ3330);
    *(pDstData + MEASURE_ACC_T2) = (float)pAccData->ACC_T2 / (1 << FORMAT_CONST_sQ3330);

    /* Update Active/Reactive Total Accumulated Quantities */
    *(pDstData + MEASURE_PT) = pt;
    *(pDstData + MEASURE_PT_F) = ptf;
    *(pDstData + MEASURE_QT) = qt;
    *(pDstData + MEASURE_QT_F) = qtf;

    /* Update Total Accumulated Energy */
    *(pDstData + MEASURE_ENERGY) += lDRV_Mcmetrology_GetEnergy(PENERGY);

    /* Update events */
    pAFEEvents->swell = (uint16_t)gDrvMCMetObj.metRegisters->MET_STATUS.SWELL_OVERCURRENT_FLAG;
    pAFEEvents->sag = (uint16_t)gDrvMCMetObj.metRegisters->MET_STATUS.SAG_FLAG;
    pAFEEvents->creep = (uint16_t)gDrvMCMetObj.metRegisters->MET_STATUS.CREEP_FLAG;
    pAFEEvents->phActive = (uint16_t)gDrvMCMetObj.metRegisters->MET_STATUS.PH_ACTIVE_FLAG;
}

static void lDRV_MCMETROLOGY_UpdateHarmonicAnalysisValues(void)
{
    DRV_MCMETROLOGY_HARMONICS_RMS *pHarmonicsRsp = gDrvMCMetObj.harmonicAnalysisData.pHarmonicAnalysisResponse;
    DRV_MCMETROLOGY_REGS_HARMONICS *pHarData = &gDrvMCMetObj.metHarData;
    int32_t real, imag;
    uint8_t channel;

    for (channel = 0; channel < DRV_MCMETROLOGY_CHANNELS_NUMBER; channel++)
    {
        real = pHarData->CHx_m_R[channel];
        imag = pHarData->CHx_m_I[channel];
        pHarmonicsRsp->CHx_m_RMS[channel] = lDRV_Mcmetrology_GetHarmonicRMS(real, imag);
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Driver Common Interface Implementation
// *****************************************************************************
// *****************************************************************************

SYS_MODULE_OBJ DRV_MCMETROLOGY_Initialize (SYS_MODULE_INIT * init, uint32_t resetCause)
{
    /* MISRA C-2012 deviation block start */
    /* MISRA C-2012 Rule 11.3 deviated once. Deviation record ID - H3_MISRAC_2012_R_11_3_DR_1 */
    DRV_MCMETROLOGY_INIT *metInit = (DRV_MCMETROLOGY_INIT *)init;
    /* MISRA C-2012 deviation block end */

    if ((gDrvMCMetObj.inUse == true) || (init == NULL))
    {
        return SYS_MODULE_OBJ_INVALID;
    }

	/* Clean the IPC interrupt generic flag */
    gDrvMCMetObj.ipcInterruptFlag = false;

    /* Disable IPC interrupts */
    (void) SYS_INT_SourceDisable(IPC1_IRQn);

	/* Clean the IPC interrupt flags */
    gDrvMCMetObj.integrationFlag = false;

    if (resetCause != RSTC_SR_RSTTYP(RSTC_SR_RSTTYP_WDT0_RST_Val))
    {
        uint32_t *pSrc;
        uint32_t *pDst;

        /* Assert reset of the coprocessor */
        RSTC_CoProcessorEnable(false);

        /* Disable coprocessor Clocks */
        CLK_Core1ProcessorClkDisable();

        gDrvMCMetObj.binStartAddress = metInit->binStartAddress;
        gDrvMCMetObj.binSize = metInit->binEndAddress - metInit->binStartAddress;

        /* Copy the Metrology bin file to SRAM1 */
        pSrc = (uint32_t *)gDrvMCMetObj.binStartAddress;
        pDst = (uint32_t *)IRAM1_ADDR;
        (void) memcpy(pDst, pSrc, gDrvMCMetObj.binSize);
    }

    /* Initialization of the interface with Metrology Lib */
    gDrvMCMetObj.metRegisters = (MCMET_REGISTERS *)metInit->regBaseAddress;
    gDrvMCMetObj.inUse = true;
    gDrvMCMetObj.integrationCallback = NULL;

    (void) memset(&gDrvMCMetObj.metAccData, 0, sizeof(DRV_MCMETROLOGY_REGS_ACCUMULATORS));
    (void) memset(&gDrvMCMetObj.metHarData, 0, sizeof(DRV_MCMETROLOGY_REGS_HARMONICS));
    (void) memset(&gDrvMCMetObj.metAFEData, 0, sizeof(DRV_MCMETROLOGY_AFE_DATA));

    (void) memcpy(gDrvMCMetObj.channel, gDrvMCMetChannelsDefault, sizeof(gDrvMCMetChannelsDefault));
    (void) memcpy(gDrvMCMetObj.power, gDrvMCMetPowersDefault, sizeof(gDrvMCMetPowersDefault));

    /* Initialization of the Metrology object */
    gDrvMCMetObj.status = DRV_MCMETROLOGY_STATUS_HALT;

    /* Configure IPC peripheral */
    lDRV_Mcmetrology_IpcInitialize();

    return (SYS_MODULE_OBJ)&gDrvMCMetObj;
}

SYS_MODULE_OBJ DRV_MCMETROLOGY_Reinitialize (SYS_MODULE_INIT * init)
{
    /* MISRA C-2012 deviation block start */
    /* MISRA C-2012 Rule 11.3 deviated once. Deviation record ID - H3_MISRAC_2012_R_11_3_DR_1 */
    DRV_MCMETROLOGY_INIT *metInit = (DRV_MCMETROLOGY_INIT *)init;
    /* MISRA C-2012 deviation block end */
    uint32_t *pSrc;
    uint32_t *pDst;

    if ((gDrvMCMetObj.inUse == false) || (init == NULL))
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    /* Disable IPC interrupts */
    (void) SYS_INT_SourceDisable(IPC1_IRQn);

    /* Assert reset of the coprocessor and its peripherals */
    RSTC_CoProcessorEnable(false);
    RSTC_CoProcessorPeripheralEnable(false);

    /* Disable coprocessor Clocks */
    CLK_Core1ProcessorClkDisable();

    gDrvMCMetObj.binStartAddress = metInit->binStartAddress;
    gDrvMCMetObj.binSize = metInit->binEndAddress - metInit->binStartAddress;
    
    /* De-assert the reset of the peripherals (Core 1) */
    RSTC_CoProcessorPeripheralEnable(true);

    /* Copy the Metrology bin file to SRAM1 */
    pSrc = (uint32_t *)gDrvMCMetObj.binStartAddress;
    pDst = (uint32_t *)IRAM1_ADDR;
    (void) memcpy(pDst, pSrc, gDrvMCMetObj.binSize);

    /* Initialization of the interface with Metrology Lib */
    gDrvMCMetObj.metRegisters = (MCMET_REGISTERS *)metInit->regBaseAddress;

    (void) memset(&gDrvMCMetObj.metAccData, 0, sizeof(DRV_MCMETROLOGY_REGS_ACCUMULATORS));
    (void) memset(&gDrvMCMetObj.metHarData, 0, sizeof(DRV_MCMETROLOGY_REGS_HARMONICS));
    (void) memset(&gDrvMCMetObj.metAFEData, 0, sizeof(DRV_MCMETROLOGY_AFE_DATA));

    (void) memcpy(gDrvMCMetObj.channel, gDrvMCMetChannelsDefault, sizeof(gDrvMCMetChannelsDefault));
    (void) memcpy(gDrvMCMetObj.power, gDrvMCMetPowersDefault, sizeof(gDrvMCMetPowersDefault));

    gDrvMCMetObj.afeType = (DRV_MCMETROLOGY_AFE_TYPE)0;

    /* Initialization of the Metrology object */
    gDrvMCMetObj.status = DRV_MCMETROLOGY_STATUS_HALT;

    lDRV_Mcmetrology_IpcInitialize();

    return (SYS_MODULE_OBJ)&gDrvMCMetObj;
}

DRV_MCMETROLOGY_RESULT DRV_MCMETROLOGY_Open (DRV_MCMETROLOGY_START_MODE mode, DRV_MCMETROLOGY_REGS_CONTROL * pConfiguration)
{
    if (gDrvMCMetObj.inUse == false)
    {
        return DRV_MCMETROLOGY_ERROR;
    }

    if (gDrvMCMetObj.status != DRV_MCMETROLOGY_STATUS_HALT)
    {
        return DRV_MCMETROLOGY_ERROR;
    }

    gDrvMCMetObj.status = DRV_MCMETROLOGY_STATUS_WAITING_IPC;

    /* Enable IPC1 Interrupt Source */
    SYS_INT_SourceStatusClear(IPC1_IRQn);
    SYS_INT_SourceEnable(IPC1_IRQn);

    if (mode == DRV_MCMETROLOGY_START_HARD)
    {
        /* Enable the coprocessor clock (Core 1) */
        CLK_Core1ProcessorClkEnable();
        
        /* De-assert the reset of the coprocessor (Core 1) */
        RSTC_CoProcessorEnable(true);

        /* Wait IPC Init interrupt */
        while(gDrvMCMetObj.status == DRV_MCMETROLOGY_STATUS_WAITING_IPC)
        {
        }

        /* Keep Metrology Lib in reset */
        gDrvMCMetObj.metRegisters->MET_CONTROL.STATE_CTRL = STATE_CTRL_STATE_CTRL_RESET_Val;

        if (pConfiguration != NULL)
        {
            /* Overwrite STATE CTRL register */
            pConfiguration->STATE_CTRL = STATE_CTRL_STATE_CTRL_RESET_Val;
            /* Load external Control values */
            (void) memcpy(&gDrvMCMetObj.metRegisters->MET_CONTROL, pConfiguration, sizeof(DRV_MCMETROLOGY_REGS_CONTROL));
        }
        else
        {
            /* Load default Control values */
            (void) memcpy(&gDrvMCMetObj.metRegisters->MET_CONTROL, &gDrvMCMetControlDefault, sizeof(DRV_MCMETROLOGY_REGS_CONTROL));
        }

        /* Set Metrology Lib state as Init */
        gDrvMCMetObj.metRegisters->MET_CONTROL.STATE_CTRL = STATE_CTRL_STATE_CTRL_INIT_Val;

        while(gDrvMCMetObj.metRegisters->MET_STATUS.STATUS != STATUS_STATUS_READY)
        {
        }
    }

    gDrvMCMetObj.status = DRV_MCMETROLOGY_STATUS_READY;

    return DRV_MCMETROLOGY_SUCCESS;
}

DRV_MCMETROLOGY_RESULT DRV_MCMETROLOGY_Close (void)
{
    if (gDrvMCMetObj.inUse == false)
    {
        return DRV_MCMETROLOGY_ERROR;
    }

    /* Disable IPC1 Interrupt Source */
    (void) SYS_INT_SourceDisable(IPC1_IRQn);
    NVIC_ClearPendingIRQ(IPC1_IRQn);

    /* Keep Metrology Lib in reset */
    gDrvMCMetObj.metRegisters->MET_CONTROL.STATE_CTRL = STATE_CTRL_STATE_CTRL_RESET_Val;
    /* Wait until the metrology resets */
    while (gDrvMCMetObj.metRegisters->MET_STATUS.STATUS != STATUS_STATUS_RESET)
    {
    }

    /* Update Driver state */
    gDrvMCMetObj.status = DRV_MCMETROLOGY_STATUS_HALT;

    return DRV_MCMETROLOGY_SUCCESS;

}

DRV_MCMETROLOGY_RESULT DRV_MCMETROLOGY_Start (void)
{
    if (gDrvMCMetObj.metRegisters->MET_STATUS.STATUS == STATUS_STATUS_READY)
    {
        /* Set Metrology Lib state as Run */
        gDrvMCMetObj.metRegisters->MET_CONTROL.STATE_CTRL = STATE_CTRL_STATE_CTRL_RUN_Val;
        /* Wait until the metrology running */
        while (gDrvMCMetObj.metRegisters->MET_STATUS.STATUS != STATUS_STATUS_DSP_RUNNING)
        {
        }
        gDrvMCMetObj.status = DRV_MCMETROLOGY_STATUS_RUNNING;

        return DRV_MCMETROLOGY_SUCCESS;
    }

    return DRV_MCMETROLOGY_ERROR;
}

DRV_MCMETROLOGY_STATUS DRV_MCMETROLOGY_GetStatus(void)
{
    return gDrvMCMetObj.status;
}

DRV_MCMETROLOGY_RESULT DRV_MCMETROLOGY_IntegrationCallbackRegister (
    DRV_MCMETROLOGY_CALLBACK callback
)
{
    if (callback == NULL)
    {
        return DRV_MCMETROLOGY_ERROR;
    }

    gDrvMCMetObj.integrationCallback = callback;
    return DRV_MCMETROLOGY_SUCCESS;
}


DRV_MCMETROLOGY_RESULT DRV_MCMETROLOGY_HarmonicAnalysisCallbackRegister (
    DRV_MCMETROLOGY_HARMONICS_CALLBACK callback
)
{
    if (callback == NULL)
    {
        return DRV_MCMETROLOGY_ERROR;
    }

    gDrvMCMetObj.harmonicAnalysisCallback = callback;
    return DRV_MCMETROLOGY_SUCCESS;
}

void DRV_MCMETROLOGY_Tasks(SYS_MODULE_OBJ object)
{
    if (object == SYS_MODULE_OBJ_INVALID)
    {
        /* Invalid system object */
        return;
    }

    if (gDrvMCMetObj.ipcInterruptFlag == false)
    {
        /* There are not IPC interrupts */
        return;
    }

    /* Clear IPC interrupt flag */
    gDrvMCMetObj.ipcInterruptFlag = false;

    if (gDrvMCMetObj.integrationFlag == true)
    {
        gDrvMCMetObj.integrationFlag = false;

        if (gDrvMCMetObj.harmonicAnalysisData.integrationPeriods > 0U)
        {
            gDrvMCMetObj.harmonicAnalysisData.integrationPeriods--;
        }

        /* Update measurements from metrology library registers */
        lDRV_MCMETROLOGY_UpdateMeasurements();

        /* Launch integration callback */
        if (gDrvMCMetObj.integrationCallback != NULL)
        {
            gDrvMCMetObj.integrationCallback();
        }

        /* Check if there is a harmonic analysis process running */
        if ((gDrvMCMetObj.harmonicAnalysisData.running == true) &&
            (gDrvMCMetObj.harmonicAnalysisData.integrationPeriods == 0U))
        {
            /* Prevent updating of harmonic registers */
            gDrvMCMetObj.harmonicAnalysisData.holdRegs = true;

            lDRV_MCMETROLOGY_UpdateHarmonicAnalysisValues();
            /* Launch Harmonic Analysis callback */
            if (gDrvMCMetObj.harmonicAnalysisCallback != NULL)
            {
                gDrvMCMetObj.harmonicAnalysisCallback(gDrvMCMetObj.harmonicAnalysisData.harmonicNumReq);
            }

            /* Reset harmonic registers update flag */
            gDrvMCMetObj.harmonicAnalysisData.holdRegs = false;
        }
    }
}

DRV_MCMETROLOGY_AFE_TYPE DRV_MCMETROLOGY_GetAFEDescription (void)
{
    return gDrvMCMetObj.afeType;
}

DRV_MCMETROLOGY_CHANNEL * DRV_MCMETROLOGY_GetChannelDescription (void)
{
    return (DRV_MCMETROLOGY_CHANNEL * )&gDrvMCMetChannelsDefault;
}

DRV_MCMETROLOGY_POWER_SOURCE * DRV_MCMETROLOGY_GetPowersDescription (void)
{
    return (DRV_MCMETROLOGY_POWER_SOURCE *)&gDrvMCMetPowersDefault;
}

DRV_MCMETROLOGY_REGS_STATUS * DRV_MCMETROLOGY_GetStatusData (void)
{
    return &gDrvMCMetObj.metRegisters->MET_STATUS;
}

DRV_MCMETROLOGY_REGS_CONTROL * DRV_MCMETROLOGY_GetControlData (void)
{
    return &gDrvMCMetObj.metRegisters->MET_CONTROL;
}

DRV_MCMETROLOGY_REGS_CONTROL * DRV_MCMETROLOGY_GetControlByDefault (void)
{
    /* MISRA C-2012 Rule 11.8 deviated below. Deviation record ID -
      H3_MISRAC_2012_R_11_8_DR_1*/
    return (DRV_MCMETROLOGY_REGS_CONTROL *)&gDrvMCMetControlDefault;
   /* MISRAC 2012 deviation block end */
}

DRV_MCMETROLOGY_REGS_ACCUMULATORS * DRV_MCMETROLOGY_GetAccData (void)
{
    return &gDrvMCMetObj.metAccData;
}

DRV_MCMETROLOGY_REGS_HARMONICS * DRV_MCMETROLOGY_GetHarData (void)
{
    return &gDrvMCMetObj.metHarData;
}

void DRV_MCMETROLOGY_SetControl (DRV_MCMETROLOGY_REGS_CONTROL * pControl)
{
    /* MISRA C-2012 Rule 11.8 deviated below. Deviation record ID -
      H3_MISRAC_2012_R_11_8_DR_1*/
    /* Keep State Control Register value */
    (void) memcpy((void *)&gDrvMCMetObj.metRegisters->MET_CONTROL.FEATURE_CTRL0,
                  (void *)&pControl->FEATURE_CTRL0,
                  sizeof(DRV_MCMETROLOGY_REGS_CONTROL) - sizeof(uint32_t));
   /* MISRAC 2012 deviation block end */
}

float DRV_MCMETROLOGY_GetEnergyValue(bool restartEnergy)
{
    float energy = gDrvMCMetObj.metAFEData.measure[MEASURE_ENERGY];

    if (restartEnergy == true)
    {
        gDrvMCMetObj.metAFEData.measure[MEASURE_ENERGY] = 0.0;
    }

    return energy;
}

float DRV_MCMETROLOGY_GetChannelMeasureValue(uint8_t channel, DRV_MCMETROLOGY_CHANNEL_MEASURE_TYPE type)
{
    float value;

    if ((channel < DRV_MCMETROLOGY_CHANNELS_NUMBER) & (type < CHN_MEASURE_TYPE_NUM))
    {
        value = gDrvMCMetObj.metAFEData.chnMeasure[channel][type];
    }
    else
    {
        value = 0.0;
    }

    return value;
}

float DRV_MCMETROLOGY_GetPowerMeasureValue(uint8_t power, DRV_MCMETROLOGY_POWER_MEASURE_TYPE type)
{
    float value;

    if ((power < DRV_MCMETROLOGY_POWERS_NUMBER) & (type < POW_MEASURE_TYPE_NUM))
    {
        value = gDrvMCMetObj.metAFEData.powMeasure[power][type];

        if (type == POW_MEASURE_ANGLE)
        {
            /* Absolute value should be between 0 and 180 degrees */
            while ((value > 180.0) == true)
            {
                value -= 360.0;
            }
        }
    }
    else
    {
        value = 0.0;
    }

    return value;
}

float DRV_MCMETROLOGY_GetMeasureValue(DRV_MCMETROLOGY_MEASURE_TYPE type)
{
    float value;

    if (type < MEASURE_TYPE_NUM)
    {
        value = gDrvMCMetObj.metAFEData.measure[type];
    }
    else
    {
        value = 0.0;
    }

    return value;
}

void DRV_MCMETROLOGY_SetConfiguration(DRV_MCMETROLOGY_CONFIGURATION * config)
{
    /* Not supported yet */
}

void DRV_MCMETROLOGY_GetEventsData(DRV_MCMETROLOGY_AFE_EVENTS * events)
{
    *events = gDrvMCMetObj.metAFEData.events;
}

void DRV_MCMETROLOGY_StartCalibration(void)
{
    /* Not supported yet */
}

bool DRV_MCMETROLOGY_StartHarmonicAnalysis(uint32_t harmonicNumReq, DRV_MCMETROLOGY_HARMONICS_RMS *pHarmonicResponse)
{
    if (harmonicNumReq == 0 || harmonicNumReq > DRV_MCMETROLOGY_HARMONICS_MAX_ORDER)
    {
        /* Requested harmonics out of bounds */
        return false;
    }

    if (gDrvMCMetObj.harmonicAnalysisData.running)
    {
        /* Harmonic Analysis already running */
        return false;
    }

    gDrvMCMetObj.harmonicAnalysisData.running = true;
    gDrvMCMetObj.harmonicAnalysisData.integrationPeriods = 2;
    gDrvMCMetObj.harmonicAnalysisData.harmonicNumReq = harmonicNumReq;

    /* Set Data pointer to store the Harmonic data result */
    gDrvMCMetObj.harmonicAnalysisData.pHarmonicAnalysisResponse = pHarmonicResponse;

    /* Set Number of Harmonic for Analysis */
    gDrvMCMetObj.metRegisters->MET_CONTROL.FEATURE_CTRL1 &= ~FEATURE_CTRL1_HARMONIC_m_REQ_Msk;
    gDrvMCMetObj.metRegisters->MET_CONTROL.FEATURE_CTRL1 |= FEATURE_CTRL1_HARMONIC_m_REQ(harmonicNumReq);
    /* Enable Harmonic Analysis */
    gDrvMCMetObj.metRegisters->MET_CONTROL.FEATURE_CTRL1 |= FEATURE_CTRL1_HARMONIC_EN_Msk;

    return true;
}

void DRV_MCMETROLOGY_StopHarmonicAnalysis(void)
{
    /* Clear flags and disable in registers */
    gDrvMCMetObj.harmonicAnalysisData.running = false;
    gDrvMCMetObj.metRegisters->MET_CONTROL.FEATURE_CTRL1 &= ~FEATURE_CTRL1_HARMONIC_EN_Msk;
}
