/*
 * Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
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

#include <stdio.h>
#include <stdlib.h>

#include "fsl_device_registers.h"
#include "fsl_port_hal.h"
#include "fsl_gpio_hal.h"
#include "fsl_tpm_hal.h"
#include "fsl_lpi2c_hal.h"
#include "fsl_edma_hal.h"
#include "fsl_dmamux_hal.h"
#include "fsl_flexio_hal.h"
#include "fsl_flexio_i2s_hal.h"
#include "ili9341.h"
#include "pic.h"
#include "wm8960.h"
#include "music.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BIT_COUNT           32  //FlexIO I2S transfer 32 bit size per frame
#define FLEXIO_I2S_TX_BUFFER        FLEXIO_SHIFTBUFBIS0
#define TX_DMA_CHANNEL              kEDMAChannel0
#define TX_DMAMUX_SRC_No            38UL
#define TX_DMA_CHANNEL_IRQ_No       DMA0_IRQn
/*******************************************************************************
 * Global Variables
 ******************************************************************************/

static flexio_i2s_dev_t flexio_i2s_dev;
static flexio_i2s_master_config_t flexio_i2s_master_config;

static uint32_t TX_Length = MUSIC_LEN;
static uint8_t *Tx_Buffer = (uint8_t *)music;
static uint32_t TX_Finished_Bytes;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

static void tpm_init_output_compare(void)
{
    uint16_t uMod, uCnv;
    uint32_t cmpMode = 0;

    // enable clock gate porte
    SIM_BWR_SCGC5_PORTA(SIM, 0x1);
    // TPM1_CH1 pin mux
    PORT_HAL_SetMuxMode(PORTA, 1u, kPortMuxAlt6);//PTA1 ALT6 TPM1_CH1
  
    // configure TPM clock source:
    // MCGFLLCLK/MCGPLLCLK/IRC48M,selected by SOPT2[PLLFLLSEL]
    SIM_BWR_SOPT2_TPMSRC(SIM, 0x1);
    SIM_BWR_SOPT2_PLLFLLSEL(SIM, 0x3);//select IRC48Mhz
  
    //enable clock gating of tpm1
    SIM_BWR_SCGC6_TPM1(SIM, 0x1);
    TPM_HAL_SetCpwms(TPM1, 0);
    TPM_HAL_SetMod(TPM1, 1);

    cmpMode = (kTpmToggleOutput << TPM_CnSC_ELSA_SHIFT);
    TPM_HAL_SetChnMsnbaElsnbaVal(TPM1, 1, ((0x1 << TPM_CnSC_MSA_SHIFT) | cmpMode));
    TPM_HAL_SetChnCountVal(TPM1, 1, 1);
    TPM_HAL_SetClockMode(TPM1, kTpmClockSourceModuleClk);//TPM counter increments on every TPM counter clock
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
    LPI2C_HAL_SetBaudRate(LPI2C0, 48000000, 100, kLpi2cMasterStandardMode); 
  
    LPI2C_WR_MFCR(LPI2C0, 0);
    // enable master
    LPI2C_HAL_SetMasterCmd(LPI2C0, true);
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
  
    //32bit fixed, 11KHz sample rate
    flexio_i2s_master_config.flexioBusClk   = 48000000;
    flexio_i2s_master_config.bitClk         = 11025 * 16UL * 2UL;
    flexio_i2s_master_config.bitCount       = 16;
  
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
        }
    }
}

