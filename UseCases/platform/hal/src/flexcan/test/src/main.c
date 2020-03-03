/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
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

#include "board.h"
#include "fsl_flexcan_hal.h"
#include "fsl_flexcan_hal_test.h"


#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_clock_manager.h"
#include <string.h>
#include <math.h>
#include <stdio.h>

#define DEBUG_UART_BAUD (115200)

void init_hardware(void)
{
#if defined(CPU_MK70FN1M0VMJ12) || defined(CPU_MK70FN1M0VMJ15)
    SIM->SCGC5 |= ( SIM_SCGC5_PORTA_MASK
                  | SIM_SCGC5_PORTB_MASK
                  | SIM_SCGC5_PORTC_MASK
                  | SIM_SCGC5_PORTD_MASK
                  | SIM_SCGC5_PORTE_MASK
                  | SIM_SCGC5_PORTF_MASK );

    /*Enable pins for UART2*/
    BW_PORT_PCRn_MUX(HW_PORTE, 17, 3);
    BW_PORT_PCRn_MUX(HW_PORTE, 16, 3);

    /* Enable clock gate to UART 2 module*/
    HW_SIM_SCGC4_SET(BM_SIM_SCGC4_UART2);

    /* Init uart driver for stdio.*/
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, DEBUG_UART_BAUD, kDebugConsoleUART);

    /* Configure GPIO for FlexCAN 1 peripheral function*/
    BW_PORT_PCRn_MUX(HW_PORTC, 16, 2);  /* CAN1_RX is ALT2 for pin C16*/
    BW_PORT_PCRn_MUX(HW_PORTC, 17, 2);  /* CAN1_TX is ALT2 for pin C17*/

    /* Enable clock gate to FlexCAN 1 module*/
    HW_SIM_SCGC3_SET(BM_SIM_SCGC3_FLEXCAN1);
#elif defined(CPU_MK64FN1M0VMD12)
    SIM->SCGC5 |= ( SIM_SCGC5_PORTA_MASK
                  | SIM_SCGC5_PORTB_MASK
                  | SIM_SCGC5_PORTC_MASK
                  | SIM_SCGC5_PORTD_MASK
                  | SIM_SCGC5_PORTE_MASK);

    /* UART1_RX: PTC3 */
    port_hal_mux_control(HW_PORTC, 3, kPortMuxAlt3);
    /* UART1_TX: PTC4 */
    port_hal_mux_control(HW_PORTC, 4, kPortMuxAlt3);
    
    /* Init uart driver for stdio.*/
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, DEBUG_UART_BAUD, kDebugConsoleUART);

    /* Configure GPIO for FlexCAN 0 peripheral function*/
    BW_PORT_PCRn_MUX(HW_PORTB, 19, 2);  /* CAN0_RX is ALT2 for pin B19*/
    BW_PORT_PCRn_MUX(HW_PORTB, 18, 2);  /* CAN0_TX is ALT2 for pin B18*/

    /* Enable clock gate to FlexCAN 0 module*/
    HW_SIM_SCGC6_SET(BM_SIM_SCGC6_FLEXCAN0);
#endif
}

/*! @brief FlexCAN HAL unit test main function, sets up hardware then jumps to flexcan_test().*/
int main()
{
    init_hardware();

    printf("\r\nRunning the FlexCAN HAL unit test main function.");
    return flexcan_hal_test() == kStatus_FLEXCAN_Success ? 0 : 1;
}
/*******************************************************************************
 * EOF
 ******************************************************************************/

