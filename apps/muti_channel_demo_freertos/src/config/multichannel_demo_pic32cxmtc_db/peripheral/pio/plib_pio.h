/*******************************************************************************
  PIO PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_pio.h

  Summary:
    PIO PLIB Header File

  Description:
    This library provides an interface to control and interact with Parallel
    Input/Output controller (PIO) module.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#ifndef PLIB_PIO_H
#define PLIB_PIO_H

#include "device.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data types and constants
// *****************************************************************************
// *****************************************************************************

/*******************Base address and group index of PIO groups ****************/
/*PIOA base address */
#define PIOA_REGS ((pio_group_registers_t*)(&(PIO0_REGS->PIO_GROUP[0])))

/*PIOA index */
#define PIO_PORT_A 0U

/*PIOB base address */
#define PIOB_REGS ((pio_group_registers_t*)(&(PIO0_REGS->PIO_GROUP[1])))

/*PIOB index */
#define PIO_PORT_B 1U

/*PIOC base address */
#define PIOC_REGS ((pio_group_registers_t*)(&(PIO0_REGS->PIO_GROUP[2])))

/*PIOC index */
#define PIO_PORT_C 2U

/*PIOD base address */
#define PIOD_REGS ((pio_group_registers_t*)(&(PIO1_REGS->PIO_GROUP[0])))

/*PIOD index */
#define PIO_PORT_D 3U

/*PIO max index */
#define PIO_PORT_MAX    4U


/*** Macros for SCRL_DOWN_BTN pin ***/
#define SCRL_DOWN_BTN_Set()               (PIOC_REGS->PIO_SODR = ((uint32_t)1U<<22U))
#define SCRL_DOWN_BTN_Clear()             (PIOC_REGS->PIO_CODR = ((uint32_t)1U<<22U))
#define SCRL_DOWN_BTN_Toggle()            do {\
                                            PIOC_REGS->PIO_MSKR = ((uint32_t)1U<<22U); \
                                            PIOC_REGS->PIO_ODSR ^= ((uint32_t)1U<<22U);\
                                        } while (0)
#define SCRL_DOWN_BTN_OutputEnable()      do {\
                                            PIOC_REGS->PIO_MSKR = ((uint32_t)1U<<22U); \
                                            PIOC_REGS->PIO_CFGR |=((uint32_t)1U << PIO_CFGR_DIR_Pos);\
                                        }while(0)
#define SCRL_DOWN_BTN_InputEnable()       do { \
                                            PIOC_REGS->PIO_MSKR = ((uint32_t)1U<<22U); \
                                            PIOC_REGS->PIO_CFGR &= ~((uint32_t)1U << PIO_CFGR_DIR_Pos);\
                                        } while (0)
#define SCRL_DOWN_BTN_Get()               ((PIOC_REGS->PIO_PDSR >> 22U) & 0x1U)
#define SCRL_DOWN_BTN_PIN                  PIO_PIN_PC22
#define SCRL_DOWN_BTN_InterruptEnable()   (PIOC_REGS->PIO_IER = (1<<22))
#define SCRL_DOWN_BTN_InterruptDisable()  (PIOC_REGS->PIO_IDR = (1<<22))

/*** Macros for RED_LED_PD17 pin ***/
#define RED_LED_PD17_Set()               (PIOD_REGS->PIO_SODR = ((uint32_t)1U<<17U))
#define RED_LED_PD17_Clear()             (PIOD_REGS->PIO_CODR = ((uint32_t)1U<<17U))
#define RED_LED_PD17_Toggle()            do {\
                                            PIOD_REGS->PIO_MSKR = ((uint32_t)1U<<17U); \
                                            PIOD_REGS->PIO_ODSR ^= ((uint32_t)1U<<17U);\
                                        } while (0)
#define RED_LED_PD17_OutputEnable()      do {\
                                            PIOD_REGS->PIO_MSKR = ((uint32_t)1U<<17U); \
                                            PIOD_REGS->PIO_CFGR |=((uint32_t)1U << PIO_CFGR_DIR_Pos);\
                                        }while(0)
#define RED_LED_PD17_InputEnable()       do { \
                                            PIOD_REGS->PIO_MSKR = ((uint32_t)1U<<17U); \
                                            PIOD_REGS->PIO_CFGR &= ~((uint32_t)1U << PIO_CFGR_DIR_Pos);\
                                        } while (0)
#define RED_LED_PD17_Get()               ((PIOD_REGS->PIO_PDSR >> 17U) & 0x1U)
#define RED_LED_PD17_PIN                  PIO_PIN_PD17

