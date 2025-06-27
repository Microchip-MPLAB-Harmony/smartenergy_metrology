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

#ifndef MCMMETROLOGY_REGS_H
#define MCMMETROLOGY_REGS_H

/* ************************************************************************** */
/*   SOFTWARE API DEFINITION FOR METROLOGY REGISTERS                          */
/* ************************************************************************** */
/* MISRA C-2012 deviation block start */
/* MISRA C-2012 Rule 5.4 deviated 64 times.  Deviation record ID - H3_MISRAC_2012_R_5_4_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block deviate "MISRA C-2012 Rule 5.4" "H3_MISRAC_2012_R_5_4_DR_1"
</#if>

/* -------- STATE_CTRL : (MET_CONTROL) (R/W 32) Metrology state control register -------- */
#define STATE_CTRL_STATE_CTRL_Pos                             _UINT32_(0)                               /* (STATE_CTRL) State Control Position */
#define STATE_CTRL_STATE_CTRL_Msk                             (_UINT32_(0xF) << STATE_CTRL_STATE_CTRL_Pos)         /* (STATE_CTRL) State Control Mask */
#define STATE_CTRL_STATE_CTRL(value)                          (STATE_CTRL_STATE_CTRL_Msk & (_UINT32_(value) << STATE_CTRL_STATE_CTRL_Pos))
#define   STATE_CTRL_STATE_CTRL_RESET_Val                       _UINT32_(0x0)                           /* (STATE_CTRL) Reset metrology module, reset ATSENSE and halt metrology filters.  */
#define   STATE_CTRL_STATE_CTRL_INIT_Val                        _UINT32_(0x1)                           /* (STATE_CTRL) Input control registers are initialized and metrology can initialize itself.  */
#define   STATE_CTRL_STATE_CTRL_RUN_Val                         _UINT32_(0x2)                           /* (STATE_CTRL) Metrology filters are free to run, and outputs are updated at commanded rate.  */

/* -------- FEATURE_CTRL0 : (MET_CONTROL) (R/W 32) Metrology feature setting register 0 -------- */
#define FEATURE_CTRL0_SYNCH_Pos                               _UINT32_(0)                               /* (FEATURE_CTRL) Active Voltage Channel Selection Position */
#define FEATURE_CTRL0_SYNCH_Msk                               (_UINT32_(0x7F) << FEATURE_CTRL_SYNCH_Pos)    /* (FEATURE_CTRL) Active Voltage Channel Selection Mask */
#define FEATURE_CTRL0_SYNCH(value)                            (FEATURE_CTRL_SYNCH_Msk & (_UINT32_(value) << FEATURE_CTRL_SYNCH_Pos))
#define   FEATURE_CTRL0_SYNCH_ACTIVE_CH                         _UINT32_(0x0)                           /* (FEATURE_CTRL) Measurement interval based on active channel, determined dynamically */
#define FEATURE_CTRL0_RZC_THRESH_DIS_EN_Pos                   _UINT32_(7)                               /* (FEATURE_CTRL) Enable Phase A Position */
#define FEATURE_CTRL0_RZC_THRESH_DIS_EN_Msk                   (_UINT32_(0x1) << FEATURE_CTRL_RZC_THRESH_DIS_EN_Pos)    /* (FEATURE_CTRL) Enable Phase A Mask */
#define FEATURE_CTRL0_RZC_THRESH_DIS_EN(value)                (FEATURE_CTRL_RZC_THRESH_DIS_EN_Msk & (_UINT32_(value) << FEATURE_CTRL_RZC_THRESH_DIS_EN_Pos))
#define   FEATURE_CTRL0_RZC_THRESH_DIS_EN_DISABLED_Val          _UINT32_(0x0)                           /* (FEATURE_CTRL) Threshold set to the average value computed in the last integration period */
#define   FEATURE_CTRL0_RZC_THRESH_DIS_EN_ENABLED_Val           _UINT32_(0x1)                           /* (FEATURE_CTRL) Threshold set to 0 */
#define FEATURE_CTRL0_POWER_NUMBER_Pos                        _UINT32_(8)                               /* (FEATURE_CTRL) Number of powers computed Position */
#define FEATURE_CTRL0_POWER_NUMBER_Msk                        (_UINT32_(0x1F) << FEATURE_CTRL_POWER_NUMBER_Pos)    /* (FEATURE_CTRL) Number of powers computed Mask */
#define FEATURE_CTRL0_POWER_NUMBER(value)                     (FEATURE_CTRL_POWER_NUMBER_Msk & (_UINT32_(value) << FEATURE_CTRL_POWER_NUMBER_Pos))
#define FEATURE_CTRL0_MAX_INT_SELECT_Pos                      _UINT32_(15)                               /* (FEATURE_CTRL) Max Integration Period Select Position */
#define FEATURE_CTRL0_MAX_INT_SELECT_Msk                      (_UINT32_(0x1) << FEATURE_CTRL_MAX_INT_SELECT_Pos)    /* (FEATURE_CTRL) Max Integration Period Select A Mask */
#define FEATURE_CTRL0_MAX_INT_SELECT(value)                   (FEATURE_CTRL_MAX_INT_SELECT_Msk & (_UINT32_(value) << FEATURE_CTRL_MAX_INT_SELECT_Pos))
#define   FEATURE_CTRL0_MAX_INT_SELECT_1S_Val                   _UINT32_(0x0)                           /* (FEATURE_CTRL) Metrology DSP will integrate for an integral number of cycles closest to 1 second */
#define   FEATURE_CTRL0_MAX_INT_SELECT_LESS1S_Val               _UINT32_(0x1)                           /* (FEATURE_CTRL) Metrology DSP will integrate for an integral number of cycles no greater than 1 second */
#define FEATURE_CTRL0_RZC_CHAN_SELECT_Pos                     _UINT32_(16)                               /* (FEATURE_CTRL) Raw Zero-Crossing Channel Select and Enable Position */
#define FEATURE_CTRL0_RZC_CHAN_SELECT_Msk                     (_UINT32_(0x7F) << FEATURE_CTRL_RZC_CHAN_SELECT_Pos)    /* (FEATURE_CTRL) Raw Zero-Crossing Channel Select and Enable Mask */
#define FEATURE_CTRL0_RZC_CHAN_SELECT(value)                  (FEATURE_CTRL_RZC_CHAN_SELECT_Msk & (_UINT32_(value) << FEATURE_CTRL_RZC_CHAN_SELECT_Pos))
#define   FEATURE_CTRL0_RZC_CHAN_SELECT_DISABLED_Val            _UINT32_(0x0)                           /* (FEATURE_CTRL) Raw Zero-Crossing detection is disabled */
#define FEATURE_CTRL0_RZC_DIR_Pos                             _UINT32_(23)                               /* (FEATURE_CTRL) Raw Zero-Crossing Direction Selection Position */
#define FEATURE_CTRL0_RZC_DIR_Msk                             (_UINT32_(0x1) << FEATURE_CTRL_RZC_DIR_Pos)    /* (FEATURE_CTRL) Raw Zero-Crossing Direction Selection Mask */
#define FEATURE_CTRL0_RZC_DIR(value)                          (FEATURE_CTRL_RZC_DIR_Msk & (_UINT32_(value) << FEATURE_CTRL_RZC_DIR_Pos))
#define   FEATURE_CTRL0_RZC_RZC_DIR_POSITIVE_Val                _UINT32_(0x0)                           /* (FEATURE_CTRL) Positive-going raw zero-crossings selected */
#define   FEATURE_CTRL0_RZC_RZC_DIR_NEGATIVE_Val                _UINT32_(0x1)                           /* (FEATURE_CTRL) Negative-going raw zero-crossings selected */
#define FEATURE_CTRL_AFE_SELECTION_Pos                        _UINT32_(28)                               /* (FEATURE_CTRL) AFE Selection Position */
#define FEATURE_CTRL_AFE_SELECTION_Msk                        (_UINT32_(0xF) << FEATURE_CTRL_AFE_SELECTION_Pos)    /* (FEATURE_CTRL) AFE Selection Mask */
#define FEATURE_CTRL_AFE_SELECTION(value)                     (FEATURE_CTRL_AFE_SELECTION_Msk & (_UINT32_(value) << FEATURE_CTRL_AFE_SELECTION_Pos))
#define   FEATURE_CTRL_AFE_SELECTION_1_MCP3914                  _UINT32_(0x0)                           /* (FEATURE_CTRL) 8 channels and up to 7 powers are allowed. Not tested */
#define   FEATURE_CTRL_AFE_SELECTION_2_MCP3914                  _UINT32_(0x1)                           /* (FEATURE_CTRL) 16 channels and up to 15 powers are allowed. Not tested */
#define   FEATURE_CTRL_AFE_SELECTION_3_MCP3914                  _UINT32_(0x2)                           /* (FEATURE_CTRL) 24 channels and up to 23 powers are allowed. Not supported in this version */
#define   FEATURE_CTRL_AFE_SELECTION_4_MCP3914                  _UINT32_(0x3)                           /* (FEATURE_CTRL) 32 channels and up to 31 powers are allowed. Not supported in this version */
#define   FEATURE_CTRL_AFE_SELECTION_1_MCP3913                  _UINT32_(0x4)                           /* (FEATURE_CTRL) 6 channels and up to 5 powers are allowed */
#define   FEATURE_CTRL_AFE_SELECTION_2_MCP3913                  _UINT32_(0x5)                           /* (FEATURE_CTRL) 12 channels and up to 11 powers are allowed */
#define   FEATURE_CTRL_AFE_SELECTION_3_MCP3913                  _UINT32_(0x6)                           /* (FEATURE_CTRL) 18 channels and up to 17 powers are allowed. Not supported in this version */
#define   FEATURE_CTRL_AFE_SELECTION_4_MCP3913                  _UINT32_(0x7)                           /* (FEATURE_CTRL) 24 channels and up to 23 powers are allowed. Not supported in this version */

