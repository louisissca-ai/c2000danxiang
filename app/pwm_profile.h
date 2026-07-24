#ifndef PWM_PROFILE_H
#define PWM_PROFILE_H

#include <stdint.h>

typedef struct
{
    uint16_t frequency_khz;
    uint16_t tbprd_counts;
    uint16_t rms_samples;
    uint16_t task_1ms_divider;
    uint32_t calibration_settle_samples;
    uint32_t calibration_window_samples;
    float control_step_s;
} PWM_Profile_t;

const PWM_Profile_t *PWM_Profile_Get(uint16_t frequency_khz);

#endif
