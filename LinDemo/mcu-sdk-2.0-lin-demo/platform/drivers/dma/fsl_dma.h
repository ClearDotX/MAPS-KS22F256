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

#ifndef _FSL_DMA_H_
#define _FSL_DMA_H_

#include "fsl_common.h"

/*!
 * @addtogroup dma_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief DMA driver version */
#define FSL_DMA_DRIVER_VERSION (MAKE_VERSION(2, 0, 0)) /*!< Version 2.0.0. */

/*! @brief status flag for the DMA driver. */
enum _dma_channel_status_flags
{
    kDMA_TransactionsBCRFlag = DMA_DSR_BCR_BCR_MASK,       /*!< Contains the number of bytes yet to be
                                                                transferred for a given block */
    kDMA_TransactionsDoneFlag = DMA_DSR_BCR_DONE_MASK,     /*!< Transactions Done */
    kDMA_TransactionsBusyFlag = DMA_DSR_BCR_BSY_MASK,      /*!< Transactions Busy */
    kDMA_TransactionsRequestFlag = DMA_DSR_BCR_REQ_MASK,   /*!< Transactions Request */
    kDMA_BusErrorOnDestinationFlag = DMA_DSR_BCR_BED_MASK, /*!< Bus Error on Destination */
    kDMA_BusErrorOnSourceFlag = DMA_DSR_BCR_BES_MASK,      /*!< Bus Error on Source */
    kDMA_ConfigurationErrorFlag = DMA_DSR_BCR_CE_MASK,     /*!< Configuration Error */
};

/*! @brief DMA transfer size type*/
typedef enum _dma_transfer_size
{
    kDMA_Transfersize32bits = 0x0U, /*!< 32 bits are transferred for every read/write */
    kDMA_Transfersize8bits,         /*!< 8 bits are transferred for every read/write */
    kDMA_Transfersize16bits,        /*!< 16b its are transferred for every read/write */
} dma_transfer_size_t;

/*! @brief Configuration type for the DMA modulo */
typedef enum _dma_modulo
{
    kDMA_ModuloDisable = 0x0U, /*!< Buffer disabled */
    kDMA_Modulo16Bytes,        /*!< Circular buffer size is 16 bytes. */
    kDMA_Modulo32Bytes,        /*!< Circular buffer size is 32 bytes. */
    kDMA_Modulo64Bytes,        /*!< Circular buffer size is 64 bytes. */
    kDMA_Modulo128Bytes,       /*!< Circular buffer size is 128 bytes. */
    kDMA_Modulo256Bytes,       /*!< Circular buffer size is 256 bytes. */
    kDMA_Modulo512Bytes,       /*!< Circular buffer size is 512 bytes. */
    kDMA_Modulo1KBytes,        /*!< Circular buffer size is 1 KB. */
    kDMA_Modulo2KBytes,        /*!< Circular buffer size is 2 KB. */
    kDMA_Modulo4KBytes,        /*!< Circular buffer size is 4 KB. */
    kDMA_Modulo8KBytes,        /*!< Circular buffer size is 8 KB. */
    kDMA_Modulo16KBytes,       /*!< Circular buffer size is 16 KB. */
    kDMA_Modulo32KBytes,       /*!< Circular buffer size is 32 KB. */
    kDMA_Modulo64KBytes,       /*!< Circular buffer size is 64 KB. */
    kDMA_Modulo128KBytes,      /*!< Circular buffer size is 128 KB. */
    kDMA_Modulo256KBytes,      /*!< Circular buffer size is 256 KB. */
} dma_modulo_t;

/*! @brief DMA channel link type */
typedef enum _dma_channel_link_type
{
    kDMA_ChannelLinkDisable = 0x0U,      /*!< No channel link */
    kDMA_ChannelLinkChannel1AndChannel2, /*!< Perform a link to channel LCH1 after each cycle-steal transfer
                                              followed by a link to LCH2 after the BCR decrements to 0 */
    kDMA_ChannelLinkChannel1,            /*!< Perform a link to LCH1 after each cycle-steal transfer. */
    kDMA_ChannelLinkChannel1AfterBCR0,   /*!< Perform a link to LCH1 after the BCR decrements. */
} dma_channel_link_type_t;

