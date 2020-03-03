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
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

volatile uint32_t *const restrict kFPROT =
#if FSL_FEATURE_FLASH_IS_FTFA
    (volatile uint32_t *)&FTFA->FPROT3;
#elif FSL_FEATURE_FLASH_IS_FTFE
    (volatile uint32_t *)&FTFE->FPROT3;
#elif FSL_FEATURE_FLASH_IS_FTFL
    (volatile uint32_t *)&FTFL->FPROT3;
#else
#error "Unknown flash controller"
#endif

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See fsl_flash.h for documentation of this function.
status_t FLASH_PflashSetProtection(flash_driver_t *driver, uint32_t protectStatus)
{
    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    *kFPROT = protectStatus;

    if (protectStatus != *kFPROT)
    {
        return kStatus_FLASH_CommandFailure;
    }

    return kStatus_FLASH_Success;
}

// See fsl_flash.h for documentation of this function.
status_t FLASH_PflashGetProtection(flash_driver_t *driver, uint32_t *protectStatus)
{
    if ((driver == NULL) || (protectStatus == NULL))
    {
        return kStatus_FLASH_InvalidArgument;
    }

    *protectStatus = *kFPROT;

    return kStatus_FLASH_Success;
}

#if FSL_FEATURE_FLASH_HAS_FLEX_NVM
// See fsl_flash.h for documentation of this function.
status_t FLASH_DflashSetProtection(flash_driver_t *driver, uint8_t protectStatus)
{
    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    if ((driver->DFlashTotalSize == 0) || (driver->DFlashTotalSize == FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED))
    {
        return kStatus_FLASH_CommandNotSupported;
    }

    FTFx->FDPROT = protectStatus;

    if (FTFx->FDPROT != protectStatus)
    {
        return kStatus_FLASH_CommandFailure;
    }

    return kStatus_FLASH_Success;
}

// See fsl_flash.h for documentation of this function.
status_t FLASH_DflashGetProtection(flash_driver_t *driver, uint8_t *protectStatus)
{
    if ((driver == NULL) || (protectStatus == NULL))
    {
        return kStatus_FLASH_InvalidArgument;
    }

    if ((driver->DFlashTotalSize == 0) || (driver->DFlashTotalSize == FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED))
    {
        return kStatus_FLASH_CommandNotSupported;
    }

    *protectStatus = FTFx->FDPROT;

    return kStatus_FLASH_Success;
}

// See fsl_flash.h for documentation of this function.
status_t FLASH_EepromSetProtection(flash_driver_t *driver, uint8_t protectStatus)
{
    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    if ((driver->EEpromTotalSize == 0) || (driver->EEpromTotalSize == FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_RESERVED))
    {
        return kStatus_FLASH_CommandNotSupported;
    }

    FTFx->FEPROT = protectStatus;

    if (FTFx->FEPROT != protectStatus)
    {
        return kStatus_FLASH_CommandFailure;
    }

    return kStatus_FLASH_Success;
}

// See fsl_flash.h for documentation of this function.
status_t FLASH_EepromGetProtection(flash_driver_t *driver, uint8_t *protectStatus)
{
    if ((driver == NULL) || (protectStatus == NULL))
    {
        return kStatus_FLASH_InvalidArgument;
    }

    if ((driver->EEpromTotalSize == 0) || (driver->EEpromTotalSize == FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_RESERVED))
    {
        return kStatus_FLASH_CommandNotSupported;
    }

    *protectStatus = FTFx->FEPROT;

    return kStatus_FLASH_Success;
}
#endif
