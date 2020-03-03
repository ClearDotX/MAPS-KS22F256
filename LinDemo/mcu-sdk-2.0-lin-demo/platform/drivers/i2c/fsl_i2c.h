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
#ifndef _FSL_I2C_H_
#define _FSL_I2C_H_

#include "fsl_common.h"

/*!
 * @addtogroup i2c
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief I2C driver version. */
#define FSL_I2C_DRIVER_VERSION (MAKE_VERSION(2, 0, 0)) /*!< Version 2.0.0. */

#if (defined(FSL_FEATURE_I2C_HAS_START_STOP_DETECT) && FSL_FEATURE_I2C_HAS_START_STOP_DETECT || \
defined(FSL_FEATURE_I2C_HAS_STOP_DETECT) && FSL_FEATURE_I2C_HAS_STOP_DETECT)
#define I2C_HAS_STOP_DETECT
#endif /* FSL_FEATURE_I2C_HAS_START_STOP_DETECT / FSL_FEATURE_I2C_HAS_STOP_DETECT */

/*! @brief  I2C status return codes. */
enum _i2c_status
{
    kStatus_I2C_Busy = MAKE_STATUS(kStatusGroup_I2C, 0),            /*!< I2C is busy with current transfer. */
    kStatus_I2C_Idle = MAKE_STATUS(kStatusGroup_I2C, 1),            /*!< Bus is Idle. */
    kStatus_I2C_Nak = MAKE_STATUS(kStatusGroup_I2C, 2),             /*!< NAK received during transfer. */
    kStatus_I2C_ArbitrationLost = MAKE_STATUS(kStatusGroup_I2C, 3), /*!< Arbitration lost during transfer. */
    kStatus_I2C_Timeout = MAKE_STATUS(kStatusGroup_I2C, 4),         /*!< Wait event timeout. */
    kStatus_I2C_TxRequest = MAKE_STATUS(kStatusGroup_I2C, 5),       /*!< Slave addressed for transmit. */
    kStatus_I2C_RxRequest = MAKE_STATUS(kStatusGroup_I2C, 6),       /*!< Slave addressed for receive. */
};

/*!
 * @brief I2C peripheral flags
 *
 * The following status register flags can be cleared:
 * - #kI2C_ArbitrationLostFlag
 * - #kI2C_IntPendingFlag
 * - #kI2C_StartDetectFlag
 * - #kI2C_StopDetectFlag
 *
 * @note These enums are meant to be OR'd together to form a bit mask.
 *
 */
enum _i2c_flags
{
    kI2C_ReceiveNakFlag = I2C_S_RXAK_MASK,       /*!< I2C receive nak flag. */
    kI2C_IntPendingFlag = I2C_S_IICIF_MASK,      /*!< I2C interrupt pending flag. */
    kI2C_TransferDirectionFlag = I2C_S_SRW_MASK, /*!< I2C transfer direction flag. */
    kI2C_RangeAddressMatchFlag = I2C_S_RAM_MASK, /*!< I2C range address match flag. */
    kI2C_ArbitrationLostFlag = I2C_S_ARBL_MASK,  /*!< I2C arbitration lost flag. */
    kI2C_BusBusyFlag = I2C_S_BUSY_MASK,          /*!< I2C bus busy flag. */
    kI2C_AddressMatchFlag = I2C_S_IAAS_MASK,     /*!< I2C address match flag. */
    kI2C_TransferCompleteFlag = I2C_S_TCF_MASK,  /*!< I2C transfer complete flag. */
#ifdef I2C_HAS_STOP_DETECT
    kI2C_StopDetectFlag = I2C_FLT_STOPF_MASK << 8, /*!< I2C stop detect flag. */
#endif /* FSL_FEATURE_I2C_HAS_START_STOP_DETECT / FSL_FEATURE_I2C_HAS_STOP_DETECT */

#if defined(FSL_FEATURE_I2C_HAS_START_STOP_DETECT) && FSL_FEATURE_I2C_HAS_START_STOP_DETECT
    kI2C_StartDetectFlag = I2C_FLT_STARTF_MASK << 8, /*!< I2C start detect flag. */
#endif                                               /* FSL_FEATURE_I2C_HAS_START_STOP_DETECT */
};

