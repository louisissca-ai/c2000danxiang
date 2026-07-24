/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: xtq2_dq_doubleloop_fullspec.c
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
#include <math.h>
#include "rtwtypes.h"
#include "xtq2_dq_doubleloop_fullspec_private.h"
#include "app_config.h"

#include <string.h>

/* Block signals (default storage) */
B_xtq2_dq_doubleloop_fullspec_T xtq2_dq_doubleloop_fullspec_B;

/* Block states (default storage) */
DW_xtq2_dq_doubleloop_fullspe_T xtq2_dq_doubleloop_fullspec_DW;

/* Real-time model */
static RT_MODEL_xtq2_dq_doubleloop_f_T xtq2_dq_doubleloop_fullspec_M_;
RT_MODEL_xtq2_dq_doubleloop_f_T *const xtq2_dq_doubleloop_fullspec_M =
  &xtq2_dq_doubleloop_fullspec_M_;

#ifndef __TMS320C28XX_CLA__

uint16_T MW_adcAInitFlag = 0;

#endif

#ifndef __TMS320C28XX_CLA__

uint16_T MW_adcCInitFlag = 0;

#endif

/* Model step function */
void xtq2_dq_doubleloop_fullspec_step(void)
{
  real_T Capacitorcurrentestimator_tmp;
  real_T Dampinghighpass_tmp;
  real_T rtb_CastToDouble1;
  real_T rtb_ActuatorSaturationError;
  real_T rtb_Gain6;
  real_T rtb_Normalizedmodulationm;
  real_T rtb_Saturation;
  real_T rtb_Saturation_a;
  real_T rtb_Saturation_d;
  real_T rtb_Saturation_ny;
  real_T rtb_Sum;
  real_T rtb_Sum_g;
  real_T rtb_Sum_j;
  real_T rtb_Sum_l;
  real_T rtb_costheta;
  real_T rtb_dcurrenterror;
  real_T rtb_derror;
  real_T rtb_qcurrenterror;
  real_T rtb_qerror;
  real_T rtb_sintheta;
  real_T rtb_UnlimitedVoltageCommand;

  /* Sum: '<Root>/d error' incorporates:
   *  Constant: '<Root>/Vd reference'
   *  DiscreteTransferFcn: '<Root>/Vd LPF'
   *  Sum: '<Root>/d feedforward'
   */
  rtb_Normalizedmodulationm = 1.4142135623730951 *
    xtq2_dq_doubleloop_fullspec_P.Vout_rms_ref *
    xtq2_dq_doubleloop_fullspec_P.Vdq_ref_gain;
  rtb_derror = rtb_Normalizedmodulationm -
    xtq2_dq_doubleloop_fullspec_P.VdLPF_NumCoef *
    xtq2_dq_doubleloop_fullspec_DW.VdLPF_states;

  /* Sum: '<S97>/Sum' incorporates:
   *  DiscreteIntegrator: '<S88>/Integrator'
   *  Gain: '<S93>/Proportional Gain'
   */
  rtb_Sum = xtq2_dq_doubleloop_fullspec_P.Kp_v_dq * rtb_derror +
    xtq2_dq_doubleloop_fullspec_DW.Integrator_DSTATE;

  /* Saturate: '<S95>/Saturation' */
  if (rtb_Sum > xtq2_dq_doubleloop_fullspec_P.Idq_max) {
    rtb_Saturation = xtq2_dq_doubleloop_fullspec_P.Idq_max;
  } else if (rtb_Sum < -xtq2_dq_doubleloop_fullspec_P.Idq_max) {
    rtb_Saturation = -xtq2_dq_doubleloop_fullspec_P.Idq_max;
  } else {
    rtb_Saturation = rtb_Sum;
  }

  /* End of Saturate: '<S95>/Saturation' */

  /* Sum: '<Root>/d current error' incorporates:
   *  DiscreteTransferFcn: '<Root>/Id LPF'
   */
  rtb_dcurrenterror = rtb_Saturation -
    xtq2_dq_doubleloop_fullspec_P.IdLPF_NumCoef *
    xtq2_dq_doubleloop_fullspec_DW.IdLPF_states;

  /* Sum: '<S47>/Sum' incorporates:
   *  DiscreteIntegrator: '<S38>/Integrator'
   *  Gain: '<S43>/Proportional Gain'
   */
  rtb_Sum_j = xtq2_dq_doubleloop_fullspec_P.Kp_i_dq * rtb_dcurrenterror +
    xtq2_dq_doubleloop_fullspec_DW.Integrator_DSTATE_h;

  /* Saturate: '<S45>/Saturation' */
  if (rtb_Sum_j > xtq2_dq_doubleloop_fullspec_P.Udq_correction_max) {
    rtb_Saturation_a = xtq2_dq_doubleloop_fullspec_P.Udq_correction_max;
  } else if (rtb_Sum_j < -xtq2_dq_doubleloop_fullspec_P.Udq_correction_max) {
    rtb_Saturation_a = -xtq2_dq_doubleloop_fullspec_P.Udq_correction_max;
  } else {
    rtb_Saturation_a = rtb_Sum_j;
  }

  /* End of Saturate: '<S45>/Saturation' */

  /* Gain: '<Root>/Electrical angle' incorporates:
   *  DigitalClock: '<Root>/Clock'
   */
  rtb_costheta = 6.2831853071795862 * xtq2_dq_doubleloop_fullspec_P.Fout *
    ((xtq2_dq_doubleloop_fullspec_M->Timing.clockTick0) * 5.0E-5);

  /* Trigonometry: '<Root>/sin theta' */
  rtb_sintheta = sin(rtb_costheta);

  /* Sum: '<Root>/q error' incorporates:
   *  Constant: '<Root>/Vq reference'
   *  DiscreteTransferFcn: '<Root>/Vq LPF'
   */
  rtb_qerror = xtq2_dq_doubleloop_fullspec_P.Vqreference_Value -
    xtq2_dq_doubleloop_fullspec_P.VqLPF_NumCoef *
    xtq2_dq_doubleloop_fullspec_DW.VqLPF_states;

  /* Sum: '<S197>/Sum' incorporates:
   *  DiscreteIntegrator: '<S188>/Integrator'
   *  Gain: '<S193>/Proportional Gain'
   */
  rtb_Sum_l = xtq2_dq_doubleloop_fullspec_P.Kp_v_dq * rtb_qerror +
    xtq2_dq_doubleloop_fullspec_DW.Integrator_DSTATE_f;

  /* Saturate: '<S195>/Saturation' */
  if (rtb_Sum_l > xtq2_dq_doubleloop_fullspec_P.Idq_max) {
    rtb_Saturation_ny = xtq2_dq_doubleloop_fullspec_P.Idq_max;
  } else if (rtb_Sum_l < -xtq2_dq_doubleloop_fullspec_P.Idq_max) {
    rtb_Saturation_ny = -xtq2_dq_doubleloop_fullspec_P.Idq_max;
  } else {
    rtb_Saturation_ny = rtb_Sum_l;
  }

  /* End of Saturate: '<S195>/Saturation' */

  /* Sum: '<Root>/q current error' incorporates:
   *  DiscreteTransferFcn: '<Root>/Iq LPF'
   */
  rtb_qcurrenterror = rtb_Saturation_ny -
    xtq2_dq_doubleloop_fullspec_P.IqLPF_NumCoef *
    xtq2_dq_doubleloop_fullspec_DW.IqLPF_states;

  /* Sum: '<S147>/Sum' incorporates:
   *  DiscreteIntegrator: '<S138>/Integrator'
   *  Gain: '<S143>/Proportional Gain'
   */
  rtb_Sum_g = xtq2_dq_doubleloop_fullspec_P.Kp_i_dq * rtb_qcurrenterror +
    xtq2_dq_doubleloop_fullspec_DW.Integrator_DSTATE_i;

  /* Saturate: '<S145>/Saturation' */
  if (rtb_Sum_g > xtq2_dq_doubleloop_fullspec_P.Udq_correction_max) {
    rtb_Saturation_d = xtq2_dq_doubleloop_fullspec_P.Udq_correction_max;
  } else if (rtb_Sum_g < -xtq2_dq_doubleloop_fullspec_P.Udq_correction_max) {
    rtb_Saturation_d = -xtq2_dq_doubleloop_fullspec_P.Udq_correction_max;
  } else {
    rtb_Saturation_d = rtb_Sum_g;
  }

  /* End of Saturate: '<S145>/Saturation' */

  /* Trigonometry: '<Root>/cos theta' */
  rtb_costheta = cos(rtb_costheta);

  /* S-Function (c2802xadc): '<Root>/ADC-A Voltage (20 kHz)' */
  {
    /*  Internal Reference Voltage : Fixed scale 0 to 3.3 V range.  */
    /*  External Reference Voltage : Allowable ranges of VREFHI(ADCINA0) = 3.3 and VREFLO(tied to ground) = 0  */
    xtq2_dq_doubleloop_fullspec_B.ADCAVoltage20kHz = (AdcaResultRegs.ADCRESULT0);
  }

  /* Gain: '<Root>/Gain6' incorporates:
   *  Constant: '<Root>/Constant5'
   *  DataTypeConversion: '<Root>/Cast To Double'
   *  Gain: '<Root>/Gain11'
   *  Sum: '<Root>/Sum7'
   */
  rtb_Gain6 = (xtq2_dq_doubleloop_fullspec_P.Gain11_Gain * (real_T)
               xtq2_dq_doubleloop_fullspec_B.ADCAVoltage20kHz +
               xtq2_dq_doubleloop_fullspec_P.Constant5_Value) *
    xtq2_dq_doubleloop_fullspec_P.Gain6_Gain;

  /* DiscreteTransferFcn: '<Root>/Capacitor current estimator' */
  Capacitorcurrentestimator_tmp = (rtb_Gain6 -
    xtq2_dq_doubleloop_fullspec_P.Capacitorcurrentestimator_DenCo[1L] *
    xtq2_dq_doubleloop_fullspec_DW.Capacitorcurrentestimator_state) /
    xtq2_dq_doubleloop_fullspec_P.Capacitorcurrentestimator_DenCo[0];

  /* DiscreteTransferFcn: '<Root>/Damping high-pass' incorporates:
   *  DiscreteTransferFcn: '<Root>/Capacitor current estimator'
   */
  Dampinghighpass_tmp =
    ((xtq2_dq_doubleloop_fullspec_P.Capacitorcurrentestimator_NumCo[0] *
      Capacitorcurrentestimator_tmp +
      xtq2_dq_doubleloop_fullspec_P.Capacitorcurrentestimator_NumCo[1L] *
      xtq2_dq_doubleloop_fullspec_DW.Capacitorcurrentestimator_state) -
     xtq2_dq_doubleloop_fullspec_P.Dampinghighpass_DenCoef[1L] *
     xtq2_dq_doubleloop_fullspec_DW.Dampinghighpass_states) /
    xtq2_dq_doubleloop_fullspec_P.Dampinghighpass_DenCoef[0];

  /* Sum: '<Root>/Active damping sum' incorporates:
   *  DiscreteTransferFcn: '<Root>/Damping high-pass'
   *  Gain: '<Root>/Virtual damping gain'
   *  Product: '<Root>/ud sin'
   *  Product: '<Root>/uq cos'
   *  Sum: '<Root>/Inverse Park alpha'
   *  Sum: '<Root>/d feedforward'
   */
  rtb_Normalizedmodulationm = ((rtb_Normalizedmodulationm + rtb_Saturation_a) *
    rtb_sintheta + rtb_Saturation_d * rtb_costheta) -
    (xtq2_dq_doubleloop_fullspec_P.Dampinghighpass_NumCoef[0] *
     Dampinghighpass_tmp +
     xtq2_dq_doubleloop_fullspec_P.Dampinghighpass_NumCoef[1L] *
     xtq2_dq_doubleloop_fullspec_DW.Dampinghighpass_states) *
    xtq2_dq_doubleloop_fullspec_P.Kad;

  /*
   * ponytail: this generated source is patched because the .slx is absent.
   * Keep the actuator limit below the dead-time/minimum-pulse boundary and
   * feed the lost voltage back into the inner PI anti-windup path.
   */
  rtb_UnlimitedVoltageCommand = rtb_Normalizedmodulationm;
  rtb_CastToDouble1 = xtq2_dq_doubleloop_fullspec_P.Vin *
    APP_PWM_MAX_MODULATION;
  if (rtb_Normalizedmodulationm > rtb_CastToDouble1) {
    rtb_Normalizedmodulationm = rtb_CastToDouble1;
  } else if (rtb_Normalizedmodulationm < -rtb_CastToDouble1) {
    rtb_Normalizedmodulationm = -rtb_CastToDouble1;
  }
  rtb_ActuatorSaturationError = rtb_Normalizedmodulationm -
    rtb_UnlimitedVoltageCommand;

  /* Gain: '<Root>/Normalized modulation m' incorporates:
   *  Saturate: '<Root>/Modulation limit'
   */
  rtb_Normalizedmodulationm *= 1.0 / xtq2_dq_doubleloop_fullspec_P.Vin;

  /* Fcn: '<Root>/Duty A [%]' */
  rtb_CastToDouble1 = (rtb_Normalizedmodulationm + 1.0) * 50.0;

  /* S-Function (c2802xpwm): '<Root>/ePWM1 Leg A (20 kHz)' */

  /*-- Update CMPA value for ePWM1 --*/
  {
    EPwm1Regs.CMPA.bit.CMPA = (uint16_T)((uint32_T)EPwm1Regs.TBPRD *
      rtb_CastToDouble1 * 0.01);
  }

  /* Fcn: '<Root>/Duty B [%]' incorporates:
   *  Gain: '<Root>/-m (Leg B)'
   */
  rtb_CastToDouble1 = (xtq2_dq_doubleloop_fullspec_P.mLegB_Gain *
                       rtb_Normalizedmodulationm + 1.0) * 50.0;

  /* S-Function (c2802xpwm): '<Root>/ePWM2 Leg B (20 kHz)' */

  /*-- Update CMPA value for ePWM2 --*/
  {
    EPwm2Regs.CMPA.bit.CMPA = (uint16_T)((uint32_T)EPwm2Regs.TBPRD *
      rtb_CastToDouble1 * 0.01);
  }

  /* S-Function (c2802xadc): '<Root>/ADC-C Current (20 kHz)' */
  {
    /*  Internal Reference Voltage : Fixed scale 0 to 3.3 V range.  */
    /*  External Reference Voltage : Allowable ranges of VREFHI(ADCINA0) = 3.3 and VREFLO(tied to ground) = 0  */
    xtq2_dq_doubleloop_fullspec_B.ADCCCurrent20kHz = (AdccResultRegs.ADCRESULT0);
  }

  /* DataTypeConversion: '<Root>/Cast To Double1' */
  rtb_CastToDouble1 = xtq2_dq_doubleloop_fullspec_B.ADCCCurrent20kHz;

  /* Gain: '<Root>/Gain4' incorporates:
   *  Constant: '<Root>/Constant1'
   *  Gain: '<Root>/Gain1'
   *  Sum: '<Root>/Sum1'
   */
  rtb_Normalizedmodulationm = (xtq2_dq_doubleloop_fullspec_P.Gain1_Gain *
    rtb_CastToDouble1 + xtq2_dq_doubleloop_fullspec_P.Constant1_Value) *
    xtq2_dq_doubleloop_fullspec_P.Gain4_Gain;

  /* Update for DiscreteTransferFcn: '<Root>/Vd LPF' incorporates:
   *  Delay: '<Root>/Voltage 90 deg delay (Ts)'
   *  Product: '<Root>/alpha sin'
   *  Product: '<Root>/beta cos'
   *  Sum: '<Root>/Park d'
   */
  xtq2_dq_doubleloop_fullspec_DW.VdLPF_states = ((rtb_Gain6 * rtb_sintheta -
    xtq2_dq_doubleloop_fullspec_DW.Voltage90degdelayTs_DSTATE[xtq2_dq_doubleloop_fullspec_DW.CircBufIdx_f]
    * rtb_costheta) - xtq2_dq_doubleloop_fullspec_P.VdLPF_DenCoef[1L] *
    xtq2_dq_doubleloop_fullspec_DW.VdLPF_states) /
    xtq2_dq_doubleloop_fullspec_P.VdLPF_DenCoef[0];

  /* Update for DiscreteIntegrator: '<S88>/Integrator' incorporates:
   *  Gain: '<S80>/Kb'
   *  Gain: '<S85>/Integral Gain'
   *  Sum: '<S80>/SumI2'
   *  Sum: '<S80>/SumI4'
   */
  xtq2_dq_doubleloop_fullspec_DW.Integrator_DSTATE += ((rtb_Saturation - rtb_Sum)
    * xtq2_dq_doubleloop_fullspec_P.daxisPIz_Kb +
    xtq2_dq_doubleloop_fullspec_P.Ki_v_dq * rtb_derror) *
    xtq2_dq_doubleloop_fullspec_P.Integrator_gainval;

  /* Update for DiscreteTransferFcn: '<Root>/Id LPF' incorporates:
   *  Delay: '<Root>/Current 90 deg delay (Ts)'
   *  Product: '<Root>/i alpha sin'
   *  Product: '<Root>/i beta cos'
   *  Sum: '<Root>/Park id'
   */
  xtq2_dq_doubleloop_fullspec_DW.IdLPF_states = ((rtb_Normalizedmodulationm *
    rtb_sintheta -
    xtq2_dq_doubleloop_fullspec_DW.Current90degdelayTs_DSTATE[xtq2_dq_doubleloop_fullspec_DW.CircBufIdx]
    * rtb_costheta) - xtq2_dq_doubleloop_fullspec_P.IdLPF_DenCoef[1L] *
    xtq2_dq_doubleloop_fullspec_DW.IdLPF_states) /
    xtq2_dq_doubleloop_fullspec_P.IdLPF_DenCoef[0];

  /* Update for DiscreteIntegrator: '<S38>/Integrator' incorporates:
   *  Gain: '<S30>/Kb'
   *  Gain: '<S35>/Integral Gain'
   *  Sum: '<S30>/SumI2'
   *  Sum: '<S30>/SumI4'
   */
  xtq2_dq_doubleloop_fullspec_DW.Integrator_DSTATE_h += ((rtb_Saturation_a -
    rtb_Sum_j) * xtq2_dq_doubleloop_fullspec_P.daxisCurrentPIz_Kb +
    rtb_ActuatorSaturationError * rtb_sintheta +
    xtq2_dq_doubleloop_fullspec_P.Ki_i_dq * rtb_dcurrenterror) *
    xtq2_dq_doubleloop_fullspec_P.Integrator_gainval_a;

  /* Update for DiscreteTransferFcn: '<Root>/Vq LPF' incorporates:
   *  Delay: '<Root>/Voltage 90 deg delay (Ts)'
   *  Product: '<Root>/alpha cos'
   *  Product: '<Root>/beta sin'
   *  Sum: '<Root>/Park q'
   */
  xtq2_dq_doubleloop_fullspec_DW.VqLPF_states = ((rtb_Gain6 * rtb_costheta +
    xtq2_dq_doubleloop_fullspec_DW.Voltage90degdelayTs_DSTATE[xtq2_dq_doubleloop_fullspec_DW.CircBufIdx_f]
    * rtb_sintheta) - xtq2_dq_doubleloop_fullspec_P.VqLPF_DenCoef[1L] *
    xtq2_dq_doubleloop_fullspec_DW.VqLPF_states) /
    xtq2_dq_doubleloop_fullspec_P.VqLPF_DenCoef[0];

  /* Update for DiscreteIntegrator: '<S188>/Integrator' incorporates:
   *  Gain: '<S180>/Kb'
   *  Gain: '<S185>/Integral Gain'
   *  Sum: '<S180>/SumI2'
   *  Sum: '<S180>/SumI4'
   */
  xtq2_dq_doubleloop_fullspec_DW.Integrator_DSTATE_f += ((rtb_Saturation_ny -
    rtb_Sum_l) * xtq2_dq_doubleloop_fullspec_P.qaxisPIz_Kb +
    xtq2_dq_doubleloop_fullspec_P.Ki_v_dq * rtb_qerror) *
    xtq2_dq_doubleloop_fullspec_P.Integrator_gainval_p;

  /* Update for DiscreteTransferFcn: '<Root>/Iq LPF' incorporates:
   *  Delay: '<Root>/Current 90 deg delay (Ts)'
   *  Product: '<Root>/i alpha cos'
   *  Product: '<Root>/i beta sin'
   *  Sum: '<Root>/Park iq'
   */
  xtq2_dq_doubleloop_fullspec_DW.IqLPF_states = ((rtb_Normalizedmodulationm *
    rtb_costheta +
    xtq2_dq_doubleloop_fullspec_DW.Current90degdelayTs_DSTATE[xtq2_dq_doubleloop_fullspec_DW.CircBufIdx]
    * rtb_sintheta) - xtq2_dq_doubleloop_fullspec_P.IqLPF_DenCoef[1L] *
    xtq2_dq_doubleloop_fullspec_DW.IqLPF_states) /
    xtq2_dq_doubleloop_fullspec_P.IqLPF_DenCoef[0];

  /* Update for DiscreteIntegrator: '<S138>/Integrator' incorporates:
   *  Gain: '<S130>/Kb'
   *  Gain: '<S135>/Integral Gain'
   *  Sum: '<S130>/SumI2'
   *  Sum: '<S130>/SumI4'
   */
  xtq2_dq_doubleloop_fullspec_DW.Integrator_DSTATE_i += ((rtb_Saturation_d -
    rtb_Sum_g) * xtq2_dq_doubleloop_fullspec_P.qaxisCurrentPIz_Kb +
    rtb_ActuatorSaturationError * rtb_costheta +
    xtq2_dq_doubleloop_fullspec_P.Ki_i_dq * rtb_qcurrenterror) *
    xtq2_dq_doubleloop_fullspec_P.Integrator_gainval_d;

  /* Update for DiscreteTransferFcn: '<Root>/Capacitor current estimator' */
  xtq2_dq_doubleloop_fullspec_DW.Capacitorcurrentestimator_state =
    Capacitorcurrentestimator_tmp;

  /* Update for DiscreteTransferFcn: '<Root>/Damping high-pass' */
  xtq2_dq_doubleloop_fullspec_DW.Dampinghighpass_states = Dampinghighpass_tmp;

  /* Update for Delay: '<Root>/Current 90 deg delay (Ts)' */
  xtq2_dq_doubleloop_fullspec_DW.Current90degdelayTs_DSTATE[xtq2_dq_doubleloop_fullspec_DW.CircBufIdx]
    = rtb_Normalizedmodulationm;
  if (xtq2_dq_doubleloop_fullspec_DW.CircBufIdx < 99U) {
    xtq2_dq_doubleloop_fullspec_DW.CircBufIdx++;
  } else {
    xtq2_dq_doubleloop_fullspec_DW.CircBufIdx = 0U;
  }

  /* End of Update for Delay: '<Root>/Current 90 deg delay (Ts)' */

  /* Update for Delay: '<Root>/Voltage 90 deg delay (Ts)' */
  xtq2_dq_doubleloop_fullspec_DW.Voltage90degdelayTs_DSTATE[xtq2_dq_doubleloop_fullspec_DW.CircBufIdx_f]
    = rtb_Gain6;
  if (xtq2_dq_doubleloop_fullspec_DW.CircBufIdx_f < 99U) {
    xtq2_dq_doubleloop_fullspec_DW.CircBufIdx_f++;
  } else {
    xtq2_dq_doubleloop_fullspec_DW.CircBufIdx_f = 0U;
  }

  /* End of Update for Delay: '<Root>/Voltage 90 deg delay (Ts)' */

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The resolution of this integer timer is 5.0E-5, which is the step size
   * of the task. Size of "clockTick0" ensures timer will not overflow during the
   * application lifespan selected.
   */
  xtq2_dq_doubleloop_fullspec_M->Timing.clockTick0++;
}

