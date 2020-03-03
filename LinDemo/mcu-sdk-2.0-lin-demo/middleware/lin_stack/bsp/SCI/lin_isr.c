/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2008-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
******************************************************************************/
/**************************************************************************//**
* @addtogroup SCI_group
* @{
******************************************************************************/
/**************************************************************************//**
*
* @file      bsp/SCI/lin_isr.c 
*
* @author    FPT Software
*
* @brief     Interrupt service routine used in LIN driver
*
******************************************************************************/  

/******************************************************************************
 *
 * History:
 *
 * 20101027		v1.0	First version
 * 20111005		v1.1	Updated hardware support, multi timers
 * 20111121   	v1.2  Updated interrupt service routine for 9S12J637
 * 20140728		v1.3	Add AUTOBAUD feature
 *
 *****************************************************************************/
 
#include "lin_hw_cfg.h"
#include "lin_lld_timesrv.h"
#include "lin_lld_sci.h"

#if AUTOBAUD == 1 /* AUTOBAUD feature applicible for S12Z MCU family*/
#include "lin_lld_autobaud.h"

extern const l_u16 lin_max_frame_res_timeout_val_19200[8];
extern const l_u16 lin_max_frame_res_timeout_val_9600[8];
extern l_u16 lin_max_frame_res_timeout_val_autobaud[8];

#endif /* End (1==AUTOBAUD) */

#if defined(_MC9S12I32_H)
#include "analog_die_reg.h"

volatile unsigned char sci_isr_flag = 0;
volatile unsigned char j = 0;
volatile unsigned char i=0;
volatile unsigned char ivr = 0;
volatile unsigned char scis = 0;
volatile unsigned char isrhi =0;
volatile unsigned char isrlo = 0;


#pragma CODE_SEG NON_BANKED

interrupt void D2D_ISR(void)
{
  ivr = IVR;
  isrhi = ISRhi;
  isrlo = ISRlo;
  scis = SCIS1;
 
  if(isrhi&0x01){
    
    sci_isr_flag = 1;  
    scis = SCIS1;
    
    lin_lld_sci_rx_isr();
    SCIS1 = scis;         /* remove interrupt flag */
  } else if(isrlo&0x40){
  
    lin_lld_sci_err_isr(); 
    
  }else if (isrlo&0x10){
    lin_lld_sci_timeout();
    TFLG1 |= 0x08;
   
  }
}     

#pragma CODE_SEG DEFAULT

#elif defined (_MC9S12I128_H)
#include "mm912j637.h"

#pragma CODE_SEG NON_BANKED

void _D2DERR(void);     /* 0xFFF4 D2DI Error Interrupt            */
void _D2D(void);        /* 0xFFF2 D2DI External Error Interrupt   */

#pragma TRAP_PROC
void _dummyISR( void )
{
    for(;;);	/* endless loop */
}

#pragma TRAP_PROC
void _D2D(void){
  volatile l_u8 reg_temp = 0x00;
  reg_temp = INT_VECT;
  switch (reg_temp){
  
    case 0x04:
      _TFLG1.Bits.C0F = 1;
      break;
      
    case 0x07:
      _TFLG1.Bits.C3F = 1;
      lin_lld_timer_isr();
      break;
         
    case 0x9:
         lin_lld_sci_err_isr();
         break;
         
    case 0xB:
         lin_lld_sci_rx_isr();
         break;
         
    default:   break; 
      
   }
}

#pragma CODE_SEG DEFAULT

#elif defined (_MM9Z1J638_H)
#include "mm9z1j638.h"

interrupt _D2D(void);

interrupt _D2D(void) {
    volatile l_u8 reg_temp = 0x00;
    reg_temp = B_INT_VECT;
    switch (reg_temp) {

    case 0x04:
        B_TFLG1_C0F = 1;
        break;

    case 0x07:
        B_TFLG1_C3F = 1;
        lin_lld_timer_isr();
        break;

    case 0x9:
#if (LIN_MODE == _SLAVE_MODE_)
        lin_lld_sci_err_isr();
#else
        lin_lld_sci_err_isr(SCI0);
#endif
        break;

    case 0xB:
#if (LIN_MODE == _SLAVE_MODE_)
        lin_lld_sci_rx_isr();
#else
        lin_lld_sci_rx_isr(SCI0);
#endif
        break;

    default:
        break;
    }
}

#pragma CODE_SEG DEFAULT

#else