/*! @brief I2C feature interrupt source. */
enum _i2c_interrupt_enable
{
    kI2C_GlobalInterruptEnable = I2C_C1_IICIE_MASK, /*!< I2C global interrupt. */

#if defined(FSL_FEATURE_I2C_HAS_STOP_DETECT) && FSL_FEATURE_I2C_HAS_STOP_DETECT
    kI2C_StopDetectInterruptEnable = I2C_FLT_STOPIE_MASK, /*!< I2C stop detect interrupt. */
#endif                                                    /* FSL_FEATURE_I2C_HAS_STOP_DETECT */

#if defined(FSL_FEATURE_I2C_HAS_START_STOP_DETECT) && FSL_FEATURE_I2C_HAS_START_STOP_DETECT
    kI2C_StartStopDetectInterruptEnable = I2C_FLT_SSIE_MASK, /*!< I2C start&stop detect interrupt. */
#endif                                                       /* FSL_FEATURE_I2C_HAS_START_STOP_DETECT */
};

/*! @brief Direction of master and slave transfers. */
typedef enum _i2c_direction
{
    kI2C_Write = 0x0U, /*!< Master transmit to slave. */
    kI2C_Read = 0x1U,  /*!< Master receive from slave. */
} i2c_direction_t;

/*! @brief Addressing mode. */
typedef enum _i2c_slave_address_mode
{
    kI2C_Address7bit = 0x0U,  /*!< 7-bit addressing mode. */
    kI2C_Address10bit = 0x1U, /*!< 10-bit addressing mode. */
    kI2C_RangeMatch = 0X2U,   /*!< Range address match addressing mode. */
} i2c_slave_address_mode_t;

/*! @brief I2C master user config. */
typedef struct _i2c_master_config
{
    bool enableMaster; /*!< Enables the I2C periperhal at init time. */
#if defined(FSL_FEATURE_I2C_HAS_HIGH_DRIVE_SELECTION) && FSL_FEATURE_I2C_HAS_HIGH_DRIVE_SELECTION
    bool enableHighDrive; /*!< Controls the drive capability of the I2C pads. */
#endif
#if defined(FSL_FEATURE_I2C_HAS_STOP_HOLD_OFF) && FSL_FEATURE_I2C_HAS_STOP_HOLD_OFF
    bool enableStopHold; /*!< Controls the stop hold enable. */
#endif
    uint32_t baudRate_Bps;     /*!< Baud rate configuration of I2C peripheral. */
    uint8_t glitchFilterWidth; /*!< Controls the width of the glitch. */
} i2c_master_config_t;

/*! @brief I2C slave user config. */
typedef struct _i2c_slave_config
{
    bool enableSlave;       /*!< Enables the I2C periperhal at init time. */
    bool enableGeneralCall; /*!< Enable general call addressing mode. */
    bool enableWakeUp;      /*!< Enables/disables waking up MCU from low power mode. */
    bool enableHighDrive;   /*!< Controls the drive capability of the I2C pads. */
    bool enableBaudRateCtl; /*!< Enables/disables independent slave baudrate on SCL in very fast I2C modes. */
    uint16_t slaveAddress;  /*!< Slave address configuration. */
    uint16_t upperAddress;  /*!< Maximum boundary slave address used in range matching mode. */
    i2c_slave_address_mode_t addressingMode; /*!< Addressing mode configuration of i2c_slave_address_mode_config_t. */
} i2c_slave_config_t;

/*! @brief I2C transfer control flag. */
enum _i2c_master_transfer_flags
{
    kI2C_TransferDefaultFlag = 0x0U,       /*!< Transfer starts with a start signal, stops with a stop signal. */
    kI2C_TransferNoStartFlag = 0x1U,       /*!< Transfer starts without a start signal. */
    kI2C_TransferRepeatedStartFlag = 0x2U, /*!< Transfer starts with a repeated start signal. */
    kI2C_TransferNoStopFlag = 0x4U,        /*!< Transfer ends without a stop signal. */
};

