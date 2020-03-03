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
#ifndef _FSL_I2C_DMA_H_
#define _FSL_I2C_DMA_H_

#include "fsl_i2c.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"

/*!
 * @addtogroup i2c
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief I2C master edma handle typedef. */
typedef struct _i2c_master_edma_handle i2c_master_edma_handle_t;

/*! @brief I2C master edma transfer callback typedef. */
typedef void (*i2c_master_edma_transfer_callback_t)(i2c_master_edma_handle_t *handle, status_t status, void *userData);

/*! @brief I2C master edma transfer structure. */
struct _i2c_master_edma_handle
{
    I2C_Type *base;                 /*!< I2C peripheral base address. */
    i2c_master_transfer_t transfer; /*!< I2C master transfer struct. */
    uint8_t state;                  /*!< I2C master transfer status. */
    edma_handle_t *dmaHandle;       /*!< The eDMA handler used. */
    i2c_master_edma_transfer_callback_t
        completionCallback; /*!< Callback function called after edma transfer finished. */
    void *userData;         /*!< Callback parameter passed to callback function. */
};

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus. */

/*!
 * @name I2C Block EDMA Transfer Operation
 * @{
 */

/*!
 * @brief Init the I2C handle which is used in transcational functions
 *
 * @param handle pointer to i2c_master_edma_handle_t structure
 * @param base I2C peripheral base address
 * @param edmaHandle EDMA handle pointer
 */
void I2C_MasterCreateEDMAHandle(i2c_master_edma_handle_t *handle, I2C_Type *base, edma_handle_t *edmaHandle);

/*!
 * @brief Performs a master edma non-blocking transfer on the I2C bus
 *
 * @param handle pointer to i2c_master_edma_handle_t structure
 * @param transfer pointer to transfer structure of i2c_master_transfer_t
 * @return status of status_t
 */
status_t I2C_MasterTransferEDMA(i2c_master_edma_handle_t *handle, i2c_master_transfer_t *transfer);

/*!
 * @brief Set the user callback which is called when the master I2C transfer EDMA completes.
 *
 * @param handle pointer to i2c_master_edma_handle_t structure which stores the transfer state
 * @param completionCallback pointer to user callback function
 * @param userData user param passed to the callback function
 */
void I2C_MasterSetTransferCallbackEDMA(i2c_master_edma_handle_t *handle,
                                       i2c_master_edma_transfer_callback_t completionCallback,
                                       void *userData);

/*!
 * @brief Get master transfer status during a edma non-blocking transfer
 *
 * @param handle pointer to i2c_master_edma_handle_t structure
 * @return remaining bytes to transfer
 */
size_t I2C_MasterGetTransferRemainingBytesEDMA(i2c_master_edma_handle_t *handle);

/*!
 * @brief Abort a master edma non-blocking transfer in a early time
 *
 * @param handle pointer to i2c_master_edma_handle_t structure
 */
void I2C_MasterAbortTransferEDMA(i2c_master_edma_handle_t *handle);

/* @} */
#if defined(__cplusplus)
}
#endif /*_cplusplus. */
/*@}*/
#endif /*_FSL_I2C_DMA_H_*/
