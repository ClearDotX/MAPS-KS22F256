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
#ifndef _FSL_LPSCI_H_
#define _FSL_LPSCI_H_

#include "fsl_common.h"

/*!
 * @addtogroup lpsci
 * @{
 */

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief LPSCI driver version */
#define FSL_LPSCI_DRIVER_VERSION (MAKE_VERSION(2, 0, 0)) /*!< Version 2.0.0. */
/*! @brief Error codes for the LPSCI driver. */
enum _lpsci_status
{
    kStatus_LPSCI_TxBusy = MAKE_STATUS(kStatusGroup_LPSCI, 0), /*!< Transmitter is busy. */
    kStatus_LPSCI_RxBusy = MAKE_STATUS(kStatusGroup_LPSCI, 1), /*!< Receiver is busy. */
    kStatus_LPSCI_TxIdle = MAKE_STATUS(kStatusGroup_LPSCI, 2), /*!< Transmitter is idle. */
    kStatus_LPSCI_RxIdle = MAKE_STATUS(kStatusGroup_LPSCI, 3), /*!< Receiver is idle. */
    kStatus_LPSCI_FlagCannotClearManually =
        MAKE_STATUS(kStatusGroup_LPSCI, 2), /*!< Status flag can't be manually cleared. */
    kStatus_LPSCI_BaudrateNotSupport =
        MAKE_STATUS(kStatusGroup_LPSCI, 3),                   /*!< Baudrate is not support in current clock source */
    kStatus_LPSCI_Error = MAKE_STATUS(kStatusGroup_LPSCI, 4), /*!< Error happens on LPSCI */
    kStatus_LPSCI_RxRingBufferOverrun =
        MAKE_STATUS(kStatusGroup_LPSCI, 8),                              /*!< LPSCI RX software ring buffer overrun. */
    kStatus_LPSCI_RxHardwareOverrun = MAKE_STATUS(kStatusGroup_LPSCI, 9) /*!< LPSCI RX receiver overrun. */
};

/*! @brief LPSCI parity mode.*/
typedef enum _lpsci_parity_mode
{
    kLPSCI_ParityDisabled = 0x0U, /*!< Parity disabled */
    kLPSCI_ParityEven = 0x2U,     /*!< Parity enabled, type even, bit setting: PE|PT = 10 */
    kLPSCI_ParityOdd = 0x3U,      /*!< Parity enabled, type odd,  bit setting: PE|PT = 11 */
} lpsci_parity_mode_t;

/*! @brief LPSCI stop bit count.*/
typedef enum _lpsci_stop_bit_count
{
    kLPSCI_OneStopBit = 0U, /*!< One stop bit */
    kLPSCI_TwoStopBit = 1U, /*!< Two stop bits */
} lpsci_stop_bit_count_t;

/*!
 * @brief LPSCI interrupt configuration structure, default settings all disabled.
 *
 * This structure contains the settings for all of the LPSCI interrupt configurations.
 */
enum _lpsci_interrupt_enable_t
{
#if defined(FSL_FEATURE_LPSCI_HAS_LIN_BREAK_DETECT) && FSL_FEATURE_LPSCI_HAS_LIN_BREAK_DETECT
    kLPSCI_LinBreakInterruptEnable = (UART0_BDH_LBKDIE_MASK), /*!< LIN break detect interrupt. */
#endif
    kLPSCI_RxActiveEdgeInterruptEnable = (UART0_BDH_RXEDGIE_MASK),   /*!< RX Active Edge interrupt. */
    kLPSCI_TxDataRegEmptyInterruptEnable = (UART0_C2_TIE_MASK << 8), /*!< Transmit data register empty interrupt. */
    kLPSCI_TransmissionCompleteInterruptEnable = (UART0_C2_TCIE_MASK << 8), /*!< Transmission complete interrupt. */
    kLPSCI_RxDataRegFullInterruptEnable = (UART0_C2_RIE_MASK << 8),  /*!< Receiver data register full interrupt. */
    kLPSCI_IdleLineInterruptEnable = (UART0_C2_ILIE_MASK << 8),      /*!< Idle line interrupt. */
    kLPSCI_RxOverrunInterruptEnable = (UART0_C3_ORIE_MASK << 16),    /*!< Receiver Overrun interrupt. */
    kLPSCI_NoiseErrorInterruptEnable = (UART0_C3_NEIE_MASK << 16),   /*!< Noise error flag interrupt. */
    kLPSCI_FramingErrorInterruptEnable = (UART0_C3_FEIE_MASK << 16), /*!< Framing error flag interrupt. */
    kLPSCI_ParityErrorInterruptEnable = (UART0_C3_PEIE_MASK << 16),  /*!< Parity error flag interrupt. */
};

