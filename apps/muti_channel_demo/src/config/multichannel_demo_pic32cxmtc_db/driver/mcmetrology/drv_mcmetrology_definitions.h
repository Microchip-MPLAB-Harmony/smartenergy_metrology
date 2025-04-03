/*******************************************************************************
  DRV_MCMETROLOGY Driver Interface Definition

  Company:
    Microchip Technology Inc.

  File Name:
    drv_mcmetrology_definitions.h

  Summary:
    Metrology Library Definitions Interface header.

  Description:
    The Metrology Library provides a interface to access the metrology data
    provided by the application running on Core 1.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef DRV_MCMETROLOGY_DEFINITIONS_H
#define DRV_MCMETROLOGY_DEFINITIONS_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdbool.h>
#include "driver/driver.h"
#include "system/system.h"
#include "drv_mcmetrology_regs.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************
#define DRV_MCMETROLOGY_CHANNELS_NUMBER             8U
#define DRV_MCMETROLOGY_POWERS_NUMBER               3U


#define DRV_MCMETROLOGY_HARMONICS_MAX_ORDER         31U

#define DRV_MCMETROLOGY_IPC_INIT_IRQ_MSK            IPC_ISR_IRQ20_Msk
#define DRV_MCMETROLOGY_IPC_INTEGRATION_IRQ_MSK     IPC_ISR_IRQ0_Msk

#define FORMAT_CONST_sQ031     31U
#define FORMAT_CONST_uQ2012    12U
#define FORMAT_CONST_sQ229     29U
#define FORMAT_CONST_uQ2440    40U
#define FORMAT_CONST_sQ2340    40U
#define FORMAT_CONST_sQ3330    30U
#define FORMAT_CONST_uQ824     24U
#define FORMAT_CONST_uQ230     30U
#define FORMAT_CONST_uQ1220    20U
#define FORMAT_CONST_sQ130     30U
#define FORMAT_CONST_uQ032     32U
#define FORMAT_CONST_uQ2210    10U
#define FORMAT_CONST_sQ940     40U

#define CONST_Pi               3.1415926
#define SAMPLING_FREQ          4000.0
#define SECS_IN_HOUR_DOUBLE    3600.0

/* Metrology Driver Sensor Type

  Summary:
    Describes the sensor type.

  Description:
    The metrology driver has been designed to interface with Current Transformers, Rogowski Coils, 
    Shunt Resistors current sensors and Resistive Divider voltage sensor.
*/
typedef enum {
    SENSOR_CT        = 0,
    SENSOR_SHUNT     = 1,
    SENSOR_ROGOWSKI  = 2,
    SENSOR_VRD       = 3,
    SENSOR_NUM_TYPE
} DRV_MCMETROLOGY_SENSOR_TYPE;

/* Metrology Driver Gain Type

  Summary:
    Gain selected for use in the ADC front-end.

  Description:
    On Current measurement channels gain for voltage is fixed to 1.
*/
typedef enum {
    GAIN_1        = 0,
    GAIN_2        = 1,
    GAIN_4        = 2,
    GAIN_8        = 3,
    GAIN_16       = 4,
    GAIN_32       = 5,
    GAIN_NUM_TYPE
} DRV_MCMETROLOGY_GAIN_TYPE;

/* Metrology Channel Description

  Summary:
    Describes the channel configuration

  Description:
    - Sensor Type
    - Channel Name
    - Channel Gain

*/
typedef struct {
    char *name;
    DRV_MCMETROLOGY_GAIN_TYPE gain;
    DRV_MCMETROLOGY_SENSOR_TYPE sensorType;
} DRV_MCMETROLOGY_CHANNEL;

/* Metrology Power Source Description

  Summary:
    Describes the power source configuration

  Description:
    - Voltage channel
    - Current channel
    - Calibration references

*/
typedef struct {
    uint8_t vChannel;
    uint8_t iChannel;
} DRV_MCMETROLOGY_POWER_SOURCE;

/* Metrology Driver Harmonic Data

  Summary:
    Identifies the result of the Harmonic Analysis process.

  Description:
    - CHx_m_RMS. RMS current value obtained as the result of last the harmonic analysis regarding channel X.
*/
typedef struct {
    double CHx_m_RMS[DRV_MCMETROLOGY_CHANNELS_NUMBER];
} DRV_MCMETROLOGY_HARMONICS_RMS;

