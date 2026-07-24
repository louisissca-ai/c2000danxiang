/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: xtq2_dq_doubleloop_fullspec.h
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

#ifndef xtq2_dq_doubleloop_fullspec_h_
#define xtq2_dq_doubleloop_fullspec_h_
#ifndef xtq2_dq_doubleloop_fullspec_COMMON_INCLUDES_
#define xtq2_dq_doubleloop_fullspec_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "c2000BoardSupport.h"
#include "MW_f280015x_includes.h"
#include "IQmathLib.h"
#endif                        /* xtq2_dq_doubleloop_fullspec_COMMON_INCLUDES_ */

#include "xtq2_dq_doubleloop_fullspec_types.h"
#include <string.h>
#include <stddef.h>
#include "MW_target_hardware_resources.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

extern void config_ePWMSyncSource(void);
extern void config_ePWM_GPIO (void);
extern void config_ePWM_TBSync (void);
extern void config_ePWM_XBAR(void);

/* Block signals (default storage) */
typedef struct {
  uint16_T ADCAVoltage20kHz;           /* '<Root>/ADC-A Voltage (20 kHz)' */
  uint16_T ADCCCurrent20kHz;           /* '<Root>/ADC-C Current (20 kHz)' */
} B_xtq2_dq_doubleloop_fullspec_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T VdLPF_states;                 /* '<Root>/Vd LPF' */
  real_T Integrator_DSTATE;            /* '<S88>/Integrator' */
  real_T IdLPF_states;                 /* '<Root>/Id LPF' */
  real_T Integrator_DSTATE_h;          /* '<S38>/Integrator' */
  real_T VqLPF_states;                 /* '<Root>/Vq LPF' */
  real_T Integrator_DSTATE_f;          /* '<S188>/Integrator' */
  real_T IqLPF_states;                 /* '<Root>/Iq LPF' */
  real_T Integrator_DSTATE_i;          /* '<S138>/Integrator' */
  real_T Capacitorcurrentestimator_state;
                                      /* '<Root>/Capacitor current estimator' */
  real_T Dampinghighpass_states;       /* '<Root>/Damping high-pass' */
  real_T Current90degdelayTs_DSTATE[100];/* '<Root>/Current 90 deg delay (Ts)' */
  real_T Voltage90degdelayTs_DSTATE[100];/* '<Root>/Voltage 90 deg delay (Ts)' */
  uint16_T CircBufIdx;                 /* '<Root>/Current 90 deg delay (Ts)' */
  uint16_T CircBufIdx_f;               /* '<Root>/Voltage 90 deg delay (Ts)' */
} DW_xtq2_dq_doubleloop_fullspe_T;

