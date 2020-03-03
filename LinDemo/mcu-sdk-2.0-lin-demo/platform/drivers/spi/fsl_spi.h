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
#ifndef _FSL_SPI_H_
#define _FSL_SPI_H_

#include "fsl_common.h"

/*!
 * @addtogroup spi
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Driver version of SPI */
#define FSL_SPI_DRIVER_VERSION (MAKE_VERSION(2, 0, 0)) /*!< Version 2.0.0 */

/*! @brief SPI dummy transfer data, the data is sent while txBuff is NULL. */
#define SPI_DUMMYDATA (0xFFU)

/*! @brief Return status for the SPI driver.*/
enum _spi_status
{
    kStatus_SPI_Busy = MAKE_STATUS(kStatusGroup_SPI, 0), /*!< SPI bus is busy */
    kStatus_SPI_Idle = MAKE_STATUS(kStatusGroup_SPI, 1), /*!< SPI is idle */
    kStatus_SPI_Error = MAKE_STATUS(kStatusGroup_SPI, 2) /*!< SPI  error */
};

/*! @brief SPI clock polarity configuration.*/
typedef enum _spi_clock_polarity
{
    kSPI_ClockPolarityActiveHigh = 0x0U, /*!< Active-high SPI clock (idles low). */
    kSPI_ClockPolarityActiveLow          /*!< Active-low SPI clock (idles high). */
} spi_clock_polarity_t;

/*! @brief SPI clock phase configuration.*/
typedef enum _spi_clock_phase
{
    kSPI_ClockPhaseFirstEdge = 0x0U, /*!< First edge on SPSCK occurs at the middle of the first
                                      *   cycle of a data transfer. */
    kSPI_ClockPhaseSecondEdge        /*!< First edge on SPSCK occurs at the start of the
                                      *   first cycle of a data transfer. */
} spi_clock_phase_t;

/*! @brief SPI data shifter direction options.*/
typedef enum _spi_shift_direction
{
    kSPI_MsbFirst = 0x0U, /*!< Data transfers start with most significant bit. */
    kSPI_LsbFirst         /*!< Data transfers start with least significant bit. */
} spi_shift_direction_t;

/*! @brief SPI slave select output mode options.*/
typedef enum _spi_ss_output_mode
{
    kSPI_SlaveSelectAsGpio = 0x0U,         /*!< Slave select pin configured as GPIO. */
    kSPI_SlaveSelectFaultInput = 0x2U,     /*!< Slave select pin configured for fault detection. */
    kSPI_SlaveSelectAutomaticOutput = 0x3U /*!< Slave select pin configured for automatic SPI output. */
} spi_ss_output_mode_t;

/*! @brief SPI pin mode options.*/
typedef enum _spi_pin_mode
{
    kSPI_PinModeNormal = 0x0U, /*!< Pins operate in normal, single-direction mode.*/
    kSPI_PinModeInput = 0x1U,  /*!< Bidirectional mode. Master: MOSI pin is input;
                                *   Slave: MISO pin is input. */
    kSPI_PinModeOutput = 0x3U  /*!< Bidirectional mode. Master: MOSI pin is output;
                                *   Slave: MISO pin is output. */
} spi_pin_mode_t;

/*! @brief SPI data length mode options.*/
typedef enum _spi_data_bitcount_mode
{
    kSPI_8BitMode = 0x0U, /*!< 8-bit data transmission mode*/
    kSPI_16BitMode        /*!< 16-bit data transmission mode*/
} spi_data_bitcount_mode_t;

/*! @brief SPI interrupt sources.*/
enum _spi_interrupt_enable
{
    kSPI_RxFullAndModfInterruptEnable = 0x1U, /*!< Receive buffer full (SPRF) and mode fault (MODF) interrupt */
    kSPI_TxEmptyInterruptEnable = 0x2U,       /*!< Transmit buffer empty interrupt */
    kSPI_MatchInterruptEnable = 0x4U,         /*!< Match interrupt */
#if defined(FSL_FEATURE_SPI_FIFO_SIZE) && FSL_FEATURE_SPI_FIFO_SIZE
    kSPI_RxFifoNearFullInterruptEnable = 0x8U,   /*!< Receive FIFO nearly full interrupt */
    kSPI_TxFifoNearEmptyInterruptEnable = 0x10U, /*!< Transmit FIFO nearly empty interrupt */
#endif                                           /* FSL_FEATURE_SPI_FIFO_SIZE */
};