/*! @brief I2C master handle typedef. */
typedef struct _i2c_master_handle i2c_master_handle_t;

/*! @brief I2C master transfer callback typedef. */
typedef void (*i2c_master_transfer_callback_t)(i2c_master_handle_t *handle, status_t status, void *userData);

/*! @brief I2C slave handle typedef. */
typedef struct _i2c_slave_handle i2c_slave_handle_t;

/*! @brief I2C slave transfer callback typedef. */
typedef void (*i2c_slave_transfer_callback_t)(i2c_slave_handle_t *handle, status_t status, void *userData);

/*! @brief I2C master transfer structure. */
typedef struct _i2c_master_transfer
{
    uint32_t flags;            /*!< Transfer flag which controls the transfer. */
    uint8_t slaveAddress;      /*!< 7-bit slave address. */
    i2c_direction_t direction; /*!< Transfer direction, read or write. */
    uint32_t subaddress;       /*!< Subaddress. Transferred MSB first. */
    uint8_t subaddressSize;    /*!< Size of command buffer. */
    uint8_t *data;             /*!< Transfer buffer. */
    size_t dataSize;           /*!< Transfer size. */
} i2c_master_transfer_t;

/*! @brief I2C master handle structure. */
struct _i2c_master_handle
{
    I2C_Type *base;                                    /*!< I2C base pointer. */
    i2c_master_transfer_t transfer;                    /*!< I2C master transfer copy. */
    uint8_t state;                                     /*!< Transfer state maintained during transfer. */
    i2c_master_transfer_callback_t completionCallback; /*!< Callback function called at transfer event. */
    void *userData;                                    /*!< Callback parameter passed to callback function. */
};

/*! @brief I2C slave transfer structure. */
typedef struct _i2c_slave_transfer
{
    uint8_t *data;   /*!< Transfer buffer. */
    size_t dataSize; /*!< Transfer size. */
} i2c_slave_transfer_t;

/*! @brief I2C slave handle structure. */
struct _i2c_slave_handle
{
    I2C_Type *base;                                   /*!< I2C base pointer. */
    i2c_slave_transfer_t transfer;                    /*!< I2C slave transfer copy. */
    uint8_t state;                                    /*!< Transfer status. */
    i2c_slave_transfer_callback_t completionCallback; /*!< Callback function called at transfer event. */
    void *userData;                                   /*!< Callback parameter passed to callback. */
};

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus. */

/*!
 * @name Initialization and deinitialization
 * @{
 */

/*!
 * @brief Initialize the I2C peripheral, call this API to ungate the i2c clock
 * and configure the I2C with master configuration.
 *
 * @note This API should be called at the beginning of the application to use
 * the i2c driver, or any operation to the i2c module could cause hard fault
 * because clock is not enabled. The configuration structure can be filled by user
 * from scratch, or be set with default values by I2C_MasterGetDefaultConfig().
 * After calling this API, the master is ready to transfer.
 * Example:
 * @code
 * i2c_master_config_t config = {
 * .enableMaster = true,
 * .enableStopHold = false,
 * .highDrive = false,
 * .baudRate_Bps = 100000,
 * .glitchFilterWidth = 0
 * };
 * I2C_MasterInit(I2C0, &config, 12000000U);
 * @endcode
 *
 * @param base I2C base pointer
 * @param masterConfig pointer to master config structure
 * @param srcClock_Hz I2C pheripharal clock frequency in HZ
 */
void I2C_MasterInit(I2C_Type *base, const i2c_master_config_t *masterConfig, uint32_t srcClock_Hz);

/*!
 * @brief Initialize the I2C peripheral, call this API to ungate the i2c clock
 * and initializes the I2C with slave configuration.
 *
 * @note This API should be called at the beginning of the application to use
 * the i2c driver, or any operation to the i2c module could cause hard fault
 * because clock is not enabled. The configuration structure could partly be set
 * with default values by I2C_SlaveGetDefaultConfig(), or can be filled by user from scratch.
 * Example
 * @code
 * i2c_slave_config_t config = {
 * .enableSlave = true,
 * .enableGeneralCall = false,
 * .addressingMode = kI2C_Address7bit,
 * .slaveAddress = 0x1DU,
 * .enableWakeUp = false,
 * .enablehighDrive = false,
 * .enableBaudRateCtl = false
 * };
 * I2C_SlaveInit(I2C0, &config);
 * @endcode
 *
 * @param base I2C base pointer
 * @param slaveConfig pointer to slave config structure
 */
