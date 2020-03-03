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
#include "fsl_i2c.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief i2c transfer state. */
enum _i2c_transfer_states
{
    kIdleState = 0x0U,             /*!< I2C bus idle. */
    kCheckAddressState = 0x1U,     /*!< 7-bit address check state. */
    kSendCommandState = 0x2U,      /*!< Send command byte phase. */
    kSendDataState = 0x3U,         /*!< Send data transfer phase. */
    kReceiveDataBeginState = 0x4U, /*!< Receive data transfer phase begin. */
    kReceiveDataState = 0x5U,      /*!< Receive data transfer phase. */
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
 * @brief Get instance number for I2C module.
 *
 * @param base I2C peripheral base address
 */
static uint32_t I2C_GetInstance(I2C_Type *base);

/*!
 * @brief Set up master transfer, send slave address and decide the initial
 * transfer state.
 *
 * @param handle pointer to i2c_master_handle_t structure which stores the transfer state
 * @param transfer pointer to i2c_master_transfer_t structure
 */
static status_t I2C_InitTransferStateMachine(i2c_master_handle_t *handle, i2c_master_transfer_t *transfer);

/*!
 * @brief Check and clear status operation.
 *
 * @param handle pointer to i2c_master_handle_t structure which stores the transfer state
 * @param status current i2c hardware status
 * @return transfer status of #_i2c_status
 */
static status_t I2C_CheckAndClearError(i2c_master_handle_t *handle, uint32_t status);

/*!
 * @brief Master run transfer state machine to perform a byte of transfer.
 *
 * @param handle pointer to i2c_master_handle_t structure which stores the transfer state
 * @param isDone input param to get whether the thing is done, true is done
 * @return transfer status of #_i2c_status
 */
static status_t I2C_MasterRunStateMachine(i2c_master_handle_t *handle, bool *isDone);

/*!
 * @brief I2C common interrupt handler.
 *
 * @param handle pointer to i2c_master_handle_t structure which stores the transfer state
 */
static void I2C_CommonIRQHandler(void *handle);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Pointers to i2c handles for each instance. */
static void *s_i2cHandle[FSL_FEATURE_SOC_I2C_COUNT] = {NULL};

/*! @brief SCL clock divider used to calculate baudrate. */
const uint16_t s_i2cDividerTable[] = {20,   22,   24,   26,   28,   30,   34,   40,   28,   32,   36,   40,  44,
                                      48,   56,   68,   48,   56,   64,   72,   80,   88,   104,  128,  80,  96,
                                      112,  128,  144,  160,  192,  240,  160,  192,  224,  256,  288,  320, 384,
                                      480,  320,  384,  448,  512,  576,  640,  768,  960,  640,  768,  896, 1024,
                                      1152, 1280, 1536, 1920, 1280, 1536, 1792, 2048, 2304, 2560, 3072, 3840};

/*! @brief Pointers to i2c bases for each instance. */
const I2C_Type *s_i2cBases[] = I2C_BASE_PTRS;

/*! @brief Pointers to i2c IRQ number for each instance. */
const IRQn_Type s_i2cIrqs[] = I2C_IRQS;

/*! @brief Pointers to i2c clocks for each instance. */
const clock_ip_name_t s_i2cClocks[] = I2C_CLOCKS;

/*******************************************************************************
 * Codes
 ******************************************************************************/

static uint32_t I2C_GetInstance(I2C_Type *base)
{
    uint8_t instance = 0;

    while ((s_i2cBases[instance] != base) && (instance < FSL_FEATURE_SOC_I2C_COUNT))
    {
        instance++;
    }

    assert(instance < FSL_FEATURE_SOC_I2C_COUNT);

    return instance;
}

static status_t I2C_InitTransferStateMachine(i2c_master_handle_t *handle, i2c_master_transfer_t *transfer)
{
    status_t result = kStatus_Success;
    i2c_direction_t direction = transfer->direction;
    uint16_t timeout = UINT16_MAX;

    /* Initialize the handle transfer information. */
    handle->transfer = *transfer;

    /* Initial transfer state. */
    if (handle->transfer.subaddressSize > 0)
    {
        handle->state = kSendCommandState;
        if (transfer->direction == kI2C_Read)
        {
            direction = kI2C_Write;
        }
    }
    else
    {
        handle->state = kCheckAddressState;
    }

    /* Wait until the data register is ready for transmit. */
    while ((!(handle->base->S & kI2C_TransferCompleteFlag)) && (timeout--))
    {
    }

    /* Failed to complete the transfer. */
    if (timeout == 0)
    {
        return kStatus_I2C_Timeout;
    }

    /* Clear all status before transfer. */
    I2C_MasterClearStatusFlags(handle->base, kClearFlags);

    /* If repeated start is requested, send repeated start. */
    if (handle->transfer.flags & kI2C_TransferRepeatedStartFlag)
    {
        result = I2C_MasterRepeatedStart(handle->base, handle->transfer.slaveAddress, direction);
    }
    else /* For normal transfer, send start. */
    {
        result = I2C_MasterStart(handle->base, handle->transfer.slaveAddress, direction);
    }

    return result;
}

static status_t I2C_CheckAndClearError(i2c_master_handle_t *handle, uint32_t status)
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
        if ((handle->transfer.direction == kI2C_Write) && (handle->transfer.dataSize != 0))
        {
            result = kStatus_I2C_Nak;
        }
        else if ((handle->transfer.direction == kI2C_Read) && (handle->transfer.dataSize != 1))
        {
            result = kStatus_I2C_Nak;
        }
    }

    return result;
}