/**
* @def ISR(ivn, x)
* Call function <B>x</B> with interrupt vector number is <B>ivn</B>
*/
#define ISR(ivn, x) interrupt ivn void x(void)

#if ((SCI_VERSION == SCI_V5) || (SCI_VERSION == SCI_V6))
/* Change vector number define */
#if (NUM_OF_SCI_CHANNEL == 1) && !defined(_MC9S12GN32_H)
	#define VectorNumber_Vsci0  			VectorNumber_Vsci
	#define SCI0_INT					  	SCI_INT
#endif /* End (NUM_OF_SCI_CHANNEL == 1) && !defined(_MC9S12GN32_H) */
/* End Change vector number define */
#if ((!defined(_MC9S12ZVML128_H)) && (!defined(_MC9S12ZVL32_H))&&(!defined(_MC9S12ZVML31_H)))
#pragma CODE_SEG __NEAR_SEG NON_BANKED /* Define a specific segment to store the code */ 
#endif /*End if ((!defined(_MC9S12ZVML128_H)) && (!defined(_MC9S12ZVL32_H))&&(!defined(_MC9S12ZVML31_H)))*/

#if _SCI0_ == 1
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ISR(VectorNumber_Vsci0, SCI0_INT)
* @brief interrupt service routine for SCI0 receiver
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt service routine for SCI0 receiver
* 
* @see #lin_lld_sci_isr 
*//*END*----------------------------------------------------------------------*/ 
void SCI0_INT(void);
ISR(VectorNumber_Vsci0, SCI0_INT)
{
#if LIN_MODE == _SLAVE_MODE_
  lin_lld_sci_isr();  
#else
  lin_lld_sci_isr(SCI0);
#endif /* End (LIN_MODE == _SLAVE_MODE_) */
}
#endif /* End (_SCI0_ == 1) */

#if _SCI1_ == 1
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ISR(VectorNumber_Vsci1, SCI1_INT)
* @brief interrupt service routine for SCI1 receiver
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt service routine for SCI1 receiver
* 
* @see #lin_lld_sci_isr 
*//*END*----------------------------------------------------------------------*/ 
ISR(VectorNumber_Vsci1, SCI1_INT)
{
#if LIN_MODE == _SLAVE_MODE_
  lin_lld_sci_isr();  
#else
  lin_lld_sci_isr(SCI1);
#endif /* End (LIN_MODE == _SLAVE_MODE_) */
}
#endif /* End (_SCI1_ == 1) */

#if _SCI2_ == 1
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ISR(VectorNumber_Vsci2, SCI2_INT)
* @brief interrupt service routine for SCI2 receiver
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt service routine for SCI2 receiver
* 
* @see #lin_lld_sci_isr 
*//*END*----------------------------------------------------------------------*/ 
ISR(VectorNumber_Vsci2, SCI2_INT)
{
#if LIN_MODE == _SLAVE_MODE_
  lin_lld_sci_isr();  
#else
  lin_lld_sci_isr(SCI2);
#endif /* End (LIN_MODE == _SLAVE_MODE_) */
}
#endif /* End (_SCI2_ == 1) */

#if _SCI3_ == 1
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ISR(VectorNumber_Vsci3, SCI3_INT)
* @brief interrupt service routine for SCI3 receiver
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt service routine for SCI3 receiver
* 
* @see #lin_lld_sci_isr 
*//*END*----------------------------------------------------------------------*/ 
ISR(VectorNumber_Vsci3, SCI3_INT)
{
#if LIN_MODE == _SLAVE_MODE_
  lin_lld_sci_isr();  
#else
  lin_lld_sci_isr(SCI3);
#endif /* End (LIN_MODE == _SLAVE_MODE_) */
}
#endif /* End (_SCI3_ == 1)*/

#if _SCI4_ == 1
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ISR(VectorNumber_Vsci4, SCI4_INT)
* @brief interrupt service routine for SCI4 receiver
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt service routine for SCI4 receiver
* 
* @see #lin_lld_sci_isr 
*//*END*----------------------------------------------------------------------*/ 
ISR(VectorNumber_Vsci4, SCI4_INT)
{
#if LIN_MODE == _SLAVE_MODE_
  lin_lld_sci_isr();  
#else
  lin_lld_sci_isr(SCI4);
#endif /* End (LIN_MODE == _SLAVE_MODE_) */
}
#endif /* End (_SCI4_ ==1) */