void I2C_SlaveInit(I2C_Type *base, const i2c_slave_config_t *slaveConfig);

/*!
 * @brief De-initialize the I2C master peripheral, call thi API will gate the i2c clock,
 * so the I2C master module can not work unless call I2C_MasterInit.
 * @param base I2C base pointer
 */
void I2C_MasterDeinit(I2C_Type *base);

/*!
 * @brief De-initialize the I2C slave peripheral, call thi API will gate the i2c clock,
 * so the I2C slave module can not work unless call I2C_SlaveInit to enable the clock.
 * @param base I2C base pointer
 */
void I2C_SlaveDeinit(I2C_Type *base);

/*!
 * @brief  Set the I2C master configuration structure to default values.
 *
 * The purpose of this API is to get the config structure initialized for use in I2C_MasterConfigure().
 * User may use the initialized structure unchanged in I2C_MasterConfigure(), or modify some fields of
 * the structure before calling I2C_MasterConfigure().
 * Example:
 * @code
 * i2c_master_config_t config;
 * I2C_MasterGetDefaultConfig(&config);
 * @endcode
 * @param masterConfig Pointer to the master config structure.
*/
void I2C_MasterGetDefaultConfig(i2c_master_config_t *masterConfig);

/*!
 * @brief  Set the I2C slave configuration structure to default values.
 *
 * The purpose of this API is to get the config structure initialized for use in I2C_SlaveConfigure().
 * User needs to modify some fields of the structure before calling I2C_SlaveConfigure().
 * Example:
 * @code
 * i2c_slave_config_t config;
 * I2C_SlaveGetDefaultConfig(&config);
 * @endcode
 * @param slaveConfig Pointer to the slave config structure.
 */
void I2C_SlaveGetDefaultConfig(i2c_slave_config_t *slaveConfig);

/*!
 * @brief Enable or disables the I2C peripheral operation.
 *
 * @param base I2C base pointer
 * @param enable pass true to enable module, false to disable module
 */
static inline void I2C_Enable(I2C_Type *base, bool enable)
{
    if (enable)
    {
        base->C1 |= I2C_C1_IICEN_MASK;
    }
    else
    {
        base->C1 &= ~I2C_C1_IICEN_MASK;
    }
}

/* @} */

/*!
 * @name Status
 * @{
 */

/*!
 * @brief Gets the I2C status flags.
 *
 * @param base I2C base pointer
 * @return status flag, use status flag to AND #_i2c_flags could get the related status.
 */
uint32_t I2C_MasterGetStatusFlags(I2C_Type *base);

/*!
 * @brief Gets the I2C status flags.
 *
 * @param base I2C base pointer
 * @return status flag, use status flag to AND #_i2c_flags could get the related status.
 */
static inline uint32_t I2C_SlaveGetStatusFlags(I2C_Type *base)
{
    return I2C_MasterGetStatusFlags(base);
}

/*!
 * @brief Clears the I2C status flag state.
 *
 * The following status register flags can be cleared: kI2C_ArbitrationLostFlag and kI2C_IntPendingFlag
 *
 * @param base I2C base pointer
 * @param statusMask The status flag mask, defined in type i2c_status_flag_t.
 *      The parameter could be any combination of the following values:
 *          @arg kI2C_StartDetectFlag (if available)
 *          @arg kI2C_StopDetectFlag (if available)
 *          @arg kI2C_ArbitrationLostFlag
 *          @arg kI2C_IntPendingFlagFlag
 */
