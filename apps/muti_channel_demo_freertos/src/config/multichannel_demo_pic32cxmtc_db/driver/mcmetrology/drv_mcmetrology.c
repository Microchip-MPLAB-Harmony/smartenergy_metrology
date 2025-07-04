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
#include "osal/osal.h"
#include "peripheral/pio/plib_pio.h"
#include "peripheral/rstc/plib_rstc.h"
#include "peripheral/clk/plib_clk.h"
#include "interrupts.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

#define MAX_WAIT_LOOPS 100000UL

typedef enum {
    PENERGY = 0U,
    QENERGY = 2U,
} DRV_MCMETROLOGY_ENERGY_TYPE;

/* This is the driver instance object array. */
static DRV_MCMETROLOGY_OBJ gDrvMCMetObj;

static CACHE_ALIGN uint32_t sCaptureBuffer[CACHE_ALIGNED_SIZE_GET(MET_CAPTURE_BUF_SIZE)];

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
    DRV_MCMETROLOGY_CONF_PULSE_PW,                         /* 29 PULSE_CONTRIBUTION_PW */
    DRV_MCMETROLOGY_CONF_PULSE_I,                          /* 30 PULSE_CONTRIBUTION_I */
    0x00000000UL,                                          /* 31 Reserved */
    DRV_MCMETROLOGY_CONF_PKT,                              /* 32 P_K_T */
    DRV_MCMETROLOGY_CONF_QKT,                              /* 33 Q_K_T */
    DRV_MCMETROLOGY_CONF_IKT,                              /* 34 I_K_T */
    DRV_MCMETROLOGY_CONF_CREEP_P,                          /* 35 CREEP_THRESHOLD_P */
    DRV_MCMETROLOGY_CONF_CREEP_Q,                          /* 36 CREEP_THRESHOLD_Q */
    DRV_MCMETROLOGY_CONF_CREEP_I,                          /* 37 CREEP_THRESHOLD_I */
    DRV_MCMETROLOGY_CONF_P_POWER_OFFSET_CTRL,              /* 38 P_POWER_OFFSET_CTRL */
    DRV_MCMETROLOGY_CONF_Q_POWER_OFFSET_CTRL,              /* 39 Q_POWER_OFFSET_CTRL */
    DRV_MCMETROLOGY_CONF_POFFSET_P,                        /* 40 POWER_OFFSET_P */
    DRV_MCMETROLOGY_CONF_POFFSET_Q,                        /* 41 POWER_OFFSET_Q */
    DRV_MCMETROLOGY_CONF_SWELL_OVERC_THRS_CHx,             /* 42 SWELL_OVERCURRENT_THRESHOLD_CHx */
    DRV_MCMETROLOGY_CONF_SAG_THRS_CHx,                     /* 74 SAG_THRESHOLD_CHx */
    DRV_MCMETROLOGY_CONF_K_CHx,                            /* 106 K_CH_x */
    DRV_MCMETROLOGY_CONF_CAL_M_CHx,                        /* 138 CAL_M_CHx */
    DRV_MCMETROLOGY_CONF_CAL_PH_CHx,                       /* 170 CAL_PH_CHx */
    DRV_MCMETROLOGY_CONF_CAPTURE_CTRL,                     /* 202 CAPTURE_CTRL */
    DRV_MCMETROLOGY_CONF_CAPTURE_CH_SEL,                   /* 203 CAPTURE_CH_SEL */
    DRV_MCMETROLOGY_CAPTURE_BUF_SIZE,                      /* 204 CAPTURE_BUFF_SIZE */
    (uint32_t)sCaptureBuffer,                              /* 205 CAPTURE_ADDR */
    {0x00000000UL,                                         /* 206 RESERVED */
    0x00000000UL,                                          /* 207 RESERVED */
    0x00000000UL},                                         /* 208 RESERVED */
    DRV_MCMETROLOGY_CONF_MCP3914_GAIN,                     /* 209 MCP3914_GAIN */
    DRV_MCMETROLOGY_CONF_MCP3914_CONFIG,                   /* 213 MCP3914_CONFIG0 */
    0x00000000UL,                                          /* 217 RESERVED */
    DRV_MCMETROLOGY_CONF_OFFSET_P_x,                       /* 218 POWER_OFFSET_P_x */
    0x00000000UL,                                          /* 249 RESERVED */
    DRV_MCMETROLOGY_CONF_OFFSET_Q_x,                       /* 250 POWER_OFFSET_Q_x */
    0x00000000UL                                           /* 281 RESERVED */
};

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

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************

