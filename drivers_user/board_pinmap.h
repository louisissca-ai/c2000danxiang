#ifndef BOARD_PINMAP_H
#define BOARD_PINMAP_H

/*
 * File: board_pinmap.h
 * Description: LAUNCHXL-F2800157 HMI pin and peripheral configuration.
 */

#include <stdint.h>

#ifndef UNIT_TEST
#include "driverlib.h"
#include "pin_map.h"
#endif

#define BOARD_MCU_NAME                 "TMS320F2800157"
#define BOARD_NAME                     "LAUNCHXL-F2800157"
#define BOARD_SYSCLK_FREQ_HZ           120000000UL
#define BOARD_LSPCLK_FREQ_HZ           BOARD_SYSCLK_FREQ_HZ

#define BOARD_OLED_SPI_BASE            SPIA_BASE
#define BOARD_OLED_SPI_MOSI_GPIO       8U
#define BOARD_OLED_SPI_CLK_GPIO        9U
#define BOARD_OLED_SPI_CS_GPIO         11U
#define BOARD_OLED_DC_GPIO             42U
#define BOARD_OLED_RES_GPIO            43U
#define BOARD_OLED_SPI_FREQ_HZ         4000000UL

#define BOARD_OLED_WIDTH               128U
#define BOARD_OLED_HEIGHT              64U
#define BOARD_OLED_PAGE_NUM            8U
#define BOARD_OLED_BUF_SIZE            1024U
#define BOARD_OLED_X_OFFSET_PIXELS     2U
#define BOARD_SPI_TIMEOUT_LOOPS        120000UL

#define KEY_ROW_NUM                    4U
#define KEY_COL_NUM                    4U

#define KEY_ROW0_GPIO                  25U
#define KEY_ROW1_GPIO                  26U
#define KEY_ROW2_GPIO                  27U
#define KEY_ROW3_GPIO                  44U

#define KEY_COL0_GPIO                  45U
#define KEY_COL1_GPIO                  46U
#define KEY_COL2_GPIO                  48U
#define KEY_COL3_GPIO                  33U

#define KEY_ACTIVE_LEVEL               0U
#define KEY_INACTIVE_LEVEL             1U

#define KEY_SCAN_PERIOD_MS             10U
#define OLED_REFRESH_PERIOD_MS         100U

#endif
