/*******************************************************************************
  DRV_METROLOGY Driver Interface Definition

  Company:
    Microchip Technology Inc.

  File Name:
    drv_metrology_definitions.h

  Summary:
    Metrology Library Definitions Interface header.

  Description:
    The Metrology Library provides a interface to access the metrology data
    provided by the application running on Core 1.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef DRV_METROLOGY_DEFINITIONS_H
#define DRV_METROLOGY_DEFINITIONS_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdbool.h>
#include "driver/driver.h"
#include "system/system.h"
#include "drv_metrology_regs.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: External Data
// *****************************************************************************
// *****************************************************************************

/* Metrology library Binary file addressing */
extern uint8_t met_bin_start;
extern uint8_t met_bin_end;

// *****************************************************************************
// *****************************************************************************
// Section: Macro Definitions
// *****************************************************************************
// *****************************************************************************
#define DRV_METROLOGY_HARMONICS_MAX_ORDER         31U

#define DRV_METROLOGY_IPC_INIT_IRQ_MSK            IPC_ISR_IRQ20_Msk
#define DRV_METROLOGY_IPC_INTEGRATION_IRQ_MSK     IPC_ISR_IRQ0_Msk
<#if DRV_MET_NOT_FULL_CYCLE == true>
#define DRV_METROLOGY_IPC_FULLCYCLE_IRQ_MSK       IPC_ISR_IRQ4_Msk
</#if>
<#if DRV_MET_NOT_HALF_CYCLE == true>
#define DRV_METROLOGY_IPC_HALFCYCLE_IRQ_MSK       IPC_ISR_IRQ5_Msk
</#if>
<#if DRV_MET_RAW_ZERO_CROSSING == true>
#define DRV_METROLOGY_IPC_ZEROCROSS_IRQ_MSK       IPC_ISR_IRQ12_Msk
</#if>
<#if DRV_MET_PULSE_0 == true>
#define DRV_METROLOGY_IPC_PULSE0_IRQ_MSK          IPC_ISR_IRQ24_Msk
</#if>
<#if DRV_MET_PULSE_1 == true>
#define DRV_METROLOGY_IPC_PULSE1_IRQ_MSK          IPC_ISR_IRQ25_Msk
</#if>
<#if DRV_MET_PULSE_2 == true>
#define DRV_METROLOGY_IPC_PULSE2_IRQ_MSK          IPC_ISR_IRQ26_Msk
</#if>

#define FREQ_Q                 12U
#define DIV_FREQ_Q_FACTOR      0x1000UL /* (1 << FREQ_Q) */
#define GAIN_P_K_T_Q           24U
#define GAIN_VI_Q              10U
#define DIV_GAIN               1024U /* (1 << GAIN_VI_Q) */
#define CAL_VI_Q               29U
#define CAL_PH_Q               31U
#define Q_FACTOR               40U
#define DIV_Q_FACTOR           0x10000000000ULL /* (1 << Q_FACTOR) */
#define Inx_Q_FACTOR           20U
#define DIV_Inx_Q_FACTOR       0x100000UL /* (1 << Inx_Q_FACTOR) */
#define PQ_OFFSET_Q_FACTOR     30U
#define DIV_PQ_OFFSET_Q_FACTOR 0x40000000UL /* (1 << PQ_OFFSET_Q_FACTOR) */
#define PQ_SYMB                0x8000000000000000ULL /* p/q symbol bit */
#define HARMONIC_FACTOR        0x80000000UL
#define CONST_Pi               3.1415926
#define SAMPLING_FREQ          4000.0
#define VI_ACCURACY_DOUBLE     10000.0
#define VI_ACCURACY_INT        10000U
#define SECS_IN_HOUR_DOUBLE    3600.0
#define PQS_ACCURACY_DOUBLE    10.0
#define PQS_ACCURACY_INT       10U
#define FREQ_ACCURACY_INT      100U
#define ANGLE_ACCURACY_DOUBLE  100000.0
#define ANGLE_ACCURACY_INT     100000U
#define ENERGY_ACCURACY_DOUBLE 10000.0
#define ENERGY_ACCURACY_INT    10000U

