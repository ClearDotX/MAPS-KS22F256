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

#ifndef _SSD_FTFx_TYPES_H_
#define _SSD_FTFx_TYPES_H_

#include <stdint.h>
#include <stdbool.h>

/*!
 * @addtogroup SSD_FTFx_Types
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Flash driver status codes.
 */

/*! @brief Construct a status code value from a group and code number. */
#if !defined(MAKE_STATUS)
#define MAKE_STATUS(group, code) ((((group)*100) + (code)))
#endif

/*! @brief Construct the version number for drivers. */
#if !defined(MAKE_VERSION)
#define MAKE_VERSION(major, minor, bugfix) (((major) << 16) | ((minor) << 8) | (bugfix))
#endif

/* clang-format off */
#if defined(kStatusGroup_FlashDriver)
#define kStatusGroupGeneric       kStatusGroup_Generic
#define kStatusGroupFlashDriver   kStatusGroup_FlashDriver
#elif defined(kStatusGroup_FLASH)
#define kStatusGroupGeneric       kStatusGroup_Generic
#define kStatusGroupFlashDriver   kStatusGroup_FLASH
#else
#define kStatusGroupGeneric       0
#define kStatusGroupFlashDriver   1
#endif
/* clang-format on */

/* clang-format off */
enum _flash_status
{
    kStatus_FLASH_Success                      = MAKE_STATUS(kStatusGroupGeneric, 0),
    kStatus_FLASH_InvalidArgument              = MAKE_STATUS(kStatusGroupGeneric, 4),

    kStatus_FLASH_SizeError                    = MAKE_STATUS(kStatusGroupFlashDriver, 0),
    kStatus_FLASH_AlignmentError               = MAKE_STATUS(kStatusGroupFlashDriver, 1),
    kStatus_FLASH_AddressError                 = MAKE_STATUS(kStatusGroupFlashDriver, 2),
    kStatus_FLASH_AccessError                  = MAKE_STATUS(kStatusGroupFlashDriver, 3),
    kStatus_FLASH_ProtectionViolation          = MAKE_STATUS(kStatusGroupFlashDriver, 4),
    kStatus_FLASH_CommandFailure               = MAKE_STATUS(kStatusGroupFlashDriver, 5),
    kStatus_FLASH_UnknownProperty              = MAKE_STATUS(kStatusGroupFlashDriver, 6),
    kStatus_FLASH_EraseKeyError                = MAKE_STATUS(kStatusGroupFlashDriver, 7),
    kStatus_FLASH_RegionExecuteOnly            = MAKE_STATUS(kStatusGroupFlashDriver, 8),
    kStatus_FLASH_ExecuteInRamFunctionNotReady = MAKE_STATUS(kStatusGroupFlashDriver, 9),
    kStatus_FLASH_PartitionStatusUpdateFailure = MAKE_STATUS(kStatusGroupFlashDriver, 10),
    kStatus_FLASH_SetFlexramAsEepromError      = MAKE_STATUS(kStatusGroupFlashDriver, 11),
    kStatus_FLASH_RecoverFlexramAsRamError     = MAKE_STATUS(kStatusGroupFlashDriver, 12),
    kStatus_FLASH_SetFlexramAsRamError         = MAKE_STATUS(kStatusGroupFlashDriver, 13),
    kStatus_FLASH_RecoverFlexramAsEepromError  = MAKE_STATUS(kStatusGroupFlashDriver, 14),
    kStatus_FLASH_CommandNotSupported          = MAKE_STATUS(kStatusGroupFlashDriver, 15),
};
/* clang-format on */

/*!
 * @brief Enumeration for supported flash margin levels.
 */
typedef enum _flash_margin_value
{
    kFlashMargin_Normal,
    kFlashMargin_User,
    kFlashMargin_Factory,
    kFlashMargin_Invalid /*!< Not real margin level, Used to determine the range of valid margin level. */
} flash_margin_value_t;

