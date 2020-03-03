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
#include "fsl_flexio_spi_hal.h"
#include "fsl_device_registers.h"
#include "fsl_port_hal.h"
#include "fsl_lpuart_hal.h"
#include "fsl_dmamux_hal.h"
#include "fsl_edma_hal.h"

/*******************************************************************************
* Definitions
*******************************************************************************/
//#define CLKOUT_PTC3_ENABLE
//CLOCK_SETUP=5, normal run mode, CORECLK = 80Mhz, default
//CLOCK_SETUP=4, high speed run mode, CORECLK = 120Mhz
#define CORECLK  120000000  
#define IRC48Mhz  48000000
#define DEBUG_LPUART_BAUDRATE  115200UL

//HSRUN Max two flexio board test baudrate= 5.45Mhz
//NRUN Max two flexio board test baudrate= 5Mhz
#define FLEXIO_SPI_BAUD_RATE        12000000UL 


#define FLEXIO_SPI_BITS             8UL
#define FLEXIO_BUSCLK               CORECLK

//FlexIO SPI master only test
#define DATA_SIZE                   256UL
#define TRANSFER_SIZE               (DATA_SIZE+1)

//#define TRANSFER_SIZE            256

#define SPI_MASTER_FLEXIO_TX_BUFFER            FLEXIO_SHIFTBUFBBS0
#define SPI_MASTER_FLEXIO_RX_BUFFER            FLEXIO_SHIFTBUFBIS1
//#define SPI_SLAVE_FLEXIO_TX_BUFFER            FLEXIO_SHIFTBUFBBS2
//#define SPI_SLAVE_FLEXIO_RX_BUFFER            FLEXIO_SHIFTBUFBIS3
#define SPI_MASTER_TX_DMA_CHANNEL              kEDMAChannel0
#define SPI_MASTER_RX_DMA_CHANNEL              kEDMAChannel1
//#define SPI_SLAVE_TX_DMA_CHANNEL              kEDMAChannel2
//#define SPI_SLAVE_RX_DMA_CHANNEL              kEDMAChannel3
#define SPI_MASTER_TX_DMAMUX_SRC_No            38UL   //FXIO0 Shift 0
#define SPI_MASTER_RX_DMAMUX_SRC_No            39UL    //FXIO0 Shift 1
//#define SPI_SLAVE_TX_DMAMUX_SRC_No            43UL   //FXIO0 Shift 2
//#define SPI_SLAVE_RX_DMAMUX_SRC_No            44UL    //FXIO0 Shift 3
#define SPI_MASTER_TX_DMA_CHANNEL_IRQ_No       DMA0_IRQn  
#define SPI_MASTER_RX_DMA_CHANNEL_IRQ_No       DMA1_IRQn
//#define SPI_SLAVE_TX_DMA_CHANNEL_IRQ_No       DMA2_IRQn
//#define SPI_SLAVE_RX_DMA_CHANNEL_IRQ_No       DMA3_IRQn

//#define FXIO_SPI_INT_MODE  //enable INT mode ,otherwise DMA mode


/*******************************************************************************
* Prototypes
******************************************************************************/
void Debug_LPUART_Init(void);
void FlexIO_SPI_Master_Init(void);
//void FlexIO_SPI_Slave_Init(void);
void ConfigDMAfor_SPI_MASTER_TX(void);
void ConfigDMAfor_SPI_MASTER_RX(void);
//void ConfigDMAfor_SPI_SLAVE_TX(void);
//void ConfigDMAfor_SPI_SLAVE_RX(void);
void DMA_Init(void);

bool CompareArray(uint8_t *Array1, uint8_t *Array2, uint32_t Length);
void PrintArray(uint8_t *Array, uint32_t Length);
/*******************************************************************************
* Variables
******************************************************************************/
flexio_spi_dev_t flexio_spi_master_dev;
//flexio_spi_dev_t flexio_spi_slave_dev;
flexio_spi_master_config_t flexio_spi_master_config;
//flexio_spi_slave_config_t flexio_spi_slave_config;

