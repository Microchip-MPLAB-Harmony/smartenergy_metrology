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

/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_metrology.c

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

#include "definitions.h"
#include "peripheral/icm/plib_icm.h"
#include "app_metrology.h"
#include "app_metrology_types.h"
#include "app_metrology_desc.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

/* ICM Hash area */
uint32_t appMetrologyOutputSHA[0x10] __ALIGNED(128);

extern APP_DATALOG_QUEUE appDatalogQueue;
APP_DATALOG_QUEUE_DATA appMetrologyDatalogQueueData;

extern APP_ENERGY_QUEUE appEnergyQueue;
extern APP_EVENTS_QUEUE appEventsQueue;

extern DRV_MCMETROLOGY_INIT drvMCMetrologyInitData;

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_METROLOGY_Initialize function.

    Application strings and buffers are be defined outside this structure.
 */

APP_METROLOGY_DATA CACHE_ALIGN app_metrologyData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************
static void _APP_METROLOGY_GetNVMDataCallback(APP_DATALOG_RESULT result);

static void _APP_METROLOGY_LoadControlFromMemory(DRV_MCMETROLOGY_REGS_CONTROL * controlReg)
{
    appMetrologyDatalogQueueData.userId = APP_DATALOG_USER_METROLOGY;
    appMetrologyDatalogQueueData.operation = APP_DATALOG_READ;
    appMetrologyDatalogQueueData.pData = (uint8_t *) controlReg;
    appMetrologyDatalogQueueData.dataLen = sizeof (DRV_MCMETROLOGY_REGS_CONTROL);
    appMetrologyDatalogQueueData.endCallback = _APP_METROLOGY_GetNVMDataCallback;
    appMetrologyDatalogQueueData.date.month = APP_DATALOG_INVALID_MONTH;
    appMetrologyDatalogQueueData.date.year = APP_DATALOG_INVALID_YEAR;

    APP_DATALOG_SendDatalogData(&appMetrologyDatalogQueueData);
}

