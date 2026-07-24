#include "adc_calibration.h"

#include "app_config.h"
#include "app_types.h"
#include "board.h"
#include "control_interface.h"
#include "control_model_if.h"
#include "hmi_param.h"
#include "pwm_profile.h"

#define ADC_CAL_WINDOW_COUNT           101u
#define ADC_CAL_TRIM_WINDOW_COUNT       10u
#define ADC_CAL_KEPT_WINDOW_COUNT \
    (ADC_CAL_WINDOW_COUNT - (2u * ADC_CAL_TRIM_WINDOW_COUNT))
#define ADC_CAL_RMS_MIN              0.000001f

static volatile ADC_Cal_State_t g_state;
static volatile ADC_Cal_Error_t g_error;
static volatile uint32_t g_sample_count;
static volatile uint32_t g_window_sample_count;
static volatile uint32_t g_window_index;
static uint32_t g_settle_samples;
static uint32_t g_window_samples;
static volatile uint32_t g_vout_window_sum;
static volatile uint32_t g_iout_window_sum;
static volatile uint32_t g_vout_window_sums[ADC_CAL_WINDOW_COUNT];
static volatile uint32_t g_iout_window_sums[ADC_CAL_WINDOW_COUNT];
static float g_vout_adc_gain;
static float g_iout_adc_gain;
static ADC_Cal_Channel_t g_channel;
typedef struct
{
    float vout_b;
    float vout_k;
    float iout_b;
    float iout_k;
} ADC_Cal_Params_t;

static ADC_Cal_Params_t g_candidate;
static float g_true_rms[2];
static float g_measured_rms;

static void ADC_Cal_SortWindowSums(volatile uint32_t *values)
{
    uint16_t index;

    for (index = 1u; index < ADC_CAL_WINDOW_COUNT; index++)
    {
        uint16_t position = index;
        uint32_t value = values[index];

        while ((position > 0u) && (values[position - 1u] > value))
        {
            values[position] = values[position - 1u];
            position--;
        }
        values[position] = value;
    }
}

static float ADC_Cal_TrimmedRawMean(volatile uint32_t *window_sums)
{
    uint16_t index;
    uint32_t selected_sum = 0uL;

    ADC_Cal_SortWindowSums(window_sums);
    for (index = ADC_CAL_TRIM_WINDOW_COUNT;
         index < (ADC_CAL_WINDOW_COUNT - ADC_CAL_TRIM_WINDOW_COUNT);
         index++)
    {
        selected_sum += window_sums[index];
    }

    return (float)selected_sum /
        (float)(ADC_CAL_KEPT_WINDOW_COUNT * g_window_samples);
}

static uint16_t ADC_Cal_IsFinite(float value)
{
    return ((value == value) && (value > -1000000.0f) &&
        (value < 1000000.0f)) ? APP_TRUE : APP_FALSE;
}

static void ADC_Cal_ReadLive(ADC_Cal_Params_t *params)
{
    params->vout_b = HMI_Param_GetVoutAdcB();
    params->vout_k = HMI_Param_GetVoutAdcK();
    params->iout_b = HMI_Param_GetIoutAdcB();
    params->iout_k = HMI_Param_GetIoutAdcK();
}

static void ADC_Cal_ApplyLive(const ADC_Cal_Params_t *params)
{
    HMI_Param_SetVoutAdcB(params->vout_b);
    HMI_Param_SetVoutAdcK(params->vout_k);
    HMI_Param_SetIoutAdcB(params->iout_b);
    HMI_Param_SetIoutAdcK(params->iout_k);
    ControlModel_ResetStoppedFeedback();
}

static uint16_t ADC_Cal_ParamsValid(const ADC_Cal_Params_t *params)
{
    return ((ADC_Cal_IsFinite(params->vout_b) != APP_FALSE) &&
        (ADC_Cal_IsFinite(params->vout_k) != APP_FALSE) &&
        (ADC_Cal_IsFinite(params->iout_b) != APP_FALSE) &&
        (ADC_Cal_IsFinite(params->iout_k) != APP_FALSE) &&
        (params->vout_b >= APP_ADC_CAL_B_MIN) &&
        (params->vout_b <= APP_ADC_CAL_B_MAX) &&
        (params->iout_b >= APP_ADC_CAL_B_MIN) &&
        (params->iout_b <= APP_ADC_CAL_B_MAX) &&
        (params->vout_k >= APP_ADC_CAL_K_MIN) &&
        (params->vout_k <= APP_ADC_CAL_K_MAX) &&
        (params->iout_k >= APP_ADC_CAL_K_MIN) &&
        (params->iout_k <= APP_ADC_CAL_K_MAX)) ? APP_TRUE : APP_FALSE;
}

