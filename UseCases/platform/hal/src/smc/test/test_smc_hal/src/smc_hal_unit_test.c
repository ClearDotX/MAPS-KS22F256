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

#include "smc_hal_unit_test.h"
#include "fsl_smc_hal.h"
#include "fsl_clock_manager.h"
#include "fsl_lptmr_driver.h"
#include "fsl_llwu_hal.h"

/*
 * README:
 * Please set baud rate to 9600.
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*
 * The auto run system waits for the log "test Succeed" with a timeout, so if
 * error occurs, unit test could loop forever, then auto run system could
 * know test failed.
 */
#define AUTORUN_CONFIG 1

#if AUTORUN_CONFIG
#define DEBUG_ASSERT debug_assert
#else
#define DEBUG_ASSERT assert
#endif

static void debug_assert(bool condition)
{
    if (!condition)
    {
        __disable_irq();
        for(;;){}
    }
}

#define TEST_SLEEP_TIME 100  /* How long stay in WAIT/STOP mode. unit: ms. */

volatile uint8_t lptmrIrqFlag = 0; // Flag to indicate that LPTMR0 interrupt occurs.

#if (FSL_FEATURE_SOC_LPTMR_COUNT == 1)
void LPTMR0_IRQHandler(void)
#else
void LPTMR0_LPTMR1_IRQHandler(void)
#endif
{
    CLOCK_SYS_EnableLptmrClock(0);
    LPTMR_DRV_IRQHandler(0U);
    LPTMR_HAL_SetIntCmd(LPTMR0, false);
    lptmrIrqFlag = 1;
}

void LLWU_IRQHandler(void)
{
#if FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE0
    CLOCK_SYS_EnableLptmrClock(0);
    LPTMR_HAL_SetIntCmd(LPTMR0, false);
    LPTMR_HAL_ClearIntFlag(LPTMR0);
#endif
}

/*
 * Ugly, because of vecter table definition dismatch, will update when vector
 * table is updated.
 */
void LLW_IRQHandler(void)
{
    LLWU_IRQHandler();
}

/*
 * Function to sleep some time. The sleep mode is defined by the parameter
 * smcConfig, could be WAIT/STOP/VLPR/VLPS/LLS. The sleep time is defined by
 * the parameter sleep_ms.
 */
static void sleep_now(smc_power_mode_config_t* smcConfig, uint32_t sleep_ms)
{
    volatile uint32_t curTime; /* Time get by LPTMR, used to test sleep time. */
    curTime = LPTMR_HAL_GetCounterValue(LPTMR0);
    LPTMR_HAL_SetCompareValue(LPTMR0, curTime+sleep_ms);
    LPTMR_HAL_SetIntCmd(LPTMR0, true);
    // sleep now.
    SMC_HAL_SetMode(SMC, smcConfig);
    // Wakeup now, check sleep time.
    // LPTMR overflow time is 65.536s, so could ignore overflow here.
    DEBUG_ASSERT((LPTMR_HAL_GetCounterValue(LPTMR0)-curTime) >= sleep_ms);
}

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    lptmr_prescaler_user_config_t prescaler_config;
    lptmr_working_mode_user_config_t lptmr_config;
    hardware_init();

    smc_power_mode_config_t smcConfig = {
        .powerModeName = kPowerModeRun,
        .stopSubMode = kSmcStopSub0,
#if FSL_FEATURE_SMC_HAS_LPWUI
        .lpwuiOptionValue = kSmcLpwuiEnabled,
#endif
#if FSL_FEATURE_SMC_HAS_PORPO
        .porOptionValue = kSmcPorEnabled,
#endif
#if FSL_FEATURE_SMC_HAS_RAM2_POWER_OPTION
        .ram2OptionValue = kSmcRam2DisPowered,
#endif
#if FSL_FEATURE_SMC_HAS_PSTOPO
        .pstopOptionValue = kSmcPstopStop,
#endif
#if FSL_FEATURE_SMC_HAS_LPOPO
        .lpoOptionValue = kSmcLpoEnabled,
#endif
    };

    /* start testing */
    printf("\r\n\r\n\r\n");
    printf("\r\n------ smc manager unit test start --------\r\n\r\n");

    printf("\r\n-----------SMC function test. ----------------\r\n");
    /*
     * Test step:
     * 1. Set to RUN mode.
     * 2. Set to HSRUN mode, then change back to RUN mode. [Optional]
     * 3. Set to VLPR mode, then change back to RUN mode.
     * 4. Set to WAIT/STOP/LLS mode, then wakeup by LPTMR. [LLS optional]
     * 5. Set to VLPR mode.
     * 6. Set to VLPW/VLPS/LLS mode, then wakeup by LPTMR. [LLS optional]
     * 7. Test the lowpower wakeup on interrupt function[LPWUI]. [Optional]
     * 8. RUN mode to VLPS directly.
     */

    /* Pre test, setup LPTMR used to wake up SOC. */
    /* LPTMR runs in freerun mode, use LPO as clock source, counts every 1ms. */
    CLOCK_SYS_EnableLptmrClock(0);
    LPTMR_HAL_Disable(LPTMR0);

    prescaler_config.prescalerBypass = true;
    prescaler_config.prescalerClockSelect = (lptmr_prescaler_clock_select_t)kClockLptmrSrcLpoClk;
    LPTMR_HAL_SetPrescalerMode(LPTMR0, prescaler_config);

    lptmr_config.freeRunningEnable = true;
    lptmr_config.timerModeSelect = kLptmrTimerModeTimeCounter;
    LPTMR_HAL_SetTimerWorkingMode(LPTMR0, lptmr_config);

    LPTMR_HAL_SetIntCmd(LPTMR0,false);

    LPTMR_HAL_Enable(LPTMR0);
    NVIC_EnableIRQ(g_lptmrIrqId[LPTMR0_IDX]);

    /* Setup LLWU.
     * Some platforms don't have internal wakeup module(such as KL03), so
     * could not test LLS/VLLSx for this platforms. For the platforms that
     * support internal wakeup module, there is a fact that LPTMR0 is always
     * the LLWU_M0IF. So here we can check the macro
     * FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE0 to determine whether LLS/VLLSx test
     * is supported on current platform.
     */
