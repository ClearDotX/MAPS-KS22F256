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
#include "fsl_lpi2c_hal.h"
#include "fsl_sai_hal.h"
#include "fsl_dmamux_hal.h"
#include "fsl_edma_hal.h"
#include "wm8960.h"

static uint32_t gAudioBuffer[1024];

static void lpi2c_master_init(void)
{
    // enable PORTB clock gating
    SIM_BWR_SCGC5_PORTB(SIM, 1);

    // Init the pinmux for LPI2C0 SCL/SDA
    PORT_HAL_SetMuxMode(PORTB, 0u, kPortMuxAlt2); // LPI2C0_SDA
    PORT_HAL_SetMuxMode(PORTB, 1u, kPortMuxAlt2); // LPI2C0_SCL

    // enable LPI2C0 clock gating
    SIM_BWR_SCGC4_LPI2C0(SIM, 1);
    // configure the LPI2C0 clock source as MCGPLLCLK
    SIM_BWR_SOPT2_LPI2C0SRC(SIM, 0x1);

    // Init the LPI2C0 as Master
    // enable working in low power and debug mode
    LPI2C_HAL_MasterInit(LPI2C0);
    // set baud rate to 400Kbps, slow mode (80MHz clksrc)
    LPI2C_HAL_SetBaudRate(LPI2C0, 80000000, 400, kLpi2cMasterFastMode); 
    LPI2C_WR_MFCR(LPI2C0, 0);
    // enable master
    LPI2C_HAL_SetMasterCmd(LPI2C0, true);
}

void i2s_init(void)
{
    uint8_t rx_bits = 24, tx_bits = 16;
    uint32_t sample_rate = 8000; // 8KHz
    uint32_t bclk;
    uint32_t mclk = 256 * sample_rate * 6;

    // init pinmux
    SIM_BWR_SCGC5_PORTA(SIM, 1);
    SIM_BWR_SCGC5_PORTC(SIM, 1);
    SIM_BWR_SCGC5_PORTE(SIM, 1);
    // I2S0 Rx
    PORT_HAL_SetMuxMode(PORTA, 14u, kPortMuxAlt6); // RX BCLK
    PORT_HAL_SetMuxMode(PORTA, 15u, kPortMuxAlt6); // RXD0
    PORT_HAL_SetMuxMode(PORTA, 16u, kPortMuxAlt6); // RX FS
    PORT_HAL_SetMuxMode(PORTA, 17u, kPortMuxAlt6); // MCLK
    // I2S1 Tx
    PORT_HAL_SetMuxMode(PORTE, 24u, kPortMuxAlt4); // TX FS
    PORT_HAL_SetMuxMode(PORTE, 25u, kPortMuxAlt4); // TX BCLK
    PORT_HAL_SetMuxMode(PORTE, 26u, kPortMuxAlt4); // TXD0
    PORT_HAL_SetMuxMode(PORTC, 11u, kPortMuxAlt7); // MCLK

    // enable clock for I2S0/1
    SIM_BWR_SCGC6_I2S0(SIM, 1);
    SIM_BWR_SCGC6_I2S1(SIM, 1);

    // configure I2S0 as Rx
    SAI_HAL_RxInit(I2S0);
    // Mclk source select
    SAI_HAL_SetMclkSrc(I2S0, kSaiMclkSourceSysclk);
    SAI_HAL_RxSetBclkSrc(I2S0, kSaiBclkSourceMclkDiv);
    SAI_HAL_RxSetSyncMode(I2S0, kSaiModeAsync);
    SAI_HAL_RxSetMasterSlave(I2S0, kSaiMaster);
    SAI_HAL_RxSetProtocol(I2S0, kSaiBusI2SLeft);
    SAI_HAL_RxSetDataChn(I2S0, 0);
    SAI_HAL_RxSetWatermark(I2S0, FSL_FEATURE_SAI_FIFO_COUNT/2);
    bclk = rx_bits * sample_rate * 2;
    // Configure master clock: 12.288MHz from 80MHz system clock
    SAI_HAL_SetMclkDiv(I2S0, mclk, 80000000);
    // Master clock and bit clock setting
    SAI_HAL_RxSetBclkDiv(I2S0, mclk/bclk);
    SAI_HAL_RxSetWordWidth(I2S0, kSaiBusI2SLeft, rx_bits);
    // The channel number configuration
    SAI_HAL_RxSetMonoStereo(I2S0, kSaiStereo);

    // Enable DMA
    SAI_HAL_RxSetDmaCmd(I2S0, kSaiDmaReqFIFORequest, true);

    // configure I2S1 as Tx
    SAI_HAL_TxInit(I2S1);
    // Mclk source select
    SAI_HAL_SetMclkSrc(I2S1, kSaiMclkSourceSysclk);
    SAI_HAL_TxSetBclkSrc(I2S1, kSaiBclkSourceMclkDiv);
    SAI_HAL_TxSetSyncMode(I2S1, kSaiModeAsync);
    SAI_HAL_TxSetMasterSlave(I2S1, kSaiMaster);
    SAI_HAL_TxSetProtocol(I2S1, kSaiBusI2SLeft);
    SAI_HAL_TxSetDataChn(I2S1, 0);
    SAI_HAL_TxSetWatermark(I2S1, FSL_FEATURE_SAI_FIFO_COUNT/2);
    // Configure master clock
    SAI_HAL_SetMclkDiv(I2S1, mclk, 80000000);
    // Master clock and bit clock setting
    bclk = tx_bits * sample_rate * 2;
    SAI_HAL_TxSetBclkDiv(I2S1, mclk/bclk);
    SAI_HAL_TxSetWordWidth(I2S1, kSaiBusI2SLeft, tx_bits);
    // The channel number configuration
    SAI_HAL_TxSetMonoStereo(I2S1, kSaiStereo);

    // Enable dma
    SAI_HAL_TxSetDmaCmd(I2S1, kSaiDmaReqFIFORequest, true);
}

