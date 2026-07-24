#ifdef UNIT_TEST

#include <assert.h>
#include <math.h>

#include "adc_calibration.h"
#include "app_config.h"
#include "app_types.h"
#include "board.h"
#include "control_interface.h"
#include "control_model_if.h"
#include "hmi_menu.h"
#include "hmi_param.h"
#include "pwm_profile.h"

static uint16_t g_pwm_released;

void APP_Init(void)
{
}

void APP_TaskScheduler_1ms_ISR(void)
{
}

void BoardPWM_ForceSafe(void)
{
    g_pwm_released = 0u;
}

void BoardPWM_Release(void)
{
    g_pwm_released = 1u;
}

uint16_t BoardPWM_IsReleased(void)
{
    return g_pwm_released;
}

static void SetRunCommand(uint16_t enable_cmd, uint16_t mode_cmd)
{
    Control_Setpoint_t setpoint;

    setpoint.vref = 20.0f;
    setpoint.iref = 2.0f;
    setpoint.enable_cmd = enable_cmd;
    setpoint.mode_cmd = mode_cmd;
    ControlIF_SetSetpoint(&setpoint);
}

static void AssertNear(float actual, float expected, float tolerance)
{
    assert(fabsf(actual - expected) <= tolerance);
}

int main(void)
{
    ADC_Cal_View_t cal_view;
    Control_Feedback_t feedback;
    Control_Setpoint_t setpoint;
    HMI_Data_t hmi_data;
    float duty_a;
    float duty_b;
    float first_ramp;
    float initial_vref;
    float initial_calibration;
    float max_duty;
    float min_duty;
    uint16_t compare_a;
    uint16_t compare_b;
    uint16_t i;
    uint16_t raw_iout;
    uint16_t raw_vout;
    uint16_t window;
    const PWM_Profile_t *profile;

    ControlIF_Init();
    ControlIF_GetSetpoint(&setpoint);
    AssertNear(setpoint.iref, 3.0f, 0.0f);
    assert(APP_VREF_MAX > 23.0f);
    assert(APP_VREF_MAX < 24.0f);
    AssertNear(APP_ADC_CAL_VOUT_K_DEFAULT, 109.781873f, 0.00001f);
    AssertNear(APP_ADC_CAL_VOUT_K_DEFAULT,
        18.138269f * 19.368f / 3.2f, 0.0001f);
    AssertNear(APP_CONTROL_ACTIVE_DAMPING_GAIN,
        50.0f * 3.2f / 19.368f, 0.0001f);
    AssertNear(APP_CONTROL_KP_I_DQ, 0.001f * 6.2831853071795865f *
        800.0f, 0.00001f);
    AssertNear(APP_CONTROL_KI_I_DQ / APP_CONTROL_KP_I_DQ,
        25.0f, 0.0001f);
    AssertNear(APP_CONTROL_KP_V_DQ, 0.006f, 0.0f);
    AssertNear(APP_CONTROL_KI_V_DQ, 2.5f, 0.0f);
    profile = PWM_Profile_Get(20u);
    assert(profile != 0);
    assert(profile->tbprd_counts == 3000u);
    assert(profile->rms_samples == 400u);
    assert(profile->task_1ms_divider == 20u);
    assert(profile->calibration_settle_samples == 10000uL);
    assert(profile->calibration_window_samples == 400uL);
    AssertNear(profile->control_step_s, 50.0e-6f, 0.000000001f);
    assert(PWM_Profile_Get(17u) == 0);
    assert((APP_VOUT_CONTROL_POLARITY == 1.0f) ||
        (APP_VOUT_CONTROL_POLARITY == -1.0f));
    assert((APP_IOUT_CONTROL_POLARITY == 1.0f) ||
        (APP_IOUT_CONTROL_POLARITY == -1.0f));
    AssertNear(fabsf((1.600f - APP_ADC_CAL_IOUT_B_DEFAULT) *
        APP_ADC_CAL_IOUT_K_DEFAULT * APP_IOUT_CONTROL_POLARITY),
        fabsf((1.600f - APP_ADC_CAL_IOUT_B_DEFAULT) *
        APP_ADC_CAL_IOUT_K_DEFAULT), 0.00001f);

    /* Open and closed loop share the same final compare safety boundary. */
    assert(ControlModel_ClampPwmCompare(APP_PWM_TBPRD_COUNTS, 0u) ==
        APP_PWM_MIN_COMPARE_COUNTS);
    assert(ControlModel_ClampPwmCompare(APP_PWM_TBPRD_COUNTS,
        APP_PWM_MIN_COMPARE_COUNTS) == APP_PWM_MIN_COMPARE_COUNTS);
    assert(ControlModel_ClampPwmCompare(APP_PWM_TBPRD_COUNTS, 1500u) ==
        1500u);
    assert(ControlModel_ClampPwmCompare(APP_PWM_TBPRD_COUNTS, 3000u) ==
        (APP_PWM_TBPRD_COUNTS - APP_PWM_MIN_COMPARE_COUNTS));
    assert(ControlModel_ClampPwmCompare(80u, 0u) == 40u);

    ControlModel_Init();
    assert(ControlModel_GetActiveMode() == APP_CONTROL_MODE_CLOSED_LOOP);

    SetRunCommand(APP_FALSE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(0.0f, 0.0f) == APP_FALSE);
    assert(ControlModel_GetFaultCode() == FAULT_NONE);
    assert(BoardPWM_IsReleased() == 0u);

    ControlModel_TripFault(FAULT_PWM);
    assert(ControlModel_GetFaultCode() == FAULT_PWM);
    assert(BoardPWM_IsReleased() == 0u);

    SetRunCommand(APP_FALSE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_FALSE);
    assert(ControlModel_GetFaultCode() == FAULT_NONE);

    /* Fixed Vin is a model input, not a measured bus protection signal. */
    SetRunCommand(APP_TRUE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(19.9f, 0.0f) == APP_TRUE);
    assert(ControlModel_UpdateSafety(80.1f, 0.0f) == APP_TRUE);
    assert(ControlModel_GetFaultCode() == FAULT_NONE);
    assert(BoardPWM_IsReleased() == 0u);

    SetRunCommand(APP_FALSE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_FALSE);
    assert(ControlModel_GetFaultCode() == FAULT_NONE);

    /* Invalid model inputs still latch a safe shutdown. */
    SetRunCommand(APP_TRUE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(0.0f, 0.0f) == APP_FALSE);
    assert(ControlModel_GetFaultCode() == FAULT_ADC);
    SetRunCommand(APP_FALSE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_FALSE);

    SetRunCommand(APP_TRUE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(NAN, 0.0f) == APP_FALSE);
    assert(ControlModel_GetFaultCode() == FAULT_ADC);
    SetRunCommand(APP_FALSE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_FALSE);

    SetRunCommand(APP_TRUE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, NAN) == APP_FALSE);
    assert(ControlModel_GetFaultCode() == FAULT_ADC);
    SetRunCommand(APP_FALSE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_FALSE);

    /* Finite current does not trip while the temporary software OCP switch is off. */
    SetRunCommand(APP_TRUE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 4.0f) == APP_TRUE);
    assert(ControlModel_GetFaultCode() == FAULT_NONE);
    assert(BoardPWM_IsReleased() == 0u);
    SetRunCommand(APP_FALSE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_FALSE);

    /* Safety approval does not release PWM before the new duty is committed. */
    SetRunCommand(APP_TRUE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_TRUE);
    assert(BoardPWM_IsReleased() == 0u);
    first_ramp = ControlModel_GetVrefRamp();
    assert(first_ramp > 0.0f);
    BoardPWM_Release();
    assert(BoardPWM_IsReleased() != 0u);

    /* Every STOP resets the ramp and the next RUN starts from the first step. */
    SetRunCommand(APP_FALSE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_FALSE);
    assert(BoardPWM_IsReleased() == 0u);
    AssertNear(ControlModel_GetVrefRamp(), 0.0f, 0.0f);
    SetRunCommand(APP_TRUE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_TRUE);
    assert(BoardPWM_IsReleased() == 0u);
    AssertNear(ControlModel_GetVrefRamp(), first_ramp, 0.000001f);
    SetRunCommand(APP_FALSE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_FALSE);

    /* One 400-sample 20 Vrms sine cycle measures 20 Vrms. */
    SetRunCommand(APP_TRUE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_TRUE);
    for (i = 0u; i < 400u; i++)
    {
        ControlModel_UpdateRunningRms(
            20.0f * 1.4142135623730951f *
            sinf(6.2831853071795865f * (float)i / 400.0f),
            2.0f * 1.4142135623730951f *
            sinf(6.2831853071795865f * (float)i / 400.0f));
    }
    AssertNear(ControlModel_GetRunningVoutRms(), 20.0f, 0.0001f);
    AssertNear(ControlModel_GetRunningIoutRms(), 2.0f, 0.0001f);

    /* RMS measurement has no path back into the closed-loop reference. */
    for (i = 0u; i < 400u; i++)
    {
        ControlModel_UpdateRunningRms(26.0f, 2.6f);
    }
    AssertNear(ControlModel_GetRunningVoutRms(), 26.0f, 0.0001f);
    for (i = 0u; i < 400u; i++)
    {
        ControlModel_UpdateRunningRms(10.0f, 1.0f);
    }
    AssertNear(ControlModel_GetRunningVoutRms(), 10.0f, 0.0001f);
    ControlIF_GetSetpoint(&setpoint);
    AssertNear(setpoint.vref, 20.0f, 0.0f);

    /* STOP clears the running RMS measurement. */
    SetRunCommand(APP_FALSE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_FALSE);
    AssertNear(ControlModel_GetRunningVoutRms(), 0.0f, 0.0f);

    /* Repeated low readings remain measurement-only and cannot accumulate. */
    SetRunCommand(APP_TRUE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_TRUE);
    for (i = 0u; i < 4000u; i++)
    {
        ControlModel_UpdateRunningRms(19.9f, 1.9f);
    }
    AssertNear(ControlModel_GetRunningVoutRms(), 19.9f, 0.0001f);
    ControlIF_GetSetpoint(&setpoint);
    AssertNear(setpoint.vref, 20.0f, 0.0f);
    SetRunCommand(APP_FALSE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_FALSE);

    /* STOP feedback is a 400-sample RMS and always reports zero duty. */
    for (i = 0u; i < 399u; i++)
    {
        ControlModel_UpdateStoppedFeedback(36.0f, 3.0f, -4.0f);
    }
    ControlIF_GetFeedback(&feedback);
    AssertNear(feedback.vout, 0.0f, 0.0f);
    AssertNear(feedback.iout, 0.0f, 0.0f);
    AssertNear(feedback.vout_inst, 3.0f, 0.0f);
    AssertNear(feedback.iout_inst, -4.0f, 0.0f);
    ControlModel_UpdateStoppedFeedback(36.0f, 3.0f, -4.0f);
    ControlIF_GetFeedback(&feedback);
    AssertNear(feedback.vout, 3.0f, 0.00001f);
    AssertNear(feedback.iout, 4.0f, 0.00001f);
    AssertNear(feedback.vout_inst, 3.0f, 0.0f);
    AssertNear(feedback.iout_inst, -4.0f, 0.0f);
    AssertNear(feedback.duty, 0.0f, 0.0f);
    assert(feedback.run_state == APP_RUN_STATE_STOP);

    /* Signed instantaneous samples stay separate from RMS measurements. */
    ControlModel_SetFeedback(36.0f, 20.0f, 2.0f,
        -28.3f, -2.5f, 14.1f, 0.2f, 1.0f, -0.1f, 50.0f);
    ControlIF_GetFeedback(&feedback);
    AssertNear(feedback.vout, 20.0f, 0.0f);
    AssertNear(feedback.iout, 2.0f, 0.0f);
    AssertNear(feedback.vout_inst, -28.3f, 0.0f);
    AssertNear(feedback.iout_inst, -2.5f, 0.0f);
    AssertNear(feedback.vd, 14.1f, 0.0f);
    AssertNear(feedback.vq, 0.2f, 0.0f);
    AssertNear(feedback.id, 1.0f, 0.0f);
    AssertNear(feedback.iq, -0.1f, 0.0f);

    HMI_Param_Init();
    HMI_Param_SetVout(20.0f);
    HMI_Param_SetIout(2.0f);
    HMI_Param_SetVoutInst(-28.3f);
    HMI_Param_SetIoutInst(-2.5f);
    HMI_Param_SetDq(14.1f, 0.2f, 1.0f, -0.1f);
    HMI_Param_GetData(&hmi_data);
    AssertNear(hmi_data.vout, 20.0f, 0.0f);
    AssertNear(hmi_data.iout, 2.0f, 0.0f);
    AssertNear(hmi_data.vout_inst, -28.3f, 0.0f);
    AssertNear(hmi_data.iout_inst, -2.5f, 0.0f);
    AssertNear(hmi_data.vd, 14.1f, 0.0f);
    AssertNear(hmi_data.vq, 0.2f, 0.0f);
    AssertNear(hmi_data.id, 1.0f, 0.0f);
    AssertNear(hmi_data.iq, -0.1f, 0.0f);

    /* A mode request while running is rejected until a stopped command arrives. */
    SetRunCommand(APP_TRUE, APP_CONTROL_MODE_OPEN_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_FALSE);
    assert(ControlModel_GetActiveMode() == APP_CONTROL_MODE_CLOSED_LOOP);
    assert(BoardPWM_IsReleased() == 0u);

    SetRunCommand(APP_FALSE, APP_CONTROL_MODE_OPEN_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_FALSE);
    assert(ControlModel_GetActiveMode() == APP_CONTROL_MODE_OPEN_LOOP);
    SetRunCommand(APP_TRUE, APP_CONTROL_MODE_OPEN_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_TRUE);
    assert(ControlModel_GetVrefRamp() > 0.0f);

    /* 20 Vrms from 36 V gives m=0.7857 and complementary unipolar duties. */
    ControlModel_GetOpenLoopDuty(20.0f, 36.0f, &duty_a, &duty_b);
    AssertNear(duty_a, 50.0f, 0.01f);
    AssertNear(duty_b, 50.0f, 0.01f);
    for (i = 0u; i < 99u; i++)
    {
        ControlModel_GetOpenLoopDuty(20.0f, 36.0f, &duty_a, &duty_b);
    }
    ControlModel_GetOpenLoopDuty(20.0f, 36.0f, &duty_a, &duty_b);
    AssertNear(duty_a, 89.2837f, 0.02f);
    AssertNear(duty_b, 10.7163f, 0.02f);
    AssertNear(duty_a + duty_b, 100.0f, 0.01f);
    for (i = 0u; i < 199u; i++)
    {
        ControlModel_GetOpenLoopDuty(20.0f, 36.0f, &duty_a, &duty_b);
    }
    ControlModel_GetOpenLoopDuty(20.0f, 36.0f, &duty_a, &duty_b);
    AssertNear(duty_a, 10.7163f, 0.02f);
    AssertNear(duty_b, 89.2837f, 0.02f);

    /* Stop resets phase; 400 enabled calls make one complete 50 Hz cycle. */
    SetRunCommand(APP_FALSE, APP_CONTROL_MODE_OPEN_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_FALSE);
    AssertNear(ControlModel_GetVrefRamp(), 0.0f, 0.0f);
    ControlModel_GetOpenLoopDuty(20.0f, 36.0f, &duty_a, &duty_b);
    AssertNear(duty_a, 50.0f, 0.01f);
    SetRunCommand(APP_TRUE, APP_CONTROL_MODE_OPEN_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_TRUE);
    for (i = 0u; i < 400u; i++)
    {
        ControlModel_GetOpenLoopDuty(20.0f, 36.0f, &duty_a, &duty_b);
        AssertNear(duty_a + duty_b, 100.0f, 0.01f);
    }
    ControlModel_GetOpenLoopDuty(20.0f, 36.0f, &duty_a, &duty_b);
    AssertNear(duty_a, 50.0f, 0.02f);
    AssertNear(duty_b, 50.0f, 0.02f);

    /*
     * Unreachable RMS requests retain a sinusoidal reference while reserving
     * dead time plus one minimum effective pulse at both compare limits.
     */
    SetRunCommand(APP_FALSE, APP_CONTROL_MODE_OPEN_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_FALSE);
    SetRunCommand(APP_TRUE, APP_CONTROL_MODE_OPEN_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_TRUE);
    min_duty = 100.0f;
    max_duty = 0.0f;
    for (i = 0u; i < 400u; i++)
    {
        ControlModel_GetOpenLoopDuty(100.0f, 36.0f, &duty_a, &duty_b);
        compare_a = (uint16_t)((float)APP_PWM_TBPRD_COUNTS *
            duty_a * 0.01f);
        compare_b = (uint16_t)((float)APP_PWM_TBPRD_COUNTS *
            duty_b * 0.01f);
        assert(compare_a >= APP_PWM_MIN_COMPARE_COUNTS);
        assert(compare_a <=
            (APP_PWM_TBPRD_COUNTS - APP_PWM_MIN_COMPARE_COUNTS));
        assert(compare_b >= APP_PWM_MIN_COMPARE_COUNTS);
        assert(compare_b <=
            (APP_PWM_TBPRD_COUNTS - APP_PWM_MIN_COMPARE_COUNTS));
        AssertNear(duty_a + duty_b, 100.0f, 0.01f);
        if (duty_a < min_duty)
        {
            min_duty = duty_a;
        }
        if (duty_a > max_duty)
        {
            max_duty = duty_a;
        }
    }
    AssertNear(min_duty, 1.6f, 0.02f);
    AssertNear(max_duty, 98.4f, 0.02f);

    SetRunCommand(APP_FALSE, APP_CONTROL_MODE_OPEN_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_FALSE);
    ControlModel_GetOpenLoopDuty(NAN, 36.0f, &duty_a, &duty_b);
    AssertNear(duty_a, 50.0f, 0.0f);
    AssertNear(duty_b, 50.0f, 0.0f);
    ControlModel_GetOpenLoopDuty(20.0f, NAN, &duty_a, &duty_b);
    AssertNear(duty_a, 50.0f, 0.0f);
    AssertNear(duty_b, 50.0f, 0.0f);

    /* Invalid modes never release PWM and never replace the active mode. */
    SetRunCommand(APP_FALSE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_FALSE);
    SetRunCommand(APP_TRUE, 2u);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_FALSE);
    assert(ControlModel_GetActiveMode() == APP_CONTROL_MODE_CLOSED_LOOP);
    assert(BoardPWM_IsReleased() == 0u);

    HMI_Param_Init();
    HMI_Menu_Init();
    HMI_Param_SetVref(100.0f);
    AssertNear(HMI_Param_GetVref(), APP_VREF_MAX, 0.00001f);
    HMI_Param_SetVref(APP_VREF_DEFAULT);
    HMI_Param_SetFaultCode(FAULT_UVLO);
    HMI_Menu_Task_20ms(KEY_EVENT_RIGHT);
    assert(HMI_Menu_GetPage() == HMI_MENU_PAGE_DQ_MONITOR);
    HMI_Menu_Task_20ms(KEY_EVENT_RIGHT);
    assert(HMI_Menu_GetPage() == HMI_MENU_PAGE_SET_VREF);

    initial_vref = HMI_Param_GetVref();
    HMI_Menu_Task_20ms(KEY_EVENT_UP);
    assert(HMI_Param_GetVref() > initial_vref);

    HMI_Menu_Task_20ms(KEY_EVENT_RUN);
    assert(HMI_Param_GetEnableCmd() == APP_TRUE);

    HMI_Menu_Task_20ms(KEY_EVENT_RIGHT);
    HMI_Menu_Task_20ms(KEY_EVENT_RIGHT);
    HMI_Menu_Task_20ms(KEY_EVENT_RIGHT);
    assert(HMI_Menu_GetPage() == HMI_MENU_PAGE_SET_MODE);
    HMI_Menu_Task_20ms(KEY_EVENT_UP);
    assert(HMI_Param_GetModeCmd() == APP_CONTROL_MODE_OPEN_LOOP);
    assert(HMI_Param_GetEnableCmd() == APP_FALSE);
    HMI_Menu_Task_20ms(KEY_EVENT_DOWN);
    assert(HMI_Param_GetModeCmd() == APP_CONTROL_MODE_CLOSED_LOOP);
    HMI_Param_SetModeCmd(3u);
    assert(HMI_Param_GetModeCmd() == APP_CONTROL_MODE_CLOSED_LOOP);

    HMI_Menu_Task_20ms(KEY_EVENT_RIGHT);
    assert(HMI_Menu_GetPage() == HMI_MENU_PAGE_ADC_CAL);
    AssertNear(APP_ADC_CAL_VOUT_B_DEFAULT, 1.643f, 0.00001f);
    AssertNear(APP_ADC_CAL_IOUT_B_DEFAULT, 1.643f, 0.00001f);
    AssertNear(HMI_Param_GetVoutAdcB(), APP_ADC_CAL_VOUT_B_DEFAULT, 0.00001f);
    AssertNear(HMI_Param_GetVoutAdcK(), APP_ADC_CAL_VOUT_K_DEFAULT, 0.00001f);

    /* The retained manual fields only change after entering edit mode. */
    HMI_Menu_Task_20ms(KEY_EVENT_DOWN);
    HMI_Menu_Task_20ms(KEY_EVENT_DOWN);
    HMI_Menu_Task_20ms(KEY_EVENT_DOWN);
    assert(HMI_Menu_GetEditIndex() == 3u);
    HMI_Menu_Task_20ms(KEY_EVENT_OK);
    assert(HMI_Menu_IsCalEditing() != 0u);
    initial_calibration = HMI_Param_GetVoutAdcB();
    HMI_Param_SetEnableCmd(APP_TRUE);
    HMI_Menu_Task_20ms(KEY_EVENT_UP);
    AssertNear(HMI_Param_GetVoutAdcB(), initial_calibration + 0.001f,
        0.00001f);
    assert(HMI_Param_GetEnableCmd() == APP_FALSE);
    HMI_Menu_Task_20ms(KEY_EVENT_OK);
    assert(HMI_Menu_IsCalEditing() == 0u);

    /*
     * Offset calibration ignores the first 0.5 s, trims ten low and ten high
     * block means independently, then averages every retained raw sample.
     */
    HMI_Param_Init();
    ADC_Cal_Init();
    HMI_Param_SetFaultCode(FAULT_NONE);
    HMI_Param_SetRunState(APP_RUN_STATE_STOP);
    assert(ADC_Cal_StartOffset() != APP_FALSE);
    for (i = 0u; i < 50400u; i++)
    {
        ADC_Cal_PushStoppedRaw(2048u, 2048u,
            3.3f / 4095.0f, 3.3f / 4095.0f);
    }
    ADC_Cal_GetView(&cal_view);
    assert(cal_view.state == ADC_CAL_STATE_OFFSET_COMPUTING);
    assert(cal_view.progress_percent == 100u);
    ADC_Cal_Cancel();

    HMI_Param_Init();
    ADC_Cal_Init();
    HMI_Param_SetFaultCode(FAULT_NONE);
    HMI_Param_SetRunState(APP_RUN_STATE_STOP);
    assert(ADC_Cal_StartOffset() != APP_FALSE);
    for (i = 0u; i < 50400u; i++)
    {
        if (i < 10000u)
        {
            raw_vout = 4095u;
            raw_iout = 0u;
        }
        else
        {
            window = (uint16_t)((i - 10000u) / 400u);
            if (window < 10u)
            {
                raw_vout = 1900u;
                raw_iout = 1800u;
            }
            else if (window >= 91u)
            {
                raw_vout = 2200u;
                raw_iout = 2300u;
            }
            else
            {
                raw_vout = (uint16_t)(2048u + (i & 1u));
                raw_iout = (uint16_t)(2038u + (i & 1u));
            }
        }
        ADC_Cal_PushStoppedRaw(raw_vout, raw_iout,
            3.3f / 4095.0f, 3.3f / 4095.0f);
    }
    ADC_Cal_GetView(&cal_view);
    assert(cal_view.state == ADC_CAL_STATE_OFFSET_COMPUTING);
    assert(cal_view.progress_percent == 100u);
    AssertNear(HMI_Param_GetVoutAdcB(), APP_ADC_CAL_VOUT_B_DEFAULT, 0.0f);
    ControlModel_SetFeedback(36.0f, 0.0f, 0.0f,
        -1.0f, -2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    ADC_Cal_Task1ms();
    AssertNear(HMI_Param_GetVoutAdcB(), (2048.5f * 3.3f / 4095.0f),
        0.000001f);
    AssertNear(HMI_Param_GetIoutAdcB(), (2038.5f * 3.3f / 4095.0f),
        0.000001f);

    /* Cancel and invalid offset results never replace the live parameters. */
    initial_calibration = HMI_Param_GetVoutAdcB();
    assert(ADC_Cal_StartOffset() != APP_FALSE);
    for (i = 0u; i < 100u; i++)
    {
        ADC_Cal_PushStoppedRaw(1000u, 1000u,
            3.3f / 4095.0f, 3.3f / 4095.0f);
    }
    ADC_Cal_Cancel();
    ADC_Cal_GetView(&cal_view);
    assert(cal_view.state == ADC_CAL_STATE_IDLE);
    AssertNear(HMI_Param_GetVoutAdcB(), initial_calibration, 0.0f);

    assert(ADC_Cal_StartOffset() != APP_FALSE);
    for (i = 0u; i < 50400u; i++)
    {
        ADC_Cal_PushStoppedRaw(65535u, 65535u,
            3.3f / 4095.0f, 3.3f / 4095.0f);
    }
    ADC_Cal_Task1ms();
    ADC_Cal_GetView(&cal_view);
    assert(cal_view.state == ADC_CAL_STATE_ERROR);
    assert(cal_view.error == ADC_CAL_ERROR_RANGE);
    AssertNear(HMI_Param_GetVoutAdcB(), initial_calibration, 0.0f);

    /* V and I gains calibrate independently and apply only after STOP. */
    initial_calibration = HMI_Param_GetVoutAdcK();
    HMI_Param_SetModeCmd(APP_CONTROL_MODE_OPEN_LOOP);
    HMI_Param_SetRunState(APP_RUN_STATE_RUN);
    assert(ADC_Cal_ApplyGain(ADC_CAL_CHANNEL_VOUT, 20.0f, 10.0f) !=
        APP_FALSE);
    AssertNear(HMI_Param_GetVoutAdcK(), initial_calibration, 0.0f);
    ControlModel_SetFeedback(36.0f, 0.0f, 0.0f,
        -1.0f, -2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    ADC_Cal_Task1ms();
    AssertNear(HMI_Param_GetVoutAdcK(), initial_calibration * 2.0f,
        0.0001f);
    AssertNear(HMI_Param_GetIoutAdcK(), APP_ADC_CAL_IOUT_K_DEFAULT,
        0.00001f);

    initial_calibration = HMI_Param_GetIoutAdcK();
    HMI_Param_SetModeCmd(APP_CONTROL_MODE_OPEN_LOOP);
    HMI_Param_SetRunState(APP_RUN_STATE_RUN);
    assert(ADC_Cal_ApplyGain(ADC_CAL_CHANNEL_IOUT, 2.0f, 1.0f) !=
        APP_FALSE);
    ControlModel_SetFeedback(36.0f, 0.0f, 0.0f,
        -1.0f, -2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    ADC_Cal_Task1ms();
    AssertNear(HMI_Param_GetIoutAdcK(), initial_calibration * 2.0f,
        0.0001f);

    return 0;
}

#endif
