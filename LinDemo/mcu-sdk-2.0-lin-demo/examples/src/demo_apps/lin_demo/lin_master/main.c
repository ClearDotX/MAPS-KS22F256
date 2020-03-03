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
#include "app.h"
#include "lin.h"

/**********************************************************************************************
* Constants and macros
**********************************************************************************************/
#define MOTOR_SELECTION_INCREASE  1
#define MOTOR_SELECTION_DECREASE  2
#define MOTOR_SELECTION_STOP      3

#define MOTOR1_OVER_TEMP   200
#define MOTOR1_MAX_TEMP   100
#define MOTOR1_MIN_TEMP   30

/* Global variables */
static uint8_t LED_counter = 0;
static uint8_t Motor1_temp;

void lin_app_timer_irq_handler()
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, PIT_TFLG_TIF_MASK);

    /* Activate LIN frame transfer for every 5ms */
    l_sch_tick(LI0); 
    if (LED_counter >= 50)
    {
        /* Toggle LED for LIN transmission */
        /* Reset counter */
        LED_GREEN_TOGGLE();
        LED_counter = 0;
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

    /* Set timer period for channel 0, interrupt every 5ms */
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(5000U, PIT_SOURCE_CLOCK));

    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(pit_irqs[0]);

    PIT_StartTimer(PIT, kPIT_Chnl_0);

    return 0;
}

void main(void)
{
    uint8_t ret = 0;
    uint8_t j, vector_number;
    uint16_t i = 0, k = 0;
    //uart_config_t config;

    // Init board hardware.
    BOARD_InitHardware();

    ret = l_sys_init();

    ret = l_ifc_init(LI0);
    
    lin_app_init_timer();
    l_sch_set(LI0, LI0_NormalTable, 0);
	
    k = l_u16_rd_LI0_MotorSpeed();

    /* loop forever */
    for (;;)
    {    	
        if (l_flg_tst_LI0_Motor1Temp_flag())
        {
            /* Clear this flag... */
            l_flg_clr_LI0_Motor1Temp_flag();
            /* Store temperature data */
            Motor1_temp = l_u8_rd_LI0_Motor1Temp();
            /* The application will change Motor selection in case 
               the temperature is greater than maximum value to release motor power
               This will be transfered by unconditional frame type in LIN bus
            */
            if (MOTOR1_OVER_TEMP < Motor1_temp)
            {
                /* Request stop motor by power off */
                LED_RED1_ON();
                LED_RED2_OFF();
                LED_RED3_OFF();
                l_u8_wr_LI0_Motor1Selection(MOTOR_SELECTION_STOP);
            }
            else if ((MOTOR1_MAX_TEMP < Motor1_temp) & (MOTOR1_OVER_TEMP > Motor1_temp))
            {
                /* Request to reduce motor speed */
                l_u8_wr_LI0_Motor1Selection(MOTOR_SELECTION_DECREASE);  
                LED_RED1_OFF();
                LED_RED2_ON();
                LED_RED3_OFF();
            }
            else
            {
                /* Request to increase motor speed if user request */
                l_u8_wr_LI0_Motor1Selection(MOTOR_SELECTION_INCREASE);
                LED_RED1_OFF();
                LED_RED2_OFF();
                LED_RED3_ON();
            }
        }    
    }
}
