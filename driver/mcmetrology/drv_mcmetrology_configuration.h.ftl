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

#ifndef MCMMETROLOGY_CONFIGURATION_H
#define MCMMETROLOGY_CONFIGURATION_H

/* ************************************************************************** */
/*   DEFAULT CONFIGURATION FOR MULTI-CHANNEL METROLOGY CONTROL REGISTERS      */
/* ************************************************************************** */

/* Metrology Default Config: Metrology feature setting register 0 */
#define DRV_MCMETROLOGY_CONF_FCTRL0                 0x${REG_MCMET_CTRL_FCTRL0?left_pad(8, "0")}UL
/* Metrology Default Config: Metrology feature setting register 1 */
#define DRV_MCMETROLOGY_CONF_FCTRL1                 0x${REG_MCMET_CTRL_FCTRL1?left_pad(8, "0")}UL
/* Metrology Default Config: Power source */
#define DRV_MCMETROLOGY_CONF_POWER_SOURCE           {0x${REG_MCMET_CTRL_PSRC_0?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CTRL_PSRC_1?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CTRL_PSRC_2?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CTRL_PSRC_3?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CTRL_PSRC_4?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CTRL_PSRC_5?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CTRL_PSRC_6?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CTRL_PSRC_7?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CTRL_PSRC_8?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CTRL_PSRC_9?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CTRL_PSRC_10?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CTRL_PSRC_11?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CTRL_PSRC_12?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CTRL_PSRC_13?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CTRL_PSRC_14?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CTRL_PSRC_15?left_pad(8, "0")}UL}
/* Metrology Default Config: Meter Type (Not implemented in this version) */
#define DRV_MCMETROLOGY_CONF_MT                     0x00000000UL
/* Metrology Default Config: Sensor Type  */
#define DRV_MCMETROLOGY_CONF_ST                     {0x${REG_MCMET_CTRL_ST0?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CTRL_ST1?left_pad(8, "0")}UL}
/* Metrology Default Config: PULSE0_CTRL */
#define DRV_MCMETROLOGY_CONF_PULSE0_CTRL            0x${REG_MCMET_CTRL_PULSE_CTRL_0?left_pad(8, "0")}UL
/* Metrology Default Config: PULSE1_CTRL */
#define DRV_MCMETROLOGY_CONF_PULSE1_CTRL            0x${REG_MCMET_CTRL_PULSE_CTRL_1?left_pad(8, "0")}UL
/* Metrology Default Config: PULSE2_CTRL */
#define DRV_MCMETROLOGY_CONF_PULSE2_CTRL            0x${REG_MCMET_CTRL_PULSE_CTRL_2?left_pad(8, "0")}UL
/* Metrology Default Config: PULSE_CONTRIBUTION_PW */
#define DRV_MCMETROLOGY_CONF_PULSE_PW               0x${REG_MCMET_CTRL_PULSE_CONT_PW?left_pad(8, "0")}UL
/* Metrology Default Config: PULSE_CONTRIBUTION_I */
#define DRV_MCMETROLOGY_CONF_PULSE_I                0x${REG_MCMET_CTRL_PULSE_CONT_I?left_pad(8, "0")}UL
/* Metrology Default Config: Meter Active Power Pulse Constant */
#define DRV_MCMETROLOGY_CONF_PKT                    0x${REG_MCMET_CONF_PKT?left_pad(8, "0")}UL
/* Metrology Default Config: Meter Reactive Power Pulse Constant */
#define DRV_MCMETROLOGY_CONF_QKT                    0x${REG_MCMET_CONF_QKT?left_pad(8, "0")}UL
/* Metrology Default Config: Meter Current Pulse Constant */
#define DRV_MCMETROLOGY_CONF_IKT                    0x${REG_MCMET_CONF_IKT?left_pad(8, "0")}UL
/* Metrology Default Config: Creep Threshold for Active Energy */
#define DRV_MCMETROLOGY_CONF_CREEP_P                0x${REG_MCMET_CREEP_THRESHOLD_P?left_pad(8, "0")}UL
/* Metrology Default Config: Creep Threshold for Reactive Energy */
#define DRV_MCMETROLOGY_CONF_CREEP_Q                0x${REG_MCMET_CREEP_THRESHOLD_Q?left_pad(8, "0")}UL
/* Metrology Default Config: Creep Threshold of Phase Current */
#define DRV_MCMETROLOGY_CONF_CREEP_I                0x${REG_MCMET_CREEP_THRESHOLD_I?left_pad(8, "0")}UL
/* Metrology Default Config: Active Power Offset Control Register */
#define DRV_MCMETROLOGY_CONF_P_POWER_OFFSET_CTRL    0x${REG_MCMET_CTRL_P_POWER_OFFSET_CTRL?left_pad(8, "0")}UL
/* Metrology Default Config: Reactive Power Offset Control Register */
#define DRV_MCMETROLOGY_CONF_Q_POWER_OFFSET_CTRL    0x${REG_MCMET_CTRL_Q_POWER_OFFSET_CTRL?left_pad(8, "0")}UL
/* Metrology Default Config: Active Power Offset Register */
#define DRV_MCMETROLOGY_CONF_POFFSET_P              0x${REG_MCMET_POWER_OFFSET_P?left_pad(8, "0")}L
/* Metrology Default Config: Rective Power Offset Register */
#define DRV_MCMETROLOGY_CONF_POFFSET_Q              0x${REG_MCMET_POWER_OFFSET_Q?left_pad(8, "0")}L
/* Metrology Default Config: Voltage Swell and Overcurrent Threshold */
#define DRV_MCMETROLOGY_CONF_SWELL_OVERC_THRS_CHx   {0x${REG_MCMET_SWELL_TH_0?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_1?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_2?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_3?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_4?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_5?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_6?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_7?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_8?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_9?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_10?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_11?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_12?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_13?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_14?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_15?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_16?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_17?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_18?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_19?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_20?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_21?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_22?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_23?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_24?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_25?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_26?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_27?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_28?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_29?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_30?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SWELL_TH_31?left_pad(8, "0")}UL}
/* Metrology Default Config: Voltage Sag Threshold */
#define DRV_MCMETROLOGY_CONF_SAG_THRS_CHx           {0x${REG_MCMET_SAG_TH_0?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_1?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_2?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_3?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_4?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_5?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_6?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_7?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_8?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_9?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_10?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_11?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_12?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_13?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_14?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_15?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_16?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_17?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_18?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_19?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_20?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_21?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_22?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_23?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_24?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_25?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_26?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_27?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_28?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_29?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_30?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_SAG_TH_31?left_pad(8, "0")}UL}
/* Metrology Default Config: Voltage and Current Conversion Factor */
#define DRV_MCMETROLOGY_CONF_K_CHx                  {0x${REG_MCMET_CONF_K_CH_0?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_1?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_2?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_3?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_4?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_5?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_6?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_7?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_8?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_9?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_10?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_11?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_12?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_13?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_14?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_15?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_16?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_17?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_18?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_19?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_20?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_21?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_22?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_23?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_24?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_25?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_26?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_27?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_28?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_29?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_30?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_K_CH_31?left_pad(8, "0")}UL}
/* Metrology Default Config: Channel Magnitude Calibration Constants */
#define DRV_MCMETROLOGY_CONF_CAL_M_CHx              {0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL, \
                                                     0x20000000UL}
