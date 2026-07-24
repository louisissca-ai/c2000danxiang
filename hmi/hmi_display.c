/*
 * 文件: hmi_display.c
 * 说明: 面向 128x64 OLED 抽象层的页面渲染器。
 * 备注: 字符串格式化仅在低速显示任务中执行。
 */

#include "hmi_display.h"

#include "adc_calibration.h"
#include "app_config.h"
#include "app_types.h"
#include "hmi_menu.h"
#include "hmi_param.h"
#include "oled.h"

static const char *HMI_Display_FaultText(uint16_t fault_code)
{
    if ((fault_code & FAULT_OVP) != 0u)
    {
        return "OVP";
    }
    if ((fault_code & FAULT_OCP) != 0u)
    {
        return "OCP";
    }
    if ((fault_code & FAULT_OTP) != 0u)
    {
        return "OTP";
    }
    if ((fault_code & FAULT_UVLO) != 0u)
    {
        return "UVLO";
    }
    if ((fault_code & FAULT_ADC) != 0u)
    {
        return "ADC Fault";
    }
    if ((fault_code & FAULT_PWM) != 0u)
    {
        return "PWM Fault";
    }
    return "No Fault";
}

static const char *HMI_Display_RunStateText(uint16_t run_state)
{
    switch (run_state)
    {
    case APP_RUN_STATE_RUN:
        return "RUN";

    case APP_RUN_STATE_FAULT:
        return "FAULT";

    case APP_RUN_STATE_STOP:
    default:
        return "STOP";
    }
}

static void HMI_Display_ShowMain(const HMI_Data_t *data)
{
    OLED_ShowString(0u, 0u, "Vin :");
    OLED_ShowFloat(42u, 0u, data->vin, 1u);
    OLED_ShowString(88u, 0u, "V");

    OLED_ShowString(0u, 1u, "Vnow:");
    OLED_ShowFloat(42u, 1u, data->vout_inst, 1u);
    OLED_ShowString(88u, 1u, "V");

    OLED_ShowString(0u, 2u, "Inow:");
    OLED_ShowFloat(42u, 2u, data->iout_inst, 2u);
    OLED_ShowString(88u, 2u, "A");

    OLED_ShowString(0u, 3u, "Duty:");
    OLED_ShowFloat(42u, 3u, data->duty, 1u);
    OLED_ShowString(88u, 3u, "%");

    OLED_ShowString(0u, 5u, (data->enable_cmd != 0u) ? "CMD: ON " : "CMD: OFF");
    OLED_ShowString(72u, 5u, HMI_Display_RunStateText(data->run_state));
    OLED_ShowString(0u, 7u, HMI_Display_FaultText(data->fault_code));
}

static void HMI_Display_ShowDqMonitor(const HMI_Data_t *data)
{
    OLED_ShowString(0u, 0u, "DQ Monitor");
    OLED_ShowString(0u, 1u, "Vd:");
    OLED_ShowFloat(30u, 1u, data->vd, 2u);
    OLED_ShowString(0u, 2u, "Vq:");
    OLED_ShowFloat(30u, 2u, data->vq, 2u);
    OLED_ShowString(0u, 3u, "Id:");
    OLED_ShowFloat(30u, 3u, data->id, 2u);
    OLED_ShowString(0u, 4u, "Iq:");
    OLED_ShowFloat(30u, 4u, data->iq, 2u);
    OLED_ShowString(0u, 7u, "BACK Main");
}

static void HMI_Display_ShowSetValue(const char *title, float value, const char *unit)
{
    OLED_ShowString(0u, 0u, title);
    OLED_ShowFloat(0u, 2u, value, 2u);
    OLED_ShowString(64u, 2u, unit);
    OLED_ShowString(0u, 5u, "UP/DOWN Adjust");
    OLED_ShowString(0u, 7u, "OK Next");
}

static void HMI_Display_ShowEnable(const HMI_Data_t *data)
{
    OLED_ShowString(0u, 0u, "Set Enable");
    OLED_ShowString(0u, 2u, (data->enable_cmd != 0u) ? "Output Request ON" : "Output Request OFF");
    OLED_ShowString(0u, 5u, "UP/DOWN Toggle");
    OLED_ShowString(0u, 7u, "RUN Toggle");
}

static void HMI_Display_ShowMode(const HMI_Data_t *data)
{
    OLED_ShowString(0u, 0u, "Set Mode");
    OLED_ShowString(0u, 2u, (data->mode_cmd == APP_CONTROL_MODE_OPEN_LOOP) ?
        "OPEN LOOP" : "CLOSED LOOP");
    OLED_ShowString(0u, 5u, "UP/DOWN Change");
    OLED_ShowString(0u, 7u, "Change Stops PWM");
}

