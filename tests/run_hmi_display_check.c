#ifdef UNIT_TEST

#include <assert.h>
#include <math.h>
#include <string.h>

#include "adc_calibration.h"
#include "hmi_display.h"
#include "hmi_menu.h"
#include "hmi_param.h"
#include "oled.h"

static HMI_MenuPage_t g_page;
static uint16_t g_edit_index;
static uint16_t g_editing;
static uint16_t g_saw_vnow;
static uint16_t g_saw_inow;
static float g_line_1_value;
static float g_line_2_value;
static float g_line_3_value;
static float g_line_4_value;

void OLED_Clear(void)
{
}

uint16_t OLED_Update(void)
{
    return OLED_STATUS_OK;
}

void OLED_ShowString(uint16_t x, uint16_t y, const char *str)
{
    (void)x;
    (void)y;
    if (strcmp(str, "Vnow:") == 0)
    {
        g_saw_vnow = 1u;
    }
    else if (strcmp(str, "Inow:") == 0)
    {
        g_saw_inow = 1u;
    }
}

void OLED_ShowInt(uint16_t x, uint16_t y, int32_t value)
{
    (void)x;
    (void)y;
    (void)value;
}

void OLED_ShowFloat(uint16_t x, uint16_t y, float value, uint16_t decimals)
{
    (void)x;
    (void)decimals;
    if (y == 1u)
    {
        g_line_1_value = value;
    }
    else if (y == 2u)
    {
        g_line_2_value = value;
    }
    else if (y == 3u)
    {
        g_line_3_value = value;
    }
    else if (y == 4u)
    {
        g_line_4_value = value;
    }
}

HMI_MenuPage_t HMI_Menu_GetPage(void)
{
    return g_page;
}

uint16_t HMI_Menu_GetEditIndex(void)
{
    return g_edit_index;
}

uint16_t HMI_Menu_IsCalEditing(void)
{
    return g_editing;
}

uint16_t HMI_Menu_GetCalDigit(void)
{
    return 0u;
}

void ADC_Cal_GetView(ADC_Cal_View_t *view)
{
    memset(view, 0, sizeof(*view));
}

float ADC_Cal_GetTrueRms(ADC_Cal_Channel_t channel)
{
    (void)channel;
    return 20.0f;
}

int main(void)
{
    HMI_Param_Init();
    HMI_Param_SetVout(20.0f);
    HMI_Param_SetIout(2.0f);
    HMI_Param_SetVoutInst(-28.3f);
    HMI_Param_SetIoutInst(-2.5f);
    HMI_Param_SetDq(14.1f, 0.2f, 1.0f, -0.1f);

    g_page = HMI_MENU_PAGE_MAIN;
    HMI_Display_Task();
    assert(g_saw_vnow != 0u);
    assert(g_saw_inow != 0u);
    assert(fabsf(g_line_1_value - -28.3f) < 0.0001f);
    assert(fabsf(g_line_2_value - -2.5f) < 0.0001f);

    g_page = HMI_MENU_PAGE_DQ_MONITOR;
    HMI_Display_Task();
    assert(fabsf(g_line_1_value - 14.1f) < 0.0001f);
    assert(fabsf(g_line_2_value - 0.2f) < 0.0001f);
    assert(fabsf(g_line_3_value - 1.0f) < 0.0001f);
    assert(fabsf(g_line_4_value - -0.1f) < 0.0001f);

    g_page = HMI_MENU_PAGE_ADC_CAL;
    g_edit_index = 1u;
    g_editing = 1u;
    HMI_Display_Task();
    assert(fabsf(g_line_3_value - 20.0f) < 0.0001f);
    return 0;
}

#endif
