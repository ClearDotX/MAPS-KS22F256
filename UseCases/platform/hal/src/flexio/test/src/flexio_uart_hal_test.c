/* flexio_i2c_hal_test.c */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "fsl_flexio_hal.h"
#include "fsl_flexio_uart_hal.h"
#include "fsl_clock_manager.h"
#include "board.h"
#include "fsl_device_registers.h"

#define FLEXIO_BUS_CLK_HZ    (48000000U)
#define FLEXIO_UART_BAUDRATE (115200U)

/* flexio_uart_tx. */
static flexio_uart_tx_dev_t mFlexioUartTxDevStruct =
{
    FLEXIO_BASE_PTR, /*!< FlexIO module base address. */
    5U,       /*!< Pin index for UART Tx in FlexIO. */
    0U,       /*!< Shifter index used for UART Tx in FlexIO. */
    0U        /*!< Timer index used for UART Tx in FlexIO. */
};

/* flexio_uart_rx. */
static flexio_uart_rx_dev_t mFlexioUartRxDevStruct =
{
    FLEXIO_BASE_PTR, /*!< FlexIO module base address. */
    7U,       /*!< Pin index for UART Rx in FlexIO. */
    1U,       /*!< Shifter index used for UART Rx in FlexIO. */
    1U        /*!< Timer index used for UART Rx in FlexIO. */
};

/* flexio_uart_config. */
static flexio_uart_config_t mFlexioUartDevConfigStruct =
{
    FLEXIO_BUS_CLK_HZ,    /*!< FlexIO bus clock frequency in Hz. */
    FLEXIO_UART_BAUDRATE, /*!< UART Tx bandrate in bps. */
    8U                    /*!< UART Tx data length in bit. */
};

static void hardware_init_flexio_uart_pins(void);
static void hardware_init_flexio_clock_source(void);

static void flexio_uart_init(void);
static uint8_t flexio_uart_getchar(void);
static void flexio_uart_putchar(uint8_t ch);

int main(void)
{
    uint8_t ch;
    uint32_t i;
    uint32_t errCounter = 0U;
    uint32_t totalErrCounter = 0U;

    hardware_init();
    dbg_uart_init();

    /* Configure the pin mux for flexio. */
    hardware_init_flexio_uart_pins();

    printf("\r\nflexio_uart HAL Test: Start...\r\n");

    /* Enable the clock for flexio. */
    hardware_init_flexio_clock_source();
    CLOCK_SYS_EnableFlexioClock(0U);

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

    flexio_uart_init();

    /* terminal uart_tx -> flexio uart_tx -> ttl uart_rx. */
    printf("\r\n");
    printf("terminal uart_tx -> flexio uart_tx -> ttl uart_rx.\r\n");
    printf("type 'x' in this terminal would exit.\r\n");
    while ('x' != (ch = getchar()) )
    {
        putchar(ch); /* echo back. */
        flexio_uart_putchar(ch); /* send by flexio_uart_tx. */
    }

    /* terminal uart_rx <- flexio uart_rx <- ttl uart_tx. */
    printf("\r\n");
    printf("terminal uart_rx <- flexio uart_rx <- ttl uart_tx.\r\n");
    printf("type 'x' in another terminal would exit.\r\n");
    while ('x' != (ch = flexio_uart_getchar()))
    {
        flexio_uart_putchar(ch); /* echo back. */
        putchar(ch); /* send by terminal uart tx. */
    }

    printf("\r\n");
    printf("flexio_uart HAL Test: End\r\n");

    return 0;
}


static void hardware_init_flexio_clock_source(void)
{
    uint32_t tmp = SIM->SOPT2 & ~SIM_SOPT2_FLEXIOSRC_MASK;
    tmp |= SIM_SOPT2_FLEXIOSRC(1U); /* IRC48M clock. */
    SIM->SOPT2 = tmp;

}
static void hardware_init_flexio_uart_pins(void)
{
    PORTD->PCR[5] = PORT_PCR_MUX(6) | PORT_PCR_PS(1) | PORT_PCR_PE(1) ;  /* FlexIO D5 , Pull up resistor enable*/
    PORTD->PCR[7] = PORT_PCR_MUX(6) | PORT_PCR_PS(1) | PORT_PCR_PE(1) ;  /* FlexIO D7 , Pull up resistor enable*/
}

static void flexio_uart_init(void)
{
    /* config flexio uart_tx. */
    FLEXIO_UART_Tx_HAL_Configure(
        &mFlexioUartTxDevStruct, &mFlexioUartDevConfigStruct);
    /* config flexio uart_rx. */
    FLEXIO_UART_Rx_HAL_Configure(
        &mFlexioUartRxDevStruct, &mFlexioUartDevConfigStruct);
    
}
static void flexio_uart_putchar(uint8_t ch)
{
    FLEXIO_UART_Tx_HAL_PutDataPolling(&mFlexioUartTxDevStruct, (uint32_t)ch);
}

static uint8_t flexio_uart_getchar(void)
{
    return (uint8_t)FLEXIO_UART_Rx_HAL_GetDataPolling(&mFlexioUartRxDevStruct);
}


/* EOF. */


