#include "c2000BoardSupport.h"
#include "MW_f280015x_includes.h"
#include "rtwtypes.h"
#include "chovy_tidy.h"
#include "chovy_tidy_private.h"

void init_board (void)
{
  DisableDog();
  EALLOW;
  EDIS;

#ifdef CPU1

  InitSysPll(INT_OSC2,48,0,1,1,SYSCTL_DCC_BASE0);

  //Turn on all peripherals
  //InitPeripheralClocks();
  EALLOW;
  CpuSysRegs.PCLKCR0.bit.CPUTIMER0 = 1U;
  CpuSysRegs.PCLKCR0.bit.CPUTIMER1 = 1U;
  CpuSysRegs.PCLKCR0.bit.CPUTIMER2 = 1U;

  //All the new processors starting from F28001x
  CpuSysRegs.PCLKCR0.bit.HRCAL = 1U;
  EDIS;

#endif                                 // #ifdef CPU1

  EALLOW;

  /* Configure low speed peripheral clocks */
  ClkCfgRegs.LOSPCP.bit.LSPCLKDIV = 0U;
  EDIS;

  /* Disable and clear all CPU interrupts */
  DINT;
  IER = 0x0000U;
  IFR = 0x0000U;
  InitPieCtrl();
  InitPieVectTable();
  InitCpuTimers();

#ifdef CPU1
#endif                                 // #ifdef CPU1

  /* initial ePWM GPIO assignment... */
  config_ePWM_GPIO();
  EALLOW;

  /* Enable clock to ePWM */
  CpuSysRegs.PCLKCR2.bit.EPWM1 = 1U;
  CpuSysRegs.PCLKCR2.bit.EPWM2 = 1U;
  CpuSysRegs.PCLKCR2.bit.EPWM3 = 1U;

  /* Disable TBCLK within ePWM before module configuration */
  CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0U;
  EDIS;
  config_ePWMSyncSource();

  /* initial GPIO qualification settings.... */
  EALLOW;
  GpioCtrlRegs.GPAQSEL1.all = 0x0U;
  GpioCtrlRegs.GPAQSEL2.all = 0x0U;
  GpioCtrlRegs.GPBQSEL1.all = 0x0U;
  GpioCtrlRegs.GPBQSEL2.all = 0x0U;
  GpioCtrlRegs.GPHQSEL1.all = 0x0U;
  GpioCtrlRegs.GPHQSEL2.all = 0x0U;

  // Set the pin to digital mode by default
  GpioCtrlRegs.GPAAMSEL.all = 0U;
  GpioCtrlRegs.GPHAMSEL.all = 0U;
  EDIS;
}
