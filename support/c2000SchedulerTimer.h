/*
 * Copyright 2013-2020 The MathWorks, Inc.
 */

#ifndef __C2000SCHEDULERTIMER_H__
#define __C2000SCHEDULERTIMER_H__
#include "c2000BoardSupport.h"

interrupt void TINT0_isr(void);
void configureTimer0(float baseRate, float systemClock);
void stopTimer0(void);
void enableTimer0Interrupt(void);
bool disableTimer0Interrupt(void);
void restoreTimer0Interrupt(bool);

#endif /* C2000SCHEDULERTIMER_H */