static status_t I2C_MasterRunStateMachine(i2c_master_handle_t *handle, bool *isDone)
{
    status_t result = kStatus_Success;
    uint32_t statusFlags = handle->base->S;
    *isDone = false;

    /* Check & clear error flags. */
    result = I2C_CheckAndClearError(handle, statusFlags);

    if (result)
    {
        return result;
    }

    /* Handle Check address state to check the slave address is Acked in slave
       probe application. */
    if (handle->state == kCheckAddressState)
    {
        if (statusFlags & kI2C_ReceiveNakFlag)
        {
            return kStatus_I2C_Nak;
        }
        else
        {
            if (handle->transfer.direction == kI2C_Write)
            {
                /* Next state, send data. */
                handle->state = kSendDataState;
            }
            else
            {
                /* Next state, receive data begin. */
                handle->state = kReceiveDataBeginState;
            }
        }
    }

    /* Run state machine. */
    switch (handle->state)
    {
        /* Send I2C command. */
        case kSendCommandState:
            if (handle->transfer.subaddressSize)
            {
                handle->transfer.subaddressSize--;
                handle->base->D = ((handle->transfer.subaddress) >> (8 * handle->transfer.subaddressSize));
            }
            else
            {
                if (handle->transfer.direction == kI2C_Write)
                {
                    /* Next state, send data. */
                    handle->state = kSendDataState;

                    /* Send first byte of data. */
                    if (handle->transfer.dataSize > 0)
                    {
                        handle->base->D = *handle->transfer.data;
                        handle->transfer.data++;
                        handle->transfer.dataSize--;
                    }
                }
                else
                {
                    /* Send repeated start and slave address. */
                    result = I2C_MasterRepeatedStart(handle->base, handle->transfer.slaveAddress, kI2C_Read);

                    /* Next state, receive data begin. */
                    handle->state = kReceiveDataBeginState;
                }
            }
            break;

        /* Send I2C data. */
        case kSendDataState:
            /* Send one byte of data. */
            if (handle->transfer.dataSize > 0)
            {
                handle->base->D = *handle->transfer.data;
                handle->transfer.data++;
                handle->transfer.dataSize--;
            }
            else
            {
                *isDone = true;
            }
            break;

        /* Start I2C data receive. */
        case kReceiveDataBeginState:
            handle->base->C1 &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK);

            /* Send nak at the last receive byte. */
            if (handle->transfer.dataSize == 1)
            {
                handle->base->C1 |= I2C_C1_TXAK_MASK;
            }

            /* Read dummy to release the bus. */
            handle->base->D;

            /* Next state, receive data. */
            handle->state = kReceiveDataState;
            break;

        /* Receive I2C data. */
        case kReceiveDataState:
            /* Receive one byte of data. */
            if (handle->transfer.dataSize--)
            {
                if (handle->transfer.dataSize == 0)
                {
                    *isDone = true;

                    /* Send stop if kI2C_TransferNoStop is not asserted. */
                    if (!(handle->transfer.flags & kI2C_TransferNoStopFlag))
                    {
                        result = I2C_MasterStop(handle->base);
                    }
                }

                /* Send NAK at the last receive byte. */
                if (handle->transfer.dataSize == 1)
                {
                    handle->base->C1 |= I2C_C1_TXAK_MASK;
                }

                /* Read the data byte into the transfer buffer. */
                *handle->transfer.data = handle->base->D;
                handle->transfer.data++;
            }
            break;

        default:
            break;
    }

    return result;
}

static void I2C_CommonIRQHandler(void *handle)
{
    /* Assume it's master IRQ. */
    I2C_Type *base = ((i2c_master_handle_t *)handle)->base;

    /* Check if master interrupt. */
    if ((base->S & kI2C_ArbitrationLostFlag) || (base->C1 & I2C_C1_MST_MASK))
    {
        I2C_MasterHandleIRQ(handle);
    }
    else
    {
        I2C_SlaveHandleIRQ(handle);
    }
}

