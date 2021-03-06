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

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_flexcan.h"
#include "fsl_flexcan_edma.h"
#include "board.h"

#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_CAN             CAN0
#define EXAMPLE_CAN_CLKSRC      kCLOCK_BusClk
#define EXAMPLE_CAN_DMA_CHANNEL 0
#define EXAMPLE_CAN_DMA_REQUEST 11
#define TX_MESSAGE_BUFFER_NUM (9)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool txComplete = false;
volatile bool rxComplete = false;
flexcan_handle_t flexcanHandle;
flexcan_edma_handle_t flexcanEdmaHandle;
edma_handle_t flexcanRxFifoEdmaHandle;
flexcan_mb_transfer_t txXfer;
flexcan_fifo_transfer_t rxFifoXfer;
flexcan_frame_t txFrame, rxFrame;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief FlexCAN Call Back function
 */
static void flexcan_callback(CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *userData)
{
    /* Process FlexCAN Tx event. */
    if ((kStatus_FLEXCAN_TxIdle == status) && (TX_MESSAGE_BUFFER_NUM == result))
    {
        txComplete = true;
    }
}

/*!
 * @brief FlexCAN DMA Call Back function
 */
static void flexcan_dma_callback(CAN_Type *base, flexcan_edma_handle_t *handle, status_t status, void *userData)
{
    /* Process FlexCAN Rx event. */
    if (kStatus_FLEXCAN_RxFifoIdle == status)
    {
        rxComplete = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    edma_config_t edmaConfig;
    flexcan_config_t flexcanConfig;
    flexcan_rx_fifo_config_t rxFifoConfig;
    uint32_t rxFifoFilter[] = {
        FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(0x321, 0, 0), FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(0x321, 1, 0),
        FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(0x123, 0, 0), FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(0x123, 1, 0)};
    uint8_t i;

    /* Initialize board hardware. */
    BOARD_InitPins();
    BOARD_BootClockHSRUN();

    BOARD_InitDebugConsole();

    PRINTF("\r\n==FlexCAN loopback edma example -- Start.==\r\n\r\n");

    /* Init FlexCAN module. */
    FLEXCAN_GetDefaultConfig(&flexcanConfig);
    flexcanConfig.clkSrc = KFLEXCAN_ClkSrcPeri;
    flexcanConfig.enableLoopBack = true;
    FLEXCAN_Init(EXAMPLE_CAN, &flexcanConfig, CLOCK_GetFreq(EXAMPLE_CAN_CLKSRC));

    /* Configure DMA. */
    DMAMUX_Init(DMAMUX0);
    DMAMUX_SetSource(DMAMUX0, EXAMPLE_CAN_DMA_CHANNEL, EXAMPLE_CAN_DMA_REQUEST);
    DMAMUX_EnableChannel(DMAMUX0, EXAMPLE_CAN_DMA_CHANNEL);

    EDMA_GetDefaultConfig(&edmaConfig);
    EDMA_Init(DMA0, &edmaConfig);

    /* Create EDMA handle. */
    EDMA_CreateHandle(&flexcanRxFifoEdmaHandle, DMA0, EXAMPLE_CAN_DMA_CHANNEL);

    /* Enable FlexCAN module. */
    FLEXCAN_Enable(EXAMPLE_CAN, true);

    /* Setup Tx Message Buffer. */
    FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM);

    /* Setup Rx FIFO. */
    rxFifoConfig.idFilterTable = rxFifoFilter;
    rxFifoConfig.idFilterType = KFLEXCAN_RxFifoFilterTypeA;
    rxFifoConfig.idFilterNum = sizeof(rxFifoFilter) / sizeof(rxFifoFilter[0]);
    rxFifoConfig.priority = KFLEXCAN_RxFifoPrioHigh;
    FlEXCAN_SetRxFifoConfig(EXAMPLE_CAN, &rxFifoConfig);

    /* Create FlexCAN handle structure and set call back function. */
    FLEXCAN_CreateHandle(EXAMPLE_CAN, &flexcanHandle, flexcan_callback, NULL);

    /* Create FlexCAN EDMA handle structure and set call back function. */
    FLEXCAN_CreateHandleEDMA(EXAMPLE_CAN, &flexcanEdmaHandle, flexcan_dma_callback, NULL, &flexcanRxFifoEdmaHandle);

    /* Send first message through Tx Message Buffer. */
    txFrame.format = KFLEXCAN_FrameFormatStandard;
    txFrame.type = KFLEXCAN_FrameTypeData;
    txFrame.id = CAN_ID_STD(0x123);
    txFrame.length = 8;
    txFrame.dataWord0 = CAN_WORD0_DATA_BYTE_0(0x11) | CAN_WORD0_DATA_BYTE_1(0x11) | CAN_WORD0_DATA_BYTE_2(0x11) |
                        CAN_WORD0_DATA_BYTE_3(0x11);
    txFrame.dataWord1 = CAN_WORD1_DATA_BYTE_4(0x11) | CAN_WORD1_DATA_BYTE_5(0x11) | CAN_WORD1_DATA_BYTE_6(0x11) |
                        CAN_WORD1_DATA_BYTE_7(0x11);

    txXfer.frame = &txFrame;
    txXfer.mbIdx = TX_MESSAGE_BUFFER_NUM;
    FLEXCAN_SendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
    while (!txComplete)
    {
    }
    txComplete = false;
    PRINTF("Send Msg1 to Rx FIFO: word0 = 0x%x, word1 = 0x%x.\r\n", txFrame.dataWord0, txFrame.dataWord1);

    /* Send second message through Tx Message Buffer. */
    txFrame.dataWord0 = CAN_WORD0_DATA_BYTE_0(0x22) | CAN_WORD0_DATA_BYTE_1(0x22) | CAN_WORD0_DATA_BYTE_2(0x22) |
                        CAN_WORD0_DATA_BYTE_3(0x22);
    txFrame.dataWord1 = CAN_WORD1_DATA_BYTE_4(0x22) | CAN_WORD1_DATA_BYTE_5(0x22) | CAN_WORD1_DATA_BYTE_6(0x22) |
                        CAN_WORD1_DATA_BYTE_7(0x22);
    FLEXCAN_SendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
    while (!txComplete)
    {
    }
    txComplete = false;
    PRINTF("Send Msg2 to Rx FIFO: word0 = 0x%x, word1 = 0x%x.\r\n", txFrame.dataWord0, txFrame.dataWord1);

    /* Send third message through Tx Message Buffer. */
    txXfer.frame = &txFrame;
    txXfer.mbIdx = TX_MESSAGE_BUFFER_NUM;
    txFrame.dataWord0 = CAN_WORD0_DATA_BYTE_0(0x33) | CAN_WORD0_DATA_BYTE_1(0x33) | CAN_WORD0_DATA_BYTE_2(0x33) |
                        CAN_WORD0_DATA_BYTE_3(0x33);
    txFrame.dataWord1 = CAN_WORD1_DATA_BYTE_4(0x33) | CAN_WORD1_DATA_BYTE_5(0x33) | CAN_WORD1_DATA_BYTE_6(0x33) |
                        CAN_WORD1_DATA_BYTE_7(0x33);
    FLEXCAN_SendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
    while (!txComplete)
    {
    }
    txComplete = false;
    PRINTF("Send Msg3 to Rx FIFO: word0 = 0x%x, word1 = 0x%x.\r\n", txFrame.dataWord0, txFrame.dataWord1);

    /* Send fourth message through Tx Message Buffer. */
    txXfer.frame = &txFrame;
    txXfer.mbIdx = TX_MESSAGE_BUFFER_NUM;
    txFrame.dataWord0 = CAN_WORD0_DATA_BYTE_0(0x44) | CAN_WORD0_DATA_BYTE_1(0x44) | CAN_WORD0_DATA_BYTE_2(0x44) |
                        CAN_WORD0_DATA_BYTE_3(0x44);
    txFrame.dataWord1 = CAN_WORD1_DATA_BYTE_4(0x44) | CAN_WORD1_DATA_BYTE_5(0x44) | CAN_WORD1_DATA_BYTE_6(0x44) |
                        CAN_WORD1_DATA_BYTE_7(0x44);
    FLEXCAN_SendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
    while (!txComplete)
    {
    }
    txComplete = false;
    PRINTF("Send Msg4 to Rx FIFO: word0 = 0x%x, word1 = 0x%x.\r\n\r\n", txFrame.dataWord0, txFrame.dataWord1);

    /* Receive data through Rx FIFO. */
    rxFifoXfer.frame = &rxFrame;
    for (i = 0; i < 4; i++)
    {
        FLEXCAN_ReceiveFifoEDMA(EXAMPLE_CAN, &flexcanEdmaHandle, &rxFifoXfer);
        while (!rxComplete)
        {
        }
        rxComplete = false;

        PRINTF("Receive Msg%d from FIFO: word0 = 0x%x, word1 = 0x%x, ID Filter Hit%d.\r\n", i + 1, rxFrame.dataWord0,
               rxFrame.dataWord1, rxFrame.idhit);
    }

    PRINTF("\r\n==FlexCAN loopback EDMA example -- Finish.==\r\n");

    while (1)
    {
        __WFI();
    }
}
