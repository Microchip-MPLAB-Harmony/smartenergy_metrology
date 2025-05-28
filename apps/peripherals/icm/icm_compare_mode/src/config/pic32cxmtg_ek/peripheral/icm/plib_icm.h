/*******************************************************************************
  Interface definition of ICM PLIB.

  Company:
    Microchip Technology Inc.

  File Name:
    plib_icm.h

  Summary:
    Interface definition of the Integrity Check Monitor Plib (ICM).

  Description:
    This file defines the interface for the ICM Plib.
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

#ifndef PLIB_ICM_H    // Guards against multiple inclusion
#define PLIB_ICM_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus // Provide C++ Compatibility
    extern "C" {
#endif


// *****************************************************************************
// *****************************************************************************
// Section: Interface
// *****************************************************************************
// *****************************************************************************

#define ICM_REGION_NUM       2U

typedef enum {
	ICM_REGION_0 = 0,
	ICM_REGION_1,
	ICM_REGION_2,
	ICM_REGION_3,
	ICM_REGION_MAX
}ICM_REGION_ID;

typedef enum {
	ICM_INTERRUPT_RHC = 0,
	ICM_INTERRUPT_RDM,
	ICM_INTERRUPT_RBE,
	ICM_INTERRUPT_RWC,
	ICM_INTERRUPT_REC,
	ICM_INTERRUPT_RSU,
	ICM_INTERRUPT_URAD,
	ICM_INTERRUPT_MAX
}ICM_INTERRUPT_SOURCE;

typedef enum {
	ICM_SHA_1 = 0,
	ICM_SHA_224 = 4,
	ICM_SHA_256 = 1,
}ICM_ALGO;

typedef struct 
{
	uint32_t startAddress;
	uint32_t reserved;
	uint32_t transferSize;
	struct ICM_SECONDARY_LIST *nextAddress;
}ICM_SECONDARY_LIST;

typedef struct 
{
	uint32_t compareMode : 1;
	uint32_t wrap : 1;
	uint32_t endMonitor : 1;
	uint32_t reserved1 : 1;
	uint32_t regHashIntEn : 1;
	uint32_t mismatchIntEn : 1;
	uint32_t busErrIntEn : 1;
	uint32_t wrapConIntEn : 1;
	uint32_t endBitConIntEn : 1;
	uint32_t statusUpdConIntEn : 1;
	uint32_t procDelay : 1;
	uint32_t reserved2 : 1;
	uint32_t algo : 3;
	uint32_t reserved3 : 9;
	uint32_t memBusProtection : 6;
	uint32_t reserved4 : 2;
}ICM_REGION_BITFIELD;

typedef union {
	uint32_t value;
	ICM_REGION_BITFIELD bitfield;
}ICM_REGION_CONFIG;

typedef struct 
{
	uint32_t startAddress;
	ICM_REGION_CONFIG config;
	uint32_t transferSize;
	ICM_SECONDARY_LIST *secondaryList;
}ICM_REGION_DESCRIPTOR;


typedef void (*ICM_CALLBACK)(ICM_REGION_ID);

/***************************** ICM API *******************************/
void ICM_Initialize ( void );
void ICM_Reset ( void );
uint32_t ICM_GetStatus(void);
uint32_t ICM_GetIStatus(void);
ICM_REGION_DESCRIPTOR * ICM_GetDefaultRegionDescriptor( void );
void ICM_SetRegionDescriptor( ICM_REGION_DESCRIPTOR * pRegionDescriptor );
void ICM_SetRegionDescriptorData(ICM_REGION_ID regionId, uint32_t * pData, size_t bytes);
uint32_t ICM_GetTransferSize(size_t bytes);
void ICM_SetHashAreaAddress( uint32_t address );
void ICM_SetUserInitialHashValue(uint32_t *pData);
void ICM_Enable ( void );
void ICM_Disable ( void );
void ICM_SetMonitorMode ( bool enable , uint8_t bbc);
void ICM_EnableRegionMonitor ( ICM_REGION_ID regionId );
void ICM_DisableRegionMonitor ( ICM_REGION_ID regionId );
void ICM_EnableInterrupt ( ICM_INTERRUPT_SOURCE source, ICM_REGION_ID regionId );
void ICM_DisableInterrupt ( ICM_INTERRUPT_SOURCE source, ICM_REGION_ID regionId );
void ICM_CallbackRegister( ICM_INTERRUPT_SOURCE source, ICM_CALLBACK callback );

#ifdef __cplusplus // Provide C++ Compatibility
 }
#endif

#endif