/*** Macros for RED_LED_PD18 pin ***/
#define RED_LED_PD18_Set()               (PIOD_REGS->PIO_SODR = ((uint32_t)1U<<18U))
#define RED_LED_PD18_Clear()             (PIOD_REGS->PIO_CODR = ((uint32_t)1U<<18U))
#define RED_LED_PD18_Toggle()            do {\
                                            PIOD_REGS->PIO_MSKR = ((uint32_t)1U<<18U); \
                                            PIOD_REGS->PIO_ODSR ^= ((uint32_t)1U<<18U);\
                                        } while (0)
#define RED_LED_PD18_OutputEnable()      do {\
                                            PIOD_REGS->PIO_MSKR = ((uint32_t)1U<<18U); \
                                            PIOD_REGS->PIO_CFGR |=((uint32_t)1U << PIO_CFGR_DIR_Pos);\
                                        }while(0)
#define RED_LED_PD18_InputEnable()       do { \
                                            PIOD_REGS->PIO_MSKR = ((uint32_t)1U<<18U); \
                                            PIOD_REGS->PIO_CFGR &= ~((uint32_t)1U << PIO_CFGR_DIR_Pos);\
                                        } while (0)
#define RED_LED_PD18_Get()               ((PIOD_REGS->PIO_PDSR >> 18U) & 0x1U)
#define RED_LED_PD18_PIN                  PIO_PIN_PD18

/*** Macros for DBG_UART_TX pin ***/
#define DBG_UART_TX_Get()               ((PIOA_REGS->PIO_PDSR >> 4U) & 0x1U)
#define DBG_UART_TX_PIN                  PIO_PIN_PA4

/*** Macros for DBG_UART_RX pin ***/
#define DBG_UART_RX_Get()               ((PIOA_REGS->PIO_PDSR >> 5U) & 0x1U)
#define DBG_UART_RX_PIN                  PIO_PIN_PA5

/*** Macros for SCRL_UP_BTN pin ***/
#define SCRL_UP_BTN_Set()               (PIOC_REGS->PIO_SODR = ((uint32_t)1U<<4U))
#define SCRL_UP_BTN_Clear()             (PIOC_REGS->PIO_CODR = ((uint32_t)1U<<4U))
#define SCRL_UP_BTN_Toggle()            do {\
                                            PIOC_REGS->PIO_MSKR = ((uint32_t)1U<<4U); \
                                            PIOC_REGS->PIO_ODSR ^= ((uint32_t)1U<<4U);\
                                        } while (0)
#define SCRL_UP_BTN_OutputEnable()      do {\
                                            PIOC_REGS->PIO_MSKR = ((uint32_t)1U<<4U); \
                                            PIOC_REGS->PIO_CFGR |=((uint32_t)1U << PIO_CFGR_DIR_Pos);\
                                        }while(0)
#define SCRL_UP_BTN_InputEnable()       do { \
                                            PIOC_REGS->PIO_MSKR = ((uint32_t)1U<<4U); \
                                            PIOC_REGS->PIO_CFGR &= ~((uint32_t)1U << PIO_CFGR_DIR_Pos);\
                                        } while (0)
#define SCRL_UP_BTN_Get()               ((PIOC_REGS->PIO_PDSR >> 4U) & 0x1U)
#define SCRL_UP_BTN_PIN                  PIO_PIN_PC4
#define SCRL_UP_BTN_InterruptEnable()   (PIOC_REGS->PIO_IER = (1<<4))
#define SCRL_UP_BTN_InterruptDisable()  (PIOC_REGS->PIO_IDR = (1<<4))
// *****************************************************************************
/* PIO Ports

  Summary:
    Datatype for PIO ports
*/
typedef uint32_t PIO_PORT;


// *****************************************************************************
/* PIO Port Pins

  Summary:
    Identifies the available PIO port pins.

  Description:
    This enumeration identifies the available PIO port pins.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all pins are available on all devices.  Refer to the specific
    device data sheet to determine which pins are supported.
*/


