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
    kFlashAccessSegmentBase = 256UL,
};

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

volatile uint32_t *const restrict kFCCOBx =
#if FSL_FEATURE_FLASH_IS_FTFA
    (volatile uint32_t *)&FTFA->FCCOB3;
#elif FSL_FEATURE_FLASH_IS_FTFE
    (volatile uint32_t *)&FTFE->FCCOB3;
#elif FSL_FEATURE_FLASH_IS_FTFL
    (volatile uint32_t *)&FTFL->FCCOB3;
#else
#error "Unknown flash controller"
#endif

#if (FLASH_DRIVER_IS_FLASH_RESIDENT && !FLASH_DRIVER_IS_EXPORTED)
//! @brief A static buffer used to hold flash_run_command()
static uint8_t s_flashRunCommand[kFlashExecuteInRamFunction_MaxSize];
//! @brief A static buffer used to hold flash_cache_clear_command()
static uint8_t s_flashCacheClearCommand[kFlashExecuteInRamFunction_MaxSize];
/* clang-format off */
static flash_execute_in_ram_function_info_t s_flashExecuteInRamFunctionInfo = {
    .activeFunctionCount = 0,
    .flashRunCommand = s_flashRunCommand,
    .flashCacheClearCommand = s_flashCacheClearCommand,
};
/* clang-format on */
#endif

/*!
 * @brief Table of pflash sizes.
 *
 *  The index into this table is the value of the SIM_FCFG1.PFSIZE bitfield.
 *
 *  The values in this table have been right shifted 10 bits so that they will all fit within
 *  an 16-bit integer. To get the actual flash density, you must left shift the looked up value
 *  by 10 bits.
 *
 *  Elements of this table have a value of 0 in cases where the PFSIZE bitfield value is
 *  reserved.
 *
 *  Code to use the table:
 *  @code
 *      uint8_t pfsize = (SIM->FCFG1 & SIM_FCFG1_PFSIZE_MASK) >> SIM_FCFG1_PFSIZE_SHIFT;
 *      flashDensity = ((uint32_t)kPFlashDensities[pfsize]) << 10;
 *  @endcode
 */
const uint16_t kPFlashDensities[] = {
    8,    // 0x0 - 8192, 8KB
    16,   // 0x1 - 16384, 16KB
    24,   // 0x2 - 24576, 24KB
    32,   // 0x3 - 32768, 32KB
    48,   // 0x4 - 49152, 48KB
    64,   // 0x5 - 65536, 64KB
    96,   // 0x6 - 98304, 96KB
    128,  // 0x7 - 131072, 128KB
    192,  // 0x8 - 196608, 192KB
    256,  // 0x9 - 262144, 256KB
    384,  // 0xa - 393216, 384KB
    512,  // 0xb - 524288, 512KB
    768,  // 0xc - 786432, 768KB
    1024, // 0xd - 1048576, 1MB
    1536, // 0xe - 1572864, 1.5MB
    // 2048, // 0xf - 2097152, 2MB
};

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See fsl_flash.h for documentation of this function.
status_t FLASH_Init(flash_driver_t *driver)
{
    uint32_t flashDensity;

    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    // calculate the flash density from SIM_FCFG1.PFSIZE
    uint8_t pfsize = (SIM->FCFG1 & SIM_FCFG1_PFSIZE_MASK) >> SIM_FCFG1_PFSIZE_SHIFT;
    // PFSIZE=0xf means that on customer parts the IFR was not correctly programmed.
    // We just use the pre-defined flash size in feature file here to support pre-production parts
    if (pfsize == 0xf)
    {
        flashDensity = FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT * FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE;
    }
    else
    {
        flashDensity = ((uint32_t)kPFlashDensities[pfsize]) << 10;
    }

    // fill out a few of the structure members
    driver->PFlashBlockBase = PFLASH_BLOCK_BASE;
    driver->PFlashTotalSize = flashDensity;
    driver->PFlashBlockCount = FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT;
    driver->PFlashSectorSize = FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE;

#if FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
    driver->PFlashAccessSegmentSize = kFlashAccessSegmentBase << FTFx->FACSS;
    driver->PFlashAccessSegmentCount = FTFx->FACSN;
#else
    driver->PFlashAccessSegmentSize = 0;
    driver->PFlashAccessSegmentCount = 0;
#endif // FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL

    driver->PFlashCallback = NULL;

// copy required flash commands to RAM
#if (FLASH_DRIVER_IS_FLASH_RESIDENT && !FLASH_DRIVER_IS_EXPORTED)
    if (kStatus_FLASH_Success != flash_check_execute_in_ram_function_info(driver))
    {
        driver->flashExecuteInRamFunctionInfo = &s_flashExecuteInRamFunctionInfo.activeFunctionCount;
        FLASH_PrepareExecuteInRamFunctions(driver);
    }
#endif

    driver->FlexRAMBlockBase = FSL_FEATURE_FLASH_FLEX_RAM_START_ADDRESS;
    driver->FlexRAMTotalSize = FSL_FEATURE_FLASH_FLEX_RAM_SIZE;

#if FLASH_SSD_IS_FLEXNVM_ENABLED
    {
        status_t returnCode;
        driver->DFlashBlockBase = FSL_FEATURE_FLASH_FLEX_NVM_START_ADDRESS;
        returnCode = flash_update_flexnvm_memory_partition_status(driver);
        if (returnCode != kStatus_FLASH_Success)
        {
            return returnCode;
        }
    }
#endif

    return kStatus_FLASH_Success;
}

// See fsl_flash.h for documentation of this function.
status_t FLASH_SetCallback(flash_driver_t *driver, flash_callback_t callback)
{
    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    driver->PFlashCallback = callback;

    return kStatus_FLASH_Success;
}

#if FLASH_DRIVER_IS_FLASH_RESIDENT
// See fsl_flash.h for documentation of this function.
status_t FLASH_PrepareExecuteInRamFunctions(flash_driver_t *driver)
{
    flash_execute_in_ram_function_info_t *flashExecuteInRamFunctionInfo;

    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    flashExecuteInRamFunctionInfo = (flash_execute_in_ram_function_info_t *)driver->flashExecuteInRamFunctionInfo;

    copy_flash_run_command(flashExecuteInRamFunctionInfo->flashRunCommand);
    copy_flash_cache_clear_command(flashExecuteInRamFunctionInfo->flashCacheClearCommand);
    flashExecuteInRamFunctionInfo->activeFunctionCount = kFlashExecuteInRamFunction_TotalNum;

    return kStatus_FLASH_Success;
}
#endif
