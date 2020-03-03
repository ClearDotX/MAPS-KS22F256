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
enum
{
    kFlashConfig_Start = 0x400,
    kFlashConfig_End = 0x40F
};

////////////////////////////////////////////////////////////////////////////////
// Declarations
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
#if (FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP)
status_t flash_check_swap_control_option(flash_swap_control_option_t option)
{
    if ((option == kFlashSwap_IntializeSystem) || (option == kFlashSwap_SetInUpdateState) ||
        (option == kFlashSwap_SetInCompleteState) || (option == kFlashSwap_ReportStatus) ||
        (option == kFlashSwap_DisableSystem))
    {
        return kStatus_FLASH_Success;
    }

    return kStatus_FLASH_InvalidArgument;
}
#endif // (FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP)

// See fsl_flash.h for documentation on this function.
status_t FLASH_SwapControl(flash_driver_t *driver,
                           uint32_t address,
                           flash_swap_control_option_t option,
                           flash_swap_info_t *returnInfo)
{
#if (!FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP)
    return kStatus_FLASH_CommandNotSupported;
#else
    status_t returnCode;

    if ((driver == NULL) || (returnInfo == NULL))
    {
        return kStatus_FLASH_InvalidArgument;
    }

    if (address & (FSL_FEATURE_FLASH_PFLASH_SWAP_CONTROL_CMD_ADDRESS_ALIGMENT - 1))
    {
        return kStatus_FLASH_AlignmentError;
    }

    // Make sure address provided is in the lower half of Program flash but not in the Flash Configuration Field
    if ((address >= (driver->PFlashTotalSize / 2)) ||
        ((address >= kFlashConfig_Start) && (address <= kFlashConfig_End)))
    {
        return kStatus_FLASH_InvalidArgument;
    }

    // Check the option.
    returnCode = flash_check_swap_control_option(option);
    if (returnCode)
    {
        return returnCode;
    }

    kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(FTFx_SWAP_CONTROL, address);
    kFCCOBx[1] = BYTES_JOIN_TO_WORD_1_3(option, 0xFFFFFFU);

    returnCode = flash_command_sequence(driver);

    returnInfo->flashSwapState = (flash_swap_state_t)FTFx->FCCOB5;
    returnInfo->currentSwapBlockStatus = (flash_swap_block_status_t)FTFx->FCCOB6;
    returnInfo->nextSwapBlockStatus = (flash_swap_block_status_t)FTFx->FCCOB7;

    return returnCode;
#endif // FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP
}

// See fsl_flash.h for documentation on this function.
status_t FLASH_Swap(flash_driver_t *driver, uint32_t address, flash_swap_function_option_t option)
{
    flash_swap_info_t returnInfo;
    status_t returnCode;

    memset(&returnInfo, 0xFFU, sizeof(returnInfo));

    do
    {
        returnCode = FLASH_SwapControl(driver, address, kFlashSwap_ReportStatus, &returnInfo);

        if (returnCode == kStatus_FLASH_Success)
        {
            switch (returnInfo.flashSwapState)
            {
                case kFlashSwapState_Uninitialized:
                    if (kFlashSwapFunction_Disable == option)
                    {
                        returnCode = FLASH_SwapControl(driver, address, kFlashSwap_DisableSystem, &returnInfo);
                    }
                    else
                    {
                        // If current swap mode is Uninitialized, Initialize Swap to Initialized/READY state.
                        returnCode = FLASH_SwapControl(driver, address, kFlashSwap_IntializeSystem, &returnInfo);
                    }
                    break;
                case kFlashSwapState_Ready:
                    // If current swap mode is Initialized/Ready, Initialize Swap to UPDATE state.
                    returnCode = FLASH_SwapControl(driver, address, kFlashSwap_SetInUpdateState, &returnInfo);
                    break;
                case kFlashSwapState_Update:
                    // If current swap mode is Update, Erase indicator sector in non active block
                    // to proceed swap system to update-erased state
                    returnCode = FLASH_Erase(driver, address + (driver->PFlashTotalSize >> 1), driver->PFlashSectorSize,
                                             kFlashEraseKey);
                    break;
                case kFlashSwapState_UpdateErased:
                    // If current swap mode is Update or Update-Erased, progress Swap to COMPLETE State
                    returnCode = FLASH_SwapControl(driver, address, kFlashSwap_SetInCompleteState, &returnInfo);
                    break;
                case kFlashSwapState_Complete:
                    break;
                case kFlashSwapState_Disabled:
                    if (kFlashSwapFunction_Disable == option)
                    {
                        return kStatus_FLASH_Success;
                    }
                    else
                    {
                        returnCode = FLASH_SwapControl(driver, address, kFlashSwap_IntializeSystem, &returnInfo);
                    }
                    break;
                default:
                    returnCode = kStatus_FLASH_InvalidArgument;
                    break;
            }
        }
        if (returnCode != kStatus_FLASH_Success)
        {
            break;
        }
    } while (!((kFlashSwapState_Complete == returnInfo.flashSwapState) && (kFlashSwapFunction_Enable == option)));

    return returnCode;
}
