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
* @file      bsp/UART/lin_lld_UART.c
*
* @author    FPT Software
*
* @brief     UART for LIN network
*
******************************************************************************/

/******************************************************************************
 *
 * History:
 *
 * 20101027		v1.0	First version
 * 20111005		v1.1	Updated hardware support, multi timers
 *
 *****************************************************************************/

#include "lin_slave_lld_uart.h"
#include "lin_hw_cfg.h"
#include "lin_common_proto.h"
#include "lin_slave_lld_timesrv.h"
#include "lin_cfg.h"
#include "fsl_uart.h"

/***** Globle variable data *****/
static uart_handle_t g_uartHandle;

/* pUART func ifc checksum_mode state  l_status cnt_byte  *ptr current_pid *response_buffer
  pid_out tbit frame_timeout_cnt res_frame_timeout_cnt idle_timeout_cnt */

/**
* @var static l_u8          ifc
*/
static l_u8          ifc = 0xFF;

/**
* @var static l_u8          state
*/
static l_u8          state = UNINIT;

/**
* @var static lin_status    l_status
*/
static lin_status    l_status;

/**
* @var static l_u8          cnt_byte
*/
static l_u8          cnt_byte = 0;

/**
* @var static l_u8         *ptr
*/
static l_u8          *ptr = 0;

/**
* @var static l_u8          current_id
*/
static l_u8          current_id = 0x00;

/**
* @var static l_u8         *response_buffer
*/
static l_u8          *response_buffer = 0;

/**
* @var static l_u8          pid
*/
static l_u8          pid = 0x80;

/**
* @var static l_u16          tbit
*/
static l_u16         tbit = 0;

/**
* @var static l_u16          frame_timeout_cnt
*/
static l_u16         frame_timeout_cnt = 0;

/**
* @var static l_u16          res_frame_timeout_cnt
*/
static l_u16         res_frame_timeout_cnt = 0;

/**
* @var static l_u16          idle_timeout_cnt
*/
static l_u16         idle_timeout_cnt = 0;

extern const l_u16 lin_max_frame_res_timeout_val[8];

extern l_u8 lin_lld_response_buffer[10];



/***** LOW-LEVEL API *****/
void lin_lld_uart0_rx_irq_handler(UART_Type *base, uart_handle_t *handle, status_t status, void *userData)
{
    lin_lld_uart_rx_isr();
}

void lin_lld_uart1_rx_irq_handler(void)
{
    lin_lld_uart_rx_isr();
}

void lin_lld_uart2_rx_irq_handler(void)
{
    lin_lld_uart_rx_isr();
}

void lin_lld_uart_init
(
    /* [IN] LIN interface name*/
    l_ifc_handle iii
)
{
    uart_config_t config;
    uart_transfer_t xfer;
    uart_transfer_t sendXfer;
    uart_transfer_t receiveXfer;
    l_u16 tmp;
    /* Resyn initialization */
    /* Config */
    ifc = (l_u8)iii;
    response_buffer=lin_lld_response_buffer;

    /* Set UART is Master or Slave */
    /* Initialize UART */
    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = LIN_BAUD_RATE;
    config.stopBitCount = kUART_OneStopBit;
    
    config.enableTx = false;
    config.enableRx = false;

    /* Enable use of 13bit breaks and UART frame for LIN */
    UART_Init(DEMO_UART, &config, CLOCK_GetFreq(DEMO_UART_CLKSRC));
    DEMO_UART->PFIFO &= ~(UART_PFIFO_TXFE_MASK | UART_PFIFO_RXFE_MASK);
    DEMO_UART->CFIFO |= (UART_CFIFO_TXFLUSH_MASK | UART_CFIFO_RXFLUSH_MASK);
    UART_EnableTx(DEMO_UART, 1);
    UART_EnableRx(DEMO_UART, 1);
    UART_ClearStatusFlags(DEMO_UART, kUART_LinBreakFlag);
    DEMO_UART->S2 |= (UART_S2_LBKDE_MASK | UART_S2_BRK13_MASK);
    UART_EnableInterrupts(DEMO_UART,
                          kUART_LinBreakInterruptEnable |
                          kUART_RxDataRegFullInterruptEnable |
                          kUART_FramingErrorInterruptEnable);
    UART_CreateHandle(DEMO_UART, &g_uartHandle, NULL, NULL);

    /* Receive data not inverted */
    DEMO_UART->S2 &= ~UART_S2_RXINV_MASK;

    /* Enter IDLE state */
    lin_goto_idle_state();
} /* End of function lin_lld_uart_init( l_ifc_handle iii ) */
/*--------------------------------------------------------------------*/

