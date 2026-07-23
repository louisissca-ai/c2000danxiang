#ifdef UNIT_TEST

#include <assert.h>
#include <math.h>

#include "app_config.h"
#include "app_types.h"
#include "board.h"
#include "control_interface.h"
#include "control_model_if.h"
#include "hmi_menu.h"
#include "hmi_param.h"

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
    Control_Feedback_t feedback;
    Control_Setpoint_t setpoint;
    float duty_a;
    float duty_b;
    float first_ramp;
    float initial_vref;
    float initial_calibration;
    float max_duty;
    float min_duty;
    float reference;
    uint16_t compare_a;
    uint16_t compare_b;
    uint16_t i;

    ControlIF_Init();
    ControlIF_GetSetpoint(&setpoint);
    AssertNear(setpoint.iref, 3.0f, 0.0f);
    assert(APP_VREF_MAX > 23.0f);
    assert(APP_VREF_MAX < 24.0f);
    AssertNear(APP_ADC_CAL_VOUT_K_DEFAULT, 109.781873f, 0.00001f);
    AssertNear(APP_ADC_CAL_VOUT_K_DEFAULT,
        18.138269f * 19.368f / 3.2f, 0.0001f);

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
        reference = ControlModel_UpdateVoutRmsReference(20.0f,
            20.0f * 1.4142135623730951f *
            sinf(6.2831853071795865f * (float)i / 400.0f), APP_TRUE);
    }
    AssertNear(ControlModel_GetRunningVoutRms(), 20.0f, 0.0001f);
    AssertNear(reference, 20.0f, 0.0001f);

    /* A 26 Vrms cycle can reduce the internal reference only 0.25 V. */
    for (i = 0u; i < 400u; i++)
    {
        reference = ControlModel_UpdateVoutRmsReference(
            20.0f, 26.0f, APP_TRUE);
    }
    AssertNear(ControlModel_GetRunningVoutRms(), 26.0f, 0.0001f);
    AssertNear(reference, 19.75f, 0.0001f);
    for (i = 0u; i < 400u; i++)
    {
        reference = ControlModel_UpdateVoutRmsReference(
            20.0f, 26.0f, APP_TRUE);
    }
    AssertNear(reference, 19.50f, 0.0001f);

    /* STOP clears both measured RMS and the supervisory correction. */
    SetRunCommand(APP_FALSE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_FALSE);
    AssertNear(ControlModel_GetRunningVoutRms(), 0.0f, 0.0f);
    reference = ControlModel_UpdateVoutRmsReference(
        20.0f, 0.0f, APP_TRUE);
    AssertNear(reference, 20.0f, 0.0f);

    /* Low RMS raises the reference, while both capability limits unwind. */
    SetRunCommand(APP_TRUE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_TRUE);
    for (i = 1u; i < 400u; i++)
    {
        reference = ControlModel_UpdateVoutRmsReference(
            20.0f, 10.0f, APP_TRUE);
    }
    AssertNear(reference, 20.25f, 0.0001f);
    SetRunCommand(APP_FALSE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_FALSE);
    SetRunCommand(APP_TRUE, APP_CONTROL_MODE_CLOSED_LOOP);
    assert(ControlModel_UpdateSafety(36.0f, 0.0f) == APP_TRUE);
    for (i = 0u; i < 400u; i++)
    {
        reference = ControlModel_UpdateVoutRmsReference(
            APP_VREF_MAX, 0.0f, APP_TRUE);
    }
    AssertNear(reference, APP_VREF_MAX, 0.0001f);
    for (i = 0u; i < 400u; i++)
    {
        reference = ControlModel_UpdateVoutRmsReference(
            0.0f, 26.0f, APP_TRUE);
    }
    AssertNear(reference, 0.0f, 0.0001f);

    /* Open loop still measures RMS but never retains a reference correction. */
    for (i = 0u; i < 400u; i++)
    {
        reference = ControlModel_UpdateVoutRmsReference(
            20.0f, 26.0f, APP_FALSE);
    }
    AssertNear(reference, 20.0f, 0.0f);
    AssertNear(ControlModel_GetRunningVoutRms(), 26.0f, 0.0001f);
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
    ControlModel_UpdateStoppedFeedback(36.0f, 3.0f, -4.0f);
    ControlIF_GetFeedback(&feedback);
    AssertNear(feedback.vout, 3.0f, 0.00001f);
    AssertNear(feedback.iout, 4.0f, 0.00001f);
    AssertNear(feedback.duty, 0.0f, 0.0f);
    assert(feedback.run_state == APP_RUN_STATE_STOP);

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
    initial_calibration = HMI_Param_GetVoutAdcB();
    HMI_Param_SetEnableCmd(APP_TRUE);
    HMI_Menu_Task_20ms(KEY_EVENT_UP);
    AssertNear(HMI_Param_GetVoutAdcB(), initial_calibration + 0.001f, 0.00001f);
    assert(HMI_Param_GetEnableCmd() == APP_FALSE);
    HMI_Menu_Task_20ms(KEY_EVENT_LONG_UP);
    AssertNear(HMI_Param_GetVoutAdcB(), initial_calibration + 0.101f, 0.00001f);
    HMI_Menu_Task_20ms(KEY_EVENT_OK);
    assert(HMI_Menu_GetEditIndex() == 1u);
    initial_calibration = HMI_Param_GetVoutAdcK();
    HMI_Menu_Task_20ms(KEY_EVENT_LONG_DOWN);
    AssertNear(HMI_Param_GetVoutAdcK(), initial_calibration - 0.1f, 0.00001f);
    HMI_Menu_Task_20ms(KEY_EVENT_OK);
    assert(HMI_Menu_GetEditIndex() == 2u);
    initial_calibration = HMI_Param_GetIoutAdcB();
    HMI_Menu_Task_20ms(KEY_EVENT_DOWN);
    AssertNear(HMI_Param_GetIoutAdcB(), initial_calibration - 0.001f, 0.00001f);
    HMI_Menu_Task_20ms(KEY_EVENT_OK);
    assert(HMI_Menu_GetEditIndex() == 3u);
    initial_calibration = HMI_Param_GetIoutAdcK();
    HMI_Menu_Task_20ms(KEY_EVENT_LONG_UP);
    AssertNear(HMI_Param_GetIoutAdcK(), initial_calibration + 0.1f, 0.00001f);

    return 0;
}

#endif
