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

#include "lin_master_lld_uart.h"
#include "lin_hw_cfg.h"
#include "lin_common_proto.h"
#include "lin_master_lld_timesrv.h"
#include "lin_cfg.h"
#include "fsl_uart.h"
#include "app.h"

/*--------------------------------------------------------------------*/
/***** Globle variable data *****/
static uart_handle_t g_uartHandle;

extern lin_node lin_node_descrs[NUM_OF_UART_CHANNEL];
extern const l_u16 lin_max_frame_res_timeout_val[LIN_NUM_OF_IFCS][8];

#ifdef MULTI_TIMER_MODE
extern const l_u16 max_idle_timeout[LIN_NUM_OF_IFCS];
#endif /* End  MULTI_TIMER_MODE */
/***** LOW-LEVEL API *****/

void lin_lld_uart0_rx_irq_handler(void)
{
    lin_lld_uart_rx_isr(0);
}

void lin_lld_uart1_rx_irq_handler(void)
{
    lin_lld_uart_rx_isr(1);
}

void lin_lld_uart2_rx_irq_handler(void)
{
    lin_lld_uart_rx_isr(2);
}

void lin_lld_uart_init
(
    /* [IN] UART channel name */
    uart_channel_name  channel,
    /* [IN] LIN interface name*/
    l_ifc_handle iii
)
{
    lin_node *lnode_p;
    UART_Type *_pUART;
    lin_configuration *lconf_p;
    uart_config_t config;
    uart_transfer_t xfer;
    uart_transfer_t sendXfer;
    uart_transfer_t receiveXfer;

    lnode_p = (lin_node *)&lin_node_descrs[channel];
    _pUART = lnode_p->pUART;
    lconf_p = (lin_configuration *)&lin_ifc_configuration[iii];

    /* Config */
    lnode_p->ifc=(l_u8)iii;
    lnode_p->response_buffer=lconf_p->response_buffer;
    /* Set UART is Master or Slave */
    lnode_p->func=(l_bool)lconf_p->function;
    lnode_p->tbit=(l_u16)(1000000/lconf_p->baud_rate);
  
    /* Initialize UART */
    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = lconf_p->baud_rate;
    config.stopBitCount = kUART_OneStopBit;
    
    config.enableTx = false;
    config.enableRx = false;

    /* Enable use of 13bit breaks and UART frame for LIN */
    UART_Init(_pUART, &config, CLOCK_GetFreq(DEMO_UART_CLKSRC));
    _pUART->PFIFO &= ~(UART_PFIFO_TXFE_MASK | UART_PFIFO_RXFE_MASK);
    _pUART->CFIFO |= (UART_CFIFO_TXFLUSH_MASK | UART_CFIFO_RXFLUSH_MASK);
    UART_EnableTx(_pUART, 1);
    UART_EnableRx(_pUART, 1);
    UART_ClearStatusFlags(_pUART, kUART_LinBreakFlag);
    _pUART->S2 |= (UART_S2_LBKDE_MASK | UART_S2_BRK13_MASK);
    UART_EnableInterrupts(_pUART,
                          kUART_LinBreakInterruptEnable |
                          kUART_RxDataRegFullInterruptEnable |
                          kUART_FramingErrorInterruptEnable);
    UART_CreateHandle(_pUART, &g_uartHandle, NULL, NULL);

    /* Enter IDLE state */
    lin_goto_idle_state(channel);

} /* End function lin_lld_uart_init(uart_channel_name  channel, l_ifc_handle iii) */


void lin_lld_uart_deinit
(
    /* [IN] UART channel name */
    uart_channel_name  channel
)
{
    lin_node *lnode_p;
    lnode_p = (lin_node *)&lin_node_descrs[channel];
    lnode_p->state = UNINIT;
    lin_lld_uart_int_disable(channel);
} /* End function lin_lld_UART_deinit(UART_channel_name  channel) */


