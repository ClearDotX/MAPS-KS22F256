/****************************************************************************** 
* 
* Freescale Semiconductor Inc. 
* (c) Copyright 2014-2015 Freescale Semiconductor, Inc. 
* ALL RIGHTS RESERVED. 
* 
****************************************************************************//*!  
* 
* @file      lin_cfg.h 
* 
* @author    FPT Software
*  
* @version   1.0 
*  
* @date      Wed May 27 16:41:35 ICT 2015
*  
* @brief     Hardware configuration file
* 
******************************************************************************/
#ifndef    _LIN_CFG_H_  
#define    _LIN_CFG_H_  
#include "lin_hw_cfg.h" 
/* Define operating mode */
#define _MASTER_MODE_     0 
#define _SLAVE_MODE_      1 
#define LIN_MODE   _SLAVE_MODE_
/* Define protocol version */
#define PROTOCOL_21       0  
#define PROTOCOL_J2602    1  
#define PROTOCOL_20       2 
#define LIN_PROTOCOL    PROTOCOL_21

#define UART_ADDR        UART0_ADDR    /* For slave */ 


#define LIN_BAUD_RATE    9600    	 /*For slave*/ 
/**********************************************************************/
/***************          Diagnostic class selection  *****************/
/**********************************************************************/
#define _DIAG_CLASS_I_          0
#define _DIAG_CLASS_II_         1
#define _DIAG_CLASS_III_        2

#define _DIAG_CLASS_SUPPORT_    _DIAG_CLASS_I_
/**************** FRAME SUPPORT DEFINITION ******************/
#define _TL_SINGLE_FRAME_       0
#define _TL_MULTI_FRAME_        1

#define _TL_FRAME_SUPPORT_      _TL_SINGLE_FRAME_

/* frame buffer size */
#define LIN_FRAME_BUF_SIZE			13
#define LIN_FLAG_BUF_SIZE			4

/**********************************************************************/
/***************               Interfaces           *******************/
/**********************************************************************/
typedef enum { 
   LI0
}l_ifc_handle; 

/**********************************************************************/
/***************               Signals              *******************/
/**********************************************************************/
/* Number of signals */
#define LIN_NUM_OF_SIGS  8
/* List of signals */   
typedef enum {

   /* Interface_name = LI0 */

   LI0_Motor1ErrorCode

   , LI0_Motor1ErrorValue
  
   , LI0_Motor1LinError
  
   , LI0_Motor1Position
  
   , LI0_Motor1Selection
  
   , LI0_Motor1Temp
  
   , LI0_MotorDirection
  
   , LI0_MotorSpeed
  
  
} l_signal_handle; 
/**********************************************************************/
/*****************               Frame             ********************/
/**********************************************************************/
/* Number of frames */
#define LIN_NUM_OF_FRMS  7 
/* List of frames */
typedef enum {
/* All frames for master node */

   /* Interface_name = LI0 */

   LI0_MotorsControl

   , LI0_Motor1Control
  
   , LI0_Motor1State_Cycl
  
   , LI0_Motor1State_Event
  
   , LI0_ETF_MotorStates
  
   , LI0_MasterReq
  
   , LI0_SlaveResp
  
  
} l_frame_handle; 
 
/**********************************************************************/
/***************             Configuration          *******************/
/**********************************************************************/
/* Size of configuration in ROM and RAM used for interface: LI1 */
#define LIN_SIZE_OF_CFG  9 
#define LIN_CFG_FRAME_NUM  5 
/*********************************************************************
 * global macros
 *********************************************************************/
#define l_bool_rd(SIGNAL) l_bool_rd_##SIGNAL()
#define l_bool_wr(SIGNAL, A) l_bool_wr_##SIGNAL(A)
#define l_u8_rd(SIGNAL) l_u8_rd_##SIGNAL()
#define l_u8_wr(SIGNAL, A) l_u8_wr_##SIGNAL(A)
#define l_u16_rd(SIGNAL) l_u16_rd_##SIGNAL()
#define l_u16_wr(SIGNAL, A) l_u16_wr_##SIGNAL(A)
#define l_bytes_rd(SIGNAL, start, count, data)  l_bytes_rd_##SIGNAL(start, count, data)
#define l_bytes_wr(SIGNAL, start, count, data) l_bytes_wr_##SIGNAL(start, count, data)
#define l_flg_tst(FLAG) l_flg_tst_##FLAG()
#define l_flg_clr(FLAG) l_flg_clr_##FLAG()
#define LIN_TEST_BIT(A,B) ((l_bool)((((A) & (1U << (B))) != 0U) ? 1U : 0U))
#define LIN_SET_BIT(A,B)                      ((A) |= (l_u8) (1U << (B)))
#define LIN_CLEAR_BIT(A,B)               ((A) &= ((l_u8) (~(1U << (B)))))
#define LIN_BYTE_MASK  ((l_u16)(((l_u16)((l_u16)1 << CHAR_BIT)) - (l_u16)1))
#define LIN_FRAME_LEN_MAX                                             10U

