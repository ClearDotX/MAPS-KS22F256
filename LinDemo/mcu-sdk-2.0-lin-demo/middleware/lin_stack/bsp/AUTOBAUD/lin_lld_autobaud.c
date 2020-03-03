/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2008-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
******************************************************************************/
/**************************************************************************//**
* @addtogroup autobaud_group
* @{
******************************************************************************/
/**************************************************************************//**
*
* @file      lin_lld_autobaud.c 
*
* @author    FPT Software
*
* @brief    Functions for Autobaud feature
*
******************************************************************************/

/******************************************************************************
 *
 * History:
 *
 * 20101027		v1.0	First version
 *
 *****************************************************************************/


#include "lin.h"
#include "lin_lld_sci.h"
#include "lin_hw_cfg.h"
#include "lin_common_proto.h"
#include "lin_lld_timesrv.h"
#include "lin_cfg.h"
#include "lin_lld_autobaud.h"

 l_u16       autobaud_tmr_capture[16];
 l_u8        autobaud_tmr_capture_index=0;
 l_u32       autobaud_bitduration=0;
 l_u16       autobaud_current=0;

 const l_u16 lin_max_frame_res_timeout_val_19200[8]={

6, 7, 9, 10, 12, 13, 15, 16

 };
 
 const l_u16 lin_max_frame_res_timeout_val_9600[8]={

9, 12, 15, 18, 21, 23, 26, 29

 };
 
 l_u16 lin_max_frame_res_timeout_val_autobaud[8];



void autobaud_init(void)
{
	l_u8 i;
	// disconnect IOC0_3 from TIM0 input capture channel 3 and reroute the timer input to the RXD0 signal of SCI0.
	// T0IC3RR1:T0IC3RR0
#if (defined(_MC9S12ZVL32_H))
	MODRR4 = 0x1;
	TIM0TCTL4_EDG3x = 0U; 				   /* disabled edge for capture */
	TIM0TFLG1_C3F = 1U;                    /* Reset interrupt request flag */
	TIM0TIE_C3I = 0U;                      /* Disable interrupt */
	TIM0TIOS_IOS3 =0;					   /*Channel 3 is configure for capture */
	TIM0PTPSR=0x00;               		   /* Set prescaler register Bus Clock / 1 */ 
	/* TSCR1: TEN=0,TSWAI=1,TSFRZ=1,TFFCA=0,PRNT=1,??=0,??=0,??=0 */
	TIM0TSCR1=0x68;
	TIM0TFLG1_C3F = 1U;                    /* Reset interrupt request flag */
	TIM0TIE_C3I = 1U;                      /* Enable interrupt from timer capture*/
	/* TSCR1: TEN=1,TSWAI=0,TSFRZ=0,TFFCA=0,PRNT=0,??=0,??=0,??=0 */
	TIM0TSCR1|=0x80U;   
	TIM0TCTL4_EDG3x = 3U; 				   /* Enable LIN Timer capture for SYNC byte */
#endif /*(defined(_MC9S12ZVL32_H))*/
	
#if (defined(_MC9S12ZVH128_H))
	MODRR2 = 0x2; /* TIM1 input capture channel 0 is connected to RXD0 */
	TIM1TCTL4_EDG0x = 0U; 				   /* disabled edge for capture */
	TIM1TFLG1_C0F = 1U;                    /* Reset interrupt request flag */
	TIM1TIE_C0I = 0U;                      /* Disable interrupt */
	TIM1TIOS_IOS0 =0;					   /*Channel 3 is configure for capture */
	TIM1PTPSR=0x00;               		   /* Set prescaler register Bus Clock / 1 */ 
		/* TSCR1: TEN=0,TSWAI=1,TSFRZ=1,TFFCA=0,PRNT=1,??=0,??=0,??=0 */
	TIM1TSCR1=0x68;
	TIM1TFLG1_C0F = 1U;                    /* Reset interrupt request flag */
	TIM1TIE_C0I = 1U;                      /* Enable interrupt from timer capture*/
		/* TSCR1: TEN=1,TSWAI=0,TSFRZ=0,TFFCA=0,PRNT=0,??=0,??=0,??=0 */
	TIM1TSCR1|=0x80U;
	TIM1TCTL4_EDG0x = 3U;
#endif /*(defined(_MC9S12ZVH128_H))*/
	
#if (defined(_MC9S12ZVML128_H)||defined(_MC9S12ZVML31_H))
	MODRR2_T0IC3RR = 0x1; /* TIM0 input capture channel 3 is connected to RXD0 */
	TIM0TCTL4_EDG3x = 0U; 				   /* disabled edge for capture */
	TIM0TFLG1_C3F = 1U;                    /* Reset interrupt request flag */
	TIM0TIE_C3I = 0U;                      /* Disable interrupt */
	TIM0TIOS_IOS3 =0;					   /*Channel 3 is configure for capture */
	TIM0PTPSR=0x00;               		   /* Set prescaler register Bus Clock / 1 */ 
		/* TSCR1: TEN=0,TSWAI=1,TSFRZ=1,TFFCA=0,PRNT=1,??=0,??=0,??=0 */
	TIM0TSCR1=0x68;
	TIM0TFLG1_C3F = 1U;                    /* Reset interrupt request flag */
	TIM0TIE_C3I = 1U;                      /* Enable interrupt from timer capture*/
		/* TSCR1: TEN=1,TSWAI=0,TSFRZ=0,TFFCA=0,PRNT=0,??=0,??=0,??=0 */
	TIM0TSCR1|=0x80U;   
	TIM0TCTL4_EDG3x = 3U; 				   /* Enable LIN Timer capture for SYNC byte */
#endif /* End if (defined(_MC9S12ZVML128_H)||defined(_MC9S12ZVML31_H))*/

#if (defined(_MC9S12VR64_H)||defined(_MC9S12VR32_H))

  MODRR2_MODRR27 = 0x1; /* TIM0 input capture channel 3 is connected to RXD0 */
	TCTL4_EDG3x = 0U; 				   /* disabled edge for capture */
	TFLG1_C3F = 1U;                    /* Reset interrupt request flag */
	TIE_C3I = 0U;                      /* Disable interrupt */
	TIOS_IOS3 =0;					   /*Channel 3 is configure for capture */
	PTPSR=0x00;               		   /* Set prescaler register Bus Clock / 1 */ 
		/* TSCR1: TEN=0,TSWAI=1,TSFRZ=1,TFFCA=0,PRNT=1,??=0,??=0,??=0 */
	TSCR1=0x68;
	TFLG1_C3F = 1U;                    /* Reset interrupt request flag */
	TIE_C3I = 1U;                      /* Enable interrupt from timer capture*/
		/* TSCR1: TEN=1,TSWAI=0,TSFRZ=0,TFFCA=0,PRNT=0,??=0,??=0,??=0 */
	TSCR1|=0x80U;   
	TCTL4_EDG3x = 3U; 				   /* Enable LIN Timer capture for SYNC byte */
#endif /* End if (defined(_MC9S12VR64_H)||defined(_MC9S12VR32_H)) */
	autobaud_current=LIN_BAUD_RATE;
	for (i=0;i<8;i++) {
		if (autobaud_current==19200)
			lin_max_frame_res_timeout_val_autobaud[i]=lin_max_frame_res_timeout_val_19200[i];
		if (autobaud_current==9600)
			lin_max_frame_res_timeout_val_autobaud[i]=lin_max_frame_res_timeout_val_9600[i];
	}

}	
	
/* End of File */

/**
* @}
*/


