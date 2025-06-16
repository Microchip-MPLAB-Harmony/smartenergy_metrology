/*******************************************************************************
  ICM Peripheral Library

  Company:
    Microchip Technology Inc.

  File Name:
    plib_icm.c

  Summary:
    Integrity Check Monitor Plib (ICM) Source File

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
#include "plib_icm.h"
#include "device.h"

static ICM_CALLBACK icmCallbackList[ICM_INTERRUPT_MAX] = {NULL};

static ICM_REGION_DESCRIPTOR icmDefaultRegDescriptor[ICM_REGION_NUM] __ALIGNED(64) = 
{
    {0, {0x${ICM_RCFG_0}}, 0, NULL},
<#if ICM_REGIONS_NUMBER gt 1>
    {0, {0x${ICM_RCFG_1}}, 0, NULL},
</#if>
<#if ICM_REGIONS_NUMBER gt 2>
    {0, {0x${ICM_RCFG_2}}, 0, NULL},
</#if>
<#if ICM_REGIONS_NUMBER gt 3>
    {0, {0x${ICM_RCFG_3}}, 0, NULL},
</#if>
};

static bool ICM_peripheral_clock_ctatus ( void )
{
  return (bool)((PMC_REGS->PMC_CSR1 & PMC_CSR1_PID57_Msk) >> PMC_CSR1_PID57_Pos);
}

static void ICM_enable_peripheral_clock ( void )
{
  PMC_REGS->PMC_PCR = PMC_PCR_CMD_Msk | PMC_PCR_EN(1) | PMC_PCR_PID(ID_ICM);
  while(ICM_peripheral_clock_ctatus() == false)
  {
    /* Wait for clock to be initialized */
  }
}

void ICM_Initialize ( void )
{
  ICM_enable_peripheral_clock();
  ICM_Reset();

  // Set ICM configuration
  ICM_REGS->ICM_CFG = 0x${ICM_CFG};

  // Set Default Descriptors
  ICM_REGS->ICM_DSCR = (uint32_t)&icmDefaultRegDescriptor[0];
}

void ICM_Reset ( void )
{
  ICM_REGS->ICM_CTRL = ICM_CTRL_SWRST_Msk;
}

uint32_t ICM_GetStatus(void)
{
  return ICM_REGS->ICM_SR;
}

uint32_t ICM_GetIStatus(void)
{
  return ICM_REGS->ICM_ISR;
}

ICM_REGION_DESCRIPTOR * ICM_GetDefaultRegionDescriptor(void)
{
  return &icmDefaultRegDescriptor[0];
}

void ICM_SetRegionDescriptor(ICM_REGION_DESCRIPTOR * pRegionDescriptor)
{
  ICM_REGS->ICM_DSCR = (uint32_t)pRegionDescriptor;
}

void ICM_SetRegionDescriptorData(ICM_REGION_ID regionId, uint32_t * pData, size_t bytes)
{
  if (bytes > 0)
  {
    ICM_REGION_DESCRIPTOR *pRegionDescriptor = icmDefaultRegDescriptor;
    uint32_t txferSize;

    pRegionDescriptor += regionId;
    pRegionDescriptor->startAddress = (uint32_t)pData;
    
    // ICM performs a transfer of (TRSIZE + 1) blocks of 512 bits (64 bytes)
    txferSize = (uint32_t)(bytes >> 6); 
    if (bytes % 64)
    {
      txferSize++;
    }

    pRegionDescriptor->transferSize = txferSize - 1;
  }
}

uint32_t ICM_GetTransferSize(size_t bytes)
{
  uint16_t txferSize;

  // ICM performs a transfer of (TRSIZE + 1) blocks of 512 bits (64 bytes)
  txferSize = (uint32_t)(bytes >> 6); 
  if (bytes % 64)
  {
    txferSize++;
  }

  return (txferSize - 1);
}

void ICM_SetHashAreaAddress(uint32_t address)
{
  ICM_REGS->ICM_HASH = address;
}

