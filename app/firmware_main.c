/*
 * File: firmware_main.c
 * Description: F2800157 scheduler and safety adapter for the generated
 * single-phase inverter model.
 */

#include "xtq2_dq_doubleloop_fullspec.h"

#include <math.h>
#include <string.h>

#include "MW_target_hardware_resources.h"
#include "adc_calibration.h"
#include "app_config.h"
#include "app_main.h"
#include "app_types.h"
#include "board.h"
#include "control_interface.h"
#include "control_model_if.h"
#include "hmi_param.h"
#include "pwm_profile.h"

volatile int IsrOverrun = 0;
static boolean_T g_overrun_flag;
static uint16_t g_task_1ms_divider;
static uint16_t g_last_control_enabled;
static const PWM_Profile_t *g_active_pwm_profile;

volatile boolean_T stopRequested;
volatile boolean_T runModel;

interrupt void Firmware_ADCC1_ISR(void);

static float Firmware_AdcToSignal(uint16_T raw, real_T adc_gain,
    real_T offset, real_T sensor_gain)
{
    return (float)((adc_gain * (real_T)raw + offset) * sensor_gain);
}

static float Firmware_GetDutyPercent(void)
{
    if (EPwm1Regs.TBPRD == 0u)
    {
        return 0.0f;
    }

    return 100.0f * (float)EPwm1Regs.CMPA.bit.CMPA /
        (float)EPwm1Regs.TBPRD;
}

static void Firmware_ApplyAdcCalibration(void)
{
    xtq2_dq_doubleloop_fullspec_P.Constant5_Value =
        -(real_T)HMI_Param_GetVoutAdcB();
    xtq2_dq_doubleloop_fullspec_P.Gain6_Gain =
        (real_T)HMI_Param_GetVoutAdcK() *
        (real_T)APP_VOUT_CONTROL_POLARITY;
    xtq2_dq_doubleloop_fullspec_P.Constant1_Value =
        -(real_T)HMI_Param_GetIoutAdcB();
    xtq2_dq_doubleloop_fullspec_P.Gain4_Gain =
        (real_T)HMI_Param_GetIoutAdcK() *
        (real_T)APP_IOUT_CONTROL_POLARITY;
}

static void Firmware_ResetControlState(void)
{
    /*
     * Generated DW contains every controller, filter, damping and quarter-cycle
     * delay state. This is only called after Trip Zone is asserted; ponytail:
     * if DW grows beyond the stopped-cycle budget, split the two delay arrays
     * across cycles instead of moving this memset back into the RUN path.
     */
    (void)memset(&xtq2_dq_doubleloop_fullspec_DW, 0,
        sizeof(xtq2_dq_doubleloop_fullspec_DW));
}

static void Firmware_ApplyOpenLoopPwm(float vref_rms, float vbus)
{
    float duty_a_percent;
    float duty_b_percent;

    ControlModel_GetOpenLoopDuty(vref_rms, vbus,
        &duty_a_percent, &duty_b_percent);
    EPwm1Regs.CMPA.bit.CMPA = ControlModel_ClampPwmCompare(
        EPwm1Regs.TBPRD, (uint16_T)((float)EPwm1Regs.TBPRD *
        duty_a_percent * 0.01f));
    EPwm2Regs.CMPA.bit.CMPA = ControlModel_ClampPwmCompare(
        EPwm2Regs.TBPRD, (uint16_T)((float)EPwm2Regs.TBPRD *
        duty_b_percent * 0.01f));
}

static void Firmware_ClampClosedLoopPwm(void)
{
    EPwm1Regs.CMPA.bit.CMPA = ControlModel_ClampPwmCompare(
        EPwm1Regs.TBPRD, EPwm1Regs.CMPA.bit.CMPA);
    EPwm2Regs.CMPA.bit.CMPA = ControlModel_ClampPwmCompare(
        EPwm2Regs.TBPRD, EPwm2Regs.CMPA.bit.CMPA);
}

