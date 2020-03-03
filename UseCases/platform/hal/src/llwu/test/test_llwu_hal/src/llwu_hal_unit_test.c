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

#include "llwu_hal_unit_test.h"
#include "fsl_llwu_hal.h"
#include "fsl_misc_utilities.h"
#include "fsl_smc_hal.h"
#include "fsl_lptmr_driver.h"
#include "fsl_clock_manager.h"

/*
 * README.
 * If no error message is output before "------ llwu hal unit test end ----------"
 * test pass.
 *
 * Please set the baud rate to 9600.
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
#define DEBUG_ASSERT test_assert
#else
#define DEBUG_ASSERT assert
#endif

static void test_assert(bool condition)
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
    LPTMR_HAL_ClearIntFlag(LPTMR0);
    LPTMR_HAL_SetIntCmd(LPTMR0, false);
}

void LLWU_IRQHandler(void)
{
#if FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE0
    DEBUG_ASSERT(LLWU_HAL_GetInternalModuleWakeupFlag(LLWU, kLlwuWakeupModule0));
    CLOCK_SYS_EnableLptmrClock(0);
    LPTMR_HAL_SetIntCmd(LPTMR0, false);
    LPTMR_HAL_ClearIntFlag(LPTMR0);
    DEBUG_ASSERT(!LLWU_HAL_GetInternalModuleWakeupFlag(LLWU, kLlwuWakeupModule0));
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

#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN
/*FUNCTION**********************************************************************
 *
 * Function Name : LLWU_HAL_GetExternalInputPinMode
 * Description   : Get external input pin source mode
 * This function will get the external input pin source mode that will be used
 * as wake up source.
 * 
 *END**************************************************************************/
llwu_external_pin_modes_t LLWU_HAL_GetExternalInputPinMode(LLWU_Type * base,
                                                           llwu_wakeup_pin_t pinNumber)
{
    /*-------------------------------------------------------------------------
      In 8-bit control register, every 2-bit controls one external pin. To set
      the proper register bit field, need to calculate the register and shift in
      register base on pinNumber.

                  X   X   X   X   X   X | X   X           <-- pinNumber
                  ----------------------|------
                  Used to get register  | Used to get shift in register

      Register: pinNumber>>2;
      Shift in register: (pinNumber & 0b11) << 1;

      For example, if pinNumber=1 (WUPE1), register and shift are like this
      -------------------------------------------------
      |   WUPE3   |   WUPE2   |   WUPE1   |   WUPE0   |   <-- Register
      -------------------------------------------------
                                          ^
                                          | Shift in register.

    --------------------------------------------------------------------------*/
    uint8_t shiftInReg = (((uint8_t)pinNumber)&0x03U) << 1U;
    uint8_t regValue   = 0U;

    switch ((uint8_t)pinNumber >> 2U) // Which register to read.
    {
#if (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN0 | FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN1 | \
     FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN2 | FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN3)
        case 0:
            regValue = LLWU_RD_PE1(base);
            break;
#endif
#if (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN4 | FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN5 | \
     FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN6 | FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN7)
        case 1:
            regValue = LLWU_RD_PE2(base);
            break;
#endif
#if (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN8 | FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN9 | \
     FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN10 | FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN11)
        case 2:
            regValue = LLWU_RD_PE3(base);
            break;
#endif
#if (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN12 | FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN13 | \
     FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN14 | FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN15)
        case 3:
            regValue = LLWU_RD_PE4(base);
            break;
#endif
#if (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN16 | FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN17 | \
     FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN18 | FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN19)
        case 4:
            regValue = LLWU_RD_PE5(base);
            break;
#endif
#if (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN20 | FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN21 | \
     FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN22 | FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN23)
        case 5:
            regValue = LLWU_RD_PE6(base);
            break;
#endif
#if (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN24 | FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN25 | \
     FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN26 | FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN27)
        case 6:
            regValue = LLWU_RD_PE7(base);
            break;
#endif
#if (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN28 | FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN29 | \
     FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN30 | FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN31)
        case 7:
            regValue = LLWU_RD_PE8(base);
            break;
