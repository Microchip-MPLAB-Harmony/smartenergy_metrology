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
    app_console.c

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
#include "app_console.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

/* Structure containing data to be stored in non volatile memory */
typedef struct
{
    /* Meter ID */
    char meterID[7];

} APP_CONSOLE_STORAGE_DATA;

#define CONSOLE_TASK_DELAY_MS_UNTIL_DATALOG_READY      100
#define CONSOLE_TASK_DEFAULT_DELAY_MS_BETWEEN_STATES   10
#define CONSOLE_TASK_DELAY_MS_BETWEEN_REGS_PRINT       30


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_CONSOLE_Initialize function.

    Application strings and buffers are be defined outside this structure.
 */

APP_CONSOLE_DATA CACHE_ALIGN app_consoleData;

/* Local variable to hold a duplicate of storage data */
APP_CONSOLE_STORAGE_DATA CACHE_ALIGN app_consoleStorageData;
/* Constant containing default value for storage data */
const APP_CONSOLE_STORAGE_DATA CACHE_ALIGN app_defaultConsoleStorageData =
{
    {'1', '2', '3', '4', '\0', '\0', '\0'}
};

/* Local storage objects */
static APP_ENERGY_ACCUMULATORS energyData;
static APP_ENERGY_MAX_DEMAND maxDemandLocalObject;
static DRV_MCMETROLOGY_HARMONICS_RMS harmonicAnalysisRMSData;

/* Local Queue element to request Datalog operations */
APP_DATALOG_QUEUE_DATA datalogQueueElement;

/* Reference to datalog queue */
extern APP_DATALOG_QUEUE appDatalogQueue;

/* Local array to hold password for Commands */
#define APP_CONSOLE_MET_PWD_SIZE             6
static char metPwd[APP_CONSOLE_MET_PWD_SIZE] = APP_CONSOLE_DEFAULT_PWD;

