"""
Copyright (C) 2024, Microchip Technology Inc., and its subsidiaries. All rights reserved.

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

######################  Harmony Smart Energy  ######################
def loadModule():

    print("Load Module: Harmony Smart Energy Metrology")

    series = ATDF.getNode("/avr-tools-device-file/devices/device").getAttribute("series")

    ## Metrology Driver
    if series == "PIC32CXMTC" or series == "PIC32CXMTSH":
        drvMetrologyComponent = Module.CreateComponent("drvMet", "METROLOGY", "/SmartEnergy/Drivers/", "driver/metrology/config/drv_metrology.py")
        drvMetrologyComponent.addDependency("drvMet_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
        drvMetrologyComponent.setDisplayType("Metrology")
        drvMetrologyComponent.setHelpKeyword("SE_drv_metrology")

    ## Multi-Channel Metrology Driver
    if series == "PIC32CXMTC":
        drvMCMetrologyComponent = Module.CreateComponent("drvMCMet", "MULTI-CHANNEL METROLOGY", "/SmartEnergy/Drivers/", "driver/mcmetrology/config/drv_mcmetrology.py")
        drvMCMetrologyComponent.addDependency("drvMCMet_coreDep", "Core Service", "Core Service", True, True)
        drvMCMetrologyComponent.setDisplayType("Multi-Channel Metrology")
        drvMCMetrologyComponent.setHelpKeyword("SE_drv_mcmetrology")

    icmModule = ATDF.getNode("/avr-tools-device-file/devices/device/peripherals/module@[name=\"ICM\"]")
    if icmModule is not None:
        if icmModule.getAttribute("name") == "ICM" and icmModule.getAttribute("id") == "11105":
            drvICMComponent = Module.CreateComponent("icm", "ICM", "/SmartEnergy/Peripherals/", "peripheral/icm/config/icm_plib.py")
            # drvICMComponent.addCapability("ICM", "ICM", "ICM", True)
            drvICMComponent.setDisplayType("Peripheral Library")
            drvICMComponent.setHelpKeyword("SE_plib_icm_config")

    emafeModule = ATDF.getNode("/avr-tools-device-file/devices/device/peripherals/module@[name=\"EMAFE\"]")
    if emafeModule is not None:
        if emafeModule.getAttribute("name") == "EMAFE" and emafeModule.getAttribute("id") == "04742":
            drvEMAFEComponent = Module.CreateComponent("emafe", "EMAFE", "/SmartEnergy/Peripherals/", "peripheral/emafe/config/emafe_plib.py")
            # drvEMAFEComponent.addCapability("EMAFE", "EMAFE", "EMAFE", True)
            drvEMAFEComponent.addDependency("mcspi_dependency", "MCSPI", "MCSPI", True, True)
            drvEMAFEComponent.setDisplayType("Peripheral Library")
            drvEMAFEComponent.setHelpKeyword("SE_plib_emafe_config")