/* -------- FEATURE_CTRL1 : (MET_CONTROL) (R/W 32) Metrology feature setting register 1 -------- */
#define FEATURE_CTRL1_CREEP_I_EN_Pos             _UINT32_(0)                                 /* (FEATURE_CTRL1) Current Creep Threshold Function Enable Position */
#define FEATURE_CTRL1_CREEP_I_EN_Msk             (_UINT32_(0x1) << FEATURE_CTRL1_CREEP_I_EN_Pos)    /* (FEATURE_CTRL1) Current Creep Threshold Function Enable Mask */
#define FEATURE_CTRL1_CREEP_I_EN(value)          (FEATURE_CTRL1_CREEP_I_EN_Msk & (_UINT32_(value) << FEATURE_CTRL1_CREEP_I_EN_Pos))
#define   FEATURE_CTRL1_CREEP_I_EN_DISABLED_Val    _UINT32_(0x0)                             /* (FEATURE_CTRL1) Disable current creep threshold feature */
#define   FEATURE_CTRL1_CREEP_I_EN_ENABLED_Val     _UINT32_(0x1)                             /* (FEATURE_CTRL1) Enable current creep threshold feature */
#define FEATURE_CTRL1_CREEP_Q_EN_Pos             _UINT32_(1)                                 /* (FEATURE_CTRL1) Reactive Creep Threshold Function Enable Position */
#define FEATURE_CTRL1_CREEP_Q_EN_Msk             (_UINT32_(0x1) << FEATURE_CTRL1_CREEP_Q_EN_Pos)    /* (FEATURE_CTRL1) Reactive Creep Threshold Function Enable Mask */
#define FEATURE_CTRL1_CREEP_Q_EN(value)          (FEATURE_CTRL1_CREEP_Q_EN_Msk & (_UINT32_(value) << FEATURE_CTRL1_CREEP_Q_EN_Pos))
#define   FEATURE_CTRL1_CREEP_Q_EN_DISABLED_Val    _UINT32_(0x0)                             /* (FEATURE_CTRL1) Disable Reactive creep threshold feature */
#define   FEATURE_CTRL1_CREEP_Q_EN_ENABLED_Val     _UINT32_(0x1)                             /* (FEATURE_CTRL1) Enable Reactive creep threshold feature */
#define FEATURE_CTRL1_CREEP_P_EN_Pos             _UINT32_(2)                                 /* (FEATURE_CTRL1) Active Creep Threshold Function Enable Position */
#define FEATURE_CTRL1_CREEP_P_EN_Msk             (_UINT32_(0x1) << FEATURE_CTRL1_CREEP_P_EN_Pos)    /* (FEATURE_CTRL1) Active Creep Threshold Function Enable Mask */
#define FEATURE_CTRL1_CREEP_P_EN(value)          (FEATURE_CTRL1_CREEP_P_EN_Msk & (_UINT32_(value) << FEATURE_CTRL1_CREEP_P_EN_Pos))
#define   FEATURE_CTRL1_CREEP_P_EN_DISABLED_Val    _UINT32_(0x0)                             /* (FEATURE_CTRL1) Disable Active creep threshold feature */
#define   FEATURE_CTRL1_CREEP_P_EN_ENABLED_Val     _UINT32_(0x1)                             /* (FEATURE_CTRL1) Enable Active creep threshold feature */
#define FEATURE_CTRL1_HARMONIC_m_REQ_Pos            _UINT32_(8)                              /* (FEATURE_CTRL1) Request number of harmonic for analysis Position */
#define FEATURE_CTRL1_HARMONIC_m_REQ_Msk            (_UINT32_(0x3F) << FEATURE_CTRL1_HARMONIC_m_REQ_Pos)    /* (FEATURE_CTRL1) Request number of harmonic for analysis Mask */
#define FEATURE_CTRL1_HARMONIC_m_REQ(value)         (FEATURE_CTRL1_HARMONIC_m_REQ_Msk & (_UINT32_(value) << FEATURE_CTRL1_HARMONIC_m_REQ_Pos))
#define FEATURE_CTRL1_HARMONIC_EN_Pos            _UINT32_(15)                                /* (FEATURE_CTRL1) Enable Harmonic Analysis Position */
#define FEATURE_CTRL1_HARMONIC_EN_Msk            (_UINT32_(0x1) << FEATURE_CTRL1_HARMONIC_EN_Pos)    /* (FEATURE_CTRL1) Enable Harmonic Analysis Mask */
#define FEATURE_CTRL1_HARMONIC_EN(value)         (FEATURE_CTRL1_HARMONIC_EN_Msk & (_UINT32_(value) << FEATURE_CTRL1_HARMONIC_EN_Pos))
#define   FEATURE_CTRL1_HARMONIC_EN_DISABLED_Val   _UINT32_(0x0)                             /* (FEATURE_CTRL1) Disable harmonic analysis  */
#define   FEATURE_CTRL1_HARMONIC_EN_ENABLED_Val    _UINT32_(0x1)                             /* (FEATURE_CTRL1) Enable harmonic analysis  */
#define FEATURE_CTRL1_I_MAX_RESET_Pos            _UINT32_(19)                                /* (FEATURE_CTRL1) Reset All I_x_MAX values Position */
#define FEATURE_CTRL1_I_MAX_RESET_Msk            (_UINT32_(0x1) << FEATURE_CTRL1_I_MAX_RESET_Pos)    /* (FEATURE_CTRL1) Reset All I_x_MAX values Mask */
#define FEATURE_CTRL1_I_MAX_RESET(value)         (FEATURE_CTRL1_I_MAX_RESET_Msk & (_UINT32_(value) << FEATURE_CTRL1_I_MAX_RESET_Pos))
#define FEATURE_CTRL1_V_MAX_RESET_Pos            _UINT32_(23)                                /* (FEATURE_CTRL1) Reset All V_x_MAX values Position */
#define FEATURE_CTRL1_V_MAX_RESET_Msk            (_UINT32_(0x1) << FEATURE_CTRL1_V_MAX_RESET_Pos)    /* (FEATURE_CTRL1) Reset All V_x_MAX values Mask */
#define FEATURE_CTRL1_V_MAX_RESET(value)         (FEATURE_CTRL1_V_MAX_RESET_Msk & (_UINT32_(value) << FEATURE_CTRL1_V_MAX_RESET_Pos))

/* -------- POWER_SOURCE : (MET_CONTROL) (R/W 32) Metrology power source -------- */
#define POWER_SOURCE_PH_a_V_SOURCE_Pos            _UINT32_(0)                                /* (POWER_SOURCE) Selects the PH A channels with the voltage source of each power Position */
#define POWER_SOURCE_PH_a_V_SOURCE_Msk            (_UINT32_(0x1F) << POWER_SOURCE_PH_a_V_SOURCE_Pos)    /* (POWER_SOURCE) Selects the PH A channels with the voltage source of each power Mask */
#define POWER_SOURCE_PH_a_V_SOURCE(value)         (POWER_SOURCE_PH_a_V_SOURCE_Msk & (_UINT32_(value) << POWER_SOURCE_PH_a_V_SOURCE_Pos))
#define POWER_SOURCE_PH_a_I_SOURCE_Pos            _UINT32_(8)                                /* (POWER_SOURCE) Selects the PH A channels with the current source of each power Position */
#define POWER_SOURCE_PH_a_I_SOURCE_Msk            (_UINT32_(0x1F) << POWER_SOURCE_PH_a_I_SOURCE_Pos)    /* (POWER_SOURCE) Selects the PH A channels with the current source of each power Mask */
#define POWER_SOURCE_PH_a_I_SOURCE(value)         (POWER_SOURCE_PH_a_I_SOURCE_Msk & (_UINT32_(value) << POWER_SOURCE_PH_a_I_SOURCE_Pos))
#define POWER_SOURCE_PH_b_V_SOURCE_Pos            _UINT32_(16)                                /* (POWER_SOURCE) Selects the PH B channels with the voltage source of each power Position */
#define POWER_SOURCE_PH_b_V_SOURCE_Msk            (_UINT32_(0x1F) << POWER_SOURCE_PH_b_V_SOURCE_Pos)    /* (POWER_SOURCE) Selects the PH B channels with the voltage source of each power Mask */
#define POWER_SOURCE_PH_b_V_SOURCE(value)         (POWER_SOURCE_PH_b_V_SOURCE_Msk & (_UINT32_(value) << POWER_SOURCE_PH_b_V_SOURCE_Msk))
#define POWER_SOURCE_PH_b_I_SOURCE_Pos            _UINT32_(24)                                /* (POWER_SOURCE) Selects the PH B channels with the current source of each power Position */
#define POWER_SOURCE_PH_b_I_SOURCE_Msk            (_UINT32_(0x1F) << POWER_SOURCE_PH_b_I_SOURCE_Pos)    /* (POWER_SOURCE) Selects the PH B channels with the current source of each power Mask */
#define POWER_SOURCE_PH_b_I_SOURCE(value)         (POWER_SOURCE_PH_b_I_SOURCE_Msk & (_UINT32_(value) << POWER_SOURCE_PH_b_I_SOURCE_Pos))

/* -------- SENSOR_TYPE : (MET_CONTROL) (R/W 32) Channel type and Sensor Configuration Register -------- */
#define SENSOR_TYPE_CHx_Pos                       _UINT32_(0)                               /* (SENSOR_TYPE) Current Sensor Type on A Phase Position */
#define SENSOR_TYPE_CHx_Msk                       (_UINT32_(0x3) << SENSOR_TYPE_CHx_Pos)    /* (SENSOR_TYPE) Current Sensor Type on A Phase Mask */
#define SENSOR_TYPE_CHx(channel, value)           ((SENSOR_TYPE_CHx_Msk & (_UINT32_(value) << SENSOR_TYPE_CHx_Pos)) << (1 << channel))
#define   SENSOR_TYPE_CHx_CT_Val                    _UINT32_(0x0)                             /* (SENSOR_TYPE) Current Transformer */
#define   SENSOR_TYPE_CHx_SHUNT_Val                 _UINT32_(0x1)                             /* (SENSOR_TYPE) Resistive Shunt */
#define   SENSOR_TYPE_CHx_CRC_Val                   _UINT32_(0x2)                             /* (SENSOR_TYPE) Rogowski Coil (CRC) current sensor */
#define   SENSOR_TYPE_CHx_VRD_Val                   _UINT32_(0x3)                             /* (SENSOR_TYPE) Resistive Divider (VRD) voltage sensor */