#if _SCI5_ == 1
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void ISR(VectorNumber_Vsci5, SCI5_INT)
* @brief interrupt service routine for SCI5 receiver
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt service routine for SCI5 receiver
* 
* @see #lin_lld_sci_isr 
*//*END*----------------------------------------------------------------------*/ 
ISR(VectorNumber_Vsci5, SCI5_INT)
{
#if LIN_MODE == _SLAVE_MODE_
  lin_lld_sci_isr();  
#else
  lin_lld_sci_isr(SCI5);
#endif /* End (LIN_MODE == _SLAVE_MODE_) */
}
#endif /* End (_SCI5_ == 1) */
#endif /* End (SCI_VERSION == SCI_V5) */

#if (SCI_VERSION == SCI_V4)
/* Change vectorNumber define */
#if (NUM_OF_SCI_CHANNEL == 1)
  #define VectorNumber_Vsci1tx VectorNumber_Vscitx
  #define VectorNumber_Vsci1rx VectorNumber_Vscirx
  #define VectorNumber_Vsci1err VectorNumber_Vscierr
#endif
/* End Change vectorNumber define */

#pragma CODE_SEG __NEAR_SEG NON_BANKED /* Define a specific segment to store the code */

#if _SCI0_ == 1
#if defined(_MC9S08RN60_H)
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void SCI1_TX_INT (void)
* @brief interrupt service routine for SCI0 transmiter
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt service routine for SCI0 transmiter
*//*END*----------------------------------------------------------------------*/ 
ISR(VectorNumber_Vsci0tx, SCI0_TX_INT)
{
}

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void SCI1_RX_INT(void)
* @brief interrupt service routine for SCI0 receiver
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt service routine for SCI0 receiver
* 
* @see #lin_lld_sci_rx_isr 
*//*END*----------------------------------------------------------------------*/ 
ISR(VectorNumber_Vsci0rx, SCI0_RX_INT)
{
#if (LIN_MODE == _SLAVE_MODE_)
  lin_lld_sci_rx_isr();
#else
  lin_lld_sci_rx_isr(SCI0);
#endif /* End (LIN_MODE == _SLAVE_MODE_) */
}

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void SCI1_ERR_INT( void )
* @brief interrupt service routine for SCI0 error
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt service routine for SCI0 error
* 
* @see #lin_lld_sci_rx_isr 
*//*END*----------------------------------------------------------------------*/ 
ISR(VectorNumber_Vsci0err, SCI0_ERR_INT)
{  
#if (LIN_MODE == _SLAVE_MODE_)
  lin_lld_sci_err_isr();
#else
  lin_lld_sci_err_isr(SCI0);
#endif /* End (LIN_MODE == _SLAVE_MODE_) */
}
#else
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void SCI1_TX_INT (void)
* @brief interrupt service routine for SCI0 transmiter
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt service routine for SCI0 transmiter
*//*END*----------------------------------------------------------------------*/ 
ISR(VectorNumber_Vsci1tx, SCI1_TX_INT)
{
  
}

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void SCI1_RX_INT(void)
* @brief interrupt service routine for SCI0 receiver
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt service routine for SCI0 receiver
* 
* @see #lin_lld_sci_rx_isr 
*//*END*----------------------------------------------------------------------*/ 
ISR(VectorNumber_Vsci1rx, SCI1_RX_INT)
{
#if (LIN_MODE == _SLAVE_MODE_)
  lin_lld_sci_rx_isr();
#else
  lin_lld_sci_rx_isr(SCI0);
#endif /* End (LIN_MODE == _SLAVE_MODE_) */
}

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void SCI1_ERR_INT( void )
* @brief interrupt service routine for SCI0 error
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt service routine for SCI0 error
* 
* @see #lin_lld_sci_rx_isr 
*//*END*----------------------------------------------------------------------*/ 
ISR(VectorNumber_Vsci1err, SCI1_ERR_INT)
{  
#if (LIN_MODE == _SLAVE_MODE_)
  lin_lld_sci_err_isr();
#else
  lin_lld_sci_err_isr(SCI0);
#endif /* End (LIN_MODE == _SLAVE_MODE_) */
}
#endif /* defined(_MC9S08RN60_H) */
#endif /* End _SCI0_ == 1 */


