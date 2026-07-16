/*
 * File: board_spi.c
 * Description: LAUNCHXL-F2800157 SPIA transport for the SSD1306 OLED.
 */

#include "board_spi.h"

#include "board_pinmap.h"

static uint16_t g_board_spi_status;

#ifdef UNIT_TEST
#define BOARD_SPI_MOCK_COMMAND_MAX 64u
static uint16_t g_board_spi_mock_command[BOARD_SPI_MOCK_COMMAND_MAX];
static uint16_t g_board_spi_mock_command_count;
static uint16_t g_board_spi_mock_data_count;
#endif

#ifndef UNIT_TEST
static void BoardSPI_SetPinMuxToGpio(uint32_t pin)
{
    switch (pin)
    {
#ifdef GPIO_11_GPIO11
    case 11U:
        GPIO_setPinConfig(GPIO_11_GPIO11);
        break;
#endif
#ifdef GPIO_42_GPIO42
    case 42U:
        GPIO_setPinConfig(GPIO_42_GPIO42);
        break;
#endif
#ifdef GPIO_43_GPIO43
    case 43U:
        GPIO_setPinConfig(GPIO_43_GPIO43);
        break;
#endif
    default:
        break;
    }
}

static void BoardSPI_ConfigPins(void)
{
#ifdef GPIO_8_SPIA_SIMO
    GPIO_setPinConfig(GPIO_8_SPIA_SIMO);
#endif
#ifdef GPIO_9_SPIA_CLK
    GPIO_setPinConfig(GPIO_9_SPIA_CLK);
#endif

    GPIO_setPadConfig(BOARD_OLED_SPI_MOSI_GPIO, GPIO_PIN_TYPE_STD);
    GPIO_setPadConfig(BOARD_OLED_SPI_CLK_GPIO, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(BOARD_OLED_SPI_MOSI_GPIO, GPIO_QUAL_ASYNC);
    GPIO_setQualificationMode(BOARD_OLED_SPI_CLK_GPIO, GPIO_QUAL_ASYNC);

    BoardSPI_SetPinMuxToGpio(BOARD_OLED_SPI_CS_GPIO);
    BoardSPI_SetPinMuxToGpio(BOARD_OLED_DC_GPIO);
    BoardSPI_SetPinMuxToGpio(BOARD_OLED_RES_GPIO);

    GPIO_setPadConfig(BOARD_OLED_SPI_CS_GPIO, GPIO_PIN_TYPE_STD);
    GPIO_setPadConfig(BOARD_OLED_DC_GPIO, GPIO_PIN_TYPE_STD);
    GPIO_setPadConfig(BOARD_OLED_RES_GPIO, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(BOARD_OLED_SPI_CS_GPIO, GPIO_DIR_MODE_OUT);
    GPIO_setDirectionMode(BOARD_OLED_DC_GPIO, GPIO_DIR_MODE_OUT);
    GPIO_setDirectionMode(BOARD_OLED_RES_GPIO, GPIO_DIR_MODE_OUT);

    GPIO_writePin(BOARD_OLED_SPI_CS_GPIO, 1U);
    GPIO_writePin(BOARD_OLED_DC_GPIO, 0U);
    GPIO_writePin(BOARD_OLED_RES_GPIO, 1U);
}

static void BoardSPI_ResetOled(void)
{
    GPIO_writePin(BOARD_OLED_RES_GPIO, 0U);
    SysCtl_delay(BOARD_SYSCLK_FREQ_HZ / 30UL);

    GPIO_writePin(BOARD_OLED_RES_GPIO, 1U);
    SysCtl_delay(BOARD_SYSCLK_FREQ_HZ / 30UL);
}

static uint16_t BoardSPI_Write(const uint16_t *data, uint16_t len, uint16_t dc_level)
{
    uint16_t index;
    uint32_t timeout;

    if ((data == 0) || (len == 0u))
    {
        return BOARD_SPI_STATUS_OK;
    }

    GPIO_writePin(BOARD_OLED_DC_GPIO, dc_level);
    GPIO_writePin(BOARD_OLED_SPI_CS_GPIO, 0U);

    for (index = 0u; index < len; index++)
    {
        timeout = BOARD_SPI_TIMEOUT_LOOPS;
        while (((HWREGH(BOARD_OLED_SPI_BASE + SPI_O_STS) & SPI_STS_BUFFULL_FLAG) != 0u) &&
               (timeout != 0u))
        {
            timeout--;
        }
        if (timeout == 0u)
        {
            GPIO_writePin(BOARD_OLED_SPI_CS_GPIO, 1U);
            return BOARD_SPI_STATUS_TIMEOUT;
        }

        SPI_writeDataNonBlocking(BOARD_OLED_SPI_BASE, (uint16_t)(data[index] << 8));

        timeout = BOARD_SPI_TIMEOUT_LOOPS;
        while (((HWREGH(BOARD_OLED_SPI_BASE + SPI_O_STS) & SPI_STS_INT_FLAG) == 0u) &&
               (timeout != 0u))
        {
            timeout--;
        }
        if (timeout == 0u)
        {
            GPIO_writePin(BOARD_OLED_SPI_CS_GPIO, 1U);
            return BOARD_SPI_STATUS_TIMEOUT;
        }

        (void)SPI_readDataNonBlocking(BOARD_OLED_SPI_BASE);
    }

    GPIO_writePin(BOARD_OLED_SPI_CS_GPIO, 1U);
    return BOARD_SPI_STATUS_OK;
}
#endif

void BoardSPI_Init(void)
{
    g_board_spi_status = BOARD_SPI_STATUS_OK;
#ifndef UNIT_TEST
    uint32_t lspclk_hz;

    SysCtl_setLowSpeedClock(SYSCTL_LSPCLK_PRESCALE_1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SPIA);
    BoardSPI_ConfigPins();
    lspclk_hz = SysCtl_getLowSpeedClock(BOARD_SYSCLK_FREQ_HZ);

    SPI_disableModule(BOARD_OLED_SPI_BASE);
    SPI_setConfig(BOARD_OLED_SPI_BASE,
                  lspclk_hz,
                  SPI_PROT_POL1PHA1,
                  SPI_MODE_MASTER,
                  BOARD_OLED_SPI_FREQ_HZ,
                  8U);
    SPI_disableLoopback(BOARD_OLED_SPI_BASE);
    SPI_setEmulationMode(BOARD_OLED_SPI_BASE, SPI_EMULATION_FREE_RUN);
    SPI_enableModule(BOARD_OLED_SPI_BASE);

    BoardSPI_ResetOled();
#endif
}

uint16_t BoardSPI_WriteCommand(const uint16_t *data, uint16_t len)
{
#ifndef UNIT_TEST
    g_board_spi_status = BoardSPI_Write(data, len, 0U);
#else
    uint16_t index;

    if (data != 0)
    {
        for (index = 0u; (index < len) &&
             (g_board_spi_mock_command_count < BOARD_SPI_MOCK_COMMAND_MAX); index++)
        {
            g_board_spi_mock_command[g_board_spi_mock_command_count++] = data[index];
        }
    }
#endif
    return g_board_spi_status;
}

uint16_t BoardSPI_WriteData(const uint16_t *data, uint16_t len)
{
#ifndef UNIT_TEST
    g_board_spi_status = BoardSPI_Write(data, len, 1U);
#else
    (void)data;
    g_board_spi_mock_data_count = (uint16_t)(g_board_spi_mock_data_count + len);
#endif
    return g_board_spi_status;
}

uint16_t BoardSPI_GetStatus(void)
{
    return g_board_spi_status;
}

#ifdef UNIT_TEST
void BoardSPI_MockResetTrace(void)
{
    g_board_spi_status = BOARD_SPI_STATUS_OK;
    g_board_spi_mock_command_count = 0u;
    g_board_spi_mock_data_count = 0u;
}

uint16_t BoardSPI_MockGetCommandCount(void)
{
    return g_board_spi_mock_command_count;
}

uint16_t BoardSPI_MockGetDataCount(void)
{
    return g_board_spi_mock_data_count;
}

uint16_t BoardSPI_MockGetCommand(uint16_t index)
{
    if (index >= g_board_spi_mock_command_count)
    {
        return 0u;
    }
    return g_board_spi_mock_command[index];
}
#endif