static void lDRV_Mcmetrology_UpdateEvents(void)
{
    /* Update events */
    gDrvMCMetObj.metAFEData.events.swell = (uint16_t)gDrvMCMetObj.metRegisters->MET_STATUS.SWELL_OVERCURRENT_FLAG;
    gDrvMCMetObj.metAFEData.events.sag = (uint16_t)gDrvMCMetObj.metRegisters->MET_STATUS.SAG_FLAG;
    gDrvMCMetObj.metAFEData.events.creep = (uint16_t)gDrvMCMetObj.metRegisters->MET_STATUS.CREEP_FLAG;
    gDrvMCMetObj.metAFEData.events.phActive = (uint16_t)gDrvMCMetObj.metRegisters->MET_STATUS.PH_ACTIVE_FLAG;
}

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
            uint8_t index;

            /* Update Accumulators Data */
            (void) memcpy(&gDrvMCMetObj.metAccData,
                          &gDrvMCMetObj.metRegisters->MET_ACCUMULATORS,
                          sizeof(DRV_MCMETROLOGY_REGS_ACCUMULATORS));
            /* Store samples in period */
            gDrvMCMetObj.samplesInPeriod = gDrvMCMetObj.metRegisters->MET_STATUS.N;

            /* Update Channel Data */
            for (index = 0; index < DRV_MCMETROLOGY_CHANNELS_NUMBER; index++)
            {
                gDrvMCMetObj.freqChx[index] = gDrvMCMetObj.metRegisters->MET_STATUS.FREQ_CHx[index];
                gDrvMCMetObj.maxChx[index] = gDrvMCMetObj.metRegisters->MET_STATUS.CHx_MAX[index];
            }

            /* Update Frequency Data */
            gDrvMCMetObj.freq = gDrvMCMetObj.metRegisters->MET_STATUS.FREQ;

            /* Update events */
            lDRV_Mcmetrology_UpdateEvents();

            if (gDrvMCMetObj.harmonicAnalysisData.holdRegs == false)
            {
                /* Update Harmonics Data */
                (void) memcpy(&gDrvMCMetObj.metHarData,
                              &gDrvMCMetObj.metRegisters->MET_HARMONICS,
                              sizeof(DRV_MCMETROLOGY_REGS_HARMONICS));
            }
        }

        gDrvMCMetObj.integrationFlag = true;
    }

    if ((status & DRV_MCMETROLOGY_IPC_FULLCYCLE_IRQ_MSK) != 0UL)
    {
        /* Update events */
        lDRV_Mcmetrology_UpdateEvents();
        if (gDrvMCMetObj.fullCycleCallback != NULL)
        {
            gDrvMCMetObj.fullCycleCallback();
        }
    }

    if ((status & DRV_MCMETROLOGY_IPC_HALFCYCLE_IRQ_MSK) != 0UL)
    {
        /* Update events */
        lDRV_Mcmetrology_UpdateEvents();
        if (gDrvMCMetObj.halfCycleCallback != NULL)
        {
            gDrvMCMetObj.halfCycleCallback();
        }
    }

    IPC1_REGS->IPC_ICCR = status;

    /* Signal Metrology thread to attend IPC interrupt */
    (void) OSAL_SEM_PostISR(&gDrvMCMetObj.semaphoreID);
}

static double lDRV_Mcmetrology_GetHarmonicRMS(int32_t real, int32_t imag)
{
    double rmsRe, rmsIm, rms;
    uint32_t divisor;

    rmsRe = (double)real;
    rmsIm = (double)imag;
    rms = 2.0f * (rmsRe * rmsRe + rmsIm * rmsIm);
    if (rms > 0.0f)
    {
        rms = sqrt(rms);
        divisor = (1UL << 6) * gDrvMCMetObj.samplesInPeriod;
        rms = rms / (double)divisor;
    }
    else
    {
        rms = 0.0f;
    }

    return rms;
}