uint8_t SPI_MASTER_Txbuffer[TRANSFER_SIZE];
uint8_t SPI_MASTER_Rxbuffer[TRANSFER_SIZE];
uint8_t SPI_MASTER_Rxbuffer_exp[TRANSFER_SIZE];

//uint8_t SPI_SLAVE_Txbuffer[TRANSFER_SIZE];
//uint8_t SPI_SLAVE_Rxbuffer[TRANSFER_SIZE];
//uint8_t SPI_SLAVE_Rxbuffer_exp[TRANSFER_SIZE];

bool Master_Done;
//bool Slave_Done;
uint32_t TX_Counter;
uint32_t RX_Counter;

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
  //  LPUART_HAL_SetIntMode(LPUART0, kLpuartIntRxDataRegFull, true);
  //  NVIC_EnableIRQ(LPUART0_IRQn);
  
  printf("\r\nDebug console uart init ok!\r\n");
}

/*!
* @breief Initialize FlexIO module to emulate SPI master
*/
void FlexIO_SPI_Master_Init(void)
{
  // Configure FlexIO pin MUX for SPI master
  // enable clock gate portc
#if 1   
  SIM_BWR_SCGC5_PORTC(SIM, 0x1);
  
  /* Configure FlexIO pin MUX */
  PORT_HAL_SetMuxMode(PORTC, 12UL, kPortMuxAlt7);//FXIO0_D0
  PORT_HAL_SetMuxMode(PORTC, 13UL, kPortMuxAlt7);//FXIO0_D1
  PORT_HAL_SetMuxMode(PORTC, 14UL, kPortMuxAlt7);//FXIO0_D2
  PORT_HAL_SetMuxMode(PORTC, 15UL, kPortMuxAlt7);//FXIO0_D3
#else
  
  SIM_BWR_SCGC5_PORTB(SIM, 0x1);
  SIM_BWR_SCGC5_PORTC(SIM, 0x1);
  PORT_HAL_SetMuxMode(PORTB, 17UL, kPortMuxAlt7);//FXIO0_D0
  PORT_HAL_SetMuxMode(PORTB, 18UL, kPortMuxAlt7);//FXIO0_D1
  PORT_HAL_SetMuxMode(PORTB, 19UL, kPortMuxAlt7);//FXIO0_D2
  PORT_HAL_SetMuxMode(PORTC, 0UL, kPortMuxAlt6);//FXIO0_D3
#endif
  
  //configure FlexIO clock source: MCGFLLCLK/MCGPLLCLK/IRC48M,selected by SOPT2[PLLFLLSEL].
  //SIM_BWR_SOPT2_FLEXIOSRC(SIM, 0x1);
  //SIM_BWR_SOPT2_PLLFLLSEL(SIM, 0x3);//select IRC48Mhz
  
  //configure FlexIO clock source: Coreclk.
  SIM_BWR_SOPT2_FLEXIOSRC(SIM, 0x0);//I2S0_MCLK or System clock, selected via SIM_MISCCTRL[FlEXIOS0]
  SIM_BWR_MISCCTL_FlexIOS0(SIM, 0x0);//select System clock  
  
  // enable FLEXIO clock gate
  SIM_BWR_SCGC5_FLEXIO(SIM, 0x1);
  /* Configure FlexIO module registers */
  flexio_spi_master_dev.flexioBase       = FLEXIO;
  flexio_spi_master_dev.txPinIdx         = 0UL;//FXIO0_D0
  flexio_spi_master_dev.rxPinIdx         = 1UL;
  flexio_spi_master_dev.sclkPinIdx       = 2UL;
  flexio_spi_master_dev.csnPinIdx        = 3UL;//FXIO0_D3
  flexio_spi_master_dev.shifterIdx[0]    = 0UL;
  flexio_spi_master_dev.shifterIdx[1]    = 1UL;
  flexio_spi_master_dev.timerIdx[0]      = 0UL;
  flexio_spi_master_dev.timerIdx[1]      = 1UL;
  
  flexio_spi_master_config.flexioBusClk   = FLEXIO_BUSCLK; //SPI baudrate
  flexio_spi_master_config.baudrate       = FLEXIO_SPI_BAUD_RATE;
  flexio_spi_master_config.bitCount       = FLEXIO_SPI_BITS;
  flexio_spi_master_config.cphaOneEnable  = false;
  
  FLEXIO_SPI_HAL_ConfigMaster(&flexio_spi_master_dev, &flexio_spi_master_config);
}


