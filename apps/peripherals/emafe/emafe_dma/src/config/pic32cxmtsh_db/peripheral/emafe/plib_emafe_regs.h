/*******************************************************************************
  Register definition of EMAFE PLIB.

  Company:
    Microchip Technology Inc.

  File Name:
    plib_emafe_regs.h

  Summary:
    Register definition of the Energy EMAFEering Analog Front End Plib (EMAFE).

  Description:
    This file defines the registers interface for the EMAFE Plib.
*******************************************************************************/

/*******************************************************************************
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
*******************************************************************************/

#ifndef EMAFE_REGS_H
#define EMAFE_REGS_H

/* ************************************************************************** */
/*   SOFTWARE API DEFINITION FOR EMAFE REGISTERS                          */
/* ************************************************************************** */
/* MISRA C-2012 deviation block start */
/* MISRA C-2012 Rule 5.4 deviated 64 times.  Deviation record ID - H3_MISRAC_2012_R_5_4_DR_1 */

/* -------- EMAFE_ADC_ADCI0_CTRL : (Register Offset: 0x20) ADCI0 Controls Register -------- */
#define EMAFE_ADC_ADCI0_CTRL_ONADC_Msk (_UINT8_(0x1u) << 0) /**< \brief (EMAFE_ADC_ADCI0_CTRL) Channel I0 ADC Enable */
#define EMAFE_ADC_ADCI0_CTRL_TEMPMEAS_Msk (_UINT8_(0x1u) << 1) /**< \brief (EMAFE_ADC_ADCI0_CTRL) Temperature Measurement Activation */
/* -------- EMAFE_ADC_ADCI1_CTRL : (Register Offset: 0x21) ADCI1 Controls Register -------- */
#define EMAFE_ADC_ADCI1_CTRL_ONADC_Msk (_UINT8_(0x1u) << 0) /**< \brief (EMAFE_ADC_ADCI1_CTRL) Channel I1 ADC Enable */
#define EMAFE_ADC_ADCI1_CTRL_GAIN_Pos _UINT8_(4)
#define EMAFE_ADC_ADCI1_CTRL_GAIN_Msk (_UINT8_(0x3u) << EMAFE_ADC_ADCI1_CTRL_GAIN_Pos) /**< \brief (EMAFE_ADC_ADCI1_CTRL) Channel I1 Gain Configuration */
#define EMAFE_ADC_ADCI1_CTRL_GAIN(value) _UINT8_((EMAFE_ADC_ADCI1_CTRL_GAIN_Msk & ((value) << EMAFE_ADC_ADCI1_CTRL_GAIN_Pos)))
#define EMAFE_ADC_ADCI1_CTRL_GAINX1 (_UINT8_(0x0u)) /**< \brief (EMAFE_ADC_ADCI1_CTRL) Input stage of the ADC has a gain of 1. */
#define EMAFE_ADC_ADCI1_CTRL_GAINX2 (_UINT8_(0x1u)) /**< \brief (EMAFE_ADC_ADCI1_CTRL) Input stage of the ADC has a gain of 2. */
#define EMAFE_ADC_ADCI1_CTRL_GAINX4 (_UINT8_(0x2u)) /**< \brief (EMAFE_ADC_ADCI1_CTRL) Input stage of the ADC has a gain of 4. */
#define EMAFE_ADC_ADCI1_CTRL_GAINX8 (_UINT8_(0x3u)) /**< \brief (EMAFE_ADC_ADCI1_CTRL) Input stage of the ADC has a gain of 8. */
/* -------- EMAFE_ADC_ADCV1_CTRL : (Register Offset: 0x22) ADCV1 Controls Register -------- */
#define EMAFE_ADC_ADCV1_CTRL_ONADC_Msk (_UINT8_(0x1u) << 0) /**< \brief (EMAFE_ADC_ADCV1_CTRL) Channel V1 ADC Enable */
/* -------- EMAFE_ADC_ADCI2_CTRL : (Register Offset: 0x23) ADCI2 Controls Register -------- */
#define EMAFE_ADC_ADCI2_CTRL_ONADC_Msk (_UINT8_(0x1u) << 0) /**< \brief (EMAFE_ADC_ADCI2_CTRL) Channel I1 ADC Enable */
#define EMAFE_ADC_ADCI2_CTRL_GAIN_Pos _UINT8_(4)
#define EMAFE_ADC_ADCI2_CTRL_GAIN_Msk (_UINT8_(0x3u) << EMAFE_ADC_ADCI2_CTRL_GAIN_Pos) /**< \brief (EMAFE_ADC_ADCI2_CTRL) Channel I2 Gain Configuration */
#define EMAFE_ADC_ADCI2_CTRL_GAIN(value) _UINT8_((EMAFE_ADC_ADCI2_CTRL_GAIN_Msk & ((value) << EMAFE_ADC_ADCI2_CTRL_GAIN_Pos)))
#define EMAFE_ADC_ADCI2_CTRL_GAINX1 (_UINT8_(0x0u)) /**< \brief (EMAFE_ADC_ADCI2_CTRL) Input stage of the ADC has a gain of 1. */
#define EMAFE_ADC_ADCI2_CTRL_GAINX2 (_UINT8_(0x1u)) /**< \brief (EMAFE_ADC_ADCI2_CTRL) Input stage of the ADC has a gain of 2. */
#define EMAFE_ADC_ADCI2_CTRL_GAINX4 (_UINT8_(0x2u)) /**< \brief (EMAFE_ADC_ADCI2_CTRL) Input stage of the ADC has a gain of 4. */
#define EMAFE_ADC_ADCI2_CTRL_GAINX8 (_UINT8_(0x3u)) /**< \brief (EMAFE_ADC_ADCI2_CTRL) Input stage of the ADC has a gain of 8. */
/* -------- EMAFE_ADC_ADCV2_CTRL : (Register Offset: 0x24) ADCV1 Controls Register -------- */
#define EMAFE_ADC_ADCV2_CTRL_ONADC_Msk (_UINT8_(0x1u) << 0) /**< \brief (EMAFE_ADC_ADCV2_CTRL) Channel V2 ADC Enable */
/* -------- EMAFE_ADC_ANALOG_CTRL : (Register Offset: 0x27) Analog Controls Register -------- */
#define EMAFE_ADC_ANALOG_CTRL_ONBIAS_Msk (_UINT8_(0x1u) << 0) /**< \brief (EMAFE_ADC_ANALOG_CTRL) Current Bias Generator Enable */
#define EMAFE_ADC_ANALOG_CTRL_ONREF_Msk (_UINT8_(0x1u) << 1) /**< \brief (EMAFE_ADC_ANALOG_CTRL) Voltage Reference Enable */
#define EMAFE_ADC_ANALOG_CTRL_ONLDO_Msk (_UINT8_(0x1u) << 2) /**< \brief (EMAFE_ADC_ANALOG_CTRL) Internal LDO Status */
/* -------- EMAFE_ADC_ANALOG_STATUS : (Register Offset: 0x29) Analog Status Register -------- */
#define EMAFE_ADC_ANALOG_STATUS_SYSRDY_Msk (_UINT8_(0x1u) << 0) /**< \brief (EMAFE_ADC_ANALOG_STATUS) Analog System Ready */
/* -------- EMAFE_ADC_NRESET : (Register Offset: 0x2D) Software Reset Register -------- */
#define EMAFE_ADC_NRESET_NRESET_Msk (_UINT8_(0x1u) << 0) /**< \brief (EMAFE_ADC_NRESET) Chip Reset */
#define EMAFE_ADC_NRESET_ACTIVE_Msk  (_UINT8_(0x0u) << 0) /**< \brief (EMAFE_ADC_NRESET) Chip Reset */
#define EMAFE_ADC_NRESET_RELEASE_Msk (_UINT8_(0x1u) << 0) /**< \brief (EMAFE_ADC_NRESET) Chip Reset */
/* -------- EMAFE_ADC_CLOCK : (Register Offset: 0x33) ADCs clocks configuration -------- */
#define EMAFE_ADC_CLOCK_MCLKDIV_ANA_Pos _UINT8_(4)
#define EMAFE_ADC_CLOCK_MCLKDIV_ANA_Msk (_UINT8_(0x7u) << EMAFE_ADC_CLOCK_MCLKDIV_ANA_Pos) /**< \brief (EMAFE_ADC_CLOCK) Analog clock prescaler ratio */
#define EMAFE_ADC_CLOCK_MCLKDIV_ANA(value) _UINT8_((EMAFE_ADC_CLOCK_MCLKDIV_ANA_Msk & ((value) << EMAFE_ADC_CLOCK_MCLKDIV_ANA_Pos)))
#define EMAFE_ADC_CLOCK_MCLKDIV_ANACK1 (_UINT8_(0x0u)) /**< \brief (EMAFE_ADC_CLOCK) ANA clock is MCLK_AFE/1 (default). */
#define EMAFE_ADC_CLOCK_MCLKDIV_ANACK2 (_UINT8_(0x1u)) /**< \brief (EMAFE_ADC_CLOCK) ANA clock is MCLK_AFE/2. */
#define EMAFE_ADC_CLOCK_MCLKDIV_ANACK4 (_UINT8_(0x2u)) /**< \brief (EMAFE_ADC_CLOCK) ANA clock is MCLK_AFE/4. */
#define EMAFE_ADC_CLOCK_MCLKDIV_ANACK8 (_UINT8_(0x3u)) /**< \brief (EMAFE_ADC_CLOCK) ANA clock is MCLK_AFE/8. */
#define EMAFE_ADC_CLOCK_MCLKDIV_ANACK16 (_UINT8_(0x4u)) /**< \brief (EMAFE_ADC_CLOCK) ANA clock is MCLK_AFE/16. */
#define EMAFE_ADC_CLOCK_MCLKDIV_ANACK32 (_UINT8_(0x5u)) /**< \brief (EMAFE_ADC_CLOCK) ANA clock is MCLK_AFE/32. */
#define EMAFE_ADC_CLOCK_FREQRATIO_Pos  _UINT8_(0x7u) /**< \brief (EMAFE_ADC_CLOCK) The frequency ratio between MCLK_AFE and Sigma-Delta bitstream */
#define EMAFE_ADC_CLOCK_FREQRATIO_Msk (_UINT8_(0x1u) << EMAFE_ADC_CLOCK_FREQRATIO_Pos) /**< \brief (EMAFE_ADC_CLOCK) Frequency ratio configuration */
#define EMAFE_ADC_CLOCK_FREQRATIO(value) _UINT8_((EMAFE_ADC_CLOCK_FREQRATIO_Msk & ((value) << EMAFE_ADC_CLOCK_FREQRATIO_Pos)))
#define EMAFE_ADC_CLOCK_FREQRATIO_8 (_UINT8_(0x0u)) /**< \brief (EMAFE_ADC_CLOCK) The frequency ratio between MCLK_AFE and Sigma-Delta bitstream is 8 */
#define EMAFE_ADC_CLOCK_FREQRATIO_4 (_UINT8_(0x1u)) /**< \brief (EMAFE_ADC_CLOCK) The frequency ratio between MCLK_AFE and Sigma-Delta bitstream is 4 */
/* -------- EMAFE_ADC_SECURITY : (Register Offset: 0x3E) Security control -------- */
#define EMAFE_ADC_SECURITY_SECRES_Msk (_UINT8_(0x1u) << 0) /**< \brief (EMAFE_ADC_SECURITY) Restricted Access Security Level */
#define EMAFE_ADC_SECURITY_SECPRIV_Msk (_UINT8_(0x1u) << 1) /**< \brief (EMAFE_ADC_SECURITY) Private Access Security Level */
#define EMAFE_ADC_SECURITY_SYS_RDY_DIS_Msk (_UINT8_(0x1u) << 2) /**< \brief (EMAFE_ADC_SECURITY) System Ready Disable */
#define EMAFE_ADC_SECURITY_SECKEY_Pos _UINT8_(4)
#define EMAFE_ADC_SECURITY_SECKEY_Msk (0xfu << EMAFE_ADC_SECURITY_SECKEY_Pos) /**< \brief (EMAFE_ADC_SECURITY) Security Key */
#define EMAFE_ADC_SECURITY_SECKEY(value) _UINT8_((EMAFE_ADC_SECURITY_SECKEY_Msk & ((value) << EMAFE_ADC_SECURITY_SECKEY_Pos)))
#define EMAFE_ADC_SECURITY_SECKEY_PASSWD (0xAu) /**< \brief (EMAFE_ADC_SECURITY) Writing any other value in this field aborts the write operation of the SECRES and SECPRIV bit. Always reads as 0. */
/* -------- EMAFE_ADC_VREF_TL : (Register Offset: 0x41) Voltage Reference Value at TL -------- */
#define EMAFE_ADC_VREF_TL_Pos _UINT16_(0)
#define EMAFE_ADC_VREF_TL_Msk (0xFFFu << EMAFE_ADC_VREF_TL_Pos) /**< \brief EMAFE_ADC_VREF_TL Mask*/
#define EMAFE_ADC_VREF_TL(value) _UINT16_((EMAFE_ADC_VREF_TL_Msk & ((value) << EMAFE_ADC_VREF_TL_Pos)))
/* -------- EMAFE_ADC_TEMP_TL : (Register Offset: 0x43) Temperature Sensor Value (read by ADC)*/
#define EMAFE_ADC_TEMP_TL_Pos _UINT16_(0)
#define EMAFE_ADC_TEMP_TL_Msk (0xFFFu << EMAFE_ADC_TEMP_TL_Pos) /**< \brief EMAFE_ADC_TEMP_TL Mask*/
#define EMAFE_ADC_TEMP_TL(value) _UINT16_((EMAFE_ADC_TEMP_TL_Msk & ((value) << EMAFE_ADC_TEMP_TL_Pos)))
/* -------- EMAFE_ADC_VREF_TH : (Register Offset: 0x45) Voltage Reference Value at TH -------- */
#define EMAFE_ADC_VREF_TH_Pos _UINT16_(0)
#define EMAFE_ADC_VREF_TH_Msk (0xFFFu << EMAFE_ADC_VREF_TH_Pos) /**< \brief EMAFE_ADC_VREF_TH Mask*/
#define EMAFE_ADC_VREF_TH(value) _UINT8_((EMAFE_ADC_VREF_TH_Msk & ((value) << EMAFE_ADC_VREF_TH_Pos)))
/* -------- EMAFE_ADC_TEMP_TH : (Register Offset: 0x47) Temperature Sensor Value (read by ADC)*/
#define EMAFE_ADC_TEMP_TH_Pos _UINT16_(0)
#define EMAFE_ADC_TEMP_TH_Msk (0xFFFu << EMAFE_ADC_TEMP_TH_Pos) /**< \brief EMAFE_ADC_TEMP_TH Mask*/
#define EMAFE_ADC_TEMP_TH(value) _UINT16_((EMAFE_ADC_TEMP_TH_Msk & ((value) << EMAFE_ADC_TEMP_TH_Pos)))

