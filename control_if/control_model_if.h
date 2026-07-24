#ifndef CONTROL_MODEL_IF_H
#define CONTROL_MODEL_IF_H

/*
 * File: control_model_if.h
 * Description: Scalar adapter functions used by Simulink generated code.
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void ControlModel_Init(void);
void ControlModel_Task1ms(void);
uint16_t ControlModel_UpdateSafety(float vbus, float iout);
uint16_t ControlModel_IsPwmAllowed(float vbus);
uint16_t ControlModel_IsControlEnabled(void);
uint16_t ControlModel_GetFaultCode(void);
uint16_t ControlModel_GetActiveMode(void);
void ControlModel_TripFault(uint16_t fault_code);
float ControlModel_GetVrefRamp(void);
void ControlModel_UpdateRunningRms(float vout_sample, float iout_sample);
float ControlModel_GetRunningVoutRms(void);
float ControlModel_GetRunningIoutRms(void);
void ControlModel_GetOpenLoopDuty(float vref_rms, float vbus,
    float *duty_a_percent, float *duty_b_percent);
uint16_t ControlModel_ClampPwmCompare(uint16_t period, uint16_t compare);
void ControlModel_UpdateStoppedFeedback(float vin, float vout_sample,
    float iout_sample);
void ControlModel_ResetStoppedFeedback(void);
void ControlModel_SetFeedback(float vin, float vout, float iout,
    float vout_inst, float iout_inst, float vd, float vq, float id, float iq,
    float duty);

#ifdef __cplusplus
}
#endif

#endif
