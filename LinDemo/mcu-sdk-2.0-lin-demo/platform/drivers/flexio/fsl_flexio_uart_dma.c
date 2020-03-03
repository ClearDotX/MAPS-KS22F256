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

#include "fsl_flexio_uart_dma.h"
#include "fsl_dmamux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*<! @brief Structure definition for DMA callback param pass in. */
typedef struct _flexio_uart_dma_private_handle
{
    FLEXIO_UART_Type *base;
    flexio_uart_dma_handle_t *handle;
} flexio_uart_dma_private_handle_t;

/*<! @brief uart transfer state. */
enum _flexio_uart_dma_transfer_states
{
    kFLEXIO_UART_TxIdle, /* TX idle. */
    kFLEXIO_UART_TxBusy, /* TX busy. */
    kFLEXIO_UART_RxIdle, /* RX idle. */
    kFLEXIO_UART_RxBusy  /* RX busy. */
};

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*< @brief user configurable flexio uart handle count. */
#define FLEXIO_UART_HANDLE_COUNT 2

/*<! Private handle only used for internally. */
flexio_uart_dma_private_handle_t s_dmaPrivateHandle[FLEXIO_UART_HANDLE_COUNT];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief FLEXIO UART DMA send finished callback function.
 *
 * This function is called when FLEXIO UART DMA send finished. It disables the UART
 * TX DMA request and sends @ref kStatus_FLEXIO_UART_TxIdle to FLEXIO UART callback.
 *
 * @param handle The DMA handle.
 * @param param Callback function parameter.
 */
static void FLEXIO_UART_SendDMACallback(dma_handle_t *handle, void *param);

/*!
 * @brief FLEXIO UART DMA receive finished callback function.
 *
 * This function is called when FLEXIO UART DMA receive finished. It disables the FLEXIO
 * UART RX DMA request and sends @ref kStatus_FLEXIO_UART_RxIdle to UART callback.
 *
 * @param handle The DMA handle.
 * @param param Callback function parameter.
 */
static void FLEXIO_UART_ReceiveDMACallback(dma_handle_t *handle, void *param);

/*******************************************************************************
 * Code
 ******************************************************************************/

static void FLEXIO_UART_SendDMACallback(dma_handle_t *handle, void *param)
{
    flexio_uart_dma_private_handle_t *uartPrivateHandle = (flexio_uart_dma_private_handle_t *)param;

    /* Disable UART TX DMA. */
    FLEXIO_UART_EnableTxDMA(uartPrivateHandle->base, false);

    /* Disable interrupt. */
    DMA_DisableInterrupts(handle->base, handle->channel);

    uartPrivateHandle->handle->txState = kFLEXIO_UART_TxIdle;

    if (uartPrivateHandle->handle->callback)
    {
        uartPrivateHandle->handle->callback(uartPrivateHandle->base, uartPrivateHandle->handle,
                                            kStatus_FLEXIO_UART_TxIdle, uartPrivateHandle->handle->userData);
    }
}

static void FLEXIO_UART_ReceiveDMACallback(dma_handle_t *handle, void *param)
{
    flexio_uart_dma_private_handle_t *uartPrivateHandle = (flexio_uart_dma_private_handle_t *)param;

    /* Disable UART RX DMA. */
    FLEXIO_UART_EnableRxDMA(uartPrivateHandle->base, false);

    /* Disable interrupt. */
    DMA_DisableInterrupts(handle->base, handle->channel);

    uartPrivateHandle->handle->rxState = kFLEXIO_UART_RxIdle;

    if (uartPrivateHandle->handle->callback)
    {
        uartPrivateHandle->handle->callback(uartPrivateHandle->base, uartPrivateHandle->handle,
                                            kStatus_FLEXIO_UART_RxIdle, uartPrivateHandle->handle->userData);
    }
}

void FLEXIO_UART_CreateHandleDMA(FLEXIO_UART_Type *base,
                                 flexio_uart_dma_handle_t *handle,
                                 flexio_uart_dma_transfer_callback_t callback,
                                 void *userData,
                                 dma_handle_t *txDmaHandle,
                                 dma_handle_t *rxDmaHandle)
{
    assert(handle);

    dma_transfer_config_t dmaXferConfig;
    uint8_t index = 0;

    /* Find the an empty handle pointer to store the handle. */
    while ((s_dmaPrivateHandle[index].handle != NULL) && (index < FLEXIO_UART_HANDLE_COUNT))
    {
        index++;
    }

    memset(handle, 0, sizeof(*handle));

    s_dmaPrivateHandle[index].base = base;
    s_dmaPrivateHandle[index].handle = handle;

    handle->rxState = kFLEXIO_UART_RxIdle;
    handle->txState = kFLEXIO_UART_TxIdle;

    handle->callback = callback;
    handle->userData = userData;

    handle->rxDmaHandle = rxDmaHandle;
    handle->txDmaHandle = txDmaHandle;

    /* Set DMA channel configuration. */
    memset(&dmaXferConfig, 0, sizeof(dmaXferConfig));
    dmaXferConfig.srcSize = kDMA_Transfersize8bits;
    dmaXferConfig.destSize = kDMA_Transfersize8bits;

    /* Configure TX. */
    if (txDmaHandle)
    {
        DMA_SetCallback(txDmaHandle, FLEXIO_UART_SendDMACallback, &s_dmaPrivateHandle[index]);

        DMA_ResetChannel(txDmaHandle->base, txDmaHandle->channel);

        dmaXferConfig.destAddr = FLEXIO_UART_GetTxDataRegisterAddress(base);
        dmaXferConfig.enableSrcIncrement = true;
        dmaXferConfig.enableDestIncrement = false;
        DMA_SetTransferConfig(txDmaHandle->base, txDmaHandle->channel, &dmaXferConfig);
    }

    /* Configure RX. */
    if (rxDmaHandle)
    {
        DMA_SetCallback(rxDmaHandle, FLEXIO_UART_ReceiveDMACallback, &s_dmaPrivateHandle[index]);

        DMA_ResetChannel(rxDmaHandle->base, rxDmaHandle->channel);

        dmaXferConfig.destAddr = 0U;
        dmaXferConfig.srcAddr = FLEXIO_UART_GetRxDataRegisterAddress(base);
        dmaXferConfig.enableSrcIncrement = false;
        dmaXferConfig.enableDestIncrement = true;
        DMA_SetTransferConfig(rxDmaHandle->base, rxDmaHandle->channel, &dmaXferConfig);
    }
}

