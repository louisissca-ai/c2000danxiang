/*
 * File: firmware_main.c
 * Description: F2800157 scheduler and safety adapter for the generated
 * single-phase inverter model.
 */

#include "xtq2_dq_doubleloop_fullspec.h"

#include <math.h>
#include <string.h>

#include "MW_target_hardware_resources.h"
#include "app_config.h"
#include "app_main.h"
#include "app_types.h"
#include "board.h"
#include "control_model_if.h"
#include "hmi_param.h"

#define CONTROL_TASK_1MS_DIVIDER   20u
#define CONTROL_INV_SQRT2          0.7071067811865475f

volatile int IsrOverrun = 0;
static boolean_T g_overrun_flag;
static uint16_t g_task_1ms_divider;
static uint16_t g_last_control_enabled;

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
        (real_T)HMI_Param_GetVoutAdcK();
    xtq2_dq_doubleloop_fullspec_P.Constant1_Value =
        -(real_T)HMI_Param_GetIoutAdcB();
    xtq2_dq_doubleloop_fullspec_P.Gain4_Gain =
        (real_T)HMI_Param_GetIoutAdcK();
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

static uint16_t Firmware_ConfigurePwmTiming(void)
{
    /*
     * TBCLKSYNC is still disabled here, so establish the application timing
     * instead of faulting on reset/generated-register values.
     */
    EALLOW;
    EPwm1Regs.TBPRD = APP_PWM_TBPRD_COUNTS;
    EPwm2Regs.TBPRD = APP_PWM_TBPRD_COUNTS;
    EPwm1Regs.TBCTR = 0u;
    EPwm2Regs.TBCTR = 0u;
    EPwm1Regs.CMPA.bit.CMPA = APP_PWM_TBPRD_COUNTS / 2u;
    EPwm2Regs.CMPA.bit.CMPA = APP_PWM_TBPRD_COUNTS / 2u;
    EPwm1Regs.DBRED.bit.DBRED = APP_PWM_DEADTIME_COUNTS;
    EPwm1Regs.DBFED.bit.DBFED = APP_PWM_DEADTIME_COUNTS;
    EPwm2Regs.DBRED.bit.DBRED = APP_PWM_DEADTIME_COUNTS;
    EPwm2Regs.DBFED.bit.DBFED = APP_PWM_DEADTIME_COUNTS;
    EDIS;

    return ((EPwm1Regs.TBPRD == APP_PWM_TBPRD_COUNTS) &&
        (EPwm2Regs.TBPRD == APP_PWM_TBPRD_COUNTS) &&
        (EPwm1Regs.DBRED.bit.DBRED == APP_PWM_DEADTIME_COUNTS) &&
        (EPwm1Regs.DBFED.bit.DBFED == APP_PWM_DEADTIME_COUNTS) &&
        (EPwm2Regs.DBRED.bit.DBRED == APP_PWM_DEADTIME_COUNTS) &&
        (EPwm2Regs.DBFED.bit.DBFED == APP_PWM_DEADTIME_COUNTS)) ?
        APP_TRUE : APP_FALSE;
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

static float Firmware_GetIoutRms(void)
{
    float id = (float)(xtq2_dq_doubleloop_fullspec_P.IdLPF_NumCoef *
        xtq2_dq_doubleloop_fullspec_DW.IdLPF_states);
    float iq = (float)(xtq2_dq_doubleloop_fullspec_P.IqLPF_NumCoef *
        xtq2_dq_doubleloop_fullspec_DW.IqLPF_states);

    return sqrtf(id * id + iq * iq) * CONTROL_INV_SQRT2;
}

void rt_OneStep(void)
{
    float iout_sample;
    float model_vref;
    float vref_ramp;
    float duty_percent;
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
        ControlModel_UpdateStoppedFeedback(
            (float)xtq2_dq_doubleloop_fullspec_P.Vin,
            vout_sample, iout_sample);
    }
    else
    {
        starting = (g_last_control_enabled == APP_FALSE) ? APP_TRUE : APP_FALSE;
        g_last_control_enabled = APP_TRUE;

        vref_ramp = ControlModel_GetVrefRamp();
        model_vref = ControlModel_UpdateVoutRmsReference(vref_ramp,
            (float)xtq2_dq_doubleloop_fullspec_DW.VoutInputLPF_state,
            (control_mode == APP_CONTROL_MODE_CLOSED_LOOP) ?
            APP_TRUE : APP_FALSE);
        xtq2_dq_doubleloop_fullspec_P.Vout_rms_ref = (control_mode ==
            APP_CONTROL_MODE_CLOSED_LOOP) ? (real_T)model_vref : 0.0;

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

        iout_sample = Firmware_AdcToSignal(
            xtq2_dq_doubleloop_fullspec_B.ADCCCurrent20kHz,
            xtq2_dq_doubleloop_fullspec_P.Gain1_Gain,
            xtq2_dq_doubleloop_fullspec_P.Constant1_Value,
            xtq2_dq_doubleloop_fullspec_P.Gain4_Gain);
        if (starting != APP_FALSE)
        {
            /*
             * Keep Trip Zone asserted for one complete control period so the
             * first post-RUN ADC result cannot arm or trip the output.
             */
            BoardPWM_ForceSafe();
            ControlModel_SetFeedback(
                (float)xtq2_dq_doubleloop_fullspec_P.Vin,
                ControlModel_GetRunningVoutRms(), 0.0f, 0.0f);
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
                    vout_rms, Firmware_GetIoutRms(), duty_percent);
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
    if (g_task_1ms_divider >= CONTROL_TASK_1MS_DIVIDER)
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
    c2000_flash_init();
    init_board();

    rtmSetErrorStatus(xtq2_dq_doubleloop_fullspec_M, 0);
    xtq2_dq_doubleloop_fullspec_initialize();
    ControlModel_Init();
    xtq2_dq_doubleloop_fullspec_P.Vin = (real_T)APP_VBUS_NOMINAL_V;

    globalInterruptDisable();
    runModel = ((rtmGetErrorStatus(xtq2_dq_doubleloop_fullspec_M) == NULL) &&
        (Firmware_ConfigurePwmTiming() != APP_FALSE));
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
