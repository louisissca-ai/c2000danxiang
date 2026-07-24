/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: xtq2_dq_doubleloop_fullspec_data.c
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

#include "xtq2_dq_doubleloop_fullspec.h"

/* Block parameters (default storage) */
P_xtq2_dq_doubleloop_fullspec_T xtq2_dq_doubleloop_fullspec_P = {
  /* Variable: Fout
   * Referenced by: '<Root>/Electrical angle'
   */
  50.0,

  /* Variable: Idq_max
   * Referenced by:
   *   '<S95>/Saturation'
   *   '<S195>/Saturation'
   */
  4.5,

  /* Variable: Kad
   * Referenced by: '<Root>/Virtual damping gain'
   */
  8.261049,

  /* Variable: Ki_i_dq
   * Referenced by:
   *   '<S35>/Integral Gain'
   *   '<S135>/Integral Gain'
   */
  125.663706,

  /* Variable: Ki_v_dq
   * Referenced by:
   *   '<S85>/Integral Gain'
   *   '<S185>/Integral Gain'
   */
  2.5,

  /* Variable: Kp_i_dq
   * Referenced by:
   *   '<S43>/Proportional Gain'
   *   '<S143>/Proportional Gain'
   */
  5.026548,

  /* Variable: Kp_v_dq
   * Referenced by:
   *   '<S93>/Proportional Gain'
   *   '<S193>/Proportional Gain'
   */
  0.006,

  /* Variable: Udq_correction_max
   * Referenced by:
   *   '<S45>/Saturation'
   *   '<S145>/Saturation'
   */
  12.0,

  /* Variable: Vdq_ref_gain
   * Referenced by: '<Root>/Vd reference'
   */
  1.0,

  /* Variable: Vin (overridden from APP_VBUS_NOMINAL_V at startup)
   * Referenced by:
   *   '<Root>/Normalized modulation m'
   *   '<Root>/Modulation limit'
   */
  36.0,

  /* Variable: Vout_rms_ref
   * Referenced by: '<Root>/Vd reference'
   */
  21.0,

  /* Mask Parameter: daxisPIz_InitialConditionForInt
   * Referenced by: '<S88>/Integrator'
   */
  0.0,

  /* Mask Parameter: daxisCurrentPIz_InitialConditio
   * Referenced by: '<S38>/Integrator'
   */
  0.0,

  /* Mask Parameter: qaxisPIz_InitialConditionForInt
   * Referenced by: '<S188>/Integrator'
   */
  0.0,

  /* Mask Parameter: qaxisCurrentPIz_InitialConditio
   * Referenced by: '<S138>/Integrator'
   */
  0.0,

  /* Mask Parameter: qaxisCurrentPIz_Kb
   * Referenced by: '<S130>/Kb'
   */
  1.0,

  /* Mask Parameter: qaxisPIz_Kb
   * Referenced by: '<S180>/Kb'
   */
  1.0,

  /* Mask Parameter: daxisCurrentPIz_Kb
   * Referenced by: '<S30>/Kb'
   */
  1.0,

  /* Mask Parameter: daxisPIz_Kb
   * Referenced by: '<S80>/Kb'
   */
  1.0,

  /* Expression: [Ts/(Vdq_lpf_tau+Ts)]
   * Referenced by: '<Root>/Vd LPF'
   */
  0.2,

  /* Expression: [1 -Vdq_lpf_tau/(Vdq_lpf_tau+Ts)]
   * Referenced by: '<Root>/Vd LPF'
   */
  { 1.0, -0.8 },

  /* Expression: 0
   * Referenced by: '<Root>/Vd LPF'
   */
  0.0,

  /* Computed Parameter: Integrator_gainval
   * Referenced by: '<S88>/Integrator'
   */
  5.0E-5,

  /* Expression: [Ts/(Idq_lpf_tau+Ts)]
   * Referenced by: '<Root>/Id LPF'
   */
  0.625,

  /* Expression: [1 -Idq_lpf_tau/(Idq_lpf_tau+Ts)]
   * Referenced by: '<Root>/Id LPF'
   */
  { 1.0, -0.375 },

  /* Expression: 0
   * Referenced by: '<Root>/Id LPF'
   */
  0.0,

  /* Computed Parameter: Integrator_gainval_a
   * Referenced by: '<S38>/Integrator'
   */
  5.0E-5,

  /* Expression: 0
   * Referenced by: '<Root>/Vq reference'
   */
  0.0,

  /* Expression: [Ts/(Vdq_lpf_tau+Ts)]
   * Referenced by: '<Root>/Vq LPF'
   */
  0.2,

  /* Expression: [1 -Vdq_lpf_tau/(Vdq_lpf_tau+Ts)]
   * Referenced by: '<Root>/Vq LPF'
   */
  { 1.0, -0.8 },

  /* Expression: 0
   * Referenced by: '<Root>/Vq LPF'
   */
  0.0,

  /* Computed Parameter: Integrator_gainval_p
   * Referenced by: '<S188>/Integrator'
   */
  5.0E-5,

  /* Expression: [Ts/(Idq_lpf_tau+Ts)]
   * Referenced by: '<Root>/Iq LPF'
   */
  0.625,

  /* Expression: [1 -Idq_lpf_tau/(Idq_lpf_tau+Ts)]
   * Referenced by: '<Root>/Iq LPF'
   */
  { 1.0, -0.375 },

  /* Expression: 0
   * Referenced by: '<Root>/Iq LPF'
   */
  0.0,

  /* Computed Parameter: Integrator_gainval_d
   * Referenced by: '<S138>/Integrator'
   */
  5.0E-5,

  /* Expression: 3.3/4095
   * Referenced by: '<Root>/Gain11'
   */
  0.00080586080586080586,

  /* Expression: -1.643
   * Referenced by: '<Root>/Constant5'
   */
  -1.643,

  /* Expression: 109.781873
   * Referenced by: '<Root>/Gain6'
   */
  109.781873,

  /* Expression: [Cfilter*(2*pi*Fad)*(2/Ts)/((2/Ts)+(2*pi*Fad)),-Cfilter*(2*pi*Fad)*(2/Ts)/((2/Ts)+(2*pi*Fad))]
   * Referenced by: '<Root>/Capacitor current estimator'
   */
  { 0.1096232909449199, -0.1096232909449199 },

  /* Expression: [1,((2*pi*Fad)-(2/Ts))/((2/Ts)+(2*pi*Fad))]
   * Referenced by: '<Root>/Capacitor current estimator'
   */
  { 1.0, -0.12019830702311476 },

  /* Expression: 0
   * Referenced by: '<Root>/Capacitor current estimator'
   */
  0.0,

  /* Expression: [(2/Ts)/((2/Ts)+(2*pi*FadHP)),-(2/Ts)/((2/Ts)+(2*pi*FadHP))]
   * Referenced by: '<Root>/Damping high-pass'
   */
  { 0.9695409720485787, -0.9695409720485787 },

  /* Expression: [1,((2*pi*FadHP)-(2/Ts))/((2/Ts)+(2*pi*FadHP))]
   * Referenced by: '<Root>/Damping high-pass'
   */
  { 1.0, -0.93908194409715751 },

  /* Expression: 0
   * Referenced by: '<Root>/Damping high-pass'
   */
  0.0,

  /* Expression: -1
   * Referenced by: '<Root>/-m (Leg B)'
   */
  -1.0,

  /* Expression: 3.3/4095
   * Referenced by: '<Root>/Gain1'
   */
  0.00080586080586080586,

  /* Expression: -1.643
   * Referenced by: '<Root>/Constant1'
   */
  -1.643,

  /* Expression: 3.678481
   * Referenced by: '<Root>/Gain4'
   */
  3.678481,

  /* Expression: 0
   * Referenced by: '<Root>/Current 90 deg delay (Ts)'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<Root>/Voltage 90 deg delay (Ts)'
   */
  0.0
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