/* Returns the low byte of the 32-bit value    */
#define BYTE_0(n)                              ((l_u8)((n) & (l_u8)0xFF))
/* Returns the second byte of the 32-bit value */
#define BYTE_1(n)                        ((l_u8)(BYTE_0((n) >> (l_u8)8)))
/* Returns the third byte of the 32-bit value  */
#define BYTE_2(n)                       ((l_u8)(BYTE_0((n) >> (l_u8)16)))
/* Returns high byte of the 32-bit value       */
#define BYTE_3(n)                       ((l_u8)(BYTE_0((n) >> (l_u8)24)))

/*
 * defines for signal access
 */


#define LIN_BYTE_OFFSET_LI0_Motor1ErrorCode    11
#define LIN_BIT_OFFSET_LI0_Motor1ErrorCode    0
#define LIN_SIGNAL_SIZE_LI0_Motor1ErrorCode    8
#define LIN_FLAG_BYTE_OFFSET_LI0_Motor1ErrorCode    3
#define LIN_FLAG_BIT_OFFSET_LI0_Motor1ErrorCode    1
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_Motor1ErrorCode    3

#define LIN_BYTE_OFFSET_LI0_Motor1ErrorValue    12
#define LIN_BIT_OFFSET_LI0_Motor1ErrorValue    0
#define LIN_SIGNAL_SIZE_LI0_Motor1ErrorValue    8
#define LIN_FLAG_BYTE_OFFSET_LI0_Motor1ErrorValue    3
#define LIN_FLAG_BIT_OFFSET_LI0_Motor1ErrorValue    2
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_Motor1ErrorValue    3

#define LIN_BYTE_OFFSET_LI0_Motor1LinError    9
#define LIN_BIT_OFFSET_LI0_Motor1LinError    0
#define LIN_SIGNAL_SIZE_LI0_Motor1LinError    1
#define LIN_FLAG_BYTE_OFFSET_LI0_Motor1LinError    2
#define LIN_FLAG_BIT_OFFSET_LI0_Motor1LinError    3
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_Motor1LinError    2

#define LIN_BYTE_OFFSET_LI0_Motor1Position    5
#define LIN_BIT_OFFSET_LI0_Motor1Position    0
#define LIN_SIGNAL_SIZE_LI0_Motor1Position    32
#define LIN_FLAG_BYTE_OFFSET_LI0_Motor1Position    2
#define LIN_FLAG_BIT_OFFSET_LI0_Motor1Position    2
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_Motor1Position    2

#define LIN_BYTE_OFFSET_LI0_Motor1Selection    3
#define LIN_BIT_OFFSET_LI0_Motor1Selection    0
#define LIN_SIGNAL_SIZE_LI0_Motor1Selection    2
#define LIN_FLAG_BYTE_OFFSET_LI0_Motor1Selection    1
#define LIN_FLAG_BIT_OFFSET_LI0_Motor1Selection    1
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_Motor1Selection    1

#define LIN_BYTE_OFFSET_LI0_Motor1Temp    4
#define LIN_BIT_OFFSET_LI0_Motor1Temp    0
#define LIN_SIGNAL_SIZE_LI0_Motor1Temp    8
#define LIN_FLAG_BYTE_OFFSET_LI0_Motor1Temp    2
#define LIN_FLAG_BIT_OFFSET_LI0_Motor1Temp    1
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_Motor1Temp    2

#define LIN_BYTE_OFFSET_LI0_MotorDirection    0
#define LIN_BIT_OFFSET_LI0_MotorDirection    0
#define LIN_SIGNAL_SIZE_LI0_MotorDirection    2
#define LIN_FLAG_BYTE_OFFSET_LI0_MotorDirection    0
#define LIN_FLAG_BIT_OFFSET_LI0_MotorDirection    1
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_MotorDirection    0

