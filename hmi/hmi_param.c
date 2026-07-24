/*
 * 文件: hmi_param.c
 * 说明: 带设定值限幅功能的 HMI 核心数据模型。
 * 备注: 这里不访问硬件，也不包含阻塞调用。
 */

#include "hmi_param.h"

#include "app_config.h"
#include "app_types.h"
#include "pwm_profile.h"

static HMI_Data_t g_hmi_param_data;

static float HMI_Param_Clamp(float value, float min_value, float max_value)
{
    if (value < min_value)
    {
        return min_value;
    }

    if (value > max_value)
    {
        return max_value;
    }

    return value;
}

/*
 * 函数: HMI_Param_Init
 * 调用周期: 在 HMI_Init() 中调用一次。
 * ISR: 否。
 * 阻塞: 否。
 */
void HMI_Param_Init(void)
{
    g_hmi_param_data.vref = APP_VREF_DEFAULT;
    g_hmi_param_data.iref = APP_IREF_DEFAULT;
    g_hmi_param_data.vin = 0.0f;
    g_hmi_param_data.vout = 0.0f;
    g_hmi_param_data.iout = 0.0f;
    g_hmi_param_data.vout_inst = 0.0f;
    g_hmi_param_data.iout_inst = 0.0f;
    g_hmi_param_data.vd = 0.0f;
    g_hmi_param_data.vq = 0.0f;
    g_hmi_param_data.id = 0.0f;
    g_hmi_param_data.iq = 0.0f;
    g_hmi_param_data.duty = 0.0f;
    g_hmi_param_data.vout_adc_b = APP_ADC_CAL_VOUT_B_DEFAULT;
    g_hmi_param_data.vout_adc_k = APP_ADC_CAL_VOUT_K_DEFAULT;
    g_hmi_param_data.iout_adc_b = APP_ADC_CAL_IOUT_B_DEFAULT;
    g_hmi_param_data.iout_adc_k = APP_ADC_CAL_IOUT_K_DEFAULT;
    g_hmi_param_data.enable_cmd = APP_FALSE;
    g_hmi_param_data.mode_cmd = 0u;
    g_hmi_param_data.pwm_frequency_khz = APP_PWM_FREQUENCY_DEFAULT_KHZ;
    g_hmi_param_data.run_state = APP_RUN_STATE_STOP;
    g_hmi_param_data.fault_code = FAULT_NONE;
}

void HMI_Param_GetData(HMI_Data_t *data)
{
    if (data != 0)
    {
        *data = g_hmi_param_data;
    }
}

float HMI_Param_GetVref(void)
{
    return g_hmi_param_data.vref;
}

float HMI_Param_GetIref(void)
{
    return g_hmi_param_data.iref;
}

float HMI_Param_GetVoutAdcB(void)
{
    return g_hmi_param_data.vout_adc_b;
}

float HMI_Param_GetVoutAdcK(void)
{
    return g_hmi_param_data.vout_adc_k;
}

float HMI_Param_GetIoutAdcB(void)
{
    return g_hmi_param_data.iout_adc_b;
}

float HMI_Param_GetIoutAdcK(void)
{
    return g_hmi_param_data.iout_adc_k;
}

uint16_t HMI_Param_GetEnableCmd(void)
{
    return g_hmi_param_data.enable_cmd;
}

uint16_t HMI_Param_GetModeCmd(void)
{
    return g_hmi_param_data.mode_cmd;
}

uint16_t HMI_Param_GetPwmFrequencyKhz(void)
{
    return g_hmi_param_data.pwm_frequency_khz;
}

uint16_t HMI_Param_GetFaultCode(void)
{
    return g_hmi_param_data.fault_code;
}

uint16_t HMI_Param_GetRunState(void)
{
    return g_hmi_param_data.run_state;
}

void HMI_Param_SetVref(float value)
{
    g_hmi_param_data.vref = HMI_Param_Clamp(value, APP_VREF_MIN, APP_VREF_MAX);
}

void HMI_Param_SetIref(float value)
{
    g_hmi_param_data.iref = HMI_Param_Clamp(value, APP_IREF_MIN, APP_IREF_MAX);
}

void HMI_Param_AdjustVref(float delta)
{
    HMI_Param_SetVref(g_hmi_param_data.vref + delta);
}

void HMI_Param_AdjustIref(float delta)
{
    HMI_Param_SetIref(g_hmi_param_data.iref + delta);
}

static void HMI_Param_StopForAdcCalibration(void)
{
    g_hmi_param_data.enable_cmd = APP_FALSE;
}

void HMI_Param_AdjustVoutAdcB(float delta)
{
    HMI_Param_SetVoutAdcB(g_hmi_param_data.vout_adc_b + delta);
}