/*!
 * @brief LPSCI status flags.
 *
 * This provides constants for the LPSCI status flags for use in the LPSCI functions.
 */
enum _lpsci_status_flag_t
{
    kLPSCI_TxDataRegEmptyFlag = (UART0_S1_TDRE_MASK), /*!< Tx data register empty flag, sets when Tx buffer is empty */
    kLPSCI_TransmissionCompleteFlag =
        (UART0_S1_TC_MASK), /*!< Transmission complete flag, sets when transmission activity complete */
    kLPSCI_RxDataRegFullFlag =
        (UART0_S1_RDRF_MASK), /*!< Rx data register full flag, sets when the receive data buffer is full */
    kLPSCI_IdleLineFlag = (UART0_S1_IDLE_MASK), /*!< Idle line detect flag, sets when idle line detected */
    kLPSCI_RxOverrunFlag =
        (UART0_S1_OR_MASK), /*!< Rx Overrun, sets when new data is received before data is read from receive register */
    kLPSCI_NoiseErrorFlag = (UART0_S1_NF_MASK), /*!< Rx takes 3 samples of each received bit. If any of these samples
                                                   differ, noise flag sets */
    kLPSCI_FramingErrorFlag =
        (UART0_S1_FE_MASK), /*!< Frame error flag, sets if logic 0 was detected where stop bit expected */
    kLPSCI_ParityErrorFlag = (UART0_S1_PF_MASK), /*!< If parity enabled, sets upon parity error detection */
#if defined(FSL_FEATURE_LPSCI_HAS_LIN_BREAK_DETECT) && FSL_FEATURE_LPSCI_HAS_LIN_BREAK_DETECT
    kLPSCI_LinBreakFlag =
        (UART0_S2_LBKDIF_MASK
         << 8), /*!< LIN break detect interrupt flag, sets when LIN break char detected and LIN circuit enabled */
#endif
    kLPSCI_RxActiveEdgeFlag =
        (UART0_S2_RXEDGIF_MASK << 8), /*!< Rx pin active edge interrupt flag, sets when active edge detected */
    kLPSCI_RxActiveFlag =
        (UART0_S2_RAF_MASK << 8), /*!< Receiver Active Flag (RAF), sets at beginning of valid start bit */
#if defined(FSL_FEATURE_LPSCI_HAS_EXTENDED_DATA_REGISTER_FLAGS) && FSL_FEATURE_LPSCI_HAS_EXTENDED_DATA_REGISTER_FLAGS
    kLPSCI_NoiseErrorInRxDataRegFlag =
        (UART0_ED_NOISY_MASK << 16), /*!< NOISY bit, sets if noise detected in current data word */
    kLPSCI_ParityErrorInRxDataRegFlag =
        (UART0_ED_PARITYE_MASK << 16), /*!< PARITYE bit, sets if noise detected in current data word */
#endif
};

/*! @brief LPSCI configure structure.*/
typedef struct _lpsci_config
{
    uint32_t baudRate;              /*!< LPSCI baud rate  */
    lpsci_parity_mode_t parityMode; /*!< Parity mode, disabled (default), even, odd */
#if defined(FSL_FEATURE_LPSCI_HAS_STOP_BIT_CONFIG_SUPPORT) && FSL_FEATURE_LPSCI_HAS_STOP_BIT_CONFIG_SUPPORT
    lpsci_stop_bit_count_t stopBitCount; /*!< Number of stop bits, 1 stop bit (default) or 2 stop bits  */
#endif
    bool enableTx; /*!< Enable TX */
    bool enableRx; /*!< Enable RX */
} lpsci_config_t;

