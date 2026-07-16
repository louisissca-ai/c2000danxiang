/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: chovy_tidy.h
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

#ifndef chovy_tidy_h_
#define chovy_tidy_h_
#ifndef chovy_tidy_COMMON_INCLUDES_
#define chovy_tidy_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "c2000BoardSupport.h"
#include "MW_f280015x_includes.h"
#include "IQmathLib.h"
#endif                                 /* chovy_tidy_COMMON_INCLUDES_ */

#include "chovy_tidy_types.h"
#include <string.h>
#include "rt_nonfinite.h"
#include <stddef.h>
#include "MW_target_hardware_resources.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmStepTask
#define rtmStepTask(rtm, idx)          ((rtm)->Timing.TaskCounters.TID[(idx)] == 0)
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

#ifndef rtmTaskCounter
#define rtmTaskCounter(rtm, idx)       ((rtm)->Timing.TaskCounters.TID[(idx)])
#endif

extern void config_ePWMSyncSource(void);
extern void config_ePWM_GPIO (void);
extern void config_ePWM_TBSync (void);
extern void config_ePWM_XBAR(void);

/* user code (top of header file) */
#include "control_model_if.h"

/* Block states (default storage) for system '<S210>/Subsystem - pi//2 delay' */
typedef struct {
  boolean_T Subsystempi2delay_MODE;    /* '<S210>/Subsystem - pi//2 delay' */
} DW_Subsystempi2delay_chovy_ti_T;

/* Block states (default storage) for system '<S210>/Subsystem1' */
typedef struct {
  boolean_T Subsystem1_MODE;           /* '<S210>/Subsystem1' */
} DW_Subsystem1_chovy_tidy_T;

/* Block signals (default storage) */
typedef struct {
  real_T ControlModelVrefRamp;         /* '<Root>/ControlModel Vref Ramp' */
  real_T Switch[2];                    /* '<S210>/Switch' */
  real_T Loadcurrentmagnitude;         /* '<Root>/Load current magnitude' */
  real_T Fcn;                          /* '<S225>/Fcn' */
  real_T Fcn1;                         /* '<S225>/Fcn1' */
  real_T Fcn_n;                        /* '<S224>/Fcn' */
  real_T Fcn1_b;                       /* '<S224>/Fcn1' */
  real_T Fcn_m;                        /* '<S221>/Fcn' */
  real_T Fcn1_p;                       /* '<S221>/Fcn1' */
  real_T Fcn_nz;                       /* '<S220>/Fcn' */
  real_T Fcn1_b2;                      /* '<S220>/Fcn1' */
  real_T Fcn_b;                        /* '<S215>/Fcn' */
  real_T Fcn1_k;                       /* '<S215>/Fcn1' */
  real_T Fcn_l;                        /* '<S214>/Fcn' */
  real_T Fcn1_c;                       /* '<S214>/Fcn1' */
  uint16_T ADC8;                       /* '<Root>/ADC8' */
  uint16_T ControlModelSafety50us;     /* '<Root>/ControlModel Safety 50us' */
  uint16_T ADC1;                       /* '<Root>/ADC1' */
  uint16_T ADC2;                       /* '<Root>/ADC2' */
  uint16_T ADC3;                       /* '<Root>/ADC3' */
  uint16_T ADC4;                       /* '<Root>/ADC4' */
  uint16_T ADC5;                       /* '<Root>/ADC5' */
  uint16_T ADC6;                       /* '<Root>/ADC6' */
} B_chovy_tidy_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T Filteredloadcurrentmagnitude_st;
                                  /* '<Root>/Filtered load current magnitude' */
  real_T Filteredvoltaged_states;      /* '<Root>/Filtered voltage d' */
  real_T Integrator_DSTATE;            /* '<S43>/Integrator' */
  real_T Filteredvoltageq_states;      /* '<Root>/Filtered voltage q' */
  real_T Integrator_DSTATE_b;          /* '<S93>/Integrator' */
  real_T Feedbackduty1ms_Buffer;       /* '<Root>/Feedback duty 1ms' */
  real_T Feedbackvout1ms_Buffer;       /* '<Root>/Feedback vout 1ms' */
  real_T Feedbackvin1ms_Buffer;        /* '<Root>/Feedback vin 1ms' */
  real_T Feedbackiout1ms_Buffer;       /* '<Root>/Feedback iout 1ms' */
  int16_T Integrator_PrevResetState;   /* '<S43>/Integrator' */
  int16_T Integrator_PrevResetState_o; /* '<S93>/Integrator' */
  boolean_T Subsystem1_MODE;           /* '<S9>/Subsystem1' */
  boolean_T Subsystempi2delay_MODE;    /* '<S9>/Subsystem - pi//2 delay' */
  DW_Subsystem1_chovy_tidy_T Subsystem1_a;/* '<S216>/Subsystem1' */
  DW_Subsystempi2delay_chovy_ti_T Subsystempi2delay_n;/* '<S216>/Subsystem - pi//2 delay' */
  DW_Subsystem1_chovy_tidy_T Subsystem1;/* '<S210>/Subsystem1' */
  DW_Subsystempi2delay_chovy_ti_T Subsystempi2delay;/* '<S210>/Subsystem - pi//2 delay' */
} DW_chovy_tidy_T;

/* Parameters for system: '<S210>/Subsystem - pi//2 delay' */
struct P_Subsystempi2delay_chovy_tid_T_ {
  real_T dq_Y0[2];                     /* Expression: [0,0]
                                        * Referenced by: '<S214>/dq'
                                        */
};

