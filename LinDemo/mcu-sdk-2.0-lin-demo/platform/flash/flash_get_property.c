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
status_t FLASH_GetProperty(flash_driver_t *driver, flash_property_t whichProperty, uint32_t *value)
{
    if ((driver == NULL) || (value == NULL))
    {
        return kStatus_FLASH_InvalidArgument;
    }

    switch (whichProperty)
    {
        case kFlashProperty_PFlashSectorSize:
            *value = driver->PFlashSectorSize;
            break;

        case kFlashProperty_PFlashTotalSize:
            *value = driver->PFlashTotalSize;
            break;

        case kFlashProperty_PFlashBlockSize:
            *value = driver->PFlashTotalSize / FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT;
            break;

        case kFlashProperty_PFlashBlockCount:
            *value = driver->PFlashBlockCount;
            break;

        case kFlashProperty_PFlashBlockBaseAddr:
            *value = driver->PFlashBlockBase;
            break;

        case kFlashProperty_PFlashFacSupport:
#if defined(FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL)
            *value = FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL;
#else
            *value = 0;
#endif // FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
            break;

        case kFlashProperty_PFlashAccessSegmentSize:
            *value = driver->PFlashAccessSegmentSize;
            break;

        case kFlashProperty_PFlashAccessSegmentCount:
            *value = driver->PFlashAccessSegmentCount;
            break;

#if FLASH_SSD_IS_FLEXNVM_ENABLED
        case kFlashProperty_DFlashSectorSize:
            *value = FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SECTOR_SIZE;
            break;
        case kFlashProperty_DFlashTotalSize:
            *value = driver->DFlashTotalSize;
            break;
        case kFlashProperty_DFlashBlockSize:
            *value = FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SIZE;
            break;
        case kFlashProperty_DFlashBlockCount:
            *value = FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_COUNT;
            break;
        case kFlashProperty_DFlashBlockBaseAddr:
            *value = driver->DFlashBlockBase;
            break;
        case kFlashProperty_EEpromTotalSize:
            *value = driver->EEpromTotalSize;
            break;
#endif // FLASH_SSD_IS_FLEXNVM_ENABLED

        default: // catch inputs that are not recognized
            return kStatus_FLASH_UnknownProperty;
    }

    return kStatus_FLASH_Success;
}