/* Metrology Driver Sensor Type

  Summary:
    Describes the sensor type.

  Description:
    The metrology driver has been designed to interface with Current Transformers, Rogowski Coils and Shunt Resistors current sensors.
*/
typedef enum {
    SENSOR_CT        = 0,
    SENSOR_SHUNT     = 1,
    SENSOR_ROGOWSKI  = 2,
    SENSOR_NUM_TYPE
} DRV_METROLOGY_SENSOR_TYPE;

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
    GAIN_NUM_TYPE
} DRV_METROLOGY_GAIN_TYPE;

/* Metrology Driver Phase Identifier

  Summary:
    Line identifier used in the calibration process.

  Description:
    Phase A, B and C are used to identify Ua, Ub and Uc respectively. Phase T includes all phases.
*/
typedef enum {
    PHASE_A        = 1,
    PHASE_B        = 2,
    PHASE_C        = 3,
    PHASE_N        = 4,
    PHASE_T        = 5,
    PHASE_TN       = 6,
    PHASE_ID_NUM   = PHASE_TN
} DRV_METROLOGY_PHASE_ID;


/* Metrology Driver Calibration References

  Summary:
    Specifies the all reference values used for the auto calibration process.

  Description:
    - aimVx refers to the RMS voltage applied to the voltage input where x = A,B,C
    - aimIx refers to the Rms current applied to the current input where x = A,B,C
    - anglex refers to the Angle between the voltage and current vectors where x = A,B,C
    - lineID identifies the phase/phases to calibrate
*/
typedef struct {
    double aimVA;
    double aimIA;
    double angleA;
    double aimVB;
    double aimIB;
    double angleB;
    double aimVC;
    double aimIC;
    double angleC;
    double aimIN;
    double angleN;
    DRV_METROLOGY_PHASE_ID lineId;
} DRV_METROLOGY_CALIBRATION_REFS;

/* Metrology Driver Calibration Data

  Summary:
    Specifies all data internally needed for the auto calibration process.

  Description:
    - metControlConf. Dummy variable used in internal computations.
    - references. Calibration references. Client must be set the references before starting the calibration process.
    - freq. Stores the mains frequency passed as parameter in the DRV_METROLOGY_SetConfiguration() routine.
    - numIntegrationPeriods. Number of integration periods needed to complete the calibration process. It is set internally to 4.
    - running. Flag used to check if the calibration process was completed.
    - result. Flag used to check if the calibration process has been successful.
    - Rest of the values are internally used to perform the calibration process.
*/
typedef struct {
    DRV_METROLOGY_REGS_CONTROL metControlConf;
    DRV_METROLOGY_CALIBRATION_REFS references;
    uint32_t featureCtrlBackup;
    double freq;
    uint32_t numIntegrationPeriods;
    uint64_t dspAccIa;
    uint64_t dspAccIb;
    uint64_t dspAccIc;
    uint64_t dspAccIn;
    uint64_t dspAccUa;
    uint64_t dspAccUb;
    uint64_t dspAccUc;
    int64_t  dspAccPa;
    int64_t  dspAccPb;
    int64_t  dspAccPc;
    int64_t  dspAccPn;
    int64_t  dspAccQa;
    int64_t  dspAccQb;
    int64_t  dspAccQc;
    int64_t  dspAccQn;
    bool  running;
    bool  result;
} DRV_METROLOGY_CALIBRATION;

/* Metrology Driver Harmonic Data

  Summary:
    Identifies the result of the Harmonic Analysis process.

  Description:
    - Irms_X_m. RMS current value obtained as the result of last the harmonic analysis regarding channel X.
    - Irms_N_m. RMS current value obtained as the result of last the harmonic analysis regarding neutral channel.
    - Vrms_X_m. RMS voltage value obtained as the result of last the harmonic analysis regarding channel X.
*/
typedef struct {
    double Irms_A_m;
    double Irms_B_m;
    double Irms_C_m;
    double Irms_N_m;
    double Vrms_A_m;
    double Vrms_B_m;
    double Vrms_C_m;
} DRV_METROLOGY_HARMONICS_RMS;

