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
#include "fsl_flexio_i2s_hal.h"
#include "fsl_device_registers.h"
#include "fsl_port_hal.h"
#include "fsl_lpuart_hal.h"
#include "fsl_lpi2c_hal.h"
#include "fsl_sai_hal.h"
#include "fsl_tpm_hal.h"
#include "wm8960.h"
#include "music.h"
#include "fsl_dmamux_hal.h"
#include "fsl_edma_hal.h"

/*******************************************************************************
* Definitions
*******************************************************************************/
//#define CLKOUT_PTC3_ENABLE
#define CORECLK  80000000   //CLOCK_SETUP=5, normal run mode
#define LPUART_CLOCK  48000000   //clock source is 48Mhz
#define LPI2C_CLOCK     48000000   //clock source is 48Mhz
#define TPM_CLOCK     48000000   //clock source is 48Mhz
#define DEBUG_LPUART_BAUDRATE  115200UL

#define FLEXIO_BUSCLK       48000000
#define SAMPLE_RATE        11025   //FlexIO sample rate could not too high
#define BIT_COUNT           32  //FlexIO I2S transfer 32 bit size per frame

#define FLEXIO_I2S_TX_BUFFER        FLEXIO_SHIFTBUFBIS0
#define TX_DMA_CHANNEL              kEDMAChannel0
#define TX_DMAMUX_SRC_No            38UL
#define TX_DMA_CHANNEL_IRQ_No       DMA0_IRQn

//#define I2S_TEST
#define FLEXIO_TEST

/*******************************************************************************
* Prototypes
******************************************************************************/
void Debug_LPUART_Init(void);
void FlexIO_I2S_MasterInit(void);
static void lpi2c_master_init(void);
void i2s_init(void);
void tpm_init(void);

/*******************************************************************************
* Variables
******************************************************************************/
flexio_i2s_dev_t flexio_i2s_dev;
flexio_i2s_master_config_t flexio_i2s_master_config;

uint32_t TX_Length = MUSIC_LEN;
uint8_t * Tx_Buffer = (uint8_t *)music;

uint32_t TX_Finished_Bytes;
bool TX_Done;
//uint32_t MCLK = 12288000;// 384,256... I2S Master clock
uint32_t MCLK = 12000000;// 384,256... I2S Master clock


/*******************************************************************************
* Functions
******************************************************************************/
void delay(int cnt)
{
  int i, j;
  
  for(i=0;i<cnt;i++)
    for(j=0;j<1000;j++)
      asm("NOP");	
}

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
  
  //configure LPUART clock source: MCGFLLCLK/MCGPLLCLK/IRC48M,selected by SOPT2[PLLFLLSEL].
  SIM_BWR_SOPT2_LPUARTSRC(SIM, 0x1);
  SIM_BWR_SOPT2_PLLFLLSEL(SIM, 0x3);//select IRC48Mhz
  // enable LPUART clock gate
  SIM_BWR_SCGC6_LPUART0(SIM, 0x1);
  
  LPUART_HAL_Init(LPUART0);
  LPUART_HAL_SetBaudRate(LPUART0, LPUART_CLOCK, DEBUG_LPUART_BAUDRATE);
  LPUART_HAL_SetTransmitterCmd(LPUART0, true);
  LPUART_HAL_SetReceiverCmd(LPUART0, true);
  
  printf("\r\nDebug console uart init ok!\r\n");
}


