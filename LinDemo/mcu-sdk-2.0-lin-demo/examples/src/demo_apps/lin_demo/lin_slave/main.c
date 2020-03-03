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

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "fsl_pit.h"
#include "fsl_port.h"
#include "fsl_uart.h"
#include "app.h"
#include "lin.h"

/**********************************************************************************************
* Constants and macros
**********************************************************************************************/
#define MOTOR_SELECTION_INCREASE 1
#define MOTOR_SELECTION_DECREASE 2
#define MOTOR_SELECTION_STOP   3

/**********************************************************************************************
* Global variables
**********************************************************************************************/
static uint8_t LED_counter = 0;
static l_u8 Motor1_temp = 30;
static l_u8 Motor1_Selection;

void lin_app_timer_irq_handler()
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, PIT_TFLG_TIF_MASK);

    if (LED_counter >= 50)
    {
        /* Toggle LED for LIN transmission */
        /* Reset counter */
        LED_GREEN_TOGGLE();
        LED_counter = 0;
        if (Motor1_Selection == MOTOR_SELECTION_INCREASE)
        {
            Motor1_temp++; 
            LED_RED2_ON();
            LED_RED3_OFF();
        } 
        else if (Motor1_Selection == MOTOR_SELECTION_DECREASE)
        {
            Motor1_temp--;
            LED_RED2_OFF();
            LED_RED3_ON();
        }
    }
    LED_counter++;
}

static int32_t lin_app_init_timer()
{
    /* Structure of initialize PIT */
    pit_config_t pitConfig;
    IRQn_Type pit_irqs[] = PIT_IRQS;

    /* Init pit module */
    PIT_Init(PIT, &pitConfig);

    /* Set timer period for channel 0, interrupt every 2.5ms */
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(2500U, PIT_SOURCE_CLOCK));

    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(pit_irqs[0]);

    PIT_StartTimer(PIT, kPIT_Chnl_0);

    return 0;
}

void main(void)
{
    uint8_t ret = 0x55;
    uint8_t j, vector_number;
    uint16_t i = 0;
    //uart_config_t config;

    // Init board hardware.
    BOARD_InitHardware();

    ret = l_sys_init();
    ret = l_ifc_init(LI0);
    
    lin_app_init_timer();
    for (;;)
    {    	
        /* Check if temp signal is updated */
        if (l_flg_tst_LI0_Motor1Selection_flag())
        {
            /* Clear this flag... */
            l_flg_clr_LI0_Motor1Selection_flag();
            /* Store selection data */
            Motor1_Selection = l_u8_rd_LI0_Motor1Selection();
            /* The application will change Motor selection in case 
            the temperature is greater than maximum value to release motor power
            This will be transfered by unconditional frame type in LIN bus */
            l_u8_wr_LI0_Motor1Temp(Motor1_temp); 
            /* Check if power off motor due to high temperature */
            if (Motor1_Selection == MOTOR_SELECTION_STOP)
            {
                /*---------- add code here to stop motor ------------*/
            }
            if (Motor1_Selection == MOTOR_SELECTION_INCREASE)
            {
                LED_RED1_ON();
            } 
            if (Motor1_Selection == MOTOR_SELECTION_DECREASE)
            {
                LED_RED1_OFF();
            }
        }
    	/*For transport layer and diagnostic class II & III only*/
#if _TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_
        if (diag_get_flag(DIAGSRV_READ_DATA_BY_IDENTIFIER_ORDER))
        {
            l_u16 req_len = 0;
            l_u8 req_data[10];
            diag_clear_flag(DIAGSRV_READ_DATA_BY_IDENTIFIER_ORDER);
            /* Process data */
            ld_receive_message(&req_len, req_data);
            req_data[0] += 0x40;
            /* Send response */
            ld_send_message(10, req_data);
        }
#endif
    }
}
