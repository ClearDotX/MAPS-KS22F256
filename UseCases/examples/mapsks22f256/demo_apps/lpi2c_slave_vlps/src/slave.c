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

#include "fsl_lpi2c_hal.h"
#include "fsl_port_hal.h"
#include "fsl_edma_hal.h"
#include "fsl_dmamux_hal.h"

#include "config.h"

static uint8_t gRegBuffer[BUFFER_SIZE] = "****************";

static volatile bool gError = false;

static void lpi2c_init(void)
{
    // enable PORTB clock gating
    SIM_BWR_SCGC5_PORTB(SIM, 1);

    // Init the pinmux for LPI2C0 SCL/SDA
    PORT_HAL_SetMuxMode(PORTB, 0u, kPortMuxAlt2); // LPI2C0_SDA
    PORT_HAL_SetMuxMode(PORTB, 1u, kPortMuxAlt2); // LPI2C0_SCL

    // enable LPI2C0 clock gating
    SIM_BWR_SCGC4_LPI2C0(SIM, 1);
    // configure the LPI2C0 clock source as MCGIRCLK
    SIM_BWR_SOPT2_LPI2C0SRC(SIM, 0x3);

    // Init the LPI2C0 as Slave
    // enable working in low power and debug mode
    LPI2C_HAL_SlaveInit(LPI2C0);
    // set 7bit address
    LPI2C_HAL_SetAddress7bit(LPI2C0, SLAVE_ADDRESS);
    // only set the TX data flag in the slave-transmit condition
    LPI2C_HAL_SlaveSetTxConfig(LPI2C0, kLpi2cSlaveTDFAssertTransmit);
    // enable DMA for Rx/Tx
    LPI2C_HAL_SlaveSetTxDmaCmd(LPI2C0, true);
    LPI2C_HAL_SlaveSetRxDmaCmd(LPI2C0, true);

    // Set SCL stretch
    LPI2C_HAL_SlaveSetSclStretch(LPI2C0, 0x6, true); 
    // enable the necessary interrupts
    LPI2C_HAL_SlaveSetIntCmd(LPI2C0,
              kLpi2cSlaveBitErrInt | kLpi2cSlaveFifoErrInt,
              true);
    // enable slaver
    LPI2C_HAL_SetSlaveCmd(LPI2C0, true);

    // Enable LPI2C0 IRQ in NVIC
    NVIC_EnableIRQ(LPI2C0_IRQn);
}

static void dmamux_init(void)
{
    // enable the DMAMUX module clock
    SIM_BWR_SCGC6_DMAMUX(SIM, 1);

    // Configure the DMAMUX
    DMAMUX_HAL_Init(DMAMUX);

    // set the DMAMUX to use LPI2C Tx as source to channel0
    DMAMUX_HAL_SetTriggerSource(DMAMUX, 0, 26);
    // enable the mux channel0
    DMAMUX_HAL_SetChannelCmd(DMAMUX, 0, true);
    // set the DMAMUX to use LPI2C Rx as source to channel1
    DMAMUX_HAL_SetTriggerSource(DMAMUX, 1, 18);
    // enable the mux channel1
    DMAMUX_HAL_SetChannelCmd(DMAMUX, 1, true);

}

