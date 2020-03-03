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

#include "fsl_spi.h"

/*******************************************************************************
 * Definitons
 ******************************************************************************/
/*! @brief SPI transfer state, which is used for SPI transactiaonl APIs' internal state. */
enum _spi_transfer_states_t
{
    kSPI_Idle = 0x0, /*!< SPI is idle state */
    kSPI_Busy        /*!< SPI is busy tranferring data. */
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Get the instance for SPI module.
 *
 * @param base SPI base address
 */
static uint32_t SPI_GetInstance(SPI_Type *base);

/*!
 * @brief Send a piece of data for SPI.
 *
 * This function computes the number of data to be written into D register or Tx FIFO,
 * and write the data into it. At the same time, this function updates the values in
 * master handle structure.
 *
 * @param handle Pointer to SPI master handle structure.
 */
static void SPI_SendTransaction(spi_master_handle_t *handle);

/*!
 * @brief Receive a piece of data for SPI master.
 *
 * This function computes the number of data to receive from D register or Rx FIFO,
 * and write the data to destination address. At the same time, this function updates
 * the values in master handle structure.
 *
 * @param handle Pointer to SPI master handle structure.
 */
static void SPI_ReceiveTransaction(spi_master_handle_t *handle);
/*******************************************************************************
 * Variables
 ******************************************************************************/
/* SPI internal handle pointer array */
static spi_master_handle_t *s_spiHandle[FSL_FEATURE_SOC_SPI_COUNT];
/* Base pointer array */
static SPI_Type *const s_spiBase[] = SPI_BASE_PTRS;
/* IRQ name array */
static const IRQn_Type s_spiIRQ[] = SPI_IRQS;
/* Clock array name */
static const clock_ip_name_t s_spiClock[] = SPI_CLOCKS;

/*******************************************************************************
 * Code
 ******************************************************************************/
static uint32_t SPI_GetInstance(SPI_Type *base)
{
    uint8_t instance = 0;

    while ((s_spiBase[instance] != base) && (instance < FSL_FEATURE_SOC_SPI_COUNT))
    {
        instance++;
    }

    assert(instance < FSL_FEATURE_SOC_SPI_COUNT);

    return instance;
}

static void SPI_SendTransaction(spi_master_handle_t *handle)
{
    SPI_Type *base = handle->base;
    uint8_t bytes = MIN((handle->watermark * 2U), handle->txRemainingBytes);
    uint8_t val = 1U;

    /* Read S register and ensure SPTEF is 1, otherwise the write would be ignored. */
    if (handle->watermark == 1U)
    {
        val = base->S & SPI_S_SPTEF_MASK;
        if (bytes != 0U)
        {
            bytes = 1U;
        }
    }

    /* Write data */
    if (val)
    {
        SPI_WriteNonBlocking(base, handle->txBuff, bytes);

        /* Update handle information */
        handle->txBuff += bytes;
        handle->txRemainingBytes -= bytes;
    }
}

static void SPI_ReceiveTransaction(spi_master_handle_t *handle)
{
    SPI_Type *base = handle->base;
    uint8_t bytes = MIN((handle->watermark * 2U), handle->rxRemainingBytes);
    uint8_t val = 1U;

    /* Read S register and ensure SPRF is 1, otherwise the write would be ignored. */
    if (handle->watermark == 1U)
    {
        val = base->S & SPI_S_SPRF_MASK;
        if (bytes != 0U)
        {
            bytes = 1U;
        }
    }

    if (val)
    {
        SPI_ReadNonBlocking(base, handle->rxBuff, bytes);

        /* Update information in handle */
        handle->rxBuff += bytes;
        handle->rxRemainingBytes -= bytes;
    }
}

void SPI_MasterGetDefaultConfig(spi_master_config_t *config)
{
    config->enableMaster = true;
    config->enableStopInWaitMode = false;
    config->polarity = kSPI_ClockPolarityActiveHigh;
    config->phase = kSPI_ClockPhaseFirstEdge;
    config->direction = kSPI_MsbFirst;

#if defined(FSL_FEATURE_SPI_16BIT_TRANSFERS) && FSL_FEATURE_SPI_16BIT_TRANSFERS
    config->dataMode = kSPI_8BitMode;
#endif /* FSL_FEATURE_SPI_16BIT_TRANSFERS */

#if defined(FSL_FEATURE_SPI_FIFO_SIZE) && FSL_FEATURE_SPI_FIFO_SIZE
    config->txWatermark = kSPI_TxFifoOneHalfEmpty;
    config->rxWatermark = kSPI_RxFifoOneHalfFull;
#endif /* FSL_FEATURE_SPI_FIFO_SIZE */

    config->pinMode = kSPI_PinModeNormal;
    config->outputMode = kSPI_SlaveSelectAutomaticOutput;
    config->baudRate_Bps = 500000U;
}

void SPI_MasterInit(SPI_Type *base, const spi_master_config_t *config, uint32_t sourceClock_Hz)
{
    uint8_t instance = SPI_GetInstance(base);

    /* Open clock gate for SPI and open interrupt */
    CLOCK_EnableClock(s_spiClock[SPI_GetInstance(base)]);

    /* Disable SPI before configuration */
    base->C1 &= ~SPI_C1_SPE_MASK;

    /* Configure clock polarity and phase, set SPI to master */
    base->C1 = SPI_C1_MSTR(1U) | SPI_C1_CPOL(config->polarity) | SPI_C1_CPHA(config->phase) |
               SPI_C1_SSOE(config->outputMode & 1U) | SPI_C1_LSBFE(config->direction);

/* Set data mode, and also pin mode and mode fault settings */
#if defined(FSL_FEATURE_SPI_16BIT_TRANSFERS) && FSL_FEATURE_SPI_16BIT_TRANSFERS
    base->C2 = SPI_C2_MODFEN(config->outputMode >> 1U) | SPI_C2_BIDIROE(config->pinMode >> 1U) |
               SPI_C2_SPISWAI(config->enableStopInWaitMode) | SPI_C2_SPC0(config->pinMode & 1U) |
               SPI_C2_SPIMODE(config->dataMode);
#else
    base->C2 = SPI_C2_MODFEN(config->outputMode >> 1U) | SPI_C2_BIDIROE(config->pinMode >> 1U) |
               SPI_C2_SPISWAI(config->enableStopInWaitMode) | SPI_C2_SPC0(config->pinMode & 1U);
#endif /* FSL_FEATURE_SPI_16BIT_TRANSFERS */

/* Set watermark, FIFO is enabled */
#if defined(FSL_FEATURE_SPI_FIFO_SIZE) && FSL_FEATURE_SPI_FIFO_SIZE
    uint8_t fifoSize[FSL_FEATURE_SOC_SPI_COUNT] = FSL_FEATURE_SPI_FIFO_SIZEx;
    if (fifoSize[instance] != 0)
    {
        base->C3 = SPI_C3_TNEAREF_MARK(config->txWatermark) | SPI_C3_RNFULLF_MARK(config->rxWatermark) |
                   SPI_C3_INTCLR(0U) | SPI_C3_FIFOMODE(1U);
    }
#endif /* FSL_FEATURE_SPI_FIFO_SIZE */

    /* Set baud rate */
    SPI_MasterSetBaudRate(base, config->baudRate_Bps, sourceClock_Hz);

    /* Enable SPI */
    if (config->enableMaster)
    {
        base->C1 |= SPI_C1_SPE_MASK;
    }
}

void SPI_SlaveGetDefaultConfig(spi_slave_config_t *config)
{
    config->enableSlave = true;
    config->polarity = kSPI_ClockPolarityActiveHigh;
    config->phase = kSPI_ClockPhaseFirstEdge;
    config->direction = kSPI_MsbFirst;
    config->enableStopInWaitMode = false;

#if defined(FSL_FEATURE_SPI_16BIT_TRANSFERS) && FSL_FEATURE_SPI_16BIT_TRANSFERS
    config->dataMode = kSPI_8BitMode;
#endif /* FSL_FEATURE_SPI_16BIT_TRANSFERS */

#if defined(FSL_FEATURE_SPI_FIFO_SIZE) && FSL_FEATURE_SPI_FIFO_SIZE
    config->txWatermark = kSPI_TxFifoOneHalfEmpty;
    config->rxWatermark = kSPI_RxFifoOneHalfFull;
#endif /* FSL_FEATURE_SPI_FIFO_SIZE */
}

void SPI_SlaveInit(SPI_Type *base, const spi_slave_config_t *config)
{
    uint8_t instance = SPI_GetInstance(base);

    /* Open clock gate for SPI and open interrupt */
    CLOCK_EnableClock(s_spiClock[SPI_GetInstance(base)]);

    /* Disable SPI before configuration */
    base->C1 &= ~SPI_C1_SPE_MASK;

    /* Configure master and clock polarity and phase */
    base->C1 =
        SPI_C1_MSTR(0U) | SPI_C1_CPOL(config->polarity) | SPI_C1_CPHA(config->phase) | SPI_C1_LSBFE(config->direction);

/* Configure data mode if needed */
#if defined(FSL_FEATURE_SPI_16BIT_TRANSFERS) && FSL_FEATURE_SPI_16BIT_TRANSFERS
    base->C2 = SPI_C2_SPIMODE(config->dataMode) | SPI_C2_SPISWAI(config->enableStopInWaitMode);
#else
    base->C2 = SPI_C2_SPISWAI(config->enableStopInWaitMode);
#endif /* FSL_FEATURE_SPI_16BIT_TRANSFERS */

/* Set watermark */
#if defined(FSL_FEATURE_SPI_FIFO_SIZE) && FSL_FEATURE_SPI_FIFO_SIZE
    uint8_t fifoSize[FSL_FEATURE_SOC_SPI_COUNT] = FSL_FEATURE_SPI_FIFO_SIZEx;
    if (fifoSize[instance] != 0U)
    {
        base->C3 = SPI_C3_TNEAREF_MARK(config->txWatermark) | SPI_C3_RNFULLF_MARK(config->rxWatermark) |
                   SPI_C3_INTCLR(0U) | SPI_C3_FIFOMODE(1U);
    }
#endif /* FSL_FEATURE_SPI_FIFO_SIZE */

    /* Enable SPI */
    if (config->enableSlave)
    {
        base->C1 |= SPI_C1_SPE_MASK;
    }
}

void SPI_Deinit(SPI_Type *base)
{
    /* Disable SPI module before shutting down */
    base->C1 &= ~SPI_C1_SPE_MASK;

    /* Gate the clock */
    CLOCK_DisableClock(s_spiClock[SPI_GetInstance(base)]);
}

uint32_t SPI_GetStatusFlags(SPI_Type *base)
{
#if defined(FSL_FEATURE_SPI_FIFO_SIZE) && FSL_FEATURE_SPI_FIFO_SIZE
    uint8_t fifoSize[FSL_FEATURE_SOC_SPI_COUNT] = FSL_FEATURE_SPI_FIFO_SIZEx;

    if (fifoSize[SPI_GetInstance(base)] != 0)
    {
        return ((base->S) | (((uint32_t)base->CI) << 8U));
    }
    else
    {
        return (base->S);
    }
#else
    return (base->S);
#endif /* FSL_FEATURE_SPI_FIFO_SIZE */
}

void SPI_EnableInterrupts(SPI_Type *base, uint32_t mask)
{
#if defined(FSL_FEATURE_SPI_FIFO_SIZE) && FSL_FEATURE_SPI_FIFO_SIZE
    uint8_t instance = SPI_GetInstance(base);
#endif
    /* Rx full interrupt */
    if (mask & kSPI_RxFullAndModfInterruptEnable)
    {
        base->C1 |= SPI_C1_SPIE_MASK;
    }

    /* Tx empty interrupt */
    if (mask & kSPI_TxEmptyInterruptEnable)
    {
        base->C1 |= SPI_C1_SPTIE_MASK;
    }

    /* Data match interrupt */
    if (mask & kSPI_MatchInterruptEnable)
    {
        base->C2 |= SPI_C2_SPMIE_MASK;
    }

    /* FIFO related interrupts */
#if defined(FSL_FEATURE_SPI_FIFO_SIZE) && FSL_FEATURE_SPI_FIFO_SIZE
    uint8_t fifoSize[FSL_FEATURE_SOC_SPI_COUNT] = FSL_FEATURE_SPI_FIFO_SIZEx;
    if (fifoSize[instance] != 0)
    {
        /* Rx FIFO near full interrupt */
        if (mask & kSPI_RxFifoNearFullInterruptEnable)
        {
            base->C3 |= SPI_C3_RNFULLIEN_MASK;
        }

        /* Tx FIFO near empty interrupt */
        if (mask & kSPI_TxFifoNearEmptyInterruptEnable)
        {
            base->C3 |= SPI_C3_TNEARIEN_MASK;
        }
    }
#endif /* FSL_FEATURE_SPI_FIFO_SIZE */
}

void SPI_DisableInterrupts(SPI_Type *base, uint32_t mask)
{
#if defined(FSL_FEATURE_SPI_FIFO_SIZE) && FSL_FEATURE_SPI_FIFO_SIZE
    uint8_t instance = SPI_GetInstance(base);
#endif
    /* Rx full interrupt */
    if (mask & kSPI_RxFullAndModfInterruptEnable)
    {
        base->C1 &= (~SPI_C1_SPIE_MASK);
    }

    /* Tx empty interrupt */
    if (mask & kSPI_TxEmptyInterruptEnable)
    {
        base->C1 &= (~SPI_C1_SPTIE_MASK);
    }

    /* Data match interrupt */
    if (mask & kSPI_MatchInterruptEnable)
    {
        base->C2 &= (~SPI_C2_SPMIE_MASK);
    }

#if defined(FSL_FEATURE_SPI_FIFO_SIZE) && FSL_FEATURE_SPI_FIFO_SIZE
    uint8_t fifoSize[FSL_FEATURE_SOC_SPI_COUNT] = FSL_FEATURE_SPI_FIFO_SIZEx;
    if (fifoSize[instance] != 0)
    {
        /* Rx FIFO near full interrupt */
        if (mask & kSPI_RxFifoNearFullInterruptEnable)
        {
            base->C3 &= ~SPI_C3_RNFULLIEN_MASK;
        }

        /* Tx FIFO near empty interrupt */
        if (mask & kSPI_TxFifoNearEmptyInterruptEnable)
        {
            base->C3 &= ~SPI_C3_TNEARIEN_MASK;
        }
    }
#endif /* FSL_FEATURE_SPI_FIFO_SIZE */
}

void SPI_MasterSetBaudRate(SPI_Type *base, uint32_t baudRate_Bps, uint32_t sourceClock_Hz)
{
    uint32_t prescaler;
    uint32_t bestPrescaler;
    uint32_t rateDivisor;
    uint32_t bestDivisor;
    uint32_t rateDivisorValue;
    uint32_t realBaudrate;
    uint32_t bestBaudrate;
    uint32_t diff;
    uint32_t min_diff;
    uint32_t freq = baudRate_Bps;

    /* Find combination of prescaler and scaler resulting in baudrate closest to the requested value */
    min_diff = 0xFFFFFFFFU;

    /* Set the maximum divisor bit settings for each of the following divisors */
    bestPrescaler = 7U;
    bestDivisor = 8U;

    /* Set baud rate to minimum baud rate possible, adjust prescale divisor and divisorbit settings in
    acutal divisor values */
    bestBaudrate = sourceClock_Hz / ((bestPrescaler + 1) * (bestDivisor * 64));

    /* In all for loops, if min_diff = 0, the exit for loop*/
    for (prescaler = 0; (prescaler <= 7) && min_diff; prescaler++)
    {
        /* Initialize to div-by-2 */
        rateDivisorValue = 2U;

        for (rateDivisor = 0; (rateDivisor <= 8U) && min_diff; rateDivisor++)
        {
            /* Calculate actual baud rate, note need to add 1 to prescaler */
            realBaudrate = ((sourceClock_Hz) / ((prescaler + 1) * rateDivisorValue));

            /* Calculate the baud rate difference based on the conditional statement ,that states that the
            calculated baud rate must not exceed the desired baud rate */
            if (freq >= realBaudrate)
            {
                diff = freq - realBaudrate;
                if (min_diff > diff)
                {
                    /* A better match found */
                    min_diff = diff;
                    bestPrescaler = prescaler;
                    bestDivisor = rateDivisor;
                    bestBaudrate = realBaudrate;
                }
            }

            /* Multiply by 2 for each iteration, possible divisor values: 2, 4, 8, 16, ... 512 */
            rateDivisorValue *= 2U;
        }
    }

    /* Write the best prescalar and baud rate scalar */
    base->BR = SPI_BR_SPR(bestDivisor) | SPI_BR_SPPR(bestPrescaler);
}

void SPI_WriteBlocking(SPI_Type *base, uint8_t *buffer, size_t size)
{
    uint32_t i = 0;

    while (i < size)
    {
        while ((base->S & SPI_S_SPTEF_MASK) == 0)
        {
        }

        /* Send data */
        SPI_WriteNonBlocking(base, buffer, size);

        /* Wait the data to be sent */
        while ((base->S & SPI_S_SPTEF_MASK) == 0)
        {
        }
    }
}

void SPI_WriteNonBlocking(SPI_Type *base, uint8_t *buffer, size_t size)
{
    uint32_t i = 0;

    while (i < size)
    {
        if (buffer != NULL)
        {
#if defined(FSL_FEATURE_SPI_16BIT_TRANSFERS) && FSL_FEATURE_SPI_16BIT_TRANSFERS
            /*16 bit mode*/
            if (base->C2 & SPI_C2_SPIMODE_MASK)
            {
                base->DL = *buffer++;
                base->DH = *buffer++;
                i += 2;
            }
            /* 8 bit mode */
            else
            {
                base->DL = *buffer++;
                i++;
            }
#else
            base->D = *buffer++;
            i++;
#endif /* FSL_FEATURE_SPI_16BIT_TRANSFERS */
        }
        /* Send dummy data */
        else
        {
#if defined(FSL_FEATURE_SPI_16BIT_TRANSFERS) && FSL_FEATURE_SPI_16BIT_TRANSFERS
            base->DL = *buffer++;
            i++;
#else
            base->D = *buffer++;
            i++;
#endif /* FSL_FEATURE_SPI_16BIT_TRANSFERS */
        }
    }
}

void SPI_ReadNonBlocking(SPI_Type *base, uint8_t *buffer, size_t size)
{
    uint32_t i = 0;

    while (i < size)
    {
#if defined(FSL_FEATURE_SPI_16BIT_TRANSFERS) && FSL_FEATURE_SPI_16BIT_TRANSFERS
        /*16 bit mode*/
        if (base->C2 & SPI_C2_SPIMODE_MASK)
        {
            *buffer++ = base->DL;
            *buffer++ = base->DH;
            i += 2;
        }
        /* 8 bit mode */
        else
        {
            *buffer++ = base->DL;
            i++;
        }
#else
        *buffer++ = base->D;
        i++;
#endif /* FSL_FEATURE_SPI_16BIT_TRANSFERS */
    }
}

void SPI_MasterCreateHandle(spi_master_handle_t *handle, SPI_Type *base)
{
    assert(handle);

    uint8_t fifoSize[FSL_FEATURE_SOC_SPI_COUNT] = FSL_FEATURE_SPI_FIFO_SIZEx;
    uint8_t instance = SPI_GetInstance(base);
    uint8_t txSize = 0U;

    /* Initialize the handle */
    handle->base = base;
    s_spiHandle[instance] = (void *)handle;

#if defined(FSL_FEATURE_SPI_FIFO_SIZE) && (FSL_FEATURE_SPI_FIFO_SIZE > 1)
    /* Get the number to be sent if there is FIFO */
    if (fifoSize[instance] != 0)
    {
        txSize = (base->C3 & SPI_C3_TNEAREF_MARK_MASK) >> SPI_C3_TNEAREF_MARK_SHIFT;
        if (txSize == 0U)
        {
            handle->watermark = FSL_FEATURE_SPI_FIFO_SIZE * 3U / 4U;
        }
        else
        {
            handle->watermark = FSL_FEATURE_SPI_FIFO_SIZE / 2U;
        }
    }
    /* If no FIFO, just set the watermark to 1 */
    else
    {
        handle->watermark = 1U;
    }
#else
    handle->watermark = 1U;
#endif /* FSL_FEATURE_SPI_FIFO_SIZE */

/* Get the bytes per frame */
#if defined(FSL_FEATURE_SPI_16BIT_TRANSFERS) && (FSL_FEATURE_SPI_16BIT_TRANSFERS)
    handle->bytePerFrame = ((base->C2 & SPI_C2_SPIMODE_MASK) >> SPI_C2_SPIMODE_SHIFT) + 1U;
#else
    handle->bytePerFrame = 1U;
#endif

    /* Enable SPI NVIC */
    EnableIRQ(s_spiIRQ[instance]);
}

status_t SPI_MasterTransferBlocking(spi_master_handle_t *handle, spi_transfer_t *xfer)
{
    assert(handle && xfer);

    SPI_Type *base = handle->base;

    /* Check if SPI is busy */
    if (handle->state == kSPI_Busy)
    {
        return kStatus_SPI_Busy;
    }

    /* Check if the argument is legal */
    if ((xfer->sendBuff == NULL) && (xfer->receiveBuff == NULL))
    {
        return kStatus_InvalidArgument;
    }

    /* Configure the values in handle */
    handle->state = kSPI_Busy;
    handle->txBuff = xfer->sendBuff;
    handle->rxBuff = xfer->receiveBuff;
    handle->txRemainingBytes = xfer->size;
    if (handle->rxBuff)
    {
        handle->rxRemainingBytes = xfer->size;
    }
    else
    {
        handle->rxRemainingBytes = 0U;
    }

    /* Disable SPI and then enable it, this is used to clear S register */
    base->C1 &= ~SPI_C1_SPE_MASK;
    base->C1 |= SPI_C1_SPE_MASK;

    /* Send a piece of data to data register or Tx FIFO */
    SPI_SendTransaction(handle);

    /* Begin the polling transfer until all data sent */
    while ((handle->txRemainingBytes > 0) || (handle->rxRemainingBytes > 0))
    {
        /* First receive a data and then send a data */
#if defined(FSL_FEATURE_SPI_FIFO_SIZE) && (FSL_FEATURE_SPI_FIFO_SIZE > 1)
        /* If there is FIFO, wait for the near full/near empty flag */
        if (handle->watermark > 1U)
        {
            /* If need to receive data, just receive */
            if (handle->rxBuff)
            {
                while ((base->S & SPI_S_RNFULLF_MASK) == 0U)
                {
                }
                SPI_ReceiveTransaction(handle);
            }

            /* Data Sending is needed to initialize a SPI transfer*/
            while ((base->S & SPI_S_TNEAREF_MASK) == 0U)
            {
            }
            SPI_SendTransaction(handle);
        }

        /* No FIFO, just wait rxfull and txempty flags */
        else
        {
            if (handle->rxBuff)
            {
                while ((base->S & SPI_S_SPRF_MASK) == 0U)
                {
                }
                SPI_ReceiveTransaction(handle);
            }

            /* Data send */
            while ((base->S & SPI_S_SPTEF_MASK) == 0U)
            {
            }
            SPI_SendTransaction(handle);
        }
#else
        /* No FIFO, just wait rxfull and txempty flags */
        if (handle->rxBuff)
        {
            while ((base->S & SPI_S_SPRF_MASK) == 0U)
            {
            }
            SPI_ReceiveTransaction(handle);
        }

        while ((base->S & SPI_S_SPTEF_MASK) == 0U)
        {
        }
        SPI_SendTransaction(handle);
#endif
    }

    /* Set the state to idle */
    handle->state = kSPI_Idle;

    return kStatus_Success;
}

status_t SPI_MasterTransferNonBlocking(spi_master_handle_t *handle, spi_transfer_t *xfer)
{
    assert(handle && xfer);

    SPI_Type *base = handle->base;

    /* Check if SPI is busy */
    if (handle->state == kSPI_Busy)
    {
        return kStatus_SPI_Busy;
    }

    /* Check if the input arguments valid */
    if (((xfer->sendBuff == NULL) && (xfer->receiveBuff == NULL)) || (xfer->size == 0U))
    {
        return kStatus_InvalidArgument;
    }

    /* Set the handle information */
    handle->txBuff = xfer->sendBuff;
    handle->rxBuff = xfer->receiveBuff;
    handle->txRemainingBytes = xfer->size;
    if (handle->rxBuff)
    {
        handle->rxRemainingBytes = xfer->size;
    }
    else
    {
        handle->rxRemainingBytes = 0U;
    }

    /* Set the SPI state to busy */
    handle->state = kSPI_Busy;

    /* Disable SPI and then enable it, this is used to clear S register*/
    base->C1 &= ~SPI_C1_SPE_MASK;
    base->C1 |= SPI_C1_SPE_MASK;

    /* Enable Interrupt, only enable Rx interrupt, use rx interrupt to driver SPI transfer */
#if defined(FSL_FEATURE_SPI_FIFO_SIZE) && (FSL_FEATURE_SPI_FIFO_SIZE > 1)
    if (handle->watermark > 1U)
    {
        /* Enable Rx near full interrupt */
        SPI_EnableInterrupts(base, kSPI_RxFifoNearFullInterruptEnable);
    }
    else
    {
        SPI_EnableInterrupts(base, kSPI_RxFullAndModfInterruptEnable);
    }
#else
    SPI_EnableInterrupts(base, kSPI_RxFullAndModfInterruptEnable);
#endif

    /* First send a piece of data to Tx Data or FIFO to start a SPI transfer */
    SPI_SendTransaction(handle);

    return kStatus_Success;
}

size_t SPI_MasterGetRemainingBytes(spi_master_handle_t *handle)
{
    assert(handle);

    /* Return remaing bytes in different cases */
    if (handle->rxBuff)
    {
        return handle->rxRemainingBytes;
    }
    else
    {
        return handle->txRemainingBytes;
    }
}

void SPI_MasterAbortTransfer(spi_master_handle_t *handle)
{
    assert(handle);

    SPI_Type *base = handle->base;

/* Stop interrupts */
#if defined(FSL_FEATURE_SPI_FIFO_SIZE) && (FSL_FEATURE_SPI_FIFO_SIZE > 1)
    if (handle->watermark > 1U)
    {
        SPI_DisableInterrupts(base, kSPI_RxFifoNearFullInterruptEnable);
    }
    else
    {
        SPI_DisableInterrupts(base, kSPI_RxFullAndModfInterruptEnable);
    }
#else
    SPI_DisableInterrupts(base, kSPI_RxFullAndModfInterruptEnable);
#endif

    /* Transfer finished, set the state to Done*/
    handle->state = kSPI_Idle;

    /* Clear the internal state */
    handle->rxRemainingBytes = 0;
    handle->txRemainingBytes = 0;
}

void SPI_MasterSetTransferCallback(spi_master_handle_t *handle, spi_master_callback_t callback, void *userData)
{
    handle->callback = callback;
    handle->userData = userData;
}

void SPI_MasterHandleIRQ(spi_master_handle_t *handle)
{
    assert(handle);

    SPI_Type *base = handle->base;

    /* If needs to receive data, do a receive */
    if ((handle->rxBuff) && (handle->rxRemainingBytes))
    {
        SPI_ReceiveTransaction(handle);
    }

    /* We always need to send a data to make the SPI run */
    if (handle->txRemainingBytes)
    {
        SPI_SendTransaction(handle);
    }

#if defined(FSL_FEATURE_SPI_FIFO_SIZE) && (FSL_FEATURE_SPI_FIFO_SIZE > 1)
    /* Handle the last data which may less than watermark. In this case, the last data can
        never be received. Check the rxRemainingBytes, if less than watermark, do a special operation. */
    if ((handle->rxRemainingBytes < (handle->watermark * 2U)) && (handle->watermark > 1U))
    {
        while (handle->rxRemainingBytes != 0U)
        {
            while (base->S & SPI_S_RFIFOEF_MASK)
            {
            }
            SPI_ReadNonBlocking(base, handle->rxBuff, handle->bytePerFrame);

            /* Update the handle information */
            handle->rxBuff += handle->bytePerFrame;
            handle->rxRemainingBytes -= handle->bytePerFrame;
        }
    }
#endif
    /* All the transfer finished */
    if ((handle->txRemainingBytes == 0) && (handle->rxRemainingBytes == 0))
    {
        /* Complete the transfer */
        SPI_MasterAbortTransfer(handle);

        if (handle->callback)
        {
            (handle->callback)(handle, kStatus_SPI_Idle, handle->userData);
        }
    }
}

static void SPI_CommonIRQHandler(SPI_Type *base, void *handle)
{
    if (base->C1 & SPI_C1_MSTR_MASK)
    {
        SPI_MasterHandleIRQ((spi_master_handle_t *)handle);
    }
    else
    {
        SPI_SlaveHandleIRQ((spi_slave_handle_t *)handle);
    }
}

#if defined(SPI0)
void SPI0_DriverIRQHandler(void)
{
    assert(s_spiHandle[0]);
    SPI_CommonIRQHandler(SPI0, s_spiHandle[0]);
}
#endif

#if defined(SPI1)
void SPI1_DriverIRQHandler(void)
{
    assert(s_spiHandle[1]);
    SPI_CommonIRQHandler(SPI1, s_spiHandle[1]);
}
#endif

#if defined(SPI2)
void SPI2_DriverIRQHandler(void)
{
    assert(s_spiHandle[2]);
    SPI_CommonIRQHandler(SPI0, s_spiHandle[2]);
}
#endif
