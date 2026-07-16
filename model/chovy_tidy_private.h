/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: chovy_tidy_private.h
 *
 * Code generated for Simulink model 'chovy_tidy'.
 *
 * Model version                  : 1.38
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 * C/C++ source code generated on : Thu Jul 16 21:51:46 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Texas Instruments->C2000
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef chovy_tidy_private_h_
#define chovy_tidy_private_h_
#include "rtwtypes.h"
#include "chovy_tidy.h"
#include "chovy_tidy_types.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"

/* Private macros used by the generated code to access rtModel */
#ifndef rtmIsMajorTimeStep
#define rtmIsMajorTimeStep(rtm)        (((rtm)->Timing.simTimeStep) == MAJOR_TIME_STEP)
#endif

#ifndef rtmIsMinorTimeStep
#define rtmIsMinorTimeStep(rtm)        (((rtm)->Timing.simTimeStep) == MINOR_TIME_STEP)
#endif

#ifndef rtmSetTPtr
#define rtmSetTPtr(rtm, val)           ((rtm)->Timing.t = (val))
#endif

void InitAdcA (void);
void config_ADCA_SOC3 (void);
void InitAdcC (void);
void config_ADCC_SOC1 (void);
void config_ADCA_SOC0 (void);
void config_ADCC_SOC2 (void);
void config_ADCA_SOC2 (void);
void config_ADCC_SOC0 (void);
void config_ADCA_SOC1 (void);
extern uint16_T MW_adcAInitFlag;
extern uint16_T MW_adcCInitFlag;
extern real_T look2_binlcpw(real_T u0, real_T u1, const real_T bp0[], const
  real_T bp1[], const real_T table[], const uint32_T maxIndex[], uint32_T stride);
extern void chov_Subsystempi2delay_Init(real_T *rty_dq, real_T *rty_dq_k,
  P_Subsystempi2delay_chovy_tid_T *localP);
extern void c_Subsystempi2delay_Disable(DW_Subsystempi2delay_chovy_ti_T *localDW);
extern void chovy_tid_Subsystempi2delay(uint16_T rtu_Enable, const real_T
  rtu_alpha_beta[2], real_T rtu_wt, real_T *rty_dq, real_T *rty_dq_k,
  DW_Subsystempi2delay_chovy_ti_T *localDW);
extern void chovy_tidy_Subsystem1_Init(real_T *rty_dq, real_T *rty_dq_b,
  P_Subsystem1_chovy_tidy_T *localP);
extern void chovy_ti_Subsystem1_Disable(DW_Subsystem1_chovy_tidy_T *localDW);
extern void chovy_tidy_Subsystem1(uint16_T rtu_Enable, const real_T
  rtu_alpha_beta[2], real_T rtu_wt, real_T *rty_dq, real_T *rty_dq_b,
  DW_Subsystem1_chovy_tidy_T *localDW);

#endif                                 /* chovy_tidy_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
