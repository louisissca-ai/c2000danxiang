#ifndef ADC_CALIBRATION_H
#define ADC_CALIBRATION_H

#include <stdint.h>

typedef enum
{
    ADC_CAL_CHANNEL_VOUT = 0,
    ADC_CAL_CHANNEL_IOUT
} ADC_Cal_Channel_t;

typedef enum
{
    ADC_CAL_STATE_IDLE = 0,
    ADC_CAL_STATE_OFFSET_SETTLING,
    ADC_CAL_STATE_OFFSET_SAMPLING,
    ADC_CAL_STATE_OFFSET_COMPUTING,
    ADC_CAL_STATE_WAIT_STOP,
    ADC_CAL_STATE_APPLIED,
    ADC_CAL_STATE_ERROR
} ADC_Cal_State_t;

typedef enum
{
    ADC_CAL_ERROR_NONE = 0,
    ADC_CAL_ERROR_STATE,
    ADC_CAL_ERROR_INPUT,
    ADC_CAL_ERROR_RANGE
} ADC_Cal_Error_t;

typedef struct
{
    ADC_Cal_State_t state;
    ADC_Cal_Channel_t channel;
    ADC_Cal_Error_t error;
    uint16_t progress_percent;
    float true_rms;
    float measured_rms;
} ADC_Cal_View_t;

void ADC_Cal_Init(void);
uint16_t ADC_Cal_StartOffset(void);
void ADC_Cal_PushStoppedRaw(uint16_t vout_raw, uint16_t iout_raw,
    float vout_adc_gain, float iout_adc_gain);
uint16_t ADC_Cal_ApplyGain(ADC_Cal_Channel_t channel, float true_rms,
    float measured_rms);
void ADC_Cal_Task1ms(void);
void ADC_Cal_Cancel(void);
uint16_t ADC_Cal_IsBusy(void);
void ADC_Cal_GetView(ADC_Cal_View_t *view);
void ADC_Cal_SetTrueRms(ADC_Cal_Channel_t channel, float value);
float ADC_Cal_GetTrueRms(ADC_Cal_Channel_t channel);

#endif
