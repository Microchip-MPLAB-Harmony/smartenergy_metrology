/*******************************************************************************
  MCSPI PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_mcspi_master.c

  Summary:
    MCSPI Master Source File

  Description:
    This file has implementation of all the interfaces provided for particular
    MCSPI peripheral.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
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

#include "plib_mcspi_master.h"
#include "interrupts.h"

// *****************************************************************************
// *****************************************************************************
// Section: MCSPI Implementation
// *****************************************************************************
// *****************************************************************************
#define MCSPI_TDR_8BIT_REG      (*(volatile uint8_t* const)((MCSPI_BASE_ADDRESS + MCSPI_TDR_REG_OFST)))
#define MCSPI_TDR_16BIT_REG     (*(volatile uint16_t* const)((MCSPI_BASE_ADDRESS + MCSPI_TDR_REG_OFST)))

#define MCSPI_RDR_8BIT_REG      (*(volatile uint8_t* const)((MCSPI_BASE_ADDRESS + MCSPI_RDR_REG_OFST)))
#define MCSPI_RDR_16BIT_REG     (*(volatile uint16_t* const)((MCSPI_BASE_ADDRESS + MCSPI_RDR_REG_OFST)))


/* Global object to save MCSPI Exchange related data */
static volatile MCSPI_OBJECT mcspiObj;

void MCSPI_Initialize( void )
{
    /* Disable and Reset the MCSPI*/
    MCSPI_REGS->MCSPI_CR = MCSPI_CR_SPIDIS_Msk | MCSPI_CR_SWRST_Msk;


    /* Enable Master mode, select source clock, select particular NPCS line for chip select and disable mode fault detection */
    MCSPI_REGS->MCSPI_MR = MCSPI_MR_MSTR_Msk | MCSPI_MR_DLYBCS(0) | MCSPI_MR_BRSRCCLK_PERIPH_CLK | MCSPI_MR_PCS(MCSPI_CHIP_SELECT_NPCS0)  | MCSPI_MR_MODFDIS_Msk;

    /* Set up clock Polarity, data phase, Communication Width, Baud Rate */
    MCSPI_REGS->MCSPI_CSR[0] = MCSPI_CSR_CPOL(0) | MCSPI_CSR_NCPHA(1) | MCSPI_CSR_BITS_8_BIT | MCSPI_CSR_SCBR(11)| MCSPI_CSR_DLYBS(0) | MCSPI_CSR_DLYBCT(0) | MCSPI_CSR_CSAAT_Msk;





    /* Initialize global variables */
    mcspiObj.transferIsBusy = false;
    mcspiObj.callback = NULL;

    /* Enable MCSPI */
    MCSPI_REGS->MCSPI_CR = MCSPI_CR_SPIEN_Msk;
}



bool MCSPI_WriteRead( void* pTransmitData, size_t txSize, void* pReceiveData, size_t rxSize )
{
    bool isRequestAccepted = false;
    uint32_t dummyData;

    if (mcspiObj.transferIsBusy == false)
    {
        /* Verify the request */
        if(((txSize > 0U) && (pTransmitData != NULL)) || ((rxSize > 0U) && (pReceiveData != NULL)))
        {
            isRequestAccepted = true;
            mcspiObj.txBuffer = pTransmitData;
            mcspiObj.rxBuffer = pReceiveData;
            mcspiObj.rxCount = 0;
            mcspiObj.txCount = 0;
            mcspiObj.dummySize = 0;

            if (pTransmitData != NULL)
            {
                mcspiObj.txSize = txSize;
            }
            else
            {
                mcspiObj.txSize = 0;
            }

            if (pReceiveData != NULL)
            {
                mcspiObj.rxSize = rxSize;
            }
            else
            {
                mcspiObj.rxSize = 0;
            }

            mcspiObj.transferIsBusy = true;

            /* Flush out any unread data in MCSPI read buffer */
            dummyData = (MCSPI_REGS->MCSPI_RDR & MCSPI_RDR_RD_Msk) >> MCSPI_RDR_RD_Pos;
            (void)dummyData;

            size_t txSz = mcspiObj.txSize;

            if (mcspiObj.rxSize > txSz)
            {
                mcspiObj.dummySize = mcspiObj.rxSize - txSz;
            }

            /* Start the first write here itself, rest will happen in ISR context */
            if((MCSPI_REGS->MCSPI_CSR[0] & MCSPI_CSR_BITS_Msk) == MCSPI_CSR_BITS_8_BIT)
            {
                if (mcspiObj.txCount < txSz)
                {
                    MCSPI_REGS->MCSPI_TDR = *((uint8_t*)mcspiObj.txBuffer);
                    mcspiObj.txCount++;
                }
                else if (mcspiObj.dummySize > 0U)
                {
                    MCSPI_REGS->MCSPI_TDR = (uint8_t)(0xff);
                    mcspiObj.dummySize--;
                }
                else
                {
                    /* Nothing to do */
                }
            }
            else
            {
                mcspiObj.txSize >>= 1;
                mcspiObj.dummySize >>= 1;
                mcspiObj.rxSize >>= 1;

                txSz = mcspiObj.txSize;

                if (mcspiObj.txCount < txSz)
                {
                    MCSPI_REGS->MCSPI_TDR = *((uint16_t*)mcspiObj.txBuffer);
                    mcspiObj.txCount++;
                }
                else if (mcspiObj.dummySize > 0U)
                {
                    MCSPI_REGS->MCSPI_TDR = (uint16_t)(0xffff);
                    mcspiObj.dummySize--;
                }
                else
                {
                    /* Nothing to do */
                }
            }

            if (rxSize > 0U)
            {
                /* Enable receive interrupt to complete the transfer in ISR context */
                MCSPI_REGS->MCSPI_IER = MCSPI_IER_RDRF_Msk;
            }
            else
            {
                /* Enable transmit interrupt to complete the transfer in ISR context */
                MCSPI_REGS->MCSPI_IER = MCSPI_IER_TDRE_Msk;
            }
        }
    }

    return isRequestAccepted;
}


