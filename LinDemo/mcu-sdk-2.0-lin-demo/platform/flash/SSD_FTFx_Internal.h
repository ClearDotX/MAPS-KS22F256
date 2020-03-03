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

#ifndef _SSD_FTFx_INTERNAL_H_
#define _SSD_FTFx_INTERNAL_H_

#include "SSD_FTFx_Types.h"
#include "SSD_FTFx_Common.h"
#include "SSD_FTFx_Config.h"

/*!
 * @addtogroup SSD_FTFx_Internal
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Externs
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

#if FLASH_DRIVER_IS_FLASH_RESIDENT
/*! @brief Copy flash_run_command() to RAM*/
void copy_flash_run_command(uint8_t *flashRunCommand);
/*! @brief Copy flash_cache_clear_command() to RAM*/
void copy_flash_cache_clear_command(uint8_t *flashCacheClearCommand);
/*! @brief Check whether flash execute-in-ram functions are ready*/
status_t flash_check_execute_in_ram_function_info(flash_driver_t *driver);
#endif // FLASH_DRIVER_IS_FLASH_RESIDENT

/*! @brief Internal function Flash command sequence. Called by driver APIs only*/
status_t flash_command_sequence(flash_driver_t *driver);

/*! @brief Internal function Flash cache clear. Called by driver APIs only*/
void flash_cache_clear(flash_driver_t *driver);

/*! @brief Validates the range and alignment of the given address range.*/
status_t flash_check_range(flash_driver_t *driver,
                           uint32_t startAddress,
                           uint32_t lengthInBytes,
                           uint32_t alignmentBaseline);

/*! @brief Gets the right address, sector and block size of current flash type which is indicated by address.*/
status_t flash_get_matched_operation_info(flash_driver_t *driver, uint32_t address, flash_operation_info_t *info);

/*! @brief Validates the given user key for flash erase APIs.*/
status_t flash_check_user_key(uint32_t key);

#if FLASH_SSD_IS_FLEXNVM_ENABLED
/*! @brief Updates FlexNVM memory partition status according to data flash 0 IFR.*/
status_t flash_update_flexnvm_memory_partition_status(flash_driver_t *driver);
#endif

/*! @}*/

#endif /* _SSD_FTFx_INTERNAL_H_ */