/* -------- PULSEx_CTRL : (MET_CONTROL) (R/W 32) Pulse 0 control: enable/disable, polarity, quantity type selection -------- */
#define PULSEx_CTRL_PCx_WIDTH_Pos                  _UINT32_(0)                                 /* (PULSEx_CTRL) Pulse Width  */
#define PULSEx_CTRL_PCx_WIDTH_Msk                  (_UINT32_(0xFFFF) << PULSEx_CTRL_PCx_WIDTH_Pos)      /* (PULSEx_CTRL) Pulse Width  */
#define PULSEx_CTRL_PCx_WIDTH(value)               (PULSEx_CTRL_PCx_WIDTH_Msk & (_UINT32_(value) << PULSEx_CTRL_PCx_WIDTH_Pos))
#define PULSEx_CTRL_PCx_TYPE_Pos                  _UINT32_(16)                                 /* (PULSEx_CTRL) Pulse Type Selection Position */
#define PULSEx_CTRL_PCx_TYPE_Msk                  (_UINT32_(0xF) << PULSEx_CTRL_PCx_TYPE_Pos)      /* (PULSEx_CTRL) Pulse Type Selection Mask */
#define PULSEx_CTRL_PCx_TYPE(value)               (PULSEx_CTRL_PCx_TYPE_Msk & (_UINT32_(value) << PULSEx_CTRL_PCx_TYPE_Pos))
#define   PULSEx_CTRL_PCx_TYPE_P_T_Val              _UINT32_(0x0)                             /* (PULSEx_CTRL) Watt-hours, total all phases, fundamental + harmonics */
#define   PULSEx_CTRL_PCx_TYPE_P_T_F_Val            _UINT32_(0x1)                             /* (PULSEx_CTRL) Watt-hours, total all phases, fundamental only */
#define   PULSEx_CTRL_PCx_TYPE_Q_T_Val              _UINT32_(0x2)                             /* (PULSEx_CTRL) Var-hours, total all phases, fundamental + harmonics */
#define   PULSEx_CTRL_PCx_TYPE_Q_T_F_Val            _UINT32_(0x3)                             /* (PULSEx_CTRL) Var-hours, total all phases, fundamental only */
#define   PULSEx_CTRL_PCx_TYPE_I_T_Val              _UINT32_(0x4)                             /* (PULSEx_CTRL) Amp-squared-hours, total all phases, fundamental + harmonics */
#define   PULSEx_CTRL_PCx_TYPE_I_T_F_Val            _UINT32_(0x5)                             /* (PULSEx_CTRL) Amp-squared-hours, total all phases, fundamental only */
#define PULSEx_CTRL_PCx_POLARITY_Pos               _UINT32_(24)                                 /* (PULSEx_CTRL) Pulse Polarity Position  */
#define PULSEx_CTRL_PCx_POLARITY_Msk               (_UINT32_(0x1) << PULSEx_CTRL_PCx_POLARITY_Pos)      /* (PULSEx_CTRL) Pulse Polarity Mask  */
#define PULSEx_CTRL_PCx_POLARITY(value)            (PULSEx_CTRL_PCx_POLARITY_Msk & (_UINT32_(value) << PULSEx_CTRL_PCx_POLARITY_Pos))
#define   PULSEx_CTRL_PCx_POLARITY_LOW_Val           _UINT32_(0x0)                             /* (PULSEx_CTRL) Output pulse is low with width set by K_WIDTH, followed by an inactive level of high */
#define   PULSEx_CTRL_PCx_POLARITY_HIGH_Val          _UINT32_(0x1)                             /* (PULSEx_CTRL) Output pulse is high with width set by K_WIDTH, followed by an inactive level of low */
#define PULSEx_CTRL_PCx_OVERRIDE_Pos               _UINT32_(25)                                 /* (PULSEx_CTRL) Pulse Override Control Position  */
#define PULSEx_CTRL_PCx_OVERRIDE_Msk               (_UINT32_(0x1) << PULSEx_CTRL_PCx_OVERRIDE_Pos)      /* (PULSEx_CTRL) Pulse Override Control Mask  */
#define PULSEx_CTRL_PCx_OVERRIDE(value)            (PULSEx_CTRL_PCx_OVERRIDE_Msk & (_UINT32_(value) << PULSEx_CTRL_PCx_OVERRIDE_Pos))
#define   PULSEx_CTRL_PCx_OVERRIDE_ENABLED_Val       _UINT32_(0x0)                             /* (PULSEx_CTRL) Pulse generation functions as normal */
#define   PULSEx_CTRL_PCx_OVERRIDE_DISABLED_Val      _UINT32_(0x1)                             /* (PULSEx_CTRL) Pulse generation is temporarily disabled */
#define PULSEx_CTRL_PCx_ACC_HOLD_Pos               _UINT32_(26)                                 /* (PULSEx_CTRL) Pulse Accumulation Hold Control Position  */
#define PULSEx_CTRL_PCx_ACC_HOLD_Msk               (_UINT32_(0x1) << PULSEx_CTRL_PCx_ACC_HOLD_Pos)      /* (PULSEx_CTRL) Pulse Accumulation Hold Control Mask  */
#define PULSEx_CTRL_PCx_ACC_HOLD(value)            (PULSEx_CTRL_PCx_ACC_HOLD_Msk & (_UINT32_(value) << PULSEx_CTRL_PCx_ACC_HOLD_Pos))
#define   PULSEx_CTRL_PCx_ACC_HOLD_DISABLED_Val      _UINT32_(0x0)                             /* (PULSEx_CTRL) Pulse accumulation registers continue to increment even if pulse generation is temporarily disabled */
#define   PULSEx_CTRL_PCx_ACC_HOLD_ENABLED_Val       _UINT32_(0x1)                             /* (PULSEx_CTRL) Pulse accumulation is held static */
#define PULSEx_CTRL_OPD_ENABLE_Pos                 _UINT32_(27)                                /* (PULSEx_CTRL) Open Drain Enable Position  */
#define PULSEx_CTRL_OPD_ENABLE_Msk                 (_UINT32_(0x1) << PULSEx_CTRL_OPD_ENABLE_Pos)   /* (PULSEx_CTRL) Open Drain Enable Mask  */
#define PULSEx_CTRL_OPD_ENABLE(value)              (PULSEx_CTRL_OPD_ENABLE_Msk & (_UINT32_(value) << PULSEx_CTRL_OPD_ENABLE_Pos))
#define   PULSEx_CTRL_OPD_ENABLE_DISABLED_Val        _UINT32_(0x0)                             /* (PULSEx_CTRL) The open drain is disabled */
#define   PULSEx_CTRL_OPD_ENABLE_ENABLED_Val         _UINT32_(0x1)                             /* (PULSEx_CTRL) The open drain is enabled */
#define PULSEx_CTRL_PCx_DETENT_Pos                 _UINT32_(28)                                /* (PULSEx_CTRL) Total Absolute Values for P and Q values Position  */
#define PULSEx_CTRL_PCx_DETENT_Msk                 (_UINT32_(0x3) << PULSEx_CTRL_PCx_DETENT_Pos)   /* (PULSEx_CTRL) Total Absolute Values for P and Q values Mask  */
#define PULSEx_CTRL_PCx_DETENT(value)              (PULSEx_CTRL_PCx_DETENT_Msk & (_UINT32_(value) << PULSEx_CTRL_PCx_DETENT_Pos))
#define   PULSEx_CTRL_PCx_DETENT_NET_Val             _UINT32_(0x0)                             /* (PULSEx_CTRL) Pulses generated based on (energy delivered ? energy generated) */
#define   PULSEx_CTRL_PCx_DETENT_ABSOLUTE_Val        _UINT32_(0x1)                             /* (PULSEx_CTRL) Pulses generated based on (energy delivered + energy generated) */
#define   PULSEx_CTRL_PCx_DETENT_DELIVERED_Val       _UINT32_(0x2)                             /* (PULSEx_CTRL) Pulses generated based on (energy delivered only) */
#define   PULSEx_CTRL_PCx_DETENT_GENERATED_Val       _UINT32_(0x3)                             /* (PULSEx_CTRL) Pulses generated based on (energy generated only) */
#define PULSEx_CTRL_PU_ENABLE_Pos                  _UINT32_(30)                                /* (PULSEx_CTRL) Pull-Up Enable Position  */
#define PULSEx_CTRL_PU_ENABLE_Msk                  (_UINT32_(0x1) << PULSEx_CTRL_PU_ENABLE_Pos)   /* (PULSEx_CTRL) Pull-Up Enable Mask  */
#define PULSEx_CTRL_PU_ENABLE(value)               (PULSEx_CTRL_PU_ENABLE_Msk & (_UINT32_(value) << PULSEx_CTRL_PU_ENABLE_Pos))
#define   PULSEx_CTRL_PU_ENABLE_DISABLED_Val         _UINT32_(0x0)                             /* (PULSEx_CTRL) Pull-Up disabled */
#define   PULSEx_CTRL_PU_ENABLE_ENABLED_Val          _UINT32_(0x1)                             /* (PULSEx_CTRL) Pull-Up enabled */
#define PULSEx_CTRL_PCx_ENABLE_Pos                 _UINT32_(31)                                /* (PULSEx_CTRL) Output Pulse Enable Position  */
#define PULSEx_CTRL_PCx_ENABLE_Msk                 (_UINT32_(0x1) << PULSEx_CTRL_PCx_ENABLE_Pos)   /* (PULSEx_CTRL) Output Pulse Enable Mask  */
#define PULSEx_CTRL_PCx_ENABLE(value)              (PULSEx_CTRL_PCx_ENABLE_Msk & (_UINT32_(value) << PULSEx_CTRL_PCx_ENABLE_Pos))
#define   PULSEx_CTRL_PCx_ENABLE_DISABLED_Val        _UINT32_(0x0)                             /* (PULSEx_CTRL) Pulse output disabled, inactive level determined by PULSE_POLARITY */
#define   PULSEx_CTRL_PCx_ENABLE_ENABLED_Val         _UINT32_(0x1)                             /* (PULSEx_CTRL) Pulse output enabled */

/* -------- PULSE_CONTRIBUTION_PW : (MET_CONTROL) (R/W 32) Pulse Contribution Power Register -------- */
#define PULSE_CONTRIBUTION_PW_Pos                       _UINT32_(0)                               /* (PULSE_CONTRIBUTION_PW) Enable Power x for pulse computation Position */
#define PULSE_CONTRIBUTION_PW_Msk                       (_UINT32_(0x1) << PULSE_CONTRIBUTION_PW_Pos)    /* (PULSE_CONTRIBUTION_PW) Enable Power x for pulse computation Mask */
#define PULSE_CONTRIBUTION_PW_VALUE(psource, value)     ((PULSE_CONTRIBUTION_PW_Msk & (_UINT32_(value) << PULSE_CONTRIBUTION_PW_Pos)) << psource)
#define   PULSE_CONTRIBUTION_PW_DISABLED_Val              _UINT32_(0x0)                             /* (PULSE_CONTRIBUTION_PW) Disable power */
#define   PULSE_CONTRIBUTION_PW_ENABLED_Val               _UINT32_(0x1)                             /* (PULSE_CONTRIBUTION_PW) Enable power */

/* -------- PULSE_CONTRIBUTION_I : (MET_CONTROL) (R/W 32) Pulse Contribution Power Register -------- */
#define PULSE_CONTRIBUTION_I_Pos                       _UINT32_(0)                               /* (PULSE_CONTRIBUTION_I) Enable current x for pulse computation Position */
#define PULSE_CONTRIBUTION_I_Msk                       (_UINT32_(0x1) << PULSE_CONTRIBUTION_I_Pos)    /* (PULSE_CONTRIBUTION_I) Enable current x for pulse computation Mask */
#define PULSE_CONTRIBUTION_I_VALUE(psource, value)     ((PULSE_CONTRIBUTION_I_Msk & (_UINT32_(value) << PULSE_CONTRIBUTION_I_Pos)) << psource)
#define   PULSE_CONTRIBUTION_I_DISABLED_Val               _UINT32_(0x0)                             /* (PULSE_CONTRIBUTION_I) Disable current */
#define   PULSE_CONTRIBUTION_I_ENABLED_Val                _UINT32_(0x1)                             /* (PULSE_CONTRIBUTION_I) Enable current */

/* -------- P_POWER_OFFSET_CTRL : (MET_CONTROL) (R/W 32) Active Power Offset Control Register -------- */
#define P_POWER_OFFSET_CTRL_ACC_PWx_Pos                       _UINT32_(0)                               /* (P_POWER_OFFSET_CTRL) Active Power Offset for Accumulator Quantities Enable Position */
#define P_POWER_OFFSET_CTRL_ACC_PWx_Msk                       (_UINT32_(0x1) << P_POWER_OFFSET_CTRL_ACC_PWx_Pos)    /* (P_POWER_OFFSET_CTRL) Active Power Offset for Accumulator Quantities Enable Mask */
#define P_POWER_OFFSET_CTRL_ACC_PWx(psource, value)           ((P_POWER_OFFSET_CTRL_ACC_PWx_Msk & (_UINT32_(value) << P_POWER_OFFSET_CTRL_ACC_PWx_Pos)) << psource)
#define   P_POWER_OFFSET_CTRL_ACC_PWx_DISABLED_Val               _UINT32_(0x0)                             /* (P_POWER_OFFSET_CTRL) Disable offset */
#define   P_POWER_OFFSET_CTRL_ACC_PWx_ENABLED_Val                _UINT32_(0x1)                             /* (P_POWER_OFFSET_CTRL) Enable offset */
#define P_POWER_OFFSET_CTRL_PUL_Pos                           _UINT32_(31)                               /* (P_POWER_OFFSET_CTRL) Active Power Offset for Pulse Quantities Enable Position */
#define P_POWER_OFFSET_CTRL_PUL_Msk                           (_UINT32_(0x1) << P_POWER_OFFSET_CTRL_PUL_Pos)    /* (P_POWER_OFFSET_CTRL) Active Power Offset for Pulse Quantities Enable Mask */
#define P_POWER_OFFSET_CTRL_PUL(value)                        (P_POWER_OFFSET_CTRL_PUL_Msk & (_UINT32_(value) << P_POWER_OFFSET_CTRL_PUL_Pos))
#define   P_POWER_OFFSET_CTRL_PUL_DISABLED_Val                   _UINT32_(0x0)                             /* (P_POWER_OFFSET_CTRL) Disable offset */
#define   P_POWER_OFFSET_CTRL_PUL_ENABLED_Val                    _UINT32_(0x1)                             /* (P_POWER_OFFSET_CTRL) Enable offset */

/* -------- Q_POWER_OFFSET_CTRL : (MET_CONTROL) (R/W 32) Active Power Offset Control Register -------- */
#define Q_POWER_OFFSET_CTRL_ACC_PWx_Pos                       _UINT32_(0)                               /* (Q_POWER_OFFSET_CTRL) Reactive Power Offset for Accumulator Quantities Enable Position */
#define Q_POWER_OFFSET_CTRL_ACC_PWx_Msk                       (_UINT32_(0x1) << Q_POWER_OFFSET_CTRL_ACC_PWx_Pos)    /* (Q_POWER_OFFSET_CTRL) Reactive Power Offset for Accumulator Quantities Enable Mask */
#define Q_POWER_OFFSET_CTRL_ACC_PWx(psource, value)           ((Q_POWER_OFFSET_CTRL_ACC_PWx_Msk & (_UINT32_(value) << Q_POWER_OFFSET_CTRL_ACC_PWx_Pos)) << psource)
#define   Q_POWER_OFFSET_CTRL_ACC_PWx_DISABLED_Val               _UINT32_(0x0)                             /* (Q_POWER_OFFSET_CTRL) Disable offset */
#define   Q_POWER_OFFSET_CTRL_ACC_PWx_ENABLED_Val                _UINT32_(0x1)                             /* (Q_POWER_OFFSET_CTRL) Enable offset */
#define Q_POWER_OFFSET_CTRL_PUL_Pos                           _UINT32_(31)                               /* (Q_POWER_OFFSET_CTRL) Reactive Power Offset for Pulse Quantities Enable Position */
#define Q_POWER_OFFSET_CTRL_PUL_Msk                           (_UINT32_(0x1) << Q_POWER_OFFSET_CTRL_PUL_Pos)    /* (Q_POWER_OFFSET_CTRL) Reactive Power Offset for Pulse Quantities Enable Mask */
#define Q_POWER_OFFSET_CTRL_PUL(value)                        (Q_POWER_OFFSET_CTRL_PUL_Msk & (_UINT32_(value) << Q_POWER_OFFSET_CTRL_PUL_Pos))
#define   Q_POWER_OFFSET_CTRL_PUL_DISABLED_Val                   _UINT32_(0x0)                             /* (Q_POWER_OFFSET_CTRL) Disable offset */
#define   Q_POWER_OFFSET_CTRL_PUL_ENABLED_Val                    _UINT32_(0x1)                             /* (Q_POWER_OFFSET_CTRL) Enable offset */

