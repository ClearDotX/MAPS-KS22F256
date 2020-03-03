/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
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
     
#ifndef __FSL_GPIO_HAL_TEST_H__
#define __FSL_GPIO_HAL_TEST_H__
 
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include "fsl_gpio_hal.h"
#include "fsl_port_hal.h"
#include "fsl_interrupt_manager.h"
#include "fsl_device_registers.h"
#include "board.h"
 
/*****************************************************************************
 * Function Prototypes
 ****************************************************************************/

/*! @name GPIO HAL Driver Test*/
/*@{*/

/*!
 * @brief Simple test of the GPIO and porting the HAL driver.
 *
 * This function tests the initial PORT and interrupt, and the GPIO input/output.
 *
 * Board Setting:
 *    TWR-K60D100M, TWR-K64F120M, TWR-K70F120M use PTC12/PTC13 as output/input.
 *    TWR-KL25Z use PTC5/PTC6 as output/input
 *
 * Procedure:
 * 1. Connect the InputPin and OutputPin which are defined above.
 * 2. First, reset the required port and the GPIO.
 * 3. Then, set the Output port as a digital output and the input port as a
 *    digital input. The input port interrupt is enabled to detect the rising edge. 
 *    if the output port is set to high logic, an interrupt is generated. 
 *    LED is lit inside the PORTE ISR. 
 */
#if defined(TWR_K60D100M) || defined(TWR_K70F120M)
#define OutputPortBaseAddr   PORTC
#define OutputGpioBaseAddr   PTC

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define OutputFGpioBaseAddr  FPTC
#endif

#define OutputPin            12

#define InputPortBaseAddr    PORTC
#define InputGpioBaseAddr    PTC

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define InputFGpioBaseAddr   FPTC
#endif

#define InputPin             13
#define Input_IRQn           PORTC_IRQn

#define LEDPortBaseAddr      PORTA
#define LEDGpioBaseAddr      PTA
#define LEDPin               11

#elif defined(TWR_K80F150M) || defined(TWR_K81F150M) 
#define OutputPortBaseAddr   PORTC
#define OutputGpioBaseAddr   GPIOC

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define OutputFGpioBaseAddr  FGPIOC
#endif

#define OutputPin            12

#define InputPortBaseAddr    PORTC
#define InputGpioBaseAddr    GPIOC

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define InputFGpioBaseAddr   FGPIOC
#endif

#define InputPin             13
#define Input_IRQn           PORTC_IRQn

#define LEDPortBaseAddr      PORTD
#define LEDGpioBaseAddr      GPIOD
#define LEDPin               11

#elif defined(FRDM_K82F) 
#define OutputPortBaseAddr   PORTC
#define OutputGpioBaseAddr   GPIOC

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define OutputFGpioBaseAddr  FGPIOC
#endif

#define OutputPin            12

#define InputPortBaseAddr    PORTC
#define InputGpioBaseAddr    GPIOC

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define InputFGpioBaseAddr   FGPIOC
#endif

#define InputPin             11
#define Input_IRQn           PORTC_IRQn

#define LEDPortBaseAddr      PORTC
#define LEDGpioBaseAddr      GPIOC
#define LEDPin               8

#elif defined(TWR_KL25Z48M)
#define OutputPortBaseAddr   PORTC
#define OutputGpioBaseAddr   PTC

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define OutputFGpioBaseAddr  FPTC
#endif

#define OutputPin            5

#define InputPortBaseAddr    PORTC
#define InputGpioBaseAddr    PTC

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define InputFGpioBaseAddr   FPTC
#endif

#define InputPin             6
#define Input_IRQn           PORTC_IRQn

#define LEDPortBaseAddr      PORTB
#define LEDGpioBaseAddr      PTB
#define LEDPin               18

#elif defined(TWR_K64F120M)
#define OutputPortBaseAddr   PORTC
#define OutputGpioBaseAddr   PTC

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define OutputFGpioBaseAddr  FPTC
#endif

#define OutputPin            12

#define InputPortBaseAddr    PORTC
#define InputGpioBaseAddr    PTC

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define InputFGpioBaseAddr   FPTC
#endif

#define InputPin             13
#define Input_IRQn           PORTC_IRQn

#define LEDPortBaseAddr      PORTE
#define LEDGpioBaseAddr      PTE
#define LEDPin               9

#elif defined(TWR_K21F120M)
#define OutputPortBaseAddr   PORTE
#define OutputGpioBaseAddr   PTE

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define OutputFGpioBaseAddr  FPTE
#endif

#define OutputPin            5

#define InputPortBaseAddr    PORTC
#define InputGpioBaseAddr    PTC

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define InputFGpioBaseAddr   FPTC
#endif

#define InputPin             13
#define Input_IRQn           PORTC_IRQn

#define LEDPortBaseAddr      PORTD
#define LEDGpioBaseAddr      PTD
#define LEDPin               7

