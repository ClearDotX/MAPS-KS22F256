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

#include "SSD_FTFx_Common.h"
#include "SSD_FTFx_Types.h"
#include "SSD_FTFx_Config.h"
#include "SSD_FTFx_Misc.h"
#include "SSD_FTFx_Internal.h"

/*!
 * @addtogroup flash
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
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

/*!
 * @brief Enumeration for flash driver API keys.
 */

//! @brief Build a 32-bit code from four character values.
//!
//! The resulting value is built with a byte order such that the string
//! being readable in expected order when viewed in a hex editor, if the value
//! is treated as a 32-bit little endian value.
#if !defined(FOUR_CHAR_CODE)
#define FOUR_CHAR_CODE(a, b, c, d) (((d) << 24) | ((c) << 16) | ((b) << 8) | ((a)))
#endif

enum _flash_driver_api_keys
{
    kFlashEraseKey = FOUR_CHAR_CODE('k', 'f', 'e', 'k') /*!< Key value used to validate all flash erase APIs.*/
};

/*!
 * @brief Constants for execute-in-ram flash function.
 */
enum _flash_execute_in_ram_function_constants
{
    kFlashExecuteInRamFunction_MaxSize = 32U,
    kFlashExecuteInRamFunction_TotalNum = 2U
};

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
 * @param driver    Pointer to storage for the driver runtime state.
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
status_t FLASH_PrepareExecuteInRamFunctions(flash_driver_t *driver);

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
status_t FLASH_EraseAllUnsecure(flash_driver_t *driver, uint32_t key);

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

status_t FLASH_ProgramSection(flash_driver_t *driver, uint32_t start, uint32_t *src, uint32_t lengthInBytes);

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
status_t FLASH_EepromWrite(flash_driver_t *driver, uint32_t start, uint8_t *src, uint32_t lengthInBytes);

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
status_t FLASH_ReadResource(
    flash_driver_t *driver, uint32_t start, uint32_t *dst, uint32_t lengthInBytes, flash_read_resource_option_t option);

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
status_t FLASH_SetFlexramFunction(flash_driver_t *driver, flash_flexram_function_option_t option);

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
status_t FLASH_SwapControl(flash_driver_t *driver,
                           uint32_t address,
                           flash_swap_control_option_t option,
                           flash_swap_info_t *returnInfo);

/*!
 * @brief Swap the lower half flash with the higher half flaock
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @param address Address used to configure the flash swap function
 * @param option The possible option used to configure Flash Swap function or check the flash swap status
 *
 * @return An error code or kStatus_Success
 */
status_t FLASH_Swap(flash_driver_t *driver, uint32_t address, flash_swap_function_option_t option);

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
status_t FLASH_ProgramPartition(flash_driver_t *driver,
                                flash_partition_flexram_load_option_t option,
                                uint32_t eepromDataSizeCode,
                                uint32_t flexnvmPartitionCode);

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
status_t FLASH_DflashSetProtection(flash_driver_t *driver, uint8_t protectStatus);

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
status_t FLASH_DflashGetProtection(flash_driver_t *driver, uint8_t *protectStatus);

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
status_t FLASH_EepromSetProtection(flash_driver_t *driver, uint8_t protectStatus);

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
status_t FLASH_EepromGetProtection(flash_driver_t *driver, uint8_t *protectStatus);

/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* _FSL_FLASH_H_ */
