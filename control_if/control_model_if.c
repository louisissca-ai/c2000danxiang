#include "control_model_if.h"

#include "app_config.h"
#include "app_main.h"
#include "app_types.h"
#include "board.h"
#include "control_interface.h"

#define CONTROL_MODEL_STEP_S          50.0e-6f
#define CONTROL_MODEL_VREF_RATE       72.0f
#define CONTROL_MODEL_ABS_VALID_LIMIT 1000000.0f

static float g_control_model_vref_ramp;
static uint16_t g_control_model_fault_code;
static uint16_t g_control_model_pwm_allowed;

static float ControlModel_Abs(float value)
{
    return (value < 0.0f) ? -value : value;
}

static uint16_t ControlModel_IsFiniteReasonable(float value)
{
    return ((value == value) &&
            (value < CONTROL_MODEL_ABS_VALID_LIMIT) &&
            (value > -CONTROL_MODEL_ABS_VALID_LIMIT)) ? APP_TRUE : APP_FALSE;
}

void ControlModel_Init(void)
{
    BoardPWM_ForceSafe();
    APP_Init();
    g_control_model_vref_ramp = 0.0f;
    g_control_model_fault_code = FAULT_NONE;
    g_control_model_pwm_allowed = APP_FALSE;
}

void ControlModel_Task1ms(void)
{
    APP_TaskScheduler_1ms_ISR();
}

uint16_t ControlModel_UpdateSafety(float vbus, float iout)
{
    Control_Setpoint_t setpoint;
    uint16_t current_fault;
    float current_magnitude;

    ControlIF_GetSetpoint(&setpoint);

    if (setpoint.enable_cmd == APP_FALSE)
    {
        g_control_model_fault_code = FAULT_NONE;
        BoardPWM_ForceSafe();
        g_control_model_pwm_allowed = APP_FALSE;
        return APP_FALSE;
    }

    current_magnitude = ControlModel_Abs(iout);

    if ((ControlModel_IsFiniteReasonable(vbus) == APP_FALSE) ||
        (ControlModel_IsFiniteReasonable(iout) == APP_FALSE) ||
        (vbus <= 0.0f))
    {
        current_fault = FAULT_ADC;
    }
    else if (vbus < APP_VBUS_MIN_VALID)
    {
        current_fault = FAULT_UVLO;
    }
    else if (vbus > APP_VBUS_MAX_VALID)
    {
        current_fault = FAULT_OVP;
    }
    else if (current_magnitude > setpoint.iref)
    {
        current_fault = FAULT_OCP;
    }
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
        BoardPWM_ForceSafe();
        g_control_model_pwm_allowed = APP_FALSE;
        return APP_FALSE;
    }

    BoardPWM_Release();
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

void ControlModel_TripFault(uint16_t fault_code)
{
    if ((fault_code != FAULT_NONE) &&
        (g_control_model_fault_code == FAULT_NONE))
    {
        g_control_model_fault_code = fault_code;
    }

    BoardPWM_ForceSafe();
    g_control_model_pwm_allowed = APP_FALSE;
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
    delta = CONTROL_MODEL_VREF_RATE * CONTROL_MODEL_STEP_S;

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

void ControlModel_SetFeedback(float vin, float vout, float iout, float duty)
{
    Control_Feedback_t feedback;

    feedback.vin = vin;
    feedback.vout = vout;
    feedback.iout = iout;
    feedback.duty = duty;
    feedback.run_state = (g_control_model_fault_code != FAULT_NONE) ? APP_RUN_STATE_FAULT :
        ((g_control_model_pwm_allowed != APP_FALSE) ? APP_RUN_STATE_RUN : APP_RUN_STATE_STOP);
    feedback.fault_code = g_control_model_fault_code;

    ControlIF_SetFeedback(&feedback);
}