#define LIN_BYTE_OFFSET_LI0_MotorSpeed    1
#define LIN_BIT_OFFSET_LI0_MotorSpeed    0
#define LIN_SIGNAL_SIZE_LI0_MotorSpeed    10
#define LIN_FLAG_BYTE_OFFSET_LI0_MotorSpeed    0
#define LIN_FLAG_BIT_OFFSET_LI0_MotorSpeed    2
#define LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_MotorSpeed    0




#define LIN_FLAG_BYTE_OFFSET_LI0_MotorsControl             0
#define LIN_FLAG_BIT_OFFSET_LI0_MotorsControl              0

#define LIN_FLAG_BYTE_OFFSET_LI0_Motor1Control             1
#define LIN_FLAG_BIT_OFFSET_LI0_Motor1Control              0

#define LIN_FLAG_BYTE_OFFSET_LI0_Motor1State_Cycl             2
#define LIN_FLAG_BIT_OFFSET_LI0_Motor1State_Cycl              0

#define LIN_FLAG_BYTE_OFFSET_LI0_Motor1State_Event             3
#define LIN_FLAG_BIT_OFFSET_LI0_Motor1State_Event              0


/**********************************************************************/
/***************        Static API Functions        *******************/
/**********************************************************************/
/*
 * the static signal access macros
 */


/* static access macros for signal LI0_Motor1ErrorCode */
#define l_u8_rd_LI0_Motor1ErrorCode() \
    ((l_u8) ((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Motor1ErrorCode] & \
    (((1U << LIN_SIGNAL_SIZE_LI0_Motor1ErrorCode) - 1) \
    << LIN_BIT_OFFSET_LI0_Motor1ErrorCode )) >> LIN_BIT_OFFSET_LI0_Motor1ErrorCode))
#define l_u8_wr_LI0_Motor1ErrorCode(A) \
    {lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Motor1ErrorCode] = \
    ((l_u8) (((l_u8) (lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Motor1ErrorCode] & \
    ((l_u8) (~(((1U << LIN_SIGNAL_SIZE_LI0_Motor1ErrorCode) - 1) << LIN_BIT_OFFSET_LI0_Motor1ErrorCode)))))  | \
    ((l_u8) ((((1U << LIN_SIGNAL_SIZE_LI0_Motor1ErrorCode) - 1) & (A)) << LIN_BIT_OFFSET_LI0_Motor1ErrorCode))));\
    lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_Motor1ErrorCode] = 1;}

/* static access macros for signal LI0_Motor1ErrorValue */
#define l_u8_rd_LI0_Motor1ErrorValue() \
    ((l_u8) ((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Motor1ErrorValue] & \
    (((1U << LIN_SIGNAL_SIZE_LI0_Motor1ErrorValue) - 1) \
    << LIN_BIT_OFFSET_LI0_Motor1ErrorValue )) >> LIN_BIT_OFFSET_LI0_Motor1ErrorValue))
#define l_u8_wr_LI0_Motor1ErrorValue(A) \
    {lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Motor1ErrorValue] = \
    ((l_u8) (((l_u8) (lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Motor1ErrorValue] & \
    ((l_u8) (~(((1U << LIN_SIGNAL_SIZE_LI0_Motor1ErrorValue) - 1) << LIN_BIT_OFFSET_LI0_Motor1ErrorValue)))))  | \
    ((l_u8) ((((1U << LIN_SIGNAL_SIZE_LI0_Motor1ErrorValue) - 1) & (A)) << LIN_BIT_OFFSET_LI0_Motor1ErrorValue))));\
    lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_Motor1ErrorValue] = 1;}

/* static access macros for signal LI0_Motor1LinError */
#define l_bool_rd_LI0_Motor1LinError() \
   	(LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Motor1LinError], \
   	LIN_BIT_OFFSET_LI0_Motor1LinError))
#define l_bool_wr_LI0_Motor1LinError(A) \
	{(A) ? \
  	(LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Motor1LinError], \
  	LIN_BIT_OFFSET_LI0_Motor1LinError)):\
  	(LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Motor1LinError], \
  	LIN_BIT_OFFSET_LI0_Motor1LinError));\
  	lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_Motor1LinError] = 1;}

/* static access macros for signal LI0_Motor1Position */
#define l_bytes_rd_LI0_Motor1Position(start, count, data) \
  	{l_u8       i; \
     for (i = 0; i < (count); ++i)  (data)[i] = lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Motor1Position + i + (start)];}
