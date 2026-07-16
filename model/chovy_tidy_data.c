/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: chovy_tidy_data.c
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

/* Block parameters (default storage) */
P_chovy_tidy_T chovy_tidy_P = {
  /* Mask Parameter: dq0toAlphaBetaZero_Alignment
   * Referenced by: '<S9>/Constant'
   */
  2.0,

  /* Mask Parameter: AlphaBetaZerotodq0_Alignment
   * Referenced by: '<S216>/Constant'
   */
  2.0,

  /* Mask Parameter: AlphaBetaZerotodq0_Alignment_a
   * Referenced by: '<S210>/Constant'
   */
  2.0,

  /* Mask Parameter: DiscretePIDController1_I
   * Referenced by: '<S90>/Integral Gain'
   */
  20.0,

  /* Mask Parameter: DiscretePIDController_I
   * Referenced by: '<S40>/Integral Gain'
   */
  20.0,

  /* Mask Parameter: DiscretePIDController_InitialCo
   * Referenced by: '<S43>/Integrator'
   */
  0.0,

  /* Mask Parameter: DiscretePIDController1_InitialC
   * Referenced by: '<S93>/Integrator'
   */
  0.0,

  /* Mask Parameter: DiscretePIDController_P
   * Referenced by: '<S48>/Proportional Gain'
   */
  0.08,

  /* Mask Parameter: DiscretePIDController1_P
   * Referenced by: '<S98>/Proportional Gain'
   */
  0.08,

  /* Mask Parameter: CompareToConstant_const
   * Referenced by: '<S222>/Constant'
   */
  1.0,

  /* Mask Parameter: CompareToConstant1_const
   * Referenced by: '<S223>/Constant'
   */
  2.0,

  /* Mask Parameter: CompareToConstant_const_o
   * Referenced by: '<S218>/Constant'
   */
  1.0,

  /* Mask Parameter: CompareToConstant1_const_c
   * Referenced by: '<S219>/Constant'
   */
  2.0,

  /* Mask Parameter: CompareToConstant1_const_f
   * Referenced by: '<S213>/Constant'
   */
  2.0,

  /* Mask Parameter: CompareToConstant_const_oy
   * Referenced by: '<S212>/Constant'
   */
  1.0,

  /* Expression: [0,0]
   * Referenced by: '<S224>/alpha_beta'
   */
  { 0.0, 0.0 },

  /* Expression: [0,0]
   * Referenced by: '<S225>/alpha_beta'
   */
  { 0.0, 0.0 },

  /* Expression: 0.0062634873752217546
   * Referenced by: '<Root>/Filtered load current magnitude'
   */
  0.0062634873752217546,

  /* Expression: [1 -0.99373651262477825]
   * Referenced by: '<Root>/Filtered load current magnitude'
   */
  { 1.0, -0.99373651262477825 },

  /* Expression: 0
   * Referenced by: '<Root>/Filtered load current magnitude'
   */
  0.0,

  /* Expression: 3.3/4095
   * Referenced by: '<Root>/Gain18'
   */
  0.00080586080586080586,

  /* Expression: -0.00138028
   * Referenced by: '<Root>/Constant10'
   */
  -0.00138028,

  /* Expression: 1/0.04828277
   * Referenced by: '<Root>/Gain17'
   */
  20.71132207203522,

  /* Expression: [0.048989794855663557 0.024494897427831779 0;0.048989794855663557 0.024494897427831779 0;0.032659863237109045 0.016329931618554522 0;0 0 0;-0.05715476066494083 -0.05715476066494083 -0.05715476066494083]
   * Referenced by: '<Root>/Load voltage compensation'
   */
  { 0.048989794855663557, 0.048989794855663557, 0.032659863237109045, 0.0,
    -0.05715476066494083, 0.024494897427831779, 0.024494897427831779,
    0.016329931618554522, 0.0, -0.05715476066494083, 0.0, 0.0, 0.0, 0.0,
    -0.05715476066494083 },

  /* Expression: [0.19 0.20499999999999999 0.215 0.29999999999999999 3]
   * Referenced by: '<Root>/Load voltage compensation'
   */
  { 0.19, 0.205, 0.215, 0.3, 3.0 },

  /* Expression: [55 67.5 80]
   * Referenced by: '<Root>/Load voltage compensation'
   */
  { 55.0, 67.5, 80.0 },

  /* Expression: 0.009380503941072593
   * Referenced by: '<Root>/Filtered voltage d'
   */
  0.0093805039410725932,

  /* Expression: [1 -0.9906194960589274]
   * Referenced by: '<Root>/Filtered voltage d'
   */
  { 1.0, -0.99061949605892741 },

  /* Expression: 0
   * Referenced by: '<Root>/Filtered voltage d'
   */
  0.0,

  /* Computed Parameter: Integrator_gainval
   * Referenced by: '<S43>/Integrator'
   */
  5.0E-5,

  /* Expression: 0
   * Referenced by: '<Root>/Constant1'
   */
  0.0,

  /* Expression: 0.009380503941072593
   * Referenced by: '<Root>/Filtered voltage q'
   */
  0.0093805039410725932,

  /* Expression: [1 -0.9906194960589274]
   * Referenced by: '<Root>/Filtered voltage q'
   */
  { 1.0, -0.99061949605892741 },

  /* Expression: 0
   * Referenced by: '<Root>/Filtered voltage q'
   */
  0.0,

  /* Computed Parameter: Integrator_gainval_m
   * Referenced by: '<S93>/Integrator'
   */
  5.0E-5,

  /* Expression: 314.16
   * Referenced by: '<Root>/Gain'
   */
  314.16,

  /* Expression: sqrt(3)
   * Referenced by: '<Root>/Gain3'
   */
  1.7320508075688772,

  /* Expression: inf
   * Referenced by: '<Root>/Safe Vdc denominator'
   */
  0.0,

  /* Expression: 1.0
   * Referenced by: '<Root>/Safe Vdc denominator'
   */
  1.0,

  /* Expression: sqrt(3)
   * Referenced by: '<Root>/Gain4'
   */
  1.7320508075688772,

  /* Expression: 0.98
   * Referenced by: '<Root>/Constant4'
   */
  0.98,

  /* Expression: -1.682
   * Referenced by: '<Root>/Constant9'
   */
  -1.682,

  /* Expression: 3.3/4095
   * Referenced by: '<Root>/Gain13'
   */
  0.00080586080586080586,

  /* Expression: 1/0.2926
   * Referenced by: '<Root>/Gain14'
   */
  3.4176349965823647,

  /* Expression: -1.699
   * Referenced by: '<Root>/Constant7'
   */
  -1.699,

  /* Expression: 3.3/4095
   * Referenced by: '<Root>/Gain16'
   */
  0.00080586080586080586,

  /* Expression: 1/0.287
   * Referenced by: '<Root>/Gain15'
   */
  3.4843205574912894,

  /* Expression: -1.684
   * Referenced by: '<Root>/Constant3'
   */
  -1.684,

  /* Expression: 3.3/4095
   * Referenced by: '<Root>/Gain8'
   */
  0.00080586080586080586,

  /* Expression: 1/0.2926
   * Referenced by: '<Root>/Gain5'
   */
  3.4176349965823647,

  /* Expression: [ 1   -1/2   -1/2; 0   sqrt(3)/2   -sqrt(3)/2; 1/2  1/2  1/2 ]
   * Referenced by: '<S217>/Gain3'
   */
  { 1.0, 0.0, 0.5, -0.5, 0.8660254037844386, 0.5, -0.5, -0.8660254037844386, 0.5
  },

  /* Expression: 2/3
   * Referenced by: '<S217>/Gain1'
   */
  0.66666666666666663,

  /* Expression: -1.686
   * Referenced by: '<Root>/Constant5'
   */
  -1.686,

  /* Expression: 3.3/4095
   * Referenced by: '<Root>/Gain11'
   */
  0.00080586080586080586,

  /* Expression: 1/0.019463
   * Referenced by: '<Root>/Gain6'
   */
  51.379540666906436,

  /* Expression: -1.6947
   * Referenced by: '<Root>/Constant6'
   */
  -1.6947,

  /* Expression: 3.3/4095
   * Referenced by: '<Root>/Gain12'
   */
  0.00080586080586080586,

  /* Expression: 1/0.0205
   * Referenced by: '<Root>/Gain7'
   */
  48.780487804878049,

  /* Expression: -1.6916
   * Referenced by: '<Root>/Constant8'
   */
  -1.6916,

  /* Expression: 3.3/4095
   * Referenced by: '<Root>/Gain10'
   */
  0.00080586080586080586,

  /* Expression: 1/0.01988
   * Referenced by: '<Root>/Gain9'
   */
  50.30181086519115,

  /* Expression: [ 1   -1/2   -1/2; 0   sqrt(3)/2   -sqrt(3)/2; 1/2  1/2  1/2 ]
   * Referenced by: '<S211>/Gain3'
   */
  { 1.0, 0.0, 0.5, -0.5, 0.8660254037844386, 0.5, -0.5, -0.8660254037844386, 0.5
  },

  /* Expression: 2/3
   * Referenced by: '<S211>/Gain1'
   */
  0.66666666666666663,

  /* Computed Parameter: Loadvoltagecompensation_maxInde
   * Referenced by: '<Root>/Load voltage compensation'
   */
  { 4U, 2U },

  /* Start of '<S216>/Subsystem1' */
  {
    /* Expression: [0,0]
     * Referenced by: '<S221>/dq'
     */
    { 0.0, 0.0 }
  }
  ,

  /* End of '<S216>/Subsystem1' */

  /* Start of '<S216>/Subsystem - pi//2 delay' */
  {
    /* Expression: [0,0]
     * Referenced by: '<S220>/dq'
     */
    { 0.0, 0.0 }
  }
  ,

  /* End of '<S216>/Subsystem - pi//2 delay' */

  /* Start of '<S210>/Subsystem1' */
  {
    /* Expression: [0,0]
     * Referenced by: '<S215>/dq'
     */
    { 0.0, 0.0 }
  }
  ,

  /* End of '<S210>/Subsystem1' */

  /* Start of '<S210>/Subsystem - pi//2 delay' */
  {
    /* Expression: [0,0]
     * Referenced by: '<S214>/dq'
     */
    { 0.0, 0.0 }
  }
  /* End of '<S210>/Subsystem - pi//2 delay' */
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