#define    PIO_PIN_PA0    (0U)
#define    PIO_PIN_PA1    (1U)
#define    PIO_PIN_PA2    (2U)
#define    PIO_PIN_PA3    (3U)
#define    PIO_PIN_PA4    (4U)
#define    PIO_PIN_PA5    (5U)
#define    PIO_PIN_PA6    (6U)
#define    PIO_PIN_PA7    (7U)
#define    PIO_PIN_PA8    (8U)
#define    PIO_PIN_PA9    (9U)
#define    PIO_PIN_PA10    (10U)
#define    PIO_PIN_PA11    (11U)
#define    PIO_PIN_PA12    (12U)
#define    PIO_PIN_PA13    (13U)
#define    PIO_PIN_PA14    (14U)
#define    PIO_PIN_PA15    (15U)
#define    PIO_PIN_PA16    (16U)
#define    PIO_PIN_PA17    (17U)
#define    PIO_PIN_PA18    (18U)
#define    PIO_PIN_PA19    (19U)
#define    PIO_PIN_PA20    (20U)
#define    PIO_PIN_PA21    (21U)
#define    PIO_PIN_PA22    (22U)
#define    PIO_PIN_PA23    (23U)
#define    PIO_PIN_PA24    (24U)
#define    PIO_PIN_PA25    (25U)
#define    PIO_PIN_PA26    (26U)
#define    PIO_PIN_PA27    (27U)
#define    PIO_PIN_PA28    (28U)
#define    PIO_PIN_PA29    (29U)
#define    PIO_PIN_PA30    (30U)
#define    PIO_PIN_PA31    (31U)
#define    PIO_PIN_PB0   (32U)
#define    PIO_PIN_PB1   (33U)
#define    PIO_PIN_PB2   (34U)
#define    PIO_PIN_PB3   (35U)
#define    PIO_PIN_PB4   (36U)
#define    PIO_PIN_PB5   (37U)
#define    PIO_PIN_PB6   (38U)
#define    PIO_PIN_PB7   (39U)
#define    PIO_PIN_PB8   (40U)
#define    PIO_PIN_PB9   (41U)
#define    PIO_PIN_PB10   (42U)
#define    PIO_PIN_PB11   (43U)
#define    PIO_PIN_PB12   (44U)
#define    PIO_PIN_PB13   (45U)
#define    PIO_PIN_PB14   (46U)
#define    PIO_PIN_PB15   (47U)
#define    PIO_PIN_PB16   (48U)
#define    PIO_PIN_PB17   (49U)
#define    PIO_PIN_PB18   (50U)
#define    PIO_PIN_PB19   (51U)
#define    PIO_PIN_PB20   (52U)
#define    PIO_PIN_PB21   (53U)
#define    PIO_PIN_PB22   (54U)
#define    PIO_PIN_PB23   (55U)
#define    PIO_PIN_PB24   (56U)
#define    PIO_PIN_PB25   (57U)
#define    PIO_PIN_PB26   (58U)
#define    PIO_PIN_PC0   (64U)
#define    PIO_PIN_PC1   (65U)
#define    PIO_PIN_PC2   (66U)
#define    PIO_PIN_PC3   (67U)
#define    PIO_PIN_PC4   (68U)
#define    PIO_PIN_PC5   (69U)
#define    PIO_PIN_PC6   (70U)
#define    PIO_PIN_PC7   (71U)
#define    PIO_PIN_PC8   (72U)
#define    PIO_PIN_PC9   (73U)
#define    PIO_PIN_PC10   (74U)
#define    PIO_PIN_PC11   (75U)
#define    PIO_PIN_PC12   (76U)
#define    PIO_PIN_PC13   (77U)
#define    PIO_PIN_PC14   (78U)
#define    PIO_PIN_PC15   (79U)
#define    PIO_PIN_PC16   (80U)
#define    PIO_PIN_PC17   (81U)
#define    PIO_PIN_PC18   (82U)
#define    PIO_PIN_PC19   (83U)
#define    PIO_PIN_PC20   (84U)
#define    PIO_PIN_PC21   (85U)
#define    PIO_PIN_PC22   (86U)
#define    PIO_PIN_PD0   (96U)
#define    PIO_PIN_PD1   (97U)
#define    PIO_PIN_PD2   (98U)
#define    PIO_PIN_PD3   (99U)
#define    PIO_PIN_PD4   (100U)
#define    PIO_PIN_PD5   (101U)
#define    PIO_PIN_PD6   (102U)
#define    PIO_PIN_PD7   (103U)
#define    PIO_PIN_PD8   (104U)
#define    PIO_PIN_PD9   (105U)
#define    PIO_PIN_PD10   (106U)
#define    PIO_PIN_PD11   (107U)
#define    PIO_PIN_PD12   (108U)
#define    PIO_PIN_PD13   (109U)
#define    PIO_PIN_PD14   (110U)
#define    PIO_PIN_PD15   (111U)
#define    PIO_PIN_PD16   (112U)
#define    PIO_PIN_PD17   (113U)
#define    PIO_PIN_PD18   (114U)
#define    PIO_PIN_PD19   (115U)