/* -------- CAPTURE_SH_SEL : (MET_CONTROL) (R/W 32) Waveform Capture Channel Selection Register -------- */
#define CAPTURE_SH_SEL_Pos                       _UINT32_(0)                              /* (CAPTURE_SH_SEL) Capture Channel x Select Position */
#define CAPTURE_SH_SEL_Msk                       (_UINT32_(0x1) << CAPTURE_SH_SEL_Pos)    /* (CAPTURE_SH_SEL) Capture Channel x Select Mask */
#define CAPTURE_SH_SEL_VALUE(channel, value)     ((CAPTURE_SH_SEL_Msk & (_UINT32_(value) << CAPTURE_SH_SEL_Pos)) << channel)
#define   CAPTURE_SH_SEL_DISABLED_Val               _UINT32_(0x0)                             /* (CAPTURE_SH_SEL) Waveform capture is disabled for channel x */
#define   CAPTURE_SH_SEL_ENABLED_Val                _UINT32_(0x1)                             /* (CAPTURE_SH_SEL) Waveform capture is enabled for channel x */

/* -------- MCP391x_GAIN : (MET_CONTROL) (R/W 32) MCP391x Gain Control Register -------- */
#define MCP391x_GAIN_PGA_CHx_Pos                       _UINT32_(0)                              /* (MCP391x_GAIN) PGA setting for channel n Position */
#define MCP391x_GAIN_PGA_CHx_Msk                       (_UINT32_(0x7) << MCP391x_GAIN_PGA_CHx_Pos)    /* (MCP391x_GAIN) PGA setting for channel n Mask */
#define MCP391x_GAIN_PGA_CHx(channel, value)           ((MCP391x_GAIN_PGA_CHx_Msk & (_UINT32_(value) << MCP391x_GAIN_PGA_CHx_Pos)) << (3 * channel))
#define   MCP391x_GAIN_PGA_CHx_GAIN1_Val                 _UINT32_(0x0)                           /* (MCP391x_GAIN) Gain is 1 */
#define   MCP391x_GAIN_PGA_CHx_GAIN2_Val                 _UINT32_(0x1)                           /* (MCP391x_GAIN) Gain is 2 */
#define   MCP391x_GAIN_PGA_CHx_GAIN4_Val                 _UINT32_(0x2)                           /* (MCP391x_GAIN) Gain is 4 */
#define   MCP391x_GAIN_PGA_CHx_GAIN8_Val                 _UINT32_(0x3)                           /* (MCP391x_GAIN) Gain is 8 */
#define   MCP391x_GAIN_PGA_CHx_GAIN16_Val                _UINT32_(0x4)                           /* (MCP391x_GAIN) Gain is 16 */
#define   MCP391x_GAIN_PGA_CHx_GAIN22_Val                _UINT32_(0x5)                           /* (MCP391x_GAIN) Gain is 32 */

/* -------- MCP391x_CONFIG : (MET_CONTROL) (R/W 32) MCP391x Control Register -------- */
#define MCP391x_CONFIG_DITHER_Pos                       _UINT32_(20)                            /* (MCP391x_CONFIG) Dithering circuit for Idle Tone's cancellation Position */
#define MCP391x_CONFIG_DITHER_Msk                       (_UINT32_(0x3) << MCP391x_CONFIG_DITHER_Pos)    /* (MCP391x_CONFIG) Dithering circuit for Idle Tone's cancellation Mask */
#define MCP391x_CONFIG_DITHER(value)                    (MCP391x_CONFIG_DITHER_Msk & (_UINT32_(value) << MCP391x_CONFIG_DITHER_Pos))
#define   MCP391x_CONFIG_DITHER_OFF_Val                   _UINT32_(0x0)                           /* (MCP391x_CONFIG) Dithering off */
#define   MCP391x_CONFIG_DITHER_ON_MIN_Val                _UINT32_(0x1)                           /* (MCP391x_CONFIG) Dithering on, Strength Minimum */
#define   MCP391x_CONFIG_DITHER_ON_MED_Val                _UINT32_(0x2)                           /* (MCP391x_CONFIG) Dithering on, Strength Medium */
#define   MCP391x_CONFIG_DITHER_ON_MAX_Val                _UINT32_(0x3)                           /* (MCP391x_CONFIG) Dithering on, Strength Maximum */
#define MCP391x_CONFIG_BOOST_Pos                       _UINT32_(20)                             /* (MCP391x_CONFIG) BIAS current selection for all ADCs Position */
#define MCP391x_CONFIG_BOOST_Msk                       (_UINT32_(0x3) << MCP391x_CONFIG_BOOST_Pos)    /* (MCP391x_CONFIG) BIAS current selection for all ADCs Mask */
#define MCP391x_CONFIG_BOOST(value)                    (MCP391x_CONFIG_BOOST_Msk & (_UINT32_(value) << MCP391x_CONFIG_BOOST_Pos))
#define   MCP391x_CONFIG_BOOST_x05_Val                  _UINT32_(0x0)                             /* (MCP391x_CONFIG) All channels have current x0.5 */
#define   MCP391x_CONFIG_BOOST_x066_Val                 _UINT32_(0x1)                             /* (MCP391x_CONFIG) All channels have current x0.66 */
#define   MCP391x_CONFIG_BOOST_x1_Val                   _UINT32_(0x2)                             /* (MCP391x_CONFIG) All channels have current x1 */
#define   MCP391x_CONFIG_BOOST_x2_Val                   _UINT32_(0x3)                             /* (MCP391x_CONFIG) All channels have current x2 */

/* -------- VERSION : (MET_STATUS) (R/W 32) Metrology Version -------- */
#define VERSION_FIRMWARE_REV_Pos                    _UINT32_(0)                                 /* (VERSION) Firmware Revision Position */
#define VERSION_FIRMWARE_REV_Msk                    (_UINT32_(0xFF) << VERSION_FIRMWARE_REV_Pos)      /* (VERSION) Firmware Revision Mask */
#define VERSION_FIRMWARE_REV(value)                 (VERSION_FIRMWARE_REV_Msk & (_UINT32_(value) << VERSION_FIRMWARE_REV_Pos))
#define VERSION_FIRMWARE_MINOR_REV_Pos              _UINT32_(8)                                 /* (VERSION) Minor Firmware Revision Position */
#define VERSION_FIRMWARE_MINOR_REV_Msk              (_UINT32_(0xFF) << VERSION_FIRMWARE_MINOR_REV_Pos)      /* (VERSION) Minor Firmware Revision Mask */
#define VERSION_FIRMWARE_MINOR_REV(value)           (VERSION_FIRMWARE_MINOR_REV_Msk & (_UINT32_(value) << VERSION_FIRMWARE_MINOR_REV_Pos))
#define VERSION_FIRMWARE_MAJOR_REV_Pos              _UINT32_(8)                                 /* (VERSION) Major  Firmware Revision Position */
#define VERSION_FIRMWARE_MAJOR_REV_Msk              (_UINT32_(0xFF) << VERSION_FIRMWARE_MAJOR_REV_Pos)      /* (VERSION) Major  Firmware Revision Mask */
#define VERSION_FIRMWARE_MAJOR_REV(value)           (VERSION_FIRMWARE_MAJOR_REV_Msk & (_UINT32_(value) << VERSION_FIRMWARE_MAJOR_REV_Pos))

/* -------- STATUS : (MET_STATUS) (R/W 32) Metrology status -------- */
#define STATUS_STATUS_Pos                           _UINT32_(0)                       /* (STATUS) Metrology status Position */
#define STATUS_STATUS_Msk                           (_UINT32_(0x0F) << STATUS_STATUS_Pos)      /* (STATUS) Metrology status Mask */
#define STATUS_STATUS(value)                        (STATUS_STATUS_Msk & (_UINT32_(value) << STATUS_STATUS_Pos))
#define   STATUS_STATUS_HALT                        _UINT32_(0x0)                   /* (STATUS) Metrology module is halted */
#define   STATUS_STATUS_RESET                       _UINT32_(0x1)                   /* (STATUS) Resetting metrology module */
#define   STATUS_STATUS_INIT_DSP                    _UINT32_(0x2)                   /* (STATUS) Initializing DSP filters */
#define   STATUS_STATUS_DSP_READY                   _UINT32_(0x3)                   /* (STATUS) DSP filters have been initialized */
#define   STATUS_STATUS_INIT_ATSENSE                _UINT32_(0x4)                   /* (STATUS) Initializing ATSENSE */
#define   STATUS_STATUS_ATSENSE_READY               _UINT32_(0x5)                   /* (STATUS) ATSENSE has been initialized */
#define   STATUS_STATUS_READY                       _UINT32_(0x6)                   /* (STATUS) ATSENSE temperature configuration & calibration data has been copied into output registers */
#define   STATUS_STATUS_DSP_SETTLING                _UINT32_(0x7)                   /* (STATUS) Waiting for DSP filters to stabilize to full accuracy */
#define   STATUS_STATUS_DSP_RUNNING                 _UINT32_(0x8)                   /* (STATUS) DSP filters have stabilized to full accuracy */