/*! @brief SPI status flags.*/
enum _spi_flags
{
    kSPI_RxBufferFullFlag = SPI_S_SPRF_MASK,   /*!< Read buffer full flag */
    kSPI_MatchFlag = SPI_S_SPMF_MASK,          /*!< Match flag */
    kSPI_TxBufferEmptyFlag = SPI_S_SPTEF_MASK, /*!< Transmit buffer empty flag */
    kSPI_ModeFaultFlag = SPI_S_MODF_MASK,      /*!< Mode fault flag */
#if defined(FSL_FEATURE_SPI_FIFO_SIZE) && FSL_FEATURE_SPI_FIFO_SIZE
    kSPI_RxFifoNearFullFlag = SPI_S_RNFULLF_MASK,  /*!< Rx FIFO near full */
    kSPI_TxFifoNearEmptyFlag = SPI_S_TNEAREF_MASK, /*!< Tx FIFO near empty */
    kSPI_RxFifoFullFlag = SPI_S_TXFULLF_MASK,      /*!< Rx fifo full */
    kSPI_TxFifoEmptyFlag = SPI_S_RFIFOEF_MASK,     /*!< Tx fifo empty */
    kSPI_TxFifoError = SPI_CI_TXFERR_MASK << 8U,   /*!< Tx FIFO error */
    kSPI_RxFifoError = SPI_CI_RXFERR_MASK << 8U,   /*!< Rx FIFO Overflow */
    kSPI_TxOverflow = SPI_CI_TXFOF_MASK << 8U,     /*!< Tx FIFO Overflow */
    kSPI_RxOverflow = SPI_CI_RXFOF_MASK << 8U      /*!< Rx FIFO Overflow */
#endif                                             /* FSL_FEATURE_SPI_FIFO_SIZE */
};

#if defined(FSL_FEATURE_SPI_FIFO_SIZE) && FSL_FEATURE_SPI_FIFO_SIZE
/*! @brief SPI FIFO write-1-to-clear interrupt flags.*/
typedef enum _spi_w1c_interrupt
{
    kSPI_RxFifoFullClearInterrupt = SPI_CI_SPRFCI_MASK,    /*!< Receive FIFO full interrupt */
    kSPI_TxFifoEmptyClearInterrupt = SPI_CI_SPTEFCI_MASK,  /*!< Transmit FIFO empty interrupt */
    kSPI_RxNearFullClearInterrupt = SPI_CI_RNFULLFCI_MASK, /*!< Receive FIFO nearly full interrupt */
    kSPI_TxNearEmptyClearInterrupt = SPI_CI_TNEAREFCI_MASK /*!< Transmit FIFO nearly empty interrupt */
} spi_w1c_interrupt_t;

/*! @brief SPI TX FIFO watermark settings.*/
typedef enum _spi_txfifo_watermark
{
    kSPI_TxFifoOneFourthEmpty = 0, /*!< SPI tx watermark at 1/4 FIFO size */
    kSPI_TxFifoOneHalfEmpty = 1    /*!< SPI tx watermark at 1/2 FIFO size */
} spi_txfifo_watermark_t;

/*! @brief SPI RX FIFO watermark settings.*/
typedef enum _spi_rxfifo_watermark
{
    kSPI_RxFifoThreeFourthsFull = 0, /*!< SPI rx watermark at 3/4 FIFO size */
    kSPI_RxFifoOneHalfFull = 1       /*!< SPI rx watermark at 1/2 FIFO size */
} spi_rxfifo_watermark_t;
#endif /* FSL_FEATURE_SPI_FIFO_SIZE */

