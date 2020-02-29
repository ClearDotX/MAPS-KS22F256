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

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#ifndef ALIGN_DOWN
#define ALIGN_DOWN(x, a) ((x) & (uint32_t)(-((int32_t)(a))))
#endif
#ifndef ALIGN_UP
#define ALIGN_UP(x, a) (-((int32_t)((uint32_t)(-((int32_t)(x))) & (uint32_t)(-((int32_t)(a))))))
#endif

#define BYTES_JOIN_TO_WORD_1_3(x, y) ((((uint32_t)(x)&0xFFU) << 24) | ((uint32_t)(y)&0xFFFFFFU))
#define BYTES_JOIN_TO_WORD_2_2(x, y) ((((uint32_t)(x)&0xFFFFU) << 16) | ((uint32_t)(y)&0xFFFFU))
#define BYTES_JOIN_TO_WORD_3_1(x, y) ((((uint32_t)(x)&0xFFFFFFU) << 8) | ((uint32_t)(y)&0xFFU))
#define BYTES_JOIN_TO_WORD_1_1_2(x, y, z) \
    ((((uint32_t)(x)&0xFFU) << 24) | (((uint32_t)(y)&0xFFU) << 16) | ((uint32_t)(z)&0xFFFFU))
#define BYTES_JOIN_TO_WORD_1_2_1(x, y, z) \
    ((((uint32_t)(x)&0xFFU) << 24) | (((uint32_t)(y)&0xFFFFU) << 8) | ((uint32_t)(z)&0xFFU))
#define BYTES_JOIN_TO_WORD_2_1_1(x, y, z) \
    ((((uint32_t)(x)&0xFFFFU) << 16) | (((uint32_t)(y)&0xFFU) << 8) | ((uint32_t)(z)&0xFFU))
#define BYTES_JOIN_TO_WORD_1_1_1_1(x, y, z, w)                                                      \
    ((((uint32_t)(x)&0xFFU) << 24) | (((uint32_t)(y)&0xFFU) << 16) | (((uint32_t)(z)&0xFFU) << 8) | \
     ((uint32_t)(w)&0xFFU))

/*! @brief Data flash IFR map Field*/
#if defined(FSL_FEATURE_FLASH_IS_FTFE) && FSL_FEATURE_FLASH_IS_FTFE
#define DFLASH_IFR_READRESOURCE_START_ADDRESS 0x8003F8U
#else /* FSL_FEATURE_FLASH_IS_FTFL == 1 or FSL_FEATURE_FLASH_IS_FTFA = =1 */
#define DFLASH_IFR_READRESOURCE_START_ADDRESS 0x8000F8U
#endif

#define FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED 0xFFFFFFFFU
#define FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_RESERVED 0xFFFFU

/*!
 * @name Common flash feature defines
 * @{
 */
#if FLASH_SSD_IS_FLEXNVM_ENABLED
#if (FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_WRITE_UNIT_SIZE != FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE)
#error "flash driver may not adapt to FlexNVM!"
#endif
#if (FSL_FEATURE_FLASH_FLEX_NVM_SECTOR_CMD_ADDRESS_ALIGMENT != FSL_FEATURE_FLASH_PFLASH_SECTOR_CMD_ADDRESS_ALIGMENT)
#error "flash driver may not adapt to FlexNVM!"
#endif
#if (FSL_FEATURE_FLASH_FLEX_NVM_SECTION_CMD_ADDRESS_ALIGMENT != FSL_FEATURE_FLASH_PFLASH_SECTION_CMD_ADDRESS_ALIGMENT)
#error "flash driver may not adapt to FlexNVM!"
#endif
#if (FSL_FEATURE_FLASH_FLEX_NVM_RESOURCE_CMD_ADDRESS_ALIGMENT != FSL_FEATURE_FLASH_PFLASH_RESOURCE_CMD_ADDRESS_ALIGMENT)
#error "flash driver may not adapt to FlexNVM!"
#endif
#if (FSL_FEATURE_FLASH_FLEX_NVM_CHECK_CMD_ADDRESS_ALIGMENT != FSL_FEATURE_FLASH_PFLASH_CHECK_CMD_ADDRESS_ALIGMENT)
#error "flash driver may not adapt to FlexNVM!"
#endif
#endif

#define COMMON_FEATURE_FLASH_BLOCK_WRITE_UNIT_SIZE FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE
#define COMMON_FEATURE_FLASH_SECTOR_CMD_ADDRESS_ALIGMENT FSL_FEATURE_FLASH_PFLASH_SECTOR_CMD_ADDRESS_ALIGMENT
#define COMMON_FEATURE_FLASH_SECTION_CMD_ADDRESS_ALIGMENT FSL_FEATURE_FLASH_PFLASH_SECTION_CMD_ADDRESS_ALIGMENT
#define COMMON_FEATURE_FLASH_RESOURCE_CMD_ADDRESS_ALIGMENT FSL_FEATURE_FLASH_PFLASH_RESOURCE_CMD_ADDRESS_ALIGMENT
#define COMMON_FEATURE_FLASH_CHECK_CMD_ADDRESS_ALIGMENT FSL_FEATURE_FLASH_PFLASH_CHECK_CMD_ADDRESS_ALIGMENT

enum __flash_read_resource_alignment
{
    kFlashReadResource_AlignSizeInBytes = COMMON_FEATURE_FLASH_RESOURCE_CMD_ADDRESS_ALIGMENT,
};

enum _flash_read_resource_unit_size
{
    kFlashReadResource_UnitInBytes = COMMON_FEATURE_FLASH_RESOURCE_CMD_ADDRESS_ALIGMENT,
};

/*@}*/

/*!
 * @name Flash Program Once Field defines
 * @{
 */
#if defined(FSL_FEATURE_FLASH_IS_FTFA) && FSL_FEATURE_FLASH_IS_FTFA
/* FTFA parts(eg. K80, KL80, L5K) support both 4-bytes and 8-bytes unit size */
#define FLASH_PROGRAM_ONCE_MIN_ID_8BYTES \
    0x10 /* Minimum Index indcating one of Progam Once Fields which is accessed in 8-byte records */
#define FLASH_PROGRAM_ONCE_MAX_ID_8BYTES \
    0x13 /* Maximum Index indcating one of Progam Once Fields which is accessed in 8-byte records */
#define FLASH_PROGRAM_ONCE_IS_4BYTES_UNIT_SUPPORT 1
#define FLASH_PROGRAM_ONCE_IS_8BYTES_UNIT_SUPPORT 1
#elif defined(FSL_FEATURE_FLASH_IS_FTFE) && FSL_FEATURE_FLASH_IS_FTFE
/* FTFE parts(eg. K65, KE18) only support 8-bytes unit size */
#define FLASH_PROGRAM_ONCE_IS_4BYTES_UNIT_SUPPORT 0
#define FLASH_PROGRAM_ONCE_IS_8BYTES_UNIT_SUPPORT 1
#elif defined(FSL_FEATURE_FLASH_IS_FTFL) && FSL_FEATURE_FLASH_IS_FTFL
/* FTFL parts(eg. K20) only support 4-bytes unit size */
#define FLASH_PROGRAM_ONCE_IS_4BYTES_UNIT_SUPPORT 1
#define FLASH_PROGRAM_ONCE_IS_8BYTES_UNIT_SUPPORT 0
#endif
/*@}*/

/*!
 * @name Flash security status defines
 * @{
 */
#define FLASH_SECURITY_STATE_KEYEN 0x80
#define FLASH_SECURITY_STATE_UNSECURED 0x02
#define FLASH_NOT_SECURE 0x01
#define FLASH_SECURE_BACKDOOR_ENABLED 0x02
#define FLASH_SECURE_BACKDOOR_DISABLED 0x04
/*@}*/

/*!
 * @name Flash controller command numbers
 * @{
 */
#define FTFx_VERIFY_BLOCK 0x00                    /*!< RD1BLK*/
#define FTFx_VERIFY_SECTION 0x01                  /*!< RD1SEC*/
#define FTFx_PROGRAM_CHECK 0x02                   /*!< PGMCHK*/
#define FTFx_READ_RESOURCE 0x03                   /*!< RDRSRC*/
#define FTFx_PROGRAM_LONGWORD 0x06                /*!< PGM4*/
#define FTFx_PROGRAM_PHRASE 0x07                  /*!< PGM8*/
#define FTFx_ERASE_BLOCK 0x08                     /*!< ERSBLK*/
#define FTFx_ERASE_SECTOR 0x09                    /*!< ERSSCR*/
#define FTFx_PROGRAM_SECTION 0x0B                 /*!< PGMSEC*/
#define FTFx_VERIFY_ALL_BLOCK 0x40                /*!< RD1ALL*/
#define FTFx_READ_ONCE 0x41                       /*!< RDONCE or RDINDEX*/
#define FTFx_PROGRAM_ONCE 0x43                    /*!< PGMONCE or PGMINDEX*/
#define FTFx_ERASE_ALL_BLOCK 0x44                 /*!< ERSALL*/
#define FTFx_SECURITY_BY_PASS 0x45                /*!< VFYKEY*/
#define FTFx_SWAP_CONTROL 0x46                    /*!< SWAP*/
#define FTFx_ERASE_ALL_BLOCK_UNSECURE 0x49        /*!< ERSALLU*/
#define FTFx_VERIFY_ALL_EXECUTE_ONLY_SEGMENT 0x4A /*!< RD1XA*/
#define FTFx_ERASE_ALL_EXECUTE_ONLY_SEGMENT 0x4B  /*!< ERSXA*/
#define FTFx_PROGRAM_PARTITION 0x80               /*!< PGMPART)*/
#define FTFx_SET_FLEXRAM_FUNCTION 0x81            /*!< SETRAM*/
/*@}*/