/* Metrology Harmonic Analysis Data

  Summary:
    Internal data used to perform an harmonic analysis

  Description:
    - pHarmonicAnalysisResponse. Pointer to store the result of the Harmonic Analysis.
    - harmonicNumReq: Store number of harmonic for analysis.
    - integrationPeriods: Indicate the number of integration periods that must be waited until get the response
    - running: Flag to indicate that harmonic analysis is in process.
*/
typedef struct {
    DRV_MCMETROLOGY_HARMONICS_RMS * pHarmonicAnalysisResponse;
    uint32_t harmonicNumReq;
    uint8_t integrationPeriods;
    bool  running;
    bool holdRegs;
} DRV_MCMETROLOGY_HARMONIC_ANALYSIS;

/* Metrology Driver AFE Events

  Summary:
    Identifies all events related to metrology library.

  Description:
    - swell. Swell (voltage channels) or overcurrent (current channels) flag.
    - sag. Sag (voltage channels) flag.
    - creep. Creep (current channels) flag.
    - phActive. Phase Active flag (voltage channels).
*/
typedef struct {
    uint16_t swell;
    uint16_t sag;
    uint16_t creep;
    uint16_t phActive;
} DRV_MCMETROLOGY_AFE_EVENTS;

/* Metrology Driver Channel Measurements type 

  Summary:
    Identifies all types of channel measurements.

  Description:
        - RMS = Voltage/Current RMS value, fundamental + harmonics
        - RMS_F = Voltage/Current RMS value, fundamental only
        - MAX = Maximum Voltage/Current value during the measurement interval
        - FREQ = Frequency, fundamental only
*/
typedef enum {
    CHN_MEASURE_RMS = 0,
    CHN_MEASURE_RMS_F,
    CHN_MEASURE_MAX,
    CHN_MEASURE_FREQ,
    CHN_MEASURE_TYPE_NUM
} DRV_MCMETROLOGY_CHANNEL_MEASURE_TYPE;

/* Metrology Driver Power Source Measurements type

  Summary:
    Identifies all types of power source measurements.

  Description:
        - P = Active power value, fundamental + harmonics
        - P_F = Active power value, fundamental only
        - Q = Reactive power value, fundamental + harmonics
        - Q_F = Reactive power value, fundamental only
        - ANGLE = Angle between the voltage and current vectors
*/
typedef enum {
    POW_MEASURE_P = 0,
    POW_MEASURE_P_F,
    POW_MEASURE_Q,
    POW_MEASURE_Q_F,
    POW_MEASURE_ANGLE,
    POW_MEASURE_TYPE_NUM
} DRV_MCMETROLOGY_POWER_MEASURE_TYPE;

/* Metrology Driver Generic Measurements type

  Summary:
    Identifies all types of generic measurements.

  Description:
    Values are calculated assuming:
      - V_A is the first (lowest index) voltage channel defined
      - V_B is the second voltage channel defined
      - V_C is the third voltage channel defined
    
    , where:
        - V_AB = accumulated voltage to voltage (V) quantities between channels: V_A - V_B, fundamental + harmonics
        - V_BC = accumulated voltage to voltage (V) quantities between channels: V_B - V_C, fundamental + harmonics
        - V_CB = accumulated voltage to voltage (V) quantities between channels: V_C - V_A, fundamental + harmonics
        - V_AB_F = accumulated voltage to voltage (V) quantities between channels: V_A - V_B, fundamental only
        - V_BC_F = accumulated voltage to voltage (V) quantities between channels: V_B - V_C, fundamental only
        - V_CB_F = accumulated voltage to voltage (V) quantities between channels: V_C - V_A, fundamental only
        - ACC_TO = accumulated total energy according to the Pulse 0 Type selection (PC0_Type)
        - ACC_T1 = accumulated total energy according to the Pulse 1 Type selection (PC1_Type)
        - ACC_T2 = accumulated total energy according to the Pulse 2 Type selection (PC2_Type)
        - FREQ = Frequency of fundamental harmonic
        - PT = accumulated active power, fundamental + harmonics
        - PT_F = accumulated active power, fundamental only
        - QT = accumulated reactive power, fundamental + harmonics
        - QT_F = accumulated reactive power, fundamental only
        - ENERGY = accumulated total active energy
*/
typedef enum {
    MEASURE_V_AB = 0,
    MEASURE_V_BC,
    MEASURE_V_CA,
    MEASURE_V_AB_F,
    MEASURE_V_BC_F,
    MEASURE_V_CA_F,
    MEASURE_ACC_TO,
    MEASURE_ACC_T1,
    MEASURE_ACC_T2,
    MEASURE_FREQ,
    MEASURE_PT,
    MEASURE_PT_F,
    MEASURE_QT,
    MEASURE_QT_F,
    MEASURE_ENERGY,
    MEASURE_TYPE_NUM
} DRV_MCMETROLOGY_MEASURE_TYPE;

