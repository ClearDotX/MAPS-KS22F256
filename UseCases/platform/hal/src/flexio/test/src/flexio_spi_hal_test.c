/* flexio_spi_master_hal_test.c */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "fsl_flexio_hal.h"
#include "fsl_flexio_spi_hal.h"
#include "fsl_clock_manager.h"
#include "board.h"
#include "fsl_device_registers.h"

#define FLEXIO_BUS_CLK_HZ    (48000000U)
#define FLEXIO_SPI_BAUDRATE  (115200U)

flexio_spi_dev_t mFlexioSpiDevStruct =
{
    FLEXIO_BASE_PTR, /* flexioBase */
    6U,       /* txPinIdx */
    7U,       /* rxPinIdx */
    5U,       /* sclkPinIdx */
    4U,       /* csnPinIdx */
    {0U, 1U}, /* shifterIdx */
    {0U, 1U}  /* timerIdx */
};
flexio_spi_master_config_t mFlexioSpiMasterConfigStruct =
{
    FLEXIO_BUS_CLK_HZ,   /* flexioBusClk */
    FLEXIO_SPI_BAUDRATE, /* baudrate */
    8U,                  /* bitCount */
    false                /* cphaOneEnable */
};

static void hardware_init_flexio_spi_pins(void);
static void hardware_init_flexio_clock_source(void);
static void flexio_common_configure(void);
static void flexio_spi_master_init(void);
static void flexio_spi_putchar(uint8_t ch);
static uint8_t flexio_spi_getchar(void);

int main(void)
{
    uint8_t masterRxByte = 0U;   /* master rx. */
    uint8_t masterTxByte = 0U;   /* master tx. */

    hardware_init();
    dbg_uart_init();

    /* Configure the pin mux for flexio. */
    hardware_init_flexio_spi_pins();

    printf("\r\nflexio_spi_master HAL Test: Start...\r\n");

    /* Enable the clock for flexio. */
    hardware_init_flexio_clock_source();
    CLOCK_SYS_EnableFlexioClock(0U);

    flexio_common_configure();
    flexio_spi_master_init();

    /* terminal uart_tx -> flexio uart_tx -> ttl uart_rx. */
    printf("\r\n");
    printf("frdm_kl43 board                 ->> frdm_kl25\r\n");
    printf("uart_tx -> flexio spi_master_tx ->> spi_slave_rx -> uart_rx.\r\n");
    printf("type 'x' in this terminal would exit.\r\n");
    while (1)
    {
        printf("\r\n");
        printf("*START-----------------------------------*\r\n");

        /* Get a token from terminal. */
        printf("Input the token: ");
        masterTxByte = getchar();
        if ('x' == masterTxByte)
        {
            printf("Exit the loop.\r\n");
            break;
        }
        printf("%c (0x%X)\r\n", masterTxByte, masterTxByte);

        /* Master - Fill the master's tx buffer to trigger the communication. */
        flexio_spi_putchar(masterTxByte); /* Send token. */
        printf("master tx done.\r\n");

        /* Master - Read the master's rx buffer. */
        masterRxByte = flexio_spi_getchar();
        printf("master rx done.\r\n");

        printf("master tx: 0x%X || rx: 0x%X\r\n", masterTxByte, masterRxByte);
        printf("*----------------------------------------*\r\n");
    }

    printf("flexio_uart HAL Test: End\r\n");

    return 0;
}

static void hardware_init_flexio_clock_source(void)
{
    uint32_t tmp = SIM->SOPT2 & ~SIM_SOPT2_FLEXIOSRC_MASK;
    tmp |= SIM_SOPT2_FLEXIOSRC(1U); /* IRC48M clock. */
    SIM->SOPT2 = tmp;
}

static void hardware_init_flexio_spi_pins(void)
{
    PORTD->PCR[4] = PORT_PCR_MUX(6);
    PORTD->PCR[5] = PORT_PCR_MUX(6);
    PORTD->PCR[6] = PORT_PCR_MUX(6);
    PORTD->PCR[7] = PORT_PCR_MUX(6);
}

static void flexio_common_configure(void)
{
    /* Configure the global setting for flexio. */
    /* Reset the FlexIO hardware. */
    FLEXIO_HAL_SetFlexioEnableCmd(FLEXIO_BASE_PTR, false); /* Disable the Flexio first. */
    FLEXIO_HAL_SetSoftwareResetCmd(FLEXIO_BASE_PTR, true);
    FLEXIO_HAL_SetSoftwareResetCmd(FLEXIO_BASE_PTR, false);
    /* Initialize FlexIO. */
    FLEXIO_HAL_SetDozeModeCmd(FLEXIO_BASE_PTR, true);
    FLEXIO_HAL_SetDebugModeCmd(FLEXIO_BASE_PTR, true);
    FLEXIO_HAL_SetFastAccessCmd(FLEXIO_BASE_PTR, true);
    FLEXIO_HAL_SetFlexioEnableCmd(FLEXIO_BASE_PTR, true);
}

static void flexio_spi_master_init(void)
{
    /* config flexio spi_master. */
    FLEXIO_SPI_HAL_ConfigMaster(&mFlexioSpiDevStruct, &mFlexioSpiMasterConfigStruct);
}
static void flexio_spi_putchar(uint8_t ch)
{
    FLEXIO_SPI_HAL_PutDataMSBPolling(&mFlexioSpiDevStruct, (uint32_t)ch);
}

static uint8_t flexio_spi_getchar(void)
{
    return FLEXIO_SPI_HAL_GetDataMSBPolling(&mFlexioSpiDevStruct)>>24U;
}

/* EOF. */