void ADC_Cal_Init(void)
{
    const PWM_Profile_t *profile = PWM_Profile_Get(
        APP_PWM_FREQUENCY_DEFAULT_KHZ);

    g_state = ADC_CAL_STATE_IDLE;
    g_error = ADC_CAL_ERROR_NONE;
    g_sample_count = 0uL;
    g_window_sample_count = 0uL;
    g_window_index = 0uL;
    g_settle_samples = profile->calibration_settle_samples;
    g_window_samples = profile->calibration_window_samples;
    g_vout_window_sum = 0uL;
    g_iout_window_sum = 0uL;
    g_vout_adc_gain = 0.0f;
    g_iout_adc_gain = 0.0f;
    g_channel = ADC_CAL_CHANNEL_VOUT;
    g_true_rms[ADC_CAL_CHANNEL_VOUT] = APP_VREF_DEFAULT;
    g_true_rms[ADC_CAL_CHANNEL_IOUT] = APP_IREF_DEFAULT;
    g_measured_rms = 0.0f;

}

uint16_t ADC_Cal_StartOffset(void)
{
    HMI_Data_t data;
    const PWM_Profile_t *profile;

    HMI_Param_GetData(&data);
    profile = PWM_Profile_Get(ControlModel_GetActivePwmFrequencyKhz());
    if ((ADC_Cal_IsBusy() != APP_FALSE) ||
        (data.enable_cmd != APP_FALSE) ||
        (data.run_state != APP_RUN_STATE_STOP) ||
        (data.fault_code != FAULT_NONE) || (profile == 0))
    {
        g_state = ADC_CAL_STATE_ERROR;
        g_error = ADC_CAL_ERROR_STATE;
        return APP_FALSE;
    }

    g_sample_count = 0uL;
    g_window_sample_count = 0uL;
    g_window_index = 0uL;
    g_vout_window_sum = 0uL;
    g_iout_window_sum = 0uL;
    g_settle_samples = profile->calibration_settle_samples;
    g_window_samples = profile->calibration_window_samples;
    g_vout_adc_gain = 0.0f;
    g_iout_adc_gain = 0.0f;
    g_error = ADC_CAL_ERROR_NONE;
    g_state = ADC_CAL_STATE_OFFSET_SETTLING;
    return APP_TRUE;
}

void ADC_Cal_PushStoppedRaw(uint16_t vout_raw, uint16_t iout_raw,
    float vout_adc_gain, float iout_adc_gain)
{
    if ((g_state != ADC_CAL_STATE_OFFSET_SETTLING) &&
        (g_state != ADC_CAL_STATE_OFFSET_SAMPLING))
    {
        return;
    }

    if (g_state == ADC_CAL_STATE_OFFSET_SETTLING)
    {
        g_sample_count++;
        if (g_sample_count >= g_settle_samples)
        {
            g_state = ADC_CAL_STATE_OFFSET_SAMPLING;
        }
        return;
    }

    if ((g_window_index == 0uL) && (g_window_sample_count == 0uL))
    {
        g_vout_adc_gain = vout_adc_gain;
        g_iout_adc_gain = iout_adc_gain;
    }

    g_vout_window_sum += (uint32_t)vout_raw;
    g_iout_window_sum += (uint32_t)iout_raw;
    g_window_sample_count++;
    g_sample_count++;

    if (g_window_sample_count >= g_window_samples)
    {
        g_vout_window_sums[g_window_index] = g_vout_window_sum;
        g_iout_window_sums[g_window_index] = g_iout_window_sum;
        g_window_index++;
        g_window_sample_count = 0uL;
        g_vout_window_sum = 0uL;
        g_iout_window_sum = 0uL;

        if (g_window_index >= ADC_CAL_WINDOW_COUNT)
        {
            g_state = ADC_CAL_STATE_OFFSET_COMPUTING;
        }
    }
}

uint16_t ADC_Cal_ApplyGain(ADC_Cal_Channel_t channel, float true_rms,
    float measured_rms)
{
    HMI_Data_t data;
    float new_k;

    HMI_Param_GetData(&data);
    if ((ADC_Cal_IsBusy() != APP_FALSE) ||
        (data.run_state != APP_RUN_STATE_RUN) ||
        (data.mode_cmd != APP_CONTROL_MODE_OPEN_LOOP) ||
        (data.fault_code != FAULT_NONE))
    {
        g_state = ADC_CAL_STATE_ERROR;
        g_error = ADC_CAL_ERROR_STATE;
        return APP_FALSE;
    }
    if ((channel > ADC_CAL_CHANNEL_IOUT) ||
        (ADC_Cal_IsFinite(true_rms) == APP_FALSE) ||
        (ADC_Cal_IsFinite(measured_rms) == APP_FALSE) ||
        (true_rms <= ADC_CAL_RMS_MIN) || (measured_rms <= ADC_CAL_RMS_MIN))
    {
        g_state = ADC_CAL_STATE_ERROR;
        g_error = ADC_CAL_ERROR_INPUT;
        return APP_FALSE;
    }

    ADC_Cal_ReadLive(&g_candidate);
    new_k = ((channel == ADC_CAL_CHANNEL_VOUT) ? g_candidate.vout_k :
        g_candidate.iout_k) * true_rms / measured_rms;
    if ((ADC_Cal_IsFinite(new_k) == APP_FALSE) ||
        (new_k < APP_ADC_CAL_K_MIN) || (new_k > APP_ADC_CAL_K_MAX))
    {
        g_state = ADC_CAL_STATE_ERROR;
        g_error = ADC_CAL_ERROR_RANGE;
        return APP_FALSE;
    }

    if (channel == ADC_CAL_CHANNEL_VOUT)
    {
        g_candidate.vout_k = new_k;
    }
    else
    {
        g_candidate.iout_k = new_k;
    }
    g_channel = channel;
    g_true_rms[channel] = true_rms;
    g_measured_rms = measured_rms;
    HMI_Param_SetEnableCmd(APP_FALSE);
    g_error = ADC_CAL_ERROR_NONE;
    g_state = ADC_CAL_STATE_WAIT_STOP;
    return APP_TRUE;
}

