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

#include "lin_cfg.h"
#include "lin.h"
/* Mapping interface with hardware */
 
const lin_hardware_name lin_virtual_ifc[LIN_NUM_OF_IFCS] = { UART0_ID };
/* Low level response buffer */
l_u8 lin_lld_response_buffer[LIN_NUM_OF_IFCS][10];
 
 /* Successful transfer flags */
l_u8 lin_successful_transfer[LIN_NUM_OF_IFCS]; 
/* Error in response */
l_u8 lin_error_in_response[LIN_NUM_OF_IFCS]; 
/* Goto sleep flag */
l_u8 lin_goto_sleep_flg[LIN_NUM_OF_IFCS];
/* Save configuration flag */
l_u8 lin_save_configuration_flg[LIN_NUM_OF_IFCS] = {0};
 /* Next transmit tick */
l_u8 lin_next_transmit[LIN_NUM_OF_IFCS];
 /* lin word status */
lin_word_status_str lin_word_status[LIN_NUM_OF_IFCS]; 
/* current pid */
l_u8 lin_current_pid[LIN_NUM_OF_IFCS];
 

/* definition and initialization of signal array */
l_u8 lin_pFrameBuf[LIN_FRAME_BUF_SIZE] =
{
    0xfc, /* 0 : 11111100 */ /* start of frame LI0_Motor1Control */
    0x05, /* 1 : 00000101 */ /* start of frame LI0_Motor1State_Cycl */
    0x00, /* 2 : 00000000 */
    0x00, /* 3 : 00000000 */
    0x00, /* 4 : 00000000 */
    0x00, /* 5 : 00000000 */
    0xfe, /* 6 : 11111110 */
    0xff, /* 7 : 11111111 */ /* start of frame LI0_Motor1State_Event */
    0x05, /* 8 : 00000101 */
    0x01, /* 9 : 00000001 */
    0xfc, /* 10 : 11111100 */ /* start of frame LI0_Motor2Control */
    0x00, /* 11 : 00000000 */ /* start of frame LI0_Motor2State_Cycl */
    0x00, /* 12 : 00000000 */
    0x00, /* 13 : 00000000 */
    0x00, /* 14 : 00000000 */
    0x00, /* 15 : 00000000 */
    0xfe, /* 16 : 11111110 */
    0xff, /* 17 : 11111111 */ /* start of frame LI0_Motor2State_Event */
    0x02, /* 18 : 00000010 */
    0x04, /* 19 : 00000100 */
    0xff, /* 20 : 11111111 */ /* start of frame LI0_MotorsControl */
    0x62, /* 21 : 01100010 */
    0xfd  /* 22 : 11111101 */
};

/* definition and initialization of signal array */
l_u8 lin_flag_handle_tbl[LIN_FLAG_BUF_SIZE] =
{
    0x00, /* 0: start of flag frame LI0_Motor1Control */
    0x00, /* 1: start of flag frame LI0_Motor1State_Cycl */
    0x00, /* 2: start of flag frame LI0_Motor1State_Event */
    0x00, /* 3: start of flag frame LI0_Motor2Control */
    0x00, /* 4: start of flag frame LI0_Motor2State_Cycl */
    0x00, /* 5: start of flag frame LI0_Motor2State_Event */
    0x00, /* 6: start of flag frame LI0_MotorsControl */
};

/*************************** Flag set when signal is updated ******************/
/* Diagnostic signal */
l_u8 lin_diag_signal_tbl[LIN_NUM_OF_IFCS][16];
/*****************************event trigger frame*****************************/
/* all event trigger frames for master node */

const l_u8 LI0_ETF_MotorStates_info_data[2] =
{
    LI0_Motor1State_Event,
    LI0_Motor2State_Event
};

const lin_associate_frame_struct LI0_ETF_MotorStates_info =
{
    2,  /* Number of associated unconditional frame ID*/
    &LI0_ETF_MotorStates_info_data[0],   /* Associated unconditional frame ID */
    LI0_ETFCollisionResolving  /* Collision resolver index in the schedule table, used in event trigger frame case*/ 
};