static inline void I2C_MasterClearStatusFlags(I2C_Type *base, uint32_t statusMask)
{
/* Must clear the STARTF / STOPF bits prior to clearing IICIF */
#if defined(FSL_FEATURE_I2C_HAS_START_STOP_DETECT) && FSL_FEATURE_I2C_HAS_START_STOP_DETECT
    if (statusMask & kI2C_StartDetectFlag)
    {
        /* Shift the odd-ball flags back into place. */
        base->FLT |= (uint8_t)(statusMask >> 8U);
    }
#endif

#ifdef I2C_HAS_STOP_DETECT
    if (statusMask & kI2C_StopDetectFlag)
    {
        /* Shift the odd-ball flags back into place. */
        base->FLT |= (uint8_t)(statusMask >> 8U);
    }
#endif

    base->S = (uint8_t)statusMask;
}

/*!
 * @brief Clears the I2C status flag state.
 *
 * The following status register flags can be cleared: kI2C_ArbitrationLostFlag and kI2C_IntPendingFlag
 *
  * @param base I2C base pointer
  * @param statusMask The status flag mask, defined in type i2c_status_flag_t.
 *      The parameter could be any combination of the following values:
 *          @arg kI2C_StartDetectFlag (if available)
 *          @arg kI2C_StopDetectFlag (if available)
 *          @arg kI2C_ArbitrationLostFlag
 *          @arg kI2C_IntPendingFlagFlag
 */
static inline void I2C_SlaveClearStatusFlags(I2C_Type *base, uint32_t statusMask)
{
    I2C_MasterClearStatusFlags(base, statusMask);
}

/* @} */

/*!
 * @name Interrupts
 * @{
 */

/*!
 * @brief Enable I2C interrupt requests.
 *
 * @param base I2C base pointer
 * @param interrupt interrupt source
 *     The parameter can be combination of the following source if defined:
 *     @arg kI2C_GlobalInterruptEnable
 *     @arg kI2C_StopDetectInterruptEnable/kI2C_StartDetectInterruptEnable
 *     @arg kI2C_SdaTimeoutInterruptEnable
 */
void I2C_EnableInterrupts(I2C_Type *base, uint32_t mask);

/*!
 * @brief Disable I2C interrupt requests.
 *
 * @param base I2C base pointer
 * @param interrupt interrupt source
 *     The parameter can be combination of the following source if defined:
 *     @arg kI2C_GlobalInterruptEnable
 *     @arg kI2C_StopDetectInterruptEnable/kI2C_StartDetectInterruptEnable
 *     @arg kI2C_SdaTimeoutInterruptEnable
 */
void I2C_DisableInterrupts(I2C_Type *base, uint32_t mask);

/*!
 * @name DMA Control
 * @{
 */
#if defined(FSL_FEATURE_I2C_HAS_DMA_SUPPORT) && FSL_FEATURE_I2C_HAS_DMA_SUPPORT
/*!
 * @brief Enables/disables the I2C DMA interrupt.
 *
 * @param base I2C base pointer
 * @param enable true for enable, false for diable
*/
static inline void I2C_EnableDMA(I2C_Type *base, bool enable)
{
    if (enable)
    {
        base->C1 |= I2C_C1_DMAEN_MASK;
    }
    else
    {
        base->C1 &= ~I2C_C1_DMAEN_MASK;
    }
}

#endif /* FSL_FEATURE_I2C_HAS_DMA_SUPPORT */

/*!
 * @brief  Get I2C tx/rx data register address This API is used to provide transfer address
 * for I2C DMA transfer configuration.
 *
 * @param base I2C base pointer
 * @return data register address
 */
static inline uint32_t I2C_GetDataRegAddr(I2C_Type *base)
{
    return (uint32_t)(&(base->D));
}

/* @} */
/*!
 * @name Bus Operations
 * @{
 */

/*!
 * @brief Sets the I2C master transfer baudrate
 *
 * @param base I2C base pointer
 * @param baudRate_Bps the baud rate value in bps
 */
void I2C_MasterSetBaudRate(I2C_Type *base, uint32_t baudRate_Bps, uint32_t srcClock_Hz);

/*!
 * @brief Sends a START on the I2C bus.
 *
 * This function is used to initiate a new master mode transfer by sending the START signal.
 * The slave address will be sent following the I2C START signal.
 *
 * @param base I2C peripheral base pointer
 * @param address 7-bit slave device address.
 * @param direction Master transfer directions(transmit/receive).
 */
