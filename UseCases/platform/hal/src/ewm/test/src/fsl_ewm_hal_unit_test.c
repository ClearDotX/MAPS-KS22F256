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

#define EWM_HAL_TEST_FALSE        1

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "fsl_ewm_hal_unit_test.h"
#include "fsl_interrupt_manager.h"
#include "fsl_clock_manager.h"
#include "fsl_os_abstraction.h"
#include "fsl_debug_console.h"
#include "board.h"

static uint32_t isrCount = 0;
extern EWM_Type * const g_ewmBase[];
extern const IRQn_Type g_ewmIrqId[];
static ewm_test_status_t s_ewmTestStatus;

int main(void)
{
    /* initialize the twr board */
    EWM_Type * base = g_ewmBase[0];
    s_ewmTestStatus.all = (uint8_t)0;
    OSA_Init();
    hardware_init();
    dbg_uart_init();
    CLOCK_SYS_EnableEwmClock(0);
#if EWM_HAL_TEST_FALSE
    ewm_config_t testCommonConfig = 
    {
       .ewmEnable        = false,
       .ewmInEnable      = false,
       .ewmInAssertLogic = false,  /*!< Configures the ewm_in assert signal in logic zero as the reset value */
       .intEnable        = false,
#if FSL_FEATURE_EWM_HAS_PRESCALER
       .ewmPrescalerValue = 0,
#endif 
       .ewmCmpLowValue  = 0x00,
       .ewmCmpHighValue = 0x00,
    };
    
    EWM_HAL_SetConfig(base, &testCommonConfig);
    if(false != EWM_RD_CTRL(base))
    {
        s_ewmTestStatus.B.testEwmConfigStatus = 1;
    }
    if(false != EWM_RD_CMPL(base) || false != EWM_RD_CMPH(base))
    {
        s_ewmTestStatus.B.testSetCmpRegStatus = 1;
    }
#if FSL_FEATURE_EWM_HAS_PRESCALER
    if(false != EWM_RD_CLKPRESCALER(base))
    {
        s_ewmTestStatus.B.testSetPrescalerValueStatus =1;
    }
#endif
    if(false != EWM_RD_CTRL_INTEN(base))
    {
        s_ewmTestStatus.B.testSetIntStatus = 1;
    }
    if(s_ewmTestStatus.all)
    {
      printf("EWM HAL false test fail\r\n");
      return 1;
    }
    printf("EWM HAL false test sucess\r\n");
#else
    ewm_config_t testCommonConfig = 
    {
       .ewmEnable        = true,
       .ewmInEnable      = true,
       .ewmInAssertLogic = true,  /*!< Configures the ewm_in assert signal in logic zero as the reset value */
       .intEnable        = true,
#if FSL_FEATURE_EWM_HAS_PRESCALER
       .ewmPrescalerValue = 1,
#endif 
       .ewmCmpLowValue  = 0x01,
       .ewmCmpHighValue = 0xfe,
    };
    EWM_HAL_SetConfig(base, &testCommonConfig);
    if(!EWM_RD_CTRL(base))
    {
        s_ewmTestStatus.B.testEwmConfigStatus = 1;
    }
    if(0x01 != EWM_RD_CMPL(base) || 0xfe != EWM_RD_CMPH(base))
    {
        s_ewmTestStatus.B.testSetCmpRegStatus = 1;
    }
#if FSL_FEATURE_EWM_HAS_PRESCALER
    if(!EWM_RD_CLKPRESCALER(base))
    {
        s_ewmTestStatus.B.testSetPrescalerValueStatus =1;
    }
#endif
    if(true != EWM_RD_CTRL_INTEN(base))
    {
        s_ewmTestStatus.B.testSetIntStatus = 1;
    }
    if(s_ewmTestStatus.all)
    {
      printf("EWM HAL true test fail\r\n");
      return 1;
    }
    printf("EWM HAL true test sucess\r\n");
    EWM_HAL_SetIntCmd(base, 0);
#endif
}
/*******************************************************************************
 * EOF
 *******************************************************************************/

