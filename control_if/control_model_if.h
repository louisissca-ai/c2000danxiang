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
void ControlModel_TripFault(uint16_t fault_code);
float ControlModel_GetVrefRamp(void);
void ControlModel_SetFeedback(float vin, float vout, float iout, float duty);

#ifdef __cplusplus
}
#endif

#endif
