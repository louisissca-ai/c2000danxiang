#ifdef UNIT_TEST

#include <assert.h>

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

static void SetRunCommand(uint16_t enable_cmd)
{
    Control_Setpoint_t setpoint;

    setpoint.vref = 36.0f;
    setpoint.iref = 2.0f;
    setpoint.enable_cmd = enable_cmd;
    setpoint.mode_cmd = 0u;
    ControlIF_SetSetpoint(&setpoint);
}

int main(void)
{
    float initial_vref;

    ControlIF_Init();
    ControlModel_Init();

    SetRunCommand(APP_FALSE);
    assert(ControlModel_UpdateSafety(0.0f, 0.0f) == APP_FALSE);
    assert(ControlModel_GetFaultCode() == FAULT_NONE);
    assert(BoardPWM_IsReleased() == 0u);

    SetRunCommand(APP_TRUE);
    assert(ControlModel_UpdateSafety(19.9f, 0.0f) == APP_FALSE);
    assert(ControlModel_GetFaultCode() == FAULT_UVLO);
    assert(BoardPWM_IsReleased() == 0u);

    SetRunCommand(APP_FALSE);
    assert(ControlModel_UpdateSafety(19.9f, 0.0f) == APP_FALSE);
    assert(ControlModel_GetFaultCode() == FAULT_NONE);
    assert(BoardPWM_IsReleased() == 0u);

    HMI_Param_Init();
    HMI_Menu_Init();
    HMI_Param_SetFaultCode(FAULT_UVLO);
    HMI_Menu_Task_20ms(KEY_EVENT_RIGHT);
    assert(HMI_Menu_GetPage() == HMI_MENU_PAGE_SET_VREF);

    initial_vref = HMI_Param_GetVref();
    HMI_Menu_Task_20ms(KEY_EVENT_UP);
    assert(HMI_Param_GetVref() > initial_vref);

    HMI_Menu_Task_20ms(KEY_EVENT_RUN);
    assert(HMI_Param_GetEnableCmd() == APP_TRUE);

    return 0;
}

#endif