void ICM_SetUserInitialHashValue(uint32_t *pValue)
{
  ICM_REGS->ICM_UIHVAL[0] = *pValue++;
  ICM_REGS->ICM_UIHVAL[1] = *pValue++;
  ICM_REGS->ICM_UIHVAL[2] = *pValue++;
  ICM_REGS->ICM_UIHVAL[3] = *pValue++;
  ICM_REGS->ICM_UIHVAL[4] = *pValue++;

  if (ICM_REGS->ICM_CFG & ICM_CFG_UALGO_Msk)
  {
    ICM_REGS->ICM_UIHVAL[5] = *pValue++;
    ICM_REGS->ICM_UIHVAL[6] = *pValue++;
    ICM_REGS->ICM_UIHVAL[7] = *pValue;
  }
  else
  {
    ICM_REGS->ICM_UIHVAL[5] = 0;
    ICM_REGS->ICM_UIHVAL[6] = 0;
    ICM_REGS->ICM_UIHVAL[7] = 0;
  }
}

void ICM_Enable ( void )
{
  ICM_REGS->ICM_CTRL = ICM_CTRL_ENABLE_Msk;
}

void ICM_Disable ( void )
{
  ICM_REGS->ICM_CTRL = ICM_CTRL_DISABLE_Msk;
}

void ICM_SetMonitorMode ( bool enable , uint8_t bbc)
{
  if (bbc > 15)
  {
    bbc = 15;
  }

  if (enable == true)
  {
    ICM_REGS->ICM_CFG &= ~ICM_CFG_BBC_Msk;
    ICM_REGS->ICM_CFG |= (ICM_CFG_ASCD_Msk | ICM_CFG_BBC(bbc));
  }
  else
  {
    ICM_REGS->ICM_CFG &= ~(ICM_CFG_ASCD_Msk | ICM_CFG_BBC_Msk);
  }
}

void ICM_EnableRegionMonitor ( ICM_REGION_ID regionId )
{
  if (regionId < ICM_REGION_NUM)
  {
    ICM_REGS->ICM_CTRL = ICM_CTRL_RMEN(1 << regionId);
  }
}

void ICM_DisableRegionMonitor ( ICM_REGION_ID regionId )
{
  if (regionId < ICM_REGION_NUM)
  {
    ICM_REGS->ICM_CTRL = ICM_CTRL_RMDIS(1 << regionId);
  }
}

void ICM_EnableInterrupt ( ICM_INTERRUPT_SOURCE source, ICM_REGION_ID regionId )
{
  if (regionId < ICM_REGION_NUM)
  {
    uint32_t regValue;

    if (source == ICM_INTERRUPT_URAD)
    {
      regValue = ICM_IER_URAD_Msk;
    }
    else
    {
      regValue = (1 << regionId) << (source << 2);
    }

    ICM_REGS->ICM_IER = regValue;
  }
}

void ICM_DisableInterrupt ( ICM_INTERRUPT_SOURCE source, ICM_REGION_ID regionId )
{
  if (regionId < ICM_REGION_NUM)
  {
    uint32_t regValue;

    if (source == ICM_INTERRUPT_URAD)
    {
      regValue = ICM_IER_URAD_Msk;
    }
    else
    {
      regValue = (1 << regionId) << (source << 2);
    }

    ICM_REGS->ICM_IDR = regValue;
  }
}

void ICM_CallbackRegister(ICM_INTERRUPT_SOURCE source, ICM_CALLBACK callback)
{
    icmCallbackList[source] = callback;
}

void ICM_InterruptHandler(void)
{
  volatile uint32_t status;
  ICM_INTERRUPT_SOURCE intIndex;
  ICM_REGION_ID regionIndex;
  uint8_t intStatus;
  
  status = ICM_REGS->ICM_IMR & ICM_REGS->ICM_ISR;

  if (status > 0)
  {
    for (intIndex = ICM_INTERRUPT_RHC; intIndex < ICM_INTERRUPT_MAX; intIndex++)
    {
      intStatus = status & 0x0F;
      if ((icmCallbackList[intIndex] != NULL) && (intStatus > 0))
      {
        for (regionIndex = 0; regionIndex < (ICM_REGION_ID)ICM_REGION_NUM; regionIndex++)
        {
          if (intStatus & 0x01)
          {
            icmCallbackList[intIndex](regionIndex);
          }
          intStatus >>= 1;
        }
      }
      status >>= 4;
    }
  }
}