static void dma_init(void)
{
    edma_transfer_config_t edmaCfg;
    edma_software_tcd_t stcd;

    // Enable the EDMA IP module clock first
    SIM_BWR_SCGC7_DMA(SIM, 1);

    // reset the DMA0 to initial status
    EDMA_HAL_Init(DMA0);

    /**
     * Configure the DMA for LPI2C Tx
     *
     * Transfer 1Bytes from gRegBuffer buffer to STDR register
     * in one minor loop, which means one DMA req per 1bytes.
     * Set the major loop count to the gRegBuffer array size with
     * the modulo set to sizeof(gRegBuffer) for loop.
     * Disable interrupt.
     *
     * source: gRegBuffer buffer (uint8_t type)
     * dest: STDR register (8bit)
     * src offset: increase 1byte to next gRegBuffer[] item after each transfer
     * dst offset: always write to STDR REG, no address offset
     * minor bytes: one data command item, 1Bytes
     * major loops: MAX
     * adjustment: none
     * modulo: modulo of sizeof(gRegBuffer)
     */

    memset(&stcd, 0, sizeof(stcd));
    // source and dest address, all 1Bytes(8bit) data width
    edmaCfg.srcAddr = (uint32_t)&gRegBuffer[0];
    edmaCfg.srcTransferSize = kEDMATransferSize_1Bytes;
    edmaCfg.destAddr = LPI2C_HAL_SlaveGetTxDataRegAddr(LPI2C0);
    edmaCfg.destTransferSize = kEDMATransferSize_1Bytes;
    // offset applied to current address to form next transfer address
    edmaCfg.srcOffset = 1;
    edmaCfg.destOffset = 0;
    edmaCfg.srcLastAddrAdjust = -sizeof(gRegBuffer);
    edmaCfg.destLastAddrAdjust = 0;
    edmaCfg.srcModulo = kEDMAModuloDisable; // gRegBuffer size is 16bytes
    edmaCfg.destModulo = kEDMAModuloDisable;
    // 1bytes per DMA request
    edmaCfg.minorLoopCount = 1;
    edmaCfg.majorLoopCount = sizeof(gRegBuffer);
    // update TCD, disable interrupt on major loop done
    EDMA_HAL_STCDSetBasicTransfer(DMA0, &stcd, &edmaCfg, false, false);
    EDMA_HAL_PushSTCDToHTCD(DMA0, kEDMAChannel0, &stcd);
    // enable DMA in async mode
    EDMA_HAL_SetAsyncRequestInStopModeCmd(DMA0, kEDMAChannel0, true);
    // enable IRQ in NVIC
    //NVIC_EnableIRQ(DMA0_IRQn);

    /**
     * Configure the DMA for LPI2C Rx
     *
     * Transfer 1Bytes from SRDR register to gRegBuffer buffer
     * in one minor loop, which means one DMA req per 1bytes.
     * Set the major loop count to the MAX for loop.
     * Enable interrupt.
     *
     * source: SRDR register (8bit)
     * dest: gRegBuffer buffer (uint8_t type)
     * src offset: always read from SRDR REG, no address offset
     * dst offset: increase 1bytes to next gRegBuffer[] item after each transfer
     * minor bytes: one data command item, 1Bytes
     * major loops: MAX
     * adjustment: none
     * modulo: modulo as 16bytes
     */
    memset(&stcd, 0, sizeof(stcd));
    // source and dest address, all 1Bytes(8bit) data width
    edmaCfg.srcAddr = LPI2C_HAL_SlaveGetRxDataRegAddr(LPI2C0);
    edmaCfg.srcTransferSize = kEDMATransferSize_1Bytes;
    edmaCfg.destAddr = (uint32_t)&gRegBuffer[0];
    edmaCfg.destTransferSize = kEDMATransferSize_1Bytes;
    // offset applied to current address to form next transfer address
    edmaCfg.srcOffset = 0;
    edmaCfg.destOffset = 1;
    edmaCfg.srcLastAddrAdjust = 0;
    edmaCfg.destLastAddrAdjust = -sizeof(gRegBuffer);
    edmaCfg.srcModulo = kEDMAModuloDisable;
    edmaCfg.destModulo = kEDMAModuloDisable;
    // 1bytes per DMA request
    edmaCfg.minorLoopCount = 1;
    edmaCfg.majorLoopCount = sizeof(gRegBuffer);
    // update TCD, enable interrupt on major loop done
    EDMA_HAL_STCDSetBasicTransfer(DMA0, &stcd, &edmaCfg, false, false);
    EDMA_HAL_PushSTCDToHTCD(DMA0, kEDMAChannel1, &stcd);
    EDMA_HAL_SetAsyncRequestInStopModeCmd(DMA0, kEDMAChannel1, true);
    // enable IRQ in NVIC
    //NVIC_EnableIRQ(DMA1_IRQn);

    // enable the DMA request
    EDMA_HAL_SetDmaRequestCmd(DMA0, kEDMAChannel0, true);
    EDMA_HAL_SetDmaRequestCmd(DMA0, kEDMAChannel1, true);
}

static void enter_vlps(void)
{
    SMC_BWR_PMPROT_AVLP(SMC, 0x1);
    SMC_CLR_PMCTRL(SMC, 0x7);
    SMC_WR_PMCTRL_STOPM(SMC, 0x2);
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    asm("WFI");
}

/*!
 * @brief example of LPI2C slave working in VLPS with async DMA
 *
 * The basic idea of this use case is demonstrate the
 * LPI2C slave working in VLPS mode without CPU interaction
 * to save power consumption.
 *
 * The slave's data transmit and receive is done by DMA
 * automatically without CPU interaction. Main funciton
 * only need to check the I2C bus error.
 */
int main(void)
{
    // init LPI2C0 for master
    lpi2c_init();
    // init the DMAMUX routing for channels
    dmamux_init();
    // init eDMA and DMAMUX for I2C Rx/Tx
    dma_init();

    while (1)
    {
        // enter VLPS
        enter_vlps();

        if (gError == true)
        {
            // when STOP or error re-init the DMA
            dma_init();
            gError = false;
        }
    }
}

void LPI2C0_IRQHandler(void)
{
    // clear status
    LPI2C_WR_SSR(LPI2C0, LPI2C_SSR_BEF_MASK |
                         LPI2C_SSR_FEF_MASK);
    // stop the DMA
    gError = true;
}