/* Parameters for system: '<S210>/Subsystem1' */
struct P_Subsystem1_chovy_tidy_T_ {
  real_T dq_Y0[2];                     /* Expression: [0,0]
                                        * Referenced by: '<S215>/dq'
                                        */
};

/* Parameters (default storage) */
struct P_chovy_tidy_T_ {
  real_T dq0toAlphaBetaZero_Alignment;
                                 /* Mask Parameter: dq0toAlphaBetaZero_Alignment
                                  * Referenced by: '<S9>/Constant'
                                  */
  real_T AlphaBetaZerotodq0_Alignment;
                                 /* Mask Parameter: AlphaBetaZerotodq0_Alignment
                                  * Referenced by: '<S216>/Constant'
                                  */
  real_T AlphaBetaZerotodq0_Alignment_a;
                               /* Mask Parameter: AlphaBetaZerotodq0_Alignment_a
                                * Referenced by: '<S210>/Constant'
                                */
  real_T DiscretePIDController1_I;   /* Mask Parameter: DiscretePIDController1_I
                                      * Referenced by: '<S90>/Integral Gain'
                                      */
  real_T DiscretePIDController_I;     /* Mask Parameter: DiscretePIDController_I
                                       * Referenced by: '<S40>/Integral Gain'
                                       */
  real_T DiscretePIDController_InitialCo;
                              /* Mask Parameter: DiscretePIDController_InitialCo
                               * Referenced by: '<S43>/Integrator'
                               */
  real_T DiscretePIDController1_InitialC;
                              /* Mask Parameter: DiscretePIDController1_InitialC
                               * Referenced by: '<S93>/Integrator'
                               */
  real_T DiscretePIDController_P;     /* Mask Parameter: DiscretePIDController_P
                                       * Referenced by: '<S48>/Proportional Gain'
                                       */
  real_T DiscretePIDController1_P;   /* Mask Parameter: DiscretePIDController1_P
                                      * Referenced by: '<S98>/Proportional Gain'
                                      */
  real_T CompareToConstant_const;     /* Mask Parameter: CompareToConstant_const
                                       * Referenced by: '<S222>/Constant'
                                       */
  real_T CompareToConstant1_const;   /* Mask Parameter: CompareToConstant1_const
                                      * Referenced by: '<S223>/Constant'
                                      */
  real_T CompareToConstant_const_o; /* Mask Parameter: CompareToConstant_const_o
                                     * Referenced by: '<S218>/Constant'
                                     */
  real_T CompareToConstant1_const_c;
                                   /* Mask Parameter: CompareToConstant1_const_c
                                    * Referenced by: '<S219>/Constant'
                                    */
  real_T CompareToConstant1_const_f;
                                   /* Mask Parameter: CompareToConstant1_const_f
                                    * Referenced by: '<S213>/Constant'
                                    */
  real_T CompareToConstant_const_oy;
                                   /* Mask Parameter: CompareToConstant_const_oy
                                    * Referenced by: '<S212>/Constant'
                                    */
  real_T alpha_beta_Y0[2];             /* Expression: [0,0]
                                        * Referenced by: '<S224>/alpha_beta'
                                        */
  real_T alpha_beta_Y0_e[2];           /* Expression: [0,0]
                                        * Referenced by: '<S225>/alpha_beta'
                                        */
  real_T Filteredloadcurrentmagnitude_Nu;/* Expression: 0.0062634873752217546
                                          * Referenced by: '<Root>/Filtered load current magnitude'
                                          */
  real_T Filteredloadcurrentmagnitude_De[2];/* Expression: [1 -0.99373651262477825]
                                             * Referenced by: '<Root>/Filtered load current magnitude'
                                             */
  real_T Filteredloadcurrentmagnitude_In;/* Expression: 0
                                          * Referenced by: '<Root>/Filtered load current magnitude'
                                          */
  real_T Gain18_Gain;                  /* Expression: 3.3/4095
                                        * Referenced by: '<Root>/Gain18'
                                        */
  real_T Constant10_Value;             /* Expression: -0.00138028
                                        * Referenced by: '<Root>/Constant10'
                                        */
  real_T Gain17_Gain;                  /* Expression: 1/0.04828277
                                        * Referenced by: '<Root>/Gain17'
                                        */
  real_T Loadvoltagecompensation_tableDa[15];
  /* Expression: [0.048989794855663557 0.024494897427831779 0;0.048989794855663557 0.024494897427831779 0;0.032659863237109045 0.016329931618554522 0;0 0 0;-0.05715476066494083 -0.05715476066494083 -0.05715476066494083]
   * Referenced by: '<Root>/Load voltage compensation'
   */
  real_T Loadvoltagecompensation_bp01Dat[5];
           /* Expression: [0.19 0.20499999999999999 0.215 0.29999999999999999 3]
            * Referenced by: '<Root>/Load voltage compensation'
            */
  real_T Loadvoltagecompensation_bp02Dat[3];/* Expression: [55 67.5 80]
                                             * Referenced by: '<Root>/Load voltage compensation'
                                             */
  real_T Filteredvoltaged_NumCoef;     /* Expression: 0.009380503941072593
                                        * Referenced by: '<Root>/Filtered voltage d'
                                        */
  real_T Filteredvoltaged_DenCoef[2];  /* Expression: [1 -0.9906194960589274]
                                        * Referenced by: '<Root>/Filtered voltage d'
                                        */
  real_T Filteredvoltaged_InitialStates;/* Expression: 0
                                         * Referenced by: '<Root>/Filtered voltage d'
                                         */
  real_T Integrator_gainval;           /* Computed Parameter: Integrator_gainval
                                        * Referenced by: '<S43>/Integrator'
                                        */
  real_T Constant1_Value;              /* Expression: 0
                                        * Referenced by: '<Root>/Constant1'
                                        */
  real_T Filteredvoltageq_NumCoef;     /* Expression: 0.009380503941072593
                                        * Referenced by: '<Root>/Filtered voltage q'
                                        */
  real_T Filteredvoltageq_DenCoef[2];  /* Expression: [1 -0.9906194960589274]
                                        * Referenced by: '<Root>/Filtered voltage q'
                                        */
  real_T Filteredvoltageq_InitialStates;/* Expression: 0
                                         * Referenced by: '<Root>/Filtered voltage q'
                                         */
  real_T Integrator_gainval_m;       /* Computed Parameter: Integrator_gainval_m
                                      * Referenced by: '<S93>/Integrator'
                                      */
  real_T Gain_Gain;                    /* Expression: 314.16
                                        * Referenced by: '<Root>/Gain'
                                        */
  real_T Gain3_Gain;                   /* Expression: sqrt(3)
                                        * Referenced by: '<Root>/Gain3'
                                        */
  real_T SafeVdcdenominator_UpperSat;  /* Expression: inf
                                        * Referenced by: '<Root>/Safe Vdc denominator'
                                        */
  real_T SafeVdcdenominator_LowerSat;  /* Expression: 1.0
                                        * Referenced by: '<Root>/Safe Vdc denominator'
                                        */
  real_T Gain4_Gain;                   /* Expression: sqrt(3)
                                        * Referenced by: '<Root>/Gain4'
                                        */
  real_T Constant4_Value;              /* Expression: 0.98
                                        * Referenced by: '<Root>/Constant4'
                                        */
  real_T Constant9_Value;              /* Expression: -1.682
                                        * Referenced by: '<Root>/Constant9'
                                        */
  real_T Gain13_Gain;                  /* Expression: 3.3/4095
                                        * Referenced by: '<Root>/Gain13'
                                        */
  real_T Gain14_Gain;                  /* Expression: 1/0.2926
                                        * Referenced by: '<Root>/Gain14'
                                        */
  real_T Constant7_Value;              /* Expression: -1.699
                                        * Referenced by: '<Root>/Constant7'
                                        */
  real_T Gain16_Gain;                  /* Expression: 3.3/4095
                                        * Referenced by: '<Root>/Gain16'
                                        */
  real_T Gain15_Gain;                  /* Expression: 1/0.287
                                        * Referenced by: '<Root>/Gain15'
                                        */
  real_T Constant3_Value;              /* Expression: -1.684
                                        * Referenced by: '<Root>/Constant3'
                                        */
  real_T Gain8_Gain;                   /* Expression: 3.3/4095
                                        * Referenced by: '<Root>/Gain8'
                                        */
  real_T Gain5_Gain;                   /* Expression: 1/0.2926
                                        * Referenced by: '<Root>/Gain5'
                                        */
  real_T Gain3_Gain_f[9];
  /* Expression: [ 1   -1/2   -1/2; 0   sqrt(3)/2   -sqrt(3)/2; 1/2  1/2  1/2 ]
   * Referenced by: '<S217>/Gain3'
   */
  real_T Gain1_Gain;                   /* Expression: 2/3
                                        * Referenced by: '<S217>/Gain1'
                                        */
  real_T Constant5_Value;              /* Expression: -1.686
                                        * Referenced by: '<Root>/Constant5'
                                        */
  real_T Gain11_Gain;                  /* Expression: 3.3/4095
                                        * Referenced by: '<Root>/Gain11'
                                        */
  real_T Gain6_Gain;                   /* Expression: 1/0.019463
                                        * Referenced by: '<Root>/Gain6'
                                        */
  real_T Constant6_Value;              /* Expression: -1.6947
                                        * Referenced by: '<Root>/Constant6'
                                        */
  real_T Gain12_Gain;                  /* Expression: 3.3/4095
                                        * Referenced by: '<Root>/Gain12'
                                        */
  real_T Gain7_Gain;                   /* Expression: 1/0.0205
                                        * Referenced by: '<Root>/Gain7'
                                        */
  real_T Constant8_Value;              /* Expression: -1.6916
                                        * Referenced by: '<Root>/Constant8'
                                        */
  real_T Gain10_Gain;                  /* Expression: 3.3/4095
                                        * Referenced by: '<Root>/Gain10'
                                        */
  real_T Gain9_Gain;                   /* Expression: 1/0.01988
                                        * Referenced by: '<Root>/Gain9'
                                        */
  real_T Gain3_Gain_c[9];
  /* Expression: [ 1   -1/2   -1/2; 0   sqrt(3)/2   -sqrt(3)/2; 1/2  1/2  1/2 ]
   * Referenced by: '<S211>/Gain3'
   */
  real_T Gain1_Gain_o;                 /* Expression: 2/3
                                        * Referenced by: '<S211>/Gain1'
                                        */
  uint32_T Loadvoltagecompensation_maxInde[2];
                          /* Computed Parameter: Loadvoltagecompensation_maxInde
                           * Referenced by: '<Root>/Load voltage compensation'
                           */
  P_Subsystem1_chovy_tidy_T Subsystem1_a;/* '<S216>/Subsystem1' */
  P_Subsystempi2delay_chovy_tid_T Subsystempi2delay_n;/* '<S216>/Subsystem - pi//2 delay' */
  P_Subsystem1_chovy_tidy_T Subsystem1;/* '<S210>/Subsystem1' */
  P_Subsystempi2delay_chovy_tid_T Subsystempi2delay;/* '<S210>/Subsystem - pi//2 delay' */
};