/* This element should not be used in any of the PIO APIs.
   It will be used by other modules or application to denote that none of the PIO Pin is used */
#define     PIO_PIN_NONE        (-1)

typedef uint8_t PIO_PIN;

// *****************************************************************************
/* PIO Pin-Event Handler Function Pointer

   Summary:
    Pointer to a PIO Pin-Event handler function.

   Description:
    This data type defines the required function signature for the
    PIO pin-event handling callback function.  The client must register
    a pointer to an event handling function whose function signature (parameter
    and return value types) match the types specified by this function pointer
    in order to receive calls back from the PLIB when a configured pin event
    occurs.

    The parameters and return values are described here and a partial example
    implementation is provided.

  Parameters:
    context         - Value identifying the context of the client that
                      registered the event handling function

  Returns:
    None.

  Example:
    A function matching this signature:
    <code>
    void APP_PinEventHandler(PIN_NAME pin, uintptr_t context)
    {

    }
    </code>
    Is registered as follows:
    <code>
    PIO_PinInterruptCallbackRegister(PIO_PIN_PA5, &APP_PinEventHandler, NULL);
    </code>
    <code>

  Remarks:
    The context parameter contains the a handle to the client context,
    provided at the time the event handling function was  registered using the
    PIO_PinInterruptCallbackRegister function. This context handle value is
    passed back to the client as the "context" parameter.  It can be any value
    (such as a pointer to the client's data) necessary to identify the client
    context.

    The event handler function executes in the PLIB's interrupt
    context. It is recommended of the application to not perform process
    intensive or blocking operations with in this function.
*/
typedef  void (*PIO_PIN_CALLBACK) ( PIO_PIN pin, uintptr_t context);
// *****************************************************************************
// *****************************************************************************
// Section: Generated API based on pin configurations done in Pin Manager
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void PIO_Initialize(void)

  Summary:
    Initialize the PIO library.

  Description:
    This function initializes the PIO library and all ports and pins configured
    in the MCC Pin Manager.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>

    PIO_Initialize();

    </code>

  Remarks:
    None.
*/
void PIO_Initialize(void);

// *****************************************************************************
// *****************************************************************************
// Section: PIO Functions which operates on multiple pins of a port
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    uint32_t PIO_PortRead(PIO_PORT port)

  Summary:
    Read all the I/O lines of the selected port port.

  Description:
    This function reads the live data values on all the I/O lines of the
    selected port.  Bit values returned in each position indicate corresponding
    pin levels.
    1 = Pin is high.
    0 = Pin is low.
    This function reads the value regardless of pin configuration, whether it is
    set as as an input, driven by the PIO Controller, or driven by a peripheral.

  Precondition:
    Reading the I/O line levels requires the clock of the PIO Controller to be
    enabled, otherwise this API reads the levels present on the I/O line at the
    time the clock was disabled.

  Parameters:
    port       - One of the IO ports from the enum PIO_PORT

  Returns:
    Returns the read value of all the I/O lines of the selected port port.

  Example:
    <code>

    uint32_t value;
    value = PIO_PortRead(PIO_PORT_C);

    </code>

  Remarks:
    If the port has less than 32-bits, unimplemented pins will read as
    low (0).
    Implemented pins are Right aligned in the 32-bit return value.
*/
uint32_t PIO_PortRead(PIO_PORT port);

// *****************************************************************************
/* Function:
    void PIO_PortWrite(PIO_PORT port, uint32_t mask, uint32_t value);

  Summary:
    Write the value on the masked I/O lines of the selected port.

  Description:
    This function writes the data values driven on selected output lines of the
    selected port.  Bit values in each position indicate corresponding pin
    levels.
    1 = Pin is driven high.
    0 = Pin is driven low.

  Precondition:
    The desired pins lines of the selected port must be setup as output(s).

  Parameters:
    port       - One of the IO ports from the enum PIO_PORT
    mask       - A 32 bit value in which positions of 0s and 1s decide
                 which IO pins of the selected port will be written.
                 1's - Will write to corresponding IO pins.
                 0's - Will remain unchanged.
    value      - Value which has to be written/driven on the I/O
                 lines of the selected port for which mask bits are '1'.
                 Values for the corresponding mask bit '0' will be ignored.
  Returns:
    None.

  Example:
    <code>

    PIO_PortWrite(PIO_PORT_C, 0x0F, 0xF563D453);

    </code>

  Remarks:
    If the port has less than 32-bits, unimplemented pins will be ignored.

    Implemented pins are Right aligned in the 32-bit value.
*/
void PIO_PortWrite(PIO_PORT port, uint32_t mask, uint32_t value);