/*! @brief LPSCI transfer structure. */
typedef struct _lpsci_transfer
{
    uint8_t *data;   /*!< The buffer of data to be transfer.*/
    size_t dataSize; /*!< The byte count to be transfer. */
} lpsci_transfer_t;

/* Forward declaration of the handle typedef. */
typedef struct _lpsci_handle lpsci_handle_t;

/*! @brief LPSCI transfer callback function. */
typedef void (*lpsci_transfer_callback_t)(UART0_Type *base, lpsci_handle_t *handle, status_t status, void *userData);

/*<! @brief LPSCI handle used for storing the state among transactional APIs' calling. This structure is only used for
 * transactional APIs. */
struct _lpsci_handle
{
    uint8_t *volatile txData;   /*!< Address of remaining data to send. */
    volatile size_t txDataSize; /*!< Size of the remaining data to send. */
    uint8_t *volatile rxData;   /*!< Address of remaining data to receive. */
    volatile size_t rxDataSize; /*!< Size of the remaining data to receive. */

    uint8_t *rxRingBuffer;              /*!< Start address of the receiver ring buffer. */
    size_t rxRingBufferSize;            /*!< Size of the ring buffer. */
    volatile uint16_t rxRingBufferHead; /*!< Index for the driver to store received data into ring buffer. */
    volatile uint16_t rxRingBufferTail; /*!< Index for the user to get data from the ring buffer. */

    lpsci_transfer_callback_t callback; /*!< Callback function. */
    void *userData;                     /*!< LPSCI callback function parameter.*/

    volatile uint8_t txState; /*!< TX transfer state. */
    volatile uint8_t rxState; /*!< RX transfer state */
};

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */

/*!
 * @name Initialization and deinitialization
 * @{
 */

/*!
* @brief Initialize an LPSCI instance with user configuration structure and peripheral clock.
*
* This function configures the LPSCI module wiht user-defined settings. User can configure the configuration
* structure themselves and user can also get the defaul configuration by LPSCI_GetDefaultConfig() function.
* Example below show how to use this API to configure the LPSCI.
*  @code
*   lpsci_config_t lpsciConfig;
*   lpsciConfig.baudRate = 115200U;
*   lpsciConfig.parityMode = kLPSCI_ParityDisabled;
*   lpsciConfig.stopBitCount = kLPSCI_OneStopBit;
*   LPSCI_Init(UART0, &lpsciConfig, 20000000U);
*   @endcode
*
* @param base LPSCI peripheral base address.
* @param config Pointer to user-defined configuration structure.
* @param srcClock_Hz LPSCI clock source freqency in HZ.
* @retval kStatus_LPSCI_BaudrateNotSupport Baudrate is not support in current clock source.
* @retval kStatus_Success Status LPSCI initialize succeed
*/
status_t LPSCI_Init(UART0_Type *base, const lpsci_config_t *config, uint32_t srcClock_Hz);

/*!
 * @brief Deinitializes an LPSCI instance.
 *
 * This function waits for TX complete, disables TX and RX, then disables the LPSCI clock.
 *
 * @param base LPSCI peripheral base address.
 */
void LPSCI_Deinit(UART0_Type *base);

/*!
 * @brief Get the default configuration structure and save the configuration to user provided pointer.
 *
 * This function initializes the LPSCI configure structure to default value. the default
 * value are:
 *   lpsciConfig->baudRate = 115200U;
 *   lpsciConfig->parityMode = kLPSCI_ParityDisabled;
 *   lpsciConfig->stopBitCount = kLPSCI_OneStopBit;
 *   lpsciConfig->enableTx = false;
 *   lpsciConfig->enableRx = false;
 *
 * @param config Pointer to configuration structure.
 */
void LPSCI_GetDefaultConfig(lpsci_config_t *config);

/* @} */

/*!
 * @name Status
 * @{
 */