void I2C_MasterInit(I2C_Type *base, const i2c_master_config_t *masterConfig, uint32_t srcClock_Hz)
{
    assert(masterConfig && srcClock_Hz);

    /* Temporary register for filter read. */
    uint8_t fltReg;
    uint8_t c2Reg;

    /* Enable I2C clock. */
    CLOCK_EnableClock(s_i2cClocks[I2C_GetInstance(base)]);

    /* Disable I2C prior to configuring it. */
    base->C1 &= ~(I2C_C1_IICEN_MASK);

    /* Clear all flags. */
    I2C_MasterClearStatusFlags(base, kClearFlags);

    /* Configure baud rate. */
    I2C_MasterSetBaudRate(base, masterConfig->baudRate_Bps, srcClock_Hz);

#if defined(FSL_FEATURE_I2C_HAS_HIGH_DRIVE_SELECTION) && FSL_FEATURE_I2C_HAS_HIGH_DRIVE_SELECTION
    /* Configure high drive feature. */
    c2Reg = base->C2;
    c2Reg &= ~(I2C_C2_HDRS_MASK);
    c2Reg |= I2C_C2_HDRS(masterConfig->enableHighDrive);
    base->C2 = c2Reg;
#endif

    /* Read out the FLT register. */
    fltReg = base->FLT;

#if defined(FSL_FEATURE_I2C_HAS_STOP_HOLD_OFF) && FSL_FEATURE_I2C_HAS_STOP_HOLD_OFF
    /* Configure the stop / hold enable. */
    fltReg &= ~(I2C_FLT_SHEN_MASK);
    fltReg |= I2C_FLT_SHEN(masterConfig->enableStopHold);
#endif

    /* Configure the glitch filter value. */
    fltReg &= ~(I2C_FLT_FLT_MASK);
    fltReg |= I2C_FLT_FLT(masterConfig->glitchFilterWidth);

    /* Write the register value back to the filter register. */
    base->FLT = fltReg;

    /* Enable the I2C peripheral based on the configuration. */
    base->C1 = I2C_C1_IICEN(masterConfig->enableMaster);
}

void I2C_MasterDeinit(I2C_Type *base)
{
    /* Disable I2C module. */
    I2C_Enable(base, false);

    /* Disable I2C clock. */
    CLOCK_DisableClock(s_i2cClocks[I2C_GetInstance(base)]);
}

void I2C_MasterGetDefaultConfig(i2c_master_config_t *masterConfig)
{
    assert(masterConfig);

    /* Default baud rate at 100kbps. */
    masterConfig->baudRate_Bps = 100000U;

/* Default pin high drive is disabled. */
#if defined(FSL_FEATURE_I2C_HAS_HIGH_DRIVE_SELECTION) && FSL_FEATURE_I2C_HAS_HIGH_DRIVE_SELECTION
    masterConfig->enableHighDrive = false;
#endif

/* Default stop hold enable is disabled. */
#if defined(FSL_FEATURE_I2C_HAS_STOP_HOLD_OFF) && FSL_FEATURE_I2C_HAS_STOP_HOLD_OFF
    masterConfig->enableStopHold = false;
#endif

    /* Default glitch filter value is no filter. */
    masterConfig->glitchFilterWidth = 0U;

    /* Enable the I2C peripheral. */
    masterConfig->enableMaster = true;
}

void I2C_EnableInterrupts(I2C_Type *base, uint32_t mask)
{
    if (mask & kI2C_GlobalInterruptEnable)
    {
        base->C1 |= I2C_C1_IICIE_MASK;
    }

#if defined(FSL_FEATURE_I2C_HAS_STOP_DETECT) && FSL_FEATURE_I2C_HAS_STOP_DETECT
    if (mask & kI2C_StopDetectInterruptEnable)
    {
        base->FLT |= I2C_FLT_STOPIE_MASK;
    }
#endif /* FSL_FEATURE_I2C_HAS_STOP_DETECT */

#if defined(FSL_FEATURE_I2C_HAS_START_STOP_DETECT) && FSL_FEATURE_I2C_HAS_START_STOP_DETECT
    if (mask & kI2C_StartStopDetectInterruptEnable)
    {
        base->FLT |= I2C_FLT_SSIE_MASK;
    }
#endif /* FSL_FEATURE_I2C_HAS_START_STOP_DETECT */
}

void I2C_DisableInterrupts(I2C_Type *base, uint32_t mask)
{
    if (mask & kI2C_GlobalInterruptEnable)
    {
        base->C1 &= ~I2C_C1_IICIE_MASK;
    }

#if defined(FSL_FEATURE_I2C_HAS_STOP_DETECT) && FSL_FEATURE_I2C_HAS_STOP_DETECT
    if (mask & kI2C_StopDetectInterruptEnable)
    {
        base->FLT &= ~I2C_FLT_STOPIE_MASK;
    }
#endif /* FSL_FEATURE_I2C_HAS_STOP_DETECT */

#if defined(FSL_FEATURE_I2C_HAS_START_STOP_DETECT) && FSL_FEATURE_I2C_HAS_START_STOP_DETECT
    if (mask & kI2C_StartStopDetectInterruptEnable)
    {
        base->FLT &= ~I2C_FLT_SSIE_MASK;
    }
#endif /* FSL_FEATURE_I2C_HAS_START_STOP_DETECT */
}