/*!
* @breief Initialize FlexIO module to emulate SPI slave
*/
#if 0
void FlexIO_SPI_Slave_Init(void)
{
  // Configure FlexIO pin MUX for SPI master
  // enable clock gate portc and portd
#if 0  
  SIM_BWR_SCGC5_PORTC(SIM, 0x1);
  SIM_BWR_SCGC5_PORTD(SIM, 0x1);
  /* Configure FlexIO pin MUX */
  PORT_HAL_SetMuxMode(PORTC, 16UL, kPortMuxAlt7);//FXIO0_D4
  PORT_HAL_SetMuxMode(PORTC, 17UL, kPortMuxAlt7);//FXIO0_D5
  PORT_HAL_SetMuxMode(PORTD, 0UL, kPortMuxAlt7);//FXIO0_D6
  PORT_HAL_SetMuxMode(PORTD, 1UL, kPortMuxAlt7);//FXIO0_D7
#else
  SIM_BWR_SCGC5_PORTB(SIM, 0x1);
  PORT_HAL_SetMuxMode(PORTB, 20UL, kPortMuxAlt7);//FXIO0_D4
  PORT_HAL_SetMuxMode(PORTB, 21UL, kPortMuxAlt6);//FXIO0_D5
  PORT_HAL_SetMuxMode(PORTB, 22UL, kPortMuxAlt6);//FXIO0_D6
  PORT_HAL_SetMuxMode(PORTB, 23UL, kPortMuxAlt6);//FXIO0_D7
#endif
  
  //configure FlexIO clock source: MCGFLLCLK/MCGPLLCLK/IRC48M,selected by SOPT2[PLLFLLSEL].
  SIM_BWR_SOPT2_FLEXIOSRC(SIM, 0x1);
  //SIM_BWR_SOPT2_PLLFLLSEL(SIM, 0x1);//select PLL
  SIM_BWR_SOPT2_PLLFLLSEL(SIM, 0x3);//select IRC48Mhz
  
  // enable FLEXIO clock gate
  SIM_BWR_SCGC5_FLEXIO(SIM, 0x1);
  /* Configure FlexIO module registers */
  flexio_spi_slave_dev.flexioBase       = FLEXIO;
  flexio_spi_slave_dev.txPinIdx         = 4UL;//FXIO0_D4
  flexio_spi_slave_dev.rxPinIdx         = 5UL;
  flexio_spi_slave_dev.sclkPinIdx       = 6UL;
  flexio_spi_slave_dev.csnPinIdx        = 7UL;//FXIO0_D7
  flexio_spi_slave_dev.shifterIdx[0]    = 2UL;
  flexio_spi_slave_dev.shifterIdx[1]    = 3UL;
  flexio_spi_slave_dev.timerIdx[0]      = 2UL;
  
  flexio_spi_slave_config.bitCount       = FLEXIO_SPI_BITS;
  flexio_spi_slave_config.cphaOneEnable  = false;
  
  FLEXIO_SPI_HAL_ConfigSlave(&flexio_spi_slave_dev, &flexio_spi_slave_config);
}
#endif

/*!
* @brief initialize DMA and DMAMUX.
*/
void DMA_Init(void)
{
  // enable DMAMUX and DMA clock gate
  SIM_BWR_SCGC6_DMAMUX(SIM, 0x1);
  SIM_BWR_SCGC7_DMA(SIM, 0x1);
  // init DMAMUX and DMA
  EDMA_HAL_Init(DMA0);
  DMAMUX_HAL_Init(DMAMUX);
}


