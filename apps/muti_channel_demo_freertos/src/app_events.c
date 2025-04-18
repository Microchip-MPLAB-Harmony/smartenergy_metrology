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
    app_events.c

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
#include "app_events.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

/* Define a semaphore to signal the Metrology Tasks to process new integration
 * data */
OSAL_SEM_DECLARE(appEventsSemID);

/* Define a queue to signal the Events Task to handle metrology events */
QueueHandle_t appEventsQueueID = NULL;

extern QueueHandle_t appDatalogQueueID;
APP_DATALOG_QUEUE_DATA appEventsDatalogQueueData;
// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_EVENTS_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_EVENTS_DATA app_eventsData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

void _APP_EVENTS_GetDataLogCallback(APP_DATALOG_RESULT result)
{
    if (result == APP_DATALOG_RESULT_SUCCESS)
    {
        app_eventsData.dataIsRdy = true;
    }

    // Post semaphore to wakeup task
    OSAL_SEM_Post(&appEventsSemID);
}

static void _APP_EVENTS_LoadEvenstDataFromMemory(void)
{
    appEventsDatalogQueueData.userId = APP_DATALOG_USER_EVENTS;
    appEventsDatalogQueueData.operation = APP_DATALOG_READ;
    appEventsDatalogQueueData.pData = (uint8_t *)&app_eventsData.events;
    appEventsDatalogQueueData.dataLen = sizeof(APP_EVENTS_DATA);
    appEventsDatalogQueueData.endCallback = _APP_EVENTS_GetDataLogCallback;
    appEventsDatalogQueueData.date.month = APP_DATALOG_INVALID_MONTH;
    appEventsDatalogQueueData.date.year = APP_DATALOG_INVALID_YEAR;

    xQueueSend(appDatalogQueueID, &appEventsDatalogQueueData, (TickType_t) 0);
}

static void _APP_EVENTS_StoreEventsDataInMemory(void)
{
    appEventsDatalogQueueData.userId = APP_DATALOG_USER_EVENTS;
    appEventsDatalogQueueData.operation = APP_DATALOG_WRITE;
    appEventsDatalogQueueData.pData = (uint8_t *)&app_eventsData.events;
    appEventsDatalogQueueData.dataLen = sizeof(APP_EVENTS_DATA);
    appEventsDatalogQueueData.endCallback = NULL;
    appEventsDatalogQueueData.date.month = APP_DATALOG_INVALID_MONTH;
    appEventsDatalogQueueData.date.year = APP_DATALOG_INVALID_YEAR;

    xQueueSend(appDatalogQueueID, &appEventsDatalogQueueData, (TickType_t) 0);
}

static bool _APP_EVENTS_RegisterEvent(APP_EVENTS_EVENT_ID type, uint16_t eventValue, uint8_t channel, struct tm * timeEvent)
{
    APP_EVENTS_EVENT_DATA * eventData;
    bool registered = false;
    bool enabled;

    eventData = &app_eventsData.events.event[type];
    enabled = (eventValue & (1<<channel))? true: false;
    switch (eventData->status)
    {
        case NO_EVENT:
        {
            if (enabled)
            {
                /* Start Holding Start time */
                eventData->status = EVENT_HOLDING_START;
                eventData->holdingCounter = EVENT_HOLDING_START_COUNTER;
            }
            break;
        }

        case EVENT_HOLDING_START:
        {
            APP_EVENTS_EVENT_INFO * eventInfo;

            /* Register Starting Event */
            eventInfo = &eventData->data[eventData->dataIndex];

            if (enabled)
            {
                eventData->holdingCounter--;
                if (eventData->holdingCounter == 0)
                {
                    /* Register Starting Event */
                    eventInfo->startTime = *timeEvent;
                    memset(&eventInfo->endTime, 0, sizeof(struct tm));

                    eventData->status = EVENT_START;
                }
            }
            else
            {
                /* Cancel Starting event */
                eventData->status = NO_EVENT;
                /* Clean starting time */
                memset(&eventInfo->startTime, 0, sizeof(struct tm));
            }

            break;
        }

        case EVENT_START:
        {
            if (enabled == 0)
            {
                /* Start Holding End time */
                eventData->status = EVENT_HOLDING_END;
                eventData->holdingCounter = EVENT_HOLDING_END_COUNTER;
            }
            break;
        }

        case EVENT_HOLDING_END:
        {
            if (enabled == 0)
            {
                eventData->holdingCounter--;
                if (eventData->holdingCounter == 0)
                {
                    APP_EVENTS_EVENT_INFO * eventInfo;

                    /* Register Ending Event */
                    eventInfo = &eventData->data[eventData->dataIndex];
                    eventInfo->endTime = *timeEvent;

                    /* Set index to next logged data */
                    eventData->dataIndex++;
                    eventData->dataIndex %= EVENT_LOG_MAX_NUMBER;

                    /* Clear data of the next index */
                    eventInfo = &eventData->data[eventData->dataIndex];
                    memset(eventInfo, 0, sizeof(APP_EVENTS_EVENT_INFO));

                    eventData->counter++;
                    eventData->status = NO_EVENT;

                    /* Register event is completed */
                    registered = true;
                }
            }
            else
            {
                /* Cancel Ending event */
                eventData->status = EVENT_START;
            }

            break;
        }
    }

    return registered;
}

