/*
 * Copyright 2019-2020 The MathWorks, Inc.
 */
#ifndef C2000BOARDSUPPORT_H
#define C2000BOARDSUPPORT_H

/* Undefine true and false as it will be defined in stdbool.h/rtwtypes.h file by CGT */
#if defined (__bool_true_false_are_defined) && defined (RTWTYPES_H)
    #define _donot_undef_true_false
#endif

#ifndef _donot_undef_true_false
    #ifdef false
        #undef false
    #endif

    #ifdef true
        #undef true
    #endif
    #include <stdbool.h>
    #include "rtwtypes.h"
#endif


extern void globalInterruptEnable(void);
extern void globalInterruptDisable(void);
extern void init_board(void);
extern void c2000_flash_init(void);
extern void initSetGPIOIPC(void);
extern void hardwareTimer1Init(void);
extern void enable_interrupts(void);
extern void bootloaderInit(void);
extern void update_MepScaleFactor(void);
extern void initWatchdog(void);
extern void c_fire_DAQs(uint8_T event_channel);

#endif /* C2000BOARDSUPPORT_H */