/*!
* @breief Initialize FlexIO module to emulate I2S Master TX
*/
void FlexIO_I2S_MasterInit(void)
{
  // Configure FlexIO pin MUX for I2S master
  // enable clock gate portb
  SIM_BWR_SCGC5_PORTB(SIM, 0x1);
  
  /* Configure FlexIO pin MUX */
  PORT_HAL_SetMuxMode(PORTB, 20UL, kPortMuxAlt7);//FXIO0_D4, bclk
  PORT_HAL_SetMuxMode(PORTB, 21UL, kPortMuxAlt6);//FXIO0_D5, tx
  PORT_HAL_SetMuxMode(PORTB, 22UL, kPortMuxAlt6);//FXIO0_D6, fs
  
  //configure FlexIO clock source: MCGFLLCLK/MCGPLLCLK/IRC48M,selected by SOPT2[PLLFLLSEL]
  SIM_BWR_SOPT2_FLEXIOSRC(SIM, 0x1);
  SIM_BWR_SOPT2_PLLFLLSEL(SIM, 0x3);// select IRC48Mhz
  // enable FLEXIO clock gate
  SIM_BWR_SCGC5_FLEXIO(SIM, 0x1);
  
  /* Configure FlexIO module registers */
  FLEXIO_HAL_SetDebugModeCmd(FLEXIO, true);
  FLEXIO_HAL_SetFlexioEnableCmd(FLEXIO, true);
  
  flexio_i2s_dev.flexioBase       = FLEXIO;
  flexio_i2s_dev.txPinIdx         = 5UL; //FLEXIO_D5 ,tx
  flexio_i2s_dev.rxPinIdx         = 0UL; //not used
  flexio_i2s_dev.sckPinIdx        = 4UL;//FXIO0_D4, bclk
  flexio_i2s_dev.wsPinIdx         = 6UL;//FXIO0_D6, fs
  flexio_i2s_dev.shifterIdx[0]    = 0UL;
  flexio_i2s_dev.shifterIdx[1]    = 1UL;
  flexio_i2s_dev.timerIdx[0]      = 0UL;
  flexio_i2s_dev.timerIdx[1]      = 1UL;
  
  //32bit fixed
  flexio_i2s_master_config.flexioBusClk   = FLEXIO_BUSCLK;
  flexio_i2s_master_config.bitClk         = SAMPLE_RATE * 32UL * 2UL;
  flexio_i2s_master_config.bitCount       = 32;
  
  FLEXIO_I2S_HAL_Configure_Master(&flexio_i2s_dev, &flexio_i2s_master_config);
}


/*!
* @brief FLEXIO interrupt IRQ Handler
*/
void FLEXIO_IRQHandler(void)
{
  uint32_t bytes = BIT_COUNT/8UL;
  uint8_t *addr = Tx_Buffer + TX_Finished_Bytes;
  uint32_t data = 0;
  uint32_t i = 0;
  
  /* Clear the error flag. */
  if (FLEXIO_I2S_HAL_GetTxErrFlag(&flexio_i2s_dev))
  {
    FLEXIO_I2S_HAL_ClearTxErrFlag(&flexio_i2s_dev);
  }
  /* If need to transfer data using ISR. */
  if ((FLEXIO_I2S_HAL_GetTxBufferEmptyIntCmd(&flexio_i2s_dev)) &&
      (FLEXIO_I2S_HAL_GetTxBufferEmptyFlag(&flexio_i2s_dev)))
  {
    for (i = 0; i < bytes; i ++)
    {
      data |= (uint32_t)(*addr) << (i * 8U);
      addr ++;
      TX_Finished_Bytes ++;
    }
    data <<= (32UL - BIT_COUNT);
    FLEXIO_I2S_HAL_PutData(&flexio_i2s_dev, data);
    /* If need to callback */
    if (TX_Finished_Bytes >= TX_Length)
    {
      FLEXIO_I2S_HAL_SetTxBufferEmptyIntCmd(&flexio_i2s_dev, false);
      FLEXIO_I2S_HAL_SetTxErrIntCmd(&flexio_i2s_dev, false);
      TX_Done = true;
    }
  }
}

static void lpi2c_master_init(void)
{
  // enable PORTB clock gating
  SIM_BWR_SCGC5_PORTB(SIM, 1);
  
  // Init the pinmux for LPI2C0 SCL/SDA
  PORT_HAL_SetMuxMode(PORTB, 0u, kPortMuxAlt2); // LPI2C0_SDA
  PORT_HAL_SetMuxMode(PORTB, 1u, kPortMuxAlt2); // LPI2C0_SCL
  
  // configure LPUART clock source: MCGFLLCLK/MCGPLLCLK/IRC48M,selected by SOPT2[PLLFLLSEL]
  SIM_BWR_SOPT2_LPI2C0SRC(SIM, 0x1);
  SIM_BWR_SOPT2_PLLFLLSEL(SIM, 0x3);//select IRC48Mhz
  
  // enable LPI2C0 clock gating
  SIM_BWR_SCGC4_LPI2C0(SIM, 1);
  
  // Init the LPI2C0 as Master
  // enable working in low power and debug mode
  LPI2C_HAL_MasterInit(LPI2C0);
  // set baud rate to 100Kbps, Standard mode (48MHz clksrc)
  LPI2C_HAL_SetBaudRate(LPI2C0, LPI2C_CLOCK, 100, kLpi2cMasterStandardMode); 
  //LPI2C_HAL_SetBaudRate(LPI2C0, LPI2C_CLOCK, 400, kLpi2cMasterFastMode); 
  
  LPI2C_WR_MFCR(LPI2C0, 0);
  // enable master
  LPI2C_HAL_SetMasterCmd(LPI2C0, true);
}


