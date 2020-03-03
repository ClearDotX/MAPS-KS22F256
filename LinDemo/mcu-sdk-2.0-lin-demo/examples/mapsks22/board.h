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

#ifndef _BOARD_H_
#define _BOARD_H_

#include "clock_config.h"
#include "fsl_gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* The board name */
#define BOARD_NAME "MAPS-KS22"

/* The UART to use for debug messages. */
#define BOARD_DEBUG_UART_TYPE       DEBUG_CONSOLE_DEVICE_TYPE_UART
#define BOARD_DEBUG_UART_BASEADDR   (uint32_t)UART1
#define BOARD_DEBUG_UART_CLKSRC     kCLOCK_PllFllSelClk
#define BOARD_UART_IRQ              UART1_RX_TX_IRQn
#define BOARD_UART_IRQ_HANDLER      UART1_RX_TX_IRQHandler

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/* Define the port interrupt number for the board switches */
/* Switch K2 Wakeup on MAPS-KS22F256 board */
#define BOARD_SW2_GPIO GPIOD
#define BOARD_SW2_PORT PORTD
#define BOARD_SW2_GPIO_PIN 0

#define BOARD_SW2_IRQ PORTD_IRQn
#define BOARD_SW2_IRQ_HANDLER PORTD_IRQHandler
#define BOARD_SW2_NAME "K2 Wakeup"

/* Switch 3 on MAPS-DOCK board */
#define BOARD_SW3_GPIO GPIOC
#define BOARD_SW3_PORT PORTC
#define BOARD_SW3_GPIO_PIN 14U

#define BOARD_SW3_IRQ PORTC_IRQn
#define BOARD_SW3_IRQ_HANDLER PORTC_IRQHandler
#define BOARD_SW3_NAME "Dock K3"

/* Board led color mapping */
#define LOGIC_LED_ON 0U
#define LOGIC_LED_OFF 1U
#define BOARD_LED_RED1_GPIO GPIOB
#define BOARD_LED_RED1_GPIO_PORT PORTB
#define BOARD_LED_RED1_GPIO_PIN 3U
#define BOARD_LED_RED2_GPIO GPIOB
#define BOARD_LED_RED2_GPIO_PORT PORTB
#define BOARD_LED_RED2_GPIO_PIN 9U
#define BOARD_LED_RED3_GPIO GPIOB
#define BOARD_LED_RED3_GPIO_PORT PORTB
#define BOARD_LED_RED3_GPIO_PIN 10U
#define BOARD_LED_GREEN_GPIO GPIOB
#define BOARD_LED_GREEN_GPIO_PORT PORTB
#define BOARD_LED_GREEN_GPIO_PIN 11U

#define LED_RED1_INIT(output)                                  \
    GPIO_PinInit(BOARD_LED_RED1_GPIO, BOARD_LED_RED1_GPIO_PIN, \
                 &(gpio_pin_config_t){kGPIO_DigitalOutput, (output)}) /*!< Enable target LED_RED1 */
#define LED_RED1_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_RED1_GPIO, 1U << BOARD_LED_RED1_GPIO_PIN) /*!< Turn on target LED_RED1 */
#define LED_RED1_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_RED1_GPIO, 1U << BOARD_LED_RED1_GPIO_PIN) /*!< Turn off target LED_RED1 */
#define LED_RED1_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_RED1_GPIO, 1U << BOARD_LED_RED1_GPIO_PIN) /*!< Toggle on target LED_RED1 */

#define LED_RED2_INIT(output)                                  \
    GPIO_PinInit(BOARD_LED_RED2_GPIO, BOARD_LED_RED2_GPIO_PIN, \
                 &(gpio_pin_config_t){kGPIO_DigitalOutput, (output)}) /*!< Enable target LED_RED2 */
#define LED_RED2_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_RED2_GPIO, 1U << BOARD_LED_RED2_GPIO_PIN) /*!< Turn on target LED_RED2 */
#define LED_RED2_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_RED2_GPIO, 1U << BOARD_LED_RED2_GPIO_PIN) /*!< Turn off target LED_RED2 */
#define LED_RED2_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_RED2_GPIO, 1U << BOARD_LED_RED2_GPIO_PIN) /*!< Toggle on target LED_RED2 */

#define LED_RED3_INIT(output)                                  \
    GPIO_PinInit(BOARD_LED_RED3_GPIO, BOARD_LED_RED3_GPIO_PIN, \
                 &(gpio_pin_config_t){kGPIO_DigitalOutput, (output)}) /*!< Enable target LED_RED3 */
#define LED_RED3_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_RED3_GPIO, 1U << BOARD_LED_RED3_GPIO_PIN) /*!< Turn on target LED_RED3 */
#define LED_RED3_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_RED3_GPIO, 1U << BOARD_LED_RED3_GPIO_PIN) /*!< Turn off target LED_RED3 */
#define LED_RED3_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_RED3_GPIO, 1U << BOARD_LED_RED3_GPIO_PIN) /*!< Toggle on target LED_RED3 */

#define LED_GREEN_INIT(output)                                   \
    GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, \
                 &(gpio_pin_config_t){kGPIO_DigitalOutput, (output)}) /*!< Enable target LED_GREEN */
#define LED_GREEN_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn on target LED_GREEN */
#define LED_GREEN_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn off target LED_GREEN */
#define LED_GREEN_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Toggle on target LED_GREEN */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/

void BOARD_InitDebugConsole(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
