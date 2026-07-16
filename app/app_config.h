#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/*
 * 文件: app_config.h
 * 说明: C2000 电源 HMI 模板的全局配置。
 * 备注: 将用户限制统一放在这里，便于 HMI、测试和生成控制代码的胶水层
 *       共享同一套安全参数范围。
 */

#define APP_VREF_DEFAULT        36.0f
#define APP_IREF_DEFAULT        2.0f

#define APP_VREF_MIN            0.0f
#define APP_VREF_MAX            48.0f

/* The control model's Vdc lookup table is calibrated for this input range. */
#define APP_VBUS_MIN_VALID       20.0f
#define APP_VBUS_MAX_VALID       80.0f

#define APP_IREF_MIN            0.0f
#define APP_IREF_MAX            10.0f

#define APP_HMI_TASK_1MS        1u
#define APP_KEY_TASK_10MS       10u
#define APP_MENU_TASK_20MS      20u
#define APP_DISPLAY_TASK_100MS  100u

/* 长按计时以 Key_Task_10ms() 的调用节拍为单位。 */
#define APP_KEY_LONG_PRESS_TICKS 100u

#endif
