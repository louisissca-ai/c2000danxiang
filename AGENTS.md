# Repository Guidelines

This is a standalone TI Code Composer Studio project for an F2800157
single-phase full-bridge inverter. Generated Simulink code is isolated in
`model/`; do not put application or HMI logic there. The hand-written target
scheduler is `app/firmware_main.c`, safety and HMI adaptation are in
`control_if/`, and board/OLED/key drivers are in `drivers_user/`.

Import this directory in CCS 12.4 and build with C2000 compiler 22.6.0.LTS.
The expected output is `single_phase_inverter.out`. Use
`device/f2800157.ccxml` for the XDS110 target connection.

Match the existing four-space C style. Public APIs use module prefixes such as
`APP_`, `ControlIF_`, `ControlModel_`, `HMI_`, and `Board`. Keep hardware-free,
assert-based checks under `tests/` and run the smallest relevant check after
changing non-trivial hand-written logic.

Trace the 20 kHz path from `rt_OneStep()` through the generated model and back
through `ControlModel_SetFeedback()` before changing interfaces. Regenerate
`model/` from the source SLX instead of manually editing generated algorithm
files. Treat ADC calibration, PWM dead time, Trip Zone behavior, and the
fixed 36 V model bus value as hardware-safety boundaries.