// *****************************************************************************
/* Function:
    uint32_t PIO_PortLatchRead ( PIO_PORT port )

  Summary:
    Read the latched value on all the I/O lines of the selected port.

  Description:
    This function reads the latched data values on all the I/O lines of the
    selected port.  Bit values returned in each position indicate corresponding
    pin levels.
    1 = Pin is high.
    0 = Pin is low.

  Precondition:
    None.

  Parameters:
    port       - One of the IO ports from the enum PIO_PORT

  Returns:
    Returns the data driven on all the I/O lines of the selected port.

  Example:
    <code>

    uint32_t value;
    value = PIO_PortLatchRead(PIO_PORT_C);

    </code>

  Remarks:
    If the port has less than 32-bits, unimplemented pins will read as
    low (0).
    Implemented pins are Right aligned in the 32-bit return value.
*/
uint32_t PIO_PortLatchRead ( PIO_PORT port );

// *****************************************************************************
/* Function:
    void PIO_PortSet(PIO_PORT port, uint32_t mask)

  Summary:
    Set the selected IO pins of a port.

  Description:
    This function sets (to '1') the selected IO pins of a port.

  Precondition:
    None.

  Parameters:
    port       - One of the IO ports from the enum PIO_PORT
    mask       - A 32 bit value in which positions of 0s and 1s decide
                 which IO pins of the selected port will be set.
                 1's - Will set corresponding IO pins to high (to 1).
                 0's - Will remain unchanged.
  Returns:
    None.

  Example:
    <code>

    PIO_PortSet(PIO_PORT_C, 0x00A0);

    </code>

  Remarks:
    If the port has less than 32-bits, unimplemented pins will be ignored.

    Implemented pins are Right aligned in the 32-bit value.
*/
void PIO_PortSet(PIO_PORT port, uint32_t mask);

// *****************************************************************************
/* Function:
    void PIO_PortClear(PIO_PORT port, uint32_t mask)

  Summary:
    Clear the selected IO pins of a port.

  Description:
    This function clears (to '0') the selected IO pins of a port.

  Precondition:
    None.

  Parameters:
    port       - One of the IO ports from the enum PIO_PORT
    mask       - A 32 bit value in which positions of 0s and 1s decide
                 which IO pins of the selected port will be cleared.
                 1's - Will clear corresponding IO pins to low (to 0).
                 0's - Will remain unchanged.
  Returns:
    None.

  Example:
    <code>

    PIO_PortClear(PIO_PORT_C, 0x00A0);

    </code>

  Remarks:
    If the port has less than 32-bits, unimplemented pins will be ignored.

    Implemented pins are Right aligned in the 32-bit value.
*/
void PIO_PortClear(PIO_PORT port, uint32_t mask);

// *****************************************************************************
/* Function:
    void PIO_PortToggle(PIO_PORT port, uint32_t mask)

  Summary:
    Toggles the selected IO pins of a port.

  Description:
    This function toggles (or invert) the selected IO pins of a port.

  Precondition:
    None.

  Parameters:
    port       - One of the IO ports from the enum PIO_PORT
    mask       - A 32 bit value in which positions of 0s and 1s decide
                 which IO pins of the selected port will be toggled.
                 1's - Will toggle (invert) corresponding IO pins.
                 0's - Will remain unchanged.
  Returns:
    None.

  Example:
    <code>

<<<<<<< HEAD
=======
     Toggles PC5 and PC7 pins
>>>>>>> d3342b368 ([PIO] pio_11264 MISRAC 2012 required rule violation fix)
    PIO_PortToggle(PIO_PORT_C, 0x00A0);

    </code>

  Remarks:
    If the port has less than 32-bits, unimplemented pins will be ignored.

    Implemented pins are Right aligned in the 32-bit value.
*/
void PIO_PortToggle(PIO_PORT port, uint32_t mask);

// *****************************************************************************
/* Function:
    void PIO_PortInputEnable(PIO_PORT port, uint32_t mask)

  Summary:
    Enables selected IO pins of a port as input.

  Description:
    This function enables selected IO pins of a port as input.

  Precondition:
    None.

  Parameters:
    port          - One or more of the of the IO ports from the enum PIO_PORT.
    mask          - A 32 bit value in which positions of 0s and 1s decide
                    which IO pins of the selected port will be setup as inputs.
                    1's - Will set corresponding IO pins as input(s).
                    0's - Will cause the direction of the corresponding IO pins
                          to remain unchanged.
  Returns:
    None.

  Example:
    <code>

    PIO_PortInputEnable(PIO_PORT_C, 0x00A0);

    </code>

  Remarks:
    None.
*/
void PIO_PortInputEnable(PIO_PORT port, uint32_t mask);

