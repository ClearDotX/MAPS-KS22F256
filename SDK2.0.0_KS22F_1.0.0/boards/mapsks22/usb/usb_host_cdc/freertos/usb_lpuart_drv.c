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

#include "fsl_common.h"
#include "board.h"
#include "usb_uart_drv.h"
extern void LPUART_HandleIRQ(LPUART_Type *base, lpuart_handle_t *handle);
/*!
 * @brief Initializes an LPUART instance.
 *
 * This function initializes the LPUART module with user-defined settings.
 *
 *
 * @param base The LPUART base point.
 * @param config The user configuration structure of type uart_config_t. The user
 *  populates the members of this structure and passes the pointer of this structure
 *  to this function.
 * @param sourceClockHz LPUART clock source freqency in HZ
 * @return 0 succeed, others failed
 */

void USB_UartInit(USB_UartType *base, const usb_uartConfiguration *config, uint32_t sourceClockHz)
{
    LPUART_Init(base, config, sourceClockHz);
}

/*!
 * @brief Initializes LPUART configure sturcture.
 *
 * This function initializes the LPUART configure structure to default value.
 *
 * @param config Pointer to master config structure.
 * @see LPUART_Configure
 */
void USB_UartGetDefaultConfiguratoion(usb_uartConfiguration *configuration)
{
    LPUART_GetDefaultConfig(configuration);
}

/*!
 * @brief Receive data using IRQ
 *
 * This function receive data using IRQ, this is non-blocking function, will return
 * right away, when all data have been received, the receive callback function will be called.
 *
 * @param base The LPUART base point.
 * @param state uart state sturcture, refer to #usb_uart_handle_t.
 * @param xfer uart transfer sturcture, refer to #usb_uart_xfer_t.
 * @return kUART_Succeed succeed, others failed, refer to #usb_xfer_status_t.
 */
status_t USB_UartReceiveDataIRQ(USB_UartType *base, usb_uart_handle_t *handle, usb_xfer_t *xfer, size_t *receivedBytes)
{
    return LPUART_ReceiveNonBlocking(base, (lpuart_handle_t *)handle, (lpuart_transfer_t *)xfer, receivedBytes);
}

/*!
 * @brief send data using polling
 *
 * This function send data using polling, this is blocking function, will not return
 * until all data have been sent out.
 *
 * @param base The LPUART base point.
 * @param buffer buff for send data.
 * @param length length to send.
 */
void USB_UartSendDataPolling(USB_UartType *base, const uint8_t *buffer, uint32_t length)
{
    LPUART_WriteBlocking(base, buffer, length);
}

/*!
 * @brief Transmit a buffer of data using the interrupt method
 *
 * This function send data using interrupt method. This is non-blocking function,
 * returns directly without waiting for all data written to TX register. When
 * all data are written to TX register in ISR, UART driver calls the callback
 * function and pass @ref kStatus_UART_TxIdle as status parameter.
 *
 * @note The kStatus_UART_TxIdle is passed to upper layer when all data written
 * to TX register, but not ensure all the data sent out. So before disable TX,
 * check kUART_TransmissionCompleteFlag to ensure the TX is finished.
 *
 * @param base UART peripheral base address.
 * @param handle UART handle pointer.
 * @param xfer UART transfer sturcture, refer to #uart_transfer_t.
 * @retval kStatus_Success Sucessully start the data transmission.
 * @retval kStatus_UART_TxBusy Previous transmission still not finished, data not all written to TX register yet.
 */
status_t USB_UartSendNonBlocking(USB_UartType *base, usb_uart_handle_t *handle, usb_xfer_t *xfer)
{
    return LPUART_SendNonBlocking(base, handle, (lpuart_transfer_t *)xfer);
}

/*!
 * @brief Shuts down the LPUART by disabling interrupts and the transmitter/receiver.
 *
 * This function disables the UART interrupts, disables the transmitter and receiver, and
 * flushes the FIFOs (for modules that support FIFOs).
 *
 * @param instance The UART instance number.
 */
void USB_UartDeinit(USB_UartType *base)
{
    LPUART_Deinit(base);
}
void USB_UartCreateHandle(USB_UartType *base,
                          usb_uart_handle_t *handle,
                          usb_uart_rx_callback_t callback,
                          void *userData)
{
    LPUART_CreateHandle(base, (lpuart_handle_t *)handle, callback, userData);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_DRV_IRQHandler
 * Description   : Interrupt handler for UART.
 * This handler uses the buffers stored in the lpuart_state_t structs to transfer
 * data. This is not a public API as it is called whenever an interrupt occurs.
 *
 *END**************************************************************************/
void USB_UartIRQHandler(USB_UartType *base, usb_uart_handle_t *handle)
{
    LPUART_HandleIRQ(base, (lpuart_handle_t *)handle);
}
