/*
 * 文件: hmi_menu.c
 * 说明: 用于设定值页和状态页的非阻塞菜单状态机。
 * 备注: 可安全在 PC 上测试；不直接访问 GPIO、OLED 或控制模块。
 */

#include "hmi_menu.h"

#include "adc_calibration.h"
#include "app_types.h"
#include "hmi_param.h"

static HMI_MenuPage_t g_hmi_menu_page;
static uint16_t g_hmi_menu_edit_index;
static uint16_t g_hmi_cal_editing;
static uint16_t g_hmi_cal_digit;

#define HMI_CAL_ITEM_COUNT 7u

static void HMI_Menu_NextPage(void)
{
    if (g_hmi_menu_page >= HMI_MENU_PAGE_FAULT)
    {
        g_hmi_menu_page = HMI_MENU_PAGE_MAIN;
    }
    else
    {
        g_hmi_menu_page = (HMI_MenuPage_t)((uint16_t)g_hmi_menu_page + 1u);
    }
    g_hmi_menu_edit_index = 0u;
    g_hmi_cal_editing = 0u;
}

static void HMI_Menu_PreviousPage(void)
{
    if (g_hmi_menu_page == HMI_MENU_PAGE_MAIN)
    {
        g_hmi_menu_page = HMI_MENU_PAGE_FAULT;
    }
    else
    {
        g_hmi_menu_page = (HMI_MenuPage_t)((uint16_t)g_hmi_menu_page - 1u);
    }
    g_hmi_menu_edit_index = 0u;
    g_hmi_cal_editing = 0u;
}

static void HMI_Menu_AdjustCurrent(int16_t step)
{
    float small_step;
    uint16_t mode_cmd;

    small_step = (float)step;

    switch (g_hmi_menu_page)
    {
    case HMI_MENU_PAGE_SET_VREF:
        HMI_Param_AdjustVref(0.1f * small_step);
        break;

    case HMI_MENU_PAGE_SET_IREF:
        HMI_Param_AdjustIref(0.1f * small_step);
        break;

    case HMI_MENU_PAGE_SET_ENABLE:
        HMI_Param_ToggleEnable();
        break;

    case HMI_MENU_PAGE_SET_MODE:
        mode_cmd = HMI_Param_GetModeCmd();
        if (step != 0)
        {
            mode_cmd = (mode_cmd == APP_CONTROL_MODE_CLOSED_LOOP) ?
                APP_CONTROL_MODE_OPEN_LOOP : APP_CONTROL_MODE_CLOSED_LOOP;
            HMI_Param_SetEnableCmd(APP_FALSE);
            HMI_Param_SetModeCmd(mode_cmd);
        }
        break;

    case HMI_MENU_PAGE_ADC_CAL:
        if (g_hmi_cal_editing == 0u)
        {
            if (step > 0)
            {
                g_hmi_menu_edit_index = (g_hmi_menu_edit_index == 0u) ?
                    (HMI_CAL_ITEM_COUNT - 1u) :
                    (g_hmi_menu_edit_index - 1u);
            }
            else if (step < 0)
            {
                g_hmi_menu_edit_index =
                    (g_hmi_menu_edit_index + 1u) % HMI_CAL_ITEM_COUNT;
            }
        }
        else if (g_hmi_menu_edit_index == 1u ||
            g_hmi_menu_edit_index == 2u)
        {
            ADC_Cal_Channel_t channel = (g_hmi_menu_edit_index == 1u) ?
                ADC_CAL_CHANNEL_VOUT : ADC_CAL_CHANNEL_IOUT;
            float digit_step = 0.001f;
            uint16_t digit;

            for (digit = 0u; digit < g_hmi_cal_digit; digit++)
            {
                digit_step *= 10.0f;
            }
            ADC_Cal_SetTrueRms(channel, ADC_Cal_GetTrueRms(channel) +
                digit_step * small_step);
        }
        else if (g_hmi_menu_edit_index == 3u)
        {
            HMI_Param_AdjustVoutAdcB(0.001f * small_step);
        }
        else if (g_hmi_menu_edit_index == 4u)
        {
            HMI_Param_AdjustVoutAdcK(0.001f * small_step);
        }
        else if (g_hmi_menu_edit_index == 5u)
        {
            HMI_Param_AdjustIoutAdcB(0.001f * small_step);
        }
        else if (g_hmi_menu_edit_index == 6u)
        {
            HMI_Param_AdjustIoutAdcK(0.001f * small_step);
        }
        break;

    default:
        break;
    }
}

/*
 * 函数: HMI_Menu_Init
 * 调用周期: 在 HMI_Init() 中调用一次。
 * ISR: 否。
 * 阻塞: 否。
 */
void HMI_Menu_Init(void)
{
    g_hmi_menu_page = HMI_MENU_PAGE_MAIN;
    g_hmi_menu_edit_index = 0u;
    g_hmi_cal_editing = 0u;
    g_hmi_cal_digit = 0u;
}

/*
 * 函数: HMI_Menu_Task_20ms
 * 调用周期: 每经过一次或多次 Key_Task_10ms() 扫描后，每 20 ms 调用一次。
 * ISR: 否。
 * 阻塞: 否。
 */