#if defined(FSL_FEATURE_SPI_HAS_DMA_SUPPORT) && FSL_FEATURE_SPI_HAS_DMA_SUPPORT
/*! @brief SPI dma source*/
enum _spi_dma_enable_t
{
    kSPI_TxDmaEnable = SPI_C2_TXDMAE_MASK,                        /*!< Tx dma request source */
    kSPI_RxDmaEnable = SPI_C2_RXDMAE_MASK,                        /*!< Rx dma request source */
    kSPI_DmaAllEnable = (SPI_C2_TXDMAE_MASK | SPI_C2_RXDMAE_MASK) /*!< All dma request source*/
};
#endif /* FSL_FEATURE_SPI_HAS_DMA_SUPPORT */

/*! @brief SPI master user configure structure.*/
typedef struct _spi_master_config
{
    bool enableMaster;               /*!< Enable SPI at init time */
    bool enableStopInWaitMode;       /*!< SPI stop in wait mode */
    spi_clock_polarity_t polarity;   /*!< Clock polarity */
    spi_clock_phase_t phase;         /*!< Clock phase */
    spi_shift_direction_t direction; /*!< MSB or LSB */
#if defined(FSL_FEATURE_SPI_16BIT_TRANSFERS) && FSL_FEATURE_SPI_16BIT_TRANSFERS
    spi_data_bitcount_mode_t dataMode; /*!< 8bit or 16bit mode */
#endif                                 /* FSL_FEATURE_SPI_16BIT_TRANSFERS */
#if defined(FSL_FEATURE_SPI_FIFO_SIZE) && FSL_FEATURE_SPI_FIFO_SIZE
    bool enableFifoInterruptClear;      /*!< Enable interrupt clear mechanism */
    spi_txfifo_watermark_t txWatermark; /*!< Tx watermark settings */
    spi_rxfifo_watermark_t rxWatermark; /*!< Rx watermark settings */
#endif                                  /* FSL_FEATURE_SPI_FIFO_SIZE */
    spi_ss_output_mode_t outputMode;    /*!< SS pin setting */
    spi_pin_mode_t pinMode;             /*!< SPI pin mode select */
    uint32_t baudRate_Bps;              /*!< Baud Rate for SPI in Hz */
} spi_master_config_t;

/*! @brief SPI slave user configure structure.*/
typedef struct _spi_slave_config
{
    bool enableSlave;                /*!< Enable SPI at init time */
    bool enableStopInWaitMode;       /*!< SPI stop in wait mode */
    spi_clock_polarity_t polarity;   /*!< Clock polarity */
    spi_clock_phase_t phase;         /*!< Clock phase */
    spi_shift_direction_t direction; /*!< MSB or LSB */
#if defined(FSL_FEATURE_SPI_16BIT_TRANSFERS) && FSL_FEATURE_SPI_16BIT_TRANSFERS
    spi_data_bitcount_mode_t dataMode; /*!< 8bit or 16bit mode */
#endif                                 /* FSL_FEATURE_SPI_16BIT_TRANSFERS */
#if defined(FSL_FEATURE_SPI_FIFO_SIZE) && FSL_FEATURE_SPI_FIFO_SIZE
    bool enableFifoInterruptClear;      /*!< Enable interrupt clear mechanism */
    spi_txfifo_watermark_t txWatermark; /*!< Tx watermark settings */
    spi_rxfifo_watermark_t rxWatermark; /*!< Rx watermark settings */
#endif                                  /* FSL_FEATURE_SPI_FIFO_SIZE */
} spi_slave_config_t;

/*! @brief SPI transfer structure */
typedef struct _spi_transfer
{
    uint8_t *sendBuff;    /*!< Send buffer */
    uint8_t *receiveBuff; /*!< Receive buffer */
    size_t size;          /*!< Transfer bytes */
    uint32_t flags;       /*!< SPI control flag, useless to SPI.*/
} spi_transfer_t;

typedef struct _spi_master_handle spi_master_handle_t;

/*! @brief  Slave handle is the same with master handle  */
typedef spi_master_handle_t spi_slave_handle_t;

