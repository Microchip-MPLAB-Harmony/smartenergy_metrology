![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# Microchip MPLAB® Harmony 3 Release Notes

## Smart Energy Metrology Release v1.1.0

### New Features

- **New Features and Enhancements**
  - Added Metrology related peripheral implementation.
    - Integrity Check Monitor (ICM). Calculates Hash and monitors the integrity of a user defined memory region.
    - Energy Metering Analog Front End (EMAFE). High resolution ADCs for Voltage and Current sensing, including Voltage reference and Temperature sensor.
  - Added Power Offset compensation to Energy and total Power calculations.
  - Enhanced Event capturing features.

- **Metrology Library Versions**
  - This release uses the following versions of the Metrology Library binaries:
    - Traditional Metrology Library v3.03.00
    - Multi-Channel Metrology Library v4.00.00

### Bug fixes

- Added missing fields and registers in Metrology Registers definitions.
- Corrected implementation of Backup Mode entering and associated wake-up.

### Metrology Driver API Changes

- Power Offset compensation implementation required the following changes in Driver API. In case you are using your own applications, update them accordingly (provided example apps are already updated).
  - DRV_METROLOGY_GetEnergyValue() function now returns int32_t value, instead of uint32_t.
  - energy field on DRV_METROLOGY_AFE_DATA structure is now int32_t, instead of uint32_t.
  - Following fields on DRV_METROLOGY_REGS_CONTROL structure are now int32_t, instead of uint32_t:
    - POWER_OFFSET_P, POWER_OFFSET_Q, POWER_OFFSET_S.
    - CAL_PH_IA, CAL_PH_VA, CAL_PH_IB, CAL_PH_VB, CAL_PH_IC, CAL_PH_VC, CAL_PH_IN.
    - POWER_OFFSET_P_A, POWER_OFFSET_P_B, POWER_OFFSET_P_C, POWER_OFFSET_Q_A, POWER_OFFSET_Q_B, POWER_OFFSET_Q_C.
  - Following fields on DRV_MCMETROLOGY_REGS_CONTROL structure are now int32_t, instead of uint32_t:
    - POWER_OFFSET_P, POWER_OFFSET_Q.
    - CAL_PH_CHx[].
    - POWER_OFFSET_P_x[], POWER_OFFSET_Q_x[].

### Known Issues

- None.

### Development Tools

- [MPLAB® X IDE v6.25](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v4.60](https://www.microchip.com/mplab/compilers)
- MPLAB® X IDE plug-ins:
  - MPLAB® Code Configurator 5.5.3 or higher
- PIC32CX-MT family (MCUs):
  - PIC32CX-MT DFP 1.3.132 or higher

### Notes

- Example Apps are no longer available in this repository. They have been moved to a dedicated repository smartenergy_metrology_apps.

## Smart Energy Metrology Release v1.1.0-E1

### New Features

- **Applications**

  - Demo Multi-channel Meter application is provided in this release. It allows the user to evaluate the multi channel features and to test the high accuracy that can be achieved with the Microchip Smart Metering solutions.

- **New Features and Enhancements**
  - Includes new multichannel metrology driver which provides a beta version of the multichannel metrology library.

### Bug fixes

- "plib_sefc_common.c" file is missed in the metrology apps of the Smart Energy Metrology Release v1.0.0.

### Known Issues

- Autocalibration is not supported in this version. It will be included in comming releases.

### Development Tools

- [MPLAB® X IDE v6.25](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v4.60](https://www.microchip.com/mplab/compilers)
- MPLAB® X IDE plug-ins:
  - MPLAB® Code Configurator 5.5.2 or higher
- PIC32CX-MT family (MCUs):
  - PIC32CX-MT DFP 1.3.132 or higher

### Notes

- None.

## Smart Energy Metrology Release v1.0.0

### New Features

- **New part support** - This release provides support for PIC32CX-MTSH/MTC Core 0 devices with functionality for metrology applications.

- **Applications**

  - Demo Meter application is provided in this release as an example of the most common functionalities included in an electricity meter. It allows the user to evaluate the features and to test the high accuracy that can be achieved with the Microchip Smart Metering solutions.

- **New Features and Enhancements**
  - Includes simultaneous multi-harmonic analysis of the first 31 harmonics.
  - Add Apparent Energy support to Metrology Pulses

- **Backup Mode Test**

  - When Backup mode is to be used with a battery, a line of code has to be modified in the *SUPC_BackupModeEnter* function, on the *Switch off voltage regulator* section, as seen on the following code block:
    ```c
    void SUPC_BackupModeEnter(void)
    {
        SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

        /* Switch off voltage regulator */
        //SUPC_REGS->SUPC_CR = SUPC_CR_KEY_PASSWD | SUPC_CR_VROFF_Msk;
        SUPC_REGS->SUPC_CR = SUPC_CR_KEY_PASSWD | SUPC_CR_SHDW_Msk;

        /* Enable CPU Interrupt */
        __DMB();
        __enable_irq();

        /* Enter Backup */
        __DSB();
        __WFI();
    }
    ```

### Bug fixes

- None.

### Known Issues

- None.

### Development Tools

- [MPLAB® X IDE v6.25](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v4.60](https://www.microchip.com/mplab/compilers)
- MPLAB® X IDE plug-ins:
  - MPLAB® Code Configurator 5.5.2 or higher
- PIC32CX-MT family (MCUs):
  - PIC32CX-MT DFP 1.3.132 or higher

### Notes

- This is the first Metrology Release in this smartenergy_metrology independent repository. Previous versions were released as part of generic smartenergy repository.

## Smart Energy Metrology Release v1.0.0-E1

### New Features

- **New part support** - This release introduces initial support for PIC32CX-MTSH/MTC Core 0 devices providing functionality for metrology and dual core applications.

- **Applications**

  - New Demo metering application has been introduced in this release as an example of the most common functionalities included in an electricity meter. It allows the user to evaluate the features and to test the high accuracy that can be achieved with the Microchip Smart Metering solutions.

- **New Features and Enhancements**
  - Updated and improved output format on HTML documentation.
  - Includes an engineering version of the simultaneous multi-harmonic analysis of the first 31 harmonics.

### Bug fixes

- None.

### Known Issues

- Some exceptions occur while executing dependency callback when MCC is started. These exceptions are related to some PLIB issues that will be fixed on next CSP release.

### Development Tools

- [MPLAB® X IDE v6.20](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v4.45](https://www.microchip.com/mplab/compilers)
- MPLAB® X IDE plug-ins:
  - MPLAB® Code Configurator 5.5.1 or higher
- PIC32CX-MT family (MCUs):
  - PIC32CX-MT DFP 1.3.132 or higher

### Notes

- None
