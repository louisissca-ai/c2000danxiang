# ccs 纯 C 工程

这是 `chovy_tidy.slx` 生成的控制代码与 `reusable_hmi_platform` 合并后的独立 CCS 工程。工程编译不需要 MATLAB 或 Simulink；模型算法已经固化在 `model/chovy_tidy.c` 和 `model/chovy_tidy_data.c` 中，可直接在 CCS 里修改。

## 环境

- 目标芯片：TMS320F2800157
- CCS：建议使用 CCS 12.4 或兼容版本
- C2000 编译器：22.6.0.LTS（工程原始生成版本）
- 芯片支持源码/头文件来自 C2000Ware 5.00.00.00，已放入 `device/`，编译时无需另行配置 C2000Ware 路径

## 导入和编译

1. CCS 选择 `File > Import > Code Composer Studio > CCS Projects`。
2. 搜索目录选择本 `ccs` 文件夹，勾选项目 `ccs`，不要勾选复制到 workspace。
3. 选择 `Debug` 或 `Release`，执行 `Build Project`。
4. 使用 `device/f2800157.ccxml` 建立目标连接并下载生成的 `ccs.out`。

已用 CCS 12.4 + C2000 编译器 22.6.0.LTS 完成 Debug 全量编译；产物位于 `Debug/ccs.out`。

## 代码入口

- `model/ert_main.c`：主函数、50 us 调度入口和后台 HMI 循环。
- `model/chovy_tidy.c`：Simulink 生成的控制算法；`chovy_tidy_step0()` 为 50 us 基准任务，`chovy_tidy_step2()` 为 1 ms 子任务。
- `control_if/control_model_if.c`：HMI 参数、安全使能与模型之间的边界。
- `app/app_main.c`：1 ms HMI 调度和后台刷新。
- `drivers_user/board_pinmap.h`：OLED、按键等板级引脚定义。

重新从 `.slx` 生成代码不会自动同步本目录；如重新生成，需要重新审查并合并 `model/`，避免覆盖在 CCS 中做的手工修改。
