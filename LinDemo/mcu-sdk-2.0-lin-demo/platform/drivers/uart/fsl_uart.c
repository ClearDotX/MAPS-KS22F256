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

#include "fsl_uart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* UART transfer state. */
enum _uart_tansfer_states
{
    kUART_TxIdle, /* TX idle. */
    kUART_TxBusy, /* TX busy. */
    kUART_RxIdle, /* RX idle. */
    kUART_RxBusy  /* RX busy. */
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Get the UART instance from peripheral base address.
 *
 * @param base UART peripheral base address.
 * @return UART instance.
 */
uint32_t UART_GetInstance(UART_Type *base);

/*!
 * @brief Get the length of received data in RX ring buffer.
 *
 * @param handle UART handle pointer.
 * @return Length of received data in RX ring buffer.
 */
static size_t UART_GetRxRingBufferLength(uart_handle_t *handle);

/*!
 * @brief Check whether the RX ring buffer is full.
 *
 * @param handle UART handle pointer.
 * @retval true  RX ring buffer is full.
 * @retval false RX ring buffer is not full.
 */
static bool UART_IsRxRingBufferFull(uart_handle_t *handle);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Array of UART handle. */
#if (defined(FSL_FEATURE_SOC_LPSCI_COUNT) && FSL_FEATURE_SOC_LPSCI_COUNT)
static uart_handle_t *s_uartHandle[FSL_FEATURE_SOC_LPSCI_COUNT + FSL_FEATURE_SOC_UART_COUNT];
#else
static uart_handle_t *s_uartHandle[FSL_FEATURE_SOC_UART_COUNT];
#endif
/* Array of UART peripheral base address. */
static UART_Type *const s_uartBase[] = UART_BASE_PTRS;

/* Array of UART IRQ number. */
static const IRQn_Type s_uartIRQ[] = UART_RX_TX_IRQS;
/* Array of UART clock name. */
static const clock_ip_name_t s_uartClock[] = UART_CLOCKS;

/*******************************************************************************
 * Code
 ******************************************************************************/

uint32_t UART_GetInstance(UART_Type *base)
{
    uint32_t instance = 0;

    while ((s_uartBase[instance] != base) && (instance < (sizeof(s_uartBase) / sizeof(s_uartBase[0]))))
    {
        instance++;
    }

    return instance;
}

static size_t UART_GetRxRingBufferLength(uart_handle_t *handle)
{
    size_t size;

    if (handle->rxRingBufferTail > handle->rxRingBufferHead)
    {
        size = (size_t)(handle->rxRingBufferHead + handle->rxRingBufferSize - handle->rxRingBufferTail);
    }
    else
    {
        size = (size_t)(handle->rxRingBufferHead - handle->rxRingBufferTail);
    }

    return size;
}

static bool UART_IsRxRingBufferFull(uart_handle_t *handle)
{
    bool full;

    if (UART_GetRxRingBufferLength(handle) == (handle->rxRingBufferSize - 1U))
    {
        full = true;
    }
    else
    {
        full = false;
    }

    return full;
}

void UART_Init(UART_Type *base, const uart_config_t *config, uint32_t srcClock_Hz)
{
    assert(config);
#if defined(FSL_FEATURE_UART_HAS_FIFO) && FSL_FEATURE_UART_HAS_FIFO
    assert(FSL_FEATURE_UART_FIFO_SIZEn(UART_GetInstance(base)) >= config->txFifoWatermark);
    assert(FSL_FEATURE_UART_FIFO_SIZEn(UART_GetInstance(base)) >= config->rxFifoWatermark);
#endif

    uint16_t sbr;
    uint8_t temp;

    /* Enable uart clock */
    CLOCK_EnableClock(s_uartClock[UART_GetInstance(base)]);

    /* Disable UART TX RX before setting. */
    base->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);

    /* Calculate the baud rate modulo divisor, sbr*/
    sbr = srcClock_Hz / (config->baudRate_Bps * 16);

