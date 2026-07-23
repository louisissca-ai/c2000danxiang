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
void ControlModel_GetOpenLoopDuty(float vref_rms, float vbus,
    float *duty_a_percent, float *duty_b_percent);
void ControlModel_UpdateStoppedFeedback(float vin, float vout_sample,
    float iout_sample);
void ControlModel_SetFeedback(float vin, float vout, float iout, float duty);

#ifdef __cplusplus
}
#endif

#endif
