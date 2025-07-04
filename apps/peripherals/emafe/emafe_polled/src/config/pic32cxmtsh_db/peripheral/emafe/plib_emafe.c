/*******************************************************************************
  EMAFE Peripheral Library

  Company:
    Microchip Technology Inc.

  File Name:
    plib_emafe.c

  Summary:
    Energy Metering Analog Front End Plib (EMAFE) source file

  Description:
    None

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#include <stdbool.h>
#include <string.h>
#include "device.h"
#include "interrupts.h"
#include "plib_emafe.h"
#include "plib_emafe_regs.h"
#include "peripheral/mcspi/mcspi_master/plib_mcspi_master.h"
#include "bsp/bsp.h"
#include "device.h"

#define EMAFE_MCSPI_TXFER_TIMEOUT    15000U
#define EMAFE_DATA_BUFFER_SIZE       5U
#define EMAFE_ADDRESS_MASK           0x7F

static uint8_t emafeTxBuffer[EMAFE_DATA_BUFFER_SIZE];
static uint8_t emafeRxBuffer[EMAFE_DATA_BUFFER_SIZE];

static EMAFE_CALLBACK emafeCallback = NULL;

static bool writeEmafeData(EMAFE_ADC_CONTROL address, uint8_t *pData, uint8_t length)
{
    uint32_t timeout;

    /* Check for NULL pointer */
    if (pData == NULL) {
        return false;
    }

    /* Waiting previous transfers */
    timeout = EMAFE_MCSPI_TXFER_TIMEOUT;
    while (MCSPI_IsTransmitterBusy() == true) {
        if (timeout == 0U) {
            return false;
        } else {
            timeout--;
        }
    }

    /* Configure command */
    emafeTxBuffer[0] = (uint8_t)((uint8_t)address & (uint8_t)EMAFE_ADDRESS_MASK);

    /* Add Data */
    (void)memcpy(&emafeTxBuffer[1], pData, (size_t)length);

    /* Write data and return result */
    return MCSPI_Write(emafeTxBuffer, (size_t)((uint32_t)length + 1U));
}

static bool readEmafeData(EMAFE_ADC_CONTROL address, uint8_t *pData, uint8_t length)
{
    bool result = false;
    uint32_t timeout;

    /* Check for NULL pointer */
    if (pData == NULL) {
        return false;
    }

    /* Waiting previous transfers */
    timeout = EMAFE_MCSPI_TXFER_TIMEOUT;
    while (MCSPI_IsTransmitterBusy() == true) {
        if (timeout == 0U) {
            return false;
        } else {
            timeout--;
        }
    }

    /* Configure command */
    emafeTxBuffer[0] = (uint8_t)((uint8_t)(1U << 7) | (uint8_t)((uint8_t)address & (uint8_t)EMAFE_ADDRESS_MASK));

    if (MCSPI_WriteRead(emafeTxBuffer, (size_t)((uint32_t)length + 1U), emafeRxBuffer, 
                        (size_t)((uint32_t)length + 1U)) == true)
    {
        /* Waiting transfer */
        timeout = EMAFE_MCSPI_TXFER_TIMEOUT;
        while (MCSPI_IsTransmitterBusy() == true) {
            if (timeout == 0U) {
                return false;
            } else {
                timeout--;
            }
        }

        (void)memcpy(pData, &emafeRxBuffer[1], (size_t)length);
        result = true;
    }

    return result;
}

/***************************** EMAFE API *******************************/
void EMAFE_Initialize (void)
{
    /* EMAFE Release Reset */
	EMAFE_Reset(false);

    /* Disable all interrupts */
    EMAFE_DisableInterrupt(EMAFE_IDR_Msk);
}

void EMAFE_Reset (bool enable)
{
    if (enable == true)
    {
        // Hold NRST pin (set Off) : PD30
        BSP_EMAFE_NRESET_Off();
    }
    else
    {
        // Release NRST pin (set ON) : PD30
        BSP_EMAFE_NRESET_On();
    }
}