status_t FLEXIO_UART_SendDMA(FLEXIO_UART_Type *base, flexio_uart_dma_handle_t *handle, flexio_uart_transfer_t *xfer)
{
    assert(handle->txDmaHandle);

    status_t status;

    /* If previous TX not finished. */
    if (kFLEXIO_UART_TxBusy == handle->txState)
    {
        status = kStatus_FLEXIO_UART_TxBusy;
    }
    else
    {
        handle->txState = kFLEXIO_UART_TxBusy;

        /* Set transfer data address and data size. */
        DMA_SetSourceAddress(handle->txDmaHandle->base, handle->txDmaHandle->channel, (uint32_t)xfer->data);
        DMA_SetTransferSize(handle->txDmaHandle->base, handle->txDmaHandle->channel, xfer->dataSize);

        /* Enable FLEXIO UART TX DMA. */
        FLEXIO_UART_EnableTxDMA(base, true);

        /* Enable DMA transfer complete interrupt and start transfer. */
        DMA_EnableInterrupts(handle->txDmaHandle->base, handle->txDmaHandle->channel);
        DMA_EnableChannelRequest(handle->txDmaHandle->base, handle->txDmaHandle->channel);

        status = kStatus_Success;
    }

    return status;
}

status_t FLEXIO_UART_ReceiveDMA(FLEXIO_UART_Type *base, flexio_uart_dma_handle_t *handle, flexio_uart_transfer_t *xfer)
{
    assert(handle->rxDmaHandle);

    status_t status;

    /* If previous RX not finished. */
    if (kFLEXIO_UART_RxBusy == handle->rxState)
    {
        status = kStatus_FLEXIO_UART_RxBusy;
    }
    else
    {
        handle->rxState = kFLEXIO_UART_RxBusy;

        /* Set transfer data address and data size. */
        DMA_SetDestinationAddress(handle->rxDmaHandle->base, handle->rxDmaHandle->channel, (uint32_t)xfer->data);
        DMA_SetTransferSize(handle->rxDmaHandle->base, handle->rxDmaHandle->channel, xfer->dataSize);

        /* Enable FLEXIO UART RX DMA. */
        FLEXIO_UART_EnableRxDMA(base, true);

        /* Enable DMA transfer complete interrupt and start transfer. */
        DMA_EnableInterrupts(handle->rxDmaHandle->base, handle->rxDmaHandle->channel);
        DMA_EnableChannelRequest(handle->rxDmaHandle->base, handle->rxDmaHandle->channel);

        status = kStatus_Success;
    }

    return status;
}

void FLEXIO_UART_AbortSendDMA(FLEXIO_UART_Type *base, flexio_uart_dma_handle_t *handle)
{
    assert(handle->txDmaHandle);

    /* Disable FLEXIO UART TX DMA. */
    FLEXIO_UART_EnableTxDMA(base, false);

    /* Stop transfer. */
    DMA_StopTransfer(handle->txDmaHandle);

    /* Write DMA->DSR[DONE] to abort transfer and clear status. */
    DMA_ClearChannelStatusFlags(handle->txDmaHandle->base, handle->txDmaHandle->channel, kDMA_TransactionsDoneFlag);

    handle->txState = kFLEXIO_UART_TxIdle;
}

void FLEXIO_UART_AbortReceiveDMA(FLEXIO_UART_Type *base, flexio_uart_dma_handle_t *handle)
{
    assert(handle->rxDmaHandle);

    /* Disable FLEXIO UART RX DMA. */
    FLEXIO_UART_EnableRxDMA(base, false);

    /* Stop transfer. */
    DMA_StopTransfer(handle->rxDmaHandle);

    /* Write DMA->DSR[DONE] to abort transfer and clear status. */
    DMA_ClearChannelStatusFlags(handle->rxDmaHandle->base, handle->rxDmaHandle->channel, kDMA_TransactionsDoneFlag);

    handle->rxState = kFLEXIO_UART_RxIdle;
}

size_t FLEXIO_UART_GetSendRemainingBytesDMA(FLEXIO_UART_Type *base, flexio_uart_dma_handle_t *handle)
{
    assert(handle->txDmaHandle);

    size_t bytes;

    if (kFLEXIO_UART_TxBusy == handle->txState)
    {
        bytes = DMA_GetRemainingBytes(handle->txDmaHandle->base, handle->txDmaHandle->channel);
    }
    else
    {
        bytes = 0;
    }

    return bytes;
}

size_t FLEXIO_UART_GetReceiveRemainingBytesDMA(FLEXIO_UART_Type *base, flexio_uart_dma_handle_t *handle)
{
    assert(handle->rxDmaHandle);

    size_t bytes;

    if (kFLEXIO_UART_RxBusy == handle->rxState)
    {
        bytes = DMA_GetRemainingBytes(handle->rxDmaHandle->base, handle->rxDmaHandle->channel);
    }
    else
    {
        bytes = 0;
    }

    return bytes;
}