static uint16_t Firmware_ConfigurePwmTiming(const PWM_Profile_t *profile)
{
    /*
     * TBCLKSYNC is still disabled here, so establish the application timing
     * instead of faulting on reset/generated-register values.
     */
    EALLOW;
    EPwm1Regs.TBPRD = profile->tbprd_counts;
    EPwm2Regs.TBPRD = profile->tbprd_counts;
    EPwm1Regs.TBCTR = 0u;
    EPwm2Regs.TBCTR = 0u;
    EPwm1Regs.CMPA.bit.CMPA = profile->tbprd_counts / 2u;
    EPwm2Regs.CMPA.bit.CMPA = profile->tbprd_counts / 2u;
    EPwm1Regs.DBRED.bit.DBRED = APP_PWM_DEADTIME_COUNTS;
    EPwm1Regs.DBFED.bit.DBFED = APP_PWM_DEADTIME_COUNTS;
    EPwm2Regs.DBRED.bit.DBRED = APP_PWM_DEADTIME_COUNTS;
    EPwm2Regs.DBFED.bit.DBFED = APP_PWM_DEADTIME_COUNTS;
    EDIS;

    return ((EPwm1Regs.TBPRD == profile->tbprd_counts) &&
        (EPwm2Regs.TBPRD == profile->tbprd_counts) &&
        (EPwm1Regs.DBRED.bit.DBRED == APP_PWM_DEADTIME_COUNTS) &&
        (EPwm1Regs.DBFED.bit.DBFED == APP_PWM_DEADTIME_COUNTS) &&
        (EPwm2Regs.DBRED.bit.DBRED == APP_PWM_DEADTIME_COUNTS) &&
        (EPwm2Regs.DBFED.bit.DBFED == APP_PWM_DEADTIME_COUNTS)) ?
        APP_TRUE : APP_FALSE;
}

static uint16_t Firmware_ApplyRequestedPwmFrequency(void)
{
    Control_Setpoint_t setpoint;
    const PWM_Profile_t *profile;

    ControlIF_GetSetpoint(&setpoint);
    if (setpoint.pwm_frequency_khz ==
        ControlModel_GetActivePwmFrequencyKhz())
    {
        return APP_TRUE;
    }

    profile = PWM_Profile_Get(setpoint.pwm_frequency_khz);
    if ((profile == 0) ||
        ((setpoint.mode_cmd == APP_CONTROL_MODE_CLOSED_LOOP) &&
         (setpoint.pwm_frequency_khz != APP_PWM_FREQUENCY_DEFAULT_KHZ)))
    {
        return APP_FALSE;
    }

    BoardPWM_ForceSafe();
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0u;
    EDIS;
    if (Firmware_ConfigurePwmTiming(profile) == APP_FALSE)
    {
        return APP_FALSE;
    }
    g_task_1ms_divider = 0u;
    if (ControlModel_ApplyPwmProfile(profile->frequency_khz) == APP_FALSE)
    {
        return APP_FALSE;
    }
    g_active_pwm_profile = profile;
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1u;
    EDIS;
    return APP_TRUE;
}

static void Firmware_ConfigureControlInterrupt(void)
{
    /*
     * EPWM1 SOCA starts ADCC SOC0 at CTR=PRD. EOC0 schedules the control
     * step with half a PWM period remaining before CMPA loads at CTR=ZERO.
     */
    EALLOW;
    PieVectTable.ADCC1_INT = &Firmware_ADCC1_ISR;
    AdccRegs.ADCINTSEL1N2.bit.INT1SEL = 0u;
    AdccRegs.ADCINTSEL1N2.bit.INT1CONT = 0u;
    AdccRegs.ADCINTSEL1N2.bit.INT1E = 1u;
    AdccRegs.ADCINTOVFCLR.bit.ADCINT1 = 1u;
    AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1u;
    EDIS;

    PieCtrlRegs.PIEIER1.bit.INTx2 = 1u;
    IER |= M_INT1;
}

