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

#include "fsl_flexio_uart_edma.h"
#include "fsl_dmamux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*<! Structure definition for uart_edma_private_handle_t. The structure is private. */
typedef struct _flexio_uart_edma_private_handle
{
    FLEXIO_UART_Type *base;
    flexio_uart_edma_handle_t *handle;
} flexio_uart_edma_private_handle_t;

/* UART EDMA transfer handle. */
enum _flexio_uart_edma_tansfer_states
{
    kFLEXIO_UART_TxIdle, /* TX idle. */
    kFLEXIO_UART_TxBusy, /* TX busy. */
    kFLEXIO_UART_RxIdle, /* RX idle. */
    kFLEXIO_UART_RxBusy  /* RX busy. */
};

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*< @brief user configurable flexio uart handle count. */
#define FLEXIO_UART_HANDLE_COUNT 2

/*<! Private handle only used for internally. */
flexio_uart_edma_private_handle_t s_edmaPrivateHandle[FLEXIO_UART_HANDLE_COUNT];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief FLEXIO UART EDMA send finished callback function.
 *
 * This function is called when FLEXIO UART EDMA send finished. It disables the UART
 * TX EDMA request and sends @ref kStatus_FLEXIO_UART_TxIdle to FLEXIO UART callback.
 *
 * @param handle The EDMA handle.
 * @param param Callback function parameter.
 */
static void FLEXIO_UART_SendEDMACallback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds);

/*!
 * @brief FLEXIO UART EDMA receive finished callback function.
 *
 * This function is called when FLEXIO UART EDMA receive finished. It disables the UART
 * RX EDMA request and sends @ref kStatus_FLEXIO_UART_RxIdle to UART callback.
 *
 * @param handle The EDMA handle.
 * @param param Callback function parameter.
 */
static void FLEXIO_UART_ReceiveEDMACallback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds);

/*******************************************************************************
 * Code
 ******************************************************************************/

static void FLEXIO_UART_SendEDMACallback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    flexio_uart_edma_private_handle_t *uartPrivateHandle = (flexio_uart_edma_private_handle_t *)param;

    /* Avoid the warning for unused variables. */
    handle = handle;
    tcds = tcds;

    if (transferDone)
    {
        FLEXIO_UART_AbortSendEDMA(uartPrivateHandle->base, uartPrivateHandle->handle);

        if (uartPrivateHandle->handle)
        {
            uartPrivateHandle->handle->callback(uartPrivateHandle->base, uartPrivateHandle->handle,
                                                kStatus_FLEXIO_UART_TxIdle, uartPrivateHandle->handle->userData);
        }
    }
}

static void FLEXIO_UART_ReceiveEDMACallback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    flexio_uart_edma_private_handle_t *uartPrivateHandle = (flexio_uart_edma_private_handle_t *)param;

    /* Avoid the warning for unused variables. */
    handle = handle;
    tcds = tcds;

    if (transferDone)
    {
        /* Disable transfer. */
        FLEXIO_UART_AbortReceiveEDMA(uartPrivateHandle->base, uartPrivateHandle->handle);

        if (uartPrivateHandle->handle->callback)
        {
            uartPrivateHandle->handle->callback(uartPrivateHandle->base, uartPrivateHandle->handle,
                                                kStatus_FLEXIO_UART_RxIdle, uartPrivateHandle->handle->userData);
        }
    }
}

void FLEXIO_UART_CreateHandleEDMA(FLEXIO_UART_Type *base,
                                  flexio_uart_edma_handle_t *handle,
                                  flexio_uart_edma_transfer_callback_t callback,
                                  void *userData,
                                  edma_handle_t *txEdmaHandle,
                                  edma_handle_t *rxEdmaHandle)
{
    assert(handle);

    edma_transfer_config_t dmaXferConfig;
    uint8_t index = 0;

    /* Find the an empty handle pointer to store the handle. */
    while ((s_edmaPrivateHandle[index].base != NULL) && (index < FLEXIO_UART_HANDLE_COUNT))
    {
        index++;
    }

    s_edmaPrivateHandle[index].base = base;
    s_edmaPrivateHandle[index].handle = handle;

    memset(handle, 0, sizeof(*handle));

    handle->rxState = kFLEXIO_UART_RxIdle;
    handle->txState = kFLEXIO_UART_TxIdle;

    handle->rxEdmaHandle = rxEdmaHandle;
    handle->txEdmaHandle = txEdmaHandle;

    handle->callback = callback;
    handle->userData = userData;

    /* Configure TX. */
    if (txEdmaHandle)
    {
        EDMA_SetCallback(handle->txEdmaHandle, FLEXIO_UART_SendEDMACallback, &s_edmaPrivateHandle);
    }

    /* Configure RX. */
    if (rxEdmaHandle)
    {
        EDMA_SetCallback(handle->rxEdmaHandle, FLEXIO_UART_ReceiveEDMACallback, &s_edmaPrivateHandle);
    }
}