static float lDRV_Mcmetrology_GetVIRMS(uint64_t val, uint32_t k_x)
{
    double m;
    double k;
    uint64_t divisor;

    // Convert uQ24.40 format to double
    divisor = (1ULL << FORMAT_CONST_uQ2440);
    m = (double)val / (double)divisor;
    // Convert uQ22.10 format to double
    divisor = (1ULL << FORMAT_CONST_uQ2210);
    k = (double)k_x / (double)divisor;

    m = m / (double)gDrvMCMetObj.samplesInPeriod;
    if (m > 0.0f)
    {
        m = sqrt(m);
        m = m * k;
    }
    else
    {
        m = 0.0f;
    }

    return (float)m;
}

static float lDRV_Mcmetrology_GetPQ(int64_t val, uint32_t k_ix, uint32_t k_vx)
{
    double m;
    double mult;
    uint64_t divisor;

    // Convert sQ23.40 format to double
    divisor = (1ULL << FORMAT_CONST_sQ2340);
    m = (double)val / (double)divisor;

    m = m / (double)gDrvMCMetObj.samplesInPeriod;
    mult = (double)k_ix * (double)k_vx;
    divisor = (1UL << (FORMAT_CONST_uQ2210 << 1));
    m = (m * mult) / (double)divisor;

    return (float)m;
}

static double lDRV_Mcmetrology_GetPQOffsetTimesFreq(int32_t powerOffsetReg)
{
    double offset;
    double freq;
    uint32_t divisor;
    
    divisor = (1UL << FORMAT_CONST_sQ130);
    offset = (double)powerOffsetReg;
    offset = offset / (double)divisor; /* offset = offset/2^30 (Wh/Var per cycle) */
    divisor = (1UL << FORMAT_CONST_uQ2012);
    freq = (double)gDrvMCMetObj.freq;
    freq = freq / (double)divisor; /* freq = freq/2^12 (Hz) */
    offset = offset * freq;

    return offset;
}

static float lDRV_Mcmetrology_GetPowerOffset(int32_t powerOffsetReg)
{
    double offset;

    offset = lDRV_Mcmetrology_GetPQOffsetTimesFreq(powerOffsetReg);
    offset = offset * SECS_IN_HOUR_DOUBLE; /* offset = offset * 3600 * freq (Wh/Var) */

    return (float)offset;
}

static float lDRV_Mcmetrology_GetPOffset(void)
{
    float offsetP = 0.0f;

    if ((gDrvMCMetObj.metRegisters->MET_CONTROL.P_POWER_OFFSET_CTRL & P_POWER_OFFSET_CTRL_PUL_Msk) != 0U)
    {
        /* Compute global active power offset in W */
        offsetP = lDRV_Mcmetrology_GetPowerOffset(gDrvMCMetObj.metRegisters->MET_CONTROL.POWER_OFFSET_P);
    }

    return offsetP;
}

static float lDRV_Mcmetrology_GetQOffset(void)
{
    float offsetQ = 0.0f;

    if ((gDrvMCMetObj.metRegisters->MET_CONTROL.Q_POWER_OFFSET_CTRL & Q_POWER_OFFSET_CTRL_PUL_Msk) != 0U)
    {
        /* Compute global active power offset in W */
        offsetQ = lDRV_Mcmetrology_GetPowerOffset(gDrvMCMetObj.metRegisters->MET_CONTROL.POWER_OFFSET_Q);
    }

    return offsetQ;
}

static float lDRV_Mcmetrology_GetAngle(int64_t p, int64_t q)
{
    double angle, pd, qd;

    pd = (double)p;
    qd = (double)q;
    angle = atan2(qd, pd);
    angle = 180.0f * angle;
    angle = angle / CONST_Pi;

    return (float)angle;
}

