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
    app_energy.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_ENERGY_Initialize" and "APP_ENERGY_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_ENERGY_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _APP_ENERGY_H
#define _APP_ENERGY_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

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

typedef struct {
    float energy;
    float Pt;
} APP_ENERGY_QUEUE_DATA;

#define APP_ENERGY_QUEUE_DATA_SIZE     5

typedef struct {
    APP_ENERGY_QUEUE_DATA data[APP_ENERGY_QUEUE_DATA_SIZE];
    APP_ENERGY_QUEUE_DATA * dataRd;
    APP_ENERGY_QUEUE_DATA * dataWr;
    uint8_t dataSize;
} APP_ENERGY_QUEUE;

typedef enum {
    TARIFF_1 = 1,
    TARIFF_2,
    TARIFF_3,
    TARIFF_4,
    TARIFF_NUM_TYPE = TARIFF_4,
    TARIFF_INVALID = 0xFF
} APP_ENERGY_TARIFF_TYPE;

typedef struct {
    APP_ENERGY_TARIFF_TYPE tariff;
    uint8_t hour;
    uint8_t minute;
} APP_ENERGY_TOU_TIME_ZONE;

#define APP_ENERGY_TOU_MAX_ZONES             8

typedef struct {
    APP_ENERGY_TOU_TIME_ZONE timeZone[APP_ENERGY_TOU_MAX_ZONES];
    uint32_t usedZones;
} APP_ENERGY_TOU;

typedef struct {
    float value;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
} APP_ENERGY_DEMAND_DATA;

typedef struct {
    APP_ENERGY_DEMAND_DATA maxDemad;
    APP_ENERGY_DEMAND_DATA tariff[TARIFF_NUM_TYPE];
} APP_ENERGY_MAX_DEMAND;

typedef struct {
    APP_ENERGY_MAX_DEMAND maxDemand;
    float window[60];
} APP_ENERGY_DEMAND;

typedef struct {
    double tariff[TARIFF_NUM_TYPE];
} APP_ENERGY_ACCUMULATORS;

typedef void (* APP_ENERGY_MAXDEMAND_CALLBACK) (struct tm * time, bool dataValid);
typedef void (* APP_ENERGY_MONTH_CALLBACK) (struct tm * time, bool dataValid);
// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
    /* Application's state machine's initial state. */
    APP_ENERGY_STATE_WAITING_DATALOG = 0,
    APP_ENERGY_STATE_INIT_RTC,
    APP_ENERGY_STATE_SET_RTC,
    APP_ENERGY_STATE_INIT_TOU,
    APP_ENERGY_STATE_SET_TOU,
    APP_ENERGY_STATE_INIT_DEMAND,
    APP_ENERGY_STATE_INIT_ENERGY,
    APP_ENERGY_STATE_INIT_EVENTS,
    APP_ENERGY_STATE_RUNNING,
    APP_ENERGY_STATE_GET_MAX_DEMAND,
    APP_ENERGY_STATE_RESPONSE_MAX_DEMAND,
    APP_ENERGY_STATE_GET_MONTH_ENERGY,
    APP_ENERGY_STATE_RESPONSE_MONTH_ENERGY,
    APP_ENERGY_STATE_WAIT_DATA,
    APP_ENERGY_STATE_ERROR

} APP_ENERGY_STATES;


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
    /* The application's current state */
    APP_ENERGY_STATES state;

    APP_ENERGY_QUEUE_DATA newQueuedData;

    APP_ENERGY_TARIFF_TYPE currentTariffIndex;

    float energyThreshold;

    APP_ENERGY_TOU tou;

    APP_ENERGY_DEMAND demand;

    APP_ENERGY_ACCUMULATORS energyAccumulator;

    float demandAccumulator;

    uint32_t counterIntegrationPeriods;

    bool eventMinute;

    bool eventMonth;

    bool dataIsRdy;

    APP_ENERGY_MAXDEMAND_CALLBACK maxDemandCallback;

    APP_ENERGY_MAX_DEMAND * pMaxDemandResponse;

    APP_ENERGY_MONTH_CALLBACK monthEnergyCallback;

    APP_ENERGY_ACCUMULATORS * pMonthEnergyResponse;

    struct tm timeResponse;

    struct tm time;

    uint8_t minRtcBackup;

    bool generateEnergyReponse;

    bool generateDemandReponse;

} APP_ENERGY_DATA;

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
    void APP_ENERGY_Initialize (void)

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_ENERGY_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_ENERGY_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_ENERGY_Initialize (void);


/*******************************************************************************
  Function:
    void APP_ENERGY_Tasks (void)

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
    APP_ENERGY_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_ENERGY_Tasks(void);


APP_ENERGY_TOU_TIME_ZONE * APP_ENERGY_GetTOUTimeZone(void);
void APP_ENERGY_SetTOUTimeZone(APP_ENERGY_TOU_TIME_ZONE *timeZone);
void APP_ENERGY_SetMonthEnergyCallback(APP_ENERGY_MONTH_CALLBACK callback,
        APP_ENERGY_ACCUMULATORS * pMonthEnergyResponse);
bool APP_ENERGY_GetMonthEnergy(struct tm * time);
void APP_ENERGY_GetCurrentEnergy(APP_ENERGY_ACCUMULATORS * pEnergy);
void APP_ENERGY_ClearEnergy(bool clearPersistentData);
void APP_ENERGY_SetMaxDemandCallback(APP_ENERGY_MAXDEMAND_CALLBACK callback,
        APP_ENERGY_MAX_DEMAND * pMaxDemandResponse);
bool APP_ENERGY_GetMonthMaxDemand(struct tm * time);
void APP_ENERGY_GetCurrentMaxDemand(APP_ENERGY_MAX_DEMAND * pMaxDemand);
void APP_ENERGY_ClearMaxDemand(bool clearPersistentData);

bool APP_ENERGY_SendEnergyData(APP_ENERGY_QUEUE_DATA *energyData);

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _APP_ENERGY_H */

/*******************************************************************************
 End of File
 */

