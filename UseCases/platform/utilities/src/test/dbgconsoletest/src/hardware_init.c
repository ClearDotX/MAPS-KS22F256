/*
 * Copyright (c) 2013-2014, Freescale Semiconductor, Inc.
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

#include "fsl_device_registers.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_clock_manager.h"
#include "fsl_debug_console.h"
#include <stdbool.h>

extern uint32_t g_xtal0ClkFreq;
#ifdef BOARD_RTC_CLK_FREQUENCY
    extern uint32_t g_xtalRtcClkFreq;
#endif

void hardware_init(void) {

  uint8_t i;

  /* enable clock for PORTs */
  for (i = 0; i < PORT_INSTANCE_COUNT; i++)
  {
    CLOCK_SYS_EnablePortClock(i);
  }

  /* Setup board clock source. */
  g_xtal0ClkFreq = BOARD_XTAL0_CLK_FREQUENCY;
#ifdef BOARD_RTC_CLK_FREQUENCY
  g_xtalRtcClkFreq = BOARD_RTC_CLK_FREQUENCY;
#endif

  for (i = 0; i < PORT_INSTANCE_COUNT; i++)
  {
    configure_gpio_pins(i);
  }
}

#ifdef BOARD_USE_UART
void dbg_uart_init(void)
    {
        // Set up debug console for UART
        configure_uart_pins(BOARD_DEBUG_UART_INSTANCE);
        DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUD, kDebugConsoleUART);
    }
#endif
#ifdef BOARD_USE_LPUART
void dbg_uart_init(void)
    {
        // Set up debug console for LPUART
#ifdef FRDM_KL03Z
        configure_uart_pins(BOARD_DEBUG_UART_INSTANCE);
#else
        configure_lpuart_pins(BOARD_DEBUG_UART_INSTANCE);
#endif
        DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUD, kDebugConsoleLPUART);
    }
#endif
#ifdef BOARD_USE_LPSCI
void dbg_uart_init(void)
    {
        // Set up debug console for LPSCI
        configure_lpsci_pins(BOARD_DEBUG_UART_INSTANCE);
        // Select different clock source for LPSCI. */
#if ((CLOCK_SETUP == 2) || (CLOCK_SETUP == 3))
        CLOCK_SYS_SetLpsciSrc(BOARD_DEBUG_UART_INSTANCE, kClockLpsciSrcMcgIrClk);
#else
        CLOCK_SYS_SetLpsciSrc(BOARD_DEBUG_UART_INSTANCE, kClockLpsciSrcPllFllSel);
#endif
        DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUD, kDebugConsoleLPSCI);
    }
#endif
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.4 [05.10]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
