#include "c2000BoardSupport.h"
#include "MW_f280015x_includes.h"
#include "rtwtypes.h"
#include "xtq2_dq_doubleloop_fullspec.h"
#include "xtq2_dq_doubleloop_fullspec_private.h"
#include "MW_c28xAnalogSubsystem.h"

void config_ADCA_SOC0(void)
{
  EALLOW;
  AdcaRegs.ADCSOC0CTL.bit.CHSEL = 4U;  /* Set SOC0 channel select to ADCIN4*/
  AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 5U;
  AdcaRegs.ADCSOC0CTL.bit.ACQPS = (uint16_T)7.0;
                                /* Set SOC0 S/H Window to 8.0 ADC Clock Cycles*/
  AdcaRegs.ADCINTSOCSEL1.bit.SOC0 = 0U;
                                   /* SOCx No ADCINT Interrupt Trigger Select.*/
  AdcaRegs.ADCOFFTRIM.bit.OFFTRIM = (uint16_T)AdcaRegs.ADCOFFTRIM.bit.OFFTRIM;/* Set Offset Error Correctino Value*/
  AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1U;
                                /* Late interrupt pulse trips AdcResults latch*/
  AdcaRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0U;/* All in round robin mode SOC Priority*/
  EDIS;
}

void config_ADCC_SOC0(void)
{
  EALLOW;
  AdccRegs.ADCSOC0CTL.bit.CHSEL = 4U;  /* Set SOC0 channel select to ADCIN4*/
  AdccRegs.ADCSOC0CTL.bit.TRIGSEL = 5U;
  AdccRegs.ADCSOC0CTL.bit.ACQPS = (uint16_T)7.0;
                                /* Set SOC0 S/H Window to 8.0 ADC Clock Cycles*/
  AdccRegs.ADCINTSOCSEL1.bit.SOC0 = 0U;
                                   /* SOCx No ADCINT Interrupt Trigger Select.*/
  AdccRegs.ADCOFFTRIM.bit.OFFTRIM = (uint16_T)AdccRegs.ADCOFFTRIM.bit.OFFTRIM;/* Set Offset Error Correctino Value*/
  AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1U;
                                /* Late interrupt pulse trips AdcResults latch*/
  AdccRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0U;/* All in round robin mode SOC Priority*/
  EDIS;
}

void InitAdcA(void)
{
  EALLOW;
  CpuSysRegs.PCLKCR13.bit.ADC_A = 1U;
  DELAY_US(1);
    // Need this to solve register setting issues for some version of the boards
  AdcaRegs.ADCCTL2.bit.PRESCALE = 2U;
  MW_SetVREF();
  EALLOW;

  //power up the ADC
  AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1U;

  //delay for 1ms to allow ADC time to power up
  DELAY_US(1000);
  EDIS;
}

void InitAdcC(void)
{
  EALLOW;
  CpuSysRegs.PCLKCR13.bit.ADC_C = 1U;
  DELAY_US(1);
    // Need this to solve register setting issues for some version of the boards
  AdccRegs.ADCCTL2.bit.PRESCALE = 2U;
  MW_SetVREF();
  EALLOW;

  //power up the ADC
  AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1U;

  //delay for 1ms to allow ADC time to power up
  DELAY_US(1000);
  EDIS;
}
