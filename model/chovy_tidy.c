/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: chovy_tidy.c
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

#include "chovy_tidy.h"
#include "rtwtypes.h"
#include "chovy_tidy_private.h"
#include <math.h>
#include "rt_nonfinite.h"
#include <string.h>

/* Block signals (default storage) */
B_chovy_tidy_T chovy_tidy_B;

/* Block states (default storage) */
DW_chovy_tidy_T chovy_tidy_DW;

/* Real-time model */
static RT_MODEL_chovy_tidy_T chovy_tidy_M_;
RT_MODEL_chovy_tidy_T *const chovy_tidy_M = &chovy_tidy_M_;
static void rate_monotonic_scheduler(void);

#ifndef __TMS320C28XX_CLA__

uint16_T MW_adcAInitFlag = 0;

#endif

#ifndef __TMS320C28XX_CLA__

uint16_T MW_adcCInitFlag = 0;

#endif

real_T look2_binlcpw(real_T u0, real_T u1, const real_T bp0[], const real_T bp1[],
                     const real_T table[], const uint32_T maxIndex[], uint32_T
                     stride)
{
  real_T fractions[2];
  real_T frac;
  real_T yL_0d0;
  real_T yL_0d1;
  uint32_T bpIndices[2];
  uint32_T bpIdx;
  uint32_T iLeft;
  uint32_T iRght;

  /* Column-major Lookup 2-D
     Search method: 'binary'
     Use previous index: 'off'
     Interpolation method: 'Linear point-slope'
     Extrapolation method: 'Clip'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0UL]) {
    iLeft = 0UL;
    frac = 0.0;
  } else if (u0 < bp0[maxIndex[0UL]]) {
    /* Binary Search */
    bpIdx = maxIndex[0UL] >> 1UL;
    iLeft = 0UL;
    iRght = maxIndex[0UL];
    while (iRght - iLeft > 1UL) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1UL;
    }

    frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1UL] - bp0[iLeft]);
  } else {
    iLeft = maxIndex[0UL] - 1UL;
    frac = 1.0;
  }

  fractions[0UL] = frac;
  bpIndices[0UL] = iLeft;

  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u1 <= bp1[0UL]) {
    iLeft = 0UL;
    frac = 0.0;
  } else if (u1 < bp1[maxIndex[1UL]]) {
    /* Binary Search */
    bpIdx = maxIndex[1UL] >> 1UL;
    iLeft = 0UL;
    iRght = maxIndex[1UL];
    while (iRght - iLeft > 1UL) {
      if (u1 < bp1[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1UL;
    }

    frac = (u1 - bp1[iLeft]) / (bp1[iLeft + 1UL] - bp1[iLeft]);
  } else {
    iLeft = maxIndex[1UL] - 1UL;
    frac = 1.0;
  }

  /* Column-major Interpolation 2-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'off'
     Overflow mode: 'portable wrapping'
   */
  bpIdx = iLeft * stride + bpIndices[0UL];
  yL_0d0 = table[bpIdx];
  yL_0d0 += (table[bpIdx + 1UL] - yL_0d0) * fractions[0UL];
  bpIdx += stride;
  yL_0d1 = table[bpIdx];
  return (((table[bpIdx + 1UL] - yL_0d1) * fractions[0UL] + yL_0d1) - yL_0d0) *
    frac + yL_0d0;
}

/*
 * Set which subrates need to run this base step (base rate always runs).
 * This function must be called prior to calling the model step function
 * in order to remember which rates need to run this base step.  The
 * buffering of events allows for overlapping preemption.
 */
void chovy_tidy_SetEventsForThisBaseStep(boolean_T *eventFlags)
{
  /* Task runs when its counter is zero, computed via rtmStepTask macro */
  eventFlags[2] = ((boolean_T)rtmStepTask(chovy_tidy_M, 2));
}

/*
 *         This function updates active task flag for each subrate
 *         and rate transition flags for tasks that exchange data.
 *         The function assumes rate-monotonic multitasking scheduler.
 *         The function must be called at model base rate so that
 *         the generated code self-manages all its subrates and rate
 *         transition flags.
 */
static void rate_monotonic_scheduler(void)
{
  /* To ensure a deterministic data transfer between two rates,
   * data is transferred at the priority of a fast task and the frequency
   * of the slow task.  The following flags indicate when the data transfer
   * happens.  That is, a rate interaction flag is set true when both rates
   * will run, and false otherwise.
   */

  /* tid 1 shares data with slower tid rate: 2 */
  if (chovy_tidy_M->Timing.TaskCounters.TID[1] == 0) {
    chovy_tidy_M->Timing.RateInteraction.TID1_2 =
      (chovy_tidy_M->Timing.TaskCounters.TID[2] == 0);
  }

  /* Compute which subrates run during the next base time step.  Subrates
   * are an integer multiple of the base rate counter.  Therefore, the subtask
   * counter is reset when it reaches its limit (zero means run).
   */
  (chovy_tidy_M->Timing.TaskCounters.TID[2])++;
  if ((chovy_tidy_M->Timing.TaskCounters.TID[2]) > 19) {/* Sample time: [0.001s, 0.0s] */
    chovy_tidy_M->Timing.TaskCounters.TID[2] = 0;
  }
}

/*
 * System initialize for enable system:
 *    '<S210>/Subsystem - pi//2 delay'
 *    '<S216>/Subsystem - pi//2 delay'
 */
void chov_Subsystempi2delay_Init(real_T *rty_dq, real_T *rty_dq_k,
  P_Subsystempi2delay_chovy_tid_T *localP)
{
  /* SystemInitialize for Outport: '<S214>/dq' */
  *rty_dq = localP->dq_Y0[0];
  *rty_dq_k = localP->dq_Y0[1];
}

/*
 * Disable for enable system:
 *    '<S210>/Subsystem - pi//2 delay'
 *    '<S216>/Subsystem - pi//2 delay'
 */
void c_Subsystempi2delay_Disable(DW_Subsystempi2delay_chovy_ti_T *localDW)
{
  localDW->Subsystempi2delay_MODE = false;
}

/*
 * Output and update for enable system:
 *    '<S210>/Subsystem - pi//2 delay'
 *    '<S216>/Subsystem - pi//2 delay'
 */
void chovy_tid_Subsystempi2delay(uint16_T rtu_Enable, const real_T
  rtu_alpha_beta[2], real_T rtu_wt, real_T *rty_dq, real_T *rty_dq_k,
  DW_Subsystempi2delay_chovy_ti_T *localDW)
{
  real_T tmp;
  real_T tmp_0;

  /* Outputs for Enabled SubSystem: '<S210>/Subsystem - pi//2 delay' incorporates:
   *  EnablePort: '<S214>/Enable'
   */
  if (rtu_Enable > 0U) {
    if (!localDW->Subsystempi2delay_MODE) {
      localDW->Subsystempi2delay_MODE = true;
    }
  } else if (localDW->Subsystempi2delay_MODE) {
    c_Subsystempi2delay_Disable(localDW);
  }

  if (localDW->Subsystempi2delay_MODE) {
    /* Fcn: '<S214>/Fcn' incorporates:
     *  Fcn: '<S214>/Fcn1'
     */
    tmp = cos(rtu_wt);
    tmp_0 = sin(rtu_wt);
    *rty_dq = rtu_alpha_beta[0] * tmp_0 - rtu_alpha_beta[1] * tmp;

    /* Fcn: '<S214>/Fcn1' */
    *rty_dq_k = rtu_alpha_beta[0] * tmp + rtu_alpha_beta[1] * tmp_0;
  }

  /* End of Outputs for SubSystem: '<S210>/Subsystem - pi//2 delay' */
}

/*
 * System initialize for enable system:
 *    '<S210>/Subsystem1'
 *    '<S216>/Subsystem1'
 */
void chovy_tidy_Subsystem1_Init(real_T *rty_dq, real_T *rty_dq_b,
  P_Subsystem1_chovy_tidy_T *localP)
{
  /* SystemInitialize for Outport: '<S215>/dq' */
  *rty_dq = localP->dq_Y0[0];
  *rty_dq_b = localP->dq_Y0[1];
}

/*
 * Disable for enable system:
 *    '<S210>/Subsystem1'
 *    '<S216>/Subsystem1'
 */
void chovy_ti_Subsystem1_Disable(DW_Subsystem1_chovy_tidy_T *localDW)
{
  localDW->Subsystem1_MODE = false;
}

/*
 * Output and update for enable system:
 *    '<S210>/Subsystem1'
 *    '<S216>/Subsystem1'
 */
void chovy_tidy_Subsystem1(uint16_T rtu_Enable, const real_T rtu_alpha_beta[2],
  real_T rtu_wt, real_T *rty_dq, real_T *rty_dq_b, DW_Subsystem1_chovy_tidy_T
  *localDW)
{
  real_T tmp;
  real_T tmp_0;

  /* Outputs for Enabled SubSystem: '<S210>/Subsystem1' incorporates:
   *  EnablePort: '<S215>/Enable'
   */
  if (rtu_Enable > 0U) {
    if (!localDW->Subsystem1_MODE) {
      localDW->Subsystem1_MODE = true;
    }
  } else if (localDW->Subsystem1_MODE) {
    chovy_ti_Subsystem1_Disable(localDW);
  }

  if (localDW->Subsystem1_MODE) {
    /* Fcn: '<S215>/Fcn' incorporates:
     *  Fcn: '<S215>/Fcn1'
     */
    tmp = sin(rtu_wt);
    tmp_0 = cos(rtu_wt);
    *rty_dq = rtu_alpha_beta[0] * tmp_0 + rtu_alpha_beta[1] * tmp;

    /* Fcn: '<S215>/Fcn1' */
    *rty_dq_b = -rtu_alpha_beta[0] * tmp + rtu_alpha_beta[1] * tmp_0;
  }

  /* End of Outputs for SubSystem: '<S210>/Subsystem1' */
}

/* Model step function for TID0 */
void chovy_tidy_step0(void)            /* Sample time: [0.0s, 0.0s] */
{
  /* local block i/o variables */
  real_T rtb_IntegralGain;
  real_T rtb_IntegralGain_k;
  boolean_T rtb_Resetcontrollerswhilestoppe;
  real_T Gain1[3];
  real_T rtb_duty[3];
  real_T Directvoltagecommandd;
  real_T Directvoltagecommandq;
  real_T Fcn_tmp;
  real_T SafeVdcdenominator;
  real_T numAccum;
  real_T rtb_Filteredvoltagemagnitude;
  real_T rtb_Filteredvoltageq;
  real_T rtb_Gain;
  real_T rtb_Gain17;
  real_T rtb_Sum1;
  real_T rtb_Sum_d;
  int16_T Compare;
  int16_T Compare_j;

  {                                    /* Sample time: [0.0s, 0.0s] */
    rate_monotonic_scheduler();
  }

  /* RelationalOperator: '<S222>/Compare' incorporates:
   *  Constant: '<S222>/Constant'
   *  Constant: '<S9>/Constant'
   */
  Compare = (chovy_tidy_P.dq0toAlphaBetaZero_Alignment ==
             chovy_tidy_P.CompareToConstant_const);

  /* CFunction: '<Root>/ControlModel Vref Ramp' */
  chovy_tidy_B.ControlModelVrefRamp = ControlModel_GetVrefRamp();

  /* DiscreteTransferFcn: '<Root>/Filtered load current magnitude' */
  numAccum = chovy_tidy_P.Filteredloadcurrentmagnitude_Nu *
    chovy_tidy_DW.Filteredloadcurrentmagnitude_st;

  /* S-Function (c2802xadc): '<Root>/ADC8' */
  {
    /*  Internal Reference Voltage : Fixed scale 0 to 3.3 V range.  */
    /*  External Reference Voltage : Allowable ranges of VREFHI(ADCINA0) = 3.3 and VREFLO(tied to ground) = 0  */
    chovy_tidy_B.ADC8 = (AdcaResultRegs.ADCRESULT3);
  }

  /* Gain: '<Root>/Gain17' incorporates:
   *  Constant: '<Root>/Constant10'
   *  DataTypeConversion: '<Root>/Cast To Double7'
   *  Gain: '<Root>/Gain18'
   *  Sum: '<Root>/Sum12'
   */
  rtb_Gain17 = (chovy_tidy_P.Gain18_Gain * (real_T)chovy_tidy_B.ADC8 +
                chovy_tidy_P.Constant10_Value) * chovy_tidy_P.Gain17_Gain;

  /* Sum: '<Root>/Corrected d voltage reference' incorporates:
   *  DiscreteTransferFcn: '<Root>/Filtered load current magnitude'
   *  Gain: '<Root>/Gain17'
   *  Lookup_n-D: '<Root>/Load voltage compensation'
   */
  rtb_Filteredvoltageq = chovy_tidy_B.ControlModelVrefRamp + look2_binlcpw
    (numAccum, rtb_Gain17, chovy_tidy_P.Loadvoltagecompensation_bp01Dat,
     chovy_tidy_P.Loadvoltagecompensation_bp02Dat,
     chovy_tidy_P.Loadvoltagecompensation_tableDa,
     chovy_tidy_P.Loadvoltagecompensation_maxInde, 5UL);

  /* DiscreteTransferFcn: '<Root>/Filtered voltage d' */
  rtb_Filteredvoltagemagnitude = chovy_tidy_P.Filteredvoltaged_NumCoef *
    chovy_tidy_DW.Filteredvoltaged_states;

  /* Sum: '<Root>/Sum' incorporates:
   *  DiscreteTransferFcn: '<Root>/Filtered voltage d'
   */
  rtb_Sum_d = rtb_Filteredvoltageq - rtb_Filteredvoltagemagnitude;

  /* CFunction: '<Root>/ControlModel Safety 50us' incorporates:
   *  DiscreteTransferFcn: '<Root>/Filtered load current magnitude'
   */
  chovy_tidy_B.ControlModelSafety50us = ControlModel_UpdateSafety(rtb_Gain17,
    numAccum);

  /* Logic: '<Root>/Reset controllers while stopped' */
  rtb_Resetcontrollerswhilestoppe = (chovy_tidy_B.ControlModelSafety50us == 0U);

  /* DiscreteIntegrator: '<S43>/Integrator' */
  if (rtb_Resetcontrollerswhilestoppe ||
      (chovy_tidy_DW.Integrator_PrevResetState != 0)) {
    chovy_tidy_DW.Integrator_DSTATE =
      chovy_tidy_P.DiscretePIDController_InitialCo;
  }

  /* Sum: '<Root>/Direct voltage command d' incorporates:
   *  DiscreteIntegrator: '<S43>/Integrator'
   *  Gain: '<S48>/Proportional Gain'
   *  Sum: '<S52>/Sum'
   */
  Directvoltagecommandd = (chovy_tidy_P.DiscretePIDController_P * rtb_Sum_d +
    chovy_tidy_DW.Integrator_DSTATE) + rtb_Filteredvoltageq;

  /* DiscreteTransferFcn: '<Root>/Filtered voltage q' */
  rtb_Filteredvoltageq = chovy_tidy_P.Filteredvoltageq_NumCoef *
    chovy_tidy_DW.Filteredvoltageq_states;

  /* Sum: '<Root>/Sum1' incorporates:
   *  Constant: '<Root>/Constant1'
   *  DiscreteTransferFcn: '<Root>/Filtered voltage q'
   */
  rtb_Sum1 = chovy_tidy_P.Constant1_Value - rtb_Filteredvoltageq;

  /* DiscreteIntegrator: '<S93>/Integrator' */
  if (rtb_Resetcontrollerswhilestoppe ||
      (chovy_tidy_DW.Integrator_PrevResetState_o != 0)) {
    chovy_tidy_DW.Integrator_DSTATE_b =
      chovy_tidy_P.DiscretePIDController1_InitialC;
  }

  /* Sum: '<Root>/Direct voltage command q' incorporates:
   *  Constant: '<Root>/Constant1'
   *  DiscreteIntegrator: '<S93>/Integrator'
   *  Gain: '<S98>/Proportional Gain'
   *  Sum: '<S102>/Sum'
   */
  Directvoltagecommandq = (chovy_tidy_P.DiscretePIDController1_P * rtb_Sum1 +
    chovy_tidy_DW.Integrator_DSTATE_b) + chovy_tidy_P.Constant1_Value;

  /* Gain: '<Root>/Gain' incorporates:
   *  Clock: '<Root>/Clock'
   */
  rtb_Gain = chovy_tidy_P.Gain_Gain * chovy_tidy_M->Timing.t[0];

  /* Outputs for Enabled SubSystem: '<S9>/Subsystem1' incorporates:
   *  EnablePort: '<S225>/Enable'
   */
  /* RelationalOperator: '<S222>/Compare' */
  if ((uint16_T)Compare > 0U) {
    if (!chovy_tidy_DW.Subsystem1_MODE) {
      chovy_tidy_DW.Subsystem1_MODE = true;
    }

    /* Fcn: '<S225>/Fcn' incorporates:
     *  Fcn: '<S225>/Fcn1'
     */
    SafeVdcdenominator = sin(rtb_Gain);
    Fcn_tmp = cos(rtb_Gain);

    /* Fcn: '<S225>/Fcn' */
    chovy_tidy_B.Fcn = Directvoltagecommandd * Fcn_tmp - Directvoltagecommandq *
      SafeVdcdenominator;

    /* Fcn: '<S225>/Fcn1' */
    chovy_tidy_B.Fcn1 = Directvoltagecommandd * SafeVdcdenominator +
      Directvoltagecommandq * Fcn_tmp;
  } else if (chovy_tidy_DW.Subsystem1_MODE) {
    chovy_tidy_DW.Subsystem1_MODE = false;
  }

  /* End of Outputs for SubSystem: '<S9>/Subsystem1' */

  /* Outputs for Enabled SubSystem: '<S9>/Subsystem - pi//2 delay' incorporates:
   *  EnablePort: '<S224>/Enable'
   */
  /* RelationalOperator: '<S223>/Compare' incorporates:
   *  Constant: '<S223>/Constant'
   *  Constant: '<S9>/Constant'
   */
  if (chovy_tidy_P.dq0toAlphaBetaZero_Alignment ==
      chovy_tidy_P.CompareToConstant1_const) {
    if (!chovy_tidy_DW.Subsystempi2delay_MODE) {
      chovy_tidy_DW.Subsystempi2delay_MODE = true;
    }

    /* Fcn: '<S224>/Fcn' incorporates:
     *  Fcn: '<S224>/Fcn1'
     */
    SafeVdcdenominator = cos(rtb_Gain);
    Fcn_tmp = sin(rtb_Gain);

    /* Fcn: '<S224>/Fcn' */
    chovy_tidy_B.Fcn_n = Directvoltagecommandd * Fcn_tmp + Directvoltagecommandq
      * SafeVdcdenominator;

    /* Fcn: '<S224>/Fcn1' */
    chovy_tidy_B.Fcn1_b = -Directvoltagecommandd * SafeVdcdenominator +
      Directvoltagecommandq * Fcn_tmp;
  } else if (chovy_tidy_DW.Subsystempi2delay_MODE) {
    chovy_tidy_DW.Subsystempi2delay_MODE = false;
  }

  /* End of RelationalOperator: '<S223>/Compare' */
  /* End of Outputs for SubSystem: '<S9>/Subsystem - pi//2 delay' */

  /* Switch: '<S9>/Switch' incorporates:
   *  RelationalOperator: '<S222>/Compare'
   */
  if ((uint16_T)Compare != 0U) {
    Directvoltagecommandd = chovy_tidy_B.Fcn;
    Directvoltagecommandq = chovy_tidy_B.Fcn1;
  } else {
    Directvoltagecommandd = chovy_tidy_B.Fcn_n;
    Directvoltagecommandq = chovy_tidy_B.Fcn1_b;
  }

  /* End of Switch: '<S9>/Switch' */

  /* Saturate: '<Root>/Safe Vdc denominator' */
  if (rtb_Gain17 > chovy_tidy_P.SafeVdcdenominator_UpperSat) {
    /* Saturate: '<Root>/Safe Vdc denominator' */
    SafeVdcdenominator = chovy_tidy_P.SafeVdcdenominator_UpperSat;
  } else if (rtb_Gain17 < chovy_tidy_P.SafeVdcdenominator_LowerSat) {
    /* Saturate: '<Root>/Safe Vdc denominator' */
    SafeVdcdenominator = chovy_tidy_P.SafeVdcdenominator_LowerSat;
  } else {
    /* Saturate: '<Root>/Safe Vdc denominator' */
    SafeVdcdenominator = rtb_Gain17;
  }

  /* End of Saturate: '<Root>/Safe Vdc denominator' */

  /* Product: '<Root>/Normalize alpha by measured Vdc' incorporates:
   *  Gain: '<Root>/Gain3'
   */
  Directvoltagecommandd = chovy_tidy_P.Gain3_Gain * Directvoltagecommandd /
    SafeVdcdenominator;

  /* Product: '<Root>/Normalize beta by measured Vdc' incorporates:
   *  Gain: '<Root>/Gain4'
   */
  Directvoltagecommandq = chovy_tidy_P.Gain4_Gain * Directvoltagecommandq /
    SafeVdcdenominator;

  /* MATLAB Function: '<Root>/MATLAB Function' incorporates:
   *  Constant: '<Root>/Constant4'
   */
  SafeVdcdenominator = sqrt(Directvoltagecommandd * Directvoltagecommandd +
    Directvoltagecommandq * Directvoltagecommandq);
  if (SafeVdcdenominator > chovy_tidy_P.Constant4_Value) {
    SafeVdcdenominator = chovy_tidy_P.Constant4_Value / SafeVdcdenominator;
  } else {
    SafeVdcdenominator = 1.0;
  }

  Directvoltagecommandd *= SafeVdcdenominator;
  Directvoltagecommandq *= SafeVdcdenominator;

  /* End of MATLAB Function: '<Root>/MATLAB Function' */

  /* MATLAB Function: '<Root>/Unipolar SPWM Duty (0-100%)' */
  rtb_duty[0] = (Directvoltagecommandd + 1.0) * 50.0;
  rtb_duty[1] = ((-0.5 * Directvoltagecommandd + 0.8660254037844386 *
                  Directvoltagecommandq) + 1.0) * 50.0;
  rtb_duty[2] = ((-0.5 * Directvoltagecommandd - 0.8660254037844386 *
                  Directvoltagecommandq) + 1.0) * 50.0;
  if ((rtb_duty[0] <= 0.0) || rtIsNaN(rtb_duty[0])) {
    Directvoltagecommandd = 0.0;
  } else {
    Directvoltagecommandd = rtb_duty[0];
  }

  if (Directvoltagecommandd >= 100.0) {
    rtb_duty[0] = 100.0;
  } else {
    rtb_duty[0] = Directvoltagecommandd;
  }

  if ((rtb_duty[1] <= 0.0) || rtIsNaN(rtb_duty[1])) {
    Directvoltagecommandd = 0.0;
  } else {
    Directvoltagecommandd = rtb_duty[1];
  }

  if (Directvoltagecommandd >= 100.0) {
    rtb_duty[1] = 100.0;
  } else {
    rtb_duty[1] = Directvoltagecommandd;
  }

  if ((rtb_duty[2] <= 0.0) || rtIsNaN(rtb_duty[2])) {
    Directvoltagecommandd = 0.0;
  } else {
    Directvoltagecommandd = rtb_duty[2];
  }

  if (Directvoltagecommandd >= 100.0) {
    rtb_duty[2] = 100.0;
  } else {
    rtb_duty[2] = Directvoltagecommandd;
  }

  /* End of MATLAB Function: '<Root>/Unipolar SPWM Duty (0-100%)' */

  /* S-Function (c2802xpwm): '<Root>/ePWM1 Leg A 20kHz' */

  /*-- Update CMPA value for ePWM1 --*/
  {
    EPwm1Regs.CMPA.bit.CMPA = (uint16_T)((uint32_T)EPwm1Regs.TBPRD * rtb_duty[0]
      * 0.01);
  }

  /* S-Function (c2802xpwm): '<Root>/ePWM1 Leg A 20kHz1' */

  /*-- Update CMPA value for ePWM2 --*/
  {
    EPwm2Regs.CMPA.bit.CMPA = (uint16_T)((uint32_T)EPwm2Regs.TBPRD * rtb_duty[1]
      * 0.01);
  }

  /* S-Function (c2802xpwm): '<Root>/ePWM1 Leg A 20kHz2' */

  /*-- Update CMPA value for ePWM3 --*/
  {
    EPwm3Regs.CMPA.bit.CMPA = (uint16_T)((uint32_T)EPwm3Regs.TBPRD * rtb_duty[2]
      * 0.01);
  }

  /* S-Function (c2802xadc): '<Root>/ADC1' */
  {
    /*  Internal Reference Voltage : Fixed scale 0 to 3.3 V range.  */
    /*  External Reference Voltage : Allowable ranges of VREFHI(ADCINA0) = 3.3 and VREFLO(tied to ground) = 0  */
    chovy_tidy_B.ADC1 = (AdccResultRegs.ADCRESULT1);
  }

  /* S-Function (c2802xadc): '<Root>/ADC2' */
  {
    /*  Internal Reference Voltage : Fixed scale 0 to 3.3 V range.  */
    /*  External Reference Voltage : Allowable ranges of VREFHI(ADCINA0) = 3.3 and VREFLO(tied to ground) = 0  */
    chovy_tidy_B.ADC2 = (AdcaResultRegs.ADCRESULT0);
  }

  /* S-Function (c2802xadc): '<Root>/ADC3' */
  {
    /*  Internal Reference Voltage : Fixed scale 0 to 3.3 V range.  */
    /*  External Reference Voltage : Allowable ranges of VREFHI(ADCINA0) = 3.3 and VREFLO(tied to ground) = 0  */
    chovy_tidy_B.ADC3 = (AdccResultRegs.ADCRESULT2);
  }

  /* S-Function (c2802xadc): '<Root>/ADC4' */
  {
    /*  Internal Reference Voltage : Fixed scale 0 to 3.3 V range.  */
    /*  External Reference Voltage : Allowable ranges of VREFHI(ADCINA0) = 3.3 and VREFLO(tied to ground) = 0  */
    chovy_tidy_B.ADC4 = (AdcaResultRegs.ADCRESULT2);
  }

  /* S-Function (c2802xadc): '<Root>/ADC5' */
  {
    /*  Internal Reference Voltage : Fixed scale 0 to 3.3 V range.  */
    /*  External Reference Voltage : Allowable ranges of VREFHI(ADCINA0) = 3.3 and VREFLO(tied to ground) = 0  */
    chovy_tidy_B.ADC5 = (AdccResultRegs.ADCRESULT0);
  }

  /* SignalConversion generated from: '<S217>/Gain3' incorporates:
   *  Constant: '<Root>/Constant3'
   *  Constant: '<Root>/Constant7'
   *  Constant: '<Root>/Constant9'
   *  DataTypeConversion: '<Root>/Cast To Double3'
   *  DataTypeConversion: '<Root>/Cast To Double4'
   *  DataTypeConversion: '<Root>/Cast To Double5'
   *  Gain: '<Root>/Gain13'
   *  Gain: '<Root>/Gain14'
   *  Gain: '<Root>/Gain15'
   *  Gain: '<Root>/Gain16'
   *  Gain: '<Root>/Gain5'
   *  Gain: '<Root>/Gain8'
   *  Sum: '<Root>/Sum11'
   *  Sum: '<Root>/Sum6'
   *  Sum: '<Root>/Sum9'
   */
  Directvoltagecommandd = (chovy_tidy_P.Gain13_Gain * (real_T)chovy_tidy_B.ADC5
    + chovy_tidy_P.Constant9_Value) * chovy_tidy_P.Gain14_Gain;
  Directvoltagecommandq = (chovy_tidy_P.Gain8_Gain * (real_T)chovy_tidy_B.ADC1 +
    chovy_tidy_P.Constant3_Value) * chovy_tidy_P.Gain5_Gain;
  SafeVdcdenominator = (chovy_tidy_P.Gain16_Gain * (real_T)chovy_tidy_B.ADC3 +
                        chovy_tidy_P.Constant7_Value) * chovy_tidy_P.Gain15_Gain;
  for (Compare = 0; Compare < 3; Compare++) {
    /* Gain: '<S217>/Gain1' incorporates:
     *  Gain: '<S217>/Gain3'
     */
    Gain1[Compare] = ((chovy_tidy_P.Gain3_Gain_f[Compare + 3] *
                       Directvoltagecommandq + chovy_tidy_P.Gain3_Gain_f[Compare]
                       * Directvoltagecommandd) +
                      chovy_tidy_P.Gain3_Gain_f[Compare + 6] *
                      SafeVdcdenominator) * chovy_tidy_P.Gain1_Gain;
  }

  /* RelationalOperator: '<S218>/Compare' incorporates:
   *  Constant: '<S216>/Constant'
   *  Constant: '<S218>/Constant'
   */
  Compare_j = (chovy_tidy_P.AlphaBetaZerotodq0_Alignment ==
               chovy_tidy_P.CompareToConstant_const_o);

  /* Outputs for Enabled SubSystem: '<S216>/Subsystem1' */
  /* Gain: '<S217>/Gain1' */
  chovy_tidy_Subsystem1((uint16_T)Compare_j, &Gain1[0], rtb_Gain,
                        &chovy_tidy_B.Fcn_m, &chovy_tidy_B.Fcn1_p,
                        &chovy_tidy_DW.Subsystem1_a);

  /* End of Outputs for SubSystem: '<S216>/Subsystem1' */

  /* Outputs for Enabled SubSystem: '<S216>/Subsystem - pi//2 delay' */
  /* RelationalOperator: '<S219>/Compare' incorporates:
   *  Constant: '<S216>/Constant'
   *  Constant: '<S219>/Constant'
   *  Gain: '<S217>/Gain1'
   */
  chovy_tid_Subsystempi2delay((uint16_T)
    (chovy_tidy_P.AlphaBetaZerotodq0_Alignment ==
     chovy_tidy_P.CompareToConstant1_const_c), &Gain1[0], rtb_Gain,
    &chovy_tidy_B.Fcn_nz, &chovy_tidy_B.Fcn1_b2,
    &chovy_tidy_DW.Subsystempi2delay_n);

  /* End of Outputs for SubSystem: '<S216>/Subsystem - pi//2 delay' */

  /* S-Function (c2802xadc): '<Root>/ADC6' */
  {
    /*  Internal Reference Voltage : Fixed scale 0 to 3.3 V range.  */
    /*  External Reference Voltage : Allowable ranges of VREFHI(ADCINA0) = 3.3 and VREFLO(tied to ground) = 0  */
    chovy_tidy_B.ADC6 = (AdcaResultRegs.ADCRESULT1);
  }

  /* SignalConversion generated from: '<S211>/Gain3' incorporates:
   *  Constant: '<Root>/Constant5'
   *  Constant: '<Root>/Constant6'
   *  Constant: '<Root>/Constant8'
   *  DataTypeConversion: '<Root>/Cast To Double'
   *  DataTypeConversion: '<Root>/Cast To Double1'
   *  DataTypeConversion: '<Root>/Cast To Double2'
   *  Gain: '<Root>/Gain10'
   *  Gain: '<Root>/Gain11'
   *  Gain: '<Root>/Gain12'
   *  Gain: '<Root>/Gain6'
   *  Gain: '<Root>/Gain7'
   *  Gain: '<Root>/Gain9'
   *  Sum: '<Root>/Sum10'
   *  Sum: '<Root>/Sum7'
   *  Sum: '<Root>/Sum8'
   */
  Directvoltagecommandd = (chovy_tidy_P.Gain11_Gain * (real_T)chovy_tidy_B.ADC2
    + chovy_tidy_P.Constant5_Value) * chovy_tidy_P.Gain6_Gain;
  Directvoltagecommandq = (chovy_tidy_P.Gain10_Gain * (real_T)chovy_tidy_B.ADC6
    + chovy_tidy_P.Constant8_Value) * chovy_tidy_P.Gain9_Gain;
  SafeVdcdenominator = (chovy_tidy_P.Gain12_Gain * (real_T)chovy_tidy_B.ADC4 +
                        chovy_tidy_P.Constant6_Value) * chovy_tidy_P.Gain7_Gain;
  for (Compare = 0; Compare < 3; Compare++) {
    /* Gain: '<S211>/Gain1' incorporates:
     *  Gain: '<S211>/Gain3'
     */
    Gain1[Compare] = ((chovy_tidy_P.Gain3_Gain_c[Compare + 3] *
                       Directvoltagecommandq + chovy_tidy_P.Gain3_Gain_c[Compare]
                       * Directvoltagecommandd) +
                      chovy_tidy_P.Gain3_Gain_c[Compare + 6] *
                      SafeVdcdenominator) * chovy_tidy_P.Gain1_Gain_o;
  }

  /* Outputs for Enabled SubSystem: '<S210>/Subsystem - pi//2 delay' */
  /* RelationalOperator: '<S213>/Compare' incorporates:
   *  Constant: '<S210>/Constant'
   *  Constant: '<S213>/Constant'
   *  Gain: '<S211>/Gain1'
   */
  chovy_tid_Subsystempi2delay((uint16_T)
    (chovy_tidy_P.AlphaBetaZerotodq0_Alignment_a ==
     chovy_tidy_P.CompareToConstant1_const_f), &Gain1[0], rtb_Gain,
    &chovy_tidy_B.Fcn_l, &chovy_tidy_B.Fcn1_c, &chovy_tidy_DW.Subsystempi2delay);

  /* End of Outputs for SubSystem: '<S210>/Subsystem - pi//2 delay' */

  /* RelationalOperator: '<S212>/Compare' incorporates:
   *  Constant: '<S210>/Constant'
   *  Constant: '<S212>/Constant'
   */
  Compare = (chovy_tidy_P.AlphaBetaZerotodq0_Alignment_a ==
             chovy_tidy_P.CompareToConstant_const_oy);

  /* Outputs for Enabled SubSystem: '<S210>/Subsystem1' */
  /* Gain: '<S211>/Gain1' */
  chovy_tidy_Subsystem1((uint16_T)Compare, &Gain1[0], rtb_Gain,
                        &chovy_tidy_B.Fcn_b, &chovy_tidy_B.Fcn1_k,
                        &chovy_tidy_DW.Subsystem1);

  /* End of Outputs for SubSystem: '<S210>/Subsystem1' */

  /* Switch: '<S210>/Switch' incorporates:
   *  RelationalOperator: '<S212>/Compare'
   */
  if ((uint16_T)Compare != 0U) {
    /* Switch: '<S210>/Switch' */
    chovy_tidy_B.Switch[0] = chovy_tidy_B.Fcn_b;
    chovy_tidy_B.Switch[1] = chovy_tidy_B.Fcn1_k;
  } else {
    /* Switch: '<S210>/Switch' */
    chovy_tidy_B.Switch[0] = chovy_tidy_B.Fcn_l;
    chovy_tidy_B.Switch[1] = chovy_tidy_B.Fcn1_c;
  }

  /* End of Switch: '<S210>/Switch' */

  /* Switch: '<S216>/Switch' incorporates:
   *  RelationalOperator: '<S218>/Compare'
   */
  if ((uint16_T)Compare_j != 0U) {
    Directvoltagecommandd = chovy_tidy_B.Fcn_m;
    Directvoltagecommandq = chovy_tidy_B.Fcn1_p;
  } else {
    Directvoltagecommandd = chovy_tidy_B.Fcn_nz;
    Directvoltagecommandq = chovy_tidy_B.Fcn1_b2;
  }

  /* End of Switch: '<S216>/Switch' */

  /* Sum: '<Root>/Load current square sum' incorporates:
   *  Product: '<Root>/Load current d square'
   *  Product: '<Root>/Load current q square'
   *  Sum: '<Root>/Load current d'
   *  Sum: '<Root>/Load current q'
   */
  rtb_Gain = Directvoltagecommandd * Directvoltagecommandd +
    Directvoltagecommandq * Directvoltagecommandq;

  /* Math: '<Root>/Load current magnitude'
   *
   * About '<Root>/Load current magnitude':
   *  Operator: sqrt
   */
  if (rtb_Gain < 0.0) {
    /* Math: '<Root>/Load current magnitude'
     *
     * About '<Root>/Load current magnitude':
     *  Operator: sqrt
     */
    chovy_tidy_B.Loadcurrentmagnitude = -sqrt(fabs(rtb_Gain));
  } else {
    /* Math: '<Root>/Load current magnitude'
     *
     * About '<Root>/Load current magnitude':
     *  Operator: sqrt
     */
    chovy_tidy_B.Loadcurrentmagnitude = sqrt(rtb_Gain);
  }

  /* End of Math: '<Root>/Load current magnitude' */

  /* MinMax: '<Root>/Maximum phase duty' */
  if (rtb_duty[0] >= rtb_duty[1]) {
    rtb_Gain = rtb_duty[0];
  } else {
    rtb_Gain = rtb_duty[1];
  }

  if (!(rtb_Gain >= rtb_duty[2])) {
    rtb_Gain = rtb_duty[2];
  }

  /* RateTransition: '<Root>/Feedback duty 1ms' incorporates:
   *  MinMax: '<Root>/Maximum phase duty'
   */
  if (chovy_tidy_M->Timing.RateInteraction.TID1_2) {
    chovy_tidy_DW.Feedbackduty1ms_Buffer = rtb_Gain;
  }

  /* End of RateTransition: '<Root>/Feedback duty 1ms' */

  /* Gain: '<S90>/Integral Gain' */
  rtb_IntegralGain = chovy_tidy_P.DiscretePIDController1_I * rtb_Sum1;

  /* Sum: '<Root>/Filtered voltage square sum' incorporates:
   *  DiscreteTransferFcn: '<Root>/Filtered voltage d'
   *  DiscreteTransferFcn: '<Root>/Filtered voltage q'
   *  Product: '<Root>/Filtered voltage d square'
   *  Product: '<Root>/Filtered voltage q square'
   */
  rtb_Filteredvoltagemagnitude = rtb_Filteredvoltagemagnitude *
    rtb_Filteredvoltagemagnitude + rtb_Filteredvoltageq * rtb_Filteredvoltageq;

  /* Math: '<Root>/Filtered voltage magnitude'
   *
   * About '<Root>/Filtered voltage magnitude':
   *  Operator: sqrt
   */
  if (rtb_Filteredvoltagemagnitude < 0.0) {
    rtb_Filteredvoltagemagnitude = -sqrt(fabs(rtb_Filteredvoltagemagnitude));
  } else {
    rtb_Filteredvoltagemagnitude = sqrt(rtb_Filteredvoltagemagnitude);
  }

  /* End of Math: '<Root>/Filtered voltage magnitude' */

  /* RateTransition: '<Root>/Feedback vout 1ms' */
  if (chovy_tidy_M->Timing.RateInteraction.TID1_2) {
    chovy_tidy_DW.Feedbackvout1ms_Buffer = rtb_Filteredvoltagemagnitude;
  }

  /* End of RateTransition: '<Root>/Feedback vout 1ms' */

  /* Gain: '<S40>/Integral Gain' */
  rtb_IntegralGain_k = chovy_tidy_P.DiscretePIDController_I * rtb_Sum_d;

  /* RateTransition: '<Root>/Feedback vin 1ms' */
  if (chovy_tidy_M->Timing.RateInteraction.TID1_2) {
    chovy_tidy_DW.Feedbackvin1ms_Buffer = rtb_Gain17;

    /* RateTransition: '<Root>/Feedback iout 1ms' incorporates:
     *  DiscreteTransferFcn: '<Root>/Filtered load current magnitude'
     */
    chovy_tidy_DW.Feedbackiout1ms_Buffer = numAccum;
  }

  /* End of RateTransition: '<Root>/Feedback vin 1ms' */

  /* Update for DiscreteTransferFcn: '<Root>/Filtered load current magnitude' */
  chovy_tidy_DW.Filteredloadcurrentmagnitude_st =
    (chovy_tidy_B.Loadcurrentmagnitude -
     chovy_tidy_P.Filteredloadcurrentmagnitude_De[1L] *
     chovy_tidy_DW.Filteredloadcurrentmagnitude_st) /
    chovy_tidy_P.Filteredloadcurrentmagnitude_De[0];

  /* Update for DiscreteTransferFcn: '<Root>/Filtered voltage d' */
  chovy_tidy_DW.Filteredvoltaged_states = (chovy_tidy_B.Switch[0] -
    chovy_tidy_P.Filteredvoltaged_DenCoef[1L] *
    chovy_tidy_DW.Filteredvoltaged_states) /
    chovy_tidy_P.Filteredvoltaged_DenCoef[0];

  /* Update for DiscreteIntegrator: '<S43>/Integrator' */
  chovy_tidy_DW.Integrator_DSTATE += chovy_tidy_P.Integrator_gainval *
    rtb_IntegralGain_k;
  chovy_tidy_DW.Integrator_PrevResetState = (int16_T)
    rtb_Resetcontrollerswhilestoppe;

  /* Update for DiscreteTransferFcn: '<Root>/Filtered voltage q' */
  chovy_tidy_DW.Filteredvoltageq_states = (chovy_tidy_B.Switch[1] -
    chovy_tidy_P.Filteredvoltageq_DenCoef[1L] *
    chovy_tidy_DW.Filteredvoltageq_states) /
    chovy_tidy_P.Filteredvoltageq_DenCoef[0];

  /* Update for DiscreteIntegrator: '<S93>/Integrator' incorporates:
   *  DiscreteIntegrator: '<S43>/Integrator'
   */
  chovy_tidy_DW.Integrator_DSTATE_b += chovy_tidy_P.Integrator_gainval_m *
    rtb_IntegralGain;
  chovy_tidy_DW.Integrator_PrevResetState_o = (int16_T)
    rtb_Resetcontrollerswhilestoppe;

  /* Update absolute time */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  chovy_tidy_M->Timing.t[0] =
    ((time_T)(++chovy_tidy_M->Timing.clockTick0)) *
    chovy_tidy_M->Timing.stepSize0;

  /* Update absolute time */
  /* The "clockTick1" counts the number of times the code of this task has
   * been executed. The resolution of this integer timer is 5.0E-5, which is the step size
   * of the task. Size of "clockTick1" ensures timer will not overflow during the
   * application lifespan selected.
   */
  chovy_tidy_M->Timing.clockTick1++;
}

/* Model step function for TID2 */
void chovy_tidy_step2(void)            /* Sample time: [0.001s, 0.0s] */
{
  /* CFunction: '<Root>/ControlModel Feedback 1ms' incorporates:
   *  RateTransition: '<Root>/Feedback duty 1ms'
   *  RateTransition: '<Root>/Feedback iout 1ms'
   *  RateTransition: '<Root>/Feedback vin 1ms'
   *  RateTransition: '<Root>/Feedback vout 1ms'
   */
  ControlModel_SetFeedback(chovy_tidy_DW.Feedbackvin1ms_Buffer,
    chovy_tidy_DW.Feedbackvout1ms_Buffer, chovy_tidy_DW.Feedbackiout1ms_Buffer,
    chovy_tidy_DW.Feedbackduty1ms_Buffer);

  /* CFunction: '<Root>/ControlModel HMI Task 1ms' */
  ControlModel_Task1ms();
}

/* Model initialize function */
void chovy_tidy_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* non-finite (run-time) assignments */
  chovy_tidy_P.SafeVdcdenominator_UpperSat = rtInf;

  /* initialize real-time model */
  (void) memset((void *)chovy_tidy_M, 0,
                sizeof(RT_MODEL_chovy_tidy_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&chovy_tidy_M->solverInfo,
                          &chovy_tidy_M->Timing.simTimeStep);
    rtsiSetTPtr(&chovy_tidy_M->solverInfo, &rtmGetTPtr(chovy_tidy_M));
    rtsiSetStepSizePtr(&chovy_tidy_M->solverInfo,
                       &chovy_tidy_M->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&chovy_tidy_M->solverInfo, (&rtmGetErrorStatus
      (chovy_tidy_M)));
    rtsiSetRTModelPtr(&chovy_tidy_M->solverInfo, chovy_tidy_M);
  }

  rtsiSetSimTimeStep(&chovy_tidy_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetIsMinorTimeStepWithModeChange(&chovy_tidy_M->solverInfo, false);
  rtsiSetIsContModeFrozen(&chovy_tidy_M->solverInfo, false);
  rtsiSetSolverName(&chovy_tidy_M->solverInfo,"FixedStepDiscrete");
  rtmSetTPtr(chovy_tidy_M, &chovy_tidy_M->Timing.tArray[0]);
  chovy_tidy_M->Timing.stepSize0 = 5.0E-5;

  /* block I/O */
  (void) memset(((void *) &chovy_tidy_B), 0,
                sizeof(B_chovy_tidy_T));

  /* states (dwork) */
  (void) memset((void *)&chovy_tidy_DW, 0,
                sizeof(DW_chovy_tidy_T));

  /* Start for CFunction: '<Root>/ControlModel Vref Ramp' */
  chovy_tidy_B.ControlModelVrefRamp = 0.0;

  /* Start for S-Function (c2802xadc): '<Root>/ADC8' */
  if (MW_adcAInitFlag == 0U) {
    InitAdcA();
    MW_adcAInitFlag = 1U;
  }

  config_ADCA_SOC3 ();

  /* Start for CFunction: '<Root>/ControlModel Safety 50us' */
  chovy_tidy_B.ControlModelSafety50us = 0U;

  /* Start for S-Function (c2802xpwm): '<Root>/ePWM1 Leg A 20kHz' */

  /*** Initialize ePWM1 modules ***/
  {
    /*  // Time Base Control Register
       EPwm1Regs.TBCTL.bit.CTRMODE              = 2U;          // Counter Mode

       EPwm1Regs.TBCTL.bit.PRDLD                = 0U;          // Shadow select

       EPwm1Regs.TBCTL2.bit.PRDLDSYNC           = 0U;          // Shadow select

       EPwm1Regs.TBCTL.bit.PHSEN                = 0U;          // Phase Load Enable
       EPwm1Regs.TBCTL.bit.PHSDIR               = 0U;          // Phase Direction Bit
       EPwm1Regs.TBCTL.bit.HSPCLKDIV            = 0U;          // High Speed TBCLK Pre-scaler
       EPwm1Regs.TBCTL.bit.CLKDIV               = 0U;          // Time Base Clock Pre-scaler
     */
    EPwm1Regs.TBCTL.all = (EPwm1Regs.TBCTL.all & ~0x3FCFU) | 0x2U;
    EPwm1Regs.TBCTL2.all = (EPwm1Regs.TBCTL2.all & ~0xC000U) | 0x0U;

    /* // Time-Base EPWMxSYNCOUT Source Enable Register
       EPwm1Regs.EPWMSYNCOUTEN.bit.ZEROEN      = 1U; // Sync Output Select - CTR = 0 event
     */
    EPwm1Regs.EPWMSYNCOUTEN.bit.SWEN = 0U;// disable the SWEN enabled during reset
    EPwm1Regs.EPWMSYNCOUTEN.all = (EPwm1Regs.EPWMSYNCOUTEN.all & ~0x2U) | 0x2U;

    /*-- Setup Time-Base (TB) Submodule --*/
    EPwm1Regs.TBPRD = 3000U;           // Time Base Period Register

    /* // Time-Base Phase Register
       EPwm1Regs.TBPHS.bit.TBPHS               = 0U;          // Phase offset register
     */
    EPwm1Regs.TBPHS.all = (EPwm1Regs.TBPHS.all & ~0xFFFF0000U) | 0x0U;

    // Time Base Counter Register
    EPwm1Regs.TBCTR = 0x0000U;         /* Clear counter*/

    /*-- Setup Counter_Compare (CC) Submodule --*/
    /*	// Counter Compare Control Register

       EPwm1Regs.CMPCTL.bit.LOADASYNC           = 0U;          // Active Compare A Load SYNC Option
       EPwm1Regs.CMPCTL.bit.LOADBSYNC           = 0U;          // Active Compare B Load SYNC Option
       EPwm1Regs.CMPCTL.bit.LOADAMODE           = 0U;          // Active Compare A Load
       EPwm1Regs.CMPCTL.bit.LOADBMODE           = 0U;          // Active Compare B Load
       EPwm1Regs.CMPCTL.bit.SHDWAMODE           = 0U;          // Compare A Register Block Operating Mode
       EPwm1Regs.CMPCTL.bit.SHDWBMODE           = 0U;          // Compare B Register Block Operating Mode
     */
    EPwm1Regs.CMPCTL.all = (EPwm1Regs.CMPCTL.all & ~0x3C5FU) | 0x0U;

    /* EPwm1Regs.CMPCTL2.bit.SHDWCMODE           = 0U;          // Compare C Register Block Operating Mode
       EPwm1Regs.CMPCTL2.bit.SHDWDMODE           = 0U;          // Compare D Register Block Operating Mode
       EPwm1Regs.CMPCTL2.bit.LOADCSYNC           = 0U;          // Active Compare C Load SYNC Option
       EPwm1Regs.CMPCTL2.bit.LOADDSYNC           = 0U;          // Active Compare D Load SYNC Option
       EPwm1Regs.CMPCTL2.bit.LOADCMODE           = 0U;          // Active Compare C Load
       EPwm1Regs.CMPCTL2.bit.LOADDMODE           = 0U;          // Active Compare D Load
     */
    EPwm1Regs.CMPCTL2.all = (EPwm1Regs.CMPCTL2.all & ~0x3C5FU) | 0x0U;
    EPwm1Regs.CMPA.bit.CMPA = 1500U;   // Counter Compare A Register
    EPwm1Regs.CMPB.bit.CMPB = 32000U;  // Counter Compare B Register
    EPwm1Regs.CMPC = 32000U;           // Counter Compare C Register
    EPwm1Regs.CMPD = 32000U;           // Counter Compare D Register

    /*-- Setup Action-Qualifier (AQ) Submodule --*/
    EPwm1Regs.AQCTLA.all = 144U;
                               // Action Qualifier Control Register For Output A
    EPwm1Regs.AQCTLB.all = 0U; // Action Qualifier Control Register For Output B

    /*	// Action Qualifier Software Force Register
       EPwm1Regs.AQSFRC.bit.RLDCSF              = 0U;          // Reload from Shadow Options
     */
    EPwm1Regs.AQSFRC.all = (EPwm1Regs.AQSFRC.all & ~0xC0U) | 0x0U;

    /*	// Action Qualifier Continuous S/W Force Register
       EPwm1Regs.AQCSFRC.bit.CSFA               = 0U;          // Continuous Software Force on output A
       EPwm1Regs.AQCSFRC.bit.CSFB               = 0U;          // Continuous Software Force on output B
     */
    EPwm1Regs.AQCSFRC.all = (EPwm1Regs.AQCSFRC.all & ~0xFU) | 0x0U;

    /*-- Setup Dead-Band Generator (DB) Submodule --*/
    /*	// Dead-Band Generator Control Register
       EPwm1Regs.DBCTL.bit.OUT_MODE             = 3U;          // Dead Band Output Mode Control
       EPwm1Regs.DBCTL.bit.IN_MODE              = 0U;          // Dead Band Input Select Mode Control
       EPwm1Regs.DBCTL.bit.POLSEL               = 2U;          // Polarity Select Control
       EPwm1Regs.DBCTL.bit.HALFCYCLE            = 0U;          // Half Cycle Clocking Enable
       EPwm1Regs.DBCTL.bit.SHDWDBREDMODE        = 0U;          // DBRED shadow mode
       EPwm1Regs.DBCTL.bit.SHDWDBFEDMODE        = 0U;          // DBFED shadow mode
       EPwm1Regs.DBCTL.bit.LOADREDMODE          = 4U;        // DBRED load
       EPwm1Regs.DBCTL.bit.LOADFEDMODE          = 4U;        // DBFED load
     */
    EPwm1Regs.DBCTL.all = (EPwm1Regs.DBCTL.all & ~0x8FFFU) | 0xBU;
    EPwm1Regs.DBRED.bit.DBRED = (uint16_T)(24.0);
                         // Dead-Band Generator Rising Edge Delay Count Register
    EPwm1Regs.DBFED.bit.DBFED = (uint16_T)(24.0);
                        // Dead-Band Generator Falling Edge Delay Count Register

    /*-- Setup Event-Trigger (ET) Submodule --*/
    /*	// Event Trigger Selection and Pre-Scale Register
       EPwm1Regs.ETSEL.bit.SOCAEN               = 1U;          // Start of Conversion A Enable
       EPwm1Regs.ETSEL.bit.SOCASELCMP           = 0U;
       EPwm1Regs.ETSEL.bit.SOCASEL              = 2U;          // Start of Conversion A Select
       EPwm1Regs.ETPS.bit.SOCPSSEL              = 1U;          // EPWM1SOC Period Select
       EPwm1Regs.ETSOCPS.bit.SOCAPRD2           = 1U;
       EPwm1Regs.ETSEL.bit.SOCBEN               = 0U;          // Start of Conversion B Enable
       EPwm1Regs.ETSEL.bit.SOCBSELCMP           = 0U;
       EPwm1Regs.ETSEL.bit.SOCBSEL              = 1U;          // Start of Conversion A Select
       EPwm1Regs.ETPS.bit.SOCPSSEL              = 1;          // EPWM1SOCB Period Select
       EPwm1Regs.ETSOCPS.bit.SOCBPRD2           = 1U;
       EPwm1Regs.ETSEL.bit.INTEN                = 0U;          // EPWM1INTn Enable
       EPwm1Regs.ETSEL.bit.INTSELCMP            = 0U;
       EPwm1Regs.ETSEL.bit.INTSEL               = 1U;          // Start of Conversion A Select
       EPwm1Regs.ETPS.bit.INTPSSEL              = 1U;          // EPWM1INTn Period Select
       EPwm1Regs.ETINTPS.bit.INTPRD2            = 1U;
     */
    EPwm1Regs.ETSEL.all = (EPwm1Regs.ETSEL.all & ~0xFF7FU) | 0x1A01U;
    EPwm1Regs.ETPS.all = (EPwm1Regs.ETPS.all & ~0x30U) | 0x30U;
    EPwm1Regs.ETSOCPS.all = (EPwm1Regs.ETSOCPS.all & ~0xF0FU) | 0x101U;
    EPwm1Regs.ETINTPS.all = (EPwm1Regs.ETINTPS.all & ~0xFU) | 0x1U;

    /*-- Setup PWM-Chopper (PC) Submodule --*/
    /*	// PWM Chopper Control Register
       EPwm1Regs.PCCTL.bit.CHPEN                = 0U;          // PWM chopping enable
       EPwm1Regs.PCCTL.bit.CHPFREQ              = 0U;          // Chopping clock frequency
       EPwm1Regs.PCCTL.bit.OSHTWTH              = 0U;          // One-shot pulse width
       EPwm1Regs.PCCTL.bit.CHPDUTY              = 0U;          // Chopping clock Duty cycle
     */
    EPwm1Regs.PCCTL.all = (EPwm1Regs.PCCTL.all & ~0x7FFU) | 0x0U;

    /*-- Set up Trip-Zone (TZ) Submodule --*/
    EALLOW;
    EPwm1Regs.TZSEL.all = 0U;          // Trip Zone Select Register

    /*-- Set CBCPULSE for CBC latch Clear --*/
    EPwm1Regs.TZCLR.bit.CBCPULSE = 0U;

    /*	// Trip Zone Control Register
       EPwm1Regs.TZCTL.bit.TZA                  = 3U;          // TZ1 to TZ6 Trip Action On EPWM1A
       EPwm1Regs.TZCTL.bit.TZB                  = 3U;          // TZ1 to TZ6 Trip Action On EPWM1B
       EPwm1Regs.TZCTL.bit.DCAEVT1              = 1U;          // EPWM1A action on DCAEVT1
       EPwm1Regs.TZCTL.bit.DCAEVT2              = 2U;          // EPWM1A action on DCAEVT2
       EPwm1Regs.TZCTL.bit.DCBEVT1              = 1U;          // EPWM1B action on DCBEVT1
       EPwm1Regs.TZCTL.bit.DCBEVT2              = 2U;          // EPWM1B action on DCBEVT2
     */
    EPwm1Regs.TZCTL.all = (EPwm1Regs.TZCTL.all & ~0xFFFU) | 0x99FU;

    /*	// Trip Zone Enable Interrupt Register
       EPwm1Regs.TZEINT.bit.OST                 = 0U;          // Trip Zones One Shot Int Enable
       EPwm1Regs.TZEINT.bit.CBC                 = 0U;          // Trip Zones Cycle By Cycle Int Enable
       EPwm1Regs.TZEINT.bit.DCAEVT1             = 0U;          // Digital Compare A Event 1 Int Enable
       EPwm1Regs.TZEINT.bit.DCAEVT2             = 0U;          // Digital Compare A Event 2 Int Enable
       EPwm1Regs.TZEINT.bit.DCBEVT1             = 0U;          // Digital Compare B Event 1 Int Enable
       EPwm1Regs.TZEINT.bit.DCBEVT2             = 0U;          // Digital Compare B Event 2 Int Enable
     */
    EPwm1Regs.TZEINT.all = (EPwm1Regs.TZEINT.all & ~0x7EU) | 0x0U;

    /*	// Digital Compare A Control Register
       EPwm1Regs.DCACTL.bit.EVT1SYNCE           = 0U;          // DCAEVT1 SYNC Enable
       EPwm1Regs.DCACTL.bit.EVT1SOCE            = 0U;          // DCAEVT1 SOC Enable
       EPwm1Regs.DCACTL.bit.EVT1FRCSYNCSEL      = 0U;          // DCAEVT1 Force Sync Signal
       EPwm1Regs.DCACTL.bit.EVT1SRCSEL          = 0U;          // DCAEVT1 Source Signal
       EPwm1Regs.DCACTL.bit.EVT2FRCSYNCSEL      = 0U;          // DCAEVT2 Force Sync Signal
       EPwm1Regs.DCACTL.bit.EVT2SRCSEL          = 0U;          // DCAEVT2 Source Signal
     */
    EPwm1Regs.DCACTL.all = (EPwm1Regs.DCACTL.all & ~0x30FU) | 0x0U;

    /*	// Digital Compare B Control Register
       EPwm1Regs.DCBCTL.bit.EVT1SYNCE           = 0U;          // DCBEVT1 SYNC Enable
       EPwm1Regs.DCBCTL.bit.EVT1SOCE            = 0U;          // DCBEVT1 SOC Enable
       EPwm1Regs.DCBCTL.bit.EVT1FRCSYNCSEL      = 0U;          // DCBEVT1 Force Sync Signal
       EPwm1Regs.DCBCTL.bit.EVT1SRCSEL          = 0U;          // DCBEVT1 Source Signal
       EPwm1Regs.DCBCTL.bit.EVT2FRCSYNCSEL      = 0U;          // DCBEVT2 Force Sync Signal
       EPwm1Regs.DCBCTL.bit.EVT2SRCSEL          = 0U;          // DCBEVT2 Source Signal
     */
    EPwm1Regs.DCBCTL.all = (EPwm1Regs.DCBCTL.all & ~0x30FU) | 0x0U;

    /*	// Digital Compare Trip Select Register
       EPwm1Regs.DCTRIPSEL.bit.DCAHCOMPSEL      = 0U;          // Digital Compare A High COMP Input Select

       EPwm1Regs.DCTRIPSEL.bit.DCALCOMPSEL      = 1U;          // Digital Compare A Low COMP Input Select
       EPwm1Regs.DCTRIPSEL.bit.DCBHCOMPSEL      = 0U;          // Digital Compare B High COMP Input Select
       EPwm1Regs.DCTRIPSEL.bit.DCBLCOMPSEL      = 1U;          // Digital Compare B Low COMP Input Select
     */
    EPwm1Regs.DCTRIPSEL.all = (EPwm1Regs.DCTRIPSEL.all & ~ 0xFFFFU) | 0x1010U;

    /*	// Trip Zone Digital Comparator Select Register
       EPwm1Regs.TZDCSEL.bit.DCAEVT1            = 0U;          // Digital Compare Output A Event 1
       EPwm1Regs.TZDCSEL.bit.DCAEVT2            = 0U;          // Digital Compare Output A Event 2
       EPwm1Regs.TZDCSEL.bit.DCBEVT1            = 0U;          // Digital Compare Output B Event 1
       EPwm1Regs.TZDCSEL.bit.DCBEVT2            = 0U;          // Digital Compare Output B Event 2
     */
    EPwm1Regs.TZDCSEL.all = (EPwm1Regs.TZDCSEL.all & ~0xFFFU) | 0x0U;

    /*	// Digital Compare Filter Control Register
       EPwm1Regs.DCFCTL.bit.BLANKE              = 0U;          // Blanking Enable/Disable
       EPwm1Regs.DCFCTL.bit.PULSESEL            = 1U;          // Pulse Select for Blanking & Capture Alignment
       EPwm1Regs.DCFCTL.bit.BLANKINV            = 0U;          // Blanking Window Inversion
       EPwm1Regs.DCFCTL.bit.SRCSEL              = 0U;          // Filter Block Signal Source Select
     */
    EPwm1Regs.DCFCTL.all = (EPwm1Regs.DCFCTL.all & ~0x3FU) | 0x10U;
    EPwm1Regs.DCFOFFSET = 0U;          // Digital Compare Filter Offset Register
    EPwm1Regs.DCFWINDOW = 0U;          // Digital Compare Filter Window Register

    /*	// Digital Compare Capture Control Register
       EPwm1Regs.DCCAPCTL.bit.CAPE              = 0U;          // Counter Capture Enable
     */
    EPwm1Regs.DCCAPCTL.all = (EPwm1Regs.DCCAPCTL.all & ~0x1U) | 0x0U;

    /*	// HRPWM Configuration Register
       EPwm1Regs.HRCNFG.bit.SWAPAB              = 0U;          // Swap EPWMA and EPWMB Outputs Bit
       EPwm1Regs.HRCNFG.bit.SELOUTB             = 1U;          // EPWMB Output Selection Bit
     */
    EPwm1Regs.HRCNFG.all = (EPwm1Regs.HRCNFG.all & ~0xA0U) | 0x20U;

    /* Update the Link Registers with the link value for all the Compare values and TBPRD */
    /* No error is thrown if the ePWM register exists in the model or not */
    EPwm1Regs.EPWMXLINK.bit.TBPRDLINK = 0U;
    EPwm1Regs.EPWMXLINK.bit.CMPALINK = 0U;
    EPwm1Regs.EPWMXLINK.bit.CMPBLINK = 0U;
    EPwm1Regs.EPWMXLINK.bit.CMPCLINK = 0U;
    EPwm1Regs.EPWMXLINK.bit.CMPDLINK = 0U;

    /* SYNCPER - Peripheral synchronization output event
       EPwm1Regs.HRPCTL.bit.PWMSYNCSEL            = 0U;          // EPWMSYNCPER selection
       EPwm1Regs.HRPCTL.bit.PWMSYNCSELX           = 0U;          //  EPWMSYNCPER selection
     */
    EPwm1Regs.HRPCTL.all = (EPwm1Regs.HRPCTL.all & ~0x72U) | 0x0U;
    EDIS;
  }

  /* Start for S-Function (c2802xpwm): '<Root>/ePWM1 Leg A 20kHz1' */

  /*** Initialize ePWM2 modules ***/
  {
    /*  // Time Base Control Register
       EPwm2Regs.TBCTL.bit.CTRMODE              = 2U;          // Counter Mode

       EPwm2Regs.TBCTL.bit.PRDLD                = 0U;          // Shadow select

       EPwm2Regs.TBCTL2.bit.PRDLDSYNC           = 0U;          // Shadow select

       EPwm2Regs.TBCTL.bit.PHSEN                = 1U;          // Phase Load Enable
       EPwm2Regs.TBCTL.bit.PHSDIR               = 0U;          // Phase Direction Bit
       EPwm2Regs.TBCTL.bit.HSPCLKDIV            = 0U;          // High Speed TBCLK Pre-scaler
       EPwm2Regs.TBCTL.bit.CLKDIV               = 0U;          // Time Base Clock Pre-scaler
     */
    EPwm2Regs.TBCTL.all = (EPwm2Regs.TBCTL.all & ~0x3FCFU) | 0x6U;
    EPwm2Regs.TBCTL2.all = (EPwm2Regs.TBCTL2.all & ~0xC000U) | 0x0U;

    /* // Time-Base EPWMxSYNCOUT Source Enable Register
     */
    EPwm2Regs.EPWMSYNCOUTEN.bit.SWEN = 0U;// disable the SWEN enabled during reset
    EPwm2Regs.EPWMSYNCOUTEN.all = (EPwm2Regs.EPWMSYNCOUTEN.all & ~0x0U) | 0x0U;

    /*-- Setup Time-Base (TB) Submodule --*/
    EPwm2Regs.TBPRD = 3000U;           // Time Base Period Register

    /* // Time-Base Phase Register
       EPwm2Regs.TBPHS.bit.TBPHS               = 0U;          // Phase offset register
     */
    EPwm2Regs.TBPHS.all = (EPwm2Regs.TBPHS.all & ~0xFFFF0000U) | 0x0U;

    // Time Base Counter Register
    EPwm2Regs.TBCTR = 0x0000U;         /* Clear counter*/

    /*-- Setup Counter_Compare (CC) Submodule --*/
    /*	// Counter Compare Control Register

       EPwm2Regs.CMPCTL.bit.LOADASYNC           = 0U;          // Active Compare A Load SYNC Option
       EPwm2Regs.CMPCTL.bit.LOADBSYNC           = 0U;          // Active Compare B Load SYNC Option
       EPwm2Regs.CMPCTL.bit.LOADAMODE           = 0U;          // Active Compare A Load
       EPwm2Regs.CMPCTL.bit.LOADBMODE           = 0U;          // Active Compare B Load
       EPwm2Regs.CMPCTL.bit.SHDWAMODE           = 0U;          // Compare A Register Block Operating Mode
       EPwm2Regs.CMPCTL.bit.SHDWBMODE           = 0U;          // Compare B Register Block Operating Mode
     */
    EPwm2Regs.CMPCTL.all = (EPwm2Regs.CMPCTL.all & ~0x3C5FU) | 0x0U;

    /* EPwm2Regs.CMPCTL2.bit.SHDWCMODE           = 0U;          // Compare C Register Block Operating Mode
       EPwm2Regs.CMPCTL2.bit.SHDWDMODE           = 0U;          // Compare D Register Block Operating Mode
       EPwm2Regs.CMPCTL2.bit.LOADCSYNC           = 0U;          // Active Compare C Load SYNC Option
       EPwm2Regs.CMPCTL2.bit.LOADDSYNC           = 0U;          // Active Compare D Load SYNC Option
       EPwm2Regs.CMPCTL2.bit.LOADCMODE           = 0U;          // Active Compare C Load
       EPwm2Regs.CMPCTL2.bit.LOADDMODE           = 0U;          // Active Compare D Load
     */
    EPwm2Regs.CMPCTL2.all = (EPwm2Regs.CMPCTL2.all & ~0x3C5FU) | 0x0U;
    EPwm2Regs.CMPA.bit.CMPA = 1500U;   // Counter Compare A Register
    EPwm2Regs.CMPB.bit.CMPB = 32000U;  // Counter Compare B Register
    EPwm2Regs.CMPC = 32000U;           // Counter Compare C Register
    EPwm2Regs.CMPD = 32000U;           // Counter Compare D Register

    /*-- Setup Action-Qualifier (AQ) Submodule --*/
    EPwm2Regs.AQCTLA.all = 144U;
                               // Action Qualifier Control Register For Output A
    EPwm2Regs.AQCTLB.all = 0U; // Action Qualifier Control Register For Output B

    /*	// Action Qualifier Software Force Register
       EPwm2Regs.AQSFRC.bit.RLDCSF              = 0U;          // Reload from Shadow Options
     */
    EPwm2Regs.AQSFRC.all = (EPwm2Regs.AQSFRC.all & ~0xC0U) | 0x0U;

    /*	// Action Qualifier Continuous S/W Force Register
       EPwm2Regs.AQCSFRC.bit.CSFA               = 0U;          // Continuous Software Force on output A
       EPwm2Regs.AQCSFRC.bit.CSFB               = 0U;          // Continuous Software Force on output B
     */
    EPwm2Regs.AQCSFRC.all = (EPwm2Regs.AQCSFRC.all & ~0xFU) | 0x0U;

    /*-- Setup Dead-Band Generator (DB) Submodule --*/
    /*	// Dead-Band Generator Control Register
       EPwm2Regs.DBCTL.bit.OUT_MODE             = 3U;          // Dead Band Output Mode Control
       EPwm2Regs.DBCTL.bit.IN_MODE              = 0U;          // Dead Band Input Select Mode Control
       EPwm2Regs.DBCTL.bit.POLSEL               = 2U;          // Polarity Select Control
       EPwm2Regs.DBCTL.bit.HALFCYCLE            = 0U;          // Half Cycle Clocking Enable
       EPwm2Regs.DBCTL.bit.SHDWDBREDMODE        = 0U;          // DBRED shadow mode
       EPwm2Regs.DBCTL.bit.SHDWDBFEDMODE        = 0U;          // DBFED shadow mode
       EPwm2Regs.DBCTL.bit.LOADREDMODE          = 4U;        // DBRED load
       EPwm2Regs.DBCTL.bit.LOADFEDMODE          = 4U;        // DBFED load
     */
    EPwm2Regs.DBCTL.all = (EPwm2Regs.DBCTL.all & ~0x8FFFU) | 0xBU;
    EPwm2Regs.DBRED.bit.DBRED = (uint16_T)(24.0);
                         // Dead-Band Generator Rising Edge Delay Count Register
    EPwm2Regs.DBFED.bit.DBFED = (uint16_T)(24.0);
                        // Dead-Band Generator Falling Edge Delay Count Register

    /*-- Setup Event-Trigger (ET) Submodule --*/
    /*	// Event Trigger Selection and Pre-Scale Register
       EPwm2Regs.ETSEL.bit.SOCAEN               = 1U;          // Start of Conversion A Enable
       EPwm2Regs.ETSEL.bit.SOCASELCMP           = 0U;
       EPwm2Regs.ETSEL.bit.SOCASEL              = 2U;          // Start of Conversion A Select
       EPwm2Regs.ETPS.bit.SOCPSSEL              = 1U;          // EPWM2SOC Period Select
       EPwm2Regs.ETSOCPS.bit.SOCAPRD2           = 1U;
       EPwm2Regs.ETSEL.bit.SOCBEN               = 0U;          // Start of Conversion B Enable
       EPwm2Regs.ETSEL.bit.SOCBSELCMP           = 0U;
       EPwm2Regs.ETSEL.bit.SOCBSEL              = 1U;          // Start of Conversion A Select
       EPwm2Regs.ETPS.bit.SOCPSSEL              = 1;          // EPWM2SOCB Period Select
       EPwm2Regs.ETSOCPS.bit.SOCBPRD2           = 1U;
       EPwm2Regs.ETSEL.bit.INTEN                = 0U;          // EPWM2INTn Enable
       EPwm2Regs.ETSEL.bit.INTSELCMP            = 0U;
       EPwm2Regs.ETSEL.bit.INTSEL               = 1U;          // Start of Conversion A Select
       EPwm2Regs.ETPS.bit.INTPSSEL              = 1U;          // EPWM2INTn Period Select
       EPwm2Regs.ETINTPS.bit.INTPRD2            = 1U;
     */
    EPwm2Regs.ETSEL.all = (EPwm2Regs.ETSEL.all & ~0xFF7FU) | 0x1A01U;
    EPwm2Regs.ETPS.all = (EPwm2Regs.ETPS.all & ~0x30U) | 0x30U;
    EPwm2Regs.ETSOCPS.all = (EPwm2Regs.ETSOCPS.all & ~0xF0FU) | 0x101U;
    EPwm2Regs.ETINTPS.all = (EPwm2Regs.ETINTPS.all & ~0xFU) | 0x1U;

    /*-- Setup PWM-Chopper (PC) Submodule --*/
    /*	// PWM Chopper Control Register
       EPwm2Regs.PCCTL.bit.CHPEN                = 0U;          // PWM chopping enable
       EPwm2Regs.PCCTL.bit.CHPFREQ              = 0U;          // Chopping clock frequency
       EPwm2Regs.PCCTL.bit.OSHTWTH              = 0U;          // One-shot pulse width
       EPwm2Regs.PCCTL.bit.CHPDUTY              = 0U;          // Chopping clock Duty cycle
     */
    EPwm2Regs.PCCTL.all = (EPwm2Regs.PCCTL.all & ~0x7FFU) | 0x0U;

    /*-- Set up Trip-Zone (TZ) Submodule --*/
    EALLOW;
    EPwm2Regs.TZSEL.all = 0U;          // Trip Zone Select Register

    /*-- Set CBCPULSE for CBC latch Clear --*/
    EPwm2Regs.TZCLR.bit.CBCPULSE = 0U;

    /*	// Trip Zone Control Register
       EPwm2Regs.TZCTL.bit.TZA                  = 3U;          // TZ1 to TZ6 Trip Action On EPWM2A
       EPwm2Regs.TZCTL.bit.TZB                  = 3U;          // TZ1 to TZ6 Trip Action On EPWM2B
       EPwm2Regs.TZCTL.bit.DCAEVT1              = 1U;          // EPWM2A action on DCAEVT1
       EPwm2Regs.TZCTL.bit.DCAEVT2              = 2U;          // EPWM2A action on DCAEVT2
       EPwm2Regs.TZCTL.bit.DCBEVT1              = 1U;          // EPWM2B action on DCBEVT1
       EPwm2Regs.TZCTL.bit.DCBEVT2              = 2U;          // EPWM2B action on DCBEVT2
     */
    EPwm2Regs.TZCTL.all = (EPwm2Regs.TZCTL.all & ~0xFFFU) | 0x99FU;

    /*	// Trip Zone Enable Interrupt Register
       EPwm2Regs.TZEINT.bit.OST                 = 0U;          // Trip Zones One Shot Int Enable
       EPwm2Regs.TZEINT.bit.CBC                 = 0U;          // Trip Zones Cycle By Cycle Int Enable
       EPwm2Regs.TZEINT.bit.DCAEVT1             = 0U;          // Digital Compare A Event 1 Int Enable
       EPwm2Regs.TZEINT.bit.DCAEVT2             = 0U;          // Digital Compare A Event 2 Int Enable
       EPwm2Regs.TZEINT.bit.DCBEVT1             = 0U;          // Digital Compare B Event 1 Int Enable
       EPwm2Regs.TZEINT.bit.DCBEVT2             = 0U;          // Digital Compare B Event 2 Int Enable
     */
    EPwm2Regs.TZEINT.all = (EPwm2Regs.TZEINT.all & ~0x7EU) | 0x0U;

    /*	// Digital Compare A Control Register
       EPwm2Regs.DCACTL.bit.EVT1SYNCE           = 0U;          // DCAEVT1 SYNC Enable
       EPwm2Regs.DCACTL.bit.EVT1SOCE            = 0U;          // DCAEVT1 SOC Enable
       EPwm2Regs.DCACTL.bit.EVT1FRCSYNCSEL      = 0U;          // DCAEVT1 Force Sync Signal
       EPwm2Regs.DCACTL.bit.EVT1SRCSEL          = 0U;          // DCAEVT1 Source Signal
       EPwm2Regs.DCACTL.bit.EVT2FRCSYNCSEL      = 0U;          // DCAEVT2 Force Sync Signal
       EPwm2Regs.DCACTL.bit.EVT2SRCSEL          = 0U;          // DCAEVT2 Source Signal
     */
    EPwm2Regs.DCACTL.all = (EPwm2Regs.DCACTL.all & ~0x30FU) | 0x0U;

    /*	// Digital Compare B Control Register
       EPwm2Regs.DCBCTL.bit.EVT1SYNCE           = 0U;          // DCBEVT1 SYNC Enable
       EPwm2Regs.DCBCTL.bit.EVT1SOCE            = 0U;          // DCBEVT1 SOC Enable
       EPwm2Regs.DCBCTL.bit.EVT1FRCSYNCSEL      = 0U;          // DCBEVT1 Force Sync Signal
       EPwm2Regs.DCBCTL.bit.EVT1SRCSEL          = 0U;          // DCBEVT1 Source Signal
       EPwm2Regs.DCBCTL.bit.EVT2FRCSYNCSEL      = 0U;          // DCBEVT2 Force Sync Signal
       EPwm2Regs.DCBCTL.bit.EVT2SRCSEL          = 0U;          // DCBEVT2 Source Signal
     */
    EPwm2Regs.DCBCTL.all = (EPwm2Regs.DCBCTL.all & ~0x30FU) | 0x0U;

    /*	// Digital Compare Trip Select Register
       EPwm2Regs.DCTRIPSEL.bit.DCAHCOMPSEL      = 0U;          // Digital Compare A High COMP Input Select

       EPwm2Regs.DCTRIPSEL.bit.DCALCOMPSEL      = 1U;          // Digital Compare A Low COMP Input Select
       EPwm2Regs.DCTRIPSEL.bit.DCBHCOMPSEL      = 0U;          // Digital Compare B High COMP Input Select
       EPwm2Regs.DCTRIPSEL.bit.DCBLCOMPSEL      = 1U;          // Digital Compare B Low COMP Input Select
     */
    EPwm2Regs.DCTRIPSEL.all = (EPwm2Regs.DCTRIPSEL.all & ~ 0xFFFFU) | 0x1010U;

    /*	// Trip Zone Digital Comparator Select Register
       EPwm2Regs.TZDCSEL.bit.DCAEVT1            = 0U;          // Digital Compare Output A Event 1
       EPwm2Regs.TZDCSEL.bit.DCAEVT2            = 0U;          // Digital Compare Output A Event 2
       EPwm2Regs.TZDCSEL.bit.DCBEVT1            = 0U;          // Digital Compare Output B Event 1
       EPwm2Regs.TZDCSEL.bit.DCBEVT2            = 0U;          // Digital Compare Output B Event 2
     */
    EPwm2Regs.TZDCSEL.all = (EPwm2Regs.TZDCSEL.all & ~0xFFFU) | 0x0U;

    /*	// Digital Compare Filter Control Register
       EPwm2Regs.DCFCTL.bit.BLANKE              = 0U;          // Blanking Enable/Disable
       EPwm2Regs.DCFCTL.bit.PULSESEL            = 1U;          // Pulse Select for Blanking & Capture Alignment
       EPwm2Regs.DCFCTL.bit.BLANKINV            = 0U;          // Blanking Window Inversion
       EPwm2Regs.DCFCTL.bit.SRCSEL              = 0U;          // Filter Block Signal Source Select
     */
    EPwm2Regs.DCFCTL.all = (EPwm2Regs.DCFCTL.all & ~0x3FU) | 0x10U;
    EPwm2Regs.DCFOFFSET = 0U;          // Digital Compare Filter Offset Register
    EPwm2Regs.DCFWINDOW = 0U;          // Digital Compare Filter Window Register

    /*	// Digital Compare Capture Control Register
       EPwm2Regs.DCCAPCTL.bit.CAPE              = 0U;          // Counter Capture Enable
     */
    EPwm2Regs.DCCAPCTL.all = (EPwm2Regs.DCCAPCTL.all & ~0x1U) | 0x0U;

    /*	// HRPWM Configuration Register
       EPwm2Regs.HRCNFG.bit.SWAPAB              = 0U;          // Swap EPWMA and EPWMB Outputs Bit
       EPwm2Regs.HRCNFG.bit.SELOUTB             = 1U;          // EPWMB Output Selection Bit
     */
    EPwm2Regs.HRCNFG.all = (EPwm2Regs.HRCNFG.all & ~0xA0U) | 0x20U;

    /* Update the Link Registers with the link value for all the Compare values and TBPRD */
    /* No error is thrown if the ePWM register exists in the model or not */
    EPwm2Regs.EPWMXLINK.bit.TBPRDLINK = 1U;
    EPwm2Regs.EPWMXLINK.bit.CMPALINK = 1U;
    EPwm2Regs.EPWMXLINK.bit.CMPBLINK = 1U;
    EPwm2Regs.EPWMXLINK.bit.CMPCLINK = 1U;
    EPwm2Regs.EPWMXLINK.bit.CMPDLINK = 1U;

    /* SYNCPER - Peripheral synchronization output event
       EPwm2Regs.HRPCTL.bit.PWMSYNCSEL            = 0U;          // EPWMSYNCPER selection
       EPwm2Regs.HRPCTL.bit.PWMSYNCSELX           = 0U;          //  EPWMSYNCPER selection
     */
    EPwm2Regs.HRPCTL.all = (EPwm2Regs.HRPCTL.all & ~0x72U) | 0x0U;
    EDIS;
  }

  /* Start for S-Function (c2802xpwm): '<Root>/ePWM1 Leg A 20kHz2' */

  /*** Initialize ePWM3 modules ***/
  {
    /*  // Time Base Control Register
       EPwm3Regs.TBCTL.bit.CTRMODE              = 2U;          // Counter Mode

       EPwm3Regs.TBCTL.bit.PRDLD                = 0U;          // Shadow select

       EPwm3Regs.TBCTL2.bit.PRDLDSYNC           = 0U;          // Shadow select

       EPwm3Regs.TBCTL.bit.PHSEN                = 1U;          // Phase Load Enable
       EPwm3Regs.TBCTL.bit.PHSDIR               = 1U;          // Phase Direction Bit
       EPwm3Regs.TBCTL.bit.HSPCLKDIV            = 0U;          // High Speed TBCLK Pre-scaler
       EPwm3Regs.TBCTL.bit.CLKDIV               = 0U;          // Time Base Clock Pre-scaler
     */
    EPwm3Regs.TBCTL.all = (EPwm3Regs.TBCTL.all & ~0x3FCFU) | 0x2006U;
    EPwm3Regs.TBCTL2.all = (EPwm3Regs.TBCTL2.all & ~0xC000U) | 0x0U;

    /* // Time-Base EPWMxSYNCOUT Source Enable Register
     */
    EPwm3Regs.EPWMSYNCOUTEN.bit.SWEN = 0U;// disable the SWEN enabled during reset
    EPwm3Regs.EPWMSYNCOUTEN.all = (EPwm3Regs.EPWMSYNCOUTEN.all & ~0x0U) | 0x0U;

    /*-- Setup Time-Base (TB) Submodule --*/
    EPwm3Regs.TBPRD = 3000U;           // Time Base Period Register

    /* // Time-Base Phase Register
       EPwm3Regs.TBPHS.bit.TBPHS               = 0U;          // Phase offset register
     */
    EPwm3Regs.TBPHS.all = (EPwm3Regs.TBPHS.all & ~0xFFFF0000U) | 0x0U;

    // Time Base Counter Register
    EPwm3Regs.TBCTR = 0x0000U;         /* Clear counter*/

    /*-- Setup Counter_Compare (CC) Submodule --*/
    /*	// Counter Compare Control Register

       EPwm3Regs.CMPCTL.bit.LOADASYNC           = 0U;          // Active Compare A Load SYNC Option
       EPwm3Regs.CMPCTL.bit.LOADBSYNC           = 0U;          // Active Compare B Load SYNC Option
       EPwm3Regs.CMPCTL.bit.LOADAMODE           = 0U;          // Active Compare A Load
       EPwm3Regs.CMPCTL.bit.LOADBMODE           = 0U;          // Active Compare B Load
       EPwm3Regs.CMPCTL.bit.SHDWAMODE           = 0U;          // Compare A Register Block Operating Mode
       EPwm3Regs.CMPCTL.bit.SHDWBMODE           = 0U;          // Compare B Register Block Operating Mode
     */
    EPwm3Regs.CMPCTL.all = (EPwm3Regs.CMPCTL.all & ~0x3C5FU) | 0x0U;

    /* EPwm3Regs.CMPCTL2.bit.SHDWCMODE           = 0U;          // Compare C Register Block Operating Mode
       EPwm3Regs.CMPCTL2.bit.SHDWDMODE           = 0U;          // Compare D Register Block Operating Mode
       EPwm3Regs.CMPCTL2.bit.LOADCSYNC           = 0U;          // Active Compare C Load SYNC Option
       EPwm3Regs.CMPCTL2.bit.LOADDSYNC           = 0U;          // Active Compare D Load SYNC Option
       EPwm3Regs.CMPCTL2.bit.LOADCMODE           = 0U;          // Active Compare C Load
       EPwm3Regs.CMPCTL2.bit.LOADDMODE           = 0U;          // Active Compare D Load
     */
    EPwm3Regs.CMPCTL2.all = (EPwm3Regs.CMPCTL2.all & ~0x3C5FU) | 0x0U;
    EPwm3Regs.CMPA.bit.CMPA = 1500U;   // Counter Compare A Register
    EPwm3Regs.CMPB.bit.CMPB = 32000U;  // Counter Compare B Register
    EPwm3Regs.CMPC = 32000U;           // Counter Compare C Register
    EPwm3Regs.CMPD = 32000U;           // Counter Compare D Register

    /*-- Setup Action-Qualifier (AQ) Submodule --*/
    EPwm3Regs.AQCTLA.all = 144U;
                               // Action Qualifier Control Register For Output A
    EPwm3Regs.AQCTLB.all = 0U; // Action Qualifier Control Register For Output B

    /*	// Action Qualifier Software Force Register
       EPwm3Regs.AQSFRC.bit.RLDCSF              = 0U;          // Reload from Shadow Options
     */
    EPwm3Regs.AQSFRC.all = (EPwm3Regs.AQSFRC.all & ~0xC0U) | 0x0U;

    /*	// Action Qualifier Continuous S/W Force Register
       EPwm3Regs.AQCSFRC.bit.CSFA               = 0U;          // Continuous Software Force on output A
       EPwm3Regs.AQCSFRC.bit.CSFB               = 0U;          // Continuous Software Force on output B
     */
    EPwm3Regs.AQCSFRC.all = (EPwm3Regs.AQCSFRC.all & ~0xFU) | 0x0U;

    /*-- Setup Dead-Band Generator (DB) Submodule --*/
    /*	// Dead-Band Generator Control Register
       EPwm3Regs.DBCTL.bit.OUT_MODE             = 3U;          // Dead Band Output Mode Control
       EPwm3Regs.DBCTL.bit.IN_MODE              = 0U;          // Dead Band Input Select Mode Control
       EPwm3Regs.DBCTL.bit.POLSEL               = 2U;          // Polarity Select Control
       EPwm3Regs.DBCTL.bit.HALFCYCLE            = 0U;          // Half Cycle Clocking Enable
       EPwm3Regs.DBCTL.bit.SHDWDBREDMODE        = 0U;          // DBRED shadow mode
       EPwm3Regs.DBCTL.bit.SHDWDBFEDMODE        = 0U;          // DBFED shadow mode
       EPwm3Regs.DBCTL.bit.LOADREDMODE          = 4U;        // DBRED load
       EPwm3Regs.DBCTL.bit.LOADFEDMODE          = 4U;        // DBFED load
     */
    EPwm3Regs.DBCTL.all = (EPwm3Regs.DBCTL.all & ~0x8FFFU) | 0xBU;
    EPwm3Regs.DBRED.bit.DBRED = (uint16_T)(24.0);
                         // Dead-Band Generator Rising Edge Delay Count Register
    EPwm3Regs.DBFED.bit.DBFED = (uint16_T)(24.0);
                        // Dead-Band Generator Falling Edge Delay Count Register

    /*-- Setup Event-Trigger (ET) Submodule --*/
    /*	// Event Trigger Selection and Pre-Scale Register
       EPwm3Regs.ETSEL.bit.SOCAEN               = 1U;          // Start of Conversion A Enable
       EPwm3Regs.ETSEL.bit.SOCASELCMP           = 0U;
       EPwm3Regs.ETSEL.bit.SOCASEL              = 2U;          // Start of Conversion A Select
       EPwm3Regs.ETPS.bit.SOCPSSEL              = 1U;          // EPWM3SOC Period Select
       EPwm3Regs.ETSOCPS.bit.SOCAPRD2           = 1U;
       EPwm3Regs.ETSEL.bit.SOCBEN               = 0U;          // Start of Conversion B Enable
       EPwm3Regs.ETSEL.bit.SOCBSELCMP           = 0U;
       EPwm3Regs.ETSEL.bit.SOCBSEL              = 1U;          // Start of Conversion A Select
       EPwm3Regs.ETPS.bit.SOCPSSEL              = 1;          // EPWM3SOCB Period Select
       EPwm3Regs.ETSOCPS.bit.SOCBPRD2           = 1U;
       EPwm3Regs.ETSEL.bit.INTEN                = 0U;          // EPWM3INTn Enable
       EPwm3Regs.ETSEL.bit.INTSELCMP            = 0U;
       EPwm3Regs.ETSEL.bit.INTSEL               = 1U;          // Start of Conversion A Select
       EPwm3Regs.ETPS.bit.INTPSSEL              = 1U;          // EPWM3INTn Period Select
       EPwm3Regs.ETINTPS.bit.INTPRD2            = 1U;
     */
    EPwm3Regs.ETSEL.all = (EPwm3Regs.ETSEL.all & ~0xFF7FU) | 0x1A01U;
    EPwm3Regs.ETPS.all = (EPwm3Regs.ETPS.all & ~0x30U) | 0x30U;
    EPwm3Regs.ETSOCPS.all = (EPwm3Regs.ETSOCPS.all & ~0xF0FU) | 0x101U;
    EPwm3Regs.ETINTPS.all = (EPwm3Regs.ETINTPS.all & ~0xFU) | 0x1U;

    /*-- Setup PWM-Chopper (PC) Submodule --*/
    /*	// PWM Chopper Control Register
       EPwm3Regs.PCCTL.bit.CHPEN                = 0U;          // PWM chopping enable
       EPwm3Regs.PCCTL.bit.CHPFREQ              = 0U;          // Chopping clock frequency
       EPwm3Regs.PCCTL.bit.OSHTWTH              = 0U;          // One-shot pulse width
       EPwm3Regs.PCCTL.bit.CHPDUTY              = 0U;          // Chopping clock Duty cycle
     */
    EPwm3Regs.PCCTL.all = (EPwm3Regs.PCCTL.all & ~0x7FFU) | 0x0U;

    /*-- Set up Trip-Zone (TZ) Submodule --*/
    EALLOW;
    EPwm3Regs.TZSEL.all = 0U;          // Trip Zone Select Register

    /*-- Set CBCPULSE for CBC latch Clear --*/
    EPwm3Regs.TZCLR.bit.CBCPULSE = 0U;

    /*	// Trip Zone Control Register
       EPwm3Regs.TZCTL.bit.TZA                  = 3U;          // TZ1 to TZ6 Trip Action On EPWM3A
       EPwm3Regs.TZCTL.bit.TZB                  = 3U;          // TZ1 to TZ6 Trip Action On EPWM3B
       EPwm3Regs.TZCTL.bit.DCAEVT1              = 1U;          // EPWM3A action on DCAEVT1
       EPwm3Regs.TZCTL.bit.DCAEVT2              = 2U;          // EPWM3A action on DCAEVT2
       EPwm3Regs.TZCTL.bit.DCBEVT1              = 1U;          // EPWM3B action on DCBEVT1
       EPwm3Regs.TZCTL.bit.DCBEVT2              = 2U;          // EPWM3B action on DCBEVT2
     */
    EPwm3Regs.TZCTL.all = (EPwm3Regs.TZCTL.all & ~0xFFFU) | 0x99FU;

    /*	// Trip Zone Enable Interrupt Register
       EPwm3Regs.TZEINT.bit.OST                 = 0U;          // Trip Zones One Shot Int Enable
       EPwm3Regs.TZEINT.bit.CBC                 = 0U;          // Trip Zones Cycle By Cycle Int Enable
       EPwm3Regs.TZEINT.bit.DCAEVT1             = 0U;          // Digital Compare A Event 1 Int Enable
       EPwm3Regs.TZEINT.bit.DCAEVT2             = 0U;          // Digital Compare A Event 2 Int Enable
       EPwm3Regs.TZEINT.bit.DCBEVT1             = 0U;          // Digital Compare B Event 1 Int Enable
       EPwm3Regs.TZEINT.bit.DCBEVT2             = 0U;          // Digital Compare B Event 2 Int Enable
     */
    EPwm3Regs.TZEINT.all = (EPwm3Regs.TZEINT.all & ~0x7EU) | 0x0U;

    /*	// Digital Compare A Control Register
       EPwm3Regs.DCACTL.bit.EVT1SYNCE           = 0U;          // DCAEVT1 SYNC Enable
       EPwm3Regs.DCACTL.bit.EVT1SOCE            = 0U;          // DCAEVT1 SOC Enable
       EPwm3Regs.DCACTL.bit.EVT1FRCSYNCSEL      = 0U;          // DCAEVT1 Force Sync Signal
       EPwm3Regs.DCACTL.bit.EVT1SRCSEL          = 0U;          // DCAEVT1 Source Signal
       EPwm3Regs.DCACTL.bit.EVT2FRCSYNCSEL      = 0U;          // DCAEVT2 Force Sync Signal
       EPwm3Regs.DCACTL.bit.EVT2SRCSEL          = 0U;          // DCAEVT2 Source Signal
     */
    EPwm3Regs.DCACTL.all = (EPwm3Regs.DCACTL.all & ~0x30FU) | 0x0U;

    /*	// Digital Compare B Control Register
       EPwm3Regs.DCBCTL.bit.EVT1SYNCE           = 0U;          // DCBEVT1 SYNC Enable
       EPwm3Regs.DCBCTL.bit.EVT1SOCE            = 0U;          // DCBEVT1 SOC Enable
       EPwm3Regs.DCBCTL.bit.EVT1FRCSYNCSEL      = 0U;          // DCBEVT1 Force Sync Signal
       EPwm3Regs.DCBCTL.bit.EVT1SRCSEL          = 0U;          // DCBEVT1 Source Signal
       EPwm3Regs.DCBCTL.bit.EVT2FRCSYNCSEL      = 0U;          // DCBEVT2 Force Sync Signal
       EPwm3Regs.DCBCTL.bit.EVT2SRCSEL          = 0U;          // DCBEVT2 Source Signal
     */
    EPwm3Regs.DCBCTL.all = (EPwm3Regs.DCBCTL.all & ~0x30FU) | 0x0U;

    /*	// Digital Compare Trip Select Register
       EPwm3Regs.DCTRIPSEL.bit.DCAHCOMPSEL      = 0U;          // Digital Compare A High COMP Input Select

       EPwm3Regs.DCTRIPSEL.bit.DCALCOMPSEL      = 1U;          // Digital Compare A Low COMP Input Select
       EPwm3Regs.DCTRIPSEL.bit.DCBHCOMPSEL      = 0U;          // Digital Compare B High COMP Input Select
       EPwm3Regs.DCTRIPSEL.bit.DCBLCOMPSEL      = 1U;          // Digital Compare B Low COMP Input Select
     */
    EPwm3Regs.DCTRIPSEL.all = (EPwm3Regs.DCTRIPSEL.all & ~ 0xFFFFU) | 0x1010U;

    /*	// Trip Zone Digital Comparator Select Register
       EPwm3Regs.TZDCSEL.bit.DCAEVT1            = 0U;          // Digital Compare Output A Event 1
       EPwm3Regs.TZDCSEL.bit.DCAEVT2            = 0U;          // Digital Compare Output A Event 2
       EPwm3Regs.TZDCSEL.bit.DCBEVT1            = 0U;          // Digital Compare Output B Event 1
       EPwm3Regs.TZDCSEL.bit.DCBEVT2            = 0U;          // Digital Compare Output B Event 2
     */
    EPwm3Regs.TZDCSEL.all = (EPwm3Regs.TZDCSEL.all & ~0xFFFU) | 0x0U;

    /*	// Digital Compare Filter Control Register
       EPwm3Regs.DCFCTL.bit.BLANKE              = 0U;          // Blanking Enable/Disable
       EPwm3Regs.DCFCTL.bit.PULSESEL            = 1U;          // Pulse Select for Blanking & Capture Alignment
       EPwm3Regs.DCFCTL.bit.BLANKINV            = 0U;          // Blanking Window Inversion
       EPwm3Regs.DCFCTL.bit.SRCSEL              = 0U;          // Filter Block Signal Source Select
     */
    EPwm3Regs.DCFCTL.all = (EPwm3Regs.DCFCTL.all & ~0x3FU) | 0x10U;
    EPwm3Regs.DCFOFFSET = 0U;          // Digital Compare Filter Offset Register
    EPwm3Regs.DCFWINDOW = 0U;          // Digital Compare Filter Window Register

    /*	// Digital Compare Capture Control Register
       EPwm3Regs.DCCAPCTL.bit.CAPE              = 0U;          // Counter Capture Enable
     */
    EPwm3Regs.DCCAPCTL.all = (EPwm3Regs.DCCAPCTL.all & ~0x1U) | 0x0U;

    /*	// HRPWM Configuration Register
       EPwm3Regs.HRCNFG.bit.SWAPAB              = 0U;          // Swap EPWMA and EPWMB Outputs Bit
       EPwm3Regs.HRCNFG.bit.SELOUTB             = 1U;          // EPWMB Output Selection Bit
     */
    EPwm3Regs.HRCNFG.all = (EPwm3Regs.HRCNFG.all & ~0xA0U) | 0x20U;

    /* Update the Link Registers with the link value for all the Compare values and TBPRD */
    /* No error is thrown if the ePWM register exists in the model or not */
    EPwm3Regs.EPWMXLINK.bit.TBPRDLINK = 2U;
    EPwm3Regs.EPWMXLINK.bit.CMPALINK = 2U;
    EPwm3Regs.EPWMXLINK.bit.CMPBLINK = 2U;
    EPwm3Regs.EPWMXLINK.bit.CMPCLINK = 2U;
    EPwm3Regs.EPWMXLINK.bit.CMPDLINK = 2U;

    /* SYNCPER - Peripheral synchronization output event
       EPwm3Regs.HRPCTL.bit.PWMSYNCSEL            = 0U;          // EPWMSYNCPER selection
       EPwm3Regs.HRPCTL.bit.PWMSYNCSELX           = 0U;          //  EPWMSYNCPER selection
     */
    EPwm3Regs.HRPCTL.all = (EPwm3Regs.HRPCTL.all & ~0x72U) | 0x0U;
    EDIS;
  }

  /* Start for S-Function (c2802xadc): '<Root>/ADC1' */
  if (MW_adcCInitFlag == 0U) {
    InitAdcC();
    MW_adcCInitFlag = 1U;
  }

  config_ADCC_SOC1 ();

  /* Start for S-Function (c2802xadc): '<Root>/ADC2' */
  if (MW_adcAInitFlag == 0U) {
    InitAdcA();
    MW_adcAInitFlag = 1U;
  }

  config_ADCA_SOC0 ();

  /* Start for S-Function (c2802xadc): '<Root>/ADC3' */
  if (MW_adcCInitFlag == 0U) {
    InitAdcC();
    MW_adcCInitFlag = 1U;
  }

  config_ADCC_SOC2 ();

  /* Start for S-Function (c2802xadc): '<Root>/ADC4' */
  if (MW_adcAInitFlag == 0U) {
    InitAdcA();
    MW_adcAInitFlag = 1U;
  }

  config_ADCA_SOC2 ();

  /* Start for S-Function (c2802xadc): '<Root>/ADC5' */
  if (MW_adcCInitFlag == 0U) {
    InitAdcC();
    MW_adcCInitFlag = 1U;
  }

  config_ADCC_SOC0 ();

  /* Start for S-Function (c2802xadc): '<Root>/ADC6' */
  if (MW_adcAInitFlag == 0U) {
    InitAdcA();
    MW_adcAInitFlag = 1U;
  }

  config_ADCA_SOC1 ();

  /* InitializeConditions for DiscreteTransferFcn: '<Root>/Filtered load current magnitude' */
  chovy_tidy_DW.Filteredloadcurrentmagnitude_st =
    chovy_tidy_P.Filteredloadcurrentmagnitude_In;

  /* InitializeConditions for DiscreteTransferFcn: '<Root>/Filtered voltage d' */
  chovy_tidy_DW.Filteredvoltaged_states =
    chovy_tidy_P.Filteredvoltaged_InitialStates;

  /* InitializeConditions for DiscreteIntegrator: '<S43>/Integrator' */
  chovy_tidy_DW.Integrator_DSTATE = chovy_tidy_P.DiscretePIDController_InitialCo;
  chovy_tidy_DW.Integrator_PrevResetState = 0;

  /* InitializeConditions for DiscreteTransferFcn: '<Root>/Filtered voltage q' */
  chovy_tidy_DW.Filteredvoltageq_states =
    chovy_tidy_P.Filteredvoltageq_InitialStates;

  /* InitializeConditions for DiscreteIntegrator: '<S93>/Integrator' */
  chovy_tidy_DW.Integrator_DSTATE_b =
    chovy_tidy_P.DiscretePIDController1_InitialC;
  chovy_tidy_DW.Integrator_PrevResetState_o = 0;

  /* SystemInitialize for Enabled SubSystem: '<S210>/Subsystem - pi//2 delay' */
  chov_Subsystempi2delay_Init(&chovy_tidy_B.Fcn_l, &chovy_tidy_B.Fcn1_c,
    &chovy_tidy_P.Subsystempi2delay);

  /* End of SystemInitialize for SubSystem: '<S210>/Subsystem - pi//2 delay' */

  /* SystemInitialize for Enabled SubSystem: '<S210>/Subsystem1' */
  chovy_tidy_Subsystem1_Init(&chovy_tidy_B.Fcn_b, &chovy_tidy_B.Fcn1_k,
    &chovy_tidy_P.Subsystem1);

  /* End of SystemInitialize for SubSystem: '<S210>/Subsystem1' */

  /* SystemInitialize for Enabled SubSystem: '<S216>/Subsystem - pi//2 delay' */
  chov_Subsystempi2delay_Init(&chovy_tidy_B.Fcn_nz, &chovy_tidy_B.Fcn1_b2,
    &chovy_tidy_P.Subsystempi2delay_n);

  /* End of SystemInitialize for SubSystem: '<S216>/Subsystem - pi//2 delay' */

  /* SystemInitialize for Enabled SubSystem: '<S216>/Subsystem1' */
  chovy_tidy_Subsystem1_Init(&chovy_tidy_B.Fcn_m, &chovy_tidy_B.Fcn1_p,
    &chovy_tidy_P.Subsystem1_a);

  /* End of SystemInitialize for SubSystem: '<S216>/Subsystem1' */

  /* SystemInitialize for Enabled SubSystem: '<S9>/Subsystem - pi//2 delay' */
  /* SystemInitialize for Fcn: '<S224>/Fcn' incorporates:
   *  Outport: '<S224>/alpha_beta'
   */
  chovy_tidy_B.Fcn_n = chovy_tidy_P.alpha_beta_Y0[0];

  /* SystemInitialize for Fcn: '<S224>/Fcn1' incorporates:
   *  Outport: '<S224>/alpha_beta'
   */
  chovy_tidy_B.Fcn1_b = chovy_tidy_P.alpha_beta_Y0[1];

  /* End of SystemInitialize for SubSystem: '<S9>/Subsystem - pi//2 delay' */

  /* SystemInitialize for Enabled SubSystem: '<S9>/Subsystem1' */
  /* SystemInitialize for Fcn: '<S225>/Fcn' incorporates:
   *  Outport: '<S225>/alpha_beta'
   */
  chovy_tidy_B.Fcn = chovy_tidy_P.alpha_beta_Y0_e[0];

  /* SystemInitialize for Fcn: '<S225>/Fcn1' incorporates:
   *  Outport: '<S225>/alpha_beta'
   */
  chovy_tidy_B.Fcn1 = chovy_tidy_P.alpha_beta_Y0_e[1];

  /* End of SystemInitialize for SubSystem: '<S9>/Subsystem1' */

  /* user code (Initialize function Body) */
  ControlModel_Init();
}

/* Model terminate function */
void chovy_tidy_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