/*! @brief DMA transfer type */
typedef enum _dma_transfer_type
{
    kDMA_MemoryToMemory = 0x0U, /*!< Memory to Memory transfer */
    kDMA_PeripheralToMemory,    /*!< Peripheral to Memory transfer */
    kDMA_MemoryToPeripheral,    /*!< Memory to Peripheral transfer */
} dma_transfer_type_t;

/*! @brief DMA transfer options */
typedef enum _dma_transfer_options
{
    kDMA_NoOptions = 0x0U, /*!< Transfer without options */
    kDMA_EnableInterrupt,  /*!< Enable interrupt while transfer complete. */
} dma_transfer_options_t;

enum _dma_transfer_status
{
    kStatus_DMA_Busy = MAKE_STATUS(kStatusGroup_DMA, 0),
};

/*! @DMA transfer configuration structure */
typedef struct _dma_transfer_config
{
    uint32_t srcAddr;             /*!< DMA transfer source address */
    uint32_t destAddr;            /*!< DMA destination address.*/
    bool enableSrcIncrement;      /*!< Source address increase after each transfer */
    dma_transfer_size_t srcSize;  /*!< Source transfer size unit */
    bool enableDestIncrement;     /*!< Destination address increase after each transfer */
    dma_transfer_size_t destSize; /*!< Destination transfer unit*/
    uint32_t transferSize;        /*!< The number of bytes to be transferred. */
} dma_transfer_config_t;

/*! @DMA transfer configuration structure */
typedef struct _dma_channel_link_config
{
    dma_channel_link_type_t linkType; /*!< Channel link type */
    uint32_t channel1;                /*!< The index of channel 1 */
    uint32_t channel2;                /*!< The index of channel 2 */
} dma_channel_link_config_t;

/*! @brief Callback function prototype for DMA driver. */
struct _dma_handle;
typedef void (*dma_callback)(struct _dma_handle *handle, void *userData);

