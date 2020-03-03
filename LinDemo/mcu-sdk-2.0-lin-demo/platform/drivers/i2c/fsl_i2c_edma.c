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

#include "fsl_i2c_edma.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief i2c master DMA transfer state. */
enum _i2c_master_dma_transfer_states
{
    kIdleState = 0x0U,         /*!< I2C bus idle. */
    kTransferDataState = 0x1U, /*!< 7-bit address check state. */
};

/*! @brief Common sets of flags used by the driver. */
enum _i2c_flag_constants
{
/*! All flags which are cleared by the driver upon starting a transfer. */
#if defined(FSL_FEATURE_I2C_HAS_START_STOP_DETECT) && FSL_FEATURE_I2C_HAS_START_STOP_DETECT
    kClearFlags = kI2C_ArbitrationLostFlag | kI2C_IntPendingFlag | kI2C_StartDetectFlag | kI2C_StopDetectFlag,
#elif defined(FSL_FEATURE_I2C_HAS_STOP_DETECT) && FSL_FEATURE_I2C_HAS_STOP_DETECT
    kClearFlags = kI2C_ArbitrationLostFlag | kI2C_IntPendingFlag | kI2C_StopDetectFlag,
#else
    kClearFlags = kI2C_ArbitrationLostFlag | kI2C_IntPendingFlag,
#endif
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief EDMA callback for I2C master EDMA driver.
 *
 * @param handle EDMA handler for I2C master EDMA driver
 * @param userData user param passed to the callback function
 */
static void I2C_MasterTransferCallbackEDMA(struct _edma_handle *handle, void *userData, bool transferDone, uint32_t tcds);

/*!
 * @brief Check and clear status operation.
 *
 * @param handle pointer to i2c_master_edma_handle_t structure which stores the transfer state
 * @param status current i2c hardware status
 * @return transfer status of #_i2c_status
 */
static status_t I2C_CheckAndClearError(i2c_master_edma_handle_t *handle, uint32_t status);

/*!
 * @brief EDMA config for I2C master driver.
 *
 * @param handle pointer to i2c_master_edma_handle_t structure which stores the transfer state
 */
static void I2C_MasterTransferEDMAConfig(i2c_master_edma_handle_t *handle);

/*!
 * @brief Set up master transfer, send slave address and sub address(if any), wait until the
 * wait until address sent status return.
 *
 * @param handle pointer to i2c_master_edma_handle_t structure which stores the transfer state
 * @param transfer pointer to i2c_master_transfer_t structure
 */
static status_t I2C_InitTransferStateMachineEDMA(i2c_master_edma_handle_t *handle, i2c_master_transfer_t *transfer);

/*******************************************************************************
 * Codes
 ******************************************************************************/

static void I2C_MasterTransferCallbackEDMA(struct _edma_handle *handle, void *userData, bool transferDone, uint32_t tcds)
{
    i2c_master_edma_handle_t *i2cHandle = (i2c_master_edma_handle_t *)userData;
    uint16_t timeout = UINT16_MAX;
    status_t result = kStatus_Success;

    /* Disable DMA. */
    I2C_EnableDMA(i2cHandle->base, false);

    /* Send stop if kI2C_TransferNoStop flag is not asserted. */
    if (!(i2cHandle->transfer.flags & kI2C_TransferNoStopFlag))
    {
        if (i2cHandle->transfer.direction == kI2C_Read)
        {
            /* Change to send NAK at the last byte. */
            i2cHandle->base->C1 |= I2C_C1_TXAK_MASK;

            /* Wait the last data to be received. */
            while ((!(i2cHandle->base->S & kI2C_TransferCompleteFlag)) && (timeout--))
            {
            }

            if (timeout == 0)
            {
                i2cHandle->state = kIdleState;

                if (i2cHandle->completionCallback)
                {
                    i2cHandle->completionCallback(i2cHandle, kStatus_I2C_Timeout, i2cHandle->userData);
                }

                return;
            }

            /* Send stop signal. */
            result = I2C_MasterStop(i2cHandle->base);

            /* Read the last data byte. */
            *(i2cHandle->transfer.data + i2cHandle->transfer.dataSize - 1) = i2cHandle->base->D;
        }
        else
        {
            /* Wait the last data to be sent. */
            while ((!(i2cHandle->base->S & kI2C_TransferCompleteFlag)) && (timeout--))
            {
            }

            if (timeout == 0)
            {
                i2cHandle->state = kIdleState;

                if (i2cHandle->completionCallback)
                {
                    i2cHandle->completionCallback(i2cHandle, kStatus_I2C_Timeout, i2cHandle->userData);
                }

                return;
            }

            /* Send stop signal. */
            result = I2C_MasterStop(i2cHandle->base);
        }
    }

    i2cHandle->state = kIdleState;

    if (i2cHandle->completionCallback)
    {
        i2cHandle->completionCallback(i2cHandle, result, i2cHandle->userData);
    }
}

static status_t I2C_CheckAndClearError(i2c_master_edma_handle_t *handle, uint32_t status)
{
    status_t result = kStatus_Success;

    /* Check arbitration lost. */
    if (status & kI2C_ArbitrationLostFlag)
    {
        /* Clear arbitration lost flag. */
        handle->base->S = kI2C_ArbitrationLostFlag;
        result = kStatus_I2C_ArbitrationLost;
    }
    /* Check NAK */
    else if (status & kI2C_ReceiveNakFlag)
    {
        result = kStatus_I2C_Nak;
    }

    return result;
}

static status_t I2C_InitTransferStateMachineEDMA(i2c_master_edma_handle_t *handle, i2c_master_transfer_t *transfer)
{
    assert(handle);
    assert(transfer);

    status_t result = kStatus_Success;
    uint16_t timeout = UINT16_MAX;

    if (handle->state != kIdleState)
    {
        return kStatus_I2C_Busy;
    }
    else
    {
        /* Set up transfer first. */
        I2C_Type *base = handle->base;
        i2c_direction_t direction = transfer->direction;

        /* Init the handle member. */
        handle->transfer = *transfer;
        handle->state = kTransferDataState;

        /* Wait until ready to complete. */
        while (!(base->S & kI2C_TransferCompleteFlag))
        {
        }

        /* Clear all status before transfer. */
        I2C_MasterClearStatusFlags(handle->base, kClearFlags);

        /* Change to send write address when it's a read operation with command. */
        if ((transfer->subaddressSize > 0) && (transfer->direction == kI2C_Read))
        {
            direction = kI2C_Write;
        }

        /* If repeated start is requested, send repeated start. */
        if (handle->transfer.flags & kI2C_TransferRepeatedStartFlag)
        {
            result = I2C_MasterRepeatedStart(handle->base, handle->transfer.slaveAddress, direction);
        }
        else /* For normal transfer, send start. */
        {
            result = I2C_MasterStart(handle->base, handle->transfer.slaveAddress, direction);
        }

        /* Send subaddress. */
        if (handle->transfer.subaddressSize)
        {
            do
            {
                /* Wait until data transfer complete. */
                while ((!(base->S & kI2C_TransferCompleteFlag)) && (timeout--))
                {
                }

                if (timeout == 0)
                {
                    return kStatus_I2C_Timeout;
                }

                /* Clear interrupt pending flag. */
                base->S = kI2C_IntPendingFlag;

                handle->transfer.subaddressSize--;
                handle->base->D = ((handle->transfer.subaddress) >> (8 * handle->transfer.subaddressSize));

                /* Check if there's transfer error. */
                result = I2C_CheckAndClearError(handle, base->S);

                if (result)
                {
                    return result;
                }

            } while ((handle->transfer.subaddressSize > 0) && (result == kStatus_Success));

            if (handle->transfer.direction == kI2C_Read)
            {
                /* Wait until data transfer complete. */
                while ((!(base->S & kI2C_TransferCompleteFlag)) && (timeout--))
                {
                }

                if (timeout == 0)
                {
                    return kStatus_I2C_Timeout;
                }

                /* Clear pending flag. */
                base->S = kI2C_IntPendingFlag;

                /* Send repeated start and slave address. */
                result = I2C_MasterRepeatedStart(handle->base, handle->transfer.slaveAddress, kI2C_Read);
            }
        }

        if (result)
        {
            return result;
        }

        /* Wait until data transfer complete. */
        while ((!(base->S & kI2C_IntPendingFlag)) && (timeout--))
        {
        }

        if (timeout == 0)
        {
            return kStatus_I2C_Timeout;
        }

        /* Clear pending flag. */
        base->S = kI2C_IntPendingFlag;

        /* Check if there's transfer error. */
        result = I2C_CheckAndClearError(handle, base->S);
    }

    return result;
}

static void I2C_MasterTransferEDMAConfig(i2c_master_edma_handle_t *handle)
{
    edma_transfer_config_t transfer_config;

    if (handle->transfer.direction == kI2C_Read)
    {
        transfer_config.srcAddr = (uint32_t)I2C_GetDataRegAddr(handle->base);
        transfer_config.destAddr = (uint32_t)(handle->transfer.data);

        /* Send stop if kI2C_TransferNoStop flag is not asserted. */
        if (!(handle->transfer.flags & kI2C_TransferNoStopFlag))
        {
            transfer_config.majorLoopCounts = (handle->transfer.dataSize - 1);
        }
        else
        {
            transfer_config.majorLoopCounts = handle->transfer.dataSize;
        }

        transfer_config.srcTransferSize = kEDMA_TransferSize1Bytes;
        transfer_config.srcOffset = 0;
        transfer_config.destTransferSize = kEDMA_TransferSize1Bytes;
        transfer_config.destOffset = 1;
        transfer_config.minorLoopBytes = 1;
    }
    else
    {
        transfer_config.srcAddr = (uint32_t)(handle->transfer.data + 1);
        transfer_config.destAddr = (uint32_t)I2C_GetDataRegAddr(handle->base);
        transfer_config.majorLoopCounts = (handle->transfer.dataSize - 1);
        transfer_config.srcTransferSize = kEDMA_TransferSize1Bytes;
        transfer_config.srcOffset = 1;
        transfer_config.destTransferSize = kEDMA_TransferSize1Bytes;
        transfer_config.destOffset = 0;
        transfer_config.minorLoopBytes = 1;
    }

    EDMA_SetCallback(handle->dmaHandle, (edma_callback)I2C_MasterTransferCallbackEDMA, handle);
    EDMA_SubmitTransfer(handle->dmaHandle, &transfer_config);
    EDMA_StartTransfer(handle->dmaHandle);
}

void I2C_MasterCreateEDMAHandle(i2c_master_edma_handle_t *handle, I2C_Type *base, edma_handle_t *edmaHandle)
{
    assert(handle);

    /* Zero handle. */
    memset(handle, 0, sizeof(*handle));

    /* Set the base for the handle. */
    handle->base = base;

    /* Set the handle for EDMA. */
    handle->dmaHandle = edmaHandle;
}

status_t I2C_MasterTransferEDMA(i2c_master_edma_handle_t *handle, i2c_master_transfer_t *transfer)
{
    assert(handle);
    assert(transfer);

    status_t result;
    uint8_t tmpReg;
    I2C_Type *base = handle->base;
    uint16_t timeout = UINT16_MAX;

    /* Disable dma transfer. */
    I2C_EnableDMA(base, false);

    /* Send address and command buffer(if there is), until senddata phase or receive data phase. */
    result = I2C_InitTransferStateMachineEDMA(handle, transfer);

    if (result)
    {
        /* Send stop if received Nak. */
        if (result == kStatus_I2C_Nak)
        {
            if (I2C_MasterStop(handle->base) != kStatus_Success)
            {
                result = kStatus_I2C_Timeout;
            }
        }

        /* Reset the state to idle state. */
        handle->state = kIdleState;

        return result;
    }

    /* Configure dma transfer. */
    /* For i2c send, need to send 1 byte first to trigger the dma, for i2c read,
    need to send stop before reading the last byte, so the dma transfer size should
    be (xSize - 1). */
    if (handle->transfer.dataSize > 1)
    {
        I2C_MasterTransferEDMAConfig(handle);
        if (handle->transfer.direction == kI2C_Read)
        {
            /* Change direction for receive. */
            base->C1 &= ~I2C_C1_TX_MASK;

            /* Read dummy to release the bus. */
            base->D;

            /* Enabe dma transfer. */
            I2C_EnableDMA(base, true);
        }
        else
        {
            /* Enabe dma transfer. */
            I2C_EnableDMA(base, true);

            /* Send the first data. */
            base->D = *handle->transfer.data;
        }
    }
    else /* If transfer size is 1, use polling method. */
    {
        if (handle->transfer.direction == kI2C_Read)
        {
            tmpReg = base->C1;

            /* Change direction to Rx. */
            tmpReg &= ~I2C_C1_TX_MASK;

            /* Configure send NAK */
            tmpReg |= I2C_C1_TXAK_MASK;

            base->C1 = tmpReg;

            /* Read dummy to release the bus. */
            base->D;
        }
        else
        {
            base->D = *handle->transfer.data;
        }

        /* Wait until data transfer complete. */
        while ((!(base->S & kI2C_IntPendingFlag)) && (timeout--))
        {
        }

        if (timeout == 0)
        {
            handle->state = kIdleState;
            return kStatus_I2C_Timeout;
        }

        /* Clear pending flag. */
        base->S = kI2C_IntPendingFlag;

        /* Send stop if kI2C_TransferNoStop flag is not asserted. */
        if (!(handle->transfer.flags & kI2C_TransferNoStopFlag))
        {
            result = I2C_MasterStop(base);
        }

        /* Read the last byte of data. */
        if (handle->transfer.direction == kI2C_Read)
        {
            *handle->transfer.data = base->D;
        }

        /* Reset the state to idle. */
        handle->state = kIdleState;
    }

    return result;
}

void I2C_MasterSetTransferCallbackEDMA(i2c_master_edma_handle_t *handle,
                                       i2c_master_edma_transfer_callback_t completionCallback,
                                       void *userData)
{
    /* Install the user callback function into the I2C driver handle. */
    handle->completionCallback = completionCallback;
    handle->userData = userData;
}

size_t I2C_MasterGetTransferRemainingBytesEDMA(i2c_master_edma_handle_t *handle)
{
    assert(handle->dmaHandle);

    size_t bytes;

    if (kIdleState != handle->state)
    {
        bytes = EDMA_GetRemainingBytes(handle->dmaHandle->base, handle->dmaHandle->channel);
    }
    else
    {
        bytes = 0;
    }

    return bytes;
}

void I2C_MasterAbortTransferEDMA(i2c_master_edma_handle_t *handle)
{
    EDMA_AbortTransfer(handle->dmaHandle);

    /* Disable dma transfer. */
    I2C_EnableDMA(handle->base, false);

    /* Reset the state to idle. */
    handle->state = kIdleState;
}
