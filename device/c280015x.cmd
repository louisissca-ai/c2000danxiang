MEMORY
{
PAGE 0 :

   #if BOOT_FROM_FLASH
      /* BEGIN is used for the "boot to Flash" bootloader mode   */
      BEGIN           : origin = 0x080000,    length = 0x000002
      RAMLS_PROG      : origin = 0x00A000,    length = 0x002000
   #else
      /* BEGIN is used for the "boot to SARAM" bootloader mode   */
      BEGIN           : origin = 0x000000,    length = 0x000002
      RAMLS_PROG      : origin = 0x00A000,    length = 0x002000
   #endif // BOOT_FROM_FLASH   
   RESET              : origin = 0x3FFFC0,    length = 0x000002

   /* Flash sectors */
   /* BANK 0 */
      FLASH_BANK0_SEC_0_31    : origin = 0x080002, length = 0x7FFD  /* on-chip Flash */

   #if defined(F2800154) || defined(F2800155) || defined(F2800156) || defined(F2800157)
      FLASH_BANK0_SEC_32_63   : origin = 0x088000, length = 0x8000  /* on-chip Flash */
   #endif 
   
   #if defined(F2800157) || defined(F2800156)
      FLASH_BANK0_SEC_64_127  : origin = 0x090000, length = 0xFFF1  /* on-chip Flash */
   #endif 
   
   BOOTROM          : origin = 0x003F0000, length = 0x00008000
   BOOTROM_EXT      : origin = 0x003F8000, length = 0x00007FC0

PAGE 1 :

   BOOT_RSVD          : origin = 0x00000002,  length = 0x00000126     /* Part of M0, BOOT rom will use this for stack */
   RAMM0M1            : origin = 0x00000128,  length = 0x000006D0
   #if BOOT_FROM_FLASH
      /* BEGIN is used for the "boot to Flash" bootloader mode   */
      BEGIN           : origin = 0x080000,    length = 0x000002
   #else
      /* BEGIN is used for the "boot to SARAM" bootloader mode   */
      BEGIN           : origin = 0x000000,    length = 0x000002
   #endif // BOOT_FROM_FLASH 
   RAMLS_DATA      : origin = 0x008000,    length = 0x002000
}