/*! @DMA DMA handle structure */
typedef struct _dma_handle
{
    DMA_Type *base;        /*!< DMA peripheral address */
    uint8_t channel;       /*!< DMA channel used */
    dma_callback callback; /*!< DMA callback function.*/
    void *userData;        /*!< Callback parameter */
} dma_handle_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 * @name DMA Initialization and De-initialization
 * @{
 */

/*!
 * @brief Initializes DMA peripheral.
 *
 * This function ungate the DMA clock.
 *
 * @param base DMA peripheral base address.
 */
void DMA_Init(DMA_Type *base);

/*!
 * @brief Deinitialize DMA peripheral.
 *
 * This function gate the DMA clock.
 *
 * @param base DMA peripheral base address.
 */
void DMA_Deinit(DMA_Type *base);

/* @} */
/*!
 * @name DMA Channel Operation
 * @{
 */

/*!
 * @brief Reset DMA channel.
 *
 * Set all register value to reset value and enable
 * cycle steal and auto stop channel request feature.
 *
 * @param base DMA peripheral base address.
 * @param channel DMA channel number.
 */
void DMA_ResetChannel(DMA_Type *base, uint32_t channel);

/*!
 * @brief Configure DMA transfer attribute.
 *
 * This function configure the transfer attribute, including source address,
 * destination address, transfer size and etc.
 * This example shows how to set up the the dma_transfer_config_t
 * parameters and how to call the DMA_ConfigBasicTransfer function.
 * @code
 *   dma_transfer_config_t transferConfig;
 *   memset(&transferConfig, 0, sizeof(transferConfig));
 *   transferConfig.srcAddr = (uint32_t)srcAddr;
 *   transferConfig.destAddr = (uint32_t)destAddr;
 *   transferConfig.enbaleSrcIncrement = true;
 *   transferConfig.enableDestIncrement = true;
 *   transferConfig.srcSize = kDMA_Transfersize32bits;
 *   transferConfig.destSize = kDMA_Transfersize32bits;
 *   transferConfig.transferSize = sizeof(uint32_t) * BUFF_LENGTH;
 *   DMA_SetTransferConfig(DMA0, 0, &transferConfig);
 * @endcode
 *
 * @param base DMA peripheral base address.
 * @param channel DMA channel number.
 * @param config Pointer to DMA transfer configuration structure.
 */
void DMA_SetTransferConfig(DMA_Type *base, uint32_t channel, const dma_transfer_config_t *config);

/*!
 * @brief Configure DMA channel link feature.
 *
 * This function allows DMA channels to have their transfers linked. The current DMA channel
 * triggers a DMA request to the linked channels (LCH1 or LCH2) depending on the channel link
 * type.
 * Perform a link to channel LCH1 after each cycle-steal transfer followed by a link to LCH2
 * after the BCR decrements to 0 if type is kDMA_ChannelLinkChannel1AndChannel2.
 * Perform a link to LCH1 after each cycle-steal transfer if type is kDMA_ChannelLinkChannel1.
 * Perform a link to LCH1 after the BCR decrements to 0 if type is kDMA_ChannelLinkChannel1AfterBCR0.
 *
 * @param base DMA peripheral base address.
 * @param channel DMA channel number.
 * @param config Pointer to channel link configuration structure.
 */
void DMA_SetChannelLinkConfig(DMA_Type *base, uint32_t channel,const  dma_channel_link_config_t *config);

/*!
 * @brief Set DMA source address for DMA transfer.
 *
 * @param base DMA peripheral base address.
 * @param channel DMA channel number.
 * @param srcAddr DMA source address.
 */
static inline void DMA_SetSourceAddress(DMA_Type *base, uint32_t channel, uint32_t srcAddr)
{
    assert(channel < FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    base->DMA[channel].SAR = srcAddr;
}

/*!
 * @brief Set DMA destination address for DMA transfer.
 *
 * @param base DMA peripheral base address.
 * @param channel DMA channel number.
 * @param srcAddr DMA destination address.
 */
static inline void DMA_SetDestinationAddress(DMA_Type *base, uint32_t channel, uint32_t destAddr)
{
    assert(channel < FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    base->DMA[channel].DAR = destAddr;
}

/*!
 * @brief Set DMA transfer size for DMA transfer.
 *
 * @param base DMA peripheral base address.
 * @param channel DMA channel number.
 * @param size The number of bytes to be transferred.
 */
static inline void DMA_SetTransferSize(DMA_Type *base, uint32_t channel, uint32_t size)
{
    assert(channel < FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    base->DMA[channel].DSR_BCR = DMA_DSR_BCR_BCR(size);
}

/*!
 * @brief Set DMA modulo for DMA transfer.
 *
 * This funciton defines a specific address range specified to be the value after (SAR + SSIZE)/(DAR + DSIZE)
 * calculation is performed or the original register value. It provides the ability to implement a circular
 * data queue easily.
 *
 * @param base DMA peripheral base address.
 * @param channel DMA channel number.
 * @param srcModulo source address modulo.
 * @param destModulo destination address modulo.
 */
void DMA_SetModulo(DMA_Type *base, uint32_t channel, dma_modulo_t srcModulo, dma_modulo_t destModulo);

/*!
 * @brief Enable DMA cycle steal for DMA transfer.
 *
 * If enable(true) cycle steal feature, DMA controller forces a single read/write transfer per request.
 * Or it continuously makes read/write transfers until the BCR decrements to 0.
 *
 * @param base DMA peripheral base address.
 * @param channel DMA channel number.
 * @param enable The command for enable(ture) or disable(false).
 */
static inline void DMA_EnableCycleSteal(DMA_Type *base, uint32_t channel, bool enable)
{
    assert(channel < FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    base->DMA[channel].DCR = (base->DMA[channel].DCR & (~DMA_DCR_CS_MASK)) | DMA_DCR_CS(enable);
}

/*!
 * @brief Enable DMA auto align for DMA transfer.
 *
 * If enable(true) auto align feature, the appropriate address register increments,
 * regardless of DINC or SINC.
 *
 * @param base DMA peripheral base address.
 * @param channel DMA channel number.
 * @param enable The command for enable(ture) or disable(false).
 */
static inline void DMA_EnableAutoAlign(DMA_Type *base, uint32_t channel, bool enable)
{
    assert(channel < FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    base->DMA[channel].DCR = (base->DMA[channel].DCR & (~DMA_DCR_AA_MASK)) | DMA_DCR_AA(enable);
}

/*!
 * @brief Enable DMA async request for DMA transfer.
 *
 * If enable(true) async request feature, DMA support asynchronous DREQs
 * while the MCU is in Stop mode.
 *
 * @param base DMA peripheral base address.
 * @param channel DMA channel number.
 * @param enable The command for enable(ture) or disable(false).
 */
static inline void DMA_EnableAsyncRequest(DMA_Type *base, uint32_t channel, bool enable)
{
    assert(channel < FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    base->DMA[channel].DCR = (base->DMA[channel].DCR & (~DMA_DCR_EADREQ_MASK)) | DMA_DCR_EADREQ(enable);
}

/*!
 * @brief Enable interrupt for DMA transfer.
 *
 * @param base DMA peripheral base address.
 * @param channel DMA channel number.
 */
static inline void DMA_EnableInterrupts(DMA_Type *base, uint32_t channel)
{
    assert(channel < FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    base->DMA[channel].DCR |= DMA_DCR_EINT(true);
}

/*!
 * @brief Disable interrupt for DMA transfer.
 *
 * @param base DMA peripheral base address.
 * @param channel DMA channel number.
 */
static inline void DMA_DisableInterrupts(DMA_Type *base, uint32_t channel)
{
    assert(channel < FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    base->DMA[channel].DCR &= ~DMA_DCR_EINT_MASK;
}

/* @} */
/*!
 * @name DMA Channel Transfer Operation
 * @{
 */

/*!
 * @brief Enable DMA hardware channel request.
 *
 * @param base DMA peripheral base address.
 * @param channel The DMA channel number.
 */
static inline void DMA_EnableChannelRequest(DMA_Type *base, uint32_t channel)
{
    assert(channel < FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    base->DMA[channel].DCR |= DMA_DCR_ERQ_MASK;
}

/*!
 * @brief Disable DMA hardware channel request.
 *
 * @param base DMA peripheral base address.
 * @param channel DMA channel number.
 */
static inline void DMA_DisableChannelRequest(DMA_Type *base, uint32_t channel)
{
    assert(channel < FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    base->DMA[channel].DCR &= ~DMA_DCR_ERQ_MASK;
}

/*!
 * @brief Start DMA transfer by software trigger.
 *
 * This funciton start only one read/write iteration.
 *
 * @param base DMA peripheral base address.
 * @param channel The DMA channel number.
 */
static inline void DMA_TriggerChannelStart(DMA_Type *base, uint32_t channel)
{
    assert(channel < FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    base->DMA[channel].DCR |= DMA_DCR_START_MASK;
}

/* @} */
/*!
 * @name DMA Channel Status Operation
 * @{
 */

/*!
 * @brief Get remaining bytes of current DMA transfer.
 *
 * @param base DMA peripheral base address.
 * @param channel DMA channel number.
 * @return The number of bytes have not been transferred yet.
 */
static inline uint32_t DMA_GetRemainingBytes(DMA_Type *base, uint32_t channel)
{
    assert(channel < FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    return (base->DMA[channel].DSR_BCR & DMA_DSR_BCR_BCR_MASK) >> DMA_DSR_BCR_BCR_SHIFT;
}

/*!
 * @brief Get DMA channel status flags.
 *
 * @param base DMA peripheral base address.
 * @param channel DMA channel number.
 * @return The mask of channel status. User need to use the _dma_channel_status_flags
 *         type to decode the return 32 bit variables.
 */
static inline uint32_t DMA_GetChannelStatusFlags(DMA_Type *base, uint32_t channel)
{
    assert(channel < FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    return base->DMA[channel].DSR_BCR;
}

/*!
 * @brief Clear DMA channel status flags.
 *
 * @param base DMA peripheral base address.
 * @param channel DMA channel number.
 * @param mask The mask of channel status to be cleared. User need to use
 *             the defined _dma_channel_status_flags type.
 */
static inline void DMA_ClearChannelStatusFlags(DMA_Type *base, uint32_t channel, uint32_t mask)
{
    assert(channel < FSL_FEATURE_DMAMUX_MODULE_CHANNEL);

    if (mask != 0U)
    {
        base->DMA[channel].DSR_BCR |= DMA_DSR_BCR_DONE(true);
    }
}

/* @} */
/*!
 * @name DMA Channel Transactional Operation
 * @{
 */

/*!
 * @brief Create DMA handle.
 *
 * This function shall be called firstly if using transaction API for DMA. This function
 * initializes the internal state of DMA handle.
 *
 * @param handle DMA handle pointer. The DMA handle stores callback function and
 *               parameters.
 * @param base DMA peripheral base address.
 * @param channel DMA channel number.
 */
void DMA_CreateHandle(dma_handle_t *handle, DMA_Type *base, uint32_t channel);

/*!
 * @brief Set DMA callback function.
 *
 * This callback is called in DMA IRQ handler, Users can use the callback to do something
 * after the current transfer complete.
 *
 * @param handle DMA handle pointer.
 * @param callback DMA callback function pointer.
 * @param param Parameter for callback function. If it is not needed, just set to NULL.
 */
void DMA_SetCallback(dma_handle_t *handle, dma_callback callback, void *userData);

/*!
 * @brief Prepare DMA transfer configuration struct.
 *
 * This function prepare transfer configuration struct according to user input.
 *
 * @param config Pointer to the user configuration structure of type dma_transfer_config_t.
 * @param srcAddr DMA transfer source address.
 * @param srcWidth DMA transfer source address width (byte).
 * @param destAddr DMA transfer destination address.
 * @param destWidth DMA transfer destination address width (byte).
 * @param transferBytes DMA transfer bytes to be transferred.
 * @param type DMA transfer type.
 */
void DMA_PrepareTransfer(dma_transfer_config_t *config,
                         void *srcAddr,
                         uint32_t srcWidth,
                         void *destAddr,
                         uint32_t destWidth,
                         uint32_t transferBytes,
                         dma_transfer_type_t type);

/*!
 * @brief Submit DMA transfer request.
 *
 * This function submit DMA transfer request according to transfer configuration structure.
 *
 * @param handle DMA handle pointer.
 * @param config Pointer to DMA transfer configuration structure.
 * @param options Additional configurations for transfer. User need to use
 *                the defined dma_transfer_options_t type.
 * @retval kStatus_DMA_Success It means DMA submit transfer request succeed.
 * @retval kStatus_DMA_Busy It means DMA is busy. Submit transfer request is not allowed.
 * @note This function can not process multi transfer request.
 */
status_t DMA_SubmitTransfer(dma_handle_t *handle, const dma_transfer_config_t *config, uint32_t options);

/*!
 * @brief DMA start transfer.
 *
 * This function enable channel request. User call this function
 * after submit transfer request.
 *
 * @param handle DMA handle pointer.
 * @retval kStatus_DMA_Success It means DMA start transfer succeed.
 * @retval kStatus_DMA_Busy It means DMA has started transfer.
 */
static inline void DMA_StartTransfer(dma_handle_t *handle)
{
    assert(handle != NULL);

    handle->base->DMA[handle->channel].DCR |= DMA_DCR_ERQ_MASK;
}

/*!
 * @brief DMA stop transfer.
 *
 * This function disable channel request to stop DMA transfer and
 * transfer can be resumed by calling DMA_StartTransfer.
 *
 * @param handle DMA handle pointer.
 */
static inline void DMA_StopTransfer(dma_handle_t *handle)
{
    assert(handle != NULL);

    handle->base->DMA[handle->channel].DCR &= ~DMA_DCR_ERQ_MASK;
}

/*!
 * @brief DMA abort transfer.
 *
 * This function disable channel request and clear all status bits.
 * User can submit another transfer after calling this API.
 *
 * @param handle DMA handle pointer.
 */
void DMA_AbortTransfer(dma_handle_t *handle);

/*!
 * @brief DMA IRQ handler for current transfer complete.
 *
 * This function clears the channel interrupt flag and call
 * the callback function if it is not NULL.
 *
 * @param handle DMA handle pointer.
 */
void DMA_HandleIRQ(dma_handle_t *handle);

/* @} */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/* @}*/

#endif /* _FSL_DMA_H_ */
