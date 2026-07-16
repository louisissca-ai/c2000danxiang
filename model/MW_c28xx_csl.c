#include "c2000BoardSupport.h"
#include "MW_f280015x_includes.h"
#include "rtwtypes.h"
#include "chovy_tidy.h"
#include "chovy_tidy_private.h"

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
