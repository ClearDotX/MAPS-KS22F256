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
#include "fsl_crc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined(CRC_DRIVER_USE_CRC16_CCIT_FALSE_AS_DEFAULT) && CRC_DRIVER_USE_CRC16_CCIT_FALSE_AS_DEFAULT
/* @brief Default user configuration structure for CRC-16-CCITT */
#define CRC_DRIVER_DEFAULT_POLYNOMIAL 0x1021U
/*< CRC-16-CCIT polynomial x**16 + x**12 + x**5 + x**0 */
#define CRC_DRIVER_DEFAULT_SEED 0xFFFFU
/*< Default initial checksum */
#define CRC_DRIVER_DEFAULT_READ_TRANSPOSE kCrcTransposeNone
/*< Default is without transpose on reading of CRC data register */
#define CRC_DRIVER_DEFAULT_WRITE_TRANSPOSE kCrcTransposeBytes
/*< Default is transpose bytes when writing CRC data register.
 * The driver tries to use 32-bit writes, and the CRC module requires MSByte
 * to be written to CRC_DATA[HU].
 * Example: {0x00, 0x01, 0x02, 0x03} is 0x03020100 (little endian) but 0x00010203 shall be written,
 *          thus, enable write transpose of bytes.
 */
#define CRC_DRIVER_DEFAULT_COMPLEMENT_CHECKSUM false
/*< Default is without complement of CRC data register read data */
#define CRC_DRIVER_DEFAULT_CRC_BITS kCrcBits16
/*< Default is 16-bit CRC protocol */
#endif /* CRC_DRIVER_USE_CRC16_CCIT_FALSE_AS_DEFAULT */

/*******************************************************************************
 * Code
 ******************************************************************************/
void CRC_Configure(CRC_Type *base, const crc_config_t *config)
{
    uint32_t crcControl;

    /* pre-compute value for CRC control registger based on user configuraton without WAS field */
    crcControl = 0 | CRC_CTRL_TOT(config->writeTranspose) | CRC_CTRL_TOTR(config->readTranspose) |
                 CRC_CTRL_FXOR(config->complementChecksum) | CRC_CTRL_TCRC(config->crcBits);

    /* make sure the control register is clear - WAS is deasserted, and protocol is set */
    base->CTRL = crcControl;

    /* write polynomial register */
    base->GPOLY = config->polynomial;

    /* write pre-computed control register value along with WAS to start checksum computation */
    base->CTRL = crcControl | CRC_CTRL_WAS(true);

    /* write seed (initial checksum) */
    base->DATA = config->seed;

    /* deassert WAS by writing pre-computed CRC control register value */
    base->CTRL = crcControl;
}

void CRC_GetDefaultConfig(crc_config_t *config)
{
    static const crc_config_t crc16ccit = {
        CRC_DRIVER_DEFAULT_POLYNOMIAL,          CRC_DRIVER_DEFAULT_SEED,
        CRC_DRIVER_DEFAULT_READ_TRANSPOSE,      CRC_DRIVER_DEFAULT_WRITE_TRANSPOSE,
        CRC_DRIVER_DEFAULT_COMPLEMENT_CHECKSUM, CRC_DRIVER_DEFAULT_CRC_BITS,
    };

    *config = crc16ccit;
}

void CRC_WriteData(CRC_Type *base, const uint8_t *data, size_t dataSize)
{
    const uint32_t *data32;

    /* 8-bit reads and writes till source address is aligned 4 bytes */
    while ((dataSize) && ((uint32_t)data & 3U))
    {
        base->ACCESS8BIT.DATALL = *data;
        data++;
        dataSize--;
    }

    /* use 32-bit reads and writes as long as possible */
    data32 = (const uint32_t *)data;
    while (dataSize >= sizeof(uint32_t))
    {
        base->DATA = *data32;
        data32++;
        dataSize -= sizeof(uint32_t);
    }

    data = (const uint8_t *)data32;

    /* 8-bit reads and writes till end of data buffer */
    while (dataSize)
    {
        base->ACCESS8BIT.DATALL = *data;
        data++;
        dataSize--;
    }
}

uint16_t CRC_Get16bitResult(CRC_Type *base)
{
    uint32_t retval;
    uint32_t totr; /* type of transpose read bitfield */

    retval = base->DATA;
    totr = (base->CTRL & CRC_CTRL_TOTR_MASK) >> CRC_CTRL_TOTR_SHIFT;

    /* check transpose type to get 16-bit out of 32-bit register */
    if (totr >= 2U)
    {
        /* transpose of bytes for read is set, the result CRC is in CRC_DATA[HU:HL] */
        retval &= 0xFFFF0000U;
        retval = retval >> 16U;
    }
    else
    {
        /* no transpose of bytes for read, the result CRC is in CRC_DATA[LU:LL] */
        retval &= 0x0000FFFFU;
    }
    return (uint16_t)retval;
}
