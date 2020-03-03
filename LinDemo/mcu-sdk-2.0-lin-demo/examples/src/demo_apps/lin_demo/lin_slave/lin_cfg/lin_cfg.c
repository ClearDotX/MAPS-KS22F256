/****************************************************************************** 
* 
* Freescale Semiconductor Inc. 
* (c) Copyright 2014-2015 Freescale Semiconductor, Inc. 
* ALL RIGHTS RESERVED. 
* 
***************************************************************************//*! 
* 
* @file      lin_cfg.c 
* 
* @author    FPT Software
*  
* @version   1.0 
*  
* @date      Wed May 27 16:41:35 ICT 2015 
*  
* @brief     Common LIN configuration, data structure
* 
******************************************************************************/
#include "lin_cfg.h"
#include "lin.h"
/* Mapping interface with hardware */
const lin_hardware_name lin_virtual_ifc = UART1_ID; 
l_u8 lin_lld_response_buffer[10];
 
l_u8 lin_successful_transfer; 
l_u8 lin_error_in_response; 
l_u8 lin_goto_sleep_flg; 
/* Save configuration flag */
l_u8 lin_save_configuration_flg = 0;
lin_word_status_str lin_word_status; 
l_u8 lin_current_pid; 

const l_signal_handle LI0_response_error_signal = LI0_Motor1LinError;
const l_u8 response_error_byte_offset = LIN_BYTE_OFFSET_LI0_Motor1LinError;
const l_u8 response_error_bit_offset = LIN_BIT_OFFSET_LI0_Motor1LinError;
 

/* definition and initialization of signal array */
l_u8    lin_pFrameBuf[LIN_FRAME_BUF_SIZE] =
{


  0xff /* 0 : 11111111 */ /* start of frame LI0_MotorsControl */

  ,0x62 /* 1 : 01100010 */
  ,0xfd /* 2 : 11111101 */


  ,0xfc /* 3 : 11111100 */ /* start of frame LI0_Motor1Control */


  ,0x05 /* 4 : 00000101 */ /* start of frame LI0_Motor1State_Cycl */

  ,0x00 /* 5 : 00000000 */
  
  ,0x00 /* 6 : 00000000 */
  
  ,0x00 /* 7 : 00000000 */
  
  ,0x00 /* 8 : 00000000 */
  
  ,0xfe /* 9 : 11111110 */
  

  ,0xff /* 10 : 11111111 */ /* start of frame LI0_Motor1State_Event */

  ,0x05 /* 11 : 00000101 */
  
  ,0x01 /* 12 : 00000001 */
  
};

/* definition and initialization of signal array */
l_u8    lin_flag_handle_tbl[LIN_FLAG_BUF_SIZE] =
{


  0x00 /* 0: start of flag frame LI0_MotorsControl */



  ,0x00 /* 1: start of flag frame LI0_Motor1Control */


  ,0x00 /* 2: start of flag frame LI0_Motor1State_Cycl */


  ,0x00 /* 3: start of flag frame LI0_Motor1State_Event */

};


/*************************** Flag set when signal is updated ******************/
/* Diagnostic signal */
l_u8 lin_diag_signal_tbl[16];
/*****************************event trigger frame*****************************/

const l_u8 LI0_ETF_MotorStates_info_data = LI0_Motor1State_Event ;  /* frame data */


/**********************************  Frame table **********************************/
const lin_frame_struct lin_frame_tbl[LIN_NUM_OF_FRMS] ={

    { LIN_FRM_UNCD, 3, LIN_RES_SUB, 0, 0, 1   , (l_u8*)0 }

   ,{ LIN_FRM_UNCD, 1, LIN_RES_SUB, 3, 1, 1 , (l_u8*)0 }
  
   ,{ LIN_FRM_UNCD, 6, LIN_RES_PUB, 4, 2, 1 , (l_u8*)&LI0_response_error_signal }
  
   ,{ LIN_FRM_UNCD, 3, LIN_RES_PUB, 10, 3, 1 , (l_u8*)0 }
  
   ,{ LIN_FRM_EVNT, 3, LIN_RES_PUB, 0, 0, 0 , (l_u8*)&LI0_ETF_MotorStates_info_data }
  
   ,{ LIN_FRM_DIAG, 8, LIN_RES_SUB, 0, 0, 0 , (l_u8*)0 }
  
   ,{ LIN_FRM_DIAG, 8, LIN_RES_PUB, 0, 0, 0 , (l_u8*)0 }
  
};

/*********************************** Frame flag Initialization **********************/
l_bool lin_frame_flag_tbl[LIN_NUM_OF_FRMS] = {0, 0, 0, 0, 0, 0, 0};

/**************************** Lin configuration Initialization ***********************/
/* max_response_frame_timeout = round((1.4x(10+Nx10)xTbit)/Tbase_period) + 3 */

const l_u16 lin_max_frame_res_timeout_val[8]={

9, 12, 15, 18, 21, 23, 26, 29

};


  

l_u8 lin_configuration_RAM[LIN_SIZE_OF_CFG]= {0x00, 0x2D, 0x30, 0x33, 0x36, 0x3A, 0x3C, 0x3D ,0xFF};
const l_u16  lin_configuration_ROM[LIN_SIZE_OF_CFG]= {0x00, 0x2D, 0x30, 0x33, 0x36, 0x3A, 0x3C, 0x3D ,0xFFFF};


/***************************************** Node Attribute*****************************************/
   
l_u8 lin_configured_NAD = 0x02;     /*<configured_NAD>*/ 
const l_u8 lin_initial_NAD    =0x0A;     /*<initial_NAD>*/ 
const lin_product_id product_id = {0x001E, 0x0001, 0x0000 };  /* {<supplier_id>,<function_id>,<variant>} */ 
const l_signal_handle response_error =  LI0_Motor1LinError;               




/************************** TL Layer and Diagnostic: SINGLE interface **************************/
lin_tl_pdu_data tx_single_pdu_data = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
lin_tl_pdu_data rx_single_pdu_data = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};





 

lin_tl_pdu_data *tl_current_tx_pdu_ptr;
lin_tl_pdu_data *tl_current_rx_pdu_ptr;
l_u8 tl_slaveresp_cnt = 0;
 
/* This function is an example of response; real implementation is application-dependent */
/* You can use one of the following define to set PCI of response frame for */
/* this service to the correct value */
l_u8 ld_read_by_id_callout(l_u8 id, l_u8 *data){
    l_u8 retval = LD_ID_NO_RESPONSE;
    /* Following code is an example - Real implementation is application-dependent */
    /*
     * the handling does essentially depend on the id of the
     * requested item
     */
    /* This example implement with ID = 32 - LIN_READ_USR_DEF_MIN */
    if (id == LIN_READ_USR_DEF_MIN)
    {
      /*
       * id received is user defined 32
       */
      /* A positive response is ready to be sent to the user defined request */
     
      data[0] = (l_u8) (id + 1);    /* Data user define */
      data[1] = (l_u8) (id + 2);    /* Data user define */
      data[2] = (l_u8) (id + 3);    /* Data user define */
      data[3] = (l_u8) (id + 4);    /* Data user define */
      data[4] = (l_u8) (id + 5);    /* Data user define */
      retval = LD_POSITIVE_RESPONSE;
    }
    else 
    {
      /*
       * id received is user defined 63 - no response
       */
    }
    return retval;    
}