void lin_lld_uart_tx_header
(
    /* [IN] UART channel name */
    uart_channel_name  channel,
    /* [IN] PID to be send */
    l_u8  pid_id
)
{
    lin_node *lnode_p;
    /* Check the UART is Master ? */
    lnode_p = (lin_node *)&lin_node_descrs[channel];
    if(lnode_p->func)
        return;
    /* Make PID and put PID into the ongoing buffer */
    lnode_p->current_id = pid_id;
    lnode_p->pid = lin_process_parity(pid_id, MAKE_PARITY);
    /* Set LIN Status */
    lnode_p->state = SEND_BREAK;
    /* Send Break*/
    lnode_p->pUART->C2 |= UART_C2_SBK_MASK;
    lnode_p->pUART->C2 &= ~UART_C2_SBK_MASK;
} /* End function lin_lld_UART_tx_header(UART_channel_name  channel, l_u8  pid_id) */


void lin_lld_uart_tx_wake_up
(
    /* [IN] UART channel name */
    uart_channel_name  channel
)
{
    //l_u8 uart_flag_sr1;
    lin_node *lnode_p;
    lnode_p = (lin_node *)&lin_node_descrs[channel];
    uint8_t wake_data = 0x80;

    if((lnode_p->state == IDLE) || (lnode_p->state == SLEEP_MODE))
    {
        //uart_flag_sr1 = lnode_p->pUART->uartsr1.byte;
        /* Send wake signal byte=0x80 */
        UART_WriteNonBlocking(lnode_p->pUART, &wake_data, 1);
        /* Set Lin state to idle */
        lin_goto_idle_state(channel);
    }
}

void lin_lld_UART_int_enable
(
    /* [IN] UART channel name */
    uart_channel_name  channel
)
{
    lin_node *lnode_p;
    /* Get Lin node descriptor */
    lnode_p = (lin_node *)&lin_node_descrs[channel];

    /* Can't enable in interrupt context */
    if((lnode_p->state == PROC_CALLBACK) || (lnode_p->state==UNINIT) || (lnode_p->state==SLEEP_MODE))
        return;

    /* Enable UART Channel*/
    UART_EnableRx(lnode_p->pUART, 1);
    UART_EnableInterrupts(lnode_p->pUART, kUART_RxDataRegFullInterruptEnable);
}


void lin_lld_uart_int_disable
(
    /* [IN] UART channel name */
    uart_channel_name  channel
)
{
    lin_node *lnode_p;
    /* Get Lin node descriptor */
    lnode_p = (lin_node *)&lin_node_descrs[channel];  
    /* Can't disable in interrupt context */
    if((lnode_p->state == PROC_CALLBACK) || (lnode_p->state==UNINIT) || (lnode_p->state==SLEEP_MODE))
        return;

    while(lnode_p->state != IDLE)
        ;
    /* Disable UART Channel*/
    UART_EnableRx(lnode_p->pUART, 0);
    UART_DisableInterrupts(lnode_p->pUART, kUART_RxDataRegFullInterruptEnable);
}


void lin_lld_uart_ignore_response
(
    /* [IN] UART channel name */
	uart_channel_name  channel
)
{
    lin_goto_idle_state(channel);
}


void lin_lld_uart_set_low_power_mode
(
    /* [IN] UART channel name */
    uart_channel_name  channel
)
{
    lin_node *lnode_p;
    /* Get Lin node descriptor */
    lnode_p = (lin_node *)&lin_node_descrs[channel];

    /* Configure Hw code */

    /* Set Lin status = receiving data*/
    lnode_p->state = SLEEP_MODE;
}


void lin_lld_uart_rx_response
(
    /* [IN] UART channel name */
    uart_channel_name  channel,
    /* [IN] Length of response data expect to wait */
    l_u8  msg_length
)
{
    lin_node *lnode_p;

    /* Get Lin node descriptor */
    lnode_p = (lin_node *)&lin_node_descrs[channel];  
    /* Put response length and pointer of response buffer into descriptor */
    *(lnode_p->response_buffer) = msg_length;
    lnode_p->cnt_byte = 0;
    lnode_p->ptr = lnode_p->response_buffer;

    /* Set Lin status = receiving data*/
    lnode_p->state = RECV_DATA;
}

