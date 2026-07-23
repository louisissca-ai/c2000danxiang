/*
 * 文件: hmi_menu.c
 * 说明: 用于设定值页和状态页的非阻塞菜单状态机。
 * 备注: 可安全在 PC 上测试；不直接访问 GPIO、OLED 或控制模块。
 */

#include "hmi_menu.h"

#include "app_types.h"
#include "hmi_param.h"

static HMI_MenuPage_t g_hmi_menu_page;
static uint16_t g_hmi_menu_edit_index;

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
        if (g_hmi_menu_edit_index == 0u)
        {
            HMI_Param_AdjustVoutAdcB(0.001f * small_step);
        }
        else if (g_hmi_menu_edit_index == 1u)
        {
            HMI_Param_AdjustVoutAdcK(0.001f * small_step);
        }
        else if (g_hmi_menu_edit_index == 2u)
        {
            HMI_Param_AdjustIoutAdcB(0.001f * small_step);
        }
        else
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
}

/*
 * 函数: HMI_Menu_Task_20ms
 * 调用周期: 每经过一次或多次 Key_Task_10ms() 扫描后，每 20 ms 调用一次。
 * ISR: 否。
 * 阻塞: 否。
 */
void HMI_Menu_Task_20ms(KeyEvent_t event)
{
    switch (event)
    {
    case KEY_EVENT_UP:
        if (g_hmi_menu_page == HMI_MENU_PAGE_MAIN || g_hmi_menu_page == HMI_MENU_PAGE_FAULT)
        {
            HMI_Menu_PreviousPage();
        }
        else
        {
            HMI_Menu_AdjustCurrent(1);
        }
        break;

    case KEY_EVENT_DOWN:
        if (g_hmi_menu_page == HMI_MENU_PAGE_MAIN || g_hmi_menu_page == HMI_MENU_PAGE_FAULT)
        {
            HMI_Menu_NextPage();
        }
        else
        {
            HMI_Menu_AdjustCurrent(-1);
        }
        break;

    case KEY_EVENT_LONG_UP:
        HMI_Menu_AdjustCurrent((g_hmi_menu_page == HMI_MENU_PAGE_ADC_CAL) ? 100 : 10);
        break;

    case KEY_EVENT_LONG_DOWN:
        HMI_Menu_AdjustCurrent((g_hmi_menu_page == HMI_MENU_PAGE_ADC_CAL) ? -100 : -10);
        break;

    case KEY_EVENT_LEFT:
        HMI_Menu_PreviousPage();
        break;

    case KEY_EVENT_RIGHT:
        HMI_Menu_NextPage();
        break;

    case KEY_EVENT_OK:
        if (g_hmi_menu_page == HMI_MENU_PAGE_ADC_CAL)
        {
            g_hmi_menu_edit_index = (g_hmi_menu_edit_index + 1u) % 4u;
        }
        else
        {
            HMI_Menu_NextPage();
        }
        break;

    case KEY_EVENT_BACK:
        g_hmi_menu_page = HMI_MENU_PAGE_MAIN;
        break;

    case KEY_EVENT_RUN:
    case KEY_EVENT_LONG_RUN:
        HMI_Param_ToggleEnable();
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