/* -------- STATUS : (MET_STATUS) (R/W 32) Metrology state flags -------- */
#define STATUS_STATE_FLAG_TIMING_Vx_Pos             _UINT32_(0)                       /* (STATUS) Active Voltage Channel Position */
#define STATUS_STATE_FLAG_TIMING_Vx_Msk             (_UINT32_(0x01F) << STATUS_STATE_FLAG_TIMING_Vx_Pos)      /* (STATUS) Active Voltage Channel Mask */
#define STATUS_STATE_FLAG_TIMING_Vx(value)          (STATUS_STATE_FLAG_TIMING_Vx_Msk & (_UINT32_(value) << STATUS_STATE_FLAG_TIMING_Vx_Pos))
#define   STATUS_STATE_FLAG_TIMING_Vx_A_Val           _UINT32_(0x0)                   /* (STATUS) Voltage phase VA is used for timing extraction purposes  */
#define   STATUS_STATE_FLAG_TIMING_Vx_B_Val           _UINT32_(0x1)                   /* (STATUS) Voltage phase VB is used for timing extraction purposes */
#define   STATUS_STATE_FLAG_TIMING_Vx_C_Val           _UINT32_(0x2)                   /* (STATUS) Voltage phase VC is used for timing extraction purposes */
#define STATUS_STATE_FLAG_FREQ_LOCKED_Pos           _UINT32_(5)                       /* (STATUS) Frequency Locked Flag Position */
#define STATUS_STATE_FLAG_FREQ_LOCKED_Msk           (_UINT32_(0x01) << STATUS_STATE_FLAG_FREQ_LOCKED_Pos)      /* (STATUS) Frequency Locked Flag Mask */
#define STATUS_STATE_FLAG_FREQ_LOCKED(value)        (STATUS_STATE_FLAG_FREQ_LOCKED_Msk & (_UINT32_(value) << STATUS_STATE_FLAG_FREQ_LOCKED_Pos))
#define   STATUS_STATE_FLAG_FREQ_LOCKED_DISABLED_Val   _UINT32_(0x0)                   /* (STATUS) Line frequency is not determined  */
#define   STATUS_STATE_FLAG_FREQ_LOCKED_ENABLED_Val    _UINT32_(0x1)                   /* (STATUS) Line frequency has been determined */
#define STATUS_STATE_FLAG_AFE_FAIL_Pos              _UINT32_(7)                       /* (STATUS) ATSENSE state Position */
#define STATUS_STATE_FLAG_AFE_FAIL_Msk              (_UINT32_(0x01) << STATUS_STATE_FLAG_AFE_FAIL_Pos)      /* (STATUS) ATSENSE state Mask */
#define STATUS_STATE_FLAG_AFE_FAIL(value)           (STATUS_STATE_FLAG_AFE_FAIL_Msk & (_UINT32_(value) << STATUS_STATE_FLAG_AFE_FAIL_Pos))
#define   STATUS_STATE_FLAG_AFE_FAIL_DISABLED_Val      _UINT32_(0x0)                   /* (STATUS) ATSENSE initialization is successful  */
#define   STATUS_STATE_FLAG_AFE_FAIL_ENABLED_Val       _UINT32_(0x1)                   /* (STATUS) ATSENSE initialization is failed */
#define STATUS_STATE_FLAG_HARMONIC_M_CFG_Pos            _UINT32_(9)                                /* (FEATURE_CTRL1) Enable Harmonic Analysis Position */
#define STATUS_STATE_FLAG_HARMONIC_M_CFG_Msk            (_UINT32_(0x7F) << STATUS_STATE_FLAG_HARMONIC_M_CFG_Pos)    /* (FEATURE_CTRL1) Enable Harmonic Analysis Mask */
#define STATUS_STATE_FLAG_HARMONIC_M_CFG(value)         (STATUS_STATE_FLAG_HARMONIC_M_CFG_Msk & (_UINT32_(value) << STATUS_STATE_FLAG_HARMONIC_M_CFG_Pos))
#define STATUS_STATE_FLAG_HOST_ID_FAIL_Pos           _UINT32_(15)                       /* (STATUS) Host Identification Failure Indication Position */
#define STATUS_STATE_FLAG_HOST_ID_FAIL_Msk           (_UINT32_(0x01) << STATUS_STATE_FLAG_HOST_ID_FAIL_Pos)      /* (STATUS) Host Identification Failure Indication Mask */
#define STATUS_STATE_FLAG_HOST_ID_FAIL(value)        (STATUS_STATE_FLAG_HOST_ID_FAIL_Msk & (_UINT32_(value) << STATUS_STATE_FLAG_HOST_ID_FAIL_Pos))
#define   STATUS_STATE_FLAG_HOST_ID_FAIL_SUCCESS_Val   _UINT32_(0x0)                   /* (STATUS) Proper PIC32CXMTx part successfully identified  */
#define   STATUS_STATE_FLAG_HOST_ID_FAIL_FAIL_Val      _UINT32_(0x1)                   /* (STATUS) Failed to identify proper PIC32CXMTx part */
#define STATUS_STATE_FLAG_CREEP_DET_Q_Pos            _UINT32_(19)                       /* (STATUS) Total Reactive Power Creep Detected Flag Position */
#define STATUS_STATE_FLAG_CREEP_DET_Q_Msk            (_UINT32_(0x01) << STATUS_STATE_FLAG_CREEP_DET_Q_Pos)      /* (STATUS) Total Reactive Power Creep Detected Flag Mask */
#define STATUS_STATE_FLAG_CREEP_DET_Q(value)         (STATUS_STATE_FLAG_CREEP_DET_Q_Msk & (_UINT32_(value) << STATUS_STATE_FLAG_CREEP_DET_Q_Pos))
#define   STATUS_STATE_FLAG_CREEP_DET_Q_NO_DETECT_Val   _UINT32_(0x0)                   /* (STATUS) Reactive power creep isn?t detected in the current full line cycle  */
#define   STATUS_STATE_FLAG_CREEP_DET_Q_DETECT_Val      _UINT32_(0x1)                   /* (STATUS) Reactive power creep is detected in the current full line cycle */
#define STATUS_STATE_FLAG_CREEP_DET_P_Pos            _UINT32_(20)                       /* (STATUS) Total Active Power Creep Detected Flag Position */
#define STATUS_STATE_FLAG_CREEP_DET_P_Msk            (_UINT32_(0x01) << STATUS_STATE_FLAG_CREEP_DET_P_Pos)      /* (STATUS) Total Active Power Creep Detected Flag Mask */
#define STATUS_STATE_FLAG_CREEP_DET_P(value)         (STATUS_STATE_FLAG_CREEP_DET_P_Msk & (_UINT32_(value) << STATUS_STATE_FLAG_CREEP_DET_P_Pos))
#define   STATUS_STATE_FLAG_CREEP_DET_P_NO_DETECT_Val   _UINT32_(0x0)                   /* (STATUS) Active power creep isn?t detected in the current full line cycle  */
#define   STATUS_STATE_FLAG_CREEP_DET_P_DETECT_Val      _UINT32_(0x1)                   /* (STATUS) Active power creep is detected in the current full line cycle */
#define STATUS_STATE_FLAG_ARCH_DETECT_FAIL_Pos            _UINT32_(21)                       /* (STATUS) Architecture Detect Failure Position */
#define STATUS_STATE_FLAG_ARCH_DETECT_FAIL_Msk            (_UINT32_(0x01) << STATUS_STATE_FLAG_ARCH_DETECT_FAIL_Pos)      /* (STATUS) Architecture Detect Failure Mask */
#define STATUS_STATE_FLAG_ARCH_DETECT_FAIL(value)         (STATUS_STATE_FLAG_ARCH_DETECT_FAIL_Msk & (_UINT32_(value) << STATUS_STATE_FLAG_ARCH_DETECT_FAIL_Pos))
#define   STATUS_STATE_FLAG_ARCH_DETECT_FAIL_NO_FAIL_Val     _UINT32_(0x0)                   /* (STATUS) A proper device (?SH? or ?C?) was detected  */
#define   STATUS_STATE_FLAG_ARCH_DETECT_FAIL_FAIL_Val        _UINT32_(0x1)                   /* (STATUS) An improper device (not ?SH? nor ?C?) was detected */
#define STATUS_STATE_FLAG_PC_OUT_OF_RANGE_Pos            _UINT32_(22)                       /* (STATUS) Phase Corrector Out of Range Position */
#define STATUS_STATE_FLAG_PC_OUT_OF_RANGE_Msk            (_UINT32_(0x01) << STATUS_STATE_FLAG_PC_OUT_OF_RANGE_Pos)      /* (STATUS) Phase Corrector Out of Range Mask */
#define STATUS_STATE_FLAG_PC_OUT_OF_RANGE(value)         (STATUS_STATE_FLAG_PC_OUT_OF_RANGE_Msk & (_UINT32_(value) << STATUS_STATE_FLAG_PC_OUT_OF_RANGE_Pos))
#define   STATUS_STATE_FLAG_PC_OUT_OF_RANGE_IN_Val         _UINT32_(0x0)                   /* (STATUS) Requested phase correction values in range  */
#define   STATUS_STATE_FLAG_PC_OUT_OF_RANGE_OUT_Val        _UINT32_(0x1)                   /* (STATUS) Requested phase correction values out of range */

/* -------- SWELL_OVERCURRENT_FLAG : (MET_STATUS) (R/W 32) Waveform Capture Channel Selection Register -------- */
#define SWELL_OVERCURRENT_FLAG_Pos                       _UINT32_(0)                       /* (SWELL_OVERCURRENT_FLAG) Voltage Swell (for voltage channels) or Overcurrent (for current channels) Detected Flag for Channel x. Position */
#define SWELL_OVERCURRENT_FLAG_Msk                       (_UINT32_(0x1) << SWELL_OVERCURRENT_FLAG_Pos)    /* (SWELL_OVERCURRENT_FLAG) Voltage Swell (for voltage channels) or Overcurrent (for current channels) Detected Flag for Channel x. Mask */
#define SWELL_OVERCURRENT_FLAG_VALUE(channel, value)     ((SWELL_OVERCURRENT_FLAG_Msk & (_UINT32_(value) << SWELL_OVERCURRENT_FLAG_Pos)) << channel)
#define   SWELL_OVERCURRENT_FLAG_DISABLED_Val               _UINT32_(0x0)                             /* (SWELL_OVERCURRENT_FLAG) Voltage swell or overcurrent is not detected for the current half line cycle */
#define   SWELL_OVERCURRENT_FLAG_ENABLED_Val                _UINT32_(0x1)                             /* (SWELL_OVERCURRENT_FLAG) Voltage swell or overcurrent is detected for the current half line cycle */

/* -------- SAG_FLAG : (MET_STATUS) (R/W 32) Waveform Capture Channel Selection Register -------- */
#define SAG_FLAG_Pos                                    _UINT32_(0)                        /* (SAG_FLAG) Voltage Sag Detected Flag for Channel x Position */
#define SAG_FLAG_Msk                                    (_UINT32_(0x1) << SAG_FLAG_Pos)    /* (SAG_FLAG) Voltage Sag Detected Flag for Channel x Mask */
#define SAG_FLAG_VALUE(channel, value)                  ((SAG_FLAG_Msk & (_UINT32_(value) << SAG_FLAG_Pos)) << channel)
#define   SAG_FLAG_DISABLED_Val                            _UINT32_(0x0)                             /* (SAG_FLAG) Voltage sag is not detected for the current half line cycle */
#define   SAG_FLAG_ENABLED_Val                             _UINT32_(0x1)                             /* (SAG_FLAG) Voltage sag is detected for the current half line cycle */

/* -------- CREEP_FLAG : (MET_STATUS) (R/W 32) Waveform Capture Channel Selection Register -------- */
#define CREEP_FLAG_Pos                                    _UINT32_(0)                          /* (CREEP_FLAG) Channel x Current Creep Detected Position */
#define CREEP_FLAG_Msk                                    (_UINT32_(0x1) << CREEP_FLAG_Pos)    /* (CREEP_FLAG) Channel x Current Creep Detected Mask */
#define CREEP_FLAG_VALUE(channel, value)                  ((CREEP_FLAG_Msk & (_UINT32_(value) << CREEP_FLAG_Pos)) << channel)
#define   CREEP_FLAG_DISABLED_Val                            _UINT32_(0x0)                             /* (CREEP_FLAG) Channel x current creep is not detected in the current full line cycle */
#define   CREEP_FLAG_ENABLED_Val                             _UINT32_(0x1)                             /* (CREEP_FLAG) Channel x current creep is detected in the current full line cycle */

/* -------- PH_ACTIVE_FLAG : (MET_STATUS) (R/W 32) Waveform Capture Channel Selection Register -------- */
#define PH_ACTIVE_FLAG_Pos                                _UINT32_(0)                          /* (PH_ACTIVE_FLAG) Phase x is active Position */
#define PH_ACTIVE_FLAG_Msk                                (_UINT32_(0x1) << PH_ACTIVE_FLAG_Pos)    /* (PH_ACTIVE_FLAG) Phase x is active Mask */
#define PH_ACTIVE_FLAG_VALUE(psource, value)              ((PH_ACTIVE_FLAG_Msk & (_UINT32_(value) << PH_ACTIVE_FLAG_Pos)) << chpsourceannel)
#define   PH_ACTIVE_FLAG_DISABLED_Val                        _UINT32_(0x0)                             /* (PH_ACTIVE_FLAG) Phase x is inactive (the channel x, if configured as voltage, is inactive) */
#define   PH_ACTIVE_FLAG_ENABLED_Val                         _UINT32_(0x1)                             /* (PH_ACTIVE_FLAG) Phase x is active (the channel x, if configured as voltage, is active) */

