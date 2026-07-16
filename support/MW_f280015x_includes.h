/*####################################################
 * FILE: MW_f280015x_includes.h
 * TITLE: All the file inclusions for F280015x target board
 * Copyright 2023 The MathWorks, Inc.
 * ####################################################*/
 
#ifndef _MW_C280015X_INCLUDES_H
#define _MW_C280015X_INCLUDES_H

/* ***************************************************************************/
/* the includes */
#include "f280015x_device.h"
#ifdef __TMS320C28XX__
#include "f280015x_cputimer.h"
#include "f280015x_cputimervars.h"
#include "f280015x_gpio_defines.h"
#include "f280015x_pie_defines.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_can.h"
#include "can.h"
#endif
#include "MW_target_hardware_resources.h"
/* ***************************************************************************/
/* the defines */

//
// The following are values that can be passed to the
// IntOsc2Sel() , XtalOscSel() to select system PLL (or) AUX PLL
//
#define   SYSTEM_PLL    (Uint16) 0

//
// The following are values that can be passed to the
// InitSysPll() , InitAuxPll() & IsPLLValid() to select clock source
//
#define   INT_OSC2     0
#define   XTAL_OSC     1
#define   INT_OSC1     2
#define   XTAL_OSC_SE    5  // External oscillator, single-ended mode
#define   INT_PLL_SYSPLL    0

//
// The following are values that can be passed to the
// IsPLLValid() to select DCC for PLL validation
//
#define   SYSCTL_DCC_BASE0              0

//
// CLKIN is a 10 MHz crystal or internal 10 MHz oscillator
//
#define CPU_RATE ((long double)10*100/MW_CLOCKING_CPUCLOCKRATEMHZ)
#define DELAY_US(A)  F28x_usDelay(((((long double) A * 1000.0L) / (long double)CPU_RATE) - 9.0L) / 5.0L)
/* ***************************************************************************/
/* the globals */

extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsLoadSize;
extern Uint16 RamfuncsRunStart;
extern Uint16 RamfuncsRunEnd;
extern Uint16 RamfuncsRunSize;

/* ***************************************************************************/
/* the functions */

extern void F28x_usDelay(long LoopCount);
extern void GPIO_SetupPinMux(Uint16 gpioNumber, Uint16 cpu, Uint16 muxPosition);
extern void GPIO_SetupPinOptions(Uint16 gpioNumber, Uint16 output, Uint16 flags);
extern void GPIO_EnableUnbondedIOPullups(void);
//extern void AdcSetMode(Uint16 adc, Uint16 resolution, Uint16 signalmode);
extern void DisableDog(void);
extern void EnableInterrupts(void);
extern void InitPieCtrl(void);
extern void InitPieVectTable(void);
extern void InitSysCtrl(void);
extern void InitSysPll(Uint16 clock_source, Uint16 imult, Uint32 refdiv, Uint32 odiv,
                       Uint16 divsel, Uint32 dccbase);
extern bool IsPLLValid(Uint32 base, Uint16 oscSource, Uint16 pllclk, Uint16 imult,
                       Uint16 odiv, Uint16 refdiv);
extern void SysIntOsc1Sel (void);
extern void SysIntOsc2Sel (void);
extern void SysXtalOscSel (void);
extern void SysXtalOscSESel (void);
extern void InitGpio();
extern Uint16 GPIO_ReadPin(Uint16 gpioNumber);
extern void GPIO_WritePin(Uint16 gpioNumber, Uint16 outVal);
extern void InitFlash(void);
#define KickDog ServiceDog     // For compatibility with previous versions
extern void ServiceDog(void);
extern void DisableDog(void);

#endif // end of _MW_C280015X_INCLUDES_H definition