/* Model initialize function */
void xtq2_dq_doubleloop_fullspec_initialize(void)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)xtq2_dq_doubleloop_fullspec_M, 0,
                sizeof(RT_MODEL_xtq2_dq_doubleloop_f_T));

  /* block I/O */
  (void) memset(((void *) &xtq2_dq_doubleloop_fullspec_B), 0,
                sizeof(B_xtq2_dq_doubleloop_fullspec_T));

  /* states (dwork) */
  (void) memset((void *)&xtq2_dq_doubleloop_fullspec_DW, 0,
                sizeof(DW_xtq2_dq_doubleloop_fullspe_T));

  {
    int16_T i;

    /* Start for S-Function (c2802xadc): '<Root>/ADC-A Voltage (20 kHz)' */
    if (MW_adcAInitFlag == 0U) {
      InitAdcA();
      MW_adcAInitFlag = 1U;
    }

    config_ADCA_SOC0 ();

    /* Start for S-Function (c2802xpwm): '<Root>/ePWM1 Leg A (20 kHz)' */

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
      EPwm1Regs.TBPRD = 3000U;         // Time Base Period Register

      /* // Time-Base Phase Register
         EPwm1Regs.TBPHS.bit.TBPHS               = 0U;          // Phase offset register
       */
      EPwm1Regs.TBPHS.all = (EPwm1Regs.TBPHS.all & ~0xFFFF0000U) | 0x0U;

      // Time Base Counter Register
      EPwm1Regs.TBCTR = 0x0000U;       /* Clear counter*/

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
      EPwm1Regs.CMPA.bit.CMPA = 1500U; // Counter Compare A Register
      EPwm1Regs.CMPB.bit.CMPB = 32000U;// Counter Compare B Register
      EPwm1Regs.CMPC = 32000U;         // Counter Compare C Register
      EPwm1Regs.CMPD = 32000U;         // Counter Compare D Register

      /*-- Setup Action-Qualifier (AQ) Submodule --*/
      EPwm1Regs.AQCTLA.all = 144U;
                               // Action Qualifier Control Register For Output A
      EPwm1Regs.AQCTLB.all = 0U;
                               // Action Qualifier Control Register For Output B

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
      EPwm1Regs.TZSEL.all = 0U;        // Trip Zone Select Register

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
      EPwm1Regs.DCFOFFSET = 0U;        // Digital Compare Filter Offset Register
      EPwm1Regs.DCFWINDOW = 0U;        // Digital Compare Filter Window Register

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

    /* Start for S-Function (c2802xpwm): '<Root>/ePWM2 Leg B (20 kHz)' */

    /*** Initialize ePWM2 modules ***/
    {
      /*  // Time Base Control Register
         EPwm2Regs.TBCTL.bit.CTRMODE              = 2U;          // Counter Mode

         EPwm2Regs.TBCTL.bit.PRDLD                = 0U;          // Shadow select

         EPwm2Regs.TBCTL2.bit.PRDLDSYNC           = 0U;          // Shadow select

         EPwm2Regs.TBCTL.bit.PHSEN                = 1U;          // Phase Load Enable
         EPwm2Regs.TBCTL.bit.PHSDIR               = 1U;          // Phase Direction Bit
         EPwm2Regs.TBCTL.bit.HSPCLKDIV            = 0U;          // High Speed TBCLK Pre-scaler
         EPwm2Regs.TBCTL.bit.CLKDIV               = 0U;          // Time Base Clock Pre-scaler
       */
      EPwm2Regs.TBCTL.all = (EPwm2Regs.TBCTL.all & ~0x3FCFU) | 0x2006U;
      EPwm2Regs.TBCTL2.all = (EPwm2Regs.TBCTL2.all & ~0xC000U) | 0x0U;

      /* // Time-Base EPWMxSYNCOUT Source Enable Register
       */
      EPwm2Regs.EPWMSYNCOUTEN.bit.SWEN = 0U;// disable the SWEN enabled during reset
      EPwm2Regs.EPWMSYNCOUTEN.all = (EPwm2Regs.EPWMSYNCOUTEN.all & ~0x0U) | 0x0U;

      /*-- Setup Time-Base (TB) Submodule --*/
      EPwm2Regs.TBPRD = 3000U;         // Time Base Period Register

      /* // Time-Base Phase Register
         EPwm2Regs.TBPHS.bit.TBPHS               = 0U;          // Phase offset register
       */
      EPwm2Regs.TBPHS.all = (EPwm2Regs.TBPHS.all & ~0xFFFF0000U) | 0x0U;

      // Time Base Counter Register
      EPwm2Regs.TBCTR = 0x0000U;       /* Clear counter*/

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
      EPwm2Regs.CMPA.bit.CMPA = 1500U; // Counter Compare A Register
      EPwm2Regs.CMPB.bit.CMPB = 32000U;// Counter Compare B Register
      EPwm2Regs.CMPC = 32000U;         // Counter Compare C Register
      EPwm2Regs.CMPD = 32000U;         // Counter Compare D Register

      /*-- Setup Action-Qualifier (AQ) Submodule --*/
      EPwm2Regs.AQCTLA.all = 144U;
                               // Action Qualifier Control Register For Output A
      EPwm2Regs.AQCTLB.all = 0U;
                               // Action Qualifier Control Register For Output B

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
         EPwm2Regs.ETSEL.bit.SOCAEN               = 0U;          // Start of Conversion A Enable
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
      EPwm2Regs.ETSEL.all = (EPwm2Regs.ETSEL.all & ~0xFF7FU) | 0x1201U;
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
      EPwm2Regs.TZSEL.all = 0U;        // Trip Zone Select Register

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
      EPwm2Regs.DCFOFFSET = 0U;        // Digital Compare Filter Offset Register
      EPwm2Regs.DCFWINDOW = 0U;        // Digital Compare Filter Window Register

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

    /* Start for S-Function (c2802xadc): '<Root>/ADC-C Current (20 kHz)' */
    if (MW_adcCInitFlag == 0U) {
      InitAdcC();
      MW_adcCInitFlag = 1U;
    }

    config_ADCC_SOC0 ();

    /* InitializeConditions for DiscreteTransferFcn: '<Root>/Vd LPF' */
    xtq2_dq_doubleloop_fullspec_DW.VdLPF_states =
      xtq2_dq_doubleloop_fullspec_P.VdLPF_InitialStates;

    /* InitializeConditions for DiscreteIntegrator: '<S88>/Integrator' */
    xtq2_dq_doubleloop_fullspec_DW.Integrator_DSTATE =
      xtq2_dq_doubleloop_fullspec_P.daxisPIz_InitialConditionForInt;

    /* InitializeConditions for DiscreteTransferFcn: '<Root>/Id LPF' */
    xtq2_dq_doubleloop_fullspec_DW.IdLPF_states =
      xtq2_dq_doubleloop_fullspec_P.IdLPF_InitialStates;

    /* InitializeConditions for DiscreteIntegrator: '<S38>/Integrator' */
    xtq2_dq_doubleloop_fullspec_DW.Integrator_DSTATE_h =
      xtq2_dq_doubleloop_fullspec_P.daxisCurrentPIz_InitialConditio;

    /* InitializeConditions for DiscreteTransferFcn: '<Root>/Vq LPF' */
    xtq2_dq_doubleloop_fullspec_DW.VqLPF_states =
      xtq2_dq_doubleloop_fullspec_P.VqLPF_InitialStates;

    /* InitializeConditions for DiscreteIntegrator: '<S188>/Integrator' */
    xtq2_dq_doubleloop_fullspec_DW.Integrator_DSTATE_f =
      xtq2_dq_doubleloop_fullspec_P.qaxisPIz_InitialConditionForInt;

    /* InitializeConditions for DiscreteTransferFcn: '<Root>/Iq LPF' */
    xtq2_dq_doubleloop_fullspec_DW.IqLPF_states =
      xtq2_dq_doubleloop_fullspec_P.IqLPF_InitialStates;

    /* InitializeConditions for DiscreteIntegrator: '<S138>/Integrator' */
    xtq2_dq_doubleloop_fullspec_DW.Integrator_DSTATE_i =
      xtq2_dq_doubleloop_fullspec_P.qaxisCurrentPIz_InitialConditio;

    /* InitializeConditions for DiscreteTransferFcn: '<Root>/Capacitor current estimator' */
    xtq2_dq_doubleloop_fullspec_DW.Capacitorcurrentestimator_state =
      xtq2_dq_doubleloop_fullspec_P.Capacitorcurrentestimator_Initi;

    /* InitializeConditions for DiscreteTransferFcn: '<Root>/Damping high-pass' */
    xtq2_dq_doubleloop_fullspec_DW.Dampinghighpass_states =
      xtq2_dq_doubleloop_fullspec_P.Dampinghighpass_InitialStates;

    /* InitializeConditions for Delay: '<Root>/Current 90 deg delay (Ts)' */
    xtq2_dq_doubleloop_fullspec_DW.CircBufIdx = 0U;
    for (i = 0; i < 100; i++) {
      xtq2_dq_doubleloop_fullspec_DW.Current90degdelayTs_DSTATE[i] =
        xtq2_dq_doubleloop_fullspec_P.Current90degdelayTs_InitialCond;

      /* InitializeConditions for Delay: '<Root>/Voltage 90 deg delay (Ts)' incorporates:
       *  Delay: '<Root>/Current 90 deg delay (Ts)'
       */
      xtq2_dq_doubleloop_fullspec_DW.Voltage90degdelayTs_DSTATE[i] =
        xtq2_dq_doubleloop_fullspec_P.Voltage90degdelayTs_InitialCond;
    }

    /* End of InitializeConditions for Delay: '<Root>/Current 90 deg delay (Ts)' */

    /* InitializeConditions for Delay: '<Root>/Voltage 90 deg delay (Ts)' */
    xtq2_dq_doubleloop_fullspec_DW.CircBufIdx_f = 0U;
  }
}

/* Model terminate function */
void xtq2_dq_doubleloop_fullspec_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
