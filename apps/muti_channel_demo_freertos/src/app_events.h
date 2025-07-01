/*
Copyright (C) 2023, Microchip Technology Inc., and its subsidiaries. All rights reserved.

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
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_events.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_EVENTS_Initialize" and "APP_EVENTS_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_EVENTS_STATES" definition).  Both
    are defined here for convenience.
 *******************************************************************************/

#ifndef _APP_EVENTS_H
#define _APP_EVENTS_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

#define EVENT_LOG_MAX_NUMBER                 10

typedef enum {
    NO_EVENT = 0,
    EVENT_START
} APP_EVENTS_EVENT_STATUS;

typedef enum {
    SWELL_TH_0_EVENT_ID,
    SWELL_TH_1_EVENT_ID,
    SWELL_TH_2_EVENT_ID,
    SWELL_TH_3_EVENT_ID,
    SWELL_TH_4_EVENT_ID,
    SWELL_TH_5_EVENT_ID,
    SWELL_TH_6_EVENT_ID,
    SWELL_TH_7_EVENT_ID,
    SWELL_TH_8_EVENT_ID,
    SWELL_TH_9_EVENT_ID,
    SWELL_TH_10_EVENT_ID,
    SWELL_TH_11_EVENT_ID,
    SWELL_TH_12_EVENT_ID,
    SWELL_TH_13_EVENT_ID,
    SWELL_TH_14_EVENT_ID,
    SWELL_TH_15_EVENT_ID,
    SAG_TH_0_EVENT_ID,
    SAG_TH_1_EVENT_ID,
    SAG_TH_2_EVENT_ID,
    SAG_TH_3_EVENT_ID,
    SAG_TH_4_EVENT_ID,
    SAG_TH_5_EVENT_ID,
    SAG_TH_6_EVENT_ID,
    SAG_TH_7_EVENT_ID,
    SAG_TH_8_EVENT_ID,
    SAG_TH_9_EVENT_ID,
    SAG_TH_10_EVENT_ID,
    SAG_TH_11_EVENT_ID,
    SAG_TH_12_EVENT_ID,
    SAG_TH_13_EVENT_ID,
    SAG_TH_14_EVENT_ID,
    SAG_TH_15_EVENT_ID,
    CREEP_TH_0_EVENT_ID,
    CREEP_TH_1_EVENT_ID,
    CREEP_TH_2_EVENT_ID,
    CREEP_TH_3_EVENT_ID,
    CREEP_TH_4_EVENT_ID,
    CREEP_TH_5_EVENT_ID,
    CREEP_TH_6_EVENT_ID,
    CREEP_TH_7_EVENT_ID,
    CREEP_TH_8_EVENT_ID,
    CREEP_TH_9_EVENT_ID,
    CREEP_TH_10_EVENT_ID,
    CREEP_TH_11_EVENT_ID,
    CREEP_TH_12_EVENT_ID,
    CREEP_TH_13_EVENT_ID,
    CREEP_TH_14_EVENT_ID,
    CREEP_TH_15_EVENT_ID,
    PHACTIVE_TH_0_EVENT_ID,
    PHACTIVE_TH_1_EVENT_ID,
    PHACTIVE_TH_2_EVENT_ID,
    PHACTIVE_TH_3_EVENT_ID,
    PHACTIVE_TH_4_EVENT_ID,
    PHACTIVE_TH_5_EVENT_ID,
    PHACTIVE_TH_6_EVENT_ID,
    PHACTIVE_TH_7_EVENT_ID,
    PHACTIVE_TH_8_EVENT_ID,
    PHACTIVE_TH_9_EVENT_ID,
    PHACTIVE_TH_10_EVENT_ID,
    PHACTIVE_TH_11_EVENT_ID,
    PHACTIVE_TH_12_EVENT_ID,
    PHACTIVE_TH_13_EVENT_ID,
    PHACTIVE_TH_14_EVENT_ID,
    PHACTIVE_TH_15_EVENT_ID,
    EVENTS_NUM_ID,
    EVENT_INVALID_ID = 0xFF,
} APP_EVENTS_EVENT_ID;