#if _SCI1_ == 1
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void SCI2_TX_INT (void)
* @brief interrupt service routine for SCI1 transmiter
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt service routine for SCI1 transmiter
*//*END*----------------------------------------------------------------------*/ 
ISR(VectorNumber_Vsci2tx, SCI2_TX_INT)
{                    
  
}

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void SCI2_RX_INT (void)
* @brief interrupt service routine for SCI1 receiver
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt service routine for SCI1 receiver
* 
* @see #lin_lld_sci_rx_isr 
*//*END*----------------------------------------------------------------------*/ 
ISR(VectorNumber_Vsci2rx, SCI2_RX_INT)
{
#if LIN_MODE == _SLAVE_MODE_
  lin_lld_sci_rx_isr();  
#else
  lin_lld_sci_rx_isr(SCI1);  
#endif /* End (LIN_MODE == _SLAVE_MODE_) */
}

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void SCI2_ERR_INT (void)
* @brief interrupt service routine for SCI1 error
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt service routine for SCI1 error
* 
* @see #lin_lld_sci_rx_isr 
*//*END*----------------------------------------------------------------------*/ 
ISR(VectorNumber_Vsci2err, SCI2_ERR_INT)
{  
#if LIN_MODE == _SLAVE_MODE_
  lin_lld_sci_err_isr();
#else
  lin_lld_sci_err_isr(SCI1);
#endif /* End (LIN_MODE == _SLAVE_MODE_) */
}
#endif /* End _SCI1_ == 1 */
#endif /*	End (SCI_VERSION == SCI_V4) */

#if (SCI_VERSION == SCI_V2)
#pragma CODE_SEG __NEAR_SEG NON_BANKED /* Define a specific segment to store the code */

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void SCI_RX_INT (void )
* @brief interrupt service routine for SCI0
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt service routine for SCI0
* 
* @see #lin_lld_xgate_isr 
*//*END*----------------------------------------------------------------------*/ 
ISR(VectorNumber_Vsci1rx, SCI_RX_INT)
{
#if (LIN_MODE == _SLAVE_MODE_)
  lin_lld_sci_rx_isr();  
#else
  lin_lld_sci_rx_isr(SCI0);
#endif /* End (LIN_MODE == _SLAVE_MODE_) */
}

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void SCI_ERR_INT(void)
* @brief interrupt service routine for SCI0
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt service routine for SCI0
* 
* @see #lin_lld_xgate_isr 
*//*END*----------------------------------------------------------------------*/ 
ISR(VectorNumber_Vsci1err, SCI_ERR_INT)
{  
#if (LIN_MODE == _SLAVE_MODE_)
  lin_lld_sci_err_isr();
#else
  lin_lld_sci_err_isr(SCI0);
#endif /* End (LIN_MODE == _SLAVE_MODE_) */
}
#endif /* End (SCI_VERSION == SCI_V2) */


#if (_MCU_ == _S12X_)
#ifdef MULTI_TIMER_MODE
#if (_PIT0_ == 1)
ISR(VectorNumber_Vpit0, PIT0_TIMER_ISR) {  
  lin_lld_timer_isr(PIT0_INDEX); 
  __asm {
    MOVB #$01,PITTF
  }
}
#endif /* (_PIT0_ == 1) */

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void PIT1_TIMER_ISR (void)
* @brief interrupt interrupt service routine for PIT1
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt interrupt service routine for PIT1
* 
* @see #lin_lld_timer_isr 
*//*END*----------------------------------------------------------------------*/ 
#if (_PIT1_ == 1)
/****CONGTH****/
ISR(VectorNumber_Vpit1, PIT1_TIMER_ISR) {  
  lin_lld_timer_isr(PIT1_INDEX);
    __asm {
    MOVB #$02,PITTF
  }
}
#endif  /* (_PIT1_ == 1) */

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void PIT2_TIMER_ISR (void)
* @brief interrupt interrupt service routine for PIT2
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt interrupt service routine for PIT2
* 
* @see #lin_lld_timer_isr 
*//*END*----------------------------------------------------------------------*/ 
#if (_PIT2_ == 1)
ISR(VectorNumber_Vpit2, PIT2_TIMER_ISR) {  
  lin_lld_timer_isr(PIT2_INDEX);
  __asm {
    MOVB #$04,PITTF
  }  
}
#endif  /* (_PIT2_ == 1) */

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void PIT3_TIMER_ISR (void)
* @brief interrupt interrupt service routine for PIT3
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt interrupt service routine for PIT3
* 
* @see #lin_lld_timer_isr 
*//*END*----------------------------------------------------------------------*/ 
#if (_PIT3_ == 1)
ISR(VectorNumber_Vpit3, PIT3_TIMER_ISR) {  
  lin_lld_timer_isr(PIT3_INDEX);
    __asm {
    MOVB #$08,PITTF
  }   
}
#endif /* (_PIT3_ == 1) */

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void PIT4_TIMER_ISR (void)
* @brief interrupt interrupt service routine for PIT4
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt interrupt service routine for PIT4
* 
* @see #lin_lld_timer_isr 
*//*END*----------------------------------------------------------------------*/ 
#if (_PIT4_ == 1)
/****CONGTH****/
ISR(VectorNumber_Vpit4, PIT4_TIMER_ISR) {  
  lin_lld_timer_isr(PIT4_INDEX);
      __asm {
    MOVB #$10,PITTF
  }
}
#endif  /* End (_PIT4_ == 1) */


