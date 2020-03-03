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
#ifndef __FSL_UART_RTOS_H__
#define __FSL_UART_RTOS_H__

#include "fsl_uart.h"
#include <os.h>

/*!
 * @addtogroup uart
 * @{
 */

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
struct rtos_uart_config
{
    UART_Type *base;
    uint32_t srcclk;
    uint32_t baudrate;
    uart_parity_mode_t parity;
    uart_stop_bit_count_t stopbits;
    uint8_t *buffer;
    uint32_t buffer_size;
};

typedef struct _uart_rtos_handle
{
    UART_Type *base;
    struct _uart_transfer tx_xfer;
    struct _uart_transfer rx_xfer;
    OS_SEM rx_sem;
    OS_SEM tx_sem;
#define RTOS_UART_COMPLETE 0x1
    OS_FLAG_GRP rx_event;
    OS_FLAG_GRP tx_event;
    void *t_state; /* transactional layer state */
} uart_rtos_handle_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name UART functional Operation
 * @{
 */

/*!
 * @brief Initializes an UART instance for operation.
 *
 * @param handle The pointer where to store the UART handle.
 * @param userConfig The user configuration structure of type uart_user_config_t. The user
 *  populates the members of this structure and passes the pointer of this structure
 *  to this function.
 * @param sourceClockHz UART clock source freqency in HZ
 * @return 0 succeed, others failed
 */
int UART_RTOS_Init(uart_rtos_handle_t *handle, uart_handle_t *t_handle, const struct rtos_uart_config *cfg);

/*!
 * @brief Deinitializes an UART instance for operation.
 *
 * This function deinitializes the UART module gates the UART clock, disable NVIC IRQ
 * set all register value to reset value.
 *
 * @param base The UART base point.
 */
int UART_RTOS_DeInit(uart_rtos_handle_t *handle);

/*!
 * @name UART transactional Operation
 * @{
 */

/*!
 * @brief send data using polling
 *
 * This function send data using polling, this is blocking function, will not
 * until all data have been sent out.
 *
 * @param base The UART base point.
 * @param buffer buff for send data.
 * @param length length to send.
 */
int UART_RTOS_Send(uart_rtos_handle_t *handle, const uint8_t *buffer, uint32_t length);

/*!
 * @brief receive data using polling
 *
 * This function receive data using polling, this is blocking function, will not
 * until all data have been received.
 *
 * @param base The UART base point.
 * @param buffer buff for received data.
 * @param length length to receive.
 */
int UART_RTOS_Receive(uart_rtos_handle_t *handle, uint8_t *buffer, uint32_t length, size_t *received);

/* @} */

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* __FSL_UART_RTOS_H__ */
