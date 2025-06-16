# coding: utf-8
"""*****************************************************************************
* Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries.
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
*****************************************************************************"""

################################################################################
#### Global Variables ####
################################################################################
global emafeInstanceName

plib_emafe_helpkeyword = "SE_plib_emafe_config"

################################################################################
#### Business Logic ####
################################################################################
def showSymbol(symbol, event):
    symbol.setVisible(event["value"])

def updateValue(symbol, event):
    symbol.setValue(event["value"])

def showPCK2Warning(symbol, event):
    symbol.setVisible(event["value"] != 4096000)

############################################################################
#### Code Generation ####
############################################################################
def instantiateComponent(emafeComponent):
    global emafeInstanceName
    configName = Variables.get("__CONFIGURATION_NAME")

    emafeInstanceName = emafeComponent.createStringSymbol("EMAFE_INSTANCE_NAME", None)
    emafeInstanceName.setVisible(False)
    emafeInstanceName.setDefaultValue(emafeComponent.getID().upper())

    Log.writeInfoMessage("Loading EMAFE Peripheral library: {}".format(emafeInstanceName.getValue()))

    # Enable clock for EMAFE
    Database.setSymbolValue("core", emafeInstanceName.getValue() + "_CLOCK_ENABLE", True, 1)

    emafeSourceClock = emafeComponent.createIntegerSymbol("EMAFE_MCSPI_MASTER_CLOCK", None)
    emafeSourceClock.setLabel("MCSPI source clock frequency (Hz)")
    emafeSourceClock.setDefaultValue(0)
    emafeSourceClock.setReadOnly(True)
    emafeSourceClock.setDependencies(updateValue, ["mcspi.MCSPI_MASTER_CLOCK"])
    emafeSourceClock.setHelp(plib_emafe_helpkeyword)

    emafePCK2Clock = emafeComponent.createIntegerSymbol("EMAFE_PCK2_CLOCK", None)
    emafePCK2Clock.setLabel("PCK2 frequency (Hz)")
    pck2Freq = Database.getSymbolValue("core", "PCK2_FREQUENCY")
    if pck2Freq != None:
        emafePCK2Clock.setDefaultValue(Database.getSymbolValue("core", "PCK2_FREQUENCY"))
    else:
        emafePCK2Clock.setDefaultValue(0)
    emafePCK2Clock.setReadOnly(True)
    emafePCK2Clock.setDependencies(updateValue, ["core.PCK2_FREQUENCY"])
    emafePCK2Clock.setHelp(plib_emafe_helpkeyword)
    
    emafePCK2Warning = emafeComponent.createCommentSymbol("EMAFE_PCK2_CLOCK_WARN", emafePCK2Clock)
    emafePCK2Warning.setLabel("Warning!!! PCK2 must be set to 4096000 Hz. Review configuration in Clock Manager.")
    emafePCK2Warning.setVisible(pck2Freq != 4096000)
    emafePCK2Warning.setDependencies(showPCK2Warning, ["EMAFE_PCK2_CLOCK"])
    emafePCK2Warning.setHelp(plib_emafe_helpkeyword)

    emafeLPFIFDMAEnable = emafeComponent.createBooleanSymbol("EMAFE_LPFIF_DMA_ENABLE", None)
    emafeLPFIFDMAEnable.setLabel("Enable DMA")
    emafeLPFIFDMAEnable.setDefaultValue(1)
    emafeLPFIFDMAEnable.setHelp(plib_emafe_helpkeyword)

    # IDs used in NVIC Manager
    Database.setSymbolValue("core", "EMAFE_DATA_INTERRUPT_ENABLE", True, 1)
    Database.setSymbolValue("core", "EMAFE_DATA_INTERRUPT_HANDLER", "EMAFE_DATA_InterruptHandler", 1)
    Database.setSymbolValue("core", "EMAFE_DATA_INTERRUPT_HANDLER_LOCK", True, 1)

    ##########################################################################################################
    # GENERATION FILES
    ##########################################################################################################
    emafePlibSourceFile = emafeComponent.createFileSymbol("PLIB_EMAFE_SOURCE", None)
    emafePlibSourceFile.setSourcePath("peripheral/emafe/plib_emafe.c.ftl")
    emafePlibSourceFile.setOutputName("plib_emafe.c")
    emafePlibSourceFile.setDestPath("peripheral/emafe")
    emafePlibSourceFile.setProjectPath("config/" + configName + "/peripheral/emafe")
    emafePlibSourceFile.setType("SOURCE")
    emafePlibSourceFile.setMarkup(True)

    emafePlibHeaderFile = emafeComponent.createFileSymbol("PLIB_EMAFE_HEADER", None)
    emafePlibHeaderFile.setSourcePath("peripheral/emafe/plib_emafe.h.ftl")
    emafePlibHeaderFile.setOutputName("plib_emafe.h")
    emafePlibHeaderFile.setDestPath("peripheral/emafe")
    emafePlibHeaderFile.setProjectPath("config/" + configName + "/peripheral/emafe")
    emafePlibHeaderFile.setType("HEADER")
    emafePlibHeaderFile.setMarkup(True)

    emafePlibRegsFile = emafeComponent.createFileSymbol("PLIB_EMAFE_REGS", None)
    emafePlibRegsFile.setSourcePath("peripheral/emafe/plib_emafe_regs.h.ftl")
    emafePlibRegsFile.setOutputName("plib_emafe_regs.h")
    emafePlibRegsFile.setDestPath("peripheral/emafe")
    emafePlibRegsFile.setProjectPath("config/" + configName + "/peripheral/emafe")
    emafePlibRegsFile.setType("HEADER")
    emafePlibRegsFile.setMarkup(True)

    emafePlibSystemDefFile = emafeComponent.createFileSymbol("PLIB_EMAFE_DEF", None)
    emafePlibSystemDefFile.setType("STRING")
    emafePlibSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    emafePlibSystemDefFile.setSourcePath("peripheral/emafe/templates/system/definitions.h.ftl")
    emafePlibSystemDefFile.setMarkup(True)

    emafePlibSystemInitFile = emafeComponent.createFileSymbol("PLIB_EMAFE_INIT", None)
    emafePlibSystemInitFile.setType("STRING")
    emafePlibSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_PERIPHERALS")
    emafePlibSystemInitFile.setSourcePath("peripheral/emafe/templates/system/initialization.c.ftl")
    emafePlibSystemInitFile.setMarkup(True)

def destroyComponent(emafeComponent):
    global emafeInstanceName
    # Disable clock for EMAFE
    Database.setSymbolValue("core", emafeInstanceName.getValue() + "_CLOCK_ENABLE", True, 0)

def onAttachmentConnected(source, target):
    localComponent = source["component"]
    remoteComponent = target["component"]
    connectID = source["id"]

    if connectID == "mcspi_dependency":
        symbol = remoteComponent.getSymbolByID("MCSPI_CSR0_BAUD_RATE")
        symbol.setValue(10000000)

        symbolMC = remoteComponent.getSymbolByID("MCSPI_MASTER_CLOCK")
        symbol = localComponent.getSymbolByID("EMAFE_MCSPI_MASTER_CLOCK")
        symbol.setValue(symbolMC.getValue())