/*!
 * @brief Get LPSCI status flags.
 *
 * This function get all LPSCI status flags, the flags are returned as the logical
 * OR value of the enumerators @ref _lpsci_flags. To check specific status,
 * compare the return value with enumerators in @ref _LPSCI_flags.
 * For example, to check whether the TX is empty:
  * @code
 *     if (kLPSCI_TxDataRegEmptyFlag | LPSCI_GetStatusFlags(UART0))
 *     {
 *         ...
 *     }
 * @endcode
 *
 * @param base LPSCI peripheral base address.
 * @return LPSCI status flags which are ORed by the enumerators in the _lpsci_flags.
 */
uint32_t LPSCI_GetStatusFlags(UART0_Type *base);

/* @brief Clear status flags with provide mask.
*
* This function clears LPSCI status flags with provided mask. Automatically cleared flag
* can't be cleared by this function.
* Some flags can only clear or set by the hardware itself, these flags are:
*    kLPSCI_TxDataRegEmptyFlag,kLPSCI_TransmissionCompleteFlag,kLPSCI_RxDataRegFullFlag,kLPSCI_RxActiveFlag,kLPSCI_NoiseErrorInRxDataRegFlag
*    kLPSCI_ParityErrorInRxDataRegFlag,kLPSCI_TxFifoEmptyFlag,kLPSCI_RxFifoEmptyFlag
*
* @param base LPSCI peripheral base address.
* @param mask The status flags to be cleared, it is logical OR value of @ref _LPSCI_flagss.
* @retval kStatus_LPSCI_FlagCannotClearManually The flag can't be cleared by this function but it is cleared
* automatically by hardware.
*       it is cleared automatically by hardware.
* @retval kStatus_Success Status in the mask are cleared.
*/
status_t LPSCI_ClearStatusFlags(UART0_Type *base, uint32_t mask);

/* @} */

/*!
 * @name Interrupts
 * @{
 */

/*!
* @brief Enable LPSCI interrupt according to provided mask.
*
* This function enables the LPSCI interrupts according to provided mask. The mask
* is a logical OR of enumeration members, see @ref _lpsci_interrupt_enable.
* For example, to enable TX empty interrupt and RX full interrupt, do like this.
* @code
*     LPSCI_EnableInterrupts(UART0,kLPSCI_TxDataRegEmptyInterruptEnable | kLPSCI_RxDataRegFullInterruptEnable);
* @endcode
*
* @param base LPSCI peripheral base address.
* @param mask The interrupts to enable. Logical OR of @ref _lpsci_interrupt_enable.
*/
void LPSCI_EnableInterrupts(UART0_Type *base, uint32_t mask);

/*!
 * @brief Disable LPSCI interrupt according to provided mask.
 *
 * This function disables the LPSCI interrupts according to provided mask. The mask
 * is a logical OR of enumeration members, see @ref _lpsci_interrupt_enable.
 * For example, to disable TX empty interrupt and RX full interrupt, do like this.
 * @code
 *     LPSCI_DisableInterrupts(UART0,kLPSCI_TxDataRegEmptyInterruptEnable | kLPSCI_RxDataRegFullInterruptEnable);
 * @endcode
 *
 * @param base LPSCI peripheral base address.
 * @param mask The interrupts to disable. Logical OR of @ref _LPSCI_interrupt_enable.
 */
void LPSCI_DisableInterrupts(UART0_Type *base, uint32_t mask);

/*!
 * @brief Get enabled LPSCI interrupts.
 *
 * This function get enabled LPSCI interrupts, the enabled interrupts are returned
 * as the logical OR value of the enumerators @ref _lpsci_interrupt_enable. To check
 * specific interrupts enable status, compare the return value with enumerators
 * in @ref _LPSCI_interrupt_enable.
 * For example, to check whether TX empty interrupt is enabled:
 * @code
 *     uint32_t enabledInterrupts = LPSCI_GetEnabledInterrupts(UART0);
 *
 *     if (kLPSCI_TxDataRegEmptyInterruptEnable & enabledInterrupts)
 *     {
 *         ...
 *     }
 * @endcode
 *
 * @param base LPSCI peripheral base address.
 * @return LPSCI interrupt flags which are logical OR of the enumerators in @ref _LPSCI_interrupt_enable.
 */
uint32_t LPSCI_GetEnabledInterrupts(UART0_Type *base);

