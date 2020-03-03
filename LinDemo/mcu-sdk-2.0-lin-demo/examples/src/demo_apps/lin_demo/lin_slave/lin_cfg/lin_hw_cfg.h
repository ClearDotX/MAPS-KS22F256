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
#ifndef      _LIN_HW_CFG_H
#define      _LIN_HW_CFG_H

typedef enum {
   UART0_ID,
   UART1_ID,
   UART2_ID
} lin_hardware_name;

/* Type of MCU */
#define _MCU_  _K_

/* Resynchronization support */
#define __RESYN_EN   0
 
/* Interface type that MCU uses */
#define XGATE_SUPPORT 0
#define _LIN_XGATE_   0 
#define _LIN_SCI_     0
#define _LIN_UART_    1 
#define _LIN_SLIC_    0
#define _LIN_GPIO_    0
/***********  SCI HARDWARE SECTION  *********/
#define NUM_OF_UART_CHANNEL    3

#define TIME_BASE_PERIOD   500

/* max idle timeout for all networks = idle_timeout_value*1000000/time_base_period */
#define _MAX_IDLE_TIMEOUT_    10000      /* idle_timeout_value = 5s */
#endif