void i2s_init(void)
{
  uint8_t  tx_bits = BIT_COUNT;
  uint32_t sample_rate = SAMPLE_RATE; 
  uint32_t mclk = MCLK; 
  uint32_t bclk = tx_bits * sample_rate * 2;
  // init clock gate
  SIM_BWR_SCGC5_PORTA(SIM, 1);
  // I2S0 Tx pinmux
  PORT_HAL_SetMuxMode(PORTA, 17u, kPortMuxAlt6); // MCLK
  PORT_HAL_SetMuxMode(PORTA, 5u, kPortMuxAlt6); // BCLK
  PORT_HAL_SetMuxMode(PORTA, 13u, kPortMuxAlt6); // TX_FS
  PORT_HAL_SetMuxMode(PORTA, 12u, kPortMuxAlt6); // TXD
  
  // enable clock for I2S0
  SIM_BWR_SCGC6_I2S0(SIM, 1);
  // configure I2S0 as Tx
  SAI_HAL_TxInit(I2S0);
  // Mclk source select
  SAI_HAL_SetMclkSrc(I2S0, kSaiMclkSourceSysclk);// 0 : Master clock from the system clock
  SAI_HAL_TxSetBclkSrc(I2S0, kSaiBclkSourceMclkDiv); // 01: Bit clock using master clock divider.
  SAI_HAL_TxSetSyncMode(I2S0, kSaiModeAsync);//Asynchronous mode
  SAI_HAL_TxSetMasterSlave(I2S0, kSaiMaster);
  //  SAI_HAL_TxSetProtocol(I2S0, kSaiBusI2SLeft);
  SAI_HAL_TxSetProtocol(I2S0, kSaiBusI2SType);
  SAI_HAL_TxSetDataChn(I2S0, 0);
  SAI_HAL_TxSetWatermark(I2S0, 0); 
  // Configure master clock
  SAI_HAL_SetMclkDiv(I2S0, mclk, CORECLK);//MCLK source clock is system clock
  // Master clock and bit clock setting
  SAI_HAL_TxSetBclkDiv(I2S0, mclk/bclk);
  
  //  SAI_HAL_TxSetWordWidth(I2S0, kSaiBusI2SLeft, tx_bits);
  SAI_HAL_TxSetWordWidth(I2S0, kSaiBusI2SType, tx_bits);
  I2S_BWR_TCR3_WDFL(I2S0,(0-1));
  SAI_HAL_TxSetMonoStereo(I2S0, kSaiStereo);
  SAI_HAL_TxEnable(I2S0);
  
}

#if 0
void tpm_init_pwm(void)
{
  uint16_t uMod, uCnv;
  
  // enable clock gate porte
  SIM_BWR_SCGC5_PORTA(SIM, 0x1);
  // TPM2_CH1 pin mux
  PORT_HAL_SetMuxMode(PORTA, 1u, kPortMuxAlt6);//PTA1 ALT6 TPM1_CH1
  
  //configure TPM clock source: MCGFLLCLK/MCGPLLCLK/IRC48M,selected by SOPT2[PLLFLLSEL]
  SIM_BWR_SOPT2_TPMSRC(SIM, 0x1);
  SIM_BWR_SOPT2_PLLFLLSEL(SIM, 0x3);//select IRC48Mhz
  
  //enable clock gating of tpm1
  SIM_BWR_SCGC6_TPM1(SIM, 0x1);
  
  tpm_pwm_param_t param = {
    .mode = kTpmEdgeAlignedPWM,
    .edgeMode = kTpmHighTrue,
    .uFrequencyHZ = 12000000,  
    .uDutyCyclePercent = 50    //duty cycle 50%
  };
  
  TPM_HAL_EnablePwmMode(TPM1, &param, 1); //TPM1_CH1
  
  switch(param.mode)
  {
  case kTpmEdgeAlignedPWM:
    TPM_HAL_SetCpwms(TPM1, 0);
    uMod = TPM_CLOCK / param.uFrequencyHZ - 1;
    uCnv = uMod * param.uDutyCyclePercent / 100;
    /* For 100% duty cycle */
    if(uCnv >= uMod)
    {
      uCnv = uMod + 1;
    }
    TPM_HAL_SetMod(TPM1, uMod);
    TPM_HAL_SetChnCountVal(TPM1, 1, uCnv);
    
    break;
  case kTpmCenterAlignedPWM:
    TPM_HAL_SetCpwms(TPM1, 1);
    uMod =  (TPM_CLOCK)  / (param.uFrequencyHZ * 2);
    uCnv = uMod * param.uDutyCyclePercent / 100;
    /* For 100% duty cycle */
    if(uCnv >= uMod)
    {
      uCnv = uMod + 1;
    }
    TPM_HAL_SetMod(TPM1, uMod);
    TPM_HAL_SetChnCountVal(TPM1, 1, uCnv);
    break;
  default:
    break;
  }
  
  TPM_HAL_SetClockMode(TPM1, kTpmClockSourceModuleClk);//TPM counter increments on every TPM counter clock
  TPM_HAL_EnableChnInt(TPM1, 1);//Enables the TPM peripheral timer channel 1 interrupt.
  NVIC_EnableIRQ(TPM1_IRQn);
}
#endif

