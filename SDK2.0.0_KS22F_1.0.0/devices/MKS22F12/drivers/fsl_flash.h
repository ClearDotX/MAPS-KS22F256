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

#ifndef _FSL_FLASH_H_
#define _FSL_FLASH_H_

#if (defined(BL_TARGET_FLASH) || defined(BL_TARGET_ROM) || defined(BL_TARGET_RAM))
#include <assert.h>
#include <string.h>
#include "fsl_device_registers.h"
#include "bootloader_common.h"
#else
#include "fsl_common.h"
#endif

/*!
 * @addtogroup flash
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @name Flash version
 * @{
 */
/*! @brief Construct the version number for drivers. */
#if !defined(MAKE_VERSION)
#define MAKE_VERSION(major, minor, bugfix) (((major) << 16) | ((minor) << 8) | (bugfix))
#endif

/*! @brief FLASH driver version for SDK*/
#define FSL_FLASH_DRIVER_VERSION (MAKE_VERSION(2, 0, 0)) /*!< Version 2.0.0. */

/*! @brief FLASH driver version for ROM*/
enum _flash_driver_version_constants
{
    kFlashDriver_Version_Name = 'F',
    kFlashDriver_Version_Major = 2,
    kFlashDriver_Version_Minor = 0,
    kFlashDriver_Version_Bugfix = 0
};
/*@}*/

/*!
 * @name Flash configuration
 * @{
 */
/*! @brief  Whether to support FlexNVM in flash driver */
#if !defined(FLASH_SSD_CONFIG_ENABLE_FLEXNVM_SUPPORT)
#define FLASH_SSD_CONFIG_ENABLE_FLEXNVM_SUPPORT 1
#endif

#define FLASH_SSD_IS_FLEXNVM_ENABLED (FLASH_SSD_CONFIG_ENABLE_FLEXNVM_SUPPORT && FSL_FEATURE_FLASH_HAS_FLEX_NVM)

/*! @brief Flash driver location. */
#if !defined(FLASH_DRIVER_IS_FLASH_RESIDENT)
#if (!defined(BL_TARGET_ROM) && !defined(BL_TARGET_RAM))
#define FLASH_DRIVER_IS_FLASH_RESIDENT 1
#else
#define FLASH_DRIVER_IS_FLASH_RESIDENT 0
#endif
#endif

/*! @brief Flash Driver Export option */
#if !defined(FLASH_DRIVER_IS_EXPORTED)
#if (defined(BL_TARGET_ROM) || defined(BL_TARGET_FLASH))
#define FLASH_DRIVER_IS_EXPORTED 1
#else
#define FLASH_DRIVER_IS_EXPORTED 0
#endif
#endif
/*@}*/

/*!
 * @name Flash status
 * @{
 */
#if defined(kStatusGroup_FlashDriver)
#define kStatusGroupGeneric kStatusGroup_Generic
#define kStatusGroupFlashDriver kStatusGroup_FlashDriver
#elif defined(kStatusGroup_FLASH)
#define kStatusGroupGeneric kStatusGroup_Generic
#define kStatusGroupFlashDriver kStatusGroup_FLASH
#else
#define kStatusGroupGeneric 0
#define kStatusGroupFlashDriver 1
#endif

/*! @brief Construct a status code value from a group and code number. */
#if !defined(MAKE_STATUS)
#define MAKE_STATUS(group, code) ((((group)*100) + (code)))
#endif

/*!
 * @brief Flash driver status codes.
 */