static bool _APP_EVENTS_UpdateEvents(APP_EVENTS_QUEUE_DATA * newEvent)
{
    APP_EVENTS_EVENT_ID eventId;
    uint8_t channel;
    bool update = false;
    
    // Register events depending on the number of channels
    for (channel = 0; channel < DRV_MCMETROLOGY_CHANNELS_NUMBER; channel++)
    {
        eventId = SWELL_TH_0_EVENT_ID + channel;
        if (_APP_EVENTS_RegisterEvent(eventId, newEvent->eventFlags.swell, channel, &newEvent->eventTime))
        {
            update = true;
        }
        
        eventId = SAG_TH_0_EVENT_ID + channel;
        if (_APP_EVENTS_RegisterEvent(eventId, newEvent->eventFlags.sag, channel, &newEvent->eventTime))
        {
            update = true;
        }
        
        eventId = CREEP_TH_0_EVENT_ID + channel;
        if (_APP_EVENTS_RegisterEvent(eventId, newEvent->eventFlags.creep, channel, &newEvent->eventTime))
        {
            update = true;
        }
        
        eventId = PHACTIVE_TH_0_EVENT_ID + channel;
        if (_APP_EVENTS_RegisterEvent(eventId, newEvent->eventFlags.phActive, channel, &newEvent->eventTime))
        {
            update = true;
        }
    }

    /* Update Event Flags */
    app_eventsData.flags = newEvent->eventFlags;

    return update;
}


static bool _APP_EVENTS_CheckCustomEvents(APP_EVENTS_CUSTOM_EVENT **customEvent)
{
    if (app_eventsData.pCustomEventsCallback != NULL)
    {
        APP_EVENTS_CUSTOM_EVENT *pCustomEvents;
        uint8_t index;

        pCustomEvents = app_eventsData.customEventsData;
        for (index = 0; index < APP_EVENTS_MAX_CUSTOM_EVENTS; index++)
        {
            if (pCustomEvents->enabled == true)
            {
                if (pCustomEvents->chnIndex < DRV_MCMETROLOGY_CHANNELS_NUMBER)
                {
                    APP_METROLOGY_GetChannelMeasure((DRV_MCMETROLOGY_CHANNEL_MEASURE_TYPE)pCustomEvents->measure, 
                                                    pCustomEvents->chnIndex, 
                                                    &pCustomEvents->currentValue);
                }
                else if (pCustomEvents->powIndex < DRV_MCMETROLOGY_POWERS_NUMBER)
                {
                    APP_METROLOGY_GetPowerMeasure((DRV_MCMETROLOGY_POWER_MEASURE_TYPE)pCustomEvents->measure, 
                                                    pCustomEvents->powIndex, 
                                                    &pCustomEvents->currentValue);
                }
                else 
                {
                    APP_METROLOGY_GetMeasure((DRV_MCMETROLOGY_MEASURE_TYPE)pCustomEvents->measure, 
                                            &pCustomEvents->currentValue);
                }

                if (pCustomEvents->currentValue >= pCustomEvents->threshold)
                {
                    *customEvent = pCustomEvents;
                    return true;
                }
            }

            pCustomEvents++;
        }
    }
    
    return false;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_EVENTS_Initialize ( void )

  Remarks:
    See prototype in app_events.h.
 */

void APP_EVENTS_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    app_eventsData.state = APP_EVENTS_STATE_WAITING_DATALOG;

    /* Initialize Events data */
    memset(&app_eventsData.events, 0, sizeof(APP_EVENTS_EVENTS));

    /* Create the Switches Semaphore. */
    if (OSAL_SEM_Create(&appEventsSemID, OSAL_SEM_TYPE_BINARY, 0, 0) == OSAL_RESULT_FALSE)
    {
        /* Handle error condition. Not sufficient memory to create semaphore */
        app_eventsData.state = APP_EVENTS_STATE_ERROR;
    }

    // Create a queue capable of containing APP_EVENTS_QUEUE_DATA_SIZE queue data elements.
    appEventsQueueID = xQueueCreate(APP_EVENTS_QUEUE_DATA_SIZE, sizeof(APP_EVENTS_QUEUE_DATA));

    if (appEventsQueueID == NULL)
    {
        // Queue was not created and must not be used.
        app_eventsData.state = APP_EVENTS_STATE_ERROR;
        return;
    }
}