void rt_OneStep(void)
{
    float iout_sample;
    float id;
    float iq;
    float vref_ramp;
    float duty_percent;
    float vd;
    float vq;
    float vout_rms;
    float vout_sample;
    uint16_t control_enabled;
    uint16_t control_mode;
    uint16_t starting;

    if (g_overrun_flag != false)
    {
        IsrOverrun = 1;
        ControlModel_TripFault(FAULT_PWM);
        return;
    }

    g_overrun_flag = true;

    Firmware_ApplyAdcCalibration();

    vout_sample = Firmware_AdcToSignal(AdcaResultRegs.ADCRESULT0,
        xtq2_dq_doubleloop_fullspec_P.Gain11_Gain,
        xtq2_dq_doubleloop_fullspec_P.Constant5_Value,
        xtq2_dq_doubleloop_fullspec_P.Gain6_Gain);
    iout_sample = Firmware_AdcToSignal(AdccResultRegs.ADCRESULT0,
        xtq2_dq_doubleloop_fullspec_P.Gain1_Gain,
        xtq2_dq_doubleloop_fullspec_P.Constant1_Value,
        xtq2_dq_doubleloop_fullspec_P.Gain4_Gain);
    control_enabled = ControlModel_IsPwmAllowed(
        (float)xtq2_dq_doubleloop_fullspec_P.Vin);
    control_mode = ControlModel_GetActiveMode();

    if (control_enabled == APP_FALSE)
    {
        if (g_last_control_enabled != APP_FALSE)
        {
            Firmware_ResetControlState();
        }
        g_last_control_enabled = APP_FALSE;
        if (Firmware_ApplyRequestedPwmFrequency() == APP_FALSE)
        {
            ControlModel_TripFault(FAULT_PWM);
        }
        ADC_Cal_PushStoppedRaw(AdcaResultRegs.ADCRESULT0,
            AdccResultRegs.ADCRESULT0,
            (float)xtq2_dq_doubleloop_fullspec_P.Gain11_Gain,
            (float)xtq2_dq_doubleloop_fullspec_P.Gain1_Gain);
        ControlModel_UpdateStoppedFeedback(
            (float)xtq2_dq_doubleloop_fullspec_P.Vin,
            vout_sample, iout_sample);
    }
    else
    {
        starting = (g_last_control_enabled == APP_FALSE) ? APP_TRUE : APP_FALSE;
        g_last_control_enabled = APP_TRUE;

        vref_ramp = ControlModel_GetVrefRamp();
        ControlModel_UpdateRunningRms(vout_sample, iout_sample);
        xtq2_dq_doubleloop_fullspec_P.Vout_rms_ref = (control_mode ==
            APP_CONTROL_MODE_CLOSED_LOOP) ? (real_T)vref_ramp : 0.0;

        xtq2_dq_doubleloop_fullspec_step();
        if (control_mode == APP_CONTROL_MODE_CLOSED_LOOP)
        {
            Firmware_ClampClosedLoopPwm();
        }
        else if (starting == APP_FALSE)
        {
            Firmware_ApplyOpenLoopPwm(vref_ramp,
                (float)xtq2_dq_doubleloop_fullspec_P.Vin);
        }

        if (ControlModel_GetActivePwmFrequencyKhz() ==
            APP_PWM_FREQUENCY_DEFAULT_KHZ)
        {
            vd = (float)(xtq2_dq_doubleloop_fullspec_P.VdLPF_NumCoef *
                xtq2_dq_doubleloop_fullspec_DW.VdLPF_states);
            vq = (float)(xtq2_dq_doubleloop_fullspec_P.VqLPF_NumCoef *
                xtq2_dq_doubleloop_fullspec_DW.VqLPF_states);
            id = (float)(xtq2_dq_doubleloop_fullspec_P.IdLPF_NumCoef *
                xtq2_dq_doubleloop_fullspec_DW.IdLPF_states);
            iq = (float)(xtq2_dq_doubleloop_fullspec_P.IqLPF_NumCoef *
                xtq2_dq_doubleloop_fullspec_DW.IqLPF_states);
        }
        else
        {
            vd = 0.0f;
            vq = 0.0f;
            id = 0.0f;
            iq = 0.0f;
        }
        if (starting != APP_FALSE)
        {
            /*
             * Keep Trip Zone asserted for one complete control period so the
             * first post-RUN ADC result cannot arm or trip the output.
             */
            BoardPWM_ForceSafe();
            ControlModel_SetFeedback(
                (float)xtq2_dq_doubleloop_fullspec_P.Vin,
                ControlModel_GetRunningVoutRms(), 0.0f,
                vout_sample, iout_sample, vd, vq, id, iq, 0.0f);
        }
        else
        {
            control_enabled = ControlModel_UpdateSafety(
                (float)xtq2_dq_doubleloop_fullspec_P.Vin, iout_sample);

            if (control_enabled != APP_FALSE)
            {
                BoardPWM_Release();
                vout_rms = ControlModel_GetRunningVoutRms();
                duty_percent = Firmware_GetDutyPercent();
                ControlModel_SetFeedback(
                    (float)xtq2_dq_doubleloop_fullspec_P.Vin,
                    vout_rms, ControlModel_GetRunningIoutRms(),
                    vout_sample, iout_sample, vd, vq, id, iq, duty_percent);
            }
            else
            {
                Firmware_ResetControlState();
                g_last_control_enabled = APP_FALSE;
                ControlModel_UpdateStoppedFeedback(
                    (float)xtq2_dq_doubleloop_fullspec_P.Vin,
                    vout_sample, iout_sample);
            }
        }
    }

    g_task_1ms_divider++;
    if (g_task_1ms_divider >= g_active_pwm_profile->task_1ms_divider)
    {
        g_task_1ms_divider = 0u;
        ControlModel_Task1ms();
    }

    g_overrun_flag = false;
}

