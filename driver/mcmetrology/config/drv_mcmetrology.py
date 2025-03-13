# coding: utf-8
"""
Copyright (C) 2022-2024, Microchip Technology Inc., and its subsidiaries. All rights reserved.

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
"""
multichannel_met_helpkeyword = "mcc_h3_mutichannel_met_config"

FORMAT_CONST_uQ824 = 24
FORMAT_CONST_uQ230 = 30
FORMAT_CONST_uQ1220 = 20
FORMAT_CONST_sQ130 = 30
FORMAT_CONST_uQ2210 = 10
FORMAT_CONST_uQ032 = 32
GAIN_VI_Q = 10
CONST_KI = 617.28396   # Temporal: Tomar la K_Ix del primer canal con sensor de corriente. Punto de mejora.

MAX_CHANNELS = 32
MAX_MCP391x_SUPPORTED = 2

MET_SENSOR_CT_VALUE = "0"
MET_SENSOR_SHUNT_VALUE = "1"
MET_SENSOR_COIL_VALUE = "2"
MET_SENSOR_VRD_VALUE = "3"

REG_FCTRL0_DEFAULT = 0
REG_FCTRL1_DEFAULT = 0x08000000

global vChannelsList
global iChannelsList
vChannelsList = ["Select Channel"]
iChannelsList = ["Select Channel"]

global metInstance

global metChannelNum
metChannelNum = 8

global powerSourceSymbolRefs
powerSourceSymbolRefs = {}

global metWaveFormCaptureChnRefs
metWaveFormCaptureChnRefs = []

def getNewRegValue(regValue, newValue, mask, offset):
  # print("MET >> getNewRegValue regValue:{} newValue:{} mask:{} offset:{}".format(regValue, newValue, mask, offset))
  # print("MET >> getNewRegValue types regValue:{} newValue:{}  mask:{} offset:{}".format(type(regValue), type(newValue), type(mask), type(offset)))
    regValue = regValue & ~(mask << offset)
    regValue = regValue | (newValue << offset)
    return regValue

def updateVisibleF(symbol, event):
    sensorTypeSymbol = event["symbol"]
    value = sensorTypeSymbol.getSelectedValue()
    if value == MET_SENSOR_COIL_VALUE:
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)

def updateVisibleTR(symbol, event):
    sensorTypeSymbol = event["symbol"]
    value = sensorTypeSymbol.getSelectedValue()
    if value == MET_SENSOR_COIL_VALUE:
        symbol.setLabel("Current Sensitivity (uV/A)")
        symbol.setVisible(True)
    elif value == MET_SENSOR_CT_VALUE:
        symbol.setLabel("Transformer Ratio")
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)

def updateVisibleRL(symbol, event):
    afeSymbol = event["symbol"]
    value = (afeSymbol.getSelectedValue())
    if value == MET_SENSOR_SHUNT_VALUE:
        # Resistive Shunt
        symbol.setLabel("Shunt Resistor (uOhms)")
        symbol.setVisible(True)
    elif value == MET_SENSOR_CT_VALUE:
        symbol.setLabel("Burden Resistor (uOhms)")
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)

def updateVisibleKU(symbol, event):
    sensorTypeSymbol = event["symbol"]
    value = sensorTypeSymbol.getSelectedValue()
    if value == MET_SENSOR_VRD_VALUE:
        symbol.setLabel("Voltage divider ratio")
        symbol.setVisible(True)
    else:
        symbol.setLabel(" ")
        symbol.setVisible(False)

def updateKCHxValue(symbol, event):
    global metInstance

    setSymbol = True
    channel = int(event["id"].split("_")[-1])

    metComp = Database.getComponentByID(metInstance)
    sensorSymbol = metComp.getSymbolByID("DRV_MCMET_CHANNEL_SENSOR_{}".format(channel))
    sensorValue = sensorSymbol.getSelectedValue()
  # print("MET >> updateKCHxValue channel:{} sensorValue:{}".format(channel, sensorValue))
    if sensorValue == MET_SENSOR_CT_VALUE:
        tRatio = Database.getSymbolValue(metInstance, "DRV_MCMET_CONF_TR_{}".format(channel))
        burdenRes = Database.getSymbolValue(metInstance, "DRV_MCMET_CONF_RL_{}".format(channel))
        gainValue = Database.getSymbolValue(metInstance, "DRV_MCMET_CHANNEL_GAIN_{}".format(channel))
        gain = 1 << int(gainValue)
      # print("MET >> MET_SENSOR_CT_VALUE updateKCHxValue tRatio:{} burdenRes:{} gain:{}".format(tRatio, burdenRes, gain))
        regValue = float(tRatio) * 1000000 / (int(burdenRes) * gain) # *1000000 due to burdenRes is in uOmhs
      # print("MET >> MET_SENSOR_CT_VALUE updateKCHxValue result:{}".format(regValue))
    elif sensorValue == MET_SENSOR_SHUNT_VALUE:
        shuntRes = Database.getSymbolValue(metInstance, "DRV_MCMET_CONF_RL_{}".format(channel))
        gainValue = Database.getSymbolValue(metInstance, "DRV_MCMET_CHANNEL_GAIN_{}".format(channel))
        gain = 1 << int(gainValue)
      # print("MET >> MET_SENSOR_SHUNT_VALUE updateKCHxValue shuntRes:{} gain:{}".format(shuntRes, gain))
        regValue = 1000000.0 / (int(shuntRes) * int(gain)) # *1000000 due to shuntRes is in uOmhs
      # print("MET >> MET_SENSOR_SHUNT_VALUE updateKCHxValue regValue:{}".format(regValue))
    elif sensorValue == MET_SENSOR_COIL_VALUE:
        freq = Database.getSymbolValue(metInstance, "DRV_MCMET_CONF_F_{}".format(channel))
        currSensivity = Database.getSymbolValue(metInstance, "DRV_MCMET_CONF_TR_{}".format(channel))
        gainValue = Database.getSymbolValue(metInstance, "DRV_MCMET_CHANNEL_GAIN_{}".format(channel))
        gain = 1 << int(gainValue)
      # print("MET >> MET_SENSOR_COIL_VALUE updateKCHxValue freq:{} currSensivity:{} gain:{}".format(freq, currSensivity, gain))
        Ksf = 1000000.0 / currSensivity # *1000000 due to currSensivity is in uV/A
        regValue = Ksf / (int(gain) * (60.0/int(freq)))
      # print("MET >> MET_SENSOR_COIL_VALUE updateKCHxValue regValue:{}".format(regValue))
    elif sensorValue == MET_SENSOR_VRD_VALUE:
        dividerRatio = Database.getSymbolValue(metInstance, "DRV_MCMET_CONF_KU_{}".format(channel))
        regValue = int(dividerRatio)
      # print("MET >> MET_SENSOR_VRD_VALUE updateKCHxValue dividerRatio:{} regValue:{}".format(dividerRatio, regValue))
    else:
        setSymbol = False

    if setSymbol == True:
        regValue = regValue * (2**FORMAT_CONST_uQ2210)
        regValue = int(round(regValue))
      # print("MET >> updateKCHxValue regValue:{:08x}".format(regValue))
        symbol.setValue(regValue)
    
def updateAdvancedConfig(symbol, event):
    afeSymbol = event["symbol"]
    value = afeSymbol.getSelectedValue()
    if value == MET_SENSOR_VRD_VALUE:
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)

def updateRegFCTRL0(symbol, event):
    global metInstance

    regValue = symbol.getValue()

    # Set Power Number
    powerSrcSelectionNum = int(event["value"])
    regValue = getNewRegValue(regValue, powerSrcSelectionNum, 0x1F, 8)

    # Set AFE Selection 
    metComp = Database.getComponentByID(metInstance)
    afeSymbol = metComp.getSymbolByID("DRV_MCMET_AFE_SELECTION")
    afeSelection = int(afeSymbol.getSelectedValue())
  # print("MET >> updateRegFCTRL0 afeSelection: {}".format(afeSelection))

    regValue = getNewRegValue(regValue, afeSelection, 0xF, 28)

  # print("MET >> updateRegFCTRL0 regValue: {:08x}".format(regValue))
    symbol.setValue(regValue)

def updateRegFCTRL1(symbol, event):
    eventSymbolId = event["id"]
    eventSymbol = event["symbol"]
    regValue = symbol.getValue()

    if eventSymbolId == "DRV_MCMET_CHANNELS_NUM":
        # Set Number of Channels
        channelsNum = int(event["value"])
        regValue = getNewRegValue(regValue, channelsNum, 0xFF, 24)
    elif eventSymbolId == "DRV_MCMET_CONF_CREEP_P_EN":
        # Set CREEP_P_EN
        creepEN = int(event["value"])
        regValue = getNewRegValue(regValue, creepEN, 0x1, 2)
    elif eventSymbolId == "DRV_MCMET_CONF_CREEP_Q_EN":
        # Set CREEP_Q_EN
        creepEN = int(event["value"])
        regValue = getNewRegValue(regValue, creepEN, 0x1, 1)
    elif eventSymbolId == "DRV_MCMET_CONF_CREEP_I_EN":
        # Set CREEP_I_EN
        creepEN = int(event["value"])
        regValue = getNewRegValue(regValue, creepEN, 0x1, 0)

  # print("MET >> updateRegFCTRL1 regValue: {:08x}".format(regValue))
    symbol.setValue(regValue)

def updateRegsPowerSources(symbol, event):
    global metInstance 

    eventSymbolId = event["id"]
    eventValue = event["value"]
    
    channelNum = int(eventSymbolId.split("_")[-1])
    powerSourceNum = channelNum >> 1

    symbolName = "REG_MCMET_CTRL_PSRC_{}".format(powerSourceNum)
    regValue = Database.getSymbolValue(metInstance, symbolName)

    regOffset = 0
    if "_I_" in eventSymbolId:
        regOffset += 8

    if channelNum % 2 != 0:
        regOffset += 16

    maxPowerSource = Database.getSymbolValue(metInstance, "DRV_MCMET_POWERS_NUM")
    if eventValue == "Select Channel" or powerSourceNum > maxPowerSource:
        eventChannel = 0
    else:
        eventChannel = int(eventValue.split(" ")[-1])

    # print("MET >> updateRegsPowerSources prev {}: {:08x} -> eventChannel:{} regOffset:{}".format(symbolName, regValue, eventChannel, regOffset))

    regValue = getNewRegValue(regValue, eventChannel, 0x1F, regOffset)

    # print("MET >> updateRegsPowerSources {}: {:08x}".format(symbolName, regValue))
    Database.setSymbolValue(metInstance, symbolName, regValue)

def updateRegSensorType0(symbol, event):
    global metInstance 

    sensorTypeSymbol = event["symbol"]
    sensorTypeValue = sensorTypeSymbol.getSelectedValue()
    if "Select" in sensorTypeValue:
        sensorTypeValue = 0
    else:
        sensorTypeValue = int(sensorTypeValue)
    eventSymbolId = event["id"]
    channelNum = int(eventSymbolId.split("_")[-1])
    regOffset = channelNum << 1
    regValue = symbol.getValue()

    regValue = getNewRegValue(regValue, sensorTypeValue, 0x03, regOffset)
    
  # print("MET >> updateRegSensorType0 regValue: {:08x}".format(regValue))
    symbol.setValue(regValue)

def updateRegSensorType1(symbol, event):
    global metInstance 

    sensorTypeSymbol = event["symbol"]
    sensorTypeValue = sensorTypeSymbol.getSelectedValue()
    if "Select" in sensorTypeValue:
        sensorTypeValue = 0
    else:
        sensorTypeValue = int(sensorTypeValue)
    eventSymbolId = event["id"]
    channelNum = int(eventSymbolId.split("_")[-1])
    if channelNum >= 16:
        channelNum = channelNum - 16
        regOffset = channelNum << 1
        regValue = symbol.getValue()

        regValue = getNewRegValue(regValue, sensorTypeValue, 0x03, regOffset)
        
        # print("MET >> updateRegSensorType0 regValue: {:08x}".format(regValue))
        symbol.setValue(regValue)

def updateRegPulseContribution(symbol, event):
    pulseContValue = int(event["value"])
    pulseContSymbolId = event["id"]
    channelNum = int(pulseContSymbolId.split("_")[-1])
    regOffset = channelNum
    regValue = symbol.getValue()
    regValue = getNewRegValue(regValue, pulseContValue, 0x01, regOffset)
    
  # print("MET >> updateRegPulseContribution {}: {:08x}".format(symbol.getID(), regValue))
    symbol.setValue(regValue)

def updateRegPowerOffsetCtrl(symbol, event):
    powerOffsetValue = int(event["value"])
    symbolId = event["id"]
    powerSrcNum = symbolId.split("_")[-1]
    if powerSrcNum == "EN":
        regOffset = 31
    else:
        regOffset = int(powerSrcNum)

    regValue = symbol.getValue()
    regValue = getNewRegValue(regValue, powerOffsetValue, 0x01, regOffset)

  # print("MET >> updateRegPowerOffsetCtrl {}: {:08x}".format(symbol.getID(), regValue))
    symbol.setValue(regValue)

def updateRegCapture(symbol, event):
    eventSymbolValue = int(event["value"])
    eventSymbolId = event["id"]

    regValue = symbol.getValue()
    
    if eventSymbolId == "DRV_MCMET_WAVECAPT_TYPE":
        regValue = getNewRegValue(regValue, eventSymbolValue, 0x03, 0)
    elif eventSymbolId == "DRV_MCMET_WAVECAPT_SRC":
        regValue = getNewRegValue(regValue, eventSymbolValue, 0x07, 4)
    elif eventSymbolId == "DRV_MCMET_WAVEFORM_CAPTURE":
        regValue = getNewRegValue(regValue, eventSymbolValue, 0x01, 31)

  # print("MET >> updateRegCapture {}: {:08x}".format(symbol.getID(), regValue))
    symbol.setValue(regValue)

