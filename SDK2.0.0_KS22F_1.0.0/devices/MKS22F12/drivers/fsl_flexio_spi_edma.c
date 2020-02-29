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

#include "fsl_flexio_spi_edma.h"

/*******************************************************************************
 * Definitons
 ******************************************************************************/
/*<! Structure definition for spi_edma_private_handle_t. The structure is private. */
typedef struct _flexio_spi_master_edma_private_handle
{
    FLEXIO_SPI_Type *base;
    flexio_spi_master_edma_handle_t *handle;
} flexio_spi_master_edma_private_handle_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief EDMA callback function for FLEXIO SPI send transfer.
 *
 * @param handle EDMA handle pointer.
 * @param param Callback function parameter.
 */
static void FLEXIO_SPI_TxEDMACallback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds);

/*!
 * @brief EDMA callback function for FLEXIO SPI receive transfer.
 *
 * @param handle EDMA handle pointer.
 * @param param Callback function parameter.
 */
static void FLEXIO_SPI_RxEDMACallback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Dummy data used to send */
const uint16_t s_dummyData = FLEXIO_SPI_DUMMYDATA;

/*< @brief user configurable flexio spi handle count. */
#define FLEXIO_SPI_HANDLE_COUNT 2

/*<! Private handle only used for internally. */
flexio_spi_master_edma_private_handle_t s_edmaPrivateHandle[FLEXIO_SPI_HANDLE_COUNT];

/*******************************************************************************
* Code
******************************************************************************/

static void FLEXIO_SPI_TxEDMACallback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    tcds = tcds;
    flexio_spi_master_edma_private_handle_t *spiPrivateHandle = (flexio_spi_master_edma_private_handle_t *)param;

    /* Disable Tx DMA */
    if (transferDone)
    {
        FLEXIO_SPI_EnableDMA(spiPrivateHandle->base, kFLEXIO_SPI_TxDmaEnable, false);

        /* change the state */
        spiPrivateHandle->handle->txInProgress = false;

        /* All finished, call the callback */
        if ((spiPrivateHandle->handle->txInProgress == false) && (spiPrivateHandle->handle->rxInProgress == false))
        {
            if (spiPrivateHandle->handle->callback)
            {
                (spiPrivateHandle->handle->callback)(spiPrivateHandle->base, spiPrivateHandle->handle, kStatus_Success,
                                                     spiPrivateHandle->handle->userData);
            }
        }
    }
}

static void FLEXIO_SPI_RxEDMACallback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    tcds = tcds;
    flexio_spi_master_edma_private_handle_t *spiPrivateHandle = (flexio_spi_master_edma_private_handle_t *)param;

    if (transferDone)
    {
        /* Disable Rx dma */
        FLEXIO_SPI_EnableDMA(spiPrivateHandle->base, kFLEXIO_SPI_RxDmaEnable, false);

        /* change the state */
        spiPrivateHandle->handle->rxInProgress = false;

        /* All finished, call the callback */
        if ((spiPrivateHandle->handle->txInProgress == false) && (spiPrivateHandle->handle->rxInProgress == false))
        {
            if (spiPrivateHandle->handle->callback)
            {
                (spiPrivateHandle->handle->callback)(spiPrivateHandle->base, spiPrivateHandle->handle, kStatus_Success,
                                                     spiPrivateHandle->handle->userData);
            }
        }
    }
}

status_t FLEXIO_SPI_MasterCreateHandleEDMA(FLEXIO_SPI_Type *base,
                                       flexio_spi_master_edma_handle_t *handle,
                                       flexio_spi_master_edma_transfer_callback_t callback,
                                       void *userData,
                                       edma_handle_t *txHandle,
                                       edma_handle_t *rxHandle)
{
    assert(handle);

    uint8_t index = 0;

    /* Find the an empty handle pointer to store the handle. */
    for (index = 0; index < FLEXIO_SPI_HANDLE_COUNT; index++)
    {
        if (s_edmaPrivateHandle[index].base == NULL)
        {
            s_edmaPrivateHandle[index].base = base;
            s_edmaPrivateHandle[index].handle = handle;
            break;
        }
    }

    if(index == FLEXIO_SPI_HANDLE_COUNT)
    {
        return kStatus_OutOfRange;
    }

    /* Set spi base to handle. */
    handle->txHandle = txHandle;
    handle->rxHandle = rxHandle;

    /* Register callback and userData. */
    handle->callback = callback;
    handle->userData = userData;

    /* Set SPI state to idle. */
    handle->txInProgress = false;
    handle->rxInProgress = false;

    /* Install callback for Tx/Rx dma channel. */
    if(handle->txHandle)
    {
        EDMA_SetCallback(handle->txHandle, FLEXIO_SPI_TxEDMACallback, &s_edmaPrivateHandle[index]);
    }
    if(handle->rxHandle)
    {
        EDMA_SetCallback(handle->rxHandle, FLEXIO_SPI_RxEDMACallback, &s_edmaPrivateHandle[index]);
    }

    return kStatus_Success;
}