void I2C_MasterSetBaudRate(I2C_Type *base, uint32_t baudRate_Bps, uint32_t srcClock_Hz)
{
    uint32_t multiplier;
    uint32_t computedRate;
    uint32_t absError;
    uint32_t bestError = UINT32_MAX;
    uint32_t bestMult = 0u;
    uint32_t bestIcr = 0u;
    uint8_t mult;
    uint8_t i;

    /* Search for the settings with the lowest error. Mult is the MULT field of the I2C_F register,
     * and ranges from 0-2. It selects the multiplier factor for the divider. */
    for (mult = 0u; (mult <= 2u) && (bestError != 0); ++mult)
    {
        multiplier = 1u << mult;

        /* Scan table to find best match. */
        for (i = 0u; i < sizeof(s_i2cDividerTable) / sizeof(uint16_t); ++i)
        {
            computedRate = srcClock_Hz / (multiplier * s_i2cDividerTable[i]);
            absError = baudRate_Bps > computedRate ? (baudRate_Bps - computedRate) : (computedRate - baudRate_Bps);

            if (absError < bestError)
            {
                bestMult = mult;
                bestIcr = i;
                bestError = absError;

                /* If the error is 0, then we can stop searching because we won't find a better match. */
                if (absError == 0)
                {
                    break;
                }
            }
        }
    }

    /* Set frequency register based on best settings. */
    base->F = I2C_F_MULT(bestMult) | I2C_F_ICR(bestIcr);
}

status_t I2C_MasterStart(I2C_Type *base, uint8_t address, i2c_direction_t direction)
{
    status_t result = kStatus_Success;
    uint32_t statusFlags = I2C_MasterGetStatusFlags(base);

    /* Return an error if the bus is already in use. */
    if (statusFlags & kI2C_BusBusyFlag)
    {
        result = kStatus_I2C_Busy;
    }
    else
    {
        /* Send the START signal. */
        base->C1 |= I2C_C1_MST_MASK | I2C_C1_TX_MASK;

#if defined(FSL_FEATURE_I2C_HAS_DOUBLE_BUFFERING) && FSL_FEATURE_I2C_HAS_DOUBLE_BUFFERING
        while (!(base->S2 & I2C_S2_EMPTY_MASK))
        {
        }
#endif /* FSL_FEATURE_I2C_HAS_DOUBLE_BUFFERING */

        base->D = (((uint32_t)address) << 1U | ((direction == kI2C_Read) ? 1U : 0U));
    }

    return result;
}

status_t I2C_MasterRepeatedStart(I2C_Type *base, uint8_t address, i2c_direction_t direction)
{
    status_t result = kStatus_Success;
    uint8_t savedMult;
    uint32_t statusFlags = I2C_MasterGetStatusFlags(base);
    uint32_t c1Reg = base->C1;
    uint8_t timeDelay = 6;

    /* Return an error if the bus is already in use, but not by us. */
    if ((statusFlags & kI2C_BusBusyFlag) && (c1Reg & I2C_C1_MST_MASK == 0))
    {
        result = kStatus_I2C_Busy;
    }
    else
    {
        savedMult = base->F;
        base->F = savedMult & (~I2C_F_MULT_MASK);

        /* We are already in a transfer, so send a repeated start. */
        base->C1 |= I2C_C1_RSTA_MASK;

        /* Restore the multiplier factor. */
        base->F = savedMult;

        /* Add some delay to wait the Re-Start signal. */
        while (timeDelay--)
        {
            __NOP();
        }

#if defined(FSL_FEATURE_I2C_HAS_DOUBLE_BUFFERING) && FSL_FEATURE_I2C_HAS_DOUBLE_BUFFERING
        while (!(base->S2 & I2C_S2_EMPTY_MASK))
        {
        }
#endif /* FSL_FEATURE_I2C_HAS_DOUBLE_BUFFERING */

        base->D = (((uint32_t)address) << 1U | ((direction == kI2C_Read) ? 1U : 0U));
    }

    return result;
}

status_t I2C_MasterStop(I2C_Type *base)
{
    status_t result = kStatus_Success;
    uint16_t timeout = UINT16_MAX;

    /* Issue the STOP command on the bus. */
    base->C1 &= ~(I2C_C1_MST_MASK | I2C_C1_TX_MASK | I2C_C1_TXAK_MASK);

    /* Wait until data transfer complete. */
    while ((base->S & kI2C_BusBusyFlag) && (timeout--))
    {
    }

    if (timeout == 0)
    {
        result = kStatus_I2C_Timeout;
    }

    return result;
}

uint32_t I2C_MasterGetStatusFlags(I2C_Type *base)
{
    uint32_t statusFlags = base->S;

#ifdef I2C_HAS_STOP_DETECT
    /* Look up the STOPF bit from the filter register. */
    if (base->FLT & I2C_FLT_STOPF_MASK)
    {
        statusFlags |= kI2C_StopDetectFlag;
    }
#endif

#if defined(FSL_FEATURE_I2C_HAS_START_STOP_DETECT) && FSL_FEATURE_I2C_HAS_START_STOP_DETECT
    /* Look up the STARTF bit from the filter register. */
    if (base->FLT & I2C_FLT_STARTF_MASK)
    {
        statusFlags |= kI2C_StartDetectFlag;
    }
#endif /* FSL_FEATURE_I2C_HAS_START_STOP_DETECT */

    return statusFlags;
}