void EMAFE_SoftReset(void)
{
    EMAFE_REGS->EMAFE_CR |= EMAFE_CR_SWRST_Msk;
}

void EMAFE_SetSincDecimationFiltersReset(bool enable)
{
    if (enable == true)
    {
        EMAFE_REGS->EMAFE_CR &= ~EMAFE_CR_SWRST_DECIM_Msk;
    }
    else
    {
        EMAFE_REGS->EMAFE_CR |= EMAFE_CR_SWRST_DECIM_Msk;
    }
}

void EMAFE_EnableFilter(uint32_t filterMask)
{
    uint32_t regValue;
    
    /* Read, clear, and set filter bits */
    regValue = EMAFE_REGS->EMAFE_MR;
    regValue &= ~EMAFE_MR_LPF_ON_Msk;
    regValue |= (filterMask & EMAFE_MR_LPF_ON_Msk);
    
    /* Wait for P24 to go low and then high, as per hardware protocol */
    if ((PIOD_REGS->PIO_PDSR & PIO_PDSR_P24_Msk) != 0U) {
        while ((PIOD_REGS->PIO_PDSR & PIO_PDSR_P24_Msk) != 0U) {
            /* Intentional empty loop: waiting for P24 to go low */
        }
    } else {
        while ((PIOD_REGS->PIO_PDSR & PIO_PDSR_P24_Msk) == 0U) {
            /* Intentional empty loop: waiting for P24 to go high */
        }
        while ((PIOD_REGS->PIO_PDSR & PIO_PDSR_P24_Msk) != 0U) {
            /* Intentional empty loop: waiting for P24 to go low */
        }
    }
    while ((PIOD_REGS->PIO_PDSR & PIO_PDSR_P24_Msk) == 0U) {
        /* Intentional empty loop: waiting for P24 to go high */
    }
    while ((PIOD_REGS->PIO_PDSR & PIO_PDSR_P24_Msk) != 0U) {
        /* Intentional empty loop: waiting for P24 to go low */
    }
    
    /* Write back the modified register value */
    EMAFE_REGS->EMAFE_MR = regValue;
}

void EMAFE_DisableFilter(uint32_t filterMask)
{
    uint32_t regValue;
    
    regValue = EMAFE_REGS->EMAFE_MR;
    regValue &= ~(filterMask & EMAFE_MR_LPF_ON_Msk);
    EMAFE_REGS->EMAFE_MR = regValue;
}

uint32_t EMAFE_GetDataChannel(EMAFE_CHANNEL_ID channel)
{
    return EMAFE_REGS->EMAFE_DATA[channel];
}

void EMAFE_GetAllDataChannel(uint8_t *pData)
{
    uint32_t data[5];
    
    data[0] = EMAFE_REGS->EMAFE_DATA[0];
    data[1] = EMAFE_REGS->EMAFE_DATA[1];
    data[2] = EMAFE_REGS->EMAFE_DATA[2];
    data[3] = EMAFE_REGS->EMAFE_DATA[3];
    data[4] = EMAFE_REGS->EMAFE_DATA[4];
    
    (void)memcpy(pData, (uint8_t *)data, 20U);
}

void EMAFE_EnableVariationDetection(EMAFE_COMP_CHANNEL_ID channel, uint16_t limit)
{
    uint32_t regValue;
    
    regValue = EMAFE_REGS->EMAFE_MR;
    regValue &= ~(EMAFE_MR_VAR_ON_Msk | EMAFE_MR_VAR_CH_Msk | EMAFE_MR_VAR_LIMIT_Msk);
    regValue |= (EMAFE_MR_VAR_ON_Msk | EMAFE_MR_VAR_CH(channel) | EMAFE_MR_VAR_LIMIT(limit));
    EMAFE_REGS->EMAFE_MR = regValue;
}

void EMAFE_DisableVariationDetection(void)
{
    uint32_t regValue;
    
    regValue = EMAFE_REGS->EMAFE_MR;
    regValue &= ~(EMAFE_MR_VAR_ON_Msk | EMAFE_MR_VAR_CH_Msk | EMAFE_MR_VAR_LIMIT_Msk);
    EMAFE_REGS->EMAFE_MR = regValue;
}

