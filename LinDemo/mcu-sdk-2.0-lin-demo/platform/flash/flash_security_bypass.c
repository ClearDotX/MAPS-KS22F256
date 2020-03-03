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

#include "fsl_flash.h"

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See fsl_flash.h for documentation of this function.
status_t FLASH_SecurityBypass(flash_driver_t *driver, const uint8_t *backdoorKey)
{
    uint8_t registerValue; // registerValue
    status_t returnCode;   // return code variable

    if ((driver == NULL) || (backdoorKey == NULL))
    {
        return kStatus_FLASH_InvalidArgument;
    }

    // set the default return code as kStatus_Success
    returnCode = kStatus_FLASH_Success;

    // Get flash security register value
    registerValue = FTFx->FSEC;

    // Check to see if flash is in secure state (any state other than 0x2)
    // If not, then skip this since flash is not secure
    if (0x02 != (registerValue & 0x03))
    {
        // preparing passing parameter to erase a flash block
        kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(FTFx_SECURITY_BY_PASS, 0xFFFFFFU);
        kFCCOBx[1] = BYTES_JOIN_TO_WORD_1_1_1_1(backdoorKey[0], backdoorKey[1], backdoorKey[2], backdoorKey[3]);
        kFCCOBx[2] = BYTES_JOIN_TO_WORD_1_1_1_1(backdoorKey[4], backdoorKey[5], backdoorKey[6], backdoorKey[7]);

        // calling flash command sequence function to execute the command
        returnCode = flash_command_sequence(driver);
    }

    return (returnCode);
}