void lin_lld_uart_tx_response
(
    /* [IN] UART channel name */
    uart_channel_name  channel
)
{
    lin_node *lnode_p;
    lnode_p = (lin_node *)&lin_node_descrs[channel];
    uint8_t ch_data = 0x22;

    /* calculate checksum */
    lnode_p->response_buffer[*(lnode_p->response_buffer) + 1] = lin_checksum(lnode_p->response_buffer , lnode_p->pid);
    lnode_p->cnt_byte = 1;
    /* Send First byte */
    UART_WriteNonBlocking(lnode_p->pUART, &(lnode_p->response_buffer[1]), 1);
    /* Set LIN Status */
    lnode_p->state = SEND_DATA;
} /* End function lin_lld_UART_tx_response(UART_channel_name  channel) */

l_u8 lin_lld_uart_get_state
(
    /* [IN] UART channel name */
    uart_channel_name  channel
)
{
    lin_node *lnode_p;
    /* Get Lin node descriptor */
    lnode_p = (lin_node *)&lin_node_descrs[channel];
	
    return lnode_p->state;
}

void lin_lld_uart_timeout
(
    /* [IN] UART channel name */
    uart_channel_name  channel
)
{
    register lin_node *lnode_p;
#ifdef MULTI_TIMER_MODE
    l_u8 i;
#endif /* End MULTI_TIMER_MODE */
    /* multi frame support */
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
    lin_configuration * conf;
    lin_tl_descriptor *tl_conf;
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_) */
    lnode_p = (lin_node *)&lin_node_descrs[channel];
#if (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)
    conf = (lin_configuration *)&lin_ifc_configuration[(l_ifc_handle)lnode_p->ifc];
    /* Get TL configuration */
    tl_conf = conf->tl_desc;

    if (LD_CHECK_N_CR_TIMEOUT == tl_conf->tl_check_timeout_type)
    {
        if(0 == --tl_conf->tl_check_timeout)
        {
            /* switch to normal table */
            if (_MASTER_ == conf->function)
            {
                *(conf->active_schedule_id) = *(conf->previous_schedule_id);
                conf->schedule_start_entry[*(conf->active_schedule_id)] = 0;
            }
        /* update status of transport layer */
        *conf->diagnostic_mode = DIAG_NONE;
        *conf->tl_service_status = LD_SERVICE_ERROR;
        tl_conf->tl_receive_msg_status = LD_N_CR_TIMEOUT;
        tl_conf->tl_rx_msg_status = LD_N_CR_TIMEOUT;
        tl_conf->tl_check_timeout_type = LD_NO_CHECK_TIMEOUT;
        }
    }

    if (LD_CHECK_N_AS_TIMEOUT == tl_conf->tl_check_timeout_type)
    {
        if (0 == --tl_conf->tl_check_timeout)
        {
            /* switch to normal table */
            if (_MASTER_ == conf->function)
            {
                *(conf->active_schedule_id) = *(conf->previous_schedule_id);
                conf->schedule_start_entry[*(conf->active_schedule_id)] = 0;
            }
            /* update status of transport layer */
            *conf->diagnostic_mode = DIAG_NONE;
            *conf->tl_service_status = LD_SERVICE_ERROR;
            tl_conf->tl_tx_msg_status = LD_N_AS_TIMEOUT;
            tl_conf->tl_check_timeout_type = LD_NO_CHECK_TIMEOUT;
        }
    }
