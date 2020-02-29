/*
 * Copyright (c) 2014, Freescale Semiconductor, Inc.
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

#include "fsl_device_registers.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! peripheral enable configurations
#define BL_ENABLE_PINMUX_UART0 (BL_CONFIG_SCUART)
#define BL_ENABLE_PINMUX_SPI0 (BL_CONFIG_DSPI)
#define BL_ENABLE_PINMUX_I2C0 (BL_CONFIG_I2C)

//! UART pinmux configurations
#define UART0_RX_PORT_BASE PORTB
#define UART0_RX_GPIO_BASE PTB
#define UART0_RX_GPIO_PIN_NUM 16               // PIN 16 in the PTB group
#define UART0_RX_FUNC_ALT_MODE kPORT_MuxAlt3   // ALT mode for UART0 RX
#define UART0_RX_GPIO_ALT_MODE kPORT_MuxAsGpio // ALT mdoe for GPIO functionality
#define UART0_RX_GPIO_IRQn PORTB_IRQn
#define UART0_RX_GPIO_IRQHandler PORTB_IRQHandler
#define UART0_TX_PORT_BASE PORTB
#define UART0_TX_GPIO_PIN_NUM 17             // PIN 17 in the PTB group
#define UART0_TX_FUNC_ALT_MODE kPORT_MuxAlt3 // ALT mode for UART0 TX

//! I2C pinmux configurations
#if FREEDOM
#define I2C0_SCL_PORT_BASE PORTB
#define I2C0_SCL_GPIO_PIN_NUM 2              // PTB2
#define I2C0_SCL_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for I2C0 SCL functionality
#define I2C0_SDA_PORT_BASE PORTB
#define I2C0_SDA_GPIO_PIN_NUM 1              // PTB1
#define I2C0_SDA_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for I2C0 SDA functionality
#else                                        // TOWER bootloader or flashloader
#define I2C0_SCL_PORT_BASE PORTD
#define I2C0_SCL_GPIO_PIN_NUM 2              // PIN 2 in the PTD group
#define I2C0_SCL_FUNC_ALT_MODE kPORT_MuxAlt7 // ALT mode for I2C0 SCL
#define I2C0_SDA_PORT_BASE PORTD
#define I2C0_SDA_GPIO_PIN_NUM 3              // PIN 3 in the PTD group
#define I2C0_SDA_FUNC_ALT_MODE kPORT_MuxAlt7 // ALT mode for I2C0 SDA
#endif                                       // FREEDOM

//! SPI pinmux configurations
#define SPI0_PCS_PORT_BASE PORTE
#define SPI0_PCS_GPIO_PIN_NUM 16             // PIN 16 in the PTE group
#define SPI0_PCS_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI0 PCS
#define SPI0_SCK_PORT_BASE PORTE
#define SPI0_SCK_GPIO_PIN_NUM 17             // PIN 17 in the PTE group
#define SPI0_SCK_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI0 SCK
#define SPI0_SOUT_PORT_BASE PORTE
#define SPI0_SOUT_GPIO_PIN_NUM 18             // PIN 18 in the PTE group
#define SPI0_SOUT_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI0 SOUT
#define SPI0_SIN_PORT_BASE PORTE
#define SPI0_SIN_GPIO_PIN_NUM 19             // PIN 19 in the PTE group
#define SPI0_SIN_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI0 SIN

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
