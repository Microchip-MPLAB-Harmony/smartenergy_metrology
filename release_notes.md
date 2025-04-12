![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# Microchip MPLAB® Harmony 3 Release Notes

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
