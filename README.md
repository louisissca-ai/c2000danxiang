# F2800157 单相全桥逆变器

本目录是可直接导入 Code Composer Studio 12.4 的独立工程。控制算法由
`xtq2_dq_doubleloop_fullspec.slx` 使用 MATLAB/Simulink R2024a Update 8 和
Embedded Coder 的 TI C2000 目标生成；手写调度、安全闭锁和 HMI 接口位于
`app/` 与 `control_if/`，不会混入生成算法文件。

## 控制与硬件配置

| 项目 | 配置 |
| --- | --- |
| MCU | TMS320F2800157，120 MHz |
| 控制周期 | 50 us（20 kHz），由 EPWM1 SOCA → ADCC EOC0 同步调度 |
| 输出基波 | 50 Hz |
| 拓扑 | 单相全桥，双闭环 dq / 开环 SPWM 可切换 |
| A 桥臂 | EPWM1A/1B，GPIO0/GPIO1 |
| B 桥臂 | EPWM2A/2B，GPIO2/GPIO3 |
| PWM | 中心对齐，TBPRD=3000，死区 24 TBCLK（约 200 ns），开环 CMPA 限制为 48–2952 |
| 输出电压采样 | ADCA SOC0 / ADCIN4，EPWM1 SOCA 触发 |
| 输出电流采样 | ADCC SOC0 / ADCIN4，EPWM1 SOCA 触发 |
| 采样滤波 | 不新增ADC前置低通；使用模型原有dq与主动阻尼滤波器 |
| Vout ADC 标定 | `(raw * 3.3 / 4095 - 1.643) * 109.781873`，控制极性由 `APP_VOUT_CONTROL_POLARITY` 配置 |
| Iout ADC 标定 | `(raw * 3.3 / 4095 - 1.643) * 3.678481`，控制极性为`+1` |
| 主动阻尼增益 Kad | `8.261049`（随 Vout 标定倍率补偿） |
| 电流环 PI | `Kp=5.026548`、`Ki=125.663706`（1 mH，约800 Hz） |
| 电压环 PI | `Kp=0.006`、`Ki=2.5`（10 µF，约80 Hz） |

生成的算法位于 `model/xtq2_dq_doubleloop_fullspec.*`。`app/firmware_main.c`
在 20 kHz ISR 中执行模型、将 HMI 的电压设定送入模型参数，并把电压/电流
RMS、带符号瞬时值、占空比和故障状态回传给 HMI。

`ADC Cal` 页包含 `ZERO AUTO`、`V GAIN`、`I GAIN` 以及四个手动 b/k 字段，对应
`Vout/Iout = (raw * 3.3 / 4095 - b) * k`。UP/DOWN 选择项目，OK 进入；
零点自动校准要求 STOP，先等待 0.5 秒，再用约 2 秒分块采样的截尾均值求真实偏置；
增益自动校准要求开环 RUN，输入外部真有效值后
会自动停机并在本次上电期间应用。所有校准值都只保存在 RAM，重新上电后恢复编译期默认值；
手动字段仍支持 0.001 微调和长按 0.1 微调。

## 控制模式

- `CLOSED LOOP`：使用现有双闭环 dq 控制，`Vref` 是输出电压 RMS 设定值。
- `OPEN LOOP`：使用 50 Hz 单极性倍频 SPWM，并沿用 `Vref`。36 V 母线下调制度
  为 `m = clamp(sqrt(2) * Vref / 36, 0, 0.968)`，两桥臂占空比分别为
  `50% * (1 + m*sin(theta))` 和 `50% * (1 - m*sin(theta))`。
- 在 HMI 的 `Set Mode` 页面改变模式会自动停止 PWM 并清除控制状态；必须再次
  按 RUN 才会以新模式启动。默认模式为 `CLOSED LOOP`，默认 `Vref` 为 20 V RMS。

开环计算使用固定的 36 V 母线参数，因此实际输出 RMS 会随真实母线电压、负载、
滤波器和死区变化；采样 RMS 仅用于测量和校准，不参与开环幅值调节。
0.968 的调制度上限在每个桥臂两端各保留 48 TBCLK，其中包含 24 TBCLK 死区和
24 TBCLK 最小有效导通时间，避免峰值附近的窄脉冲被死区吞没。
闭环也使用同一调制度和 CMPA 边界；最终电压命令发生饱和时，削减量回算到电流
内环积分器，避免波峰削顶期间继续积分。36 V 标称母线再保留 5% 能力余量后，
HMI 可设电压上限约为 23.4 V RMS。

