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
plib_icm_helpkeyword = "SE_plib_icm_config"

################################################################################
#### Business Logic ####
################################################################################
def checkInterruptHandler():
    global icmInstance

    enableICMInterrupt = False
    icmRegions = Database.getSymbolValue(icmInstance, "ICM_REGIONS_NUMBER")
    for icmRegion in range(0, icmRegions):
        rcfg = Database.getSymbolValue(icmInstance, "ICM_RCFG_{}".format(icmRegion))
        intBits = ~(rcfg & 0x3F0)
        if (intBits & 0x3F0) > 0:
            enableICMInterrupt = True
            break

    if enableICMInterrupt == True:
        # Enable ICM interrupt
        Database.setSymbolValue("core", "ICM_INTERRUPT_ENABLE", True, 2)
        Database.setSymbolValue("core", "ICM_INTERRUPT_HANDLER", "ICM_InterruptHandler", 2)
        Database.setSymbolValue("core", "ICM_INTERRUPT_HANDLER_LOCK", True, 2)
    else:
        # Disable ICM interrupt
        Database.clearSymbolValue("core", "ICM_INTERRUPT_ENABLE")
        Database.clearSymbolValue("core", "ICM_INTERRUPT_HANDLER")
        Database.clearSymbolValue("core", "ICM_INTERRUPT_HANDLER_LOCK")

def updateIcmConfiguration(symbol, event):
    regId = symbol.getID()
    regValue = symbol.getValue()

    updSymbolId = event["id"]
    updSymbol = event["symbol"]
    updSymbolValue = int(event["value"])
    
    mask = 0x1
    shift = 0
    if "DAPROT" in updSymbolId:
        shift = 24
        mask = 0x3F
    elif "HAPROT" in updSymbolId:
        shift = 16
        mask = 0x3F
    elif "UALGO" in updSymbolId:
        updSymbolValue = int(updSymbol.getSelectedValue())
        shift = 13
        mask = 0x7
    elif "DUALBUFF" in updSymbolId:
        shift = 9
    elif "ASCD" in updSymbolId:
        shift = 8
    elif "BBC" in updSymbolId:
        shift = 4
        mask = 0xF
    elif "SLBDIS" in updSymbolId:
        shift = 2
    elif "EOMDIS" in updSymbolId:
        shift = 1
    elif "WBDIS" in updSymbolId:
        shift = 0

    # Update register value
    regValue &= ~(mask << shift)

    if "DIS" in updSymbolId:
        updSymbolValue = not updSymbolValue

    if updSymbolValue > 0:
        regValue |= (updSymbolValue << shift)

    symbol.setValue(regValue)

def updateIcmRegionDescriptors(symbol, event):
    regId = symbol.getID()
    regValue = symbol.getValue()

    updSymbolId = event["id"]
    updSymbol = event["symbol"]
    updSymbolValue = int(event["value"])
    
    mask = 0x1
    shift = 0
    if "MPROT" in updSymbolId:
        shift = 24
        mask = 0x3F
    elif "ALGO" in updSymbolId:
        updSymbolValue = int(updSymbol.getSelectedValue())
        shift = 12
        mask = 0x7
    elif "PROCDLY" in updSymbolId:
        shift = 10
    elif "SUIEN" in updSymbolId:
        shift = 9
    elif "ECIEN" in updSymbolId:
        shift = 8
    elif "WCIEN" in updSymbolId:
        shift = 7
    elif "BEIEN" in updSymbolId:
        shift = 6
    elif "DMIEN" in updSymbolId:
        shift = 5
    elif "RHIEN" in updSymbolId:
        shift = 4
    elif "EOM" in updSymbolId:
        shift = 2
    elif "WRAP" in updSymbolId:
        shift = 1
    elif "CDWBN" in updSymbolId:
        shift = 0

    # Update register value
    regValue &= ~(mask << shift)

    checkInts = False
    if "IEN" in updSymbolId:
        updSymbolValue = not updSymbolValue
        checkInts = True

    if updSymbolValue > 0:
        regValue |= (updSymbolValue << shift)

    symbol.setValue(regValue)

    if checkInts == True:
        checkInterruptHandler()