/*!
* @brief Configure DMA and DMAMUX for FlexIO SPI Master TX.
*/
void ConfigDMAfor_SPI_MASTER_TX(void)
{
  edma_software_tcd_t stcd;
  edma_transfer_config_t edmaconfig;
  
  // Configure the DMAMUX for edma channel
  DMAMUX_HAL_SetChannelCmd(DMAMUX, SPI_MASTER_TX_DMA_CHANNEL, false);
  //set DMAMUXx_CHCFGn_SOURCE :FLEXIO shift 0 to DMAMUX channel 0 for SPI master TX	
  DMAMUX_HAL_SetTriggerSource(DMAMUX, SPI_MASTER_TX_DMA_CHANNEL, SPI_MASTER_TX_DMAMUX_SRC_No);
  DMAMUX_HAL_SetChannelCmd(DMAMUX, SPI_MASTER_TX_DMA_CHANNEL, true);//set DMAMUXx_CHCFGn_ENBL
  
  // Configure TCD
  EDMA_HAL_HTCDClearReg(DMA0, SPI_MASTER_TX_DMA_CHANNEL); //config channel 0 DMA0_TCD0 related registers
  
  edmaconfig.srcAddr = (uint32_t)(&SPI_MASTER_Txbuffer[1]);//txbuf
  edmaconfig.destAddr = (uint32_t)(&SPI_MASTER_FLEXIO_TX_BUFFER);//spi master tx flexio shift 0 :FLEXIO0_SHIFTBUFBBS0
  
  edmaconfig.srcTransferSize = kEDMATransferSize_1Bytes;//ATTR_SSIZE = 1 bytes = 8bits
  edmaconfig.destTransferSize = kEDMATransferSize_1Bytes;//ATTR_DSIZE = 1 bytes = 8bits
  edmaconfig.srcOffset = 1;//SOFF
  edmaconfig.srcLastAddrAdjust = -(TRANSFER_SIZE-1);	
  edmaconfig.minorLoopCount = 1;
  edmaconfig.destOffset = 0;//DOFF
  edmaconfig.destLastAddrAdjust = 0;//SLAST
  edmaconfig.srcModulo = kEDMAModuloDisable;//ATTR_SMOD
  edmaconfig.destModulo = kEDMAModuloDisable;//ATTR_DMOD
  edmaconfig.majorLoopCount = (TRANSFER_SIZE-1);
  
  stcd.CSR = 0;
  EDMA_HAL_STCDSetBasicTransfer(DMA0, &stcd, &edmaconfig, false, true);
  EDMA_HAL_PushSTCDToHTCD(DMA0, SPI_MASTER_TX_DMA_CHANNEL, &stcd);//DMAMUX channel 0 for SPI master TX
}


/*!
* @brief Configure DMA and DMAMUX for FlexIO SPI Master RX.
*/
void ConfigDMAfor_SPI_MASTER_RX(void)
{
  edma_software_tcd_t stcd;
  edma_transfer_config_t edmaconfig;
  
  // Configure the DMAMUX for edma channel
  DMAMUX_HAL_SetChannelCmd(DMAMUX, SPI_MASTER_RX_DMA_CHANNEL, false);
  //set DMAMUXx_CHCFGn_SOURCE :FLEXIO shift 1 to DMAMUX channel 1 for SPI master RX	
  DMAMUX_HAL_SetTriggerSource(DMAMUX, SPI_MASTER_RX_DMA_CHANNEL, SPI_MASTER_RX_DMAMUX_SRC_No);
  DMAMUX_HAL_SetChannelCmd(DMAMUX, SPI_MASTER_RX_DMA_CHANNEL, true);
  
  // Configure TCD
  EDMA_HAL_HTCDClearReg(DMA0, SPI_MASTER_RX_DMA_CHANNEL);//config channel 1 DMA0_TCD1 related registers
  
  edmaconfig.srcAddr = (uint32_t)(&SPI_MASTER_FLEXIO_RX_BUFFER);//spi master rx flexio shift 1:FLEXIO0_SHIFTBUFBIS1
  edmaconfig.destAddr = (uint32_t)(SPI_MASTER_Rxbuffer);//rxbuf
  edmaconfig.srcTransferSize = kEDMATransferSize_1Bytes;//ATTR_SSIZE = 1 bytes = 8bits
  edmaconfig.destTransferSize = kEDMATransferSize_1Bytes;//ATTR_DSIZE = 1 bytes = 8bits
  edmaconfig.destOffset = 1;
  edmaconfig.destLastAddrAdjust = -(int32_t)TRANSFER_SIZE;
  edmaconfig.minorLoopCount = 1;
  edmaconfig.srcOffset = 0;
  edmaconfig.srcLastAddrAdjust = 0;
  edmaconfig.srcModulo = kEDMAModuloDisable;
  edmaconfig.destModulo = kEDMAModuloDisable;
  edmaconfig.majorLoopCount = TRANSFER_SIZE;
  
  stcd.CSR = 0;
  EDMA_HAL_STCDSetBasicTransfer(DMA0, &stcd, &edmaconfig, true, true);
  EDMA_HAL_PushSTCDToHTCD(DMA0, SPI_MASTER_RX_DMA_CHANNEL, &stcd);//DMAMUX channel 1 for SPI master RX
  EDMA_HAL_HTCDSetChannelMinorLink(DMA0, SPI_MASTER_RX_DMA_CHANNEL, SPI_MASTER_TX_DMA_CHANNEL, true);//Link channel0 and channel1
}

#if 0
/*!
* @brief Configure DMA and DMAMUX for FlexIO SPI Slave TX.
*/
void ConfigDMAfor_SPI_SLAVE_TX(void)
{
  edma_software_tcd_t stcd;
  edma_transfer_config_t edmaconfig;
  
  // Configure the DMAMUX for edma channel
  DMAMUX_HAL_SetChannelCmd(DMAMUX, SPI_SLAVE_TX_DMA_CHANNEL, false);
  //set DMAMUXx_CHCFGn_SOURCE :FLEXIO shift 2 to DMAMUX channel 2	
  DMAMUX_HAL_SetTriggerSource(DMAMUX, SPI_SLAVE_TX_DMA_CHANNEL, SPI_SLAVE_TX_DMAMUX_SRC_No);
  DMAMUX_HAL_SetChannelCmd(DMAMUX, SPI_SLAVE_TX_DMA_CHANNEL, true);//set DMAMUXx_CHCFGn_ENBL 
  
  // Configure TCD
  EDMA_HAL_HTCDClearReg(DMA0, SPI_SLAVE_TX_DMA_CHANNEL); //config channel 2 DMA0_TCD2 related registers
  
  edmaconfig.srcAddr = (uint32_t)(&SPI_SLAVE_Txbuffer[1]);//txbuf
  edmaconfig.destAddr = (uint32_t)(&SPI_SLAVE_FLEXIO_TX_BUFFER);//spi slave tx flexio shift 2 :FLEXIO0_SHIFTBUFBBS2
  edmaconfig.srcTransferSize = kEDMATransferSize_1Bytes;//ATTR_SSIZE = 1 bytes = 8bits
  edmaconfig.destTransferSize = kEDMATransferSize_1Bytes;//ATTR_DSIZE = 1 bytes = 8bits
  edmaconfig.srcOffset = 1;//SOFF
  edmaconfig.srcLastAddrAdjust = -(int32_t)(TRANSFER_SIZE-1);//SLAST
  edmaconfig.minorLoopCount = 1;
  edmaconfig.destOffset = 0;//DOFF
  edmaconfig.destLastAddrAdjust = 0;//SLAST
  edmaconfig.srcModulo = kEDMAModuloDisable;//ATTR_SMOD
  edmaconfig.destModulo = kEDMAModuloDisable;//ATTR_DMOD
  edmaconfig.majorLoopCount = TRANSFER_SIZE-1;
  
  stcd.CSR = 0;
  EDMA_HAL_STCDSetBasicTransfer(DMA0, &stcd, &edmaconfig, false, true);
  EDMA_HAL_PushSTCDToHTCD(DMA0, SPI_SLAVE_TX_DMA_CHANNEL, &stcd);//DMAMUX channel 2 for SPI slave TX
}

/*!
* @brief Configure DMA and DMAMUX for FlexIO SPI Slave RX.
*/
void ConfigDMAfor_SPI_SLAVE_RX(void)
{
  edma_software_tcd_t stcd;
  edma_transfer_config_t edmaconfig;
  
  // Configure the DMAMUX for edma channel
  DMAMUX_HAL_SetChannelCmd(DMAMUX, SPI_SLAVE_RX_DMA_CHANNEL, false);
  //set DMAMUXx_CHCFGn_SOURCE :FLEXIO shift 3 to DMAMUX channel 3	
  DMAMUX_HAL_SetTriggerSource(DMAMUX, SPI_SLAVE_RX_DMA_CHANNEL, SPI_SLAVE_RX_DMAMUX_SRC_No);
  DMAMUX_HAL_SetChannelCmd(DMAMUX, SPI_SLAVE_RX_DMA_CHANNEL, true);
  
  // Configure TCD
  EDMA_HAL_HTCDClearReg(DMA0, SPI_SLAVE_RX_DMA_CHANNEL);//config channel 3 DMA0_TCD3 related registers
  
  edmaconfig.srcAddr = (uint32_t)(&SPI_SLAVE_FLEXIO_RX_BUFFER);//spi slave rx flexio shift 3:FLEXIO0_SHIFTBUFBIS3
  edmaconfig.destAddr = (uint32_t)(SPI_SLAVE_Rxbuffer);//rxbuf
  edmaconfig.srcTransferSize = kEDMATransferSize_1Bytes;//ATTR_SSIZE = 1 bytes = 8bits
  edmaconfig.destTransferSize = kEDMATransferSize_1Bytes;//ATTR_DSIZE = 1 bytes = 8bits
  edmaconfig.destOffset = 1;
  edmaconfig.destLastAddrAdjust = -(int32_t)TRANSFER_SIZE;
  edmaconfig.minorLoopCount = 1;
  edmaconfig.srcOffset = 0;
  edmaconfig.srcLastAddrAdjust = 0;
  edmaconfig.srcModulo = kEDMAModuloDisable;
  edmaconfig.destModulo = kEDMAModuloDisable;
  edmaconfig.majorLoopCount = TRANSFER_SIZE;
  stcd.CSR = 0;
  
  EDMA_HAL_STCDSetBasicTransfer(DMA0, &stcd, &edmaconfig, true, true);
  EDMA_HAL_PushSTCDToHTCD(DMA0, SPI_SLAVE_RX_DMA_CHANNEL, &stcd);//DMAMUX channel 3 for SPI slave RX
  EDMA_HAL_HTCDSetChannelMinorLink(DMA0, SPI_SLAVE_RX_DMA_CHANNEL, SPI_SLAVE_TX_DMA_CHANNEL, true);//Link channel2 and channel3
}
#endif

/*!
* @Brief DMA channel 1 spi master rx interrupt handler
*/
void DMA1_IRQHandler(void)
{
  /* FlexIO SPI Instance 1(master) RX IRQ handler */
  if(true == EDMA_HAL_GetIntStatusFlag(DMA0, SPI_MASTER_RX_DMA_CHANNEL))
  {
    EDMA_HAL_ClearDoneStatusFlag(DMA0, SPI_MASTER_TX_DMA_CHANNEL);
    EDMA_HAL_ClearDoneStatusFlag(DMA0, SPI_MASTER_RX_DMA_CHANNEL);
    EDMA_HAL_ClearIntStatusFlag(DMA0, SPI_MASTER_RX_DMA_CHANNEL);
    Master_Done = true;
  }
}

#if 0
/*!
* @Brief DMA channel 3 spi slave rx interrupt handler
*/
void DMA3_IRQHandler(void)
{
  /* FlexIO SPI Instance 2(slave) RX IRQ handler */
  if(true == EDMA_HAL_GetIntStatusFlag(DMA0, SPI_SLAVE_RX_DMA_CHANNEL))
  {
    EDMA_HAL_ClearDoneStatusFlag(DMA0, SPI_SLAVE_TX_DMA_CHANNEL);
    EDMA_HAL_ClearDoneStatusFlag(DMA0, SPI_SLAVE_RX_DMA_CHANNEL);
    EDMA_HAL_ClearIntStatusFlag(DMA0, SPI_SLAVE_RX_DMA_CHANNEL);
    Slave_Done = true;
  }
}
#endif