/* Metrology Harmonic Analysis Data

  Summary:
    Internal data used to perform an harmonic analysis

  Description:
    - pHarmonicAnalysisResponse. Pointer to store the result of the Harmonic Analysis.
    - harmonicBitmap: Store the harmonics to be analyzed.
    - integrationPeriods: Indicate the number of integration periods that must be waited until get the response
    - running: Flag to indicate that harmonic analysis is in process.
*/
typedef struct {
    DRV_METROLOGY_HARMONICS_RMS * pHarmonicAnalysisResponse;
    uint32_t harmonicBitmap;
    uint8_t integrationPeriods;
    bool  running;
    bool holdRegs;
} DRV_METROLOGY_HARMONIC_ANALYSIS;

/* Metrology Driver AFE Events

  Summary:
    Identifies all events related to metrology library.

  Description:
    - pXDir. Identifies the sign of the active power in channel X. "1" means a negative value, "0" is a positive value.
    - qXDir. Identifies the sign of the reactive power in channel X. "1" means a negative value, "0" is a positive value.
    - sagX. Voltage Sag Detected Flag for Channel X. "1" means that voltage sag is detected.
    - swellX. Voltage Swell Detected Flag for Channel X. "1" means that voltage Swell is detected.
    - creepX. Channel X Current or Power Creep Detected Flag. "1" means that Creep is detected.
    - phActiveX. Voltage Active Detected Flag for Channel X. "1" means that voltage Active is detected.
*/
typedef struct {
    unsigned int paDir : 1;
    unsigned int pbDir : 1;
    unsigned int pcDir : 1;
    unsigned int ptDir : 1;
    unsigned int qaDir : 1;
    unsigned int qbDir : 1;
    unsigned int qcDir : 1;
    unsigned int qtDir : 1;
    unsigned int pafDir : 1;
    unsigned int pbfDir : 1;
    unsigned int pcfDir : 1;
    unsigned int ptfDir : 1;
    unsigned int qafDir : 1;
    unsigned int qbfDir : 1;
    unsigned int qcfDir : 1;
    unsigned int qtfDir : 1;
    unsigned int sagA : 1;
    unsigned int sagB : 1;
    unsigned int sagC : 1;
    unsigned int swellA : 1;
    unsigned int swellB : 1;
    unsigned int swellC : 1;
    unsigned int creepIA : 1;
    unsigned int creepIB : 1;
    unsigned int creepIC : 1;
    unsigned int creepQ : 1;
    unsigned int creepP : 1;
    unsigned int creepS : 1;
    unsigned int phActiveA : 1;
    unsigned int phActiveB : 1;
    unsigned int phActiveC : 1;
    unsigned int reserved : 1;
} DRV_METROLOGY_AFE_EVENTS;

typedef union {
    DRV_METROLOGY_AFE_EVENTS afeEvents;
    uint32_t afeEventsMask;
} DRV_METROLOGY_AFE_EVENTS_UNION;

/* Metrology Driver Measurements type

  Summary:
    Identifies all types of measurements.

  Description:
    Values are calculated both including all harmonics and fundamental only (F added at the end of magnitude name), where:
        - U = Voltage RMS value
        - I = Current RMS value
        - P = Active power value
        - Q = Reactive power value
        - S = Aparent power value
        - FREQ = Frequency of the line voltage fundamental harmonic component determined by the Metrology library using the dominant phase
        - ANGLE = Angle between the voltage and current vectors
*/
typedef enum {
    MEASURE_UA_RMS = 0,
    MEASURE_UB_RMS,
    MEASURE_UC_RMS,
    MEASURE_IA_RMS,
    MEASURE_IB_RMS,
    MEASURE_IC_RMS,
    MEASURE_INI_RMS,
    MEASURE_INM_RMS,
    MEASURE_INMI_RMS,
    MEASURE_PT,
    MEASURE_PA,
    MEASURE_PB,
    MEASURE_PC,
    MEASURE_QT,
    MEASURE_QA,
    MEASURE_QB,
    MEASURE_QC,
    MEASURE_ST,
    MEASURE_SA,
    MEASURE_SB,
    MEASURE_SC,
    MEASURE_UAF_RMS,
    MEASURE_UBF_RMS,
    MEASURE_UCF_RMS,
    MEASURE_IAF_RMS,
    MEASURE_IBF_RMS,
    MEASURE_ICF_RMS,
    MEASURE_IMNF_RMS,
    MEASURE_PTF,
    MEASURE_PAF,
    MEASURE_PBF,
    MEASURE_PCF,
    MEASURE_QTF,
    MEASURE_QAF,
    MEASURE_QBF,
    MEASURE_QCF,
    MEASURE_STF,
    MEASURE_SAF,
    MEASURE_SBF,
    MEASURE_SCF,
    MEASURE_FREQ,
    MEASURE_FREQA,
    MEASURE_FREQB,
    MEASURE_FREQC,
    MEASURE_ANGLEA,
    MEASURE_ANGLEB,
    MEASURE_ANGLEC,
    MEASURE_ANGLEN,
    MEASURE_TYPE_NUM
} DRV_METROLOGY_MEASURE_TYPE;

