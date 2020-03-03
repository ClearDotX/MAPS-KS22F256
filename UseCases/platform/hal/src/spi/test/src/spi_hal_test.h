/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
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
#if !defined(_HAL_TEST_H_)
#define _HAL_TEST_H_


#include "fsl_device_registers.h"
#include "fsl_spi_hal.h"
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

enum{
  HAL_INIT = 0,
  HAL_ENABLE,
  HAL_DISABLE,
  HAL_SET_BAUD,
  HAL_SET_BAUD_DIVISOR,
  HAL_SET_MASTER_SLAVE,
  HAL_SET_SLAVE_OUTPUT_MODE,
  HAL_SET_DATA_FORMAT,
  HAL_SET_PIN_MODE,
  HAL_CONFIGURE_DMA,
  HAL_SET_STOP_WAIT_MODE,
  HAL_ENABLE_RF_INTERRUPT,
  HAL_DISABLE_RF_INTERRUPT,
  HAL_ENABLE_TM_INTERRUPT,
  HAL_DISABLE_TM_INTERRUPT,
  HAL_ENABLE_MATCH_INTERRUPT,
  HAL_DISABLE_MATCH_INTERRUPT,
  HAL_CLEAR_MODE_FAULT,
  HAL_CLEAR_MATCH,
  HAL_SET_MATCH,
  HAL_SPI8_SPI16_MODE,
  HAL_SPI_INTCLR,
  HAL_SPI_FIFOMODE,
  HAL_SPI_FIFOINTMODE,
  HAL_SPI_STATUSFLAG,
};

#define HW_SPI_HAL_TEST_COUNT  (HAL_SPI_STATUSFLAG+1)

/*! @addtogroup spi_hal*/
/*! @{*/

/*! @file*/

/**/
/* hal test functions*/
/**/