/* -------- CAPTURE_STATUS : (MET_STATUS) (R/W 32) Waveform Capture Function Status -------- */
#define CAPTURE_STATUS_CAPTURE_OFFSET_Pos                  _UINT32_(0)                       /* (CAPTURE_STATUS) Offset within the buffer Position */
#define CAPTURE_STATUS_CAPTURE_OFFSET_Msk                  (_UINT32_(0xFFFFFF) << CAPTURE_STATUS_CAPTURE_OFFSET_Pos)      /* (CAPTURE_STATUS) Offset within the buffer Mask */
#define CAPTURE_STATUS_CAPTURE_OFFSET(value)               (CAPTURE_STATUS_CAPTURE_OFFSET_Msk & (_UINT32_(value) << CAPTURE_STATUS_CAPTURE_OFFSET_Pos))
#define CAPTURE_STATUS_CAPTURE_STATE_Pos                   _UINT32_(24)                       /* (CAPTURE_STATUS) Waveform Capture Status Position */
#define CAPTURE_STATUS_CAPTURE_STATE_Msk                   (_UINT32_(0xF) << CAPTURE_STATUS_CAPTURE_STATE_Pos)      /* (CAPTURE_STATUS) Waveform Capture Status Mask */
#define CAPTURE_STATUS_CAPTURE_STATE(value)                (CAPTURE_STATUS_CAPTURE_STATE_Msk & (_UINT32_(value) << CAPTURE_STATUS_CAPTURE_STATE_Pos))
#define   CAPTURE_STATUS_CAPTURE_STATE_DISABLED_Val          _UINT32_(0x0)                   /* (CAPTURE_STATUS) Disabled  */
#define   CAPTURE_STATUS_CAPTURE_STATE_ACTIVE_Val            _UINT32_(0x1)                   /* (CAPTURE_STATUS) Capture in progress  */
#define   CAPTURE_STATUS_CAPTURE_STATE_COMPLETE_Val          _UINT32_(0x2)                   /* (CAPTURE_STATUS) Capture completed  */
#define CAPTURE_STATUS_INVALID_ADDR_Pos                    _UINT32_(28)                       /* (CAPTURE_STATUS) Invalid Capture Address Position */
#define CAPTURE_STATUS_INVALID_ADDR_Msk                    (_UINT32_(0x1) << CAPTURE_STATUS_INVALID_ADDR_Pos)      /* (CAPTURE_STATUS) Invalid Capture Address Mask */
#define CAPTURE_STATUS_INVALID_ADDR(value)                 (CAPTURE_STATUS_INVALID_ADDR_Msk & (_UINT32_(value) << CAPTURE_STATUS_INVALID_ADDR_Pos))
#define   CAPTURE_STATUS_INVALID_ADDR_WITHIN_Val             _UINT32_(0x0)                   /* (CAPTURE_STATUS) Capture address within valid range   */
#define   CAPTURE_STATUS_INVALID_ADDR_OUTSIDE_Val            _UINT32_(0x1)                   /* (CAPTURE_STATUS) Capture address outside valid range  */
#define CAPTURE_STATUS_CAPTURE_WRAP_Pos                    _UINT32_(31)                       /* (CAPTURE_STATUS) Capture Buffer Wrapped Position */
#define CAPTURE_STATUS_CAPTURE_WRAP_Msk                    (_UINT32_(0x1) << CAPTURE_STATUS_CAPTURE_WRAP_Pos)      /* (STATUS) Capture Buffer Wrapped Mask */
#define CAPTURE_STATUS_CAPTURE_WRAP(value)                 (CAPTURE_STATUS_CAPTURE_WRAP_Msk & (_UINT32_(value) << CAPTURE_STATUS_CAPTURE_WRAP_Pos))
#define   CAPTURE_STATUS_CAPTURE_WRAP_NOT_WRAPPED_Val        _UINT32_(0x0)                   /* (CAPTURE_STATUS) Buffer has not wrapped  */
#define   CAPTURE_STATUS_CAPTURE_WRAP_WRAPPED_Val            _UINT32_(0x1)                   /* (CAPTURE_STATUS) Buffer has wrapped at least once */

/* -------- INTERVAL_NUM : (MET_STATUS) (R/W 32) Measurement Interval Number -------- */
#define INTERVAL_NUM_HALFCYCLE_COUNT_Pos                   _UINT32_(0)                       /* (INTERVAL_NUM) Subinterval number Position */
#define INTERVAL_NUM_HALFCYCLE_COUNT_Msk                   (_UINT32_(0xFFFF) << INTERVAL_NUM_HALFCYCLE_COUNT_Pos)      /* (INTERVAL_NUM) Subinterval number Mask */
#define INTERVAL_NUM_HALFCYCLE_COUNT(value)                (INTERVAL_NUM_HALFCYCLE_COUNT_Msk & (_UINT32_(value) << INTERVAL_NUM_HALFCYCLE_COUNT_Pos))
#define INTERVAL_NUM_INTERVAL_NUM_Pos                      _UINT32_(16)                       /* (INTERVAL_NUM) Interval number Position */
#define INTERVAL_NUM_INTERVAL_NUM_Msk                      (_UINT32_(0xFFFF) << INTERVAL_NUM_INTERVAL_NUM_Pos)      /* (INTERVAL_NUM) Interval number Mask */
#define INTERVAL_NUM_INTERVAL_NUM(value)                   (INTERVAL_NUM_INTERVAL_NUM_Msk & (_UINT32_(value) << INTERVAL_NUM_INTERVAL_NUM_Pos))

/* -------- N : (MET_STATUS) (R/W 32) Number of Samples in the Last Measurement Interval (uQ16.0) -------- */
#define N_Pos                            _UINT32_(0)                       /* (N) Number of Samples in the Last Measurement Interval Position */
#define N_Msk                            (_UINT32_(0xFFFF) << N_Pos)      /* (N) Number of Samples in the Last Measurement Interval Mask */
#define N_Val(value)                     (N_Msk & (_UINT32_(value) << N_Pos))

/* -------- PH_OFFSET : (MET_STATUS) (R/W 32) Phase Correction Filter Offset (sQ0.31) -------- */
#define PH_OFFSET_Pos                    _UINT32_(0)                       /* (PH_OFFSET) Phase Correction Filter Offset Position */
#define PH_OFFSET_Msk                    (_UINT32_(0xFFFFFFFF) << N_Pos)      /* (PH_OFFSET) Phase Correction Filter Offset Mask */
#define PH_OFFSET_Val(value)             (PH_OFFSET_Msk & (_UINT32_(value) << PH_OFFSET_Pos))

/* -------- FREQ : (MET_STATUS) (R/W 32) Active Line Voltage Fundamental Harmonic Frequency (uQ20.12) -------- */
#define FREQ_Pos                         _UINT32_(0)                       /* (FREQ) Active Line Voltage Fundamental Harmonic Frequency Position */
#define FREQ_Msk                         (_UINT32_(0xFFFFFFFF) << N_Pos)      /* (FREQ) Active Line Voltage Fundamental Harmonic Frequency Mask */
#define FREQ_Val(value)                  (FREQ_Msk & (_UINT32_(value) << FREQ_Pos))

/* -------- FREQ_CHx : (MET_STATUS) (R/W 32) Separate frequency is determined for each individual voltage phase (uQ20.12) -------- */
#define FREQ_CHx_Pos                     _UINT32_(0)                       /* (FREQ_CHx) Frequency of Fundamental Harmonic Position */
#define FREQ_CHx_Msk                     (_UINT32_(0xFFFFFFFF) << N_Pos)      /* (FREQ_CHx) Frequency of Fundamental Harmonic Mask */
#define FREQ_CHx_Val(value)              (FREQ_CHx_Msk & (_UINT32_(value) << FREQ_CHx_Pos))

/* -------- CHx_MAX : (MET_STATUS) (R/W 32) Channel Peak Registers (sQ2.29) -------- */
#define CHx_MAX_Pos                      _UINT32_(0)                          /* (CHx_MAX) Max Current or voltage During Measurement Interval Position */
#define CHx_MAX_Msk                      (_UINT32_(0xFFFFFFFF) << N_Pos)      /* (CHx_MAX) Max Current or voltage During Measurement Interval Mask */
#define CHx_MAX_Val(value)               (CHx_MAX_Msk & (_UINT32_(value) << CHx_MAX_Pos))

