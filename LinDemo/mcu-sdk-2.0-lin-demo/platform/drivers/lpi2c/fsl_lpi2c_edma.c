/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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

#include "fsl_lpi2c_edma.h"
#include "fsl_dmamux.h"
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* @brief Mask to align an address to 32 bytes. */
#define ALIGN_32_MASK (0x1fU)

/*! @brief Common sets of flags used by the driver. */
enum _lpi2c_flag_constants
{
    /*! All flags which are cleared by the driver upon starting a transfer. */
    kMasterClearFlags = kLPI2C_MasterEndOfPacketFlag | kLPI2C_MasterStopDetectFlag | kLPI2C_MasterNackDetectFlag |
                        kLPI2C_MasterArbitrationLostFlag | kLPI2C_MasterFifoErrFlag | kLPI2C_MasterPinLowTimeoutFlag |
                        kLPI2C_MasterDataMatchFlag,

    /*! IRQ sources enabled by the non-blocking transactional API. */
    kMasterIrqFlags = kLPI2C_MasterArbitrationLostFlag | kLPI2C_MasterTxReadyFlag | kLPI2C_MasterRxReadyFlag |
                      kLPI2C_MasterStopDetectFlag | kLPI2C_MasterNackDetectFlag | kLPI2C_MasterPinLowTimeoutFlag |
                      kLPI2C_MasterFifoErrFlag,

    /*! Errors to check for. */
    kMasterErrorFlags = kLPI2C_MasterNackDetectFlag | kLPI2C_MasterArbitrationLostFlag | kLPI2C_MasterFifoErrFlag |
                        kLPI2C_MasterPinLowTimeoutFlag,

    /*! All flags which are cleared by the driver upon starting a transfer. */
    kSlaveClearFlags = kLPI2C_SlaveRepeatedStartDetectFlag | kLPI2C_SlaveStopDetectFlag | kLPI2C_SlaveBitErrFlag |
                       kLPI2C_SlaveFifoErrFlag,

    /*! IRQ sources enabled by the non-blocking transactional API. */
    kSlaveIrqFlags = kLPI2C_SlaveTxReadyFlag | kLPI2C_SlaveRxReadyFlag | kLPI2C_SlaveStopDetectFlag |
                     kLPI2C_SlaveRepeatedStartDetectFlag | kLPI2C_SlaveFifoErrFlag | kLPI2C_SlaveBitErrFlag |
                     kLPI2C_SlaveTransmitAckFlag | kLPI2C_SlaveAddressValidFlag,

    /*! Errors to check for. */
    kSlaveErrorFlags = kLPI2C_SlaveFifoErrFlag | kLPI2C_SlaveBitErrFlag,
};

/* ! @brief LPI2C master fifo commands. */
enum _lpi2c_master_fifo_cmd
{
    kTxDataCmd = LPI2C_MTDR_CMD(0x0U), /*!< Transmit DATA[7:0] */
    kRxDataCmd = LPI2C_MTDR_CMD(0X1U), /*!< Receive (DATA[7:0] + 1) bytes */
    kStopCmd   = LPI2C_MTDR_CMD(0x2U), /*!< Generate STOP condition */
    kStartCmd  = LPI2C_MTDR_CMD(0x4U), /*!< Generate(repeated) START and transmit address in DATA[[7:0] */
};

/*! @brief States for the state machine used by transactional APIs. */
enum _lpi2c_transfer_states
{
    kIdleState = 0,
    kSendCommandState,
    kIssueReadCommandState,
    kTransferDataState,
    kStopState,
    kWaitForCompletionState,
};

/*! @brief Typedef for interrupt handler. */
typedef void (*lpi2c_isr_t)(LPI2C_Type *base, void *handle);

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static uint32_t LPI2C_GetInstance(LPI2C_Type *base);

static status_t LPI2C_MasterCheckAndClearError(LPI2C_Type *base, uint32_t status);

static status_t LPI2C_CheckForBusyBus(LPI2C_Type *base);

static status_t LPI2C_RunTransferStateMachine(lpi2c_master_handle_t *handle, bool *isDone);

static void LPI2C_InitTransferStateMachine(lpi2c_master_handle_t *handle);

