#include "c2000BoardSupport.h"
#include "MW_f280015x_includes.h"
#include "rtwtypes.h"
#include "xtq2_dq_doubleloop_fullspec.h"
#include "xtq2_dq_doubleloop_fullspec_private.h"

void enableExtInterrupt (void);
void configureGPIOExtInterrupt(void);

#include <stdint.h>

void disableWatchdog(void)
{
  int *WatchdogWDCR = (int *)(uintptr_t)0x7029;
  asm(" EALLOW ");
  *WatchdogWDCR = 0x0068;
  asm(" EDIS ");
}