/*****************************sporadic frame*****************************/
/*all sporadic frames for master node*/
const l_u8 LI0_SporadicControlFrame_info_data[2] =
{    
    LI0_Motor1Control,
    LI0_Motor2Control
};

const lin_associate_frame_struct LI0_SporadicControlFrame_info =
{
    2,
    &LI0_SporadicControlFrame_info_data[0],
    0xFF
};
/**********************************  Frame table **********************************/
const lin_frame_struct lin_frame_tbl[LIN_NUM_OF_FRMS] =
{
    { LIN_FRM_UNCD, 1, LIN_RES_PUB, 0, 0, 1   , (l_u8*)0 },
    { LIN_FRM_UNCD, 6, LIN_RES_SUB, 1, 1, 1 , (l_u8*)0 },
    { LIN_FRM_UNCD, 3, LIN_RES_SUB, 7, 2, 1 , (l_u8*)0 },
    { LIN_FRM_UNCD, 1, LIN_RES_PUB, 10, 3, 1 , (l_u8*)0 },
    { LIN_FRM_UNCD, 6, LIN_RES_SUB, 11, 4, 1 , (l_u8*)0 },
    { LIN_FRM_UNCD, 3, LIN_RES_SUB, 17, 5, 1 , (l_u8*)0 },
    { LIN_FRM_UNCD, 3, LIN_RES_PUB, 20, 6, 1 , (l_u8*)0 },
    { LIN_FRM_EVNT, 3, LIN_RES_SUB, 0, 0, 0 , (l_u8*)&LI0_ETF_MotorStates_info },
    { LIN_FRM_DIAG, 8, LIN_RES_PUB, 0, 0, 0 , (l_u8*)0 },
    { LIN_FRM_DIAG, 8, LIN_RES_SUB, 0, 0, 0 , (l_u8*)0 },
    { LIN_FRM_SPRDC, 1, LIN_RES_PUB, 0, 0, 0 , (l_u8*)&LI0_SporadicControlFrame_info }
};