static void _APP_METROLOGY_StoreControlInMemory(DRV_MCMETROLOGY_REGS_CONTROL * controlReg)
{
    appMetrologyDatalogQueueData.userId = APP_DATALOG_USER_METROLOGY;
    appMetrologyDatalogQueueData.operation = APP_DATALOG_WRITE;
    appMetrologyDatalogQueueData.pData = (uint8_t *) controlReg;
    appMetrologyDatalogQueueData.dataLen = sizeof (DRV_MCMETROLOGY_REGS_CONTROL);
    appMetrologyDatalogQueueData.endCallback = NULL;
    appMetrologyDatalogQueueData.date.month = APP_DATALOG_INVALID_MONTH;
    appMetrologyDatalogQueueData.date.year = APP_DATALOG_INVALID_YEAR;

    APP_DATALOG_SendDatalogData(&appMetrologyDatalogQueueData);
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

static void _APP_METROLOGY_ICMHashCompletedCallback(ICM_REGION_ID regionId)
{
	if (regionId == ICM_REGION_0) {
        app_metrologyData.metBinHashCompleted = true;
	} 
}

static void _APP_METROLOGY_ICMDigestMismatchCallback(ICM_REGION_ID regionId)
{
	if (regionId == ICM_REGION_0) {
		/* MCU reset. Additional actions could be done: reload metrology library without restarting MCU,
		count number of mismatches and store it lin NVM, ... */
        app_metrologyData.metBinMismatch = true;
	} 
}

static void _APP_METROLOGY_IntegrationCallback(void)
{
    if (app_metrologyData.state == APP_METROLOGY_STATE_RUNNING)
    {
        /* Signal Metrology thread to update measurements for an integration period */
        app_metrologyData.integrationFlag = true;
    }
}

static void _APP_METROLOGY_HarmonicAnalysisCallback(uint8_t harmonicNum)
{
    if (app_metrologyData.stopHarmonicAnalysis)
    {
        app_metrologyData.harmonicAnalysisPending = false;
        DRV_MCMETROLOGY_StopHarmonicAnalysis();
    }

    if (app_metrologyData.pHarmonicAnalysisCallback)
    {
        if (app_metrologyData.sendHarmonicsToConsole)
        {
            app_metrologyData.sendHarmonicsToConsole = false;
            app_metrologyData.pHarmonicAnalysisCallback(harmonicNum);
        }
    }
}

static void _APP_METROLOGY_HalfFullCycleCallback(void)
{
    if (app_metrologyData.state == APP_METROLOGY_STATE_RUNNING)
    {
        /* Signal Metrology thread to update events for a Half/Full Cycle */
        app_metrologyData.halfFullCycleFlag = true;
    }
}

static void _APP_METROLOGY_GetNVMDataCallback(APP_DATALOG_RESULT result)
{
    if (result == APP_DATALOG_RESULT_SUCCESS)
    {
        app_metrologyData.dataIsRdy = true;
    }

    // Handle task state
    app_metrologyData.dataFlag = true;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_METROLOGY_Initialize (void)

  Remarks:
    See prototype in app_metrology.h.
 */

void APP_METROLOGY_Initialize(void)
{
    /* Place the App state machine in its initial state. */
    app_metrologyData.state = APP_METROLOGY_STATE_ICM_GET_HASH;
    
    /* Get met bin file descriptor to check integrity (ICM) */
    app_metrologyData.metBinStartAddress = drvMCMetrologyInitData.binStartAddress;
    app_metrologyData.metBinSize = drvMCMetrologyInitData.binEndAddress - drvMCMetrologyInitData.binStartAddress;
    app_metrologyData.metBinHashCompleted = false;
    app_metrologyData.metBinMismatch = false;

    /* Flag to indicate if configuration should be applied */
    app_metrologyData.setConfiguration = false;

    /* Detection of the WDOG0 Reset */
    if (RSTC_ResetCauseGet() == RSTC_SR_RSTTYP(RSTC_SR_RSTTYP_WDT0_RST_Val))
    {
        app_metrologyData.startMode = DRV_MCMETROLOGY_START_SOFT;
    }
    else
    {
        app_metrologyData.startMode = DRV_MCMETROLOGY_START_HARD;
    }

    /* Get Pointers to metrology data regions */
    app_metrologyData.pMetControl = DRV_MCMETROLOGY_GetControlData();
    app_metrologyData.pMetStatus = DRV_MCMETROLOGY_GetStatusData();
    app_metrologyData.pMetAccData = DRV_MCMETROLOGY_GetAccData();
    app_metrologyData.pMetHarData = DRV_MCMETROLOGY_GetHarData();

    /* Get Pointers to metrology channel and power description */
    app_metrologyData.pMetChannelDesc = DRV_MCMETROLOGY_GetChannelDescription();
    app_metrologyData.pMetPowerDesc = DRV_MCMETROLOGY_GetPowersDescription();

    /* Set Callback for each metrology integration process */
    DRV_MCMETROLOGY_IntegrationCallbackRegister(_APP_METROLOGY_IntegrationCallback);
    /* Set Callback for harmonic analysis process */
    DRV_MCMETROLOGY_HarmonicAnalysisCallbackRegister(_APP_METROLOGY_HarmonicAnalysisCallback);
    /* Set Callback for full cycle */
    DRV_MCMETROLOGY_FullCycleCallbackRegister(_APP_METROLOGY_HalfFullCycleCallback);
    /* Set Callback for half cycle */
    DRV_MCMETROLOGY_HalfCycleCallbackRegister(_APP_METROLOGY_HalfFullCycleCallback);

    /* Clear Harmonic Analysis Data */
    app_metrologyData.harmonicAnalysisPending = false;
    app_metrologyData.pHarmonicAnalysisCallback = NULL;
    app_metrologyData.pHarmonicAnalysisResponse = NULL;

    /* Initialize integration Flag */
    app_metrologyData.integrationFlag = false;
    app_metrologyData.halfFullCycleFlag = false;
    app_metrologyData.dataFlag = false;
    app_metrologyData.eventFlagsPrev.swell = 0;
    app_metrologyData.eventFlagsPrev.sag = 0;
    app_metrologyData.eventFlagsPrev.creep = 0;
    app_metrologyData.eventFlagsPrev.phActive = 0;

}

/******************************************************************************
  Function:
    void APP_METROLOGY_Tasks (void)

  Remarks:
    See prototype in app_metrology.h.
 */
void APP_METROLOGY_Tasks(void)
{
    APP_ENERGY_QUEUE_DATA newMetrologyData;
    APP_EVENTS_QUEUE_DATA newEvent;
    
    if (app_metrologyData.metBinMismatch)
    {
        app_metrologyData.state = APP_METROLOGY_STATE_ERROR;
    }
    
    /* Check the application's current state. */
    switch (app_metrologyData.state)
    {
        case APP_METROLOGY_STATE_ICM_GET_HASH:
        {
            ICM_Disable();
            
            ICM_SetRegionDescriptorData(ICM_REGION_0, 
                                        (uint32_t *)app_metrologyData.metBinStartAddress,
                                        app_metrologyData.metBinSize);
            
            ICM_SetHashAreaAddress((uint32_t)appMetrologyOutputSHA);
                
            // Set ICM callbacks (Hash Completed))
            ICM_CallbackRegister(ICM_INTERRUPT_RHC, _APP_METROLOGY_ICMHashCompletedCallback);
            ICM_EnableInterrupt(ICM_INTERRUPT_RHC, ICM_REGION_0);
            
            app_metrologyData.state = APP_METROLOGY_STATE_ICM_START_MONITOR;
            app_metrologyData.metBinHashCompleted = false;
            
            // Compute de Hash values
            ICM_Enable();
            
            break;
        }
        
        case APP_METROLOGY_STATE_ICM_START_MONITOR:
        {
            if (app_metrologyData.metBinHashCompleted == true)
            {
                ICM_REGION_DESCRIPTOR *pDescriptor;

                ICM_Disable();
            
                ICM_SetRegionDescriptorData(ICM_REGION_0, (uint32_t *)IRAM1_ADDR,
                                            app_metrologyData.metBinSize);

                // Disable ICM callbacks (Hash Completed))
                ICM_CallbackRegister(ICM_INTERRUPT_RHC, NULL);
                ICM_DisableInterrupt(ICM_INTERRUPT_RHC, ICM_REGION_0);
                
                // Set ICM Monitor Mode
                pDescriptor = ICM_GetRegionDescriptor(ICM_REGION_0);
                pDescriptor->config.bitfield.compareMode = 1;
                pDescriptor->config.bitfield.wrap = 1;
                pDescriptor->config.bitfield.endMonitor = 0;
                pDescriptor->config.bitfield.regHashIntDis = 1;
                pDescriptor->config.bitfield.mismatchIntDis = 0;
                
                // Set ICM callbacks (Digest mismatch)
                ICM_CallbackRegister(ICM_INTERRUPT_RDM, _APP_METROLOGY_ICMDigestMismatchCallback);
                ICM_EnableInterrupt(ICM_INTERRUPT_RDM, ICM_REGION_0);
                
                ICM_SetMonitorMode(true, 15);
                ICM_EnableRegionMonitor(ICM_REGION_0);
                
                ICM_Enable();

                app_metrologyData.state = APP_METROLOGY_STATE_WAITING_DATALOG;
            }
            break;
        }
        
        case APP_METROLOGY_STATE_WAITING_DATALOG:
        {
            if (APP_DATALOG_GetStatus() == APP_DATALOG_STATE_READY)
            {
                /* Check if there are Metrology data in memory */
                if (APP_DATALOG_FileExists(APP_DATALOG_USER_METROLOGY, NULL))
                {
                    /* Metrology data exists */
                    _APP_METROLOGY_LoadControlFromMemory(&app_metrologyData.configuration);
                    /* Wait for the loading data from memory */
                    app_metrologyData.state = APP_METROLOGY_STATE_WAIT_DATA;
                }
                else
                {
                    app_metrologyData.state = APP_METROLOGY_STATE_INIT;
                }
            }
            break;
        }

        case APP_METROLOGY_STATE_WAIT_DATA:
        {
            /* Apply Configuration Data */
            if (app_metrologyData.dataIsRdy)
            {
                /* Update Flag to apply external configuration */
                app_metrologyData.setConfiguration = true;
                app_metrologyData.state = APP_METROLOGY_STATE_INIT;
            }
            break;
        }

        case APP_METROLOGY_STATE_INIT:
        {
            DRV_MCMETROLOGY_REGS_CONTROL * pConfiguration = NULL;

            /* Check if external configuration should be applied */
            if (app_metrologyData.setConfiguration)
            {
                pConfiguration = &app_metrologyData.configuration;
            }
            
            if (DRV_MCMETROLOGY_Open(app_metrologyData.startMode, pConfiguration) == DRV_MCMETROLOGY_SUCCESS)
            {
                if (app_metrologyData.startMode == DRV_MCMETROLOGY_START_HARD)
                {
                    app_metrologyData.state = APP_METROLOGY_STATE_START;
                }
                else
                {
                    // Check ICM status before running metrology
                    if ((ICM_GetStatus() & ICM_SR_ENABLE_Msk) == 0)
                    {
                        ICM_Enable();
                    }
                    app_metrologyData.state = APP_METROLOGY_STATE_RUNNING;
                }
            }
            else
            {
                app_metrologyData.state = APP_METROLOGY_STATE_ERROR;
            }

            break;
        }

        case APP_METROLOGY_STATE_START:
        {
            if (DRV_MCMETROLOGY_GetStatus() == DRV_MCMETROLOGY_STATUS_READY)
            {
                /* Check if there are Metrology data in memory */
                if (APP_DATALOG_FileExists(APP_DATALOG_USER_METROLOGY, NULL) == false)
                {
                    /* Metrology data does not exists. Store in NVM */
                    _APP_METROLOGY_StoreControlInMemory(app_metrologyData.pMetControl);
                }

                if (DRV_MCMETROLOGY_Start() == DRV_MCMETROLOGY_SUCCESS)
                {
                    // Check ICM status before running metrology
                    if ((ICM_GetStatus() & ICM_SR_ENABLE_Msk) == 0)
                    {
                        ICM_Enable();
                    }
                    app_metrologyData.state = APP_METROLOGY_STATE_RUNNING;
                }
                else
                {
                    app_metrologyData.state = APP_METROLOGY_STATE_ERROR;
                }

            }

            break;
        }

        case APP_METROLOGY_STATE_RUNNING:
        {
            /* Wait for the integration flag to get measurements at the end of the integration period. */
            if (app_metrologyData.integrationFlag)
            {
                app_metrologyData.integrationFlag = false;

                // Send new Energy values to the Energy Task
                newMetrologyData.energy = DRV_MCMETROLOGY_GetEnergyValue(true);
                newMetrologyData.Pt = DRV_MCMETROLOGY_GetMeasureValue(MEASURE_PT);
                if (APP_ENERGY_SendEnergyData(&newMetrologyData) == false)
                {
                    SYS_CMD_MESSAGE("ENERGY Queue is FULL!!!\r\n");
                }

                // Send new Events to the Events Task
                RTC_TimeGet(&newEvent.eventTime);
                DRV_MCMETROLOGY_GetEventsData(&newEvent.eventFlags);
                if ((app_metrologyData.eventFlagsPrev.swell != newEvent.eventFlags.swell) || 
                        (app_metrologyData.eventFlagsPrev.sag != newEvent.eventFlags.sag) ||
                        (app_metrologyData.eventFlagsPrev.creep != newEvent.eventFlags.creep) ||
                        (app_metrologyData.eventFlagsPrev.phActive != newEvent.eventFlags.phActive))
                {
                    app_metrologyData.eventFlagsPrev = newEvent.eventFlags;
                    if (APP_EVENTS_RegisterEventsData(&newEvent) == false)
                    {
                        SYS_CMD_MESSAGE("EVENTS Queue is FULL!!!\r\n");
                    }
                }
            }
            
            /* Wait for the Half/Full Cycle to get events. */
            if (app_metrologyData.halfFullCycleFlag)
            {
                app_metrologyData.halfFullCycleFlag = false;

                // Send new Events to the Events Task
                RTC_TimeGet(&newEvent.eventTime);
                DRV_MCMETROLOGY_GetEventsData(&newEvent.eventFlags);
                if ((app_metrologyData.eventFlagsPrev.swell != newEvent.eventFlags.swell) || 
                        (app_metrologyData.eventFlagsPrev.sag != newEvent.eventFlags.sag) ||
                        (app_metrologyData.eventFlagsPrev.creep != newEvent.eventFlags.creep) ||
                        (app_metrologyData.eventFlagsPrev.phActive != newEvent.eventFlags.phActive))
                {
                    app_metrologyData.eventFlagsPrev = newEvent.eventFlags;
                    if (APP_EVENTS_RegisterEventsData(&newEvent) == false)
                    {
                        SYS_CMD_MESSAGE("EVENTS Queue is FULL!!!\r\n");
                    }
                }
            }

            break;
        }

            /* The default state should never be executed. */
        case APP_METROLOGY_STATE_ERROR:
        {
            if (app_metrologyData.metBinMismatch)
            {
                app_metrologyData.metBinMismatch = false;
                SYS_CMD_MESSAGE("ERROR: Metrology library has been corrupted. Results are not valid.\r\n");
            }
        }
            break;
			
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

APP_METROLOGY_STATE APP_METROLOGY_GetState(void)
{
    return app_metrologyData.state;
}

bool APP_METROLOGY_GetControlRegister(CONTROL_REG_ID regId, uint32_t * regValue, char *regName)
{
    uint32_t *pData;

    if (regId >= CONTROL_REG_NUM)
    {
        return false;
    }

    pData = (uint32_t *)app_metrologyData.pMetControl;
    pData += regId;
    *regValue = *pData;

    if (regName)
    {
        sprintf(regName, "%s", APP_METROLOGY_GetRegControlDescr(regId));
    }

    return true;

}

bool APP_METROLOGY_SetControlRegister(CONTROL_REG_ID regId, uint32_t value)
{
    uint32_t *pData;

    if (regId >= CONTROL_REG_NUM)
    {
        return false;
    }

    pData = (uint32_t *)app_metrologyData.pMetControl;
    pData += regId;
    *pData = value;

    pData = (uint32_t *)&app_metrologyData.configuration;
    pData += regId;
    *pData = value;

    return true;
}

bool APP_METROLOGY_GetStatusRegister(STATUS_REG_ID regId, uint32_t * regValue, char *regName)
{
    uint32_t *pData;

    if (regId >= STATUS_REG_NUM)
    {
        return false;
    }

    pData = (uint32_t *)app_metrologyData.pMetStatus;
    pData += regId;
    *regValue = *pData;

    if (regName)
    {
        sprintf(regName, "%s", APP_METROLOGY_GetRegStatusDescr(regId));
    }

    return true;
}

bool APP_METROLOGY_GetAccumulatorRegister(ACCUMULATOR_REG_ID regId, uint64_t * regValue, char *regName)
{
    uint64_t *pData;

    if (regId >= ACCUMULATOR_REG_NUM)
    {
        return false;
    }

    pData = (uint64_t *) app_metrologyData.pMetAccData;
    pData += regId;
    *regValue = *pData;

    if (regName)
    {
        sprintf(regName, "%s", APP_METROLOGY_GetRegAccDescr(regId));
    }

    return true;
}

void APP_METROLOGY_CaptureHarmonicData(void)
{
    (void) memcpy((uint8_t *) & app_metrologyData.harmonicsData,
                  (uint8_t *) app_metrologyData.pMetHarData, sizeof (DRV_MCMETROLOGY_REGS_HARMONICS));

}

bool APP_METROLOGY_GetHarmonicRegister(HARMONICS_REG_ID regId, uint32_t *regValue, char *regName)
{
    uint32_t *pData;

    if (regId >= HARMONICS_REG_NUM)
    {
        return false;
    }

    pData = (uint32_t *)app_metrologyData.pMetHarData;
    pData += regId;
    *regValue = *pData;

    if (regName)
    {
        sprintf(regName, "%s", APP_METROLOGY_GetRegHarDescr(regId));
    }

    return true;
}

bool APP_METROLOGY_GetMeasure(DRV_MCMETROLOGY_MEASURE_TYPE measureId, 
                              float * value)
{
    if (measureId >= MEASURE_TYPE_NUM)
    {
        return false;
    }

    *value = DRV_MCMETROLOGY_GetMeasureValue(measureId);
    return true;
}

bool APP_METROLOGY_GetChannelMeasure(DRV_MCMETROLOGY_CHANNEL_MEASURE_TYPE measureId, 
                                     uint8_t channelIndex, float * value)
{
    if (measureId >= CHN_MEASURE_TYPE_NUM)
    {
        return false;
    }

    *value = DRV_MCMETROLOGY_GetChannelMeasureValue(channelIndex, measureId);
    return true;
}

bool APP_METROLOGY_GetPowerMeasure(DRV_MCMETROLOGY_POWER_MEASURE_TYPE measureId, 
                                   uint8_t powerIndex, float * value)
{
    if (measureId >= POW_MEASURE_TYPE_NUM)
    {
        return false;
    }

    *value = DRV_MCMETROLOGY_GetPowerMeasureValue(powerIndex, measureId);
    return true;
}

void APP_METROLOGY_SetControlByDefault(void)
{
    DRV_MCMETROLOGY_REGS_CONTROL *pSrc;

    pSrc = DRV_MCMETROLOGY_GetControlByDefault();
    DRV_MCMETROLOGY_SetControl(pSrc);

    memcpy(&app_metrologyData.configuration, pSrc, sizeof(DRV_MCMETROLOGY_REGS_CONTROL));
}

void APP_METROLOGY_StoreMetrologyData(void)
{
    _APP_METROLOGY_StoreControlInMemory(app_metrologyData.pMetControl);
}

void APP_METROLOGY_SetConfiguration(DRV_MCMETROLOGY_CONFIGURATION * config)
{
    DRV_MCMETROLOGY_SetConfiguration(config);

    _APP_METROLOGY_StoreControlInMemory(app_metrologyData.pMetControl);
}

size_t APP_METROLOGY_GetWaveformCaptureData(uint32_t *address)
{
    *address = (uint32_t) app_metrologyData.pMetControl->CAPTURE_ADDR;
    return (size_t) app_metrologyData.pMetControl->CAPTURE_BUFF_SIZE;
}

bool APP_METROLOGY_StartHarmonicAnalysis(uint8_t harmonicNum, bool singleMode)
{
    if (app_metrologyData.harmonicAnalysisPending)
    {
        return false;
    }

    if (app_metrologyData.pHarmonicAnalysisCallback == NULL)
    {
        return false;
    }

    if (app_metrologyData.pHarmonicAnalysisResponse == NULL)
    {
        return false;
    }

    if (DRV_MCMETROLOGY_StartHarmonicAnalysis(harmonicNum, app_metrologyData.pHarmonicAnalysisResponse))
    {
        app_metrologyData.harmonicAnalysisPending = true;
        app_metrologyData.stopHarmonicAnalysis = singleMode;
        app_metrologyData.sendHarmonicsToConsole = true;
        return true;
    }
    else
    {
        return false;
    }
}

void APP_METROLOGY_StopHarmonicAnalysis(void)
{
    app_metrologyData.stopHarmonicAnalysis = true;
    app_metrologyData.sendHarmonicsToConsole = true;
}

void APP_METROLOGY_SetHarmonicAnalysisCallback(DRV_MCMETROLOGY_HARMONICS_CALLBACK callback,
                                               DRV_MCMETROLOGY_HARMONICS_RMS * pHarmonicAnalysisResponse)
{
    app_metrologyData.pHarmonicAnalysisCallback = callback;
    app_metrologyData.pHarmonicAnalysisResponse = pHarmonicAnalysisResponse;
}

void APP_METROLOGY_Restart(bool reloadRegsFromMemory)
{
    DRV_MCMETROLOGY_RESULT result;

    result = DRV_MCMETROLOGY_Close();
    if (result == DRV_MCMETROLOGY_SUCCESS)
    {
        if (reloadRegsFromMemory)
        {
            app_metrologyData.state = APP_METROLOGY_STATE_WAITING_DATALOG;
        }
        else
        {
            app_metrologyData.state = APP_METROLOGY_STATE_INIT;
        }
        app_metrologyData.startMode = DRV_MCMETROLOGY_START_HARD;

        /* Disable ICM before resetting peripheral clocks */
        ICM_Disable();

        sysObj.drvMCMet = DRV_MCMETROLOGY_Reinitialize((SYS_MODULE_INIT *)&drvMCMetrologyInitData);
    }
}

void APP_METROLOGY_SetLowPowerMode(void)
{
    DRV_MCMETROLOGY_Close();
    SUPC_BackupModeEnter();
}

bool APP_METROLOGY_CheckIsVoltageChannel(uint8_t channel)
{
    bool isVoltage = false;

    if (channel <= DRV_MCMETROLOGY_CHANNELS_NUMBER)
    {
        DRV_MCMETROLOGY_CHANNEL * pChannelDesc;

        pChannelDesc = (DRV_MCMETROLOGY_CHANNEL *) (app_metrologyData.pMetChannelDesc + channel);
        if (pChannelDesc->sensorType == SENSOR_VRD)
        {
            isVoltage = true;
        }
    }

    return isVoltage;
}

void APP_METROLOGY_StopMetrology (void)
{
    DRV_MCMETROLOGY_Close();
}

DRV_MCMETROLOGY_AFE_TYPE APP_METROLOGY_GetAFEDescription(void)
{
    return DRV_MCMETROLOGY_GetAFEDescription();
}

uint8_t APP_METROLOGY_GetChannelsDescription(DRV_MCMETROLOGY_CHANNEL **pChannelDesc)
{
    *pChannelDesc = app_metrologyData.pMetChannelDesc;
    
    return DRV_MCMETROLOGY_CHANNELS_NUMBER;
}

uint8_t APP_METROLOGY_GetPowersDescription(DRV_MCMETROLOGY_POWER_SOURCE **pPowersDesc)
{
    *pPowersDesc = app_metrologyData.pMetPowerDesc;
    
    return DRV_MCMETROLOGY_POWERS_NUMBER;
}

/*******************************************************************************
 End of File
 */
