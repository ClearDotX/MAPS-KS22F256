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
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
#if FLASH_DRIVER_IS_FLASH_RESIDENT
// See SSD_FTFx_Common.h for documentation of this function.
status_t flash_check_execute_in_ram_function_info(flash_driver_t *driver)
{
    flash_execute_in_ram_function_info_t *flashExecuteInRamFunctionInfo;

    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    flashExecuteInRamFunctionInfo = (flash_execute_in_ram_function_info_t *)driver->flashExecuteInRamFunctionInfo;

    if ((driver->flashExecuteInRamFunctionInfo) &&
        (kFlashExecuteInRamFunction_TotalNum == flashExecuteInRamFunctionInfo->activeFunctionCount))
    {
        return kStatus_FLASH_Success;
    }

    return kStatus_FLASH_ExecuteInRamFunctionNotReady;
}
#endif

// See SSD_FTFx_Common.h for documentation of this function.
status_t flash_check_range(flash_driver_t *driver,
                           uint32_t startAddress,
                           uint32_t lengthInBytes,
                           uint32_t alignmentBaseline)
{
    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    // Verify the start and length are alignmentBaseline aligned.
    if ((startAddress & (alignmentBaseline - 1)) || (lengthInBytes & (alignmentBaseline - 1)))
    {
        return kStatus_FLASH_AlignmentError;
    }

// check for valid range of the target addresses
#if !FLASH_SSD_IS_FLEXNVM_ENABLED
    if ((startAddress < driver->PFlashBlockBase) ||
        ((startAddress + lengthInBytes) > (driver->PFlashBlockBase + driver->PFlashTotalSize)))
#else
    if (!(((startAddress >= driver->PFlashBlockBase) &&
           ((startAddress + lengthInBytes) <= (driver->PFlashBlockBase + driver->PFlashTotalSize))) ||
          ((startAddress >= driver->DFlashBlockBase) &&
           ((startAddress + lengthInBytes) <= (driver->DFlashBlockBase + driver->DFlashTotalSize)))))
#endif
    {
        return kStatus_FLASH_AddressError;
    }

    return kStatus_FLASH_Success;
}

// See SSD_FTFx_Common.h for documentation of this function.
status_t flash_get_matched_operation_info(flash_driver_t *driver, uint32_t address, flash_operation_info_t *info)
{
    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

#if FLASH_SSD_IS_FLEXNVM_ENABLED
    if ((address >= driver->DFlashBlockBase) && (address <= (driver->DFlashBlockBase + driver->DFlashTotalSize)))
    {
        info->convertedAddress = address - driver->DFlashBlockBase + 0x800000U;
        info->activeSectorSize = FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SECTOR_SIZE;
        info->activeBlockSize = driver->DFlashTotalSize / FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_COUNT;
    }
    else
#endif
    {
        info->convertedAddress = address;
        info->activeSectorSize = driver->PFlashSectorSize;
        info->activeBlockSize = driver->PFlashTotalSize / driver->PFlashBlockCount;
    }

    return kStatus_FLASH_Success;
}

// See SSD_FTFx_Common.h for documentation of this function.
status_t flash_check_user_key(uint32_t key)
{
    // Validate the user key
    if (key != kFlashEraseKey)
    {
        return kStatus_FLASH_EraseKeyError;
    }

    return kStatus_FLASH_Success;
}

#if FLASH_SSD_IS_FLEXNVM_ENABLED
// See SSD_FTFx_Common.h for documentation of this function.
status_t flash_update_flexnvm_memory_partition_status(flash_driver_t *driver)
{
    struct
    {
        uint32_t reserved0;
        uint8_t FlexNVMPartitionCode;
        uint8_t EEPROMDataSetSize;
        uint16_t reserved1;
    } dataIFRReadOut;
    status_t returnCode;

    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    // Get FlexNVM memory partition info from data flash IFR
    returnCode = FLASH_ReadResource(driver, DFLASH_IFR_READRESOURCE_START_ADDRESS, (uint32_t *)&dataIFRReadOut,
                                    sizeof(dataIFRReadOut), kFlashResource_FlashIFR);
    if (returnCode != kStatus_FLASH_Success)
    {
        return kStatus_FLASH_PartitionStatusUpdateFailure;
    }

    // Fill out partitioned EEPROM size
    dataIFRReadOut.EEPROMDataSetSize &= 0x0FU;
    switch (dataIFRReadOut.EEPROMDataSetSize)
    {
        case 0x00U:
            driver->EEpromTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0000;
            break;
        case 0x01U:
            driver->EEpromTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0001;
            break;
        case 0x02U:
            driver->EEpromTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0010;
            break;
        case 0x03U:
            driver->EEpromTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0011;
            break;
        case 0x04U:
            driver->EEpromTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0100;
            break;
        case 0x05U:
            driver->EEpromTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0101;
            break;
        case 0x06U:
            driver->EEpromTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0110;
            break;
        case 0x07U:
            driver->EEpromTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0111;
            break;
        case 0x08U:
            driver->EEpromTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1000;
            break;
        case 0x09U:
            driver->EEpromTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1001;
            break;
        case 0x0AU:
            driver->EEpromTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1010;
            break;
        case 0x0BU:
            driver->EEpromTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1011;
            break;
        case 0x0CU:
            driver->EEpromTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1100;
            break;
        case 0x0DU:
            driver->EEpromTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1101;
            break;
        case 0x0EU:
            driver->EEpromTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1110;
            break;
        case 0x0FU:
            driver->EEpromTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1111;
            break;
        default:
            driver->EEpromTotalSize = FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_RESERVED;
            break;
    }

    // Fill out partitioned DFlash size
    dataIFRReadOut.FlexNVMPartitionCode &= 0x0FU;
    switch (dataIFRReadOut.FlexNVMPartitionCode)
    {
        case 0x00U:
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0000;
            break;
        case 0x01U:
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0001;
            break;
        case 0x02U:
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0010;
            break;
        case 0x03U:
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0011;
            break;
        case 0x04U:
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0100;
            break;
        case 0x05U:
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0101;
            break;
        case 0x06U:
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0110;
            break;
        case 0x07U:
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0111;
            break;
        case 0x08U:
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1000;
            break;
        case 0x09U:
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1001;
            break;
        case 0x0AU:
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1010;
            break;
        case 0x0BU:
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1011;
            break;
        case 0x0CU:
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1100;
            break;
        case 0x0DU:
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1101;
            break;
        case 0x0EU:
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1110;
            break;
        case 0x0FU:
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1111;
            break;
        default:
            driver->DFlashTotalSize = FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED;
            break;
    }

    return kStatus_FLASH_Success;
}
#endif