status_t I2C_MasterStart(I2C_Type *base, uint8_t address, i2c_direction_t direction);

/*!
 * @brief Sends a STOP signal on the I2C bus.
 *
 * @return Whether the sending of STOP single is success or not.
 */
status_t I2C_MasterStop(I2C_Type *base);

/*!
 * @brief Sends a REPEATED START on the I2C bus.
 *
 * @param base I2C peripheral base pointer
 * @param address 7-bit slave device address.
 * @param direction Master transfer directions(transmit/receive).
 */
status_t I2C_MasterRepeatedStart(I2C_Type *base, uint8_t address, i2c_direction_t direction);

/*!
 * @brief Performs a polling send transaction on the I2C bus.
 *
 * @param base  The I2C peripheral base pointer.
 * @param txBuff The pointer to the data to be transferred.
 * @param txSize The length in bytes of the data to be transferred.
 * @return Error or success status returned by API.
 */
status_t I2C_MasterWrite(I2C_Type *base, const uint8_t *txBuff, size_t txSize);

/*!
 * @brief Performs a polling receive transaction on the I2C bus.
 *
 * @note The I2C_MasterRead function will STOP the bus before the read of the final byte.
 * Without stopping the bus prior to the final read, the bus will issue another read, resulting
 * in garbage data being read into the data register.
 *
 * @param base I2C peripheral base pointer.
 * @param rxBuff The pointer to the data to store the received data.
 * @param rxSize The length in bytes of the data to be received.
 * @return Error or success status returned by API.
 */
status_t I2C_MasterRead(I2C_Type *base, uint8_t *rxBuff, size_t rxSize);

/*!
 * @brief Performs a polling send transaction on the I2C bus.
 *
 * @param base  The I2C peripheral base pointer.
 * @param txBuff The pointer to the data to be transferred.
 * @param txSize The length in bytes of the data to be transferred.
 * @return Error or success status returned by API.
 */
status_t I2C_SlaveSend(I2C_Type *base, const uint8_t *txBuff, size_t txSize);

/*!
 * @brief Performs a polling receive transaction on the I2C bus.
 *
 * @param base I2C peripheral base pointer.
 * @param rxBuff The pointer to the data to store the received data.
 * @param rxSize The length in bytes of the data to be received.
 * @return Error or success status returned by API.
 */
status_t I2C_SlaveReceive(I2C_Type *base, uint8_t *rxBuff, size_t rxSize);

/* @} */

/*!
 * @name Transactional
 * @{
 */

/*!
 * @brief Init the I2C handle which is used in transactional functions
 *
 * @param handle pointer to i2c_master_handle_t structure to store the transfer state.
 * @param base I2C base pointer
 */
void I2C_MasterCreateHandle(i2c_master_handle_t *handle, I2C_Type *base);

/*!
 * @brief Performs a master polling transfer on the I2C bus.
 *
 * @note Calling the API will not return until the transfer succeeds or fails due
 * to arbitration lost or receiving a NAK.
 *
 * @param handle Pointer to the driver handle.
 * @param transfer Pointer to the transfer structure.
 * @return status Status of the transfer call.
 */
status_t I2C_MasterTransferBlocking(i2c_master_handle_t *handle, i2c_master_transfer_t *transfer);

/*!
 * @brief Performs a master interrupt non-blocking transfer on the I2C bus
 *
 * @note Calling the API will return immediately after transfer initiates, user needs
 * to call I2C_MasterGetTransferRemainingBytes to poll the transfer status to check whether
 * the transfer is finished, if the return status is not kStatus_I2C_Busy, the transfer
 * is finished.
 *
 * @param handle pointer to i2c_master_handle_t structure which stores the transfer state
 * @param transfer pointer to i2c_master_transfer_t structure
 * @return status of uint32_t
 */
status_t I2C_MasterTransferNonBlocking(i2c_master_handle_t *handle, i2c_master_transfer_t *transfer);

