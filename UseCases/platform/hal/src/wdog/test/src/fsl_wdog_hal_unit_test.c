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
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "fsl_wdog_hal.h"
#include "fsl_wdog_hal_unit_test.h"
#include "fsl_clock_manager.h"
#include "fsl_os_abstraction.h"
#include "fsl_debug_console.h"
#include "board.h"

/* used to record watchdog interrupt */
#define MAGIC_VALUE (0x55AAU)
#define MAGIC_VALUE_BUF (*((uint32_t*)0x1FFFFF00)) /* buffer in RAM used in interrupt callback function */

extern WDOG_Type * const g_wdogBase[];
static wdog_test_status_t s_wdogTestStatus;

void wait_wct_close(void)
{
    OSA_TimeDelay(50);
}

int main(void)
{
    /* initialize the twr board */
    WDOG_Type *base = g_wdogBase[0];
    
    wdog_work_mode_t initWorkmodeTrue =
    {
      .kWdogEnableInWaitMode  = true,
      .kWdogEnableInStopMode  = true,
      .kWdogEnableInDebugMode = true,
    };
    
    wdog_config_t initConfigTrue = 
    {
      .wdogEnable   = true,
      .clkSrc       = kWdogAlternateClkSrc,
      .prescaler    = kWdogClkPrescalerDivide2,
      .workMode     = initWorkmodeTrue,
      .updateEnable = true,
      .intEnable    = true,
      .winEnable    = true,
      .windowValue  = 0x10000001,
      .timeoutValue = 0xFFFFFFFF,
    };
   
    OSA_Init();
    hardware_init();
    dbg_uart_init();
    memset(&s_wdogTestStatus, 0, sizeof(s_wdogTestStatus));
    
    WDOG_HAL_Unlock(base);
    WDOG_HAL_SetConfig(base, &initConfigTrue);
    wait_wct_close();
    if(0xFF != (uint8_t)WDOG_RD_STCTRLH(base))
    {
        s_wdogTestStatus.B.testStatusConfig = 1;
    }
    if(0xFFFF != (WDOG_RD_TOVALH(base) | 0xFFFF != WDOG_RD_TOVALL(base)))
    {
        s_wdogTestStatus.B.testStatusSetTimeoutValue = 1;
    }
    if(!(WDOG_RD_WINH(base) | false != WDOG_RD_WINL(base)))
    {
        s_wdogTestStatus.B.testStatusSetWindowValue = 1;
    }
    if(true != WDOG_RD_PRESC_PRESCVAL(base))
    {
        s_wdogTestStatus.B.testStatusSetClockPrescaler = 1;
    }
    if(s_wdogTestStatus.all)
    {
        printf("wdog HAL true test fail\r\n");
        return 1;
    }
    printf("wdog HAL true test success\r\n");
    
    wdog_work_mode_t initWorkmodeFalse =
    {
      .kWdogEnableInWaitMode  = false,
      .kWdogEnableInStopMode  = false,
      .kWdogEnableInDebugMode = false,
    };
    
    wdog_config_t initConfigFalse = 
    {
      .wdogEnable   = false,
      .clkSrc       = kWdogLpoClkSrc,
      .prescaler    = kWdogClkPrescalerDivide1,
      .workMode     = initWorkmodeFalse,
      .updateEnable = false,
      .intEnable    = false,
      .winEnable    = false,
      .windowValue  = 0,
      .timeoutValue = 0x01,
    };
    
    WDOG_HAL_Unlock(base);
    WDOG_HAL_SetConfig(base, &initConfigFalse);
    wait_wct_close();
    if(false != (uint8_t)WDOG_RD_STCTRLH(base))
    {
        s_wdogTestStatus.B.testStatusConfig = 1;
    }
    if(0x00 != WDOG_RD_TOVALH(base) || 0x01 != WDOG_RD_TOVALL(base))
    {
        s_wdogTestStatus.B.testStatusSetTimeoutValue = 1;
    }
    if(false != WDOG_RD_WINH(base) || false != WDOG_RD_WINL(base))
    {
        s_wdogTestStatus.B.testStatusSetWindowValue = 1;
    }
    if(false != WDOG_RD_PRESC_PRESCVAL(base))
    {
        s_wdogTestStatus.B.testStatusSetClockPrescaler = 1;
    }
    if(s_wdogTestStatus.all)
    {
        printf("wdog HAL false test fail\r\n");
        return 1;
    }
    printf("wdog HAL false test success\r\n");
    
    
}

/*******************************************************************************
 * EOF
 *******************************************************************************/