void HMI_Param_AdjustVoutAdcK(float delta)
{
    HMI_Param_SetVoutAdcK(g_hmi_param_data.vout_adc_k + delta);
}

void HMI_Param_AdjustIoutAdcB(float delta)
{
    HMI_Param_SetIoutAdcB(g_hmi_param_data.iout_adc_b + delta);
}

void HMI_Param_AdjustIoutAdcK(float delta)
{
    HMI_Param_SetIoutAdcK(g_hmi_param_data.iout_adc_k + delta);
}

void HMI_Param_SetVoutAdcB(float value)
{
    g_hmi_param_data.vout_adc_b = HMI_Param_Clamp(value,
        APP_ADC_CAL_B_MIN, APP_ADC_CAL_B_MAX);
    HMI_Param_StopForAdcCalibration();
}

void HMI_Param_SetVoutAdcK(float value)
{
    g_hmi_param_data.vout_adc_k = HMI_Param_Clamp(value,
        APP_ADC_CAL_K_MIN, APP_ADC_CAL_K_MAX);
    HMI_Param_StopForAdcCalibration();
}

void HMI_Param_SetIoutAdcB(float value)
{
    g_hmi_param_data.iout_adc_b = HMI_Param_Clamp(value,
        APP_ADC_CAL_B_MIN, APP_ADC_CAL_B_MAX);
    HMI_Param_StopForAdcCalibration();
}

void HMI_Param_SetIoutAdcK(float value)
{
    g_hmi_param_data.iout_adc_k = HMI_Param_Clamp(value,
        APP_ADC_CAL_K_MIN, APP_ADC_CAL_K_MAX);
    HMI_Param_StopForAdcCalibration();
}

void HMI_Param_ToggleEnable(void)
{
    g_hmi_param_data.enable_cmd = (g_hmi_param_data.enable_cmd == 0u) ? 1u : 0u;
}

void HMI_Param_SetEnableCmd(uint16_t enable)
{
    g_hmi_param_data.enable_cmd = (enable != 0u) ? APP_TRUE : APP_FALSE;
}

void HMI_Param_SetModeCmd(uint16_t mode)
{
    uint16_t new_mode = (mode == APP_CONTROL_MODE_OPEN_LOOP) ?
        APP_CONTROL_MODE_OPEN_LOOP : APP_CONTROL_MODE_CLOSED_LOOP;

    if (new_mode != g_hmi_param_data.mode_cmd)
    {
        g_hmi_param_data.enable_cmd = APP_FALSE;
    }
    g_hmi_param_data.mode_cmd = new_mode;
    if (new_mode == APP_CONTROL_MODE_CLOSED_LOOP)
    {
        g_hmi_param_data.pwm_frequency_khz =
            APP_PWM_FREQUENCY_DEFAULT_KHZ;
    }
}

void HMI_Param_SetPwmFrequencyKhz(uint16_t frequency_khz)
{
    if ((PWM_Profile_Get(frequency_khz) == 0) ||
        (g_hmi_param_data.mode_cmd != APP_CONTROL_MODE_OPEN_LOOP) ||
        (g_hmi_param_data.enable_cmd != APP_FALSE) ||
        (g_hmi_param_data.run_state != APP_RUN_STATE_STOP))
    {
        return;
    }

    if (frequency_khz != g_hmi_param_data.pwm_frequency_khz)
    {
        g_hmi_param_data.enable_cmd = APP_FALSE;
        g_hmi_param_data.pwm_frequency_khz = frequency_khz;
    }
}

void HMI_Param_SetVin(float value)
{
    g_hmi_param_data.vin = value;
}

void HMI_Param_SetVout(float value)
{
    g_hmi_param_data.vout = value;
}

void HMI_Param_SetIout(float value)
{
    g_hmi_param_data.iout = value;
}

void HMI_Param_SetVoutInst(float value)
{
    g_hmi_param_data.vout_inst = value;
}

void HMI_Param_SetIoutInst(float value)
{
    g_hmi_param_data.iout_inst = value;
}

void HMI_Param_SetDq(float vd, float vq, float id, float iq)
{
    g_hmi_param_data.vd = vd;
    g_hmi_param_data.vq = vq;
    g_hmi_param_data.id = id;
    g_hmi_param_data.iq = iq;
}

void HMI_Param_SetDuty(float value)
{
    g_hmi_param_data.duty = HMI_Param_Clamp(value, 0.0f, 100.0f);
}

void HMI_Param_SetFaultCode(uint16_t fault_code)
{
    g_hmi_param_data.fault_code = fault_code;
}

void HMI_Param_SetRunState(uint16_t run_state)
{
    g_hmi_param_data.run_state = run_state;
}
