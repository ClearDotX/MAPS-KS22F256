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

#include "fsl_debug_console.h"
#include "fsl_flexcan.h"
#include "board.h"

#include "fsl_device_registers.h"
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_CAN             CAN0
#define EXAMPLE_CAN_CLKSRC      kCLOCK_BusClk
#define RX_MESSAGE_BUFFER_NUM (8)
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
flexcan_mb_transfer_t txXfer, rxXfer;
flexcan_frame_t txFrame, rxFrame;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief FlexCAN Call Back function
 */
static void flexcan_callback(CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *userData)
{
    switch (status)
    {
        /* Process FlexCAN Rx event. */
        case kStatus_FLEXCAN_RxIdle:
            if (RX_MESSAGE_BUFFER_NUM == result)
            {
                rxComplete = true;
            }
            break;

        /* Process FlexCAN Tx event. */
        case kStatus_FLEXCAN_TxIdle:
            if (TX_MESSAGE_BUFFER_NUM == result)
            {
                txComplete = true;
            }
            break;

        default:
            break;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    flexcan_config_t flexcanConfig;
    flexcan_rx_mb_config_t mbConfig;

    /* Initialize board hardware. */
    BOARD_InitPins();
    BOARD_BootClockHSRUN();

    BOARD_InitDebugConsole();

    PRINTF("\r\n==FlexCAN loopback example -- Start.==\r\n\r\n");

    /* Init FlexCAN module. */
    FLEXCAN_GetDefaultConfig(&flexcanConfig);
    flexcanConfig.clkSrc = KFLEXCAN_ClkSrcPeri;
    flexcanConfig.enableLoopBack = true;
    FLEXCAN_Init(EXAMPLE_CAN, &flexcanConfig, CLOCK_GetFreq(EXAMPLE_CAN_CLKSRC));

    /* Enable FlexCAN module. */
    FLEXCAN_Enable(EXAMPLE_CAN, true);

    /* Setup Rx Message Buffer. */
    mbConfig.format = KFLEXCAN_FrameFormatStandard;
    mbConfig.type = KFLEXCAN_FrameTypeData;
    mbConfig.id = CAN_ID_STD(0x123);
    FLEXCAN_SetRxMbConfig(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &mbConfig);

    /* Setup Tx Message Buffer. */
    FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM);

    /* Create FlexCAN handle structure and set call back function. */
    FLEXCAN_CreateHandle(EXAMPLE_CAN, &flexcanHandle, flexcan_callback, NULL);

    /* Start receive data through Rx Message Buffer. */
    rxXfer.frame = &rxFrame;
    rxXfer.mbIdx = RX_MESSAGE_BUFFER_NUM;
    FLEXCAN_ReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);

    /* Prepare Tx Frame for sending. */
    txFrame.format = KFLEXCAN_FrameFormatStandard;
    txFrame.type = KFLEXCAN_FrameTypeData;
    txFrame.id = CAN_ID_STD(0x123);
    txFrame.length = 8;
    txFrame.dataWord0 = CAN_WORD0_DATA_BYTE_0(0x11) | CAN_WORD0_DATA_BYTE_1(0x22) | CAN_WORD0_DATA_BYTE_2(0x33) |
                        CAN_WORD0_DATA_BYTE_3(0x44);
    txFrame.dataWord1 = CAN_WORD1_DATA_BYTE_4(0x55) | CAN_WORD1_DATA_BYTE_5(0x66) | CAN_WORD1_DATA_BYTE_6(0x77) |
                        CAN_WORD1_DATA_BYTE_7(0x88);

    PRINTF("Send message from MB%d to MB%d\r\n", TX_MESSAGE_BUFFER_NUM, RX_MESSAGE_BUFFER_NUM);
    PRINTF("tx word0 = 0x%x\r\n", txFrame.dataWord0);
    PRINTF("tx word1 = 0x%x\r\n", txFrame.dataWord1);

    /* Send data through Tx Message Buffer. */
    txXfer.frame = &txFrame;
    txXfer.mbIdx = TX_MESSAGE_BUFFER_NUM;
    FLEXCAN_SendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);

    /* Waiting for Rx Message finish. */
    while ((!rxComplete) || (!txComplete))
    {
    };

    PRINTF("\r\nReceved message from MB%d\r\n", RX_MESSAGE_BUFFER_NUM);
    PRINTF("rx word0 = 0x%x\r\n", rxFrame.dataWord0);
    PRINTF("rx word1 = 0x%x\r\n", rxFrame.dataWord1);

    /* Stop FlexCAN Send & Receive. */
    FLEXCAN_AbortReceive(EXAMPLE_CAN, &flexcanHandle, RX_MESSAGE_BUFFER_NUM);
    FLEXCAN_AbortSend(EXAMPLE_CAN, &flexcanHandle, TX_MESSAGE_BUFFER_NUM);

    PRINTF("\r\n==FlexCAN loopback example -- Finish.==\r\n");

    while (1)
    {
        __WFI();
    }
}