def updateShowRegions(symbol, event):
    global icmInstance

    # numRegions = int(event["value"])
    numRegions = Database.getSymbolValue(icmInstance, "ICM_REGIONS_NUMBER")
    monitorMode = Database.getSymbolValue(icmInstance, "ICM_CONFIG_ASCD")
    icmComp = Database.getComponentByID(icmInstance)
    for icmRegion in range(0, 4):
        sensorSymbol = icmComp.getSymbolByID("ICM_REG_{}_CONFIG".format(icmRegion))
        enableSymbol = icmComp.getSymbolByID("ICM_REG_{}_ENABLED".format(icmRegion))
        eomSymbol = icmComp.getSymbolByID("ICM_REG_{}_EOM".format(icmRegion))
        wrapSymbol = icmComp.getSymbolByID("ICM_REG_{}_WRAP".format(icmRegion))
        if icmRegion < numRegions:
            if icmRegion == (numRegions - 1):
                if monitorMode == True:
                    eomSymbol.setValue(False)
                    wrapSymbol.setValue(True)
                else:
                    eomSymbol.setValue(True)
                    wrapSymbol.setValue(False)
            else:
                eomSymbol.setValue(False)
                wrapSymbol.setValue(False)

            sensorSymbol.setVisible(True)
            enableSymbol.setValue(True)
        else:
            sensorSymbol.setVisible(False)
            enableSymbol.setValue(False)

def showSymbol(symbol, event):
    symbol.setVisible(event["value"])