    /* Write the sbr value to the BDH and BDL registers*/
    base->BDH = (base->BDH & ~UART_BDH_SBR_MASK) | (uint8_t)(sbr >> 8);
    base->BDL = (uint8_t)sbr;

#if defined(FSL_FEATURE_UART_HAS_BAUD_RATE_FINE_ADJUST_SUPPORT) && FSL_FEATURE_UART_HAS_BAUD_RATE_FINE_ADJUST_SUPPORT
    /* Determine if a fractional divider is needed to fine tune closer to the
     * desired baud, each value of brfa is in 1/32 increments,
     * hence the multiply-by-32. */
    uint16_t brfa = (32 * srcClock_Hz / (config->baudRate_Bps * 16)) - 32 * sbr;

    /* Write the brfa value to the register*/
    base->C4 = (base->C4 & ~UART_C4_BRFA_MASK) | (brfa & UART_C4_BRFA_MASK);
#endif

    /* Set bit count */
    temp = base->C1 & ~UART_C1_M_MASK;

    /* Set parity mode */
    temp = temp & ~(UART_C1_PE_MASK | UART_C1_PT_MASK);
    base->C1 = temp | (uint8_t)config->parityMode;

#if defined(FSL_FEATURE_UART_HAS_STOP_BIT_CONFIG_SUPPORT) && FSL_FEATURE_UART_HAS_STOP_BIT_CONFIG_SUPPORT
    /* Set stop bit per char */
    base->BDH = (base->BDH & ~UART_BDH_SBNS_MASK) | UART_BDH_SBNS((uint8_t)config->stopBitCount);
#endif

#if defined(FSL_FEATURE_UART_HAS_FIFO) && FSL_FEATURE_UART_HAS_FIFO
    /* Set tx/rx FIFO watermark */
    base->TWFIFO = config->txFifoWatermark;
    base->RWFIFO = config->rxFifoWatermark;

    /* Enable tx/rx FIFO */
    base->PFIFO |= (UART_PFIFO_TXFE_MASK | UART_PFIFO_RXFE_MASK);

    /* Flush FIFO */
    base->CFIFO |= (UART_CFIFO_TXFLUSH_MASK | UART_CFIFO_RXFLUSH_MASK);
#endif

    /* Enable TX/RX base on configure structure. */
    temp = base->C2;

    if (config->enableTx)
    {
        temp |= UART_C2_TE_MASK;
    }

    if (config->enableRx)
    {
        temp |= UART_C2_RE_MASK;
    }

    base->C2 = temp;
}

void UART_Deinit(UART_Type *base)
{
#if defined(FSL_FEATURE_UART_HAS_FIFO) && FSL_FEATURE_UART_HAS_FIFO
    /* Wait tx FIFO send out*/
    while (0 != base->TCFIFO)
    {
    }
#endif
    /* Wait last char shoft out */
    while (0 == (base->S1 & UART_S1_TC_MASK))
    {
    }

    /* Disable the module. */
    base->C2 = 0;

    /* Disable uart clock */
    CLOCK_DisableClock(s_uartClock[UART_GetInstance(base)]);
}

void UART_GetDefaultConfig(uart_config_t *config)
{
    assert(config);

    config->baudRate_Bps = 115200U;
    config->parityMode = kUART_ParityDisabled;
#if defined(FSL_FEATURE_UART_HAS_STOP_BIT_CONFIG_SUPPORT) && FSL_FEATURE_UART_HAS_STOP_BIT_CONFIG_SUPPORT
    config->stopBitCount = kUART_OneStopBit;
#endif
#if defined(FSL_FEATURE_UART_HAS_FIFO) && FSL_FEATURE_UART_HAS_FIFO
    config->txFifoWatermark = 0;
    config->rxFifoWatermark = 1;
#endif
    config->enableTx = false;
    config->enableRx = false;
}