bool MCSPI_Write( void* pTransmitData, size_t txSize )
{
    return(MCSPI_WriteRead(pTransmitData, txSize, NULL, 0));
}

bool MCSPI_Read( void* pReceiveData, size_t rxSize )
{
    return(MCSPI_WriteRead(NULL, 0, pReceiveData, rxSize));
}

bool MCSPI_TransferSetup( MCSPI_TRANSFER_SETUP * setup, uint32_t mcspiSourceClock )
{
    uint32_t scbr;

    if ((setup == NULL) || (setup->clockFrequency == 0U))
    {
        return false;
    }

    if(mcspiSourceClock == 0U)
    {
        // Fetch Master Clock Frequency directly
        mcspiSourceClock = 118784000;
    }

    scbr = mcspiSourceClock/setup->clockFrequency;

    if(scbr == 0U)
    {
        scbr = 1;
    }
    else if(scbr > 255U)
    {
        scbr = 255;
    }
    else
    {
        /* Nothing to do */
    }

    MCSPI_REGS->MCSPI_CSR[0] = (MCSPI_REGS->MCSPI_CSR[0] & ~(MCSPI_CSR_CPOL_Msk | MCSPI_CSR_NCPHA_Msk | MCSPI_CSR_BITS_Msk | MCSPI_CSR_SCBR_Msk)) |((uint32_t)setup->clockPolarity | (uint32_t)setup->clockPhase | (uint32_t)setup->dataBits | MCSPI_CSR_SCBR(scbr));

    return true;
}

bool MCSPI_IsTransmitterBusy( void )
{
    return ((MCSPI_REGS->MCSPI_SR & MCSPI_SR_TXEMPTY_Msk) == 0U)? true : false;
}

void MCSPI_CallbackRegister( MCSPI_CALLBACK callback, uintptr_t context )
{
    mcspiObj.callback = callback;
    mcspiObj.context = context;
}

