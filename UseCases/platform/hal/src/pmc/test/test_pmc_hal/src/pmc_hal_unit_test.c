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

#include "pmc_hal_unit_test.h"
#include "fsl_pmc_hal.h"

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

static void test_bandgap(pmc_bandgap_buffer_config_t * config)
{
    config->enable = true;
    PMC_HAL_BandgapBufferConfig(PMC, config);
    DEBUG_ASSERT(PMC_BRD_REGSC_BGBE(PMC));
#if FSL_FEATURE_PMC_HAS_BGEN
    DEBUG_ASSERT(config->enableInLowPower == (bool)PMC_BRD_REGSC_BGBE(PMC));
#endif
#if FSL_FEATURE_PMC_HAS_BGBDS
    DEBUG_ASSERT(config->drive == PMC_BRD_REGSC_BGBDS(PMC));
#endif

    config->enable = false;
    PMC_HAL_BandgapBufferConfig(PMC, config);
    DEBUG_ASSERT(!PMC_BRD_REGSC_BGBE(PMC));
}

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    bool enableInt, enableReset;
    pmc_low_volt_detect_volt_select_t voltSelect_LVD;
    pmc_low_volt_warn_volt_select_t voltSelect_LVW;
    pmc_bandgap_buffer_config_t bandgapConfig;

    /* init debug uart */
    hardware_init();

    /* start testing */
    printf("\r\n\r\n\r\n");
    printf("\r\n------ pmc hal unit test start --------\r\n\r\n");

    /*
     * LVD TEST:
     * LVDRE could only be written once and its reset value is 1,
     * so here write it to 0 to test.
     * Another point is if other register bits of LVDSC1 are written
     * before LVDRE, then LVDRE could not be written either.
     */
    printf("LVD test now:\r\n");
    for (voltSelect_LVD =  kPmcLowVoltDetectVoltLowTrip;
         voltSelect_LVD <= kPmcLowVoltDetectVoltHighTrip;
         voltSelect_LVD++)
    {
        printf("Volt: %d\r\n", voltSelect_LVD);
        PMC_HAL_LowVoltDetectConfig(PMC, false, false, voltSelect_LVD);
        DEBUG_ASSERT(false           == (bool)PMC_BRD_LVDSC1_LVDIE(PMC));
        DEBUG_ASSERT(false           == (bool)PMC_BRD_LVDSC1_LVDRE(PMC));
        DEBUG_ASSERT(voltSelect_LVD  == (pmc_low_volt_detect_volt_select_t)PMC_BRD_LVDSC1_LVDV(PMC));
        printf("interrupt disable\r\n");

        PMC_HAL_LowVoltDetectConfig(PMC, true, false, voltSelect_LVD);
        DEBUG_ASSERT(true            == (bool)PMC_BRD_LVDSC1_LVDIE(PMC));
        DEBUG_ASSERT(false           == (bool)PMC_BRD_LVDSC1_LVDRE(PMC));
        DEBUG_ASSERT(voltSelect_LVD  == (pmc_low_volt_detect_volt_select_t)PMC_BRD_LVDSC1_LVDV(PMC));
        printf("interrupt enable\r\n");
    }

    // LVD flag test.
    voltSelect_LVD =  kPmcLowVoltDetectVoltLowTrip;
    PMC_HAL_LowVoltDetectConfig(PMC, true, false, voltSelect_LVD);
    PMC_HAL_SetLowVoltDetectAck(PMC);
    DEBUG_ASSERT(!PMC_HAL_GetLowVoltDetectFlag(PMC));
    printf("LVD test pass.\r\n");

    /*
     * LVW TEST:
     */
    printf("\r\nLVW test now:\r\n");
    for (voltSelect_LVW =  kPmcLowVoltWarnVoltLowTrip;
         voltSelect_LVW <= kPmcLowVoltWarnVoltHighTrip;
         voltSelect_LVW++)
    {
        printf("Volt: %d\r\n", voltSelect_LVW);

        PMC_HAL_LowVoltWarnConfig(PMC, false, voltSelect_LVW);
        DEBUG_ASSERT(false           == (bool)PMC_BRD_LVDSC2_LVWIE(PMC));
        DEBUG_ASSERT(voltSelect_LVW  == (pmc_low_volt_warn_volt_select_t)PMC_BRD_LVDSC2_LVWV(PMC));
        printf("interrupt disable\r\n");

        PMC_HAL_LowVoltWarnConfig(PMC, true, voltSelect_LVW);
        DEBUG_ASSERT(true            == (bool)PMC_BRD_LVDSC2_LVWIE(PMC));
        DEBUG_ASSERT(voltSelect_LVW  == (pmc_low_volt_warn_volt_select_t)PMC_BRD_LVDSC2_LVWV(PMC));
        printf("interrupt enable\r\n");
    }

    // LVD flag test.
    voltSelect_LVW =  kPmcLowVoltWarnVoltLowTrip;
    PMC_HAL_LowVoltWarnConfig(PMC, true, voltSelect_LVW);
    PMC_HAL_SetLowVoltWarnAck(PMC);
    DEBUG_ASSERT(!PMC_HAL_GetLowVoltWarnFlag(PMC));
    printf("LVW test pass\r\n");

    /* Bandgap test. */
    printf("\r\nBandgap test now:\r\n");

#if FSL_FEATURE_PMC_HAS_BGEN
    bandgapConfig.enableInLowPower = true;
#endif
#if FSL_FEATURE_PMC_HAS_BGBDS
    bandgapConfig.drive = kPmcBandgapBufferDriveHigh;
#endif
    test_bandgap(&bandgapConfig);

    printf("Bandgap test pass.\r\n");

    printf("\r\nRegulator status: %d.\r\n", PMC_HAL_GetRegulatorStatus(PMC));
    printf("ACKISO status:");
    if (PMC_HAL_GetAckIsolation(PMC))
    {
        printf("true");
    }
    else
    {
        printf("false");
    }
    printf("\r\n");

    PMC_HAL_ClearAckIsolation(PMC);
    DEBUG_ASSERT(!PMC_HAL_GetAckIsolation(PMC));

    printf("\r\n-------- PMC unit test Succeed ----------\r\n");
    while(1) {}
    return 0;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

