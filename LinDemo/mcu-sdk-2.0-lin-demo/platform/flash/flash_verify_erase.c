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
// Code
////////////////////////////////////////////////////////////////////////////////

// See fsl_flash.h for documentation of this function.
status_t FLASH_VerifyErase(flash_driver_t *driver, uint32_t start, uint32_t lengthInBytes, flash_margin_value_t margin)
{
    // Check arguments.
    uint32_t blockSize;
    flash_operation_info_t flashInfo;
    uint32_t nextBlockStartAddress;
    uint32_t remainingBytes;

    status_t returnCode =
        flash_check_range(driver, start, lengthInBytes, COMMON_FEATURE_FLASH_SECTION_CMD_ADDRESS_ALIGMENT);
    if (returnCode)
    {
        return returnCode;
    }

    flash_get_matched_operation_info(driver, start, &flashInfo);
    start = flashInfo.convertedAddress;
    blockSize = flashInfo.activeBlockSize;

    nextBlockStartAddress = ALIGN_UP(start, blockSize);
    if (nextBlockStartAddress == start)
    {
        nextBlockStartAddress += blockSize;
    }

    remainingBytes = lengthInBytes;

    while (remainingBytes)
    {
        uint32_t numberOfPhrases;
        uint32_t verifyLength = nextBlockStartAddress - start;
        if (verifyLength > remainingBytes)
        {
            verifyLength = remainingBytes;
        }

        numberOfPhrases = verifyLength / COMMON_FEATURE_FLASH_SECTION_CMD_ADDRESS_ALIGMENT;

        // Fill in verify section command parameters.
        kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(FTFx_VERIFY_SECTION, start);
        kFCCOBx[1] = BYTES_JOIN_TO_WORD_2_1_1(numberOfPhrases, margin, 0xFFU);

        // calling flash command sequence function to execute the command
        returnCode = flash_command_sequence(driver);
        if (returnCode)
        {
            return returnCode;
        }

        remainingBytes -= verifyLength;
        start += verifyLength;
        nextBlockStartAddress += blockSize;
    }

    return kStatus_FLASH_Success;
}