/* @} */

#if defined(FSL_FEATURE_LPSCI_HAS_DMA_ENABLE) && FSL_FEATURE_LPSCI_HAS_DMA_ENABLE

/*!
 * @name DMA Control
 * @{
 */

/*!
 * @brief get LPSCI data register address
 *
 * This function return the LPSCI data register address, which is mainly used by DMA/eDMA case.
 *
 * @param base LPSCI peripheral base address.
 * @return LPSCI data register address which are used both by transmitter and receiver.
 */
static inline uint32_t LPSCI_GetDataRegisterAddress(UART0_Type *base)
{
    return (uint32_t) & (base->D);
}

/*!
 * @brief Enable or disable LPSCI transmiter DMA request.
 *
 * This function enables or disables the transmit data register empty flag, S1[TDRE], to generate DMA requests.
 *
 * @param base LPSCI peripheral base address.
 * @param enable True to enable, false to disable.
 */
static inline void LPSCI_EnableTxDMA(UART0_Type *base, bool enable)
{
    if (enable)
    {
        base->C5 |= UART0_C5_TDMAE_MASK;
        base->C2 |= UART0_C2_TIE_MASK;
    }
    else
    {
        base->C5 &= ~UART0_C5_TDMAE_MASK;
        base->C2 &= ~UART0_C2_TIE_MASK;
    }
}

/*!
 * @brief Enable or disable LPSCI receiver DMA
 *
 * This function enables or disables the receiver data register full flag, S1[RDRF], to generate DMA requests.
 *
 * @param base LPSCI peripheral base address.
 * @param enable True to enable, false to disable.
 */
static inline void LPSCI_EnableRxDMA(UART0_Type *base, bool enable)
{
    if (enable)
    {
        base->C5 |= UART0_C5_RDMAE_MASK;
        base->C2 |= UART0_C2_RIE_MASK;
    }
    else
    {
        base->C5 &= ~UART0_C5_RDMAE_MASK;
        base->C2 &= ~UART0_C2_RIE_MASK;
    }
}

/* @} */
#endif /* defined(FSL_FEATURE_LPSCI_HAS_DMA_ENABLE) && FSL_FEATURE_LPSCI_HAS_DMA_ENABLE */

/*!
 * @name Bus Operations
 * @{
 */

/*!
 * @brief Enable or disable LPSCI transmitter.
 *
 * This function enables or disables the LPSCI transmitter.
 *
 * @param base LPSCI peripheral base address.
 * @param enable True to enable, false to disable.
 */
static inline void LPSCI_EnableTx(UART0_Type *base, bool enable)
{
    if (enable)
    {
        base->C2 |= UART0_C2_TE_MASK;
    }
    else
    {
        base->C2 &= ~UART0_C2_TE_MASK;
    }
}

/*!
 * @brief Enable or disable LPSCI receiver.
 *
 * This function enables or disables the LPSCI receiver.
 *
 * @param base LPSCI peripheral base address.
 * @param enable True to enable, false to disable.
 */
static inline void LPSCI_EnableRx(UART0_Type *base, bool enable)
{
    if (enable)
    {
        base->C2 |= UART0_C2_RE_MASK;
    }
    else
    {
        base->C2 &= ~UART0_C2_RE_MASK;
    }
}

/*!
 * @brief Write to TX register using blocking method.
 *
 * This function polls the TX register, waits for the TX register empty
 * then writes data to the TX buffer.
 *
 * @note This function does not check whether all the data has been sent out to bus,
 * so before disable TX, check kLPSCI_TransmissionCompleteFlag to ensure the TX is
 * finished.
 *
 * @param base LPSCI peripheral base address.
 * @param data Start addresss of the data to write.
 * @param length Size of the data to write.
 */
void LPSCI_WriteBlocking(UART0_Type *base, const uint8_t *data, size_t length);

/*!
 * @brief Write to TX register using non-blocking method.
 *
 * This function writes data to the TX register directly, upper layer must make
 * sure the TX register is empty before calling this function.
 *
 * @note This function does not check whether all the data has been sent out to bus,
 * so before disable TX, check kLPSCI_TransmissionCompleteFlag to ensure the TX is
 * finished.
 *
 * @param base LPSCI peripheral base address.
 * @param data Start addresss of the data to write.
 * @param length Size of the buffer to be sent.
 */
