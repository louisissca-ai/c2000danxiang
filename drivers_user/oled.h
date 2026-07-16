#ifndef OLED_H
#define OLED_H

/*
 * File: oled.h
 * Description: SSD1306-style OLED drawing API used by the HMI renderer.
 * Notes: The default target is a 128x64 OLED connected through 4-wire SPI.
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OLED_STATUS_OK    0u
#define OLED_STATUS_ERROR 1u

uint16_t OLED_Init(void);
void OLED_Clear(void);
uint16_t OLED_Update(void);

void OLED_ShowString(uint16_t x, uint16_t y, const char *str);
void OLED_ShowInt(uint16_t x, uint16_t y, int32_t value);
void OLED_ShowFloat(uint16_t x, uint16_t y, float value, uint16_t decimals);

#ifdef UNIT_TEST
const char *OLED_MockGetLine(uint16_t y);
uint16_t OLED_MockGetPageByte(uint16_t page, uint16_t x);
#endif

#ifdef __cplusplus
}
#endif

#endif