static float lDRV_Mcmetrology_GetEnergy(DRV_MCMETROLOGY_ENERGY_TYPE id)
{
    double energy = 0.0;
    uint8_t index;
    double offset = 0.0;

    for (index = 0; index < DRV_MCMETROLOGY_POWERS_NUMBER; index++)
    {
        energy += gDrvMCMetObj.metAFEData.powMeasure[index][id];
    }

    if (id == PENERGY)
    {
        if ((gDrvMCMetObj.metRegisters->MET_CONTROL.P_POWER_OFFSET_CTRL & P_POWER_OFFSET_CTRL_PUL_Msk) != 0U)
        {
            /* Compute global active power offset in Wh */
            offset = lDRV_Mcmetrology_GetPQOffsetTimesFreq(gDrvMCMetObj.metRegisters->MET_CONTROL.POWER_OFFSET_P);
        }
    }
    else
    {
        if ((gDrvMCMetObj.metRegisters->MET_CONTROL.Q_POWER_OFFSET_CTRL & Q_POWER_OFFSET_CTRL_PUL_Msk) != 0U)
        {
            /* Compute global active power offset in Wh */
            offset = lDRV_Mcmetrology_GetPQOffsetTimesFreq(gDrvMCMetObj.metRegisters->MET_CONTROL.POWER_OFFSET_Q);
        }
    }

    energy = energy / SECS_IN_HOUR_DOUBLE;   /* (P[Wh]; Q[Varh]) */
    offset = offset * (double)gDrvMCMetObj.samplesInPeriod / SAMPLING_FREQ; /* offset = offset * num_cycles (P[Wh]; Q[Varh]) */
    energy = energy - offset;

    return (float)energy;
}

static void lDRV_Mcmetrology_IpcInitialize (void)
{
    /* Clear interrupts */
    IPC1_REGS->IPC_ICCR = 0xFFFFFFFFUL;
    /* Enable interrupts */
    IPC1_REGS->IPC_IECR = DRV_MCMETROLOGY_IPC_INIT_IRQ_MSK |
        DRV_MCMETROLOGY_IPC_FULLCYCLE_IRQ_MSK |
        DRV_MCMETROLOGY_IPC_HALFCYCLE_IRQ_MSK |
        DRV_MCMETROLOGY_IPC_INTEGRATION_IRQ_MSK;
}