/* Local array to print the sensor type */
static const char *gConsoleSensorTypes[SENSOR_NUM_TYPE] = {"CT", "SHUNT", "ROGOWSKI", "VRD"};

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************
static void _commandBUF(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandDAR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandDEV(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandDCB(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandDCD(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandDCM(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandDCR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandDCS(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandDCW(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandDSR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandENC(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandENR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandEVEC(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandEVER(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandHAR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandHRR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandHRRX(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandIDR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandIDW(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandMDC(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandMDR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandPAR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandRTCR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandRTCW(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandTOUR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandTOUW(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandRST(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandRLD(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static void _commandHELP(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);

static const SYS_CMD_DESCRIPTOR appCmdTbl[] =
{
    {"BUF", _commandBUF, ": Read waveform capture data (if a parameter is used, only a 512 samples sector is returned)"},
    {"DAR", _commandDAR, ": Read DSP_ACC registers"},
    {"DEV", _commandDEV, ": Read topology of device connections"},
    {"DCB", _commandDCB, ": Go to low power mode"},
    {"DCD", _commandDCD, ": Load default metrology control register values"},
    {"DCM", _commandDCM, ": Write DSP_CONTROL several registers"},
    {"DCR", _commandDCR, ": Read DSP_CONTROL registers"},
    {"DCS", _commandDCS, ": Save metrology constants to non volatile memory"},
    {"DCW", _commandDCW, ": Write DSP_CONTROL register"},
    {"DSR", _commandDSR, ": Read DSP_ST registers"},
    {"ENC", _commandENC, ": Clear all energy"},
    {"ENR", _commandENR, ": Read energy"},
    {"EVEC", _commandEVEC, ": Clear all event record"},
    {"EVER", _commandEVER, ": Read single event record"},
    {"HAR", _commandHAR, ": Read harmonic register"},
    {"HRR", _commandHRR, ": Read harmonic Irms/Vrms"},
    {"HRRX", _commandHRRX, ": Extended version of HRR, using Start/Stop functionality"},
    {"IDR", _commandIDR, ": Read meter id"},
    {"IDW", _commandIDW, ": Write meter id (id length limited to 6 characters)"},
    {"MDC", _commandMDC, ": Clear all maxim demand and happen time"},
    {"MDR", _commandMDR, ": Read maxim demand"},
    {"PAR", _commandPAR, ": Read measure parameter"},
    {"RTCR", _commandRTCR, ": Read meter RTC"},
    {"RTCW", _commandRTCW, ": Write meter RTC"},
    {"TOUR", _commandTOUR, ": Read meter TOU"},
    {"TOUW", _commandTOUW, ": Write meter TOU"},
    {"RST", _commandRST, ": System reset"},
    {"RLD", _commandRLD, ": Reload Metrology Coprocessor"},
    {"HELP", _commandHELP, ": Help on commands"}
};

/*******************************************************************************
  Function:
    Timer in milliseconds for creating and waiting the delay.
 */

static bool APP_CONSOLE_TaskDelay(uint32_t ms, SYS_TIME_HANDLE* handle)
{
    // Check if there is the timer has been created and running
    if (*handle == SYS_TIME_HANDLE_INVALID)
    {
        // Create timer
        if (SYS_TIME_DelayMS(ms, handle) != SYS_TIME_SUCCESS)
        {
            return false;
        }
    }
    else
    {
        // Check timer
        if (SYS_TIME_DelayIsComplete(*handle) == true)
        {
            *handle = SYS_TIME_HANDLE_INVALID;
            return true;
        }
    }

    return false;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

void _getSysTimeFromMonthIndex(struct tm *time, uint8_t index)
{
    struct tm sysTime;

    RTC_TimeGet(&sysTime);

    if (sysTime.tm_mon < index)
    {
        sysTime.tm_mon += 12;
        sysTime.tm_year--;
    }
    sysTime.tm_mon -= index;

    *time = sysTime;
}

uint8_t _getMonthIndexFromSysTime(struct tm *time)
{
    struct tm sysTime;
    int8_t index;

    RTC_TimeGet(&sysTime);

    index = sysTime.tm_mon - time->tm_mon;
    if (index < 0)
    {
        index += 12;
    }

    return (uint8_t) index;
}

bool _preprocessorCallback(const SYS_CMD_DESCRIPTOR* pCmdTbl,
                           SYS_CMD_DEVICE_NODE* pCmdIO, char* cmdBuff,
                           size_t bufSize, void* hParam)
{
    char *pBuff = cmdBuff;
    char cmdChar;
    size_t idx;

    for (idx = 0; idx < bufSize; idx++, pBuff++)
    {
        cmdChar = *pBuff;
        if ((cmdChar >= 'a') && (cmdChar <= 'z'))
        {
            // Convert upper case
            *pBuff -= 32;
        }
        else if ((cmdChar == '\t') || (cmdChar == ',') || (cmdChar == ';') ||
                (cmdChar == '[') || (cmdChar == ']') ||
                (cmdChar == '(') || (cmdChar == ')'))
        {
            // Replace command separator
            *pBuff = ' ';
        }
    }

    return false;
}

static void _consoleReadStorage(APP_DATALOG_RESULT result)
{
    // Check result and go to corresponding state
    if (result == APP_DATALOG_RESULT_SUCCESS)
    {
        app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
    }
    else
    {
        app_consoleData.state = APP_CONSOLE_STATE_READ_STORAGE_ERROR;
    }
}

static void _monthlyEnergyCallback(struct tm * time, bool dataValid)
{
    if (!dataValid)
    {
        memset(&energyData, 0, sizeof (energyData));
    }

    app_consoleData.timeRequest = *time;
    app_consoleData.state = APP_CONSOLE_STATE_PRINT_MONTHLY_ENERGY;
}

static void _maxDemandCallback(struct tm * time, bool dataValid)
{
    if (!dataValid)
    {
        memset(&maxDemandLocalObject, 0, sizeof (maxDemandLocalObject));
    }

    app_consoleData.timeRequest = *time;
    app_consoleData.state = APP_CONSOLE_STATE_PRINT_MAX_DEMAND;
}

static void _harmonicAnalysisCallback(uint8_t harmonicNum)
{
    app_consoleData.harmonicNum = harmonicNum;
    app_consoleData.channelNum = DRV_MCMETROLOGY_CHANNELS_NUMBER;
    app_consoleData.state = APP_CONSOLE_STATE_PRINT_ALL_HARMONIC_ANALYSIS;
}

// *****************************************************************************
// COMMANDS
// *****************************************************************************

static inline void _removePrompt(void)
{
    SYS_CMD_MESSAGE("\b");
}

static void _commandHELP(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 2)
    {
        SYS_CMD_DESCRIPTOR *pCmd;
        uint8_t idx;
        size_t len;

        // Show help for a single command
        pCmd = (SYS_CMD_DESCRIPTOR *) appCmdTbl;
        for (idx = 0; idx < app_consoleData.numCommands; idx++, pCmd++)
        {
            len = strlen(argv[1]);

            if (strncmp(pCmd->cmdStr, argv[1], len) == 0)
            {
                SYS_CMD_PRINT("%s\t%s\r\n", pCmd->cmdStr, pCmd->cmdDescr);
                break;
            }
        }

        if (idx == app_consoleData.numCommands)
        {
            SYS_CMD_MESSAGE("Command is not found.\r\n");
        }
    }
    else
    {
        app_consoleData.state = APP_CONSOLE_STATE_PRINT_HELP;
        app_consoleData.cmdNumToShowHelp = app_consoleData.numCommands;
        app_consoleData.pCmdDescToShowHelp = (SYS_CMD_DESCRIPTOR *) appCmdTbl;

        // Remove Prompt symbol
        _removePrompt();
    }

    /* Show console communication icon */
    APP_DISPLAY_SetSerialCommunication();
}

static void _commandBUF(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint32_t captureAddress;
    size_t captureSize;
    uint8_t idxMax;
    uint8_t idx = 0xFF;

    if (argc > 2)
    {
        SYS_CMD_MESSAGE("Incorrect param number\r\n");
        return;
    }

    if (argc == 2)
    {
        // Extract index from parameters
        idx = (uint8_t) strtol(argv[1], NULL, 10);
    }

    // Store parameters in local variables and go to corresponding state
    captureSize = APP_METROLOGY_GetWaveformCaptureData(&captureAddress);

    if ((captureSize == 0) || (captureAddress == 0))
    {
        SYS_CMD_MESSAGE("Waveform data is disabled.\r\n");
        return;
    }

    app_consoleData.rawDataLen = captureSize;
    app_consoleData.rawData = (uint32_t *) captureAddress;

    idxMax = (captureSize - 1) >> 9;

    if (idx != 0xFF)
    {
        if (idx > idxMax)
        {
            SYS_CMD_MESSAGE("Parameter is out of range.\r\n");
            return;
        }

        if (captureSize > 512)
        {
            /* Check if it is the last fragment */
            if (idx == idxMax)
            {
                app_consoleData.rawDataLen = captureSize - (idx << 9);
            }
            else
            {
                app_consoleData.rawDataLen = 512;
            }
        }

        app_consoleData.rawData += (idx << 9);
    }

    app_consoleData.rawDataFlag = true;
    app_consoleData.state = APP_CONSOLE_STATE_PRINT_WAVEFORM_DATA;

    /* Show console communication icon */
    APP_DISPLAY_SetSerialCommunication();

}

static void _commandDAR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint16_t idx;

    if (argc == 1)
    {
        // Read all metrology accumulator registers
        app_consoleData.accumRegToRead = 0;
        app_consoleData.state = APP_CONSOLE_STATE_READ_ALL_ACCUM_REGS;

        /* Show console communication icon */
        APP_DISPLAY_SetSerialCommunication();
    }
    else if (argc == 2)
    {
        // Extract register index from parameters
        idx = (uint16_t) strtol(argv[1], NULL, 10);
        if (idx < ACCUMULATOR_REG_NUM)
        {
            // Read register value
            app_consoleData.accumRegToRead = idx;
            app_consoleData.state = APP_CONSOLE_STATE_READ_ACCUM_REG;

            /* Show console communication icon */
            APP_DISPLAY_SetSerialCommunication();
        }
        else
        {
            // Invalid index
            SYS_CMD_MESSAGE("Invalid register index\r\n");
        }
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\r\n");
    }
}

static void _commandDEV(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 1)
    {
        // Read topology of device connections
        app_consoleData.state = APP_CONSOLE_STATE_READ_DEVICES;

        /* Show console communication icon */
        APP_DISPLAY_SetSerialCommunication();
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\r\n");
    }
}

static void _commandDCB(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 1)
    {
        app_consoleData.state = APP_CONSOLE_STATE_LOW_POWER_MODE;

        /* Show console communication icon */
        APP_DISPLAY_SetSerialCommunication();
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\r\n");
    }
}

static void _commandDCD(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 1)
    {
        SYS_CMD_MESSAGE("Load Default Is Ok !\r\n");
        // Set default control register values
        APP_METROLOGY_SetControlByDefault();

        /* Show console communication icon */
        APP_DISPLAY_SetSerialCommunication();
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\r\n");
    }
}

static void _commandDCM(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t i;
    uint8_t numRegsToWrite;
    char *p;
    bool parseError = false;

    if (argc > 1)
    {
        // Each argument includes reg index and value
        numRegsToWrite = (argc - 1);
        for (i = 0; i < numRegsToWrite; i++)
        {
            // Extract register index and value from argument
            p = argv[i + 1];
            app_consoleData.regsToModify[i].index = (uint16_t) strtol(p, NULL, 10);
            // Look for ":" char and advance to next char
            p = strstr(p, ":");
            if (p != NULL)
            {
                p++;
                app_consoleData.regsToModify[i].value = (uint32_t) strtoul(p, NULL, 16);
            }
            else
            {
                SYS_CMD_MESSAGE("Unsupported Command !\r\n");
                parseError = true;
                break;
            }
        }

        if (!parseError)
        {
            // Write invalid values after last, to later detect it
            app_consoleData.regsToModify[i].index = 0xFFFF;
            app_consoleData.regsToModify[i].value = 0xFFFFFFFF;
            // Go to corresponding state
            app_consoleData.state = APP_CONSOLE_STATE_WRITE_CONTROL_REG;

            /* Show console communication icon */
            APP_DISPLAY_SetSerialCommunication();
        }
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\r\n");
    }
}

static void _commandDCR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint16_t idx;

    if (argc == 1)
    {
        // Read all metrology control registers
        app_consoleData.ctrlRegToRead = 0;
        app_consoleData.state = APP_CONSOLE_STATE_READ_ALL_CONTROL_REGS;
    }
    else if (argc == 2)
    {
        // Extract register index from parameters
        idx = (uint16_t) strtol(argv[1], NULL, 10);
        if (idx < CONTROL_REG_NUM)
        {
            // Read register value
            app_consoleData.ctrlRegToRead = idx;
            app_consoleData.state = APP_CONSOLE_STATE_READ_CONTROL_REG;

            /* Show console communication icon */
            APP_DISPLAY_SetSerialCommunication();
        }
        else
        {
            // Invalid index
            SYS_CMD_MESSAGE("Invalid register index\r\n");
        }
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\r\n");
    }
}

static void _commandDCS(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 1)
    {
        SYS_CMD_MESSAGE("Save Data Is Ok !\r\n");
        // Save Metrology Constants and configuration settings to NVM
        APP_METROLOGY_StoreMetrologyData();

        /* Show console communication icon */
        APP_DISPLAY_SetSerialCommunication();
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\r\n");
    }
}

static void _commandDCW(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint16_t idx;
    uint32_t regValue;

    if (argc == 3)
    {
        // Extract register index from parameters
        idx = (uint16_t) strtol(argv[1], NULL, 10);
        if (idx < CONTROL_REG_NUM)
        {
            // Extract register value
            regValue = (uint32_t) strtoul(argv[2], NULL, 16);
            // Write register value
            if (APP_METROLOGY_SetControlRegister((CONTROL_REG_ID) idx, regValue))
            {
                // Show response on console
                SYS_CMD_MESSAGE("Set Is Ok !\r\n");

                /* Show console communication icon */
                APP_DISPLAY_SetSerialCommunication();
            }
            else
            {
                // Cannot write register
                SYS_CMD_PRINT("Could not write register %02d\r\n", idx);
            }
        }
        else
        {
            // Invalid index
            SYS_CMD_MESSAGE("Invalid register index\r\n");
        }
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\r\n");
    }
}

static void _commandDSR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint16_t idx;

    if (argc == 1)
    {
        // Read all metrology status registers
        app_consoleData.statusRegToRead = 0;
        app_consoleData.state = APP_CONSOLE_STATE_READ_ALL_STATUS_REGS;

        /* Show console communication icon */
        APP_DISPLAY_SetSerialCommunication();
    }
    else if (argc == 2)
    {
        // Extract register index from parameters
        idx = (uint16_t) strtol(argv[1], NULL, 10);
        if (idx < STATUS_REG_NUM)
        {
            // Read register value
            app_consoleData.statusRegToRead = idx;
            app_consoleData.state = APP_CONSOLE_STATE_READ_STATUS_REG;

            /* Show console communication icon */
            APP_DISPLAY_SetSerialCommunication();
        }
        else
        {
            // Invalid index
            SYS_CMD_MESSAGE("Invalid register index\r\n");
        }
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\r\n");
    }
}

static void _commandENC(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 2)
    {
        // Check password from parameters
        if (strncmp(argv[1], metPwd, APP_CONSOLE_MET_PWD_SIZE) == 0)
        {
            // Correct password, Clear Energy records
            APP_ENERGY_ClearEnergy(true);
            // Show response on console
            SYS_CMD_MESSAGE("Clear Energy is ok !\r\n");

            /* Show console communication icon */
            APP_DISPLAY_SetSerialCommunication();
        }
        else
        {
            // Invalid password
            SYS_CMD_MESSAGE("Invalid password\r\n");
        }
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\r\n");
    }
}

static void _commandENR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t monthIndex;

    if (argc > 2)
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\r\n");
    }

    if (argc == 2)
    {
        // Extract month index from parameters
        monthIndex = (uint8_t) strtol(argv[1], NULL, 10);
        monthIndex %= 12;
        app_consoleData.requestCounter = 1;
    }
    else
    {
        monthIndex = 0;
        // Start process to get full Monthly energy data
        app_consoleData.requestCounter = 12;
    }

    // Get SysTime
    _getSysTimeFromMonthIndex(&app_consoleData.sysTime, monthIndex);

    // Get monthly energy from energy app
    if (APP_ENERGY_GetMonthEnergy(&app_consoleData.sysTime) == false)
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param\r\n");
    }
    else
    {
        /* Show console communication icon */
        APP_DISPLAY_SetSerialCommunication();
    }
    // Response will be provided on _monthlyEnergyCallback function
}

static void _commandEVEC(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 2)
    {
        // Check password from parameters
        if (strncmp(argv[1], metPwd, APP_CONSOLE_MET_PWD_SIZE) == 0)
        {
            APP_EVENTS_ClearEvents();
            // Show response on console
            SYS_CMD_MESSAGE("Clear All Event is ok !\r\n");

            /* Show console communication icon */
            APP_DISPLAY_SetSerialCommunication();
        }
        else
        {
            // Invalid password
            SYS_CMD_MESSAGE("Invalid password\r\n");
        }
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\r\n");
    }
}

static void _commandEVER(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t channel;

    app_consoleData.eventIdRequest = EVENT_INVALID_ID;

    if (argc == 4)
    {
        // Extract channel
        channel = (uint8_t) strtol(argv[2], NULL, 10);
        if (channel < DRV_MCMETROLOGY_CHANNELS_NUMBER)
        {
            // Extract event id from parameters
            if (strcmp(argv[1], "SWELL") == 0)
            {
                app_consoleData.eventIdRequest = SWELL_TH_0_EVENT_ID + channel;
            }
            else if (strcmp(argv[1], "SAG") == 0)
            {
                app_consoleData.eventIdRequest = SAG_TH_0_EVENT_ID + channel;
            }
            else if (strcmp(argv[1], "CREEP") == 0)
            {
                app_consoleData.eventIdRequest = CREEP_TH_0_EVENT_ID + channel;
            }
            else if (strcmp(argv[1], "PHACTIVE") == 0)
            {
                app_consoleData.eventIdRequest = PHACTIVE_TH_0_EVENT_ID + channel;
            }
        }

        if (app_consoleData.eventIdRequest < EVENTS_NUM_ID)
        {
            // Extract last times from parameters
            app_consoleData.eventLastTimeRequest = (uint8_t) strtol(argv[3], NULL, 10);

            if (app_consoleData.eventLastTimeRequest >= EVENT_LOG_MAX_NUMBER)
            {
                // Invalid time index
                SYS_CMD_MESSAGE("Last time exceed the Event Log Max Number!\r\n");
            }
            else
            {
                app_consoleData.state = APP_CONSOLE_STATE_PRINT_EVENT;

                /* Show console communication icon */
                APP_DISPLAY_SetSerialCommunication();
            }
        }
        else
        {
            // Invalid Command
            SYS_CMD_MESSAGE("Unsupported Command !\r\n");
        }
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\r\n");
    }
}

static void _commandHAR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t idx;

    if (argc == 1)
    {
        APP_METROLOGY_CaptureHarmonicData();

        // Read all metrology harmonics registers
        app_consoleData.numRegsPending = HARMONICS_REG_NUM;
        app_consoleData.state = APP_CONSOLE_STATE_READ_ALL_HARMONICS_REGS;
    }
    else if (argc == 2)
    {
        // Extract channel index from parameters
        idx = (uint8_t) strtol(argv[1], NULL, 10);
        if (idx < DRV_MCMETROLOGY_CHANNELS_NUMBER)
        {
            app_consoleData.channelNum = idx;

            APP_METROLOGY_CaptureHarmonicData();

            // Read registers for the channel number
            app_consoleData.state = APP_CONSOLE_STATE_READ_HARMONIC_REGS;

            /* Show console communication icon */
            APP_DISPLAY_SetSerialCommunication();
        }
        else
        {
            // Invalid index
            SYS_CMD_MESSAGE("Invalid channel\r\n");
        }
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\r\n");
    }
}

static void _commandHRR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t harmonicNum = 0xFF;

    if (argc == 2)
    {
        // Extract harmonic number from parameters
        harmonicNum = (uint8_t) strtol(argv[1], NULL, 10);
    }

    if ((harmonicNum > 0) && (harmonicNum <= DRV_MCMETROLOGY_HARMONICS_MAX_ORDER))
    {
        // Set harmonics calculation mode on metrology driver
        if (APP_METROLOGY_StartHarmonicAnalysis(harmonicNum, true) == false)
        {
            // Incorrect parameter number
            SYS_CMD_MESSAGE("Previous harmonic analysis is running\r\n");
        }
        else
        {
            /* Show console communication icon */
            APP_DISPLAY_SetSerialCommunication();
        }
        // Response will be provided on _harmonicAnalysisCallback function
    }
    else
    {
        // Incorrect harmonic number
        SYS_CMD_MESSAGE("Incorrect harmonic number\r\n");
    }
}

static void _commandHRRX(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t harmonicNum = 0xFF;
    uint8_t mode = 0xFF;

    if (argc == 3)
    {
        // This must be a Start command
        mode = (uint8_t) strtol(argv[1], NULL, 10);
        if (mode == 1)
        {
            // Extract harmonic number from parameters
            harmonicNum = (uint32_t) strtoul(argv[2], NULL, 10);
        }
    }

    if (mode == 0)
    {
        // Stop command
        APP_METROLOGY_StopHarmonicAnalysis();
    }
    else if (mode == 1)
    {
        if ((harmonicNum > 0) && (harmonicNum <= DRV_MCMETROLOGY_HARMONICS_MAX_ORDER))
        {
            // Start command
            // Set harmonics calculation mode on metrology driver
            if (APP_METROLOGY_StartHarmonicAnalysis(harmonicNum, false) == false)
            {
                // Analysis already running
                SYS_CMD_MESSAGE("Previous harmonic analysis is running\r\n");
            }
            else
            {
                /* Show console communication icon */
                APP_DISPLAY_SetSerialCommunication();
            }
            // Response will be provided on _harmonicAnalysisCallback function
        }
        else
        {
            // Incorrect harmonic number
            SYS_CMD_MESSAGE("Incorrect harmonic number\r\n");
        }
    }
    else
    {
        // Incorrect parameter format
        SYS_CMD_MESSAGE("Incorrect parameter format\r\n");
    }
}

static void _commandIDR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 1)
    {
        // Read Meter ID
        app_consoleData.state = APP_CONSOLE_STATE_READ_METER_ID;

        /* Show console communication icon */
        APP_DISPLAY_SetSerialCommunication();
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\r\n");
    }
}

static void _commandIDW(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 3)
    {
        // Check password from parameters
        if (strncmp(argv[1], metPwd, APP_CONSOLE_MET_PWD_SIZE) == 0)
        {
            // Correct password, write Meter ID
            memcpy(&app_consoleStorageData.meterID, argv[2], sizeof (app_consoleStorageData.meterID));
            // Build queue element to write it to storage
            datalogQueueElement.userId = APP_DATALOG_USER_CONSOLE;
            datalogQueueElement.operation = APP_DATALOG_WRITE;
            datalogQueueElement.endCallback = NULL;
            datalogQueueElement.date.year = APP_DATALOG_INVALID_YEAR; /* Not used */
            datalogQueueElement.date.month = APP_DATALOG_INVALID_MONTH; /* Not used */
            datalogQueueElement.dataLen = sizeof (app_consoleStorageData);
            datalogQueueElement.pData = (uint8_t*) & app_consoleStorageData;
            // Put it in queue
            APP_DATALOG_SendDatalogData(&datalogQueueElement);
            SYS_CMD_MESSAGE("Set Meter ID is Ok\r\n");

            /* Show console communication icon */
            APP_DISPLAY_SetSerialCommunication();
        }
        else
        {
            // Invalid password
            SYS_CMD_MESSAGE("Invalid password\r\n");
        }
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\r\n");
    }
}