/* Parameters (default storage) */
struct P_xtq2_dq_doubleloop_fullspec_T_ {
  real_T Fout;                         /* Variable: Fout
                                        * Referenced by: '<Root>/Electrical angle'
                                        */
  real_T Idq_max;                      /* Variable: Idq_max
                                        * Referenced by:
                                        *   '<S95>/Saturation'
                                        *   '<S195>/Saturation'
                                        */
  real_T Kad;                          /* Variable: Kad
                                        * Referenced by: '<Root>/Virtual damping gain'
                                        */
  real_T Ki_i_dq;                      /* Variable: Ki_i_dq
                                        * Referenced by:
                                        *   '<S35>/Integral Gain'
                                        *   '<S135>/Integral Gain'
                                        */
  real_T Ki_v_dq;                      /* Variable: Ki_v_dq
                                        * Referenced by:
                                        *   '<S85>/Integral Gain'
                                        *   '<S185>/Integral Gain'
                                        */
  real_T Kp_i_dq;                      /* Variable: Kp_i_dq
                                        * Referenced by:
                                        *   '<S43>/Proportional Gain'
                                        *   '<S143>/Proportional Gain'
                                        */
  real_T Kp_v_dq;                      /* Variable: Kp_v_dq
                                        * Referenced by:
                                        *   '<S93>/Proportional Gain'
                                        *   '<S193>/Proportional Gain'
                                        */
  real_T Udq_correction_max;           /* Variable: Udq_correction_max
                                        * Referenced by:
                                        *   '<S45>/Saturation'
                                        *   '<S145>/Saturation'
                                        */
  real_T Vdq_ref_gain;                 /* Variable: Vdq_ref_gain
                                        * Referenced by: '<Root>/Vd reference'
                                        */
  real_T Vin;                          /* Variable: Vin
                                        * Referenced by:
                                        *   '<Root>/Normalized modulation m'
                                        *   '<Root>/Modulation limit'
                                        */
  real_T Vout_rms_ref;                 /* Variable: Vout_rms_ref
                                        * Referenced by: '<Root>/Vd reference'
                                        */
  real_T daxisPIz_InitialConditionForInt;
                              /* Mask Parameter: daxisPIz_InitialConditionForInt
                               * Referenced by: '<S88>/Integrator'
                               */
  real_T daxisCurrentPIz_InitialConditio;
                              /* Mask Parameter: daxisCurrentPIz_InitialConditio
                               * Referenced by: '<S38>/Integrator'
                               */
  real_T qaxisPIz_InitialConditionForInt;
                              /* Mask Parameter: qaxisPIz_InitialConditionForInt
                               * Referenced by: '<S188>/Integrator'
                               */
  real_T qaxisCurrentPIz_InitialConditio;
                              /* Mask Parameter: qaxisCurrentPIz_InitialConditio
                               * Referenced by: '<S138>/Integrator'
                               */
  real_T qaxisCurrentPIz_Kb;           /* Mask Parameter: qaxisCurrentPIz_Kb
                                        * Referenced by: '<S130>/Kb'
                                        */
  real_T qaxisPIz_Kb;                  /* Mask Parameter: qaxisPIz_Kb
                                        * Referenced by: '<S180>/Kb'
                                        */
  real_T daxisCurrentPIz_Kb;           /* Mask Parameter: daxisCurrentPIz_Kb
                                        * Referenced by: '<S30>/Kb'
                                        */
  real_T daxisPIz_Kb;                  /* Mask Parameter: daxisPIz_Kb
                                        * Referenced by: '<S80>/Kb'
                                        */
  real_T VdLPF_NumCoef;                /* Expression: [Ts/(Vdq_lpf_tau+Ts)]
                                        * Referenced by: '<Root>/Vd LPF'
                                        */
  real_T VdLPF_DenCoef[2];      /* Expression: [1 -Vdq_lpf_tau/(Vdq_lpf_tau+Ts)]
                                 * Referenced by: '<Root>/Vd LPF'
                                 */
  real_T VdLPF_InitialStates;          /* Expression: 0
                                        * Referenced by: '<Root>/Vd LPF'
                                        */
  real_T Integrator_gainval;           /* Computed Parameter: Integrator_gainval
                                        * Referenced by: '<S88>/Integrator'
                                        */
  real_T IdLPF_NumCoef;                /* Expression: [Ts/(Idq_lpf_tau+Ts)]
                                        * Referenced by: '<Root>/Id LPF'
                                        */
  real_T IdLPF_DenCoef[2];      /* Expression: [1 -Idq_lpf_tau/(Idq_lpf_tau+Ts)]
                                 * Referenced by: '<Root>/Id LPF'
                                 */
  real_T IdLPF_InitialStates;          /* Expression: 0
                                        * Referenced by: '<Root>/Id LPF'
                                        */
  real_T Integrator_gainval_a;       /* Computed Parameter: Integrator_gainval_a
                                      * Referenced by: '<S38>/Integrator'
                                      */
  real_T Vqreference_Value;            /* Expression: 0
                                        * Referenced by: '<Root>/Vq reference'
                                        */
  real_T VqLPF_NumCoef;                /* Expression: [Ts/(Vdq_lpf_tau+Ts)]
                                        * Referenced by: '<Root>/Vq LPF'
                                        */
  real_T VqLPF_DenCoef[2];      /* Expression: [1 -Vdq_lpf_tau/(Vdq_lpf_tau+Ts)]
                                 * Referenced by: '<Root>/Vq LPF'
                                 */
  real_T VqLPF_InitialStates;          /* Expression: 0
                                        * Referenced by: '<Root>/Vq LPF'
                                        */
  real_T Integrator_gainval_p;       /* Computed Parameter: Integrator_gainval_p
                                      * Referenced by: '<S188>/Integrator'
                                      */
  real_T IqLPF_NumCoef;                /* Expression: [Ts/(Idq_lpf_tau+Ts)]
                                        * Referenced by: '<Root>/Iq LPF'
                                        */
  real_T IqLPF_DenCoef[2];      /* Expression: [1 -Idq_lpf_tau/(Idq_lpf_tau+Ts)]
                                 * Referenced by: '<Root>/Iq LPF'
                                 */
  real_T IqLPF_InitialStates;          /* Expression: 0
                                        * Referenced by: '<Root>/Iq LPF'
                                        */
  real_T Integrator_gainval_d;       /* Computed Parameter: Integrator_gainval_d
                                      * Referenced by: '<S138>/Integrator'
                                      */
  real_T Gain11_Gain;                  /* Expression: 3.3/4095
                                        * Referenced by: '<Root>/Gain11'
                                        */
  real_T Constant5_Value;              /* Expression: -1.643
                                        * Referenced by: '<Root>/Constant5'
                                        */
  real_T Gain6_Gain;                   /* Expression: 109.781873
                                        * Referenced by: '<Root>/Gain6'
                                        */
  real_T Capacitorcurrentestimator_NumCo[2];
  /* Expression: [Cfilter*(2*pi*Fad)*(2/Ts)/((2/Ts)+(2*pi*Fad)),-Cfilter*(2*pi*Fad)*(2/Ts)/((2/Ts)+(2*pi*Fad))]
   * Referenced by: '<Root>/Capacitor current estimator'
   */
  real_T Capacitorcurrentestimator_DenCo[2];
                      /* Expression: [1,((2*pi*Fad)-(2/Ts))/((2/Ts)+(2*pi*Fad))]
                       * Referenced by: '<Root>/Capacitor current estimator'
                       */
  real_T Capacitorcurrentestimator_Initi;/* Expression: 0
                                          * Referenced by: '<Root>/Capacitor current estimator'
                                          */
  real_T Dampinghighpass_NumCoef[2];
     /* Expression: [(2/Ts)/((2/Ts)+(2*pi*FadHP)),-(2/Ts)/((2/Ts)+(2*pi*FadHP))]
      * Referenced by: '<Root>/Damping high-pass'
      */
  real_T Dampinghighpass_DenCoef[2];
                  /* Expression: [1,((2*pi*FadHP)-(2/Ts))/((2/Ts)+(2*pi*FadHP))]
                   * Referenced by: '<Root>/Damping high-pass'
                   */
  real_T Dampinghighpass_InitialStates;/* Expression: 0
                                        * Referenced by: '<Root>/Damping high-pass'
                                        */
  real_T mLegB_Gain;                   /* Expression: -1
                                        * Referenced by: '<Root>/-m (Leg B)'
                                        */
  real_T Gain1_Gain;                   /* Expression: 3.3/4095
                                        * Referenced by: '<Root>/Gain1'
                                        */
  real_T Constant1_Value;              /* Expression: -1.643
                                        * Referenced by: '<Root>/Constant1'
                                        */
  real_T Gain4_Gain;                   /* Expression: 3.678481
                                        * Referenced by: '<Root>/Gain4'
                                        */
  real_T Current90degdelayTs_InitialCond;/* Expression: 0
                                          * Referenced by: '<Root>/Current 90 deg delay (Ts)'
                                          */
  real_T Voltage90degdelayTs_InitialCond;/* Expression: 0
                                          * Referenced by: '<Root>/Voltage 90 deg delay (Ts)'
                                          */
};