/*! @brief SPI master callback for finished transmit */
typedef void (*spi_master_callback_t)(spi_master_handle_t *handle, status_t status, void *userData);

/*! @brief SPI master callback for finished transmit */
typedef void (*spi_slave_callback_t)(spi_slave_handle_t *handle, status_t status, void *userData);

/*! @brief SPI transfer handle structure */
struct _spi_master_handle
{
    SPI_Type *base;                   /*!< SPI peripheral base pointer */
    uint8_t *volatile txBuff;         /*!< Transfer buffer */
    uint8_t *volatile rxBuff;         /*!< Receive buffer */
    volatile size_t txRemainingBytes; /*!< Send data remaining in bytes */
    volatile size_t rxRemainingBytes; /*!< Receive data remaining in bytes */
    volatile uint32_t state;          /*!< SPI interanl state */
    uint8_t bytePerFrame;             /*!< SPI mode, 2bytes or 1byte in a frame */
    uint8_t watermark;                /*!< Watermark value for SPI transfer */
    spi_master_callback_t callback;   /*!< SPI callback */
    void *userData;                   /*!< Cllback parameter */
};

#if defined(__cplusplus)
extern "C" {
#endif
/*******************************************************************************
 * APIs
 ******************************************************************************/
/*!
 * @name Initialization and deinitialization
 * @{
 */

/*!
 * @brief  Set the SPI master configuration structure to default values.
 *
 * The purpose of this API is to get the config structure initialized for use in SPI_MasterInit().
 * User may use the initialized structure unchanged in SPI_MasterInit(), or modify
 * some fields of the structure before calling SPI_MasterInit(). After calling this API,
 * The master is ready to transfer.
 * Example:
   @code
   spi_master_config_t config;
   SPI_MasterGetDefaultConfig(&config);
   @endcode
 *
 * @param userConfig pointer to master config structure
 */
void SPI_MasterGetDefaultConfig(spi_master_config_t *config);

/*!
 * @brief Initialize SPI with master configuration.
 *
 * The configuration structure can be filled by user from scratch, or be set with default
 * values by SPI_MasterGetDefaultConfig(). After calling this API, the slave is ready to transfer.
 * Example
   @code
   spi_master_config_t config = {
   .baudRate_Bps = 400000,
   ...
   };
   SPI_MasterInit(SPI0, &config);
   @endcode
 *
 * @param base SPI base pointer
 * @param userConfig pointer to master config structure
 */
void SPI_MasterInit(SPI_Type *base, const spi_master_config_t *config, uint32_t sourceClock_Hz);

/*!
 * @brief  Set the SPI slave configuration structure to default values.
 *
 * The purpose of this API is to get the config structure initialized for use in SPI_SlaveInit().
 * User need to modify some fields of the structure before calling SPI_SlaveInit().
 * Example:
   @code
   spi_slave_config_t config;
   SPI_SlaveGetDefaultConfig(&config);
   @endcode
 *
 * @param userConfig pointer to slave config structure
 */
void SPI_SlaveGetDefaultConfig(spi_slave_config_t *config);

/*!
 * @brief Initialize SPI with slave configuration.
 *
 * The configuration structure can be filled by user from scratch, or be set with
 * default values by SPI_SlaveGetDefaultConfig().
 * After calling this API, the slave is ready to transfer.
 * Example
   @code
    spi_slave_config_t config = {
    .polarity = kSPIClockPolarity_ActiveHigh;
    .phase = kSPIClockPhase_FirstEdge;
    .direction = kSPIMsbFirst;
    ...
    };
    SPI_MasterInit(SPI0, &config);
   @endcode
 *
 * @param base SPI base pointer
 * @param userConfig pointer to master config structure
 */
void SPI_SlaveInit(SPI_Type *base, const spi_slave_config_t *config);

/*!
 * @brief De-initialize SPI.
 *
 * Call this API will reset the SPI module, gate the spi clock ,
 * so the SPI module can not work unless call SPI_MasterInit/SPI_SlaveInit to initialize module.
 *
 * @param base SPI base pointer
 */
void SPI_Deinit(SPI_Type *base);

/*!
 * @brief Enable or disables the SPI.
 *
 * @param base SPI base pointer
 * @param enable pass true to enable module, false to disable module
 */
static inline void SPI_Enable(I2C_Type *base, bool enable)
{
    if (enable)
    {
        base->C1 |= SPI_C1_SPE_MASK;
    }
    else
    {
        base->C1 &= ~SPI_C1_SPE_MASK;
    }
}

/*! @} */

/*!
 * @name Status
 * @{
 */

/*!
 * @brief Get the status flag.
 *
 * @param base SPI base pointer
 * @return SPI Status, use status flag to AND #_spi_flags could get the related status.
 */
uint32_t SPI_GetStatusFlags(SPI_Type *base);

#if defined(FSL_FEATURE_SPI_FIFO_SIZE) && FSL_FEATURE_SPI_FIFO_SIZE
/*!
 * @brief Clear the interrupt if enable INCTLR.
 *
 * @param base SPI base pointer
 * @param interrupt Interrupt need to be cleared
 *      The parameter could be any combination of the following values:
 *          @arg kSPIRxFifoFullClearInt
 *          @arg kSPITxFifoEmptyClearInt
 *          @arg kSPIRxNearFullClearInt
 *          @arg kSPITxNearEmptyClearInt
 */
static inline void SPI_ClearInterrupt(SPI_Type *base, uint32_t mask)
{
    base->CI |= mask;
}
#endif /* FSL_FEATURE_SPI_FIFO_SIZE */

/*! @} */

/*!
 * @name Interrupts
 * @{
 */

/*!
 * @brief Enable interrupt for SPI.
 *
 * @param base SPI base pointer
 * @param mask SPI interrupt source. The parameter could be any combination of the following values:
 *        @arg kSPI_RxFullAndModfInterruptEnable
 *        @arg kSPI_TxEmptyInterruptEnable
 *        @arg kSPI_MatchInterruptEnable
 *        @arg kSPI_RxFifoNearFullInterruptEnable
 *        @arg kSPI_TxFifoNearEmptyInterruptEnable
 */
void SPI_EnableInterrupts(SPI_Type *base, uint32_t mask);

/*!
 * @brief Disable interrupt for SPI.
 *
 * @param base SPI base pointer
 * @param mask SPI interrupt source. The parameter could be any combination of the following values:
 *        @arg kSPI_RxFullAndModfInterruptEnable
 *        @arg kSPI_TxEmptyInterruptEnable
 *        @arg kSPI_MatchInterruptEnable
 *        @arg kSPI_RxFifoNearFullInterruptEnable
 *        @arg kSPI_TxFifoNearEmptyInterruptEnable
 */
void SPI_DisableInterrupts(SPI_Type *base, uint32_t mask);

/*! @} */

/*!
 * @name DMA Control
 * @{
 */

#if defined(FSL_FEATURE_SPI_HAS_DMA_SUPPORT) && FSL_FEATURE_SPI_HAS_DMA_SUPPORT
/*!
 * @brief Enable dma source for SPI.
 *
 * @param base SPI base pointer
 * @param source SPI dma source.
 * @param enable True means enable DMA, false means disable DMA
 */
static inline void SPI_EnableDMA(SPI_Type *base, uint32_t mask, bool enable)
{
    if (enable)
    {
        base->C2 |= mask;
    }
    else
    {
        base->C2 &= ~mask;
    }
}
#endif /* FSL_FEATURE_SPI_HAS_DMA_SUPPORT */

/*!
 * @brief  Get SPI tx/rx data register address.
 *
 * This API is used to provide transfer address for SPI DMA transfer configuration.
 *
 * @param base SPI base pointer
 * @return data register address
 */
static inline uint32_t SPI_GetDataRegAddr(SPI_Type *base)
{
#if defined(FSL_FEATURE_SPI_16BIT_TRANSFERS) && FSL_FEATURE_SPI_16BIT_TRANSFERS
    return (uint32_t)(&(base->DL));
#else
    return (uint32_t)(&(base->D));
#endif /* FSL_FEATURE_SPI_16BIT_TRANSFERS */
}

/*! @} */

/*!
 * @name Bus Operations
 * @{
 */

/*!
 * @brief Set Baud rate for SPI transfer, this only used in master.
 *
 * @param base SPI base pointer
 * @param baudRate_BpsHz Buad Rate needed in Hz.
 * @param srcClockHz SPI source clock frequency in Hz.
 */
void SPI_MasterSetBaudRate(SPI_Type *base, uint32_t baudRate_Bps, uint32_t sourceClock_Hz);

/*!
 * @brief Set match data for SPI.
 *
 * Match Data is a hardware compare value, when the value received in the SPI receive data
 * buffer equals this hardware compare value, the SPI Match Flag in the S register (S[SPMF]) sets,
 * this can also generate a interrupt if the enable bit sets.
 *
 * @param base SPI base pointer
 * @param matchData Match data.
 */
static inline void SPI_SetMatchData(SPI_Type *base, uint32_t matchData)
{
#if defined(FSL_FEATURE_SPI_16BIT_TRANSFERS) && FSL_FEATURE_SPI_16BIT_TRANSFERS
    base->ML = matchData & 0xFFU;
    base->MH = (matchData >> 8U) & 0xFFU;
#else
    base->M = matchData;
#endif /* FSL_FEATURE_SPI_16BIT_TRANSFERS */
}

/*!
 * @brief Sends a buffer of data bytes in blocking way.
 *
 * @note This function blocks via polling until all bytes have been sent.
 *
 * @param base SPI base pointer
 * @param buffer The data bytes to send
 * @param size The number of data bytes to send
 */
void SPI_WriteBlocking(SPI_Type *base, uint8_t *buffer, size_t size);

/*!
 * @brief Sends a buffer of data bytes in non-blocking way.
 *
 * @param base SPI base pointer
 * @param buffer The data bytes to send
 * @param size The number of data bytes to send
 */
void SPI_WriteNonBlocking(SPI_Type *base, uint8_t *buffer, size_t size);

/*!
 * @brief Receive a buffer of data bytes in non-blocking way.
 *
 * @param base SPI base pointer
 * @param buffer The data to be received
 * @param size The number of data bytes to send
 */
void SPI_ReadNonBlocking(SPI_Type *base, uint8_t *buffer, size_t size);

/*! @} */

/*!
 * @name Transactional
 * @{
 */

/*!
 * @brief Initialize the SPI master handle.
 *
 * This function initializes the SPI master handle which can be used for other SPI master transactional APIs. Usually,
 * for a specified SPI instance, user need only call this API once to get the initialized handle.
 *
 * @param handle SPI handle pointer.
 * @param base SPI peripheral base address.
 */
void SPI_MasterCreateHandle(spi_master_handle_t *handle, SPI_Type *base);

/*!
 * @brief Performs a polling SPI transfer.
 *
 * @note Calling the API will not return until transfer succeeds.
 *
 * @param base SPI base pointer
 * @param xfer pointer to spi_xfer_config_t structure
 * @retval kStatus_Success Successfully start a transfer.
 * @retval kStatus_InvalidArgument Input argument is invalid.
 * @retval kStatus_SPI_Busy SPI is not idle, is running another transfer.
 */
status_t SPI_MasterTransferBlocking(spi_master_handle_t *handle, spi_transfer_t *xfer);

/*!
 * @brief Performs a non-blocking SPI interrupt transfer.
 *
 * @note Calling the API will immediately return after transfer intialize finished. Users need to
 * call SPI_GetStatusIRQ() to get the transfer status
 *
 * @param handle pointer to spi_master_handle_t structure which stores the transfer state
 * @param xfer pointer to spi_xfer_config_t structure
 * @retval kStatus_Success Successfully start a transfer.
 * @retval kStatus_InvalidArgument Input argument is invalid.
 * @retval kStatus_SPI_Busy SPI is not idle, is running another transfer.
 */
status_t SPI_MasterTransferNonBlocking(spi_master_handle_t *handle, spi_transfer_t *xfer);

/*!
 * @brief Get the remaining bytes of SPI Interrupt transfer
 *
 * @param handle Pointer to SPI transfer handle, this should be a static variable.
 * @return Remaining bytes to be transferred.
 */
size_t SPI_MasterGetRemainingBytes(spi_master_handle_t *handle);

/*!
 * @brief Abort a SPI transfer using interrupt.
 *
 * @param handle Pointer to SPI transfer handle, this should be a static variable.
 */
void SPI_MasterAbortTransfer(spi_master_handle_t *handle);

/*!
 * @brief Install callback for SPI transfer finished.
 *
 * @param handle Pointer to SPI transfer handle, this should be a static variable.
 * @param callback SPI callback function.
 * @param userData Parameter for spi callback.
 */
void SPI_MasterSetTransferCallback(spi_master_handle_t *handle, spi_master_callback_t callback, void *userData);

/*!
 * @brief Interrupt handler for SPI
 *
 * @param handle pointer to spi_master_handle_t structure which stores the transfer state.
 */
void SPI_MasterHandleIRQ(spi_master_handle_t *handle);

/*!
 * @brief Initialize the SPI slave handle.
 *
 * This function initializes the SPI slave handle which can be used for other SPI slave transactional APIs. Usually,
 * for a specified SPI instance, user need only call this API once to get the initialized handle.
 *
 * @param handle SPI handle pointer.
 * @param base SPI peripheral base address.
 */
static inline void SPI_SlaveCreateHandle(spi_slave_handle_t *handle, SPI_Type *base)
{
    SPI_MasterCreateHandle(handle, base);
}

/*!
 * @brief Performs a non-blocking SPI slave interrupt transfer.
 *
 * @note Calling the API will immediately return after transfer intialize finished. Users need to
 * call SPI_GetStatusIRQ() to get the transfer status.
 *
 * @param handle pointer to spi_master_handle_t structure which stores the transfer state
 * @param xfer pointer to spi_xfer_config_t structure
 * @retval kStatus_Success Successfully start a transfer.
 * @retval kStatus_InvalidArgument Input argument is invalid.
 * @retval kStatus_SPI_Busy SPI is not idle, is running another transfer.
 */
static inline status_t SPI_SlaveTransferNonBlocking(spi_slave_handle_t *handle, spi_transfer_t *xfer)
{
    return SPI_MasterTransferNonBlocking(handle, xfer);
}

/*!
 * @brief Get the slave transfer remaining bytes.
 *
 * @param handle Pointer to SPI transfer handle, this should be a static variable.
 * @return Remaining bytes need to be transferred.
 */
static inline size_t SPI_SlaveGetRemainingBytes(spi_slave_handle_t *handle)
{
    return SPI_MasterGetRemainingBytes(handle);
}

/*!
 * @brief Abort a SPI slave transfer using interrupt.
 *
 * @param handle Pointer to SPI transfer handle, this should be a static variable.
 */
static inline void SPI_SlaveAbortTransfer(spi_slave_handle_t *handle)
{
    SPI_MasterAbortTransfer(handle);
}

/*!
 * @brief Install callback for SPI slave transfer finished.
 *
 * @param handle Pointer to SPI transfer handle, this should be a static variable.
 * @param callback SPI callback function.
 * @param userData Parameter for spi callback.
 */
static inline void SPI_SlaveSetTransferCallback(spi_slave_handle_t *handle,
                                                spi_slave_callback_t callback,
                                                void *userData)
{
    SPI_MasterSetTransferCallback(handle, callback, userData);
}

/*!
 * @brief Interrupt handler for SPI slave
 *
 * @param handle pointer to spi_slave_handle_t structure which stores the transfer state
 */
static inline void SPI_SlaveHandleIRQ(spi_slave_handle_t *handle)
{
    SPI_MasterHandleIRQ(handle);
}

/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* _FSL_SPI_H_*/