// *****************************************************************************
/* Function:
    void PIO_PortOutputEnable(PIO_PORT port, uint32_t mask)

  Summary:
    Enables selected IO pins of a port as output(s).

  Description:
    This function enables selected IO pins of the given port as output(s).

  Precondition:
    None.

  Parameters:
    port       - One of the IO ports from the enum PIO_PORT
    mask       - A 32 bit value in which positions of 0s and 1s decide
                 which IO pins of the selected port will be setup as outputs.
                 1's - Will set corresponding IO pins as output(s).
                 0's - Will cause the direction of the corresponding IO pins
                       to remain unchanged.
  Returns:
    None.

  Example:
    <code>

    PIO_PortOutputEnable(PIO_PORT_C, 0x00A0);

    </code>

  Remarks:
    None.
*/
void PIO_PortOutputEnable(PIO_PORT port, uint32_t mask);

// *****************************************************************************
/* Function:
    void PIO_PortInterruptEnable(PIO_PORT port, uint32_t mask)

  Summary:
    Enables IO interrupt on selected IO pins of a port.

  Description:
    This function enables interrupt on selected IO pins of selected port.

  Precondition:
    None.

  Parameters:
    port       - One of the IO ports from the enum PIO_PORT

    mask       - Is a 32 bit value in which positions of 0s and 1s decide
                 which IO pins of the selected port will have interrupt
                 enabled.  The bit positions of mask value which are set as 1,
                 IO interrupt of corresponding IO pin of the selected port
                 will be enabled.  The bit positions of mask value which are
                 cleared to 0, IO interrupt of corresponding IO pin of the
                 selected port will remain unchanged.

  Returns:
    None.

  Example:
    <code>

    PIO_PortInterruptEnable(PIO_PORT_C, 0x00A0);

    </code>

  Remarks:
    None.
*/
void PIO_PortInterruptEnable(PIO_PORT port, uint32_t mask);

// *****************************************************************************
/* Function:
    void PIO_PortInterruptDisable(PIO_PORT port, uint32_t mask)

  Summary:
    Disables IO interrupt on selected IO pins of a port.

  Description:
    This function disables IO interrupt on selected IO pins of selected port.

  Precondition:
    None.

  Parameters:
    port       - One of the IO ports from the enum PIO_PORT
    mask       - Is a 32 bit value in which positions of 0s and 1s decide
                 which IO pins of the selected port will have interrupt
                 disabled.  The bit positions of mask value which are set as 1,
                 IO interrupt of corresponding IO pin of the selected port
                 will be disabled.  The bit positions of mask value which are
                 cleared to 0, IO interrupt of corresponding IO pin of the
                 selected port will remain unchanged.
  Returns:
    None.

  Example:
    <code>

    PIO_PortInterruptDisable(PIO_PORT_C, 0x0202);

    </code>

  Remarks:
    None.
*/
void PIO_PortInterruptDisable(PIO_PORT port, uint32_t mask);

// *****************************************************************************
// *****************************************************************************
// Section: Local Data types and Prototypes
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* PIO Pin Callback Object

  Summary:
    Structure to hold callback related details

  Description:
    This structure is used internally by the PIO PLIB to hold pin specific
    callback details.

  Remarks:
    None.
*/
typedef struct {

    /* target pin */
    PIO_PIN                 pin;

    /* Callback for event on target pin*/
    PIO_PIN_CALLBACK        callback;

    /* Callback Context */
    uintptr_t               context;

} PIO_PIN_CALLBACK_OBJ;


// *****************************************************************************
// *****************************************************************************
// Section: PIO Functions which operates on one pin at a time
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void PIO_PinWrite(PIO_PIN pin, bool value)

  Summary:
    Writes the selected pin.

  Description:
    This function writes/drives the "value" on the selected I/O line/pin.

  Precondition:
    None.

  Parameters:
    pin       - One of the IO pins from the enum PIO_PIN
    value     - value to be written on the selected pin:
                true  = set pin to high (1).
                false = clear pin to low (0).

  Returns:
    None.

  Example:
    <code>
    PIO_PinWrite(PIO_PIN_PB3, true);
    </code>

  Remarks:
    None.
