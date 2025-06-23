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
#include "app_metrology.h"
#include "app_metrology_types.h"
#include "app_metrology_desc.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

/* Define a semaphore to signal the Metrology Tasks to process new integration
 * data */
OSAL_SEM_DECLARE(appMetrologySemID);
/* Define a semaphore to signal the Metrology Calibration Tasks */
OSAL_SEM_DECLARE(appMetrologyCalibrationSemID);

extern QueueHandle_t appDatalogQueueID;
APP_DATALOG_QUEUE_DATA appMetrologyDatalogQueueData;

extern QueueHandle_t appEnergyQueueID;
extern QueueHandle_t appEventsQueueID;

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

    xQueueSend(appDatalogQueueID, &appMetrologyDatalogQueueData, (TickType_t) 0);
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

    xQueueSend(appDatalogQueueID, &appMetrologyDatalogQueueData, (TickType_t) 0);
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

static void _APP_METROLOGY_IntegrationCallback(void)
{
    if (app_metrologyData.state == APP_METROLOGY_STATE_RUNNING)
    {
        /* Signal Metrology thread to update measurements for an integration period */
        app_metrologyData.integrationFlag = true;
        OSAL_SEM_Post(&appMetrologySemID);
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
        OSAL_SEM_PostISR(&appMetrologySemID);
    }
}

static void _APP_METROLOGY_GetNVMDataCallback(APP_DATALOG_RESULT result)
{
    if (result == APP_DATALOG_RESULT_SUCCESS)
    {
        app_metrologyData.dataIsRdy = true;
    }

    // Post semaphore to wakeup task
    OSAL_SEM_Post(&appMetrologySemID);
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
    app_metrologyData.state = APP_METROLOGY_STATE_WAITING_DATALOG;

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

    /* Create the Metrology Integration Semaphore. */
    if (OSAL_SEM_Create(&appMetrologySemID, OSAL_SEM_TYPE_BINARY, 0, 0) == OSAL_RESULT_FALSE)
    {
        /* Handle error condition. Not sufficient memory to create semaphore */
    }

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

    /* Check the application's current state. */
    switch (app_metrologyData.state)
    {
        /* Application's initial state. */
        case APP_METROLOGY_STATE_WAITING_DATALOG:
        {
            if (APP_DATALOG_GetStatus() == APP_DATALOG_STATE_READY)
            {
                /* Check if there are Metrology data in memory */
                if (APP_DATALOG_FileExists(APP_DATALOG_USER_METROLOGY, NULL))
                {
                    /* Metrology data exists */
                    _APP_METROLOGY_LoadControlFromMemory(&app_metrologyData.configuration);
                    /* Wait for the semaphore to load data from memory */
                    OSAL_SEM_Pend(&appMetrologySemID, OSAL_WAIT_FOREVER);

                    /* Apply COnfiguration Data */
                    if (app_metrologyData.dataIsRdy)
                    {
                        /* Update Flag to apply external configuration */
                        app_metrologyData.setConfiguration = true;
                    }
                }
                app_metrologyData.state = APP_METROLOGY_STATE_INIT;
            }

            vTaskDelay(10 / portTICK_PERIOD_MS);
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
            
            /* Reinitialize Metrology, to ensure Core 1 and Peripherals are reset */
            sysObj.drvMCMet = DRV_MCMETROLOGY_Reinitialize((SYS_MODULE_INIT *)&drvMCMetrologyInitData);

            if (DRV_MCMETROLOGY_Open(app_metrologyData.startMode, pConfiguration) == DRV_MCMETROLOGY_SUCCESS)
            {
                if (app_metrologyData.startMode == DRV_MCMETROLOGY_START_HARD)
                {
                    app_metrologyData.state = APP_METROLOGY_STATE_START;
                }
                else
                {
                    app_metrologyData.state = APP_METROLOGY_STATE_RUNNING;
                }
            }
            else
            {
                app_metrologyData.state = APP_METROLOGY_STATE_ERROR;
            }

            vTaskDelay(10 / portTICK_PERIOD_MS);
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
                    app_metrologyData.state = APP_METROLOGY_STATE_RUNNING;
                }
                else
                {
                    app_metrologyData.state = APP_METROLOGY_STATE_ERROR;
                }

                vTaskDelay(10 / portTICK_PERIOD_MS);
            }

            break;
        }

        case APP_METROLOGY_STATE_RUNNING:
        {
            /* Wait for the metrology semaphore to get measurements at the end of the integration period. */
            OSAL_SEM_Pend(&appMetrologySemID, OSAL_WAIT_FOREVER);

            if (app_metrologyData.state == APP_METROLOGY_STATE_INIT)
            {
                /* Received Reload Command */
                break;
            }
            
            if (app_metrologyData.integrationFlag)
            {
                app_metrologyData.integrationFlag = false;

                // Send new Energy values to the Energy Task
                app_metrologyData.queueFree = uxQueueSpacesAvailable(appEnergyQueueID);
                if (app_metrologyData.queueFree)
                {
                    newMetrologyData.energy = DRV_MCMETROLOGY_GetEnergyValue(true);
                    newMetrologyData.Pt = DRV_MCMETROLOGY_GetMeasureValue(MEASURE_PT);
                    xQueueSend(appEnergyQueueID, &newMetrologyData, (TickType_t) 0);
                }
                else
                {
                    SYS_CMD_MESSAGE("ENERGY Queue is FULL!!!\r\n");
                }

                // Send new Events to the Events Task
                app_metrologyData.queueFree = uxQueueSpacesAvailable(appEventsQueueID);
                if (app_metrologyData.queueFree)
                {
                    RTC_TimeGet(&newEvent.eventTime);
                    DRV_MCMETROLOGY_GetEventsData(&newEvent.eventFlags);
                    xQueueSend(appEventsQueueID, &newEvent, (TickType_t) 0);
                }
                else
                {
                    SYS_CMD_MESSAGE("EVENTS Queue is FULL!!!\r\n");
                }
            }
            
            if (app_metrologyData.halfFullCycleFlag)
            {
                app_metrologyData.halfFullCycleFlag = false;
                
                // Send new Events to the Events Task
                app_metrologyData.queueFree = uxQueueSpacesAvailable(appEventsQueueID);
                if (app_metrologyData.queueFree)
                {
                    RTC_TimeGet(&newEvent.eventTime);
                    DRV_MCMETROLOGY_GetEventsData(&newEvent.eventFlags);
                    xQueueSend(appEventsQueueID, &newEvent, (TickType_t) 0);
                }
                else
                {
                    SYS_CMD_MESSAGE("EVENTS Queue is FULL!!!\r\n");
                }
            }

            break;
        }

        /* The default state should never be executed. */
        case APP_METROLOGY_STATE_ERROR:
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

    pData = (uint64_t *)app_metrologyData.pMetAccData;
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
    (void) memcpy((uint8_t *)&app_metrologyData.harmonicsData,
                  (uint8_t *)app_metrologyData.pMetHarData, sizeof (DRV_MCMETROLOGY_REGS_HARMONICS));

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
