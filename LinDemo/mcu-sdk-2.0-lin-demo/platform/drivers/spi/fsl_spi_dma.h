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
#ifndef _FSL_SPI_DMA_H_
#define _FSL_SPI_DMA_H_

#include "fsl_spi.h"
#include "fsl_dma.h"

/*!
 * @addtogroup spi
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct _spi_dma_handle spi_dma_handle_t;

typedef void (*spi_dma_callback_t)(spi_dma_handle_t *handle, status_t status, void *userData);

/*! @brief SPI DMA transfer handle, users should not touch the content of the handle.*/
struct _spi_dma_handle
{
    SPI_Type *base;              /*!< SPI base pointer */
    bool txInProgress;           /*!< Send transfer finished */
    bool rxInProgress;           /*!< Receive transfer finished */
    dma_handle_t *txHandle;      /*!< DMA handler for SPI send */
    dma_handle_t *rxHandle;      /*!< DMA handler for SPI receive */
    uint8_t bytesPerFrame;       /*!< Bytes in a frame for SPI tranfer */
    spi_dma_callback_t callback; /*!< Callback for SPI DMA transfer */
    void *userData;              /*!< User Data for SPI DMA callback */
    uint32_t state;              /*!< Internal state of SPI DMA transfer */
};

/*******************************************************************************
 * APIs
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name DMA Transactional
 * @{
 */

/*!
 * @brief Initialize the SPI master DMA handle.
 *
 * This function initializes the SPI master DMA handle which can be used for other SPI master transactional APIs.
 * Usually, for a specified SPI instance, user need only call this API once to get the initialized handle.
 *
 * @param handle SPI handle pointer.
 * @param base SPI peripheral base address.
 */
void SPI_MasterCreateHandleDMA(spi_dma_handle_t *handle,
                               SPI_Type *base,
                               dma_handle_t *txHandle,
                               dma_handle_t *rxHandle);

/*!
 * @brief Perform a non-blocking SPI transfer using DMA.
 *
 * @note This interface returned immediately after transfer initiates, users should call
 * SPI_GetTransferStatus to poll the transfer status to check whether SPI transfer finished.
 *
 * @param handle SPI DMA handle pointer.
 * @param xfer Pointer to dma transfer structure.
 * @retval kStatus_Success Successfully start a transfer.
 * @retval kStatus_InvalidArgument Input argument is invalid.
 * @retval kStatus_SPI_Busy SPI is not idle, is running another transfer.
 */
status_t SPI_MasterTransferDMA(spi_dma_handle_t *handle, spi_transfer_t *xfer);

/*!
 * @brief Abort a SPI transfer using DMA.
 *
 * @param handle SPI DMA handle pointer.
 * @param remainingbytes Bytes remaining to be transfer. This parameter can be NULL if user do no need it.
 */
void SPI_MasterAbortTransferDMA(spi_dma_handle_t *handle);

/*!
 * @brief Get the remaining bytes for SPI DMA transfer.
 *
 * @param handle SPI DMA handle pointer.
 * @return Remaining bytes to be transferred.
 */
size_t SPI_MasterGetRemainingBytesDMA(spi_dma_handle_t *handle);

/*!
 * @brief Initialize the SPI slave DMA handle.
 *
 * This function initializes the SPI slave DMA handle which can be used for other SPI master transactional APIs.
 * Usually, for a specified SPI instance, user need only call this API once to get the initialized handle.
 *
 * @param handle SPI handle pointer.
 * @param base SPI peripheral base address.
 */
static inline void SPI_SlaveCreateHandleDMA(spi_dma_handle_t *handle,
                               SPI_Type *base,
                               dma_handle_t *txHandle,
                               dma_handle_t *rxHandle)
{
    SPI_MasterCreateHandleDMA(handle, base, txHandle, rxHandle);
}

/*!
 * @brief Perform a non-blocking SPI transfer using DMA.
 *
 * @note This interface returned immediately after transfer initiates, users should call
 * SPI_GetTransferStatus to poll the transfer status to check whether SPI transfer finished.
 *
 * @param handle SPI DMA handle pointer.
 * @param xfer Pointer to dma transfer structure.
 * @retval kStatus_Success Successfully start a transfer.
 * @retval kStatus_InvalidArgument Input argument is invalid.
 * @retval kStatus_SPI_Busy SPI is not idle, is running another transfer.
 */
static inline status_t SPI_SlaveTransferDMA(spi_dma_handle_t *handle, spi_transfer_t *xfer)
{
    return SPI_MasterTransferDMA(handle, xfer);
}

/*!
 * @brief Abort a SPI transfer using DMA.
 *
 * @param handle SPI DMA handle pointer.
 */
static inline void SPI_SlaveAbortTransferDMA(spi_dma_handle_t *handle)
{
    SPI_MasterAbortTransferDMA(handle);
}

/*!
 * @brief Get the remaining bytes to be transferred for SPI DMA.
 *
 * @param handle SPI DMA handle pointer.
 * @return Remaining bytes to be transferred.
 */
static inline size_t SPI_SlaveGetRemainingBytesDMA(spi_dma_handle_t *handle)
{
    return SPI_MasterGetRemainingBytesDMA(handle);
}

/*!
 * @brief Install callback for SPI DMA transfer finished.
 *
 * @param handle Pointer to SPI transfer handle, this should be a static variable.
 * @param callback SPI callback function.
 * @param userData Parameter for SPI DMAcallback.
 */
void SPI_SetTransferCallbackDMA(spi_dma_handle_t *handle, spi_dma_callback_t callback, void *userData);

/*! @} */

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif
