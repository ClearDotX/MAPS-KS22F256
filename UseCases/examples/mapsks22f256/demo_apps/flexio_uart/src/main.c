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
#include <stdbool.h>
#include "fsl_flexio_hal.h"
#include "fsl_flexio_uart_hal.h"
#include "fsl_device_registers.h"
#include "fsl_port_hal.h"
#include "fsl_lpuart_hal.h"

uint32_t test;
/*******************************************************************************
* Definitions
*******************************************************************************/
#define CLKOUT_PTC3_ENABLE
//CLOCK_SETUP=5, normal run mode, MCGPLLCLK = 80Mhz, default
//CLOCK_SETUP=4, high speed run mode, MCGPLLCLK = 120Mhz
#define MCGPLLCLK  80000000   
#define IRC48Mhz  48000000
#define FASTIRC  4000000

#define DEBUG_LPUART_BAUDRATE  115200UL
#define FLEXIO_UART_BAUD_RATE       115200UL
#define FLEXIO_UART_DATA_BITS       8UL

/*******************************************************************************
* Prototypes
******************************************************************************/
void Debug_LPUART_Init(void);
void FlexIO_UART_Init(void);
/*******************************************************************************
* Variables
******************************************************************************/
flexio_uart_config_t flexio_uart_config;
flexio_uart_tx_dev_t flexio_uart_tx_dev;
flexio_uart_rx_dev_t flexio_uart_rx_dev;

/*!
* @breief Initialize LPUART for debug console
*/
void Debug_LPUART_Init(void)
{
  // enable clock gate porte
  SIM_BWR_SCGC5_PORTE(SIM, 0x1);
  // LPUART Tx/Rx pin mux
  PORT_HAL_SetMuxMode(PORTE, 4u, kPortMuxAlt3);//PTE4 LPUART_TX
  PORT_HAL_SetMuxMode(PORTE, 5u, kPortMuxAlt3);//PTE5 LPUART_RX
  
  //configure Debug LPUART clock source: MCGFLLCLK/MCGPLLCLK/IRC48M,selected by SOPT2[PLLFLLSEL].
  SIM_BWR_SOPT2_LPUARTSRC(SIM, 0x1);
  //SIM_BWR_SOPT2_PLLFLLSEL(SIM, 0x1);//select PLL
  SIM_BWR_SOPT2_PLLFLLSEL(SIM, 0x3);//select IRC48Mhz
  
  // enable LPUART clock gate
  SIM_BWR_SCGC6_LPUART0(SIM, 0x1);
  
  LPUART_HAL_Init(LPUART0);
  // MCGPLLCLK clock is 80MHz
  LPUART_HAL_SetBaudRate(LPUART0, IRC48Mhz, DEBUG_LPUART_BAUDRATE);
  LPUART_HAL_SetTransmitterCmd(LPUART0, true);
  LPUART_HAL_SetReceiverCmd(LPUART0, true);
  
  // enable RX interrupt
  LPUART_HAL_SetIntMode(LPUART0, kLpuartIntRxDataRegFull, true);
  NVIC_EnableIRQ(LPUART0_IRQn);
  
  printf("\r\nDebug console uart init ok!\r\n");
}


