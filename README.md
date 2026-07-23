# F2800157 单相全桥逆变器

本目录是可直接导入 Code Composer Studio 12.4 的独立工程。控制算法由
`xtq2_dq_doubleloop_fullspec.slx` 使用 MATLAB/Simulink R2024a Update 8 和
Embedded Coder 的 TI C2000 目标生成；手写调度、安全闭锁和 HMI 接口位于
`app/` 与 `control_if/`，不会混入生成算法文件。

## 控制与硬件配置

| 项目 | 配置 |
| --- | --- |
| MCU | TMS320F2800157，120 MHz |
| 控制周期 | 50 us（20 kHz） |
| 输出基波 | 50 Hz |
| 拓扑 | 单相全桥，双闭环 dq 控制 |
| A 桥臂 | EPWM1A/1B，GPIO0/GPIO1 |
| B 桥臂 | EPWM2A/2B，GPIO2/GPIO3 |
| PWM | 中心对齐，TBPRD=3000，死区 24 TBCLK（约 200 ns） |
| 输出电压采样 | ADCA SOC0 / ADCIN4，EPWM1 SOCA 触发 |
| 输出电流采样 | ADCC SOC0 / ADCIN4，EPWM1 SOCA 触发 |
| ADC 标定 | `(raw * 3.3 / 4095 - 1.686) / 0.019463` |

生成的算法位于 `model/xtq2_dq_doubleloop_fullspec.*`。`app/firmware_main.c`
在 20 kHz ISR 中执行模型、将 HMI 的电压设定送入模型参数，并把电压/电流
RMS、占空比和故障状态回传给 HMI。

## 安全行为

- 上电默认不使能；EPWM1/EPWM2 通过 Trip Zone 强制为低电平。
- HMI 的 RUN 命令通过 `control_if/` 释放 PWM，电压参考从 0 V 斜坡上升。
- ADC 非法、欠压、过压、过流或 ISR 超时会闭锁 PWM；先停止使能再重新启动
  才能清除闭锁。
- 电压参考范围为 0–25 V RMS；软件过流设定范围为 0–4.5 A。

当前模型把直流母线 `Vin` 固定为 36 V，没有独立的母线 ADC 通道。因此软件
欠压/过压判断使用该模型参数，不能替代真实母线保护。接功率级前必须按实际
板卡校准 ADC 偏置/增益，并使用硬件过流、欠压和急停保护。

## 编译与烧录

1. 在 CCS 中导入本目录，工程名为 `single_phase_inverter`。
2. 选择 Debug 或 Release 并 Build Project。
3. 使用 `device/f2800157.ccxml` 连接 XDS110。
4. 下载生成的 `single_phase_inverter.out`。固件下载后仍保持 PWM 关断，需从
   HMI 明确执行 RUN 才会释放输出。

主机侧安全/HMI 自检：

```powershell
gcc -DUNIT_TEST -I app -I control_if -I drivers_user -I hmi `
  tests/run_fault_hmi_check.c control_if/control_interface.c `
  control_if/control_model_if.c hmi/hmi_param.c hmi/hmi_menu.c `
  -o tests/run_fault_hmi_check.exe
tests/run_fault_hmi_check.exe
```

## 目录

- `model/`：从 Simulink 生成的单相控制算法和 ADC/PWM 初始化代码。
- `app/`：目标调度、默认参数和后台任务。
- `control_if/`：HMI、保护逻辑与生成模型之间的边界。
- `hmi/`、`drivers_user/`：OLED、按键和板级驱动。
- `support/`、`device/`：MathWorks C2000 支持代码和 TI 器件文件。
- `tests/`：不依赖硬件的最小自检。