/*FUNCTION*--------------------------------------------------------------*//**
* @fn void PIT5_TIMER_ISR (void)
* @brief interrupt interrupt service routine for PIT5
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt interrupt service routine for PIT5
* 
* @see #lin_lld_timer_isr 
*//*END*----------------------------------------------------------------------*/ 

#if (_PIT5_ == 1)
/****CONGTH****/
ISR(VectorNumber_Vpit5, PIT5_TIMER_ISR) {  
  lin_lld_timer_isr(PIT5_INDEX);
      __asm {
    MOVB #$20,PITTF
  }  
}
#endif  /* End (_PIT6_ == 1) */

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void PIT6_TIMER_ISR (void)
* @brief interrupt interrupt service routine for PIT6
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt interrupt service routine for PIT6
* 
* @see #lin_lld_timer_isr 
*//*END*----------------------------------------------------------------------*/ 
#if (_PIT6_ == 1)
/****CONGTH****/
ISR(VectorNumber_Vpit6, PIT6_TIMER_ISR) {  
  lin_lld_timer_isr(PIT6_INDEX);
      __asm {
    MOVB #$40,PITTF
  }  
}
#endif  /* End (_PIT6_ == 1) */

/*FUNCTION*--------------------------------------------------------------*//**
* @fn void PIT7_TIMER_ISR (void)
* @brief interrupt interrupt service routine for PIT7
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt interrupt service routine for PIT7
* 
* @see #lin_lld_timer_isr 
*//*END*----------------------------------------------------------------------*/ 
#if (_PIT7_ == 1)
/****CONGTH****/
ISR(VectorNumber_Vpit7, PIT7_TIMER_ISR) {  
  lin_lld_timer_isr(PIT7_INDEX);
      __asm {
    MOVB #$80,PITTF
  }  
}
#endif  /* End (_PIT7_ == 1) */

#else /* Else MULTI_TIMER_MODE */
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void PIT0_TIMER_ISR (void)
* @brief interrupt interrupt service routine for PIT0
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt interrupt service routine for PIT0
* 
* @see #lin_lld_timer_isr 
*//*END*----------------------------------------------------------------------*/ 
ISR(VectorNumber_Vpit0, PIT0_TIMER_ISR) {  
  lin_lld_timer_isr();
  PITTF_PTF0 = 1;  
}
   #endif /* End MULTI_TIMER_MODE */
#endif /* End (_MCU_ == _S12X_) */

#if (_MCU_ == _S12_)
#if defined(_MC9S12XHY256_H) || defined(_MC9S12HY64_H)
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void TIM0CH7_TIMER_ISR (void)
* @brief interrupt interrupt service routine for TIM0
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt interrupt service routine for TIM0
* 
* @see #lin_lld_timer_isr 
*//*END*----------------------------------------------------------------------*/ 
ISR(VectorNumber_Vtim0ch7, TIM0CH7_TIMER_ISR) {
  lin_lld_timer_isr();
  TIM0_TFLG1 = 128;                     /* Reset interrupt request flag */
}
#endif /* End for 9S12XHY256 and 9S12HY64 */

#if defined(_MC9S12P128_H) || defined(_MC9S12XS128_H) || defined(_MC9S12XS256_H) || defined(_MC9S12G128_H) || defined(_MC9S12G240_H)
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void TIM0_TIMER_ISR (void)
* @brief interrupt interrupt service routine for TIM timer
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt interrupt service routine for TIM timer
* 
* @see #lin_lld_timer_isr 
*//*END*----------------------------------------------------------------------*/ 
ISR(VectorNumber_Vtimch7, TIM0_TIMER_ISR) {
  lin_lld_timer_isr();
  TFLG1_C7F = 1;
}
#endif /* End for 9S12P128, 9S12XS128, 9S12XS256 and 9S12G128 */

