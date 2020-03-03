/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"

#include "rcm_hal_unit_test.h"
#include "fsl_rcm_hal.h"

#if defined(WDOG_INSTANCE_COUNT) // Uses WDOG
#include "fsl_wdog_driver.h"
#else
#include "fsl_cop_driver.h"
#endif

/* NOTE:
 * When the chip uses COP, please don't test with jlink, because jlink
 * disables the COP. COP control register could be written only once, so
 * if use jlink, COP could not work.
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEBUG_ASSERT(condition) \
    if (!(condition)) \
    { \
        printf("Error: function: %s, line: %d\r\n", __func__, __LINE__); \
        __disable_irq(); \
        for(;;){} \
    }

rcm_source_names_t rcmSrcTable[] =
{
    kRcmWakeup,
    kRcmLowVoltDetect,
#if FSL_FEATURE_RCM_HAS_LOC
    kRcmLossOfClk,
#endif
#if FSL_FEATURE_RCM_HAS_LOL
    kRcmLossOfLock,
#endif
    kRcmWatchDog,
    kRcmExternalPin,
    kRcmPowerOn,
#if FSL_FEATURE_RCM_HAS_JTAG
    kRcmJtag,
#endif
    kRcmCoreLockup,
    kRcmSoftware,
    kRcmMdmAp,
#if FSL_FEATURE_RCM_HAS_EZPORT
    kRcmEzport,
#endif
    kRcmStopModeAckErr,

#if FSL_FEATURE_RCM_HAS_CORE1
    kRcmCore1Int,
#endif
};


/*
 * Functions for Watchdog and COP.
 */
// Setup watchdog.
static void test_wdog_init(void)
{
#if defined(WDOG_INSTANCE_COUNT) // Uses WDOG
    wdog_work_mode_t initWorkmode =
    {
      .kWdogEnableInWaitMode  = true,
      .kWdogEnableInStopMode  = true,
      .kWdogEnableInDebugMode = false,
    };

    wdog_config_t initConfig =
    {
      .wdogEnable   = true,
      .clkSrc       = kWdogLpoClkSrc,
      .prescaler    = kWdogClkPrescalerDivide1,
      .workMode     = initWorkmode,
      .updateEnable = true,
      .intEnable    = false,
      .winEnable    = false,
      .timeoutValue = 1024U,
      .windowValue  = 0,
    };
    WDOG_DRV_Init(&initConfig);
#else // Uses COP.
    cop_config_t copInit =
    {
        .copWindowModeEnable = false,
#if FSL_FEATURE_COP_HAS_LONGTIME_MODE
        .copTimeoutMode      = kCopShortTimeoutMode,
        .copStopModeEnable   = false,
        .copDebugModeEnable  = false,
#endif
        .copClockSource      = kCopLpoClock,
        .copTimeout          = kCopTimeout_short_2to5_or_long_2to13
    };
    COP_DRV_Init(0, &copInit);
#endif
}

// Stop watchdog.
static void test_wdog_stop(void)
{
#if defined(WDOG_INSTANCE_COUNT) // Uses WDOG
    WDOG_DRV_Deinit();
#else
    COP_DRV_Disable(0);
#endif
}


/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    uint32_t i;
    rcm_reset_pin_filter_config_t resetConfig;
    rcm_filter_run_wait_modes_t filterInRunWait;
    hardware_init();

    /* start testing */
    printf("\r\n\r\n\r\n");
#if !defined(WDOG_INSTANCE_COUNT) // Uses WDOG
    printf("********************** NOTE!!! ************************\r\n");
    printf("****  1. If current chip uses COP instead of WDOG, ****\r\n");
    printf("****  please define DISABLE_WDOG=0 in project.     ****\r\n");
    printf("****  2. If chip uses COP, please don't test with  ****\r\n");
    printf("****  jlink.                                       ****\r\n");
    printf("*******************************************************\r\n");