status_t I2C_MasterWrite(I2C_Type *base, const uint8_t *txBuff, size_t txSize)
{
    status_t result = kStatus_Success;
    uint8_t statusFlags = 0;
    uint16_t timeout = UINT16_MAX;

    /* Wait until the data register is ready for transmit. */
    while ((!(base->S & kI2C_TransferCompleteFlag)) && (timeout--))
    {
    }

    /* Failed to complete the transfer. */
    if (timeout == 0)
    {
        return kStatus_I2C_Timeout;
    }

    /* Clear the IICIF flag. */
    base->S = kI2C_IntPendingFlag;

    /* Setup the I2C peripheral to transmit data. */
    base->C1 |= I2C_C1_TX_MASK;

    while (txSize--)
    {
        /* Send a byte of data. */
        base->D = *txBuff++;

        /* Wait until data transfer complete. */
        while ((!(base->S & kI2C_IntPendingFlag)) && (timeout--))
        {
        }

        /* Failed to complete the transfer. */
        if (timeout == 0)
        {
            result = kStatus_I2C_Timeout;
        }

        statusFlags = base->S;

        /* Clear the IICIF flag. */
        base->S = kI2C_IntPendingFlag;

        /* Check if arbitration lost or no acknowledgement (NAK), return failure status. */
        if (statusFlags & kI2C_ArbitrationLostFlag)
        {
            base->S = kI2C_ArbitrationLostFlag;
            result = kStatus_I2C_ArbitrationLost;
        }

        if (statusFlags & kI2C_ReceiveNakFlag)
        {
            base->S = kI2C_ReceiveNakFlag;
            result = kStatus_I2C_Nak;
        }

        if (result != kStatus_Success)
        {
            /* Breaking out of the send loop. */
            break;
        }
    }

    return result;
}

status_t I2C_MasterRead(I2C_Type *base, uint8_t *rxBuff, size_t rxSize)
{
    status_t result = kStatus_Success;
    uint16_t timeout = UINT16_MAX;

    /* Wait until the data register is ready for transmit. */
    while ((!(base->S & kI2C_TransferCompleteFlag)) && (timeout--))
    {
    }

    /* Failed to complete the transfer. */
    if (timeout == 0)
    {
        return kStatus_I2C_Timeout;
    }

    /* Clear the IICIF flag. */
    base->S = kI2C_IntPendingFlag;

    /* Setup the I2C peripheral to receive data. */
    base->C1 &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK);

    /* If rxSize equals 1, configure to send NAK. */
    if (rxSize == 1)
    {
        /* Issue NACK on read. */
        base->C1 |= I2C_C1_TXAK_MASK;
    }

    /* Do dummy read. */
    base->D;

    while ((rxSize--))
    {
        /* Wait until data transfer complete. */
        while ((!(base->S & kI2C_IntPendingFlag)) && (timeout--))
        {
        }

        if (timeout == 0)
        {
            result = kStatus_I2C_Timeout;
            break;
        }

        /* Clear the IICIF flag. */
        base->S = kI2C_IntPendingFlag;

        /* Single byte use case. */
        if (rxSize == 0)
        {
            /* Read the final byte. */
            result = I2C_MasterStop(base);
        }
        else if (rxSize == 1)
        {
            /* Issue NACK on read. */
            base->C1 |= I2C_C1_TXAK_MASK;
        }

        /* Read from the data register. */
        *rxBuff++ = base->D;
    }

    return result;
}

void I2C_MasterCreateHandle(i2c_master_handle_t *handle, I2C_Type *base)
{
    assert(handle);

    uint32_t instance = I2C_GetInstance(base);

    /* Zero handle. */
    memset(handle, 0, sizeof(*handle));

    /* Set the base for the handle. */
    handle->base = base;

    /* Save the context in global variables to support the double weak mechanism. */
    s_i2cHandle[instance] = handle;

    /* Enable NVIC interrupt. */
    EnableIRQ(s_i2cIrqs[instance]);
}

void I2C_MasterSetTransferCallback(i2c_master_handle_t *handle,
                                   i2c_master_transfer_callback_t completionCallback,
                                   void *userData)
{
    /* Install the user callback function into the I2C driver handle. */
    handle->completionCallback = completionCallback;
    handle->userData = userData;
}

status_t I2C_MasterTransferBlocking(i2c_master_handle_t *handle, i2c_master_transfer_t *transfer)
{
    assert(handle);
    assert(transfer);

    status_t result = kStatus_Success;
    bool isDone = false;
    uint16_t timeout = UINT16_MAX;

    if (handle->state != kIdleState)
    {
        result = kStatus_I2C_Busy;
    }
    else
    {
        result = I2C_InitTransferStateMachine(handle, transfer);

        if (result)
        {
            return result;
        }

        do
        {
            /* Wait until data transfer complete or start/stop detected. */
            while ((!(handle->base->S & kI2C_IntPendingFlag)) && (timeout--))
            {
            }

            /* Break the loop if i2c timeout happens. */
            if (timeout == 0)
            {
                result = kStatus_I2C_Timeout;
                break;
            }

            /* Clear pending flag. */
            handle->base->S = kI2C_IntPendingFlag;

            /* Run state machine. */
            result = I2C_MasterRunStateMachine(handle, &isDone);

        } while ((result == kStatus_Success) && (!isDone));

        if ((!(handle->transfer.flags & kI2C_TransferNoStopFlag)) || (result == kStatus_I2C_Nak))
        {
            /* Ensure stop command is a need. */
            if ((handle->base->C1 & I2C_C1_MST_MASK))
            {
                if (I2C_MasterStop(handle->base) != kStatus_Success)
                {
                    result = kStatus_I2C_Timeout;
                }
            }
        }

        /* Restore handle to idle state. */
        handle->state = kIdleState;
    }

    return result;
}

