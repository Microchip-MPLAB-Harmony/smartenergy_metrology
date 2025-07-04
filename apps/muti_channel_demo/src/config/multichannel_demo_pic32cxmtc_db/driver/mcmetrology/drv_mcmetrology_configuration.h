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
#define DRV_MCMETROLOGY_CONF_FCTRL0                 0x00000300UL
/* Metrology Default Config: Metrology feature setting register 1 */
#define DRV_MCMETROLOGY_CONF_FCTRL1                 0x08000000UL
/* Metrology Default Config: Power source */
#define DRV_MCMETROLOGY_CONF_POWER_SOURCE           {0x04010300UL, \
                                                     0x00000502UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL}
/* Metrology Default Config: Meter Type (Not implemented in this version) */
#define DRV_MCMETROLOGY_CONF_MT                     0x00000000UL
/* Metrology Default Config: Sensor Type  */
#define DRV_MCMETROLOGY_CONF_ST                     {0x0000003fUL, \
                                                     0x00000000UL}
/* Metrology Default Config: PULSE0_CTRL */
#define DRV_MCMETROLOGY_CONF_PULSE0_CTRL            0x810001d0UL
/* Metrology Default Config: PULSE1_CTRL */
#define DRV_MCMETROLOGY_CONF_PULSE1_CTRL            0x810201d0UL
/* Metrology Default Config: PULSE2_CTRL */
#define DRV_MCMETROLOGY_CONF_PULSE2_CTRL            0x00000000UL
/* Metrology Default Config: PULSE_CONTRIBUTION_PW */
#define DRV_MCMETROLOGY_CONF_PULSE_PW               0x00000007UL
/* Metrology Default Config: PULSE_CONTRIBUTION_I */
#define DRV_MCMETROLOGY_CONF_PULSE_I                0x00000000UL
/* Metrology Default Config: Meter Active Power Pulse Constant */
#define DRV_MCMETROLOGY_CONF_PKT                    0x00500000UL
/* Metrology Default Config: Meter Reactive Power Pulse Constant */
#define DRV_MCMETROLOGY_CONF_QKT                    0x00500000UL
/* Metrology Default Config: Meter Current Pulse Constant */
#define DRV_MCMETROLOGY_CONF_IKT                    0x00500000UL
/* Metrology Default Config: Creep Threshold for Active Energy */
#define DRV_MCMETROLOGY_CONF_CREEP_P                0x00002e9aUL
/* Metrology Default Config: Creep Threshold for Reactive Energy */
#define DRV_MCMETROLOGY_CONF_CREEP_Q                0x00002e9aUL
/* Metrology Default Config: Creep Threshold of Phase Current */
#define DRV_MCMETROLOGY_CONF_CREEP_I                0x0000212dUL
/* Metrology Default Config: Active Power Offset Control Register */
#define DRV_MCMETROLOGY_CONF_P_POWER_OFFSET_CTRL    0x00000000UL
/* Metrology Default Config: Reactive Power Offset Control Register */
#define DRV_MCMETROLOGY_CONF_Q_POWER_OFFSET_CTRL    0x00000000UL
/* Metrology Default Config: Active Power Offset Register */
#define DRV_MCMETROLOGY_CONF_POFFSET_P              0x00000000L
/* Metrology Default Config: Rective Power Offset Register */
#define DRV_MCMETROLOGY_CONF_POFFSET_Q              0x00000000L
/* Metrology Default Config: Voltage Swell and Overcurrent Threshold */
#define DRV_MCMETROLOGY_CONF_SWELL_OVERC_THRS_CHx   {0x05e84f62UL, \
                                                     0x05e84f62UL, \
                                                     0x05e84f62UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL}
/* Metrology Default Config: Voltage Sag Threshold */
#define DRV_MCMETROLOGY_CONF_SAG_THRS_CHx           {0x01a2ec26UL, \
                                                     0x01a2ec26UL, \
                                                     0x01a2ec26UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL}
/* Metrology Default Config: Voltage and Current Conversion Factor */
#define DRV_MCMETROLOGY_CONF_K_CHx                  {0x0019cc00UL, \
                                                     0x0019cc00UL, \
                                                     0x0019cc00UL, \
                                                     0x0009a523UL, \
                                                     0x0009a523UL, \
                                                     0x0009a523UL, \
                                                     0x0009a523UL, \
                                                     0x0009a523UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL}
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
#define DRV_MCMETROLOGY_CONF_CAPTURE_CTRL           0x80000000UL
/* Metrology Default Config: Waveform Capture Channel Selection Register */
#define DRV_MCMETROLOGY_CONF_CAPTURE_CH_SEL         0x0000003fUL
/* Metrology Default Config: Capture Buffer Size */
#define DRV_MCMETROLOGY_CAPTURE_BUF_SIZE            96000UL
/* Metrology Default Config: MCP391x Control Register 1 */
#define DRV_MCMETROLOGY_CONF_MCP3914_GAIN           {0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL}
/* Metrology Default Config: MCP391x Control Register 2 */
#define DRV_MCMETROLOGY_CONF_MCP3914_CONFIG         {0x00384050UL, \
                                                     0x00384050UL, \
                                                     0x00000000UL, \
                                                     0x00000000UL}
/* Metrology Default Config: Per-Power Active Power Offset Register */
#define DRV_MCMETROLOGY_CONF_OFFSET_P_x             {0x00000000L, \
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
/* Metrology Default Config: Per-Power Reactive Power Offset Register */
#define DRV_MCMETROLOGY_CONF_OFFSET_Q_x             {0x00000000L, \
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

#endif /* MCMMETROLOGY_CONFIGURATION_H */