void lin_lld_uart_deinit(void)
{
    state = UNINIT;
    lin_lld_uart_int_disable();
}


void lin_lld_uart_tx_wake_up(void)
{
    uint8_t wake_data = 0x80;

    if ((state == IDLE) || (state == SLEEP_MODE))
    {
        /* Send wake signal byte=0x80 */
        UART_WriteNonBlocking(DEMO_UART, &wake_data, 1);
        /* Set Lin state to idle */
        lin_goto_idle_state();
    }
/*--------------------------------------------------------------------*/
} /* End function lin_lld_UART_tx_wake_up() */


void lin_lld_uart_int_enable(void)
{
    /* Can't enable in interrupt context */
    if ((state == PROC_CALLBACK) || (state == UNINIT) || (state == SLEEP_MODE))
        return;

    /* Enable UART Channel*/
    UART_EnableRx(DEMO_UART, 1);
    UART_EnableInterrupts(DEMO_UART, kUART_RxDataRegFullInterruptEnable);
    /* Enable timeout interrupt 0 channel */
} /* End function lin_lld_UART_int_enable() */
/*--------------------------------------------------------------------*/

void lin_lld_uart_int_disable(void)
{
/*--------------------------------------------------------------------*/
  /* Can't disable in interrupt context */
    if ((state == PROC_CALLBACK) || (state == UNINIT) || (state == SLEEP_MODE))
        return;

    while (state != IDLE)
        ;
    /* Disable UART Channel*/
    UART_EnableRx(DEMO_UART, 0);
    UART_DisableInterrupts(DEMO_UART, kUART_RxDataRegFullInterruptEnable);
    /* Disable timeout interrupt 0 channel */
} /* End function lin_lld_UART_int_disable() */


void lin_lld_uart_ignore_response(void)
{
    lin_goto_idle_state();
}


void lin_lld_uart_set_low_power_mode(void)
{
    /* Configure Hw code */

    /* Set Lin status = sleep mode*/
    state = SLEEP_MODE;
}
void lin_lld_uart_rx_response
(
    /* [IN] Length of response data expect to wait */
    l_u8  msg_length
)
{
    /* Put response length and pointer of response buffer into descriptor */
    *(response_buffer) = msg_length;
    cnt_byte = 0;
    ptr = response_buffer;
    /* Set Lin status = receiving data*/
    state = RECV_DATA;
}

void lin_lld_uart_tx_response(void)
{
    /* calculate checksum */
    response_buffer[*(response_buffer) + 1] = lin_checksum(response_buffer, pid);
    cnt_byte = 1;
    /* Send First byte */
    UART_WriteNonBlocking(DEMO_UART, &response_buffer[1], 1);
    /* Set LIN Status */
    state = SEND_DATA;
} /* End function lin_lld_UART_tx_response() */


l_u8 lin_lld_uart_get_status(void)
{
    return l_status.byte;
}


l_u8 lin_lld_uart_get_state(void)
{
    return state;
}


void lin_lld_uart_timeout(void)
{
    /* Multi frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
    if (LD_CHECK_N_CR_TIMEOUT == tl_check_timeout_type)
    {
        if (0 == --tl_check_timeout)
        {
            /* update status of transport layer */
            tl_service_status = LD_SERVICE_ERROR;
            tl_receive_msg_status = LD_N_CR_TIMEOUT;
            tl_rx_msg_status = LD_N_CR_TIMEOUT;
            tl_check_timeout_type = LD_NO_CHECK_TIMEOUT;
            tl_diag_state = LD_DIAG_IDLE;
        }
    }

    if (LD_CHECK_N_AS_TIMEOUT == tl_check_timeout_type)
    {
        if (0 == --tl_check_timeout)
        {
            /* update status of transport layer */
            tl_service_status = LD_SERVICE_ERROR;
            tl_tx_msg_status = LD_N_AS_TIMEOUT;
            tl_check_timeout_type = LD_NO_CHECK_TIMEOUT;
            tl_diag_state = LD_DIAG_IDLE;
        }
    }