/* Metrology Driver AFE calculated data

  Summary:
    Identifies the data calculated from the metrology AFE measurements.

  Description:
    - energy. Active energy calculated value.
    - afeEvents. AFE events data.
    - measure[MEASURE_TYPE_NUM]. Measure calculated values.
*/
typedef struct {
    int32_t energy;
    DRV_METROLOGY_AFE_EVENTS afeEvents;
    uint32_t measure[MEASURE_TYPE_NUM];
} DRV_METROLOGY_AFE_DATA;

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
      - In the case of current transformer, this is the resistor load or burden resistor (units: ?).
      - In the case of shunt resistor, this is the shunt resistor value (units: u?)
    - ku. Voltage divider ratio.
    - st. Sensor Type. Refer to DRV_METROLOGY_SENSOR_TYPE.
    - gain. Programmable Gain Amplifier of the AFE (analog front end). Refer to DRV_METROLOGY_GAIN_TYPE.
*/
typedef struct {
    uint32_t mc;
    double freq;
    double tr;
    double rl;
    uint32_t ku;
    DRV_METROLOGY_SENSOR_TYPE st;
    DRV_METROLOGY_GAIN_TYPE gain;
} DRV_METROLOGY_CONFIGURATION;

/* METROLOGY Driver Status

  Summary:
    Defines the status of the DRV_METROLOGY driver.

  Description:
    This enumeration defines the status of the DRV_METROLOGY Driver:
        - DRV_METROLOGY_STATUS_UNINITIALIZED: Metrology driver has not been initialized.
        - DRV_METROLOGY_STATUS_READY: Metrology driver is ready to be used.
        - DRV_METROLOGY_STATUS_HALT: Metrology driver has been initialized but not opened.
        - DRV_METROLOGY_STATUS_WAITING_IPC: Metrology driver is waiting the init IPC interrupt
        from the metrology lib as part of the opening routine.
        - DRV_METROLOGY_STATUS_INIT_DSP: IPC interrupt has been triggered indicating that DSP
        filters has been stabilized to full accuracy.
        - DRV_METROLOGY_STATUS_RUNNING: Metrology library is running and periodic data
        acquisition is performed.

  Remarks:
    None.
*/

typedef enum
{
    DRV_METROLOGY_STATUS_UNINITIALIZED = SYS_STATUS_UNINITIALIZED,
    DRV_METROLOGY_STATUS_BUSY = SYS_STATUS_BUSY,
    DRV_METROLOGY_STATUS_READY = SYS_STATUS_READY,
    DRV_METROLOGY_STATUS_HALT = SYS_STATUS_READY_EXTENDED + 1U,
    DRV_METROLOGY_STATUS_WAITING_IPC = SYS_STATUS_READY_EXTENDED + 2U,
    DRV_METROLOGY_STATUS_INIT_DSP = SYS_STATUS_READY_EXTENDED + 3U,
    DRV_METROLOGY_STATUS_RUNNING = SYS_STATUS_READY_EXTENDED + 4U,
    DRV_METROLOGY_STATUS_ERROR = SYS_STATUS_ERROR,
} DRV_METROLOGY_STATUS;

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
} DRV_METROLOGY_INIT;

#ifdef __cplusplus
}
#endif

#endif // #ifndef DRV_METROLOGY_DEFINITIONS_H
/*******************************************************************************
 End of File
*/