#if FSL_FEATURE_SIM_HAS_SCGC_LLWU
    CLOCK_SYS_EnableLlwuClock(0);
#endif
#if FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE0
    LLWU_HAL_SetInternalModuleCmd(LLWU, kLlwuWakeupModule0, true);
    NVIC_EnableIRQ(LLWU_IRQn);
#endif

    /* STEP 1. Set to RUN mode. */
    smcConfig.powerModeName = kPowerModeRun;
    SMC_HAL_SetMode(SMC, &smcConfig);
    DEBUG_ASSERT(SMC_HAL_GetStat(SMC) == kStatRun);

    /* STEP 2. Set to HSRUN mode, then back to RUN mode. */
#if FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
    /* Set to HSRUN mode. */
    smcConfig.powerModeName = kPowerModeHsrun;
    SMC_HAL_SetMode(SMC, &smcConfig);
    DEBUG_ASSERT(SMC_HAL_GetStat(SMC) == kStatHsrun);
    /* Set to RUN mode. */
    smcConfig.powerModeName = kPowerModeRun;
    SMC_HAL_SetMode(SMC, &smcConfig);
    DEBUG_ASSERT(SMC_HAL_GetStat(SMC) == kStatRun);
#endif

    /* STEP 3. Set to VLPR mode, then back to RUN mode. */
    /* Set to VLPR mode. */
    smcConfig.powerModeName = kPowerModeVlpr;
    SMC_HAL_SetMode(SMC, &smcConfig);
    DEBUG_ASSERT(SMC_HAL_GetStat(SMC) == kStatVlpr);
    /* Set to RUN mode. */
    smcConfig.powerModeName = kPowerModeRun;
    SMC_HAL_SetMode(SMC, &smcConfig);
    DEBUG_ASSERT(SMC_HAL_GetStat(SMC) == kStatRun);

    /* STEP 4. Set to WAIT/STOP/LLS mode. */
    /* 4.1 WAIT mode */
    smcConfig.powerModeName = kPowerModeWait;
    sleep_now(&smcConfig, TEST_SLEEP_TIME);
    DEBUG_ASSERT(SMC_HAL_GetStat(SMC) == kStatRun);

    /* 4.2 STOP mode */
    smcConfig.powerModeName = kPowerModeStop;
    sleep_now(&smcConfig, TEST_SLEEP_TIME);
    DEBUG_ASSERT(SMC_HAL_GetStat(SMC) == kStatRun);