/*!
* @brief Configure DMA and DMAMUX for FlexIO I2S TX.
*/
void ConfigDMAforFlexIOI2STX(void)
{
    edma_software_tcd_t stcd;
    edma_transfer_config_t edmaconfig;
  
    // enable DMAMUX and DMA clock gate
    SIM_BWR_SCGC6_DMAMUX(SIM, 0x1);
    SIM_BWR_SCGC7_DMA(SIM, 0x1);
    // init DMAMUX and DMA
    EDMA_HAL_Init(DMA0);
    DMAMUX_HAL_Init(DMAMUX);

    // Configure the DMAMUX for edma channel
    DMAMUX_HAL_SetChannelCmd(DMAMUX, TX_DMA_CHANNEL, false);
    DMAMUX_HAL_SetTriggerSource(DMAMUX, TX_DMA_CHANNEL, TX_DMAMUX_SRC_No);
    DMAMUX_HAL_SetChannelCmd(DMAMUX, TX_DMA_CHANNEL, true);
  
    // Configure TCD
    EDMA_HAL_HTCDClearReg(DMA0, TX_DMA_CHANNEL);
  
    edmaconfig.srcAddr = (uint32_t)(Tx_Buffer);
    edmaconfig.destAddr = (uint32_t)(&FLEXIO_I2S_TX_BUFFER) + (4UL - BIT_COUNT/8UL);
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
    EDMA_HAL_STCDSetBasicTransfer(DMA0, &stcd, &edmaconfig, false, false);
    EDMA_HAL_PushSTCDToHTCD(DMA0, TX_DMA_CHANNEL, &stcd);
}

// main
int main(void)
{
    int row, col, i;
    unsigned short *p = (unsigned short*)Freescale_Logo;
    uint8_t leds[4] = {3, 9, 10, 11};
    
    ////////////////////////////////
    // LED part
    ////////////////////////////////
    SIM_BWR_SCGC5_PORTB(SIM, 1);

    for (i = 0; i < 4; i++)
    {
        PORT_HAL_SetMuxMode(PORTB, leds[i], kPortMuxAsGpio);
        GPIO_HAL_SetPinDir(PTB, leds[i], kGpioDigitalOutput);
        GPIO_HAL_SetPinOutput(PTB, leds[i]);
    }

    ////////////////////////////////
    // Color LCD part
    ////////////////////////////////

    SIM_BWR_SCGC5_PORTD(SIM, 1);
    // BLC
    PORT_HAL_SetMuxMode(PORTD, 1, kPortMuxAsGpio);
    GPIO_HAL_SetPinDir(PTD, 1, kGpioDigitalOutput); 
    // CS
    PORT_HAL_SetMuxMode(PORTD, 4, kPortMuxAsGpio);
    GPIO_HAL_SetPinDir(PTD, 4, kGpioDigitalOutput);  
    // CLK
    PORT_HAL_SetMuxMode(PORTD, 5, kPortMuxAsGpio);
    GPIO_HAL_SetPinDir(PTD, 5, kGpioDigitalOutput);  
    // MOSI
    PORT_HAL_SetMuxMode(PORTD, 6, kPortMuxAsGpio);
    GPIO_HAL_SetPinDir(PTD, 6, kGpioDigitalOutput);  

    // BLC back light
    GPIO_HAL_SetPinOutput(PTD, 1);
    
    // init LCD
    ili9341_init();
    // fill BMP - draw pixels
    for (row = 0; row < 240; row ++)
    {
        for (col = 0; col < 320; col ++)
        {
            ili9341_draw_pixel((int)*(p + row*320 + col), 239 - row, col);
        }
    }

    ////////////////////////////////
    // FlexIO I2S part
    ////////////////////////////////

    // enable TPM for 12M to simulate I2S MCLK
    tpm_init_output_compare();
  
    // init the I2C to access WM8960
    lpi2c_master_init();
  
    // init WM8960 for Tx
    WOLFSON_TxInit();
    WOLFSON_SetProtocol(kWolfsonBusI2S);

    // Initialize FlexIO module to emulate I2S master
    FlexIO_I2S_MasterInit();
  
    // Initialize DMA and DMAMUX module for FlexIO I2S
    ConfigDMAforFlexIOI2STX();
    // Enable FlexIO eDMA request
    FLEXIO_I2S_HAL_SetTxDmaCmd(&flexio_i2s_dev, true);
    // Enable FlexIO interrupt in NVIC
    NVIC_EnableIRQ(TX_DMA_CHANNEL_IRQ_No);
    // enable DMA request
    EDMA_HAL_SetDmaRequestCmd(DMA0, TX_DMA_CHANNEL, true);

    i = 0;
    while(1)
    {
        // LED switch
        GPIO_HAL_SetPinOutput(PTB, leds[i]);
        i = (++i) % 4;
        GPIO_HAL_ClearPinOutput(PTB, leds[i]);
        for (row = 0; row < 1000000; row ++)
        {
            asm("nop");
        }
    }

}