// setup DMA
void dma_init(void)
{
    edma_transfer_config_t edmaCfg;
    edma_software_tcd_t stcd;

    // enable the DMAMUX module clock
    SIM_BWR_SCGC6_DMAMUX(SIM, 1);

    // Configure the DMAMUX
    DMAMUX_HAL_Init(DMAMUX);

    // set the DMAMUX to use I2S0 Rx as source to channel0
    DMAMUX_HAL_SetTriggerSource(DMAMUX, 0, 12);
    // enable the mux channel0
    DMAMUX_HAL_SetChannelCmd(DMAMUX, 0, true);
    // set the DMAMUX to use I2S1 Tx as source to channel1
    DMAMUX_HAL_SetTriggerSource(DMAMUX, 1, 47);
    // enable the mux channel1
    DMAMUX_HAL_SetChannelCmd(DMAMUX, 1, true);

    // Enable the EDMA IP module clock first
    SIM_BWR_SCGC7_DMA(SIM, 1);

    // reset the DMA0 to initial status
    EDMA_HAL_Init(DMA0);
    EDMA_HAL_SetChannelArbitrationMode(DMA0, kEDMAChnArbitrationRoundrobin);
    EDMA_HAL_SetHaltOnErrorCmd(DMA0, true);

    /**
     * Configure the DMA for I2S0 Rx
     *
     * Transfer data in FIFO from RDR register to gAudioBuffer buffer
     * in one minor loop, which means one DMA req per 4bytes * watermark size.
     * Set the major loop count to the total buffer size / minor loop
     * Disable interrupt.
     *
     * source: RDR register (32bit)
     * dest: gAudioBuffer buffer (uint32_t type)
     * src offset: always read from RDR REG, no address offset
     * dst offset: increase 4bytes to next gAudioBuffer[] item after each transfer
     * minor bytes: one data command item, 4 * watermark bytes
     * major loops: entire buffer
     * adjustment: none
     * modulo: disable modulo
     */
    memset(&stcd, 0, sizeof(stcd));
    // source and dest address, all 4Bytes data width
    edmaCfg.srcAddr = SAI_HAL_RxGetFifoAddr(I2S0, 0);
    edmaCfg.srcTransferSize = kEDMATransferSize_4Bytes;
    edmaCfg.destAddr = (uint32_t)&gAudioBuffer[0];
    edmaCfg.destTransferSize = kEDMATransferSize_4Bytes;
    // offset applied to current address to form next transfer address
    edmaCfg.srcOffset = 0;
    edmaCfg.destOffset = 4;
    edmaCfg.srcLastAddrAdjust = 0;
    edmaCfg.destLastAddrAdjust = -sizeof(gAudioBuffer);
    edmaCfg.srcModulo = kEDMAModuloDisable;
    edmaCfg.destModulo = kEDMAModuloDisable;
    // 1bytes per DMA request
    edmaCfg.minorLoopCount = 4 * SAI_HAL_RxGetWatermark(I2S0);
    edmaCfg.majorLoopCount = sizeof(gAudioBuffer)/edmaCfg.minorLoopCount;
    // update TCD, enable interrupt on major loop done
    EDMA_HAL_STCDSetBasicTransfer(DMA0, &stcd, &edmaCfg, false, false);
    EDMA_HAL_PushSTCDToHTCD(DMA0, kEDMAChannel0, &stcd);
    EDMA_HAL_SetAsyncRequestInStopModeCmd(DMA0, kEDMAChannel0, true);
    // enable IRQ in NVIC
    //NVIC_EnableIRQ(DMA1_IRQn);

    /**
     * Configure the DMA for I2S1 Tx
     *
     * Transfer (FIFO size - watermark) data from gAudioBuffer buffer to TDR register
     * in one minor loop, which means one DMA req per 2bytes * (FIFO - watermark) size.
     * Set the major loop count to the gAudioBuffer array size with
     * Disable interrupt.
     *
     * source: gAudioBuffer buffer (uint32_t type)
     * dest: TDR register (32bit)
     * src offset: increase 4byte to next gAudioBuffer[] item after each transfer
     * dst offset: always write to TDR REG, no address offset
     * minor bytes: one data command item, 2Bytes x (FIFO - watermark)
     * major loops: entire buffer
     * adjustment: none
     * modulo: disabled
     */

    memset(&stcd, 0, sizeof(stcd));
    // source and dest address, all 4Bytes data width
    edmaCfg.srcAddr = (uint32_t)&gAudioBuffer[0] + 0x2;
    edmaCfg.srcTransferSize = kEDMATransferSize_2Bytes;
    edmaCfg.destAddr = SAI_HAL_TxGetFifoAddr(I2S1, 0);
    edmaCfg.destTransferSize = kEDMATransferSize_2Bytes;
    // offset applied to current address to form next transfer address
    edmaCfg.srcOffset = 4;
    edmaCfg.destOffset = 0;
    edmaCfg.srcLastAddrAdjust = -sizeof(gAudioBuffer);
    edmaCfg.destLastAddrAdjust = 0;
    edmaCfg.srcModulo = kEDMAModuloDisable;
    edmaCfg.destModulo = kEDMAModuloDisable;
    // 1bytes per DMA request
    edmaCfg.minorLoopCount =
          2 * (FSL_FEATURE_SAI_FIFO_COUNT - SAI_HAL_TxGetWatermark(I2S1));
    edmaCfg.majorLoopCount = sizeof(gAudioBuffer)/edmaCfg.minorLoopCount/2;
    // update TCD, disable interrupt on major loop done
    EDMA_HAL_STCDSetBasicTransfer(DMA0, &stcd, &edmaCfg, false, false);
    EDMA_HAL_PushSTCDToHTCD(DMA0, kEDMAChannel1, &stcd);
    // enable DMA in async mode
    EDMA_HAL_SetAsyncRequestInStopModeCmd(DMA0, kEDMAChannel1, true);
    // enable IRQ in NVIC
    //NVIC_EnableIRQ(DMA0_IRQn);

    // enable the DMA request
    EDMA_HAL_SetDmaRequestCmd(DMA0, kEDMAChannel0, true);
    EDMA_HAL_SetDmaRequestCmd(DMA0, kEDMAChannel1, true);

}

int main (void)
{
    // init the I2C to access WM8960
    lpi2c_master_init();
    // init the i2s with pinmux and clock
    i2s_init();
    // init the DMA for both I2S Tx and Rx
    dma_init();
    // init WM8960 for Rx
    WOLFSON_RxInit();

    // start Rx
    SAI_HAL_RxEnable(I2S0);
    // Start Tx
    SAI_HAL_TxEnable(I2S1);

    while (1)
    {
    }
}