/* Real-time Model Data Structure */
struct tag_RTM_chovy_tidy_T {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    time_T stepSize0;
    uint32_T clockTick1;
    struct {
      uint16_T TID[3];
    } TaskCounters;

    struct {
      boolean_T TID1_2;
    } RateInteraction;

    SimTimeStep simTimeStep;
    time_T *t;
    time_T tArray[3];
  } Timing;
};

/* Block parameters (default storage) */
extern P_chovy_tidy_T chovy_tidy_P;

/* Block signals (default storage) */
extern B_chovy_tidy_T chovy_tidy_B;

/* Block states (default storage) */
extern DW_chovy_tidy_T chovy_tidy_DW;

/* External function called from main */
extern void chovy_tidy_SetEventsForThisBaseStep(boolean_T *eventFlags);

/* Model entry point functions */
extern void chovy_tidy_initialize(void);
extern void chovy_tidy_step0(void);
extern void chovy_tidy_step2(void);
extern void chovy_tidy_terminate(void);

/* Real-time Model object */
extern RT_MODEL_chovy_tidy_T *const chovy_tidy_M;
extern volatile boolean_T stopRequested;
extern volatile boolean_T runModel;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<Root>/Constant' : Unused code path elimination
 * Block '<Root>/Constant2' : Unused code path elimination
 * Block '<S140>/Integral Gain' : Unused code path elimination
 * Block '<S143>/Integrator' : Unused code path elimination
 * Block '<S148>/Proportional Gain' : Unused code path elimination
 * Block '<S152>/Sum' : Unused code path elimination
 * Block '<S190>/Integral Gain' : Unused code path elimination
 * Block '<S193>/Integrator' : Unused code path elimination
 * Block '<S198>/Proportional Gain' : Unused code path elimination
 * Block '<S202>/Sum' : Unused code path elimination
 * Block '<Root>/Gain1' : Unused code path elimination
 * Block '<Root>/Gain2' : Unused code path elimination
 * Block '<Root>/Saturation' : Unused code path elimination
 * Block '<Root>/Saturation1' : Unused code path elimination
 * Block '<Root>/Scope1' : Unused code path elimination
 * Block '<Root>/Scope2' : Unused code path elimination
 * Block '<Root>/Sum2' : Unused code path elimination
 * Block '<Root>/Sum3' : Unused code path elimination
 * Block '<Root>/Sum4' : Unused code path elimination
 * Block '<Root>/Sum5' : Unused code path elimination
 * Block '<Root>/ToWs_Vabc_7sample_ts' : Unused code path elimination
 * Block '<Root>/ToWs_Vdc_7sample_ts' : Unused code path elimination
 * Block '<Root>/Sample Iabc 20kHz' : Eliminated since input and output rates are identical
 * Block '<Root>/Sample Vabc 20kHz' : Eliminated since input and output rates are identical
 * Block '<Root>/Sample Vdc 20kHz' : Eliminated since input and output rates are identical
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'chovy_tidy'
 * '<S1>'   : 'chovy_tidy/Discrete PID Controller'
 * '<S2>'   : 'chovy_tidy/Discrete PID Controller1'
 * '<S3>'   : 'chovy_tidy/Discrete PID Controller2'
 * '<S4>'   : 'chovy_tidy/Discrete PID Controller3'
 * '<S5>'   : 'chovy_tidy/MATLAB Function'
 * '<S6>'   : 'chovy_tidy/Unipolar SPWM Duty (0-100%)'
 * '<S7>'   : 'chovy_tidy/abc to dq0'
 * '<S8>'   : 'chovy_tidy/abc to dq1'
 * '<S9>'   : 'chovy_tidy/dq0 to Alpha-Beta-Zero'
 * '<S10>'  : 'chovy_tidy/Discrete PID Controller/Anti-windup'
 * '<S11>'  : 'chovy_tidy/Discrete PID Controller/D Gain'
 * '<S12>'  : 'chovy_tidy/Discrete PID Controller/External Derivative'
 * '<S13>'  : 'chovy_tidy/Discrete PID Controller/Filter'
 * '<S14>'  : 'chovy_tidy/Discrete PID Controller/Filter ICs'
 * '<S15>'  : 'chovy_tidy/Discrete PID Controller/I Gain'
 * '<S16>'  : 'chovy_tidy/Discrete PID Controller/Ideal P Gain'
 * '<S17>'  : 'chovy_tidy/Discrete PID Controller/Ideal P Gain Fdbk'
 * '<S18>'  : 'chovy_tidy/Discrete PID Controller/Integrator'
 * '<S19>'  : 'chovy_tidy/Discrete PID Controller/Integrator ICs'
 * '<S20>'  : 'chovy_tidy/Discrete PID Controller/N Copy'
 * '<S21>'  : 'chovy_tidy/Discrete PID Controller/N Gain'
 * '<S22>'  : 'chovy_tidy/Discrete PID Controller/P Copy'
 * '<S23>'  : 'chovy_tidy/Discrete PID Controller/Parallel P Gain'
 * '<S24>'  : 'chovy_tidy/Discrete PID Controller/Reset Signal'
 * '<S25>'  : 'chovy_tidy/Discrete PID Controller/Saturation'
 * '<S26>'  : 'chovy_tidy/Discrete PID Controller/Saturation Fdbk'
 * '<S27>'  : 'chovy_tidy/Discrete PID Controller/Sum'
 * '<S28>'  : 'chovy_tidy/Discrete PID Controller/Sum Fdbk'
 * '<S29>'  : 'chovy_tidy/Discrete PID Controller/Tracking Mode'
 * '<S30>'  : 'chovy_tidy/Discrete PID Controller/Tracking Mode Sum'
 * '<S31>'  : 'chovy_tidy/Discrete PID Controller/Tsamp - Integral'
 * '<S32>'  : 'chovy_tidy/Discrete PID Controller/Tsamp - Ngain'
 * '<S33>'  : 'chovy_tidy/Discrete PID Controller/postSat Signal'
 * '<S34>'  : 'chovy_tidy/Discrete PID Controller/preSat Signal'
 * '<S35>'  : 'chovy_tidy/Discrete PID Controller/Anti-windup/Passthrough'
 * '<S36>'  : 'chovy_tidy/Discrete PID Controller/D Gain/Disabled'
 * '<S37>'  : 'chovy_tidy/Discrete PID Controller/External Derivative/Disabled'
 * '<S38>'  : 'chovy_tidy/Discrete PID Controller/Filter/Disabled'
 * '<S39>'  : 'chovy_tidy/Discrete PID Controller/Filter ICs/Disabled'
 * '<S40>'  : 'chovy_tidy/Discrete PID Controller/I Gain/Internal Parameters'
 * '<S41>'  : 'chovy_tidy/Discrete PID Controller/Ideal P Gain/Passthrough'
 * '<S42>'  : 'chovy_tidy/Discrete PID Controller/Ideal P Gain Fdbk/Disabled'
 * '<S43>'  : 'chovy_tidy/Discrete PID Controller/Integrator/Discrete'
 * '<S44>'  : 'chovy_tidy/Discrete PID Controller/Integrator ICs/Internal IC'
 * '<S45>'  : 'chovy_tidy/Discrete PID Controller/N Copy/Disabled wSignal Specification'
 * '<S46>'  : 'chovy_tidy/Discrete PID Controller/N Gain/Disabled'
 * '<S47>'  : 'chovy_tidy/Discrete PID Controller/P Copy/Disabled'
 * '<S48>'  : 'chovy_tidy/Discrete PID Controller/Parallel P Gain/Internal Parameters'
 * '<S49>'  : 'chovy_tidy/Discrete PID Controller/Reset Signal/External Reset'
 * '<S50>'  : 'chovy_tidy/Discrete PID Controller/Saturation/Passthrough'
 * '<S51>'  : 'chovy_tidy/Discrete PID Controller/Saturation Fdbk/Disabled'
 * '<S52>'  : 'chovy_tidy/Discrete PID Controller/Sum/Sum_PI'
 * '<S53>'  : 'chovy_tidy/Discrete PID Controller/Sum Fdbk/Disabled'
 * '<S54>'  : 'chovy_tidy/Discrete PID Controller/Tracking Mode/Disabled'
 * '<S55>'  : 'chovy_tidy/Discrete PID Controller/Tracking Mode Sum/Passthrough'
 * '<S56>'  : 'chovy_tidy/Discrete PID Controller/Tsamp - Integral/TsSignalSpecification'
 * '<S57>'  : 'chovy_tidy/Discrete PID Controller/Tsamp - Ngain/Passthrough'
 * '<S58>'  : 'chovy_tidy/Discrete PID Controller/postSat Signal/Forward_Path'
 * '<S59>'  : 'chovy_tidy/Discrete PID Controller/preSat Signal/Forward_Path'
 * '<S60>'  : 'chovy_tidy/Discrete PID Controller1/Anti-windup'
 * '<S61>'  : 'chovy_tidy/Discrete PID Controller1/D Gain'
 * '<S62>'  : 'chovy_tidy/Discrete PID Controller1/External Derivative'
 * '<S63>'  : 'chovy_tidy/Discrete PID Controller1/Filter'
 * '<S64>'  : 'chovy_tidy/Discrete PID Controller1/Filter ICs'
 * '<S65>'  : 'chovy_tidy/Discrete PID Controller1/I Gain'
 * '<S66>'  : 'chovy_tidy/Discrete PID Controller1/Ideal P Gain'
 * '<S67>'  : 'chovy_tidy/Discrete PID Controller1/Ideal P Gain Fdbk'
 * '<S68>'  : 'chovy_tidy/Discrete PID Controller1/Integrator'
 * '<S69>'  : 'chovy_tidy/Discrete PID Controller1/Integrator ICs'
 * '<S70>'  : 'chovy_tidy/Discrete PID Controller1/N Copy'
 * '<S71>'  : 'chovy_tidy/Discrete PID Controller1/N Gain'
 * '<S72>'  : 'chovy_tidy/Discrete PID Controller1/P Copy'
 * '<S73>'  : 'chovy_tidy/Discrete PID Controller1/Parallel P Gain'
 * '<S74>'  : 'chovy_tidy/Discrete PID Controller1/Reset Signal'
 * '<S75>'  : 'chovy_tidy/Discrete PID Controller1/Saturation'
 * '<S76>'  : 'chovy_tidy/Discrete PID Controller1/Saturation Fdbk'
 * '<S77>'  : 'chovy_tidy/Discrete PID Controller1/Sum'
 * '<S78>'  : 'chovy_tidy/Discrete PID Controller1/Sum Fdbk'
 * '<S79>'  : 'chovy_tidy/Discrete PID Controller1/Tracking Mode'
 * '<S80>'  : 'chovy_tidy/Discrete PID Controller1/Tracking Mode Sum'
 * '<S81>'  : 'chovy_tidy/Discrete PID Controller1/Tsamp - Integral'
 * '<S82>'  : 'chovy_tidy/Discrete PID Controller1/Tsamp - Ngain'
 * '<S83>'  : 'chovy_tidy/Discrete PID Controller1/postSat Signal'
 * '<S84>'  : 'chovy_tidy/Discrete PID Controller1/preSat Signal'
 * '<S85>'  : 'chovy_tidy/Discrete PID Controller1/Anti-windup/Passthrough'
 * '<S86>'  : 'chovy_tidy/Discrete PID Controller1/D Gain/Disabled'
 * '<S87>'  : 'chovy_tidy/Discrete PID Controller1/External Derivative/Disabled'
 * '<S88>'  : 'chovy_tidy/Discrete PID Controller1/Filter/Disabled'
 * '<S89>'  : 'chovy_tidy/Discrete PID Controller1/Filter ICs/Disabled'
 * '<S90>'  : 'chovy_tidy/Discrete PID Controller1/I Gain/Internal Parameters'
 * '<S91>'  : 'chovy_tidy/Discrete PID Controller1/Ideal P Gain/Passthrough'
 * '<S92>'  : 'chovy_tidy/Discrete PID Controller1/Ideal P Gain Fdbk/Disabled'
 * '<S93>'  : 'chovy_tidy/Discrete PID Controller1/Integrator/Discrete'
 * '<S94>'  : 'chovy_tidy/Discrete PID Controller1/Integrator ICs/Internal IC'
 * '<S95>'  : 'chovy_tidy/Discrete PID Controller1/N Copy/Disabled wSignal Specification'
 * '<S96>'  : 'chovy_tidy/Discrete PID Controller1/N Gain/Disabled'
 * '<S97>'  : 'chovy_tidy/Discrete PID Controller1/P Copy/Disabled'
 * '<S98>'  : 'chovy_tidy/Discrete PID Controller1/Parallel P Gain/Internal Parameters'
 * '<S99>'  : 'chovy_tidy/Discrete PID Controller1/Reset Signal/External Reset'
 * '<S100>' : 'chovy_tidy/Discrete PID Controller1/Saturation/Passthrough'
 * '<S101>' : 'chovy_tidy/Discrete PID Controller1/Saturation Fdbk/Disabled'
 * '<S102>' : 'chovy_tidy/Discrete PID Controller1/Sum/Sum_PI'
 * '<S103>' : 'chovy_tidy/Discrete PID Controller1/Sum Fdbk/Disabled'
 * '<S104>' : 'chovy_tidy/Discrete PID Controller1/Tracking Mode/Disabled'
 * '<S105>' : 'chovy_tidy/Discrete PID Controller1/Tracking Mode Sum/Passthrough'
 * '<S106>' : 'chovy_tidy/Discrete PID Controller1/Tsamp - Integral/TsSignalSpecification'
 * '<S107>' : 'chovy_tidy/Discrete PID Controller1/Tsamp - Ngain/Passthrough'
 * '<S108>' : 'chovy_tidy/Discrete PID Controller1/postSat Signal/Forward_Path'
 * '<S109>' : 'chovy_tidy/Discrete PID Controller1/preSat Signal/Forward_Path'
 * '<S110>' : 'chovy_tidy/Discrete PID Controller2/Anti-windup'
 * '<S111>' : 'chovy_tidy/Discrete PID Controller2/D Gain'
 * '<S112>' : 'chovy_tidy/Discrete PID Controller2/External Derivative'
 * '<S113>' : 'chovy_tidy/Discrete PID Controller2/Filter'
 * '<S114>' : 'chovy_tidy/Discrete PID Controller2/Filter ICs'
 * '<S115>' : 'chovy_tidy/Discrete PID Controller2/I Gain'
 * '<S116>' : 'chovy_tidy/Discrete PID Controller2/Ideal P Gain'
 * '<S117>' : 'chovy_tidy/Discrete PID Controller2/Ideal P Gain Fdbk'
 * '<S118>' : 'chovy_tidy/Discrete PID Controller2/Integrator'
 * '<S119>' : 'chovy_tidy/Discrete PID Controller2/Integrator ICs'
 * '<S120>' : 'chovy_tidy/Discrete PID Controller2/N Copy'
 * '<S121>' : 'chovy_tidy/Discrete PID Controller2/N Gain'
 * '<S122>' : 'chovy_tidy/Discrete PID Controller2/P Copy'
 * '<S123>' : 'chovy_tidy/Discrete PID Controller2/Parallel P Gain'
 * '<S124>' : 'chovy_tidy/Discrete PID Controller2/Reset Signal'
 * '<S125>' : 'chovy_tidy/Discrete PID Controller2/Saturation'
 * '<S126>' : 'chovy_tidy/Discrete PID Controller2/Saturation Fdbk'
 * '<S127>' : 'chovy_tidy/Discrete PID Controller2/Sum'
 * '<S128>' : 'chovy_tidy/Discrete PID Controller2/Sum Fdbk'
 * '<S129>' : 'chovy_tidy/Discrete PID Controller2/Tracking Mode'
 * '<S130>' : 'chovy_tidy/Discrete PID Controller2/Tracking Mode Sum'
 * '<S131>' : 'chovy_tidy/Discrete PID Controller2/Tsamp - Integral'
 * '<S132>' : 'chovy_tidy/Discrete PID Controller2/Tsamp - Ngain'
 * '<S133>' : 'chovy_tidy/Discrete PID Controller2/postSat Signal'
 * '<S134>' : 'chovy_tidy/Discrete PID Controller2/preSat Signal'
 * '<S135>' : 'chovy_tidy/Discrete PID Controller2/Anti-windup/Passthrough'
 * '<S136>' : 'chovy_tidy/Discrete PID Controller2/D Gain/Disabled'
 * '<S137>' : 'chovy_tidy/Discrete PID Controller2/External Derivative/Disabled'
 * '<S138>' : 'chovy_tidy/Discrete PID Controller2/Filter/Disabled'
 * '<S139>' : 'chovy_tidy/Discrete PID Controller2/Filter ICs/Disabled'
 * '<S140>' : 'chovy_tidy/Discrete PID Controller2/I Gain/Internal Parameters'
 * '<S141>' : 'chovy_tidy/Discrete PID Controller2/Ideal P Gain/Passthrough'
 * '<S142>' : 'chovy_tidy/Discrete PID Controller2/Ideal P Gain Fdbk/Disabled'
 * '<S143>' : 'chovy_tidy/Discrete PID Controller2/Integrator/Discrete'
 * '<S144>' : 'chovy_tidy/Discrete PID Controller2/Integrator ICs/Internal IC'
 * '<S145>' : 'chovy_tidy/Discrete PID Controller2/N Copy/Disabled wSignal Specification'
 * '<S146>' : 'chovy_tidy/Discrete PID Controller2/N Gain/Disabled'
 * '<S147>' : 'chovy_tidy/Discrete PID Controller2/P Copy/Disabled'
 * '<S148>' : 'chovy_tidy/Discrete PID Controller2/Parallel P Gain/Internal Parameters'
 * '<S149>' : 'chovy_tidy/Discrete PID Controller2/Reset Signal/External Reset'
 * '<S150>' : 'chovy_tidy/Discrete PID Controller2/Saturation/Passthrough'
 * '<S151>' : 'chovy_tidy/Discrete PID Controller2/Saturation Fdbk/Disabled'
 * '<S152>' : 'chovy_tidy/Discrete PID Controller2/Sum/Sum_PI'
 * '<S153>' : 'chovy_tidy/Discrete PID Controller2/Sum Fdbk/Disabled'
 * '<S154>' : 'chovy_tidy/Discrete PID Controller2/Tracking Mode/Disabled'
 * '<S155>' : 'chovy_tidy/Discrete PID Controller2/Tracking Mode Sum/Passthrough'
 * '<S156>' : 'chovy_tidy/Discrete PID Controller2/Tsamp - Integral/TsSignalSpecification'
 * '<S157>' : 'chovy_tidy/Discrete PID Controller2/Tsamp - Ngain/Passthrough'
 * '<S158>' : 'chovy_tidy/Discrete PID Controller2/postSat Signal/Forward_Path'
 * '<S159>' : 'chovy_tidy/Discrete PID Controller2/preSat Signal/Forward_Path'
 * '<S160>' : 'chovy_tidy/Discrete PID Controller3/Anti-windup'
 * '<S161>' : 'chovy_tidy/Discrete PID Controller3/D Gain'
 * '<S162>' : 'chovy_tidy/Discrete PID Controller3/External Derivative'
 * '<S163>' : 'chovy_tidy/Discrete PID Controller3/Filter'
 * '<S164>' : 'chovy_tidy/Discrete PID Controller3/Filter ICs'
 * '<S165>' : 'chovy_tidy/Discrete PID Controller3/I Gain'
 * '<S166>' : 'chovy_tidy/Discrete PID Controller3/Ideal P Gain'
 * '<S167>' : 'chovy_tidy/Discrete PID Controller3/Ideal P Gain Fdbk'
 * '<S168>' : 'chovy_tidy/Discrete PID Controller3/Integrator'
 * '<S169>' : 'chovy_tidy/Discrete PID Controller3/Integrator ICs'
 * '<S170>' : 'chovy_tidy/Discrete PID Controller3/N Copy'
 * '<S171>' : 'chovy_tidy/Discrete PID Controller3/N Gain'
 * '<S172>' : 'chovy_tidy/Discrete PID Controller3/P Copy'
 * '<S173>' : 'chovy_tidy/Discrete PID Controller3/Parallel P Gain'
 * '<S174>' : 'chovy_tidy/Discrete PID Controller3/Reset Signal'
 * '<S175>' : 'chovy_tidy/Discrete PID Controller3/Saturation'
 * '<S176>' : 'chovy_tidy/Discrete PID Controller3/Saturation Fdbk'
 * '<S177>' : 'chovy_tidy/Discrete PID Controller3/Sum'
 * '<S178>' : 'chovy_tidy/Discrete PID Controller3/Sum Fdbk'
 * '<S179>' : 'chovy_tidy/Discrete PID Controller3/Tracking Mode'
 * '<S180>' : 'chovy_tidy/Discrete PID Controller3/Tracking Mode Sum'
 * '<S181>' : 'chovy_tidy/Discrete PID Controller3/Tsamp - Integral'
 * '<S182>' : 'chovy_tidy/Discrete PID Controller3/Tsamp - Ngain'
 * '<S183>' : 'chovy_tidy/Discrete PID Controller3/postSat Signal'
 * '<S184>' : 'chovy_tidy/Discrete PID Controller3/preSat Signal'
 * '<S185>' : 'chovy_tidy/Discrete PID Controller3/Anti-windup/Passthrough'
 * '<S186>' : 'chovy_tidy/Discrete PID Controller3/D Gain/Disabled'
 * '<S187>' : 'chovy_tidy/Discrete PID Controller3/External Derivative/Disabled'
 * '<S188>' : 'chovy_tidy/Discrete PID Controller3/Filter/Disabled'
 * '<S189>' : 'chovy_tidy/Discrete PID Controller3/Filter ICs/Disabled'
 * '<S190>' : 'chovy_tidy/Discrete PID Controller3/I Gain/Internal Parameters'
 * '<S191>' : 'chovy_tidy/Discrete PID Controller3/Ideal P Gain/Passthrough'
 * '<S192>' : 'chovy_tidy/Discrete PID Controller3/Ideal P Gain Fdbk/Disabled'
 * '<S193>' : 'chovy_tidy/Discrete PID Controller3/Integrator/Discrete'
 * '<S194>' : 'chovy_tidy/Discrete PID Controller3/Integrator ICs/Internal IC'
 * '<S195>' : 'chovy_tidy/Discrete PID Controller3/N Copy/Disabled wSignal Specification'
 * '<S196>' : 'chovy_tidy/Discrete PID Controller3/N Gain/Disabled'
 * '<S197>' : 'chovy_tidy/Discrete PID Controller3/P Copy/Disabled'
 * '<S198>' : 'chovy_tidy/Discrete PID Controller3/Parallel P Gain/Internal Parameters'
 * '<S199>' : 'chovy_tidy/Discrete PID Controller3/Reset Signal/External Reset'
 * '<S200>' : 'chovy_tidy/Discrete PID Controller3/Saturation/Passthrough'
 * '<S201>' : 'chovy_tidy/Discrete PID Controller3/Saturation Fdbk/Disabled'
 * '<S202>' : 'chovy_tidy/Discrete PID Controller3/Sum/Sum_PI'
 * '<S203>' : 'chovy_tidy/Discrete PID Controller3/Sum Fdbk/Disabled'
 * '<S204>' : 'chovy_tidy/Discrete PID Controller3/Tracking Mode/Disabled'
 * '<S205>' : 'chovy_tidy/Discrete PID Controller3/Tracking Mode Sum/Passthrough'
 * '<S206>' : 'chovy_tidy/Discrete PID Controller3/Tsamp - Integral/TsSignalSpecification'
 * '<S207>' : 'chovy_tidy/Discrete PID Controller3/Tsamp - Ngain/Passthrough'
 * '<S208>' : 'chovy_tidy/Discrete PID Controller3/postSat Signal/Forward_Path'
 * '<S209>' : 'chovy_tidy/Discrete PID Controller3/preSat Signal/Forward_Path'
 * '<S210>' : 'chovy_tidy/abc to dq0/Alpha-Beta-Zero to dq0'
 * '<S211>' : 'chovy_tidy/abc to dq0/abc to Alpha-Beta-Zero'
 * '<S212>' : 'chovy_tidy/abc to dq0/Alpha-Beta-Zero to dq0/Compare To Constant'
 * '<S213>' : 'chovy_tidy/abc to dq0/Alpha-Beta-Zero to dq0/Compare To Constant1'
 * '<S214>' : 'chovy_tidy/abc to dq0/Alpha-Beta-Zero to dq0/Subsystem - pi//2 delay'
 * '<S215>' : 'chovy_tidy/abc to dq0/Alpha-Beta-Zero to dq0/Subsystem1'
 * '<S216>' : 'chovy_tidy/abc to dq1/Alpha-Beta-Zero to dq0'
 * '<S217>' : 'chovy_tidy/abc to dq1/abc to Alpha-Beta-Zero'
 * '<S218>' : 'chovy_tidy/abc to dq1/Alpha-Beta-Zero to dq0/Compare To Constant'
 * '<S219>' : 'chovy_tidy/abc to dq1/Alpha-Beta-Zero to dq0/Compare To Constant1'
 * '<S220>' : 'chovy_tidy/abc to dq1/Alpha-Beta-Zero to dq0/Subsystem - pi//2 delay'
 * '<S221>' : 'chovy_tidy/abc to dq1/Alpha-Beta-Zero to dq0/Subsystem1'
 * '<S222>' : 'chovy_tidy/dq0 to Alpha-Beta-Zero/Compare To Constant'
 * '<S223>' : 'chovy_tidy/dq0 to Alpha-Beta-Zero/Compare To Constant1'
 * '<S224>' : 'chovy_tidy/dq0 to Alpha-Beta-Zero/Subsystem - pi//2 delay'
 * '<S225>' : 'chovy_tidy/dq0 to Alpha-Beta-Zero/Subsystem1'
 */
#endif                                 /* chovy_tidy_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
