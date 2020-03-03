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
#ifndef _FSL_FLEXCAN_EDMA_H_
#define _FSL_FLEXCAN_EDMA_H_

#include "fsl_flexcan.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"

/*!
 * @addtogroup flexcan_driver
 * @{
 */

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Forward declaration of the handle typedef. */
typedef struct _flexcan_edma_handle flexcan_edma_handle_t;

/*! @brief FlexCAN transfer callback function. */
typedef void (*flexcan_edma_transfer_callback_t)(CAN_Type *base,
                                                 flexcan_edma_handle_t *handle,
                                                 status_t status,
                                                 void *userData);

/*!
* @brief FlexCAN EDMA handle
*/
struct _flexcan_edma_handle
{
    flexcan_edma_transfer_callback_t callback; /*!< Callback function. */
    void *userData;                            /*!< FlexCAN callback function parameter.*/
    edma_handle_t *rxFifoEdmaHandle;           /*!< The EDMA Rx FIFO channel used. */
    volatile uint8_t rxFifoState;              /*!< Rx FIFO transfer state. */
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
 * @brief Init the FlexCAN handle which is used in transcational functions
 *
 * @param base FlexCAN peripheral base address
 * @param handle Pointer to flexcan_edma_handle_t structure
 * @param callback The callback function.
 * @param userData The parameter of the callback function.
 * @param rxFifoEdmaHandle User requested DMA handle for Rx FIFO DMA transfer
 */
void FLEXCAN_CreateHandleEDMA(CAN_Type *base,
                              flexcan_edma_handle_t *handle,
                              flexcan_edma_transfer_callback_t callback,
                              void *userData,
                              edma_handle_t *rxFifoEdmaHandle);

/*!
 * @brief Receive CAN Message from Rx FIFO using EDMA
 *
 * This function receive CAN Message using EDMA, this is non-blocking function, will return
 * right away, when CAN Message have been received, the receive callback function will be called.
 *
 * @param base FlexCAN peripheral base address.
 * @param handle Pointer to flexcan_edma_handle_t structure
 * @param xfer FlexCAN Rx FIFO EDMA transfer sturcture, refer to #flexcan_fifo_transfer_t.
 * @retval kStatus_Success if succeed, others failed.
 * @retval kStatus_FLEXCAN_RxFifoBusy Previous transfer on going.
 */
status_t FLEXCAN_ReceiveFifoEDMA(CAN_Type *base, flexcan_edma_handle_t *handle, flexcan_fifo_transfer_t *xfer);

/*!
 * @brief Abort the receive process which using EDMA
 *
 * This function abort receive process which using EDMA.
 *
 * @param base FlexCAN peripheral base address.
 * @param handle Pointer to flexcan_edma_handle_t structure
 */
void FLEXCAN_AbortReceiveFifoEDMA(CAN_Type *base, flexcan_edma_handle_t *handle);

/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* _FSL_FLEXCAN_EDMA_H_ */
