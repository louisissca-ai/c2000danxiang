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
| 拓扑 | 单相全桥，双闭环 dq / 开环 SPWM 可切换 |
| A 桥臂 | EPWM1A/1B，GPIO0/GPIO1 |
| B 桥臂 | EPWM2A/2B，GPIO2/GPIO3 |
| PWM | 中心对齐，TBPRD=3000，死区 24 TBCLK（约 200 ns） |
| 输出电压采样 | ADCA SOC0 / ADCIN4，EPWM1 SOCA 触发 |
| 输出电流采样 | ADCC SOC0 / ADCIN4，EPWM1 SOCA 触发 |
| 采样滤波 | Vout：20 kHz 采样的一阶 2 kHz 低通；Iout：不新增输入滤波 |
| Vout ADC 标定 | `(raw * 3.3 / 4095 - 1.608) * 51.37954` |
| Iout ADC 标定 | `(raw * 3.3 / 4095 - 1.606) * 3.33333` |

生成的算法位于 `model/xtq2_dq_doubleloop_fullspec.*`。`app/firmware_main.c`
在 20 kHz ISR 中执行模型、将 HMI 的电压设定送入模型参数，并把电压/电流
RMS、占空比和故障状态回传给 HMI。

`ADC Cal` 页可在线调整 `V b`、`V k`、`I b`、`I k`，对应
`Vout/Iout = (raw * 3.3 / 4095 - b) * k`。UP/DOWN 每次调整 0.001，
长按 UP/DOWN 每次调整 0.1，OK 切换字段；修改标定会停止输出请求，需重新按 RUN。

## 控制模式

- `CLOSED LOOP`：使用现有双闭环 dq 控制，`Vref` 是输出电压 RMS 设定值。
- `OPEN LOOP`：使用 50 Hz 单极性倍频 SPWM，并沿用 `Vref`。36 V 母线下调制度
  为 `m = clamp(sqrt(2) * Vref / 36, 0, 1)`，两桥臂占空比分别为
  `50% * (1 + m*sin(theta))` 和 `50% * (1 - m*sin(theta))`。
- 在 HMI 的 `Set Mode` 页面改变模式会自动停止 PWM 并清除控制状态；必须再次
  按 RUN 才会以新模式启动。默认模式为 `CLOSED LOOP`，默认 `Vref` 为 20 V RMS。

开环计算使用固定的 36 V 母线参数，因此实际输出 RMS 会随真实母线电压、负载、
滤波器和死区变化；OLED 显示的采样 RMS 用于观察，不参与开环幅值调节。

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