/*********************************** Frame flag Initialization **********************/
l_bool lin_frame_flag_tbl[LIN_NUM_OF_FRMS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/**************************** Lin configuration Initialization ***********************/
/* max_response_frame_timeout = round((1.4x(10+Nx10)xTbit)/Tbase_period) + 3 */
const l_u16 lin_max_frame_res_timeout_val[LIN_NUM_OF_IFCS][8] =
{
   { 9, 12, 15, 18, 21, 23, 26, 29 } 
};


l_u8 LI0_lin_configuration_RAM[LI0_LIN_SIZE_OF_CFG] =
{
    0x00, 0x30, 0x33, 0x36, 0x31, 0x34, 0x37, 0x2D, 0x3A, 0x3C, 0x3D, 0xFF ,0xFF
};


const l_u16  LI0_lin_configuration_ROM[LI0_LIN_SIZE_OF_CFG] =
{
    0x0000, 0x30, 0x33, 0x36, 0x31, 0x34, 0x37, 0x2D, 0x3A, 0x3C, 0x3D, 0xFF ,0xFFFF
};

/**************** Node attributes Initialization  ****************************/

/********************** Go to sleep Initialization *************************/
const lin_schedule_data LI0_lin_gotosleep_data[1] =
{ 
    {LI0_MasterReq, 15, {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}}
};


/******************** Schedule table Initialization ************************/
const lin_schedule_data LI0_NormalTable_data[2] =
{
    {LI0_Motor1Control, 10, 0},
    {LI0_Motor1State_Cycl, 10, 0}
};

/******************** Schedule table Initialization ************************/
const lin_schedule_data LI0_ETFCollisionResolving_data[6] =
{
    {LI0_Motor1State_Event, 10, 0},
    {LI0_Motor2State_Event, 10, 0}, 
    {LI0_MotorsControl, 10, 0},
    {LI0_Motor1State_Cycl, 10, 0},
    {LI0_Motor2State_Cycl, 10, 0},
    {LI0_ETF_MotorStates, 10, 0},
};

/******************** Schedule table Initialization ************************/
const lin_schedule_data LI0_InitTable_data[8] =
{
    {LI0_MasterReq, 2, {0x0A,0x06,0xB0,0x1E,0x00,0x01,0x00,0x02}},
    {LI0_SlaveResp, 2, 0},
    {LI0_MasterReq, 2, {0x0C,0x06,0xB0,0x2E,0x00,0x0B,0x00,0x03}},
    {LI0_SlaveResp, 2, 0},
    {LI0_MasterReq, 2, {0x02,0x06,0xB7,0x00,0xAD,0xF0,0x73,0x76}}, 
    {LI0_MasterReq, 2, {0x02,0x06,0xB7,0x04,0xBA,0xFF,0xFF,0xFF}},
    {LI0_MasterReq, 2, {0x03,0x06,0xB7,0x00,0xAD,0xB1,0xB4,0x37}},
    {LI0_MasterReq, 2, {0x03,0x06,0xB7,0x04,0xBA,0xFF,0xFF,0xFF}},
};

/******************** Schedule table Initialization ************************/
const lin_schedule_data LI0_MasterReqTable_data[1] =
{
    {LI0_MasterReq, 2, 0}  
};

/******************** Schedule table Initialization ************************/
const lin_schedule_data LI0_SlaveRespTable_data[1] =
{
    {LI0_SlaveResp, 2, 0}   
};

/********************* Schedule table structure Initialization ***************/
const lin_schedule_struct lin_schedule_tbl[LIN_NUM_OF_SCHD_TBL] =
{
   /*interface_name = LI0 */
    {0, LIN_SCH_TBL_NULL  ,(lin_schedule_data*)0 },
    {1, LIN_SCH_TBL_GOTO  , &LI0_lin_gotosleep_data[0] },
    {1, LIN_SCH_TBL_DIAG  , &LI0_MasterReqTable_data[0] },
    {1, LIN_SCH_TBL_DIAG  , &LI0_SlaveRespTable_data[0] },
    {2, LIN_SCH_TBL_NORM  , &LI0_NormalTable_data[0] },
    {6, LIN_SCH_TBL_COLL  , &LI0_ETFCollisionResolving_data[0] },
    {8, LIN_SCH_TBL_NORM  , &LI0_InitTable_data[0] },
};
/********************** Schedule table status Initialization ******************/
l_u8 lin_schedule_start_entry[LIN_NUM_OF_SCHD_TBL] = {0, 0, 0, 0, 0, 0, 0};
l_u8 lin_active_schedule_id[LIN_NUM_OF_IFCS] = {0};
l_u8 lin_previous_schedule_id[LIN_NUM_OF_IFCS] = {0};

l_u8 lin_diag_frame_to_send[LIN_NUM_OF_IFCS] = {0}; 
/********************** PDU data Initialization *******************************/
lin_tl_pdu_data tx_single_pdu_data[LIN_NUM_OF_IFCS] =
{
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} 
};
lin_tl_pdu_data rx_single_pdu_data[LIN_NUM_OF_IFCS] =
{
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};