#if (defined(_MC9S12VR64_H)||defined(_MC9S12VR32_H))
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void TIM0_TIMER_ISR (void)
* @brief interrupt interrupt service routine for TIM timer
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt interrupt service routine for TIM timer
* 
* @see #lin_lld_timer_isr 
*//*END*----------------------------------------------------------------------*/ 
ISR(VectorNumber_Vtimch0, TIM_TIMER_ISR) { 
  lin_lld_timer_isr();
  TFLG1 |= TFLG1_C0F_MASK;
  TC0   = (TC0 + ((MCU_BUS_FREQ/1000000)*TIME_BASE_PERIOD) ) & 0xFFFF;  
}

#endif /* End if (defined(_MC9S12VR64_H)||defined(_MC9S12VR32_H)) */

#if defined(_MC9S12GN32_H) || defined(_MC9S12G64_H)
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void TIM0_TIMER_ISR (void)
* @brief interrupt interrupt service routine for TIM timer
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt interrupt service routine for TIM timer
* 
* @see #lin_lld_timer_isr 
*//*END*----------------------------------------------------------------------*/ 
ISR(VectorNumber_Vtimch0, TIM_TIMER_ISR) { 
  lin_lld_timer_isr();
  TFLG1 |= TFLG1_C0F_MASK;
  TC0   = (TC0 + ((MCU_BUS_FREQ/1000000)*TIME_BASE_PERIOD) ) & 0xFFFF;  
}

#endif /* End defined(_MC9S12GN32_H) || defined(_MC9S12G64_H) */

#if (defined(_MC9S12ZVML128_H) || defined(_MC9S12ZVL32_H)||defined(_MC9S12ZVHY64_H)||defined(_MC9S12ZVH128_H)||defined(_MC9S12ZVML31_H))
void TIMchan2_ISR(void);
ISR(VectorNumber_Vtim0ch2, TIMchan2_ISR){
	lin_lld_timer_isr();
	/*Clear the flag */
	TIM0TFLG1_C2F = 0x01;
	TIM0TC2   = (TIM0TC2 + ((MCU_BUS_FREQ/1000000)*TIME_BASE_PERIOD) ) & 0xFFFF;
}
#endif /* End if (defined(_MC9S12ZVML128_H) || defined(_MC9S12ZVL32_H)||defined(_MC9S12ZVHY64_H)||defined(_MC9S12ZVH128_H)||defined(_MC9S12ZVML31_H)) */
#if (defined(_MC9S12ZVC64_H))
ISR(VectorNumber_Vtim1ch2, TIMchan2_ISR){
	lin_lld_timer_isr();
	/*Clear the flag */
	TIM1TFLG1_C2F = 0x01;
	TIM1TC2   = (TIM1TC2 + ((MCU_BUS_FREQ/500000)*TIME_BASE_PERIOD) ) & 0xFFFF;
}
#endif

#if (AUTOBAUD == 1)
void TIM_Input_capture_ISR(void);
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void TIM_Input_capture_ISR (void)
* @brief timer interrupt service routine for AUTOBAUD feature
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt interrupt service routine for TIM timer
* 
* @see #lin_lld_timer_isr 
*//*END*----------------------------------------------------------------------*/ 

#if (defined(_MC9S12ZVML128_H)||defined(_MC9S12ZVML31_H))
ISR(VectorNumber_Vtim0ch3, TIM_Input_capture_ISR){
	l_u8 i;
	/* TFLG1: C7F=0,C6F=0,C5F=0,C4F=0,C3F=1,C2F=0,C1F=0,C0F=0 */
	TIM0TFLG1_C3F = 1U;                       /* Reset interrupt request flag */
	if (autobaud_tmr_capture_index<10) {
			autobaud_tmr_capture[autobaud_tmr_capture_index]=TIM0TC3;
			autobaud_tmr_capture_index++;
	}
	if (autobaud_tmr_capture_index>=10) {
	        TIM0TCTL4_EDG3x = 0U; 				   /* disable LIN Timer after SYNC byte */
			autobaud_bitduration=0;
			for (autobaud_tmr_capture_index=1;autobaud_tmr_capture_index<9;autobaud_tmr_capture_index++) {
				autobaud_bitduration+= (l_u16) (autobaud_tmr_capture[autobaud_tmr_capture_index+1]-autobaud_tmr_capture[autobaud_tmr_capture_index]);
			}
			autobaud_bitduration=autobaud_bitduration/8;
			autobaud_tmr_capture_index=0;
			if (autobaud_current==19200) {
				if ((autobaud_bitduration>=800) && (autobaud_bitduration<=970)) {
					lin_lld_sci_init_autobaud (9600);
				}
			}
			if (autobaud_current==9600) {
				if ((autobaud_bitduration>=400) && (autobaud_bitduration<=500)) {
					lin_lld_sci_init_autobaud (19200);
				}
			}
			for (i=0;i<8;i++) {
					if (autobaud_current==19200)
						lin_max_frame_res_timeout_val_autobaud[i]=lin_max_frame_res_timeout_val_19200[i];
					if (autobaud_current==9600)
						lin_max_frame_res_timeout_val_autobaud[i]=lin_max_frame_res_timeout_val_9600[i];
			}
	}
}
#endif /* End if (defined(_MC9S12ZVML128_H)||defined(_MC9S12ZVML31_H))*/