typedef struct {
    struct tm startTime;
    struct tm endTime;
} APP_EVENTS_EVENT_INFO;

typedef struct {
    APP_EVENTS_EVENT_STATUS status;
    APP_EVENTS_EVENT_INFO data[EVENT_LOG_MAX_NUMBER];
    uint16_t counter;
    uint8_t dataIndex;
} APP_EVENTS_EVENT_DATA;

typedef struct {
    APP_EVENTS_EVENT_DATA event[EVENTS_NUM_ID];
} APP_EVENTS_EVENTS;

typedef struct {
    // Metrology AFE Events
    DRV_MCMETROLOGY_AFE_EVENTS eventFlags;

    // Time stamp when events have been detected
    struct tm eventTime;

} APP_EVENTS_QUEUE_DATA;

#define APP_EVENTS_QUEUE_DATA_SIZE     10

typedef struct {
    float currentValue;
    float threshold;
    char par[5];
    uint8_t chnIndex;
    uint8_t powIndex;
    uint8_t measure;
    bool enabled;
} APP_EVENTS_CUSTOM_EVENT;

#define APP_EVENTS_MAX_CUSTOM_EVENTS   5

typedef void (* APP_EVENTS_CUSTOM_EVENT_CALLBACK) (APP_EVENTS_CUSTOM_EVENT *event);

// *****************************************************************************

/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
  */

typedef enum {
    APP_EVENTS_STATE_WAITING_DATALOG = 0,
    APP_EVENTS_STATE_INIT,
    APP_EVENTS_STATE_WAIT_DATA,
    APP_EVENTS_STATE_RUNNING,
    APP_EVENTS_STATE_ERROR

} APP_EVENTS_STATES;


// *****************************************************************************

/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
  */

typedef struct {
    APP_EVENTS_STATES state;

    APP_EVENTS_QUEUE_DATA newEvent;

    APP_EVENTS_EVENTS events;

    DRV_MCMETROLOGY_AFE_EVENTS flags;

    bool dataIsRdy;
    
    APP_EVENTS_CUSTOM_EVENT_CALLBACK pCustomEventsCallback;
    
    APP_EVENTS_CUSTOM_EVENT customEventsData[APP_EVENTS_MAX_CUSTOM_EVENTS];

} APP_EVENTS_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
  */

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_EVENTS_Initialize ( void )

  Summary:
      MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_EVENTS_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_EVENTS_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
  */

void APP_EVENTS_Initialize(void);


/*******************************************************************************
  Function:
    void APP_EVENTS_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_EVENTS_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
  */

void APP_EVENTS_Tasks(void);


void APP_EVENTS_ClearEvents(void);
bool APP_EVENTS_GetNumEvents(APP_EVENTS_EVENT_ID eventId, uint8_t * counter);
bool APP_EVENTS_GetEventInfo(APP_EVENTS_EVENT_ID eventId, uint8_t offset, APP_EVENTS_EVENT_INFO *eventInfo);
void APP_EVENTS_GetLastEventFlags(DRV_MCMETROLOGY_AFE_EVENTS *eventFlags);

bool APP_EVENTS_RegisterEventsData(APP_EVENTS_QUEUE_DATA *eventsData);
void APP_EVENTS_SetCustomEventsCallback(APP_EVENTS_CUSTOM_EVENT_CALLBACK callback);
bool APP_EVENTS_RegisterCustomEventData(APP_EVENTS_CUSTOM_EVENT *event);
bool APP_EVENTS_UnregisterCustomEventData(APP_EVENTS_CUSTOM_EVENT *event);
uint8_t APP_EVENTS_GetCustomEventData(APP_EVENTS_CUSTOM_EVENT **event);

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _APP_EVENTS_H */

/*******************************************************************************
 End of File
 */