bool EMAFE_IsEnableVariationDetection(void)
{
    if ((EMAFE_REGS->EMAFE_MR & EMAFE_MR_VAR_ON_Msk) != 0U)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void EMAFE_SetDifferenceComparisonMode(EMAFE_DIFF_COMPARISON_DATA *data)
{
    uint32_t regValue;

    regValue = EMAFE_CDCR_CHA_IDX(data->channelA);
    regValue |= EMAFE_CDCR_CHB_IDX(data->channelB);
    regValue |= EMAFE_CDCR_DIFFTYPE(data->type);
    regValue |= EMAFE_CDCR_DIFFCOUNT(data->diffCount);
    regValue |= EMAFE_CDCR_DIFFTHRES(data->diffThreshold);

    EMAFE_REGS->EMAFE_CDCR = regValue;
}

void EMAFE_SetDataFormat(uint32_t format)
{
    uint32_t regValue;
    
    regValue = EMAFE_REGS->EMAFE_EMR;
    regValue &= ~EMAFE_EMR_FORMAT_Msk;
    regValue |= EMAFE_EMR_FORMAT(format);
    EMAFE_REGS->EMAFE_EMR = regValue;
}

void EMAFE_SetClockPrescalerRatio(uint32_t mclkdiv)
{
    uint32_t regValue;
    
    regValue = EMAFE_REGS->EMAFE_EMR;
    regValue &= ~EMAFE_EMR_MCLKDIV_LPF_Msk;
    regValue |= EMAFE_EMR_MCLKDIV_LPF(mclkdiv);
    EMAFE_REGS->EMAFE_EMR = regValue;
}

void EMAFE_SetOSR(uint32_t osr)
{
    uint32_t regValue;
    
    regValue = EMAFE_REGS->EMAFE_EMR;
    regValue &= ~EMAFE_EMR_OSR_Msk;
    regValue |= EMAFE_EMR_OSR(osr);
    EMAFE_REGS->EMAFE_EMR = regValue;
}

void EMAFE_SetComparisonModeChannelA(EMAFE_COMPARISON_DATA *data)
{
    uint32_t regValue;

    regValue = EMAFE_CHA_CFG1_THRESH1(data->threshold1);
    regValue |= EMAFE_CHA_CFG1_IDX(data->channel);
    regValue |= EMAFE_CHA_CFG1_COND(data->condition);
    EMAFE_REGS->EMAFE_CHA_CFG1 = regValue;

    regValue = EMAFE_CHA_CFG2_THRESH2(data->threshold2);
    EMAFE_REGS->EMAFE_CHA_CFG2 = regValue;
}

void EMAFE_SetComparisonModeChannelB(EMAFE_COMPARISON_DATA *data)
{
    uint32_t regValue;

    regValue = EMAFE_CHB_CFG1_THRESH1(data->threshold1);
    regValue |= EMAFE_CHB_CFG1_IDX(data->channel);
    regValue |= EMAFE_CHB_CFG1_COND(data->condition);
    EMAFE_REGS->EMAFE_CHB_CFG1 = regValue;

    regValue = EMAFE_CHB_CFG2_THRESH2(data->threshold2);
    EMAFE_REGS->EMAFE_CHB_CFG2 = regValue;
}

void EMAFE_EnableInterrupt(uint32_t mask)
{
    EMAFE_REGS->EMAFE_IER |= (mask & EMAFE_IER_Msk);
}

void EMAFE_DisableInterrupt(uint32_t mask)
{
    EMAFE_REGS->EMAFE_IDR |= (mask & EMAFE_IDR_Msk);
}

uint32_t EMAFE_GetInterruptMask(void)
{
    return EMAFE_REGS->EMAFE_IMR;
}

uint32_t EMAFE_GetInterruptSatus(void)
{
    return EMAFE_REGS->EMAFE_ISR;
}

uint32_t EMAFE_GetStatus(void)
{
    return EMAFE_REGS->EMAFE_SR;
}

void EMAFE_WriteProtectionSet(uint32_t mode)
{
    EMAFE_REGS->EMAFE_WPMR = (EMAFE_WPMR_WPKEY_PASSWD | mode);
}

uint32_t EMAFE_WriteProtectionGet(void)
{
    return EMAFE_REGS->EMAFE_WPMR;
}

void EMAFE_CallbackRegister(EMAFE_CALLBACK callback)
{
    emafeCallback = callback;
}

/***************************** EMAFE ADC API *******************************/
void EMAFE_ADCI0Enable(bool tempEnable)
{
    uint8_t data;

    data = EMAFE_ADC_ADCI0_CTRL_ONADC_Msk;
    if (tempEnable == true) 
	{
        data |= EMAFE_ADC_ADCI0_CTRL_TEMPMEAS_Msk;
    }

    (void)writeEmafeData(EMAFE_ADC_ADCI0_CTRL, &data, 1U);
}

void EMAFE_ADCI0Disable(void)
{
    uint8_t data = 0;

    (void)writeEmafeData(EMAFE_ADC_ADCI0_CTRL, &data, 1U);
}

bool EMAFE_ADCI0IsEnable(void)
{
    uint8_t data = 0;

    (void)readEmafeData(EMAFE_ADC_ADCI0_CTRL, &data, 1U);

    if ((data & EMAFE_ADC_ADCI0_CTRL_ONADC_Msk) != 0U)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void EMAFE_ADCI1Enable(uint8_t gain)
{
    uint8_t data = 0;

    data = (uint8_t)(EMAFE_ADC_ADCI1_CTRL_ONADC_Msk | EMAFE_ADC_ADCI1_CTRL_GAIN(gain));

    (void)writeEmafeData(EMAFE_ADC_ADCI1_CTRL, &data, 1U);
}

void EMAFE_ADCI1Disable(void)
{
    uint8_t data = 0;

    (void)writeEmafeData(EMAFE_ADC_ADCI1_CTRL, &data, 1U);
}

bool EMAFE_ADCI1IsEnable(void)
{
    uint8_t data = 0;

    (void)readEmafeData(EMAFE_ADC_ADCI1_CTRL, &data, 1U);

    if ((data & EMAFE_ADC_ADCI1_CTRL_ONADC_Msk) != 0U)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void EMAFE_ADCV1Enable(void)
{
    uint8_t data;

    data = EMAFE_ADC_ADCV1_CTRL_ONADC_Msk;

    (void)writeEmafeData(EMAFE_ADC_ADCV1_CTRL, &data, 1U);
}

void EMAFE_ADCV1Disable(void)
{
    uint8_t data = 0;

    (void)writeEmafeData(EMAFE_ADC_ADCV1_CTRL, &data, 1U);
}

bool EMAFE_ADCV1IsEnable(void)
{
    uint8_t data = 0;

    (void)readEmafeData(EMAFE_ADC_ADCV1_CTRL, &data, 1U);

    if ((data & EMAFE_ADC_ADCV1_CTRL_ONADC_Msk) != 0U)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void EMAFE_ADCI2Enable(uint8_t gain)
{
    uint8_t data;

    data = (uint8_t)(EMAFE_ADC_ADCI2_CTRL_ONADC_Msk | EMAFE_ADC_ADCI2_CTRL_GAIN(gain));

    (void)writeEmafeData(EMAFE_ADC_ADCI2_CTRL, &data, 1U);
}

void EMAFE_ADCI2Disable(void)
{
    uint8_t data = 0;

    (void)writeEmafeData(EMAFE_ADC_ADCI2_CTRL, &data, 1U);
}

bool EMAFE_ADCI2IsEnable(void)
{
    uint8_t data = 0;

    (void)readEmafeData(EMAFE_ADC_ADCI2_CTRL, &data, 1U);

    if ((data & EMAFE_ADC_ADCI2_CTRL_ONADC_Msk) != 0U)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void EMAFE_ADCV2Enable(void)
{
    uint8_t data;

    data = EMAFE_ADC_ADCV2_CTRL_ONADC_Msk;

    (void)writeEmafeData(EMAFE_ADC_ADCV2_CTRL, &data, 1U);
}

void EMAFE_ADCV2Disable(void)
{
    uint8_t data = 0;

    (void)writeEmafeData(EMAFE_ADC_ADCV2_CTRL, &data, 1U);
}

bool EMAFE_ADCV2IsEnable(void)
{
    uint8_t data = 0;

    (void)readEmafeData(EMAFE_ADC_ADCV2_CTRL, &data, 1U);

    if ((data & EMAFE_ADC_ADCV2_CTRL_ONADC_Msk) != 0U)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void EMAFE_ADCSetClockConfig(uint8_t freqRatio, uint8_t mclkdiv)
{
    uint8_t data = 0;

    data = (uint8_t)(EMAFE_ADC_CLOCK_FREQRATIO(freqRatio) | EMAFE_ADC_CLOCK_MCLKDIV_ANA(mclkdiv));

    (void)writeEmafeData(EMAFE_ADC_CLOCK, &data, 1U);
}

void EMAFE_ADCEnableBIAS(void)
{
    uint8_t data = 0;

    (void)readEmafeData(EMAFE_ADC_ANALOG_CTRL, &data, 1U);

    if ((data & EMAFE_ADC_ANALOG_CTRL_ONBIAS_Msk) == 0U)
    {
        data |= EMAFE_ADC_ANALOG_CTRL_ONBIAS_Msk;
        (void)writeEmafeData(EMAFE_ADC_ANALOG_CTRL, &data, 1U);
    }
}

void EMAFE_ADCDisableBIAS(void)
{
    uint8_t data = 0;

    (void)readEmafeData(EMAFE_ADC_ANALOG_CTRL, &data, 1U);

    if ((data & EMAFE_ADC_ANALOG_CTRL_ONBIAS_Msk) == EMAFE_ADC_ANALOG_CTRL_ONBIAS_Msk)
    {
        data &= (uint8_t)~EMAFE_ADC_ANALOG_CTRL_ONBIAS_Msk;
        (void)writeEmafeData(EMAFE_ADC_ANALOG_CTRL, &data, 1U);
    }
}

bool EMAFE_ADCIsBIASEnable(void)
{
    uint8_t data = 0;

    (void)readEmafeData(EMAFE_ADC_ANALOG_CTRL, &data, 1U);

    if ((data & EMAFE_ADC_ANALOG_CTRL_ONBIAS_Msk) == EMAFE_ADC_ANALOG_CTRL_ONBIAS_Msk)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void EMAFE_ADCEnableREF(void)
{
    uint8_t data = 0;

    (void)readEmafeData(EMAFE_ADC_ANALOG_CTRL, &data, 1U);

    if ((data & EMAFE_ADC_ANALOG_CTRL_ONREF_Msk) == 0U)
    {
        data |= EMAFE_ADC_ANALOG_CTRL_ONREF_Msk;
        (void)writeEmafeData(EMAFE_ADC_ANALOG_CTRL, &data, 1U);
    }
}

void EMAFE_ADCDisableREF(void)
{
    uint8_t data = 0;

    (void)readEmafeData(EMAFE_ADC_ANALOG_CTRL, &data, 1U);

    if ((data & EMAFE_ADC_ANALOG_CTRL_ONREF_Msk) == EMAFE_ADC_ANALOG_CTRL_ONREF_Msk)
    {
        data &= (uint8_t)~EMAFE_ADC_ANALOG_CTRL_ONREF_Msk;
        (void)writeEmafeData(EMAFE_ADC_ANALOG_CTRL, &data, 1U);
    }
}

bool EMAFE_ADCIsREFEnable(void)
{
    uint8_t data = 0;

    (void)readEmafeData(EMAFE_ADC_ANALOG_CTRL, &data, 1U);

    if ((data & EMAFE_ADC_ANALOG_CTRL_ONREF_Msk) == EMAFE_ADC_ANALOG_CTRL_ONREF_Msk)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void EMAFE_ADCEnableLDO(void)
{
    uint8_t data = 0;

    (void)readEmafeData(EMAFE_ADC_ANALOG_CTRL, &data, 1U);

    if ((data & EMAFE_ADC_ANALOG_CTRL_ONLDO_Msk) == 0U)
    {
        data |= EMAFE_ADC_ANALOG_CTRL_ONLDO_Msk;
        (void)writeEmafeData(EMAFE_ADC_ANALOG_CTRL, &data, 1U);
    }
}

void EMAFE_ADCDisableLDO(void)
{
    uint8_t data = 0;

    (void)readEmafeData(EMAFE_ADC_ANALOG_CTRL, &data, 1U);

    if ((data & EMAFE_ADC_ANALOG_CTRL_ONLDO_Msk) == EMAFE_ADC_ANALOG_CTRL_ONLDO_Msk)
    {
        data &= (uint8_t)~EMAFE_ADC_ANALOG_CTRL_ONLDO_Msk;
        (void)writeEmafeData(EMAFE_ADC_ANALOG_CTRL, &data, 1U);
    }
}

bool EMAFE_ADCIsLDOEnable(void)
{
    uint8_t data = 0;

    (void)readEmafeData(EMAFE_ADC_ANALOG_CTRL, &data, 1U);

    if ((data & EMAFE_ADC_ANALOG_CTRL_ONLDO_Msk) == EMAFE_ADC_ANALOG_CTRL_ONLDO_Msk)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool EMAFE_ADCIsAnalogSystemReady(void)
{
    uint8_t data = 0;

    (void)readEmafeData(EMAFE_ADC_ANALOG_STATUS, &data, 1U);

    if ((data & EMAFE_ADC_ANALOG_STATUS_SYSRDY_Msk) != 0U)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void EMAFE_ADCEnableSoftReset(void)
{
    uint8_t data = 0;

    (void)writeEmafeData(EMAFE_ADC_NRESET, &data, 1U);
}

void EMAFE_ADCDisableSoftReset(void)
{
    uint8_t data = 0;

    data = EMAFE_ADC_ANALOG_STATUS_SYSRDY_Msk;
    (void)writeEmafeData(EMAFE_ADC_NRESET, &data, 1U);
}

void EMAFE_ADCSetSecurityControls(uint8_t mask)
{
    uint8_t data = 0;

    data = EMAFE_ADC_SECURITY_SECKEY(EMAFE_ADC_SECURITY_SECKEY_PASSWD);
    data |= mask;
    (void)writeEmafeData(EMAFE_ADC_SECURITY, &data, 1U);
}

uint8_t EMAFE_ADCGetSecurityControls(void)
{
    uint8_t data = 0;

    (void)readEmafeData(EMAFE_ADC_SECURITY, &data, 1U);

    return data;
}

uint16_t EMAFE_ADCGGetVREFLowTemp(void)
{
    uint16_t data = 0;

    (void)readEmafeData(EMAFE_ADC_VREF_TLOW, (uint8_t *)&data, 2U);

    return data;
}

uint16_t EMAFE_ADCGGetTEMPLowTemp(void)
{
    uint16_t data = 0;

    (void)readEmafeData(EMAFE_ADC_TEMP_TLOW, (uint8_t *)&data, 2U);

    return data;
}

uint16_t EMAFE_ADCGGetVREFHighTemp(void)
{
    uint16_t data = 0;

    (void)readEmafeData(EMAFE_ADC_VREF_THIGH, (uint8_t *)&data, 2U);

    return data;
}

uint16_t EMAFE_ADCGGetTEMPHighTemp(void)
{
    uint16_t data = 0;

    (void)readEmafeData(EMAFE_ADC_TEMP_THIGH, (uint8_t *)&data, 2U);

    return data;
}

void EMAFE_DATA_InterruptHandler(void)
{
    volatile uint32_t status;
    status = EMAFE_REGS->EMAFE_IMR & EMAFE_REGS->EMAFE_ISR;

    if (emafeCallback != NULL)
    {
        emafeCallback(status);
    }
}