void UART_EnableInterrupts(UART_Type *base, uint32_t mask)
{
    /* The interrupt mask is combined by control bits from several register: ((CFIFO<<24) | (C3<<16) | (C2<<8) |(BDH))
     */
    base->BDH |= (mask & 0xFF);
    base->C2 |= ((mask >> 8) & 0xFF);
    base->C3 |= ((mask >> 16) & 0xFF);

#if defined(FSL_FEATURE_UART_HAS_FIFO) && FSL_FEATURE_UART_HAS_FIFO
    base->CFIFO |= ((mask >> 24) & 0xFF);
#endif
}

void UART_DisableInterrupts(UART_Type *base, uint32_t mask)
{
    /* The interrupt mask is combined by control bits from several register: ((CFIFO<<24) | (C3<<16) | (C2<<8) |(BDH))
     */
    base->BDH &= ~(mask & 0xFF);
    base->C2 &= ~((mask >> 8) & 0xFF);
    base->C3 &= ~((mask >> 16) & 0xFF);

#if defined(FSL_FEATURE_UART_HAS_FIFO) && FSL_FEATURE_UART_HAS_FIFO
    base->CFIFO &= ~((mask >> 24) & 0xFF);
#endif
}

uint32_t UART_GetEnabledInterrupts(UART_Type *base)
{
    uint32_t temp;

    temp = base->BDH | ((uint32_t)(base->C2) << 8) | ((uint32_t)(base->C3) << 16);

#if defined(FSL_FEATURE_UART_HAS_FIFO) && FSL_FEATURE_UART_HAS_FIFO
    temp |= ((uint32_t)(base->CFIFO) << 24);
#endif

    return temp;
}

uint32_t UART_GetStatusFlags(UART_Type *base)
{
    uint32_t status_flag;

    status_flag = base->S1 | ((uint32_t)(base->S2) << 8);

#if defined(FSL_FEATURE_UART_HAS_EXTENDED_DATA_REGISTER_FLAGS) && FSL_FEATURE_UART_HAS_EXTENDED_DATA_REGISTER_FLAGS
    status_flag |= ((uint32_t)(base->ED) << 16);
#endif

#if defined(FSL_FEATURE_UART_HAS_FIFO) && FSL_FEATURE_UART_HAS_FIFO
    status_flag |= ((uint32_t)(base->SFIFO) << 24);
#endif

    return status_flag;
}

status_t UART_ClearStatusFlags(UART_Type *base, uint32_t mask)
{
    volatile uint8_t dummy;
    uint8_t reg = base->S2;
    status_t status;

#if defined(FSL_FEATURE_UART_HAS_LIN_BREAK_DETECT) && FSL_FEATURE_UART_HAS_LIN_BREAK_DETECT
    reg &= ~(UART_S2_RXEDGIF_MASK | UART_S2_LBKDIF_MASK);
#else
    reg &= ~UART_S2_RXEDGIF_MASK;
#endif

    base->S2 = reg | (uint8_t)(mask >> 8);

#if defined(FSL_FEATURE_UART_HAS_FIFO) && FSL_FEATURE_UART_HAS_FIFO
    base->SFIFO = (uint8_t)(mask >> 24);
#endif

    if (mask & (kUART_IdleLineFlag | kUART_RxOverrunFlag | kUART_NoiseErrorFlag | kUART_FramingErrorFlag |
                kUART_ParityErrorFlag))
    {
        /* Read base->D to clear the flags. */
        dummy = base->S1;
        dummy = base->D;
        dummy = dummy;
    }

    /* If some flags still pending. */
    if (mask & UART_GetStatusFlags(base))
    {
        /* Some flags can only clear or set by the hardware itself, these flags are: kUART_TxDataRegEmptyFlag,
        kUART_TransmissionCompleteFlag, kUART_RxDataRegFullFlag, kUART_RxActiveFlag, kUART_NoiseErrorInRxDataRegFlag,
        kUART_ParityErrorInRxDataRegFlag, kUART_TxFifoEmptyFlag, kUART_RxFifoEmptyFlag. */
        status = kStatus_UART_FlagCannotClearManually;
    }
    else
    {
        status = kStatus_Success;
    }

    return status;
}