#endif
        default:
            break;
    }
    return (llwu_external_pin_modes_t)((regValue >> shiftInReg) & 0x03U);
}
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : LLWU_HAL_GetPinFilterMode
 * Description   : Get pin filter configuration.
 * This function will get the pin filter configuration.
 * 
 *END**************************************************************************/
void LLWU_HAL_GetPinFilterMode(LLWU_Type * base,
                               uint32_t filterNumber, 
                               llwu_external_pin_filter_mode_t *pinFilterMode)
{
    /* check filter and pin number */
    assert(filterNumber < FSL_FEATURE_LLWU_HAS_PIN_FILTER);

    /* branch to filter number */
    switch(filterNumber)
    {
#if (FSL_FEATURE_LLWU_HAS_PIN_FILTER >= 1)
    case 0:
        pinFilterMode->pinNumber = (llwu_wakeup_pin_t)LLWU_BRD_FILT1_FILTSEL(base);
        pinFilterMode->filterMode = (llwu_filter_modes_t)LLWU_BRD_FILT1_FILTE(base);
        break;
#endif
#if (FSL_FEATURE_LLWU_HAS_PIN_FILTER >= 2)
    case 1:
        pinFilterMode->pinNumber = (llwu_wakeup_pin_t)LLWU_BRD_FILT2_FILTSEL(base);
        pinFilterMode->filterMode = (llwu_filter_modes_t)LLWU_BRD_FILT2_FILTE(base);
        break;
#endif
#if (FSL_FEATURE_LLWU_HAS_PIN_FILTER >= 3)
    case 2:
        pinFilterMode->pinNumber = (llwu_wakeup_pin_t)LLWU_BRD_FILT3_FILTSEL(base);
        pinFilterMode->filterMode = (llwu_filter_modes_t)LLWU_BRD_FILT3_FILTE(base);
        break;
#endif
#if (FSL_FEATURE_LLWU_HAS_PIN_FILTER >= 4)
    case 3:
        pinFilterMode->pinNumber = (llwu_wakeup_pin_t)LLWU_BRD_FILT4_FILTSEL(base);
        pinFilterMode->filterMode = (llwu_filter_modes_t)LLWU_BRD_FILT4_FILTE(base);
        break;
#endif
    default:
        break;
    }
}

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Table of base addresses for LLWU instances. */
LLWU_Type * const g_llwuBase[] = LLWU_BASE_PTRS;

#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN
/* LLWU wakeup source table. */
llwu_wakeup_pin_t wakeup_pin_table [] = {
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN0
    kLlwuWakeupPin0,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN1
    kLlwuWakeupPin1,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN2
    kLlwuWakeupPin2,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN3
    kLlwuWakeupPin3,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN4
    kLlwuWakeupPin4,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN5
    kLlwuWakeupPin5,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN6
    kLlwuWakeupPin6,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN7
    kLlwuWakeupPin7,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN8
    kLlwuWakeupPin8,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN9
    kLlwuWakeupPin9,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN10
    kLlwuWakeupPin10,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN11
    kLlwuWakeupPin11,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN12
    kLlwuWakeupPin12,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN13
    kLlwuWakeupPin13,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN14
    kLlwuWakeupPin14,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN15
    kLlwuWakeupPin15,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN16
    kLlwuWakeupPin16,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN17
    kLlwuWakeupPin17,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN18
    kLlwuWakeupPin18,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN19
    kLlwuWakeupPin19,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN20
    kLlwuWakeupPin20,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN21
    kLlwuWakeupPin21,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN22
    kLlwuWakeupPin22,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN23
    kLlwuWakeupPin23,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN24
    kLlwuWakeupPin24,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN25
    kLlwuWakeupPin25,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN26
    kLlwuWakeupPin26,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN27
    kLlwuWakeupPin27,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN28
    kLlwuWakeupPin28,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN29
    kLlwuWakeupPin29,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN30
    kLlwuWakeupPin30,
#endif
#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN31
    kLlwuWakeupPin31
#endif
};
#endif