/* MISRA C-2012 deviation block end */

/** \brief EMAFE Filter Control Registers */
typedef enum {
	EMAFE_ADC_ADCI0_CTRL           = 0x20u, /** ADCI0 Controls Register */
	EMAFE_ADC_ADCI1_CTRL           = 0x21u, /** ADCI1 Controls Register */
	EMAFE_ADC_ADCV1_CTRL           = 0x22u, /** ADCV1 Controls Register */
	EMAFE_ADC_ADCI2_CTRL           = 0x23u, /** ADCI2 Controls Register */
	EMAFE_ADC_ADCV2_CTRL           = 0x24u, /** ADCV2 Controls Register */
	EMAFE_ADC_ANALOG_CTRL          = 0x27u, /** Analog Controls Register */
	EMAFE_ADC_ANALOG_STATUS        = 0x29u, /** Analog Status Register */
	EMAFE_ADC_NRESET               = 0x2Du, /** Software Reset Register */
	EMAFE_ADC_CLOCK                = 0x33u, /** ADCs clocks configuration */
	EMAFE_ADC_SECURITY             = 0x3Eu, /** Security control */
	EMAFE_ADC_VREF_TLOW            = 0x41u, /** Voltage Reference Value at TLOW   */
	EMAFE_ADC_TEMP_TLOW            = 0x43u, /** Temperature Sensor Value (read by ADC) at TLOW */
	EMAFE_ADC_VREF_THIGH           = 0x45u, /** Voltage Reference Value at THIGH  */
	EMAFE_ADC_TEMP_THIGH           = 0x47u, /** Temperature Sensor Value (read by ADC) at THIGH  */
} EMAFE_ADC_CONTROL;

#endif /* EMAFE_REGS_H */