#if FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE
#if FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE0
    /* 4.3 LLS mode */
    /*
     * When entering LLS, must enable FTF clock because LLWU service routine
     * is located in flash.
     */
    CLOCK_SYS_EnableFtfClock(0);
    smcConfig.powerModeName = kPowerModeLls;
    smcConfig.stopSubMode = kSmcStopSub3;
    sleep_now(&smcConfig, TEST_SLEEP_TIME);
    DEBUG_ASSERT(SMC_HAL_GetStat(SMC) == kStatRun);
#endif
#endif

    /* STEP 5. Set to VLPR mode. */
#if  FSL_FEATURE_SMC_HAS_LPWUI
    /* System remains in lowpower mode if interrupt occurs. */
    smcConfig.lpwuiOptionValue = kSmcLpwuiEnabled;
#endif
    smcConfig.powerModeName = kPowerModeVlpr;
    SMC_HAL_SetMode(SMC, &smcConfig);
    DEBUG_ASSERT(SMC_HAL_GetStat(SMC) == kStatVlpr);

    /* STEP 6. Set to VLPR/VLPS/LLS mode. */
    /* 6.1 VLPW mode. */
    smcConfig.powerModeName = kPowerModeVlpw;
    sleep_now(&smcConfig, TEST_SLEEP_TIME);
    DEBUG_ASSERT(SMC_HAL_GetStat(SMC) == kStatVlpr);

    /* 6.2 VLPS mode */
    smcConfig.powerModeName = kPowerModeVlps;
    sleep_now(&smcConfig, TEST_SLEEP_TIME);
    DEBUG_ASSERT(SMC_HAL_GetStat(SMC) == kStatVlpr);

#if (FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE && FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE0)
    /* 6.3 LLS mode */
    /*
     * When entering LLS, must enable FTF clock because LLWU service routine
     * is located in flash.
     */
    CLOCK_SYS_EnableFtfClock(0);
    smcConfig.powerModeName = kPowerModeLls;
    smcConfig.stopSubMode = kSmcStopSub3;
    sleep_now(&smcConfig, TEST_SLEEP_TIME);
#endif

    smcConfig.powerModeName = kPowerModeRun;
    SMC_HAL_SetMode(SMC, &smcConfig);
    DEBUG_ASSERT(SMC_HAL_GetStat(SMC) == kStatRun);

    /* STEP 7. Test the function of LPWUI. */
#if FSL_FEATURE_SMC_HAS_LPWUI
    /* System returns to RUN mode on interrupt. */
    /* 7.1 Enter VLPR mode. */
    smcConfig.lpwuiOptionValue = kSmcLpwuiDisabled;
    smcConfig.powerModeName = kPowerModeVlpr;
    SMC_HAL_SetMode(SMC, &smcConfig);
    DEBUG_ASSERT(SMC_HAL_GetStat(SMC) == kStatVlpr);
    /* 7.2 Use LPTMR interrupt to return to RUN mode. */
    /* Set LPTMR interrupt. */
    lptmrIrqFlag = 0;
    LPTMR_HAL_SetCompareValue(LPTMR0, LPTMR_HAL_GetCounterValue(LPTMR0)+5u);
    LPTMR_HAL_SetIntCmd(LPTMR0, true);
    while (!lptmrIrqFlag){}
    DEBUG_ASSERT(SMC_HAL_GetStat(SMC) == kStatRun);

    /* 7.3 Enter VLPW and return to RUN mode. */
    // VLPR first.
    smcConfig.powerModeName = kPowerModeVlpr;
    SMC_HAL_SetMode(SMC, &smcConfig);
    // VLPW.
    smcConfig.powerModeName = kPowerModeVlpw;
    sleep_now(&smcConfig, TEST_SLEEP_TIME);
    DEBUG_ASSERT(SMC_HAL_GetStat(SMC) == kStatRun);

    /* 7.4 Enter VLPS and return to RUN mode. */
    smcConfig.powerModeName = kPowerModeVlps;
    sleep_now(&smcConfig, TEST_SLEEP_TIME);
    DEBUG_ASSERT(SMC_HAL_GetStat(SMC) == kStatRun);
#endif

    /* STEP 8. RUN mode to VLPS directly. */
    smcConfig.powerModeName = kPowerModeVlps;
    sleep_now(&smcConfig, TEST_SLEEP_TIME);
    DEBUG_ASSERT(SMC_HAL_GetStat(SMC) == kStatRun);

    printf("\r\n------ smc manager unit test end ----------\r\n");
    printf("\r\n-------- SMC unit test Succeed ----------\r\n");
    while(1){}
    return 0;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/


