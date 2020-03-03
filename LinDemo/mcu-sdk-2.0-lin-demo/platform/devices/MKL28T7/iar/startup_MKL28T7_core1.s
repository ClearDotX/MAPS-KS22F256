; ---------------------------------------------------------------------------------------
;  @file:    startup_MKL28T7_CORE1.s
;  @purpose: CMSIS Cortex-M0P Core Device Startup File
;            MKL28T7_CORE1
;  @version: 1.7
;  @date:    2015-5-16
;  @build:   b151106
; ---------------------------------------------------------------------------------------
;
; Copyright (c) 1997 - 2015 , Freescale Semiconductor, Inc.
; All rights reserved.
;
; Redistribution and use in source and binary forms, with or without modification,
; are permitted provided that the following conditions are met:
;
; o Redistributions of source code must retain the above copyright notice, this list
;   of conditions and the following disclaimer.
;
; o Redistributions in binary form must reproduce the above copyright notice, this
;   list of conditions and the following disclaimer in the documentation and/or
;   other materials provided with the distribution.
;
; o Neither the name of Freescale Semiconductor, Inc. nor the names of its
;   contributors may be used to endorse or promote products derived from this
;   software without specific prior written permission.
;
; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
; ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
; WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
; DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
; ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
; (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
; ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
; (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
; SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA

__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler

        DCD     NMI_Handler                                   ;NMI Handler
        DCD     HardFault_Handler                             ;Hard Fault Handler
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
__vector_table_0x1c
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     SVC_Handler                                   ;SVCall Handler
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     PendSV_Handler                                ;PendSV Handler
        DCD     SysTick_Handler                               ;SysTick Handler

                                                              ;External Interrupts
        DCD     DMA1_04_IRQHandler                            ;DMA1 channel 0/4 transfer complete
        DCD     DMA1_15_IRQHandler                            ;DMA1 channel 1/5 transfer complete
        DCD     DMA1_26_IRQHandler                            ;DMA1 channel 2/6 transfer complete
        DCD     DMA1_37_IRQHandler                            ;DMA1 channel 3/7 transfer complete
        DCD     CTI1_DMA1_Error_IRQHandler                    ;CTI1 or DMA1 error
        DCD     FLEXIO0_IRQHandler                            ;FLEXIO0
        DCD     TPM0_IRQHandler                               ;TPM0 single interrupt vector for all sources
        DCD     TPM1_IRQHandler                               ;TPM1 single interrupt vector for all sources
        DCD     Reserved24_IRQHandler                         ;Reserved interrupt
        DCD     LPIT1_IRQHandler                              ;LPIT1 interrupt
        DCD     LPSPI0_IRQHandler                             ;LPSPI0 single interrupt vector for all sources
        DCD     LPSPI1_IRQHandler                             ;LPSPI1 single interrupt vector for all sources
        DCD     LPUART0_IRQHandler                            ;LPUART0 status and error
        DCD     LPUART1_IRQHandler                            ;LPUART1 status and error
        DCD     LPI2C0_IRQHandler                             ;LPI2C0 interrupt
        DCD     LPI2C1_IRQHandler                             ;LPI2C1 interrupt
        DCD     MU0_B_IRQHandler                              ;MU 0 Side B interrupt
        DCD     PORTM_IRQHandler                              ;PORTM Pin detect
        DCD     TRNG_IRQHandler                               ;TRNG
        DCD     TSI0_IRQHandler                               ;TSI0
        DCD     DAC0_IRQHandler                               ;DAC0
        DCD     CMP1_IRQHandler                               ;CMP1
        DCD     LLWU1_IRQHandler                              ;Low leakage wakeup 1
        DCD     I2S0_IRQHandler                               ;I2S0 interrupt
        DCD     USB0_IRQHandler                               ;USB0 interrupt
        DCD     ADC0_IRQHandler                               ;ADC0 interrupt
        DCD     LPTMR1_IRQHandler                             ;LPTMR1 interrupt
        DCD     RTC_Seconds_IRQHandler                        ;RTC seconds
        DCD     INTMUX1_0_IRQHandler                          ;INTMUX1_0 interrupt
        DCD     INTMUX1_1_IRQHandler                          ;INTMUX1_1 interrupt
        DCD     INTMUX1_2_IRQHandler                          ;INTMUX1_2 interrupt
        DCD     INTMUX1_3_IRQHandler                          ;INTMUX1_3 interrupt
        DCD     LPTMR0_IRQHandler                             ;LPTMR0 interrupt (INTMUX source IRQ0)
        DCD     LPIT0_IRQHandler                              ;LPIT0 interrupt (INTMUX source IRQ1)
        DCD     TPM2_IRQHandler                               ;TPM2 single interrupt vector for all sources (INTMUX source IRQ2)
        DCD     Reserved51_IRQHandler                         ;Reserved interrupt
        DCD     LPSPI2_IRQHandler                             ;LPSPI2 single interrupt vector for all sources (INTMUX source IRQ4)
        DCD     LPUART2_IRQHandler                            ;LPUART2 status and error (INTMUX source IRQ5)
        DCD     EMVSIM0_IRQHandler                            ;EMVSIM0 interrupt (INTMUX source IRQ6)
        DCD     LPI2C2_IRQHandler                             ;LPI2C2 interrupt (INTMUX source IRQ7)
        DCD     Reserved56_IRQHandler                         ;Reserved interrupt
        DCD     PMC_IRQHandler                                ;PMC interrupt (INTMUX source IRQ9)
        DCD     FTFA_IRQHandler                               ;FTFA interrupt (INTMUX source IRQ10)
        DCD     SCG_IRQHandler                                ;SCG interrupt (INTMUX source IRQ11)
        DCD     XRDC_IRQHandler                               ;XRDC interrupt (INTMUX source IRQ12)
        DCD     Reserved61_IRQHandler                         ;Reserved interrupt
        DCD     Reserved62_IRQHandler                         ;Reserved interrupt
        DCD     RCM_IRQHandler                                ;RCM interrupt (INTMUX source IRQ15)
        DCD     CMP0_IRQHandler                               ;CMP0 interrupt (INTMUX source IRQ16)
        DCD     Reserved65_IRQHandler                         ;Reserved interrupt
        DCD     RTC_IRQHandler                                ;RTC Alarm interrupt (INTMUX source IRQ18)
        DCD     PORTA_IRQHandler                              ;PORTA Pin detect (INTMUX source IRQ19)
        DCD     PORTB_IRQHandler                              ;PORTB Pin detect (INTMUX source IRQ20)
        DCD     PORTC_IRQHandler                              ;PORTC Pin detect (INTMUX source IRQ21)
        DCD     PORTD_IRQHandler                              ;PORTD Pin detect (INTMUX source IRQ22)
        DCD     PORTE_IRQHandler                              ;PORTE Pin detect (INTMUX source IRQ23)
        DCD     DMA0_04_IRQHandler                            ;DMA0 channel 0/4 transfer complete (INTMUX source IRQ24)
        DCD     DMA0_15_IRQHandler                            ;DMA0 channel 1/5 transfer complete (INTMUX source IRQ25)
        DCD     DMA0_26_IRQHandler                            ;DMA0 channel 2/6 transfer complete (INTMUX source IRQ26)
        DCD     DMA0_37_IRQHandler                            ;DMA0 channel 3/7 transfer complete (INTMUX source IRQ27)
        DCD     DMA0_Error_IRQHandler                         ;DMA0 error (INTMUX source IRQ28)
        DCD     Reserved77_IRQHandler                         ;Reserved interrupt
        DCD     Reserved78_IRQHandler                         ;Reserved interrupt
        DCD     Reserved79_IRQHandler                         ;Reserved interrupt
__Vectors_End

        SECTION FlashConfig:CODE
__FlashConfig
        DCD 0xFFFFFFFF
        DCD 0xFFFFFFFF
        DCD 0xFFFFFFFF
        DCD 0xFFFF3DFE
__FlashConfig_End

__Vectors       EQU   __vector_table
__Vectors_Size  EQU   __Vectors_End - __Vectors


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        CPSID   I               ; Mask interrupts
        LDR     R0, =0xE000ED08
        LDR     R1, =__vector_table
        STR     R1, [R0]
        LDR     R0, =SystemInit
        BLX     R0
        CPSIE   I               ; Unmask interrupts
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B .

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B .

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B .

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B .

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B .

        PUBWEAK DMA1_04_IRQHandler
        PUBWEAK DMA1_04_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA1_04_IRQHandler
        LDR     R0, =DMA1_04_DriverIRQHandler
        BX      R0

        PUBWEAK DMA1_15_IRQHandler
        PUBWEAK DMA1_15_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA1_15_IRQHandler
        LDR     R0, =DMA1_15_DriverIRQHandler
        BX      R0

        PUBWEAK DMA1_26_IRQHandler
        PUBWEAK DMA1_26_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA1_26_IRQHandler
        LDR     R0, =DMA1_26_DriverIRQHandler
        BX      R0

        PUBWEAK DMA1_37_IRQHandler
        PUBWEAK DMA1_37_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA1_37_IRQHandler
        LDR     R0, =DMA1_37_DriverIRQHandler
        BX      R0

        PUBWEAK CTI1_DMA1_Error_IRQHandler
        PUBWEAK CTI1_DMA1_Error_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
CTI1_DMA1_Error_IRQHandler
        LDR     R0, =CTI1_DMA1_Error_DriverIRQHandler
        BX      R0

        PUBWEAK FLEXIO0_IRQHandler
        PUBWEAK FLEXIO0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
FLEXIO0_IRQHandler
        LDR     R0, =FLEXIO0_DriverIRQHandler
        BX      R0

        PUBWEAK TPM0_IRQHandler
        PUBWEAK TPM1_IRQHandler
        PUBWEAK Reserved24_IRQHandler
        PUBWEAK LPIT1_IRQHandler
        PUBWEAK LPSPI0_IRQHandler
        PUBWEAK LPSPI0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPSPI0_IRQHandler
        LDR     R0, =LPSPI0_DriverIRQHandler
        BX      R0

        PUBWEAK LPSPI1_IRQHandler
        PUBWEAK LPSPI1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPSPI1_IRQHandler
        LDR     R0, =LPSPI1_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART0_IRQHandler
        PUBWEAK LPUART0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART0_IRQHandler
        LDR     R0, =LPUART0_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART1_IRQHandler
        PUBWEAK LPUART1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART1_IRQHandler
        LDR     R0, =LPUART1_DriverIRQHandler
        BX      R0

        PUBWEAK LPI2C0_IRQHandler
        PUBWEAK LPI2C0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C0_IRQHandler
        LDR     R0, =LPI2C0_DriverIRQHandler
        BX      R0

        PUBWEAK LPI2C1_IRQHandler
        PUBWEAK LPI2C1_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C1_IRQHandler
        LDR     R0, =LPI2C1_DriverIRQHandler
        BX      R0

        PUBWEAK MU0_B_IRQHandler
        PUBWEAK PORTM_IRQHandler
        PUBWEAK TRNG_IRQHandler
        PUBWEAK TSI0_IRQHandler
        PUBWEAK DAC0_IRQHandler
        PUBWEAK CMP1_IRQHandler
        PUBWEAK LLWU1_IRQHandler
        PUBWEAK I2S0_IRQHandler
        PUBWEAK I2S0_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
I2S0_IRQHandler
        LDR     R0, =I2S0_DriverIRQHandler
        BX      R0

        PUBWEAK USB0_IRQHandler
        PUBWEAK ADC0_IRQHandler
        PUBWEAK LPTMR1_IRQHandler
        PUBWEAK RTC_Seconds_IRQHandler
        PUBWEAK INTMUX1_0_IRQHandler
        PUBWEAK INTMUX1_1_IRQHandler
        PUBWEAK INTMUX1_2_IRQHandler
        PUBWEAK INTMUX1_3_IRQHandler
        PUBWEAK LPTMR0_IRQHandler
        PUBWEAK LPIT0_IRQHandler
        PUBWEAK TPM2_IRQHandler
        PUBWEAK Reserved51_IRQHandler
        PUBWEAK LPSPI2_IRQHandler
        PUBWEAK LPSPI2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPSPI2_IRQHandler
        LDR     R0, =LPSPI2_DriverIRQHandler
        BX      R0

        PUBWEAK LPUART2_IRQHandler
        PUBWEAK LPUART2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPUART2_IRQHandler
        LDR     R0, =LPUART2_DriverIRQHandler
        BX      R0

        PUBWEAK EMVSIM0_IRQHandler
        PUBWEAK LPI2C2_IRQHandler
        PUBWEAK LPI2C2_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
LPI2C2_IRQHandler
        LDR     R0, =LPI2C2_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved56_IRQHandler
        PUBWEAK PMC_IRQHandler
        PUBWEAK FTFA_IRQHandler
        PUBWEAK SCG_IRQHandler
        PUBWEAK XRDC_IRQHandler
        PUBWEAK Reserved61_IRQHandler
        PUBWEAK Reserved62_IRQHandler
        PUBWEAK RCM_IRQHandler
        PUBWEAK CMP0_IRQHandler
        PUBWEAK Reserved65_IRQHandler
        PUBWEAK RTC_IRQHandler
        PUBWEAK PORTA_IRQHandler
        PUBWEAK PORTB_IRQHandler
        PUBWEAK PORTC_IRQHandler
        PUBWEAK PORTD_IRQHandler
        PUBWEAK PORTE_IRQHandler
        PUBWEAK DMA0_04_IRQHandler
        PUBWEAK DMA0_04_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_04_IRQHandler
        LDR     R0, =DMA0_04_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_15_IRQHandler
        PUBWEAK DMA0_15_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_15_IRQHandler
        LDR     R0, =DMA0_15_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_26_IRQHandler
        PUBWEAK DMA0_26_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_26_IRQHandler
        LDR     R0, =DMA0_26_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_37_IRQHandler
        PUBWEAK DMA0_37_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_37_IRQHandler
        LDR     R0, =DMA0_37_DriverIRQHandler
        BX      R0

        PUBWEAK DMA0_Error_IRQHandler
        PUBWEAK DMA0_Error_DriverIRQHandler
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_Error_IRQHandler
        LDR     R0, =DMA0_Error_DriverIRQHandler
        BX      R0

        PUBWEAK Reserved77_IRQHandler
        PUBWEAK Reserved78_IRQHandler
        PUBWEAK Reserved79_IRQHandler
        PUBWEAK DefaultISR
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA1_04_DriverIRQHandler
DMA1_15_DriverIRQHandler
DMA1_26_DriverIRQHandler
DMA1_37_DriverIRQHandler
CTI1_DMA1_Error_DriverIRQHandler
FLEXIO0_DriverIRQHandler
TPM0_IRQHandler
TPM1_IRQHandler
Reserved24_IRQHandler
LPIT1_IRQHandler
LPSPI0_DriverIRQHandler
LPSPI1_DriverIRQHandler
LPUART0_DriverIRQHandler
LPUART1_DriverIRQHandler
LPI2C0_DriverIRQHandler
LPI2C1_DriverIRQHandler
MU0_B_IRQHandler
PORTM_IRQHandler
TRNG_IRQHandler
TSI0_IRQHandler
DAC0_IRQHandler
CMP1_IRQHandler
LLWU1_IRQHandler
I2S0_DriverIRQHandler
USB0_IRQHandler
ADC0_IRQHandler
LPTMR1_IRQHandler
RTC_Seconds_IRQHandler
INTMUX1_0_IRQHandler
INTMUX1_1_IRQHandler
INTMUX1_2_IRQHandler
INTMUX1_3_IRQHandler
LPTMR0_IRQHandler
LPIT0_IRQHandler
TPM2_IRQHandler
Reserved51_IRQHandler
LPSPI2_DriverIRQHandler
LPUART2_DriverIRQHandler
EMVSIM0_IRQHandler
LPI2C2_DriverIRQHandler
Reserved56_IRQHandler
PMC_IRQHandler
FTFA_IRQHandler
SCG_IRQHandler
XRDC_IRQHandler
Reserved61_IRQHandler
Reserved62_IRQHandler
RCM_IRQHandler
CMP0_IRQHandler
Reserved65_IRQHandler
RTC_IRQHandler
PORTA_IRQHandler
PORTB_IRQHandler
PORTC_IRQHandler
PORTD_IRQHandler
PORTE_IRQHandler
DMA0_04_DriverIRQHandler
DMA0_15_DriverIRQHandler
DMA0_26_DriverIRQHandler
DMA0_37_DriverIRQHandler
DMA0_Error_DriverIRQHandler
Reserved77_IRQHandler
Reserved78_IRQHandler
Reserved79_IRQHandler
DefaultISR
        LDR R0, =DefaultISR
        BX R0

        END