/* Metrology Default Config: Per-Channel Phase Calibration Constants */
#define DRV_MCMETROLOGY_CONF_CAL_PH_CHx             {0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L, \
                                                     0x00000000L}
/* Metrology Default Config: Waveform Capture Control Register */
#define DRV_MCMETROLOGY_CONF_CAPTURE_CTRL           0x${REG_MCMET_CTRL_CAPTURE_CTRL?left_pad(8, "0")}UL
/* Metrology Default Config: Waveform Capture Channel Selection Register */
#define DRV_MCMETROLOGY_CONF_CAPTURE_CH_SEL         0x${REG_MCMET_CTRL_CAPTURE_CH_SEL?left_pad(8, "0")}UL
<#assign CAPTURE_BUF_SIZE = (DRV_MCMET_CAPTURE_BUF_SIZE_CHN * DRV_MCMET_CAPTURE_CHN_NUM)>
/* Metrology Default Config: Capture Buffer Size */
#define DRV_MCMETROLOGY_CAPTURE_BUF_SIZE            ${CAPTURE_BUF_SIZE?string}UL
/* Metrology Default Config: MCP391x Control Register 1 */
#define DRV_MCMETROLOGY_CONF_MCP3914_GAIN           {0x${REG_MCMET_CONF_MCP391X_GAIN_0?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_MCP391X_GAIN_1?left_pad(8, "0")}UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL}
/* Metrology Default Config: MCP391x Control Register 2 */
#define DRV_MCMETROLOGY_CONF_MCP3914_CONFIG         {0x${REG_MCMET_CONF_MCP391X_CONFIG_0?left_pad(8, "0")}UL, \
                                                     0x${REG_MCMET_CONF_MCP391X_CONFIG_1?left_pad(8, "0")}UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL}
/* Metrology Default Config: Per-Power Active Power Offset Register */
#define DRV_MCMETROLOGY_CONF_OFFSET_P_x             {0x${DRV_MCMET_CONF_P_OFFSET_0?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_1?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_2?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_3?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_4?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_5?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_6?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_7?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_8?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_9?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_10?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_11?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_12?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_13?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_14?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_15?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_16?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_17?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_18?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_19?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_20?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_21?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_22?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_23?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_24?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_25?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_26?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_27?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_28?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_29?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_P_OFFSET_30?left_pad(8, "0")}L}
/* Metrology Default Config: Per-Power Reactive Power Offset Register */
#define DRV_MCMETROLOGY_CONF_OFFSET_Q_x             {0x${DRV_MCMET_CONF_Q_OFFSET_0?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_1?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_2?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_3?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_4?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_5?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_6?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_7?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_8?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_9?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_10?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_11?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_12?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_13?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_14?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_15?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_16?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_17?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_18?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_19?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_20?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_21?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_22?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_23?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_24?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_25?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_26?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_27?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_28?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_29?left_pad(8, "0")}L, \
                                                     0x${DRV_MCMET_CONF_Q_OFFSET_30?left_pad(8, "0")}L}

#endif /* MCMMETROLOGY_CONFIGURATION_H */