#if (defined(_MC9S12ZVL32_H))
ISR(VectorNumber_Vtim0ch3, TIM_Input_capture_ISR){
	l_u8 i;
	/* TFLG1: C7F=0,C6F=0,C5F=0,C4F=0,C3F=1,C2F=0,C1F=0,C0F=0 */
	TIM0TFLG1_C3F = 1U;                       /* Reset interrupt request flag */
	if (autobaud_tmr_capture_index<10) {
			autobaud_tmr_capture[autobaud_tmr_capture_index]=TIM0TC3;
			autobaud_tmr_capture_index++;
	}
	if (autobaud_tmr_capture_index>=10) {
	        TIM0TCTL4_EDG3x = 0U; 				   /* disable LIN Timer after SYNC byte */
			autobaud_bitduration=0;
			for (autobaud_tmr_capture_index=1;autobaud_tmr_capture_index<9;autobaud_tmr_capture_index++) {
				autobaud_bitduration+= (l_u16) (autobaud_tmr_capture[autobaud_tmr_capture_index+1]-autobaud_tmr_capture[autobaud_tmr_capture_index]);
			}
			autobaud_bitduration=autobaud_bitduration/8;
			autobaud_tmr_capture_index=0;
			if (autobaud_current==19200) {
				if ((autobaud_bitduration>=750) && (autobaud_bitduration<=916)) {
					lin_lld_sci_init_autobaud (9600);
				}
			}
			if (autobaud_current==9600) {
				if ((autobaud_bitduration>=375) && (autobaud_bitduration<=458)) {
					lin_lld_sci_init_autobaud (19200);
				}
			}
			for (i=0;i<8;i++) {
					if (autobaud_current==19200)
						lin_max_frame_res_timeout_val_autobaud[i]=lin_max_frame_res_timeout_val_19200[i];
					if (autobaud_current==9600)
						lin_max_frame_res_timeout_val_autobaud[i]=lin_max_frame_res_timeout_val_9600[i];
			}
	}
}
#endif /* End (defined(_MC9S12ZVL32_H)*/

#if (defined(_MC9S12ZVH128_H))
ISR(VectorNumber_Vtim1ch0, TIM_Input_capture_ISR){
	l_u8 i;
	/* TFLG1: C7F=0,C6F=0,C5F=0,C4F=0,C3F=1,C2F=0,C1F=0,C0F=0 */
	TIM1TFLG1_C0F = 1U;                       /* Reset interrupt request flag */
	if (autobaud_tmr_capture_index<10) {
			autobaud_tmr_capture[autobaud_tmr_capture_index]=TIM1TC0;
			autobaud_tmr_capture_index++;
	}
	if (autobaud_tmr_capture_index>=10) {
	        TIM1TCTL4_EDG0x = 0U; 				   /* disable LIN Timer after SYNC byte */
			autobaud_bitduration=0;
			for (autobaud_tmr_capture_index=1;autobaud_tmr_capture_index<9;autobaud_tmr_capture_index++) {
				autobaud_bitduration+= (l_u16) (autobaud_tmr_capture[autobaud_tmr_capture_index+1]-autobaud_tmr_capture[autobaud_tmr_capture_index]);
			}
			autobaud_bitduration=autobaud_bitduration/8;
			autobaud_tmr_capture_index=0;
			if (autobaud_current==19200) {
				if ((autobaud_bitduration>=850) && (autobaud_bitduration<=970)) {
					lin_lld_sci_init_autobaud (9600);
				}
			}
			if (autobaud_current==9600) {
				if ((autobaud_bitduration>=400) && (autobaud_bitduration<=500)) {
					lin_lld_sci_init_autobaud (19200);
				}
			}
			for (i=0;i<8;i++) {
					if (autobaud_current==19200)
						lin_max_frame_res_timeout_val_autobaud[i]=lin_max_frame_res_timeout_val_19200[i];
					if (autobaud_current==9600)
						lin_max_frame_res_timeout_val_autobaud[i]=lin_max_frame_res_timeout_val_9600[i];
			}
	}
}
#endif /* End (defined(_MC9S12ZVH128_H)*/