/*!
 * @brief Set the user callback which is called when the master I2C transfer completes.
 *
 * @param handle pointer to i2c_master_handle_t structure which stores the transfer state
 * @param completionCallback pointer to user callback function
 * @param userData user param passed to the callback function
 */
void I2C_MasterSetTransferCallback(i2c_master_handle_t *handle,
                                   i2c_master_transfer_callback_t completionCallback,
                                   void *userData);

/*!
 * @brief Get master transfer status during a interrupt non-blocking transfer
 *
 * @param handle pointer to i2c_master_handle_t structure which stores the transfer state
 * @return remaining bytes to transfer
 */
static inline size_t I2C_MasterGetTransferRemainingBytes(i2c_master_handle_t *handle)
{
    return handle->transfer.dataSize;
}

/*!
 * @brief Abort a interrupt non-blocking transfer in a early time
 *
 * @note This API could be called at any time when interrupt non-blocking transfer initiates
 * to abort the transfer in a early time.
 *
 * @param handle pointer to i2c_master_handle_t structure which stores the transfer state
 */
void I2C_MasterAbortTransfer(i2c_master_handle_t *handle);

/*!
 * @brief Master interrupt handler
 *
 * @param handle pointer to i2c_master_handle_t structure
 */
void I2C_MasterHandleIRQ(i2c_master_handle_t *handle);

/*!
 * @brief Init the I2C handle which is used in transcational functions
 *
 * @param handle pointer to i2c_slave_handle_t structure to store the transfer state.
 * @param base I2C base pointer
 */
void I2C_SlaveCreateHandle(i2c_slave_handle_t *handle, I2C_Type *base);

/*!
 * @brief Set up a slave transfer
 *
 * @note Call this API after calling I2C_SlaveConfigure to set up a interrupt-driven passive slave.
 * The slave will monitor the event on I2C bus and handle the events in IRQHandler.
 *
 * @param handle pointer to i2c_slave_handle_t structure which stores the transfer state
 * @param transfer pointer to i2c_slave_transfer_t structure
 * @retval kStatus_Success Successfully set up a slave transfer
 * @retval kStatus_I2C_Busy Current slave is busy, transfer set up failed
 */
status_t I2C_SlaveTransferNonBlocking(i2c_slave_handle_t *handle, i2c_slave_transfer_t *transfer);

/*!
 * @brief Update slave transfer buffer
 *
 * @note Call this API in slave callback to update the transfer buffer and transfer size.
 *
 * @param handle pointer to i2c_slave_handle_t structure which stores the transfer state
 * @param transfer pointer to i2c_slave_transfer_t structure
 */
void I2C_SlaveTransferUpdateBuffer(i2c_slave_handle_t *handle, i2c_slave_transfer_t *transfer);

/*!
 * @brief Install user callback which is called when slave transfer finishes.
 *
 * @param handle pointer to i2c_slave_handle_t structure which stores the transfer state
 * @param completionCallback pointer to user callback function
 * @param userData user param passed to the callback function
 */
void I2C_SlaveSetTransferCallback(i2c_slave_handle_t *handle,
                                  i2c_slave_transfer_callback_t completionCallback,
                                  void *userData);

/*!
 * @brief Abort slave transfer
 *
 * @note This API could be called at any time to stop slave for handling the bus events.
 * @param handle pointer to i2c_slave_handle_t structure which stores the transfer state
 */
void I2C_SlaveAbortTransfer(i2c_slave_handle_t *handle);

/*!
 * @brief Get slave transfer remaining bytes during a interrupt non-blocking transfer
 *
 * @param handle pointer to i2c_slave_handle_t structure
 * @return remaining bytes to transfer
 */
static inline size_t I2C_SlaveGetTransferRemainingBytes(i2c_slave_handle_t *handle)
{
    return handle->transfer.dataSize;
}

/*!
 * @brief Slave interrupt handler
 *
 * @param handle pointer to i2c_slave_handle_t structure which stores the transfer state
 */
void I2C_SlaveHandleIRQ(i2c_slave_handle_t *handle);

/* @} */
#if defined(__cplusplus)
}
#endif /*_cplusplus. */
/*@}*/

#endif /* _FSL_I2C_H_*/
