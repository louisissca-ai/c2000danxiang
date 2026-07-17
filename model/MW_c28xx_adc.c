#include "c2000BoardSupport.h"
#include "MW_f280015x_includes.h"
#include "rtwtypes.h"
#include "chovy_tidy.h"
#include "chovy_tidy_private.h"
#include "MW_c28xAnalogSubsystem.h"

void config_ADCC_SOC1(void)
{
  EALLOW;
  AdccRegs.ADCSOC1CTL.bit.CHSEL = 1U;  /* Set SOC1 channel select to ADCIN1*/
  AdccRegs.ADCSOC1CTL.bit.TRIGSEL = 5U;
  AdccRegs.ADCSOC1CTL.bit.ACQPS = (uint16_T)7.0;
                                /* Set SOC1 S/H Window to 8.0 ADC Clock Cycles*/
  AdccRegs.ADCINTSOCSEL1.bit.SOC1 = 0U;
                                   /* SOCx No ADCINT Interrupt Trigger Select.*/
  AdccRegs.ADCOFFTRIM.bit.OFFTRIM = (uint16_T)AdccRegs.ADCOFFTRIM.bit.OFFTRIM;/* Set Offset Error Correctino Value*/
  AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1U;
                                /* Late interrupt pulse trips AdcResults latch*/
  AdccRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0U;/* All in round robin mode SOC Priority*/
  EDIS;
}

void config_ADCA_SOC0(void)
{
  EALLOW;
  AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0U;  /* Set SOC0 channel select to ADCIN0*/
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

void config_ADCC_SOC2(void)
{
  EALLOW;
  AdccRegs.ADCSOC2CTL.bit.CHSEL = 2U;  /* Set SOC2 channel select to ADCIN2*/
  AdccRegs.ADCSOC2CTL.bit.TRIGSEL = 5U;
  AdccRegs.ADCSOC2CTL.bit.ACQPS = (uint16_T)7.0;
                                /* Set SOC2 S/H Window to 8.0 ADC Clock Cycles*/
  AdccRegs.ADCINTSOCSEL1.bit.SOC2 = 0U;
                                   /* SOCx No ADCINT Interrupt Trigger Select.*/
  AdccRegs.ADCOFFTRIM.bit.OFFTRIM = (uint16_T)AdccRegs.ADCOFFTRIM.bit.OFFTRIM;/* Set Offset Error Correctino Value*/
  AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1U;
                                /* Late interrupt pulse trips AdcResults latch*/
  AdccRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0U;/* All in round robin mode SOC Priority*/
  EDIS;
}

void config_ADCA_SOC2(void)
{
  EALLOW;
  AdcaRegs.ADCSOC2CTL.bit.CHSEL = 2U;  /* Set SOC2 channel select to ADCIN2*/
  AdcaRegs.ADCSOC2CTL.bit.TRIGSEL = 5U;
  AdcaRegs.ADCSOC2CTL.bit.ACQPS = (uint16_T)7.0;
                                /* Set SOC2 S/H Window to 8.0 ADC Clock Cycles*/
  AdcaRegs.ADCINTSOCSEL1.bit.SOC2 = 0U;
                                   /* SOCx No ADCINT Interrupt Trigger Select.*/
  AdcaRegs.ADCOFFTRIM.bit.OFFTRIM = (uint16_T)AdcaRegs.ADCOFFTRIM.bit.OFFTRIM;/* Set Offset Error Correctino Value*/
  AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1U;
                                /* Late interrupt pulse trips AdcResults latch*/
  AdcaRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0U;/* All in round robin mode SOC Priority*/
  GpioCtrlRegs.GPHAMSEL.bit.GPIO224 = 1U;
  AnalogSubsysRegs.AGPIOCTRLH.bit.GPIO224 = 1U;
  EDIS;
}

void config_ADCC_SOC0(void)
{
  EALLOW;
  AdccRegs.ADCSOC0CTL.bit.CHSEL = 0U;  /* Set SOC0 channel select to ADCIN0*/
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

void config_ADCA_SOC1(void)
{
  EALLOW;
  AdcaRegs.ADCSOC1CTL.bit.CHSEL = 1U;  /* Set SOC1 channel select to ADCIN1*/
  AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 5U;
  AdcaRegs.ADCSOC1CTL.bit.ACQPS = (uint16_T)7.0;
                                /* Set SOC1 S/H Window to 8.0 ADC Clock Cycles*/
  AdcaRegs.ADCINTSOCSEL1.bit.SOC1 = 0U;
                                   /* SOCx No ADCINT Interrupt Trigger Select.*/
  AdcaRegs.ADCOFFTRIM.bit.OFFTRIM = (uint16_T)AdcaRegs.ADCOFFTRIM.bit.OFFTRIM;/* Set Offset Error Correctino Value*/
  AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1U;
                                /* Late interrupt pulse trips AdcResults latch*/
  AdcaRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0U;/* All in round robin mode SOC Priority*/
  EDIS;
}

void config_ADCA_SOC3(void)
{
  EALLOW;
  AdcaRegs.ADCSOC3CTL.bit.CHSEL = 3U;  /* Set SOC3 channel select to ADCIN3*/
  AdcaRegs.ADCSOC3CTL.bit.TRIGSEL = 5U;
  AdcaRegs.ADCSOC3CTL.bit.ACQPS = (uint16_T)7.0;
                                /* Set SOC3 S/H Window to 8.0 ADC Clock Cycles*/
  AdcaRegs.ADCINTSOCSEL1.bit.SOC3 = 0U;
                                   /* SOCx No ADCINT Interrupt Trigger Select.*/
  AdcaRegs.ADCOFFTRIM.bit.OFFTRIM = (uint16_T)AdcaRegs.ADCOFFTRIM.bit.OFFTRIM;/* Set Offset Error Correctino Value*/
  AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1U;
                                /* Late interrupt pulse trips AdcResults latch*/
  AdcaRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0U;/* All in round robin mode SOC Priority*/
  GpioCtrlRegs.GPHAMSEL.bit.GPIO242 = 1U;
  AnalogSubsysRegs.AGPIOCTRLH.bit.GPIO242 = 1U;
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
