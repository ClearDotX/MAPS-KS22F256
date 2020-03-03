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
#include "fsl_lptmr_hal.h"
#include "fsl_lptmr_hal_unit_test.h"
#include "fsl_clock_manager.h"
#include "fsl_debug_console.h"
#include "board.h"

static lptmr_test_status_t s_lptmrTestStatus;
extern LPTMR_Type * const g_lptmrBase[];

uint8_t delay_some_time(uint32_t count)
{
    uint32_t i,j;
    for(i=0;i<count;i++)
    {
        for(j=0;j<1000;j++)
          __NOP();
    }
    return 0;
}

void main(void)
{
    uint32_t instance;
    LPTMR_Type* baseAddr;
    lptmr_prescaler_user_config_t prescaler_config;
    lptmr_working_mode_user_config_t lptmr_mode_config;
    

    /* initialize the twr board */
    hardware_init();
    dbg_uart_init();

    memset(&s_lptmrTestStatus, 0, sizeof(s_lptmrTestStatus));

    for(instance=0; instance < LPTMR_INSTANCE_COUNT; instance++)
    {
        baseAddr = g_lptmrBase[instance];

        /* enable clock for lptmr */
        CLOCK_SYS_EnableLptmrClock(instance);

        /* Set All value to True */
        LPTMR_HAL_SetIntCmd(baseAddr, true);
        if ( true !=  LPTMR_RD_CSR_TIE(baseAddr))
        {
            s_lptmrTestStatus.B.testStatusSetIntCmd = 1;
        }

        lptmr_mode_config.freeRunningEnable = true;
        lptmr_mode_config.pinPolarity = kLptmrPinPolarityActiveHigh;
        lptmr_mode_config.pinSelect = kLptmrPinSelectInput1;
        lptmr_mode_config.timerModeSelect = kLptmrTimerModeTimeCounter;
        LPTMR_HAL_SetTimerWorkingMode(baseAddr, lptmr_mode_config);
        
        if ( kLptmrPinSelectInput1 != LPTMR_BRD_CSR_TPS(baseAddr))
        {
            s_lptmrTestStatus.B.testStatusPinSelect = 1;
        }

        if ( (uint32_t)kLptmrPinPolarityActiveHigh != LPTMR_BRD_CSR_TPP(baseAddr))
        {
            s_lptmrTestStatus.B.testStatusPinPolatiry = 1;
        }

        if ( true != LPTMR_BRD_CSR_TFC(baseAddr))
        {
            s_lptmrTestStatus.B.testStatusSetFreeRunningCmd = 1;
        }

        if ( kLptmrTimerModeTimeCounter != LPTMR_BRD_CSR_TMS(baseAddr))
        {
            s_lptmrTestStatus.B.testStatusTimerMode = 1;
        }

        prescaler_config.prescalerBypass = false;
        prescaler_config.prescalerClockSelect = (lptmr_prescaler_clock_select_t)kClockLptmrSrcLpoClk;
        prescaler_config.prescalerValue = kLptmrPrescalerDivide65536GlitchFilter32768;
        LPTMR_HAL_SetPrescalerMode(baseAddr, prescaler_config);
        
        if ( kLptmrPrescalerDivide65536GlitchFilter32768 !=  LPTMR_BRD_PSR_PRESCALE(baseAddr))
        {
            s_lptmrTestStatus.B.testStatusPrescalerValue = 1;
        }

        if ( false != LPTMR_BRD_PSR_PBYP(baseAddr))
        {
            s_lptmrTestStatus.B.testStatusSetPrescalerCmd = 1;
        }

        if ( kClockLptmrSrcLpoClk !=  LPTMR_BRD_PSR_PCS(baseAddr))
        {
            s_lptmrTestStatus.B.testStatusPrescaleClockSource = 1;
        }

        LPTMR_HAL_SetCompareValue(baseAddr, 0xFF);
        if ( 0xFF !=  LPTMR_HAL_GetCompareValue(baseAddr))
        {
            s_lptmrTestStatus.B.testStatusCompareValue = 1;
        }

        LPTMR_HAL_Enable(baseAddr);
        if ( true !=  LPTMR_BRD_CSR_TEN(baseAddr))
        {
            s_lptmrTestStatus.B.testStatusEnable = 1;
        }

        LPTMR_HAL_Disable(baseAddr);

        if (s_lptmrTestStatus.all)
        {
            printf("\r\nInstance %d: LPTMR HAL all true test FAILED", instance);
            while(1); /* Stop when failed */
        }
        else
        {
            printf("\r\nInstance %d: LPTMR HAL all true test PASSED", instance);
        }

        delay_some_time(5000);
        memset(&s_lptmrTestStatus, 0, sizeof(s_lptmrTestStatus));

        /* Set All value to False */
        LPTMR_HAL_ClearIntFlag(baseAddr);
        if ( false !=  LPTMR_HAL_IsIntPending(baseAddr))
        {
            s_lptmrTestStatus.B.testStatusClearIntFlag = 1;
        }

        LPTMR_HAL_SetIntCmd(baseAddr, false);
        if ( false !=  LPTMR_BRD_CSR_TIE(baseAddr))
        {
            s_lptmrTestStatus.B.testStatusSetIntCmd = 1;
        }

        LPTMR_HAL_SetCompareValue(baseAddr, 0x0);
        if ( 0x0 !=  LPTMR_HAL_GetCompareValue(baseAddr))
        {
            s_lptmrTestStatus.B.testStatusCompareValue = 1;
        }

        LPTMR_HAL_Disable(baseAddr);
        if ( false !=  LPTMR_BRD_CSR_TEN(baseAddr))
        {
            s_lptmrTestStatus.B.testStatusDisable = 1;
        }

        if (s_lptmrTestStatus.all)
        {
            printf("\r\nInstance %d: LPTMR HAL all false test FAILED",instance);
            while(1); /* Stop when failed */
        }
        else
        {
            printf("\r\nInstance %d: LPTMR HAL all false test PASSED",instance);
        }
        printf("\r\nInstance %d: LPTMR HAL Test Succeed",instance);
    }
}

/*******************************************************************************
 * EOF
 *******************************************************************************/

