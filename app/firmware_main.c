/*
 * File: firmware_main.c
 * Description: F2800157 scheduler and safety adapter for the generated
 * single-phase inverter model.
 */

#include "xtq2_dq_doubleloop_fullspec.h"

#include <math.h>

#include "MW_target_hardware_resources.h"
#include "app_main.h"
#include "app_types.h"
#include "board.h"
#include "control_model_if.h"
#include "hmi_param.h"

#define CONTROL_ISR_PERIOD_S       5.0e-5f
#define CONTROL_TASK_1MS_DIVIDER   20u
#define CONTROL_SYSTEM_CLOCK_MHZ   120.0f
#define CONTROL_INV_SQRT2          0.7071067811865475f

volatile int IsrOverrun = 0;
static boolean_T g_overrun_flag;
static uint16_t g_task_1ms_divider;
static uint16_t g_last_control_enabled;

volatile boolean_T stopRequested;
volatile boolean_T runModel;

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

static void Firmware_ResetClosedLoopControllers(void)
{
    xtq2_dq_doubleloop_fullspec_DW.Integrator_DSTATE = 0.0;
    xtq2_dq_doubleloop_fullspec_DW.Integrator_DSTATE_h = 0.0;
    xtq2_dq_doubleloop_fullspec_DW.Integrator_DSTATE_f = 0.0;
    xtq2_dq_doubleloop_fullspec_DW.Integrator_DSTATE_i = 0.0;
}

static void Firmware_ApplyOpenLoopPwm(float vref_rms, float vbus)
{
    float duty_a_percent;
    float duty_b_percent;

    ControlModel_GetOpenLoopDuty(vref_rms, vbus,
        &duty_a_percent, &duty_b_percent);
    EPwm1Regs.CMPA.bit.CMPA = (uint16_T)((float)EPwm1Regs.TBPRD *
        duty_a_percent * 0.01f);
    EPwm2Regs.CMPA.bit.CMPA = (uint16_T)((float)EPwm2Regs.TBPRD *
        duty_b_percent * 0.01f);
}

static float Firmware_GetVoutRms(void)
{
    float vd = (float)xtq2_dq_doubleloop_fullspec_DW.VdLPF_states;
    float vq = (float)xtq2_dq_doubleloop_fullspec_DW.VqLPF_states;

    return sqrtf(vd * vd + vq * vq) * CONTROL_INV_SQRT2;
}

static float Firmware_GetIoutRms(void)
{
    float id = (float)xtq2_dq_doubleloop_fullspec_DW.IdLPF_states;
    float iq = (float)xtq2_dq_doubleloop_fullspec_DW.IqLPF_states;

    return sqrtf(id * id + iq * iq) * CONTROL_INV_SQRT2;
}

void rt_OneStep(void)
{
    float iout_sample;
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
    enableTimer0Interrupt();

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
        g_last_control_enabled = APP_FALSE;
        ControlModel_UpdateStoppedFeedback(
            (float)xtq2_dq_doubleloop_fullspec_P.Vin,
            vout_sample, iout_sample);
    }
    else
    {
        starting = (g_last_control_enabled == APP_FALSE) ? APP_TRUE : APP_FALSE;
        if (starting != APP_FALSE)
        {
            Firmware_ResetClosedLoopControllers();
        }
        g_last_control_enabled = APP_TRUE;

        vref_ramp = ControlModel_GetVrefRamp();
        xtq2_dq_doubleloop_fullspec_P.Vout_rms_ref = (control_mode ==
            APP_CONTROL_MODE_CLOSED_LOOP) ? (real_T)vref_ramp : 0.0;

        xtq2_dq_doubleloop_fullspec_step();
        if ((control_mode == APP_CONTROL_MODE_OPEN_LOOP) &&
            (starting == APP_FALSE))
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
            ControlModel_UpdateStoppedFeedback(
                (float)xtq2_dq_doubleloop_fullspec_P.Vin,
                vout_sample, iout_sample);
        }
        else
        {
            control_enabled = ControlModel_UpdateSafety(
                (float)xtq2_dq_doubleloop_fullspec_P.Vin, iout_sample);

            if (control_enabled != APP_FALSE)
            {
                BoardPWM_Release();
                vout_rms = Firmware_GetVoutRms();
                duty_percent = Firmware_GetDutyPercent();
                ControlModel_SetFeedback(
                    (float)xtq2_dq_doubleloop_fullspec_P.Vin,
                    vout_rms, Firmware_GetIoutRms(), duty_percent);
            }
            else
            {
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

    disableTimer0Interrupt();
    g_overrun_flag = false;
}

int main(void)
{
    c2000_flash_init();
    init_board();

    rtmSetErrorStatus(xtq2_dq_doubleloop_fullspec_M, 0);
    xtq2_dq_doubleloop_fullspec_initialize();
    ControlModel_Init();

    globalInterruptDisable();
    configureTimer0(CONTROL_ISR_PERIOD_S, CONTROL_SYSTEM_CLOCK_MHZ);
    runModel = (rtmGetErrorStatus(xtq2_dq_doubleloop_fullspec_M) == NULL);
    enableTimer0Interrupt();
    config_ePWM_TBSync();
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