status_t I2C_MasterTransferNonBlocking(i2c_master_handle_t *handle, i2c_master_transfer_t *transfer)
{
    assert(handle);
    assert(transfer);
    status_t result = kStatus_Success;

    /* Check if the I2C bus is idle - if not return busy status. */
    if (handle->state != kIdleState)
    {
        result = kStatus_I2C_Busy;
    }
    else
    {
        /* Start up the master transfer state machine. */
        result = I2C_InitTransferStateMachine(handle, transfer);

        if (result == kStatus_Success)
        {
            /* Enable the I2C interrupts. */
            I2C_EnableInterrupts(handle->base, kI2C_GlobalInterruptEnable);
        }
    }

    return result;
}

void I2C_MasterAbortTransfer(i2c_master_handle_t *handle)
{
    assert(handle);

    /* Disable interrupt. */
    I2C_DisableInterrupts(handle->base, kI2C_GlobalInterruptEnable);

    /* Reset the state to idle. */
    handle->state = kIdleState;
}

void I2C_MasterHandleIRQ(i2c_master_handle_t *handle)
{
    assert(handle);

    status_t result = kStatus_Success;
    bool isDone;

    /* Clear the interrupt flag. */
    handle->base->S = kI2C_IntPendingFlag;

    /* Check transfer complete flag. */
    result = I2C_MasterRunStateMachine(handle, &isDone);

    if (isDone || result)
    {
        /* Send stop command if transfer done or received Nak. */
        if ((!(handle->transfer.flags & kI2C_TransferNoStopFlag)) || (result == kStatus_I2C_Nak))
        {
            /* Ensure stop command is a need. */
            if ((handle->base->C1 & I2C_C1_MST_MASK))
            {
                if (I2C_MasterStop(handle->base) != kStatus_Success)
                {
                    result = kStatus_I2C_Timeout;
                }
            }
        }

        /* Restore handle to idle state. */
        handle->state = kIdleState;

        /* Disable interrupt. */
        I2C_DisableInterrupts(handle->base, kI2C_GlobalInterruptEnable);

        /* Call the callback function after the function has completed. */
        if (handle->completionCallback)
        {
            handle->completionCallback(handle, result, handle->userData);
        }
    }
}

void I2C_SlaveInit(I2C_Type *base, const i2c_slave_config_t *slaveConfig)
{
    assert(slaveConfig);

    uint8_t tmpReg;

    CLOCK_EnableClock(s_i2cClocks[I2C_GetInstance(base)]);

    /* Configure addressing mode. */
    switch (slaveConfig->addressingMode)
    {
        case kI2C_Address7bit:
            base->A1 = ((uint32_t)(slaveConfig->slaveAddress)) << 1U;
            break;
        case kI2C_Address10bit:
            base->A1 = ((uint32_t)(slaveConfig->slaveAddress)) << 1U;
            base->C2 |= I2C_C2_ADEXT(1) | I2C_C2_AD((slaveConfig->slaveAddress & 0x0380U) >> 7U);
            break;
        case kI2C_RangeMatch:
            assert(slaveConfig->slaveAddress < slaveConfig->upperAddress);
            base->A1 = ((uint32_t)(slaveConfig->slaveAddress)) << 1U;
            base->RA = ((uint32_t)(slaveConfig->upperAddress)) << 1U;
            base->C2 |= I2C_C2_RMEN_MASK;
            break;
        default:
            break;
    }

    /* Configure low power wake up feature. */
    tmpReg = base->C1;
    tmpReg &= ~I2C_C1_WUEN_MASK;
    base->C1 = tmpReg | I2C_C1_WUEN(slaveConfig->enableWakeUp) | I2C_C1_IICEN(slaveConfig->enableSlave);

    /* Configure general call & baud rate control & high drive feature. */
    tmpReg = base->C2;
    tmpReg &= ~(I2C_C2_SBRC_MASK | I2C_C2_HDRS_MASK);
    base->C2 = tmpReg | I2C_C2_SBRC(slaveConfig->enableBaudRateCtl) | I2C_C2_HDRS(slaveConfig->enableHighDrive) |
               I2C_C2_GCAEN(slaveConfig->enableGeneralCall);
}

void I2C_SlaveDeinit(I2C_Type *base)
{
    /* Disable I2C module. */
    I2C_Enable(base, false);

    /* Disable I2C clock. */
    CLOCK_DisableClock(s_i2cClocks[I2C_GetInstance(base)]);
}

void I2C_SlaveGetDefaultConfig(i2c_slave_config_t *slaveConfig)
{
    assert(slaveConfig);

    /* By default slave is addressed with 7-bit address. */
    slaveConfig->addressingMode = kI2C_Address7bit;

    /* General call mode is disabled by default. */
    slaveConfig->enableGeneralCall = false;

    /* Slave address match waking up MCU from low power mode is disabled. */
    slaveConfig->enableWakeUp = false;

#if defined(FSL_FEATURE_I2C_HAS_HIGH_DRIVE_SELECTION) && FSL_FEATURE_I2C_HAS_HIGH_DRIVE_SELECTION
    /* Default pin high drive is disabled. */
    slaveConfig->enableHighDrive = false;
#endif

    /* Independent slave mode baud rate at maximum frequency is disabled. */
    slaveConfig->enableBaudRateCtl = false;

    /* Enable the I2C peripheral. */
    slaveConfig->enableSlave = true;
}