闭环 dq 外环调节 50 Hz 基波；固件另外对校准后的输出电压每 400 点计算
一次总有效值。总 RMS 仅用于测量和 ADC 增益校准，不再积分修正 dq 基波参考；
闭环 dq 参考直接使用软启动输出，避免微小采样误差造成稳态慢漂。OLED 主页面的
`Vnow/Inow` 使用带符号瞬时采样值，并以 90 ms 周期刷新，避免与 50 Hz 波形整周期
同步而长期显示同一相位；校准页面仍使用 400 点总 RMS。
当前电压增益由偏置确认后的单点实测得到：
`109.781873 = 18.138269 * 19.368 / 3.2`。

### 闭环反馈极性诊断

首次闭环前先在低压、限流条件下使用 `OPEN LOOP` 和 5–10 V 参考。稳态后从
OLED 主页面按一次 RIGHT 或 DOWN 进入 `DQ Monitor` 页面，直接观察 Vd/Vq/Id/Iq：

- Vd 应约为 `+sqrt(2) * Vout_rms`，Vq 应接近 0。
- 若 Vd 幅值正确但为负，将 `APP_VOUT_CONTROL_POLARITY` 从 `+1.0f` 改为
  `-1.0f`；若 Vq 占主导，先检查实际控制频率和 100 点四分之一周期延时。
- 接阻性负载后，Id 应主要为正；否则修正
  `APP_IOUT_CONTROL_POLARITY`。

只确认 RMS 幅值不能确认反馈极性。完成以上检查后才能切入闭环。

## 安全行为

- 上电默认不使能；EPWM1/EPWM2 通过 Trip Zone 强制为低电平。
- HMI 的 RUN 命令通过 `control_if/` 释放 PWM，电压参考从 0 V 斜坡上升。
- ADC 非法或 ISR 超时会闭锁 PWM；先停止使能再重新启动
  才能清除闭锁。
- 电压参考范围为 0–25 V RMS；电流设定默认值为 3 A，可调范围为 0–4.5 A。
- RUN 后 PWM 会继续保持一个控制周期的安全关断；下一周期取得新的电流 ADC
  样本后才释放输出，避免使用启动首样本。

软件 ADC 过流保护当前通过 `APP_SOFTWARE_OCP_ENABLED=0` 临时关闭，电流设定值
不触发软件 OCP。功率级必须具备独立硬件过流保护；完成电流采样瞬态标定后将
该宏改为 `1`，即可恢复软件 OCP。

当前模型把直流母线 `Vin` 固定为 36 V，仅用于控制模型和开环调制度计算，
不是母线测量值。由于没有独立的母线 ADC 通道，软件不执行母线欠压/过压判断。
接功率级前必须按实际板卡校准 ADC 偏置/增益，并使用硬件过流、过压、欠压和
急停保护。

每次 STOP、故障后的控制停机以及重新 RUN 都会清除生成模型的全部控制状态，
包括四个积分器、dq 滤波、输入低通、主动阻尼滤波和 90° 延时缓冲区，避免把
上一段波形历史带入下一次启动。

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
  app/adc_calibration.c app/pwm_profile.c -lm `
  -o tests/run_fault_hmi_check.exe
tests/run_fault_hmi_check.exe

gcc -DUNIT_TEST -I app -I control_if -I drivers_user -I hmi `
  tests/run_hmi_display_check.c app/pwm_profile.c `
  hmi/hmi_display.c hmi/hmi_param.c -lm `
  -o tests/run_hmi_display_check.exe
tests/run_hmi_display_check.exe

```

## 目录

- `model/`：从 Simulink 生成的单相控制算法和 ADC/PWM 初始化代码。
- `app/`：目标调度、默认参数和后台任务。
- `control_if/`：HMI、保护逻辑与生成模型之间的边界。
- `hmi/`、`drivers_user/`：OLED、按键和板级驱动。
- `support/`、`device/`：MathWorks C2000 支持代码和 TI 器件文件。
- `tests/`：不依赖硬件的最小自检。
