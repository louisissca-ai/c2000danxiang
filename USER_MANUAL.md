# 三相逆变器 CCS 工程用户使用说明

本文档基于当前 `ccs` 工程源码整理，适用于 TMS320F2800157 / LAUNCHXL-F2800157 目标板。若硬件原理图或 Simulink 模型重新生成，请重新核对 `model/` 和 `drivers_user/` 下的引脚配置。

## 1. 工程概况

- 工程类型：TI Code Composer Studio 独立工程。
- 目标芯片：TMS320F2800157。
- 建议工具链：CCS 12.4 兼容版本，C2000 编译器 22.6.0.LTS。
- 主控制模型：`model/chovy_tidy.c`，由 Simulink 生成。
- 手写应用层：`app/`、`hmi/`、`control_if/`、`drivers_user/`。
- 入口函数：`model/ert_main.c`。

运行链路：

1. `main()` 调用 `init_board()` 初始化系统时钟、PIE、ePWM GPIO 和 ePWM 时钟。
2. `chovy_tidy_initialize()` 初始化 ePWM、ADC、控制接口和 HMI。
3. Timer0 以 50 us 基准周期调用 `chovy_tidy_step0()`。
4. 1 ms 子任务调用 `chovy_tidy_step2()`，并把反馈量送到 HMI。
5. 后台循环 `APP_BackgroundLoop()` 刷新 HMI、按键和 OLED。

## 2. 编译和下载

1. 打开 CCS。
2. 选择 `File > Import > Code Composer Studio > CCS Projects`。
3. 选择本目录 `E:\c2000\Three_phase_inverter\ccs`，导入工程 `ccs`。
4. 选择 `Debug` 或 `Release` 配置，执行 `Build Project`。
5. 使用 `device/f2800157.ccxml` 或 `NewTargetConfiguration.ccxml` 连接目标板。
6. 下载并运行生成的 `ccs.out`。

## 3. PWM 输出引脚

PWM 由 `model/MW_c28xx_pwm.c` 配置，引脚复用如下：

| 功能 | ePWM 模块 | 芯片 GPIO | 说明 |
| --- | --- | ---: | --- |
| A 相 PWM 高/主输出 | EPWM1A | GPIO0 | 模型中 `rtb_duty[0]` 更新 CMPA |
| A 相 PWM 互补输出 | EPWM1B | GPIO1 | 由 ePWM 死区模块生成 |
| B 相 PWM 高/主输出 | EPWM2A | GPIO2 | 模型中 `rtb_duty[1]` 更新 CMPA |
| B 相 PWM 互补输出 | EPWM2B | GPIO3 | 由 ePWM 死区模块生成 |
| C 相 PWM 高/主输出 | EPWM3A | GPIO4 | 模型中 `rtb_duty[2]` 更新 CMPA |
| C 相 PWM 互补输出 | EPWM3B | GPIO5 | 由 ePWM 死区模块生成 |

当前 PWM 配置：

- 频率：20 kHz。`TBPRD = 3000`，系统时钟 120 MHz，上下计数模式。
- 占空比：模型计算 0% 到 100%，写入 EPWM1/2/3 的 CMPA。
- 死区：`DBRED = 24`，`DBFED = 24`。
- ADC 触发：EPWM1/2/3 均开启 SOCA；ADC1/2/4/5/6 使用 `TRIGSEL = 5`，即 ePWM1 SOCA 触发。
- 安全关断：故障或未使能时，`BoardPWM_ForceSafe()` 强制 EPWM1/2/3 Trip Zone one-shot，TZA/TZB 输出拉低；恢复时 `BoardPWM_Release()` 清除 OST 标志。

## 4. ADC 输入通道

ADC 配置位于 `model/MW_c28xx_adc.c`，采样结果在 `model/chovy_tidy.c` 中使用。

| 模型信号 | ADC 模块/SOC | ADC 通道 | 触发源 | 工程内可见模拟 GPIO | 用途 |
| --- | --- | --- | --- | --- | --- |
| ADC1 | ADCC SOC1 | ADCINC1 | ePWM1 SOCA | 未在工程中配置 GPIO 号 | 三相电压反馈之一 |
| ADC2 | ADCA SOC0 | ADCINA0 | ePWM1 SOCA | 未在工程中配置 GPIO 号 | 三相电流反馈之一 |
| ADC3 | ADCC SOC2 | ADCINC2 | ePWM1 SOCA | 未在工程中配置 GPIO 号 | 三相电压反馈之一 |
| ADC4 | ADCA SOC2 | ADCINA2 | ePWM1 SOCA | GPIO224 置模拟模式 | 三相电流反馈之一 |
| ADC5 | ADCC SOC0 | ADCINC0 | ePWM1 SOCA | 未在工程中配置 GPIO 号 | 三相电压反馈之一 |
| ADC6 | ADCA SOC1 | ADCINA1 | ePWM1 SOCA | 未在工程中配置 GPIO 号 | 三相电流反馈之一 |
| ADC8 | ADCA SOC3 | ADCINA3 | ePWM1 SOCA | GPIO242 置模拟模式 | 母线电压 Vin/Vdc |

模型中的量纲换算：

