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
#ifndef _FSL_FLEXIO_UART_DMA_H_
#define _FSL_FLEXIO_UART_DMA_H_

#include "fsl_flexio_uart.h"
#include "fsl_dmamux.h"
#include "fsl_dma.h"

/*!
 * @addtogroup flexio_uart
 * @{
 */

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Forward declaration of the handle typedef. */
typedef struct _flexio_uart_dma_handle flexio_uart_dma_handle_t;

/*! @brief UART transfer callback function. */
typedef void (*flexio_uart_dma_transfer_callback_t)(FLEXIO_UART_Type *base,
                                                    flexio_uart_dma_handle_t *handle,
                                                    status_t status,
                                                    void *userData);

/*!
* @brief UART DMA handle
*/
struct _flexio_uart_dma_handle
{
    flexio_uart_dma_transfer_callback_t callback; /*!< Callback function. */
    void *userData;                               /*!< Uart callback function parameter.*/

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
 * @brief Init the FLEXIO_UART handle which is used in transcational functions
 *
 * @param pointer to FLEXIO_UART_Type structure
 * @param handle Pointer to flexio_uart_dma_handle_t structure
 * @param callback FLEXIO UART callback, NULL means no callback.
 * @param userData User callback function data.
 * @param rxDmaHandle User requested DMA handle for RX DMA transfer
 * @param txDmaHandle User requested DMA handle for TX DMA transfer
 */
void FLEXIO_UART_CreateHandleDMA(FLEXIO_UART_Type *base,
                                 flexio_uart_dma_handle_t *handle,
                                 flexio_uart_dma_transfer_callback_t callback,
                                 void *userData,
                                 dma_handle_t *txDmaHandle,
                                 dma_handle_t *rxDmaHandle);

/*!
 * @brief Send data using DMA
 *
 * This function send data using DMA, this is non-blocking function, will return
 * right away, when all data have been sent out, the send callback function will be called.
 *
 * @param pointer to FLEXIO_UART_Type structure
 * @param handle Pointer to flexio_uart_dma_handle_t structure
 * @param xfer FLEXIO_UART DMA transfer sturcture, refer to #flexio_uart_transfer_t.
 * @retval kStatus_Success if succeed, others failed.
 * @retval kStatus_FLEXIO_UART_TxBusy Previous transfer on going.
 */
status_t FLEXIO_UART_SendDMA(FLEXIO_UART_Type *base, flexio_uart_dma_handle_t *handle, flexio_uart_transfer_t *xfer);

/*!
 * @brief Receive data using DMA
 *
 * This function receive data using DMA, this is non-blocking function, will return
 * right away, when all data have been received, the receive callback function will be called.
 *
 * @param pointer to FLEXIO_UART_Type structure
 * @param handle Pointer to flexio_uart_dma_handle_t structure
 * @param xfer FLEXIO_UART DMA transfer sturcture, refer to #flexio_uart_transfer_t.
 * @retval kStatus_Success if succeed, others failed.
 * @retval kStatus_FLEXIO_UART_RxBusy Previous transfer on going.
 */
status_t FLEXIO_UART_ReceiveDMA(FLEXIO_UART_Type *base, flexio_uart_dma_handle_t *handle, flexio_uart_transfer_t *xfer);

/*!
 * @brief Abort the send data which using DMA
 *
 * This function abort send data which using DMA.
 *
 * @param pointer to FLEXIO_UART_Type structure
 * @param handle Pointer to flexio_uart_dma_handle_t structure
 */
void FLEXIO_UART_AbortSendDMA(FLEXIO_UART_Type *base, flexio_uart_dma_handle_t *handle);

/*!
 * @brief Abort the receive data which using DMA
 *
 * This function abort receive data which using DMA.
 *
 * @param pointer to FLEXIO_UART_Type structure
 * @param handle Pointer to flexio_uart_dma_handle_t structure
 */
void FLEXIO_UART_AbortReceiveDMA(FLEXIO_UART_Type *base, flexio_uart_dma_handle_t *handle);

/*!
 * @brief Get the number of bytes still not send out.
 *
 * This function gets the number of bytes still not send out.
 *
 * @param pointer to FLEXIO_UART_Type structure
 * @param handle Pointer to flexio_uart_dma_handle_t structure
 * @return The number of bytes remaining not send out.
 */
size_t FLEXIO_UART_GetSendRemainingBytesDMA(FLEXIO_UART_Type *base, flexio_uart_dma_handle_t *handle);

/*!
 * @brief Get the number of bytes still not received.
 *
 * This function gets the number of bytes still not received.
 *
 * @param pointer to FLEXIO_UART_Type structure
 * @param handle Pointer to flexio_uart_dma_handle_t structure
 * @return The number of bytes still not received.
 */
size_t FLEXIO_UART_GetReceiveRemainingBytesDMA(FLEXIO_UART_Type *base, flexio_uart_dma_handle_t *handle);

/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* _FSL_UART_DMA_H_ */
