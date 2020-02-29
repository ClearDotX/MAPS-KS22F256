/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "AT24C02.h"

#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_LED_GPIO BOARD_LED_RED2_GPIO
#define BOARD_LED_GPIO_PIN BOARD_LED_RED2_GPIO_PIN

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    at24c02_handler_t handler;
    at24c02_status_t status;
    uint8_t write_data;
    uint8_t read_data;
    uint8_t eeprom_add;
    uint16_t cnt;

    /* Initializes board hardware. */
    BOARD_InitPins();
    BOARD_BootClockHSRUN();
    BOARD_InitDebugConsole();
    CLOCK_SetPllFllSelClock(3U); /* IRC48 MHz clock. */
    CLOCK_SetLpi2c0Clock(1);

    PRINTF("EEPROM Demo\r\n\r\n");
    PRINTF("This demo will write data into on board EEPROM and then read it back.\r\n\r\n");

    /* Configures the at24c02 handler */
    handler.i2cBase = AT24C02_I2C_BASE;
    handler.device_addr = AT24C02_I2C_ADDR;
    handler.baudRate_kbps = AT24C02_I2C_BAUDRATE;

    /* Initializes the i2c which to be used to operate the at24c02 */
    AT24C02_I2CInit(&handler);

    for (cnt = 0; cnt < 256; cnt++)
    {
        eeprom_add = cnt;
        write_data = 255 - cnt;

        status = AT24C02_ByteWrite(&handler, eeprom_add, write_data);
        if (status == kStatus_AT24C02_I2CFail)
        {
            PRINTF("Write failure\r\n");
        }

        AT24C02_ACKPoll(&handler);

        status = AT24C02_ByteRead(&handler, eeprom_add, &read_data);
        if (status == kStatus_AT24C02_I2CFail)
        {
            PRINTF("Read failure\r\n");
        }

        if (read_data != write_data)
        {
            PRINTF("Read data is 0x%x instead of 0x%x\r\n", read_data, write_data);
        }
        else
        {
            PRINTF("Write data at 0x%x is 0x%x, read back data is 0x%x\r\n", eeprom_add, write_data, read_data);
        }
    }

    PRINTF("\r\nEnd of the demo.\n\r");

    while (1)
    {
    }
}
