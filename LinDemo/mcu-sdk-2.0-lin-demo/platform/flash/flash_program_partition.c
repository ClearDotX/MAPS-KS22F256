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
status_t FLASH_ProgramPartition(flash_driver_t *driver,
                                flash_partition_flexram_load_option_t option,
                                uint32_t eepromDataSizeCode,
                                uint32_t flexnvmPartitionCode)
{
#if (!FSL_FEATURE_FLASH_HAS_PROGRAM_PARTITION_CMD)
    {
        return kStatus_FLASH_CommandNotSupported;
    }
#else
    status_t returnCode;

    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    // eepromDataSizeCode[7:6], flexnvmPartitionCode[7:4] should be all 1'b0
    //  or it will cause access error.
    // eepromDataSizeCode &= 0x3FU;
    // flexnvmPartitionCode &= 0x0FU;

    // preparing passing parameter to program the flash block
    kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_2_1(FTFx_PROGRAM_PARTITION, 0xFFFFU, option);
    kFCCOBx[1] = BYTES_JOIN_TO_WORD_1_1_2(eepromDataSizeCode, flexnvmPartitionCode, 0xFFFFU);

    // calling flash command sequence function to execute the command
    returnCode = flash_command_sequence(driver);

    flash_cache_clear(driver);

#if FLASH_SSD_IS_FLEXNVM_ENABLED
    // Data flash IFR will be updated by program partition command during reset sequence,
    // so we just set reserved values for partitioned FlexNVM size here
    driver->EEpromTotalSize = FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_RESERVED;
    driver->DFlashTotalSize = FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED;
#endif

    return (returnCode);
#endif // FSL_FEATURE_FLASH_HAS_PROGRAM_PARTITION_CMD
}
