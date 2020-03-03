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
status_t FLASH_EraseAll(flash_driver_t *driver, uint32_t key)
{
    status_t returnCode;

    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    // preparing passing parameter to erase all flash blocks
    kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(FTFx_ERASE_ALL_BLOCK, 0xFFFFFFU);

    // Validate the user key
    returnCode = flash_check_user_key(key);
    if (returnCode)
    {
        return returnCode;
    }

    // calling flash command sequence function to execute the command
    returnCode = flash_command_sequence(driver);

    flash_cache_clear(driver);

#if FLASH_SSD_IS_FLEXNVM_ENABLED
    // Data flash IFR will be erased by erase all command, so we need to
    //  update FlexNVM memory partition status synchronously
    if (returnCode == kStatus_FLASH_Success)
    {
        returnCode = flash_update_flexnvm_memory_partition_status(driver);
    }
#endif

    return returnCode;
}