status_t FLEXIO_UART_SendEDMA(FLEXIO_UART_Type *base, flexio_uart_edma_handle_t *handle, flexio_uart_transfer_t *xfer)
{
    assert(handle->txEdmaHandle);

    edma_transfer_config_t xferConfig;
    status_t status;

    /* If previous TX not finished. */
    if (kFLEXIO_UART_TxBusy == handle->txState)
    {
        status = kStatus_FLEXIO_UART_TxBusy;
    }
    else
    {
        handle->txState = kFLEXIO_UART_TxBusy;

        /* Prepare transfer. */
        EDMA_PrepareTransfer(&xferConfig, xfer->data, sizeof(uint8_t),
                             (void *)FLEXIO_UART_GetTxDataRegisterAddress(base), sizeof(uint8_t), sizeof(uint8_t),
                             xfer->dataSize, kEDMA_MemoryToPeripheral);

        /* Submit transfer. */
        EDMA_SubmitTransfer(handle->txEdmaHandle, &xferConfig);
        EDMA_StartTransfer(handle->txEdmaHandle);

        /* Enable UART TX EDMA. */
        FLEXIO_UART_EnableTxDMA(base, true);

        status = kStatus_Success;
    }

    return status;
}

status_t FLEXIO_UART_ReceiveEDMA(FLEXIO_UART_Type *base,
                                 flexio_uart_edma_handle_t *handle,
                                 flexio_uart_transfer_t *xfer)
{
    assert(handle->rxEdmaHandle);

    edma_transfer_config_t xferConfig;
    status_t status;

    /* If previous RX not finished. */
    if (kFLEXIO_UART_RxBusy == handle->rxState)
    {
        status = kStatus_FLEXIO_UART_RxBusy;
    }
    else
    {
        handle->rxState = kFLEXIO_UART_RxBusy;

        /* Prepare transfer. */
        EDMA_PrepareTransfer(&xferConfig, (void *)FLEXIO_UART_GetRxDataRegisterAddress(base), sizeof(uint8_t),
                             xfer->data, sizeof(uint8_t), sizeof(uint8_t), xfer->dataSize, kEDMA_PeripheralToMemory);

        /* Submit transfer. */
        EDMA_SubmitTransfer(handle->rxEdmaHandle, &xferConfig);
        EDMA_StartTransfer(handle->rxEdmaHandle);

        /* Enable UART RX EDMA. */
        FLEXIO_UART_EnableRxDMA(base, true);

        status = kStatus_Success;
    }

    return status;
}

void FLEXIO_UART_AbortSendEDMA(FLEXIO_UART_Type *base, flexio_uart_edma_handle_t *handle)
{
    assert(handle->txEdmaHandle);

    /* Disable UART TX EDMA. */
    FLEXIO_UART_EnableTxDMA(base, false);

    /* Stop transfer. */
    EDMA_StopTransfer(handle->txEdmaHandle);

    handle->txState = kFLEXIO_UART_TxIdle;
}

void FLEXIO_UART_AbortReceiveEDMA(FLEXIO_UART_Type *base, flexio_uart_edma_handle_t *handle)
{
    assert(handle->rxEdmaHandle);

    /* Disable UART RX EDMA. */
    FLEXIO_UART_EnableRxDMA(base, false);

    /* Stop transfer. */
    EDMA_StopTransfer(handle->rxEdmaHandle);

    handle->rxState = kFLEXIO_UART_RxIdle;
}

size_t FLEXIO_UART_GetReceiveRemainingBytesEDMA(FLEXIO_UART_Type *base, flexio_uart_edma_handle_t *handle)
{
    assert(handle->rxEdmaHandle);

    size_t bytes;

    if (kFLEXIO_UART_RxBusy == handle->rxState)
    {
        bytes = EDMA_GetRemainingBytes(handle->rxEdmaHandle->base, handle->rxEdmaHandle->channel);
    }
    else
    {
        bytes = 0;
    }

    return bytes;
}

size_t FLEXIO_UART_GetSendRemainingBytesEDMA(FLEXIO_UART_Type *base, flexio_uart_edma_handle_t *handle)
{
    assert(handle->txEdmaHandle);

    size_t bytes;

    if (kFLEXIO_UART_TxBusy == handle->txState)
    {
        bytes = EDMA_GetRemainingBytes(handle->txEdmaHandle->base, handle->txEdmaHandle->channel);
    }
    else
    {
        bytes = 0;
    }

    return bytes;
}