static void _commandMDC(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 2)
    {
        // Check password from parameters
        if (strncmp(argv[1], metPwd, APP_CONSOLE_MET_PWD_SIZE) == 0)
        {
            // Correct password, Clear Max Demand records
            APP_ENERGY_ClearMaxDemand(true);
            // Show response on console
            SYS_CMD_MESSAGE("Clear MaxDemand is ok !\r\n");

            /* Show console communication icon */
            APP_DISPLAY_SetSerialCommunication();
        }
        else
        {
            // Invalid password
            SYS_CMD_MESSAGE("Invalid password\r\n");
        }
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\r\n");
    }
}

static void _commandMDR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    uint8_t monthIndex;

    if (argc > 2)
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\r\n");
    }

    if (argc == 2)
    {
        // Extract month index from parameters
        monthIndex = (uint8_t) strtol(argv[1], NULL, 10);
        monthIndex %= 12;
        app_consoleData.requestCounter = 1;
    }
    else
    {
        monthIndex = 0;
        // Start process to get full Monthly energy data
        app_consoleData.requestCounter = 12;
    }

    // Get SysTime
    _getSysTimeFromMonthIndex(&app_consoleData.sysTime, monthIndex);

    // Get monthly energy from energy app
    if (APP_ENERGY_GetMonthMaxDemand(&app_consoleData.sysTime) == false)
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param\r\n");
    }
    else
    {
        /* Show console communication icon */
        APP_DISPLAY_SetSerialCommunication();
    }
    // Response will be provided on _maxDemandCallback function
}

static void _commandPAR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    bool wakeup = false;

    if (argc >= 2)
    {
        // Extract data to retrieve from parameters
        if (strcmp(argv[1], "CH") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_CHANNEL;
            app_consoleData.cmdParMeasure = (uint8_t) CHN_MEASURE_RMS;
            wakeup = true;
        }
        else if (strcmp(argv[1], "P") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_POWER;
            app_consoleData.cmdParMeasure = (uint8_t) POW_MEASURE_P;
            wakeup = true;
        }
        else if (strcmp(argv[1], "Q") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_POWER;
            app_consoleData.cmdParMeasure = (uint8_t) POW_MEASURE_Q;
            wakeup = true;
        }
        else if (strcmp(argv[1], "CHF") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_CHANNEL;
            app_consoleData.cmdParMeasure = (uint8_t) CHN_MEASURE_RMS_F;
            wakeup = true;
        }
        else if (strcmp(argv[1], "PF") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_POWER;
            app_consoleData.cmdParMeasure = (uint8_t) POW_MEASURE_P_F;
            wakeup = true;
        }
        else if (strcmp(argv[1], "QF") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_POWER;
            app_consoleData.cmdParMeasure = (uint8_t) POW_MEASURE_Q_F;
            wakeup = true;
        }
        else if (strcmp(argv[1], "F") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_CHANNEL;
            app_consoleData.cmdParMeasure = (uint8_t) CHN_MEASURE_FREQ;
            wakeup = true;
        }
        else if (strcmp(argv[1], "A") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_POWER;
            app_consoleData.cmdParMeasure = (uint8_t) POW_MEASURE_ANGLE;
            wakeup = true;
        }
        else if (strcmp(argv[1], "MAX") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_CHANNEL;
            app_consoleData.cmdParMeasure = (uint8_t) CHN_MEASURE_MAX;
            wakeup = true;
        }
        else if (strcmp(argv[1], "VAB") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_MEASURE;
            app_consoleData.cmdParMeasure = (uint8_t) MEASURE_V_AB;
            wakeup = true;
        }
        else if (strcmp(argv[1], "VBC") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_MEASURE;
            app_consoleData.cmdParMeasure = (uint8_t) MEASURE_V_BC;
            wakeup = true;
        }
        else if (strcmp(argv[1], "VCA") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_MEASURE;
            app_consoleData.cmdParMeasure = (uint8_t) MEASURE_V_CA;
            wakeup = true;
        }
        else if (strcmp(argv[1], "VABF") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_MEASURE;
            app_consoleData.cmdParMeasure = (uint8_t) MEASURE_V_AB_F;
            wakeup = true;
        }
        else if (strcmp(argv[1], "VBCF") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_MEASURE;
            app_consoleData.cmdParMeasure = (uint8_t) MEASURE_V_BC_F;
            wakeup = true;
        }
        else if (strcmp(argv[1], "VCAF") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_MEASURE;
            app_consoleData.cmdParMeasure = (uint8_t) MEASURE_V_CA_F;
            wakeup = true;
        }
        else if (strcmp(argv[1], "PT") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_MEASURE;
            app_consoleData.cmdParMeasure = (uint8_t) MEASURE_PT;
            wakeup = true;
        }
        else if (strcmp(argv[1], "PTF") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_MEASURE;
            app_consoleData.cmdParMeasure = (uint8_t) MEASURE_PT_F;
            wakeup = true;
        }
        else if (strcmp(argv[1], "QT") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_MEASURE;
            app_consoleData.cmdParMeasure = (uint8_t) MEASURE_QT;
            wakeup = true;
        }
        else if (strcmp(argv[1], "QTF") == 0)
        {
            app_consoleData.state = APP_CONSOLE_STATE_PRINT_MEASURE;
            app_consoleData.cmdParMeasure = (uint8_t) MEASURE_QT_F;
            wakeup = true;
        }
        else
        {
            // Invalid Command
            SYS_CMD_MESSAGE("Unsupported Parameter.\r\n");
            SYS_CMD_MESSAGE("Supported parameter list: \r\n" \
                            "  CH: Voltage/Current channel, fundamental + harmonics\r\n" \
                            "  P: Active power, fundamental + harmonics\r\n" \
                            "  Q: Reactive power, fundamental + harmonics\r\n" \
                            "  CHF: Voltage/Current channel, fundamental only\r\n" \
                            "  PF: Active power, fundamental only\r\n" \
                            "  QF: Reactive power, fundamental only\r\n" \
                            "  F: Current Frequency\r\n" \
                            "  A: Voltage and Current angle\r\n" \
                            "  MAX: Max voltage or current in the interval\r\n" \
                            "  VAB: VA - VB, fundamental + harmonics\r\n" \
                            "  VBC: VB - VC, fundamental + harmonics\r\n" \
                            "  VCA: VC - VA, fundamental + harmonics\r\n" \
                            "  VABF: VA - VB, fundamental only \r\n" \
                            "  VBCF: VB - VC, fundamental only\r\n" \
                            "  VCAF: VC - VA, fundamental only\r\n" \
                            "  PT: Total Active Power, fundamental + harmonics\r\n" \
                            "  PTF: Total Active Power, fundamental only\r\n" \
                            "  QT: Total Reactive Power, fundamental + harmonics\r\n" \
                            "  QTF: Total Reactive Power, fundamental only\r\n" \
                            "\r\n");
        }


        if (wakeup)
        {
            // Set channel to get the parameters
            app_consoleData.cmdParCh = 0xFF;
            if (argc == 3)
            {
                uint8_t channel;

                // Extract channel
                channel = (uint8_t) strtol(argv[2], NULL, 10);
                app_consoleData.cmdParCh = channel;
            }
            /* Show console communication icon */
            APP_DISPLAY_SetSerialCommunication();
        }
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\r\n");
    }
}

static void _commandRTCR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 1)
    {
        // Read RTC
        app_consoleData.state = APP_CONSOLE_STATE_READ_RTC;

        /* Show console communication icon */
        APP_DISPLAY_SetSerialCommunication();
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\r\n");
    }
}