void UART_WriteBlocking(UART_Type *base, const uint8_t *data, size_t length)
{
    /* This API can only ensure that the data is written into the data buffer but can't
    ensure all data in the data buffer are sent into the transmit shift buffer. */
    while (length--)
    {
        while (!(base->S1 & UART_S1_TDRE_MASK))
        {
        }
        base->D = *(data++);
    }
}

void UART_WriteNonBlocking(UART_Type *base, const uint8_t *data, size_t length)
{
    /* The Non Blocking write data API assume user have ensured there is enough space in
    peripheral to write. */
    for (size_t i = 0; i < length; i++)
    {
        base->D = data[i];
    }
}

void UART_ReadBlocking(UART_Type *base, uint8_t *data, size_t length)
{
    while (length--)
    {
#if defined(FSL_FEATURE_UART_HAS_FIFO) && FSL_FEATURE_UART_HAS_FIFO
        while (!base->RCFIFO)
#else
        while (!(base->S1 & UART_S1_RDRF_MASK))
#endif
        {
        }
        *(data++) = base->D;
    }
}

void UART_ReadNonBlocking(UART_Type *base, uint8_t *data, size_t length)
{
    /* The Non Blocking read data API assume user have ensured there is enough space in
    peripheral to write. */
    for (size_t i = 0; i < length; i++)
    {
        data[i] = base->D;
    }
}

void UART_CreateHandle(UART_Type *base, uart_handle_t *handle, uart_transfer_callback_t callback, void *userData)
{
    assert(handle);

    uint32_t instance;

    /* Zero the handle. */
    memset(handle, 0, sizeof(*handle));

    /* Set the TX/RX state. */
    handle->rxState = kUART_RxIdle;
    handle->txState = kUART_TxIdle;

    /* Set the callback and user data. */
    handle->callback = callback;
    handle->userData = userData;

#if defined(FSL_FEATURE_UART_HAS_FIFO) && FSL_FEATURE_UART_HAS_FIFO
    /* Note:
       Take care of the RX FIFO, RX interrupt request only assert when received bytes
       equal or more than RX water mark, there is potential issue if RX water
       mark larger than 1.
       For example, if RX FIFO water mark is 2, upper layer needs 5 bytes and
       5 bytes are received. the last byte will be saved in FIFO but not trigger
       RX interrupt because the water mark is 2.
     */
    base->RWFIFO = 1U;
#endif

    /* Get instance from peripheral base address. */
    instance = UART_GetInstance(base);

    /* Save the handle in global variables to support the double weak mechanism. */
    s_uartHandle[instance] = handle;

    /* Enable interrupt in NVIC. */
    EnableIRQ(s_uartIRQ[instance]);
}