#endif /* End (_TL_FRAME_SUPPORT_ == _TL_MULTI_FRAME_)*/

    switch(lnode_p->state)
    {
    case IDLE:
        if (lnode_p->idle_timeout_cnt==0)
        {
            /* trigger callback */
            CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_BUS_ACTIVITY_TIMEOUT, 0xFF);
            /* goback to IDLE, reset max idle timeout */
            lnode_p->idle_timeout_cnt = _MAX_IDLE_TIMEOUT_;
            /* disable LIN break detect interrupt */
            lnode_p->pUART->S2 &= ~UART_S2_LBKDE_MASK;
        }
        else
            lnode_p->idle_timeout_cnt--;
        break;
    case SEND_PID:    /* Master */
    case RECV_SYN:
    case RECV_PID:
    case SEND_DATA:
    case SEND_DATA_COMPLETED:
        /* timeout send has occurred - change state of the node and inform core */
        if (0 == lnode_p->frame_timeout_cnt)
            lin_goto_idle_state(channel);
        else
            lnode_p->frame_timeout_cnt--;
        break;
    case RECV_DATA:
        /* timeout receive has occurred - change state of the node and inform core */
        if(0 == lnode_p->res_frame_timeout_cnt)
        {
            if(lnode_p->cnt_byte)
            {
                /* set lin status: error_in_response */
                lnode_p->l_status.byte |= LIN_STA_ERROR_RESP;
                /* trigger callback */
                CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_NODATA_TIMEOUT, lnode_p->current_id);
            }
            lin_goto_idle_state(channel);
        }
        else
            lnode_p->res_frame_timeout_cnt--;
        break;
    case PROC_CALLBACK:
        break;
    default:
        break;
    }
} /* End function lin_lld_UART_timeout(UART_channel_name  channel) */

/*** INTERNAL FUNTIONS ***/

void lin_goto_idle_state
(
    /* [IN] UART channel name */
    uart_channel_name  channel
)
{
    lin_node *lnode_p;
    lnode_p = (lin_node *)&lin_node_descrs[channel];
    /* set lin status: ~bus_activity */
    lnode_p->l_status.byte &= ~LIN_STA_BUS_ACTIVITY;
    /* Set max idle timeout */
    lnode_p->idle_timeout_cnt = _MAX_IDLE_TIMEOUT_;
    lnode_p->state = IDLE;
    /* Enable LBK detect */
    lnode_p->pUART->S2 |= UART_S2_LBKDE_MASK;
} /* End function lin_goto_idle_state(UART_channel_name  channel) */



