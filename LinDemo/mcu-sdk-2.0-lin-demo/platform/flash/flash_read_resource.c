/*
 * Copyright (c) 2013-2015, Freescale Semiconductor, Inc.
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

#include "fsl_flash.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

enum __flash_read_resource_alignment
{
    kFlashReadResource_AlignSizeInBytes = COMMON_FEATURE_FLASH_RESOURCE_CMD_ADDRESS_ALIGMENT,
};

enum _flash_read_resource_unit_size
{
    kFlashReadResource_UnitInBytes = COMMON_FEATURE_FLASH_RESOURCE_CMD_ADDRESS_ALIGMENT,
};

//! @brief Enumeration for the maximum size of special-purpose flash resource
enum __flash_read_resource_max_size
{
// kFlashResource_PFlashIfrStart       = 0x0000,
#if (FSL_FEATURE_FLASH_IS_FTFE == 1)
    kFlashResource_PFlashIFRSizeInBytes = 1024,
    kFlashResource_VersionIdSizeInBytes = 8,
    kFlashResource_VersionIdStart = 0x08,
    kFlashResource_VersionIdSizeEnd = 0x0F,
#else /* FSL_FEATURE_FLASH_IS_FTFL == 1 or FSL_FEATURE_FLASH_IS_FTFA = =1 */
    kFlashResource_PFlashIFRSizeInBytes = 256,
    kFlashResource_VersionIdSizeInBytes = 8,
    kFlashResource_VersionIdStart = 0x00,
    kFlashResource_VersionIdSizeEnd = 0x07,
#endif

#if FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP
    kFlashResource_PFlashSwapIfrStart = 0x400000,
    kFlashResource_PFlashSwapIfrEnd = 0x4003FF,
    kFlashResource_DFlashIfrStart = 0x800000,
    kFlashResource_DFlashIfrEnd = 0x8003FF,
#endif
};

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
status_t flash_check_resource_range(uint32_t start, uint32_t lengthInBytes, flash_read_resource_option_t option);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See fsl_flash.h for documentation of this function.
status_t FLASH_ReadResource(
    flash_driver_t *driver, uint32_t start, uint32_t *dst, uint32_t lengthInBytes, flash_read_resource_option_t option)
{
    status_t returnCode;

    if ((driver == NULL) || (dst == NULL))
    {
        return kStatus_FLASH_InvalidArgument;
    }

    // Check the supplied address range.
    returnCode = flash_check_resource_range(start, lengthInBytes, option);
    if (returnCode != kStatus_FLASH_Success)
    {
        return returnCode;
    }

    while (lengthInBytes > 0)
    {
        // preparing passing parameter
        kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(FTFx_READ_RESOURCE, start);
#if (COMMON_FEATURE_FLASH_RESOURCE_CMD_ADDRESS_ALIGMENT == 8)
        kFCCOBx[1] = BYTES_JOIN_TO_WORD_1_3(option, 0xFFFFFFU);
#else
        kFCCOBx[2] = BYTES_JOIN_TO_WORD_1_3(option, 0xFFFFFFU);
#endif
        // calling flash command sequence function to execute the command
        returnCode = flash_command_sequence(driver);

        if (kStatus_FLASH_Success != returnCode)
        {
            break;
        }

        // fetch data
        *dst++ = kFCCOBx[1];
#if (COMMON_FEATURE_FLASH_RESOURCE_CMD_ADDRESS_ALIGMENT > 4)
        *dst++ = kFCCOBx[2];
#endif
        // update start address for next iteration
        start += kFlashReadResource_UnitInBytes;
        // update lengthInBytes for next iteration
        lengthInBytes -= kFlashReadResource_UnitInBytes;
    }

    return (returnCode);
}

status_t flash_check_resource_range(uint32_t start, uint32_t lengthInBytes, flash_read_resource_option_t option)
{
    status_t status;
    uint32_t maxReadbleAddress;

    if ((start & (kFlashReadResource_AlignSizeInBytes - 1)) ||
        (lengthInBytes & (kFlashReadResource_AlignSizeInBytes - 1)))
    {
        return kStatus_FLASH_AlignmentError;
    }

    status = kStatus_FLASH_Success;

    maxReadbleAddress = start + lengthInBytes - 1;
    if (option == kFlashResource_VersionID)
    {
        if ((start != kFlashResource_VersionIdStart) ||
            ((start + lengthInBytes - 1) != kFlashResource_VersionIdSizeEnd))
        {
            status = kStatus_FLASH_InvalidArgument;
        }
    }
    else if (option == kFlashResource_FlashIFR)
    {
        if (maxReadbleAddress < kFlashResource_PFlashIFRSizeInBytes)
        {
        }
#if FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP
        else if ((start >= kFlashResource_PFlashSwapIfrStart) && (maxReadbleAddress <= kFlashResource_PFlashSwapIfrEnd))
        {
        }
        else if ((start >= kFlashResource_DFlashIfrStart) && (maxReadbleAddress <= kFlashResource_DFlashIfrEnd))
        {
        }
#endif // FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP
        else
        {
            status = kStatus_FLASH_InvalidArgument;
        }
    }
    else
    {
        status = kStatus_FLASH_InvalidArgument;
    }

    return status;
}