void HMI_Menu_Task_20ms(KeyEvent_t event)
{
    if ((g_hmi_menu_page == HMI_MENU_PAGE_ADC_CAL) &&
        (ADC_Cal_IsBusy() != APP_FALSE) &&
        (event != KEY_EVENT_BACK) && (event != KEY_EVENT_NONE))
    {
        return;
    }

    switch (event)
    {
    case KEY_EVENT_UP:
        if (g_hmi_menu_page == HMI_MENU_PAGE_MAIN ||
            g_hmi_menu_page == HMI_MENU_PAGE_DQ_MONITOR ||
            g_hmi_menu_page == HMI_MENU_PAGE_FAULT)
        {
            HMI_Menu_PreviousPage();
        }
        else
        {
            HMI_Menu_AdjustCurrent(1);
        }
        break;

    case KEY_EVENT_DOWN:
        if (g_hmi_menu_page == HMI_MENU_PAGE_MAIN ||
            g_hmi_menu_page == HMI_MENU_PAGE_DQ_MONITOR ||
            g_hmi_menu_page == HMI_MENU_PAGE_FAULT)
        {
            HMI_Menu_NextPage();
        }
        else
        {
            HMI_Menu_AdjustCurrent(-1);
        }
        break;

    case KEY_EVENT_LONG_UP:
        HMI_Menu_AdjustCurrent((g_hmi_menu_page == HMI_MENU_PAGE_ADC_CAL &&
            g_hmi_cal_editing != 0u &&
            g_hmi_menu_edit_index >= 3u) ? 100 : 10);
        break;

    case KEY_EVENT_LONG_DOWN:
        HMI_Menu_AdjustCurrent((g_hmi_menu_page == HMI_MENU_PAGE_ADC_CAL &&
            g_hmi_cal_editing != 0u &&
            g_hmi_menu_edit_index >= 3u) ? -100 : -10);
        break;

    case KEY_EVENT_LEFT:
        if (g_hmi_menu_page == HMI_MENU_PAGE_ADC_CAL &&
            g_hmi_cal_editing != 0u &&
            (g_hmi_menu_edit_index == 1u ||
            g_hmi_menu_edit_index == 2u))
        {
            if (g_hmi_cal_digit > 0u)
            {
                g_hmi_cal_digit--;
            }
        }
        else
        {
            HMI_Menu_PreviousPage();
        }
        break;

    case KEY_EVENT_RIGHT:
        if (g_hmi_menu_page == HMI_MENU_PAGE_ADC_CAL &&
            g_hmi_cal_editing != 0u &&
            (g_hmi_menu_edit_index == 1u ||
            g_hmi_menu_edit_index == 2u))
        {
            if (g_hmi_cal_digit < 4u)
            {
                g_hmi_cal_digit++;
            }
        }
        else
        {
            HMI_Menu_NextPage();
        }
        break;

    case KEY_EVENT_OK:
        if (g_hmi_menu_page == HMI_MENU_PAGE_ADC_CAL)
        {
            HMI_Data_t data;

            HMI_Param_GetData(&data);
            if (ADC_Cal_IsBusy() != APP_FALSE)
            {
                break;
            }
            if (g_hmi_cal_editing == 0u)
            {
                if (g_hmi_menu_edit_index == 0u)
                {
                    (void)ADC_Cal_StartOffset();
                }
                else
                {
                    g_hmi_cal_editing = 1u;
                    g_hmi_cal_digit = 0u;
                    if ((g_hmi_menu_edit_index == 1u) &&
                        (data.vout > 0.001f))
                    {
                        ADC_Cal_SetTrueRms(ADC_CAL_CHANNEL_VOUT,
                            data.vout);
                    }
                    else if ((g_hmi_menu_edit_index == 2u) &&
                        (data.iout > 0.001f))
                    {
                        ADC_Cal_SetTrueRms(ADC_CAL_CHANNEL_IOUT,
                            data.iout);
                    }
                }
            }
            else if (g_hmi_menu_edit_index == 1u)
            {
                (void)ADC_Cal_ApplyGain(ADC_CAL_CHANNEL_VOUT,
                    ADC_Cal_GetTrueRms(ADC_CAL_CHANNEL_VOUT), data.vout);
                g_hmi_cal_editing = 0u;
            }
            else if (g_hmi_menu_edit_index == 2u)
            {
                (void)ADC_Cal_ApplyGain(ADC_CAL_CHANNEL_IOUT,
                    ADC_Cal_GetTrueRms(ADC_CAL_CHANNEL_IOUT), data.iout);
                g_hmi_cal_editing = 0u;
            }
            else
            {
                g_hmi_cal_editing = 0u;
            }
        }
        else
        {
            HMI_Menu_NextPage();
        }
        break;

    case KEY_EVENT_BACK:
        if (g_hmi_menu_page == HMI_MENU_PAGE_ADC_CAL &&
            ADC_Cal_IsBusy() != APP_FALSE)
        {
            ADC_Cal_Cancel();
        }
        else if (g_hmi_menu_page == HMI_MENU_PAGE_ADC_CAL &&
            g_hmi_cal_editing != 0u)
        {
            g_hmi_cal_editing = 0u;
        }
        else
        {
            g_hmi_menu_page = HMI_MENU_PAGE_MAIN;
        }
        break;

    case KEY_EVENT_RUN:
    case KEY_EVENT_LONG_RUN:
        if (ADC_Cal_IsBusy() == APP_FALSE)
        {
            HMI_Param_ToggleEnable();
        }
        break;

    case KEY_EVENT_LONG_OK:
        g_hmi_menu_page = HMI_MENU_PAGE_MAIN;
        break;

    default:
        break;
    }

}

HMI_MenuPage_t HMI_Menu_GetPage(void)
{
    return g_hmi_menu_page;
}

uint16_t HMI_Menu_GetEditIndex(void)
{
    return g_hmi_menu_edit_index;
}

uint16_t HMI_Menu_IsCalEditing(void)
{
    return g_hmi_cal_editing;
}

uint16_t HMI_Menu_GetCalDigit(void)
{
    return g_hmi_cal_digit;
}