void LPSCI_WriteNonBlocking(UART0_Type *base, const uint8_t *data, size_t length);

/*!
 * @brief Read RX data register using blocking method.
 *
 * This function polls the RX register, waits for the RX register full
 * then read data from TX register.
 *
 * @param base LPSCI peripheral base address.
 * @param data Start addresss of the buffer to store the received data.
 * @param length Size of the buffer.
 */
void LPSCI_ReadNonBlocking(UART0_Type *base, uint8_t *data, size_t length);
/*!
* @brief Read RX register using non-blocking method.
*
* This function reads data from the TX register directly, upper layer must make
* sure the RX register is full before calling this function.
*
* @param base LPSCI peripheral base address.
* @param data Start addresss of the buffer to store the received data.
* @param length Size of the buffer.
*/
void LPSCI_ReadBlocking(UART0_Type *base, uint8_t *data, size_t length);

/* @} */

/*!
 * @name Transactional
 * @{
 */

/*!
 * @brief Initialize the LPSCI handle.
 *
 * This function initializes the LPSCI handle which can be used for other LPSCI
 * transactional APIs. Usually, for a specified LPSCI instance, user only need
 * to call this API once to get the initialized handle.
 *
 * LPSCI driver supports the "backaround" receiveing, which means that user could setup
 * a RX ring buffer optionally. Data received are stored into the ring buffer even that
 * user don't call the LPSCI_ReceiveNonBlocking() API. If there are already data received
 * in the ring buffer, user can get the received data from the ring buffer directly.
 * The ring buffer is disabled if pass NULL as @p ringBuffer.
 *
 * @param handle LPSCI handle pointer.
 * @param base LPSCI peripheral base address.
 * @param ringBuffer Start address of ring buffer for backaround receiveing. Pass NULL to disable the ring buffer.
 * @param ringBufferSize size of the ringbuffer.
 */
void LPSCI_CreateHandle(UART0_Type *base, lpsci_handle_t *handle, lpsci_transfer_callback_t callback, void *userData);

/*!
 * @brief Set up the RX ring buffer.
 *
 * This function sets up the RX ring buffer to specific LPSCI handle.
 *
 * When RX ring buffer is used, data received are stored into the ring buffer even that
 * user don't call the LPSCI_ReceiveNonBlocking() API. If there are already data received
 * in the ring buffer, user can get the received data from the ring buffer directly.
 *
 * @note When using RX ring buffer, one byte is reserved for internal use. In other
 * words, if @p ringBufferSize is 32, then only 31 bytes are used for saving data.
 *
 * @param base LPSCI peripheral base address.
 * @param handle LPSCI handle pointer.
 * @param ringBuffer Start address of ring buffer for backaround receiveing. Pass NULL to disable the ring buffer.
 * @param ringBufferSize size of the ringbuffer.
 */
void LPSCI_InstallRingBuffer(UART0_Type *base, lpsci_handle_t *handle, uint8_t *ringBuffer, size_t ringBufferSize);

/*!
 * @brief Abort the background transfer and uninstall ring buffer.
 *
 * This function abort the background transfer and uninstall the ringbuffer.
 *
 * @param base LPSCI peripheral base address.
 * @param handle LPSCI handle pointer.
 */
void LPSCI_UninstallRingBuffer(UART0_Type *base, lpsci_handle_t *handle);

/*!
 * @brief Transmit a buffer of data using the interrupt method
 *
 * This function send data using interrupt method. This is non-blocking function,
 * returns directly without waiting for all data written to TX register. When
 * all data are written to TX register in ISR, LPSCI driver calls the callback
 * function and pass @ref kStatus_LPSCI_TxIdle as status parameter.
 *
 * @note The kStatus_LPSCI_TxIdle is passed to upper layer when all data written
 * to TX register, but not ensure all the data sent out. So before disable TX,
 * check kLPSCI_TransmissionCompleteFlag to ensure the TX is finished.
 *
 * @param handle LPSCI handle pointer.
 * @param xfer LPSCI transfer sturcture, refer to #LPSCI_transfer_t.
 * @retval kStatus_Success Sucessully start the data transmission.
 * @retval kStatus_LPSCI_TxBusy Previous transmission still not finished, data not all written to TX register yet.
 */