/*!
 * @brief Enumeration for the three possible flash security states.
 */
typedef enum _flash_security_state
{
    kFlashNotSecure,
    kFlashSecureBackdoorEnabled,
    kFlashSecureBackdoorDisabled
} flash_security_state_t;

/*!
 * @brief Enumeration for the three possible flash protection levels.
 */
typedef enum _flash_protection_state
{
    kFlashProtection_Unprotected,
    kFlashProtection_Protected,
    kFlashProtection_Mixed
} flash_protection_state_t;

/*!
 * @brief Enumeration for the three possible flash execute access levels.
 */
typedef enum _flash_execute_only_access_state
{
    kFlashAccess_UnLimited,
    kFlashAccess_ExecuteOnly,
    kFlashAccess_Mixed
} flash_execute_only_access_state_t;

/*!
 * @brief Enumeration for various flash properties.
 */
typedef enum _flash_property
{
    kFlashProperty_PFlashSectorSize = 0U,
    kFlashProperty_PFlashTotalSize = 1U,
    kFlashProperty_PFlashBlockSize = 2U,
    kFlashProperty_PFlashBlockCount = 3U,
    kFlashProperty_PFlashBlockBaseAddr = 4U,
    kFlashProperty_PFlashFacSupport = 5U,
    kFlashProperty_PFlashAccessSegmentSize = 6U,
    kFlashProperty_PFlashAccessSegmentCount = 7U,

    kFlashProperty_FlexRamBlockBaseAddr = 8U,
    kFlashProperty_FlexRamTotalSize = 9U,

    kFlashProperty_DFlashSectorSize = 10U,
    kFlashProperty_DFlashTotalSize = 11U,
    kFlashProperty_DFlashBlockSize = 12U,
    kFlashProperty_DFlashBlockCount = 13U,
    kFlashProperty_DFlashBlockBaseAddr = 14U,
    kFlashProperty_EEpromTotalSize = 15U

} flash_property_t;

/*!
 * @brief Flash execute-in-ram function information.
 */
typedef struct _flash_execute_in_ram_function_info
{
    uint32_t activeFunctionCount;
    uint8_t *flashRunCommand;
    uint8_t *flashCacheClearCommand;
} flash_execute_in_ram_function_info_t;

/*!
 * @brief Enumeration for the two possible options of flash read resource command.
 */
typedef enum _flash_read_resource_option
{
    kFlashResource_FlashIFR =
        0U, /*!< Select code for Program flash 0 IFR, Program flash swap 0 IFR, Data flash 0 IFR */
    kFlashResource_VersionID = 1U /*!< Select code for Version ID*/
} flash_read_resource_option_t;

/*!
 * @brief Enumeration for the two possilbe options of set flexram function command.
 */
typedef enum _flash_flexram_fcunction_option
{
    kFlash_FlexRAM_Available_As_RAM = 0xFFU,    /*!< Option used to make FlexRAM available as RAM */
    kFlash_FlexRAM_Available_For_EEPROM = 0x00U /*!< Option used to make FlexRAM available for EEPROM */
} flash_flexram_function_option_t;

/*!
 * @brief Enumeration for the possible options of Swap function
 */
typedef enum _flash_swap_function_option
{
    kFlashSwapFunction_Enable = 0U, /*!< Option used to enable Swap function */
    kFlashSwapFunction_Disable = 1U /*!< Option used to Disable Swap function */
} flash_swap_function_option_t;

/*!
 * @brief Enumeration for the possible options of Swap Control commands
 */
typedef enum _flash_swap_control_option
{
    kFlashSwap_IntializeSystem = 1U,    /*!< Option used to Intialize Swap System */
    kFlashSwap_SetInUpdateState = 2U,   /*!< Option used to Set Swap in Update State */
    kFlashSwap_SetInCompleteState = 4U, /*!< Option used to Set Swap in Complete State */
    kFlashSwap_ReportStatus = 8U,       /*!< Option used to Report Swap Status */
    kFlashSwap_DisableSystem = 16U      /*!< Option used to Disable Swap Status */
} flash_swap_control_option_t;