status_t FLEXIO_SPI_MasterTransferEDMA(FLEXIO_SPI_Type *base,
                                       flexio_spi_master_edma_handle_t *handle,
                                       flexio_spi_transfer_t *xfer)
{
    assert(handle);
    assert(xfer);

    edma_transfer_config_t xferConfig;
    flexio_spi_shift_direction_t direction;
    uint8_t bytesPerFrame;
    uint32_t dataMode = 0;
    uint16_t timerCmp = base->flexioBase->TIMCMP[base->timerIndex[0]];
    
    timerCmp &= 0x00FFU;

    /* Check if the device is busy. */
    if ((handle->txInProgress) || (handle->rxInProgress))
    {
        return kStatus_FLEXIO_SPI_Busy;
    }

    /* Check if input parameter invalid. */
    if (((xfer->txData == NULL) && (xfer->rxData == NULL)) || (xfer->dataSize == 0U))
    {
        return kStatus_InvalidArgument;
    }

    /* Configure the values in handle. */
    switch (xfer->flags)
    {
        case kFLEXIO_SPI_8bitMsb:
            dataMode = (8 * 2 - 1U) << 8U;
            bytesPerFrame = 1;
            direction = kFLEXIO_SPI_MsbFirst;
            break;
        case kFLEXIO_SPI_8bitLsb:
            dataMode = (8 * 2 - 1U) << 8U;
            bytesPerFrame = 1;
            direction = kFLEXIO_SPI_LsbFirst;
            break;
        case kFLEXIO_SPI_16bitMsb:
            dataMode = (16 * 2 - 1U) << 8U;
            bytesPerFrame = 2;
            direction = kFLEXIO_SPI_MsbFirst;
            break;
        case kFLEXIO_SPI_16bitLsb:
            dataMode = (16 * 2 - 1U) << 8U;
            bytesPerFrame = 2;
            direction = kFLEXIO_SPI_LsbFirst;
            break;
        default:
            dataMode = (8 * 2 - 1U) << 8U;
            bytesPerFrame = 1U;
            direction = kFLEXIO_SPI_MsbFirst;
            assert(true);
            break;
    }

    dataMode |= timerCmp;
    
    /* Configure transfer size. */
    base->flexioBase->TIMCMP[base->timerIndex[0]] = dataMode;

    /* Configure tx transfer EDMA. */
    xferConfig.destAddr = FLEXIO_SPI_GetTxDataRegisterAddress(base, direction);
    xferConfig.destOffset = 0;
    if (bytesPerFrame == 1U)
    {
        xferConfig.srcTransferSize = kEDMA_TransferSize1Bytes;
        xferConfig.destTransferSize = kEDMA_TransferSize1Bytes;
        xferConfig.minorLoopBytes = 1;
    }
    else
    {
        xferConfig.srcTransferSize = kEDMA_TransferSize2Bytes;
        xferConfig.destTransferSize = kEDMA_TransferSize2Bytes;
        xferConfig.minorLoopBytes = 2;
    }

    /* Configure DMA channel. */
    if (xfer->txData)
    {
        xferConfig.srcOffset = 1;
        xferConfig.srcAddr = (uint32_t)(xfer->txData);
    }
    else
    {
        /* Disable the source increasement and source set to dummyData. */
        xferConfig.srcOffset = 0;
        xferConfig.srcAddr = (uint32_t)(&s_dummyData);
    }

    xferConfig.majorLoopCounts = (xfer->dataSize / xferConfig.minorLoopBytes);

    if(handle->txHandle)
    {
        EDMA_SubmitTransfer(handle->txHandle, &xferConfig);
    }

    /* Configure tx transfer EDMA. */
    if (xfer->rxData)
    {
        xferConfig.srcAddr = FLEXIO_SPI_GetRxDataRegisterAddress(base, direction);
        xferConfig.srcOffset = 0;
        xferConfig.destAddr = (uint32_t)(xfer->rxData);
        xferConfig.destOffset = 1;
        EDMA_SubmitTransfer(handle->rxHandle, &xferConfig);
        handle->rxInProgress = true;
        FLEXIO_SPI_EnableDMA(base, kFLEXIO_SPI_RxDmaEnable, true);
        EDMA_StartTransfer(handle->rxHandle);
    }

    /* Always start Tx transfer. */
    if(handle->txHandle)
    {
        handle->txInProgress = true;
        FLEXIO_SPI_EnableDMA(base, kFLEXIO_SPI_TxDmaEnable, true);
        EDMA_StartTransfer(handle->txHandle);
    }

    return kStatus_Success;
}

size_t FLEXIO_SPI_MasterGetTransferRemainingBytesEDMA(FLEXIO_SPI_Type *base, flexio_spi_master_edma_handle_t *handle)
{
    assert(handle);

    size_t remainingBytes = 0;

    if (handle->rxInProgress)
    {
        remainingBytes = EDMA_GetRemainingBytes(handle->rxHandle->base, handle->rxHandle->channel);
    }
    else
    {
        remainingBytes = EDMA_GetRemainingBytes(handle->txHandle->base, handle->txHandle->channel);
    }

    return remainingBytes;
}

void FLEXIO_SPI_MasterAbortTransferEDMA(FLEXIO_SPI_Type *base, flexio_spi_master_edma_handle_t *handle)
{
    assert(handle);

    /* Disable dma. */
    EDMA_StopTransfer(handle->txHandle);
    EDMA_StopTransfer(handle->rxHandle);

    /* Disable DMA enable bit. */
    FLEXIO_SPI_EnableDMA(base, kFLEXIO_SPI_DmaAllEnable, false);

    /* Set the handle state. */
    handle->txInProgress = false;
    handle->rxInProgress = false;
}