/* Real-time Model Data Structure */
struct tag_RTM_xtq2_dq_doubleloop_fu_T {
  const char_T * volatile errorStatus;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
  } Timing;
};

/* Block parameters (default storage) */
extern P_xtq2_dq_doubleloop_fullspec_T xtq2_dq_doubleloop_fullspec_P;

/* Block signals (default storage) */
extern B_xtq2_dq_doubleloop_fullspec_T xtq2_dq_doubleloop_fullspec_B;

/* Block states (default storage) */
extern DW_xtq2_dq_doubleloop_fullspe_T xtq2_dq_doubleloop_fullspec_DW;

/* Model entry point functions */
extern void xtq2_dq_doubleloop_fullspec_initialize(void);
extern void xtq2_dq_doubleloop_fullspec_step(void);
extern void xtq2_dq_doubleloop_fullspec_terminate(void);

/* Real-time Model object */
extern RT_MODEL_xtq2_dq_doubleloop_f_T *const xtq2_dq_doubleloop_fullspec_M;
extern volatile boolean_T stopRequested;
extern volatile boolean_T runModel;

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
 * '<Root>' : 'xtq2_dq_doubleloop_fullspec'
 * '<S1>'   : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)'
 * '<S2>'   : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)'
 * '<S3>'   : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)'
 * '<S4>'   : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)'
 * '<S5>'   : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Anti-windup'
 * '<S6>'   : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/D Gain'
 * '<S7>'   : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/External Derivative'
 * '<S8>'   : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Filter'
 * '<S9>'   : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Filter ICs'
 * '<S10>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/I Gain'
 * '<S11>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Ideal P Gain'
 * '<S12>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Ideal P Gain Fdbk'
 * '<S13>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Integrator'
 * '<S14>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Integrator ICs'
 * '<S15>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/N Copy'
 * '<S16>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/N Gain'
 * '<S17>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/P Copy'
 * '<S18>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Parallel P Gain'
 * '<S19>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Reset Signal'
 * '<S20>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Saturation'
 * '<S21>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Saturation Fdbk'
 * '<S22>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Sum'
 * '<S23>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Sum Fdbk'
 * '<S24>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Tracking Mode'
 * '<S25>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Tracking Mode Sum'
 * '<S26>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Tsamp - Integral'
 * '<S27>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Tsamp - Ngain'
 * '<S28>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/postSat Signal'
 * '<S29>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/preSat Signal'
 * '<S30>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Anti-windup/Back Calculation'
 * '<S31>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/D Gain/Disabled'
 * '<S32>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/External Derivative/Disabled'
 * '<S33>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Filter/Disabled'
 * '<S34>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Filter ICs/Disabled'
 * '<S35>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/I Gain/Internal Parameters'
 * '<S36>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Ideal P Gain/Passthrough'
 * '<S37>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Ideal P Gain Fdbk/Disabled'
 * '<S38>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Integrator/Discrete'
 * '<S39>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Integrator ICs/Internal IC'
 * '<S40>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/N Copy/Disabled wSignal Specification'
 * '<S41>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/N Gain/Disabled'
 * '<S42>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/P Copy/Disabled'
 * '<S43>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Parallel P Gain/Internal Parameters'
 * '<S44>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Reset Signal/Disabled'
 * '<S45>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Saturation/Enabled'
 * '<S46>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Saturation Fdbk/Disabled'
 * '<S47>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Sum/Sum_PI'
 * '<S48>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Sum Fdbk/Disabled'
 * '<S49>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Tracking Mode/Disabled'
 * '<S50>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Tracking Mode Sum/Passthrough'
 * '<S51>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Tsamp - Integral/TsSignalSpecification'
 * '<S52>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/Tsamp - Ngain/Passthrough'
 * '<S53>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/postSat Signal/Forward_Path'
 * '<S54>'  : 'xtq2_dq_doubleloop_fullspec/d-axis Current PI(z)/preSat Signal/Forward_Path'
 * '<S55>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Anti-windup'
 * '<S56>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/D Gain'
 * '<S57>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/External Derivative'
 * '<S58>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Filter'
 * '<S59>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Filter ICs'
 * '<S60>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/I Gain'
 * '<S61>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Ideal P Gain'
 * '<S62>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Ideal P Gain Fdbk'
 * '<S63>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Integrator'
 * '<S64>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Integrator ICs'
 * '<S65>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/N Copy'
 * '<S66>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/N Gain'
 * '<S67>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/P Copy'
 * '<S68>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Parallel P Gain'
 * '<S69>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Reset Signal'
 * '<S70>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Saturation'
 * '<S71>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Saturation Fdbk'
 * '<S72>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Sum'
 * '<S73>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Sum Fdbk'
 * '<S74>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Tracking Mode'
 * '<S75>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Tracking Mode Sum'
 * '<S76>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Tsamp - Integral'
 * '<S77>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Tsamp - Ngain'
 * '<S78>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/postSat Signal'
 * '<S79>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/preSat Signal'
 * '<S80>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Anti-windup/Back Calculation'
 * '<S81>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/D Gain/Disabled'
 * '<S82>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/External Derivative/Disabled'
 * '<S83>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Filter/Disabled'
 * '<S84>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Filter ICs/Disabled'
 * '<S85>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/I Gain/Internal Parameters'
 * '<S86>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Ideal P Gain/Passthrough'
 * '<S87>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Ideal P Gain Fdbk/Disabled'
 * '<S88>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Integrator/Discrete'
 * '<S89>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Integrator ICs/Internal IC'
 * '<S90>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/N Copy/Disabled wSignal Specification'
 * '<S91>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/N Gain/Disabled'
 * '<S92>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/P Copy/Disabled'
 * '<S93>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Parallel P Gain/Internal Parameters'
 * '<S94>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Reset Signal/Disabled'
 * '<S95>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Saturation/Enabled'
 * '<S96>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Saturation Fdbk/Disabled'
 * '<S97>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Sum/Sum_PI'
 * '<S98>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Sum Fdbk/Disabled'
 * '<S99>'  : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Tracking Mode/Disabled'
 * '<S100>' : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Tracking Mode Sum/Passthrough'
 * '<S101>' : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Tsamp - Integral/TsSignalSpecification'
 * '<S102>' : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/Tsamp - Ngain/Passthrough'
 * '<S103>' : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/postSat Signal/Forward_Path'
 * '<S104>' : 'xtq2_dq_doubleloop_fullspec/d-axis PI(z)/preSat Signal/Forward_Path'
 * '<S105>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Anti-windup'
 * '<S106>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/D Gain'
 * '<S107>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/External Derivative'
 * '<S108>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Filter'
 * '<S109>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Filter ICs'
 * '<S110>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/I Gain'
 * '<S111>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Ideal P Gain'
 * '<S112>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Ideal P Gain Fdbk'
 * '<S113>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Integrator'
 * '<S114>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Integrator ICs'
 * '<S115>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/N Copy'
 * '<S116>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/N Gain'
 * '<S117>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/P Copy'
 * '<S118>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Parallel P Gain'
 * '<S119>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Reset Signal'
 * '<S120>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Saturation'
 * '<S121>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Saturation Fdbk'
 * '<S122>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Sum'
 * '<S123>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Sum Fdbk'
 * '<S124>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Tracking Mode'
 * '<S125>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Tracking Mode Sum'
 * '<S126>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Tsamp - Integral'
 * '<S127>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Tsamp - Ngain'
 * '<S128>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/postSat Signal'
 * '<S129>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/preSat Signal'
 * '<S130>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Anti-windup/Back Calculation'
 * '<S131>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/D Gain/Disabled'
 * '<S132>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/External Derivative/Disabled'
 * '<S133>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Filter/Disabled'
 * '<S134>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Filter ICs/Disabled'
 * '<S135>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/I Gain/Internal Parameters'
 * '<S136>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Ideal P Gain/Passthrough'
 * '<S137>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Ideal P Gain Fdbk/Disabled'
 * '<S138>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Integrator/Discrete'
 * '<S139>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Integrator ICs/Internal IC'
 * '<S140>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/N Copy/Disabled wSignal Specification'
 * '<S141>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/N Gain/Disabled'
 * '<S142>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/P Copy/Disabled'
 * '<S143>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Parallel P Gain/Internal Parameters'
 * '<S144>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Reset Signal/Disabled'
 * '<S145>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Saturation/Enabled'
 * '<S146>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Saturation Fdbk/Disabled'
 * '<S147>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Sum/Sum_PI'
 * '<S148>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Sum Fdbk/Disabled'
 * '<S149>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Tracking Mode/Disabled'
 * '<S150>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Tracking Mode Sum/Passthrough'
 * '<S151>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Tsamp - Integral/TsSignalSpecification'
 * '<S152>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/Tsamp - Ngain/Passthrough'
 * '<S153>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/postSat Signal/Forward_Path'
 * '<S154>' : 'xtq2_dq_doubleloop_fullspec/q-axis Current PI(z)/preSat Signal/Forward_Path'
 * '<S155>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Anti-windup'
 * '<S156>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/D Gain'
 * '<S157>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/External Derivative'
 * '<S158>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Filter'
 * '<S159>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Filter ICs'
 * '<S160>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/I Gain'
 * '<S161>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Ideal P Gain'
 * '<S162>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Ideal P Gain Fdbk'
 * '<S163>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Integrator'
 * '<S164>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Integrator ICs'
 * '<S165>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/N Copy'
 * '<S166>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/N Gain'
 * '<S167>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/P Copy'
 * '<S168>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Parallel P Gain'
 * '<S169>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Reset Signal'
 * '<S170>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Saturation'
 * '<S171>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Saturation Fdbk'
 * '<S172>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Sum'
 * '<S173>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Sum Fdbk'
 * '<S174>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Tracking Mode'
 * '<S175>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Tracking Mode Sum'
 * '<S176>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Tsamp - Integral'
 * '<S177>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Tsamp - Ngain'
 * '<S178>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/postSat Signal'
 * '<S179>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/preSat Signal'
 * '<S180>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Anti-windup/Back Calculation'
 * '<S181>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/D Gain/Disabled'
 * '<S182>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/External Derivative/Disabled'
 * '<S183>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Filter/Disabled'
 * '<S184>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Filter ICs/Disabled'
 * '<S185>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/I Gain/Internal Parameters'
 * '<S186>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Ideal P Gain/Passthrough'
 * '<S187>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Ideal P Gain Fdbk/Disabled'
 * '<S188>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Integrator/Discrete'
 * '<S189>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Integrator ICs/Internal IC'
 * '<S190>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/N Copy/Disabled wSignal Specification'
 * '<S191>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/N Gain/Disabled'
 * '<S192>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/P Copy/Disabled'
 * '<S193>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Parallel P Gain/Internal Parameters'
 * '<S194>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Reset Signal/Disabled'
 * '<S195>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Saturation/Enabled'
 * '<S196>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Saturation Fdbk/Disabled'
 * '<S197>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Sum/Sum_PI'
 * '<S198>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Sum Fdbk/Disabled'
 * '<S199>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Tracking Mode/Disabled'
 * '<S200>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Tracking Mode Sum/Passthrough'
 * '<S201>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Tsamp - Integral/TsSignalSpecification'
 * '<S202>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/Tsamp - Ngain/Passthrough'
 * '<S203>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/postSat Signal/Forward_Path'
 * '<S204>' : 'xtq2_dq_doubleloop_fullspec/q-axis PI(z)/preSat Signal/Forward_Path'
 */
#endif                                 /* xtq2_dq_doubleloop_fullspec_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
