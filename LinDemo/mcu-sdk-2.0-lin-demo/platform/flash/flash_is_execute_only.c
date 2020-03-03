/*
 * Copyright (c) 2014, Freescale Semiconductor, Inc.
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
status_t FLASH_IsExecuteOnly(flash_driver_t *driver,
                             uint32_t start,
                             uint32_t lengthInBytes,
                             flash_execute_only_access_state_t *access_state)
{
    status_t returnCode;

    if (access_state == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    // Check the supplied address range.
    returnCode = flash_check_range(driver, start, lengthInBytes, FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE);
    if (returnCode)
    {
        return returnCode;
    }

#if !FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
    *access_state = kFlashAccess_UnLimited;
#else
    {
        uint32_t executeOnlySegmentCounter = 0;

        // calculating end address
        uint32_t endAddress = start + lengthInBytes;

        // Aligning start address and end address
        uint32_t alignedStartAddress = ALIGN_DOWN(start, driver->PFlashAccessSegmentSize);
        uint32_t alignedEndAddress = ALIGN_UP(endAddress, driver->PFlashAccessSegmentSize);

        uint32_t segmentIndex = 0;
        uint32_t maxSupportedExecuteOnlySegmentCount =
            (alignedEndAddress - alignedStartAddress) / driver->PFlashAccessSegmentSize;

        while (start < endAddress)
        {
            uint32_t xacc;

            segmentIndex = start / driver->PFlashAccessSegmentSize;

            if (segmentIndex < 32)
            {
                xacc = *(const volatile uint32_t *)&FTFx->XACCL3;
            }
            else if (segmentIndex < driver->PFlashAccessSegmentCount)
            {
                xacc = *(const volatile uint32_t *)&FTFx->XACCH3;
                segmentIndex -= 32;
            }
            else
            {
                break;
            }

            // Determine if this address range is in a execute-only protection flash segment.
            if ((~xacc) & (1u << segmentIndex))
            {
                executeOnlySegmentCounter++;
            }

            start += driver->PFlashAccessSegmentSize;
        }

        if (executeOnlySegmentCounter < 1u)
        {
            *access_state = kFlashAccess_UnLimited;
        }
        else if (executeOnlySegmentCounter < maxSupportedExecuteOnlySegmentCount)
        {
            *access_state = kFlashAccess_Mixed;
        }
        else
        {
            *access_state = kFlashAccess_ExecuteOnly;
        }
    }

#endif // FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL

    return (returnCode);
}