/*!
* @breief Initialize FlexIO module to emulate UART
*/
void FlexIO_UART_Init(void)
{
  // enable clock gate portc
  SIM_BWR_SCGC5_PORTC(SIM, 0x1);
  
  /* Configure FlexIO pin MUX */
  PORT_HAL_SetMuxMode(PORTC, 16UL, kPortMuxAlt7);//FXIO0_D4
  PORT_HAL_SetMuxMode(PORTC, 17UL, kPortMuxAlt7);//FXIO0_D5
  
  //configure FlexIO clock source: MCGFLLCLK/MCGPLLCLK/IRC48M,selected by SOPT2[PLLFLLSEL].
  SIM_BWR_SOPT2_FLEXIOSRC(SIM, 0x1);
  //SIM_BWR_SOPT2_PLLFLLSEL(SIM, 0x1);//select PLL
  SIM_BWR_SOPT2_PLLFLLSEL(SIM, 0x3);//select IRC48Mhz
  
  // enable FLEXIO clock gate
  SIM_BWR_SCGC5_FLEXIO(SIM, 0x1);
  
  /* Configure FlexIO module registers */
  FLEXIO_HAL_SetDebugModeCmd(FLEXIO, true);//enable in debug mode
  FLEXIO_HAL_SetFlexioEnableCmd(FLEXIO, true);//FlexIO module is enabled.
  
  flexio_uart_config.baudrate     = FLEXIO_UART_BAUD_RATE;
  flexio_uart_config.bitCount     = FLEXIO_UART_DATA_BITS;
  flexio_uart_config.flexioBusClk = IRC48Mhz;
  
  //UART bus can be supported by using two timers, two shifters and two pins. 
  flexio_uart_tx_dev.flexioBase   = FLEXIO;
  flexio_uart_tx_dev.shifterIdx   = 0UL;//shift0 for tx
  flexio_uart_tx_dev.timerIdx     = 0UL;//timer0 for tx
  
  flexio_uart_rx_dev.flexioBase   = FLEXIO;
  flexio_uart_rx_dev.shifterIdx   = 1UL;//shift1 for rx
  flexio_uart_rx_dev.timerIdx     = 1UL;//timer1 for rx 
  
  flexio_uart_tx_dev.txPinIdx     = 4UL;//FXIO0_D4
  flexio_uart_rx_dev.rxPinIdx     = 5UL;//FXIO0_D5
  
  FLEXIO_UART_Tx_HAL_Configure(&flexio_uart_tx_dev, &flexio_uart_config);//FlexIO uart tx
  FLEXIO_UART_Rx_HAL_Configure(&flexio_uart_rx_dev, &flexio_uart_config);//FlexIO uart rx
}


/*!
* @brief FLEXIO interrupt IRQ Handler
*/
void FLEXIO_IRQHandler(void)
{
  uint32_t ch_flexio;
  
  FLEXIO_UART_Rx_HAL_ClearRxBufferFullFlag(&flexio_uart_rx_dev);
  
  ch_flexio = FLEXIO_UART_Rx_HAL_GetData(&flexio_uart_rx_dev);
  
  FLEXIO_UART_Tx_HAL_PutData(&flexio_uart_tx_dev, ch_flexio);
  
}


void LPUART0_IRQHandler(void)
{
  uint8_t ch;
  LPUART_HAL_Getchar(LPUART0, &ch);
  LPUART_HAL_Putchar(LPUART0, ch);
}

void IRC48MHZ_init(void)
{
  SIM_BWR_SCGC4_USBOTG(SIM, 0x1);//enable USB clock gating
  USB_BWR_CLK_RECOVER_CTRL_CLOCK_RECOVER_EN(USB0, 1);//Crystal-less USB enable
  USB_BWR_CLK_RECOVER_IRC_EN_IRC_EN(USB0,1);//Enable the IRC48M module
}

void main (void)
{  
  
  IRC48MHZ_init();//IRC48Mhz init for FlexIO source
  
#ifdef CLKOUT_PTC3_ENABLE
  // enable clock gate
  SIM_BWR_SCGC5_PORTC(SIM, 0x1);
  PORT_HAL_SetMuxMode(PORTC, 3u, kPortMuxAlt5);//ptc3 as clkout
  //SIM_BWR_SOPT2_CLKOUTSEL(SIM, 0x2);//Output flash clock 
  SIM_BWR_SOPT2_CLKOUTSEL(SIM, 0x7);//Output IRC48Mhz clock 
#endif

  Debug_LPUART_Init();
  
  /* Initialize FlexIO module to emulate UART */
  FlexIO_UART_Init();
  
  /* Enable FlexIO tx/rx interrupt and NVIC */
  FLEXIO_UART_Rx_HAL_ClearRxBufferFullFlag(&flexio_uart_rx_dev);
  FLEXIO_UART_Tx_HAL_ClearTxBufferEmptyFlag(&flexio_uart_tx_dev);
  
  FLEXIO_UART_Rx_HAL_SetRxBufferFullIntCmd(&flexio_uart_rx_dev, true);
  NVIC_EnableIRQ(FLEXIO_IRQn);
  
  printf("\r\nDisconnet CN7/UART1 on MAPS-Dock, Switch to CN12/UART2 on MAPS-Dock!\r\n");
  printf("\r\nThen enter any key to start FlexIO Emulate UART test!\r\n");
  
  while(1);
  
}




