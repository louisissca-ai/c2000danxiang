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

#define APP_VREF_MIN            0.0f
#define APP_VREF_MAX            25.0f

#define APP_IREF_MIN            0.0f
#define APP_IREF_MAX            4.5f
/* ponytail: software OCP is temporarily disabled; set to 1 after ADC transient calibration. */
#define APP_SOFTWARE_OCP_ENABLED 0u

#define APP_ADC_CAL_VOUT_B_DEFAULT  1.608f
#define APP_ADC_CAL_VOUT_K_DEFAULT  51.37954f
#define APP_ADC_CAL_IOUT_B_DEFAULT  1.606f
#define APP_ADC_CAL_IOUT_K_DEFAULT  3.33333f
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
