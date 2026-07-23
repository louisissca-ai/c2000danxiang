/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: xtq2_dq_doubleloop_fullspec_private.h
 *
 * Code generated for Simulink model 'xtq2_dq_doubleloop_fullspec'.
 *
 * Model version                  : 1.31
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 * C/C++ source code generated on : Thu Jul 23 16:51:21 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Texas Instruments->C2000
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef xtq2_dq_doubleloop_fullspec_private_h_
#define xtq2_dq_doubleloop_fullspec_private_h_
#include "rtwtypes.h"
#include "xtq2_dq_doubleloop_fullspec_types.h"

void InitAdcA (void);
void config_ADCA_SOC0 (void);
void InitAdcC (void);
void config_ADCC_SOC0 (void);
extern uint16_T MW_adcAInitFlag;
extern uint16_T MW_adcCInitFlag;

#endif                              /* xtq2_dq_doubleloop_fullspec_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