#elif defined(TWR_K22F120M) || defined(TWR_K22F120M128R) || defined(TWR_K22F120M256R)
#define OutputPortBaseAddr   PORTC
#define OutputGpioBaseAddr   PTC

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define OutputFGpioBaseAddr  FPTC
#endif

#define OutputPin            12

#define InputPortBaseAddr    PORTC
#define InputGpioBaseAddr    PTC

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define InputFGpioBaseAddr   FPTC
#endif

#define InputPin             13
#define Input_IRQn           PORTC_IRQn

#define LEDPortBaseAddr      PORTD
#define LEDGpioBaseAddr      PTD
#define LEDPin               7

#elif defined(TWR_K24F120M)
#define OutputPortBaseAddr   PORTD
#define OutputGpioBaseAddr   PTD

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define OutputFGpioBaseAddr  FPTD
#endif

#define OutputPin            2

#define InputPortBaseAddr    PORTD
#define InputGpioBaseAddr    PTD

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define InputFGpioBaseAddr   FPTD
#endif

#define InputPin             3
#define Input_IRQn           PORTD_IRQn

#define LEDPortBaseAddr      PORTD
#define LEDGpioBaseAddr      PTD
#define LEDPin               4

#elif defined(TWR_KV10Z32)
#define OutputPortBaseAddr   PORTA
#define OutputGpioBaseAddr   PTA

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define OutputFGpioBaseAddr  FPTA
#endif

#define OutputPin            1

#define InputPortBaseAddr    PORTA
#define InputGpioBaseAddr    PTA

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define InputFGpioBaseAddr   FPTA
#endif

#define InputPin             2
#define Input_IRQn           PORTA_IRQn

#define LEDPortBaseAddr      PORTC
#define LEDGpioBaseAddr      PTC
#define LEDPin               1

#elif defined(TWR_KV11Z75M)
#define OutputPortBaseAddr   PORTE
#define OutputGpioBaseAddr   GPIOE

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define OutputFGpioBaseAddr  FGPIOE
#endif

#define OutputPin            16

#define InputPortBaseAddr    PORTE
#define InputGpioBaseAddr    GPIOE

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define InputFGpioBaseAddr   FGPIOE
#endif

#define InputPin             17
#define Input_IRQn           PORTB_PORTC_PORTD_PORTE_IRQn

#define LEDPortBaseAddr      PORTD
#define LEDGpioBaseAddr      GPIOD
#define LEDPin               7

#elif defined(TWR_KV31F120M) || defined(TWR_KV31F120M128R) || defined(TWR_KV31F120M256R) || \
      defined(TWR_KV30F100M)
#define OutputPortBaseAddr   PORTC
#define OutputGpioBaseAddr   PTC

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define OutputFGpioBaseAddr  FPTC
#endif

#define OutputPin            12

#define InputPortBaseAddr    PORTC
#define InputGpioBaseAddr    PTC

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define InputFGpioBaseAddr   FPTC
#endif

#define InputPin             13
#define Input_IRQn           PORTC_IRQn

#define LEDPortBaseAddr      PORTE
#define LEDGpioBaseAddr      PTE
#define LEDPin               1

#elif defined(FRDM_K22F)
#define OutputPortBaseAddr   PORTD
#define OutputGpioBaseAddr   PTD

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define OutputFGpioBaseAddr  FPTD
#endif

#define OutputPin            6

#define InputPortBaseAddr    PORTD
#define InputGpioBaseAddr    PTD

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define InputFGpioBaseAddr   FPTD
#endif

#define InputPin             7
#define Input_IRQn           PORTD_IRQn

#define LEDPortBaseAddr      PORTA
#define LEDGpioBaseAddr      PTA
#define LEDPin               2

#elif defined(FRDM_K64F)
#define OutputPortBaseAddr   PORTC
#define OutputGpioBaseAddr   PTC

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define OutputFGpioBaseAddr  FPTC
#endif

#define OutputPin            16

#define InputPortBaseAddr    PORTC
#define InputGpioBaseAddr    PTC

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define InputFGpioBaseAddr   FPTC
#endif

#define InputPin             17
#define Input_IRQn           PORTC_IRQn

#define LEDPortBaseAddr      PORTE
#define LEDGpioBaseAddr      PTE
#define LEDPin               26

#elif defined(FRDM_KL03Z)
#define OutputPortBaseAddr   PORTA
#define OutputGpioBaseAddr   PTA

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define OutputFGpioBaseAddr  FPTA
#endif

#define OutputPin            6

#define InputPortBaseAddr    PORTA
#define InputGpioBaseAddr    PTA

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define InputFGpioBaseAddr   FPTA
#endif

#define InputPin             7
#define Input_IRQn           PORTA_IRQn

#define LEDPortBaseAddr      PORTB
#define LEDGpioBaseAddr      PTB
#define LEDPin               10

#elif defined(FRDM_KL26Z) || defined(FRDM_KL46Z) || defined(MRB_KW01)
#define OutputPortBaseAddr   PORTD
#define OutputGpioBaseAddr   PTD

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define OutputFGpioBaseAddr  FPTD
#endif

#define OutputPin            6