#define l_bytes_wr_LI0_Motor1Position(start, count, data) \
  	{l_u8       i; \
     for (i = 0; i < (count); ++i)lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Motor1Position + i + (start)]  = (data)[i]; \
  	 lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_Motor1Position] = 1;}

/* static access macros for signal LI0_Motor1Selection */
#define l_u8_rd_LI0_Motor1Selection() \
    ((l_u8) ((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Motor1Selection] & \
    (((1U << LIN_SIGNAL_SIZE_LI0_Motor1Selection) - 1) \
    << LIN_BIT_OFFSET_LI0_Motor1Selection )) >> LIN_BIT_OFFSET_LI0_Motor1Selection))
#define l_u8_wr_LI0_Motor1Selection(A) \
    {lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Motor1Selection] = \
    ((l_u8) (((l_u8) (lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Motor1Selection] & \
    ((l_u8) (~(((1U << LIN_SIGNAL_SIZE_LI0_Motor1Selection) - 1) << LIN_BIT_OFFSET_LI0_Motor1Selection)))))  | \
    ((l_u8) ((((1U << LIN_SIGNAL_SIZE_LI0_Motor1Selection) - 1) & (A)) << LIN_BIT_OFFSET_LI0_Motor1Selection))));\
    lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_Motor1Selection] = 1;}

/* static access macros for signal LI0_Motor1Temp */
#define l_u8_rd_LI0_Motor1Temp() \
    ((l_u8) ((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Motor1Temp] & \
    (((1U << LIN_SIGNAL_SIZE_LI0_Motor1Temp) - 1) \
    << LIN_BIT_OFFSET_LI0_Motor1Temp )) >> LIN_BIT_OFFSET_LI0_Motor1Temp))
#define l_u8_wr_LI0_Motor1Temp(A) \
    {lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Motor1Temp] = \
    ((l_u8) (((l_u8) (lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Motor1Temp] & \
    ((l_u8) (~(((1U << LIN_SIGNAL_SIZE_LI0_Motor1Temp) - 1) << LIN_BIT_OFFSET_LI0_Motor1Temp)))))  | \
    ((l_u8) ((((1U << LIN_SIGNAL_SIZE_LI0_Motor1Temp) - 1) & (A)) << LIN_BIT_OFFSET_LI0_Motor1Temp))));\
    lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_Motor1Temp] = 1;}

/* static access macros for signal LI0_MotorDirection */
#define l_u8_rd_LI0_MotorDirection() \
    ((l_u8) ((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_MotorDirection] & \
    (((1U << LIN_SIGNAL_SIZE_LI0_MotorDirection) - 1) \
    << LIN_BIT_OFFSET_LI0_MotorDirection )) >> LIN_BIT_OFFSET_LI0_MotorDirection))
#define l_u8_wr_LI0_MotorDirection(A) \
    {lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_MotorDirection] = \
    ((l_u8) (((l_u8) (lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_MotorDirection] & \
    ((l_u8) (~(((1U << LIN_SIGNAL_SIZE_LI0_MotorDirection) - 1) << LIN_BIT_OFFSET_LI0_MotorDirection)))))  | \
    ((l_u8) ((((1U << LIN_SIGNAL_SIZE_LI0_MotorDirection) - 1) & (A)) << LIN_BIT_OFFSET_LI0_MotorDirection))));\
    lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_MotorDirection] = 1;}

/* static access macros for signal LI0_MotorSpeed */
#define l_u16_rd_LI0_MotorSpeed() \
    ((l_u16) ((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_MotorSpeed + 1] & 0x03) << 8) + ((l_u16) (lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_MotorSpeed]) >> 0x00))
#define l_u16_wr_LI0_MotorSpeed(A) \
    {lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_MotorSpeed + 1] = \
	((l_u8) (((l_u8) (lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_MotorSpeed + 1] &  (0xfc))) | \
	((l_u8) ((l_u8) ((A) >> 8)) & 0x03))); \
	lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_MotorSpeed] = \
	((l_u8) (((l_u8) (lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_MotorSpeed] & (0x00))) | \
	((l_u8) (A) << LIN_BIT_OFFSET_LI0_MotorSpeed))); \
	lin_frame_flag_tbl[LIN_FLAG_UPDATE_BYTE_OFFSET_LI0_MotorSpeed] = 1;}



/* Signal flag APIs */


#define l_flg_tst_LI0_Motor1ErrorCode_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Motor1ErrorCode],\
         LIN_FLAG_BIT_OFFSET_LI0_Motor1ErrorCode)