static void _commandRTCW(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 5)
    {
        if (strncmp(argv[1], metPwd, APP_CONSOLE_MET_PWD_SIZE) == 0)
        {
            char *p;

            // Get Date
            p = argv[2];
            // Year
            app_consoleData.sysTime.tm_year = (uint8_t) strtol(p, NULL, 10) + 2000 - 1900;
            // Look for "-" char and advance to next char
            p = strstr(p, "-");
            if (p != NULL)
            {
                p++;
                // Month
                app_consoleData.sysTime.tm_mon = (uint8_t) strtol(p, NULL, 10) - 1;
                // Look for "-" char and advance to next char
                p = strstr(p, "-");
                if (p != NULL)
                {
                    p++;
                    // Day
                    app_consoleData.sysTime.tm_mday = (uint8_t) strtol(p, NULL, 10);
                }
            }

            // Get Week Day
            p = argv[3];
            app_consoleData.sysTime.tm_wday = (uint8_t) strtol(p, NULL, 10) - 1;

            // Get Time
            p = argv[4];
            // Hour
            app_consoleData.sysTime.tm_hour = (uint8_t) strtol(p, NULL, 10);
            // Look for ":" char and advance to next char
            p = strstr(p, ":");
            if (p != NULL)
            {
                p++;
                // Minute
                app_consoleData.sysTime.tm_min = (uint8_t) strtol(p, NULL, 10);
                // Look for ":" char and advance to next char
                p = strstr(p, ":");
                if (p != NULL)
                {
                    p++;
                    // Second
                    app_consoleData.sysTime.tm_sec = (uint8_t) strtol(p, NULL, 10);
                }
            }

            if (RTC_TimeSet(&app_consoleData.sysTime))
            {
                // Build queue element to write it to storage
                datalogQueueElement.userId = APP_DATALOG_USER_RTC;
                datalogQueueElement.operation = APP_DATALOG_WRITE;
                datalogQueueElement.endCallback = NULL;
                datalogQueueElement.date.year = APP_DATALOG_INVALID_YEAR; /* Not used */
                datalogQueueElement.date.month = APP_DATALOG_INVALID_MONTH; /* Not used */
                datalogQueueElement.dataLen = sizeof (struct tm);
                datalogQueueElement.pData = (uint8_t*) & app_consoleData.sysTime;
                // Put it in queue
                APP_DATALOG_SendDatalogData(&datalogQueueElement);

                // Clear No-persistent energy/demand data
                APP_ENERGY_ClearEnergy(false);
                APP_ENERGY_ClearMaxDemand(false);

                SYS_CMD_MESSAGE("Set RTC is ok!\r\n");

                /* Show console communication icon */
                APP_DISPLAY_SetSerialCommunication();
            }
            else
            {
                SYS_CMD_MESSAGE("Unsupported Command !\r\n");
            }
        }
        else
        {
            SYS_CMD_MESSAGE("Password Error !\r\n");
        }
    }
    else
    {
        SYS_CMD_MESSAGE("Unsupported Command !\r\n");
    }
}

static void _commandTOUR(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 1)
    {
        // Go to state to read TOU
        app_consoleData.state = APP_CONSOLE_STATE_READ_TOU;

        /* Show console communication icon */
        APP_DISPLAY_SetSerialCommunication();
    }
    else
    {
        // Incorrect param number
        SYS_CMD_MESSAGE("Unsupported Command !\r\n");
    }
}

static void _commandTOUW(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    char *p;
    APP_ENERGY_TOU_TIME_ZONE timeZone[APP_ENERGY_TOU_MAX_ZONES];
    uint8_t idx, argIdx;
    bool parseError = false;

    if ((argc > 3) && ((argc - 2) % 2 == 0))
    {
        // Check password from parameters
        if (strncmp(argv[1], metPwd, APP_CONSOLE_MET_PWD_SIZE) == 0)
        {
            // Correct password, write TOW
            for (idx = 0; idx < APP_ENERGY_TOU_MAX_ZONES; idx++)
            {
                // Check whether there are arguments left to write
                argIdx = ((idx << 1) + 2);
                if (argc > argIdx)
                {
                    // Extract hour, minute and rate
                    p = argv[argIdx];
                    // Extract hour from argument
                    timeZone[idx].hour = (uint8_t) strtol(p, NULL, 10);
                    // Look for ":" char and advance to next char
                    p = strstr(p, ":");
                    if (p != NULL)
                    {
                        p++;
                        // Extract minute from argument
                        timeZone[idx].minute = (uint8_t) strtol(p, NULL, 10);
                        // Extract rate from next argument
                        timeZone[idx].tariff = (uint8_t) strtol(argv[argIdx + 1], NULL, 10);
                    }
                    else
                    {
                        parseError = true;
                        break;
                    }

                    if ((timeZone[idx].hour > 23) ||
                            (timeZone[idx].minute > 59) ||
                            (timeZone[idx].tariff > TARIFF_4))
                    {
                        parseError = true;
                        break;
                    }
                }
                else
                {
                    // No more arguments, fill TOU index with invalid data
                    timeZone[idx].hour = 0;
                    timeZone[idx].minute = 0;
                    timeZone[idx].tariff = TARIFF_INVALID;
                }
            }
        }
        else
        {
            // Invalid password
            parseError = true;
        }
    }
    else
    {
        // Incorrect parameter number
        parseError = true;
    }

    if (parseError)
    {
        SYS_CMD_MESSAGE("Unsupported Command !\r\n");
    }
    else
    {
        APP_ENERGY_SetTOUTimeZone(timeZone);

        // Clear No-persistent energy/demand data
        APP_ENERGY_ClearEnergy(false);
        APP_ENERGY_ClearMaxDemand(false);

        SYS_CMD_MESSAGE("Set TOU is Ok !\r\n");

        /* Show console communication icon */
        APP_DISPLAY_SetSerialCommunication();
    }
}

static void _commandRST(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 2)
    {
        // Check password from parameters
        if (strncmp(argv[1], metPwd, APP_CONSOLE_MET_PWD_SIZE) == 0)
        {
            // Correct password, Reset System
            SYS_CMD_MESSAGE("Reset Command is Ok !\r\n");
            // Go to state to reset system
            app_consoleData.state = APP_CONSOLE_STATE_SW_RESET;

            /* Show console communication icon */
            APP_DISPLAY_SetSerialCommunication();
        }
        else
        {
            // Invalid password
            SYS_CMD_MESSAGE("Invalid password\r\n");
        }
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\r\n");
    }
}

static void _commandRLD(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (argc == 2)
    {
        // Check password from parameters
        if (strncmp(argv[1], metPwd, APP_CONSOLE_MET_PWD_SIZE) == 0)
        {
            // Correct password, Reset System
            SYS_CMD_MESSAGE("Reloading Metrology...\r\n\r\n");
            // Reload Metrology coprocessor
            APP_METROLOGY_Restart();

            /* Show console communication icon */
            APP_DISPLAY_SetSerialCommunication();
        }
        else
        {
            // Invalid password
            SYS_CMD_MESSAGE("Invalid password\r\n");
        }
    }
    else
    {
        // Incorrect parameter number
        SYS_CMD_MESSAGE("Incorrect param number\r\n");
    }
}

static void _showChannelMeasure(DRV_MCMETROLOGY_CHANNEL_MEASURE_TYPE measure, uint8_t channelIndex)
{
    DRV_MCMETROLOGY_CHANNEL *pChannels;
    float value;
    uint8_t channelStart, channelEnd;
    uint8_t index;

    if (measure == CHN_MEASURE_FREQ)
    {
        APP_METROLOGY_GetMeasure(MEASURE_FREQ, &value);
        SYS_CMD_PRINT("Present frequency is : F=%.2fHz\r\n", value);
    }
    else if (measure == CHN_MEASURE_RMS)
    {
        SYS_CMD_MESSAGE("Present RMS values are:\r\n");
    }
    else if (measure == CHN_MEASURE_RMS_F)
    {
        SYS_CMD_MESSAGE("Present RMS (only fundamental) values are:\r\n");
    }
    else if (measure == CHN_MEASURE_MAX)
    {
        SYS_CMD_MESSAGE("Present MAX values are:\r\n");
    }
    else
    {
        SYS_CMD_MESSAGE("Invalid measure\r\n");
        return;
    }

    // Get Channels description
    channelEnd = APP_METROLOGY_GetChannelsDescription(&pChannels);

    if (channelIndex >= DRV_MCMETROLOGY_CHANNELS_NUMBER)
    {
        channelStart = 0;
    }
    else
    {
        channelStart = channelIndex;
        channelEnd = channelIndex + 1;
    }

    for (index = channelStart; index < channelEnd; index++)
    {
        APP_METROLOGY_GetChannelMeasure(measure, index, &value);
        if (measure == CHN_MEASURE_FREQ)
        {
            SYS_CMD_PRINT("F%02u=%.2fHz",index, value);
        }
        else
        {
            char units;

            if (APP_METROLOGY_CheckIsVoltageChannel(index) == true)
            {
                units = 'V';
            }
            else
            {
                units = 'A';
            }
            
            SYS_CMD_PRINT("CH%02u=%.3f%c",index, value, units);
        }
            
        if ((index % 4) == 3)
        {
            SYS_CMD_MESSAGE("\r\n");
        }
        else
        {
            SYS_CMD_MESSAGE(" ");
        }
    }
    SYS_CMD_MESSAGE("\r\n");
}

static void _showPowerMeasure(DRV_MCMETROLOGY_POWER_MEASURE_TYPE measure, uint8_t powerIndex)
{
    DRV_MCMETROLOGY_POWER_SOURCE *pPowers;
    float value;
    uint8_t powerStart, powerEnd;
    uint8_t index;
    
    if (measure == POW_MEASURE_ANGLE)
    {
        SYS_CMD_PRINT("Voltage and Current Angle are:\r\n");
    }
    else if (measure == POW_MEASURE_P)
    {
        SYS_CMD_PRINT("Active Powers are:\r\n");
    }
    else if (measure == POW_MEASURE_P_F)
    {
        SYS_CMD_PRINT("Active Powers are: (only fundamental)\r\n");
    }
    else if (measure == POW_MEASURE_Q)
    {
        SYS_CMD_PRINT("Reactive Powers are:\r\n");
    }
    else if (measure == POW_MEASURE_Q_F)
    {
        SYS_CMD_PRINT("Reactive Powers are: (only fundamental)\r\n");
    }
    else
    {
        SYS_CMD_MESSAGE("Invalid power measure\r\n");
        return;
    }

    // Get Channels description
    powerEnd = APP_METROLOGY_GetPowersDescription(&pPowers);

    if (powerIndex >= DRV_MCMETROLOGY_POWERS_NUMBER)
    {
        powerStart = 0;
    }
    else
    {
        powerStart = powerIndex;
        powerEnd = powerIndex + 1;
    }

    for (index = powerStart; index < powerEnd; index++)
    {
        APP_METROLOGY_GetPowerMeasure(measure, index, &value);
        if (measure == POW_MEASURE_ANGLE)
        {
            SYS_CMD_PRINT("Angle_PW%02u=%.3f",index, value);
        }
        else if (measure == POW_MEASURE_P)
        {
            SYS_CMD_PRINT("P%02u=%.3fW", index, value);
        }
        else if (measure == POW_MEASURE_P_F)
        {
            SYS_CMD_PRINT("PF%02u=%.3fW", index, value);
        }
        else if (measure == POW_MEASURE_Q)
        {
            SYS_CMD_PRINT("Q%02u=%.3fVar", index, value);
        }
        else if (measure == POW_MEASURE_Q_F)
        {
            SYS_CMD_PRINT("QF%02u=%.3fVar", index, value);
        }
        else
        {
            SYS_CMD_MESSAGE("Invalid power measure\r\n");
        }
            
        if ((index % 4) == 3)
        {
            SYS_CMD_MESSAGE("\r\n");
        }
        else
        {
            SYS_CMD_MESSAGE(" ");
        }
    }
    SYS_CMD_MESSAGE("\r\n");
}