#if FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE
llwu_wakeup_module_t wakeup_module_table [] = {
#if FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE0
    kLlwuWakeupModule0,
#endif
#if FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE1
    kLlwuWakeupModule1,
#endif
#if FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE2
    kLlwuWakeupModule2,
#endif
#if FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE3
    kLlwuWakeupModule3,
#endif
#if FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE4
    kLlwuWakeupModule4,
#endif
#if FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE5
    kLlwuWakeupModule5,
#endif
#if FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE6
    kLlwuWakeupModule6,
#endif
#if FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE7
    kLlwuWakeupModule7
#endif
};
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    uint32_t i, j;
    lptmr_prescaler_user_config_t prescaler_config;
    lptmr_working_mode_user_config_t lptmr_config;
    llwu_external_pin_filter_mode_t pinFilterMode;
    llwu_external_pin_modes_t pinMode;
    llwu_filter_modes_t filterMode;
#if FSL_FEATURE_LLWU_HAS_RESET_ENABLE
    llwu_reset_pin_mode_t resetEnableMode;
#endif

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

    LLWU_Type * base = g_llwuBase[0];

    hardware_init();

    /* start testing */
    printf("\r\n\r\n\r\n");
    printf("\r\n------ llwu hal unit test start --------\r\n\r\n");

    /* Setup LLWU. */
#if FSL_FEATURE_SIM_HAS_SCGC_LLWU
    CLOCK_SYS_EnableLlwuClock(0);
#endif

#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN
    printf("Test external pin wakeup source setting.\r\n");
    for (i=0; i<ARRAY_SIZE(wakeup_pin_table); i++)
    {
        for (pinMode=kLlwuExternalPinDisabled; pinMode<=kLlwuExternalPinChangeDetect; pinMode++)
        {
            LLWU_HAL_SetExternalInputPinMode(base, pinMode, wakeup_pin_table[i]);
            if (pinMode != LLWU_HAL_GetExternalInputPinMode(base, wakeup_pin_table[i]))
            {
                printf("External pin set error: PIN: %d, Mode: %d!\r\n", wakeup_pin_table[i], pinMode);
                DEBUG_ASSERT(0);  // For auto run.
                break;
            }
        }
        LLWU_HAL_SetExternalInputPinMode(base, kLlwuExternalPinDisabled, wakeup_pin_table[i]);
    }
#endif

#if FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE
    printf("Test internal wakeup module setting.\r\n");
    for (i=0; i<ARRAY_SIZE(wakeup_module_table); i++)
    {
        LLWU_HAL_SetInternalModuleCmd(base, wakeup_module_table[i], true);

        if (!(LLWU_RD_ME(base) & (1U << (uint8_t)wakeup_module_table[i])))
        {
            printf("Internal wakeup module %d enable error!\r\n", wakeup_module_table[i]);
            DEBUG_ASSERT(0);  // For auto run.
            break;
        }
        LLWU_HAL_SetInternalModuleCmd(base, wakeup_module_table[i], false);
        if ((LLWU_RD_ME(base) & (1U << (uint8_t)wakeup_module_table[i])))
        {
            printf("Internal wakeup module %d disable error!\r\n", wakeup_module_table[i]);
            DEBUG_ASSERT(0);  // For auto run.
            break;
        }
    }
#endif

#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN
    printf("Test external pin wakeup flag.\r\n");
    for (i=0; i<ARRAY_SIZE(wakeup_pin_table); i++)
    {
        if (LLWU_HAL_GetExternalPinWakeupFlag(base, wakeup_pin_table[i]))
        {
            LLWU_HAL_ClearExternalPinWakeupFlag(base, wakeup_pin_table[i]);
        }
        if (LLWU_HAL_GetExternalPinWakeupFlag(base, wakeup_pin_table[i]))
        {
            printf("External wakeup flag %d test error!\r\n", wakeup_pin_table[i]);
            DEBUG_ASSERT(0);  // For auto run.
        }
    }
#endif

