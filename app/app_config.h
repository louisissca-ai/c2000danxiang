#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/*
 * 文件: app_config.h
 * 说明: C2000 电源 HMI 模板的全局配置。
 * 备注: 将用户限制统一放在这里，便于 HMI、测试和生成控制代码的胶水层
 *       共享同一套安全参数范围。
 */

#define APP_VREF_DEFAULT        20.0f
#define APP_IREF_DEFAULT        3.0f
#define APP_OUTPUT_FREQUENCY_HZ 50.0f
#define APP_VBUS_NOMINAL_V      36.0f

#define APP_PWM_TBPRD_COUNTS              3000u
#define APP_PWM_DEADTIME_COUNTS            24u
/* ponytail: 24 counts assumes the present driver accepts a 200 ns effective pulse; raise it after gate-waveform calibration if needed. */
#define APP_PWM_MIN_EFFECTIVE_PULSE_COUNTS 24u
#define APP_PWM_MIN_COMPARE_COUNTS \
    (APP_PWM_DEADTIME_COUNTS + APP_PWM_MIN_EFFECTIVE_PULSE_COUNTS)
#define APP_PWM_MAX_MODULATION \
    (1.0f - 2.0f * (float)APP_PWM_MIN_COMPARE_COUNTS / \
    (float)APP_PWM_TBPRD_COUNTS)
#define APP_VREF_CAPABILITY_RESERVE 0.95f
#define APP_SQRT2                    1.4142135623730951f
#define APP_VREF_MAX \
    (APP_VBUS_NOMINAL_V * APP_PWM_MAX_MODULATION * \
    APP_VREF_CAPABILITY_RESERVE / APP_SQRT2)

#if (2u * APP_PWM_MIN_COMPARE_COUNTS >= APP_PWM_TBPRD_COUNTS)
#error "PWM minimum compare margin must be less than half of TBPRD"
#endif

#define APP_VREF_MIN            0.0f

#define APP_IREF_MIN            0.0f
#define APP_IREF_MAX            4.5f
/* ponytail: software OCP is temporarily disabled; set to 1 after ADC transient calibration. */
#define APP_SOFTWARE_OCP_ENABLED 0u

#define APP_ADC_CAL_VOUT_B_DEFAULT  1.643f
/*
 * One-point board calibration: 19.368 V true-RMS / 3.2 V indicated,
 * with the board voltage gain below.
 */
#define APP_ADC_CAL_VOUT_K_DEFAULT  109.781873f
#define APP_ADC_CAL_IOUT_B_DEFAULT  1.643f
#define APP_ADC_CAL_IOUT_K_DEFAULT  3.678481f
#define APP_ADC_CAL_B_MIN           0.0f
#define APP_ADC_CAL_B_MAX           3.3f
#define APP_ADC_CAL_K_MIN           0.001f
#define APP_ADC_CAL_K_MAX           1000.0f

#define APP_HMI_TASK_1MS        1u
#define APP_KEY_TASK_10MS       10u
#define APP_MENU_TASK_20MS      20u
#define APP_DISPLAY_TASK_100MS  100u

/* 长按计时以 Key_Task_10ms() 的调用节拍为单位。 */
#define APP_KEY_LONG_PRESS_TICKS 100u

#endif
