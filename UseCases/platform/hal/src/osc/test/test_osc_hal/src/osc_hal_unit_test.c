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

#include "osc_hal_unit_test.h"
#include "fsl_osc_hal.h"
#include "fsl_clock_manager.h"

/*
 * README:
 * Please set baud rate to 9600!!!!
 * Please make sure OSC is not used as clock source in this test.
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER
#define OSC_DIV_MAX (1 << OSC_DIV_ERPS_WIDTH)
#endif
#define OSC_SC_MASK (OSC_CR_SC2P_MASK | OSC_CR_SC4P_MASK | OSC_CR_SC8P_MASK | OSC_CR_SC16P_MASK)

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

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
int main(void)
{
    OSC_Type * base;
    uint32_t i;
    osc_capacitor_config_t sc_config;

    hardware_init();

    // start testing
    printf("\r\n\r\n\r\n");
    printf("\r\n------ osc hal unit test start --------\r\n\r\n");

    for (i=0; i<OSC_INSTANCE_COUNT; i++)
    {
        base = g_oscBase[i];

        printf("\r\n--------- Test OSC %d -----------\r\n", i);

        printf("The initial OSC CR reg value = 0x%04x\r\n" , OSC_RD_CR(base));

        printf("Test OSC_CR[ERCLKEN]\r\n");
        OSC_HAL_SetExternalRefClkCmd(base, true);
        DEBUG_ASSERT(OSC_HAL_GetExternalRefClkCmd(base));
        OSC_HAL_SetExternalRefClkCmd(base, false);
        DEBUG_ASSERT(!OSC_HAL_GetExternalRefClkCmd(base));

        printf("Test OSC_CR[EREFSTEN]\r\n");
        OSC_HAL_SetExternalRefClkInStopModeCmd(base, true);
        DEBUG_ASSERT(OSC_BRD_CR_EREFSTEN(base));
        OSC_HAL_SetExternalRefClkInStopModeCmd(base, false);
        DEBUG_ASSERT(!OSC_BRD_CR_EREFSTEN(base));

        printf("Test Capacitor Load configuration\r\n");
        for (i=0; i<=OSC_SC_MASK; i++)
        {
            OSC_HAL_SetCapacitor(base, i);
            printf("OSC CR reg value = 0x%04x\r\n" , OSC_RD_CR(base));
            DEBUG_ASSERT(i == (OSC_RD_CR(base)&OSC_SC_MASK));
        }

#if FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER
        printf("Test OSC_DIV[ERPS]\r\n");
        for (i=0; i<OSC_DIV_MAX; i++)
        {
            OSC_HAL_SetExternalRefClkDiv(base, i);
            DEBUG_ASSERT(i == OSC_HAL_GetExternalRefClkDiv(base));
        }
#endif
    }

    printf("\r\n-------- OSC HAL unit test Succeed ----------\r\n");
    while(1) { }
    return 0;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/