- `ADC8` 经 `3.3 / 4095` 和校准系数换算为母线电压 `vin`，并用于安全判断和 PWM 归一化。
- `ADC5/ADC1/ADC3` 经偏置和比例换算后进入三相电压 `abc to dq0`。
- `ADC2/ADC6/ADC4` 经偏置和比例换算后进入三相电流 `abc to dq1`，并计算输出电流幅值。
- ADC 采样窗口：8 个 ADC clock cycles，`ACQPS = 7`。

注意：工程源码能确认的是 ADC 模块通道名和部分模拟 GPIO 模式配置；LAUNCHXL 排针号、芯片封装物理脚位需要以所用板卡原理图为准。

## 5. OLED 和按键引脚

OLED 和按键引脚定义在 `drivers_user/board_pinmap.h`。

### OLED SPI

| 功能 | 外设/信号 | GPIO | 说明 |
| --- | --- | ---: | --- |
| OLED MOSI | SPIA SIMO | GPIO8 | 4 线 SPI 数据输出 |
| OLED SCLK | SPIA CLK | GPIO9 | SPI 时钟 |
| OLED CS | GPIO | GPIO11 | 片选，低有效 |
| OLED DC | GPIO | GPIO42 | 命令/数据选择 |
| OLED RES | GPIO | GPIO43 | 复位 |

OLED 参数：

- 分辨率：128 x 64。
- SPI 频率：4 MHz。
- 刷新周期：100 ms。
- X 偏移：2 像素。

### 4x4 矩阵键盘

默认 `KEY_MODE = KEY_MODE_MATRIX`。

| 矩阵信号 | GPIO | 方向 | 电平 |
| --- | ---: | --- | --- |
| ROW0 | GPIO25 | 输出 | 扫描时拉低有效 |
| ROW1 | GPIO26 | 输出 | 扫描时拉低有效 |
| ROW2 | GPIO27 | 输出 | 扫描时拉低有效 |
| ROW3 | GPIO44 | 输出 | 扫描时拉低有效 |
| COL0 | GPIO45 | 输入上拉 | 读到低电平表示按下 |
| COL1 | GPIO46 | 输入上拉 | 读到低电平表示按下 |
| COL2 | GPIO48 | 输入上拉 | 读到低电平表示按下 |
| COL3 | GPIO33 | 输入上拉 | 读到低电平表示按下 |

按键逻辑映射：

| 按键 | 矩阵位置 |
| --- | --- |
| UP | ROW0 / COL0 |
| DOWN | ROW0 / COL1 |
| LEFT | ROW0 / COL2 |
| RIGHT | ROW0 / COL3 |
| OK | ROW1 / COL3 |
| BACK | ROW2 / COL3 |
| RUN | ROW3 / COL3 |

按键扫描周期为 10 ms，消抖 3 次，长按阈值为 100 个扫描周期，约 1 s。

## 6. HMI 操作

OLED 主页面显示：

- `Vin`：母线输入电压。
- `Vout`：输出电压反馈。
- `Iout`：输出电流反馈。
- `Duty`：三相最大占空比。
- `State`：`STOP`、`RUN` 或 `FAULT`。
- 故障文本：`NONE`、`OVP`、`OCP`、`OTP`、`UVLO`、`ADC`、`PWM`。

菜单页面：

| 页面 | 作用 | 默认值/范围 |
| --- | --- | --- |
| Main | 运行状态和反馈量 | 只显示 |
| Set Vref | 设置输出电压参考 | 默认 36.0 V，范围 0.0 到 48.0 V |
| Set Iref | 设置输出电流限制 | 默认 2.0 A，范围 0.0 到 10.0 A |
| Enable | 使能/停止控制 | 默认停止 |
| Mode | 模式命令 | 0 到 3 |
| Fault | 故障详情 | 只显示 |

按键操作：

| 按键 | 主页面/故障页 | 设置页 |
| --- | --- | --- |
| UP | 上一页 | 数值 +0.1 |
| DOWN | 下一页 | 数值 -0.1 |
| 长按 UP | 当前页不变 | 数值 +1.0 |
| 长按 DOWN | 当前页不变 | 数值 -1.0 |
| LEFT | 上一页 | 上一页 |
| RIGHT / OK | 下一页 | 下一页 |
| BACK | 返回 Main | 返回 Main |
| RUN / 长按 RUN | 切换 Enable | 切换 Enable |
| 长按 OK | 返回 Main | 返回 Main |

## 7. 保护逻辑

保护逻辑位于 `control_if/control_model_if.c`：

- 未使能时：清除故障，PWM 强制安全关断。
- ADC 数据非法、非有限值或母线电压小于等于 0：`FAULT_ADC`。
- 母线电压低于 20.0 V：`FAULT_UVLO`。
- 母线电压高于 80.0 V：`FAULT_OVP`。
- 输出电流幅值大于 `Iref`：`FAULT_OCP`。

发生故障后 PWM 保持关断。需要先停止使能，待条件恢复后再重新使能。

## 8. 使用注意事项

- 上电前确认三相桥驱动、电流采样、电压采样和母线电源接线正确。
- 首次运行建议不要接高压母线，先用低压或隔离电源验证 PWM、ADC 和 HMI 显示。
- ADC 输入不得超过器件允许模拟输入范围；工程按 0 到 3.3 V ADC 量程换算。
- 修改 PWM 或 ADC 配置时，优先修改 Simulink 模型后重新生成；手写修改 `model/` 文件容易在下次生成时被覆盖。
- 修改 OLED 或按键接线时，优先更新 `drivers_user/board_pinmap.h`，不要在驱动实现里散落硬编码。