*/
static inline void PIO_PinWrite(PIO_PIN pin, bool value)
{
    uint32_t port = (uint32_t)pin >> 5U;
    uint32_t mask = 1UL << ((uint32_t)pin & 0x1FU);
    PIO_PortWrite(port, mask, value ? mask : 0U);
}

// *****************************************************************************
/* Function:
    bool PIO_PinRead(PIO_PIN pin)

  Summary:
    Read the selected pin value.

  Description:
    This function reads the selected pin value.
    it reads the value regardless of pin configuration, whether uniquely as an
    input, or driven by the PIO Controller, or driven by peripheral.

  Precondition:
    Reading the I/O line levels requires the clock of the PIO Controller to be
    enabled, otherwise this API reads the levels present on the I/O line at the
    time the clock was disabled.

  Parameters:
    pin       - One of the IO pins from the enum PIO_PIN

  Returns:
    Returns the read value of the selected I/O pin.

  Example:
    <code>

    bool value;
    value = PIO_PinRead(PIO_PIN_PB3);

    </code>

  Remarks:
       To read the latched value on this pin, PIO_PinLatchRead API should be used.
*/

static inline bool PIO_PinRead(PIO_PIN pin)
{
    uint32_t port = (uint32_t)pin >> 5U;
    uint32_t mask = 1UL << ((uint32_t)pin & 0x1FU);
    return (((PIO_PortRead(port) & mask) != 0U) ? true : false);
}


// *****************************************************************************
/* Function:
    bool PIO_PinLatchRead ( PIO_PIN pin )

  Summary:
    Read the value driven on the selected pin.

  Description:
    This function reads the data driven on the selected I/O line/pin.
    Whatever data is written/driven on I/O line by using any of the PIO PLIB
    APIs, will be read by this API.

  Precondition:
    None.

  Parameters:
    pin       - One of the IO pins from the enum PIO_PIN

  Returns:
    Returns the value driven on the selected I/O pin.

  Example:
    <code>

    bool value;
    value = PIO_PinLatchRead(PIO_PIN_PB3);

    </code>

  Remarks:
    To read actual pin value, PIO_PinRead API should be used.
*/
static inline bool PIO_PinLatchRead(PIO_PIN pin)
{
    uint32_t port = (uint32_t)pin >> 5U;
    uint32_t mask = 1UL << ((uint32_t)pin & 0x1FU);
    return (((PIO_PortLatchRead(port) & mask) != 0U) ? true : false);
}

// *****************************************************************************
/* Function:
    void PIO_PinToggle(PIO_PIN pin)

  Summary:
    Toggles the selected pin.

  Description:
    This function toggles/inverts the value on the selected I/O line/pin.

  Precondition:
    None.

  Parameters:
    pin       - One of the IO pins from the enum PIO_PIN

  Returns:
    None.

  Example:
    <code>

    PIO_PinToggle(PIO_PIN_PB3);

    </code>

  Remarks:
    None.
*/
static inline void PIO_PinToggle(PIO_PIN pin)
{
    uint32_t port = (uint32_t)pin >> 5U;
    uint32_t mask = 1UL << ((uint32_t)pin & 0x1FU);
    PIO_PortToggle(port, mask);
}

// *****************************************************************************
/* Function:
    void PIO_PinSet(PIO_PIN pin)

  Summary:
    Sets the selected pin.

  Description:
    This function drives '1' on the selected I/O line/pin.

  Precondition:
    None.

  Parameters:
    pin       - One of the IO pins from the enum PIO_PIN

  Returns:
    None.

  Example:
    <code>

    PIO_PinSet(PIO_PIN_PB3);

    </code>

  Remarks:
    None.
*/
static inline void PIO_PinSet(PIO_PIN pin)
{
    uint32_t port = (uint32_t)pin >> 5U;
    uint32_t mask = 1UL << ((uint32_t)pin & 0x1FU);
    PIO_PortSet(port, mask);
}

// *****************************************************************************
/* Function:
    void PIO_PinClear(PIO_PIN pin)

  Summary:
    Clears the selected pin.

  Description:
    This function drives '0' on the selected I/O line/pin.

  Precondition:
    None.

  Parameters:
    pin       - One of the IO pins from the enum PIO_PIN

  Returns:
    None.

  Example:
    <code>

    PIO_PinClear(PIO_PIN_PB3);

    </code>

  Remarks:
    None.
*/
static inline void PIO_PinClear(PIO_PIN pin)
{
    uint32_t port = (uint32_t)pin >> 5U;
    uint32_t mask = 1UL << ((uint32_t)pin & 0x1FU);
    PIO_PortClear(port, mask);
}