void UART_InstallRingBuffer(UART_Type *base, uart_handle_t *handle, uint8_t *ringBuffer, size_t ringBufferSize)
{
    assert(handle);

    /* Setup the ringbuffer address */
    if (ringBuffer)
    {
        handle->rxRingBuffer = ringBuffer;
        handle->rxRingBufferSize = ringBufferSize;
        handle->rxRingBufferHead = 0U;
        handle->rxRingBufferTail = 0U;

        /* Enable the interrupt to accept the data when user need the ring buffer. */
        UART_EnableInterrupts(base, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
    }
}

void UART_UninstallRingBuffer(UART_Type *base, uart_handle_t *handle)
{
    assert(handle);

    if (handle->rxState == kUART_RxIdle)
    {
        UART_DisableInterrupts(base, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
    }

    handle->rxRingBuffer = NULL;
    handle->rxRingBufferSize = 0U;
    handle->rxRingBufferHead = 0U;
    handle->rxRingBufferTail = 0U;
}

status_t UART_SendNonBlocking(UART_Type *base, uart_handle_t *handle, uart_transfer_t *xfer)
{
    status_t status;

    /* Return error if current TX busy. */
    if (kUART_TxBusy == handle->txState)
    {
        status = kStatus_UART_TxBusy;
    }
    else
    {
        handle->txData = xfer->data;
        handle->txDataSize = xfer->dataSize;
        handle->txState = kUART_TxBusy;

        /* Enable transmiter interrupt. */
        UART_EnableInterrupts(base, kUART_TxDataRegEmptyInterruptEnable);

        status = kStatus_Success;
    }

    return status;
}

void UART_AbortSend(UART_Type *base, uart_handle_t *handle)
{
    UART_DisableInterrupts(base, kUART_TxDataRegEmptyInterruptEnable | kUART_TransmissionCompleteInterruptEnable);

    handle->txDataSize = 0;
    handle->txState = kUART_TxIdle;
}

status_t UART_ReceiveNonBlocking(UART_Type *base, uart_handle_t *handle, uart_transfer_t *xfer, size_t *receivedBytes)
{
    uint32_t i;
    status_t status;
    /* How many bytes to copy from ring buffer to user memory. */
    size_t bytesToCopy = 0U;
    /* How many bytes to receive. */
    size_t bytesToReceive;
    /* How many bytes currently have received. */
    size_t bytesCurrentReceived;
    uint32_t regPrimask = 0U;

    /* How to get data:
       1. If RX ring buffer is not enabled, then save xfer->data and xfer->dataSize
          to uart handle, enable interrupt to store received data to xfer->data. When
          all data received, trigger callback.
       2. If RX ring buffer is enabled and not empty, get data from ring buffer first.
          If there are enough data in ring buffer, copy them to xfer->data and return.
          If there are not enough data in ring buffer, copy all of them to xfer->data,
          save the xfer->data remained empty space to uart handle, receive data
          to this empty space and trigger callback when finished. */

    if (kUART_RxBusy == handle->rxState)
    {
        status = kStatus_UART_RxBusy;
    }
    else
    {
        bytesToReceive = xfer->dataSize;
        bytesCurrentReceived = 0U;

        /* If RX ring buffer is used. */
        if (handle->rxRingBuffer)
        {
            /* Disable IRQ, protect ring buffer. */
            regPrimask = __get_PRIMASK();
            __disable_irq();

            /* How many bytes in RX ring buffer currently. */
            bytesToCopy = UART_GetRxRingBufferLength(handle);

            if (bytesToCopy)
            {
                bytesToCopy = MIN(bytesToReceive, bytesToCopy);

                bytesToReceive -= bytesToCopy;

                /* Copy data from ring buffer to user memory. */
                for (i = 0U; i < bytesToCopy; i++)
                {
                    xfer->data[bytesCurrentReceived++] = handle->rxRingBuffer[handle->rxRingBufferTail];

                    /* Wrap to 0. Not use modulo (%) because it might be large and slow. */
                    if (handle->rxRingBufferTail + 1U == handle->rxRingBufferSize)
                    {
                        handle->rxRingBufferTail = 0U;
                    }
                    else
                    {
                        handle->rxRingBufferTail++;
                    }
                }
            }

            /* If ring buffer does not have enough data, still need to read more data. */
            if (bytesToReceive)
            {
                /* No data in ring buffer, save the request to UART handle. */
                handle->rxData = xfer->data + bytesCurrentReceived;
                handle->rxDataSize = bytesToReceive;
                handle->rxState = kUART_RxBusy;
            }

            /* Recover PRIMASK, enable IRQ if previously enabled. */
            __set_PRIMASK(regPrimask);
        }
        /* Ring buffer not used. */
        else
        {
            handle->rxData = xfer->data + bytesCurrentReceived;
            handle->rxDataSize = bytesToReceive;
            handle->rxState = kUART_RxBusy;

            /* Enable RX interrupt. */
            UART_EnableInterrupts(base, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
        }

        /* Return the how many bytes have read. */
        if (receivedBytes)
        {
            *receivedBytes = bytesCurrentReceived;
        }

        status = kStatus_Success;
    }

    return status;
}

void UART_AbortReceive(UART_Type *base, uart_handle_t *handle)
{
    /* Only abort the receive to handle->rxData, the RX ring buffer is still working. */
    if (!handle->rxRingBuffer)
    {
        /* Disable RX interrupt. */
        UART_DisableInterrupts(base, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
    }

    handle->rxDataSize = 0U;
    handle->rxState = kUART_RxIdle;
}

void UART_HandleIRQ(UART_Type *base, uart_handle_t *handle)
{
    uint8_t count;
    uint8_t tempCount;
    volatile uint8_t dummy;

    /* If RX overrun. */
    if (UART_S1_OR_MASK & base->S1)
    {
        /* Read base->D, otherwise the RX does not work. */
        dummy = base->D;
        dummy = dummy;

        /* Trigger callback. */
        if (handle->callback)
        {
            handle->callback(base, handle, kStatus_UART_RxHardwareOverrun, handle->userData);
        }
    }

    /* Receive data register full */
    if ((UART_S1_RDRF_MASK & base->S1) && (UART_C2_RIE_MASK & base->C2))
    {
/* Get the size that can be stored into buffer for this interrupt. */
#if defined(FSL_FEATURE_UART_HAS_FIFO) && FSL_FEATURE_UART_HAS_FIFO
        count = base->RCFIFO;
#else
        count = 1;
#endif

        /* If handle->rxDataSize is not 0, first save data to handle->rxData. */
        while ((count) && (handle->rxDataSize))
        {
#if defined(FSL_FEATURE_UART_HAS_FIFO) && FSL_FEATURE_UART_HAS_FIFO
            tempCount = MIN(handle->rxDataSize, count);
#else
            tempCount = 1;
#endif

            /* Using non block API to read the data from the registers. */
            UART_ReadNonBlocking(base, handle->rxData, tempCount);
            handle->rxData += tempCount;
            handle->rxDataSize -= tempCount;
            count -= tempCount;

            /* If all the data required for upper layer is ready, trigger callback. */
            if (!handle->rxDataSize)
            {
                handle->rxState = kUART_RxIdle;

                if (handle->callback)
                {
                    handle->callback(base, handle, kStatus_UART_RxIdle, handle->userData);
                }
            }
        }

        /* If use RX ring buffer, receive data to ring buffer. */
        if (handle->rxRingBuffer)
        {
            while (count--)
            {
                /* If RX ring buffer is full, trigger callback to notify over run. */
                if (UART_IsRxRingBufferFull(handle))
                {
                    if (handle->callback)
                    {
                        handle->callback(base, handle, kStatus_UART_RxRingBufferOverrun, handle->userData);
                    }
                }

                /* If ring buffer is still full after callback function, the oldest data is overrided. */
                if (UART_IsRxRingBufferFull(handle))
                {
                    /* Increase handle->rxRingBufferTail to make room for new data. */
                    if (handle->rxRingBufferTail + 1U == handle->rxRingBufferSize)
                    {
                        handle->rxRingBufferTail = 0U;
                    }
                    else
                    {
                        handle->rxRingBufferTail++;
                    }
                }

                /* Read data. */
                handle->rxRingBuffer[handle->rxRingBufferHead] = base->D;

                /* Increase handle->rxRingBufferHead. */
                if (handle->rxRingBufferHead + 1U == handle->rxRingBufferSize)
                {
                    handle->rxRingBufferHead = 0U;
                }
                else
                {
                    handle->rxRingBufferHead++;
                }
            }
        }
        /* If no receive requst pending, stop RX interrupt. */
        else if (!handle->rxDataSize)
        {
            UART_DisableInterrupts(base, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
        }
    }

    /* Send data register empty and the interrupt is enabled. */
    if ((base->S1 & UART_S1_TDRE_MASK) && (base->C2 & UART_C2_TIE_MASK))
    {
/* Get the bytes that available at this moment. */
#if defined(FSL_FEATURE_UART_HAS_FIFO) && FSL_FEATURE_UART_HAS_FIFO
        count = FSL_FEATURE_UART_FIFO_SIZEn(UART_GetInstance(base)) - base->TCFIFO;
#else
        count = 1;
#endif

        while ((count) && (handle->txDataSize))
        {
#if defined(FSL_FEATURE_UART_HAS_FIFO) && FSL_FEATURE_UART_HAS_FIFO
            tempCount = MIN(handle->txDataSize, count);
#else
            tempCount = 1;
#endif

            /* Using non block API to write the data to the registers. */
            UART_WriteNonBlocking(base, handle->txData, tempCount);
            handle->txData += tempCount;
            handle->txDataSize -= tempCount;
            count -= tempCount;

            /* If all the data are written to data register, TX finished. */
            if (!handle->txDataSize)
            {
                handle->txState = kUART_TxIdle;

                /* Disable TX register empty interrupt. */
                base->C2 = (base->C2 & ~UART_C2_TIE_MASK);

                /* Trigger callback. */
                if (handle->callback)
                {
                    handle->callback(base, handle, kStatus_UART_TxIdle, handle->userData);
                }
            }
        }
    }
}

void UART_HandleErrorIRQ(UART_Type *base, uart_handle_t *handle)
{
    /* TODO: To be implemented. */
}

#if defined(UART0)
#if ((!(defined(FSL_FEATURE_SOC_LPSCI_COUNT))) || \
     ((defined(FSL_FEATURE_SOC_LPSCI_COUNT)) && (FSL_FEATURE_SOC_LPSCI_COUNT == 0)))
void UART0_DriverIRQHandler(void)
{
    assert(s_uartHandle[0]);

    UART_HandleIRQ(UART0, s_uartHandle[0]);
}

void UART0_RX_TX_DriverIRQHandler(void)
{
    UART0_DriverIRQHandler();
}
#endif
#endif

#if defined(UART1)
void UART1_DriverIRQHandler(void)
{
    assert(s_uartHandle[1]);

    UART_HandleIRQ(UART1, s_uartHandle[1]);
}

void UART1_RX_TX_DriverIRQHandler(void)
{
    UART1_DriverIRQHandler();
}
#endif

#if defined(UART2)
void UART2_DriverIRQHandler(void)
{
    assert(s_uartHandle[2]);

    UART_HandleIRQ(UART2, s_uartHandle[2]);
}

void UART2_RX_TX_DriverIRQHandler(void)
{
    UART2_DriverIRQHandler();
}
#endif

#if defined(UART3)
void UART3_DriverIRQHandler(void)
{
    assert(s_uartHandle[3]);

    UART_HandleIRQ(UART3, s_uartHandle[3]);
}

void UART3_RX_TX_DriverIRQHandler(void)
{
    UART3_DriverIRQHandler();
}
#endif

#if defined(UART4)
void UART4_DriverIRQHandler(void)
{
    assert(s_uartHandle[4]);

    UART_HandleIRQ(UART4, s_uartHandle[4]);
}

void UART4_RX_TX_DriverIRQHandler(void)
{
    UART4_DriverIRQHandler();
}
#endif

#if defined(UART5)
void UART5_DriverIRQHandler(void)
{
    assert(s_uartHandle[5]);

    UART_HandleIRQ(UART5, s_uartHandle[5]);
}

void UART5_RX_TX_DriverIRQHandler(void)
{
    UART5_DriverIRQHandler();
}
#endif