#if defined(FTFA)
#define FTFx FTFA
#define FTFx_BASE FTFA_BASE
#define FTFx_FSTAT_CCIF_MASK FTFA_FSTAT_CCIF_MASK
#define FTFx_FSTAT_RDCOLERR_MASK FTFA_FSTAT_RDCOLERR_MASK
#define FTFx_FSTAT_ACCERR_MASK FTFA_FSTAT_ACCERR_MASK
#define FTFx_FSTAT_FPVIOL_MASK FTFA_FSTAT_FPVIOL_MASK
#define FTFx_FSTAT_MGSTAT0_MASK FTFA_FSTAT_MGSTAT0_MASK
#define FTFx_FSEC_SEC_MASK FTFA_FSEC_SEC_MASK
#define FTFx_FSEC_KEYEN_MASK FTFA_FSEC_KEYEN_MASK
#if defined(FSL_FEATURE_FLASH_HAS_FLEX_RAM) && FSL_FEATURE_FLASH_HAS_FLEX_RAM
#define FTFx_FCNFG_RAMRDY_MASK FTFA_FCNFG_RAMRDY_MASK
#endif /* FSL_FEATURE_FLASH_HAS_FLEX_RAM */
#if defined(FSL_FEATURE_FLASH_HAS_FLEX_NVM) && FSL_FEATURE_FLASH_HAS_FLEX_NVM
#define FTFx_FCNFG_EEERDY_MASK FTFA_FCNFG_EEERDY_MASK
#endif /* FSL_FEATURE_FLASH_HAS_FLEX_NVM */
#elif defined(FTFE)
#define FTFx FTFE
#define FTFx_BASE FTFE_BASE
#define FTFx_FSTAT_CCIF_MASK FTFE_FSTAT_CCIF_MASK
#define FTFx_FSTAT_RDCOLERR_MASK FTFE_FSTAT_RDCOLERR_MASK
#define FTFx_FSTAT_ACCERR_MASK FTFE_FSTAT_ACCERR_MASK
#define FTFx_FSTAT_FPVIOL_MASK FTFE_FSTAT_FPVIOL_MASK
#define FTFx_FSTAT_MGSTAT0_MASK FTFE_FSTAT_MGSTAT0_MASK
#define FTFx_FSEC_SEC_MASK FTFE_FSEC_SEC_MASK
#define FTFx_FSEC_KEYEN_MASK FTFE_FSEC_KEYEN_MASK
#if defined(FSL_FEATURE_FLASH_HAS_FLEX_RAM) && FSL_FEATURE_FLASH_HAS_FLEX_RAM
#define FTFx_FCNFG_RAMRDY_MASK FTFE_FCNFG_RAMRDY_MASK
#endif /* FSL_FEATURE_FLASH_HAS_FLEX_RAM */
#if defined(FSL_FEATURE_FLASH_HAS_FLEX_NVM) && FSL_FEATURE_FLASH_HAS_FLEX_NVM
#define FTFx_FCNFG_EEERDY_MASK FTFE_FCNFG_EEERDY_MASK
#endif /* FSL_FEATURE_FLASH_HAS_FLEX_NVM */
#elif defined(FTFL)
#define FTFx FTFL
#define FTFx_BASE FTFL_BASE
#define FTFx_FSTAT_CCIF_MASK FTFL_FSTAT_CCIF_MASK
#define FTFx_FSTAT_RDCOLERR_MASK FTFL_FSTAT_RDCOLERR_MASK
#define FTFx_FSTAT_ACCERR_MASK FTFL_FSTAT_ACCERR_MASK
#define FTFx_FSTAT_FPVIOL_MASK FTFL_FSTAT_FPVIOL_MASK
#define FTFx_FSTAT_MGSTAT0_MASK FTFL_FSTAT_MGSTAT0_MASK
#define FTFx_FSEC_SEC_MASK FTFL_FSEC_SEC_MASK
#define FTFx_FSEC_KEYEN_MASK FTFL_FSEC_KEYEN_MASK
#if defined(FSL_FEATURE_FLASH_HAS_FLEX_RAM) && FSL_FEATURE_FLASH_HAS_FLEX_RAM
#define FTFx_FCNFG_RAMRDY_MASK FTFL_FCNFG_RAMRDY_MASK
#endif /* FSL_FEATURE_FLASH_HAS_FLEX_RAM */
#if defined(FSL_FEATURE_FLASH_HAS_FLEX_NVM) && FSL_FEATURE_FLASH_HAS_FLEX_NVM
#define FTFx_FCNFG_EEERDY_MASK FTFL_FCNFG_EEERDY_MASK
#endif /* FSL_FEATURE_FLASH_HAS_FLEX_NVM */
#else
#error "Unknown flash controller"
#endif

enum
{
    kFlashAccessSegmentBase = 256UL,
};

enum
{
    kFlash_AccelerationRam_Size = 0x400
};

enum
{
    kFlashConfig_Start = 0x400,
    kFlashConfig_End = 0x40F
};

/*! @brief program Flash block base address*/
#define PFLASH_BLOCK_BASE 0x00

#define FSL_FEATURE_FTFx_REGION_COUNT (32)

#if FLASH_DRIVER_IS_FLASH_RESIDENT
#define FTFx_REG_ACCESS_TYPE volatile uint8_t *
#define FTFx_REG32_ACCESS_TYPE volatile uint32_t *
#endif /* FLASH_DRIVER_IS_FLASH_RESIDENT */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

#if FLASH_DRIVER_IS_FLASH_RESIDENT
/*! @brief Copy flash_run_command() to RAM*/
static void copy_flash_run_command(uint8_t *flashRunCommand);
/*! @brief Copy flash_cache_clear_command() to RAM*/
static void copy_flash_cache_clear_command(uint8_t *flashCacheClearCommand);
/*! @brief Check whether flash execute-in-ram functions are ready*/
static status_t flash_check_execute_in_ram_function_info(flash_driver_t *driver);
#endif /* FLASH_DRIVER_IS_FLASH_RESIDENT */

/*! @brief Internal function Flash command sequence. Called by driver APIs only*/
static status_t flash_command_sequence(flash_driver_t *driver);

/*! @brief Validates the range and alignment of the given address range.*/
static status_t flash_check_range(flash_driver_t *driver,
                                  uint32_t startAddress,
                                  uint32_t lengthInBytes,
                                  uint32_t alignmentBaseline);
/*! @brief Gets the right address, sector and block size of current flash type which is indicated by address.*/
static status_t flash_get_matched_operation_info(flash_driver_t *driver,
                                                 uint32_t address,
                                                 flash_operation_info_t *info);
/*! @brief Validates the given user key for flash erase APIs.*/
static status_t flash_check_user_key(uint32_t key);

#if FLASH_SSD_IS_FLEXNVM_ENABLED
/*! @brief Updates FlexNVM memory partition status according to data flash 0 IFR.*/
static status_t flash_update_flexnvm_memory_partition_status(flash_driver_t *driver);
#endif /* FLASH_SSD_IS_FLEXNVM_ENABLED */

#if defined(FSL_FEATURE_FLASH_HAS_READ_RESOURCE_CMD) && FSL_FEATURE_FLASH_HAS_READ_RESOURCE_CMD
/*! @brief Validates the range of the given resource address.*/
static status_t flash_check_resource_range(uint32_t start, uint32_t lengthInBytes, flash_read_resource_option_t option);
#endif /* FSL_FEATURE_FLASH_HAS_READ_RESOURCE_CMD */

#if defined(FSL_FEATURE_FLASH_HAS_SWAP_CONTROL_CMD) && FSL_FEATURE_FLASH_HAS_SWAP_CONTROL_CMD
/*! @brief Validates the gived swap control option.*/
static status_t flash_check_swap_control_option(flash_swap_control_option_t option);
#endif /* FSL_FEATURE_FLASH_HAS_SWAP_CONTROL_CMD */

#if defined(FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP) && FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP
/*! @brief Validates the gived address to see if it is equal to swap indicator address in pflash swap IFR.*/
static status_t flash_validate_swap_indicator_address(flash_driver_t *driver, uint32_t address);
#endif /* FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP */

#if defined(FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD) && FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD
/*! @brief Validates the gived flexram function option.*/
static inline status_t flasn_check_flexram_function_option_range(flash_flexram_function_option_t option);
#endif /* FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD */

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile uint32_t *const restrict kFCCOBx =
#if defined(FSL_FEATURE_FLASH_IS_FTFA) && FSL_FEATURE_FLASH_IS_FTFA
    (volatile uint32_t *)&FTFA->FCCOB3;
#elif defined(FSL_FEATURE_FLASH_IS_FTFE) && FSL_FEATURE_FLASH_IS_FTFE
    (volatile uint32_t *)&FTFE->FCCOB3;
#elif defined(FSL_FEATURE_FLASH_IS_FTFL) && FSL_FEATURE_FLASH_IS_FTFL
    (volatile uint32_t *)&FTFL->FCCOB3;
#else
#error "Unknown flash controller"
#endif

volatile uint32_t *const restrict kFPROT =
#if defined(FSL_FEATURE_FLASH_IS_FTFA) && FSL_FEATURE_FLASH_IS_FTFA
    (volatile uint32_t *)&FTFA->FPROT3;
#elif defined(FSL_FEATURE_FLASH_IS_FTFE) && FSL_FEATURE_FLASH_IS_FTFE
    (volatile uint32_t *)&FTFE->FPROT3;
#elif defined(FSL_FEATURE_FLASH_IS_FTFL) && FSL_FEATURE_FLASH_IS_FTFL
    (volatile uint32_t *)&FTFL->FPROT3;
#else
#error "Unknown flash controller"
#endif

#if FLASH_DRIVER_IS_FLASH_RESIDENT
/*! @brief A function pointer used to point to relocated flash_run_command() */
static void (*callFlashRunCommand)(FTFx_REG_ACCESS_TYPE ftfx_fstat);
/*! @brief A function pointer used to point to relocated flash_cache_clear_command() */
static void (*callFlashCacheClearCommand)(FTFx_REG32_ACCESS_TYPE ftfx_reg);
#endif /* FLASH_DRIVER_IS_FLASH_RESIDENT */

#if (FLASH_DRIVER_IS_FLASH_RESIDENT && !FLASH_DRIVER_IS_EXPORTED)
/*! @brief A static buffer used to hold flash_run_command() */
static uint8_t s_flashRunCommand[kFlashExecuteInRamFunction_MaxSize];
/*! @brief A static buffer used to hold flash_cache_clear_command() */
static uint8_t s_flashCacheClearCommand[kFlashExecuteInRamFunction_MaxSize];
static flash_execute_in_ram_function_info_t s_flashExecuteInRamFunctionInfo = {
    .activeFunctionCount = 0, .flashRunCommand = s_flashRunCommand, .flashCacheClearCommand = s_flashCacheClearCommand,
};
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
    8,    /* 0x0 - 8192, 8KB */
    16,   /* 0x1 - 16384, 16KB */
    24,   /* 0x2 - 24576, 24KB */
    32,   /* 0x3 - 32768, 32KB */
    48,   /* 0x4 - 49152, 48KB */
    64,   /* 0x5 - 65536, 64KB */
    96,   /* 0x6 - 98304, 96KB */
    128,  /* 0x7 - 131072, 128KB */
    192,  /* 0x8 - 196608, 192KB */
    256,  /* 0x9 - 262144, 256KB */
    384,  /* 0xa - 393216, 384KB */
    512,  /* 0xb - 524288, 512KB */
    768,  /* 0xc - 786432, 768KB */
    1024, /* 0xd - 1048576, 1MB */
    1536, /* 0xe - 1572864, 1.5MB */
    /* 2048,  0xf - 2097152, 2MB */
};

/*******************************************************************************
 * Code
 ******************************************************************************/