static void lDRV_MCMETROLOGY_UpdateMeasurements(void)
{
    DRV_MCMETROLOGY_REGS_ACCUMULATORS *pAccData = NULL;
    DRV_MCMETROLOGY_REGS_CONTROL *pMetControl = NULL;
    float *pDstData = NULL;
    float pt = 0.0f;
    float ptf = 0.0f;
    float qt = 0.0f;
    float qtf = 0.0f;
    float powerOffset;
    uint32_t divisor;
    uint32_t kxData;
    uint8_t index;

    pAccData = &gDrvMCMetObj.metAccData;
    pMetControl = &gDrvMCMetObj.metRegisters->MET_CONTROL;

    /* Update Accumulated Voltage/Current/Frequency/Max Quantities per Channel */
    for (index = 0; index < DRV_MCMETROLOGY_CHANNELS_NUMBER; index++)
    {
        pDstData = (float *)&gDrvMCMetObj.metAFEData.chnMeasure[index];

        /* Improve accuracy and apply format values to Freq and Max values */
        divisor = (1UL << FORMAT_CONST_uQ2012);
        *(pDstData + (uint8_t)CHN_MEASURE_FREQ) = (float)gDrvMCMetObj.freqChx[index] / (float)divisor;
        divisor = (1UL << FORMAT_CONST_sQ229);
        *(pDstData + (uint8_t)CHN_MEASURE_MAX) = (float)gDrvMCMetObj.maxChx[index] / (float)divisor;

        kxData = pMetControl->K_CHx[index];
        *(pDstData + (uint8_t)CHN_MEASURE_RMS) = lDRV_Mcmetrology_GetVIRMS(pAccData->CHx[index], kxData);
        *(pDstData + (uint8_t)CHN_MEASURE_RMS_F) = lDRV_Mcmetrology_GetVIRMS(pAccData->CHx_F[index], kxData);
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
        *(pDstData + (uint8_t)POW_MEASURE_P) = value;
        pt += value;

        value = lDRV_Mcmetrology_GetPQ(pAccData->P_CHx_F[index], kIData, kxData);
        *(pDstData + (uint8_t)POW_MEASURE_P_F) = value;
        ptf += value;

        value = lDRV_Mcmetrology_GetPQ(pAccData->Q_CHx[index], kIData, kxData);
        *(pDstData + (uint8_t)POW_MEASURE_Q) = value;
        qt += value;

        value = lDRV_Mcmetrology_GetPQ(pAccData->Q_CHx_F[index], kIData, kxData);
        *(pDstData + (uint8_t)POW_MEASURE_Q_F) = value;
        qtf += value;

        *(pDstData + (uint8_t)POW_MEASURE_ANGLE) = lDRV_Mcmetrology_GetAngle(pAccData->P_CHx[index], pAccData->Q_CHx[index]);
    }

    pDstData = gDrvMCMetObj.metAFEData.measure;

    /* Update Accumulated Voltage Quantities Between Phases. It assumes all KVx are the same */
    kxData = pMetControl->K_CHx[gDrvMCMetPowersDefault[0].vChannel];
    *(pDstData + (uint8_t)MEASURE_V_AB) = lDRV_Mcmetrology_GetVIRMS(pAccData->V_AB, kxData);
    *(pDstData + (uint8_t)MEASURE_V_BC) = lDRV_Mcmetrology_GetVIRMS(pAccData->V_BC, kxData);
    *(pDstData + (uint8_t)MEASURE_V_CA) = lDRV_Mcmetrology_GetVIRMS(pAccData->V_CA, kxData);
    *(pDstData + (uint8_t)MEASURE_V_AB_F) = lDRV_Mcmetrology_GetVIRMS(pAccData->V_AB_F, kxData);
    *(pDstData + (uint8_t)MEASURE_V_BC_F) = lDRV_Mcmetrology_GetVIRMS(pAccData->V_BC_F, kxData);
    *(pDstData + (uint8_t)MEASURE_V_CA_F) = lDRV_Mcmetrology_GetVIRMS(pAccData->V_CA_F, kxData);

    /* Update Frequency */
    divisor = (1UL << FORMAT_CONST_uQ2012);
    *(pDstData + (uint8_t)MEASURE_FREQ) = (float)gDrvMCMetObj.freq / (float)divisor;

    /* Update Total Accumulated Quantities */
    divisor = (1UL << FORMAT_CONST_sQ3330);
    *(pDstData + (uint8_t)MEASURE_ACC_TO) = (float)pAccData->ACC_T0 / (float)divisor;
    *(pDstData + (uint8_t)MEASURE_ACC_T1) = (float)pAccData->ACC_T1 / (float)divisor;
    *(pDstData + (uint8_t)MEASURE_ACC_T2) = (float)pAccData->ACC_T2 / (float)divisor;

    /* Update Active/Reactive Total Accumulated Quantities */
    powerOffset = lDRV_Mcmetrology_GetPOffset();
    *(pDstData + (uint8_t)MEASURE_PT) = pt - powerOffset;
    *(pDstData + (uint8_t)MEASURE_PT_F) = ptf - powerOffset;
    powerOffset = lDRV_Mcmetrology_GetQOffset();
    *(pDstData + (uint8_t)MEASURE_QT) = qt - powerOffset;
    *(pDstData + (uint8_t)MEASURE_QT_F) = qtf - powerOffset;

    /* Update Total Accumulated Energy */
    *(pDstData + (uint8_t)MEASURE_ENERGY) += lDRV_Mcmetrology_GetEnergy(PENERGY);
}

