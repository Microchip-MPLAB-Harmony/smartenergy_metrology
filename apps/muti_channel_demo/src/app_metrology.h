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
    app_metrology.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_METROLOGY_Initialize" and "APP_METROLOGY_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_METROLOGY_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _APP_METROLOGY_H
#define _APP_METROLOGY_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "app_metrology_types.h"

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
    APP_METROLOGY_STATE_WAITING_DATALOG = 0,
    APP_METROLOGY_STATE_INIT,
    APP_METROLOGY_STATE_START,
    APP_METROLOGY_STATE_RUNNING,
    APP_METROLOGY_STATE_WAIT_DATA,
    APP_METROLOGY_STATE_ERROR

} APP_METROLOGY_STATE;

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
    APP_METROLOGY_STATE state;

    DRV_MCMETROLOGY_START_MODE startMode;
    DRV_MCMETROLOGY_REGS_CONTROL configuration;

    DRV_MCMETROLOGY_REGS_CONTROL * pMetControl;
    DRV_MCMETROLOGY_REGS_STATUS * pMetStatus;
    DRV_MCMETROLOGY_REGS_ACCUMULATORS * pMetAccData;
    DRV_MCMETROLOGY_REGS_HARMONICS * pMetHarData;
    
    DRV_MCMETROLOGY_CHANNEL * pMetChannelDesc;
    DRV_MCMETROLOGY_POWER_SOURCE * pMetPowerDesc;

    bool harmonicAnalysisPending;
    bool stopHarmonicAnalysis;
    bool sendHarmonicsToConsole;
    DRV_MCMETROLOGY_HARMONICS_RMS * pHarmonicAnalysisResponse;
    DRV_MCMETROLOGY_HARMONICS_CALLBACK pHarmonicAnalysisCallback;
    DRV_MCMETROLOGY_REGS_HARMONICS harmonicsData;

    uint32_t queueFree;

    bool setConfiguration;

    bool dataIsRdy;

    volatile bool integrationFlag;

    bool dataFlag;

} APP_METROLOGY_DATA;

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
    void APP_METROLOGY_Initialize (void)

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_METROLOGY_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_METROLOGY_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_METROLOGY_Initialize (void);


/*******************************************************************************
  Function:
    void APP_METROLOGY_Tasks (void)

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
    APP_METROLOGY_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_METROLOGY_Tasks(void);
APP_METROLOGY_STATE APP_METROLOGY_GetState(void);
bool APP_METROLOGY_GetControlRegister(CONTROL_REG_ID regId, uint32_t * regValue, char *regName);
bool APP_METROLOGY_SetControlRegister(CONTROL_REG_ID regId, uint32_t value);
bool APP_METROLOGY_GetStatusRegister(STATUS_REG_ID regId, uint32_t * regValue, char *regName);
bool APP_METROLOGY_GetAccumulatorRegister(ACCUMULATOR_REG_ID regId, uint64_t * regValue, char *regName);
void APP_METROLOGY_CaptureHarmonicData(void);
bool APP_METROLOGY_GetHarmonicRegister(HARMONICS_REG_ID regId, uint32_t *regValue, char *regName);
bool APP_METROLOGY_GetMeasure(DRV_MCMETROLOGY_MEASURE_TYPE measureId, float * value);
bool APP_METROLOGY_GetChannelMeasure(DRV_MCMETROLOGY_CHANNEL_MEASURE_TYPE measureId, 
                                     uint8_t channelIndex, float * value);
bool APP_METROLOGY_GetPowerMeasure(DRV_MCMETROLOGY_POWER_MEASURE_TYPE measureId, 
                                   uint8_t powerIndex, float * value);
void APP_METROLOGY_SetControlByDefault(void);
void APP_METROLOGY_StoreMetrologyData(void);
size_t APP_METROLOGY_GetWaveformCaptureData(uint32_t *pData);
bool APP_METROLOGY_StartHarmonicAnalysis(uint8_t harmonicNum, bool singleMode);
void APP_METROLOGY_StopHarmonicAnalysis(void);
void APP_METROLOGY_SetHarmonicAnalysisCallback(DRV_MCMETROLOGY_HARMONICS_CALLBACK callback,
        DRV_MCMETROLOGY_HARMONICS_RMS * pHarmonicAnalysisResponse);
void APP_METROLOGY_Restart(void);
void APP_METROLOGY_SetLowPowerMode (void);
bool APP_METROLOGY_CheckIsVoltageChannel(uint8_t channel);
void APP_METROLOGY_StopMetrology (void);
uint8_t APP_METROLOGY_GetChannelsDescription(DRV_MCMETROLOGY_CHANNEL **pChannelDesc);
uint8_t APP_METROLOGY_GetPowersDescription(DRV_MCMETROLOGY_POWER_SOURCE **pPowersDesc);


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _APP_METROLOGY_H */

/*******************************************************************************
 End of File
 */

