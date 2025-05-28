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
#include <string.h>
#include "peripheral/icm/plib_icm.h"
#include "bsp/bsp.h"
#include "app.h"

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

#define APP_TIME_TO_CHANGE_DATA   2000  // ms.

APP_DATA appData;

/* Hash area */
uint32_t outputSHA[0x10] __ALIGNED(128);

/* Memory region0 area */
volatile uint32_t appMonitoredData[16] = {
	0x12345678,
	0x12345678,
	0x12345678,
	0x12345678,
	0x12345678,
	0x12345678,
	0x12345678,
	0x12345678,
	0x12345678,
	0x12345678,
	0x12345678,
	0x12345678,
	0x12345678,
	0x12345678,
	0x12345678,
	0x12345678
};

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************
static void lAppDigestMismatchCallback(ICM_REGION_ID regionId)
{
	if (regionId == ICM_REGION_0) {
        appData.dataChanged = true;
	} 
}

static void lAppHashCompletedCallback(ICM_REGION_ID regionId)
{
	if (regionId == ICM_REGION_0) {
        appData.hashCompleted = true;
	} 
}

void lAppTimerCallback ( uintptr_t context )
{
    appData.timerExpired = true;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

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
    appData.state = APP_STATE_CONFIGURE_ICM;
    appData.dataChanged = false;
    appData.timerExpired = false;
    
    (void *)memset((uint8_t *)outputSHA, 0, sizeof(outputSHA));
    
    /* Output example information */
    printf("-- ICM Monitor Mode Example --\r\n");
    printf("-- %s\r\n", BOARD_NAME);
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
        case APP_STATE_CONFIGURE_ICM:
        {
            printf("Getting the monitored data HASH value\r\n");
            
            ICM_SetRegionDescriptorData(ICM_REGION_0, (uint32_t *)appMonitoredData, sizeof(appMonitoredData));
            
            ICM_SetHashAreaAddress((uint32_t)outputSHA);
                
            // Set ICM callbacks (Hash Completed))
            ICM_CallbackRegister(ICM_INTERRUPT_RHC, lAppHashCompletedCallback);
            ICM_EnableInterrupt(ICM_INTERRUPT_RHC, ICM_REGION_0);
            
            // Compute de Hash values
            ICM_Enable();
            
            appData.state = APP_STATE_WAIT_DIGEST;
            
            break;
        }
        
        case APP_STATE_WAIT_DIGEST:
        {
            if (appData.hashCompleted == true)
            {
                // Disable ICM callbacks (Hash Completed))
                ICM_CallbackRegister(ICM_INTERRUPT_RHC, NULL);
                ICM_DisableInterrupt(ICM_INTERRUPT_RHC, ICM_REGION_0);
                
                printf("Configure ICM Monitor Mode\r\n");
                
                // Set ICM Monitor Mode
                appData.pRegionDescriptor = ICM_GetDefaultRegionDescriptor();
                appData.pRegionDescriptor[0].config.bitfield.compareMode = 1;

                // Set ICM callbacks (Digest mismatch)
                ICM_CallbackRegister(ICM_INTERRUPT_RDM, lAppDigestMismatchCallback);
                ICM_EnableInterrupt(ICM_INTERRUPT_RDM, ICM_REGION_0);
                
                ICM_EnableRegionMonitor(ICM_REGION_0);
                
                // Launch Periodic Timer to Modify data
                appData.timer = SYS_TIME_CallbackRegisterMS(lAppTimerCallback, 0, 
                                                            APP_TIME_TO_CHANGE_DATA, 
                                                            SYS_TIME_SINGLE);
                
                printf("Waiting %u seconds...\r\n", APP_TIME_TO_CHANGE_DATA/1000);
                
                appData.state = APP_STATE_MODIFY_REGION0_DATA;
            }
            break;
        }
        
        case APP_STATE_MODIFY_REGION0_DATA:
        {
            if (appData.timerExpired == true)
            {
                appData.timerExpired = false;

                printf("Changing Monitored Data...\r\n");
                appMonitoredData[2] = 1;

                appData.state = APP_STATE_WAIT_REGION0_MISMATCH; 
            }
            break;
        }
        
        case APP_STATE_WAIT_REGION0_MISMATCH:
        {
            if (appData.dataChanged == true)
            {
                appData.dataChanged = false;
                
                // restore data content
                appMonitoredData[2] = 0x12345678;
                
                printf("...appMonitoredData content has been modified\r\n\r\n");
                
                appData.state = APP_STATE_IDLE;     
            }
            break;
        }
        
        case APP_STATE_IDLE:
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
