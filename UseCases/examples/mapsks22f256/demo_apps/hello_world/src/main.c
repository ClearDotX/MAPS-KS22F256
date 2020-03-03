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
 * THIS SOFTWARE IS PROVIDEDRRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTA BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <string.h>
#include "fsl_device_registers.h"
#include "fsl_port_hal.h"
#include "fsl_lpuart_hal.h"

int main (void)
{  
#ifdef USE_USBPIN_FOR_LPUART
    // enable clock gate
    SIM_BWR_SCGC4_USBOTG(SIM, 0x1);
    // select LPUART as USB PHY pin
    USB_BWR_USBCTRL_UARTSEL(USB0, 0x1);
    SIM_BWR_MISCCTL_UARTSELONUSB(SIM, 0x3);
#else
    // enable clock gate
    SIM_BWR_SCGC5_PORTE(SIM, 0x1);
    // LPUART Tx/Rx pin mux
    PORT_HAL_SetMuxMode(PORTE, 4u, kPortMuxAlt3);
    PORT_HAL_SetMuxMode(PORTE, 5u, kPortMuxAlt3);
#endif
    
    //configure UART clock source: MCGPLLCLK
    SIM_BWR_SOPT2_LPUARTSRC(SIM, 0x1);
    // enable clock gate
    SIM_BWR_SCGC6_LPUART0(SIM, 0x1);

    LPUART_HAL_Init(LPUART0);
    // MCGPLLCLK clock is 80MHz
    LPUART_HAL_SetBaudRate(LPUART0, 80*1000*1000, 115200);
    LPUART_HAL_SetTransmitterCmd(LPUART0, true);
    LPUART_HAL_SetReceiverCmd(LPUART0, true);
    
    // enable RX interrupt
    LPUART_HAL_SetIntMode(LPUART0, kLpuartIntRxDataRegFull, true);
    NVIC_EnableIRQ(LPUART0_IRQn);

    printf("\r\nHello world!\r\n");
    
    while(1)
    {
    }

}

void LPUART0_IRQHandler(void)
{
    uint8_t ch;
    LPUART_HAL_Getchar(LPUART0, &ch);
    LPUART_HAL_Putchar(LPUART0, ch);
}