/*!
 * @brief Enumeration for the possible flash swap status.
 */
typedef enum _flash_swap_state
{
    kFlashSwapState_Uninitialized = 0U,
    kFlashSwapState_Ready = 1U,
    kFlashSwapState_Update = 2U,
    kFlashSwapState_UpdateErased = 3U,
    kFlashSwapState_Complete = 4U,
    kFlashSwapState_Disabled = 5U
} flash_swap_state_t;

/*!
 * @breif Enumeration for the possible flash swap block status
 */
typedef enum _flash_swap_block_status
{
    kFlashSwapBlockStatus_LowerHalfProgramBlocksAtZero = 0U,
    kFlashSwapBlockStatus_UpperHalfProgramBlocksAtZero = 1U,
} flash_swap_block_status_t;

/*!
 * @brief Flash Swap information.
 */
typedef struct _flash_swap_info
{
    flash_swap_state_t flashSwapState;
    flash_swap_block_status_t currentSwapBlockStatus;
    flash_swap_block_status_t nextSwapBlockStatus;
} flash_swap_info_t;

/*!
 * @brief Enumeration for FlexRAM load during reset option.
 */
typedef enum _flash_partition_flexram_load_option
{
    kFlashPartition_FlexramNotLoaded = 0U,
    kFlashPartition_FlexramLoadedWithValidEepromData = 1U
} flash_partition_flexram_load_option_t;

/*! @brief callback type used for pflash block*/
typedef void (*flash_callback_t)(void);

/*!
 * @brief Active flash information for current operation.
 */
typedef struct _flash_operation_info
{
    uint32_t convertedAddress;
    uint32_t activeSectorSize;
    uint32_t activeBlockSize;
} flash_operation_info_t;

/*! @brief Flash driver state information.
 *
 * An instance of this structure is allocated by the user of the flash driver and
 * passed into each of the driver APIs.
 */
typedef struct _flash_driver
{
    uint32_t PFlashBlockBase;                /*!< Base address of the first PFlash block */
    uint32_t PFlashTotalSize;                /*!< Size of all combined PFlash block. */
    uint32_t PFlashBlockCount;               /*!< Number of PFlash blocks. */
    uint32_t PFlashSectorSize;               /*!< Size in bytes of a sector of PFlash. */
    flash_callback_t PFlashCallback;         /*!< Callback function for flash API. */
    uint32_t PFlashAccessSegmentSize;        /*!< Size in bytes of a access segment of PFlash. */
    uint32_t PFlashAccessSegmentCount;       /*!< Number of PFlash access segments. */
    uint32_t *flashExecuteInRamFunctionInfo; /*!< Info struct of flash execute-in-ram function. */

    uint32_t FlexRAMBlockBase; /*!< For FlexNVM device, this is the base address of FlexRAM
                                    For non-FlexNVM device, this is the base address of acceleration RAM memory */
    uint32_t FlexRAMTotalSize; /*!< For FlexNVM device, this is the size of FlexRAM
                                    For non-FlexNVM device, this is the size of acceleration RAM memory */
    uint32_t DFlashBlockBase;  /*!< For FlexNVM device, this is the base address of D-Flash memory (FlexNVM memory);
                                    For non-FlexNVM device, this field is unused */
    uint32_t DFlashTotalSize;  /*!< For FlexNVM device, this is total size of the FlexNVM memory;
                                    For non-FlexNVM device, this field is unused */
    uint32_t EEpromTotalSize;  /*!< For FlexNVM device, this is the size in byte of EEPROM area which was partitioned
                                  from FlexRAM;
                                    For non-FlexNVM device, this field is unused */
} flash_driver_t;

/*******************************************************************************
 * Externs
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*! @}*/

#endif /* _SSD_FTFx_TYPES_H_ */
