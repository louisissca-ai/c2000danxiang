#include "pwm_profile.h"

static const PWM_Profile_t g_pwm_profiles[] =
{
    {20u, 3000u, 400u, 20u, 10000uL, 400uL, 50.0e-6f}
};

const PWM_Profile_t *PWM_Profile_Get(uint16_t frequency_khz)
{
    uint16_t index;

    for (index = 0u;
         index < (uint16_t)(sizeof(g_pwm_profiles) / sizeof(g_pwm_profiles[0]));
         index++)
    {
        if (g_pwm_profiles[index].frequency_khz == frequency_khz)
        {
            return &g_pwm_profiles[index];
        }
    }

    return 0;
}
