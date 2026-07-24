# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

F2800157 单相全桥逆变器工程，可直接导入 Code Composer Studio 12.4。控制算法由
`xtq2_dq_doubleloop_fullspec.slx`（MATLAB/Simulink R2024a Update 8 + Embedded Coder
TI C2000 目标）生成，位于 `model/`；手写的调度、安全闭锁和 HMI 接口位于 `app/` 与
`control_if/`，**不要把手写代码混入 `model/` 下的生成文件**（下次重新生成模型会覆盖它们）。

硬件/控制关键参数（MCU 型号、控制周期、PWM 拓扑、ADC 标定系数、PI 增益等）见
`README.md`，那里已有权威记录，改动这些参数前先读一遍，避免与固件行为不一致。

## Build / Flash

- 在 CCS 中导入本目录（工程名 `single_phase_inverter`），选择 Debug/Release 并 Build Project。
- 用 `device/f2800157.ccxml` 连接 XDS110 下载 `single_phase_inverter.out`。
- 固件下载后 PWM 保持关断（Trip Zone 强制低电平），必须从 HMI 执行 RUN 才会释放输出。
- 没有命令行编译流程；工程只能在 CCS 中构建，这里的 gcc 命令仅用于运行下面的主机侧单元测试。

## Tests (host-side, no hardware)

`tests/` 下是不依赖硬件的最小自检，用 `-DUNIT_TEST` 在宿主 gcc 上编译运行：

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

每个 `run_*_check.c` 都是独立的自检程序（没有单独用例可选择运行）；新增自检时新建一个
`tests/run_xxx_check.c` 并只链接它实际用到的源文件。`UNIT_TEST` 宏用于在这些文件里 stub
掉真实寄存器/硬件访问，改动 `app/`、`control_if/`、`hmi/` 中被这些测试覆盖的逻辑后应重新跑一遍。

## Architecture

数据流是分层的，且都通过明确的适配层交换数据，不直接互相访问内部状态：

```
20 kHz ISR (Firmware_ADCC1_ISR -> rt_OneStep, app/firmware_main.c)
  ADC 原始值 -> Firmware_AdcToSignal (标定) -> xtq2_dq_doubleloop_fullspec_step()
                                                        |
                                                        v
                                          control_if/control_model_if.c
                                    (安全/RMS/PWM频率切换/开环占空比的标量适配)
                                                        |
                                                        v
                                     control_if/control_interface.c
                                (Setpoint/Feedback 双缓冲，HMI <-> 控制回路解耦)
                                                        |
                                                        v
                                          hmi/ (显示、菜单、参数、ADC 标定)
```

- **`app/firmware_main.c`** 是唯一的控制入口：`main()` 做一次性初始化（PWM 时序、模型参数、
  中断），`rt_OneStep()` 在 EPWM1 SOCA → ADCC EOC0 触发的 20 kHz ISR 里跑一次模型步进，
  处理 ISR 超时（`IsrOverrun`/`g_overrun_flag`）、控制使能状态迁移（含
  `Firmware_ResetControlState()` 清空生成模型的全部 DW 状态）、闭环/开环 PWM 输出路径的分支，
  以及每 N 个控制周期触发一次的 1ms 后台任务（`ControlModel_Task1ms`，周期由
  `PWM_Profile_t.task_1ms_divider` 决定，即 `app/pwm_profile.c`）。
- **`control_if/control_model_if.c`** 把生成模型的原始接口（`xtq2_dq_doubleloop_fullspec_*`）
  包装成固件可用的标量函数：使能判定、故障闭锁、RMS 计算、开环占空比计算、PWM 比较值限幅、
  PWM 频率切换等。Simulink 生成代码本身不应直接被固件其余部分调用。
- **`control_if/control_interface.c`** 是 HMI 与控制回路之间的唯一数据边界：
  `Control_Setpoint_t`（vref/iref/使能/模式/PWM 频率）从 HMI 流向控制回路，
  `Control_Feedback_t`（电压电流 RMS 与瞬时值、dq 分量、占空比、运行/故障状态）反向流动，
  用双缓冲实现无锁快照，避免 ISR 与后台任务之间的竞态。
- **`hmi/`**：`hmi_menu.c` 是页面状态机，`hmi_param.c` 管理运行参数与 ADC 标定值（仅存 RAM，
  掉电恢复编译期默认值），`hmi_display.c` 负责 OLED 渲染。ADC 标定（零点/增益自动或手动
  校准）只影响 `hmi_param` 里的 b/k 系数，经 `Firmware_ApplyAdcCalibration()` 写回生成模型
  的 Constant/Gain 参数，不改代码。
- **`app/adc_calibration.c`**：STOP 态零点自动校准（截尾均值）和 RUN 态增益自动校准的状态机。
- **`device/`、`support/`**：TI driverlib 与 MathWorks C2000 板级支持代码，通常不需要修改；
  仅在改变 MCU 外设配置（ADC 触发源、EPWM 同步、PIE 向量表等）时涉及。
- **`model/`**：Simulink/Embedded Coder 生成的控制算法与 ADC/PWM 初始化代码，只能通过重新
  生成模型来修改；固件通过 `control_if/` 里的适配函数与它交互。

## Safety-critical invariants

修改控制/安全相关代码时要保持以下约束（详见 `README.md`「安全行为」一节）：

- 上电、STOP、故障后必须保持/恢复 Trip Zone 强制关断，RUN 后仍先保持一个控制周期的安全
  关断，取得新的电流 ADC 样本后才释放输出。
- 每次 STOP、故障停机、RUN 都必须清空生成模型的全部 DW 状态（`Firmware_ResetControlState()`），
  避免历史波形带入下一次启动。
- `APP_SOFTWARE_OCP_ENABLED` 当前为 0（软件过流保护关闭，见 `app/app_config.h`）；改动电流
  采样/标定相关代码后应重新评估是否可以打开该宏，且功率级必须始终具备独立硬件保护。
- 电压/电流参考范围（`APP_VREF_MIN/MAX`、`APP_IREF_MIN/MAX`）与 PWM 占空比限幅
  （`APP_PWM_MAX_MODULATION`、`ControlModel_ClampPwmCompare`）是互相关联的推导值，改一个
  常量前检查 `app/app_config.h` 里的推导链和编译期 `#error` 检查。