enum _flash_status
{
    kStatus_FLASH_Success = MAKE_STATUS(kStatusGroupGeneric, 0),
    kStatus_FLASH_InvalidArgument = MAKE_STATUS(kStatusGroupGeneric, 4),
    kStatus_FLASH_SizeError = MAKE_STATUS(kStatusGroupFlashDriver, 0),
    kStatus_FLASH_AlignmentError = MAKE_STATUS(kStatusGroupFlashDriver, 1),
    kStatus_FLASH_AddressError = MAKE_STATUS(kStatusGroupFlashDriver, 2),
    kStatus_FLASH_AccessError = MAKE_STATUS(kStatusGroupFlashDriver, 3),
    kStatus_FLASH_ProtectionViolation = MAKE_STATUS(kStatusGroupFlashDriver, 4),
    kStatus_FLASH_CommandFailure = MAKE_STATUS(kStatusGroupFlashDriver, 5),
    kStatus_FLASH_UnknownProperty = MAKE_STATUS(kStatusGroupFlashDriver, 6),
    kStatus_FLASH_EraseKeyError = MAKE_STATUS(kStatusGroupFlashDriver, 7),
    kStatus_FLASH_RegionExecuteOnly = MAKE_STATUS(kStatusGroupFlashDriver, 8),
    kStatus_FLASH_ExecuteInRamFunctionNotReady = MAKE_STATUS(kStatusGroupFlashDriver, 9),
    kStatus_FLASH_PartitionStatusUpdateFailure = MAKE_STATUS(kStatusGroupFlashDriver, 10),
    kStatus_FLASH_SetFlexramAsEepromError = MAKE_STATUS(kStatusGroupFlashDriver, 11),
    kStatus_FLASH_RecoverFlexramAsRamError = MAKE_STATUS(kStatusGroupFlashDriver, 12),
    kStatus_FLASH_SetFlexramAsRamError = MAKE_STATUS(kStatusGroupFlashDriver, 13),
    kStatus_FLASH_RecoverFlexramAsEepromError = MAKE_STATUS(kStatusGroupFlashDriver, 14),
    kStatus_FLASH_CommandNotSupported = MAKE_STATUS(kStatusGroupFlashDriver, 15),
    kStatus_FLASH_SwapSystemNotInUninitialized = MAKE_STATUS(kStatusGroupFlashDriver, 16),
    kStatus_FLASH_SwapIndicatorAddressError = MAKE_STATUS(kStatusGroupFlashDriver, 17),
};
/*@}*/

/*!
 * @name Flash API key
 * @{
 */
/*! @brief Construct the four char code for flash driver API key. */
#if !defined(FOUR_CHAR_CODE)
#define FOUR_CHAR_CODE(a, b, c, d) (((d) << 24) | ((c) << 16) | ((b) << 8) | ((a)))
#endif

/*!
 * @brief Enumeration for flash driver API keys.
 *
 * Build a 32-bit code from four character values.
 *
 * The resulting value is built with a byte order such that the string
 * being readable in expected order when viewed in a hex editor, if the value
 * is treated as a 32-bit little endian value.
 */
enum _flash_driver_api_keys
{
    kFlashEraseKey = FOUR_CHAR_CODE('k', 'f', 'e', 'k') /*!< Key value used to validate all flash erase APIs.*/
};
/*@}*/

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
 * @brief Constants for execute-in-ram flash function.
 */
enum _flash_execute_in_ram_function_constants
{
    kFlashExecuteInRamFunction_MaxSize = 64U,
    kFlashExecuteInRamFunction_TotalNum = 2U
};

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
 * @brief Enumeration for the maximum size of special-purpose flash resource
 */
enum __flash_read_resource_max_size
{
#if (FSL_FEATURE_FLASH_IS_FTFE == 1)
    kFlashResource_PFlashIFRSizeInBytes = 1024,
    kFlashResource_VersionIdSizeInBytes = 8,
    kFlashResource_VersionIdStart = 0x08,
    kFlashResource_VersionIdSizeEnd = 0x0F,
#else /* FSL_FEATURE_FLASH_IS_FTFL == 1 or FSL_FEATURE_FLASH_IS_FTFA = =1 */
    kFlashResource_PFlashIFRSizeInBytes = 256,
    kFlashResource_VersionIdSizeInBytes = 8,
    kFlashResource_VersionIdStart = 0x00,
    kFlashResource_VersionIdSizeEnd = 0x07,
#endif
    kFlashResource_PFlashSwapIfrStart = 0x40000,
    kFlashResource_PFlashSwapIfrEnd = 0x403FF,
    kFlashResource_DFlashIfrStart = 0x800000,
    kFlashResource_DFlashIfrEnd = 0x8003FF,
};

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
 * @brief Flash Swap IFR fileds.
 */