bool MCSPI_IsBusy( void )
{
    bool transferIsBusy = mcspiObj.transferIsBusy;

    return (((MCSPI_REGS->MCSPI_SR & MCSPI_SR_TXEMPTY_Msk) == 0U) || (transferIsBusy));
}
void __attribute__((used)) MCSPI_InterruptHandler( void )
{
    uint32_t dataBits;
    uint32_t receivedData;
    static bool isLastByteTransferInProgress = false;

    dataBits = MCSPI_REGS->MCSPI_CSR[0] & MCSPI_CSR_BITS_Msk;


    uintptr_t context = mcspiObj.context;

    size_t rxSize = mcspiObj.rxSize;

    if ((MCSPI_REGS->MCSPI_SR & MCSPI_SR_RDRF_Msk ) == MCSPI_SR_RDRF_Msk)
    {
        receivedData = (MCSPI_REGS->MCSPI_RDR & MCSPI_RDR_RD_Msk) >> MCSPI_RDR_RD_Pos;

        size_t rxCount = mcspiObj.rxCount;

        if (rxCount < mcspiObj.rxSize)
        {
            if(dataBits == MCSPI_CSR_BITS_8_BIT)
            {
                ((uint8_t*)mcspiObj.rxBuffer)[rxCount] = (uint8_t)receivedData;
                rxCount++;
            }
            else
            {
                ((uint16_t*)mcspiObj.rxBuffer)[rxCount] = (uint16_t)receivedData;
                rxCount++;
            }
        }

        mcspiObj.rxCount = rxCount;
    }

    /* If there are more words to be transmitted, then transmit them here and keep track of the count */
    if((MCSPI_REGS->MCSPI_SR & MCSPI_SR_TDRE_Msk) == MCSPI_SR_TDRE_Msk)
    {
        /* Disable the TDRE interrupt. This will be enabled back if more than
         * one byte is pending to be transmitted */
        MCSPI_REGS->MCSPI_IDR = MCSPI_IDR_TDRE_Msk;

        size_t txCount = mcspiObj.txCount;

        if(dataBits == MCSPI_CSR_BITS_8_BIT)
        {
            if (txCount < mcspiObj.txSize)
            {
                MCSPI_REGS->MCSPI_TDR = ((uint8_t*)mcspiObj.txBuffer)[txCount];
                txCount++;
            }
            else if (mcspiObj.dummySize > 0U)
            {
                MCSPI_REGS->MCSPI_TDR = (uint8_t)(0xff);
                mcspiObj.dummySize--;
            }
            else
            {
                /* Nothing to do */
            }
        }
        else
        {
            if (txCount < mcspiObj.txSize)
            {
                MCSPI_REGS->MCSPI_TDR = ((uint16_t*)mcspiObj.txBuffer)[txCount];
                txCount++;
            }
            else if (mcspiObj.dummySize > 0U)
            {
                MCSPI_REGS->MCSPI_TDR = (uint16_t)(0xffff);
                mcspiObj.dummySize--;
            }
            else
            {
                /* Nothing to do */
            }

        }

        size_t dummySize = mcspiObj.dummySize;
        mcspiObj.txCount = txCount;

        if ((txCount == mcspiObj.txSize) && (dummySize == 0U))
        {
            /* At higher baud rates, the data in the shift register can be
             * shifted out and TXEMPTY flag can get set resulting in a
             * callback been given to the application with the MCSPI interrupt
             * pending with the application. This will then result in the
             * interrupt handler being called again with nothing to transmit.
             * To avoid this, a software flag is set, but
             * the TXEMPTY interrupt is not enabled until the very end.
             */

            isLastByteTransferInProgress = true;

        }
        else if (mcspiObj.rxCount == rxSize)
        {
            /* Enable TDRE interrupt as all the requested bytes are received
             * and can now make use of the internal transmit shift register.
             */
            MCSPI_REGS->MCSPI_IDR = MCSPI_IDR_RDRF_Msk;
            MCSPI_REGS->MCSPI_IER = MCSPI_IDR_TDRE_Msk;
        }
        else
        {
            /* Nothing to do */
        }
    }

    /* See if Exchange is complete */
    if ((isLastByteTransferInProgress == true) && ((MCSPI_REGS->MCSPI_SR & MCSPI_SR_TXEMPTY_Msk) == MCSPI_SR_TXEMPTY_Msk))
    {
        if (mcspiObj.rxCount == rxSize)
        {
            /* Set Last transfer to deassert NPCS after the last byte written in TDR has been transferred. */
            MCSPI_REGS->MCSPI_CR = MCSPI_CR_LASTXFER_Msk;

            mcspiObj.transferIsBusy = false;

            /* Disable TDRE, RDRF and TXEMPTY interrupts */
            MCSPI_REGS->MCSPI_IDR = MCSPI_IDR_TDRE_Msk | MCSPI_IDR_RDRF_Msk | MCSPI_IDR_TXEMPTY_Msk;

            isLastByteTransferInProgress = false;

            if(mcspiObj.callback != NULL)
            {
                mcspiObj.callback(context);
            }
        }
    }
    if (isLastByteTransferInProgress == true)
    {
        /* For the last byte transfer, the TDRE interrupt is already disabled.
         * Enable TXEMPTY interrupt to ensure no data is present in the shift
         * register before application callback is called.
         */
        MCSPI_REGS->MCSPI_IER = MCSPI_IER_TXEMPTY_Msk;
    }
}