############################################################################
#### Code Generation ####
############################################################################
def instantiateComponent(icmComponent):
    global icmInstance

    configName = Variables.get("__CONFIGURATION_NAME")

    icmInstance = icmComponent.getID()
    Log.writeInfoMessage("Loading ICM Metrology Driver: {}".format(icmInstance))

    # ICM Configuration
    icmCfgDaprot = icmComponent.createKeyValueSetSymbol("ICM_CONFIG_DAPROT", None)
    icmCfgDaprot.setLabel("Region Descriptor Area Protection")
    icmCfgDaprot.setDefaultValue(1)
    icmCfgDaprot.setOutputMode("Value")
    icmCfgDaprot.setDisplayMode("Description")
    icmCfgDaprot.addKey("USR", "0", "User-access level")
    icmCfgDaprot.addKey("PRI", "1", "Privileged-access level")
    icmCfgDaprot.setHelp(plib_icm_helpkeyword)
    
    icmCfgHaprot = icmComponent.createKeyValueSetSymbol("ICM_CONFIG_HAPROT", None)
    icmCfgHaprot.setLabel("Region Hash Area Protection")
    icmCfgHaprot.setDefaultValue(1)
    icmCfgHaprot.setOutputMode("Value")
    icmCfgHaprot.setDisplayMode("Description")
    icmCfgHaprot.addKey("USR", "0", "User-access level")
    icmCfgHaprot.addKey("PRI", "1", "Privileged-access level")
    icmCfgHaprot.setHelp(plib_icm_helpkeyword)

    icmCfgAlgo = icmComponent.createKeyValueSetSymbol("ICM_CONFIG_UALGO", None)
    icmCfgAlgo.setLabel("User SHA Algorithm")
    icmCfgAlgo.setDefaultValue(0)
    icmCfgAlgo.setOutputMode("Value")
    icmCfgAlgo.setDisplayMode("Description")
    icmCfgAlgo.addKey("SHA1", "0", "SHA1")
    icmCfgAlgo.addKey("SHA256", "1", "SHA256")
    icmCfgAlgo.addKey("SHA224", "4", "SHA224")
    icmCfgAlgo.setHelp(plib_icm_helpkeyword)

    icmCfgDualBuffer = icmComponent.createBooleanSymbol("ICM_CONFIG_DUALBUFF", None)
    icmCfgDualBuffer.setLabel("Dual Input Buffer")
    icmCfgDualBuffer.setDefaultValue(0)
    icmCfgDualBuffer.setHelp(plib_icm_helpkeyword)

    icmCfgMonitorMode = icmComponent.createBooleanSymbol("ICM_CONFIG_ASCD", None)
    icmCfgMonitorMode.setLabel("Automatic monitoring mode")
    icmCfgMonitorMode.setDefaultValue(0)
    icmCfgMonitorMode.setDependencies(updateShowRegions, ["ICM_CONFIG_ASCD"])
    icmCfgMonitorMode.setHelp(plib_icm_helpkeyword)
                               
    icmCfgBurden = icmComponent.createIntegerSymbol("ICM_CONFIG_BBC", icmCfgMonitorMode)
    icmCfgBurden.setLabel("Bus Burden Control")
    icmCfgBurden.setDefaultValue(0)
    icmCfgBurden.setMin(0)
    icmCfgBurden.setMax(15)
    icmCfgBurden.setVisible(False)
    icmCfgBurden.setDependencies(showSymbol, ["ICM_CONFIG_ASCD"])
    icmCfgBurden.setHelp(plib_icm_helpkeyword)

    icmCfgSecList = icmComponent.createBooleanSymbol("ICM_CONFIG_SLBDIS", None)
    icmCfgSecList.setLabel("Allow Secondary List")
    icmCfgSecList.setDefaultValue(1)
    icmCfgSecList.setHelp(plib_icm_helpkeyword)

    icmCfgEOMDIS = icmComponent.createBooleanSymbol("ICM_CONFIG_EOMDIS", None)
    icmCfgEOMDIS.setLabel("Allow End of Monitoring")
    icmCfgEOMDIS.setDefaultValue(1)
    icmCfgEOMDIS.setHelp(plib_icm_helpkeyword)

    icmCfgWBDIS = icmComponent.createBooleanSymbol("ICM_CONFIG_WBDIS", None)
    icmCfgWBDIS.setLabel("Allow Write Back")
    icmCfgWBDIS.setDefaultValue(1)
    icmCfgWBDIS.setHelp(plib_icm_helpkeyword)

    icmCFGRegister = icmComponent.createHexSymbol("ICM_CFG", None)
    icmCFGRegister.setLabel("ICM_CFG")
    icmCFGRegister.setDefaultValue(0x01010000)
    icmCFGRegister.setVisible(False)
    icmCFGRegister.setDependencies(updateIcmConfiguration, ["ICM_CONFIG_DAPROT", "ICM_CONFIG_HAPROT", 
                                                            "ICM_CONFIG_UALGO", "ICM_CONFIG_DUALBUFF",
                                                            "ICM_CONFIG_DUALBUFF", "ICM_CONFIG_ASCD",
                                                            "ICM_CONFIG_BBC", "ICM_CONFIG_SLBDIS",
                                                            "ICM_CONFIG_EOMDIS", "ICM_CONFIG_WBDIS"])

    # ICM Region Configuration
    icmNumRegions = icmComponent.createIntegerSymbol("ICM_REGIONS_NUMBER", None)
    icmNumRegions.setLabel("Number of Regions")
    icmNumRegions.setDefaultValue(1)
    icmNumRegions.setMin(1)
    icmNumRegions.setMax(4)
    icmNumRegions.setDependencies(updateShowRegions, ["ICM_REGIONS_NUMBER"])
    icmNumRegions.setHelp(plib_icm_helpkeyword)

    for icmRegion in range(0, 4):
        symbolDepList = []

        icmRegionConfigMenu = icmComponent.createMenuSymbol("ICM_REG_{}_CONFIG".format(icmRegion), icmNumRegions)
        icmRegionConfigMenu.setLabel("Region {} Configuration".format(icmRegion))
        icmRegionConfigMenu.setVisible(icmRegion == 0)
        icmRegionConfigMenu.setHelp(plib_icm_helpkeyword)

        symbolName = "ICM_REG_{}_ENABLED".format(icmRegion)
        icmDescEnabled = icmComponent.createBooleanSymbol(symbolName, icmRegionConfigMenu)
        icmDescEnabled.setLabel("Descr Enabled")
        icmDescEnabled.setDefaultValue(icmRegion == 0)
        icmDescEnabled.setVisible(False)

        symbolName = "ICM_REG_{}_MPROT".format(icmRegion)
        icmBusProtection = icmComponent.createKeyValueSetSymbol(symbolName, icmRegionConfigMenu)
        icmBusProtection.setLabel("Bus Protection")
        icmBusProtection.setDefaultValue(1)
        icmBusProtection.setOutputMode("Value")
        icmBusProtection.setDisplayMode("Description")
        icmBusProtection.addKey("USR", "0", "User-access level")
        icmBusProtection.addKey("PRI", "1", "Privileged-access level")
        icmBusProtection.setHelp(plib_icm_helpkeyword)
        symbolDepList.append(symbolName)

        symbolName = "ICM_REG_{}_ALGO".format(icmRegion)
        icmAlgo = icmComponent.createKeyValueSetSymbol(symbolName, icmRegionConfigMenu)
        icmAlgo.setLabel("SHA Algorithm")
        icmAlgo.setDefaultValue(0)
        icmAlgo.setOutputMode("Value")
        icmAlgo.setDisplayMode("Description")
        icmAlgo.addKey("SHA1", "0", "SHA1")
        icmAlgo.addKey("SHA256", "1", "SHA256")
        icmAlgo.addKey("SHA224", "4", "SHA224")
        icmAlgo.setHelp(plib_icm_helpkeyword)
        symbolDepList.append(symbolName)

        symbolName = "ICM_REG_{}_PROCDLY".format(icmRegion)
        icmDelay = icmComponent.createKeyValueSetSymbol(symbolName, icmRegionConfigMenu)
        icmDelay.setLabel("Processing Delay")
        icmDelay.setDefaultValue(0)
        icmDelay.setOutputMode("Value")
        icmDelay.setDisplayMode("Description")
        icmDelay.addKey("SHORTEST", "0", "SHORTEST")
        icmDelay.addKey("LONGEST", "1", "LONGEST")
        icmDelay.setHelp(plib_icm_helpkeyword)
        symbolDepList.append(symbolName)

        symbolName = "ICM_REG_{}_EOM".format(icmRegion)
        icmEOMInt = icmComponent.createBooleanSymbol(symbolName, icmRegionConfigMenu)
        icmEOMInt.setLabel("End of Monitoring")
        icmEOMInt.setValue(icmRegion == 0)
        icmEOMInt.setVisible(False)
        symbolDepList.append(symbolName)

        symbolName = "ICM_REG_{}_WRAP".format(icmRegion)
        icmWrapCmd = icmComponent.createBooleanSymbol(symbolName, icmRegionConfigMenu)
        icmWrapCmd.setLabel("Wrap enable")
        icmWrapCmd.setVisible(False)
        symbolDepList.append(symbolName)

        symbolName = "ICM_REG_{}_CDWBN".format(icmRegion)
        icmCDWBN = icmComponent.createKeyValueSetSymbol(symbolName, icmRegionConfigMenu)
        icmCDWBN.setLabel("Digest action")
        icmCDWBN.setDefaultValue(0)
        icmCDWBN.setOutputMode("Value")
        icmCDWBN.setDisplayMode("Description")
        icmCDWBN.addKey("WRITE", "0", "The digest is written")
        icmCDWBN.addKey("COMPARE", "1", "The digest value is compared")
        icmCDWBN.setVisible(False)
        symbolDepList.append(symbolName)
        
        icmIntMenu = icmComponent.createMenuSymbol("ICM_REG_{}_INTERRUPTS".format(icmRegion), icmRegionConfigMenu)
        icmIntMenu.setLabel("Interrupts Configuration")

        symbolName = "ICM_REG_{}_SUIEN".format(icmRegion)
        icmStatusInt = icmComponent.createBooleanSymbol(symbolName, icmIntMenu)
        icmStatusInt.setLabel("Status Updated Interrupt")
        icmStatusInt.setHelp(plib_icm_helpkeyword)
        symbolDepList.append(symbolName)

        symbolName = "ICM_REG_{}_ECIEN".format(icmRegion)
        icmEndBitInt = icmComponent.createBooleanSymbol(symbolName, icmIntMenu)
        icmEndBitInt.setLabel("End Bit Condition Interrupt")
        icmEndBitInt.setHelp(plib_icm_helpkeyword)
        symbolDepList.append(symbolName)

        symbolName = "ICM_REG_{}_WCIEN".format(icmRegion)
        icmWrapInt = icmComponent.createBooleanSymbol(symbolName, icmIntMenu)
        icmWrapInt.setLabel("Wrap Condition Interrupt")
        icmWrapInt.setHelp(plib_icm_helpkeyword)
        symbolDepList.append(symbolName)

        symbolName = "ICM_REG_{}_BEIEN".format(icmRegion)
        icmBusErrorInt = icmComponent.createBooleanSymbol(symbolName, icmIntMenu)
        icmBusErrorInt.setLabel("Bus Error Interrupt")
        icmBusErrorInt.setHelp(plib_icm_helpkeyword)
        symbolDepList.append(symbolName)

        symbolName = "ICM_REG_{}_DMIEN".format(icmRegion)
        icmDigestMismatchInt = icmComponent.createBooleanSymbol(symbolName, icmIntMenu)
        icmDigestMismatchInt.setLabel("Digest Mismatch Interrupt")
        icmDigestMismatchInt.setHelp(plib_icm_helpkeyword)
        symbolDepList.append(symbolName)

        symbolName = "ICM_REG_{}_RHIEN".format(icmRegion)
        icmHashInt = icmComponent.createBooleanSymbol(symbolName, icmIntMenu)
        icmHashInt.setLabel("Hash Completed Interrupt")
        icmHashInt.setHelp(plib_icm_helpkeyword)
        symbolDepList.append(symbolName)

        icmRCFGRegister = icmComponent.createHexSymbol("ICM_RCFG_{}".format(icmRegion), icmRegionConfigMenu)
        icmRCFGRegister.setLabel("RCFG")
        icmRCFGRegister.setDefaultValue(0x010003F4)
        icmRCFGRegister.setDependencies(updateIcmRegionDescriptors, symbolDepList)
        icmRCFGRegister.setVisible(False)


    ##########################################################################################################
    # GENERATION FILES
    ##########################################################################################################
    icmPlibSourceFile = icmComponent.createFileSymbol("PLIB_ICM_SOURCE", None)
    icmPlibSourceFile.setSourcePath("peripheral/icm/plib_icm.c.ftl")
    icmPlibSourceFile.setOutputName("plib_icm.c")
    icmPlibSourceFile.setDestPath("peripheral/icm")
    icmPlibSourceFile.setProjectPath("config/" + configName + "/peripheral/icm")
    icmPlibSourceFile.setType("SOURCE")
    icmPlibSourceFile.setMarkup(True)

    icmPlibHeaderFile = icmComponent.createFileSymbol("PLIB_ICM_HEADER", None)
    icmPlibHeaderFile.setSourcePath("peripheral/icm/plib_icm.h.ftl")
    icmPlibHeaderFile.setOutputName("plib_icm.h")
    icmPlibHeaderFile.setDestPath("peripheral/icm")
    icmPlibHeaderFile.setProjectPath("config/" + configName + "/peripheral/icm")
    icmPlibHeaderFile.setType("HEADER")
    icmPlibHeaderFile.setMarkup(True)

    icmPlibSystemDefFile = icmComponent.createFileSymbol("PLIB_ICM_DEF", None)
    icmPlibSystemDefFile.setType("STRING")
    icmPlibSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    icmPlibSystemDefFile.setSourcePath("peripheral/icm/templates/system/definitions.h.ftl")
    icmPlibSystemDefFile.setMarkup(True)

    icmPlibSystemInitFile = icmComponent.createFileSymbol("PLIB_ICM_INIT", None)
    icmPlibSystemInitFile.setType("STRING")
    icmPlibSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_PERIPHERALS")
    icmPlibSystemInitFile.setSourcePath("peripheral/icm/templates/system/initialization.c.ftl")
    icmPlibSystemInitFile.setMarkup(True)