status_t I2C_SlaveSend(I2C_Type *base, const uint8_t *txBuff, size_t txSize)
{
    return I2C_MasterWrite(base, txBuff, txSize);
}

status_t I2C_SlaveReceive(I2C_Type *base, uint8_t *rxBuff, size_t rxSize)
{
    status_t result = kStatus_Success;
    uint16_t timeout = UINT16_MAX;

    /* Clear the IICIF flag. */
    base->S = kI2C_IntPendingFlag;

    /* Wait until the data register is ready for receive. */
    while (!(base->S & kI2C_TransferCompleteFlag))
    {
    }

    /* Setup the I2C peripheral to receive data. */
    base->C1 &= ~(I2C_C1_TX_MASK);

    while (rxSize--)
    {
        /* Clear the IICIF flag. */
        base->S = kI2C_IntPendingFlag;

        /* Read from the data register. */
        *rxBuff++ = base->D;

        /* Wait until data transfer complete. */
        while ((!(base->S & kI2C_IntPendingFlag)) && (timeout--))
        {
        }

        if (timeout == 0)
        {
            result = kStatus_I2C_Timeout;
            break;
        }
    }

    return result;
}

void I2C_SlaveCreateHandle(i2c_slave_handle_t *handle, I2C_Type *base)
{
    assert(handle);

    uint32_t instance = I2C_GetInstance(base);

    /* Zero handle. */
    memset(handle, 0, sizeof(*handle));

    /* Set the base for the handle. */
    handle->base = base;

    /* Save the context in global variables to support the double weak mechanism. */
    s_i2cHandle[instance] = handle;

    /* Enable NVIC interrupt. */
    EnableIRQ(s_i2cIrqs[instance]);
}

void I2C_SlaveSetTransferCallback(i2c_slave_handle_t *handle,
                                  i2c_slave_transfer_callback_t completionCallback,
                                  void *userData)
{
    handle->completionCallback = completionCallback;
    handle->userData = userData;
}

status_t I2C_SlaveTransferNonBlocking(i2c_slave_handle_t *handle, i2c_slave_transfer_t *transfer)
{
    assert(handle);
    assert(transfer);

    /* Check if the I2C bus is idle - if not return busy status. */
    if (handle->state != kIdleState)
    {
        return kStatus_I2C_Busy;
    }
    else
    {
        handle->transfer = *transfer;

        I2C_EnableInterrupts(handle->base, kI2C_GlobalInterruptEnable);

#if defined(FSL_FEATURE_I2C_HAS_STOP_DETECT) && FSL_FEATURE_I2C_HAS_STOP_DETECT
        I2C_EnableInterrupts(handle->base, kI2C_StopDetectInterruptEnable);
#endif /* FSL_FEATURE_I2C_HAS_STOP_DETECT */

#if defined(FSL_FEATURE_I2C_HAS_START_STOP_DETECT) && FSL_FEATURE_I2C_HAS_START_STOP_DETECT
        I2C_EnableInterrupts(handle->base, kI2C_StartStopDetectInterruptEnable);
#endif /* FSL_FEATURE_I2C_HAS_START_STOP_DETECT */
    }

    return kStatus_Success;
}

void I2C_SlaveTransferUpdateBuffer(i2c_slave_handle_t *handle, i2c_slave_transfer_t *transfer)
{
    assert(handle);
    assert(transfer);

    handle->transfer = *transfer;
}

void I2C_SlaveAbortTransfer(i2c_slave_handle_t *handle)
{
    assert(handle);

    /* Reset the state to idle. */
    handle->state = kIdleState;

    /* Disable interrupts. */
    I2C_DisableInterrupts(handle->base, kI2C_GlobalInterruptEnable);

#if defined(FSL_FEATURE_I2C_HAS_STOP_DETECT) && FSL_FEATURE_I2C_HAS_STOP_DETECT
    I2C_DisableInterrupts(handle->base, kI2C_StopDetectInterruptEnable);
#endif /* FSL_FEATURE_I2C_HAS_STOP_DETECT */

#if defined(FSL_FEATURE_I2C_HAS_START_STOP_DETECT) && FSL_FEATURE_I2C_HAS_START_STOP_DETECT
    I2C_DisableInterrupts(handle->base, kI2C_StartStopDetectInterruptEnable);
#endif /* FSL_FEATURE_I2C_HAS_START_STOP_DETECT */
}

