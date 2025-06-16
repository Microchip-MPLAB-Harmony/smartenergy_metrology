/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

#define APP_SAMPLES_PRINT                  40
/* Buffer setting */
#define APP_SAMPLE_CHANNELS                1600 // 320 samples/cycle(50Hz) * 5 cycles
/** Buffer to store sample sets */
static uint32_t pSamplesBuffer[APP_SAMPLE_CHANNELS] = {0};

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

void _appEmafeCallback(uint32_t interruptStatus)
{
    if (interruptStatus & EMAFE_IER_ENDRX_Msk)
    {
        EMAFE_DMATransfer(false, EMAFE_PTCR_RXTDIS_Msk);
        EMAFE_DisableInterrupt(EMAFE_IDR_ENDRX_Msk);
        appData.newDataReady = true;
    }
}

void _appPIOHandler ( PIO_PIN pin, uintptr_t context)
{
    if (pin == BSP_SCRL_UP_BTN_PIN)
    {
        appData.btnPressed = true;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************
static void _emafeADCReset(bool enable)
{
	if (enable) {
		/* Enable ADC SOFT RESET */
		EMAFE_ADCEnableSoftReset();
	} else {
		/* Disable ADC SOFT RESET */
		EMAFE_ADCDisableSoftReset();

		/* Wait SYSRDY */
		while (!(EMAFE_ADCIsAnalogSystemReady()));
	}
}

static void _emafeConfigADCChannels(bool tempEnable, uint8_t gainI1, uint8_t gainI2)
{
    /* EMAFE Release Reset */
	_emafeADCReset(false);
    
	/* Enable ADC Channels */
    EMAFE_ADCI0Enable(tempEnable);
	while (!EMAFE_ADCI0IsEnable());

	EMAFE_ADCI1Enable(gainI1);
	while (!EMAFE_ADCI1IsEnable());

	EMAFE_ADCV1Enable();
	while (!EMAFE_ADCV1IsEnable());

	EMAFE_ADCI2Enable(gainI2);
	while (!EMAFE_ADCI2IsEnable());

	EMAFE_ADCV2Enable();
	while (!EMAFE_ADCV2IsEnable());

	/* Set ADC clock configuration */
    EMAFE_ADCSetClockConfig(EMAFE_ADC_CLOCK_FREQRATIO_4,
			EMAFE_ADC_CLOCK_MCLKDIV_ANACK2);
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_ADC_CONTROL_INIT;
    
    appData.timer = SYS_TIME_HANDLE_INVALID;
    appData.newDataReady = false;
    appData.btnPressed = false;
    appData.dataFormat = EMAFE_EMR_FORMAT_FULLRANGE_Val;
    
    PIO_PinInterruptCallbackRegister(BSP_SCRL_UP_BTN_PIN, _appPIOHandler, 0);
    PIO_PinInterruptEnable(BSP_SCRL_UP_BTN_PIN);
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_ADC_CONTROL_INIT:
        {
            _emafeConfigADCChannels(true, 0, 0);
            
            /* Enable Analog controls. First enable LDO. */
            appData.state = APP_STATE_ADC_ENABLE_LDO;
            SYS_TIME_DelayMS(1, &appData.timer);
            break;
        }

        case APP_STATE_ADC_ENABLE_LDO:
        {
            if ((appData.timer == SYS_TIME_HANDLE_INVALID) || (SYS_TIME_DelayIsComplete(appData.timer)))
            {
                if (EMAFE_ADCIsLDOEnable())
                {
                    appData.state = APP_STATE_ADC_ENABLE_BIAS;
                    appData.timer = SYS_TIME_HANDLE_INVALID;
                }
                else
                {
                    EMAFE_ADCEnableLDO();
                    SYS_TIME_DelayMS(10, &appData.timer);
                }
            }
        }
            break;

        case APP_STATE_ADC_ENABLE_BIAS:
        {
            if ((appData.timer == SYS_TIME_HANDLE_INVALID) || (SYS_TIME_DelayIsComplete(appData.timer)))
            {
                if (EMAFE_ADCIsBIASEnable())
                {
                    appData.state = APP_STATE_ADC_ENABLE_REF;
                    appData.timer = SYS_TIME_HANDLE_INVALID;
                }
                else
                {
                    EMAFE_ADCEnableBIAS();
                    SYS_TIME_DelayMS(10, &appData.timer);
                }
            }
        }
            break;

        case APP_STATE_ADC_ENABLE_REF:
        {
            if ((appData.timer == SYS_TIME_HANDLE_INVALID) || (SYS_TIME_DelayIsComplete(appData.timer)))
            {
                if (EMAFE_ADCIsREFEnable())
                {
                    appData.state = APP_STATE_FILTER_CONTROL_INIT;
                    appData.timer = SYS_TIME_HANDLE_INVALID;
                }
                else
                {
                    EMAFE_ADCEnableREF();
                    /* Wait 100 ms for VREF setting */
                    SYS_TIME_DelayMS(100, &appData.timer);
                }
            }
        }
            break;

        case APP_STATE_FILTER_CONTROL_INIT:
        {
            /* LPFIF reset */
            EMAFE_SoftReset();
            /* Decimation LPFIF Filters exits the reset state */
            EMAFE_SetSincDecimationFiltersReset(false);
            /* Set DSP clock prescaler ratio */
            EMAFE_SetClockPrescalerRatio(EMAFE_EMR_MCLKDIV_LPF_MCLKDIV4_Val);
            EMAFE_SetDataFormat(appData.dataFormat);
            EMAFE_SetOSR(EMAFE_EMR_OSR_OSR64_Val);
            
            appData.state = APP_STATE_FILTER_START_CAPTURE;
            SYS_TIME_DelayMS(2, &appData.timer);
            break;
        }

        case APP_STATE_FILTER_START_CAPTURE:
        {
            if (SYS_TIME_DelayIsComplete(appData.timer))
            {
                /* Register callback */
                EMAFE_CallbackRegister(_appEmafeCallback);
                
                /* Init Decimation Filters */
                EMAFE_EnableFilter(EMAFE_MR_LPF_ON2_Msk);
                
                /* Setup DMA */
                EMAFE_EnableDMAChannel(EMAFE_MR_DMACH2_Msk);
                
                EMAFE_DMATransfer(false, EMAFE_PTCR_RXTDIS_Msk);
                
                EMAFE_DMASetup(pSamplesBuffer, APP_SAMPLE_CHANNELS, NULL, 0);
                
                EMAFE_DMATransfer(true, EMAFE_PTCR_RXTEN_Msk);
                
                /* Enable Interrupt */
                EMAFE_EnableInterrupt(EMAFE_IER_ENDRX_Msk);
                
                appData.state = APP_STATE_FILTER_PRINT_SAMPLES;
            }
        }
            break;

        case APP_STATE_FILTER_PRINT_SAMPLES:
        {
            if (appData.newDataReady)
            {
                appData.newDataReady = false;
                appData.pSamplesData = pSamplesBuffer;
                appData.samplesCounter = 0;
                
                SYS_TIME_DelayMS(5, &appData.timer);
            }
            
            if (appData.pSamplesData != NULL)
            {
                if (SYS_TIME_DelayIsComplete(appData.timer))
                {
                    if (appData.samplesCounter < APP_SAMPLE_CHANNELS)
                    {
                        uint16_t index;
                        
                        for (index = 0; index < APP_SAMPLES_PRINT; index += 5)
                        {
                             uint32_t s0, s1, s2, s3, s4;

	                        if (appData.dataFormat == EMAFE_EMR_FORMAT_FULLRANGE_Val)
	                        {
	                            s0 = *appData.pSamplesData;
	                            appData.pSamplesData++;
	                            s1 = *appData.pSamplesData;
	                            appData.pSamplesData++;
	                            s2 = *appData.pSamplesData;
	                            appData.pSamplesData++;
	                            s3 = *appData.pSamplesData;
	                            appData.pSamplesData++;
	                            s4 = *appData.pSamplesData;
	                            appData.pSamplesData++;
	                        } 
	                        else 
	                        {
	                            s0 = *appData.pSamplesData << 8;
	                            appData.pSamplesData++;
	                            s1 = *appData.pSamplesData << 8;
	                            appData.pSamplesData++;
	                            s2 = *appData.pSamplesData << 8;
	                            appData.pSamplesData++;
	                            s3 = *appData.pSamplesData << 8;
	                            appData.pSamplesData++;
	                            s4 = *appData.pSamplesData << 8;
	                            appData.pSamplesData++;
	                        }

                            appData.samplesCounter += 5;

	                        SYS_CONSOLE_Print(SYS_CONSOLE_INDEX_0, "%08X\r\n%08X\r\n%08X\r\n%08X\r\n%08X\r\n", 
	                                      s0, s1, s2, s3, s4);
                    }

                        SYS_TIME_DelayMS(100, &appData.timer);
                    }
                    else
                    {
                        appData.pSamplesData = NULL;
                        appData.state = APP_STATE_FILTER_WAIT_BUTTON;
                    }
                }
            }
        }
            break;

        case APP_STATE_FILTER_WAIT_BUTTON:
        {
            if (appData.btnPressed == true)
            {
                appData.btnPressed = false;
                appData.state = APP_STATE_FILTER_CONTROL_INIT;
            }
        }
            break;

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