def updateRegCaptureCHSel(symbol, event):
    eventSymbolValue = int(event["value"])
    eventSymbolId = event["id"]
    channelNum = int(eventSymbolId.split("_")[-1])

    regValue = symbol.getValue()
    regValue = getNewRegValue(regValue, eventSymbolValue, 0x01, channelNum)

  # print("MET >> updateRegCaptureCHSel {}: {:08x}".format(symbol.getID(), regValue))
    symbol.setValue(regValue)

def updateCaptureNumberChannels(symbol, event):
    regValue = symbol.getValue()
    if event["value"] == True:
        regValue = regValue + 1
    else:
        regValue = regValue - 1

  # print("MET >> updateCaptureNumberChannels {}: {:08x}".format(symbol.getID(), regValue))
    symbol.setValue(regValue)

def updateRegMCP391XGain(symbol, event):
    global metInstance 

    metComp = Database.getComponentByID(metInstance)
    afeSymbol = metComp.getSymbolByID("DRV_MCMET_AFE_SELECTION")
    afeValue = afeSymbol.getSelectedValue()
    if afeValue in ["0", "1"]:
        channelsPerAfe = 8
    else:
        channelsPerAfe = 6

    eventSymbolId = event["id"]
    eventValue = int(event["value"])
    channelNum = int(eventSymbolId.split("_")[-1])
    regOffset = channelNum % channelsPerAfe
    mcp391xNum = channelNum / channelsPerAfe
    symbolName = "REG_MCMET_CONF_MCP391X_GAIN_{}".format(mcp391xNum)
    regValue = Database.getSymbolValue(metInstance, symbolName)
    regValue = getNewRegValue(regValue, eventValue, 0x07, regOffset)

  # print("MET >> updateRegMCP391XGain {}: {:08x}".format(symbolName, regValue))
    Database.setSymbolValue(metInstance, symbolName, regValue)

def updateRegMCP391XConfig(symbol, event):
    eventSymbolId = event["id"]
    eventValue = int(event["value"])

    regOffset = 0
    if "_DITHER_" in eventSymbolId:
        regOffset = 20
    elif "_BOOST_" in eventSymbolId:
        regOffset = 18

    if regOffset > 0:
        regValue = symbol.getValue()
        regValue = getNewRegValue(regValue, eventValue, 0x03, regOffset)

      # print("MET >> updateRegMCP391XConfig {}: {:08x}".format(symbol.getID(), regValue))
        symbol.setValue(regValue)

def setNumberOfChannels(symbol, event):  
    global metInstance
    global metChannelNum

    numChannels = [8, 16, 24, 32, 6, 12, 18, 24]
    afeValue = int(event["symbol"].getSelectedValue())
    channelNum = numChannels[afeValue]
    metChannelNum = channelNum
    symbol.setValue(channelNum)

    # Update Waveform Capure channel list
    captureValue = Database.getSymbolValue(metInstance, "DRV_MCMET_WAVEFORM_CAPTURE")
    if captureValue == True:
        for channel in range(0, MAX_CHANNELS):
            symbolRef = metWaveFormCaptureChnRefs[channel]
            if channel < channelNum:
                symbolRef.setVisible(True)
            else:
                symbolRef.setVisible(False)

def updatePowerSourceSymbols():
    global powerSourceSymbolRefs
    global vChannelsList
    global iChannelsList

    for powerId, symbolRefs in powerSourceSymbolRefs.items():
        vSourceSymbol = symbolRefs.get('v')
        if len(vChannelsList) > 1:
            vSourceSymbol.setRange(vChannelsList)
            vSourceSymbol.setValue(vChannelsList[-1])
        else:
            vSourceSymbol.setRange(["Select Channel"])

        iSourceSymbol = symbolRefs.get('i')
        if len(iChannelsList) > 1:
            iSourceSymbol.setRange(iChannelsList)
            iSourceSymbol.setValue(iChannelsList[-1])
        else:
            iSourceSymbol.setRange(["Select Channel"])

def updateChannelsLists(symbol, event):
    global vChannelsList
    global iChannelsList

    channel = "Channel {}".format(int(symbol.getID().split("_")[-1]))
    sensor = symbol.getSelectedKey()
    if sensor == "VRD":
        # update V channels
        if channel not in vChannelsList:
            vChannelsList.append(channel)
        # remove from I channels if found
        if channel in iChannelsList:
            iChannelsList.remove(channel)
    elif sensor == "Select":
        if channel in iChannelsList:
            iChannelsList.remove(channel)
        elif channel in vChannelsList:
            vChannelsList.remove(channel)
    else:
        # update I channels
        if channel not in iChannelsList:
            iChannelsList.append(channel)
        # remove from V channels if found
        if channel in vChannelsList:
            vChannelsList.remove(channel)

    vChannelsList = sorted(vChannelsList)
    iChannelsList = sorted(iChannelsList)

    updatePowerSourceSymbols()

def showChannelsFromAFESelection(symbol, event):  
    global metInstance

    numChannels = [8, 16, 24, 32, 6, 12, 18, 24]
    afeValue = int(event["symbol"].getSelectedValue())
    channelMax = numChannels[afeValue]
    symbolChannel = int(symbol.getID().split("_")[-1])
    if symbolChannel < channelMax:
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)

def enableChannelsFromAFESelection(symbol, event):  
    numChannels = [8, 16, 24, 32, 6, 12, 18, 24]
    afeValue = int(event["symbol"].getSelectedValue())
    channelMax = numChannels[afeValue]
    symbolChannel = int(symbol.getID().split("_")[-1])
    if symbolChannel < channelMax:
        symbol.setValue(True)
    else:
        symbol.setValue(False)

def showMCPDevicesFromAFESelection(symbol, event):  
    mcpDeviceIndex = int(symbol.getID().split("_")[-1])
    if mcpDeviceIndex == 0:
        symbol.setVisible(True)
    else:
        afeKey = event["symbol"].getSelectedKey()
        symbol.setVisible(afeKey.startswith("2"))

def showPowerSource(symbol, event):  
    numPowers = int(event["value"])
    symbolChannel = int(symbol.getID().split("_")[-1])
    if symbolChannel < numPowers:
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)

def setMaximumPowerSource(symbol, event):  
    numPowers = [7, 15, 23, 31, 5, 11, 17, 23]
    afeValue = int(event["symbol"].getSelectedValue())
    powerMax = numPowers[afeValue]
    symbol.setMax(powerMax)

def showCaptureSymbol(symbol, event):  
    global metChannelNum

    symbolChannel = int(symbol.getID().split("_")[-1])
    if symbolChannel < metChannelNum:
        symbol.setVisible(event["value"])
    else:
        symbol.setVisible(False)

def updateRegConfigConstantsPQI_K_t(symbol, event):
    regValue = 1000.0 / int(event["value"])
    regValue = regValue * (2**FORMAT_CONST_uQ824)
    regValue = int(round(regValue))
    
    symbol.setValue(regValue)
  # print("MET >> updateRegConfigConstantsPQI_K_t {}:{:08x}".format(symbol.getID(), regValue))

def updateRegConfigPQCreepThreshold(symbol, event):
    global metInstance

    freq = Database.getSymbolValue(metInstance, "DRV_MCMET_CONF_FREQ")
    regValue = float(event["value"])/(int(freq)*3600)
    regValue = regValue * (2**FORMAT_CONST_uQ230)
    regValue = int(round(regValue))
    
    symbol.setValue(regValue)
  # print("MET >> updateRegConfigPQCreepThreshold {}:{:08x}".format(symbol.getID(), regValue))

def updateRegConfigICreepThreshold(symbol, event):
    regValue = float(event["value"])/CONST_KI
    regValue = regValue * (2**FORMAT_CONST_uQ1220)
    regValue = int(round(regValue))
    
    symbol.setValue(regValue)
  # print("MET >> updateRegConfigICreepThreshold {}:{:08x}".format(symbol.getID(), regValue))

def updateRegPowerOffset(symbol, event):
    global metInstance

    freq = Database.getSymbolValue(metInstance, "DRV_MCMET_CONF_FREQ")
    regValue = float(event["value"]) / (1000 * (int(freq)*3600))  # 1000 due to value is in mWh / mVAh
    regValue = regValue * (2**FORMAT_CONST_sQ130)
    regValue = int(round(regValue))
    symbol.setValue(regValue)
  # print("MET >> updateRegPowerOffset {}:{:08x}".format(symbol.getID(), regValue))

def updateRegSwellSagThreshold(symbol, event):
    global metInstance

    symbolId = event["id"]
    channel = symbolId.split("_")[-1]

    metComp = Database.getComponentByID(metInstance)
    sensorSymbol = metComp.getSymbolByID("DRV_MCMET_CHANNEL_SENSOR_{}".format(channel))
    sensorValue = sensorSymbol.getSelectedValue()
    if sensorValue == MET_SENSOR_VRD_VALUE:
        if "CONF_K_CH_" in symbolId:
            k_chx = event["value"]
            if "SWELL" in symbol.getID():
                swellThresholdValue = Database.getSymbolValue(metInstance, "DRV_MCMET_CONF_SWELL_TH_{}".format(channel)) 
            else:
                swellThresholdValue = Database.getSymbolValue(metInstance, "DRV_MCMET_CONF_SAG_TH_{}".format(channel)) 
        else:
            swellThresholdValue = event["value"]
            k_chx = Database.getSymbolValue(metInstance, "REG_MCMET_CONF_K_CH_{}".format(channel)) 

        kx = float(k_chx) / (2**FORMAT_CONST_uQ2210)  # revert format uQ22.10
        regValue = float(swellThresholdValue) / (1000.0 * kx) # threshold is in mV.
        regValue = (regValue**2) * (2**FORMAT_CONST_uQ032)
        regValue = int(round(regValue))
    else:
        # only implemented for Voltage channels
        regValue = 0

  # print("MET >> updateRegSwellSagThreshold {} regValue:{:08x}".format(symbolId, regValue))
    symbol.setValue(regValue)

# def updateConfigHarmonicCtrl(symbol, event):
#     harmonicCtrl = symbol.getValue()
#     harmonicId = event["id"]
#     harmonicValue = event["value"]
#     harmonicPos = int("".join(filter(lambda x: x.isdigit(), harmonicId))) - 1

#     if harmonicValue == 0:
#         harmonicCtrl &= ~(1 << harmonicPos)
#     else:
#         harmonicCtrl |= (1 << harmonicPos)

#     if harmonicCtrl > 0:
#         harmonicCtrl |= (1 << 31)
    
#     symbol.setValue(harmonicCtrl)

def updateRegPulseXCtrl(symbol, event):
    global metInstance 

    pulseId = event["id"]
    pulseId = pulseId[-1]
    pEn = Database.getSymbolValue(metInstance, "DRV_MCMET_CONF_EN_P" + str(pulseId))
    pDet = Database.getSymbolValue(metInstance, "DRV_MCMET_CONF_DET_P" + str(pulseId))
    pPol = Database.getSymbolValue(metInstance, "DRV_MCMET_CONF_POL_P" + str(pulseId))
    pTyp = Database.getSymbolValue(metInstance, "DRV_MCMET_CONF_TYP_P" + str(pulseId))
    pWid = Database.getSymbolValue(metInstance, "DRV_MCMET_CONF_WID_P" + str(pulseId))
    reg = (pEn << 31) | (pDet << 28) | (pPol << 24) | (pTyp << 16) | pWid
    symbol.setValue(reg)

  # print("MET >> updateRegPulseXCtrl {}: event:{} regValue:{:08x}".format(symbol.getID(), event["id"], reg))

def showSymbolOnBoolEvent(symbol, event):
    symbol.setVisible(event["value"])