void I2C_SlaveHandleIRQ(i2c_slave_handle_t *handle)
{
    assert(handle);

    uint16_t status;
    bool doTransmit = false;

    status = I2C_SlaveGetStatusFlags(handle->base);

#if defined(FSL_FEATURE_I2C_HAS_START_STOP_DETECT) && FSL_FEATURE_I2C_HAS_START_STOP_DETECT
    /* Check start flag. */
    if (status & kI2C_StartDetectFlag)
    {
        I2C_MasterClearStatusFlags(handle->base, kI2C_StartDetectFlag);
        /* Clear the interrupt flag. */
        handle->base->S = kI2C_IntPendingFlag;
        return;
    }
#endif /* FSL_FEATURE_I2C_HAS_START_STOP_DETECT */

#ifdef I2C_HAS_STOP_DETECT
    /* Check stop flag. */
    if (status & kI2C_StopDetectFlag)
    {
        I2C_MasterClearStatusFlags(handle->base, kI2C_StopDetectFlag);

        /* Clear the interrupt flag. */
        handle->base->S = kI2C_IntPendingFlag;
        handle->state = kIdleState;

        if (handle->completionCallback)
        {
            handle->completionCallback(handle, kStatus_Success, handle->userData);
        }
        return;
    }
#endif /* I2C_HAS_STOP_DETECT */

    /* Clear the interrupt flag. */
    handle->base->S = kI2C_IntPendingFlag;

    /* Check NAK */
    if (status & kI2C_ReceiveNakFlag)
    {
        /* Set receive mode. */
        handle->base->C1 &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK);

        /* Read dummy. */
        handle->base->D;

        if (handle->transfer.dataSize != 0)
        {
            handle->state = kIdleState;
            if (handle->completionCallback)
            {
                handle->completionCallback(handle, kStatus_I2C_Nak, handle->userData);
            }
        }
        else
        {
#ifndef I2C_HAS_STOP_DETECT
            handle->state = kIdleState;
            if (handle->completionCallback)
            {
                handle->completionCallback(handle, kStatus_Success, handle->userData);
            }
#endif /* !FSL_FEATURE_I2C_HAS_START_STOP_DETECT or !FSL_FEATURE_I2C_HAS_STOP_DETECT */
        }
    }
    /* Check address match. */
    else if (status & kI2C_AddressMatchFlag)
    {
        /* Slave transmit, master reading from slave. */
        if (status & kI2C_TransferDirectionFlag)
        {
            handle->state = kSendDataState;

            /* Change direction to send data. */
            handle->base->C1 |= I2C_C1_TX_MASK;

            /* Call slave callback to proceed the tx request. */
            if (handle->completionCallback)
            {
                handle->completionCallback(handle, kStatus_I2C_TxRequest, handle->userData);
            }

            doTransmit = true;
        }
        else
        {
            handle->state = kReceiveDataState;

            /* Slave receive, master writing to slave. */
            handle->base->C1 &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK);

            /* Call slave callback to proceed the receive request. */
            if (handle->completionCallback)
            {
                handle->completionCallback(handle, kStatus_I2C_RxRequest, handle->userData);
            }

            /* Read dummy to release the bus. */
            handle->base->D;
        }
    }
    /* Check transfer complete flag. */
    else if (status & kI2C_TransferCompleteFlag)
    {
        /* Slave transmit, master reading from slave. */
        if (status & kI2C_TransferDirectionFlag)
        {
            doTransmit = true;
        }
        else
        {
            /* Slave receive, master writing to slave. */
            uint8_t data = handle->base->D;

            if (handle->transfer.dataSize)
            {
                /* Receive data. */
                *handle->transfer.data++ = data;
                handle->transfer.dataSize--;
                if (!handle->transfer.dataSize)
                {
#ifndef I2C_HAS_STOP_DETECT
                    handle->state = kIdleState;

                    /* Proceed receive complete event. */
                    if (handle->completionCallback)
                    {
                        handle->completionCallback(handle, kStatus_Success, handle->userData);
                    }
#endif /* !FSL_FEATURE_I2C_HAS_START_STOP_DETECT or !FSL_FEATURE_I2C_HAS_STOP_DETECT */
                }
            }
        }
    }
    else
    {
        /* Read dummy to release bus. */
        handle->base->D;
    }

    /* Send data if there is the need. */
    if (doTransmit)
    {
        if (handle->transfer.dataSize)
        {
            /* Send data. */
            handle->base->D = *handle->transfer.data++;
            handle->transfer.dataSize--;
        }
        else
        {
            /* Switch to receive mode. */
            handle->base->C1 &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK);

            /* Read dummy to release bus. */
            handle->base->D;

#ifndef I2C_HAS_STOP_DETECT
            handle->state = kIdleState;

            /* Proceed txdone event. */
            if (handle->completionCallback)
            {
                handle->completionCallback(handle, kStatus_Success, handle->userData);
            }
#endif /* !FSL_FEATURE_I2C_HAS_START_STOP_DETECT or !FSL_FEATURE_I2C_HAS_STOP_DETECT */
        }
    }
}

void I2C0_DriverIRQHandler(void)
{
    I2C_CommonIRQHandler(s_i2cHandle[0]);
}

#if (FSL_FEATURE_SOC_I2C_COUNT > 1)
void I2C1_DriverIRQHandler(void)
{
    I2C_CommonIRQHandler(s_i2cHandle[1]);
}
#endif /* I2C COUNT > 1 */

#if (FSL_FEATURE_SOC_I2C_COUNT > 2)
void I2C2_DriverIRQHandler(void)
{
    I2C_CommonIRQHandler(s_i2cHandle[2]);
}
#endif /* I2C COUNT > 2 */
