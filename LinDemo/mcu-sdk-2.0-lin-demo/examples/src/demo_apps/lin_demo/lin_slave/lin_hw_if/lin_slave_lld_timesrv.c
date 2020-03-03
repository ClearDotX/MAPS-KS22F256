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

/**************************************************************************//**
* @addtogroup UART_group
* @{
******************************************************************************/
/**************************************************************************//**
*
* @file      bsp/UART/lin_lld_timesrv.c
*
* @author    FPT Software
*
* @brief     Timer for KEA
*
******************************************************************************/  

/******************************************************************************
 *
 * History:
 * 2014   v1.0  First version for Kinetis timer handler
 *
 *****************************************************************************/
 
#include "lin_slave_lld_timesrv.h"
#include "lin_slave_lld_uart.h"
#include "lin_common_proto.h"
#include "lin_hw_cfg.h"
#include "fsl_pit.h"

void lin_lld_timersrv_init
(
    void
) 
{
    /* Structure of initialize PIT */
    pit_config_t pitConfig;
    IRQn_Type pit_irqs[] = PIT_IRQS;

    /* Init pit module */
    PIT_Init(PIT, &pitConfig);

    /* Set timer period for channel 1, interrupt every 500us */
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, USEC_TO_COUNT(TIME_BASE_PERIOD, PIT_SOURCE_CLOCK));

    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(pit_irqs[1]);

    PIT_StartTimer(PIT, kPIT_Chnl_1);
}

#ifdef MULTI_TIMER_MODE
void lin_lld_timersrv_irq_handler(l_u8 index)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, PIT_TFLG_TIF_MASK);
    /* Timeout for UART channel */    
    lin_lld_uart_timeout(lin_virtual_ifc[index]);    
    /*** Master task firing ***/  
}
#else /* Else MULTI_TIMER_MODE */
void lin_lld_timersrv_irq_handler(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, PIT_TFLG_TIF_MASK);
    lin_lld_uart_timeout();
}
#endif /* End MULTI_TIMER_MODE */

/**
* @}
*/