#endif /* END (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)  */

    switch(state)
    {
    case IDLE:
        if (idle_timeout_cnt == 0)
        {
            /* Set LIN mode to sleep mode */
            lin_goto_sleep_flg = 1;
            /* trigger callback */
            CALLBACK_HANDLER((l_ifc_handle)ifc, LIN_LLD_BUS_ACTIVITY_TIMEOUT, 0xFF);
            /* goback to IDLE, reset max idle timeout */
            idle_timeout_cnt = _MAX_IDLE_TIMEOUT_;
            /* disable LIN break detect interrupt */
            DEMO_UART->S2 &= ~UART_S2_LBKDE_MASK;
            /* Set state to sleep mode */
            state = SLEEP_MODE;
        }
        else
            idle_timeout_cnt--;
      break;
    case SEND_PID:    /* Master */
    case RECV_SYN:
    case RECV_PID:
    case SEND_DATA:
    case SEND_DATA_COMPLETED:
        /* timeout send has occurred - change state of the node and inform core */
        if(0 == frame_timeout_cnt)
            lin_goto_idle_state();
        else
            frame_timeout_cnt--;
        break;
    case RECV_DATA:
        /* timeout receive has occurred - change state of the node and inform core */
        if (0 == res_frame_timeout_cnt)
        {
            if(cnt_byte)
            {
                /* set lin status: error_in_response */
                l_status.byte |= LIN_STA_ERROR_RESP;
                /* trigger callback */
                CALLBACK_HANDLER((l_ifc_handle)ifc, LIN_LLD_NODATA_TIMEOUT, current_id);
            }
            lin_goto_idle_state();
        }
        else
            res_frame_timeout_cnt--;
        break;
    case PROC_CALLBACK:
        break;
    default:
        ;
    }
} /* End function lin_lld_UART_timeout() */


/*** INTERNAL FUNTIONS ***/
void lin_goto_idle_state(void)
{
    /* set lin status: ~bus_activity */
    l_status.byte &= ~LIN_STA_BUS_ACTIVITY;
    /* Set max idle timeout */
    idle_timeout_cnt = _MAX_IDLE_TIMEOUT_;
    state = IDLE;
    /* Enable LBK detect */
    DEMO_UART->S2 |= UART_S2_LBKDE_MASK;
} /* End function lin_goto_idle_state() */