static void LPI2C_MasterEDMACallback(edma_handle_t *dmaHandle, void *userData, bool isTransferDone, uint32_t tcds);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Array to map LPI2C instance number to base pointer. */
static LPI2C_Type *const kLpi2cBases[] = LPI2C_BASE_PTRS;

/*! @brief Array to map LPI2C instance number to IRQ number. */
static IRQn_Type const kLpi2cIrqs[] = LPI2C_IRQS;

/*! @brief Pointers to master handles for each instance. */
static lpi2c_master_edma_handle_t *s_lpi2cMasterEDMAHandle[FSL_FEATURE_SOC_LPI2C_COUNT];

/*******************************************************************************
 * Code
 ******************************************************************************/

uint32_t LPI2C_GetInstance(LPI2C_Type *base)
{
    uint32_t instance;
    for (instance = 0; instance < FSL_FEATURE_SOC_LPI2C_COUNT; ++instance)
    {
        if (kLpi2cBases[instance] == base)
        {
            return instance;
        }
    }

    assert(false);
    return 0;
}

/*!
 * @brief Make sure the bus isn't already busy.
 *
 * A busy bus is allowed if we are the one driving it.
 */
status_t LPI2C_CheckForBusyBus(LPI2C_Type *base)
{
    uint32_t status = LPI2C_MasterGetStatusFlags(base);
    if ((status & kLPI2C_MasterBusBusyFlag) && !(status & kLPI2C_MasterBusyFlag))
    {
        return kStatus_LPI2C_Busy;
    }

    return kStatus_Success;
}

void LPI2C_MasterCreateEDMAHandle(LPI2C_Type *base,
                                  lpi2c_master_edma_handle_t *handle,
                                  edma_handle_t *rxDmaHandle,
                                  edma_handle_t *txDmaHandle,
                                  lpi2c_master_edma_transfer_callback_t callback,
                                  void *userData)
{
    uint32_t instance;

    assert(handle);
    assert(rxDmaHandle);
    assert(txDmaHandle);

    /* Clear out the handle. */
    memset(handle, 0, sizeof(*handle));

    /* Look up instance number */
    instance = LPI2C_GetInstance(base);

    /* Save base and instance. */
    handle->base               = base;
    handle->completionCallback = callback;
    handle->userData           = userData;
    handle->rx                 = rxDmaHandle;
    handle->tx                 = txDmaHandle;

    /* Save this handle for IRQ use. */
    s_lpi2cMasterEDMAHandle[instance] = handle;

    /* Set DMA channel completion callbacks. */
    EDMA_SetCallback(handle->rx, LPI2C_MasterEDMACallback, handle);
    EDMA_SetCallback(handle->tx, LPI2C_MasterEDMACallback, handle);
}

/*!
 * @brief Prepares the transfer state machine and fills in the command buffer.
 * @return Number of command words.
 */
uint32_t LPI2C_InitEDMATransferStateMachine(lpi2c_master_edma_handle_t *handle)
{
    lpi2c_master_transfer_t *xfer = &handle->transfer;
    uint16_t *cmd                 = (uint16_t *)&handle->commandBuffer;
    uint32_t cmdCount             = 0;

    /* Handle no start option. */
    if (xfer->flags & kLPI2C_TransferNoStartFlag)
    {
        if (xfer->direction == kLPI2C_Read)
        {
            /* Need to issue read command first. */
            cmd[cmdCount++] = kRxDataCmd | LPI2C_MTDR_DATA(xfer->dataSize - 1);
        }
    }
    else
    {

        /*
         * Initial direction depends on whether a subaddress was provided, and of course the actual
         * data transfer direction.
         */
        lpi2c_direction_t direction = xfer->subaddressSize ? kLPI2C_Write : xfer->direction;

        /* Start command. */
        cmd[cmdCount++] = kStartCmd | (((uint16_t)xfer->slaveAddress << 1U) | (uint16_t)direction);

        /* Subaddress, MSB first. */
        if (xfer->subaddressSize)
        {
            uint32_t subaddressRemaining = xfer->subaddressSize;
            while (subaddressRemaining--)
            {
                uint8_t subaddressByte = (xfer->subaddress >> (8 * subaddressRemaining)) & 0xff;
                cmd[cmdCount++] = subaddressByte;
            }
        }

        /* Reads need special handling. */
        if (xfer->dataSize && xfer->direction == kLPI2C_Read)
        {
            /* Need to send repeated start if switching directions to read. */
            if (direction == kLPI2C_Write)
            {
                cmd[cmdCount++] = kStartCmd | (((uint16_t)xfer->slaveAddress << 1U) | kLPI2C_Read);
            }

            /* Read command. */
            cmd[cmdCount++] = kRxDataCmd | LPI2C_MTDR_DATA(xfer->dataSize - 1);
        }
    }

    return cmdCount;
}