/* Metrology Driver AFE calculated data

  Summary:
    Identifies the data calculated from the metrology AFE measurements.

  Description:
    - energy. Active energy calculated value.
    - afeEvents. AFE events data.
    - chnMeasure. Measure calculated values depending on channel.
    - powMeasure. Measure calculated values depending on power source.
    - measure. Measure calculated values.
*/
typedef struct {
    DRV_MCMETROLOGY_AFE_EVENTS events;
    float chnMeasure[DRV_MCMETROLOGY_CHANNELS_NUMBER][CHN_MEASURE_TYPE_NUM];
    float powMeasure[DRV_MCMETROLOGY_POWERS_NUMBER][POW_MEASURE_TYPE_NUM];
    float measure[MEASURE_TYPE_NUM];
} DRV_MCMETROLOGY_AFE_DATA;

/* Metrology Driver Configuration

  Summary:
    Identifies values needed to set different metrology configurations.

  Description:
    - mc. Meter Constant (amount of energy signified by one output pulse). Units: pulses/kWh (active energy), pulses/kVARh (reactive energy), or pulses/kAmp2-h (amp square)
    - freq. Mains frequency. Units: Hz.
    - tr.
      - In the case of a current transformer, this is the current transformer ratio
      - In the case of a Rogowski Coil, this is the current sensitivity (units: uV/A) at the main frequency specified in ?Frequency? parameter.
    - rl.
      - In the case of current transformer, this is the resistor load or burden resistor (units uOhms).
      - In the case of shunt resistor, this is the shunt resistor value (units uOhms)
    - ku. Voltage divider ratio.
    - st. Sensor Type. Refer to DRV_MCMETROLOGY_SENSOR_TYPE.
    - gain. Programmable Gain Amplifier of the AFE (analog front end). Refer to DRV_MCMETROLOGY_GAIN_TYPE.
*/
typedef struct {
    uint32_t mc;
    double freq;
    double tr;
    double rl;
    uint32_t ku;
    DRV_MCMETROLOGY_SENSOR_TYPE st;
    DRV_MCMETROLOGY_GAIN_TYPE gain;
} DRV_MCMETROLOGY_CONFIGURATION;

/* METROLOGY Driver Status

  Summary:
    Defines the status of the DRV_MCMETROLOGY driver.

  Description:
    This enumeration defines the status of the DRV_MCMETROLOGY Driver:
        - DRV_MCMETROLOGY_STATUS_UNINITIALIZED: Metrology driver has not been initialized.
        - DRV_MCMETROLOGY_STATUS_READY: Metrology driver is ready to be used.
        - DRV_MCMETROLOGY_STATUS_HALT: Metrology driver has been initialized but not opened.
        - DRV_MCMETROLOGY_STATUS_WAITING_IPC: Metrology driver is waiting the init IPC interrupt
        from the metrology lib as part of the opening routine.
        - DRV_MCMETROLOGY_STATUS_INIT_DSP: IPC interrupt has been triggered indicating that DSP
        filters has been stabilized to full accuracy.
        - DRV_MCMETROLOGY_STATUS_RUNNING: Metrology library is running and periodic data
        acquisition is performed.

  Remarks:
    None.
*/

typedef enum
{
    DRV_MCMETROLOGY_STATUS_UNINITIALIZED = SYS_STATUS_UNINITIALIZED,
    DRV_MCMETROLOGY_STATUS_BUSY = SYS_STATUS_BUSY,
    DRV_MCMETROLOGY_STATUS_READY = SYS_STATUS_READY,
    DRV_MCMETROLOGY_STATUS_HALT = SYS_STATUS_READY_EXTENDED + 1U,
    DRV_MCMETROLOGY_STATUS_WAITING_IPC = SYS_STATUS_READY_EXTENDED + 2U,
    DRV_MCMETROLOGY_STATUS_INIT_DSP = SYS_STATUS_READY_EXTENDED + 3U,
    DRV_MCMETROLOGY_STATUS_RUNNING = SYS_STATUS_READY_EXTENDED + 4U,
    DRV_MCMETROLOGY_STATUS_ERROR = SYS_STATUS_ERROR,
} DRV_MCMETROLOGY_STATUS;

/* Metrology Driver Initialization Data

  Summary:
    Defines the data required to initialize the Metrology driver

  Description:
    - regBaseAddress. Base Address for Metrology registers.
    - binStartAddress. Start Address where Metrology library application file is located.
    - binEndAddress. End Address where Metrology library application binary file is located.

  Remarks:
    None.
*/
typedef struct {
    uint32_t regBaseAddress;
    uint32_t binStartAddress;
    uint32_t binEndAddress;
} DRV_MCMETROLOGY_INIT;

#ifdef __cplusplus
}
#endif

#endif // #ifndef DRV_MCMETROLOGY_DEFINITIONS_H
/*******************************************************************************
 End of File
*/
