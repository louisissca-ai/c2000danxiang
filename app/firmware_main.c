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

#define CONTROL_ISR_PERIOD_S       5.0e-5f
#define CONTROL_TASK_1MS_DIVIDER   20u
#define CONTROL_SYSTEM_CLOCK_MHZ   120.0f
#define CONTROL_INV_SQRT2          0.7071067811865475f

volatile int IsrOverrun = 0;
static boolean_T g_overrun_flag;
static uint16_t g_task_1ms_divider;
static float g_last_iout_sample;

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
    float duty_percent;
    float vout_rms;

    if (g_overrun_flag != false)
    {
        IsrOverrun = 1;
        ControlModel_TripFault(FAULT_PWM);
        return;
    }

    g_overrun_flag = true;
    enableTimer0Interrupt();

    (void)ControlModel_UpdateSafety(
        (float)xtq2_dq_doubleloop_fullspec_P.Vin, g_last_iout_sample);
    xtq2_dq_doubleloop_fullspec_P.Vout_rms_ref =
        (real_T)ControlModel_GetVrefRamp();

    xtq2_dq_doubleloop_fullspec_step();

    g_last_iout_sample = Firmware_AdcToSignal(
        xtq2_dq_doubleloop_fullspec_B.ADCCCurrent20kHz,
        xtq2_dq_doubleloop_fullspec_P.Gain1_Gain,
        xtq2_dq_doubleloop_fullspec_P.Constant1_Value,
        xtq2_dq_doubleloop_fullspec_P.Gain4_Gain);
    (void)ControlModel_UpdateSafety(
        (float)xtq2_dq_doubleloop_fullspec_P.Vin, g_last_iout_sample);

    vout_rms = Firmware_GetVoutRms();
    duty_percent = Firmware_GetDutyPercent();
    ControlModel_SetFeedback((float)xtq2_dq_doubleloop_fullspec_P.Vin,
        vout_rms, Firmware_GetIoutRms(), duty_percent);

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
