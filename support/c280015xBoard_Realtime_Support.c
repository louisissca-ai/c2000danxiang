/* Copyright 2023 The MathWorks, Inc. */
#include "MW_target_hardware_resources.h"
#include <string.h>

#if MW_RUNTIME_FLASHLOAD
extern Uint16 MW_RamfuncsRunStart;
extern Uint16 MW_RamfuncsLoadSize;
extern Uint16 MW_RamfuncsLoadStart;
#endif

void c2000_flash_init(void)
{
  #if MW_RUNTIME_FLASHLOAD
  /* Copy InitFlash function code and Flash setup code to RAM */
  memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
  memcpy(&MW_RamfuncsRunStart, &MW_RamfuncsLoadStart, (Uint32)&MW_RamfuncsLoadSize);
  /* Call Flash Initialization to setup flash waitstates
   This function must reside in RAM */
  InitFlash();
  #endif
}