void ADC_Cal_Task1ms(void)
{
    Control_Feedback_t feedback;

    if (g_state == ADC_CAL_STATE_OFFSET_COMPUTING)
    {
        ADC_Cal_ReadLive(&g_candidate);
        g_candidate.vout_b = g_vout_adc_gain *
            ADC_Cal_TrimmedRawMean(g_vout_window_sums);
        g_candidate.iout_b = g_iout_adc_gain *
            ADC_Cal_TrimmedRawMean(g_iout_window_sums);
        if (ADC_Cal_ParamsValid(&g_candidate) == APP_FALSE)
        {
            g_error = ADC_CAL_ERROR_RANGE;
            g_state = ADC_CAL_STATE_ERROR;
            return;
        }
        g_state = ADC_CAL_STATE_WAIT_STOP;
    }

    if (g_state != ADC_CAL_STATE_WAIT_STOP)
    {
        return;
    }

    ControlIF_GetFeedback(&feedback);
    if (feedback.run_state != APP_RUN_STATE_STOP)
    {
        return;
    }

    BoardPWM_ForceSafe();
    ADC_Cal_ApplyLive(&g_candidate);
    g_error = ADC_CAL_ERROR_NONE;
    g_state = ADC_CAL_STATE_APPLIED;
}

void ADC_Cal_Cancel(void)
{
    if ((g_state == ADC_CAL_STATE_OFFSET_SETTLING) ||
        (g_state == ADC_CAL_STATE_OFFSET_SAMPLING) ||
        (g_state == ADC_CAL_STATE_OFFSET_COMPUTING) ||
        (g_state == ADC_CAL_STATE_WAIT_STOP))
    {
        g_error = ADC_CAL_ERROR_NONE;
        g_state = ADC_CAL_STATE_IDLE;
    }
}

uint16_t ADC_Cal_IsBusy(void)
{
    return ((g_state == ADC_CAL_STATE_OFFSET_SETTLING) ||
        (g_state == ADC_CAL_STATE_OFFSET_SAMPLING) ||
        (g_state == ADC_CAL_STATE_OFFSET_COMPUTING) ||
        (g_state == ADC_CAL_STATE_WAIT_STOP)) ? APP_TRUE : APP_FALSE;
}

void ADC_Cal_GetView(ADC_Cal_View_t *view)
{
    if (view == 0)
    {
        return;
    }
    view->state = g_state;
    view->channel = g_channel;
    view->error = g_error;
    view->progress_percent = ((g_state == ADC_CAL_STATE_OFFSET_SETTLING) ||
        (g_state == ADC_CAL_STATE_OFFSET_SAMPLING)) ?
        (uint16_t)((100uL * g_sample_count) /
        (g_settle_samples +
        (g_window_samples * ADC_CAL_WINDOW_COUNT))) :
        ((g_state == ADC_CAL_STATE_OFFSET_COMPUTING) ? 100u : 0u);
    view->true_rms = g_true_rms[g_channel];
    view->measured_rms = g_measured_rms;
}

void ADC_Cal_SetTrueRms(ADC_Cal_Channel_t channel, float value)
{
    float maximum;

    if (channel > ADC_CAL_CHANNEL_IOUT)
    {
        return;
    }
    maximum = (channel == ADC_CAL_CHANNEL_VOUT) ?
        APP_VREF_MAX : APP_IREF_MAX;
    if (value < 0.001f)
    {
        value = 0.001f;
    }
    else if (value > maximum)
    {
        value = maximum;
    }
    g_true_rms[channel] = value;
    g_channel = channel;
}

float ADC_Cal_GetTrueRms(ADC_Cal_Channel_t channel)
{
    return (channel <= ADC_CAL_CHANNEL_IOUT) ?
        g_true_rms[channel] : 0.0f;
}