#endif
    printf("\r\n------ rcm hal unit test start --------\r\n\r\n");

    /*
     * Functionnality test: Reset by watch dog.
     */
    if (!RCM_HAL_GetSrcStatus(RCM, kRcmWatchDog))
    {
        test_wdog_init();
        for (;;){ }
    }
    DEBUG_ASSERT(RCM_HAL_GetSrcStatus(RCM, kRcmWatchDog));
    test_wdog_stop();
    printf("\r\nReset by Watchdog.\r\n");

    printf("\r\nDump the RCM status reg0 = 0x%x\r\n\r\n", RCM_RD_SRS0(RCM));
    printf("\r\nDump the RCM status reg1 = 0x%x\r\n\r\n", RCM_RD_SRS1(RCM));

    for (i=0; i<sizeof(rcmSrcTable)/sizeof(rcmSrcTable[0]); i++)
    {
        RCM_HAL_GetSrcStatus(RCM, rcmSrcTable[i]);
#if FSL_FEATURE_RCM_HAS_SSRS
        RCM_HAL_GetStickySrcStatus(RCM, rcmSrcTable[i]);
#endif
    }

#if FSL_FEATURE_RCM_HAS_SSRS
    printf("\r\nTest SSRS0 and SSRS1.\r\n\r\n");
    printf("\r\nDump the SSRS0 = 0x%X\r\n\r\n", RCM_RD_SSRS0(RCM));
    printf("\r\nDump the SSRS1 = 0x%X\r\n\r\n", RCM_RD_SSRS1(RCM));

    RCM_HAL_ClearStickySrcStatus(RCM);
    DEBUG_ASSERT(0U == RCM_RD_SSRS0(RCM));
    DEBUG_ASSERT(0U == RCM_RD_SSRS1(RCM));
#endif

    printf("\r\nReset filter test\r\n");

    resetConfig.filterInStop = true;

filter_test:
    for (filterInRunWait  = kRcmFilterDisabled;
         filterInRunWait <= kRcmFilterLpoClk;
         filterInRunWait++)
    {
        // filter in RUN and WAIT.
        resetConfig.filterInRunWait = filterInRunWait;

        if (filterInRunWait == kRcmFilterBusClk)
        {
            // Bus clock count.
            for (i=0; i<32; i++)
            {
                resetConfig.busClockFilterCount = i;
                RCM_HAL_SetResetPinFilterConfig(RCM, &resetConfig);
                DEBUG_ASSERT(resetConfig.filterInStop == (bool)RCM_BRD_RPFC_RSTFLTSS(RCM));
                DEBUG_ASSERT((uint8_t)filterInRunWait == RCM_BRD_RPFC_RSTFLTSRW(RCM));
                DEBUG_ASSERT(i == RCM_BRD_RPFW_RSTFLTSEL(RCM));
            }
        }

        RCM_HAL_SetResetPinFilterConfig(RCM, &resetConfig);
        DEBUG_ASSERT(resetConfig.filterInStop == (bool)RCM_BRD_RPFC_RSTFLTSS(RCM));
        DEBUG_ASSERT((uint8_t)filterInRunWait == RCM_BRD_RPFC_RSTFLTSRW(RCM));
    }

    if(resetConfig.filterInStop)
    {
        resetConfig.filterInStop = false;
        goto filter_test;
    }

#if FSL_FEATURE_RCM_HAS_BOOTROM
    printf("\r\nTest BOOTROM.\r\n\r\n");
    printf("\r\nBoot ROM source: %d\r\n", RCM_HAL_GetBootRomSrc(RCM));
    RCM_HAL_ClearBootRomSrc(RCM);
    DEBUG_ASSERT(0 == RCM_HAL_GetBootRomSrc(RCM));
#endif

    printf("\r\n-------- RCM unit test Succeed ----------\r\n");
    while(1) {}
    return 0;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

  