typedef struct _flash_swap_ifr_field
{
    uint16_t swapIndicatorAddress;
    uint16_t swapEnableWord;
    uint8_t reserved0[6];
    uint16_t swapDisableWord;
    uint8_t reserved1[4];
} flash_swap_ifr_field_t;

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
    uint32_t FlexRAMBlockBase;               /*!< For FlexNVM device, this is the base address of FlexRAM
                                                  For non-FlexNVM device, this is the base address of acceleration RAM memory */
    uint32_t FlexRAMTotalSize;               /*!< For FlexNVM device, this is the size of FlexRAM
                                                  For non-FlexNVM device, this is the size of acceleration RAM memory */
    uint32_t DFlashBlockBase; /*!< For FlexNVM device, this is the base address of D-Flash memory (FlexNVM memory);
                                   For non-FlexNVM device, this field is unused */
    uint32_t DFlashTotalSize; /*!< For FlexNVM device, this is total size of the FlexNVM memory;
                                   For non-FlexNVM device, this field is unused */
    uint32_t EEpromTotalSize; /*!< For FlexNVM device, this is the size in byte of EEPROM area which was partitioned
                                 from FlexRAM;
                                   For non-FlexNVM device, this field is unused */
} flash_driver_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name Initialization
 * @{
 */

/*!
 * @brief Initializes global flash properties structure members
 *
 * This function checks and initializes Flash module for the other Flash APIs.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @return An error code or kStatus_Success
 */
status_t FLASH_Init(flash_driver_t *driver);

/*!
 * @brief Set the desired flash callback function
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param callback  callback function to be stored in driver
 * @return An error code or kStatus_Success
 */
status_t FLASH_SetCallback(flash_driver_t *driver, flash_callback_t callback);

/*!
 * @brief Prepare flash execute-in-ram functions
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @return An error code or kStatus_Success
 */
#if FLASH_DRIVER_IS_FLASH_RESIDENT
status_t FLASH_PrepareExecuteInRamFunctions(flash_driver_t *driver);
#endif

/*!
 * @brief Perform the cache clear to the flash
 *
 * @param driver Pointer to storage for the driver runtime state.
 */
void flash_cache_clear(flash_driver_t *driver);

/*@}*/

/*!
 * @name Erasing
 * @{
 */

/*!
 * @brief Erases entire flash
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param key value used to validate all flash erase APIs.
 * @return An error code or kStatus_Success
 */
status_t FLASH_EraseAll(flash_driver_t *driver, uint32_t key);

/*!
 * @brief Erases flash sectors encompassed by parameters passed into function
 *
 * This function erases the appropriate number of flash sectors based on the
 * desired start address and length.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param start The start address of the desired flash memory to be erased.
 *        The start address does not need to be sector aligned but must be word-aligned.
 * @param lengthInBytes The length, given in bytes (not words or long-words)
 *        to be erased. Must be word aligned.
 * @param key value used to validate all flash erase APIs.
 *
 * @return An error code or kStatus_Success
 */
status_t FLASH_Erase(flash_driver_t *driver, uint32_t start, uint32_t lengthInBytes, uint32_t key);

/*!
 * @brief Erases entire flash, including protected sectors.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param key value used to validate all flash erase APIs.
 * @return An error code or kStatus_Success
 */
#if defined(FSL_FEATURE_FLASH_HAS_ERASE_ALL_BLOCKS_UNSECURE_CMD) && FSL_FEATURE_FLASH_HAS_ERASE_ALL_BLOCKS_UNSECURE_CMD
status_t FLASH_EraseAllUnsecure(flash_driver_t *driver, uint32_t key);
#endif

/*!
 * @brief Erases all program flash execute-only segments defined by the FXACC registers.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param key value used to validate all flash erase APIs.
 * @return An error code or kStatus_Success
 */
status_t FLASH_EraseAllExecuteOnlySegments(flash_driver_t *driver, uint32_t key);

/*@}*/

/*!
 * @name Programming
 * @{
 */

/*!
 * @brief Programs flash with data at locations passed in through parameters
 *
 * This function programs the flash memory with desired data for a given
 * flash area as determined by the start address and length.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param start The start address of the desired flash memory to be programmed. Must be
 *              word-aligned.
 * @param lengthInBytes The length, given in bytes (not words or long-words)
 *        to be programmed. Must be word-aligned.
 * @param src Pointer to the source buffer of data that is to be programmed
 *        into the flash.
 * @return An error code or kStatus_Success
 */
status_t FLASH_Program(flash_driver_t *driver, uint32_t start, uint32_t *src, uint32_t lengthInBytes);

/*!
 * @brief Programs Program Once Field through parameters
 *
 * This function programs the Program Once Field with desired data for a given
 * flash area as determined by the index and length.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param index The index indicating which area of Program Once Field to be programmed.
 * @param lengthInBytes The length, given in bytes (not words or long-words)
 *        to be programmed. Must be word-aligned.
 * @param src Pointer to the source buffer of data that is to be programmed
 *        into the Program Once Field.
 * @return An error code or kStatus_Success
 */
status_t FLASH_ProgramOnce(flash_driver_t *driver, uint32_t index, uint32_t *src, uint32_t lengthInBytes);

/*!
 * @brief Programs flash with data at locations passed in through parameters via Program Section command
 *
 * This function programs the flash memory with desired data for a given
 * flash area as determined by the start address and length.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param start The start address of the desired flash memory to be programmed. Must be
 *              word-aligned.
 * @param lengthInBytes The length, given in bytes (not words or long-words)
 *        to be programmed. Must be word-aligned.
 * @param src Pointer to the source buffer of data that is to be programmed
 *        into the flash.
 * @return An error code or kStatus_Success
 */
#if defined(FSL_FEATURE_FLASH_HAS_PROGRAM_SECTION_CMD) && FSL_FEATURE_FLASH_HAS_PROGRAM_SECTION_CMD
status_t FLASH_ProgramSection(flash_driver_t *driver, uint32_t start, uint32_t *src, uint32_t lengthInBytes);
#endif

/*!
 * @brief Programs EEPROM with data at locations passed in through parameters
 *
 * This function programs the Emulated EEPROM with desired data for a given
 * flash area as determined by the start address and length.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param start The start address of the desired flash memory to be programmed. Must be
 *              word-aligned.
 * @param lengthInBytes The length, given in bytes (not words or long-words)
 *        to be programmed. Must be word-aligned.
 * @param src Pointer to the source buffer of data that is to be programmed
 *        into the flash.
 * @return An error code or kStatus_Success
 */
#if FLASH_SSD_IS_FLEXNVM_ENABLED
status_t FLASH_EepromWrite(flash_driver_t *driver, uint32_t start, uint8_t *src, uint32_t lengthInBytes);
#endif

/*@}*/

/*!
 * @name Reading
 * @{
 */

/*!
 * @brief Read resource with data at locations passed in through parameters
 *
 * This function reads the flash memory with desired location for a given
 * flash area as determined by the start address and length.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param start The start address of the desired flash memory to be programmed. Must be
 *              word-aligned.
 * @param lengthInBytes The length, given in bytes (not words or long-words)
 *        to be read. Must be word-aligned.
 * @param dst Pointer to the destination buffer of data that is used to store
 *        data to be read.
 * @param option The resource option which indicates which area should be read back.
 * @return An error code or kStatus_Success
 */
#if defined(FSL_FEATURE_FLASH_HAS_READ_RESOURCE_CMD) && FSL_FEATURE_FLASH_HAS_READ_RESOURCE_CMD
status_t FLASH_ReadResource(
    flash_driver_t *driver, uint32_t start, uint32_t *dst, uint32_t lengthInBytes, flash_read_resource_option_t option);
#endif

/*!
 * @brief Read Program Once Field through parameters
 *
 * This function reads the read once feild with given index and length
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param index The index indicating the area of program once field to be read.
 * @param lengthInBytes The length, given in bytes (not words or long-words)
 *        to be programmed. Must be word-aligned.
 * @param dst Pointer to the destination buffer of data that is used to store
 *        data to be read.
 * @return An error code or kStatus_Success
 */
status_t FLASH_ReadOnce(flash_driver_t *driver, uint32_t index, uint32_t *dst, uint32_t lengthInBytes);

/*@}*/

/*!
 * @name Security
 * @{
 */

/*!
 * @brief Returns the security state via the pointer passed into the function
 *
 * This function retrieves the current Flash security status, including the
 * security enabling state and the backdoor key enabling state.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param state Pointer to the value returned for the current security status code:
 *          - #kFlashNotSecure
 *          - #kFlashSecureBackdoorEnabled
 *          - #kFlashSecureBackdoorDisabled
 * @return kStatus_Success
 */
status_t FLASH_GetSecurityState(flash_driver_t *driver, flash_security_state_t *state);

/*!
 * @brief Allows user to bypass security with a backdoor key
 *
 * If the MCU is in secured state, this function will unsecure the MCU by
 * comparing the provided backdoor key with ones in the Flash Configuration
 * Field.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param backdoorKey Pointer to the user buffer containing the backdoor key.
 * @return An error code or kStatus_Success
 */
status_t FLASH_SecurityBypass(flash_driver_t *driver, const uint8_t *backdoorKey);

/*@}*/

/*!
 * @name Verification
 * @{
 */

/*!
 * @brief Verifies erasure of entire flash at specified margin level
 *
 * This function will check to see if the flash have been erased to the
 * specified read margin level.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param margin Read margin choice as follows: #kFlashMargin_Normal,
 *        #kFlashMargin_User, or #kFlashMargin_Factory
 * @return An error code or kStatus_Success
 */
status_t FLASH_VerifyEraseAll(flash_driver_t *driver, flash_margin_value_t margin);

/*!
 * @brief Verifies erasure of desired flash area at specified margin level
 *
 * This function will check the appropriate number of flash sectors based on
 * the desired start address and length to see if the flash have been erased
 * to the specified read margin level.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param start The start address of the desired flash memory to be verified.
 *        The start address does not need to be sector aligned but must be word-aligned.
 * @param lengthInBytes The length, given in bytes (not words or long-words)
 *        to be verified. Must be word-aligned.
 * @param margin Read margin choice as follows: #kFlashMargin_Normal,
 *        #kFlashMargin_User, or #kFlashMargin_Factory
 * @return An error code or kStatus_Success
 */
status_t FLASH_VerifyErase(flash_driver_t *driver, uint32_t start, uint32_t lengthInBytes, flash_margin_value_t margin);

/*!
 * @brief Verifies programming of desired flash area at specified margin level
 *
 * This function verifies the data programed in the flash memory using the
 * Flash Program Check Command and compares it with expected data for a given
 * flash area as determined by the start address and length.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param start The start address of the desired flash memory to be verified. Must be word-aligned.
 * @param lengthInBytes The length, given in bytes (not words or long-words)
 *        to be verified. Must be word-aligned.
 * @param expectedData Pointer to the expected data that is to be
 *        verified against.
 * @param margin Read margin choice as follows: kFlashMargin_User or
 *        kFlashMargin_Factory
 * @param failedAddress Pointer to returned failing address.
 * @param failedData Pointer to returned failing data.  Some derivitives do
 *        not included failed data as part of the FCCOBx registers.  In this
 *        case, zeros are returned upon failure.
 * @return An error code or kStatus_Success
 */
status_t FLASH_VerifyProgram(flash_driver_t *driver,
                             uint32_t start,
                             uint32_t lengthInBytes,
                             const uint32_t *expectedData,
                             flash_margin_value_t margin,
                             uint32_t *failedAddress,
                             uint32_t *failedData);

/*!
 * @brief Verifies if the program flash executeonly segments have been erased to
 *  the specified read margin level
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param margin Read margin choice as follows: kFlashMargin_User or
 *        kFlashMargin_Factory
 * @return An error code or kStatus_Success
 */
status_t FLASH_VerifyEraseAllExecuteOnlySegments(flash_driver_t *driver, flash_margin_value_t margin);

/*@}*/

/*!
 * @name Protection
 * @{
 */

/*!
 * @brief Returns the protection state of desired flash area via the pointer passed into the function
 *
 * This function retrieves the current Flash protect status for a given
 * flash area as determined by the start address and length.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param start The start address of the desired flash memory to be checked. Must be word-aligned.
 * @param lengthInBytes The length, given in bytes (not words or long-words)
 *        to be checked.  Must be word-aligned.
 * @param protection_state Pointer to the value returned for the current
 *        protection status code for the desired flash area.  The status code
 *        can be kFlashProtection_Unprotected, kFlashProtection_Protected, or
 *        kFlashProtection_Mixed.
 * @return An error code or kStatus_Success
 */
status_t FLASH_IsProtected(flash_driver_t *driver,
                           uint32_t start,
                           uint32_t lengthInBytes,
                           flash_protection_state_t *protection_state);

/*!
 * @brief Returns the access state of desired flash area via the pointer passed into the function
 *
 * This function retrieves the current Flash access status for a given
 * flash area as determined by the start address and length.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param start The start address of the desired flash memory to be checked. Must be word-aligned.
 * @param lengthInBytes The length, given in bytes (not words or long-words)
 *        to be checked.  Must be word-aligned.
 * @param access_state Pointer to the value returned for the current
 *        access status code for the desired flash area.  The status code
 *        can be kFlashAccess_UnLimited, kFlashAccess_ExecuteOnly, or
 *        kFlashAccess_Mixed.
 * @return An error code or kStatus_Success
 */
status_t FLASH_IsExecuteOnly(flash_driver_t *driver,
                             uint32_t start,
                             uint32_t lengthInBytes,
                             flash_execute_only_access_state_t *access_state);

/*@}*/

/*!
 * @name Properties
 * @{
 */

/*!
 * @brief Returns the desired flash property.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param whichProperty The desired property from the list of properties in
 *        enum flash_property_t
 * @param value Pointer to the value returned for the desired flash property
 * @return An error code or kStatus_Success
 */
status_t FLASH_GetProperty(flash_driver_t *driver, flash_property_t whichProperty, uint32_t *value);

/*@}*/

/*!
 * @name FlexRAM
 * @{
 */

/*!
 * @brief Set FlexRAM Function command
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param option The option used to set work mode of FlexRAM
 * @return An error code or kStatus_Success
 */
#if defined(FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD) && FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD
status_t FLASH_SetFlexramFunction(flash_driver_t *driver, flash_flexram_function_option_t option);
#endif

/*@}*/

/*!
 * @name Swap
 * @{
 */

/*!
 * @brief Configure Swap function or Check the swap state of Flash Module
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param address Address used to configure the flash swap function
 * @param option The possible option used to configure Flash Swap function or check the flash swap status
 * @param returnInfo Pointer to the data which is used to return the information of flash swap.
 *
 * @return An error code or kStatus_Success
 */
#if defined(FSL_FEATURE_FLASH_HAS_SWAP_CONTROL_CMD) && FSL_FEATURE_FLASH_HAS_SWAP_CONTROL_CMD
status_t FLASH_SwapControl(flash_driver_t *driver,
                           uint32_t address,
                           flash_swap_control_option_t option,
                           flash_swap_info_t *returnInfo);
#endif

/*!
 * @brief Swap the lower half flash with the higher half flaock
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param address Address used to configure the flash swap function
 * @param option The possible option used to configure Flash Swap function or check the flash swap status
 *
 * @return An error code or kStatus_Success
 */
#if defined(FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP) && FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP
status_t FLASH_Swap(flash_driver_t *driver, uint32_t address, flash_swap_function_option_t option);
#endif

/*!
 * @name FlexNVM
 * @{
 */

/*!
 * @brief Prepares the FlexNVM block for use as data flash, EEPROM backup, or a combination of both and initializes the
 * FlexRAM.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param option The option used to set FlexRAM load behavior during reset.
 * @param eepromDataSizeCode Determines the amount of FlexRAM used in each of the available EEPROM subsystems.
 * @param flexnvmPartitionCode Specifies how to split the FlexNVM block between data flash memory and EEPROM backup
 * memory supporting EEPROM functions.
 * @return An error code or kStatus_Success
 */
#if defined(FSL_FEATURE_FLASH_HAS_PROGRAM_PARTITION_CMD) && FSL_FEATURE_FLASH_HAS_PROGRAM_PARTITION_CMD
status_t FLASH_ProgramPartition(flash_driver_t *driver,
                                flash_partition_flexram_load_option_t option,
                                uint32_t eepromDataSizeCode,
                                uint32_t flexnvmPartitionCode);
#endif

/*@}*/

/*!
* @name Flash Protection Utilities
* @{
*/

/*!
 * @brief Set PFLASH Protection to the intended protection status.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param protectStatus The expected protect status user wants to set to PFlash protection register. Each bit is
 * corresponding to protection of 1/32 of the total PFlash. The least significant bit is corresponding to the lowest
 * address area of P-Flash. The most significant bit is corresponding to the highest address area of PFlash. There are
 * two possible cases as shown below:
 *       0: this area is protected.
 *       1: this area is unprotected.
 *
 * @return An error code or kStatus_Success
 */
status_t FLASH_PflashSetProtection(flash_driver_t *driver, uint32_t protectStatus);

/*!
 * @brief Get PFLASH Protection Status.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param protectStatus  Protect status returned by PFlash IP. Each bit is corresponding to protection of 1/32 of the
 * total PFlash. The least significant bit is corresponding to the lowest address area of PFlash. The most significant
 * bit is corresponding to the highest address area of PFlash. Thee are two possible cases as below:
 *       0: this area is protected.
 *       1: this area is unprotected.
 *
 * @return An error code or kStatus_Success
 */
status_t FLASH_PflashGetProtection(flash_driver_t *driver, uint32_t *protectStatus);

/*!
 * @brief Set DFLASH Protection to the intended protection status.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param protectStatus The expected protect status user wants to set to DFlash protection register. Each bit is
 * corresponding to protection of 1/8 of the total DFlash. The least significant bit is corresponding to the lowest
 * address area of DFlash. The most significant bit is corresponding to the highest address area of  DFlash. There are
 * two possible cases as shown below:
 *       0: this area is protected.
 *       1: this area is unprotected.
 *
 * @return An error code or kStatus_Success
 */
#if FLASH_SSD_IS_FLEXNVM_ENABLED
status_t FLASH_DflashSetProtection(flash_driver_t *driver, uint8_t protectStatus);
#endif

/*!
 * @brief Get DFLASH Protection Status.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param protectStatus  DFlash Protect status returned by PFlash IP. Each bit is corresponding to protection of 1/8 of
 * the total DFlash. The least significant bit is corresponding to the lowest address area of DFlash. The most
 * significant bit is corresponding to the highest address area of DFlash and so on. There are two possible cases as
 * below:
 *       0: this area is protected.
 *       1: this area is unprotected.
 *
 * @return An error code or kStatus_Success
 */
#if FLASH_SSD_IS_FLEXNVM_ENABLED
status_t FLASH_DflashGetProtection(flash_driver_t *driver, uint8_t *protectStatus);
#endif

/*!
 * @brief Set EEPROM Protection to the intended protection status.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param protectStatus The expected protect status user wants to set to EEPROM protection register. Each bit is
 * corresponding to protection of 1/8 of the total EEPROM. The least significant bit is corresponding to the lowest
 * address area of EEPROM. The most significant bit is corresponding to the highest address area of EEPROM, and so on.
 * There are two possible cases as shown below:
 *       0: this area is protected.
 *       1: this area is unprotected.
 *
 * @return An error code or kStatus_Success
 */
#if FLASH_SSD_IS_FLEXNVM_ENABLED
status_t FLASH_EepromSetProtection(flash_driver_t *driver, uint8_t protectStatus);
#endif

/*!
 * @brief Get DFLASH Protection Status.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param protectStatus  DFlash Protect status returned by PFlash IP. Each bit is corresponding to protection of 1/8 of
 * the total EEPROM. The least significant bit is corresponding to the lowest address area of EEPROM. The most
 * significant bit is corresponding to the highest address area of EEPROM. There are two possible cases as below:
 *       0: this area is protected.
 *       1: this area is unprotected.
 *
 * @return An error code or kStatus_Success
 */
#if FLASH_SSD_IS_FLEXNVM_ENABLED
status_t FLASH_EepromGetProtection(flash_driver_t *driver, uint8_t *protectStatus);
#endif

/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* _FSL_FLASH_H_ */
