#include "control_model_if.h"

#include <math.h>

#include "app_config.h"
#include "app_main.h"
#include "app_types.h"
#include "board.h"
#include "control_interface.h"
#include "pwm_profile.h"

#define CONTROL_MODEL_VREF_RATE       72.0f
#define CONTROL_MODEL_ABS_VALID_LIMIT 1000000.0f
#define CONTROL_MODEL_SQRT2           1.4142135623730951f
#define CONTROL_MODEL_TWO_PI          6.2831853071795865f

static float g_control_model_vref_ramp;
static float g_control_model_open_loop_phase;
static float g_control_model_running_vout_square_sum;
static float g_control_model_running_iout_square_sum;
static float g_control_model_running_vout_rms;
static float g_control_model_running_iout_rms;
static float g_control_model_vout_square_sum;
static float g_control_model_iout_square_sum;
static float g_control_model_stopped_vout_rms;
static float g_control_model_stopped_iout_rms;
static uint16_t g_control_model_fault_code;
static uint16_t g_control_model_pwm_allowed;
static uint16_t g_control_model_active_mode;
static uint16_t g_control_model_running_rms_sample_count;
static uint16_t g_control_model_rms_sample_count;
static const PWM_Profile_t *g_control_model_pwm_profile;

#if APP_SOFTWARE_OCP_ENABLED
static float ControlModel_Abs(float value)
{
    return (value < 0.0f) ? -value : value;
}
#endif

static uint16_t ControlModel_IsFiniteReasonable(float value)
{
    return ((value == value) &&
            (value < CONTROL_MODEL_ABS_VALID_LIMIT) &&
            (value > -CONTROL_MODEL_ABS_VALID_LIMIT)) ? APP_TRUE : APP_FALSE;
}

static uint16_t ControlModel_IsModeValid(uint16_t mode)
{
    return ((mode == APP_CONTROL_MODE_CLOSED_LOOP) ||
            (mode == APP_CONTROL_MODE_OPEN_LOOP)) ? APP_TRUE : APP_FALSE;
}

void ControlModel_ResetStoppedFeedback(void)
{
    g_control_model_vout_square_sum = 0.0f;
    g_control_model_iout_square_sum = 0.0f;
    g_control_model_stopped_vout_rms = 0.0f;
    g_control_model_stopped_iout_rms = 0.0f;
    g_control_model_rms_sample_count = 0u;
}

static void ControlModel_ResetRunningRms(void)
{
    g_control_model_running_vout_square_sum = 0.0f;
    g_control_model_running_iout_square_sum = 0.0f;
    g_control_model_running_vout_rms = 0.0f;
    g_control_model_running_iout_rms = 0.0f;
    g_control_model_running_rms_sample_count = 0u;
}

static void ControlModel_DisablePwm(void)
{
    if (g_control_model_pwm_allowed != APP_FALSE)
    {
        ControlModel_ResetStoppedFeedback();
        ControlModel_ResetRunningRms();
    }

    BoardPWM_ForceSafe();
    g_control_model_pwm_allowed = APP_FALSE;
    g_control_model_vref_ramp = 0.0f;
    g_control_model_open_loop_phase = 0.0f;
}

void ControlModel_Init(void)
{
    BoardPWM_ForceSafe();
    APP_Init();
    g_control_model_vref_ramp = 0.0f;
    g_control_model_open_loop_phase = 0.0f;
    ControlModel_ResetStoppedFeedback();
    ControlModel_ResetRunningRms();
    g_control_model_fault_code = FAULT_NONE;
    g_control_model_pwm_allowed = APP_FALSE;
    g_control_model_active_mode = APP_CONTROL_MODE_CLOSED_LOOP;
    g_control_model_pwm_profile = PWM_Profile_Get(
        APP_PWM_FREQUENCY_DEFAULT_KHZ);
}

void ControlModel_Task1ms(void)
{
    APP_TaskScheduler_1ms_ISR();
}

