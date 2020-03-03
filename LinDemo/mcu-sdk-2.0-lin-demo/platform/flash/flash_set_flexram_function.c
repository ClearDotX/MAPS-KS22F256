/*
 * Copyright (c) 2014-2015, Freescale Semiconductor, Inc.
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
// Definitionss
////////////////////////////////////////////////////////////////////////////////
#if FSL_FEATURE_FLASH_HAS_FLEX_NVM
#if defined(FTFE)
#define FTFx_FCNFG_EEERDY_MASK FTFE_FCNFG_EEERDY_MASK
#elif defined(FTFL)
#define FTFx_FCNFG_EEERDY_MASK FTFL_FCNFG_EEERDY_MASK
#elif defined(FTFA)
#define FTFx_FCNFG_EEERDY_MASK FTFA_FCNFG_EEERDY_MASK
#endif //
#endif // FSL_FEATURE_FLASH_HAS_FLEX_NVM

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
#if (FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD)
static inline status_t flasn_check_flexram_function_option_range(flash_flexram_function_option_t option)
{
    if ((option != kFlash_FlexRAM_Available_As_RAM) && (option != kFlash_FlexRAM_Available_For_EEPROM))
    {
        return kStatus_FLASH_InvalidArgument;
    }

    return kStatus_FLASH_Success;
}
#endif // #if (FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD)

// See fsl_flash.h for documentation of this function.
status_t FLASH_SetFlexramFunction(flash_driver_t *driver, flash_flexram_function_option_t option)
{
#if (!FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD)
    {
        return kStatus_FLASH_CommandNotSupported;
    }

#else
    status_t status;

    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    status = flasn_check_flexram_function_option_range(option);
    if (status != kStatus_FLASH_Success)
    {
        return status;
    }

    // preparing passing parameter to verify all block command
    kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_1_2(FTFx_SET_FLEXRAM_FUNCTION, option, 0xFFFFU);

    // calling flash command sequence function to execute the command
    return flash_command_sequence(driver);
#endif // FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD
}

#if FLASH_SSD_IS_FLEXNVM_ENABLED
status_t FLASH_EepromWrite(flash_driver_t *driver, uint32_t start, uint8_t *src, uint32_t lengthInBytes)
{
    status_t returnCode;
    bool needSwitchFlexRamMode = false;

    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    // Validates the range of the given address
    if ((start < driver->FlexRAMBlockBase) ||
        ((start + lengthInBytes) > (driver->FlexRAMBlockBase + driver->EEpromTotalSize)))
    {
        return kStatus_FLASH_AddressError;
    }

    returnCode = kStatus_FLASH_Success;

    // Switch function of FlexRAM if needed
    if (!(FTFx->FCNFG & FTFx_FCNFG_EEERDY_MASK))
    {
        needSwitchFlexRamMode = true;

        returnCode = FLASH_SetFlexramFunction(driver, kFlash_FlexRAM_Available_For_EEPROM);
        if (returnCode != kStatus_FLASH_Success)
        {
            return kStatus_FLASH_SetFlexramAsEepromError;
        }
    }

    // Write data to FlexRAM when it is used as EEPROM emulator
    while (lengthInBytes > 0)
    {
        if ((!(start & 0x3U)) && (lengthInBytes >= 4))
        {
            *(uint32_t *)start = *(uint32_t *)src;
            start += 4;
            src += 4;
            lengthInBytes -= 4;
        }
        else if ((!(start & 0x1U)) && (lengthInBytes >= 2))
        {
            *(uint16_t *)start = *(uint16_t *)src;
            start += 2;
            src += 2;
            lengthInBytes -= 2;
        }
        else
        {
            *(uint8_t *)start = *src;
            start += 1;
            src += 1;
            lengthInBytes -= 1;
        }
        // Wait till EEERDY bit is set
        while (!(FTFx->FCNFG & FTFx_FCNFG_EEERDY_MASK))
            ;

        // Check for protection violation error
        if (FTFx->FSTAT & FTFx_FSTAT_FPVIOL_MASK)
        {
            return kStatus_FLASH_ProtectionViolation;
        }
    }

    // Switch function of FlexRAM if needed
    if (needSwitchFlexRamMode)
    {
        returnCode = FLASH_SetFlexramFunction(driver, kFlash_FlexRAM_Available_As_RAM);
        if (returnCode != kStatus_FLASH_Success)
        {
            return kStatus_FLASH_RecoverFlexramAsRamError;
        }
    }

    return returnCode;
}
#endif // FLASH_SSD_IS_FLEXNVM_ENABLED