/*!
* @brief compare two uint8_t data arrays with given length
*/
bool CompareArray(uint8_t *Array1, uint8_t *Array2, uint32_t Length)
{
  bool ReValue = true;
  
  for(uint32_t i=0; i < Length; i++)
  {
    if(Array1[i] != Array2[i])
    {
      ReValue = false;
      break;
    }
  }
  return ReValue;
}


/*!
* @brief compare two uint8_t data arrays with given length
*/
void PrintArray(uint8_t *Array, uint32_t Length)
{
  for(uint32_t i=0; i < Length; i++)
  {
    printf("0x%02x ", Array[i]);
    if(((i+1) & 0x7UL) == 0)
      printf("\n\r");
  }
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
  IRC48MHZ_init();
#ifdef CLKOUT_PTC3_ENABLE
  // enable clock gate
  SIM_BWR_SCGC5_PORTC(SIM, 0x1);
  // LPUART Tx/Rx pin mux
  PORT_HAL_SetMuxMode(PORTC, 3u, kPortMuxAlt5);//ptc3 as clkout
  
  SIM_BWR_SOPT2_CLKOUTSEL(SIM, 0x2);//Output flash clock 20Mhz cause normal run mode core clock=80Mhz
  
#endif
  Debug_LPUART_Init();
  
  /* Initialize FlexIO module. */
  FlexIO_SPI_Master_Init();
  FLEXIO_HAL_SetDebugModeCmd(FLEXIO, true);
  FLEXIO_HAL_SetFlexioEnableCmd(FLEXIO, true);
  
  /* Initialize DMA and DMAMUX module. */
  DMA_Init();
  ConfigDMAfor_SPI_MASTER_TX();
  ConfigDMAfor_SPI_MASTER_RX();
  
  /* Initialize SPI_MASTER_Txbuffer  */
  for(uint32_t i=0UL; i<TRANSFER_SIZE; i++)
  {
    SPI_MASTER_Txbuffer[i] = i;            /* Initialize SPI_MASTER_Txbuffer buffer: SPI_MASTER_Txbuffer[i] = i */
    SPI_MASTER_Rxbuffer_exp[i] = i;           
  }
  
  //Shifter Status DMA Enable
  FLEXIO_SPI_HAL_SetTxDmaCmd(&flexio_spi_master_dev, true);//FXIO SPI Master TX DMA enable
  FLEXIO_SPI_HAL_SetRxDmaCmd(&flexio_spi_master_dev, true);//FXIO SPI Master RX DMA enable
  
  NVIC_EnableIRQ(SPI_MASTER_RX_DMA_CHANNEL_IRQ_No);//enable DMA0 channel 1 interrupt, master rx dma
  
  while(1)
  {
    
    Master_Done = false;
    
    printf("\n\rStart the slave code in advance, then press any key to start this master code.\n\r");
    getchar();
    printf("Start to send...\n\r");
    printf("\n\rData send:\n\r");
    
    PrintArray(SPI_MASTER_Txbuffer, TRANSFER_SIZE);
    
    /* Enable RX DMA request */
    EDMA_HAL_SetDmaRequestCmd(DMA0, SPI_MASTER_RX_DMA_CHANNEL, true);//Enable dma channel 1: spi master rx request for channel 1
    
    /* Master transmit the first byte, to start the transfers */
    FLEXIO_SPI_HAL_PutDataMSB(&flexio_spi_master_dev, SPI_MASTER_Txbuffer[0]);
    
    while(false == Master_Done);
    
    printf("End of the transfer.\n\r");
    
    printf("\n\rData received:\n\r");
    //   PrintArray(SPI_MASTER_Rxbuffer, TRANSFER_SIZE);
    PrintArray(SPI_MASTER_Rxbuffer+1, DATA_SIZE); //test
    
#if 1    
    if(false == CompareArray(SPI_MASTER_Rxbuffer_exp, SPI_MASTER_Rxbuffer+1, DATA_SIZE))
      printf("\n\rDMA Mode Receive failed.\n\r");
    else
      printf("\n\rMDA Mode Receive succeeded.\n\r");
#endif	
    
  }
}