void tpm_init_output_compare(void)
{
  uint16_t uMod, uCnv;
  uint32_t cmpMode = 0;
  // enable clock gate porte
  SIM_BWR_SCGC5_PORTA(SIM, 0x1);
  // TPM1_CH1 pin mux
  PORT_HAL_SetMuxMode(PORTA, 1u, kPortMuxAlt6);//PTA1 ALT6 TPM1_CH1
  
  //configure TPM clock source: MCGFLLCLK/MCGPLLCLK/IRC48M,selected by SOPT2[PLLFLLSEL]
  SIM_BWR_SOPT2_TPMSRC(SIM, 0x1);
  SIM_BWR_SOPT2_PLLFLLSEL(SIM, 0x3);//select IRC48Mhz
  
  //enable clock gating of tpm1
  SIM_BWR_SCGC6_TPM1(SIM, 0x1);
  TPM_HAL_SetCpwms(TPM1, 0);
  TPM_HAL_SetMod(TPM1, 1);
  cmpMode = (kTpmToggleOutput << TPM_CnSC_ELSA_SHIFT);
  TPM_HAL_SetChnMsnbaElsnbaVal(TPM1, 1,
                               ((0x1 << TPM_CnSC_MSA_SHIFT) | cmpMode));
  TPM_HAL_SetChnCountVal(TPM1, 1, 1);
  TPM_HAL_SetClockMode(TPM1, kTpmClockSourceModuleClk);//TPM counter increments on every TPM counter clock
}

#if 0
void TPM1_IRQHandler(void)
{
  TPM_HAL_ClearChnInt(TPM1, 1);
  TPM_HAL_SetChnCountVal(TPM1, 1, TPM_HAL_GetChnCountVal(TPM1, 1));
}
#endif

void IRC48MHZ_init(void)
{
  SIM_BWR_SCGC4_USBOTG(SIM, 0x1);//enable USB clock gating
  USB_BWR_CLK_RECOVER_CTRL_CLOCK_RECOVER_EN(USB0, 1);//Crystal-less USB enable
  USB_BWR_CLK_RECOVER_IRC_EN_IRC_EN(USB0,1);//Enable the IRC48M module
}

void I2S_SendDataBlocking(I2S_Type * base,uint32_t tx_channel,
                          uint8_t * txBuff, uint32_t size)
{
  uint32_t bytes = (BIT_COUNT)/8;
  uint32_t i =0, j = 0, data = 0;
  uint32_t tcsr;
  
  for (i = 0; i < (size/bytes); i++)
  {
    for (j = 0; j < bytes; j++)
    {
      data |= ((uint32_t)(*txBuff) << (j * 8u));
      txBuff++;
    }
    SAI_HAL_TxEnable(I2S0);
    tcsr = I2S_RD_TCSR(I2S0);//important
    while((tcsr & I2S_TCSR_FWF_MASK) != I2S_TCSR_FWF_MASK)
    {
      tcsr = I2S_RD_TCSR(I2S0);
    }
    I2S_WR_TDR(base, tx_channel, data);
    data = 0;
  }
}	

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
* @brief Configure DMA and DMAMUX for FlexIO I2S TX.
*/
void ConfigDMAforFlexIOI2STX(void)
{
  edma_software_tcd_t stcd;
  edma_transfer_config_t edmaconfig;
  
  // Configure the DMAMUX for edma channel
  DMAMUX_HAL_SetChannelCmd(DMAMUX, TX_DMA_CHANNEL, false);
  DMAMUX_HAL_SetTriggerSource(DMAMUX, TX_DMA_CHANNEL, TX_DMAMUX_SRC_No);
  DMAMUX_HAL_SetChannelCmd(DMAMUX, TX_DMA_CHANNEL, true);
  
  // Configure TCD
  EDMA_HAL_HTCDClearReg(DMA0, TX_DMA_CHANNEL);
  
  edmaconfig.srcAddr = (uint32_t)(Tx_Buffer);
  edmaconfig.destAddr = (uint32_t)(&FLEXIO_I2S_TX_BUFFER) + (4UL - BIT_COUNT/8UL);
  //    edmaconfig.srcTransferSize = kEDMATransferSize_2Bytes;
  //    edmaconfig.destTransferSize = kEDMATransferSize_2Bytes;
  edmaconfig.srcTransferSize = kEDMATransferSize_4Bytes;
  edmaconfig.destTransferSize = kEDMATransferSize_4Bytes;
  edmaconfig.srcOffset = BIT_COUNT/8UL;
  edmaconfig.destOffset = 0UL;
  edmaconfig.srcLastAddrAdjust = -(int32_t)TX_Length;
  edmaconfig.destLastAddrAdjust = 0UL;
  edmaconfig.srcModulo = kEDMAModuloDisable;
  edmaconfig.destModulo = kEDMAModuloDisable;
  edmaconfig.minorLoopCount = BIT_COUNT/8UL;
  edmaconfig.majorLoopCount = TX_Length/(BIT_COUNT/8UL);
  stcd.CSR = 0;
  EDMA_HAL_STCDSetBasicTransfer(DMA0, &stcd, &edmaconfig, true, true);
  EDMA_HAL_PushSTCDToHTCD(DMA0, TX_DMA_CHANNEL, &stcd);
}