static void lDRV_MCMETROLOGY_UpdateHarmonicAnalysisValues(void)
{
    DRV_MCMETROLOGY_HARMONICS_RMS *pHarmonicsRsp = gDrvMCMetObj.harmonicAnalysisData.pHarmonicAnalysisResponse;
    DRV_MCMETROLOGY_REGS_HARMONICS *pHarData = &gDrvMCMetObj.metHarData;
    int32_t real, imag;
    uint8_t channel;

    for (channel = 0; channel < DRV_MCMETROLOGY_CHANNELS_NUMBER; channel++)
    {
        real = (int32_t)(pHarData->CHx_m_R[channel]);
        imag = (int32_t)(pHarData->CHx_m_I[channel]);
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

    /* Create the Semaphore */
    if (OSAL_SEM_Create(&gDrvMCMetObj.semaphoreID, OSAL_SEM_TYPE_BINARY, 0U, 0U) == OSAL_RESULT_FAIL)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    /* Disable IPC interrupts */
    (void) SYS_INT_SourceDisable(IPC1_IRQn);

	/* Clean the IPC interrupt flags */
    gDrvMCMetObj.integrationFlag = false;

    if (resetCause != RSTC_SR_RSTTYP(RSTC_SR_RSTTYP_WDT0_RST_Val))
    {
        uint32_t *pSrc;
        uint32_t *pDst;

        /* Assert reset of the coprocessor and its peripherals */
        RSTC_CoProcessorEnable(false);
        RSTC_CoProcessorPeripheralEnable(false);

        /* Disable coprocessor Clocks */
        CLK_Core1ProcessorClkDisable();

        /* De-assert reset of the coprocessor peripherals */
        RSTC_CoProcessorPeripheralEnable(true);

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

    /* Clean the IPC interrupt flags */
    gDrvMCMetObj.integrationFlag = false;

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
    uint32_t loopCount;

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
    loopCount = 0;
    while (gDrvMCMetObj.metRegisters->MET_STATUS.STATUS != STATUS_STATUS_RESET)
    {
        if (++loopCount > MAX_WAIT_LOOPS)
        {
            /* Way out */
            break;
        }
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

DRV_MCMETROLOGY_RESULT DRV_MCMETROLOGY_FullCycleCallbackRegister(DRV_MCMETROLOGY_CALLBACK callback)
{
    if (callback == NULL)
    {
        return DRV_MCMETROLOGY_ERROR;
    }

    gDrvMCMetObj.fullCycleCallback = callback;
    return DRV_MCMETROLOGY_SUCCESS;
}

DRV_MCMETROLOGY_RESULT DRV_MCMETROLOGY_HalfCycleCallbackRegister(DRV_MCMETROLOGY_CALLBACK callback)
{
    if (callback == NULL)
    {
        return DRV_MCMETROLOGY_ERROR;
    }

    gDrvMCMetObj.halfCycleCallback = callback;
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

    /* Wait for the metrology semaphore to get IPC interrupts */
    (void) OSAL_SEM_Pend(&gDrvMCMetObj.semaphoreID, OSAL_WAIT_FOREVER);

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
        gDrvMCMetObj.metAFEData.measure[MEASURE_ENERGY] = 0.0f;
    }

    return energy;
}

float DRV_MCMETROLOGY_GetChannelMeasureValue(uint8_t channel, DRV_MCMETROLOGY_CHANNEL_MEASURE_TYPE type)
{
    float value;

    if ((channel < DRV_MCMETROLOGY_CHANNELS_NUMBER) && (type < CHN_MEASURE_TYPE_NUM))
    {
        value = gDrvMCMetObj.metAFEData.chnMeasure[channel][type];
    }
    else
    {
        value = 0.0f;
    }

    return value;
}

float DRV_MCMETROLOGY_GetPowerMeasureValue(uint8_t power, DRV_MCMETROLOGY_POWER_MEASURE_TYPE type)
{
    float value;

    if ((power < DRV_MCMETROLOGY_POWERS_NUMBER) && (type < POW_MEASURE_TYPE_NUM))
    {
        value = gDrvMCMetObj.metAFEData.powMeasure[power][type];

        if (type == POW_MEASURE_ANGLE)
        {
            /* Absolute value should be between 0 and 180 degrees */
            while (true)
            {
                if (value < -180.0f)
                {
                    value += 360.0f;
                }
                else if (value > 180.0f)
                {
                    value -= 360.0f;
                }
                else
                {
                    break;
                }
            }
        }
    }
    else
    {
        value = 0.0f;
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
        value = 0.0f;
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

bool DRV_MCMETROLOGY_StartHarmonicAnalysis(uint32_t harmonicNum, DRV_MCMETROLOGY_HARMONICS_RMS *pHarmonicResponse)
{
    if (harmonicNum == 0U || harmonicNum > DRV_MCMETROLOGY_HARMONICS_MAX_ORDER)
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
    gDrvMCMetObj.harmonicAnalysisData.harmonicNumReq = harmonicNum;

    /* Set Data pointer to store the Harmonic data result */
    gDrvMCMetObj.harmonicAnalysisData.pHarmonicAnalysisResponse = pHarmonicResponse;

    /* Set Number of Harmonic for Analysis */
    gDrvMCMetObj.metRegisters->MET_CONTROL.FEATURE_CTRL1 &= ~FEATURE_CTRL1_HARMONIC_m_REQ_Msk;
    gDrvMCMetObj.metRegisters->MET_CONTROL.FEATURE_CTRL1 |= FEATURE_CTRL1_HARMONIC_m_REQ(harmonicNum);
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