static void HMI_Display_ShowAdcCalibration(const HMI_Data_t *data,
    uint16_t edit_index)
{
    const char *field;
    float value;
    ADC_Cal_View_t view;
    uint16_t editing;

    ADC_Cal_GetView(&view);
    editing = HMI_Menu_IsCalEditing();

    OLED_ShowString(0u, 0u, "ADC Cal");
    if (view.state == ADC_CAL_STATE_OFFSET_SETTLING)
    {
        OLED_ShowString(0u, 2u, "ZERO SETTLE");
        OLED_ShowInt(0u, 4u, (int32_t)view.progress_percent);
        OLED_ShowString(28u, 4u, "%");
        OLED_ShowString(0u, 7u, "BACK Cancel");
        return;
    }
    if (view.state == ADC_CAL_STATE_OFFSET_SAMPLING)
    {
        OLED_ShowString(0u, 2u, "ZERO SAMPLING");
        OLED_ShowInt(0u, 4u, (int32_t)view.progress_percent);
        OLED_ShowString(28u, 4u, "%");
        OLED_ShowString(0u, 7u, "BACK Cancel");
        return;
    }
    if (view.state == ADC_CAL_STATE_OFFSET_COMPUTING)
    {
        OLED_ShowString(0u, 2u, "ZERO COMPUTE");
        OLED_ShowString(0u, 7u, "BACK Cancel");
        return;
    }
    if (view.state == ADC_CAL_STATE_WAIT_STOP)
    {
        OLED_ShowString(0u, 2u, "WAIT STOP");
        OLED_ShowString(0u, 7u, "BACK Cancel");
        return;
    }
    if (view.state == ADC_CAL_STATE_APPLIED)
    {
        OLED_ShowString(0u, 2u, "APPLIED RAM");
    }
    else if (view.state == ADC_CAL_STATE_ERROR)
    {
        OLED_ShowString(0u, 2u, "CAL ERROR");
        OLED_ShowString(72u, 2u, "E");
        OLED_ShowInt(82u, 2u, (int32_t)view.error);
    }

    if (editing != 0u && (edit_index == 1u || edit_index == 2u))
    {
        value = (edit_index == 1u) ? data->vout : data->iout;
        OLED_ShowString(0u, 3u, "MEAS:");
        OLED_ShowFloat(42u, 3u, value, 3u);
        OLED_ShowString(0u, 4u, "TRUE:");
        OLED_ShowFloat(42u, 4u, ADC_Cal_GetTrueRms(
            (edit_index == 1u) ? ADC_CAL_CHANNEL_VOUT :
            ADC_CAL_CHANNEL_IOUT), 3u);
        OLED_ShowString(0u, 6u, "Digit 10^");
        OLED_ShowInt(60u, 6u, (int32_t)HMI_Menu_GetCalDigit() - 3);
        OLED_ShowString(0u, 7u, "OK Apply");
        return;
    }

    switch (edit_index)
    {
    case 0u:
        field = "ZERO AUTO";
        value = 0.0f;
        break;
    case 1u:
        field = "V GAIN";
        value = ADC_Cal_GetTrueRms(ADC_CAL_CHANNEL_VOUT);
        break;
    case 2u:
        field = "I GAIN";
        value = ADC_Cal_GetTrueRms(ADC_CAL_CHANNEL_IOUT);
        break;
    case 3u:
        field = "V b";
        value = data->vout_adc_b;
        break;
    case 4u:
        field = "V k";
        value = data->vout_adc_k;
        break;
    case 5u:
        field = "I b";
        value = data->iout_adc_b;
        break;
    default:
        field = "I k";
        value = data->iout_adc_k;
        break;
    }

    OLED_ShowString(0u, 4u, field);
    if (edit_index != 0u)
    {
        OLED_ShowString(42u, 4u, "=");
        OLED_ShowFloat(54u, 4u, value, 3u);
    }
    OLED_ShowString(0u, 6u, (editing != 0u) ?
        "UP/DN Adjust" : "UP/DN Select");
    OLED_ShowString(0u, 7u, (editing != 0u) ?
        "OK Done" : "OK Enter");
}

static void HMI_Display_ShowFault(const HMI_Data_t *data)
{
    OLED_ShowString(0u, 0u, "Fault");
    OLED_ShowString(0u, 1u, HMI_Display_FaultText(data->fault_code));
    OLED_ShowString(0u, 3u, "Vin:");
    OLED_ShowFloat(36u, 3u, data->vin, 1u);
    OLED_ShowString(82u, 3u, "V");
    OLED_ShowString(0u, 5u, "Code:");
    OLED_ShowInt(48u, 5u, (int32_t)data->fault_code);
    OLED_ShowString(0u, 7u, "BACK Main");
}

/*
 * 函数: HMI_Display_Init
 * 调用周期: 在 HMI_Init() 中调用一次。
 * ISR: 否。
 * 阻塞: 在该抽象层中为否。
 */
void HMI_Display_Init(void)
{
    OLED_Clear();
    OLED_ShowString(0u, 0u, "C2000 INVERTER");
    OLED_ShowString(0u, 2u, "OLED READY");
    OLED_ShowString(0u, 7u, "STOP");
    (void)OLED_Update();
}

/*
 * Function: HMI_Display_Task
 * 调用周期: APP_DISPLAY_TASK_MS 后台任务。
 * ISR: 否，因为该任务会格式化文本并可能触发显示传输。
 * 阻塞: 在本层为否；板级传输应实现为非阻塞方式。
 */
void HMI_Display_Task(void)
{
    HMI_Data_t data;

    HMI_Param_GetData(&data);
    OLED_Clear();

    switch (HMI_Menu_GetPage())
    {
    case HMI_MENU_PAGE_DQ_MONITOR:
        HMI_Display_ShowDqMonitor(&data);
        break;

    case HMI_MENU_PAGE_SET_VREF:
        HMI_Display_ShowSetValue("Set Vref", data.vref, "V");
        break;

    case HMI_MENU_PAGE_SET_IREF:
        HMI_Display_ShowSetValue("Set Iref", data.iref, "A");
        break;

    case HMI_MENU_PAGE_SET_ENABLE:
        HMI_Display_ShowEnable(&data);
        break;

    case HMI_MENU_PAGE_SET_MODE:
        HMI_Display_ShowMode(&data);
        break;

    case HMI_MENU_PAGE_ADC_CAL:
        HMI_Display_ShowAdcCalibration(&data, HMI_Menu_GetEditIndex());
        break;

    case HMI_MENU_PAGE_FAULT:
        HMI_Display_ShowFault(&data);
        break;

    case HMI_MENU_PAGE_MAIN:
    default:
        HMI_Display_ShowMain(&data);
        break;
    }

    (void)OLED_Update();
}
