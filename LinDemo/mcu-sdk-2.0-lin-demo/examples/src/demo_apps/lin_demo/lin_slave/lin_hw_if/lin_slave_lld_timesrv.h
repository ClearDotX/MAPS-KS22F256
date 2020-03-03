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
* @file      bsp/UART/lin_lld_timesrv.h
*
* @author    FPT Software
*
* @brief     Timer and timer interrupt for KEA
*
******************************************************************************/

/******************************************************************************
 *
 * History:
 *
 * 20140516		v1.0	First version
 *
 *****************************************************************************/  

#ifndef __LIN_LLD_TIMESRV
#define __LIN_LLD_TIMESRV

/* MODULE LIN_LLD_TIMESRV. */
#include "lin_cfg.h" 
#include "lin.h"


/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_timer_init()
* @brief Initialize TIM1 timer
*
*
* @SDD_ID LIN_SDD_114
* @endif
*
* @return #void
*
* @details
*   Initialize TIM1 timer
*
*//*END*----------------------------------------------------------------------*/
void lin_lld_timersrv_init(void);

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void lin_lld_timer_irq_handler()
* @brief Timer interrupt service routine
*
* @return #void
* 
* @SDD_ID LIN_SDD_113
* @endif
*
* @local_var
*   -# <B>#l_u8</B> <I>i</I>
*
* @see #lin_lld_uart_timeout
*
* @details
*   Timer interrupt service routine
*//*END*----------------------------------------------------------------------*/ 
#ifdef MULTI_TIMER_MODE
void lin_lld_timersrv_irq_handler(l_u8 index);
#else
void lin_lld_timersrv_irq_handler(void);
#endif /* End MULTI_TIMER_MODE */

#endif /* __LIN_LLD_TIMESRV */

/**
* @}
*/
