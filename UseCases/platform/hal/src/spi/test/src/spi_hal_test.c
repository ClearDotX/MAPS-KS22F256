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
#include "fsl_device_registers.h"
#include <assert.h>
#include <math.h>
#include "spi_hal_test.h"
#include "fsl_clock_manager.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Bit offsets for bits encoded in enum values.*/
enum _spi_pin_bit_encodings
{
    kSpiSsoeBit = 0U,    /*!< SSOE is bit 0 of #spi_ss_output_mode_t.*/
    kSpiModfenBit = 1U,  /*!< MODFEN is bit 1 of #spi_ss_output_mode_t.*/
    kSpiSpc0Bit = 0U,    /*!< SPC0 is bit 0 of #spi_pin_mode_t.*/
    kSpiBidiroeBit = 1U  /*!< BIDIROE is bit 1 of #spi_pin_mode_t.*/
};

/*******************************************************************************
 * Code
 ******************************************************************************/

/* See test_spi_hal.h for documentation of this function.*/
spi_status_t test_SPI_HAL_Init(SPI_Type * base)
{
    SPI_HAL_Init(base);

    if((SPI_RD_C1(base)&SPI_C1_CPHA_MASK) != SPI_C1_CPHA_MASK)
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
    if(SPI_RD_C2(base) != 0)
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
    if(SPI_RD_BR(base)!= 0)
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }

#if FSL_FEATURE_SPI_16BIT_TRANSFERS

    if(SPI_RD_MH(base)!= 0)
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }

    if(SPI_RD_ML(base)!= 0)
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }

#else

    if(SPI_RD_M(base)!= 0)
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }

#endif

    return kStatus_SPI_Success;
}

/* See test_spi_hal.h for documentation of this function.*/
spi_status_t test_SPI_HAL_SetBaud(SPI_Type * base, uint32_t bitsPerSec, uint32_t sourceClock)
{
    uint32_t setHz;
    uint32_t bestBaudRate;
    uint8_t sppr, spr;
//    uint32_t hz = bitsPerSec;

    bestBaudRate = SPI_HAL_SetBaud(base, bitsPerSec, sourceClock);
    spr = SPI_RD_BR_SPR(base)+1;
    sppr = SPI_RD_BR_SPPR(base)+1;
    setHz = lrint(sourceClock/(sppr *pow(2,spr)));
    uint32_t error = (setHz > bestBaudRate) ? setHz - bestBaudRate : bestBaudRate - setHz;

    if(error >1)
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
    else
    {
        return kStatus_SPI_Success;
    }
}

/* See test_spi_hal.h for documentation of this function.*/
spi_status_t test_SPI_HAL_SetSlaveSelectOutputMode(SPI_Type * base, spi_ss_output_mode_t mode)
{
    SPI_HAL_SetSlaveSelectOutputMode(base, mode);
    /* The mode enum values encode the SSOE and MODFEN bit values.*/
    /* Bit 0: SSOE*/
    /* Bit 1: MODFEN*/
    if(SPI_RD_C1_SSOE(base)!=((uint32_t)mode & (1U << kSpiSsoeBit)) >> kSpiSsoeBit)
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
    if(SPI_RD_C2_MODFEN(base) !=((uint32_t)mode & (1U << kSpiModfenBit)) >> kSpiModfenBit)
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }

    return kStatus_SPI_Success;
}

/* See test_spi_hal.h for documentation of this function.*/
spi_status_t test_SPI_HAL_SetDataFormat(SPI_Type * base,
    spi_clock_polarity_t polarity,
    spi_clock_phase_t phase,
    spi_shift_direction_t direction)
{
    SPI_HAL_SetDataFormat(base, polarity, phase, direction);
    if(SPI_RD_C1_CPOL(base) != polarity)
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
    if(SPI_RD_C1_CPHA(base) != phase)
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
    if(SPI_RD_C1_LSBFE(base) != direction)
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
    return kStatus_SPI_Success;
}

/* See test_spi_hal.h for documentation of this function.*/
spi_status_t test_SPI_HAL_SetPinMode(SPI_Type * base, spi_pin_mode_t mode)
{
    SPI_HAL_SetPinMode(base, mode);
    /* The mode enum values encode the SPC0 and BIDIROE bit values.*/
    /* Bit 0: SPC0*/
    /* Bit 1: BIDIROE*/
    if(SPI_RD_C2_SPC0(base) != ((uint32_t)mode & (1U << kSpiSpc0Bit)) >> kSpiSpc0Bit)
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
    if(SPI_RD_C2_BIDIROE(base)!= ((uint32_t)mode & (1U << kSpiBidiroeBit)) >> kSpiBidiroeBit)
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
    return kStatus_SPI_Success;
}

#if FSL_FEATURE_SPI_HAS_DMA_SUPPORT
/* See test_spi_hal.h for documentation of this function.*/
spi_status_t test_SPI_HAL_ConfigureDma(SPI_Type * base, bool enableTransmit, bool enableReceive)
{
    uint8_t setByte;
    SPI_HAL_SetTxDmaCmd(base, enableTransmit);
    setByte = enableTransmit ? 1 : 0 ;
    if(SPI_RD_C2_TXDMAE(base) != setByte)
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }

    SPI_HAL_SetRxDmaCmd(base, enableReceive);
    setByte = enableReceive ? 1 : 0 ;
    if(SPI_RD_C2_RXDMAE(base) != setByte)
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
    return kStatus_SPI_Success;
}
#endif

/* See fsl_spi_hal.h for documentation of this function.*/
spi_status_t test_SPI_HAL_ClearModeFault(SPI_Type * base)
{
    SPI_HAL_ClearModeFaultFlag(base);
    /* Must make sure we read from the status register first.*/
    if (SPI_HAL_IsModeFaultPending(base))
    {
        return (spi_status_t)(!kStatus_SPI_Success);
    }
    return kStatus_SPI_Success;
}

/* See fsl_spi_hal.h for documentation of this function.*/
spi_status_t test_SPI_HAL_ClearMatch(SPI_Type * base)
{
    SPI_HAL_ClearMatchFlag(base);

    if (SPI_HAL_IsMatchPending(base))
    {
       return (spi_status_t)(!kStatus_SPI_Success);
    }
    return kStatus_SPI_Success;
}
/*******************************************************************************
 * EOF
 ******************************************************************************/

