/*
 * File: board.c
 * Description: Unified board initialization and PWM safety for LAUNCHXL-F2800157.
 */

#include "board.h"

#include "board_gpio.h"
#include "board_spi.h"
#include "key.h"
#include "oled.h"

#ifndef UNIT_TEST
#include "driverlib.h"
#endif

static uint16_t g_board_pwm_released;

#ifndef UNIT_TEST
static void BoardPWM_ConfigureLowAction(uint32_t base)
{
    EPWM_setTripZoneAction(base, EPWM_TZ_ACTION_EVENT_TZA, EPWM_TZ_ACTION_LOW);
    EPWM_setTripZoneAction(base, EPWM_TZ_ACTION_EVENT_TZB, EPWM_TZ_ACTION_LOW);
}
#endif

void Board_Init(void)
{
    BoardGPIO_Init();
    BoardSPI_Init();
    Key_Init();
    (void)OLED_Init();
}

void BoardPWM_ForceSafe(void)
{
#ifndef UNIT_TEST
    BoardPWM_ConfigureLowAction(EPWM1_BASE);
    BoardPWM_ConfigureLowAction(EPWM2_BASE);
    BoardPWM_ConfigureLowAction(EPWM3_BASE);
    EPWM_forceTripZoneEvent(EPWM1_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(EPWM2_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(EPWM3_BASE, EPWM_TZ_FORCE_EVENT_OST);
#endif
    g_board_pwm_released = 0u;
}

void BoardPWM_Release(void)
{
#ifndef UNIT_TEST
    EPWM_clearTripZoneFlag(EPWM1_BASE, EPWM_TZ_FLAG_OST);
    EPWM_clearTripZoneFlag(EPWM2_BASE, EPWM_TZ_FLAG_OST);
    EPWM_clearTripZoneFlag(EPWM3_BASE, EPWM_TZ_FLAG_OST);
#endif
    g_board_pwm_released = 1u;
}

uint16_t BoardPWM_IsReleased(void)
{
    return g_board_pwm_released;
}