void lin_lld_uart_rx_isr(void)
{
    l_u32 uart_flag;
    l_u8 tmp_byte;

    /******************************
    *** 1. BREAK DETECTED
    *******************************/
    /* if the lbkdif is set */
    uart_flag = UART_GetStatusFlags(DEMO_UART);

    if((uart_flag & kUART_RxActiveEdgeFlag) && (!(uart_flag & kUART_LinBreakFlag)))
    {
        /* Clear flag */
        UART_ClearStatusFlags(DEMO_UART, kUART_RxActiveEdgeFlag);
        /* Disable rx edged detection */
        UART_DisableInterrupts(DEMO_UART, kUART_RxActiveEdgeInterruptEnable);
  	 /* End Resynchronization */

        if (SLEEP_MODE == state)
        {
            lin_goto_idle_state();
            lin_goto_sleep_flg = 0;
        }

        /* Enable Break interrupt */
        UART_EnableInterrupts(DEMO_UART, kUART_LinBreakInterruptEnable);

        /* Receive data not inverted */
        DEMO_UART->S2 &= ~UART_S2_RXINV_MASK;
        /* check state of node is SLEEP_MODE */
  	return;
    }
    /******************************
    *** 3. FRAME ERROR DETECTED
    *******************************/
    if (0 != (uart_flag & kUART_FramingErrorFlag))
    {
        /* Clear FE flags */
        UART_ClearStatusFlags(DEMO_UART, kUART_FramingErrorFlag);
        UART_ReadNonBlocking(DEMO_UART, &tmp_byte, 1);
        /* set lin status: error_in_response, framing_error */
        l_status.byte |= (LIN_STA_ERROR_RESP | LIN_STA_FRAME_ERR);
        /* trigger callback */
        if ((state == RECV_DATA)||(state == SEND_DATA) || (state == SEND_DATA_COMPLETED))
        {
            CALLBACK_HANDLER((l_ifc_handle)ifc, LIN_LLD_FRAME_ERR, current_id);		
        }
        lin_goto_idle_state();	
    } 
  
    if(uart_flag & kUART_LinBreakFlag)
    {
        /* Clear flag */
        UART_ClearStatusFlags(DEMO_UART, kUART_LinBreakFlag);
        /* Enable Active Edge interrupt */
        UART_EnableInterrupts(DEMO_UART, kUART_RxActiveEdgeInterruptEnable);
        /* Disable Break interrupt */
        UART_DisableInterrupts(DEMO_UART, kUART_LinBreakInterruptEnable);
        /* check state of node is SLEEP_MODE */
        if (SLEEP_MODE == state)
        {
            lin_goto_idle_state();
            return;
        }
        /* reset lin status */
        l_status.byte = LIN_STA_BUS_ACTIVITY;
        /* Set max frame timeout */
        frame_timeout_cnt = lin_max_frame_res_timeout_val[7];
        /******************************
        *** 1.2 SLAVE NODE: Wait for SYN
        *******************************/
        /* Start resyn */
        /* Enable Active Edge interrupt */
        UART_EnableInterrupts(DEMO_UART, kUART_RxActiveEdgeInterruptEnable);

        state = RECV_SYN;
        /* Disable LBK detect */
        DEMO_UART->S2 &= ~UART_S2_LBKDE_MASK;
        return;
    }
    else
    {
        UART_ReadNonBlocking(DEMO_UART, &tmp_byte, 1);
        /******************************
        *** 4. BYTE RECIEVED
        *******************************/
        if(0 != (uart_flag & kUART_RxDataRegFullFlag))
        {
            switch(state)
            {
            /******************************
            *** 4.2 SLAVE: Receiving SYN byte
            *******************************/
            case RECV_SYN:
                if (0x55 == tmp_byte)
                {
                    state = RECV_PID;
                }
                else
                {
#if (LIN_PROTOCOL == PROTOCOL_J2602)
                    CALLBACK_HANDLER((l_ifc_handle)ifc, LIN_LLD_READBACK_ERR, current_id);
#endif /* End of (LIN_PROTOCOL == PROTOCOL_J2602) */
                    lin_goto_idle_state();
                }
                break;
            /******************************
            *** 4.3 SLAVE: Receiving PID
            *******************************/
            case RECV_PID:
                /* checkparity and extrait PID */
                current_id = lin_process_parity(tmp_byte, CHECK_PARITY);
                /* Keep the PID */
                pid = tmp_byte;
                if (current_id != 0xFF)
                {
                    /*****************************************/
                    /*** ID received correctly - parity OK ***/
                    /*****************************************/
                    /* trigger callback */
                    CALLBACK_HANDLER((l_ifc_handle)ifc, LIN_LLD_PID_OK, current_id);
                    /* Set Maximum response frame timeout */
                    res_frame_timeout_cnt = lin_max_frame_res_timeout_val[*(response_buffer) - 1];
                }
                else
                {
                    /*****************************************/
                    /*** ID Parity Error                   ***/
                    /*****************************************/
                    /* set lin status: parity_error */
                    l_status.byte |= LIN_STA_PARITY_ERR;
                    /* trigger callback */
                    CALLBACK_HANDLER((l_ifc_handle)ifc, LIN_LLD_PID_ERR, 0xFF);
                    lin_goto_idle_state();
                }
                break;
            /******************************
            *** 4.4 SLAVE: Receiving data
            *******************************/
            case RECV_DATA:
                ptr++;
                *(ptr) = tmp_byte;
                /* Check bytes received fully */
                if (cnt_byte == (response_buffer[0]))
                {
                    /* checksum checking */
                    if (lin_checksum(response_buffer, pid) == tmp_byte)
                    {
                        /*******************************************/
                        /***  RX Buffer Full - Checksum OK       ***/
                        /*******************************************/
                        /* set lin status: successful_transfer */
                        l_status.byte |= LIN_STA_SUCC_TRANSFER;
                        /* disable RX interrupt */
                        UART_EnableRx(DEMO_UART, 0);
                        UART_DisableInterrupts(DEMO_UART, kUART_RxDataRegFullInterruptEnable);
                        state = PROC_CALLBACK;
                        /* trigger callback */
                        CALLBACK_HANDLER((l_ifc_handle)ifc, LIN_LLD_RX_COMPLETED, current_id);

                        /* enable RX interrupt */
                        UART_EnableRx(DEMO_UART, 1);
                        UART_EnableInterrupts(DEMO_UART, kUART_RxDataRegFullInterruptEnable);
                        if (SLEEP_MODE != state)
                        {
                            lin_goto_idle_state();
                        }
                    }
                else
                {
                    /*******************************************/
                    /***  RX Buffer Full - Checksum ERROR    ***/
                    /*******************************************/
                    /* set lin status: error_in_response, checksum_error */
                    l_status.byte |= (LIN_STA_ERROR_RESP | LIN_STA_CHECKSUM_ERR);
                    /* trigger callback */
                    CALLBACK_HANDLER((l_ifc_handle)ifc, LIN_LLD_CHECKSUM_ERR, current_id);
                    lin_goto_idle_state();
                }
            }
            cnt_byte++;
            break;

            /******************************
            *** 4.5 SLAVE: Sending data
            *******************************/
            case SEND_DATA:
                /* Check for READBACK error */
                if (0 == (uart_flag & kUART_TransmissionCompleteFlag))
                {
                    CALLBACK_HANDLER((l_ifc_handle)ifc, LIN_LLD_READBACK_ERR, current_id);
                    lin_goto_idle_state();
                    break;
                }
                else
                {
                    if (tmp_byte != response_buffer[cnt_byte])
                    {
                        /* Check if event trigger frame then allow to continue sending data */
                        CALLBACK_HANDLER((l_ifc_handle)ifc, LIN_LLD_READBACK_ERR, current_id);
                        lin_goto_idle_state();
                        break;               
                    }//End if (tmp_byte != response_buffer[cnt_byte]){
                }
                if (cnt_byte <= (response_buffer[0]))
                {
                    /* Send data bytes and checksum */
                    cnt_byte++;
                    UART_WriteNonBlocking(DEMO_UART, &(response_buffer[cnt_byte]), 1);
                }
                else
                {
                    /* TX transfer complete */
                    l_status.byte |= LIN_STA_SUCC_TRANSFER;
                    /* Disable RX interrupt */
                    UART_EnableRx(DEMO_UART, 0);
                    UART_DisableInterrupts(DEMO_UART, kUART_RxDataRegFullInterruptEnable);
                    state = PROC_CALLBACK;
                    /* trigger CALLBACK */
                    CALLBACK_HANDLER((l_ifc_handle)ifc, LIN_LLD_TX_COMPLETED, current_id);
                    /* Enable RX interrupt */
                    UART_EnableRx(DEMO_UART, 1);
                    UART_EnableInterrupts(DEMO_UART, kUART_RxDataRegFullInterruptEnable);
                    lin_goto_idle_state();
            }
            break;
            /******************************
            *** 4.8 SLAVE: Low power mode
            *******************************/
            case SLEEP_MODE:
                if ((tmp_byte == 0xF0) || (tmp_byte == 0xE0) || (tmp_byte == 0xC0) || (tmp_byte == 0x80) || (tmp_byte == 0x00))
                {
                    /* Set idle timeout again */
                    lin_goto_idle_state();
                    /* Enable LIN break detect interrupt */
                    DEMO_UART->S2 |= UART_S2_LBKDE_MASK;
                }
                break;
            default:
                break;
            }
        }
    }
} /* End function lin_lld_UART_rx_isr() */

/**
* @}
*/
