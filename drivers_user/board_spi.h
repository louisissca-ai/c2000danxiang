#ifndef BOARD_SPI_H
#define BOARD_SPI_H

/*
 * File: board_spi.h
 * Description: Board-level 4-wire SPI write abstraction for the OLED.
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BOARD_SPI_STATUS_OK      0u
#define BOARD_SPI_STATUS_TIMEOUT 1u

void BoardSPI_Init(void);
uint16_t BoardSPI_WriteCommand(const uint16_t *data, uint16_t len);
uint16_t BoardSPI_WriteData(const uint16_t *data, uint16_t len);
uint16_t BoardSPI_GetStatus(void);

#ifdef UNIT_TEST
void BoardSPI_MockResetTrace(void);
uint16_t BoardSPI_MockGetCommandCount(void);
uint16_t BoardSPI_MockGetDataCount(void);
uint16_t BoardSPI_MockGetCommand(uint16_t index);
#endif

#ifdef __cplusplus
}
#endif

#endif