// *****************************************************************************
/* Function:
    void PIO_PinInputEnable(PIO_PIN pin)

  Summary:
    Enables selected IO pin as input.

  Description:
    This function enables selected IO pin as input.

  Precondition:
    None.

  Parameters:
    pin       - One of the IO pins from the enum PIO_PIN

  Returns:
    None.

  Example:
    <code>

    PIO_PinInputEnable(PIO_PIN_PB3);

    </code>

  Remarks:
    None.
*/
static inline void PIO_PinInputEnable(PIO_PIN pin)
{
    uint32_t port = (uint32_t)pin >> 5U;
    uint32_t mask = 1UL << ((uint32_t)pin & 0x1FU);
    PIO_PortInputEnable(port, mask);
}

// *****************************************************************************
/* Function:
    void PIO_PinOutputEnable(PIO_PIN pin)

  Summary:
    Enables selected IO pin as output.

  Description:
    This function enables selected IO pin as output.

  Precondition:
    None.

  Parameters:
    pin       - One of the IO pins from the enum PIO_PIN

  Returns:
    None.

  Example:
    <code>

    PIO_PinOutputEnable(PIO_PIN_PB3);

    </code>

  Remarks:
    None.
*/
static inline void PIO_PinOutputEnable(PIO_PIN pin)
{
    uint32_t port = (uint32_t)pin >> 5U;
    uint32_t mask = 1UL << ((uint32_t)pin & 0x1FU);
    PIO_PortOutputEnable(port, mask);
}

// *****************************************************************************
/* Function:
    void PIO_PinInterruptEnable(PIO_PIN pin)

  Summary:
    Enables IO interrupt on selected IO pin.

  Description:
    This function enables interrupt on selected IO pin.

  Precondition:
    None.

  Parameters:
    pin           - One of the IO pins from the enum PIO_PIN

  Returns:
    None.

  Example:
    <code>

    PIO_PinInterruptEnable(PIO_PIN_PB3);

    </code>

  Remarks:
    None.
*/
static inline void PIO_PinInterruptEnable(PIO_PIN pin)
{
    uint32_t port = (uint32_t)pin >> 5U;
    uint32_t mask = 1UL << ((uint32_t)pin & 0x1FU);
    PIO_PortInterruptEnable(port, mask);
}


// *****************************************************************************
/* Function:
    void PIO_PinInterruptDisable(PIO_PIN pin)

  Summary:
    Disables IO interrupt on selected IO pin.

  Description:
    This function disables IO interrupt on selected IO pin.

  Precondition:
    None.

  Parameters:
    pin       - One of the IO pins from the enum PIO_PIN

  Returns:
    None.

  Example:
    <code>

    PIO_PinInterruptDisable(PIO_PIN_PB3);

    </code>

  Remarks:
    None.
*/
static inline void PIO_PinInterruptDisable(PIO_PIN pin)
{
    uint32_t port = (uint32_t)pin >> 5U;
    uint32_t mask = 1UL << ((uint32_t)pin & 0x1FU);
    PIO_PortInterruptDisable(port, mask);
}

// *****************************************************************************
/* Function:
    bool PIO_PinInterruptCallbackRegister(
        PIO_PIN pin,
        const PIO_PIN_CALLBACK callBack,
        uintptr_t context
    );

  Summary:
    Allows application to register callback for every pin.

  Description:
    This function allows application to register an event handling function
    for the PLIB to call back when I/O interrupt occurs on the selected pin.

    At any point if application wants to stop the callback, it can call this
    function with "eventHandler" value as NULL.

    If a pin is not configured for interrupt in Pin Manager and yet its callback
    registration is attempted using this API, then registration doesn't happen
    and API returns false indicating the same.

  Precondition:
    The PIO_Initialize function must have been called.

  Parameters:
    pin          - One of the IO pins from the enum PIO_PIN
    eventHandler - Pointer to the event handler function implemented by the user

    context      - The value of parameter will be passed back to the application
                   unchanged, when the eventHandler function is called. It can
                   be used to identify any application specific value.

  Returns:
    Callback registration status:
    - true: Callback was successfully registered
    - false: Callback was not registered

  Example:
    <code>

    PIO_PinInterruptCallbackRegister(PIO_PIN_PB3, &APP_PinHandler, NULL);

    </code>

  Remarks:
    None.
*/
bool PIO_PinInterruptCallbackRegister(
    PIO_PIN pin,
    const PIO_PIN_CALLBACK callback,
    uintptr_t context
);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

}

#endif
// DOM-IGNORE-END
#endif // PLIB_PIO_H