status_t LPI2C_MasterTransferEDMA(LPI2C_Type *base, lpi2c_master_edma_handle_t *handle, lpi2c_master_transfer_t *xfer)
{
    status_t result;

    assert(handle);
    assert(xfer);
    assert(xfer->subaddressSize <= sizeof(xfer->subaddress));

    /* Return busy if another transaction is in progress. */
    if (handle->isBusy)
    {
        return kStatus_LPI2C_Busy;
    }

    /* Return an error if the bus is already in use not by us. */
    result = LPI2C_CheckForBusyBus(handle->base);
    if (result)
    {
        return result;
    }

    /* We're now busy. */
    handle->isBusy = true;

    /* Disable LPI2C IRQ and DMA sources while we configure stuff. */
    LPI2C_MasterDisableInterrupts(handle->base, kMasterIrqFlags);
    LPI2C_MasterSetDmaEnable(base, false, false);

    /* Clear all flags. */
    LPI2C_MasterClearStatusFlags(handle->base, kMasterClearFlags);

    /* Save transfer into handle. */
    handle->transfer = *xfer;

    /* Generate commands to send. */
    uint32_t commandCount = LPI2C_InitEDMATransferStateMachine(handle);

    /* If the user is transmitting no data with no start or stop, then just go ahead and invoke the callback. */
    if (commandCount == 0 && xfer->dataSize == 0)
    {
        if (handle->completionCallback)
        {
            handle->completionCallback(base, handle, kStatus_Success, handle->userData);
        }
        return kStatus_Success;
    }

    /* Turn on auto-stop option unless the user doesn't want to send a stop. */
    if (!(xfer->flags & kLPI2C_TransferNoStopFlag))
    {
        base->MCFGR1 |= LPI2C_MCFGR1_AUTOSTOP_MASK;
    }

    /* Get a 32-byte aligned TCD pointer. */
    edma_tcd_t *txTcd = (edma_tcd_t *)((uint32_t)(&handle->txTcds[1]) & (~ALIGN_32_MASK));

    edma_transfer_config_t transferConfig;

    EDMA_ResetChannel(handle->rx->base, handle->rx->channel);
    EDMA_ResetChannel(handle->tx->base, handle->tx->channel);

    /* Set up data transmit. */
    if (xfer->direction == kLPI2C_Write)
    {
        transferConfig.srcAddr          = (uint32_t)xfer->data;
        transferConfig.destAddr         = (uint32_t)LPI2C_MasterGetTxFifoAddress(base);
        transferConfig.srcTransferSize  = kEDMA_TransferSize1Bytes;
        transferConfig.destTransferSize = kEDMA_TransferSize1Bytes;
        transferConfig.srcOffset        = sizeof(uint8_t);
        transferConfig.destOffset       = 0;
        transferConfig.minorLoopBytes   = sizeof(uint8_t); /* TODO optimize to fill fifo */
        transferConfig.majorLoopCounts  = xfer->dataSize;

        if (commandCount > 0)
        {
            EDMA_TCDReset(txTcd);
            EDMA_TCDSetTransferConfig(txTcd, &transferConfig, NULL);
            EDMA_TCDEnableInterrupts(txTcd, kEDMA_MajorInterruptEnable);
        }
        else
        {
            EDMA_SetTransferConfig(handle->tx->base, handle->tx->channel, &transferConfig, NULL);
        }
    }
    else
    {
        /* Set up data receive. */
        transferConfig.srcAddr          = (uint32_t)LPI2C_MasterGetRxFifoAddress(base);
        transferConfig.destAddr         = (uint32_t)xfer->data;
        transferConfig.srcTransferSize  = kEDMA_TransferSize1Bytes;
        transferConfig.destTransferSize = kEDMA_TransferSize1Bytes;
        transferConfig.srcOffset        = 0;
        transferConfig.destOffset       = sizeof(uint8_t);
        transferConfig.minorLoopBytes   = sizeof(uint8_t); /* TODO optimize to empty fifo */
        transferConfig.majorLoopCounts  = xfer->dataSize;
        EDMA_SetTransferConfig(handle->rx->base, handle->rx->channel, &transferConfig, NULL);
        EDMA_EnableChannelInterrupts(handle->rx->base, handle->rx->channel, kEDMA_MajorInterruptEnable);
    }

    if (commandCount > 0)
    {
        /* Set up command transfer. */
        transferConfig.srcAddr          = (uint32_t)handle->commandBuffer;
        transferConfig.destAddr         = (uint32_t)LPI2C_MasterGetTxFifoAddress(base);
        transferConfig.srcTransferSize  = kEDMA_TransferSize2Bytes;
        transferConfig.destTransferSize = kEDMA_TransferSize2Bytes;
        transferConfig.srcOffset        = sizeof(uint16_t);
        transferConfig.destOffset       = 0;
        transferConfig.minorLoopBytes   = sizeof(uint16_t); /* TODO optimize to fill fifo */
        transferConfig.majorLoopCounts  = commandCount;

        EDMA_SetTransferConfig(handle->tx->base, handle->tx->channel, &transferConfig,
                               (xfer->direction == kLPI2C_Write) ? txTcd : NULL);
    }

    /* Start DMA transfer. */
    if (xfer->direction == kLPI2C_Read)
    {
        EDMA_StartTransfer(handle->rx);
    }
    EDMA_StartTransfer(handle->tx);

    /* Enable DMA in both directions. */
    LPI2C_MasterSetDmaEnable(base, true, true);

    return result;
}

