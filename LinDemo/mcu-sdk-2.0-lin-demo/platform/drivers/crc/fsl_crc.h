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

#ifndef _FSL_CRC_H_
#define _FSL_CRC_H_

#include "fsl_common.h"

/*!
 * @addtogroup crc_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief CRC driver version. */
#define FSL_CRC_DRIVER_VERSION (MAKE_VERSION(2, 0, 0)) /*!< Version 2.0.0 */

/*! @brief Default configuration struct filled by CRC_GetDefaultConfig(). */
#ifndef CRC_DRIVER_CUSTOM_DEFAULTS
#define CRC_DRIVER_USE_CRC16_CCIT_FALSE_AS_DEFAULT 1
#endif

/*! @brief CRC type of transpose of read write data */
typedef enum _crc_transpose_type
{
    kCrcTransposeNone = 0U,         /*! No transpose  */
    kCrcTransposeBits = 1U,         /*! Tranpose bits in bytes  */
    kCrcTransposeBitsAndBytes = 2U, /*! Transpose bytes and bits in bytes */
    kCrcTransposeBytes = 3U,        /*! Transpose bytes */
} crc_transpose_type_t;

/*! @brief CRC bit width */
typedef enum _crc_bits
{
    kCrcBits16 = 0U, /*! Generate 16-bit CRC code  */
    kCrcBits32 = 1U  /*! Generate 32-bit CRC code  */
} crc_bits_t;

/*!
* @brief CRC module configuration.
*
* This structure holds the configuration for the CRC module.
*/
typedef struct _crc_config
{
    uint32_t polynomial;                 /*!< CRC Polynomial, MSBit first.@n
                                              Example polynomial: 0x1021 = 1_0000_0010_0001 = x^12+x^5+1 */
    uint32_t seed;                       /*!< Starting checksum value */
    crc_transpose_type_t readTranspose;  /*!< Type of transpose when reading CRC result.
                                              Use CRC_GetTransposeTypeFromReflectOut() function to set this value. */
    crc_transpose_type_t writeTranspose; /*!< Type of transpose when writing CRC input data.
                                              Use CRC_GetTransposeTypeFromReflectIn() functions to set this value. */
    bool complementChecksum;             /*!< True if the result shall be complement of the actual checksum. */
    crc_bits_t crcBits;                  /*!< Selects 16- or 32- bit CRC protocol. */
} crc_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Enable the CRC peripheral module
 *
 * This functions enables the clock gate in the Kinetis SIM module for the CRC peripheral.
 *
 * @param base CRC peripheral address.
 */
static inline void CRC_Init(CRC_Type *base)
{
    /* ungate clock */
    CLOCK_EnableClock(kCLOCK_Crc0);
}

/*!
 * @brief Disable the CRC peripheral module
 *
 * This functions disables the clock gate in the Kinetis SIM module for the CRC peripheral.
 *
 * @param base CRC peripheral address.
 */
static inline void CRC_Deinit(CRC_Type *base)
{
    /* gate clock */
    CLOCK_DisableClock(kCLOCK_Crc0);
}

/*!
 * @brief Returns transpose type for CRC protocol reflect in parameter.
 *
 * This functions helps to set writeTranspose member of crc_config_t structure. Reflect in is CRC protocol parameter.
 *
 * @param base CRC peripheral address.
 * @param enable True or false for the selected CRC protocol Reflect In (refin) parameter.
 */
static inline crc_transpose_type_t CRC_GetTransposeTypeFromReflectIn(CRC_Type *base, bool enable)
{
    return ((enable) ? kCrcTransposeBitsAndBytes : kCrcTransposeBytes);
}

/*!
 * @brief Returns transpose type for CRC protocol reflect out parameter.
 *
 * This functions helps to set readTranspose member of crc_config_t structure. Reflect out is CRC protocol parameter.
 *
 * @param base CRC peripheral address.
 * @param enable True or false for the selected CRC protocol Reflect Out (refout) parameter.
 */
static inline crc_transpose_type_t CRC_GetTransposeTypeFromReflectOut(CRC_Type *base, bool enable)
{
    return ((enable) ? kCrcTransposeBitsAndBytes : kCrcTransposeNone);
}

/*!
 * @brief Configures the CRC module.
 *
 * Configures the CRC module according to configuration structure. @n
 * Starts the checksum computation by writing the seed value (the specified type of transpose is applied).
 *
 * @param base CRC peripheral address.
 * @param config Pointer to configuration structure.
 */
void CRC_Configure(CRC_Type *base, const crc_config_t *config);

/*!
 * @brief Loads default values to CRC user configuration structure.
 *
 * Loads default values to CRC user configuration structure:
 * @n
 * CRC-16-CCIT polynomial: x**16 + x**12 + x**5 + x**0 @n
 * Initial checksum: 0xFFFFU @n
 * Type of read transpose: kCrcTransposeNone @n
 * Type of write transpose:  kCrcTransposeBytes when writing CRC data register.@n
 *     The driver tries to use 32-bit writes, and the CRC module requires MSByte
 *     to be written to CRC_DATA[HU].@n
 *     Example: {0x00, 0x01, 0x02, 0x03} is 0x03020100 (little endian) but 0x00010203 shall be written,
 *              thus, enable write transpose of bytes.@n
 * No complement of checksum read.@n
 * 16-bit CRC.@n
 *
 * @param config Pointer to configuration structure.
 */
void CRC_GetDefaultConfig(crc_config_t *config);

/*!
 * @brief Write data to the CRC module.
 *
 * Writes input data buffer bytes to CRC data register. @n
 * The configured type of transpose is applied.
 *
 * @param base CRC peripheral address.
 * @param data Input data stream, MSByte in data[0].
 * @param dataSize Size in bytes of the input data buffer.
 */
void CRC_WriteData(CRC_Type *base, const uint8_t *data, size_t dataSize);

/*!
 * @brief Read 32-bit checksum from the CRC module.
 *
 * Reads CRC data register (intermediate or final checksum). @n
 * The configured type of transpose and complement are applied.
 *
 * @param base CRC peripheral address.
 * @return intermediate or final 32-bit checksum, after configured transpose and complement operations.
 */
static inline uint32_t CRC_Get32bitResult(CRC_Type *base)
{
    return base->DATA;
}

/*!
 * @brief Read 16-bit checksum from the CRC module.
 *
 * Reads CRC data register (intermediate or final checksum). @n
 * The configured type of transpose and complement are applied.
 *
 * @param base CRC peripheral address.
 * @return intermediate or final 16-bit checksum, after configured transpose and complement operations.
 */
uint16_t CRC_Get16bitResult(CRC_Type *base);

#if defined(__cplusplus)
}
#endif

/*!
 *@}
 */

#endif /* _FSL_CRC_H_ */
