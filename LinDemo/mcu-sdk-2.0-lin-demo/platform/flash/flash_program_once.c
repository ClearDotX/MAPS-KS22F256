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
status_t FLASH_ProgramOnce(flash_driver_t *driver, uint32_t index, uint32_t *src, uint32_t lengthInBytes)
{
    status_t returnCode;

    if ((driver == NULL) || (src == NULL))
    {
        return kStatus_FLASH_InvalidArgument;
    }

    // pass paramters to FTFx
    kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_1_2(FTFx_PROGRAM_ONCE, index, 0xFFFFU);

    kFCCOBx[1] = *src;

// Note: Have to seperate the first index from the rest if it equals 0
// to avoid a pointless comparison of unsigned int to 0 compiler warning
#if FLASH_PROGRAM_ONCE_IS_8BYTES_UNIT_SUPPORT
#if FLASH_PROGRAM_ONCE_IS_4BYTES_UNIT_SUPPORT
    if (((index == FLASH_PROGRAM_ONCE_MIN_ID_8BYTES) ||
         // Range check
         ((index >= FLASH_PROGRAM_ONCE_MIN_ID_8BYTES + 1) && (index <= FLASH_PROGRAM_ONCE_MAX_ID_8BYTES))) &&
        (lengthInBytes == 8))
#endif // FLASH_PROGRAM_ONCE_IS_4BYTES_UNIT_SUPPORT
    {
        kFCCOBx[2] = *(src + 1);
    }
#endif // FLASH_PROGRAM_ONCE_IS_8BYTES_UNIT_SUPPORT

    // calling flash command sequence function to execute the command
    returnCode = flash_command_sequence(driver);

    flash_cache_clear(driver);

    return returnCode;
}
