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
#ifndef _FSL_UART_EDMA_H_
#define _FSL_UART_EDMA_H_

#include "fsl_uart.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"

/*!
 * @addtogroup uart
 * @{
 */

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Forward declaration of the handle typedef. */
typedef struct _uart_edma_handle uart_edma_handle_t;

/*! @brief UART transfer callback function. */
typedef void (*uart_edma_transfer_callback_t)(UART_Type *base,
                                              uart_edma_handle_t *handle,
                                              status_t status,
                                              void *userData);

/*!
* @brief UART EDMA handle
*/
struct _uart_edma_handle
{
    uart_edma_transfer_callback_t callback; /*!< Callback function. */
    void *userData;                         /*!< UART callback function parameter.*/

    edma_handle_t *txEdmaHandle; /*!< The EDMA TX channel used. */
    edma_handle_t *rxEdmaHandle; /*!< The EDMA RX channel used. */

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
 * @brief Init the UART handle which is used in transcational functions
 * @param handle Pointer to uart_edma_handle_t structure
 * @param base UART peripheral base address
 * @param rxEdmaHandle User requested DMA handle for RX DMA transfer
 * @param txEdmaHandle User requested DMA handle for TX DMA transfer
 */
void UART_CreateHandleEDMA(UART_Type *base,
                           uart_edma_handle_t *handle,
                           uart_edma_transfer_callback_t callback,
                           void *userData,
                           edma_handle_t *txEdmaHandle,
                           edma_handle_t *rxEdmaHandle);

/*!
 * @brief Send data using EDMA
 *
 * This function send data using EDMA, this is non-blocking function, will return
 * right away, when all data have been sent out, the send callback function will be called.
 *
 * @param handle UART handle pointer.
 * @param xfer UART EDMA transfer sturcture, refer to #uart_transfer_t.
 * @retval kStatus_Success if succeed, others failed.
 * @retval kStatus_UART_TxBusy Previous transfer on going.
 */
status_t UART_SendEDMA(UART_Type *base, uart_edma_handle_t *handle, uart_transfer_t *xfer);

/*!
 * @brief Receive data using EDMA
 *
 * This function receive data using EDMA, this is non-blocking function, will return
 * right away, when all data have been received, the receive callback function will be called.
 *
 * @param handle Pointer to uart_edma_handle_t structure
 * @param xfer UART EDMA transfer sturcture, refer to #uart_transfer_t.
 * @retval kStatus_Success if succeed, others failed.
 * @retval kStatus_UART_RxBusy Previous transfer on going.
 */
status_t UART_ReceiveEDMA(UART_Type *base, uart_edma_handle_t *handle, uart_transfer_t *xfer);

/*!
 * @brief Abort the send data which using EDMA
 *
 * This function abort send data which using EDMA.
 *
 * @param handle Pointer to uart_edma_handle_t structure
 */
void UART_AbortSendEDMA(UART_Type *base, uart_edma_handle_t *handle);

/*!
 * @brief Abort the receive data which using EDMA
 *
 * This function abort receive data which using EDMA.
 *
 * @param handle Pointer to uart_edma_handle_t structure
 */
void UART_AbortReceiveEDMA(UART_Type *base, uart_edma_handle_t *handle);

/*!
 * @brief Get the number of bytes still not send out.
 *
 * This function gets the number of bytes still not send out.
 *
 * @param handle Pointer to uart_edma_handle_t structure
 * @return The number of bytes remaining not send out.
 */
size_t UART_GetSendRemainingBytesEDMA(UART_Type *base, uart_edma_handle_t *handle);

/*!
 * @brief Get the number of bytes still not received.
 *
 * This function gets the number of bytes still not received.
 *
 * @param handle Pointer to uart_edma_handle_t structure
 * @return The number of bytes still not received.
 */
size_t UART_GetReceiveRemainingBytesEDMA(UART_Type *base, uart_edma_handle_t *handle);

/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* _FSL_UART_EDMA_H_ */