def showRTOSMenu(symbol, event):
    show_rtos_menu = False

    if (Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal"):
        show_rtos_menu = True

    symbol.setVisible(show_rtos_menu)

def enableRTOS(symbol, event):
    enable_rtos = False

    if (Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal"):
        enable_rtos = True

    symbol.setValue(enable_rtos)

def genRtosTask(symbol, event):
    gen_rtos_task = False
    component = symbol.getComponent()

    if (Database.getSymbolValue("HarmonyCore", "SELECT_RTOS") != "BareMetal"):
        gen_rtos_task = True

    symbol.setEnabled(gen_rtos_task)

def commandRtosMicriumOSIIIAppTaskVisibility(symbol, event):
    if (event["value"] == "MicriumOSIII"):
        symbol.setVisible(True)
    else:
        symbol.setVisible(False)

def commandRtosMicriumOSIIITaskOptVisibility(symbol, event):
    symbol.setVisible(event["value"])

def commandSendMessageHeapSize(symbol, event):
    dummyDict = {}
    dummyDict = Database.sendMessage("core", "HEAP_SIZE", {"heap_size" : 1024})

def getActiveRtos():
    activeComponents = Database.getActiveComponentIDs()

    for i in range(0, len(activeComponents)):
        if (activeComponents[i] == "FreeRTOS"):
            return "FreeRTOS"
        elif (activeComponents[i] == "ThreadX"):
            return "ThreadX"
        elif (activeComponents[i] == "MicriumOSIII"):
            return "MicriumOSIII"
        elif (activeComponents[i] == "MbedOS"):
            return "MbedOS"

def instantiateComponent(metComponentCommon):
    global metChannelNum
    global metInstance
    global metWaveFormCaptureChnRefs

    Log.writeInfoMessage("Loading Muti-Channel Metrology Driver")

    ############################################################################
    #### Code Generation ####
    ############################################################################
    configName = Variables.get("__CONFIGURATION_NAME")

    Database.activateComponents(["HarmonyCore"])

    metInstance = metComponentCommon.getID()
    Log.writeInfoMessage("Loading Muti-Channel Metrology Driver: {}".format(metInstance))

    featCtrlDependencies = []
    harmDependencies = []

    Database.clearSymbolValue("core", "IPC1_INTERRUPT_ENABLE")
    Database.setSymbolValue("core", "IPC1_INTERRUPT_ENABLE", True)
    Database.clearSymbolValue("core", "IPC1_INTERRUPT_HANDLER")
    Database.setSymbolValue("core", "IPC1_INTERRUPT_HANDLER", "IPC1_InterruptHandler")
    Database.clearSymbolValue("core", "IPC1_CLOCK_ENABLE")
    Database.setSymbolValue("core", "IPC1_CLOCK_ENABLE", True)
    Database.clearSymbolValue("core", "SRAM1_CLOCK_ENABLE")
    Database.setSymbolValue("core", "SRAM1_CLOCK_ENABLE", True)

    drvMCMetRegBaseAddress = metComponentCommon.createHexSymbol("DRV_MCMET_BASE_ADDRESS", None)
    drvMCMetRegBaseAddress.setLabel("Register Base Address")
    drvMCMetRegBaseAddress.setVisible(True)
    drvMCMetRegBaseAddress.setDefaultValue(0x20086000)
    drvMCMetRegBaseAddress.setReadOnly(True)
    drvMCMetRegBaseAddress.setHelp(multichannel_met_helpkeyword)
    
    ##### AFE AND CHANNELS SELECTION #########################################################################
    drvMCMetAFESelection = metComponentCommon.createKeyValueSetSymbol("DRV_MCMET_AFE_SELECTION", None)
    drvMCMetAFESelection.setLabel("AFE Selection")
    drvMCMetAFESelection.setDefaultValue(0)
    drvMCMetAFESelection.setOutputMode("Value")
    drvMCMetAFESelection.setDisplayMode("Description")
    drvMCMetAFESelection.setHelp(multichannel_met_helpkeyword)
    drvMCMetAFESelection.addKey("1xMCP3914", "0", "1xMCP3914 - 8 channels and up to 7 powers")
    drvMCMetAFESelection.addKey("2xMCP3914", "1", "2xMCP3914 - 16 channels and up to 15 powers")
    drvMCMetAFESelection.addKey("1xMCP3913", "4", "1xMCP3913 - 6 channels and up to 5 powers")
    drvMCMetAFESelection.addKey("2xMCP3913", "5", "2xMCP3913 - 12 channels and up to 11 powers")
    
    drvMCMetChannelNum = metComponentCommon.createIntegerSymbol("DRV_MCMET_CHANNELS_NUM", None)
    drvMCMetChannelNum.setLabel("Channels")
    drvMCMetChannelNum.setDefaultValue(metChannelNum)
    drvMCMetChannelNum.setReadOnly(True)
    drvMCMetChannelNum.setVisible(True)
    drvMCMetChannelNum.setDependencies(setNumberOfChannels, ["DRV_MCMET_AFE_SELECTION"])

    sensorType0SymbolList = []
    sensorType1SymbolList = []
    pulseContributionPWSymbolList = []
    pulseContributionISymbolList = []
    pPowerOffsetCtrlSymbolList = []
    qPowerOffsetCtrlSymbolList = []
    captureChannelSymbolList = []
    channelGainSymbolList = []
    mcp3910CfgSymbolList = []
    for channel in range(0, MAX_CHANNELS):
        drvMCMetChannel = metComponentCommon.createBooleanSymbol("DRV_MCMET_CHANNEL_{}".format(channel), drvMCMetAFESelection)
        drvMCMetChannel.setLabel("CHANNEL {}".format(channel))
        drvMCMetChannel.setDefaultValue(True)
        drvMCMetChannel.setReadOnly(True)
        drvMCMetChannel.setDependencies(showChannelsFromAFESelection, ["DRV_MCMET_AFE_SELECTION"])
        if channel >= 8:
            drvMCMetChannel.setVisible(False)

        drvMCMetChannelEnable = metComponentCommon.createBooleanSymbol("DRV_MCMET_CHANNEL_ENABLE_{}".format(channel), drvMCMetChannel)
        drvMCMetChannelEnable.setLabel("Channel Enable")
        drvMCMetChannelEnable.setVisible(False)
        drvMCMetChannelEnable.setDependencies(enableChannelsFromAFESelection, ["DRV_MCMET_AFE_SELECTION"])
        if channel >= 8:
            drvMCMetChannelEnable.setDefaultValue(False)
        else:
            drvMCMetChannelEnable.setDefaultValue(True)

        drvMCMetChannelName = metComponentCommon.createStringSymbol("DRV_MCMET_CHANNEL_NAME_{}".format(channel), drvMCMetChannel)
        drvMCMetChannelName.setLabel("Channel Name")
        drvMCMetChannelName.setDefaultValue("")
        
        KCHxDependencyList = []

        sensorSymbolName = "DRV_MCMET_CHANNEL_SENSOR_{}".format(channel)
        drvMCMetConfST = metComponentCommon.createKeyValueSetSymbol(sensorSymbolName, drvMCMetChannel)
        drvMCMetConfST.setLabel("Sensor Type")
        drvMCMetConfST.setDefaultValue(0)
        drvMCMetConfST.setOutputMode("Key")
        drvMCMetConfST.setDisplayMode("Description")
        drvMCMetConfST.setHelp(multichannel_met_helpkeyword)
        drvMCMetConfST.addKey("Select", "Select", "Select Sensor Type")
        drvMCMetConfST.addKey("CT", MET_SENSOR_CT_VALUE, "Current Transformer")
        drvMCMetConfST.addKey("SHUNT", MET_SENSOR_SHUNT_VALUE, "Resistive Shunt")
        drvMCMetConfST.addKey("ROGOWSKI", MET_SENSOR_COIL_VALUE, "Rogowski Coil Current Sensor")
        drvMCMetConfST.addKey("VRD", MET_SENSOR_VRD_VALUE, "Resistive Divider Voltage Sensor")
        drvMCMetConfST.setDependencies(updateChannelsLists, [sensorSymbolName])
        if channel < 16:
            sensorType0SymbolList.append(sensorSymbolName)
        else:
            sensorType1SymbolList.append(sensorSymbolName)
        
        drvMCMetConfFreq = metComponentCommon.createIntegerSymbol("DRV_MCMET_CONF_F_{}".format(channel), drvMCMetConfST)
        drvMCMetConfFreq.setLabel("Mains Frequency (Hz)")
        drvMCMetConfFreq.setVisible(False)
        drvMCMetConfFreq.setDefaultValue(50)
        drvMCMetConfFreq.setHelp(multichannel_met_helpkeyword)
        drvMCMetConfFreq.setDependencies(updateVisibleF, [sensorSymbolName])
        KCHxDependencyList.append("DRV_MCMET_CONF_F_{}".format(channel))
        
        drvMCMetConfTr = metComponentCommon.createIntegerSymbol("DRV_MCMET_CONF_TR_{}".format(channel), drvMCMetConfST)
        drvMCMetConfTr.setLabel("Transformer Ratio")
        drvMCMetConfTr.setVisible(False)
        drvMCMetConfTr.setDefaultValue(2000)
        drvMCMetConfTr.setHelp(multichannel_met_helpkeyword)
        drvMCMetConfTr.setDependencies(updateVisibleTR, [sensorSymbolName])
        KCHxDependencyList.append("DRV_MCMET_CONF_TR_{}".format(channel))

        drvMCMetConfRl = metComponentCommon.createIntegerSymbol("DRV_MCMET_CONF_RL_{}".format(channel), drvMCMetConfST)
        drvMCMetConfRl.setLabel("Resistor Load (uOhms)")
        drvMCMetConfRl.setVisible(False)
        drvMCMetConfRl.setDefaultValue(3240000)
        drvMCMetConfRl.setHelp(multichannel_met_helpkeyword)
        drvMCMetConfRl.setDependencies(updateVisibleRL, [sensorSymbolName])
        KCHxDependencyList.append("DRV_MCMET_CONF_RL_{}".format(channel))

        drvMCMetConfKU = metComponentCommon.createIntegerSymbol("DRV_MCMET_CONF_KU_{}".format(channel), drvMCMetConfST)
        drvMCMetConfKU.setLabel(" ")
        drvMCMetConfKU.setVisible(False)
        drvMCMetConfKU.setDefaultValue(1651)
        drvMCMetConfKU.setDependencies(updateVisibleKU, [sensorSymbolName])
        drvMCMetConfKU.setHelp(multichannel_met_helpkeyword)
        KCHxDependencyList.append("DRV_MCMET_CONF_KU_{}".format(channel))

        symbolName = "DRV_MCMET_CHANNEL_GAIN_{}".format(channel)
        drvMCMetChannelGain = metComponentCommon.createKeyValueSetSymbol(symbolName, drvMCMetChannel)
        drvMCMetChannelGain.setLabel("Channel Gain")
        drvMCMetChannelGain.setOutputMode("Value")
        drvMCMetChannelGain.setDisplayMode("Description")
        drvMCMetChannelGain.setDefaultValue(0)
        drvMCMetChannelGain.addKey("x1", "0", "x1")
        drvMCMetChannelGain.addKey("x2", "1", "x2")
        drvMCMetChannelGain.addKey("x4", "2", "x4")
        drvMCMetChannelGain.addKey("x8", "3", "x8")
        drvMCMetChannelGain.addKey("x16", "4", "x16")
        drvMCMetChannelGain.addKey("x32", "5", "x32")
        channelGainSymbolList.append(symbolName)
        KCHxDependencyList.append("DRV_MCMET_CHANNEL_GAIN_{}".format(channel))

        drvMCMetConfKCH = metComponentCommon.createHexSymbol("REG_MCMET_CONF_K_CH_{}".format(channel), drvMCMetConfST)
        drvMCMetConfKCH.setLabel("REG_MCMET_CONF_K_CH_{}".format(channel))
        drvMCMetConfKCH.setVisible(False)
        drvMCMetConfKCH.setDefaultValue(0)
        drvMCMetConfKCH.setDependencies(updateKCHxValue, KCHxDependencyList)
        drvMCMetConfKCH.setHelp(multichannel_met_helpkeyword)

        drvMCMetChannelConfig = metComponentCommon.createMenuSymbol("DRV_MCMET_CHANNEL_CONFIGURATION_{}".format(channel), drvMCMetChannel)
        drvMCMetChannelConfig.setLabel("Advanced Configuration")
        drvMCMetChannelConfig.setVisible(False)
        drvMCMetChannelConfig.setDependencies(updateAdvancedConfig,[sensorSymbolName])

        drvMCMetConfSwellEn = metComponentCommon.createBooleanSymbol("DRV_MCMET_CONF_SWELL_EN_{}".format(channel), drvMCMetChannelConfig)
        drvMCMetConfSwellEn.setLabel("SWELL Enable")

        drvMCMetConfSwellThrs = metComponentCommon.createIntegerSymbol("DRV_MCMET_CONF_SWELL_TH_{}".format(channel), drvMCMetConfSwellEn)
        drvMCMetConfSwellThrs.setLabel("SWELL Threshold RMS (mV)")
        drvMCMetConfSwellThrs.setVisible(False)
        drvMCMetConfSwellThrs.setDefaultValue(250800)
        drvMCMetConfSwellThrs.setHelp(multichannel_met_helpkeyword)
        drvMCMetConfSwellThrs.setDependencies(showSymbolOnBoolEvent, ["DRV_MCMET_CONF_SWELL_EN_{}".format(channel)])

        drvMCMetConfSagEn = metComponentCommon.createBooleanSymbol("DRV_MCMET_CONF_SAG_EN_{}".format(channel), drvMCMetChannelConfig)
        drvMCMetConfSagEn.setLabel("SAG Enable")

        drvMCMetConfSagThrsh = metComponentCommon.createIntegerSymbol("DRV_MCMET_CONF_SAG_TH_{}".format(channel), drvMCMetConfSagEn)
        drvMCMetConfSagThrsh.setLabel("SAG Threshold RMS (mV)")
        drvMCMetConfSagThrsh.setVisible(False)
        drvMCMetConfSagThrsh.setDefaultValue(132000)
        drvMCMetConfSagThrsh.setHelp(multichannel_met_helpkeyword)
        drvMCMetConfSagThrsh.setDependencies(showSymbolOnBoolEvent, ["DRV_MCMET_CONF_SAG_EN_{}".format(channel)])
    
    ##### POWER SOURCE SELECTION #####################################################################################
    drvMCMetPowerSourceNum = metComponentCommon.createIntegerSymbol("DRV_MCMET_POWERS_NUM", None)
    drvMCMetPowerSourceNum.setLabel("Power Source Selection")
    drvMCMetPowerSourceNum.setDefaultValue(7)
    drvMCMetPowerSourceNum.setMin(0)
    drvMCMetPowerSourceNum.setMax(7)
    drvMCMetPowerSourceNum.setDependencies(setMaximumPowerSource, ["DRV_MCMET_AFE_SELECTION"])

    for powerId in range(0, MAX_CHANNELS - 1):
        enableSymbolName = "DRV_MCMET_POWER_SOURCE_{}".format(powerId)
        drvMCMetPowerSource = metComponentCommon.createMenuSymbol(enableSymbolName, drvMCMetPowerSourceNum)
        drvMCMetPowerSource.setLabel("POWER {}".format(powerId))
        if powerId >= 7:
            drvMCMetPowerSource.setVisible(False)
        drvMCMetPowerSource.setDependencies(showPowerSource, ["DRV_MCMET_POWERS_NUM"])

        drvMCMetPowerSourceV = metComponentCommon.createComboSymbol("DRV_MCMET_POWER_SOURCE_V_{}".format(powerId), drvMCMetPowerSource, ["Select Channel"])
        drvMCMetPowerSourceV.setLabel("V source")
        drvMCMetPowerSourceV.setDefaultValue("Select Channel")

        drvMCMetPowerSourceI = metComponentCommon.createComboSymbol("DRV_MCMET_POWER_SOURCE_I_{}".format(powerId), drvMCMetPowerSource, ["Select Channel"])
        drvMCMetPowerSourceI.setLabel("I source")
        drvMCMetPowerSourceI.setDefaultValue("Select Channel")

        drvMCMetPowerSourceAdvConfMenu = metComponentCommon.createMenuSymbol("DRV_MCMET_POWER_SOURCE_ADV_COMMON_CONF_MENU_{}".format(powerId), drvMCMetPowerSource)
        drvMCMetPowerSourceAdvConfMenu.setLabel("Advanced Configuration")
        drvMCMetPowerSourceAdvConfMenu.setHelp(multichannel_met_helpkeyword)
        
        drvMCMetConfPHActiveEn = metComponentCommon.createBooleanSymbol("DRV_MCMET_CONF_PHACTIVE_EN_{}".format(powerId), drvMCMetPowerSourceAdvConfMenu)
        drvMCMetConfPHActiveEn.setLabel("Phase Active Enable")

        drvMCMetConfPOffsetEn = metComponentCommon.createBooleanSymbol("DRV_MCMET_CONF_P_OFFSET_EN_{}".format(powerId), drvMCMetPowerSourceAdvConfMenu)
        drvMCMetConfPOffsetEn.setLabel("Active Power Offset Control Enable")
        pPowerOffsetCtrlSymbolList.append("DRV_MCMET_CONF_P_OFFSET_EN_{}".format(powerId))

        drvMCMetConfPOffset = metComponentCommon.createIntegerSymbol("DRV_MCMET_CONF_P_OFFSET_{}".format(powerId), drvMCMetConfPOffsetEn)
        drvMCMetConfPOffset.setLabel("Active Power Offset Register (W samp-scaled)")
        drvMCMetConfPOffset.setVisible(False)
        drvMCMetConfPOffset.setDefaultValue(0)
        drvMCMetConfPOffset.setHelp(multichannel_met_helpkeyword)
        drvMCMetConfPOffset.setDependencies(showSymbolOnBoolEvent, ["DRV_MCMET_CONF_P_OFFSET_EN_{}".format(powerId)])

        drvMCMetConfQOffsetEn = metComponentCommon.createBooleanSymbol("DRV_MCMET_CONF_Q_OFFSET_EN_{}".format(powerId), drvMCMetPowerSourceAdvConfMenu)
        drvMCMetConfQOffsetEn.setLabel("Reactive Power Offset Control Enable")
        qPowerOffsetCtrlSymbolList.append("DRV_MCMET_CONF_Q_OFFSET_EN_{}".format(powerId))

        drvMCMetConfQOffset = metComponentCommon.createIntegerSymbol("DRV_MCMET_CONF_Q_OFFSET_{}".format(powerId), drvMCMetConfQOffsetEn)
        drvMCMetConfQOffset.setLabel("Reactive Power Offset Register (VAR samp-scaled)")
        drvMCMetConfQOffset.setVisible(False)
        drvMCMetConfQOffset.setDefaultValue(0)
        drvMCMetConfQOffset.setHelp(multichannel_met_helpkeyword)
        drvMCMetConfQOffset.setDependencies(showSymbolOnBoolEvent, ["DRV_MCMET_CONF_Q_OFFSET_EN_{}".format(powerId)])

        drvMCMetConfPulseContributionPWEn = metComponentCommon.createBooleanSymbol("DRV_MCMET_CONF_PULSE_CONT_PW_EN_{}".format(powerId), drvMCMetPowerSourceAdvConfMenu)
        drvMCMetConfPulseContributionPWEn.setLabel("Pulse Contribution PW Enable")
        pulseContributionPWSymbolList.append("DRV_MCMET_CONF_PULSE_CONT_PW_EN_{}".format(powerId))

        drvMCMetConfPulseContributionIEn = metComponentCommon.createBooleanSymbol("DRV_MCMET_CONF_PULSE_CONT_I_EN_{}".format(powerId), drvMCMetPowerSourceAdvConfMenu)
        drvMCMetConfPulseContributionIEn.setLabel("Pulse Contribution I Enable")
        pulseContributionISymbolList.append("DRV_MCMET_CONF_PULSE_CONT_I_EN_{}".format(powerId))

        powerSourceSymbolRefs.setdefault(powerId, {'v': drvMCMetPowerSourceV, 'i': drvMCMetPowerSourceI})

    ##### MCP391x CONFIGURATION #####################################################################################
    drvMCMetMCP39xMenu = metComponentCommon.createMenuSymbol("DRV_MCMET_MCP391x", None)
    drvMCMetMCP39xMenu.setLabel("MCP391x devices")
    for mcpDevice in range(0, MAX_MCP391x_SUPPORTED):
        drvMCMetMCP391xConf = metComponentCommon.createMenuSymbol("DRV_MCMET_MCP391x_{}".format(mcpDevice), drvMCMetMCP39xMenu)
        drvMCMetMCP391xConf.setLabel("MCP391x device {}".format(mcpDevice))
        if mcpDevice > 0:
            drvMCMetMCP391xConf.setVisible(False)
        drvMCMetMCP391xConf.setDependencies(showMCPDevicesFromAFESelection, ["DRV_MCMET_AFE_SELECTION"])
        
        symbolName = "DRV_MCMET_MCP391x_DITHER_{}".format(mcpDevice)
        drvMCMetDither = metComponentCommon.createKeyValueSetSymbol(symbolName, drvMCMetMCP391xConf)
        drvMCMetDither.setLabel("Dithering circuit")
        drvMCMetDither.setDefaultValue(3)
        drvMCMetDither.setOutputMode("Key")
        drvMCMetDither.setDisplayMode("Description")
        drvMCMetDither.addKey("0", "0", "Dithering turned off")
        drvMCMetDither.addKey("1", "1", "Dithering on, Minimum Strength")
        drvMCMetDither.addKey("2", "2", "Dithering on, Medium Strength")
        drvMCMetDither.addKey("3", "3", "Dithering on, Maximum Strength")

        symbolName = "DRV_MCMET_MCP391x_BOOST_{}".format(mcpDevice)
        drvMCMetBoost = metComponentCommon.createKeyValueSetSymbol(symbolName, drvMCMetMCP391xConf)
        drvMCMetBoost.setLabel("Bias Current selection")
        drvMCMetBoost.setDefaultValue(2)
        drvMCMetBoost.setOutputMode("Key")
        drvMCMetBoost.setDisplayMode("Description")
        drvMCMetBoost.addKey("0", "0", "All channels have current x 0.5")
        drvMCMetBoost.addKey("1", "1", "All channels have current x 0.66")
        drvMCMetBoost.addKey("2", "2", "All channels have current x 1")
        drvMCMetBoost.addKey("3", "3", "All channels have current x 2")


    ##### WAVEFORM CAPTURE CONFIGURATION ############################################################################
    drvMCMetWaveformCapture = metComponentCommon.createBooleanSymbol("DRV_MCMET_WAVEFORM_CAPTURE", None)
    drvMCMetWaveformCapture.setLabel("Waveform Capture")
    drvMCMetWaveformCapture.setDefaultValue(False)
    drvMCMetWaveformCapture.setHelp(multichannel_met_helpkeyword)

    for channel in range(0, MAX_CHANNELS):
        drvMCMetWaveformCaptureChnSel = metComponentCommon.createBooleanSymbol("DRV_MCMET_WAVECAPT_SEL_CH_{}".format(channel), drvMCMetWaveformCapture)
        drvMCMetWaveformCaptureChnSel.setLabel("Capture Channel {}".format(channel))
        drvMCMetWaveformCaptureChnSel.setDefaultValue(False)
        drvMCMetWaveformCaptureChnSel.setVisible(False)
        drvMCMetWaveformCaptureChnSel.setDependencies(showCaptureSymbol, ["DRV_MCMET_WAVEFORM_CAPTURE"])
        
        metWaveFormCaptureChnRefs.append(drvMCMetWaveformCaptureChnSel)
        captureChannelSymbolList.append("DRV_MCMET_WAVECAPT_SEL_CH_{}".format(channel))

    drvMCMetWaveformCaptureSrc = metComponentCommon.createKeyValueSetSymbol("DRV_MCMET_WAVECAPT_SRC", drvMCMetWaveformCapture)
    drvMCMetWaveformCaptureSrc.setLabel("Capture Source")
    drvMCMetWaveformCaptureSrc.setDefaultValue(0)
    drvMCMetWaveformCaptureSrc.setVisible(False)
    drvMCMetWaveformCaptureSrc.setOutputMode("Key")
    drvMCMetWaveformCaptureSrc.setDisplayMode("Description")
    drvMCMetWaveformCaptureSrc.setHelp(multichannel_met_helpkeyword)
    drvMCMetWaveformCaptureSrc.addKey("16KHz", "0", "16KHz data")
    drvMCMetWaveformCaptureSrc.addKey("4KHz_FBW", "1", "4KHz Full Bandwidth")
    drvMCMetWaveformCaptureSrc.addKey("4KHz_NBW", "2", "4KHz Narrow Bandwidth")
    drvMCMetWaveformCaptureSrc.addKey("8KHz_FBW", "3", "8KHz Full Bandwidth")
    drvMCMetWaveformCaptureSrc.setDependencies(showSymbolOnBoolEvent, ["DRV_MCMET_WAVEFORM_CAPTURE"])

    drvMCMetWaveformCaptureType = metComponentCommon.createKeyValueSetSymbol("DRV_MCMET_WAVECAPT_TYPE", drvMCMetWaveformCapture)
    drvMCMetWaveformCaptureType.setLabel("Capture Type")
    drvMCMetWaveformCaptureType.setDefaultValue(0)
    drvMCMetWaveformCaptureType.setVisible(False)
    drvMCMetWaveformCaptureType.setOutputMode("Key")
    drvMCMetWaveformCaptureType.setDisplayMode("Description")
    drvMCMetWaveformCaptureType.setHelp(multichannel_met_helpkeyword)
    drvMCMetWaveformCaptureType.addKey("One-shoot", "0", "One-shot capture")
    drvMCMetWaveformCaptureType.addKey("Continuous", "1", "Continuous capture")
    drvMCMetWaveformCaptureType.setDependencies(showSymbolOnBoolEvent, ["DRV_MCMET_WAVEFORM_CAPTURE"])

    drvMCMetCaptureBufSizeChn = metComponentCommon.createIntegerSymbol("DRV_MCMET_CAPTURE_BUF_SIZE_CHN", drvMCMetWaveformCapture)
    drvMCMetCaptureBufSizeChn.setLabel("Samples per Channel")
    drvMCMetCaptureBufSizeChn.setVisible(False)
    drvMCMetCaptureBufSizeChn.setDefaultValue(8000)
    drvMCMetCaptureBufSizeChn.setDependencies(showSymbolOnBoolEvent, ["DRV_MCMET_WAVEFORM_CAPTURE"])
    drvMCMetCaptureBufSizeChn.setHelp(multichannel_met_helpkeyword)

    drvMCMetCaptureChnNum = metComponentCommon.createIntegerSymbol("DRV_MCMET_CAPTURE_CHN_NUM", drvMCMetWaveformCapture)
    drvMCMetCaptureChnNum.setLabel("Number of Channels")
    drvMCMetCaptureChnNum.setVisible(False)
    drvMCMetCaptureChnNum.setDefaultValue(0)
    drvMCMetCaptureChnNum.setDependencies(updateCaptureNumberChannels, captureChannelSymbolList)
    drvMCMetCaptureChnNum.setHelp(multichannel_met_helpkeyword)

    ##### ADDITIONAL NOTIFICATIONS (IPC) #########################################################################
    drvMCMetNotificationMenu = metComponentCommon.createMenuSymbol("DRV_MCMET_NOTIFICATION_MENU", None)
    drvMCMetNotificationMenu.setLabel("Additional Notifications")
    drvMCMetNotificationMenu.setHelp(multichannel_met_helpkeyword)

    drvMCMetNotFullCycle = metComponentCommon.createBooleanSymbol("DRV_MCMET_NOT_FULL_CYCLE", drvMCMetNotificationMenu)
    drvMCMetNotFullCycle.setLabel("Line Cycle")
    drvMCMetNotFullCycle.setDefaultValue(False)
    drvMCMetNotFullCycle.setHelp(multichannel_met_helpkeyword)

    drvMCMetNotHalfCycle = metComponentCommon.createBooleanSymbol("DRV_MCMET_NOT_HALF_CYCLE", drvMCMetNotificationMenu)
    drvMCMetNotHalfCycle.setLabel("Half Line Cycle")
    drvMCMetNotHalfCycle.setDefaultValue(False)
    drvMCMetNotHalfCycle.setHelp(multichannel_met_helpkeyword)

    drvMCMetNotRawZeroCrossing = metComponentCommon.createBooleanSymbol("DRV_MCMET_RAW_ZERO_CROSSING", drvMCMetNotificationMenu)
    drvMCMetNotRawZeroCrossing.setLabel("Raw Zero Crossing")
    drvMCMetNotRawZeroCrossing.setDefaultValue(False)
    drvMCMetNotRawZeroCrossing.setHelp(multichannel_met_helpkeyword)

    drvMCMetNotPulse0 = metComponentCommon.createBooleanSymbol("DRV_MCMET_PULSE_0", drvMCMetNotificationMenu)
    drvMCMetNotPulse0.setLabel("Pulse 0")
    drvMCMetNotPulse0.setDefaultValue(False)
    drvMCMetNotPulse0.setHelp(multichannel_met_helpkeyword)

    drvMCMetNotPulse1 = metComponentCommon.createBooleanSymbol("DRV_MCMET_PULSE_1", drvMCMetNotificationMenu)
    drvMCMetNotPulse1.setLabel("Pulse 1")
    drvMCMetNotPulse1.setDefaultValue(False)
    drvMCMetNotPulse1.setHelp(multichannel_met_helpkeyword)

    drvMCMetNotPulse2 = metComponentCommon.createBooleanSymbol("DRV_MCMET_PULSE_2", drvMCMetNotificationMenu)
    drvMCMetNotPulse2.setLabel("Pulse 2")
    drvMCMetNotPulse2.setDefaultValue(False)
    drvMCMetNotPulse2.setHelp(multichannel_met_helpkeyword)

    ##### COMMON CONFIGURATION ############################################################################
    drvMCMetAdvConfMenu = metComponentCommon.createMenuSymbol("DRV_MCMET_ADV_COMMON_CONF_MENU", None)
    drvMCMetAdvConfMenu.setLabel("Advanced Configuration")
    drvMCMetAdvConfMenu.setHelp(multichannel_met_helpkeyword)

    drvMCMetConfF = metComponentCommon.createIntegerSymbol("DRV_MCMET_CONF_FREQ", drvMCMetAdvConfMenu)
    drvMCMetConfF.setLabel("Frequency [Hz]")
    drvMCMetConfF.setDefaultValue(50)
    drvMCMetConfF.setHelp(multichannel_met_helpkeyword)

    drvMCMetConfPKT = metComponentCommon.createIntegerSymbol("DRV_MCMET_CONF_PKT", drvMCMetAdvConfMenu)
    drvMCMetConfPKT.setLabel("Meter Constant P [Pulses/KWh]")
    drvMCMetConfPKT.setDefaultValue(3200)
    drvMCMetConfPKT.setHelp(multichannel_met_helpkeyword)

    drvMCMetConfQKT = metComponentCommon.createIntegerSymbol("DRV_MCMET_CONF_QKT", drvMCMetAdvConfMenu)
    drvMCMetConfQKT.setLabel("Meter Constant Q [Pulses/KVARh]")
    drvMCMetConfQKT.setDefaultValue(3200)
    drvMCMetConfQKT.setHelp(multichannel_met_helpkeyword)

    drvMCMetConfIKT = metComponentCommon.createIntegerSymbol("DRV_MCMET_CONF_IKT", drvMCMetAdvConfMenu)
    drvMCMetConfIKT.setLabel("Meter Constant I [Pulses/A2h]")
    drvMCMetConfIKT.setDefaultValue(3200)
    drvMCMetConfIKT.setHelp(multichannel_met_helpkeyword)

    drvMCMetConfCreepPEn = metComponentCommon.createBooleanSymbol("DRV_MCMET_CONF_CREEP_P_EN", drvMCMetAdvConfMenu)
    drvMCMetConfCreepPEn.setLabel("Active Power Creep Threshold Enable")

    drvMCMetConfCreepPThrsh = metComponentCommon.createIntegerSymbol("DRV_MCMET_CONF_CREEP_P_THRES", drvMCMetConfCreepPEn)
    drvMCMetConfCreepPThrsh.setLabel("Creep Active Energy (Wh)")
    drvMCMetConfCreepPThrsh.setVisible(False)
    drvMCMetConfCreepPThrsh.setDefaultValue(2)
    drvMCMetConfCreepPThrsh.setHelp(multichannel_met_helpkeyword)
    drvMCMetConfCreepPThrsh.setDependencies(showSymbolOnBoolEvent, ["DRV_MCMET_CONF_CREEP_P_EN"])

    drvMCMetConfCreepQEn = metComponentCommon.createBooleanSymbol("DRV_MCMET_CONF_CREEP_Q_EN", drvMCMetAdvConfMenu)
    drvMCMetConfCreepQEn.setLabel("Reactive Power Creep Threshold Enable")

    drvMCMetConfCreepQThrsh = metComponentCommon.createIntegerSymbol("DRV_MCMET_CONF_CREEP_Q_THRES", drvMCMetConfCreepQEn)
    drvMCMetConfCreepQThrsh.setLabel("Creep Reactive Energy (VARh)")
    drvMCMetConfCreepQThrsh.setVisible(False)
    drvMCMetConfCreepQThrsh.setDefaultValue(2)
    drvMCMetConfCreepQThrsh.setHelp(multichannel_met_helpkeyword)
    drvMCMetConfCreepQThrsh.setDependencies(showSymbolOnBoolEvent, ["DRV_MCMET_CONF_CREEP_Q_EN"])

    drvMCMetConfCreepIEn = metComponentCommon.createBooleanSymbol("DRV_MCMET_CONF_CREEP_I_EN", drvMCMetAdvConfMenu)
    drvMCMetConfCreepIEn.setLabel("Current Creep Threshold Enable")

    drvMCMetConfCreepIThrsh = metComponentCommon.createIntegerSymbol("DRV_MCMET_CONF_CREEP_I_THRES", drvMCMetConfCreepIEn)
    drvMCMetConfCreepIThrsh.setLabel("Creep Current (mA)")
    drvMCMetConfCreepIThrsh.setVisible(False)
    drvMCMetConfCreepIThrsh.setDefaultValue(5)
    drvMCMetConfCreepIThrsh.setHelp(multichannel_met_helpkeyword)
    drvMCMetConfCreepIThrsh.setDependencies(showSymbolOnBoolEvent, ["DRV_MCMET_CONF_CREEP_I_EN"])

    drvMCMetConfPOffsetEn = metComponentCommon.createBooleanSymbol("DRV_MCMET_CONF_POWER_OFFSET_P_EN", drvMCMetAdvConfMenu)
    drvMCMetConfPOffsetEn.setLabel("Active Power Offset Enable")
    pPowerOffsetCtrlSymbolList.append("DRV_MCMET_CONF_POWER_OFFSET_P_EN")

    drvMCMetConfPOffsetThrsh = metComponentCommon.createIntegerSymbol("DRV_MCMET_CONF_POWER_OFFSET_P", drvMCMetConfPOffsetEn)
    drvMCMetConfPOffsetThrsh.setLabel("Active Power Offset (mWh)")
    drvMCMetConfPOffsetThrsh.setVisible(False)
    drvMCMetConfPOffsetThrsh.setDefaultValue(0)
    drvMCMetConfPOffsetThrsh.setHelp(multichannel_met_helpkeyword)
    drvMCMetConfPOffsetThrsh.setDependencies(showSymbolOnBoolEvent, ["DRV_MCMET_CONF_POWER_OFFSET_P_EN"])

    drvMCMetConfQOffsetEn = metComponentCommon.createBooleanSymbol("DRV_MCMET_CONF_POWER_OFFSET_Q_EN", drvMCMetAdvConfMenu)
    drvMCMetConfQOffsetEn.setLabel("Reactive Power Offset Enable")
    qPowerOffsetCtrlSymbolList.append("DRV_MCMET_CONF_POWER_OFFSET_Q_EN")

    drvMCMetConfQOffsetThrsh = metComponentCommon.createIntegerSymbol("DRV_MCMET_CONF_POWER_OFFSET_Q", drvMCMetConfQOffsetEn)
    drvMCMetConfQOffsetThrsh.setLabel("Reactive Power Offset (mVARh)")
    drvMCMetConfQOffsetThrsh.setVisible(False)
    drvMCMetConfQOffsetThrsh.setDefaultValue(0)
    drvMCMetConfQOffsetThrsh.setHelp(multichannel_met_helpkeyword)
    drvMCMetConfQOffsetThrsh.setDependencies(showSymbolOnBoolEvent, ["DRV_MCMET_CONF_POWER_OFFSET_Q_EN"])

    drvMCMetConfPulse0 = metComponentCommon.createMenuSymbol("DRV_MCMET_CONF_PULSE0", drvMCMetAdvConfMenu)
    drvMCMetConfPulse0.setLabel("Pulse 0 Control")
    drvMCMetConfPulse0.setHelp(multichannel_met_helpkeyword)

    drvMCMetConfP0En = metComponentCommon.createBooleanSymbol("DRV_MCMET_CONF_EN_P0", drvMCMetConfPulse0)
    drvMCMetConfP0En.setLabel("Output Pulse Enable (PD17)")
    drvMCMetConfP0En.setDefaultValue(1)
    drvMCMetConfP0En.setHelp(multichannel_met_helpkeyword)

    drvMCMetConfP0Detent = metComponentCommon.createKeyValueSetSymbol("DRV_MCMET_CONF_DET_P0", drvMCMetConfPulse0)
    drvMCMetConfP0Detent.setLabel("Total Absolute Values (P/Q)")
    drvMCMetConfP0Detent.setDefaultValue(0)
    drvMCMetConfP0Detent.setOutputMode("Key")
    drvMCMetConfP0Detent.setDisplayMode("Description")
    drvMCMetConfP0Detent.setHelp(multichannel_met_helpkeyword)
    drvMCMetConfP0Detent.addKey("NET", "0", "NET")
    drvMCMetConfP0Detent.addKey("ABSOLUTE", "1", "ABSOLUTE")
    drvMCMetConfP0Detent.addKey("DELIVERED", "2", "DELIVERED")
    drvMCMetConfP0Detent.addKey("GENERATED", "3", "GENERATED")

    drvMCMetConfP0Polarity = metComponentCommon.createKeyValueSetSymbol("DRV_MCMET_CONF_POL_P0", drvMCMetConfPulse0)
    drvMCMetConfP0Polarity.setLabel("Polarity")
    drvMCMetConfP0Polarity.setDefaultValue(1)
    drvMCMetConfP0Polarity.setOutputMode("Key")
    drvMCMetConfP0Polarity.setDisplayMode("Description")
    drvMCMetConfP0Polarity.setHelp(multichannel_met_helpkeyword)
    drvMCMetConfP0Polarity.addKey("PULSE_LOW", "0", "PULSE_LOW")
    drvMCMetConfP0Polarity.addKey("PULSE_HIGH", "1", "PULSE_HIGH")

    drvMCMetConfP0Type = metComponentCommon.createKeyValueSetSymbol("DRV_MCMET_CONF_TYP_P0", drvMCMetConfPulse0)
    drvMCMetConfP0Type.setLabel("Type")
    drvMCMetConfP0Type.setDefaultValue(0)
    drvMCMetConfP0Type.setOutputMode("Key")
    drvMCMetConfP0Type.setDisplayMode("Description")
    drvMCMetConfP0Type.setHelp(multichannel_met_helpkeyword)
    drvMCMetConfP0Type.addKey("P_T", "0", "P_T")
    drvMCMetConfP0Type.addKey("P_T_F", "1", "P_T_F")
    drvMCMetConfP0Type.addKey("Q_T", "2", "Q_T")
    drvMCMetConfP0Type.addKey("Q_T_F", "3", "Q_T_F")
    drvMCMetConfP0Type.addKey("I_T", "4", "I_T")
    drvMCMetConfP0Type.addKey("I_T_F", "5", "I_T_F")

    drvMCMetConfP0Width = metComponentCommon.createHexSymbol("DRV_MCMET_CONF_WID_P0", drvMCMetConfPulse0)
    drvMCMetConfP0Width.setLabel("Width")
    drvMCMetConfP0Width.setVisible(True)
    drvMCMetConfP0Width.setDefaultValue(0x9100)
    drvMCMetConfP0Width.setHelp(multichannel_met_helpkeyword)

    drvMCMetConfPulse1 = metComponentCommon.createMenuSymbol("DRV_MCMET_CONF_PULSE1", drvMCMetAdvConfMenu)
    drvMCMetConfPulse1.setLabel("Pulse 1 Control")
    drvMCMetConfPulse1.setHelp(multichannel_met_helpkeyword)

    drvMCMetConfP1En = metComponentCommon.createBooleanSymbol("DRV_MCMET_CONF_EN_P1", drvMCMetConfPulse1)
    drvMCMetConfP1En.setLabel("Output Pulse Enable (PD18)")
    drvMCMetConfP1En.setDefaultValue(1)
    drvMCMetConfP1En.setHelp(multichannel_met_helpkeyword)

    drvMCMetConfP1Detent = metComponentCommon.createKeyValueSetSymbol("DRV_MCMET_CONF_DET_P1", drvMCMetConfPulse1)
    drvMCMetConfP1Detent.setLabel("Total Absolute Values (P/Q)")
    drvMCMetConfP1Detent.setDefaultValue(0)
    drvMCMetConfP1Detent.setOutputMode("Key")
    drvMCMetConfP1Detent.setDisplayMode("Description")
    drvMCMetConfP1Detent.setHelp(multichannel_met_helpkeyword)
    drvMCMetConfP1Detent.addKey("NET", "0", "NET")
    drvMCMetConfP1Detent.addKey("ABSOLUTE", "1", "ABSOLUTE")
    drvMCMetConfP1Detent.addKey("DELIVERED", "2", "DELIVERED")
    drvMCMetConfP1Detent.addKey("GENERATED", "3", "GENERATED")

    drvMCMetConfP1Polarity = metComponentCommon.createKeyValueSetSymbol("DRV_MCMET_CONF_POL_P1", drvMCMetConfPulse1)
    drvMCMetConfP1Polarity.setLabel("Polarity")
    drvMCMetConfP1Polarity.setDefaultValue(1)
    drvMCMetConfP1Polarity.setOutputMode("Key")
    drvMCMetConfP1Polarity.setDisplayMode("Description")
    drvMCMetConfP1Polarity.setHelp(multichannel_met_helpkeyword)
    drvMCMetConfP1Polarity.addKey("PULSE_LOW", "0", "PULSE_LOW")
    drvMCMetConfP1Polarity.addKey("PULSE_HIGH", "1", "PULSE_HIGH")

    drvMCMetConfP1Type = metComponentCommon.createKeyValueSetSymbol("DRV_MCMET_CONF_TYP_P1", drvMCMetConfPulse1)
    drvMCMetConfP1Type.setLabel("Type")
    drvMCMetConfP1Type.setDefaultValue(2)
    drvMCMetConfP1Type.setOutputMode("Key")
    drvMCMetConfP1Type.setDisplayMode("Description")
    drvMCMetConfP1Type.setHelp(multichannel_met_helpkeyword)
    drvMCMetConfP1Type.addKey("P_T", "0", "P_T")
    drvMCMetConfP1Type.addKey("P_T_F", "1", "P_T_F")
    drvMCMetConfP1Type.addKey("Q_T", "2", "Q_T")
    drvMCMetConfP1Type.addKey("Q_T_F", "3", "Q_T_F")
    drvMCMetConfP1Type.addKey("I_T", "4", "I_T")
    drvMCMetConfP1Type.addKey("I_T_F", "5", "I_T_F")

    drvMCMetConfP1Width = metComponentCommon.createHexSymbol("DRV_MCMET_CONF_WID_P1", drvMCMetConfPulse1)
    drvMCMetConfP1Width.setLabel("Width")
    drvMCMetConfP1Width.setVisible(True)
    drvMCMetConfP1Width.setDefaultValue(0x9100)
    drvMCMetConfP1Width.setHelp(multichannel_met_helpkeyword)

    drvMCMetConfPulse2 = metComponentCommon.createMenuSymbol("DRV_MCMET_CONF_PULSE2", drvMCMetAdvConfMenu)
    drvMCMetConfPulse2.setLabel("Pulse 2 Control")
    drvMCMetConfPulse2.setHelp(multichannel_met_helpkeyword)

    drvMCMetConfP2En = metComponentCommon.createBooleanSymbol("DRV_MCMET_CONF_EN_P2", drvMCMetConfPulse2)
    drvMCMetConfP2En.setLabel("Output Pulse Enable (PD19)")
    drvMCMetConfP2En.setDefaultValue(0)
    drvMCMetConfP2En.setHelp(multichannel_met_helpkeyword)

    drvMCMetConfP2Detent = metComponentCommon.createKeyValueSetSymbol("DRV_MCMET_CONF_DET_P2", drvMCMetConfPulse2)
    drvMCMetConfP2Detent.setLabel("Total Absolute Values (P/Q)")
    drvMCMetConfP2Detent.setDefaultValue(0)
    drvMCMetConfP2Detent.setOutputMode("Key")
    drvMCMetConfP2Detent.setDisplayMode("Description")
    drvMCMetConfP2Detent.setHelp(multichannel_met_helpkeyword)
    drvMCMetConfP2Detent.addKey("NET", "0", "NET")
    drvMCMetConfP2Detent.addKey("ABSOLUTE", "1", "ABSOLUTE")
    drvMCMetConfP2Detent.addKey("DELIVERED", "2", "DELIVERED")
    drvMCMetConfP2Detent.addKey("GENERATED", "3", "GENERATED")

    drvMCMetConfP2Polarity = metComponentCommon.createKeyValueSetSymbol("DRV_MCMET_CONF_POL_P2", drvMCMetConfPulse2)
    drvMCMetConfP2Polarity.setLabel("Polarity")
    drvMCMetConfP2Polarity.setDefaultValue(0)
    drvMCMetConfP2Polarity.setOutputMode("Key")
    drvMCMetConfP2Polarity.setDisplayMode("Description")
    drvMCMetConfP2Polarity.setHelp(multichannel_met_helpkeyword)
    drvMCMetConfP2Polarity.addKey("PULSE_LOW", "0", "PULSE_LOW")
    drvMCMetConfP2Polarity.addKey("PULSE_HIGH", "1", "PULSE_HIGH")

    drvMCMetConfP2Type = metComponentCommon.createKeyValueSetSymbol("DRV_MCMET_CONF_TYP_P2", drvMCMetConfPulse2)
    drvMCMetConfP2Type.setLabel("Type")
    drvMCMetConfP2Type.setDefaultValue(0)
    drvMCMetConfP2Type.setOutputMode("Key")
    drvMCMetConfP2Type.setDisplayMode("Description")
    drvMCMetConfP2Type.setHelp(multichannel_met_helpkeyword)
    drvMCMetConfP2Type.addKey("P_T", "0", "P_T")
    drvMCMetConfP2Type.addKey("P_T_F", "1", "P_T_F")
    drvMCMetConfP2Type.addKey("Q_T", "2", "Q_T")
    drvMCMetConfP2Type.addKey("Q_T_F", "3", "Q_T_F")
    drvMCMetConfP2Type.addKey("I_T", "4", "I_T")
    drvMCMetConfP2Type.addKey("I_T_F", "5", "I_T_F")

    drvMCMetConfP2Width = metComponentCommon.createHexSymbol("DRV_MCMET_CONF_WID_P2", drvMCMetConfPulse2)
    drvMCMetConfP2Width.setLabel("Width")
    drvMCMetConfP2Width.setVisible(True)
    drvMCMetConfP2Width.setDefaultValue(0x0000)
    drvMCMetConfP2Width.setHelp(multichannel_met_helpkeyword)

    # #####################################################################################################################################
    # HARMONICS PARAMS

    # drvMCMetConfHarmonics = metComponentCommon.createMenuSymbol("DRV_MCMET_CONF_HARMONICS", None)
    # drvMCMetConfHarmonics.setLabel("Harmonics Analysis")
    # drvMCMetConfHarmonics.setHelp(multichannel_met_helpkeyword)

    # drvMCMetConfHarmonicsChannel = metComponentCommon.createMenuSymbol("DRV_MCMET_CONF_HARMONICS_CHANNELS", drvMCMetConfHarmonics)
    # drvMCMetConfHarmonicsChannel.setLabel("Enable Channels")
    # drvMCMetConfHarmonicsChannel.setHelp(multichannel_met_helpkeyword)

    # drvMCMetConfHarmonicsChannelIA = metComponentCommon.createBooleanSymbol("DRV_MCMET_CONF_HARMONICS_CHN_IA", drvMCMetConfHarmonicsChannel)
    # drvMCMetConfHarmonicsChannelIA.setLabel("Enable IA")
    # drvMCMetConfHarmonicsChannelIA.setDefaultValue(1)
    # drvMCMetConfHarmonicsChannelIA.setHelp(multichannel_met_helpkeyword)
    # featCtrlDependencies.append("DRV_MCMET_CONF_HARMONICS_CHN_IA")

    # drvMCMetConfHarmonicsChannelVA = metComponentCommon.createBooleanSymbol("DRV_MCMET_CONF_HARMONICS_CHN_VA", drvMCMetConfHarmonicsChannel)
    # drvMCMetConfHarmonicsChannelVA.setLabel("Enable VA")
    # drvMCMetConfHarmonicsChannelVA.setDefaultValue(1)
    # drvMCMetConfHarmonicsChannelVA.setHelp(multichannel_met_helpkeyword)
    # featCtrlDependencies.append("DRV_MCMET_CONF_HARMONICS_CHN_VA")

    # drvMCMetConfHarmonicsChannelIB = metComponentCommon.createBooleanSymbol("DRV_MCMET_CONF_HARMONICS_CHN_IB", drvMCMetConfHarmonicsChannel)
    # drvMCMetConfHarmonicsChannelIB.setLabel("Enable IB")
    # drvMCMetConfHarmonicsChannelIB.setDefaultValue(1)
    # drvMCMetConfHarmonicsChannelIB.setHelp(multichannel_met_helpkeyword)
    # featCtrlDependencies.append("DRV_MCMET_CONF_HARMONICS_CHN_IB")

    # drvMCMetConfHarmonicsChannelVB = metComponentCommon.createBooleanSymbol("DRV_MCMET_CONF_HARMONICS_CHN_VB", drvMCMetConfHarmonicsChannel)
    # drvMCMetConfHarmonicsChannelVB.setLabel("Enable VB")
    # drvMCMetConfHarmonicsChannelVB.setDefaultValue(1)
    # drvMCMetConfHarmonicsChannelVB.setHelp(multichannel_met_helpkeyword)
    # featCtrlDependencies.append("DRV_MCMET_CONF_HARMONICS_CHN_VB")

    # drvMCMetConfHarmonicsChannelIC = metComponentCommon.createBooleanSymbol("DRV_MCMET_CONF_HARMONICS_CHN_IC", drvMCMetConfHarmonicsChannel)
    # drvMCMetConfHarmonicsChannelIC.setLabel("Enable IC")
    # drvMCMetConfHarmonicsChannelIC.setVisible(enablePhase3)
    # drvMCMetConfHarmonicsChannelIC.setDefaultValue(1)
    # drvMCMetConfHarmonicsChannelIC.setHelp(multichannel_met_helpkeyword)
    # featCtrlDependencies.append("DRV_MCMET_CONF_HARMONICS_CHN_IC")

    # drvMCMetConfHarmonicsChannelVC = metComponentCommon.createBooleanSymbol("DRV_MCMET_CONF_HARMONICS_CHN_VC", drvMCMetConfHarmonicsChannel)
    # drvMCMetConfHarmonicsChannelVC.setLabel("Enable VC")
    # drvMCMetConfHarmonicsChannelVC.setVisible(enablePhase3)
    # drvMCMetConfHarmonicsChannelVC.setDefaultValue(1)
    # drvMCMetConfHarmonicsChannelVC.setHelp(multichannel_met_helpkeyword)
    # featCtrlDependencies.append("DRV_MCMET_CONF_HARMONICS_CHN_VC")

    # drvMCMetConfHarmonicsChannelIN = metComponentCommon.createBooleanSymbol("DRV_MCMET_CONF_HARMONICS_CHN_IN", drvMCMetConfHarmonicsChannel)
    # drvMCMetConfHarmonicsChannelIN.setLabel("Enable IN")
    # drvMCMetConfHarmonicsChannelIN.setDefaultValue(1)
    # drvMCMetConfHarmonicsChannelIN.setHelp(multichannel_met_helpkeyword)
    # featCtrlDependencies.append("DRV_MCMET_CONF_HARMONICS_CHN_IN")

    # drvMCMetConfHarmonicsOrder = metComponentCommon.createMenuSymbol("DRV_MCMET_CONF_HARMONICS_ORDER", drvMCMetConfHarmonics)
    # drvMCMetConfHarmonicsOrder.setLabel("Enable Harmonics Order")
    # drvMCMetConfHarmonicsOrder.setHelp(multichannel_met_helpkeyword)

    # for x in range(31):
    #     symbolName = "DRV_MCMET_CONF_HARM{}".format(x+1)
    #     drvMCMetConfHarm = metComponentCommon.createBooleanSymbol(symbolName, drvMCMetConfHarmonicsOrder)
    #     drvMCMetConfHarm.setLabel("Enable Harmonic {}".format(x+1))
    #     drvMCMetConfHarm.setDefaultValue(0)
    #     drvMCMetConfHarm.setHelp(multichannel_met_helpkeyword)
    #     harmDependencies.append(symbolName)

    # #####################################################################################################################################
    # # REGISTER VALUES (hex)

    reg = metComponentCommon.createHexSymbol("REG_MCMET_CTRL_FCTRL0", None)
    reg.setLabel("REG_MCMET_CTRL_FCTRL0")
    reg.setDefaultValue(REG_FCTRL0_DEFAULT)
    reg.setVisible(False)
    reg.setReadOnly(True)
    reg.setDependencies(updateRegFCTRL0, ["DRV_MCMET_POWERS_NUM"])

    reg = metComponentCommon.createHexSymbol("REG_MCMET_CTRL_FCTRL1", None)
    reg.setLabel("REG_MCMET_CTRL_FCTRL1")
    reg.setDefaultValue(REG_FCTRL1_DEFAULT)
    reg.setVisible(False)
    reg.setReadOnly(True)
    reg.setDependencies(updateRegFCTRL1, ["DRV_MCMET_CHANNELS_NUM", "DRV_MCMET_CONF_CREEP_P_EN", "DRV_MCMET_CONF_CREEP_Q_EN", "DRV_MCMET_CONF_CREEP_I_EN"])

    for index in range(0, 16):
        reg = metComponentCommon.createHexSymbol("REG_MCMET_CTRL_PSRC_{}".format(index), None)
        reg.setLabel("REG_MCMET_CTRL_PSRC_{}".format(index))
        reg.setDefaultValue(0)
        reg.setVisible(False)
        reg.setReadOnly(True)

    symbolList = []
    for powerId in range(0, MAX_CHANNELS - 1):
        symbolList.append("DRV_MCMET_POWER_SOURCE_V_{}".format(powerId))
        symbolList.append("DRV_MCMET_POWER_SOURCE_I_{}".format(powerId))

    reg.setDependencies(updateRegsPowerSources, symbolList)

    reg = metComponentCommon.createHexSymbol("REG_MCMET_CTRL_ST0", None)
    reg.setLabel("REG_MCMET_CTRL_ST0")
    reg.setVisible(False)
    reg.setReadOnly(True)
    reg.setDependencies(updateRegSensorType0, sensorType0SymbolList)

    reg = metComponentCommon.createHexSymbol("REG_MCMET_CTRL_ST1", None)
    reg.setLabel("REG_MCMET_CTRL_ST1")
    reg.setVisible(False)
    reg.setReadOnly(True)
    reg.setDependencies(updateRegSensorType1, sensorType1SymbolList)

    reg = metComponentCommon.createHexSymbol("REG_MCMET_CTRL_PULSE_CTRL_0", None)
    reg.setLabel("REG_MCMET_CTRL_PULSE_CTRL_0")
    reg.setVisible(False)
    reg.setDefaultValue(0x810001D0)
    reg.setReadOnly(True)
    reg.setDependencies(updateRegPulseXCtrl, ["DRV_MCMET_CONF_EN_P0", "DRV_MCMET_CONF_DET_P0", "DRV_MCMET_CONF_POL_P0", "DRV_MCMET_CONF_TYP_P0", "DRV_MCMET_CONF_WID_P0"])

    reg = metComponentCommon.createHexSymbol("REG_MCMET_CTRL_PULSE_CTRL_1", None)
    reg.setLabel("REG_MCMET_CTRL_PULSE_CTRL_1")
    reg.setVisible(False)
    reg.setDefaultValue(0x810201D0)
    reg.setReadOnly(True)
    reg.setDependencies(updateRegPulseXCtrl, ["DRV_MCMET_CONF_EN_P1", "DRV_MCMET_CONF_DET_P1", "DRV_MCMET_CONF_POL_P1", "DRV_MCMET_CONF_TYP_P1", "DRV_MCMET_CONF_WID_P1"])

    reg = metComponentCommon.createHexSymbol("REG_MCMET_CTRL_PULSE_CTRL_2", None)
    reg.setLabel("PULSE2_CTRL")
    reg.setVisible(False)
    reg.setDefaultValue(0x0)
    reg.setReadOnly(True)
    reg.setDependencies(updateRegPulseXCtrl, ["DRV_MCMET_CONF_EN_P2", "DRV_MCMET_CONF_DET_P2", "DRV_MCMET_CONF_POL_P2", "DRV_MCMET_CONF_TYP_P2", "DRV_MCMET_CONF_WID_P2"])

    reg = metComponentCommon.createHexSymbol("REG_MCMET_CTRL_PULSE_CONT_PW", None)
    reg.setLabel("REG_MCMET_CTRL_PULSE_CONT_PW")
    reg.setVisible(False)
    reg.setDefaultValue(0)
    reg.setReadOnly(True)
    reg.setDependencies(updateRegPulseContribution, pulseContributionPWSymbolList)

    reg = metComponentCommon.createHexSymbol("REG_MCMET_CTRL_PULSE_CONT_I", None)
    reg.setLabel("REG_MCMET_CTRL_PULSE_CONT_I")
    reg.setVisible(False)
    reg.setDefaultValue(0)
    reg.setReadOnly(True)
    reg.setDependencies(updateRegPulseContribution, pulseContributionISymbolList)

    reg = metComponentCommon.createHexSymbol("REG_MCMET_CTRL_P_POWER_OFFSET_CTRL", None)
    reg.setLabel("REG_MCMET_CTRL_P_POWER_OFFSET_CTRL")
    reg.setVisible(False)
    reg.setDefaultValue(0)
    reg.setReadOnly(True)
    reg.setDependencies(updateRegPowerOffsetCtrl, pPowerOffsetCtrlSymbolList)

    reg = metComponentCommon.createHexSymbol("REG_MCMET_CTRL_Q_POWER_OFFSET_CTRL", None)
    reg.setLabel("REG_MCMET_CTRL_Q_POWER_OFFSET_CTRL")
    reg.setVisible(False)
    reg.setDefaultValue(0)
    reg.setReadOnly(True)
    reg.setDependencies(updateRegPowerOffsetCtrl, qPowerOffsetCtrlSymbolList)

    reg = metComponentCommon.createHexSymbol("REG_MCMET_CTRL_CAPTURE_CTRL", None)
    reg.setLabel("REG_MCMET_CTRL_CAPTURE_CTRL")
    reg.setVisible(False)
    reg.setDefaultValue(0)
    reg.setReadOnly(True)
    reg.setDependencies(updateRegCapture, ["DRV_MCMET_WAVEFORM_CAPTURE", "DRV_MCMET_WAVECAPT_SRC", "DRV_MCMET_WAVECAPT_TYPE"])

    reg = metComponentCommon.createHexSymbol("REG_MCMET_CTRL_CAPTURE_CH_SEL", None)
    reg.setLabel("REG_MCMET_CTRL_CAPTURE_CH_SEL")
    reg.setVisible(False)
    reg.setDefaultValue(0)
    reg.setReadOnly(True)
    reg.setDependencies(updateRegCaptureCHSel, captureChannelSymbolList)

    for index in range(0, MAX_MCP391x_SUPPORTED):
        regGain = metComponentCommon.createHexSymbol("REG_MCMET_CONF_MCP391X_GAIN_{}".format(index), None)
        regGain.setLabel("REG_MCMET_CONF_MCP391X_GAIN_{}".format(index))
        regGain.setDefaultValue(0)
        regGain.setVisible(False)
        regGain.setReadOnly(True)

        regConfig = metComponentCommon.createHexSymbol("REG_MCMET_CONF_MCP391X_CONFIG_{}".format(index), None)
        regConfig.setLabel("REG_MCMET_CONF_MCP391X_CONFIG_{}".format(index))
        regConfig.setDefaultValue(0x00384050)  # Dither max, boost 10, OSR=128, PRE=0 
        regConfig.setVisible(False)
        regConfig.setReadOnly(True)
        regConfig.setDependencies(updateRegMCP391XConfig, ["DRV_MCMET_MCP391x_DITHER_{}".format(index), "DRV_MCMET_MCP391x_BOOST_{}".format(index)])

    regGain.setDependencies(updateRegMCP391XGain, channelGainSymbolList)

    reg = metComponentCommon.createHexSymbol("REG_MCMET_CONF_PKT", None)
    reg.setLabel("REG_MCMET_CONF_PKT")
    reg.setVisible(False)
    reg.setDefaultValue(0x500000)
    reg.setReadOnly(True)
    reg.setDependencies(updateRegConfigConstantsPQI_K_t, ["DRV_MCMET_CONF_PKT"])

    reg = metComponentCommon.createHexSymbol("REG_MCMET_CONF_QKT", None)
    reg.setLabel("REG_MCMET_CONF_QKT")
    reg.setVisible(False)
    reg.setDefaultValue(0x500000)
    reg.setReadOnly(True)
    reg.setDependencies(updateRegConfigConstantsPQI_K_t, ["DRV_MCMET_CONF_QKT"])

    reg = metComponentCommon.createHexSymbol("REG_MCMET_CONF_IKT", None)
    reg.setLabel("REG_MCMET_CONF_IKT")
    reg.setVisible(False)
    reg.setDefaultValue(0x500000)
    reg.setReadOnly(True)
    reg.setDependencies(updateRegConfigConstantsPQI_K_t, ["DRV_MCMET_CONF_IKT"])

    reg = metComponentCommon.createHexSymbol("REG_MCMET_CREEP_THRESHOLD_P", None)
    reg.setLabel("REG_MCMET_CREEP_THRESHOLD_P")
    reg.setVisible(False)
    reg.setDefaultValue(0x00002E9A)
    reg.setReadOnly(True)
    reg.setDependencies(updateRegConfigPQCreepThreshold, ["DRV_MCMET_CONF_CREEP_P_THRES"])

    reg = metComponentCommon.createHexSymbol("REG_MCMET_CREEP_THRESHOLD_Q", None)
    reg.setLabel("REG_MCMET_CREEP_THRESHOLD_Q")
    reg.setVisible(False)
    reg.setDefaultValue(0x00002E9A)
    reg.setReadOnly(True)
    reg.setDependencies(updateRegConfigPQCreepThreshold, ["DRV_MCMET_CONF_CREEP_Q_THRES"])

    reg = metComponentCommon.createHexSymbol("REG_MCMET_CREEP_THRESHOLD_I", None)
    reg.setLabel("REG_MCMET_CREEP_THRESHOLD_I")
    reg.setVisible(False)
    reg.setDefaultValue(0x0000212D)
    reg.setReadOnly(True)
    reg.setDependencies(updateRegConfigICreepThreshold, ["DRV_MCMET_CONF_CREEP_I_THRES"])

    reg = metComponentCommon.createHexSymbol("REG_MCMET_POWER_OFFSET_P", None)
    reg.setLabel("REG_MCMET_POWER_OFFSET_P")
    reg.setVisible(False)
    reg.setDefaultValue(0)
    reg.setReadOnly(True)
    reg.setDependencies(updateRegPowerOffset, ["DRV_MCMET_CONF_POWER_OFFSET_P"])

    reg = metComponentCommon.createHexSymbol("REG_MCMET_POWER_OFFSET_Q", None)
    reg.setLabel("REG_MCMET_POWER_OFFSET_Q")
    reg.setVisible(False)
    reg.setDefaultValue(0)
    reg.setReadOnly(True)
    reg.setDependencies(updateRegPowerOffset, ["DRV_MCMET_CONF_POWER_OFFSET_Q"])

    for channel in range(0, MAX_CHANNELS):
        reg = metComponentCommon.createHexSymbol("REG_MCMET_SWELL_TH_{}".format(channel), None)
        reg.setLabel("REG_MCMET_SWELL_TH_{}".format(channel))
        reg.setVisible(False)
        reg.setDefaultValue(0)
        reg.setReadOnly(True)
        reg.setDependencies(updateRegSwellSagThreshold, ["DRV_MCMET_CONF_SWELL_TH_{}".format(channel), "REG_MCMET_CONF_K_CH_{}".format(channel)])

        reg = metComponentCommon.createHexSymbol("REG_MCMET_SAG_TH_{}".format(channel), None)
        reg.setLabel("REG_MCMET_SAG_TH_{}".format(channel))
        reg.setVisible(False)
        reg.setDefaultValue(0)
        reg.setReadOnly(True)
        reg.setDependencies(updateRegSwellSagThreshold, ["DRV_MCMET_CONF_SAG_TH_{}".format(channel), "REG_MCMET_CONF_K_CH_{}".format(channel)])


    # drvMCMetRegHARMONICCTRL = metComponentCommon.createHexSymbol("DRV_MCMET_HARMONIC_CTRL", None)
    # drvMCMetRegHARMONICCTRL.setLabel("HARMONIC_CTRL")
    # drvMCMetRegHARMONICCTRL.setVisible(False)
    # drvMCMetRegHARMONICCTRL.setDefaultValue(0)
    # drvMCMetRegHARMONICCTRL.setReadOnly(True)
    # drvMCMetRegHARMONICCTRL.setDependencies(updateConfigHarmonicCtrl, harmDependencies)

    #####################################################################################################################################
    # RTOS CONFIG
    metrologyRTOSSupport = metComponentCommon.createBooleanSymbol("DRV_MCMET_RTOS_ENABLE", None)
    metrologyRTOSSupport.setLabel("RTOS support")
    metrologyRTOSSupport.setDefaultValue(0)
    metrologyRTOSSupport.setVisible(False)
    metrologyRTOSSupport.setDependencies(enableRTOS, ["HarmonyCore.SELECT_RTOS"])

    metrologyRTOSMenu = metComponentCommon.createMenuSymbol("DRV_MCMET_RTOS_MENU", None)
    metrologyRTOSMenu.setLabel("RTOS settings")
    metrologyRTOSMenu.setHelp(multichannel_met_helpkeyword)
    metrologyRTOSMenu.setDescription("RTOS settings")
    metrologyRTOSMenu.setVisible(False)
    metrologyRTOSMenu.setDependencies(showRTOSMenu, ["HarmonyCore.SELECT_RTOS"])

    metrologyRTOSStackSize = metComponentCommon.createIntegerSymbol("DRV_MCMET_RTOS_STACK_SIZE", metrologyRTOSMenu)
    metrologyRTOSStackSize.setLabel("Stack Size (in bytes)")
    metrologyRTOSStackSize.setHelp(multichannel_met_helpkeyword)
    metrologyRTOSStackSize.setDefaultValue(1024)

    metrologyRTOSMsgQSize = metComponentCommon.createIntegerSymbol("DRV_MCMET_RTOS_TASK_MSG_QTY", metrologyRTOSMenu)
    metrologyRTOSMsgQSize.setLabel("Maximum Message Queue Size")
    metrologyRTOSMsgQSize.setHelp(multichannel_met_helpkeyword)
    metrologyRTOSMsgQSize.setDescription("A µC/OS-III task contains an optional internal message queue (if OS_CFG_TASK_Q_EN is set to DEF_ENABLED in os_cfg.h). This argument specifies the maximum number of messages that the task can receive through this message queue. The user may specify that the task is unable to receive messages by setting this argument to 0")
    metrologyRTOSMsgQSize.setDefaultValue(0)
    metrologyRTOSMsgQSize.setVisible(getActiveRtos() == "MicriumOSIII")
    metrologyRTOSMsgQSize.setDependencies(commandRtosMicriumOSIIIAppTaskVisibility, ["HarmonyCore.SELECT_RTOS"])

    metrologyRTOSTaskTimeQuanta = metComponentCommon.createIntegerSymbol("DRV_MCMET_RTOS_TASK_TIME_QUANTA", metrologyRTOSMenu)
    metrologyRTOSTaskTimeQuanta.setLabel("Task Time Quanta")
    metrologyRTOSTaskTimeQuanta.setHelp(multichannel_met_helpkeyword)
    metrologyRTOSTaskTimeQuanta.setDescription("The amount of time (in clock ticks) for the time quanta when Round Robin is enabled. If you specify 0, then the default time quanta will be used which is the tick rate divided by 10.")
    metrologyRTOSTaskTimeQuanta.setDefaultValue(0)
    metrologyRTOSTaskTimeQuanta.setVisible(getActiveRtos() == "MicriumOSIII")
    metrologyRTOSTaskTimeQuanta.setDependencies(commandRtosMicriumOSIIIAppTaskVisibility, ["HarmonyCore.SELECT_RTOS"])

    metrologyRTOSTaskPriority = metComponentCommon.createIntegerSymbol("DRV_MCMET_RTOS_TASK_PRIORITY", metrologyRTOSMenu)
    metrologyRTOSTaskPriority.setLabel("Task Priority")
    metrologyRTOSTaskPriority.setHelp(multichannel_met_helpkeyword)
    metrologyRTOSTaskPriority.setDefaultValue(1)

    metrologyRTOSTaskSpecificOpt = metComponentCommon.createBooleanSymbol("DRV_MCMET_RTOS_TASK_OPT_NONE", metrologyRTOSMenu)
    metrologyRTOSTaskSpecificOpt.setLabel("Task Specific Options")
    metrologyRTOSTaskSpecificOpt.setHelp(multichannel_met_helpkeyword)
    metrologyRTOSTaskSpecificOpt.setDescription("Contains task-specific options. Each option consists of one bit. The option is selected when the bit is set. The current version of µC/OS-III supports the following options:")
    metrologyRTOSTaskSpecificOpt.setDefaultValue(True)
    metrologyRTOSTaskSpecificOpt.setVisible(getActiveRtos() == "MicriumOSIII")
    metrologyRTOSTaskSpecificOpt.setDependencies(commandRtosMicriumOSIIIAppTaskVisibility, ["HarmonyCore.SELECT_RTOS"])

    metrologyRTOSTaskStkChk = metComponentCommon.createBooleanSymbol("DRV_MCMET_RTOS_TASK_OPT_STK_CHK", metrologyRTOSTaskSpecificOpt)
    metrologyRTOSTaskStkChk.setLabel("Stack checking is allowed for the task")
    metrologyRTOSTaskStkChk.setHelp(multichannel_met_helpkeyword)
    metrologyRTOSTaskStkChk.setDescription("Specifies whether stack checking is allowed for the task")
    metrologyRTOSTaskStkChk.setDefaultValue(True)
    metrologyRTOSTaskStkChk.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["DRV_MCMET_RTOS_TASK_OPT_NONE"])

    metrologyRTOSTaskStkClr = metComponentCommon.createBooleanSymbol("DRV_MCMET_RTOS_TASK_OPT_STK_CLR", metrologyRTOSTaskSpecificOpt)
    metrologyRTOSTaskStkClr.setLabel("Stack needs to be cleared")
    metrologyRTOSTaskStkClr.setHelp(multichannel_met_helpkeyword)
    metrologyRTOSTaskStkClr.setDescription("Specifies whether the stack needs to be cleared")
    metrologyRTOSTaskStkClr.setDefaultValue(True)
    metrologyRTOSTaskStkClr.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["DRV_MCMET_RTOS_TASK_OPT_NONE"])

    metrologyRTOSTaskSaveFp = metComponentCommon.createBooleanSymbol("DRV_MCMET_RTOS_TASK_OPT_SAVE_FP", metrologyRTOSTaskSpecificOpt)
    metrologyRTOSTaskSaveFp.setLabel("Floating-point registers needs to be saved")
    metrologyRTOSTaskSaveFp.setHelp(multichannel_met_helpkeyword)
    metrologyRTOSTaskSaveFp.setDescription("Specifies whether floating-point registers are saved. This option is only valid if the processor has floating-point hardware and the processor-specific code saves the floating-point registers")
    metrologyRTOSTaskSaveFp.setDefaultValue(False)
    metrologyRTOSTaskSaveFp.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["DRV_MCMET_RTOS_TASK_OPT_NONE"])

    metrologyRTOSTaskNoTls = metComponentCommon.createBooleanSymbol("DRV_MCMET_RTOS_TASK_OPT_NO_TLS", metrologyRTOSTaskSpecificOpt)
    metrologyRTOSTaskNoTls.setLabel("TLS (Thread Local Storage) support needed for the task")
    metrologyRTOSTaskNoTls.setHelp(multichannel_met_helpkeyword)
    metrologyRTOSTaskNoTls.setDescription("If the caller doesn’t want or need TLS (Thread Local Storage) support for the task being created. If you do not include this option, TLS will be supported by default. TLS support was added in V3.03.00")
    metrologyRTOSTaskNoTls.setDefaultValue(False)
    metrologyRTOSTaskNoTls.setDependencies(commandRtosMicriumOSIIITaskOptVisibility, ["DRV_MCMET_RTOS_TASK_OPT_NONE"])


    #####################################################################################################################################
    # METROLOGY FILES

    drvMCMetSourceFile = metComponentCommon.createFileSymbol("DRV_MCMET_SOURCE", None)
    drvMCMetSourceFile.setSourcePath("driver/mcmetrology/drv_mcmetrology.c.ftl")
    drvMCMetSourceFile.setOutputName("drv_mcmetrology.c")
    drvMCMetSourceFile.setDestPath("driver/mcmetrology")
    drvMCMetSourceFile.setProjectPath("config/" + configName + "/driver/mcmetrology/")
    drvMCMetSourceFile.setType("SOURCE")
    drvMCMetSourceFile.setOverwrite(True)
    drvMCMetSourceFile.setMarkup(True)

    drvMCMetHeaderFile = metComponentCommon.createFileSymbol("DRV_MCMET_HEADER", None)
    drvMCMetHeaderFile.setSourcePath("driver/mcmetrology/drv_mcmetrology.h.ftl")
    drvMCMetHeaderFile.setOutputName("drv_mcmetrology.h")
    drvMCMetHeaderFile.setDestPath("driver/mcmetrology")
    drvMCMetHeaderFile.setProjectPath("config/" + configName + "/driver/mcmetrology/")
    drvMCMetHeaderFile.setType("HEADER")
    drvMCMetHeaderFile.setOverwrite(True)
    drvMCMetHeaderFile.setMarkup(True)

    drvMCMetHeaderFileDefs = metComponentCommon.createFileSymbol("DRV_MCMET_HEADER_DEFINITIONS", None)
    drvMCMetHeaderFileDefs.setSourcePath("driver/mcmetrology/drv_mcmetrology_definitions.h.ftl")
    drvMCMetHeaderFileDefs.setOutputName("drv_mcmetrology_definitions.h")
    drvMCMetHeaderFileDefs.setDestPath("driver/mcmetrology")
    drvMCMetHeaderFileDefs.setProjectPath("config/" + configName + "/driver/mcmetrology/")
    drvMCMetHeaderFileDefs.setType("HEADER")
    drvMCMetHeaderFileDefs.setOverwrite(True)
    drvMCMetHeaderFileDefs.setMarkup(True)

    drvMCMetHeaderFileLocal = metComponentCommon.createFileSymbol("DRV_MCMET_HEADER_LOCAL", None)
    drvMCMetHeaderFileLocal.setSourcePath("driver/mcmetrology/drv_mcmetrology_local.h.ftl")
    drvMCMetHeaderFileLocal.setOutputName("drv_mcmetrology_local.h")
    drvMCMetHeaderFileLocal.setDestPath("driver/mcmetrology")
    drvMCMetHeaderFileLocal.setProjectPath("config/" + configName + "/driver/mcmetrology/")
    drvMCMetHeaderFileLocal.setType("HEADER")
    drvMCMetHeaderFileLocal.setOverwrite(True)
    drvMCMetHeaderFileLocal.setMarkup(True)

    drvMCMetHeaderRegsFile = metComponentCommon.createFileSymbol("DRV_MCMET_HEADER_REGS", None)
    drvMCMetHeaderRegsFile.setSourcePath("driver/mcmetrology/drv_mcmetrology_regs.h.ftl")
    drvMCMetHeaderRegsFile.setOutputName("drv_mcmetrology_regs.h")
    drvMCMetHeaderRegsFile.setDestPath("driver/mcmetrology")
    drvMCMetHeaderRegsFile.setProjectPath("config/" + configName + "/driver/mcmetrology/")
    drvMCMetHeaderRegsFile.setType("HEADER")
    drvMCMetHeaderRegsFile.setOverwrite(True)
    drvMCMetHeaderRegsFile.setMarkup(True)

    drvMCMetRegConfigFile = metComponentCommon.createFileSymbol("DRV_MCMET_CONFIG_REGS", None)
    drvMCMetRegConfigFile.setSourcePath("driver/mcmetrology/drv_mcmetrology_configuration.h.ftl")
    drvMCMetRegConfigFile.setOutputName("drv_mcmetrology_configuration.h")
    drvMCMetRegConfigFile.setDestPath("driver/mcmetrology")
    drvMCMetRegConfigFile.setProjectPath("config/" + configName + "/driver/mcmetrology/")
    drvMCMetRegConfigFile.setType("HEADER")
    drvMCMetRegConfigFile.setOverwrite(True)
    drvMCMetRegConfigFile.setMarkup(True)

    #### ASM Path Settings #####################################################

    drvMCMetAsmPathSetting = metComponentCommon.createSettingSymbol("DRV_MCMET_ASM_PATH_SETTING", None)
    drvMCMetAsmPathSetting.setCategory("C32-AS")
    drvMCMetAsmPathSetting.setKey("extra-include-directories-for-assembler")
    drvMCMetAsmPathSetting.setValue("../src/config/" + configName + "/driver/mcmetrology/bin")
    drvMCMetAsmPathSetting.setAppend(True, ";")

    #### Binary MET Files ######################################################

    drvMCMetBinFile = metComponentCommon.createLibrarySymbol("DRV_MCMET_BIN", None)
    drvMCMetBinFile.setSourcePath("driver/mcmetrology/bin/core1_metlib.bin")
    drvMCMetBinFile.setOutputName("core1_metlib.bin")
    drvMCMetBinFile.setDestPath("driver/mcmetrology/bin")

    drvMCMetAssemblyBinFile = metComponentCommon.createFileSymbol("DRV_MCMET_ASSEMBLY_BIN", None)
    drvMCMetAssemblyBinFile.setSourcePath("driver/mcmetrology/bin/met_bin.S")
    drvMCMetAssemblyBinFile.setOutputName("met_bin.S")
    drvMCMetAssemblyBinFile.setDestPath("driver/mcmetrology/bin")
    drvMCMetAssemblyBinFile.setProjectPath("config/" + configName + "/driver/mcmetrology/bin")
    drvMCMetAssemblyBinFile.setType("SOURCE")

    #### FreeMaker System Files ######################################################

    drvMCMetSystemDefFile = metComponentCommon.createFileSymbol("DRV_MCMET_SYSTEM_DEF", None)
    drvMCMetSystemDefFile.setType("STRING")
    drvMCMetSystemDefFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
    drvMCMetSystemDefFile.setSourcePath("driver/mcmetrology/templates/system/definitions.h.ftl")
    drvMCMetSystemDefFile.setMarkup(True)

    drvMCMetSystemConfigFile = metComponentCommon.createFileSymbol("DRV_MCMET_CONFIGURATION", None)
    drvMCMetSystemConfigFile.setType("STRING")
    drvMCMetSystemConfigFile.setOutputName("core.LIST_SYSTEM_CONFIG_H_DRIVER_CONFIGURATION")
    drvMCMetSystemConfigFile.setSourcePath("driver/mcmetrology/templates/system/configuration.h.ftl")
    drvMCMetSystemConfigFile.setMarkup(True)

    drvMCMetSystemInitDataFile = metComponentCommon.createFileSymbol("DRV_MCMET_INIT_DATA", None)
    drvMCMetSystemInitDataFile.setType("STRING")
    drvMCMetSystemInitDataFile.setOutputName("core.LIST_SYSTEM_INIT_C_DRIVER_INITIALIZATION_DATA")
    drvMCMetSystemInitDataFile.setSourcePath("driver/mcmetrology/templates/system/initialize_data.c.ftl")
    drvMCMetSystemInitDataFile.setMarkup(True)

    drvMCMetSystemInitFile = metComponentCommon.createFileSymbol("DRV_MCMET_INIT", None)
    drvMCMetSystemInitFile.setType("STRING")
    drvMCMetSystemInitFile.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
    drvMCMetSystemInitFile.setSourcePath("driver/mcmetrology/templates/system/initialize.c.ftl")
    drvMCMetSystemInitFile.setMarkup(True)

    drvMCMetSystemTasksFile = metComponentCommon.createFileSymbol("DRV_MCMET_SYS_TASK", None)
    drvMCMetSystemTasksFile.setType("STRING")
    drvMCMetSystemTasksFile.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
    drvMCMetSystemTasksFile.setSourcePath("driver/mcmetrology/templates/system/system_tasks.c.ftl")
    drvMCMetSystemTasksFile.setMarkup(True)

    drvMCMetSystemDefObjFile = metComponentCommon.createFileSymbol("DRV_MCMET_SYSTEM_DEF_OBJECT", None)
    drvMCMetSystemDefObjFile.setType("STRING")
    drvMCMetSystemDefObjFile.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_OBJECTS")
    drvMCMetSystemDefObjFile.setSourcePath("driver/mcmetrology/templates/system/definitions_objects.h.ftl")
    drvMCMetSystemDefObjFile.setMarkup(True)

    drvMCMetSystemRtosTasksFile = metComponentCommon.createFileSymbol("DRV_MCMET_SYS_RTOS_TASK", None)
    drvMCMetSystemRtosTasksFile.setType("STRING")
    drvMCMetSystemRtosTasksFile.setOutputName("core.LIST_SYSTEM_RTOS_TASKS_C_DEFINITIONS")
    drvMCMetSystemRtosTasksFile.setSourcePath("driver/mcmetrology/templates/system/system_rtos_tasks.c.ftl")
    drvMCMetSystemRtosTasksFile.setMarkup(True)
    drvMCMetSystemRtosTasksFile.setEnabled(False)
    drvMCMetSystemRtosTasksFile.setDependencies(genRtosTask, ["HarmonyCore.SELECT_RTOS"])