interrupt void Firmware_ADCC1_ISR(void)
{
    rt_OneStep();

    /* A second EOC while ADCINT1 is still set means the control step overran. */
    if (AdccRegs.ADCINTOVF.bit.ADCINT1 != 0u)
    {
        /*
         * A full state clear may deliberately consume a stopped PWM period.
         * It is harmless while Trip Zone is asserted, but never ignore a
         * missed deadline while the control output is armed.
         */
        if (ControlModel_IsControlEnabled() != APP_FALSE)
        {
            IsrOverrun = 1;
            ControlModel_TripFault(FAULT_PWM);
        }
        AdccRegs.ADCINTOVFCLR.bit.ADCINT1 = 1u;
    }
    AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1u;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

int main(void)
{
    const PWM_Profile_t *default_profile;

    c2000_flash_init();
    init_board();

    rtmSetErrorStatus(xtq2_dq_doubleloop_fullspec_M, 0);
    xtq2_dq_doubleloop_fullspec_initialize();
    ControlModel_Init();
    default_profile = PWM_Profile_Get(APP_PWM_FREQUENCY_DEFAULT_KHZ);
    g_active_pwm_profile = default_profile;
    xtq2_dq_doubleloop_fullspec_P.Vin = (real_T)APP_VBUS_NOMINAL_V;
    xtq2_dq_doubleloop_fullspec_P.Kad =
        (real_T)APP_CONTROL_ACTIVE_DAMPING_GAIN;
    xtq2_dq_doubleloop_fullspec_P.Kp_i_dq =
        (real_T)APP_CONTROL_KP_I_DQ;
    xtq2_dq_doubleloop_fullspec_P.Ki_i_dq =
        (real_T)APP_CONTROL_KI_I_DQ;
    xtq2_dq_doubleloop_fullspec_P.Kp_v_dq =
        (real_T)APP_CONTROL_KP_V_DQ;
    xtq2_dq_doubleloop_fullspec_P.Ki_v_dq =
        (real_T)APP_CONTROL_KI_V_DQ;

    globalInterruptDisable();
    runModel = ((rtmGetErrorStatus(xtq2_dq_doubleloop_fullspec_M) == NULL) &&
        (default_profile != 0) &&
        (Firmware_ConfigurePwmTiming(default_profile) != APP_FALSE));
    if (runModel != false)
    {
        Firmware_ConfigureControlInterrupt();
        config_ePWM_TBSync();
    }
    else
    {
        ControlModel_TripFault(FAULT_PWM);
    }
    globalInterruptEnable();

    while (runModel != false)
    {
        APP_BackgroundLoop();
        stopRequested = (rtmGetErrorStatus(
            xtq2_dq_doubleloop_fullspec_M) != NULL);
        runModel = (stopRequested == false);
    }

    BoardPWM_ForceSafe();
    xtq2_dq_doubleloop_fullspec_terminate();
    globalInterruptDisable();
    return 0;
}