void DMA0_IRQHandler(void)
{
  if(true == EDMA_HAL_GetIntStatusFlag(DMA0, TX_DMA_CHANNEL))
  {
    EDMA_HAL_ClearDoneStatusFlag(DMA0, TX_DMA_CHANNEL);
    EDMA_HAL_ClearIntStatusFlag(DMA0, TX_DMA_CHANNEL);
    TX_Done = true;
  }
}


void main (void)
{  
  IRC48MHZ_init();
  
#ifdef CLKOUT_PTC3_ENABLE
  // enable clock gate
  SIM_BWR_SCGC5_PORTC(SIM, 0x1);
  // LPUART Tx/Rx pin mux
  PORT_HAL_SetMuxMode(PORTC, 3u, kPortMuxAlt5);//ptc3 as clkout
  //SIM_BWR_SOPT2_CLKOUTSEL(SIM, 0x2);//Output flash clock 
  SIM_BWR_SOPT2_CLKOUTSEL(SIM, 0x7);//Output IRC48Mhz clock 
#endif
  
  Debug_LPUART_Init();
  
  //use TPM to generate 12Mhz clock to simulate I2S master clock  
  tpm_init_output_compare();
  
  // init the I2C to access WM8960
  lpi2c_master_init();
  
  // init WM8960 for Tx
  printf("\r\nCodec init start!\r\n");
  WOLFSON_TxInit();
  WOLFSON_SetProtocol(kWolfsonBusI2S);
  delay(1000);
  
#ifdef I2S_TEST   
  // init the i2s with pinmux and clock
  i2s_init();
#endif
  
#ifdef FLEXIO_TEST  
  /* Initialize FlexIO module to emulate I2S master */
  FlexIO_I2S_MasterInit();
  //  NVIC_EnableIRQ(FLEXIO_IRQn);
  
#if 1   //Use DMA mode
  /* Initialize DMA and DMAMUX module. */
  DMA_Init();
  ConfigDMAforFlexIOI2STX();
  /* Enable FlexIO eDMA request */
  FLEXIO_I2S_HAL_SetTxDmaCmd(&flexio_i2s_dev, true);
  /* Enable FlexIO interrupt in NVIC */
  NVIC_EnableIRQ(TX_DMA_CHANNEL_IRQ_No);
#endif  
  
#endif 
  
  //FlexIO test
#ifdef FLEXIO_TEST  
  while(1)
  {
#if 0  //interrupt mode
    TX_Done = false;
    TX_Length =  sizeof(music);
    Tx_Buffer = (uint8_t *)music;
    TX_Finished_Bytes = 0;
    FLEXIO_I2S_HAL_SetTxErrIntCmd(&flexio_i2s_dev, true);
    FLEXIO_I2S_HAL_SetTxBufferEmptyIntCmd(&flexio_i2s_dev, true);
    while(false == TX_Done);
    
#else  //DMA mode
    TX_Done = false;
    /* Enable TX DMA request to start the transfer */
    EDMA_HAL_SetDmaRequestCmd(DMA0, TX_DMA_CHANNEL, true);
    while(false == TX_Done);
#endif	
  }
#endif
  
  
  //I2S test
#ifdef I2S_TEST  
  while(1)
  {
    printf("\r\nPlaying sound!\r\n");
    I2S_SendDataBlocking(I2S0,0,(uint8_t *)music, sizeof(music));
  }
#endif
  
}