/* -------- FEATURES : (MET_STATUS) (R/W 32) Compiled feature indicator -------- */
#define FEATURES_DEBUG_MODES_Pos                    _UINT32_(0)
#define FEATURES_DEBUG_MODES_Msk                    (_UINT32_(0x1) << FEATURES_DEBUG_MODES_Pos)
#define FEATURES_DEBUG_MODES(value)                 (FEATURES_DEBUG_MODES_Msk & (_UINT32_(value) << FEATURES_DEBUG_MODES_Pos))
#define FEATURES_ATSENSE_HALF_CLK_Pos               _UINT32_(1)
#define FEATURES_ATSENSE_HALF_CLK_Msk               (_UINT32_(0x1) << FEATURES_ATSENSE_HALF_CLK_Pos)
#define FEATURES_ATSENSE_HALF_CLK(value)            (FEATURES_ATSENSE_HALF_CLK_Msk & (_UINT32_(value) << FEATURES_ATSENSE_HALF_CLK_Pos))
#define FEATURES_ATSENSE_SYNTHESIZE_Pos             _UINT32_(2)
#define FEATURES_ATSENSE_SYNTHESIZE_Msk             (_UINT32_(0x1) << FEATURES_ATSENSE_SYNTHESIZE_Pos)
#define FEATURES_ATSENSE_SYNTHESIZE(value)          (FEATURES_ATSENSE_SYNTHESIZE_Msk & (_UINT32_(value) << FEATURES_ATSENSE_SYNTHESIZE_Pos))
#define FEATURES_DUPLICATE_V1_Pos                   _UINT32_(3)
#define FEATURES_DUPLICATE_V1_Msk                   (_UINT32_(0x1) << FEATURES_DUPLICATE_V1_Pos)
#define FEATURES_DUPLICATE_V1(value)                (FEATURES_DUPLICATE_V1_Msk & (_UINT32_(value) << FEATURES_DUPLICATE_V1_Pos))
#define FEATURES_FIXED_VREF_Pos                     _UINT32_(4)
#define FEATURES_FIXED_VREF_Msk                     (_UINT32_(0x1) << FEATURES_FIXED_VREF_Pos)
#define FEATURES_FIXED_VREF(value)                  (FEATURES_FIXED_VREF_Msk & (_UINT32_(value) << FEATURES_FIXED_VREF_Pos))
#define FEATURES_DEBUG_CONST_PWM_PD18_Pos           _UINT32_(5)
#define FEATURES_DEBUG_CONST_PWM_PD18_Msk           (_UINT32_(0x1) << FEATURES_DEBUG_CONST_PWM_PD18_Pos)
#define FEATURES_DEBUG_CONST_PWM_PD18(value)        (FEATURES_DEBUG_CONST_PWM_PD18_Msk & (_UINT32_(value) << FEATURES_DEBUG_CONST_PWM_PD18_Pos))
#define FEATURES_DEBUG_DAC_PD19_Pos                 _UINT32_(6)
#define FEATURES_DEBUG_DAC_PD19_Msk                 (_UINT32_(0x1) << FEATURES_DEBUG_DAC_PD19_Pos)
#define FEATURES_DEBUG_DAC_PD19(value)              (FEATURES_DEBUG_DAC_PD19_Msk & (_UINT32_(value) << FEATURES_DEBUG_DAC_PD19_Pos))
#define FEATURES_DEBUG_FLEX_PC0_PC22_Pos            _UINT32_(7)
#define FEATURES_DEBUG_FLEX_PC0_PC22_Msk            (_UINT32_(0x1) << FEATURES_DEBUG_FLEX_PC0_PC22_Pos)
#define FEATURES_DEBUG_FLEX_PC0_PC22(value)         (FEATURES_DEBUG_FLEX_PC0_PC22_Msk & (_UINT32_(value) << FEATURES_DEBUG_FLEX_PC0_PC22_Pos))
#define FEATURES_HALF_COPROC_CLK_Pos                _UINT32_(8)
#define FEATURES_HALF_COPROC_CLK_Msk                (_UINT32_(0x1) << FEATURES_HALF_COPROC_CLK_Pos)
#define FEATURES_HALF_COPROC_CLK(value)             (FEATURES_HALF_COPROC_CLK_Msk & (_UINT32_(value) << FEATURES_HALF_COPROC_CLK_Pos))
#define FEATURES_I_N_MUXING_Pos                     _UINT32_(9)
#define FEATURES_I_N_MUXING_Msk                     (_UINT32_(0x1) << FEATURES_I_N_MUXING_Pos)
#define FEATURES_I_N_MUXING(value)                  (FEATURES_I_N_MUXING_Msk & (_UINT32_(value) << FEATURES_I_N_MUXING_Pos))
#define FEATURES_PQ_OFFSET_X_Pos                     _UINT32_(10)
#define FEATURES_PQ_OFFSET_X_Msk                    (_UINT32_(0x1) << FEATURES_PQ_OFFSET_X_Pos)
#define FEATURES_PQ_OFFSET_X(value)                 (FEATURES_PQ_OFFSET_X_Msk & (_UINT32_(value) << FEATURES_PQ_OFFSET_X_Pos))
#define FEATURES_RZC_DETECT_Pos                     _UINT32_(11)
#define FEATURES_RZC_DETECT_Msk                     (_UINT32_(0x1) << FEATURES_RZC_DETECT_Pos)
#define FEATURES_RZC_DETECT(value)                  (FEATURES_RZC_DETECT_Msk & (_UINT32_(value) << FEATURES_RZC_DETECT_Pos))
#define FEATURES_CORE_CLK_SPEED_Pos                 _UINT32_(12)
#define FEATURES_CORE_CLK_SPEED_Msk                 (_UINT32_(0xF) << FEATURES_CORE_CLK_SPEED_Pos)
#define FEATURES_CORE_CLK_SPEED(value)              (FEATURES_CORE_CLK_SPEED_Msk & (_UINT32_(value) << FEATURES_CORE_CLK_SPEED_Pos))
#define   FEATURES_CORE_CLK_SPEED_UNDEFINED_Val       _UINT32_(0x0)                   /* (FEATURES) Compiled to be used with Core-0 clock = undefined  */
#define   FEATURES_CORE_CLK_SPEED_102_Val             _UINT32_(0x1)                   /* (FEATURES) Compiled to be used with Core-0 clock = 102.400MHz */
#define   FEATURES_CORE_CLK_SPEED_106_Val             _UINT32_(0x2)                   /* (FEATURES) Compiled to be used with Core-0 clock = 106.496MHz */
#define   FEATURES_CORE_CLK_SPEED_110_Val             _UINT32_(0x3)                   /* (FEATURES) Compiled to be used with Core-0 clock = 110.592MHz */
#define   FEATURES_CORE_CLK_SPEED_114_Val             _UINT32_(0x4)                   /* (FEATURES) Compiled to be used with Core-0 clock = 114.688MHz */
#define   FEATURES_CORE_CLK_SPEED_118_Val             _UINT32_(0x5)                   /* (FEATURES) Compiled to be used with Core-0 clock = 118.784MHz */
#define   FEATURES_CORE_CLK_SPEED_240_Val             _UINT32_(0x6)                   /* (FEATURES) Compiled to be used with Core-0 clock = 240.000MHz */
#define   FEATURES_CORE_CLK_SPEED_237_Val             _UINT32_(0x7)                   /* (FEATURES) Compiled to be used with Core-0 clock = 237.568MHz */
#define FEATURES_DFT_ENABLED_Pos                    _UINT32_(16)
#define FEATURES_DFT_ENABLED_Msk                    (_UINT32_(0x1) << FEATURES_DFT_ENABLED_Pos)
#define FEATURES_DFT_ENABLED(value)                 (FEATURES_DFT_ENABLED_Msk & (_UINT32_(value) << FEATURES_DFT_ENABLED_Pos))
#define FEATURES_CREEP_Pos                          _UINT32_(17)
#define FEATURES_CREEP_Msk                          (_UINT32_(0x1) << FEATURES_CREEP_Pos)
#define FEATURES_CREEP(value)                       (FEATURES_CREEP_Msk & (_UINT32_(value) << FEATURES_CREEP_Pos))
#define FEATURES_CAPTURE_Pos                        _UINT32_(18)
#define FEATURES_CAPTURE_Msk                        (_UINT32_(0x1) << FEATURES_CAPTURE_Pos)
#define FEATURES_CAPTURE(value)                     (FEATURES_CAPTURE_Msk & (_UINT32_(value) << FEATURES_CAPTURE_Pos))
#define FEATURES_ROGOWSKI_DC_REMOVE_Pos             _UINT32_(19)
#define FEATURES_ROGOWSKI_DC_REMOVE_Msk             (_UINT32_(0x1) << FEATURES_ROGOWSKI_DC_REMOVE_Pos)
#define FEATURES_ROGOWSKI_DC_REMOVE(value)          (FEATURES_ROGOWSKI_DC_REMOVE_Msk & (_UINT32_(value) << FEATURES_ROGOWSKI_DC_REMOVE_Pos))
#define FEATURES_NUM_PULSES_Pos                     _UINT32_(20)
#define FEATURES_NUM_PULSES_Msk                     (_UINT32_(0x3) << FEATURES_NUM_PULSES_Pos)
#define FEATURES_NUM_PULSES(value)                  (FEATURES_NUM_PULSES_Msk & (_UINT32_(value) << FEATURES_NUM_PULSES_Pos))
#define FEATURES_ATSENSE_LOC_Pos                    _UINT32_(22)
#define FEATURES_ATSENSE_LOC_Msk                    (_UINT32_(0x1) << FEATURES_ATSENSE_LOC_Pos)
#define FEATURES_ATSensE_LOC(value)                 (FEATURES_ATSENSE_LOC_Msk & (_UINT32_(value) << FEATURES_ATSENSE_LOC_Pos))
#define FEATURES_MODIFY_CHIPID_Pos                  _UINT32_(23)
#define FEATURES_MODIFY_CHIPID_Msk                  (_UINT32_(0x1) << FEATURES_MODIFY_CHIPID_Pos)
#define FEATURES_MODIFY_CHIPID(value)               (FEATURES_MODIFY_CHIPID_Msk & (_UINT32_(value) << FEATURES_MODIFY_CHIPID_Pos))
#define FEATURES_Fs_SAMPLE_RATE_Pos                 _UINT32_(24)
#define FEATURES_Fs_SAMPLE_RATE_Msk                 (_UINT32_(0x1) << FEATURES_Fs_SAMPLE_RATE_Pos)
#define FEATURES_Fs_SAMPLE_RATE(value)              (FEATURES_Fs_SAMPLE_RATE_Msk & (_UINT32_(value) << FEATURES_Fs_SAMPLE_RATE_Pos))
#define   FEATURES_Fs_SAMPLE_RATE_4MHz_Val            _UINT32_(0x0)                   /* (FEATURES) Baseband sample rate FS = 4.000KHz  */
#define   FEATURES_Fs_SAMPLE_RATE_8MHz_Val            _UINT32_(0x1)                   /* (FEATURES) Baseband sample rate FS = 8.000KHz  */
#define FEATURES_I_Ni_SOURCE_Pos                    _UINT32_(25)
#define FEATURES_I_Ni_SOURCE_Msk                    (_UINT32_(0x1) << FEATURES_I_Ni_SOURCE_Pos)
#define FEATURES_I_Ni_SOURCE(value)                 (FEATURES_I_Ni_SOURCE_Msk & (_UINT32_(value) << FEATURES_I_Ni_SOURCE_Pos))
#define FEATURES_RCZ_OUT_Pos                        _UINT32_(26)
#define FEATURES_RCZ_OUT_Msk                        (_UINT32_(0x1) << FEATURES_RCZ_OUT_Pos)
#define FEATURES_RCZ_OUT(value)                     (FEATURES_RCZ_OUT_Msk & (_UINT32_(value) << FEATURES_RCZ_OUT_Pos))
#define FEATURES_DEBUG_ENABLE_VREF_TEST_DATA_Pos    _UINT32_(27)
#define FEATURES_DEBUG_ENABLE_VREF_TEST_DATA_Msk    (_UINT32_(0x1) << FEATURES_DEBUG_ENABLE_VREF_TEST_DATA_Pos)
#define FEATURES_DEBUG_ENABLE_VREF_TEST_DATA(value) (FEATURES_DEBUG_ENABLE_VREF_TEST_DATA_Msk & (_UINT32_(value) << FEATURES_DEBUG_ENABLE_VREF_TEST_DATA_Pos))
#define FEATURES_METER_TYPE_10_Pos                  _UINT32_(28)
#define FEATURES_METER_TYPE_10_Msk                  (_UINT32_(0x1) << FEATURES_METER_TYPE_10_Pos)
#define FEATURES_METER_TYPE_10(value)               (FEATURES_METER_TYPE_10_Msk & (_UINT32_(value) << FEATURES_METER_TYPE_10_Pos))
#define FEATURES_METER_TYPE_09_Pos                  _UINT32_(29)
#define FEATURES_METER_TYPE_09_Msk                  (_UINT32_(0x1) << FEATURES_METER_TYPE_09_Pos)
#define FEATURES_METER_TYPE_09(value)               (FEATURES_METER_TYPE_09_Msk & (_UINT32_(value) << FEATURES_METER_TYPE_09_Pos))
#define FEATURES_SINGLEPHASE_Pos                    _UINT32_(30)
#define FEATURES_SINGLEPHASE_Msk                    (_UINT32_(0x1) << FEATURES_SINGLEPHASE_Pos)
#define FEATURES_SINGLEPHASE(value)                 (FEATURES_SINGLEPHASE_Msk & (_UINT32_(value) << FEATURES_SINGLEPHASE_Pos))
#define FEATURES_POLYPHASE_Pos                      _UINT32_(31)
#define FEATURES_POLYPHASE_Msk                      (_UINT32_(0x1) << FEATURES_POLYPHASE_Pos)
#define FEATURES_POLYPHASE(value)                   (FEATURES_POLYPHASE_Msk & (_UINT32_(value) << FEATURES_POLYPHASE_Pos))

/* -------- ZC_N_CHx : (MET_STATUS) (R/W 32) Zero-Crossing Sample Number of Channel x Voltage (uQ20.12) -------- */
#define ZC_N_CHx_Pos                    _UINT32_(0)                                 /* (ZC_N_CHx) Zero Crossing Count Position */     
#define ZC_N_CHx_Msk                    (_UINT32_(0xFFFFFF) << ZC_N_CHx_Pos)        /* (ZC_N_CHx) Zero Crossing Count Mask */     
#define ZC_N_CHx_Val(value)             (ZC_N_CHx_Msk & (_UINT32_(value) << ZC_N_CHx_Pos))

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 5.4"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>
</#if>
/* MISRA C-2012 deviation block end */

/** \brief Metrology Harmonic Analysis Output register API structure */
typedef struct
{
    __O  uint32_t CHx_m_R[32];             /**< Offset: 0x00 (R/ 32) The real part of DFT result channel x, m-th harmonics (sQ25.6) */
    __O  uint32_t CHx_m_I[32];             /**< Offset: 0x00 (R/ 32) The imaginary part of DFT result channel x, m-th harmonics (sQ25.6) */
} DRV_MCMETROLOGY_REGS_HARMONICS;