#if FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE
    printf("Test internal wakeup module flag.\r\n");
    for (i=0; i<ARRAY_SIZE(wakeup_module_table); i++)
    {
        LLWU_HAL_GetInternalModuleWakeupFlag(base, wakeup_module_table[i]);
    }
#endif

#if FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN
    printf("Test digital filter.\r\n");
    for (i=0; i<FSL_FEATURE_LLWU_HAS_PIN_FILTER; i++)
    {
        /* Test filter flag. */
        if (LLWU_HAL_GetFilterDetectFlag(base, i))
        {
            LLWU_HAL_ClearFilterDetectFlag(base, i);
        }
        if (LLWU_HAL_GetFilterDetectFlag(base, i))
        {
            printf("Filter flag test error %d!\r\n", i);
            DEBUG_ASSERT(0);  // For auto run.
        }

        for (filterMode=kLlwuFilterDisabled; filterMode<=kLlwuFilterAnyEdgeDetect; filterMode++)
        {
            pinFilterMode.filterMode = filterMode;
            for (j=0; j<ARRAY_SIZE(wakeup_pin_table); j++)
            {
                pinFilterMode.pinNumber = wakeup_pin_table[j];
                LLWU_HAL_SetPinFilterMode(base, i, pinFilterMode);

                pinFilterMode.filterMode = kLlwuFilterAnyEdgeDetect;
                pinFilterMode.pinNumber  = (llwu_wakeup_pin_t)0;
                LLWU_HAL_GetPinFilterMode(base, i, &pinFilterMode);
                if ((wakeup_pin_table[j] != pinFilterMode.pinNumber) ||
                    (filterMode != pinFilterMode.filterMode))
                {
                    printf("Digital filter test error: FILT%d, FILTE=%d, FILTSEL=%d!\r\n",
                            i, filterMode, wakeup_pin_table[j]);
                    DEBUG_ASSERT(0);  // For auto run.
                }
            }
        }
    }
#endif

#if FSL_FEATURE_LLWU_HAS_RESET_ENABLE
    printf("Test reset enable.\r\n");
    resetEnableMode.enable = true;
    resetEnableMode.filter = true;
    LLWU_HAL_SetResetPinMode(base, resetEnableMode);

    if ((0U == LLWU_BRD_RST_RSTFILT(base)) ||
        (0U == LLWU_BRD_RST_LLRSTE(base)))
    {
        printf("Reset enable test error!\r\n");
        DEBUG_ASSERT(0);  // For auto run.
    }

    resetEnableMode.enable = false;
    resetEnableMode.filter = false;
    LLWU_HAL_SetResetPinMode(base, resetEnableMode);

    if ((1U == LLWU_BRD_RST_RSTFILT(base)) ||
        (1U == LLWU_BRD_RST_LLRSTE(base)))
    {
        printf("Reset enable test error!\r\n");
        DEBUG_ASSERT(0);  // For auto run.
    }
#endif

    /*
     * Functionality test:
     * Set the platform to LLS mode and wakeup by LPTMR, then check
     * the wakeup flag.
     */
#if FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE
#if FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE0

    printf("Enter LLS and wakeup to test.\r\n");
    /* Wait for uart output completed. */
    i = 0x4000;
    while(i--)
    {
        __asm("nop");
    }
    DbgConsole_DeInit();
    /* Setup LPTMR used to wake up SOC. */
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

#if FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE0
    LLWU_HAL_SetInternalModuleCmd(LLWU, kLlwuWakeupModule0, true);
    NVIC_EnableIRQ(LLWU_IRQn);
#endif

    /*
     * When entering LLS, must enable FTF clock because LLWU service routine
     * is located in flash.
     */
    CLOCK_SYS_EnableFtfClock(0);
    smcConfig.powerModeName = kPowerModeLls;
    smcConfig.stopSubMode = kSmcStopSub3;
    sleep_now(&smcConfig, TEST_SLEEP_TIME);
#endif
#endif
    dbg_uart_init();
    printf("\r\n-------- LLWU unit test Succeed ----------\r\n");
    while(1) {}

    return 0;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

  