#define InputPortBaseAddr    PORTD
#define InputGpioBaseAddr    PTD

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define InputFGpioBaseAddr   FPTD
#endif

#define InputPin             7
#define Input_IRQn           PORTC_PORTD_IRQn

#define LEDPortBaseAddr      PORTD
#define LEDGpioBaseAddr      PTD
#define LEDPin               5
        
#elif defined(FRDM_KL25Z)
#define OutputPortBaseAddr   PORTD
#define OutputGpioBaseAddr   PTD

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define OutputFGpioBaseAddr  FPTD
#endif

#define OutputPin            6

#define InputPortBaseAddr    PORTD
#define InputGpioBaseAddr    PTD

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define InputFGpioBaseAddr   FPTD
#endif

#define InputPin             7
#define Input_IRQn           PORTD_IRQn

#define LEDPortBaseAddr      PORTB
#define LEDGpioBaseAddr      PTB
#define LEDPin               18

#elif defined(FRDM_KL27Z)
#define OutputPortBaseAddr   PORTD
#define OutputGpioBaseAddr   PTD

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define OutputFGpioBaseAddr  FPTD
#endif

#define OutputPin            6

#define InputPortBaseAddr    PORTD
#define InputGpioBaseAddr    PTD

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define InputFGpioBaseAddr   FPTD
#endif

#define InputPin             7
#define Input_IRQn           PORTBCDE_IRQn

#define LEDPortBaseAddr      PORTB
#define LEDGpioBaseAddr      PTB
#define LEDPin               18        

#elif defined(TWR_KL43Z48M)
#define OutputPortBaseAddr   PORTB
#define OutputGpioBaseAddr   PTB

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define OutputFGpioBaseAddr  FPTB
#endif

#define OutputPin            1

#define InputPortBaseAddr    PORTA
#define InputGpioBaseAddr    PTA

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define InputFGpioBaseAddr   FPTA
#endif

#define InputPin             2
#define Input_IRQn           PORTA_IRQn

#define LEDPortBaseAddr      PORTA
#define LEDGpioBaseAddr      PTA
#define LEDPin               12   
       
#elif defined(FRDM_KL43Z)
#define OutputPortBaseAddr   PORTB
#define OutputGpioBaseAddr   PTB

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define OutputFGpioBaseAddr  FPTB
#endif

#define OutputPin            18

#define InputPortBaseAddr    PORTA
#define InputGpioBaseAddr    PTA

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define InputFGpioBaseAddr   FPTA
#endif

#define InputPin             1
#define Input_IRQn           PORTA_IRQn

#define LEDPortBaseAddr      PORTD
#define LEDGpioBaseAddr      PTD
#define LEDPin               5 

#elif defined(FRDM_KL02Z)
#define OutputPortBaseAddr   PORTA
#define OutputGpioBaseAddr   PTA

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define OutputFGpioBaseAddr  FPTA
#endif

#define OutputPin            11

#define InputPortBaseAddr    PORTA
#define InputGpioBaseAddr    PTA

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define InputFGpioBaseAddr   FPTA
#endif

#define InputPin             12
#define Input_IRQn           PORTA_IRQn

#define LEDPortBaseAddr      PORTB
#define LEDGpioBaseAddr      PTB
#define LEDPin               6
        
/* TWR-KM34Z75M */
#elif defined(TWR_KM34Z75M)                   
#define OutputPortBaseAddr   PORTF
#define OutputGpioBaseAddr   GPIOF

#define OutputPin            5

#define InputPortBaseAddr    PORTF
#define InputGpioBaseAddr    GPIOF

#define InputPin             6
#define Input_IRQn           PTx_IRQn

#define LEDPortBaseAddr      PORTJ
#define LEDGpioBaseAddr      GPIOJ
#define LEDPin               3         
#elif defined(TWR_K65F180M)
#define OutputPortBaseAddr   PORTE
#define OutputGpioBaseAddr   PTE
#define OutputPin            5

#define InputPortBaseAddr    PORTA
#define InputGpioBaseAddr    PTA
#define InputPin             9
#define Input_IRQn           PORTA_IRQn

#define LEDPortBaseAddr      PORTB
#define LEDGpioBaseAddr      PTB
#define LEDPin               4
        
#elif defined(FRDM_KW40Z)
#define OutputPortBaseAddr   PORTC
#define OutputGpioBaseAddr   GPIOC
#define OutputPin            2

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define OutputFGpioBaseAddr  FGPIOC
#endif        
        
#define InputPortBaseAddr    PORTC
#define InputGpioBaseAddr    GPIOC
#define InputPin             3
#define Input_IRQn           PORTB_PORTC_IRQn

#if FSL_FEATURE_GPIO_HAS_FAST_GPIO
#define InputFGpioBaseAddr   FGPIOC
#endif

#define LEDPortBaseAddr      PORTC
#define LEDGpioBaseAddr      GPIOC
#define LEDPin               0        
       
#endif

/*! @}*/
 
#endif /* __FSL_GPIO_HAL_TEST_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/