/** \brief Metrology Accumulated Output register API structure */
typedef struct
{
    __O  uint64_t CHx[32];               /**< Offset: 0x00  (R/ 64) Amp/Volt-squared-samples, channel x, fundamental + harmonics (uQ24.40) */
    __O  uint64_t CHx_F[32];             /**< Offset: 0x100 (R/ 64) Amp/Volt-squared-samples, channel x, fundamental only (uQ24.40) */
    __O  int64_t  P_CHx[31];             /**< Offset: 0x200 (R/ 64) Watt-samples, power x, fundamental + harmonics (sQ23.40) */
    __O  uint64_t Reserved1;
    __O  int64_t  P_CHx_F[31];           /**< Offset: 0x300 (R/ 64) Watt-samples, power x, fundamental only (sQ23.40) */
    __O  uint64_t Reserved2;
    __O  int64_t  Q_CHx[31];             /**< Offset: 0x400 (R/ 64) VAR-samples, power x, fundamental + harmonics (sQ23.40) */
    __O  uint64_t Reserved3;
    __O  int64_t  Q_CHx_F[31];           /**< Offset: 0x500 (R/ 64) VAR-samples, power x, fundamental only (sQ23.40) */
    __O  uint64_t Reserved4;
    __O  uint64_t V_AB;                  /**< Offset: 0x600 (R/ 32) Volt-square-samples, VA - VB, fundamental + harmonics (uQ24.40) */
    __O  uint64_t V_BC;                  /**< Offset: 0x608 (R/ 32) Volt-square-samples, VB - VC, fundamental + harmonics (uQ24.40) */
    __O  uint64_t V_CA;                  /**< Offset: 0x610 (R/ 32) Volt-square-samples, VC - VA, fundamental + harmonics (uQ24.40) */
    __O  uint64_t V_AB_F;                /**< Offset: 0x618 (R/ 32) Volt-square-samples, VA - VB, fundamental only (uQ24.40) */
    __O  uint64_t V_BC_F;                /**< Offset: 0x620 (R/ 32) Volt-square-samples, VB - VC, fundamental only (uQ24.40) */
    __O  uint64_t V_CA_F;                /**< Offset: 0x628 (R/ 32) Volt-square-samples, VC - VA, fundamental only (uQ24.40) */
    __O  int64_t  ACC_T0;                /**< Offset: 0x630 (R/ 32) Total accumulated quantities specified by PC0_TYPE (sQ33.30) */
    __O  int64_t  ACC_T1;                /**< Offset: 0x638 (R/ 32) Total accumulated quantities specified by PC1_TYPE (sQ33.30) */
    __O  int64_t  ACC_T2;                /**< Offset: 0x640 (R/ 32) Total accumulated quantities specified by PC2_TYPE (sQ33.30) */
    __O  uint64_t Reserved5[15];
} DRV_MCMETROLOGY_REGS_ACCUMULATORS;

/** \brief Metrology Status register API structure */
typedef struct
{
    __O  uint32_t VERSION;                 /**< Offset: 0x00 (R/ 32) Metrology firmware version */
    __O  uint32_t STATUS;                  /**< Offset: 0x04 (R/ 32) Metrology status register */
    __O  uint32_t STATE_FLAG;              /**< Offset: 0x08 (R/ 32) Metrology state flags register */
    __O  uint32_t SWELL_OVERCURRENT_FLAG;  /**< Offset: 0x0C (R/ 32) Metrology state flags register */
    __O  uint32_t SAG_FLAG;                /**< Offset: 0x10 (R/ 32) Sag (voltage channels) flag */             
    __O  uint32_t CREEP_FLAG;              /**< Offset: 0x14 (R/ 32) Creep (current channels) flag */
    __O  uint32_t PH_ACTIVE_FLAG;          /**< Offset: 0x18 (R/ 32) Phase x Active flag */
    __O  uint32_t CAPTURE_STATUS;          /**< Offset: 0x1C (R/ 32) Waveform Capture Function Status */
    __O  uint32_t INTERVAL_NUM;            /**< Offset: 0x20 (R/ 32) Interval number */
    __O  uint32_t N;                       /**< Offset: 0x24 (R/ 32) Number of Samples in the Last Measurement Interval (uQ16.0) */
    __O  uint32_t PH_OFFSET;               /**< Offset: 0x28 (R/ 32) Phase calibration offset (sQ0.31) */
    __O  uint32_t FREQ;                    /**< Offset: 0x2C (R/ 32) Frequency of fundamental harmonic (uQ20.12) */
    __O  uint32_t FREQ_CHx[32];            /**< Offset: 0x30 (R/ 32) Frequency of fundamental harmonic, channel x (uQ20.12) */
    __O  uint32_t Reserved1[2];
    __O  uint32_t CHx_MAX[32];             /**< Offset: 0xB8 (R/ 32) Max current/voltage of channel x during measurement interval (sQ2.29) */
    __O  uint32_t FEATURES;                /**< Offset: 0x138 (R/ 32) Compiled feature indicator */
    __O  uint32_t MPC_CRC_ERRORS;          /**< Offset: 0x13C (R/ 32) CRC errors in the MCP391x MCSPI communication */
    __O  uint32_t Reserved2[2];
    __O  uint32_t PULSE0_COUNTER;          /**< Offset: 0x148 (R/ 32) Counter of Pulse 0 */
    __O  uint32_t PULSE1_COUNTER;          /**< Offset: 0x14C (R/ 32) Counter of Pulse 1 */
    __O  uint32_t PULSE2_COUNTER;          /**< Offset: 0x150 (R/ 32) Counter of Pulse 2 */
    __O  uint32_t Reserved3[2];
    __O  uint32_t ZC_N_CHx[32];            /**< Offset: 0x15C (R/ 32) Zero-crossing sample number of channel x. Just for voltage channels (uQ20.12) */
    __O  uint32_t Reserved4[3];
} DRV_MCMETROLOGY_REGS_STATUS;

/** \brief Metrology Control register API structure */
typedef struct
{
   __IO  uint32_t STATE_CTRL;                           /**< Offset: 0x00 (R/W 32) Metrology state control register */
   __IO  uint32_t FEATURE_CTRL0;                        /**< Offset: 0x04 (R/W 32) Metrology feature setting register */
   __IO  uint32_t FEATURE_CTRL1;                        /**< Offset: 0x08 (R/W 32) Metrology feature setting register */
   __IO  uint32_t Reserved1;             
   __IO  uint32_t POWER_SOURCE[16];                     /**< Offset: 0x10 (R/W 32) Metrology feature setting register */
   __IO  uint32_t METER_TYPE;                           /**< Offset: 0x50 (R/W 32) Sensor type and service type settings register */
   __IO  uint32_t SENSOR_TYPE[2];                       /**< Offset: 0x54 (R/W 32) Sensor type selection */
   __IO  uint32_t M;                                    /**< Offset: 0x5C (R/W 32) Number of line cycles for integration period (uQ12.0) */
   __IO  uint32_t N_MAX;                                /**< Offset: 0x60 (R/W 32) Maximum number of samples in an integration period (uQ24.0) */
   __IO  uint32_t PULSE0_CTRL;                          /**< Offset: 0x64 (R/W 32) Pulse 0 control: enable/disable, polarity, quantity type selection */
   __IO  uint32_t PULSE1_CTRL;                          /**< Offset: 0x68 (R/W 32) Pulse 1 control: enable/disable, polarity, quantity type selection */
   __IO  uint32_t PULSE2_CTRL;                          /**< Offset: 0x6C (R/W 32) Pulse 2 control: enable/disable, polarity, quantity type selection */
   __IO  uint32_t Reserved2;             
   __IO  uint32_t PULSE_CONTRIBUTION_PW;                /**< Offset: 0x74 (R/W 32) Selection of power sources contributing to pulse, for pulses configured to pulse P or Q */
   __IO  uint32_t PULSE_CONTRIBUTION_I;                 /**< Offset: 0x78 (R/W 32) Selection of power sources contributing to pulse, for pulses configured to pulse I */
   __IO  uint32_t Reserved3;             
   __IO  uint32_t P_K_t;                                /**< Offset: 0x80 (R/W 32) Meter Active Power Pulse Constant (uQ8.24) */
   __IO  uint32_t Q_K_t;                                /**< Offset: 0x84 (R/W 32) Meter Reactive Power Pulse Constant (uQ8.24) */
   __IO  uint32_t I_K_t;                                /**< Offset: 0x88 (R/W 32) Meter Current Pulse Constant (uQ8.24) */
   __IO  uint32_t CREEP_THRESHOLD_P;                    /**< Offset: 0x8C (R/W 32) Creep Threshold for Active Energy (uQ2.30) */
   __IO  uint32_t CREEP_THRESHOLD_Q;                    /**< Offset: 0x90 (R/W 32) Creep Threshold for Quadergy (uQ2.30) */
   __IO  uint32_t CREEP_THRESHOLD_I;                    /**< Offset: 0x94 (R/W 32) Creep Threshold of Phase Current (uQ12.20) */
   __IO  uint32_t P_POWER_OFFSET_CTRL;                  /**< Offset: 0x98 (R/W 32) Active Power offset control */
   __IO  uint32_t Q_POWER_OFFSET_CTRL;                  /**< Offset: 0x9C (R/W 32) Reactive Power offset control */
   __IO  int32_t POWER_OFFSET_P;                       /**< Offset: 0xA0 (R/W 32) Active Power Offset (sQ1.30) */
   __IO  int32_t POWER_OFFSET_Q;                       /**< Offset: 0xA4 (R/W 32) Reactive Power Offset (sQ1.30) */
   __IO  uint32_t SWELL_OVERCURRENT_THRESHOLD_CHx[32];  /**< Offset: 0xA8 (R/W 32) Voltage swells or overcurrent threshold for each half cycle of CHx channel (uQ0.32) */
   __IO  uint32_t SAG_THRESHOLD_CHx[32];                /**< Offset: 0x128 (R/W 32) Voltage sag threshold for each half cycle of CHx voltage (uQ0.32) */
   __IO  uint32_t K_CHx[32];                            /**< Offset: 0x1A8 (R/W 32) ADC input channel conversion factor (I_A(rms)/Vadc(rms) or V_A(rms)/Vadc(rms)) (uQ22.10) */
   __IO  uint32_t CAL_M_CHx[32];                        /**< Offset: 0x228 (R/W 32) Magnitude calibration constant for channel CHx (sQ2.29) */
   __IO  int32_t CAL_PH_CHx[32];                       /**< Offset: 0x2A8 (R/W 32) Phase calibration constant for channel CHx (sQ0.31) */
   __IO  uint32_t CAPTURE_CTRL;                         /**< Offset: 0x328 (R/W 32) Waveform capture control register */
   __IO  uint32_t CAPTURE_SH_SEL;                       /**< Offset: 0x32C (R/W 32) Waveform capture channel selection */
   __IO  uint32_t CAPTURE_BUFF_SIZE;                    /**< Offset: 0x330 (R/W 32) Waveform capture buffer size */
   __IO  uint32_t CAPTURE_ADDR;                         /**< Offset: 0x334 (R/W 32) Waveform capture buffer address pointer */
   __IO  uint32_t Reserved4[3];
   __IO  uint32_t MCP391x_GAIN[4];                      /**< Offset: 0x344 (R/W 32) MCP391x GAIN register */
   __IO  uint32_t MCP391x_CONFIG[4];                    /**< Offset: 0x354 (R/W 32) MCP391x CONFIG register */
   __IO  uint32_t Reserved5;             
   __IO  int32_t POWER_OFFSET_P_x[31];                 /**< Offset: 0x368 (R/W 32) Phase-x accumulation active power offset compensation used to eliminate voltage effects at small current (cross talk) (sQ9.40) */
   __IO  uint32_t Reserved6;             
   __IO  int32_t POWER_OFFSET_Q_x[31];                 /**< Offset: 0x3E8 (R/W 32) Phase-x accumulation reactive power offset compensation used to eliminate voltage effects at small current (cross talk) (sQ9.40) */
   __IO  uint32_t Reserved7;             
} DRV_MCMETROLOGY_REGS_CONTROL;

/** \brief METROLOGY register API structure */
typedef struct
{
    DRV_MCMETROLOGY_REGS_CONTROL       MET_CONTROL;         /**< Offset: 0x0000 (R/W 32) Metrology Control Registers */
    DRV_MCMETROLOGY_REGS_STATUS        MET_STATUS;          /**< Offset: 0x00F4 (R/ 32) Metrology Status Registers */
    DRV_MCMETROLOGY_REGS_ACCUMULATORS  MET_ACCUMULATORS;    /**< Offset: 0x0180 (R/ 32) Metrology Accumulated Output Registers */
    DRV_MCMETROLOGY_REGS_HARMONICS     MET_HARMONICS;       /**< Offset: 0x0338 (R/ 32) Metrology Harmonic Analysis Output Registers */
} MCMET_REGISTERS;

#endif /* MCMMETROLOGY_REGS_H */
