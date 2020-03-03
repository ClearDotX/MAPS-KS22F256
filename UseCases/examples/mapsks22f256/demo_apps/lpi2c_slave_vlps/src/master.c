/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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
 * THIS SOFTWARE IS PROVIDEDRRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTA BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <string.h>
#include "fsl_device_registers.h"

#include "fsl_lpuart_hal.h"
#include "fsl_lpi2c_hal.h"
#include "fsl_port_hal.h"

#include "config.h"

static uint8_t gBuffer[BUFFER_SIZE] = "hello i2c slaver";

static void dbguart_init(void)
{
    // enable the debug uart port clock
    SIM_BWR_SCGC5_PORTE(SIM, 1);
    // uart pin mux
    PORT_HAL_SetMuxMode(PORTE, 4u, kPortMuxAlt3);
    PORT_HAL_SetMuxMode(PORTE, 5u, kPortMuxAlt3);

    // enable uart clock gating
    SIM_BWR_SCGC6_LPUART0(SIM, 1);
    // select the MCGPLLCLK as LPUART0 source
    SIM_BWR_SOPT2_LPUARTSRC(SIM, 0x1);

    // init UART
    LPUART_HAL_Init(LPUART0);
    LPUART_HAL_SetBaudRate(LPUART0, 80000000, 115200);
    LPUART_HAL_SetTransmitterCmd(LPUART0, true);
    LPUART_HAL_SetReceiverCmd(LPUART0, true);
}

static void lpi2c_init(void)
{
    // enable PORTB clock gating
    SIM_BWR_SCGC5_PORTB(SIM, 1);

    // Init the pinmux for LPI2C0 SCL/SDA
    PORT_HAL_SetMuxMode(PORTB, 0u, kPortMuxAlt2); // LPI2C0_SDA
    PORT_HAL_SetMuxMode(PORTB, 1u, kPortMuxAlt2); // LPI2C0_SCL

    // enable LPI2C0 clock gating
    SIM_BWR_SCGC4_LPI2C0(SIM, 1);
    // configure the LPI2C0 clock source as MCGPLLCLK
    SIM_BWR_SOPT2_LPI2C0SRC(SIM, 0x1);

    // Init the LPI2C0 as Master
    // enable working in low power and debug mode
    LPI2C_HAL_MasterInit(LPI2C0);
    // set baud rate to 400Kbps, slow mode (4MHz clksrc)
    LPI2C_HAL_SetBaudRate(LPI2C0, 80000000, 400, kLpi2cMasterFastMode); 
    LPI2C_WR_MFCR(LPI2C0, 0);
    // enable master
    LPI2C_HAL_SetMasterCmd(LPI2C0, true);
}

static int32_t recv_print(uint8_t *recvBuf, uint32_t dataLen)
{
    lpi2c_status_t ret;
    int32_t i;

    // receive data from the slave
    ret = LPI2C_HAL_MasterReceiveDataPolling(LPI2C0,
                                  SLAVE_ADDRESS, NULL, 0,
                                  recvBuf, dataLen);
    if (kStatus_LPI2C_Success != ret)
    {
        printf("Failed to read from the slave! len:%d ret:%u\r\n",
                                                     dataLen, ret);
        return -1;
    }

    putchar('"');
    // print out all of the datas in slave
    for (i = 0; i < dataLen; i++)
    {
        printf("%c", recvBuf[i]);
    }
    printf("\"\r\n");
    return 0;
}

/*!
 * @brief master to demonstrate the LPI2C lp slave
 *
 */
int main(void)
{
    lpi2c_status_t ret;
    uint8_t recvBuf[BUFFER_SIZE];
    uint32_t dataLen;

    // init debug uart
    dbguart_init();

    // init LPI2C0 for master
    lpi2c_init();

    printf("\r\nRead data from the slave:\r\n");
    recv_print(recvBuf, sizeof(recvBuf));

    printf("Fill the slave buffer.....");

    // Fill the slave with master's buffer
    ret = LPI2C_HAL_MasterSendDataPolling(LPI2C0, SLAVE_ADDRESS,
                                          NULL, 0, gBuffer, sizeof(gBuffer));
    if (kStatus_LPI2C_Success != ret)
    {
        printf("Reset slave buffer failed, ret:%u\r\n", ret);
        return 0;
    }
    printf("done\r\n");

    printf("Read back from slave:\r\n");
    recv_print(recvBuf, sizeof(recvBuf));


    while (1)
    {
    }
}
