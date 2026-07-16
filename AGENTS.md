# Repository Guidelines

## Project Structure & Module Organization

This repository is a standalone TI Code Composer Studio project for a C2000 three-phase inverter target. The generated Simulink control code lives in `model/`; keep regenerated model files isolated there and review merges before overwriting local CCS edits. Application scheduling and defaults are in `app/`, HMI state and display logic in `hmi/`, board/OLED/key drivers in `drivers_user/`, and the HMI-to-control boundary in `control_if/`. TI device, driverlib, linker, and board support files are under `device/` and `support/`. CCS metadata is stored in `.project`, `.cproject`, `.ccsproject`, `.settings/`, and launch/config files such as `NewTargetConfiguration.ccxml`.

## Build, Test, and Development Commands

Import the project in CCS with `File > Import > Code Composer Studio > CCS Projects`, select this `ccs` directory, and build the `Debug` or `Release` configuration. The expected target is TMS320F2800157 using CCS 12.4-compatible tooling and the C2000 compiler 22.6.0.LTS.

For host-side checks, compile small C programs directly with GCC/MinGW when possible. Example:

```sh
gcc -DUNIT_TEST -I drivers_user tests/example_check.c drivers_user/oled.c -o tests/example_check.exe
```

Run target firmware from CCS using the project target configuration, typically `device/f2800157.ccxml` or `NewTargetConfiguration.ccxml`.

## Coding Style & Naming Conventions

Use C with 4-space indentation and brace style matching nearby files. Public APIs use module prefixes such as `APP_`, `HMI_`, `OLED_`, `Board`, and `ControlIF_`. Type names end in `_t`; constants and macros use uppercase names like `APP_VREF_DEFAULT`. Prefer fixed-width integer types from `<stdint.h>`. Header files should have include guards and `extern "C"` wrappers when exposing C APIs.

## Testing Guidelines

There is no top-level test runner. Add focused, standalone `assert`-style checks for non-trivial logic, preferably under `tests/` if that directory exists or beside the module being checked if it does not. Name checks after the protected behavior, for example `controlif_nonblocking_check.c`. Keep tests hardware-free by using `UNIT_TEST` hooks or existing mocks.

## Commit & Pull Request Guidelines

No project-specific Git convention is visible here, so use short imperative commit titles such as `Fix HMI parameter clamp` or `Update OLED pin map`. Pull requests should describe firmware behavior changes, list CCS or host checks run, and call out any changes to generated `model/` files, linker files, pin maps, or hardware calibration assumptions.

## Agent-Specific Instructions

Trace the real call path before editing. Reuse existing module boundaries and helpers before adding new code. Do not place hand-written application logic inside generated Simulink files unless the change is intentionally model-local and documented.
