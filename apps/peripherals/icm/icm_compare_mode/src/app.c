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

APP_DATA appData;

/* Hash area */
uint32_t outputSHA[0x20] __ALIGNED(128);

/* Memory region0 area, string "abc" */
volatile uint32_t appMessageSHA0[16] = {
	0x80636261,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x18000000
};

/* Memory region1 main list area, 
 * string "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq" */
volatile uint32_t appMessageSHA1[16] = {
	0x64636261,
	0x65646362,
	0x66656463,
	0x67666564,
	0x68676665,
	0x69686766,
	0x6a696867,
	0x6b6a6968,
	0x6c6b6a69,
	0x6d6c6b6a,
	0x6e6d6c6b,
	0x6f6e6d6c,
	0x706f6e6d,
	0x71706f6e,
	0x00000080,
	0x00000000
};

/* Memory region1 secondary list area */
volatile uint32_t appMessageSHA1Sec[16] = {
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0xc0010000
};

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************
static void _appDigestMismatchCallback(ICM_REGION_ID regionId)
{
	if (regionId == ICM_REGION_0) {
        printf("...ICM_REGION_0 is modified\r\n");
        appData.data0Changed = true;
	} else if (regionId == ICM_REGION_1) {
        printf("...ICM_REGION_1 is modified\r\n");
        appData.data1Changed = true;
	}
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
    appData.data0Changed = false;
    appData.data1Changed = false;
    
    (void *)memset((uint8_t *)outputSHA, 0, sizeof(outputSHA));
    
    /* Output example information */
    printf("-- ICM Compare Mode Example --\r\n");
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
            ICM_SECONDARY_LIST icmSecList;
    
            ICM_SetRegionDescriptorData(ICM_REGION_0, (uint32_t *)appMessageSHA0, sizeof(appMessageSHA0));
            ICM_SetRegionDescriptorData(ICM_REGION_1, (uint32_t *)appMessageSHA1, sizeof(appMessageSHA1));
            
            ICM_SetHashAreaAddress((uint32_t)outputSHA);
            
            // Configure Secondary List of the region 1
            appData.pRegionDescriptor = ICM_GetRegionDescriptor(ICM_REGION_1);
            appData.pRegionDescriptor->secondaryList = &icmSecList;
            icmSecList.startAddress = (uint32_t)appMessageSHA1Sec;
            icmSecList.transferSize = ICM_GetTransferSize(sizeof(appMessageSHA1Sec));
            icmSecList.nextAddress = 0;
            
            // Compute de Hash values
            ICM_Enable();
            
            appData.state = APP_STATE_WAIT_DIGEST;
            
            break;
        }
        
        case APP_STATE_WAIT_DIGEST:
        {
            if (outputSHA[0] > 0)
            {
                appData.state = APP_STATE_MODIFY_REGION0_DATA;
            }
            break;
        }
            
        
        case APP_STATE_MODIFY_REGION0_DATA:
        {
            // Set Compare mode
            appData.pRegionDescriptor = ICM_GetRegionDescriptor(ICM_REGION_0);
            appData.pRegionDescriptor->config.bitfield.compareMode = 1;
            appData.pRegionDescriptor = ICM_GetRegionDescriptor(ICM_REGION_1);
            appData.pRegionDescriptor->config.bitfield.compareMode = 1;

            // Set ICM callbacks
            ICM_CallbackRegister(ICM_INTERRUPT_RDM, _appDigestMismatchCallback);
            ICM_EnableInterrupt(ICM_INTERRUPT_RDM, ICM_REGION_0);
            ICM_EnableInterrupt(ICM_INTERRUPT_RDM, ICM_REGION_1);

            printf("Change Data in Region 0\r\n");
            appMessageSHA0[2] = 1;

            ICM_Enable();
            appData.state = APP_STATE_WAIT_REGION0_MISMATCH; 
            break;
        }
        
        case APP_STATE_WAIT_REGION0_MISMATCH:
        {
            if (appData.data0Changed == true)
            {
                appData.data0Changed = false;
                
                printf("...appMessageSHA0 content has been modified\r\n\r\n");
                
                // restore data content
                appMessageSHA0[2] = 0;
                appData.state = APP_STATE_MODIFY_REGION1_DATA;     
            }
            break;
        }
        
        case APP_STATE_MODIFY_REGION1_DATA:
        {
            printf("Change Data in Region 1\r\n");
            appMessageSHA1[5] = 0xAA5555AA;

            ICM_Enable();
            appData.state = APP_STATE_WAIT_REGION1_MISMATCH; 
            break;
        }
            
        case APP_STATE_WAIT_REGION1_MISMATCH:
        {
            if (appData.data1Changed == true)
            {
                appData.data1Changed = false;
                
                printf("...appMessageSHA1 content has been modified\r\n\r\n");
                
                // restore data content
                appMessageSHA1[5] = 0x69686766;
                appData.state = APP_STATE_MODIFY_REGION1SEC_DATA;     
            }
            break;
        }
        
        case APP_STATE_MODIFY_REGION1SEC_DATA:
        {
            printf("Change Data in Region 1 - secondary list\r\n");
            appMessageSHA1Sec[5] = 0xAA5555AA;

            ICM_Enable();
            appData.state = APP_STATE_WAIT_REGION1SEC_MISMATCH; 
            break;
        }
            
        case APP_STATE_WAIT_REGION1SEC_MISMATCH:
        {
            if (appData.data1Changed == true)
            {
                appData.data1Changed = false;
                
                printf("...appMessageSHA1Sec content has been modified\r\n\r\n");
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