#if (defined(_MC9S12VR64_H)||defined(_MC9S12VR32_H))
ISR(VectorNumber_Vtimch3, TIM_Input_capture_ISR){
	l_u8 i;
	/* TFLG1: C7F=0,C6F=0,C5F=0,C4F=0,C3F=1,C2F=0,C1F=0,C0F=0 */
	TFLG1_C3F = 1U;                       /* Reset interrupt request flag */
	if (autobaud_tmr_capture_index<10) {
			autobaud_tmr_capture[autobaud_tmr_capture_index]=TC3;
			autobaud_tmr_capture_index++;
	}
	if (autobaud_tmr_capture_index>=10) {
	    TCTL4_EDG3x = 0U; 				   /* disable LIN Timer after SYNC byte */
			autobaud_bitduration=0;
			for (autobaud_tmr_capture_index=1;autobaud_tmr_capture_index<9;autobaud_tmr_capture_index++) {
				autobaud_bitduration+= (l_u16) (autobaud_tmr_capture[autobaud_tmr_capture_index+1]-autobaud_tmr_capture[autobaud_tmr_capture_index]);
			}
			autobaud_bitduration=autobaud_bitduration/8;
			autobaud_tmr_capture_index=0;
			if (autobaud_current==19200) {   
				/* Set baudrate as 9600 bps */
				if ((autobaud_bitduration>=850) && (autobaud_bitduration<=970)) {
					lin_lld_sci_init_autobaud (9600);
				}
			}
			/* Set baudrate as 19200 bps */
			if (autobaud_current==9600) {
				if ((autobaud_bitduration>=400) && (autobaud_bitduration<=500)) {
					lin_lld_sci_init_autobaud (19200);
				}
			}
			/* Reset timeout value */
			for (i=0;i<8;i++) {
					if (autobaud_current==19200)
						lin_max_frame_res_timeout_val_autobaud[i]=lin_max_frame_res_timeout_val_19200[i];
					if (autobaud_current==9600)
						lin_max_frame_res_timeout_val_autobaud[i]=lin_max_frame_res_timeout_val_9600[i];
			}
	}
}
#endif /* End if (defined(_MC9S12VR64_H)||defined(_MC9S12VR32_H))*/

#endif /* End (AUTOBAUD == 1)*/

#endif /* End (_MCU_ == _S12_) */     

#if (_MCU_ == _S08_)
/*FUNCTION*--------------------------------------------------------------*//**
* @fn void TPM1_TIMER_ISR(void)
* @brief interrupt service routine for TPM1/ FTM timer
*  
* @return #void
* 
* @SDD_ID N/A
* @endif
* 
* @details
*   interrupt service routine for TPM1/ FTM timer
* 
* @see #lin_lld_timer_isr 
*//*END*----------------------------------------------------------------------*/
#if defined(_MC9S08RN60_H)

ISR(VectorNumber_Vmtim0, TPM1_TIMER_ISR) {
  /* CH0F = 0 */
    MTIM0_SC &= ~MTIM0_SC_TOF_MASK;
    lin_lld_timer_isr(); 
}
#else
ISR(VectorNumber_Vtpm1ch0, TPM1_TIMER_ISR) {
  /* CH0F = 0 */
  TPM1C0SC &= ~TPM1C0SC_CH0F_MASK;
  lin_lld_timer_isr(); 
}
#endif /* End defined(_MC9S08RN60_H)*/
#endif /* End (_MCU_ == _S08_) */

#if ((!defined(_MC9S12ZVML128_H))&&(!defined(_MC9S12ZVML31_H)))                                      
#pragma CODE_SEG DEFAULT /* Return to default code segment */
#endif /* End if ((!defined(_MC9S12ZVML128_H))&&(!defined(_MC9S12ZVML31_H))) */

#endif /* End defined(_MC9S12I32_H) */
/**
* @}
*/