#if defined(__cplusplus)
extern "C" {
#endif

/*! @name test hal init*/
/*@{*/

/*!
 * @brief test for spi init
 */
spi_status_t test_SPI_HAL_Init(SPI_Type * base);

/*!
 * @brief test Enable the SPI peripheral.
 */
static inline spi_status_t test_SPI_HAL_Enable(SPI_Type * base)
{
    SPI_HAL_Enable(base);
    if(SPI_RD_C1(base)& SPI_C1_SPE_MASK)
    {
        return kStatus_SPI_Success;
    }
    else
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
}

/*!
 * @brief test Disable the SPI peripheral.
 */
static inline spi_status_t test_SPI_HAL_Disable(SPI_Type * base)
{

    SPI_HAL_Disable(base);
    if(SPI_RD_C1(base)& SPI_C1_SPE_MASK)
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
    else
    {
        return kStatus_SPI_Success;
    }
}

/*!
 * @brief Set the SPI baud rate in kilobits per second.
 */
spi_status_t test_SPI_HAL_SetBaud(SPI_Type * base, uint32_t bitsPerSec, uint32_t sourceClock);

/*!
 * @brief test Configure the baud rate divisors manually.
 */
static inline spi_status_t test_SPI_HAL_SetBaudDivisors(SPI_Type * base, uint32_t prescaleDivisor, uint32_t rateDivisor)
{

    SPI_HAL_SetBaudDivisors(base, prescaleDivisor, rateDivisor);
    if(( SPI_RD_BR_SPPR(base) == prescaleDivisor) && ( SPI_RD_BR_SPR(base) == rateDivisor))
    {
        return kStatus_SPI_Success;
    }
    else
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
}

/*!
 * @brief test Configure SPI for master or slave.
 */
static inline spi_status_t test_SPI_HAL_SetMasterSlave(SPI_Type * base, spi_master_slave_mode_t mode)
{

    SPI_HAL_SetMasterSlave(base, mode);
    if(SPI_RD_C1_MSTR(base) == mode)
    {
        return kStatus_SPI_Success;
    }
    else
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
}

/*!
 * @brief Set how the slave select output operates.
 */
spi_status_t test_SPI_HAL_SetSlaveSelectOutputMode(SPI_Type * base, spi_ss_output_mode_t mode);

/*!
 * @brief test Set the polarity, phase, and shift direction.
 */
spi_status_t test_SPI_HAL_SetDataFormat(SPI_Type * base,
    spi_clock_polarity_t polarity,
    spi_clock_phase_t phase,
    spi_shift_direction_t direction);

/*!
 * @brief Set the SPI pin mode.
 */
spi_status_t test_SPI_HAL_SetPinMode(SPI_Type * base, spi_pin_mode_t mode);

/*@}*/

/*! @name DMA*/
/*@{*/

/*!
 * @brief test Configure transmit and receive DMA requests.
 */
spi_status_t test_SPI_HAL_ConfigureDma(SPI_Type * base, bool enableTransmit, bool enableReceive);

/*@}*/

/*! @name Low power*/
/*@{*/

/*!
 * @brief test Enable or disable the SPI clock to stop when the CPU enters wait mode.
 */
static inline spi_status_t test_SPI_HAL_ConfigureStopInWaitMode(SPI_Type * base, bool enable)
{

    SPI_BWR_C2_SPISWAI(base,enable);
    uint8_t setByte = enable? 1 : 0 ;
    if(SPI_RD_C2_SPISWAI(base) == setByte)
    {
        return kStatus_SPI_Success;
    }
    else
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
}

/*@}*/

/*! @name Interrupts*/
/*@{*/

/*!
 * @brief test Enable the receive buffer full and mode fault interrupt.
 */
static inline spi_status_t test_SPI_HAL_Enable_receive_and_fault_interrupt(SPI_Type * base)
{

    SPI_HAL_SetIntMode(base, kSpiRxFullAndModfInt, true); 
    if(SPI_RD_C1_SPIE(base))
    {
        return kStatus_SPI_Success;
    }
    else
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
}

/*!
 * @brief test Disable the receive buffer full and mode fault interrupt.
 */
static inline spi_status_t test_SPI_HAL_Disable_receive_and_fault_interrupt(SPI_Type * base)
{

    SPI_HAL_SetIntMode(base, kSpiRxFullAndModfInt, false); 
    if(!(SPI_RD_C1_SPIE(base)))
    {
        return kStatus_SPI_Success;
    }
    else
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
}

/*!
 * @brief test Enable the transmit buffer empty interrupt.
 */
static inline spi_status_t test_SPI_HAL_Enable_transmit_interrupt(SPI_Type * base)
{

    SPI_HAL_SetIntMode(base, kSpiTxEmptyInt, true);
    if(SPI_RD_C1_SPTIE(base))
    {
        return kStatus_SPI_Success;
    }
    else
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
}

/*!
 * @brief test Disable the transmit buffer empty interrupt.
 */
static inline spi_status_t test_SPI_HAL_Disable_transmit_interrupt(SPI_Type * base)
{

    SPI_HAL_SetIntMode(base, kSpiTxEmptyInt, false);
    if(!(SPI_RD_C1_SPTIE(base)))
    {
        return kStatus_SPI_Success;
    }
    else
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
}

/*!
 * @brief test Enable the match interrupt.
 */
static inline spi_status_t test_SPI_HAL_Enable_match_interrupt(SPI_Type * base)
{

    SPI_HAL_SetIntMode(base, kSpiMatchInt, true);
    if(SPI_RD_C2_SPMIE(base))
    {
        return kStatus_SPI_Success;
    }
    else
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
}

/*!
 * @brief test Disable the match interrupt.
 */
static inline spi_status_t test_SPI_HAL_Disable_match_interrupt(SPI_Type * base)
{

     SPI_HAL_SetIntMode(base, kSpiMatchInt, false);
    if(!(SPI_RD_C2_SPMIE(base)))
    {
        return kStatus_SPI_Success;
    }
    else
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
}


/*!
 * @brief Clear the mode fault flag.
 */
spi_status_t test_SPI_HAL_ClearModeFault(SPI_Type * base);


/*!
 * @brief Clear the match flag.
 */
spi_status_t test_SPI_HAL_ClearMatch(SPI_Type * base);


/*! @name Match byte*/
/*@{*/

/*!
 * @brief Set the value which will trigger the match interrupt.
 */
static inline spi_status_t test_SPI_HAL_SetMatchVal(SPI_Type * base, uint8_t matchByte)
{
#if FSL_FEATURE_SPI_16BIT_TRANSFERS
    uint8_t temp = ~matchByte;
    SPI_HAL_Set8or16BitMode(base, kSpi16BitMode);
    SPI_HAL_SetMatchValueHigh(base, matchByte);
    SPI_HAL_SetMatchValueLow(base, temp);
    if ((SPI_RD_MH(base)== matchByte) && (SPI_RD_ML(base)== temp))
    {
        return kStatus_SPI_Success;
    }
    else
    {
      return (spi_status_t)(!kStatus_SPI_Success);
    }
#else

    SPI_HAL_SetMatchValue(base, matchByte);
    if(SPI_RD_M(base)== matchByte)
    {
        return kStatus_SPI_Success;
    }
    else
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
#endif
}

#if FSL_FEATURE_SPI_16BIT_TRANSFERS
static inline spi_status_t test_SPI_HAL_Set8or16BitMode(SPI_Type * base)
{
    spi_status_t errorCode;

    SPI_HAL_Set8or16BitMode(base, kSpi16BitMode);
    if (SPI_HAL_Get8or16BitMode(base) == kSpi16BitMode)
    {
        errorCode = kStatus_SPI_Success;
    }
    else
    {
        errorCode = (spi_status_t)(!kStatus_SPI_Success);
    }
    SPI_HAL_Set8or16BitMode(base, kSpi8BitMode);
    if (SPI_HAL_Get8or16BitMode(base) == kSpi8BitMode)
    {
        errorCode = kStatus_SPI_Success;
    }
    else
    {
        errorCode = (spi_status_t)(!kStatus_SPI_Success);
    }
    return errorCode;
}
#endif

#if FSL_FEATURE_SPI_FIFO_SIZE
static inline spi_status_t test_SPI_HAL_SetIntClearCmd(SPI_Type * base)
{
    spi_status_t errorCode;

    SPI_HAL_SetIntClearCmd(base, true);
    if (SPI_RD_C3_INTCLR(base) == 1)
    {
        errorCode =  kStatus_SPI_Success;
    }
    else
    {
        errorCode =  (spi_status_t)(!kStatus_SPI_Success);
    }
    SPI_HAL_SetIntClearCmd(base, false);
    if (SPI_RD_C3_INTCLR(base) == 0)
    {
        errorCode =  kStatus_SPI_Success;
    }
    else
    {
        errorCode =  (spi_status_t)(!kStatus_SPI_Success);
    }
    return errorCode;
}

static inline spi_status_t test_SPI_HAL_SetFifoMode(SPI_Type * base)
{
    spi_status_t errorCode;

    SPI_HAL_SetFifoMode(base, true, kSpiTxFifoOneHalfEmpty, kSpiRxFifoOneHalfFull);
    if ((SPI_RD_C3_FIFOMODE(base) == 1) &&
        (SPI_RD_C3_TNEAREF_MARK(base) == kSpiTxFifoOneHalfEmpty) &&
        (SPI_RD_C3_RNFULLF_MARK(base) == kSpiRxFifoOneHalfFull))
    {
        errorCode =  kStatus_SPI_Success;
    }
    else
    {
        errorCode =  (spi_status_t)(!kStatus_SPI_Success);
    }
    SPI_HAL_SetFifoMode(base, false, kSpiTxFifoOneFourthEmpty, kSpiRxFifoThreeFourthsFull);
    if ((SPI_RD_C3_FIFOMODE(base) == 0) &&
        (SPI_RD_C3_TNEAREF_MARK(base) == kSpiTxFifoOneFourthEmpty) &&
        (SPI_RD_C3_RNFULLF_MARK(base) == kSpiRxFifoThreeFourthsFull))
    {
        errorCode =  kStatus_SPI_Success;
    }
    else
    {
        errorCode =  (spi_status_t)(!kStatus_SPI_Success);
    }
    return errorCode;
}

static inline spi_status_t test_SPI_HAL_SetFifoIntCmd(SPI_Type * base)
{
    spi_status_t errorCode;

    SPI_HAL_SetFifoIntCmd(base, kSpiRxFifoNearFullInt, true);
    if (SPI_RD_C3_RNFULLIEN(base) == 1)
    {
        errorCode =  kStatus_SPI_Success;
    }
    else
    {
        errorCode =  (spi_status_t)(!kStatus_SPI_Success);
    }

    SPI_HAL_SetFifoIntCmd(base, kSpiRxFifoNearFullInt, false);
    if (SPI_RD_C3_RNFULLIEN(base) == 0)
    {
        errorCode =  kStatus_SPI_Success;
    }
    else
    {
        errorCode =  (spi_status_t)(!kStatus_SPI_Success);
    }

    SPI_HAL_SetFifoIntCmd(base, kSpiTxFifoNearEmptyInt, true);
    if (SPI_RD_C3_TNEARIEN(base) == 1)
    {
        errorCode =  kStatus_SPI_Success;
    }
    else
    {
        errorCode =  (spi_status_t)(!kStatus_SPI_Success);
    }

    SPI_HAL_SetFifoIntCmd(base, kSpiTxFifoNearEmptyInt, false);
    if (SPI_RD_C3_TNEARIEN(base) == 0)
    {
        errorCode =  kStatus_SPI_Success;
    }
    else
    {
        errorCode =  (spi_status_t)(!kStatus_SPI_Success);
    }

    return errorCode;
}

static inline spi_status_t test_SPI_HAL_GetFifoStatusFlag(SPI_Type * base)
{
    spi_status_t errorCode = kStatus_SPI_Success;
    uint32_t i;
    spi_fifo_error_flag_t errorFlag;

    SPI_HAL_Enable(base);
    SPI_HAL_SetMasterSlave(base, kSpiMaster);
    SPI_HAL_SetFifoMode(base, true, kSpiTxFifoOneHalfEmpty, kSpiRxFifoOneHalfFull);

    if (SPI_HAL_GetFifoStatusFlag(base, kSpiRxFifoEmpty) != 1)
    {
        errorCode =  (spi_status_t)(!kStatus_SPI_Success);
    }

    if (SPI_HAL_GetFifoStatusFlag(base, kSpiTxFifoFull) != 0)
    {
        errorCode =  (spi_status_t)(!kStatus_SPI_Success);
    }

    if (SPI_HAL_GetFifoStatusFlag(base, kSpiTxNearEmpty) != 1)
    {
        errorCode =  (spi_status_t)(!kStatus_SPI_Success);
    }

    if (SPI_HAL_GetFifoStatusFlag(base, kSpiRxNearFull) != 0)
    {
        errorCode =  (spi_status_t)(!kStatus_SPI_Success);
    }

    /* Cause an overflow condition in the TX FIFO */
    for (i = 0; i <= 10; i++)
    {
        SPI_HAL_WriteDataLow(base, 0xAA);
    }

    if (SPI_HAL_GetFifoStatusFlag(base, kSpiTxFifoFull) != 1)
    {
        errorCode =  (spi_status_t)(!kStatus_SPI_Success);
    }

    if (SPI_HAL_GetFifoStatusFlag(base, kSpiTxNearEmpty) != 0)
    {
        errorCode =  (spi_status_t)(!kStatus_SPI_Success);
    }

    while (SPI_HAL_IsReadBuffFullPending(base) == 0);

    if (SPI_HAL_GetFifoStatusFlag(base, kSpiRxNearFull) != 1)
    {
        errorCode =  (spi_status_t)(!kStatus_SPI_Success);
    }

    if (SPI_HAL_GetFifoStatusFlag(base, kSpiRxFifoEmpty) != 0)
    {
        errorCode =  (spi_status_t)(!kStatus_SPI_Success);
    }

    /* wait till the TX FIFO is done transmitting and then transmit some more */
    while (SPI_HAL_IsTxBuffEmptyPending(base) == 0) { }
    /* Cause an overflow condition in the RX FIFO */
    for (i = 0; i <= 10; i++)
    {
        SPI_HAL_WriteDataLow(base, 0xAA);
    }
    /* wait again till the TX FIFO is done transmitting to amke sure RX FIFO is overflowed */
    while (SPI_HAL_IsTxBuffEmptyPending(base) == 0) { }

    /* Now check for TX and RX FIFO overflows */
    errorFlag = SPI_HAL_GetFifoErrorFlag(base);

    if (errorFlag != kSpiRxfofTxfof)
    {
        errorCode =  (spi_status_t)(!kStatus_SPI_Success);
    }

    /* Now make sure there are no more error flags */
    errorFlag = SPI_HAL_GetFifoErrorFlag(base);

    if (errorFlag != kSpiNoFifoError)
    {
        errorCode =  (spi_status_t)(!kStatus_SPI_Success);
    }

    /* Now make sure there are no more error flags */
    return errorCode;
}

#endif

/*! @brief init hardware*/
/*! @param none*/
/*! @return none*/
void init_hardware(void);

/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* __FSL_SPI_HAL_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