void lin_lld_uart_rx_isr
(
    /* [IN] UART channel name */
    uart_channel_name  channel
)
{
    l_u32 uart_flag;
    l_u8 tmp_byte;

    register lin_node *lnode_p;     /* local pointer to the lin node descriptor */
    volatile UART_Type *_pUART;
    lnode_p = (lin_node *)&lin_node_descrs[channel];
    _pUART = lnode_p->pUART;

    uart_flag = UART_GetStatusFlags((UART_Type *)_pUART);

    if((uart_flag & kUART_RxActiveEdgeFlag) && (!(uart_flag & kUART_LinBreakFlag)))
    {
        /* Clear flag */
        UART_ClearStatusFlags((UART_Type *)_pUART, kUART_RxActiveEdgeFlag);
        /* check state of node is SLEEP_MODE */
        if (SLEEP_MODE == lnode_p->state)
        {
            lin_goto_idle_state(channel);
        }

        /* Enable Break interrupt */
        UART_EnableInterrupts((UART_Type *)_pUART, kUART_LinBreakInterruptEnable);
        /* Disable Active Edge interrupt */
        UART_DisableInterrupts((UART_Type *)_pUART, kUART_RxActiveEdgeInterruptEnable);

        /* Receive data not inverted */
        _pUART->S2 &= ~UART_S2_RXINV_MASK;

        return;
    }

    /* Check LBK flag */
    if (kUART_LinBreakFlag & uart_flag)
    {
        /* Clear flag */
        UART_ClearStatusFlags((UART_Type *)_pUART, kUART_LinBreakFlag);
        /* Enable Active Edge interrupt */
        UART_EnableInterrupts((UART_Type *)_pUART, kUART_RxActiveEdgeInterruptEnable);
        /* Disable Break interrupt */
        UART_DisableInterrupts((UART_Type *)_pUART, kUART_LinBreakInterruptEnable);
        /* check state of node is SLEEP_MODE */
        if (SLEEP_MODE == lnode_p->state)
        {
            lin_goto_idle_state(channel);
            return;
        }
        /* reset lin status */
        lnode_p->l_status.byte = LIN_STA_BUS_ACTIVITY;
        /* Set max frame timeout */
        lnode_p->frame_timeout_cnt  = lin_max_frame_res_timeout_val[lnode_p->ifc][7];
        /******************************
        *** 1.1 MASTER NODE: Sending SYN field
        *******************************/
        if (lnode_p->func == 0/* Master */)
        {
            lnode_p->state = SEND_PID;
            uint8_t sync_data = 0x55;
            /* Send syn field */
            UART_WriteNonBlocking((UART_Type *)_pUART, &sync_data, 1);
        }
        /******************************
        *** 1.2 SLAVE NODE: Wait for SYN
        *******************************/
        else
        {
            lnode_p->state = RECV_SYN;
        }
        /* Disable LBK detection */
        _pUART->S2 &= ~UART_S2_LBKDE_MASK;
        return;
    }
    else 
    {
        UART_ReadNonBlocking((UART_Type *)_pUART, &tmp_byte, 1);

        /******************************
        *** 4. BYTE RECIEVED
        *******************************/
        if (0 != (uart_flag & kUART_RxDataRegFullFlag))
        {
            switch(lnode_p->state)
            {
            /******************************
            *** 4.1 MASTER: Sending PID of frame
            *******************************/
            case SEND_PID:
                lnode_p->state = RECV_PID;
                /* Send PID byte */
                UART_WriteNonBlocking((UART_Type *)_pUART, &(lnode_p->pid), 1);
                break;
            /******************************
            *** 4.2 SLAVE: Receiving SYN byte
            *******************************/
            case RECV_SYN:
                if (0x55 == tmp_byte)
                {
                    lnode_p->state = RECV_PID;
                }
                else
                {
#if (LIN_PROTOCOL == PROTOCOL_J2602)
                    CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_READBACK_ERR, lnode_p->current_id);
#endif /* End of (LIN_PROTOCOL == PROTOCOL_J2602) */
                    lin_goto_idle_state(channel);
                }
                break;
                /******************************
                *** 4.3 SLAVE: Receiving PID
                *******************************/
            case RECV_PID:
                if (!lnode_p->func)
                { /*Master*/
                    /* trigger callback */
                    CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_PID_OK, lnode_p->current_id);
                    /* Set Maximum response frame timeout */
                    lnode_p->res_frame_timeout_cnt = lin_max_frame_res_timeout_val[lnode_p->ifc][*(lnode_p->response_buffer) - 1];
                }
                else
                { /* Slave node */
                    /* checkparity and extrait PID */
                    lnode_p->current_id= lin_process_parity(tmp_byte, CHECK_PARITY);
                    /* Keep the PID */
                    lnode_p->pid = tmp_byte;
                    if (lnode_p->current_id != 0xFF)
                    {
                        /*****************************************/
                        /*** ID received correctly - parity OK ***/
                        /*****************************************/
                        /* trigger callback */
                        CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_PID_OK, lnode_p->current_id);
                        /* Set Maximum response frame timeout */
                        lnode_p->res_frame_timeout_cnt = lin_max_frame_res_timeout_val[lnode_p->ifc][*(lnode_p->response_buffer) - 1];
                    }
                    else
                    {
                        /*****************************************/
                        /*** ID Parity Error                   ***/
                        /*****************************************/
                        /* set lin status: parity_error */
                        lnode_p->l_status.byte |= LIN_STA_PARITY_ERR;
                        /* trigger callback */
                        CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_PID_ERR, 0xFF);
                        lin_goto_idle_state(channel);
                    }
                }
                break;

            /******************************
            *** 4.4 SLAVE: Receiving data
            *******************************/
            case RECV_DATA:
                lnode_p->ptr++;
                *(lnode_p->ptr) = tmp_byte;
                /* Check bytes received fully */
                if (lnode_p->cnt_byte == (lnode_p->response_buffer[0]))
                {
                    /* checksum checking */
                    if (lin_checksum(lnode_p->response_buffer, lnode_p->pid) == tmp_byte)
                    {
                        /*******************************************/
                        /***  RX Buffer Full - Checksum OK       ***/
                        /*******************************************/
                        /* set lin status: successful_transfer */
                        lnode_p->l_status.byte |= LIN_STA_SUCC_TRANSFER;
                        /* disable RX interrupt */
                        UART_EnableRx((UART_Type *)_pUART, 0);
                        UART_DisableInterrupts((UART_Type *)_pUART, kUART_RxDataRegFullInterruptEnable);
                        lnode_p->state=PROC_CALLBACK;
                        /* trigger callback */
                        CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_RX_COMPLETED, lnode_p->current_id);

                        /* enable RX interrupt */
                        UART_EnableRx((UART_Type *)_pUART, 1);
                        UART_EnableInterrupts((UART_Type *)_pUART, kUART_RxDataRegFullInterruptEnable);
                        if (SLEEP_MODE != lnode_p->state)
                        {
                            lin_goto_idle_state(channel);
                        }
                    }
                    else
                    {
                        /*******************************************/
                        /***  RX Buffer Full - Checksum ERROR    ***/
                        /*******************************************/
                        /* set lin status: error_in_response, checksum_error */
                        lnode_p->l_status.byte |= (LIN_STA_ERROR_RESP|LIN_STA_CHECKSUM_ERR);
                        /* trigger callback */
                        CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_CHECKSUM_ERR, lnode_p->current_id);

                        lin_goto_idle_state(channel);
                    }
                }
                lnode_p->cnt_byte++;
                break;

            /******************************
            *** 4.5 SLAVE: Sending data
            *******************************/
            case SEND_DATA:
                /* Check for READBACK error */
                if (0 == (uart_flag & kUART_TransmissionCompleteFlag))
                {
                    CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_READBACK_ERR, lnode_p->current_id);
                    lin_goto_idle_state(channel);
                    break;
                }
                else
                {
                    if (tmp_byte != lnode_p->response_buffer[lnode_p->cnt_byte])
                    {
                        CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_READBACK_ERR, lnode_p->current_id);
                        lin_goto_idle_state(channel);
                        break;
                    }
                }

                if (lnode_p->cnt_byte <= (lnode_p->response_buffer[0]))
                {
                    /* Send data bytes and checksum */
                    lnode_p->cnt_byte++;
                    UART_WriteNonBlocking((UART_Type *)_pUART, &(lnode_p->response_buffer[lnode_p->cnt_byte]), 1);
                }
                else
                {
                    /* TX transfer complete */
                    lnode_p->l_status.byte |= LIN_STA_SUCC_TRANSFER;
                    /* Disable RX interrupt */
                    UART_EnableRx((UART_Type *)_pUART, 0);
                    UART_DisableInterrupts((UART_Type *)_pUART, kUART_RxDataRegFullInterruptEnable);
                    lnode_p->state = PROC_CALLBACK;
                    /* trigger CALLBACK */
                    CALLBACK_HANDLER((l_ifc_handle)lnode_p->ifc, LIN_LLD_TX_COMPLETED, lnode_p->current_id);
                    /* Enable RX interrupt */
                    UART_EnableRx((UART_Type *)_pUART, 1);
                    UART_EnableInterrupts((UART_Type *)_pUART, kUART_RxDataRegFullInterruptEnable);
                    lin_goto_idle_state(channel);
                }
                break;
            /******************************
            *** 4.8 SLAVE: Low power mode
            *******************************/
            case SLEEP_MODE:
                if ((tmp_byte == 0xF0) || (tmp_byte == 0xE0) || (tmp_byte == 0xC0) || (tmp_byte == 0x80) || (tmp_byte == 0x00))
                {
                    /* Set idle timeout again */
                    lin_goto_idle_state(channel);
                    /* disable LIN break detect interrupt */
                    _pUART->S2 &= ~UART_S2_LBKDE_MASK;
                }
                break;
            default:
                break;
            }
        }
    }
} /* End function lin_lld_UART_rx_isr(UART_channel_name  channel) */

/**
* @}
*/