uint16_t ControlModel_UpdateSafety(float vbus, float iout)
{
    Control_Setpoint_t setpoint;
    uint16_t current_fault;
#if APP_SOFTWARE_OCP_ENABLED
    float current_magnitude;
#endif

    ControlIF_GetSetpoint(&setpoint);

    if (ControlModel_IsModeValid(setpoint.mode_cmd) == APP_FALSE)
    {
        ControlModel_DisablePwm();
        return APP_FALSE;
    }

    if (setpoint.enable_cmd == APP_FALSE)
    {
        g_control_model_fault_code = FAULT_NONE;
        ControlModel_DisablePwm();
        g_control_model_active_mode = setpoint.mode_cmd;
        return APP_FALSE;
    }

    if (setpoint.mode_cmd != g_control_model_active_mode)
    {
        ControlModel_DisablePwm();
        return APP_FALSE;
    }

#if APP_SOFTWARE_OCP_ENABLED
    current_magnitude = ControlModel_Abs(iout);
#endif

    if ((ControlModel_IsFiniteReasonable(vbus) == APP_FALSE) ||
        (ControlModel_IsFiniteReasonable(iout) == APP_FALSE) ||
        (vbus <= 0.0f))
    {
        current_fault = FAULT_ADC;
    }
#if APP_SOFTWARE_OCP_ENABLED
    else if (current_magnitude > setpoint.iref)
    {
        current_fault = FAULT_OCP;
    }
#endif
    else
    {
        current_fault = FAULT_NONE;
    }

    if ((current_fault != FAULT_NONE) && (g_control_model_fault_code == FAULT_NONE))
    {
        g_control_model_fault_code = current_fault;
    }

    if (g_control_model_fault_code != FAULT_NONE)
    {
        if ((setpoint.enable_cmd == APP_FALSE) && (current_fault == FAULT_NONE))
        {
            g_control_model_fault_code = FAULT_NONE;
        }
        ControlModel_DisablePwm();
        return APP_FALSE;
    }

    if (g_control_model_pwm_allowed == APP_FALSE)
    {
        ControlModel_ResetStoppedFeedback();
    }
    g_control_model_pwm_allowed = APP_TRUE;

    return g_control_model_pwm_allowed;
}

uint16_t ControlModel_IsPwmAllowed(float vbus)
{
    return ControlModel_UpdateSafety(vbus, 0.0f);
}

uint16_t ControlModel_IsControlEnabled(void)
{
    return g_control_model_pwm_allowed;
}

uint16_t ControlModel_GetFaultCode(void)
{
    return g_control_model_fault_code;
}

uint16_t ControlModel_GetActiveMode(void)
{
    return g_control_model_active_mode;
}

void ControlModel_TripFault(uint16_t fault_code)
{
    if ((fault_code != FAULT_NONE) &&
        (g_control_model_fault_code == FAULT_NONE))
    {
        g_control_model_fault_code = fault_code;
    }

    ControlModel_DisablePwm();
}

float ControlModel_GetVrefRamp(void)
{
    Control_Setpoint_t setpoint;
    float delta;

    if (g_control_model_pwm_allowed == APP_FALSE)
    {
        g_control_model_vref_ramp = 0.0f;
        return 0.0f;
    }

    ControlIF_GetSetpoint(&setpoint);
    delta = CONTROL_MODEL_VREF_RATE *
        g_control_model_pwm_profile->control_step_s;

    if (g_control_model_vref_ramp < setpoint.vref)
    {
        g_control_model_vref_ramp += delta;
        if (g_control_model_vref_ramp > setpoint.vref)
        {
            g_control_model_vref_ramp = setpoint.vref;
        }
    }
    else if (g_control_model_vref_ramp > setpoint.vref)
    {
        g_control_model_vref_ramp -= delta;
        if (g_control_model_vref_ramp < setpoint.vref)
        {
            g_control_model_vref_ramp = setpoint.vref;
        }
    }

    return g_control_model_vref_ramp;
}

void ControlModel_UpdateRunningRms(float vout_sample, float iout_sample)
{
    if ((ControlModel_IsFiniteReasonable(vout_sample) == APP_FALSE) ||
        (ControlModel_IsFiniteReasonable(iout_sample) == APP_FALSE))
    {
        ControlModel_ResetRunningRms();
        return;
    }

    g_control_model_running_vout_square_sum += vout_sample * vout_sample;
    g_control_model_running_iout_square_sum += iout_sample * iout_sample;
    g_control_model_running_rms_sample_count++;
    if (g_control_model_running_rms_sample_count >=
        g_control_model_pwm_profile->rms_samples)
    {
        g_control_model_running_vout_rms = sqrtf(
            g_control_model_running_vout_square_sum /
            (float)g_control_model_pwm_profile->rms_samples);
        g_control_model_running_iout_rms = sqrtf(
            g_control_model_running_iout_square_sum /
            (float)g_control_model_pwm_profile->rms_samples);
        g_control_model_running_vout_square_sum = 0.0f;
        g_control_model_running_iout_square_sum = 0.0f;
        g_control_model_running_rms_sample_count = 0u;
    }
}

float ControlModel_GetRunningIoutRms(void)
{
    return g_control_model_running_iout_rms;
}

float ControlModel_GetRunningVoutRms(void)
{
    return g_control_model_running_vout_rms;
}

