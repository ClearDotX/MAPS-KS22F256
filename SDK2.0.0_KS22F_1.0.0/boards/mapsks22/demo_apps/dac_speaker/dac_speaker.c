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
#include "fsl_dac.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_DAC_INSTANCE DAC0

uint8_t g_msg[] =
    "DAC demo\r\n"
    "DAC output to control the sound of speaker.\r\n";

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
    dac_config_t dacConfigStruct;
    uint32_t dacData;
    uint8_t receiveBuff[8];
    uint8_t i, count;

    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockHSRUN();
    BOARD_InitDebugConsole();

    /* Configure the DAC. */
    DAC_GetDefaultConfig(&dacConfigStruct);
    DAC_Init(DEMO_DAC_INSTANCE, &dacConfigStruct);
    DAC_SetBufferReadPointer(DEMO_DAC_INSTANCE, 0U);

    PRINTF("%s", g_msg);

    while (1)
    {
        PRINTF("\r\nInput value(0--4095): ");
        i = 0;
        count = 0;
        while (1)
        {
            receiveBuff[i] = GETCHAR();

            if (receiveBuff[i] == 0x08) /* backspace */
            {
                PUTCHAR(0x08);
                PUTCHAR(0x20);
                PUTCHAR(0x08);
                count--;
                i--;
            }
            else if ((receiveBuff[i] >= '0') && (receiveBuff[i] <= '9'))
            {
                if (count < 4)
                {
                    PUTCHAR(receiveBuff[i]);
                    receiveBuff[i] -= '0';
                    i++;
                    count++;
                }
            }
            else if (receiveBuff[i] == 0x0D) /* Enter */
            {
                break;
            }
            else
            {
            }
        }

        dacData = 0;
        for (i = 0; i < count; i++)
        {
            dacData = dacData * 10 + receiveBuff[i];
        }

        if (dacData > 4095)
        {
            PRINTF("\r\nThe input value is invalid.");
            continue;
        }

        /* DAC output to the speaker */
        DAC_SetBufferValue(DEMO_DAC_INSTANCE, 0U, dacData);
    }
}