/****************************Transport Layer Initialization ***********************/
lin_tl_descriptor lin_tl_desc[LIN_NUM_OF_IFCS] =
{
    /* interface_name = LI0 */
    {
        &tx_single_pdu_data[LI0],               /* *pointer to transmit queue on TL */ 
        &rx_single_pdu_data[LI0],               /* *pointer to receive queue on TL */ 
        (lin_tl_pdu_data*)0,                    /* tl_current_tx_pdu_ptr */
        (lin_tl_pdu_data*)0,                    /* tl_current_rx_pdu_ptr */
        /* Declaration only for Master interface */
        0,                                      /* tl_cnt_to_send */
        LD_SERVICE_IDLE,                        /* tl_service_status */
        LD_SUCCESS,                             /* tl_last_cfg_result */
        0,                                      /* last_RSID */
        0,                                      /* ld_error_code */
        (l_u8*)0,                               /* *tl_ident_data */
        /* End of declaration for only Master interface */  
        /* Declaration only for Slave interface */
        0                                       /* tl_slaveresp_cnt */
        /* End of declaration only for Slave interface */ 
    }
};

/****************************LIN interface configuration ****************************/
const lin_configuration lin_ifc_configuration[LIN_NUM_OF_IFCS] =
{ 
    /* Interface_name = LI0 */
    {
        LIN_PROTOCOL_21,                        /*lin_protocol_version */
        LIN_PROTOCOL_21,                        /*lin_language_version */
        9600,                                   /*  baud_rate */   
        _MASTER_,                               /*  function _SLAVE_ | _MASTER_*/
        0,                                      /*  node attribute is only used for slave node*/
        /* LIN data pointer */
        &lin_lld_response_buffer[LI0][0],       /*  *response_buffer */
        &lin_successful_transfer[LI0],          /*  *lin_successful_transfer */
        &lin_error_in_response[LI0],            /*  *lin_error_in_response */
        &lin_goto_sleep_flg[LI0],               /*  *goto_sleep_flg */
        &lin_current_pid[LI0],                  /*  *current_pid */
        &lin_word_status[LI0],                  /*  *word_status */
        /* Protocol */
        10,                                     /*  2*timebase */
        &lin_diag_signal_tbl[LI0][0],           /*  *diag_signal_tbl */
        11,                                     /*  num_of_frames */
        0,                                      /*  frame_start */
        &lin_frame_tbl[0],                      /*  frame_tbl */
        &lin_frame_flag_tbl[0],                 /*  *frame_flg */
        7,                                      /*  num_of_schedules */
        0,                                      /*  schedule_start */
        &lin_schedule_tbl[0],                   /*  schedule_tbl */
        &lin_schedule_start_entry[0],           /*  schedule_start_entry */
        &lin_next_transmit[LI0],                /*  next_transmit_tick */
        &lin_active_schedule_id[LI0],           /*  active_schedule_id */
        &lin_previous_schedule_id[LI0],         /*  previous_schedule_id */
        &lin_diag_frame_to_send[LI0],           /*  *diagnostic_frame_to_send */
        DIAG_NONE,                              /*  diagnostic_mode */
        &LI0_lin_configuration_RAM[0],          /*  *configuration_RAM */
        &LI0_lin_configuration_ROM[0],          /*  *configuration_ROM */
        &lin_tl_desc[LI0]   
    }
}; 
 
/*************************** Node hardware configuration definition *************************/
/* Node hardware configuration */
lin_node lin_node_descrs[NUM_OF_UART_CHANNEL] =
{  
    {(UART_Type *)UART0, 0, 0xFF, UNINIT,  0, 0,(l_u8*)0, 0x00, (l_u8*)0, 0x80, 0, 0, 0, 0 },
    {(UART_Type *)UART1, 0, 0xFF, UNINIT,  0, 0,(l_u8*)0, 0x00, (l_u8*)0, 0x80, 0, 0, 0, 0 },
    {(UART_Type *)UART2, 0, 0xFF, UNINIT,  0, 0,(l_u8*)0, 0x00, (l_u8*)0, 0x80, 0, 0, 0, 0 } 
};

/* This function is an example of response; real implementation is application-dependent */
/* You can use one of the following define to set PCI of response frame for */
/* this service to the correct value */
l_u8 ld_read_by_id_callout(l_ifc_handle iii, l_u8 id, l_u8 *data){
	(void) iii;
	(void) id;
	(void) data;
	return LD_ID_NO_RESPONSE;
}