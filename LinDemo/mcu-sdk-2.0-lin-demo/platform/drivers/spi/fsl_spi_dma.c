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

#include "fsl_spi_dma.h"

/*******************************************************************************
 * Definitons
 ******************************************************************************/
/*! @brief SPI transfer state, which is used for SPI transactiaonl APIs' internal state. */
enum _spi_dma_states_t
{
    kSPI_Idle = 0x0, /*!< SPI is idle state */
    kSPI_Busy        /*!< SPI is busy tranferring data. */
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Get the instance for SPI module.
 *
 * @param base SPI base address
 */
static uint32_t SPI_GetInstance(SPI_Type *base);

/*!
 * @brief DMA callback function for SPI send transfer.
 *
 * @param handle DMA handle pointer.
 * @param userData User data for DMA callback function.
 */
static void SPI_TxDMACallback(dma_handle_t *handle, void *userData);

/*!
 * @brief DMA callback function for SPI receive transfer.
 *
 * @param handle DMA handle pointer.
 * @param userData User data for DMA callback function.
 */
static void SPI_RxDMACallback(dma_handle_t *handle, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Base pointer array */
static SPI_Type *const s_spiBase[] = {SPI0, SPI1};

/* Dummy data used to send */
const uint8_t s_dummyData = SPI_DUMMYDATA;

/*******************************************************************************
* Code
******************************************************************************/
static uint32_t SPI_GetInstance(SPI_Type *base)
{
    uint8_t instance = 0;

    while ((s_spiBase[instance] != base) && (instance < FSL_FEATURE_SOC_SPI_COUNT))
    {
        instance++;
    }

    assert(instance < FSL_FEATURE_SOC_SPI_COUNT);

    return instance;
}

static void SPI_TxDMACallback(dma_handle_t *handle, void *userData)
{
    spi_dma_handle_t *spiHandle = (spi_dma_handle_t *)userData;

    /* Disable Tx dma */
    SPI_EnableDMA(spiHandle->base, kSPI_TxDmaEnable, false);

    /* Stop DMA tranfer */
    DMA_StopTransfer(spiHandle->txHandle);

    /* change the state */
    spiHandle->txInProgress = false;

    /* All finished, call the callback */
    if ((spiHandle->txInProgress == false) && (spiHandle->rxInProgress == false))
    {
        spiHandle->state = kSPI_Idle;
        if (spiHandle->callback)
        {
            (spiHandle->callback)(spiHandle, kStatus_Success, spiHandle->userData);
        }
    }
}

static void SPI_RxDMACallback(dma_handle_t *handle, void *userData)
{
    spi_dma_handle_t *spiHandle = (spi_dma_handle_t *)userData;

    /* Disable Tx dma */
    SPI_EnableDMA(spiHandle->base, kSPI_RxDmaEnable, false);

    /* Stop DMA tranfer */
    DMA_StopTransfer(spiHandle->rxHandle);

    /* change the state */
    spiHandle->rxInProgress = false;

    /* All finished, call the callback */
    if ((spiHandle->txInProgress == false) && (spiHandle->rxInProgress == false))
    {
        spiHandle->state = kSPI_Idle;
        if (spiHandle->callback)
        {
            (spiHandle->callback)(spiHandle, kStatus_Success, spiHandle->userData);
        }
    }
}

void SPI_MasterCreateHandleDMA(
    spi_dma_handle_t *handle, SPI_Type *base, dma_handle_t *txHandle, dma_handle_t *rxHandle)
{
    assert(handle);

    uint8_t fifoSize[FSL_FEATURE_SOC_SPI_COUNT] = FSL_FEATURE_SPI_FIFO_SIZEx;
    dma_transfer_config_t config = {0};

    /* Set spi base to handle */
    handle->base = base;
    handle->txHandle = txHandle;
    handle->rxHandle = rxHandle;

    /* Set SPI state to idle */
    handle->state = kSPI_Idle;

/* Compute internal state */
#if defined(FSL_FEATURE_SPI_16BIT_TRANSFERS) && (FSL_FEATURE_SPI_16BIT_TRANSFERS)
    handle->bytesPerFrame = ((base->C2 & SPI_C2_SPIMODE_MASK) >> SPI_C2_SPIMODE_SHIFT) + 1U;
#else
    handle->bytesPerFrame = 1U;
#endif /* FSL_FEATURE_SPI_16BIT_TRANSFERS */

#if defined(FSL_FEATURE_SPI_FIFO_SIZE) && (FSL_FEATURE_SPI_FIFO_SIZE > 1)
    /* If using DMA, disable FIFO, as the FIFO may cause data loss if the data size is not integer
       times of 2bytes. As SPI cannot set watermark to 0, only can set to 1/2 FIFO size or 3/4 FIFO
       size. */
    if (fifoSize[SPI_GetInstance(base)] != 0)
    {
        base->C3 &= ~SPI_C3_FIFOMODE_MASK;
    }

#endif /* FSL_FEATURE_SPI_FIFO_SIZE */

    /* Set the non-change attribute for Tx DMA transfer, to improve efficiency */
    config.destAddr = SPI_GetDataRegAddr(base);
    config.enableDestIncrement = false;
    config.enableSrcIncrement = true;
    if (handle->bytesPerFrame == 1U)
    {
        config.srcSize = kDMA_Transfersize8bits;
        config.destSize = kDMA_Transfersize8bits;
    }
    else
    {
        config.srcSize = kDMA_Transfersize16bits;
        config.destSize = kDMA_Transfersize16bits; 
    }

    DMA_SubmitTransfer(handle->txHandle, &config, true);

    /* Set non-change attribute for Rx DMA */
    config.srcAddr = SPI_GetDataRegAddr(base);
    config.destAddr = 0U;
    config.enableDestIncrement = true;
    config.enableSrcIncrement = false;
    DMA_SubmitTransfer(handle->rxHandle, &config, true);

    /* Install callback for Tx dma channel */
    DMA_SetCallback(handle->txHandle, SPI_TxDMACallback, handle);
    DMA_SetCallback(handle->rxHandle, SPI_RxDMACallback, handle);
}

status_t SPI_MasterTransferDMA(spi_dma_handle_t *handle, spi_transfer_t *xfer)
{
    assert(handle && xfer);

    dma_transfer_config_t config = {0};

    /* Check if the device is busy */
    if (handle->state == kSPI_Busy)
    {
        return kStatus_SPI_Busy;
    }

    /* Check if input parameter invalid */
    if (((xfer->sendBuff == NULL) && (xfer->receiveBuff == NULL)) || (xfer->size == 0U))
    {
        return kStatus_InvalidArgument;
    }

    /* Configure tx transfer DMA */
    config.destAddr = SPI_GetDataRegAddr(handle->base);
    config.enableDestIncrement = false;
    if (handle->bytesPerFrame == 1U)
    {
        config.srcSize = kDMA_Transfersize8bits;
        config.destSize = kDMA_Transfersize8bits;
    }
    else
    {
        config.srcSize = kDMA_Transfersize16bits;
        config.destSize = kDMA_Transfersize16bits; 
    }
    config.transferSize = xfer->size;
    /* Configure DMA channel */
    if (xfer->sendBuff)
    {
        config.enableSrcIncrement = true;
        config.srcAddr = (uint32_t)(xfer->sendBuff);
    }
    else
    {
        /* Disable the source increasement and source set to dummyData */
        config.enableSrcIncrement = false;
        config.srcAddr = (uint32_t)(&s_dummyData);
    }
    DMA_SubmitTransfer(handle->txHandle, &config, true);

    /* Handle rx transfer */
    if (xfer->receiveBuff)
    {
        /* Set the source address */
        DMA_SetDestinationAddress(handle->rxHandle->base, handle->rxHandle->channel, (uint32_t)(xfer->receiveBuff));

        /* Set the transfer size */
        DMA_SetTransferSize(handle->rxHandle->base, handle->rxHandle->channel, xfer->size);
    }

    /* Change the state of handle */
    handle->state = kSPI_Busy;

    /* Start Rx transfer if needed */
    if (xfer->receiveBuff)
    {
        handle->rxInProgress = true;
        SPI_EnableDMA(handle->base, kSPI_RxDmaEnable, true);
        DMA_StartTransfer(handle->rxHandle);
    }

    /* Always start Tx transfer */
    handle->txInProgress = true;
    SPI_EnableDMA(handle->base, kSPI_TxDmaEnable, true);
    DMA_StartTransfer(handle->txHandle);

    return kStatus_Success;
}

size_t SPI_MasterGetRemainingBytesDMA(spi_dma_handle_t *handle)
{
    assert(handle);
    
    size_t remainingBytes = 0;
    
    if (handle->rxInProgress)
    {
        remainingBytes = DMA_GetRemainingBytes(handle->rxHandle->base, handle->rxHandle->channel);
    }
    else
    {
        remainingBytes = DMA_GetRemainingBytes(handle->txHandle->base, handle->txHandle->channel);
    }
    
    return remainingBytes;
}

void SPI_MasterAbortTransferDMA(spi_dma_handle_t *handle)
{
    assert(handle);

    /* Disable dma */
    DMA_StopTransfer(handle->txHandle);
    DMA_StopTransfer(handle->rxHandle);

    /* Disable DMA enable bit */
    SPI_EnableDMA(handle->base, kSPI_DmaAllEnable, false);

    /* Set the handle state */
    handle->txInProgress = false;
    handle->rxInProgress = false;
    handle->state = kSPI_Idle;
}

void SPI_SetTransferCallbackDMA(spi_dma_handle_t *handle, spi_dma_callback_t callback, void *userData)
{
    assert(handle);

    handle->callback = callback;
    handle->userData = userData;
}