SECTIONS
{
   #if BOOT_FROM_FLASH
      codestart        : > BEGIN,                PAGE = 0, ALIGN(4)
       #if defined(F2800157) || defined(F2800156)
          .text        : >> FLASH_BANK0_SEC_0_31 | FLASH_BANK0_SEC_32_63 | FLASH_BANK0_SEC_64_127,     PAGE = 0, ALIGN(4)
       #elif defined(F2800155) || defined(F2800154)
          .text        : >> FLASH_BANK0_SEC_0_31 | FLASH_BANK0_SEC_32_63,     PAGE = 0, ALIGN(4)
       #else
          .text        : > FLASH_BANK0_SEC_0_31,     PAGE = 0, ALIGN(4)
       #endif
      .cinit           : > FLASH_BANK0_SEC_0_31,     PAGE = 0, ALIGN(4)
      .switch          : > FLASH_BANK0_SEC_0_31,     PAGE = 0, ALIGN(4)
      #if defined(__TI_EABI__)
        .bss           : > RAMLS_DATA,           PAGE = 1
        .bss:output    : > RAMLS_DATA,           PAGE = 1
        .bss:cio       : > RAMLS_DATA,           PAGE = 1
        .init_array    : > FLASH_BANK0_SEC_0_31,     PAGE = 0, ALIGN(8)
        .const         : > FLASH_BANK0_SEC_0_31,     PAGE = 0, ALIGN(8)
        .data          : > RAMLS_DATA,           PAGE = 1
        .sysmem        : > RAMLS_DATA,           PAGE = 1  
        .TI.ramfunc    : LOAD = FLASH_BANK0_SEC_0_31,
                         RUN = RAMLS_PROG,
                         LOAD_START(RamfuncsLoadStart),
                         LOAD_SIZE(RamfuncsLoadSize),
                         LOAD_END(RamfuncsLoadEnd),
                         RUN_START(RamfuncsRunStart),
                         RUN_SIZE(RamfuncsRunSize),
                         RUN_END(RamfuncsRunEnd),
                         PAGE = 0, ALIGN(8)
      
        ramfuncs       : LOAD = FLASH_BANK0_SEC_0_31,
                         RUN = RAMLS_PROG,
                         LOAD_START(MW_RamfuncsLoadStart),
                         LOAD_END(MW_RamfuncsLoadEnd),
                         LOAD_SIZE(MW_RamfuncsLoadSize),
                         RUN_START(MW_RamfuncsRunStart),
                         PAGE = 0, ALIGN(8)
      #else
        .pinit         : > FLASH_BANK0_SEC_0_31,     PAGE = 0, ALIGN(4)
        .econst        : > FLASH_BANK0_SEC_0_31,     PAGE = 0, ALIGN(4)
        .cio           : > RAMLS_DATA,           PAGE = 1
        .ebss          : > RAMLS_DATA,           PAGE = 1
        .esysmem       : > RAMLS_DATA,           PAGE = 1   
        .TI.ramfunc    : LOAD = FLASH_BANK0_SEC_0_31,
                         RUN = RAMLS_PROG,
                         LOAD_START(_RamfuncsLoadStart),
                         LOAD_SIZE(_RamfuncsLoadSize),
                         LOAD_END(_RamfuncsLoadEnd),
                         RUN_START(_RamfuncsRunStart),
                         RUN_SIZE(_RamfuncsRunSize),
                         RUN_END(_RamfuncsRunEnd),
                         PAGE = 0, ALIGN(4)
      
        ramfuncs       : LOAD = FLASH_BANK0_SEC_0_31,
                         RUN = RAMLS_PROG,
                         LOAD_START(_MW_RamfuncsLoadStart),
                         LOAD_END(MW_RamfuncsLoadEnd),
                         LOAD_SIZE(_MW_RamfuncsLoadSize),
                         RUN_START(_MW_RamfuncsRunStart),
                         PAGE = 0, ALIGN(4)
      #endif //defined(__TI_EABI__)
      .reset           : > RESET,                PAGE = 0, TYPE = DSECT /* not used, */
      .stack           : > RAMM0M1,              PAGE = 1


    /* Allocate IQ math areas: */
      IQmath           : > FLASH_BANK0_SEC_0_31,     PAGE = 0, ALIGN(4)            /* Math Code */
      IQmathTables     : > FLASH_BANK0_SEC_0_31,     PAGE = 0, ALIGN(4)
      
   #else
      codestart        : > BEGIN,                PAGE = 0
      .TI.ramfunc      : > RAMLS_PROG,           PAGE = 0
      ramfuncs         : > RAMLS_PROG,           PAGE = 0
      .text            : > RAMLS_PROG,           PAGE = 0
      .cinit           : > RAMLS_DATA,           PAGE = 1
      .switch          : > RAMLS_DATA,           PAGE = 1
      #if defined(__TI_EABI__)
        .bss           : > RAMLS_DATA,           PAGE = 1
        .bss:output    : > RAMLS_DATA,           PAGE = 1
        .bss:cio       : > RAMLS_DATA,           PAGE = 1
        .init_array    : > RAMLS_DATA,           PAGE = 1
        .const         : > RAMLS_DATA,           PAGE = 1
        .data          : > RAMLS_DATA,           PAGE = 1
        .sysmem        : > RAMLS_PROG,           PAGE = 0 
      #else                                      
        .pinit         : > RAMLS_DATA,           PAGE = 1
        .cio           : > RAMLS_DATA,           PAGE = 1
        .ebss          : > RAMLS_DATA,           PAGE = 1
        .econst        : > RAMLS_PROG,           PAGE = 0
        .esysmem       : > RAMLS_PROG,           PAGE = 0
      #endif //defined(__TI_EABI__)              
      .reset           : > RESET,                PAGE = 0, TYPE = DSECT /* not used, */
      .stack           : > RAMM0M1,              PAGE = 1

      
      /* Allocate IQ math areas: */
      IQmath           : > RAMLS_PROG,           PAGE = 0            /* Math Code */
      IQmathTables     : > RAMLS_PROG,           PAGE = 0
      
   #endif //BOOT_FROM_FLASH
   
}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