status_t LPI2C_MasterGetEDMATransferStatus(LPI2C_Type *base, lpi2c_master_edma_handle_t *handle, size_t *transferCount)
{
    assert(handle);

    /* Catch when there is not an active transfer. */
    if (!handle->isBusy)
    {
        if (transferCount)
        {
            *transferCount = 0;
        }
        return kStatus_LPI2C_Idle;
    }

    if (transferCount)
    {
        uint32_t remaining;
        if (handle->transfer.direction == kLPI2C_Write)
        {
            remaining = EDMA_GetRemainingBytes(handle->tx->base, handle->tx->channel);
        }
        else
        {
            remaining = EDMA_GetRemainingBytes(handle->rx->base, handle->rx->channel);
        }
        *transferCount = handle->transfer.dataSize - remaining;
    }

    return kStatus_Success;
}

status_t LPI2C_MasterAbortEDMATransfer(LPI2C_Type *base, lpi2c_master_edma_handle_t *handle)
{
    /* Catch when there is not an active transfer. */
    if (!handle->isBusy)
    {
        return kStatus_LPI2C_Idle;
    }

    /* Terminate DMA transfers. */
    EDMA_AbortTransfer(handle->rx);
    EDMA_AbortTransfer(handle->tx);

    /* Turn off auto-stop option. */
    handle->base->MCFGR1 &= ~LPI2C_MCFGR1_AUTOSTOP_MASK;

    /* Reset fifos. */
    base->MCR |= LPI2C_MCR_RRF_MASK | LPI2C_MCR_RTF_MASK;

    /* Send a stop command to finalize the transfer. */
    base->MTDR = kStopCmd;

    /* Reset handle. */
    handle->isBusy = false;

    return kStatus_Success;
}

void LPI2C_MasterEDMACallback(edma_handle_t *dmaHandle, void *userData, bool isTransferDone, uint32_t tcds)
{
    lpi2c_master_edma_handle_t *handle = (lpi2c_master_edma_handle_t *)userData;
    if (!handle)
    {
        return;
    }

    /* Turn off auto-stop option. */
    handle->base->MCFGR1 &= ~LPI2C_MCFGR1_AUTOSTOP_MASK;

    /* Invoke callback. */
    if (handle->completionCallback)
    {
        handle->completionCallback(handle->base, handle, kStatus_Success, handle->userData);
    }

    /* Done with this transaction. */
    handle->isBusy = false;
}