/* See fsl_flash.h for documentation of this function. */
status_t FLASH_Init(flash_driver_t *driver)
{
    uint32_t flashDensity;

    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* calculate the flash density from SIM_FCFG1.PFSIZE */
    uint8_t pfsize = (SIM->FCFG1 & SIM_FCFG1_PFSIZE_MASK) >> SIM_FCFG1_PFSIZE_SHIFT;
    /* PFSIZE=0xf means that on customer parts the IFR was not correctly programmed.
     * We just use the pre-defined flash size in feature file here to support pre-production parts */
    if (pfsize == 0xf)
    {
        flashDensity = FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT * FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE;
    }
    else
    {
        flashDensity = ((uint32_t)kPFlashDensities[pfsize]) << 10;
    }

    /* fill out a few of the structure members */
    driver->PFlashBlockBase = PFLASH_BLOCK_BASE;
    driver->PFlashTotalSize = flashDensity;
    driver->PFlashBlockCount = FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT;
    driver->PFlashSectorSize = FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE;

#if defined(FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL) && FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
    driver->PFlashAccessSegmentSize = kFlashAccessSegmentBase << FTFx->FACSS;
    driver->PFlashAccessSegmentCount = FTFx->FACSN;
#else
    driver->PFlashAccessSegmentSize = 0;
    driver->PFlashAccessSegmentCount = 0;
#endif /* FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL */

    driver->PFlashCallback = NULL;

/* copy required flash commands to RAM */
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

/* See fsl_flash.h for documentation of this function. */
status_t FLASH_SetCallback(flash_driver_t *driver, flash_callback_t callback)
{
    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    driver->PFlashCallback = callback;

    return kStatus_FLASH_Success;
}

/* See fsl_flash.h for documentation of this function. */
#if FLASH_DRIVER_IS_FLASH_RESIDENT
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
#endif /* FLASH_DRIVER_IS_FLASH_RESIDENT */

/* See fsl_flash.h for documentation of this function. */
status_t FLASH_EraseAll(flash_driver_t *driver, uint32_t key)
{
    status_t returnCode;

    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* preparing passing parameter to erase all flash blocks */
    kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(FTFx_ERASE_ALL_BLOCK, 0xFFFFFFU);

    /* Validate the user key */
    returnCode = flash_check_user_key(key);
    if (returnCode)
    {
        return returnCode;
    }

    /* calling flash command sequence function to execute the command */
    returnCode = flash_command_sequence(driver);

    flash_cache_clear(driver);

#if FLASH_SSD_IS_FLEXNVM_ENABLED
    /* Data flash IFR will be erased by erase all command, so we need to
     *  update FlexNVM memory partition status synchronously */
    if (returnCode == kStatus_FLASH_Success)
    {
        returnCode = flash_update_flexnvm_memory_partition_status(driver);
    }
#endif

    return returnCode;
}

/* See fsl_flash.h for documentation of this function. */
status_t FLASH_Erase(flash_driver_t *driver, uint32_t start, uint32_t lengthInBytes, uint32_t key)
{
    uint32_t sectorSize;
    flash_operation_info_t flashInfo;
    uint32_t endAddress;      /* storing end address */
    uint32_t numberOfSectors; /* number of sectors calculated by endAddress */

    /* Check the supplied address range. */
    status_t returnCode =
        flash_check_range(driver, start, lengthInBytes, COMMON_FEATURE_FLASH_SECTOR_CMD_ADDRESS_ALIGMENT);
    if (returnCode)
    {
        return returnCode;
    }

    flash_get_matched_operation_info(driver, start, &flashInfo);
    start = flashInfo.convertedAddress;
    sectorSize = flashInfo.activeSectorSize;

    /* calculating Flash end address */
    endAddress = start + lengthInBytes - 1;

    /* re-calculate the endAddress and align it to the start of the next sector
     * which will be used in the comparison below */
    if (endAddress % sectorSize)
    {
        numberOfSectors = endAddress / sectorSize + 1;
        endAddress = numberOfSectors * sectorSize - 1;
    }

    /* the start address will increment to the next sector address
     * until it reaches the endAdddress */
    while (start <= endAddress)
    {
        /* preparing passing parameter to erase a flash block */
        kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(FTFx_ERASE_SECTOR, start);

        /* Validate the user key */
        returnCode = flash_check_user_key(key);
        if (returnCode)
        {
            return returnCode;
        }

        /* calling flash command sequence function to execute the command */
        returnCode = flash_command_sequence(driver);

        /* calling flash callback function if it is available */
        if (driver->PFlashCallback)
        {
            driver->PFlashCallback();
        }

        /* checking the success of command execution */
        if (kStatus_FLASH_Success != returnCode)
        {
            break;
        }
        else
        {
            /* Increment to the next sector */
            start += sectorSize;
        }
    }

    flash_cache_clear(driver);

    return (returnCode);
}

#if defined(FSL_FEATURE_FLASH_HAS_ERASE_ALL_BLOCKS_UNSECURE_CMD) && FSL_FEATURE_FLASH_HAS_ERASE_ALL_BLOCKS_UNSECURE_CMD
/* See flash.h for documentation of this function. */
status_t FLASH_EraseAllUnsecure(flash_driver_t *driver, uint32_t key)
{
    status_t returnCode;

    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* Prepare passing parameter to erase all flash blocks (unsecure). */
    kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(FTFx_ERASE_ALL_BLOCK_UNSECURE, 0xFFFFFFU);

    /* Validate the user key */
    returnCode = flash_check_user_key(key);
    if (returnCode)
    {
        return returnCode;
    }

    /* calling flash command sequence function to execute the command */
    returnCode = flash_command_sequence(driver);

    flash_cache_clear(driver);

#if FLASH_SSD_IS_FLEXNVM_ENABLED
    /* Data flash IFR will be erased by erase all unsecure command, so we need to
     *  update FlexNVM memory partition status synchronously */
    if (returnCode == kStatus_FLASH_Success)
    {
        returnCode = flash_update_flexnvm_memory_partition_status(driver);
    }
#endif

    return returnCode;
}
#endif /* FSL_FEATURE_FLASH_HAS_ERASE_ALL_BLOCKS_UNSECURE_CMD */

/* See flash.h for documentation of this function. */
status_t FLASH_EraseAllExecuteOnlySegments(flash_driver_t *driver, uint32_t key)
{
    status_t returnCode;

    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* preparing passing parameter to erase all execute-only segments
     * 1st element for the FCCOB register */
    kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(FTFx_ERASE_ALL_EXECUTE_ONLY_SEGMENT, 0xFFFFFFU);

    /* Validate the user key */
    returnCode = flash_check_user_key(key);
    if (returnCode)
    {
        return returnCode;
    }

    /* calling flash command sequence function to execute the command */
    returnCode = flash_command_sequence(driver);

    flash_cache_clear(driver);

    return returnCode;
}

/* See fsl_flash.h for documentation of this function. */
status_t FLASH_Program(flash_driver_t *driver, uint32_t start, uint32_t *src, uint32_t lengthInBytes)
{
    status_t returnCode;
    flash_operation_info_t flashInfo;

    if (src == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* Check the supplied address range. */
    returnCode = flash_check_range(driver, start, lengthInBytes, COMMON_FEATURE_FLASH_BLOCK_WRITE_UNIT_SIZE);
    if (returnCode)
    {
        return returnCode;
    }

    flash_get_matched_operation_info(driver, start, &flashInfo);
    start = flashInfo.convertedAddress;

    while (lengthInBytes > 0)
    {
        /* preparing passing parameter to program the flash block */
        kFCCOBx[1] = *src++;
#if (4 == COMMON_FEATURE_FLASH_BLOCK_WRITE_UNIT_SIZE)
        kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(FTFx_PROGRAM_LONGWORD, start);
#elif(8 == COMMON_FEATURE_FLASH_BLOCK_WRITE_UNIT_SIZE)
        kFCCOBx[2] = *src++;
        kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(FTFx_PROGRAM_PHRASE, start);
#else
#error "Untreated program unit size"
#endif

        /* calling flash command sequence function to execute the command */
        returnCode = flash_command_sequence(driver);

        /* calling flash callback function if it is available */
        if (driver->PFlashCallback)
        {
            driver->PFlashCallback();
        }

        /* checking for the success of command execution */
        if (kStatus_FLASH_Success != returnCode)
        {
            break;
        }
        else
        {
            /* update start address for next iteration */
            start += COMMON_FEATURE_FLASH_BLOCK_WRITE_UNIT_SIZE;

            /* update lengthInBytes for next iteration */
            lengthInBytes -= COMMON_FEATURE_FLASH_BLOCK_WRITE_UNIT_SIZE;
        }
    }

    flash_cache_clear(driver);

    return (returnCode);
}

/* See fsl_flash.h for documentation of this function. */
status_t FLASH_ProgramOnce(flash_driver_t *driver, uint32_t index, uint32_t *src, uint32_t lengthInBytes)
{
    status_t returnCode;

    if ((driver == NULL) || (src == NULL))
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* pass paramters to FTFx */
    kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_1_2(FTFx_PROGRAM_ONCE, index, 0xFFFFU);

    kFCCOBx[1] = *src;

/* Note: Have to seperate the first index from the rest if it equals 0
 * to avoid a pointless comparison of unsigned int to 0 compiler warning */
#if FLASH_PROGRAM_ONCE_IS_8BYTES_UNIT_SUPPORT
#if FLASH_PROGRAM_ONCE_IS_4BYTES_UNIT_SUPPORT
    if (((index == FLASH_PROGRAM_ONCE_MIN_ID_8BYTES) ||
         /* Range check */
         ((index >= FLASH_PROGRAM_ONCE_MIN_ID_8BYTES + 1) && (index <= FLASH_PROGRAM_ONCE_MAX_ID_8BYTES))) &&
        (lengthInBytes == 8))
#endif /* FLASH_PROGRAM_ONCE_IS_4BYTES_UNIT_SUPPORT */
    {
        kFCCOBx[2] = *(src + 1);
    }
#endif /* FLASH_PROGRAM_ONCE_IS_8BYTES_UNIT_SUPPORT */

    /* calling flash command sequence function to execute the command */
    returnCode = flash_command_sequence(driver);

    flash_cache_clear(driver);

    return returnCode;
}

#if defined(FSL_FEATURE_FLASH_HAS_PROGRAM_SECTION_CMD) && FSL_FEATURE_FLASH_HAS_PROGRAM_SECTION_CMD
/* See fsl_flash.h for documentation of this function. */
status_t FLASH_ProgramSection(flash_driver_t *driver, uint32_t start, uint32_t *src, uint32_t lengthInBytes)
{
    status_t returnCode;
    uint32_t sectorSize;
    flash_operation_info_t flashInfo;
#if defined(FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD) && FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD
    bool needSwitchFlexRamMode = false;
#endif /* FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD */

    if (src == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* Check the supplied address range. */
    returnCode = flash_check_range(driver, start, lengthInBytes, COMMON_FEATURE_FLASH_SECTION_CMD_ADDRESS_ALIGMENT);
    if (returnCode)
    {
        return returnCode;
    }

    flash_get_matched_operation_info(driver, start, &flashInfo);
    start = flashInfo.convertedAddress;
    sectorSize = flashInfo.activeSectorSize;

#if defined(FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD) && FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD
    /* Switch function of FlexRAM if needed */
    if (!(FTFx->FCNFG & FTFx_FCNFG_RAMRDY_MASK))
    {
        needSwitchFlexRamMode = true;

        returnCode = FLASH_SetFlexramFunction(driver, kFlash_FlexRAM_Available_As_RAM);
        if (returnCode != kStatus_FLASH_Success)
        {
            return kStatus_FLASH_SetFlexramAsRamError;
        }
    }
#endif /* FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD */

    while (lengthInBytes > 0)
    {
        /* Make sure the write operation doesn't span two sectors */
        uint32_t endAddressOfCurrentSector = ALIGN_UP(start, sectorSize);
        uint32_t lengthTobeProgrammedOfCurrentSector;
        uint32_t currentOffset = 0;

        if (endAddressOfCurrentSector == start)
        {
            endAddressOfCurrentSector += sectorSize;
        }

        if (lengthInBytes + start > endAddressOfCurrentSector)
        {
            lengthTobeProgrammedOfCurrentSector = endAddressOfCurrentSector - start;
        }
        else
        {
            lengthTobeProgrammedOfCurrentSector = lengthInBytes;
        }

        /* Program Current Sector */
        while (lengthTobeProgrammedOfCurrentSector > 0)
        {
            /* Make sure the program size doesn't exceeds Acceleration RAM size */
            uint32_t programSizeOfCurrentPass;
            uint32_t numberOfPhases;

            if (lengthTobeProgrammedOfCurrentSector > kFlash_AccelerationRam_Size)
            {
                programSizeOfCurrentPass = kFlash_AccelerationRam_Size;
            }
            else
            {
                programSizeOfCurrentPass = lengthTobeProgrammedOfCurrentSector;
            }

            /* Copy data to FlexRAM */
            memcpy((void *)FSL_FEATURE_FLASH_FLEX_RAM_START_ADDRESS, src + currentOffset / 4, programSizeOfCurrentPass);
            /* Set start address of the data to be programmed */
            kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(FTFx_PROGRAM_SECTION, start + currentOffset);
            /* Set program size in terms of  COMMON_FEATURE_FLASH_SECTION_CMD_ADDRESS_ALIGMENT */
            numberOfPhases = programSizeOfCurrentPass / COMMON_FEATURE_FLASH_SECTION_CMD_ADDRESS_ALIGMENT;

            kFCCOBx[1] = BYTES_JOIN_TO_WORD_2_2(numberOfPhases, 0xFFFFU);

            /* Peform command sequence */
            returnCode = flash_command_sequence(driver);

            /* calling flash callback function if it is available */
            if (driver->PFlashCallback)
            {
                driver->PFlashCallback();
            }

            if (returnCode != kStatus_FLASH_Success)
            {
                flash_cache_clear(driver);
                return returnCode;
            }

            lengthTobeProgrammedOfCurrentSector -= programSizeOfCurrentPass;
            currentOffset += programSizeOfCurrentPass;
        }

        src += currentOffset / 4;
        start += currentOffset;
        lengthInBytes -= currentOffset;
    }

    flash_cache_clear(driver);

#if defined(FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD) && FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD
    /* Restore function of FlexRAM if needed. */
    if (needSwitchFlexRamMode)
    {
        returnCode = FLASH_SetFlexramFunction(driver, kFlash_FlexRAM_Available_For_EEPROM);
        if (returnCode != kStatus_FLASH_Success)
        {
            return kStatus_FLASH_RecoverFlexramAsEepromError;
        }
    }
#endif /* FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD */

    return returnCode;
}
#endif /* FSL_FEATURE_FLASH_HAS_PROGRAM_SECTION_CMD */

#if FLASH_SSD_IS_FLEXNVM_ENABLED
/* See fsl_flash.h for documentation of this function. */
status_t FLASH_EepromWrite(flash_driver_t *driver, uint32_t start, uint8_t *src, uint32_t lengthInBytes)
{
    status_t returnCode;
    bool needSwitchFlexRamMode = false;

    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* Validates the range of the given address */
    if ((start < driver->FlexRAMBlockBase) ||
        ((start + lengthInBytes) > (driver->FlexRAMBlockBase + driver->EEpromTotalSize)))
    {
        return kStatus_FLASH_AddressError;
    }

    returnCode = kStatus_FLASH_Success;

    /* Switch function of FlexRAM if needed */
    if (!(FTFx->FCNFG & FTFx_FCNFG_EEERDY_MASK))
    {
        needSwitchFlexRamMode = true;

        returnCode = FLASH_SetFlexramFunction(driver, kFlash_FlexRAM_Available_For_EEPROM);
        if (returnCode != kStatus_FLASH_Success)
        {
            return kStatus_FLASH_SetFlexramAsEepromError;
        }
    }

    /* Write data to FlexRAM when it is used as EEPROM emulator */
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
        /* Wait till EEERDY bit is set */
        while (!(FTFx->FCNFG & FTFx_FCNFG_EEERDY_MASK))
        {
        }

        /* Check for protection violation error */
        if (FTFx->FSTAT & FTFx_FSTAT_FPVIOL_MASK)
        {
            return kStatus_FLASH_ProtectionViolation;
        }
    }

    /* Switch function of FlexRAM if needed */
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
#endif /* FLASH_SSD_IS_FLEXNVM_ENABLED */

#if defined(FSL_FEATURE_FLASH_HAS_READ_RESOURCE_CMD) && FSL_FEATURE_FLASH_HAS_READ_RESOURCE_CMD
/* See fsl_flash.h for documentation of this function. */
status_t FLASH_ReadResource(
    flash_driver_t *driver, uint32_t start, uint32_t *dst, uint32_t lengthInBytes, flash_read_resource_option_t option)
{
    status_t returnCode;

    if ((driver == NULL) || (dst == NULL))
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* Check the supplied address range. */
    returnCode = flash_check_resource_range(start, lengthInBytes, option);
    if (returnCode != kStatus_FLASH_Success)
    {
        return returnCode;
    }

    while (lengthInBytes > 0)
    {
        /* preparing passing parameter */
        kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(FTFx_READ_RESOURCE, start);
#if (COMMON_FEATURE_FLASH_RESOURCE_CMD_ADDRESS_ALIGMENT == 8)
        kFCCOBx[1] = BYTES_JOIN_TO_WORD_1_3(option, 0xFFFFFFU);
#else
        kFCCOBx[2] = BYTES_JOIN_TO_WORD_1_3(option, 0xFFFFFFU);
#endif
        /* calling flash command sequence function to execute the command */
        returnCode = flash_command_sequence(driver);

        if (kStatus_FLASH_Success != returnCode)
        {
            break;
        }

        /* fetch data */
        *dst++ = kFCCOBx[1];
#if (COMMON_FEATURE_FLASH_RESOURCE_CMD_ADDRESS_ALIGMENT > 4)
        *dst++ = kFCCOBx[2];
#endif
        /* update start address for next iteration */
        start += kFlashReadResource_UnitInBytes;
        /* update lengthInBytes for next iteration */
        lengthInBytes -= kFlashReadResource_UnitInBytes;
    }

    return (returnCode);
}
#endif /* FSL_FEATURE_FLASH_HAS_READ_RESOURCE_CMD */

/* See fsl_flash.h for documentation of this function. */
status_t FLASH_ReadOnce(flash_driver_t *driver, uint32_t index, uint32_t *dst, uint32_t lengthInBytes)
{
    status_t returnCode;

    if ((driver == NULL) || (dst == NULL))
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* pass paramters to FTFx */
    kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_1_2(FTFx_READ_ONCE, index, 0xFFFFU);

    /* calling flash command sequence function to execute the command */
    returnCode = flash_command_sequence(driver);

    if (kStatus_FLASH_Success == returnCode)
    {
        *dst = kFCCOBx[1];
/* Note: Have to seperate the first index from the rest if it equals 0
 *       to avoid a pointless comparison of unsigned int to 0 compiler warning */
#if FLASH_PROGRAM_ONCE_IS_8BYTES_UNIT_SUPPORT
#if FLASH_PROGRAM_ONCE_IS_4BYTES_UNIT_SUPPORT
        if (((index == FLASH_PROGRAM_ONCE_MIN_ID_8BYTES) ||
             /* Range check */
             ((index >= FLASH_PROGRAM_ONCE_MIN_ID_8BYTES + 1) && (index <= FLASH_PROGRAM_ONCE_MAX_ID_8BYTES))) &&
            (lengthInBytes == 8))
#endif /* FLASH_PROGRAM_ONCE_IS_4BYTES_UNIT_SUPPORT */
        {
            *(dst + 1) = kFCCOBx[2];
        }
#endif /* FLASH_PROGRAM_ONCE_IS_8BYTES_UNIT_SUPPORT */
    }

    return returnCode;
}

/* See fsl_flash.h for documentation of this function. */
status_t FLASH_GetSecurityState(flash_driver_t *driver, flash_security_state_t *state)
{
    /* store data read from flash register */
    uint8_t registerValue;

    if ((driver == NULL) || (state == NULL))
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* Get flash security register value */
    registerValue = FTFx->FSEC;

    /* check the status of the flash security bits in the security register */
    if (FLASH_SECURITY_STATE_UNSECURED == (registerValue & FTFx_FSEC_SEC_MASK))
    {
        /* Flash in unsecured state */
        *state = kFlashNotSecure;
    }
    else
    {
        /* Flash in secured state
         * check for backdoor key security enable bit */
        if (FLASH_SECURITY_STATE_KEYEN == (registerValue & FTFx_FSEC_KEYEN_MASK))
        {
            /* Backdoor key security enabled */
            *state = kFlashSecureBackdoorEnabled;
        }
        else
        {
            /* Backdoor key security disabled */
            *state = kFlashSecureBackdoorDisabled;
        }
    }

    return (kStatus_FLASH_Success);
}

/* See fsl_flash.h for documentation of this function. */
status_t FLASH_SecurityBypass(flash_driver_t *driver, const uint8_t *backdoorKey)
{
    uint8_t registerValue; /* registerValue */
    status_t returnCode;   /* return code variable */

    if ((driver == NULL) || (backdoorKey == NULL))
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* set the default return code as kStatus_Success */
    returnCode = kStatus_FLASH_Success;

    /* Get flash security register value */
    registerValue = FTFx->FSEC;

    /* Check to see if flash is in secure state (any state other than 0x2)
     * If not, then skip this since flash is not secure */
    if (0x02 != (registerValue & 0x03))
    {
        /* preparing passing parameter to erase a flash block */
        kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(FTFx_SECURITY_BY_PASS, 0xFFFFFFU);
        kFCCOBx[1] = BYTES_JOIN_TO_WORD_1_1_1_1(backdoorKey[0], backdoorKey[1], backdoorKey[2], backdoorKey[3]);
        kFCCOBx[2] = BYTES_JOIN_TO_WORD_1_1_1_1(backdoorKey[4], backdoorKey[5], backdoorKey[6], backdoorKey[7]);

        /* calling flash command sequence function to execute the command */
        returnCode = flash_command_sequence(driver);
    }

    return (returnCode);
}

/* See fsl_flash.h for documentation of this function. */
status_t FLASH_VerifyEraseAll(flash_driver_t *driver, flash_margin_value_t margin)
{
    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* preparing passing parameter to verify all block command */
    kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_1_2(FTFx_VERIFY_ALL_BLOCK, margin, 0xFFFFU);

    /* calling flash command sequence function to execute the command */
    return flash_command_sequence(driver);
}

/* See fsl_flash.h for documentation of this function. */
status_t FLASH_VerifyErase(flash_driver_t *driver, uint32_t start, uint32_t lengthInBytes, flash_margin_value_t margin)
{
    /* Check arguments. */
    uint32_t blockSize;
    flash_operation_info_t flashInfo;
    uint32_t nextBlockStartAddress;
    uint32_t remainingBytes;

    status_t returnCode =
        flash_check_range(driver, start, lengthInBytes, COMMON_FEATURE_FLASH_SECTION_CMD_ADDRESS_ALIGMENT);
    if (returnCode)
    {
        return returnCode;
    }

    flash_get_matched_operation_info(driver, start, &flashInfo);
    start = flashInfo.convertedAddress;
    blockSize = flashInfo.activeBlockSize;

    nextBlockStartAddress = ALIGN_UP(start, blockSize);
    if (nextBlockStartAddress == start)
    {
        nextBlockStartAddress += blockSize;
    }

    remainingBytes = lengthInBytes;

    while (remainingBytes)
    {
        uint32_t numberOfPhrases;
        uint32_t verifyLength = nextBlockStartAddress - start;
        if (verifyLength > remainingBytes)
        {
            verifyLength = remainingBytes;
        }

        numberOfPhrases = verifyLength / COMMON_FEATURE_FLASH_SECTION_CMD_ADDRESS_ALIGMENT;

        /* Fill in verify section command parameters. */
        kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(FTFx_VERIFY_SECTION, start);
        kFCCOBx[1] = BYTES_JOIN_TO_WORD_2_1_1(numberOfPhrases, margin, 0xFFU);

        /* calling flash command sequence function to execute the command */
        returnCode = flash_command_sequence(driver);
        if (returnCode)
        {
            return returnCode;
        }

        remainingBytes -= verifyLength;
        start += verifyLength;
        nextBlockStartAddress += blockSize;
    }

    return kStatus_FLASH_Success;
}

/* See fsl_flash.h for documentation of this function. */
status_t FLASH_VerifyProgram(flash_driver_t *driver,
                             uint32_t start,
                             uint32_t lengthInBytes,
                             const uint32_t *expectedData,
                             flash_margin_value_t margin,
                             uint32_t *failedAddress,
                             uint32_t *failedData)
{
    status_t returnCode;
    flash_operation_info_t flashInfo;

    if (expectedData == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    returnCode = flash_check_range(driver, start, lengthInBytes, COMMON_FEATURE_FLASH_CHECK_CMD_ADDRESS_ALIGMENT);
    if (returnCode)
    {
        return returnCode;
    }

    flash_get_matched_operation_info(driver, start, &flashInfo);
    start = flashInfo.convertedAddress;

    while (lengthInBytes)
    {
        /* preparing passing parameter to program check the flash block */
        kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(FTFx_PROGRAM_CHECK, start);
        kFCCOBx[1] = BYTES_JOIN_TO_WORD_1_3(margin, 0xFFFFFFU);
        kFCCOBx[2] = *expectedData;

        /* calling flash command sequence function to execute the command */
        returnCode = flash_command_sequence(driver);

        /* checking for the success of command execution */
        if (kStatus_FLASH_Success != returnCode)
        {
            if (failedAddress)
            {
                *failedAddress = start;
            }
            if (failedData)
            {
                *failedData = 0;
            }
            break;
        }

        lengthInBytes -= COMMON_FEATURE_FLASH_CHECK_CMD_ADDRESS_ALIGMENT;
        expectedData += COMMON_FEATURE_FLASH_CHECK_CMD_ADDRESS_ALIGMENT / sizeof(*expectedData);
        start += COMMON_FEATURE_FLASH_CHECK_CMD_ADDRESS_ALIGMENT;
    }

    return (returnCode);
}

/* See flash.h for documentation of this function. */
status_t FLASH_VerifyEraseAllExecuteOnlySegments(flash_driver_t *driver, flash_margin_value_t margin)
{
    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* preparing passing parameter to verify erase all execute-only segments command */
    kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_1_2(FTFx_VERIFY_ALL_EXECUTE_ONLY_SEGMENT, margin, 0xFFFFU);

    /* calling flash command sequence function to execute the command */
    return flash_command_sequence(driver);
}

/* See fsl_flash.h for documentation of this function. */
status_t FLASH_IsProtected(flash_driver_t *driver,
                           uint32_t start,
                           uint32_t lengthInBytes,
                           flash_protection_state_t *protection_state)
{
    uint32_t endAddress;           /* end address for protection check */
    uint32_t protectionRegionSize; /* size of flash protection region */
    uint32_t regionCheckedCounter; /* increments each time the flash address was checked for
                                    * protection status */
    uint32_t regionCounter;        /* incrementing variable used to increment through the flash
                                    * protection regions */
    uint32_t protectStatusCounter; /* increments each time a flash region was detected as protected */

    uint8_t flashRegionProtectStatus[FSL_FEATURE_FTFx_REGION_COUNT]; /* array of the protection status for each
                                                                      * protection region */
    uint32_t flashRegionAddress[FSL_FEATURE_FTFx_REGION_COUNT + 1];  /* array of the start addresses for each flash
                                                                      * protection region. Note this is REGION_COUNT+1
                                                                      * due to requiring the next start address after
                                                                      * the end of flash for loop-check purposes below */
    status_t returnCode;

    if (protection_state == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* Check the supplied address range. */
    returnCode = flash_check_range(driver, start, lengthInBytes, FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE);
    if (returnCode)
    {
        return returnCode;
    }

    /* calculating Flash end address */
    endAddress = start + lengthInBytes;

    /* Calculate the size of the flash protection region
     * If the flash density is > 32KB, then protection region is 1/32 of total flash density
     * Else if flash density is < 32KB, then flash protection region is set to 1KB */
    if (driver->PFlashTotalSize > 32 * 1024)
    {
        protectionRegionSize = (driver->PFlashTotalSize) / FSL_FEATURE_FTFx_REGION_COUNT;
    }
    else
    {
        protectionRegionSize = 1024;
    }

    /* populate the flashRegionAddress array with the start address of each flash region */
    regionCounter = 0; /* make sure regionCounter is initialized to 0 first */

    /* populate up to 33rd element of array, this is the next address after end of flash array */
    while (regionCounter <= FSL_FEATURE_FTFx_REGION_COUNT)
    {
        flashRegionAddress[regionCounter] = driver->PFlashBlockBase + protectionRegionSize * regionCounter;
        regionCounter++;
    }

    /* populate flashRegionProtectStatus array with status information
     * Protection status for each region is stored in the FPROT[3:0] registers
     * Each bit represents one region of flash
     * 4 registers * 8-bits-per-register = 32-bits (32-regions)
     * The convention is:
     * FPROT3[bit 0] is the first protection region (start of flash memory)
     * FPROT0[bit 7] is the last protection region (end of flash memory)
     * regionCounter is used to determine which FPROT[3:0] register to check for protection status
     * Note: FPROT=1 means NOT protected, FPROT=0 means protected */
    regionCounter = 0; /* make sure regionCounter is initialized to 0 first */
    while (regionCounter < FSL_FEATURE_FTFx_REGION_COUNT)
    {
        if (regionCounter < 8)
        {
            flashRegionProtectStatus[regionCounter] = ((FTFx->FPROT3) >> regionCounter) & (0x01u);
        }
        else if ((regionCounter >= 8) && (regionCounter < 16))
        {
            flashRegionProtectStatus[regionCounter] = ((FTFx->FPROT2) >> (regionCounter - 8)) & (0x01u);
        }
        else if ((regionCounter >= 16) && (regionCounter < 24))
        {
            flashRegionProtectStatus[regionCounter] = ((FTFx->FPROT1) >> (regionCounter - 16)) & (0x01u);
        }
        else
        {
            flashRegionProtectStatus[regionCounter] = ((FTFx->FPROT0) >> (regionCounter - 24)) & (0x01u);
        }
        regionCounter++;
    }

    /* loop through the flash regions and check
     * desired flash address range for protection status
     * loop stops when it is detected that start has exceeded the endAddress */
    regionCounter = 0; /* make sure regionCounter is initialized to 0 first */
    regionCheckedCounter = 0;
    protectStatusCounter = 0; /* make sure protectStatusCounter is initialized to 0 first */
    while (start < endAddress)
    {
        /* check to see if the address falls within this protection region
         * Note that if the entire flash is to be checked, the last protection
         * region checked would consist of the last protection start address and
         * the start address following the end of flash */
        if ((start >= flashRegionAddress[regionCounter]) && (start < flashRegionAddress[regionCounter + 1]))
        {
            /* increment regionCheckedCounter to indicate this region was checked */
            regionCheckedCounter++;

            /* check the protection status of this region
             * Note: FPROT=1 means NOT protected, FPROT=0 means protected */
            if (!flashRegionProtectStatus[regionCounter])
            {
                /* increment protectStatusCounter to indicate this region is protected */
                protectStatusCounter++;
            }
            start += protectionRegionSize; /* increment to an address within the next region */
        }
        regionCounter++; /* increment regionCounter to check for the next flash protection region */
    }

    /* if protectStatusCounter == 0, then no region of the desired flash region is protected */
    if (protectStatusCounter == 0)
    {
        *protection_state = kFlashProtection_Unprotected;
    }
    /* if protectStatusCounter == regionCheckedCounter, then each region checked was protected */
    else if (protectStatusCounter == regionCheckedCounter)
    {
        *protection_state = kFlashProtection_Protected;
    }
    /* if protectStatusCounter != regionCheckedCounter, then protection status is mixed
     * In other words, some regions are protected while others are unprotected */
    else
    {
        *protection_state = kFlashProtection_Mixed;
    }

    return (returnCode);
}

/* See fsl_flash.h for documentation of this function. */
status_t FLASH_IsExecuteOnly(flash_driver_t *driver,
                             uint32_t start,
                             uint32_t lengthInBytes,
                             flash_execute_only_access_state_t *access_state)
{
    status_t returnCode;

    if (access_state == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* Check the supplied address range. */
    returnCode = flash_check_range(driver, start, lengthInBytes, FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE);
    if (returnCode)
    {
        return returnCode;
    }

#if defined(FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL) && FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL
    {
        uint32_t executeOnlySegmentCounter = 0;

        /* calculating end address */
        uint32_t endAddress = start + lengthInBytes;

        /* Aligning start address and end address */
        uint32_t alignedStartAddress = ALIGN_DOWN(start, driver->PFlashAccessSegmentSize);
        uint32_t alignedEndAddress = ALIGN_UP(endAddress, driver->PFlashAccessSegmentSize);

        uint32_t segmentIndex = 0;
        uint32_t maxSupportedExecuteOnlySegmentCount =
            (alignedEndAddress - alignedStartAddress) / driver->PFlashAccessSegmentSize;

        while (start < endAddress)
        {
            uint32_t xacc;

            segmentIndex = start / driver->PFlashAccessSegmentSize;

            if (segmentIndex < 32)
            {
                xacc = *(const volatile uint32_t *)&FTFx->XACCL3;
            }
            else if (segmentIndex < driver->PFlashAccessSegmentCount)
            {
                xacc = *(const volatile uint32_t *)&FTFx->XACCH3;
                segmentIndex -= 32;
            }
            else
            {
                break;
            }

            /* Determine if this address range is in a execute-only protection flash segment. */
            if ((~xacc) & (1u << segmentIndex))
            {
                executeOnlySegmentCounter++;
            }

            start += driver->PFlashAccessSegmentSize;
        }

        if (executeOnlySegmentCounter < 1u)
        {
            *access_state = kFlashAccess_UnLimited;
        }
        else if (executeOnlySegmentCounter < maxSupportedExecuteOnlySegmentCount)
        {
            *access_state = kFlashAccess_Mixed;
        }
        else
        {
            *access_state = kFlashAccess_ExecuteOnly;
        }
    }
#else
    *access_state = kFlashAccess_UnLimited;
#endif /* FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL */

    return (returnCode);
}

/* See fsl_flash.h for documentation of this function. */
status_t FLASH_GetProperty(flash_driver_t *driver, flash_property_t whichProperty, uint32_t *value)
{
    if ((driver == NULL) || (value == NULL))
    {
        return kStatus_FLASH_InvalidArgument;
    }

    switch (whichProperty)
    {
        case kFlashProperty_PFlashSectorSize:
            *value = driver->PFlashSectorSize;
            break;

        case kFlashProperty_PFlashTotalSize:
            *value = driver->PFlashTotalSize;
            break;

        case kFlashProperty_PFlashBlockSize:
            *value = driver->PFlashTotalSize / FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT;
            break;

        case kFlashProperty_PFlashBlockCount:
            *value = driver->PFlashBlockCount;
            break;

        case kFlashProperty_PFlashBlockBaseAddr:
            *value = driver->PFlashBlockBase;
            break;

        case kFlashProperty_PFlashFacSupport:
#if defined(FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL)
            *value = FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL;
#else
            *value = 0;
#endif /* FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL */
            break;

        case kFlashProperty_PFlashAccessSegmentSize:
            *value = driver->PFlashAccessSegmentSize;
            break;

        case kFlashProperty_PFlashAccessSegmentCount:
            *value = driver->PFlashAccessSegmentCount;
            break;

#if FLASH_SSD_IS_FLEXNVM_ENABLED
        case kFlashProperty_DFlashSectorSize:
            *value = FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SECTOR_SIZE;
            break;
        case kFlashProperty_DFlashTotalSize:
            *value = driver->DFlashTotalSize;
            break;
        case kFlashProperty_DFlashBlockSize:
            *value = FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SIZE;
            break;
        case kFlashProperty_DFlashBlockCount:
            *value = FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_COUNT;
            break;
        case kFlashProperty_DFlashBlockBaseAddr:
            *value = driver->DFlashBlockBase;
            break;
        case kFlashProperty_EEpromTotalSize:
            *value = driver->EEpromTotalSize;
            break;
#endif /* FLASH_SSD_IS_FLEXNVM_ENABLED */

        default: /* catch inputs that are not recognized */
            return kStatus_FLASH_UnknownProperty;
    }

    return kStatus_FLASH_Success;
}

#if defined(FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD) && FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD
/* See fsl_flash.h for documentation of this function. */
status_t FLASH_SetFlexramFunction(flash_driver_t *driver, flash_flexram_function_option_t option)
{
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

    /* preparing passing parameter to verify all block command */
    kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_1_2(FTFx_SET_FLEXRAM_FUNCTION, option, 0xFFFFU);

    /* calling flash command sequence function to execute the command */
    return flash_command_sequence(driver);
}
#endif /* FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD */

#if defined(FSL_FEATURE_FLASH_HAS_SWAP_CONTROL_CMD) && FSL_FEATURE_FLASH_HAS_SWAP_CONTROL_CMD
/* See fsl_flash.h for documentation on this function. */
status_t FLASH_SwapControl(flash_driver_t *driver,
                           uint32_t address,
                           flash_swap_control_option_t option,
                           flash_swap_info_t *returnInfo)
{
    status_t returnCode;

    if ((driver == NULL) || (returnInfo == NULL))
    {
        return kStatus_FLASH_InvalidArgument;
    }

    if (address & (FSL_FEATURE_FLASH_PFLASH_SWAP_CONTROL_CMD_ADDRESS_ALIGMENT - 1))
    {
        return kStatus_FLASH_AlignmentError;
    }

    /* Make sure address provided is in the lower half of Program flash but not in the Flash Configuration Field */
    if ((address >= (driver->PFlashTotalSize / 2)) ||
        ((address >= kFlashConfig_Start) && (address <= kFlashConfig_End)))
    {
        return kStatus_FLASH_SwapIndicatorAddressError;
    }

    /* Check the option. */
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
}
#endif /* FSL_FEATURE_FLASH_HAS_SWAP_CONTROL_CMD */

#if defined(FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP) && FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP
/* See fsl_flash.h for documentation on this function. */
status_t FLASH_Swap(flash_driver_t *driver, uint32_t address, flash_swap_function_option_t option)
{
    flash_swap_info_t returnInfo;
    status_t returnCode;

    memset(&returnInfo, 0xFFU, sizeof(returnInfo));

    do
    {
        returnCode = FLASH_SwapControl(driver, address, kFlashSwap_ReportStatus, &returnInfo);
        if (returnCode != kStatus_FLASH_Success)
        {
            return returnCode;
        }

        if (kFlashSwapFunction_Disable == option)
        {
            if (returnInfo.flashSwapState == kFlashSwapState_Disabled)
            {
                return kStatus_FLASH_Success;
            }
            else if (returnInfo.flashSwapState == kFlashSwapState_Uninitialized)
            {
                /* The swap system changed to the DISABLED state with Program flash block 0
                 * located at relative flash address 0x0_0000 */
                returnCode = FLASH_SwapControl(driver, address, kFlashSwap_DisableSystem, &returnInfo);
            }
            else
            {
                /* Swap disable should be requested only when swap system is in the uninitialized state */
                return kStatus_FLASH_SwapSystemNotInUninitialized;
            }
        }
        else
        {
            /* When first swap: the initial swap state is Uninitialized, flash swap inidicator address is unset,
             *    the swap procedure should be Uninitialized -> Update-Erased -> Complete.
             * After the first swap has been completed, the flash swap inidicator address cannot be modified
             *    unless EraseAllBlocks command is issued, the swap procedure is changed to Update -> Update-Erased ->
             *    Complete. */
            switch (returnInfo.flashSwapState)
            {
                case kFlashSwapState_Uninitialized:
                    /* If current swap mode is Uninitialized, Initialize Swap to Initialized/READY state. */
                    returnCode = FLASH_SwapControl(driver, address, kFlashSwap_IntializeSystem, &returnInfo);
                    break;
                case kFlashSwapState_Ready:
                    /* Validate whether the address provided to the swap system is matched to
                     * swap indicator address in the IFR */
                    returnCode = flash_validate_swap_indicator_address(driver, address);
                    if (returnCode == kStatus_FLASH_Success)
                    {
                        /* If current swap mode is Initialized/Ready, Initialize Swap to UPDATE state. */
                        returnCode = FLASH_SwapControl(driver, address, kFlashSwap_SetInUpdateState, &returnInfo);
                    }
                    break;
                case kFlashSwapState_Update:
                    /* If current swap mode is Update, Erase indicator sector in non active block
                     * to proceed swap system to update-erased state */
                    returnCode = FLASH_Erase(driver, address + (driver->PFlashTotalSize >> 1),
                                             COMMON_FEATURE_FLASH_SECTOR_CMD_ADDRESS_ALIGMENT, kFlashEraseKey);
                    break;
                case kFlashSwapState_UpdateErased:
                    /* If current swap mode is Update or Update-Erased, progress Swap to COMPLETE State */
                    returnCode = FLASH_SwapControl(driver, address, kFlashSwap_SetInCompleteState, &returnInfo);
                    break;
                case kFlashSwapState_Complete:
                    break;
                case kFlashSwapState_Disabled:
                    /* When swap system is in disabled state, We need to clear swap system back to uninitialized
                     * by issuing EraseAllBlocks command */
                    returnCode = kStatus_FLASH_SwapSystemNotInUninitialized;
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
#endif /* FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP */

#if defined(FSL_FEATURE_FLASH_HAS_PROGRAM_PARTITION_CMD) && FSL_FEATURE_FLASH_HAS_PROGRAM_PARTITION_CMD
/* See fsl_flash.h for documentation of this function. */
status_t FLASH_ProgramPartition(flash_driver_t *driver,
                                flash_partition_flexram_load_option_t option,
                                uint32_t eepromDataSizeCode,
                                uint32_t flexnvmPartitionCode)
{
    status_t returnCode;

    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* eepromDataSizeCode[7:6], flexnvmPartitionCode[7:4] should be all 1'b0
     *  or it will cause access error. */
    /* eepromDataSizeCode &= 0x3FU;  */
    /* flexnvmPartitionCode &= 0x0FU; */

    /* preparing passing parameter to program the flash block */
    kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_2_1(FTFx_PROGRAM_PARTITION, 0xFFFFU, option);
    kFCCOBx[1] = BYTES_JOIN_TO_WORD_1_1_2(eepromDataSizeCode, flexnvmPartitionCode, 0xFFFFU);

    /* calling flash command sequence function to execute the command */
    returnCode = flash_command_sequence(driver);

    flash_cache_clear(driver);

#if FLASH_SSD_IS_FLEXNVM_ENABLED
    /* Data flash IFR will be updated by program partition command during reset sequence,
     * so we just set reserved values for partitioned FlexNVM size here */
    driver->EEpromTotalSize = FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_RESERVED;
    driver->DFlashTotalSize = FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED;
#endif

    return (returnCode);
}
#endif /* FSL_FEATURE_FLASH_HAS_PROGRAM_PARTITION_CMD */

/* See fsl_flash.h for documentation of this function. */
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

/* See fsl_flash.h for documentation of this function. */
status_t FLASH_PflashGetProtection(flash_driver_t *driver, uint32_t *protectStatus)
{
    if ((driver == NULL) || (protectStatus == NULL))
    {
        return kStatus_FLASH_InvalidArgument;
    }

    *protectStatus = *kFPROT;

    return kStatus_FLASH_Success;
}

#if FLASH_SSD_IS_FLEXNVM_ENABLED
/* See fsl_flash.h for documentation of this function. */
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
#endif /* FLASH_SSD_IS_FLEXNVM_ENABLED */

#if FLASH_SSD_IS_FLEXNVM_ENABLED
/* See fsl_flash.h for documentation of this function. */
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
#endif /* FLASH_SSD_IS_FLEXNVM_ENABLED */

#if FLASH_SSD_IS_FLEXNVM_ENABLED
/* See fsl_flash.h for documentation of this function. */
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
#endif /* FLASH_SSD_IS_FLEXNVM_ENABLED */

#if FLASH_SSD_IS_FLEXNVM_ENABLED
/* See fsl_flash.h for documentation of this function. */
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
#endif /* FLASH_SSD_IS_FLEXNVM_ENABLED */

/*!
 * @brief Run flash command
 *
 * This function should be copied to RAM for execution to make sure that code works
 * properly even flash cache is disabled.
 * It is for flash-resident bootloader only, not technically required for ROM or
 *  flashloader (RAM-resident bootloader).
 */
#if FLASH_DRIVER_IS_FLASH_RESIDENT
void flash_run_command(FTFx_REG_ACCESS_TYPE ftfx_fstat)
{
    /* clear CCIF bit */
    *ftfx_fstat = FTFx_FSTAT_CCIF_MASK;

    /* Check CCIF bit of the flash status register, wait till it is set.
     * IP team indicates that this loop will always complete. */
    while (!((*ftfx_fstat) & FTFx_FSTAT_CCIF_MASK))
    {
    }
}

/*!
 * @brief Be used for determining the size of flash_run_command()
 *
 * This function must be defined that lexically follows flash_run_command(),
 * so we can determine the size of flash_run_command() at runtime and not worry
 * about toolchain or code generation differences.
 */
void flash_run_command_end(void)
{
}

/*!
 * @brief Copy flash_run_command() to RAM
 *
 * This function copys the memory between flash_run_command() and flash_run_command_end()
 * into the buffer which is also means that copying flash_run_command() to RAM.
 */
static void copy_flash_run_command(uint8_t *flashRunCommand)
{
    /* Calculate the valid length of flash_run_command() memory */
    uint32_t funcLength = (uint32_t)flash_run_command_end - (uint32_t)flash_run_command;

    assert(funcLength <= kFlashExecuteInRamFunction_MaxSize);

    /* Since the value of ARM function pointer is always odd, but the real start address
     * of function memory should be even, that's why -1 and +1 operation exist. */
    memcpy((void *)flashRunCommand, (void *)((uint32_t)flash_run_command - 1), funcLength);
    callFlashRunCommand = (void (*)(FTFx_REG_ACCESS_TYPE ftfx_fstat))((uint32_t)flashRunCommand + 1);
}
#endif

/*!
 * @brief Flash Command Sequence
 *
 * This function is used to perform the command write sequence to the flash.
 *
 * @param driver Pointer to storage for the driver runtime state.
 * @return An error code or kStatus_FLASH_Success
 */
static status_t flash_command_sequence(flash_driver_t *driver)
{
    uint8_t registerValue;

#if FLASH_DRIVER_IS_FLASH_RESIDENT
    /* clear RDCOLERR & ACCERR & FPVIOL flag in flash status register */
    FTFx->FSTAT = FTFx_FSTAT_RDCOLERR_MASK | FTFx_FSTAT_ACCERR_MASK | FTFx_FSTAT_FPVIOL_MASK;

    status_t returnCode = flash_check_execute_in_ram_function_info(driver);
    if (kStatus_FLASH_Success != returnCode)
    {
        return returnCode;
    }

    /* We pass the ftfx_fstat address as a parameter to flash_run_comamnd() instead of using
     * pre-processed MICRO sentences or operating global variable in flash_run_comamnd()
     * to make sure that flash_run_command() will be compiled into position-independent code (PIC). */
    callFlashRunCommand((FTFx_REG_ACCESS_TYPE)(&FTFx->FSTAT));
#else
    /* clear RDCOLERR & ACCERR & FPVIOL flag in flash status register */
    FTFx->FSTAT = FTFx_FSTAT_RDCOLERR_MASK | FTFx_FSTAT_ACCERR_MASK | FTFx_FSTAT_FPVIOL_MASK;

    /* clear CCIF bit */
    FTFx->FSTAT = FTFx_FSTAT_CCIF_MASK;

    /* Check CCIF bit of the flash status register, wait till it is set.
     * IP team indicates that this loop will always complete. */
    while (!(FTFx->FSTAT & FTFx_FSTAT_CCIF_MASK))
    {
    }
#endif /* FLASH_DRIVER_IS_FLASH_RESIDENT */

    /* Check error bits */
    /* Get flash status register value */
    registerValue = FTFx->FSTAT;

    /* checking access error */
    if (registerValue & FTFx_FSTAT_ACCERR_MASK)
    {
        return kStatus_FLASH_AccessError;
    }
    /* checking protection error */
    else if (registerValue & FTFx_FSTAT_FPVIOL_MASK)
    {
        return kStatus_FLASH_ProtectionViolation;
    }
    /* checking MGSTAT0 non-correctable error */
    else if (registerValue & FTFx_FSTAT_MGSTAT0_MASK)
    {
        return kStatus_FLASH_CommandFailure;
    }
    else
    {
        return kStatus_FLASH_Success;
    }
}

/*!
 * @brief Run flash cache clear command
 *
 * This function should be copied to RAM for execution to make sure that code works
 * properly even flash cache is disabled.
 * It is for flash-resident bootloader only, not technically required for ROM or
 * flashloader (RAM-resident bootloader).
 */
#if FLASH_DRIVER_IS_FLASH_RESIDENT
void flash_cache_clear_command(FTFx_REG32_ACCESS_TYPE ftfx_reg)
{
#if FSL_FEATURE_FLASH_HAS_MCM_FLASH_CACHE_CONTROLS
    *ftfx_reg |= MCM_PLACR_CFCC_MASK;
#elif defined(K66F18_SERIES) || defined(K65F18_SERIES)
    *ftfx_reg = (*ftfx_reg & ~FMC_PFB01CR_CINV_WAY_MASK) | FMC_PFB01CR_CINV_WAY(~0);
#elif FSL_FEATURE_FLASH_HAS_FMC_FLASH_CACHE_CONTROLS
    *ftfx_reg = (*ftfx_reg & ~FMC_PFB0CR_CINV_WAY_MASK) | FMC_PFB0CR_CINV_WAY(~0);
#else
/*    #error "Unknown flash cache controller"  */
#endif /* FSL_FEATURE_FTFx_MCM_FLASH_CACHE_CONTROLS */
       /* Memory barriers for good measure.
        * All Cache, Branch predictor and TLB maintenance operations before this instruction complete */
    __ISB();
    __DSB();
}

/*!
 * @brief Be used for determining the size of flash_cache_clear_command()
 *
 * This function must be defined that lexically follows flash_cache_clear_command(),
 * so we can determine the size of flash_cache_clear_command() at runtime and not worry
 * about toolchain or code generation differences.
 */
void flash_cache_clear_command_end(void)
{
}

/*!
 * @brief Copy flash_cache_clear_command() to RAM
 *
 * This function must be defined that lexically follows flash_cache_clear_command(),
 * into the buffer which is also means that copying flash_cache_clear_command() to RAM.
 */
static void copy_flash_cache_clear_command(uint8_t *flashCacheClearCommand)
{
    /* Calculate the valid length of flash_cache_clear_command() memory */
    uint32_t funcLength = (uint32_t)flash_cache_clear_command_end - (uint32_t)flash_cache_clear_command;

    assert(funcLength <= kFlashExecuteInRamFunction_MaxSize);

    /* Since the value of ARM function pointer is always odd, but the real start address
     * of function memory should be even, that's why -1 and +1 operation exist. */
    memcpy((void *)flashCacheClearCommand, (void *)((uint32_t)flash_cache_clear_command - 1), funcLength);
    callFlashCacheClearCommand = (void (*)(FTFx_REG32_ACCESS_TYPE ftfx_reg))((uint32_t)flashCacheClearCommand + 1);
}
#endif /* FLASH_DRIVER_IS_FLASH_RESIDENT */

/*!
 * @brief Flash Cache Clear
 *
 * This function is used to perform the cache clear to the flash.
 */
#if (defined(__ICCARM__))
#pragma optimize = none
#endif
void flash_cache_clear(flash_driver_t *driver)
{
#if FLASH_DRIVER_IS_FLASH_RESIDENT
    status_t returnCode = flash_check_execute_in_ram_function_info(driver);
    if (kStatus_FLASH_Success != returnCode)
    {
        return;
    }

/* We pass the ftfx register address as a parameter to flash_cache_clear_comamnd() instead of using
 * pre-processed MACROs or a global variable in flash_cache_clear_comamnd()
 * to make sure that flash_cache_clear_command() will be compiled into position-independent code (PIC). */
#if defined(FSL_FEATURE_FLASH_HAS_MCM_FLASH_CACHE_CONTROLS) && FSL_FEATURE_FLASH_HAS_MCM_FLASH_CACHE_CONTROLS
#if defined KL28Z7_SERIES
    callFlashCacheClearCommand((FTFx_REG32_ACCESS_TYPE)&MCM0->PLACR);
#else
    callFlashCacheClearCommand((FTFx_REG32_ACCESS_TYPE)&MCM->PLACR);
#endif
#elif defined(K66F18_SERIES) || defined(K65F18_SERIES)
    callFlashCacheClearCommand((FTFx_REG32_ACCESS_TYPE)&FMC->PFB01CR);
#elif defined(FSL_FEATURE_FLASH_HAS_FMC_FLASH_CACHE_CONTROLS) && FSL_FEATURE_FLASH_HAS_FMC_FLASH_CACHE_CONTROLS
#if (defined(K66F18_SERIES) || defined(K65F18_SERIES))
    callFlashCacheClearCommand((FTFx_REG32_ACCESS_TYPE)&FMC->PFB01CR);
#else
    callFlashCacheClearCommand((FTFx_REG32_ACCESS_TYPE)&FMC->PFB0CR);
#endif
#else
/*    #error "Unknown flash cache controller" */
#endif /* FSL_FEATURE_FTFx_MCM_FLASH_CACHE_CONTROLS */
#else
#if defined(FSL_FEATURE_FLASH_HAS_MCM_FLASH_CACHE_CONTROLS) && FSL_FEATURE_FLASH_HAS_MCM_FLASH_CACHE_CONTROLS
#if defined KL28Z7_SERIES
    MCM0->PLACR |= MCM_PLACR_CFCC_MASK;
#else
    MCM->PLACR |= MCM_PLACR_CFCC_MASK;
#endif
#elif defined(K66F18_SERIES) || defined(K65F18_SERIES)
    FMC->PFB01CR = (FMC->PFB01CR & ~FMC_PFB01CR_CINV_WAY_MASK) | FMC_PFB01CR_CINV_WAY(~0);
#elif defined(FSL_FEATURE_FLASH_HAS_FMC_FLASH_CACHE_CONTROLS) && FSL_FEATURE_FLASH_HAS_FMC_FLASH_CACHE_CONTROLS
    FMC->PFB0CR = (FMC->PFB0CR & ~FMC_PFB0CR_CINV_WAY_MASK) | FMC_PFB0CR_CINV_WAY(~0);
#elif defined(FSL_FEATURE_FLASH_HAS_MSCM_FLASH_CAHCE_CONTROLS) && FSL_FEATURE_FLASH_HAS_MSCM_FLASH_CAHCE_CONTROLS
    MSCM->OCMDR[0] |= MSCM_OCMDR_OCMC1(2);
    MSCM->OCMDR[0] |= MSCM_OCMDR_OCMC1(1);
#else
/*    #error "Unknown flash cache controller" */
#endif /* FSL_FEATURE_FTFx_MCM_FLASH_CACHE_CONTROLS */
#endif /* FLASH_DRIVER_IS_FLASH_RESIDENT */
}

#if FLASH_DRIVER_IS_FLASH_RESIDENT
/*! @brief Check whether flash execute-in-ram functions are ready  */
static status_t flash_check_execute_in_ram_function_info(flash_driver_t *driver)
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
#endif /* FLASH_DRIVER_IS_FLASH_RESIDENT */

/*! @brief Validates the range and alignment of the given address range.*/
static status_t flash_check_range(flash_driver_t *driver,
                                  uint32_t startAddress,
                                  uint32_t lengthInBytes,
                                  uint32_t alignmentBaseline)
{
    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* Verify the start and length are alignmentBaseline aligned. */
    if ((startAddress & (alignmentBaseline - 1)) || (lengthInBytes & (alignmentBaseline - 1)))
    {
        return kStatus_FLASH_AlignmentError;
    }

/* check for valid range of the target addresses */
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

/*! @brief Gets the right address, sector and block size of current flash type which is indicated by address.*/
static status_t flash_get_matched_operation_info(flash_driver_t *driver, uint32_t address, flash_operation_info_t *info)
{
    if (driver == NULL)
    {
        return kStatus_FLASH_InvalidArgument;
    }

    /* Clean up info Structure*/
    memset(info, 0, sizeof(flash_operation_info_t));

#if FLASH_SSD_IS_FLEXNVM_ENABLED
    if ((address >= driver->DFlashBlockBase) && (address <= (driver->DFlashBlockBase + driver->DFlashTotalSize)))
    {
        info->convertedAddress = address - driver->DFlashBlockBase + 0x800000U;
        info->activeSectorSize = FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SECTOR_SIZE;
        info->activeBlockSize = driver->DFlashTotalSize / FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_COUNT;
    }
    else
#endif /* FLASH_SSD_IS_FLEXNVM_ENABLED */
    {
        info->convertedAddress = address;
        info->activeSectorSize = driver->PFlashSectorSize;
        info->activeBlockSize = driver->PFlashTotalSize / driver->PFlashBlockCount;
    }

    return kStatus_FLASH_Success;
}

/*! @brief Validates the given user key for flash erase APIs.*/
static status_t flash_check_user_key(uint32_t key)
{
    /* Validate the user key */
    if (key != kFlashEraseKey)
    {
        return kStatus_FLASH_EraseKeyError;
    }

    return kStatus_FLASH_Success;
}

#if FLASH_SSD_IS_FLEXNVM_ENABLED
/*! @brief Updates FlexNVM memory partition status according to data flash 0 IFR.*/
static status_t flash_update_flexnvm_memory_partition_status(flash_driver_t *driver)
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

    /* Get FlexNVM memory partition info from data flash IFR */
    returnCode = FLASH_ReadResource(driver, DFLASH_IFR_READRESOURCE_START_ADDRESS, (uint32_t *)&dataIFRReadOut,
                                    sizeof(dataIFRReadOut), kFlashResource_FlashIFR);
    if (returnCode != kStatus_FLASH_Success)
    {
        return kStatus_FLASH_PartitionStatusUpdateFailure;
    }

    /* Fill out partitioned EEPROM size */
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

    /* Fill out partitioned DFlash size */
    dataIFRReadOut.FlexNVMPartitionCode &= 0x0FU;
    switch (dataIFRReadOut.FlexNVMPartitionCode)
    {
        case 0x00U:
#if (FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0000 != 0xFFFFFFFF)
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0000;
#else
            driver->DFlashTotalSize = FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED;
#endif /* FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0000 */
            break;
        case 0x01U:
#if (FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0001 != 0xFFFFFFFF)
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0001;
#else
            driver->DFlashTotalSize = FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED;
#endif /* FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0001 */
            break;
        case 0x02U:
#if (FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0010 != 0xFFFFFFFF)
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0010;
#else
            driver->DFlashTotalSize = FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED;
#endif /* FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0010 */
            break;
        case 0x03U:
#if (FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0011 != 0xFFFFFFFF)
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0011;
#else
            driver->DFlashTotalSize = FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED;
#endif /* FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0011 */
            break;
        case 0x04U:
#if (FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0100 != 0xFFFFFFFF)
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0100;
#else
            driver->DFlashTotalSize = FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED;
#endif /* FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0100 */
            break;
        case 0x05U:
#if (FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0101 != 0xFFFFFFFF)
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0101;
#else
            driver->DFlashTotalSize = FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED;
#endif /* FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0101 */
            break;
        case 0x06U:
#if (FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0110 != 0xFFFFFFFF)
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0110;
#else
            driver->DFlashTotalSize = FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED;
#endif /* FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0110 */
            break;
        case 0x07U:
#if (FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0111 != 0xFFFFFFFF)
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0111;
#else
            driver->DFlashTotalSize = FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED;
#endif /* FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0111 */
            break;
        case 0x08U:
#if (FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1000 != 0xFFFFFFFF)
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1000;
#else
            driver->DFlashTotalSize = FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED;
#endif /* FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1000 */
            break;
        case 0x09U:
#if (FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1001 != 0xFFFFFFFF)
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1001;
#else
            driver->DFlashTotalSize = FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED;
#endif /* FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1001 */
            break;
        case 0x0AU:
#if (FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1010 != 0xFFFFFFFF)
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1010;
#else
            driver->DFlashTotalSize = FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED;
#endif /* FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1010 */
            break;
        case 0x0BU:
#if (FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1011 != 0xFFFFFFFF)
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1011;
#else
            driver->DFlashTotalSize = FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED;
#endif /* FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1011 */
            break;
        case 0x0CU:
#if (FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1100 != 0xFFFFFFFF)
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1100;
#else
            driver->DFlashTotalSize = FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED;
#endif /* FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1100 */
            break;
        case 0x0DU:
#if (FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1101 != 0xFFFFFFFF)
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1101;
#else
            driver->DFlashTotalSize = FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED;
#endif /* FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1101 */
            break;
        case 0x0EU:
#if (FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1110 != 0xFFFFFFFF)
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1110;
#else
            driver->DFlashTotalSize = FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED;
#endif /* FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1110 */
            break;
        case 0x0FU:
#if (FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1111 != 0xFFFFFFFF)
            driver->DFlashTotalSize = FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1111;
#else
            driver->DFlashTotalSize = FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED;
#endif /* FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1111 */
            break;
        default:
            driver->DFlashTotalSize = FLEX_NVM_DFLASH_SIZE_FOR_DEPART_RESERVED;
            break;
    }

    return kStatus_FLASH_Success;
}
#endif /* FLASH_SSD_IS_FLEXNVM_ENABLED */

#if defined(FSL_FEATURE_FLASH_HAS_READ_RESOURCE_CMD) && FSL_FEATURE_FLASH_HAS_READ_RESOURCE_CMD
/*! @brief Validates the range of the given resource address.*/
static status_t flash_check_resource_range(uint32_t start, uint32_t lengthInBytes, flash_read_resource_option_t option)
{
    status_t status;
    uint32_t maxReadbleAddress;

    if ((start & (kFlashReadResource_AlignSizeInBytes - 1)) ||
        (lengthInBytes & (kFlashReadResource_AlignSizeInBytes - 1)))
    {
        return kStatus_FLASH_AlignmentError;
    }

    status = kStatus_FLASH_Success;

    maxReadbleAddress = start + lengthInBytes - 1;
    if (option == kFlashResource_VersionID)
    {
        if ((start != kFlashResource_VersionIdStart) ||
            ((start + lengthInBytes - 1) != kFlashResource_VersionIdSizeEnd))
        {
            status = kStatus_FLASH_InvalidArgument;
        }
    }
    else if (option == kFlashResource_FlashIFR)
    {
        if (maxReadbleAddress < kFlashResource_PFlashIFRSizeInBytes)
        {
        }
#if defined(FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP) && FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP
        else if ((start >= kFlashResource_PFlashSwapIfrStart) && (maxReadbleAddress <= kFlashResource_PFlashSwapIfrEnd))
        {
        }
        else if ((start >= kFlashResource_DFlashIfrStart) && (maxReadbleAddress <= kFlashResource_DFlashIfrEnd))
        {
        }
#endif /* FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP */
        else
        {
            status = kStatus_FLASH_InvalidArgument;
        }
    }
    else
    {
        status = kStatus_FLASH_InvalidArgument;
    }

    return status;
}
#endif /* FSL_FEATURE_FLASH_HAS_READ_RESOURCE_CMD */

#if defined(FSL_FEATURE_FLASH_HAS_SWAP_CONTROL_CMD) && FSL_FEATURE_FLASH_HAS_SWAP_CONTROL_CMD
/*! @brief Validates the gived swap control option.*/
static status_t flash_check_swap_control_option(flash_swap_control_option_t option)
{
    if ((option == kFlashSwap_IntializeSystem) || (option == kFlashSwap_SetInUpdateState) ||
        (option == kFlashSwap_SetInCompleteState) || (option == kFlashSwap_ReportStatus) ||
        (option == kFlashSwap_DisableSystem))
    {
        return kStatus_FLASH_Success;
    }

    return kStatus_FLASH_InvalidArgument;
}
#endif /* FSL_FEATURE_FLASH_HAS_SWAP_CONTROL_CMD */

#if defined(FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP) && FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP
/*! @brief Validates the gived address to see if it is equal to swap indicator address in pflash swap IFR.*/
static status_t flash_validate_swap_indicator_address(flash_driver_t *driver, uint32_t address)
{
    flash_swap_ifr_field_t flashSwapIfrField;
    uint32_t swapIndicatorAddress;

    status_t returnCode;
    returnCode = FLASH_ReadResource(driver, kFlashResource_PFlashSwapIfrStart, (uint32_t *)&flashSwapIfrField,
                                    sizeof(flash_swap_ifr_field_t), kFlashResource_FlashIFR);
    if (returnCode != kStatus_FLASH_Success)
    {
        return returnCode;
    }

    /* The high 2 byte value of Swap Indicator Address is stored in Program Flash Swap IFR Field,
     * the low 4 bit value of Swap Indicator Address is always 4'b0000 */
    swapIndicatorAddress =
        (uint32_t)flashSwapIfrField.swapIndicatorAddress * FSL_FEATURE_FLASH_PFLASH_SWAP_CONTROL_CMD_ADDRESS_ALIGMENT;
    if (address != swapIndicatorAddress)
    {
        return kStatus_FLASH_SwapIndicatorAddressError;
    }

    return returnCode;
}
#endif /* FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP */

#if defined(FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD) && FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD
/*! @brief Validates the gived flexram function option.*/
static inline status_t flasn_check_flexram_function_option_range(flash_flexram_function_option_t option)
{
    if ((option != kFlash_FlexRAM_Available_As_RAM) && (option != kFlash_FlexRAM_Available_For_EEPROM))
    {
        return kStatus_FLASH_InvalidArgument;
    }

    return kStatus_FLASH_Success;
}
#endif /* FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD */
