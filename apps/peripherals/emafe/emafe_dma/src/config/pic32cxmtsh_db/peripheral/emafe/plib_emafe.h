/*******************************************************************************
  Interface definition of EMAFE PLIB.

  Company:
    Microchip Technology Inc.

  File Name:
    plib_emafe.h

  Summary:
    Interface definition of the Energy Metering Analog Front End Plib (EMAFE).

  Description:
    This file defines the interface for the EMAFE Plib.
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

#ifndef PLIB_EMAFE_H    // Guards against multiple inclusion
#define PLIB_EMAFE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "peripheral/mcspi/mcspi_master/plib_mcspi_master.h"

#ifdef __cplusplus // Provide C++ Compatibility
    extern "C" {
#endif

#define EMAFE_MCSPI_MASTER_CLOCK  118784000

// *****************************************************************************
// *****************************************************************************
// Section: Interface
// *****************************************************************************
// *****************************************************************************

typedef enum {
	EMAFE_ERROR = -1,
	EMAFE_OK = 0,
	EMAFE_ERROR_TIMEOUT = 1,
	EMAFE_ERROR_ARGUMENT,
	EMAFE_ERROR_OVERRUN,
	EMAFE_ERROR_MODE_FAULT,
	EMAFE_ERROR_OVERRUN_AND_MODE_FAULT
} EMAFE_STATUS;

typedef enum {
	EMAFE_CHN_0_ID = 0U,
	EMAFE_CHN_1_ID,
	EMAFE_CHN_2_ID,
	EMAFE_CHN_3_ID,
	EMAFE_CHN_4_ID,
	EMAFE_CHN_NUM,
} EMAFE_CHANNEL_ID;

typedef enum {
	EMAFE_COMP_CHN_0_ID = 0U,
	EMAFE_COMP_CHN_1_ID,
	EMAFE_COMP_CHN_2_ID,
	EMAFE_COMP_CHN_3_ID,
	EMAFE_COMP_CHN_4_ID,
	EMAFE_COMP_CHN_5_ID,
	EMAFE_COMP_CHN_6_ID
} EMAFE_COMP_CHANNEL_ID;

typedef enum {
	EMAFE_COMPARISON_HIGH = 0U,
	EMAFE_COMPARISON_LOW
} EMAFE_DIFF_COMPARISON_TYPE;

typedef struct {
	uint16_t diffThreshold;
	uint8_t diffCount;
	EMAFE_DIFF_COMPARISON_TYPE type;
	EMAFE_COMP_CHANNEL_ID channelA;
	EMAFE_COMP_CHANNEL_ID channelB;
} EMAFE_DIFF_COMPARISON_DATA;

typedef enum {
	EMAFE_CONDITION_OR = 0U,
	EMAFE_CONDITION_AND
} EMAFE_CONDITION_TYPE;

typedef struct {
	uint32_t threshold1;
	uint32_t threshold2;
	EMAFE_CONDITION_TYPE condition;
	EMAFE_COMP_CHANNEL_ID channel;
} EMAFE_COMPARISON_DATA;

typedef void (*EMAFE_CALLBACK)(uint32_t interruptStatus);

/***************************** EMAFE API *******************************/
void EMAFE_Initialize (void);
void EMAFE_Reset (bool enable);
void EMAFE_SoftReset(void);
void EMAFE_SetSincDecimationFiltersReset(bool enable);
void EMAFE_EnableFilter(uint32_t filterMask);
void EMAFE_DisableFilter(uint32_t filterMask);
void EMAFE_EnableDMAChannel(uint32_t channelMask);
void EMAFE_DisableDMAChannel(uint32_t channelMask);
uint32_t EMAFE_GetDMAChannelEnableMask(void);
void EMAFE_DMASetup(uint32_t *pData, uint16_t samplesNum, uint32_t *pNextData, uint16_t samplesNextNum);
void EMAFE_DMATransfer(bool enable, uint32_t controlMask);
void EMAFE_EnableVariationDetection(EMAFE_COMP_CHANNEL_ID channel, uint16_t limit);
void EMAFE_DisableVariationDetection(void);
bool EMAFE_IsEnableVariationDetection(void);
void EMAFE_SetDifferenceComparisonMode(EMAFE_DIFF_COMPARISON_DATA *data);
void EMAFE_SetDataFormat(uint32_t format);
void EMAFE_SetClockPrescalerRatio(uint32_t mclkdiv);
void EMAFE_SetOSR(uint32_t osr);
void EMAFE_SetComparisonModeChannelA(EMAFE_COMPARISON_DATA *data);
void EMAFE_SetComparisonModeChannelB(EMAFE_COMPARISON_DATA *data);
void EMAFE_EnableInterrupt(uint32_t mask);
void EMAFE_DisableInterrupt(uint32_t mask);
uint32_t EMAFE_GetInterruptMask(void);
uint32_t EMAFE_GetInterruptSatus(void);
uint32_t EMAFE_GetStatus(void);
void EMAFE_WriteProtectionSet(uint32_t mode);
uint32_t EMAFE_WriteProtectionGet(void);
void EMAFE_CallbackRegister(EMAFE_CALLBACK callback);

/***************************** EMAFE ADC API *******************************/
void EMAFE_ADCI0Enable(bool tempEnable);
void EMAFE_ADCI0Disable(void);
bool EMAFE_ADCI0IsEnable(void);
void EMAFE_ADCI1Enable(uint8_t gain);
void EMAFE_ADCI1Disable(void);
bool EMAFE_ADCI1IsEnable(void);
void EMAFE_ADCV1Enable(void);
void EMAFE_ADCV1Disable(void);
bool EMAFE_ADCV1IsEnable(void);
void EMAFE_ADCI2Enable(uint8_t gain);
void EMAFE_ADCI2Disable(void);
bool EMAFE_ADCI2IsEnable(void);
void EMAFE_ADCV2Enable(void);
void EMAFE_ADCV2Disable(void);
bool EMAFE_ADCV2IsEnable(void);
void EMAFE_ADCSetClockConfig(uint8_t freqRatio, uint8_t mclkdiv);
void EMAFE_ADCEnableBIAS(void);
void EMAFE_ADCDisableBIAS(void);
bool EMAFE_ADCIsBIASEnable(void);
void EMAFE_ADCEnableREF(void);
void EMAFE_ADCDisableREF(void);
bool EMAFE_ADCIsREFEnable(void);
void EMAFE_ADCEnableLDO(void);
void EMAFE_ADCDisableLDO(void);
bool EMAFE_ADCIsLDOEnable(void);
bool EMAFE_ADCIsAnalogSystemReady(void);
void EMAFE_ADCEnableSoftReset(void);
void EMAFE_ADCDisableSoftReset(void);
void EMAFE_ADCSetSecurityControls(uint8_t mask);
uint8_t EMAFE_ADCGetSecurityControls(void);
uint16_t EMAFE_ADCGGetVREFLowTemp(void);
uint16_t EMAFE_ADCGGetTEMPLowTemp(void);
uint16_t EMAFE_ADCGGetVREFHighTemp(void);
uint16_t EMAFE_ADCGGetTEMPHighTemp(void);

#ifdef __cplusplus // Provide C++ Compatibility
 }
#endif

#endif