void ControlModel_GetOpenLoopDuty(float vref_rms, float vbus,
    float *duty_a_percent, float *duty_b_percent)
{
    float modulation;
    float instantaneous;

    if ((duty_a_percent == 0) || (duty_b_percent == 0))
    {
        return;
    }

    modulation = 0.0f;
    if ((ControlModel_IsFiniteReasonable(vref_rms) != APP_FALSE) &&
        (ControlModel_IsFiniteReasonable(vbus) != APP_FALSE) &&
        (vref_rms > 0.0f) && (vbus > 0.0f))
    {
        modulation = CONTROL_MODEL_SQRT2 * vref_rms / vbus;
        if (modulation > APP_PWM_MAX_MODULATION)
        {
            modulation = APP_PWM_MAX_MODULATION;
        }
    }

    instantaneous = modulation * sinf(g_control_model_open_loop_phase);

    /*
     * Half-cycle clamped modulation: only the leg carrying the instantaneous
     * polarity switches; the other leg is held at 0% for the whole
     * half-cycle, halving each leg's switching loss versus a 50%-centered
     * duty pair.
     */
    if (instantaneous >= 0.0f)
    {
        *duty_a_percent = instantaneous * 100.0f;
        *duty_b_percent = 0.0f;
    }
    else
    {
        *duty_a_percent = 0.0f;
        *duty_b_percent = -instantaneous * 100.0f;
    }

    if (g_control_model_pwm_allowed != APP_FALSE)
    {
        g_control_model_open_loop_phase += CONTROL_MODEL_TWO_PI *
            APP_OUTPUT_FREQUENCY_HZ *
            g_control_model_pwm_profile->control_step_s;
        if (g_control_model_open_loop_phase >= CONTROL_MODEL_TWO_PI)
        {
            g_control_model_open_loop_phase -= CONTROL_MODEL_TWO_PI;
        }
    }
}

uint16_t ControlModel_ClampPwmCompare(uint16_t period, uint16_t compare)
{
    uint16_t maximum;

    if (period <= (2u * APP_PWM_MIN_COMPARE_COUNTS))
    {
        return period / 2u;
    }

    maximum = period - APP_PWM_MIN_COMPARE_COUNTS;
    if (compare < APP_PWM_MIN_COMPARE_COUNTS)
    {
        return APP_PWM_MIN_COMPARE_COUNTS;
    }

    return (compare > maximum) ? maximum : compare;
}

void ControlModel_UpdateStoppedFeedback(float vin, float vout_sample,
    float iout_sample)
{
    if ((ControlModel_IsFiniteReasonable(vout_sample) == APP_FALSE) ||
        (ControlModel_IsFiniteReasonable(iout_sample) == APP_FALSE))
    {
        ControlModel_ResetStoppedFeedback();
        vout_sample = 0.0f;
        iout_sample = 0.0f;
    }
    else
    {
        g_control_model_vout_square_sum += vout_sample * vout_sample;
        g_control_model_iout_square_sum += iout_sample * iout_sample;
        g_control_model_rms_sample_count++;

        if (g_control_model_rms_sample_count >=
            g_control_model_pwm_profile->rms_samples)
        {
            g_control_model_stopped_vout_rms = sqrtf(
                g_control_model_vout_square_sum /
                (float)g_control_model_pwm_profile->rms_samples);
            g_control_model_stopped_iout_rms = sqrtf(
                g_control_model_iout_square_sum /
                (float)g_control_model_pwm_profile->rms_samples);
            g_control_model_vout_square_sum = 0.0f;
            g_control_model_iout_square_sum = 0.0f;
            g_control_model_rms_sample_count = 0u;
        }
    }

    ControlModel_SetFeedback(vin, g_control_model_stopped_vout_rms,
        g_control_model_stopped_iout_rms, vout_sample, iout_sample,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

void ControlModel_SetFeedback(float vin, float vout, float iout,
    float vout_inst, float iout_inst, float vd, float vq, float id, float iq,
    float duty)
{
    Control_Feedback_t feedback;

    feedback.vin = vin;
    feedback.vout = vout;
    feedback.iout = iout;
    feedback.vout_inst = vout_inst;
    feedback.iout_inst = iout_inst;
    feedback.vd = vd;
    feedback.vq = vq;
    feedback.id = id;
    feedback.iq = iq;
    feedback.duty = duty;
    feedback.run_state = (g_control_model_fault_code != FAULT_NONE) ? APP_RUN_STATE_FAULT :
        ((g_control_model_pwm_allowed != APP_FALSE) ? APP_RUN_STATE_RUN : APP_RUN_STATE_STOP);
    feedback.fault_code = g_control_model_fault_code;

    ControlIF_SetFeedback(&feedback);
}