#define l_flg_clr_LI0_Motor1ErrorCode_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Motor1ErrorCode],\
         LIN_FLAG_BIT_OFFSET_LI0_Motor1ErrorCode)

#define l_flg_tst_LI0_Motor1ErrorValue_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Motor1ErrorValue],\
         LIN_FLAG_BIT_OFFSET_LI0_Motor1ErrorValue)
#define l_flg_clr_LI0_Motor1ErrorValue_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Motor1ErrorValue],\
         LIN_FLAG_BIT_OFFSET_LI0_Motor1ErrorValue)

#define l_flg_tst_LI0_Motor1LinError_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Motor1LinError],\
         LIN_FLAG_BIT_OFFSET_LI0_Motor1LinError)
#define l_flg_clr_LI0_Motor1LinError_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Motor1LinError],\
         LIN_FLAG_BIT_OFFSET_LI0_Motor1LinError)

#define l_flg_tst_LI0_Motor1Position_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Motor1Position],\
         LIN_FLAG_BIT_OFFSET_LI0_Motor1Position)
#define l_flg_clr_LI0_Motor1Position_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Motor1Position],\
         LIN_FLAG_BIT_OFFSET_LI0_Motor1Position)

#define l_flg_tst_LI0_Motor1Selection_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Motor1Selection],\
         LIN_FLAG_BIT_OFFSET_LI0_Motor1Selection)
#define l_flg_clr_LI0_Motor1Selection_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Motor1Selection],\
         LIN_FLAG_BIT_OFFSET_LI0_Motor1Selection)

#define l_flg_tst_LI0_Motor1Temp_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Motor1Temp],\
         LIN_FLAG_BIT_OFFSET_LI0_Motor1Temp)
#define l_flg_clr_LI0_Motor1Temp_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Motor1Temp],\
         LIN_FLAG_BIT_OFFSET_LI0_Motor1Temp)

#define l_flg_tst_LI0_MotorDirection_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_MotorDirection],\
         LIN_FLAG_BIT_OFFSET_LI0_MotorDirection)
#define l_flg_clr_LI0_MotorDirection_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_MotorDirection],\
         LIN_FLAG_BIT_OFFSET_LI0_MotorDirection)

#define l_flg_tst_LI0_MotorSpeed_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_MotorSpeed],\
         LIN_FLAG_BIT_OFFSET_LI0_MotorSpeed)
#define l_flg_clr_LI0_MotorSpeed_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_MotorSpeed],\
         LIN_FLAG_BIT_OFFSET_LI0_MotorSpeed)



/* Frame flag APIs */

#define l_flg_tst_LI0_MotorsControl_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_MotorsControl],\
         LIN_FLAG_BIT_OFFSET_LI0_MotorsControl)
#define l_flg_clr_LI0_MotorsControl_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_MotorsControl],\
         LIN_FLAG_BIT_OFFSET_LI0_MotorsControl)

#define l_flg_tst_LI0_Motor1Control_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Motor1Control],\
         LIN_FLAG_BIT_OFFSET_LI0_Motor1Control)
#define l_flg_clr_LI0_Motor1Control_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Motor1Control],\
         LIN_FLAG_BIT_OFFSET_LI0_Motor1Control)

#define l_flg_tst_LI0_Motor1State_Cycl_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Motor1State_Cycl],\
         LIN_FLAG_BIT_OFFSET_LI0_Motor1State_Cycl)
#define l_flg_clr_LI0_Motor1State_Cycl_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Motor1State_Cycl],\
         LIN_FLAG_BIT_OFFSET_LI0_Motor1State_Cycl)

#define l_flg_tst_LI0_Motor1State_Event_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Motor1State_Event],\
         LIN_FLAG_BIT_OFFSET_LI0_Motor1State_Event)
#define l_flg_clr_LI0_Motor1State_Event_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Motor1State_Event],\
         LIN_FLAG_BIT_OFFSET_LI0_Motor1State_Event)



/* INTERFACE MANAGEMENT */

#define l_ifc_init_LI0() l_ifc_init(LI0)



#define l_ifc_wake_up_LI0() l_ifc_wake_up(LI0)



#define l_ifc_rx_LI0() l_ifc_rx(LI0)



#define l_ifc_tx_LI0() l_ifc_tx(LI0)



#define l_ifc_aux_LI0() l_ifc_aux(LI0)



#define l_ifc_read_status_LI0() l_ifc_read_status(LI0)


#endif    /* _LIN_CFG_H_ */