static void _showMeasure(DRV_MCMETROLOGY_MEASURE_TYPE measure)
{
    float value;

    APP_METROLOGY_GetMeasure(measure, &value);

    if (measure == MEASURE_V_AB)
    {
        SYS_CMD_PRINT("Present V_AB=%.3fV\r\n", value);
    }
    else if (measure == MEASURE_V_BC)
    {
        SYS_CMD_PRINT("Present V_BC=%.3fV\r\n", value);
    }
    else if (measure == MEASURE_V_CA)
    {
        SYS_CMD_PRINT("Present V_CA=%.3fV\r\n", value);
    }
    else if (measure == MEASURE_V_AB_F)
    {
        SYS_CMD_PRINT("Present V_AB_F=%.3fV\r\n", value);
    }
    else if (measure == MEASURE_V_BC_F)
    {
        SYS_CMD_PRINT("Present V_BC_F=%.3fV\r\n", value);
    }
    else if (measure == MEASURE_V_CA_F)
    {
        SYS_CMD_PRINT("Present V_CA_F=%.3fV\r\n", value);
    }
    else if (measure == MEASURE_PT)
    {
        SYS_CMD_PRINT("Present PT=%.3fW\r\n", value);
    }
    else if (measure == MEASURE_PT_F)
    {
        SYS_CMD_PRINT("Present PT_F=%.3fW\r\n", value);
    }
    else if (measure == MEASURE_QT)
    {
        SYS_CMD_PRINT("Present QT=%.3fVAR\r\n", value);
    }
    else if (measure == MEASURE_QT_F)
    {
        SYS_CMD_PRINT("Present QT_F=%.3fVAR\r\n", value);
    }
    else
    {
        SYS_CMD_MESSAGE("Invalid measure\r\n");
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_CONSOLE_Initialize ( void )

  Remarks:
    See prototype in app_console.h.
 */

void APP_CONSOLE_Initialize(void)
{
    /* Place the App state machine in its initial state. */
    app_consoleData.state = APP_CONSOLE_STATE_INIT;
    app_consoleData.numCommands = sizeof (appCmdTbl) / sizeof (SYS_CMD_DESCRIPTOR);

    /* Init timer */
    app_consoleData.timer = SYS_TIME_HANDLE_INVALID;

    if (!SYS_CMD_ADDGRP(appCmdTbl, app_consoleData.numCommands, "App Console", ": Metering console commands"))
    {
        SYS_CONSOLE_Print(SYS_CONSOLE_INDEX_0, "Failed to create APP Console Commands\r\n");
        app_consoleData.numCommands = 0;
    }
    else
    {
        SYS_CMD_CallbackRegister(appCmdTbl, _preprocessorCallback, 0);
    }
}

/******************************************************************************
  Function:
    void APP_CONSOLE_Tasks ( void )

  Remarks:
    See prototype in app_console.h.
 */

void APP_CONSOLE_Tasks(void)
{
    char regName[4][20];
    uint32_t regValue32[4];
    uint64_t regValue64[4];
    uint16_t numRegsPending;

    /* Check the application's current state. */
    switch (app_consoleData.state)
    {
            /* Application's initial state. */
        case APP_CONSOLE_STATE_IDLE:
        case APP_CONSOLE_STATE_WAIT_DATA:
        {
            break;
        }

        case APP_CONSOLE_STATE_PROMPT:
        {
            SYS_CMD_MESSAGE(">");
            app_consoleData.state = APP_CONSOLE_STATE_IDLE;
            break;
        }

        case APP_CONSOLE_STATE_INIT:
        {
            if (SYS_CMD_READY_TO_READ())
            {
                /* Initialize Energy App callbacks */
                APP_ENERGY_SetMonthEnergyCallback(_monthlyEnergyCallback, &energyData);
                APP_ENERGY_SetMaxDemandCallback(_maxDemandCallback, &maxDemandLocalObject);

                /* Initialize Metrology App callbacks */
                APP_METROLOGY_SetHarmonicAnalysisCallback(_harmonicAnalysisCallback, &harmonicAnalysisRMSData);

                app_consoleData.currentWaitForDatalogReady = 0;
                app_consoleData.state = APP_CONSOLE_STATE_WAIT_STORAGE_READY;

                // Set default console storage data just in case it cannot be read later
                app_consoleStorageData = app_defaultConsoleStorageData;
            }
            break;
        }

        case APP_CONSOLE_STATE_WAIT_STORAGE_READY:
        {
            if (APP_DATALOG_GetStatus() == APP_DATALOG_STATE_READY)
            {
                app_consoleData.state = APP_CONSOLE_STATE_READ_STORAGE;
            }
            break;
        }

        case APP_CONSOLE_STATE_READ_STORAGE:
        {
            // Build queue element
            datalogQueueElement.userId = APP_DATALOG_USER_CONSOLE;
            datalogQueueElement.operation = APP_DATALOG_READ;
            datalogQueueElement.endCallback = _consoleReadStorage;
            datalogQueueElement.date.year = APP_DATALOG_INVALID_YEAR; /* Not used */
            datalogQueueElement.date.month = APP_DATALOG_INVALID_MONTH; /* Not used */
            datalogQueueElement.dataLen = sizeof (app_consoleStorageData);
            datalogQueueElement.pData = (uint8_t*) & app_consoleStorageData;
            // Put it in queue
            APP_DATALOG_SendDatalogData(&datalogQueueElement);

            // Wait for data to be read (semaphore is released in callback)
            app_consoleData.state = APP_CONSOLE_STATE_WAIT_DATA;
            // Data read, depending on read result, state has changed to READ_OK or READ_ERROR
            break;
        }

        case APP_CONSOLE_STATE_READ_STORAGE_ERROR:
        {
            // Build queue element to write it to storage
            datalogQueueElement.userId = APP_DATALOG_USER_CONSOLE;
            datalogQueueElement.operation = APP_DATALOG_WRITE;
            datalogQueueElement.endCallback = NULL;
            datalogQueueElement.date.year = APP_DATALOG_INVALID_YEAR; /* Not used */
            datalogQueueElement.date.month = APP_DATALOG_INVALID_MONTH; /* Not used */
            datalogQueueElement.dataLen = sizeof (app_consoleStorageData);
            datalogQueueElement.pData = (uint8_t*) & app_consoleStorageData;
            // Put it in queue
            APP_DATALOG_SendDatalogData(&datalogQueueElement);
            // Go to Idle state
            app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
            break;
        }

        case APP_CONSOLE_STATE_DATALOG_NOT_READY:
        {
            SYS_CMD_MESSAGE("Datalog Service not ready!\r\nApplication will run without storage capabilities\r\n");
            // Go to Idle state
            app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
            break;
        }

        case APP_CONSOLE_STATE_READ_CONTROL_REG:
        {
            // Remove Prompt symbol
            _removePrompt();

            // Read register value
            if (APP_METROLOGY_GetControlRegister((CONTROL_REG_ID) app_consoleData.ctrlRegToRead, &regValue32[0], regName[0]))
            {
                SYS_CMD_PRINT("%s\r\n%X\r\n", regName[0], regValue32[0]);
            }
            else
            {
                // Cannot read register
                SYS_CMD_PRINT("Could not read register %02d\r\n", app_consoleData.ctrlRegToRead);
            }
            // Go back to Idle
            app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
            break;
        }

        case APP_CONSOLE_STATE_WRITE_CONTROL_REG:
        {
            uint8_t idx;

            // Remove Prompt symbol
            _removePrompt();

            for (idx = 0; idx < APP_CONSOLE_MAX_REGS; idx++)
            {
                if (app_consoleData.regsToModify[idx].index != 0xFFFF)
                {
                    if (app_consoleData.regsToModify[idx].index < CONTROL_REG_NUM)
                    {
                        // Write register value
                        if (APP_METROLOGY_SetControlRegister((CONTROL_REG_ID) app_consoleData.regsToModify[idx].index,
                                                             app_consoleData.regsToModify[idx].value))
                        {
                            // Show response on console
                            SYS_CMD_PRINT("Set %02d Is Ok !\r\n", app_consoleData.regsToModify[idx].index);
                        }
                        else
                        {
                            // Cannot write register
                            SYS_CMD_PRINT("Could not write register %02d\r\n", app_consoleData.regsToModify[idx].index);
                        }
                    }
                    else
                    {
                        // Invalid index
                        SYS_CMD_PRINT("Invalid register index %02d\r\n", app_consoleData.regsToModify[idx].index);
                    }
                }
                else
                {
                    // All registers have been written
                    app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
                    break;
                }

                if ((idx % 10) == 0)
                {
                    app_consoleData.nextState = app_consoleData.state;
                    app_consoleData.state = APP_CONSOLE_STATE_DELAY;
                    app_consoleData.delayMs = CONSOLE_TASK_DEFAULT_DELAY_MS_BETWEEN_STATES;
                }
            }
            break;
        }

        case APP_CONSOLE_STATE_READ_ALL_CONTROL_REGS:
        {
            if (app_consoleData.ctrlRegToRead < CONTROL_REG_NUM)
            {
                if (app_consoleData.ctrlRegToRead == 0)
                {
                    // Remove Prompt symbol
                    _removePrompt();
                }

                // Check how many registers are pending to print, to format line
                numRegsPending = CONTROL_REG_NUM - app_consoleData.ctrlRegToRead;
                // Read and print register values
                if (numRegsPending >= 4)
                {
                    if ((APP_METROLOGY_GetControlRegister((CONTROL_REG_ID) app_consoleData.ctrlRegToRead, &regValue32[0], regName[0])) &&
                            (APP_METROLOGY_GetControlRegister((CONTROL_REG_ID) app_consoleData.ctrlRegToRead + 1, &regValue32[1], regName[1])) &&
                            (APP_METROLOGY_GetControlRegister((CONTROL_REG_ID) app_consoleData.ctrlRegToRead + 2, &regValue32[2], regName[2])) &&
                            (APP_METROLOGY_GetControlRegister((CONTROL_REG_ID) app_consoleData.ctrlRegToRead + 3, &regValue32[3], regName[3])))
                    {
                        SYS_CMD_PRINT("%-20s%-19s%-19s%-19s\r\n", regName[0], regName[1], regName[2], regName[3]);
                        SYS_CMD_PRINT("%-20X%-19X%-19X%-19X\r\n", regValue32[0], regValue32[1], regValue32[2], regValue32[3]);
                    }
                    else
                    {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\r\n", app_consoleData.ctrlRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.ctrlRegToRead += 4;
                }
                else if (numRegsPending == 3)
                {
                    if ((APP_METROLOGY_GetControlRegister((CONTROL_REG_ID) app_consoleData.ctrlRegToRead, &regValue32[0], regName[0])) &&
                            (APP_METROLOGY_GetControlRegister((CONTROL_REG_ID) app_consoleData.ctrlRegToRead + 1, &regValue32[1], regName[1])) &&
                            (APP_METROLOGY_GetControlRegister((CONTROL_REG_ID) app_consoleData.ctrlRegToRead + 2, &regValue32[2], regName[2])))
                    {
                        SYS_CMD_PRINT("%-20s%-19s%-19s\r\n", regName[0], regName[1], regName[2]);
                        SYS_CMD_PRINT("%-20X%-19X%-19X\r\n", regValue32[0], regValue32[1], regValue32[2]);
                    }
                    else
                    {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\r\n", app_consoleData.ctrlRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.ctrlRegToRead += 3;
                }
                else if (numRegsPending == 2)
                {
                    if ((APP_METROLOGY_GetControlRegister((CONTROL_REG_ID) app_consoleData.ctrlRegToRead, &regValue32[0], regName[0])) &&
                            (APP_METROLOGY_GetControlRegister((CONTROL_REG_ID) app_consoleData.ctrlRegToRead + 1, &regValue32[1], regName[1])))
                    {
                        SYS_CMD_PRINT("%-20s%-19s\r\n", regName[0], regName[1]);
                        SYS_CMD_PRINT("%-20X%-19X\r\n", regValue32[0], regValue32[1]);
                    }
                    else
                    {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\r\n", app_consoleData.ctrlRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.ctrlRegToRead += 2;
                }
                else if (numRegsPending == 1)
                {
                    if ((APP_METROLOGY_GetControlRegister((CONTROL_REG_ID) app_consoleData.ctrlRegToRead, &regValue32[0], regName[0])))
                    {
                        SYS_CMD_PRINT("%-20s\r\n", regName[0]);
                        SYS_CMD_PRINT("%-20X\r\n", regValue32[0]);
                    }
                    else
                    {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\r\n", app_consoleData.ctrlRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.ctrlRegToRead += 1;
                }
                else
                {
                    // All registers have been read
                    app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
                }

                app_consoleData.nextState = app_consoleData.state;
                app_consoleData.state = APP_CONSOLE_STATE_DELAY;
                app_consoleData.delayMs = CONSOLE_TASK_DELAY_MS_BETWEEN_REGS_PRINT;
            }
            else
            {
                app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
            }
            break;
        }

        case APP_CONSOLE_STATE_READ_ACCUM_REG:
        {
            // Remove Prompt symbol
            _removePrompt();

            // Read register value
            if (APP_METROLOGY_GetAccumulatorRegister((ACCUMULATOR_REG_ID) app_consoleData.accumRegToRead, &regValue64[0], regName[0]))
            {
                SYS_CMD_PRINT("%s\r\n%llX\r\n", regName[0], regValue64[0]);
            }
            else
            {
                // Cannot read register
                SYS_CMD_PRINT("Could not read register %02d\r\n", app_consoleData.accumRegToRead);
            }
            // Go back to Idle
            app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
            break;
        }

        case APP_CONSOLE_STATE_READ_ALL_ACCUM_REGS:
        {
            if (app_consoleData.accumRegToRead < ACCUMULATOR_REG_NUM)
            {
                if (app_consoleData.accumRegToRead == 0)
                {
                    // Remove Prompt symbol
                    _removePrompt();
                }

                // Check how many registers are pending to print, to format line
                numRegsPending = ACCUMULATOR_REG_NUM - app_consoleData.accumRegToRead;
                // Read and print register values
                if (numRegsPending >= 4)
                {
                    if ((APP_METROLOGY_GetAccumulatorRegister((ACCUMULATOR_REG_ID) app_consoleData.accumRegToRead, &regValue64[0], regName[0])) &&
                            (APP_METROLOGY_GetAccumulatorRegister((ACCUMULATOR_REG_ID) app_consoleData.accumRegToRead + 1, &regValue64[1], regName[1])) &&
                            (APP_METROLOGY_GetAccumulatorRegister((ACCUMULATOR_REG_ID) app_consoleData.accumRegToRead + 2, &regValue64[2], regName[2])) &&
                            (APP_METROLOGY_GetAccumulatorRegister((ACCUMULATOR_REG_ID) app_consoleData.accumRegToRead + 3, &regValue64[3], regName[3])))
                    {
                        SYS_CMD_PRINT("%-20s%-19s%-19s%-19s\r\n", regName[0], regName[1], regName[2], regName[3]);
                        SYS_CMD_PRINT("%-20llX%-19llX%-19llX%-19llX\r\n", regValue64[0], regValue64[1], regValue64[2], regValue64[3]);
                    }
                    else
                    {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\r\n", app_consoleData.accumRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.accumRegToRead += 4;
                }
                else if (numRegsPending == 3)
                {
                    if ((APP_METROLOGY_GetAccumulatorRegister((ACCUMULATOR_REG_ID) app_consoleData.accumRegToRead, &regValue64[0], regName[0])) &&
                            (APP_METROLOGY_GetAccumulatorRegister((ACCUMULATOR_REG_ID) app_consoleData.accumRegToRead + 1, &regValue64[1], regName[1])) &&
                            (APP_METROLOGY_GetAccumulatorRegister((ACCUMULATOR_REG_ID) app_consoleData.accumRegToRead + 2, &regValue64[2], regName[2])))
                    {
                        SYS_CMD_PRINT("%-20s%-19s%-19s\r\n", regName[0], regName[1], regName[2]);
                        SYS_CMD_PRINT("%-20llX%-19llX%-19llX\r\n", regValue64[0], regValue64[1], regValue64[2]);
                    }
                    else
                    {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\r\n", app_consoleData.accumRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.accumRegToRead += 3;
                }
                else if (numRegsPending == 2)
                {
                    if ((APP_METROLOGY_GetAccumulatorRegister((ACCUMULATOR_REG_ID) app_consoleData.accumRegToRead, &regValue64[0], regName[0])) &&
                            (APP_METROLOGY_GetAccumulatorRegister((ACCUMULATOR_REG_ID) app_consoleData.accumRegToRead + 1, &regValue64[1], regName[1])))
                    {
                        SYS_CMD_PRINT("%-20s%-19s\r\n", regName[0], regName[1]);
                        SYS_CMD_PRINT("%-20llX%-19llX\r\n", regValue64[0], regValue64[1]);
                    }
                    else
                    {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\r\n", app_consoleData.accumRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.accumRegToRead += 2;
                }
                else if (numRegsPending == 1)
                {
                    if ((APP_METROLOGY_GetAccumulatorRegister((ACCUMULATOR_REG_ID) app_consoleData.accumRegToRead, &regValue64[0], regName[0])))
                    {
                        SYS_CMD_PRINT("%-20s\r\n", regName[0]);
                        SYS_CMD_PRINT("%-20llX\r\n", regValue64[0]);
                    }
                    else
                    {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\r\n", app_consoleData.accumRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.accumRegToRead += 1;
                }
                else
                {
                    // All registers have been read
                    app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
                }

                app_consoleData.nextState = app_consoleData.state;
                app_consoleData.state = APP_CONSOLE_STATE_DELAY;
                app_consoleData.delayMs = CONSOLE_TASK_DELAY_MS_BETWEEN_REGS_PRINT;
            }
            else
            {
                app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
            }
            break;
        }

        case APP_CONSOLE_STATE_READ_STATUS_REG:
        {
            // Remove Prompt symbol
            _removePrompt();

            // Read register value
            if (APP_METROLOGY_GetStatusRegister((STATUS_REG_ID) app_consoleData.statusRegToRead, &regValue32[0], regName[0]))
            {
                SYS_CMD_PRINT("%s\r\n%X\r\n", regName[0], regValue32[0]);
            }
            else
            {
                // Cannot read register
                SYS_CMD_PRINT("Could not read register %02d\r\n", app_consoleData.statusRegToRead);
            }
            // Go back to Idle
            app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
            break;
        }

        case APP_CONSOLE_STATE_READ_ALL_STATUS_REGS:
        {
            if (app_consoleData.statusRegToRead < STATUS_REG_NUM)
            {
                if (app_consoleData.statusRegToRead == 0)
                {
                    // Remove Prompt symbol
                    _removePrompt();
                }

                // Check how many registers are pending to print, to format line
                numRegsPending = STATUS_REG_NUM - app_consoleData.statusRegToRead;
                // Read and print register values
                if (numRegsPending >= 4)
                {
                    if ((APP_METROLOGY_GetStatusRegister((STATUS_REG_ID) app_consoleData.statusRegToRead, &regValue32[0], regName[0])) &&
                            (APP_METROLOGY_GetStatusRegister((STATUS_REG_ID) app_consoleData.statusRegToRead + 1, &regValue32[1], regName[1])) &&
                            (APP_METROLOGY_GetStatusRegister((STATUS_REG_ID) app_consoleData.statusRegToRead + 2, &regValue32[2], regName[2])) &&
                            (APP_METROLOGY_GetStatusRegister((STATUS_REG_ID) app_consoleData.statusRegToRead + 3, &regValue32[3], regName[3])))
                    {
                        SYS_CMD_PRINT("%-20s%-19s%-19s%-19s\r\n", regName[0], regName[1], regName[2], regName[3]);
                        SYS_CMD_PRINT("%-20X%-19X%-19X%-19X\r\n", regValue32[0], regValue32[1], regValue32[2], regValue32[3]);
                    }
                    else
                    {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\r\n", app_consoleData.statusRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.statusRegToRead += 4;
                }
                else if (numRegsPending == 3)
                {
                    if ((APP_METROLOGY_GetStatusRegister((STATUS_REG_ID) app_consoleData.statusRegToRead, &regValue32[0], regName[0])) &&
                            (APP_METROLOGY_GetStatusRegister((STATUS_REG_ID) app_consoleData.statusRegToRead + 1, &regValue32[1], regName[1])) &&
                            (APP_METROLOGY_GetStatusRegister((STATUS_REG_ID) app_consoleData.statusRegToRead + 2, &regValue32[2], regName[2])))
                    {
                        SYS_CMD_PRINT("%-20s%-19s%-19s\r\n", regName[0], regName[1], regName[2]);
                        SYS_CMD_PRINT("%-20X%-19X%-19X\r\n", regValue32[0], regValue32[1], regValue32[2]);
                    }
                    else
                    {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\r\n", app_consoleData.statusRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.statusRegToRead += 3;
                }
                else if (numRegsPending == 2)
                {
                    if ((APP_METROLOGY_GetStatusRegister((STATUS_REG_ID) app_consoleData.statusRegToRead, &regValue32[0], regName[0])) &&
                            (APP_METROLOGY_GetStatusRegister((STATUS_REG_ID) app_consoleData.statusRegToRead + 1, &regValue32[1], regName[1])))
                    {
                        SYS_CMD_PRINT("%-20s%-19s\r\n", regName[0], regName[1]);
                        SYS_CMD_PRINT("%-20X%-19X\r\n", regValue32[0], regValue32[1]);
                    }
                    else
                    {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\r\n", app_consoleData.statusRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.statusRegToRead += 2;
                }
                else if (numRegsPending == 1)
                {
                    if ((APP_METROLOGY_GetStatusRegister((STATUS_REG_ID) app_consoleData.statusRegToRead, &regValue32[0], regName[0])))
                    {
                        SYS_CMD_PRINT("%-20s\r\n", regName[0]);
                        SYS_CMD_PRINT("%-20X\r\n", regValue32[0]);
                    }
                    else
                    {
                        // Cannot read register
                        SYS_CMD_PRINT("Could not read register %02d\r\n", app_consoleData.statusRegToRead);
                    }
                    // Advance to next register group
                    app_consoleData.statusRegToRead += 1;
                }
                else
                {
                    // All registers have been read
                    app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
                }

                app_consoleData.nextState = app_consoleData.state;
                app_consoleData.state = APP_CONSOLE_STATE_DELAY;
                app_consoleData.delayMs = CONSOLE_TASK_DELAY_MS_BETWEEN_REGS_PRINT;
            }
            else
            {
                app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
            }
            break;
        }

        case APP_CONSOLE_STATE_READ_HARMONIC_REGS:
        {
            uint8_t numRegId;

            // Remove Prompt symbol
            _removePrompt();

            numRegId = app_consoleData.channelNum;
            APP_METROLOGY_GetHarmonicRegister(numRegId, &regValue32[0], regName[0]);
            APP_METROLOGY_GetHarmonicRegister(numRegId + 31, &regValue32[1], regName[1]);

            SYS_CMD_PRINT("%-20s%-19s\r\n", regName[0], regName[1]);
            SYS_CMD_PRINT("%-20X%-19X\r\n", regValue32[0], regValue32[1]);

            app_consoleData.state = APP_CONSOLE_STATE_PROMPT;

            break;
        }

        case APP_CONSOLE_STATE_READ_ALL_HARMONICS_REGS:
        {
            if (app_consoleData.numRegsPending == HARMONICS_REG_NUM)
            {
                // Remove Prompt symbol
                _removePrompt();
            }

            // Read and print register values
            if (app_consoleData.numRegsPending == 0)
            {
                // All registers have been read
                app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
            }
            else
            {
                uint8_t numRegId;

                numRegId = HARMONICS_REG_NUM - app_consoleData.numRegsPending;

                // Read and print harmonic values
                if (app_consoleData.numRegsPending >= 4)
                {
                    APP_METROLOGY_GetHarmonicRegister(numRegId, &regValue32[0], regName[0]);
                    APP_METROLOGY_GetHarmonicRegister(numRegId + 1, &regValue32[1], regName[1]);
                    APP_METROLOGY_GetHarmonicRegister(numRegId + 2, &regValue32[2], regName[2]);
                    APP_METROLOGY_GetHarmonicRegister(numRegId + 3, &regValue32[3], regName[3]);

                    SYS_CMD_PRINT("%-20s%-19s%-19s%-19s\r\n", regName[0], regName[1], regName[2], regName[3]);
                    SYS_CMD_PRINT("%-20X%-19X%-19X%-19X\r\n", regValue32[0], regValue32[1], regValue32[2], regValue32[3]);

                    // Advance to next register group
                    app_consoleData.numRegsPending -= 4;
                }
                else if (app_consoleData.numRegsPending == 3)
                {
                    APP_METROLOGY_GetHarmonicRegister(numRegId, &regValue32[0], regName[0]);
                    APP_METROLOGY_GetHarmonicRegister(numRegId + 1, &regValue32[1], regName[1]);
                    APP_METROLOGY_GetHarmonicRegister(numRegId + 2, &regValue32[2], regName[2]);

                    SYS_CMD_PRINT("%-20s%-19s%-19s\r\n", regName[0], regName[1], regName[2]);
                    SYS_CMD_PRINT("%-20X%-19X%-19X\r\n", regValue32[0], regValue32[1], regValue32[2]);

                    // Advance to next register group
                    app_consoleData.numRegsPending -= 3;
                }
                else if (app_consoleData.numRegsPending == 2)
                {
                    APP_METROLOGY_GetHarmonicRegister(numRegId, &regValue32[0], regName[0]);
                    APP_METROLOGY_GetHarmonicRegister(numRegId + 1, &regValue32[1], regName[1]);

                    SYS_CMD_PRINT("%-20s%-19s\r\n", regName[0], regName[1]);
                    SYS_CMD_PRINT("%-20X%-19X\r\n", regValue32[0], regValue32[1]);

                    // Advance to next register group
                    app_consoleData.numRegsPending -= 2;
                }
                else if (app_consoleData.numRegsPending == 1)
                {
                    APP_METROLOGY_GetHarmonicRegister(numRegId, &regValue32[0], regName[0]);

                    SYS_CMD_PRINT("%-20s\r\n", regName[0]);
                    SYS_CMD_PRINT("%-20X\r\n", regValue32[0]);

                    // Advance to next register group
                    app_consoleData.numRegsPending--;
                }
                else
                {
                    // All registers have been read
                    app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
                }

                app_consoleData.nextState = app_consoleData.state;
                app_consoleData.state = APP_CONSOLE_STATE_DELAY;
                app_consoleData.delayMs = CONSOLE_TASK_DELAY_MS_BETWEEN_REGS_PRINT;
            }

            break;
        }

        case APP_CONSOLE_STATE_READ_METER_ID:
        {
            // Remove Prompt symbol
            _removePrompt();

            // Show response on console
            SYS_CMD_MESSAGE("Meter ID is:\r\n");
            SYS_CMD_PRINT("%s\r\n", app_consoleStorageData.meterID);
            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
            break;
        }

        case APP_CONSOLE_STATE_READ_RTC:
        {
            // Remove Prompt symbol
            _removePrompt();

            // Read and print RTC
            RTC_TimeGet(&app_consoleData.sysTime);
            SYS_CMD_MESSAGE("Present RTC is(yy-mm-dd w hh:mm:ss):\r\n");
            SYS_CMD_PRINT("%02u-%02u-%02u %u %02u:%02u:%02u\r\n",
                          app_consoleData.sysTime.tm_year + 1900 - 2000, app_consoleData.sysTime.tm_mon + 1, app_consoleData.sysTime.tm_mday,
                          app_consoleData.sysTime.tm_wday + 1, app_consoleData.sysTime.tm_hour, app_consoleData.sysTime.tm_min, app_consoleData.sysTime.tm_sec);
            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
            break;
        }

        case APP_CONSOLE_STATE_READ_TOU:
        {
            APP_ENERGY_TOU_TIME_ZONE * timeZone;
            uint8_t idx;

            timeZone = APP_ENERGY_GetTOUTimeZone();

            // Remove Prompt symbol
            _removePrompt();

            SYS_CMD_MESSAGE("TOU table is:\r\n");
            for (idx = 0; idx < APP_ENERGY_TOU_MAX_ZONES; idx++, timeZone++)
            {
                if (timeZone->tariff != TARIFF_INVALID)
                {
                    SYS_CMD_PRINT("TOU%d=%02d:%02d T%d ",
                                  (idx + 1), timeZone->hour, timeZone->minute, timeZone->tariff);
                }
            }
            SYS_CMD_MESSAGE("\r\n");

            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
            break;
        }

        case APP_CONSOLE_STATE_READ_DEVICES:
        {
            DRV_MCMETROLOGY_CHANNEL *pChannels;
            DRV_MCMETROLOGY_POWER_SOURCE *pPowers;
            uint8_t index;
            uint8_t indexMax;

            // Remove Prompt symbol
            _removePrompt();

            // Read devices topology
            SYS_CMD_PRINT("Channel X:\tName\tGAIN\tSENSOR TYPE\r\n");
            indexMax = APP_METROLOGY_GetChannelsDescription(&pChannels);
            for (index = 0; index < indexMax; index++)
            {
                SYS_CMD_PRINT("Channel %u:\t%s\tGAIN_%u\t%s\r\n", index,
                              pChannels->name, 1 << pChannels->gain,
                              gConsoleSensorTypes[pChannels->sensorType]);
                pChannels++;
            }

            // Read powers topology. restart pointer to pChannels
            SYS_CMD_PRINT("\r\nPower X:\tV_CH\t\tI_CH\r\n");
            indexMax = APP_METROLOGY_GetChannelsDescription(&pChannels);
            indexMax = APP_METROLOGY_GetPowersDescription(&pPowers);
            for (index = 0; index < indexMax; index++)
            {
                SYS_CMD_PRINT("Power %u:\tCH%u[%s] \tCH%u[%s]\r\n", index,
                              pPowers->vChannel, ((DRV_MCMETROLOGY_CHANNEL *) (pChannels + pPowers->vChannel))->name,
                              pPowers->iChannel, ((DRV_MCMETROLOGY_CHANNEL *) (pChannels + pPowers->iChannel))->name);
                pPowers++;
            }

            // Go back to Idle
            app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_ALL_HARMONIC_ANALYSIS:
        {
            double rmsValues[4];
            char units[4];
            uint8_t channel = DRV_MCMETROLOGY_CHANNELS_NUMBER - app_consoleData.channelNum;

            if (app_consoleData.channelNum == DRV_MCMETROLOGY_CHANNELS_NUMBER)
            {
                // Remove Prompt symbol
                _removePrompt();
            }

            // Read and print harmonic values
            if (app_consoleData.channelNum >= 4)
            {
                uint8_t index;

                for (index = 0; index < 4; index++)
                {
                    rmsValues[index] = harmonicAnalysisRMSData.CHx_m_RMS[channel + index];
                    if (APP_METROLOGY_CheckIsVoltageChannel(channel) == true)
                    {
                        sprintf(regName[index], "Vrms_Har_CH%02u(V)", channel);
                        units[index] = 'V';
                    }
                    else
                    {
                        sprintf(regName[index], "Irms_Har_CH%02u(A)", channel);
                        units[index] = 'A';
                    }
                }

                SYS_CMD_PRINT("%-20s%-19s%-19s%-19s\r\n", regName[0], regName[1], regName[2], regName[3]);
                SYS_CMD_PRINT("%-20.3f%C%-19.3f%C%-19.3f%C%-19.3f%C\r\n",
                              rmsValues[0], units[0], rmsValues[1], units[0],
                              rmsValues[2], units[2], rmsValues[3], units[3]);

                // Advance to next register group
                app_consoleData.channelNum -= 4;
            }
            else if (app_consoleData.channelNum == 3)
            {
                uint8_t index;

                for (index = 0; index < 3; index++)
                {
                    rmsValues[index] = harmonicAnalysisRMSData.CHx_m_RMS[channel + index];
                    if (APP_METROLOGY_CheckIsVoltageChannel(channel) == true)
                    {
                        sprintf(regName[index], "Vrms_Har_CH%02u(V)", channel);
                        units[index] = 'V';
                    }
                    else
                    {
                        sprintf(regName[index], "Irms_Har_CH%02u(A)", channel);
                        units[index] = 'A';
                    }
                }

                SYS_CMD_PRINT("%-20s%-19s%-19s\r\n", regName[0], regName[1], regName[2]);
                SYS_CMD_PRINT("%-20.3f%C%-19.3f%C%-19.3f%C\r\n",
                              rmsValues[0], units[0], rmsValues[1], units[0],
                              rmsValues[2], units[2]);

                // Advance to next register group
                app_consoleData.channelNum -= 3;
            }
            else if (app_consoleData.channelNum == 2)
            {
                uint8_t index;

                for (index = 0; index < 2; index++)
                {
                    rmsValues[index] = harmonicAnalysisRMSData.CHx_m_RMS[channel + index];
                    if (APP_METROLOGY_CheckIsVoltageChannel(channel) == true)
                    {
                        sprintf(regName[index], "Vrms_Har_CH%02u(V)", channel);
                        units[index] = 'V';
                    }
                    else
                    {
                        sprintf(regName[index], "Irms_Har_CH%02u(A)", channel);
                        units[index] = 'A';
                    }
                }

                SYS_CMD_PRINT("%-20s%-19s\r\n", regName[0], regName[1]);
                SYS_CMD_PRINT("%-20.3f%C%-19.3f%C\r\n",
                              rmsValues[0], units[0], rmsValues[1], units[0]);

                // Advance to next register group
                app_consoleData.channelNum -= 2;
            }
            else if (app_consoleData.channelNum == 1)
            {
                rmsValues[0] = harmonicAnalysisRMSData.CHx_m_RMS[channel];
                if (APP_METROLOGY_CheckIsVoltageChannel(channel) == true)
                {
                    sprintf(regName[0], "Vrms_Har_CH%02u(V)", channel);
                    units[0] = 'V';
                }
                else
                {
                    sprintf(regName[0], "Irms_Har_CH%02u(A)", channel);
                    units[0] = 'A';
                }

                SYS_CMD_PRINT("%-20s\r\n", regName[0]);
                SYS_CMD_PRINT("%-20.3f%C\r\n", rmsValues[0], units[0]);

                // Advance to next register group
                app_consoleData.channelNum -= 1;
            }

            // Read and print register values
            if (app_consoleData.channelNum == 0)
            {
                // All registers have been read
                app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
            }
            else
            {
                // Delay before continue printing
                app_consoleData.nextState = app_consoleData.state;
                app_consoleData.state = APP_CONSOLE_STATE_DELAY;
                app_consoleData.delayMs = CONSOLE_TASK_DELAY_MS_BETWEEN_REGS_PRINT;
            }

            break;
        }

        case APP_CONSOLE_STATE_PRINT_MONTHLY_ENERGY:
        {
            uint64_t total = 0;
            int8_t idx;

            for (idx = 0; idx < TARIFF_NUM_TYPE; idx++)
            {
                total += energyData.tariff[idx];
            }

            // Remove Prompt symbol
            _removePrompt();

            // Show received data on console
            idx = _getMonthIndexFromSysTime(&app_consoleData.timeRequest);
            SYS_CMD_PRINT("Last %d Month Energy is:\r\n", idx);

            SYS_CMD_PRINT("TT=%.2fkWh T1=%.2fkWh T2=%.2fkWh T3=%.2fkWh T4=%.2fkWh\r\n",
                          (float) total / 10000000, (float) energyData.tariff[0] / 10000000, (float) energyData.tariff[1] / 10000000,
                          (float) energyData.tariff[2] / 10000000, (float) energyData.tariff[3] / 10000000);

            /* Introduce a delay to wait console visualization */
            app_consoleData.nextState = APP_CONSOLE_STATE_PRINT_MONTHLY_ENERGY_NEXT;
            app_consoleData.state = APP_CONSOLE_STATE_DELAY;
            app_consoleData.delayMs = CONSOLE_TASK_DELAY_MS_BETWEEN_REGS_PRINT;

            break;
        }

        case APP_CONSOLE_STATE_PRINT_MONTHLY_ENERGY_NEXT:
        {
            // Check pending monthly requests
            app_consoleData.requestCounter--;
            if (app_consoleData.requestCounter > 0)
            {
                int8_t idx;

                idx = _getMonthIndexFromSysTime(&app_consoleData.timeRequest);

                _getSysTimeFromMonthIndex(&app_consoleData.timeRequest, idx + 1);
                APP_ENERGY_GetMonthEnergy(&app_consoleData.timeRequest);
            }

            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_EVENT:
        {
            APP_EVENTS_EVENT_INFO eventInfo;
            uint8_t numEvents;
            struct tm invalidTime = {0};

            // Remove Prompt symbol
            _removePrompt();

            APP_EVENTS_GetNumEvents(app_consoleData.eventIdRequest, &numEvents);
            if (APP_EVENTS_GetEventInfo(app_consoleData.eventIdRequest, app_consoleData.eventLastTimeRequest, &eventInfo))
            {
                uint8_t channel;

                // Print Event ID and requested Times
                if (app_consoleData.eventIdRequest < SAG_TH_0_EVENT_ID)
                {
                    channel = app_consoleData.eventIdRequest;
                    SYS_CMD_PRINT("Last %d Times SWELL in Channel %u is:\r\n",
                                  app_consoleData.eventLastTimeRequest, channel);
                }
                else if (app_consoleData.eventIdRequest < CREEP_TH_0_EVENT_ID)
                {
                    channel = app_consoleData.eventIdRequest - SAG_TH_0_EVENT_ID;
                    SYS_CMD_PRINT("Last %d Times SAG in Channel %u is:\r\n",
                                  app_consoleData.eventLastTimeRequest, channel);
                }
                else if (app_consoleData.eventIdRequest < PHACTIVE_TH_0_EVENT_ID)
                {
                    channel = app_consoleData.eventIdRequest - CREEP_TH_0_EVENT_ID;
                    SYS_CMD_PRINT("Last %d Times CREEP in Channel %u is:\r\n",
                                  app_consoleData.eventLastTimeRequest, channel);
                }
                else if (app_consoleData.eventIdRequest < EVENTS_NUM_ID)
                {
                    channel = app_consoleData.eventIdRequest - PHACTIVE_TH_0_EVENT_ID;
                    SYS_CMD_PRINT("Last %d Times PHACTIVE in Channel %u is:\r\n",
                                  app_consoleData.eventLastTimeRequest, channel);
                }

                // Show received data on console
                SYS_CMD_PRINT("Total Num=%d ", numEvents);

                if (memcmp(&eventInfo.startTime, &invalidTime, sizeof (struct tm)) == 0)
                {
                    SYS_CMD_PRINT("start time is invalid ");
                }
                else
                {
                    SYS_CMD_PRINT("start time is %02d-%02d %02d:%02d ",
                                  eventInfo.startTime.tm_mon + 1,
                                  eventInfo.startTime.tm_mday,
                                  eventInfo.startTime.tm_hour,
                                  eventInfo.startTime.tm_min);
                }

                if (memcmp(&eventInfo.endTime, &invalidTime, sizeof (struct tm)) == 0)
                {
                    SYS_CMD_PRINT("end time is invalid\r\n");
                }
                else
                {
                    SYS_CMD_PRINT("end time is %02d-%02d %02d:%02d\r\n",
                                  eventInfo.endTime.tm_mon + 1,
                                  eventInfo.endTime.tm_mday,
                                  eventInfo.endTime.tm_hour,
                                  eventInfo.endTime.tm_min);
                }
            }
            else
            {
                SYS_CMD_MESSAGE("Maximum number of reported events exceeded: 10\r\n");
            }

            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_MAX_DEMAND:
        {
            APP_ENERGY_DEMAND_DATA *pDataMax;
            APP_ENERGY_DEMAND_DATA *pDataTariff;
            int8_t idx;

            // Remove Prompt symbol
            _removePrompt();

            // Show received data on console
            idx = _getMonthIndexFromSysTime(&app_consoleData.timeRequest);
            SYS_CMD_PRINT("Last %d Month MaxDemand is:\r\n", idx);

            pDataMax = &maxDemandLocalObject.maxDemad;
            pDataTariff = &maxDemandLocalObject.tariff[0];
            SYS_CMD_PRINT("TT=%.3fkW %d-%d %02d:%02d T1=%.3fkW %d-%d %02d:%02d T2=%.3fkW %d-%d %02d:%02d T3=%.3fkW %d-%d %02d:%02d T4=%.3fkW %d-%d %02d:%02d\r\n",
                          (float) pDataMax->value / 1000, pDataMax->month + 1, pDataMax->day, pDataMax->hour, pDataMax->minute,
                          (float) pDataTariff->value / 1000, pDataTariff->month + 1, pDataTariff->day, pDataTariff->hour, pDataTariff->minute,
                          (float) (pDataTariff + 1)->value / 1000, (pDataTariff + 1)->month + 1, (pDataTariff + 1)->day, (pDataTariff + 1)->hour, (pDataTariff + 1)->minute,
                          (float) (pDataTariff + 2)->value / 1000, (pDataTariff + 2)->month + 1, (pDataTariff + 2)->day, (pDataTariff + 2)->hour, (pDataTariff + 2)->minute,
                          (float) (pDataTariff + 3)->value / 1000, (pDataTariff + 3)->month + 1, (pDataTariff + 3)->day, (pDataTariff + 3)->hour, (pDataTariff + 3)->minute);

            /* Introduce a delay to wait console visualization */
            app_consoleData.nextState = APP_CONSOLE_STATE_PRINT_MAX_DEMAND_NEXT;
            app_consoleData.state = APP_CONSOLE_STATE_DELAY;
            app_consoleData.delayMs = CONSOLE_TASK_DELAY_MS_BETWEEN_REGS_PRINT;

            break;
        }

        case APP_CONSOLE_STATE_PRINT_MAX_DEMAND_NEXT:
        {
            // Check pending monthly requests
            app_consoleData.requestCounter--;
            if (app_consoleData.requestCounter > 0)
            {
                int8_t idx;

                idx = _getMonthIndexFromSysTime(&app_consoleData.timeRequest);

                _getSysTimeFromMonthIndex(&app_consoleData.timeRequest, idx + 1);
                APP_ENERGY_GetMonthMaxDemand(&app_consoleData.timeRequest);
            }

            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_PROMPT;

            break;
        }

        case APP_CONSOLE_STATE_PRINT_CHANNEL:
        {
            // Remove Prompt symbol
            _removePrompt();

            // Display Channel Measure
            _showChannelMeasure(app_consoleData.cmdParMeasure, app_consoleData.cmdParCh);

            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_POWER:
        {
            // Remove Prompt symbol
            _removePrompt();

            // Display Power Measure
            _showPowerMeasure(app_consoleData.cmdParMeasure, app_consoleData.cmdParCh);

            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_MEASURE:
        {
            // Remove Prompt symbol
            _removePrompt();

            // Display Power Measure
            _showMeasure(app_consoleData.cmdParMeasure);

            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_WAVEFORM_DATA:
        {
            uint8_t idx;

            if (app_consoleData.rawDataFlag)
            {
                _removePrompt();
                app_consoleData.rawDataFlag = false;
            }

            for (idx = 0; idx < 10; idx++)
            {
                if (app_consoleData.rawDataLen > 0)
                {
                    // Print value
                    SYS_CMD_PRINT("%08X\r\n", *(app_consoleData.rawData));
                    // Advance to next value
                    app_consoleData.rawData++;
                    app_consoleData.rawDataLen--;
                }
                else
                {
                    // All registers have been read
                    app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
                    break;
                }
            }

            app_consoleData.nextState = app_consoleData.state;
            app_consoleData.state = APP_CONSOLE_STATE_DELAY;
            app_consoleData.delayMs = CONSOLE_TASK_DEFAULT_DELAY_MS_BETWEEN_STATES;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_CALIBRATION_RESULT:
        {
            // Remove Prompt symbol
            _removePrompt();

            // Show calibration result
            if (app_consoleData.calibrationResult)
            {
                SYS_CMD_MESSAGE("Calibrating Done!\r\n");
            }
            else
            {
                SYS_CMD_MESSAGE("Calibrating Fails!\r\n");
            }

            // Go back to IDLE
            app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
            break;
        }

        case APP_CONSOLE_STATE_PRINT_HELP:
        {
            uint8_t idx;
            uint8_t idxMax = 2;

            if (app_consoleData.cmdNumToShowHelp > 0)
            {
                if (app_consoleData.cmdNumToShowHelp < 5)
                {
                    idxMax = app_consoleData.cmdNumToShowHelp;
                }

                for (idx = 0; idx < idxMax; idx++, app_consoleData.pCmdDescToShowHelp++)
                {
                    SYS_CMD_PRINT("%s\t%s\r\n", app_consoleData.pCmdDescToShowHelp->cmdStr,
                                  app_consoleData.pCmdDescToShowHelp->cmdDescr);
                    app_consoleData.cmdNumToShowHelp--;
                }
            }
            else
            {
                // All commands have been represented
                app_consoleData.state = APP_CONSOLE_STATE_PROMPT;
                break;
            }

            app_consoleData.nextState = app_consoleData.state;
            app_consoleData.state = APP_CONSOLE_STATE_DELAY;
            app_consoleData.delayMs = CONSOLE_TASK_DEFAULT_DELAY_MS_BETWEEN_STATES;
            break;
        }

        case APP_CONSOLE_STATE_LOW_POWER_MODE:
        {
            // Remove Prompt symbol
            _removePrompt();

            SYS_CMD_MESSAGE("Entering Low Power... Press FWUP/TAMPER switch to wake up.\r\n");

            // Update display info
            APP_DISPLAY_ShowLowPowerMode();

            app_consoleData.nextState = app_consoleData.state;
            app_consoleData.state = APP_CONSOLE_STATE_DELAY;
            app_consoleData.delayMs = 100;
            break;
        }

        case APP_CONSOLE_STATE_SW_RESET:
        {
            /* Wait time to show message through the Console */
            app_consoleData.nextState = app_consoleData.state;
            app_consoleData.state = APP_CONSOLE_STATE_DELAY;
            app_consoleData.delayMs = 100;
            break;
        }

        case APP_CONSOLE_STATE_DELAY:
        {
            // Wait delay time
            if (APP_CONSOLE_TaskDelay(app_consoleData.delayMs, &app_consoleData.timer))
            {
                // Check low power state
                if (app_consoleData.nextState == APP_CONSOLE_STATE_LOW_POWER_MODE)
                {
                    // Go to Low Power mode
                    APP_METROLOGY_SetLowPowerMode();

                    // Execution should not come here during normal operation
                }
                else if (app_consoleData.nextState == APP_CONSOLE_STATE_SW_RESET)
                {
                    // Stop Metrology and its peripherals before reset
                    APP_METROLOGY_StopMetrology();
                    // Perform Reset
                    RSTC_Reset(RSTC_PROCESSOR_RESET);

                    // Execution should not come here during normal operation
                }
                else
                {
                    // Set next app state
                    app_consoleData.state = app_consoleData.nextState;
                }
            }
            break;
        }

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
