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

#if FLASH_DRIVER_IS_FLASH_RESIDENT
//! @brief A function pointer used to point to relocated flash_cache_clear_command()
#define FTFx_REG32_ACCESS_TYPE volatile uint32_t *
static void (*callFlashCacheClearCommand)(FTFx_REG32_ACCESS_TYPE ftfx_reg);

//! @brief Run flash cache clear command
//!
//! This function should be copied to RAM for execution to make sure that code works
//! properly even flash cache is disabled.
//! It is for flash-resident bootloader only, not technically required for ROM or
//! flashloader (RAM-resident bootloader).
void flash_cache_clear_command(FTFx_REG32_ACCESS_TYPE ftfx_reg)
{
#if FSL_FEATURE_FLASH_HAS_MCM_FLASH_CACHE_CONTROLS
    *ftfx_reg |= MCM_PLACR_CFCC_MASK;
#elif(defined(K66F18_SERIES) || defined(K65F18_SERIES))
    *ftfx_reg = (*ftfx_reg & ~FMC_PFB01CR_CINV_WAY_MASK) | FMC_PFB01CR_CINV_WAY(~0);
#elif FSL_FEATURE_FLASH_HAS_FMC_FLASH_CACHE_CONTROLS
    *ftfx_reg = (*ftfx_reg & ~FMC_PFB0CR_CINV_WAY_MASK) | FMC_PFB0CR_CINV_WAY(~0);
#else
//    #error "Unknown flash cache controller"
#endif // FSL_FEATURE_FTFx_MCM_FLASH_CACHE_CONTROLS
    // Memory barriers for good measure.
    // All Cache, Branch predictor and TLB maintenance operations before this instruction complete
    __ISB();
    __DSB();
}

//! @brief Be used for determining the size of flash_cache_clear_command()
//!
//! This function must be defined that lexically follows flash_cache_clear_command(),
//! so we can determine the size of flash_cache_clear_command() at runtime and not worry
//! about toolchain or code generation differences.
void flash_cache_clear_command_end(void)
{
}

//! @brief Copy flash_cache_clear_command() to RAM
//!
//! This function copies the memory between flash_cache_clear_command() and flash_cache_clear_command_end()
//! into the buffer which is also means that copying flash_cache_clear_command() to RAM.
void copy_flash_cache_clear_command(uint8_t *flashCacheClearCommand)
{
    // Calculate the valid length of flash_cache_clear_command() memory
    uint32_t funcLength = (uint32_t)flash_cache_clear_command_end - (uint32_t)flash_cache_clear_command;

    assert(funcLength <= kFlashExecuteInRamFunction_MaxSize);

    // Since the value of ARM function pointer is always odd, but the real start address
    // of function memory should be even, that's why -1 and +1 operation exist.
    memcpy((void *)flashCacheClearCommand, (void *)((uint32_t)flash_cache_clear_command - 1), funcLength);
    callFlashCacheClearCommand = (void (*)(FTFx_REG32_ACCESS_TYPE ftfx_reg))((uint32_t)flashCacheClearCommand + 1);
}
#endif // FLASH_DRIVER_IS_FLASH_RESIDENT

////////////////////////////////////////////////////////////////////////////////
//!
//! @brief Flash Cache Clear
//!
//! This function is used to perform the cache clear to the flash.
////////////////////////////////////////////////////////////////////////////////
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

// We pass the ftfx register address as a parameter to flash_cache_clear_comamnd() instead of using
// pre-processed MACROs or a global variable in flash_cache_clear_comamnd()
// to make sure that flash_cache_clear_command() will be compiled into position-independent code (PIC).
#if FSL_FEATURE_FLASH_HAS_MCM_FLASH_CACHE_CONTROLS
#if defined KL28Z7_SERIES
    callFlashCacheClearCommand((FTFx_REG32_ACCESS_TYPE)&MCM0->PLACR);
#else
    callFlashCacheClearCommand((FTFx_REG32_ACCESS_TYPE)&MCM->PLACR);
#endif
#elif(defined(K66F18_SERIES) || defined(K65F18_SERIES))
    callFlashCacheClearCommand((FTFx_REG32_ACCESS_TYPE)&FMC->PFB01CR);
#elif FSL_FEATURE_FLASH_HAS_FMC_FLASH_CACHE_CONTROLS
#if (defined(K66F18_SERIES) || defined(K65F18_SERIES))
    callFlashCacheClearCommand((FTFx_REG32_ACCESS_TYPE)&FMC->PFB01CR);
#else
    callFlashCacheClearCommand((FTFx_REG32_ACCESS_TYPE)&FMC->PFB0CR);
#endif
#else
//    #error "Unknown flash cache controller"
#endif // FSL_FEATURE_FTFx_MCM_FLASH_CACHE_CONTROLS
#else
#if FSL_FEATURE_FLASH_HAS_MCM_FLASH_CACHE_CONTROLS
#if defined KL28Z7_SERIES
    MCM0->PLACR |= MCM_PLACR_CFCC_MASK;
#else
    MCM->PLACR |= MCM_PLACR_CFCC_MASK;
#endif
#elif(defined(K66F18_SERIES) || defined(K65F18_SERIES))
    FMC->PFB01CR = (FMC->PFB01CR & ~FMC_PFB01CR_CINV_WAY_MASK) | FMC_PFB01CR_CINV_WAY(~0)
#elif FSL_FEATURE_FLASH_HAS_FMC_FLASH_CACHE_CONTROLS
    FMC->PFB0CR = (FMC->PFB0CR & ~FMC_PFB0CR_CINV_WAY_MASK) | FMC_PFB0CR_CINV_WAY(~0)
#elif FSL_FEATURE_FLASH_HAS_MSCM_FLASH_CAHCE_CONTROLS
    MSCM->OCMDR[0] |= MSCM_OCMDR_OCMC1(2);
    MSCM->OCMDR[0] |= MSCM_OCMDR_OCMC1(1);
#else
//    #error "Unknown flash cache controller"
#endif // FSL_FEATURE_FTFx_MCM_FLASH_CACHE_CONTROLS
#endif // FLASH_DRIVER_IS_FLASH_RESIDENT
}