/******************************************************************************
  Function:
    void APP_EVENTS_Tasks ( void )

  Remarks:
    See prototype in app_events.h.
 */

void APP_EVENTS_Tasks ( void )
{
    /* Check the application's current state. */
    switch ( app_eventsData.state )
    {
        case APP_EVENTS_STATE_WAITING_DATALOG:
        {
            if (APP_DATALOG_GetStatus() == APP_DATALOG_STATE_READY)
            {
                app_eventsData.state = APP_EVENTS_STATE_INIT;
            }

            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }

        case APP_EVENTS_STATE_INIT:
        {
            /* Reset flag to request data to datalog app */
            app_eventsData.dataIsRdy = false;

            /* Check if there are ENERGY data in memory */
            if (APP_DATALOG_FileExists(APP_DATALOG_USER_EVENTS, NULL))
            {
                /* EVENTS data exists */
                _APP_EVENTS_LoadEvenstDataFromMemory();
                /* Wait for the semaphore to load data from memory */
                OSAL_SEM_Pend(&appEventsSemID, OSAL_WAIT_FOREVER);
            }
            else
            {
                /* There is no valid data in memory. Create Events Data in memory. */
                _APP_EVENTS_StoreEventsDataInMemory();
            }

            app_eventsData.state = APP_EVENTS_STATE_RUNNING;

            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        }

        case APP_EVENTS_STATE_RUNNING:
        {
            APP_EVENTS_CUSTOM_EVENT *customEvent;
            
            if (xQueueReceive(appEventsQueueID, &app_eventsData.newEvent, portMAX_DELAY) == pdPASS)
            {
                if (_APP_EVENTS_UpdateEvents(&app_eventsData.newEvent))
                {
                    _APP_EVENTS_StoreEventsDataInMemory();
                }
            }
            
            if (_APP_EVENTS_CheckCustomEvents(&customEvent) == true)
            {
                app_eventsData.pCustomEventsCallback(customEvent);
                customEvent->enabled = false;
            }

            break;
        }

        /* The default state should never be executed. */
        case APP_EVENTS_STATE_ERROR:
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

void APP_EVENTS_ClearEvents(void)
{
    /* Erase all the event records stored in non volatile memory */
    APP_DATALOG_ClearData(APP_DATALOG_USER_EVENTS);

    /* Clear all events data */
    memset(&app_eventsData.events, 0, sizeof(APP_EVENTS_EVENTS));
}

bool APP_EVENTS_GetNumEvents(APP_EVENTS_EVENT_ID eventId, uint8_t * counter)
{
    if (eventId >= EVENTS_NUM_ID)
    {
        return false;
    }

    *counter = app_eventsData.events.event[eventId].counter;

    return true;
}

bool APP_EVENTS_GetEventInfo(APP_EVENTS_EVENT_ID eventId, uint8_t offset, APP_EVENTS_EVENT_INFO *eventInfo)
{
    APP_EVENTS_EVENT_DATA *pEvent;
    uint8_t index;

    if (eventId >= EVENTS_NUM_ID)
    {
        return false;
    }

    if (offset >= EVENT_LOG_MAX_NUMBER)
    {
        return false;
    }

    pEvent = &app_eventsData.events.event[eventId];

    // Get index from the last event offset
    index = pEvent->dataIndex - offset;
    if (offset > pEvent->dataIndex)
    {
        index += EVENT_LOG_MAX_NUMBER;
    }

    *eventInfo = pEvent->data[index];

    return true;
}

void APP_EVENTS_GetLastEventFlags(DRV_MCMETROLOGY_AFE_EVENTS *eventFlags)
{
    if (eventFlags)
    {
        *eventFlags = app_eventsData.flags;
    }
}

void APP_EVENTS_SetCustomEventsCallback(APP_EVENTS_CUSTOM_EVENT_CALLBACK callback)
{
    app_eventsData.pCustomEventsCallback = callback;
}

bool APP_EVENTS_RegisterCustomEventData(APP_EVENTS_CUSTOM_EVENT *event)
{
    APP_EVENTS_CUSTOM_EVENT *pCustomEvents;
    uint8_t index;
    
    pCustomEvents = app_eventsData.customEventsData;
    for (index = 0; index < APP_EVENTS_MAX_CUSTOM_EVENTS; index++)
    {
        if (pCustomEvents->enabled == false)
        {
            pCustomEvents->chnIndex = event->chnIndex;
            pCustomEvents->powIndex = event->powIndex;
            pCustomEvents->measure = event->measure;
            pCustomEvents->threshold = event->threshold;
            memcpy(pCustomEvents->par, event->par, sizeof(pCustomEvents->par));
            if (pCustomEvents->chnIndex < DRV_MCMETROLOGY_CHANNELS_NUMBER)
            {
                pCustomEvents->enabled = true;
            }
            else if (pCustomEvents->powIndex < DRV_MCMETROLOGY_POWERS_NUMBER)
            {
                pCustomEvents->enabled = true;
            }
            else if (pCustomEvents->measure < MEASURE_ENERGY)
            {
                pCustomEvents->enabled = true;
            }
            else
            {
                pCustomEvents->enabled = false;
            }
            
            return pCustomEvents->enabled;
        }
        
        pCustomEvents++;
    }
    
    return false;
}

bool APP_EVENTS_UnregisterCustomEventData(APP_EVENTS_CUSTOM_EVENT *event)
{
    APP_EVENTS_CUSTOM_EVENT *pCustomEvents;
    uint8_t index;
    
    pCustomEvents = app_eventsData.customEventsData;
    for (index = 0; index < APP_EVENTS_MAX_CUSTOM_EVENTS; index++)
    {
        if (memcmp(event->par, pCustomEvents->par, strlen(pCustomEvents->par)) == 0)
        {
            if ((event->chnIndex == pCustomEvents->chnIndex) && 
                (event->powIndex == pCustomEvents->powIndex))
            {
                pCustomEvents->enabled = false;
                memset(pCustomEvents->par, 0, sizeof(pCustomEvents->par));
                return true;
            }
        }
        
        pCustomEvents++;
    }
    
    return false;
}

uint8_t APP_EVENTS_GetCustomEventData(APP_EVENTS_CUSTOM_EVENT **event)
{
    APP_EVENTS_CUSTOM_EVENT *pCustomEvents;
    uint8_t index;
    uint8_t eventCounter = 0;
    
    pCustomEvents = app_eventsData.customEventsData;
    for (index = 0; index < APP_EVENTS_MAX_CUSTOM_EVENTS; index++)
    {
        if (pCustomEvents->enabled == true)
        {
            eventCounter++;
        }
        
        pCustomEvents++;
    }
    
    if (eventCounter > 0)
    {
        *event = &app_eventsData.customEventsData[0];
    }
    
    return eventCounter;
}

/*******************************************************************************
 End of File
 */
