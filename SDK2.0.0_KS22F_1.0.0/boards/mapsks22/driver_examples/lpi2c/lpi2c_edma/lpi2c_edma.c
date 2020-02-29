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

/*  Standard C Included Files */
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_lpi2c.h"
#include "fsl_lpi2c_edma.h"
#include "fsl_edma.h"
#include "fsl_dmamux.h"

#include "fsl_device_registers.h"
#include "pin_mux.h"
#include <stdbool.h>
#include "clock_config.h"
#include "stdbool.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_I2C_MASTER_BASE (LPI2C0_BASE)
#define EXAMPLE_I2C_SLAVE_BASE (LPI2C1_BASE)
#define LPI2C_CLOCK_FREQUENCY 48000000U
#define LPI2CMASTER_TRANSMIT_EDMA_REQUEST_SOURCE 26
#define LPI2CMASTER_RECEIVE_EDMA_REQUEST_SOURCE 18
#define EXAMPLE_LPI2C_MASTER_DMA_MUX (DMAMUX)
#define EXAMPLE_LPI2C_MASTER_DMA (DMA0)

#define EXAMPLE_I2C_MASTER ((LPI2C_Type *)EXAMPLE_I2C_MASTER_BASE)
#define EXAMPLE_I2C_SLAVE ((LPI2C_Type *)EXAMPLE_I2C_SLAVE_BASE)

#define I2C_MASTER_SLAVE_ADDR_7BIT (0x7EU)
#define I2C_BAUDRATE (100000) /* 100K */

#define I2C_DATA_LENGTH (32) /* MAX is 256 */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_slave_buff[I2C_DATA_LENGTH];
uint8_t g_master_buff[I2C_DATA_LENGTH];

lpi2c_master_edma_handle_t g_m_edma_handle;
lpi2c_slave_handle_t g_s_handle;
edma_handle_t g_edmaTxHandle;
edma_handle_t g_edmaRxHandle;

volatile bool g_slaveCompleted = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void lpi2c_slave_callback(LPI2C_Type *base, lpi2c_slave_transfer_t *xfer, void *param)
{
    switch (xfer->event)
    {
        /* Setup the slave receive buffer */
        case kLPI2C_SlaveReceiveEvent:
            xfer->data = g_slave_buff;
            xfer->dataSize = I2C_DATA_LENGTH;
            break;

        /* The master has sent a stop transition on the bus */
        case kLPI2C_SlaveCompletionEvent:
            g_slaveCompleted = true;
            break;

        /* Transmit event not handled in this demo */
        case kLPI2C_SlaveTransmitEvent:
        default:
            break;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t i = 0;
    lpi2c_slave_config_t slaveConfig;
    lpi2c_master_config_t masterConfig;
    lpi2c_master_transfer_t masterXfer = {0};
    edma_config_t userConfig;

    BOARD_InitPins();
    BOARD_BootClockHSRUN();
    BOARD_InitDebugConsole();
 
    CLOCK_SetPllFllSelClock(3U); /* IRC48 MHz clock. */
    CLOCK_SetLpi2c0Clock(1);
    CLOCK_SetLpi2c1Clock(1);

    PRINTF("\r\n==I2C example -- MasterDMA_SlaveInterrupt.==\r\n");

    /* DMAMUX init */
    DMAMUX_Init(EXAMPLE_LPI2C_MASTER_DMA_MUX);

    DMAMUX_SetSource(EXAMPLE_LPI2C_MASTER_DMA_MUX, 0, LPI2CMASTER_TRANSMIT_EDMA_REQUEST_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_LPI2C_MASTER_DMA_MUX, 0);

    DMAMUX_SetSource(EXAMPLE_LPI2C_MASTER_DMA_MUX, 1, LPI2CMASTER_RECEIVE_EDMA_REQUEST_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_LPI2C_MASTER_DMA_MUX, 1);

    /* EDMA init */
    EDMA_GetDefaultConfig(&userConfig);
    EDMA_Init(EXAMPLE_LPI2C_MASTER_DMA, &userConfig);

    /* Set up i2c slave first */
    LPI2C_SlaveGetDefaultConfig(&slaveConfig);

    /* Change the slave address */
    slaveConfig.address0 = I2C_MASTER_SLAVE_ADDR_7BIT;

    /* Initialize the LPI2C slave peripheral */
    LPI2C_SlaveInit(EXAMPLE_I2C_SLAVE, &slaveConfig, LPI2C_CLOCK_FREQUENCY);

    memset(g_slave_buff, 0, sizeof(g_slave_buff));

    /* Create the LPI2C handle for the non-blocking transfer */
    LPI2C_SlaveCreateHandle(EXAMPLE_I2C_SLAVE, &g_s_handle, lpi2c_slave_callback, NULL);

    /* Start accepting I2C transfers on the LPI2C slave peripheral */
    LPI2C_SlaveTransferNonBlocking(EXAMPLE_I2C_SLAVE, &g_s_handle,
                                   kLPI2C_SlaveReceiveEvent | kLPI2C_SlaveCompletionEvent);

    /* Set up i2c master to send data to slave */
    for (i = 0; i < I2C_DATA_LENGTH; i++)
    {
        g_master_buff[i] = i;
    }

    /* Display the data the master will send */
    PRINTF("Master will send data :");
    for (i = 0; i < I2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_master_buff[i]);
    }

    PRINTF("\r\n\r\n");

    LPI2C_MasterGetDefaultConfig(&masterConfig);

    /* Change the default baudrate configuration */
    masterConfig.baudRate_Hz = I2C_BAUDRATE;

    /* Initialize the LPI2C master peripheral */
    LPI2C_MasterInit(EXAMPLE_I2C_MASTER, &masterConfig, LPI2C_CLOCK_FREQUENCY);

    /* Create the EDMA channel handles */
    EDMA_CreateHandle(&g_edmaTxHandle, EXAMPLE_LPI2C_MASTER_DMA, 0);
    EDMA_CreateHandle(&g_edmaRxHandle, EXAMPLE_LPI2C_MASTER_DMA, 1);

    /* Create the LPI2C master DMA driver handle */
    LPI2C_MasterCreateEDMAHandle(EXAMPLE_I2C_MASTER, &g_m_edma_handle, &g_edmaRxHandle, &g_edmaTxHandle, NULL, NULL);

    /* Setup the master transfer */
    masterXfer.slaveAddress = I2C_MASTER_SLAVE_ADDR_7BIT;
    masterXfer.direction = kLPI2C_Write;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = g_master_buff;
    masterXfer.dataSize = I2C_DATA_LENGTH;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;

    /* Send master non-blocking data to slave */
    LPI2C_MasterTransferEDMA(EXAMPLE_I2C_MASTER, &g_m_edma_handle, &masterXfer);

    /* Wait for the transfer to complete. */
    while (!g_slaveCompleted)
    {
    }

    /* Transfer completed. Check the data. */
    for (i = 0; i < I2C_DATA_LENGTH; i++)
    {
        if (g_slave_buff[i] != g_master_buff[i])
        {
            PRINTF("\r\nError occured in this transfer ! \r\n");
            break;
        }
    }

    /* Check if we matched on all of the bytes */
    if (i == I2C_DATA_LENGTH)
    {
        PRINTF("\r\n Transfer successful!\r\n ");
    }

    /* Display the slave data */
    PRINTF("\r\nSlave received data :");
    for (i = 0; i < I2C_DATA_LENGTH; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_slave_buff[i]);
    }
    PRINTF("\r\n\r\n");

    /* Hang at the end */
    while (1)
    {
    }
}
