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
#ifndef _FSL_LPUART_DMA_H_
#define _FSL_LPUART_DMA_H_

#include "fsl_lpuart.h"
#include "fsl_dma.h"

/*!
 * @addtogroup lpuart
 * @{
 */

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Forward declaration of the handle typedef. */
typedef struct _lpuart_dma_handle lpuart_dma_handle_t;

/*! @brief LPUART transfer callback function. */
typedef void (*lpuart_dma_transfer_callback_t)(LPUART_Type *base,
                                               lpuart_dma_handle_t *handle,
                                               status_t status,
                                               void *userData);

/*!
* @brief LPUART DMA handle
*/
struct _lpuart_dma_handle
{
    lpuart_dma_transfer_callback_t callback; /*!< Callback function. */
    void *userData;                          /*!< LPUART callback function parameter.*/

    dma_handle_t *txDmaHandle; /*!< The DMA TX channel used. */
    dma_handle_t *rxDmaHandle; /*!< The DMA RX channel used. */

    volatile uint8_t txState; /*!< TX transfer state. */
    volatile uint8_t rxState; /*!< RX transfer state */
};

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name EDMA transactional
 * @{
 */

/*!
 * @brief Init the LPUART handle which is used in transcational functions
 * @param handle Pointer to lpuart_dma_handle_t structure
 * @param base LPUART peripheral base address
 * @param rxDmaHandle User requested DMA handle for RX DMA transfer
 * @param txDmaHandle User requested DMA handle for TX DMA transfer
 */
void LPUART_CreateHandleDMA(LPUART_Type *base,
                            lpuart_dma_handle_t *handle,
                            lpuart_dma_transfer_callback_t callback,
                            void *userData,
                            dma_handle_t *txDmaHandle,
                            dma_handle_t *rxDmaHandle);

/*!
 * @brief Send data using DMA
 *
 * This function send data using DMA, this is non-blocking function, will return
 * right away, when all data have been sent out, the send callback function will be called.
 *
 * @param handle LPUART handle pointer.
 * @param xfer LPUART DMA transfer sturcture, refer to #lpuart_transfer_t.
 * @retval kStatus_Success if succeed, others failed.
 * @retval kStatus_LPUART_TxBusy Previous transfer on going.
 */
status_t LPUART_SendDMA(LPUART_Type *base, lpuart_dma_handle_t *handle, lpuart_transfer_t *xfer);

/*!
 * @brief Receive data using DMA
 *
 * This function receive data using DMA, this is non-blocking function, will return
 * right away, when all data have been received, the receive callback function will be called.
 *
 * @param handle Pointer to lpuart_dma_handle_t structure
 * @param xfer LPUART DMA transfer sturcture, refer to #lpuart_transfer_t.
 * @retval kStatus_Success if succeed, others failed.
 * @retval kStatus_LPUART_RxBusy Previous transfer on going.
 */
status_t LPUART_ReceiveDMA(LPUART_Type *base, lpuart_dma_handle_t *handle, lpuart_transfer_t *xfer);

/*!
 * @brief Abort the send data which using DMA
 *
 * This function abort send data which using DMA.
 *
 * @param handle Pointer to lpuart_dma_handle_t structure
 */
void LPUART_AbortSendDMA(LPUART_Type *base, lpuart_dma_handle_t *handle);

/*!
 * @brief Abort the receive data which using DMA
 *
 * This function abort receive data which using DMA.
 *
 * @param handle Pointer to lpuart_dma_handle_t structure
 */
void LPUART_AbortReceiveDMA(LPUART_Type *base, lpuart_dma_handle_t *handle);

/*!
 * @brief Get the number of bytes still not send out.
 *
 * This function gets the number of bytes still not send out.
 *
 * @param handle Pointer to lpuart_dma_handle_t structure
 * @return The number of bytes remaining not send out.
 */
size_t LPUART_GetSendRemainingBytesDMA(LPUART_Type *base, lpuart_dma_handle_t *handle);

/*!
 * @brief Get the number of bytes still not received.
 *
 * This function gets the number of bytes still not received.
 *
 * @param handle Pointer to lpuart_dma_handle_t structure
 * @return The number of bytes still not received.
 */
size_t LPUART_GetReceiveRemainingBytesDMA(LPUART_Type *base, lpuart_dma_handle_t *handle);

/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* _FSL_LPUART_DMA_H_ */