status_t LPSCI_SendNonBlocking(UART0_Type *base, lpsci_handle_t *handle, lpsci_transfer_t *xfer);

/*!
 * @brief Abort interrupt driven data transmit.
 *
 * This function aborts interrupt driven data sending.
 *
 * @param handle LPSCI handle pointer.
 */
void LPSCI_AbortSend(UART0_Type *base, lpsci_handle_t *handle);

/*!
 * @brief Get the number of remaining bytes not send.
 *
 * This function gets the number of remaining bytes not send driven by interrupt.
 *
 * @param handle LPSCI handle pointer.
 * @return The number of bytes not send.
 */
static inline size_t LPSCI_GetSendRemainingBytes(UART0_Type *base, lpsci_handle_t *handle)
{
    return handle->txDataSize;
}

/*!
 * @brief Receive a buffer of data using the interrupt method.
 *
 * This function receives data using interrupt method. This is non-blocking function
 * and will return without necessarily wait all data are received.
 * If RX ring buffer is used and not empty, the data in ring buffer is copied and
 * the parameter @p receivedBytes shows how many bytes are copied from ring buffer.
 * After copy, if the data in ring buffer is not enough for read, the receive
 * request is saved by LPSCI driver, when new data arrived, the receive request
 * is serviced first. When all data received, LPSCI driver notifies upper layer
 * through callback function, pass status parameter @ref kStatus_LPSCI_RxIdle.
 * For example, upper layer needs 10 bytes but there are only 5 bytes in ring buffer,
 * then the 5 bytes are copied to xfer->data, this function returns with the
 * parameter @p receivedBytes set to 5. For the left 5 bytes, new arrived data is
 * saved from xfer->data[5], when 5 bytes received, LPSCI driver notifies upper layer.
 * If RX ring buffer is not enabled, this function enable RX and RX interrupt
 * to receive data to xfer->data. When all data received, upper layer is notified.
 *
 * @param handle LPSCI handle pointer.
 * @param xfer lpsci transfer sturcture, refer to #lpsci_transfer_t.
 * @param receivedBytes Bytes received from the ring buffer directly.
 * @retval kStatus_Success Sucessully queue the transfer into transmit queue.
 * @retval kStatus_LPSCI_RxBusy Previous receive requst is not finished.
 */
status_t LPSCI_ReceiveNonBlocking(UART0_Type *base,
                                  lpsci_handle_t *handle,
                                  lpsci_transfer_t *xfer,
                                  size_t *receivedBytes);

/*!
 * @brief Abort interrupt driven data receiving.
 *
 * This function aborts interrupt driven data receiving.
 *
 * @param handle LPSCI handle pointer.
 */
void LPSCI_AbortReceive(UART0_Type *base, lpsci_handle_t *handle);

/*!
 * @brief Get the number of remaining bytes not received.
 *
 * This function gets the number of remaining bytes not received driven by interrupt.
 *
 * @param handle LPSCI handle pointer.
 * @return The number of bytes not received.
 */
static inline size_t LPSCI_GetReceiveRemainingBytes(UART0_Type *base, lpsci_handle_t *handle)
{
    return handle->rxDataSize;
}

/*!
 * @brief LPSCI IRQ handle function
 *
 * This function handles the LPSCI transmit and receive IRQ request.
 *
 * @param handle LPSCI handle pointer.
 */
void LPSCI_HandleIRQ(UART0_Type *base, lpsci_handle_t *handle);

/*!
 * @brief LPSCI Error IRQ handle function
 *
 * This function handle the LPSCI error IRQ request.
 *
 * @param handle LPSCI handle pointer.
 */
void LPSCI_HandleErrorIRQ(UART0_Type *base, lpsci_handle_t *handle);

/* @} */

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* _FSL_LPSCI_H_ */
