#ifndef BOARD_H
#define BOARD_H

/*
 * File: board.h
 * Description: Explicit board initialization and PWM safety API.
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void Board_Init(void);
void BoardPWM_ForceSafe(void);
void BoardPWM_Release(void);
uint16_t BoardPWM_IsReleased(void);

#ifdef __cplusplus
}
#endif

#endif
