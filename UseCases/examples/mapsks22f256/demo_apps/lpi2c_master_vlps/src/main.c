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

#include "fsl_lpuart_hal.h"
#include "fsl_mcg_hal.h"
#include "fsl_lpi2c_hal.h"
#include "fsl_port_hal.h"
#include "fsl_edma_hal.h"
#include "fsl_dmamux_hal.h"
#include "fsl_lptmr_hal.h"

#define I2C_READ               1
#define I2C_WRITE              0
#define AT24C02_I2C_ADDR       0x50
#define AT24C02_ROM_SIZE       512 // bytes


#define LPI2C_CMD_TX         0
#define LPI2C_CMD_RX         1
#define LPI2C_CMD_STOP       2
#define LPI2C_CMD_START_SEND 4

// receive buffer would be 128 bytes, read all data by 4 times
static uint8_t recvData[AT24C02_ROM_SIZE >> 2];

static uint32_t sendDataCmd[] = 
{
    (LPI2C_CMD_START_SEND<<8) | (AT24C02_I2C_ADDR<<1) | I2C_READ,
    (LPI2C_CMD_RX<<8)         | (sizeof(recvData) - 1), // sequential read
    (LPI2C_CMD_STOP<<8)       | 0
};

static volatile bool gDmaDone = false;
static int counter = 0;

static void dbguart_init(void)
{
    // enable the debug uart port clock
    SIM_BWR_SCGC5_PORTE(SIM, 1);
    // uart pin mux
    PORT_HAL_SetMuxMode(PORTE, 4u, kPortMuxAlt3);
    PORT_HAL_SetMuxMode(PORTE, 5u, kPortMuxAlt3);

    // enable uart clock gating
    SIM_BWR_SCGC6_LPUART0(SIM, 1);
    // select the MCGIRCLK as LPUART0 source
    SIM_BWR_SOPT2_LPUARTSRC(SIM, 0x3);

    // init UART
    LPUART_HAL_Init(LPUART0);
    LPUART_HAL_SetBaudRate(LPUART0, 4000000, 115200);
    LPUART_HAL_SetTransmitterCmd(LPUART0, true);
    LPUART_HAL_SetReceiverCmd(LPUART0, true);
}

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

    // Init the LPI2C0 as Master
    // enable working in low power and debug mode
    LPI2C_HAL_MasterInit(LPI2C0);
    // set baud rate to 100Kbps, slow mode (4MHz clksrc)
    LPI2C_HAL_SetBaudRate(LPI2C0, 4000000, 100, kLpi2cMasterFastMode); 

    LPI2C_WR_MFCR(LPI2C0, 0);
    // enable DMA for Rx/Tx
    LPI2C_HAL_MasterSetTxDmaCmd(LPI2C0, true);
    LPI2C_HAL_MasterSetRxDmaCmd(LPI2C0, true);
    // enable master
    LPI2C_HAL_SetMasterCmd(LPI2C0, true);
}

// AT24C02 EEPROM data fillup and read current word address
static int32_t at24c02_filldata(void)
{
    uint32_t addr;
    uint8_t data = AT24C02_ROM_SIZE;
    uint16_t devAddr = AT24C02_I2C_ADDR;
    lpi2c_status_t ret;

    // fill the EEPROM with data of [addr]=ROM_SIZE-1-addr
    for (addr = 0; addr < AT24C02_ROM_SIZE; addr ++)
    {
        // as the second 256bytes word address is 9bit, so need
        // to append 0x1 to the device address
        if (addr == AT24C02_ROM_SIZE/2)
        {
            devAddr |= 0x1;
            data = 0;
        }
        else if (addr > AT24C02_ROM_SIZE/2)
        {
            data ++;
        }
        else
        {
            data --;
        }
        // Send data
        ret = LPI2C_HAL_MasterSendDataPolling(LPI2C0, devAddr,
                                              (uint8_t *)&addr, 1, &data, 1);
        if (ret != kStatus_LPI2C_Success)
        {
            printf("Failed to write data to 0x%x\r\n", addr);
            return (int32_t)ret;
        }

        // ACK polling for internal write cycle
        do
        {
            ret = LPI2C_HAL_MasterSendDataPolling(LPI2C0, devAddr,
                                                  NULL, 0, NULL, 0);
        } while (ret == kStatus_LPI2C_ReceivedNak);
    }

    // Use random read to set the internal read point to word address 0x0
    // then sequential read can start from 0x0 address
    addr = 0xFF;
    ret = LPI2C_HAL_MasterReceiveDataPolling(LPI2C0, AT24C02_I2C_ADDR | 0x1,
                              (uint8_t *)&addr, 1, &data, 1);
    if (ret != kStatus_LPI2C_Success)
    {
        printf("Cannot read the first address\r\n");
        return (int32_t)ret;
    }

    return 0;
}

static void dma_init(void)
{
    edma_transfer_config_t edmaCfg;
    edma_software_tcd_t stcd;

    // Enable the DMAMUX/EDMA IP module clock first
    SIM_BWR_SCGC6_DMAMUX(SIM, 0x1);
    SIM_BWR_SCGC7_DMA(SIM, 0x1);

    // Configure the DMAMUX
    DMAMUX_HAL_Init(DMAMUX);

    // set the DMAMUX to use LPI2C0 Tx as source to channel0
    DMAMUX_HAL_SetTriggerSource(DMAMUX, 0, 26);
    // enable the mux channel0
    DMAMUX_HAL_SetChannelCmd(DMAMUX, 0, true);
    // set the DMAMUX to use LPI2C0 Rx as source to channel1
    DMAMUX_HAL_SetTriggerSource(DMAMUX, 1, 18);
    // enable the mux channel1
    DMAMUX_HAL_SetChannelCmd(DMAMUX, 1, true);

    // reset the DMA0 to initial status
    EDMA_HAL_Init(DMA0);

    /**
     * Configure the DMA for LPI2C Tx
     *
     * Transfer 4Bytes from sendDataCmd buffer to MTDR register
     * in one minor loop, which means one DMA req per 4bytes.
     * Set the major loop count to the sendDataCmd array size.
     * Disable interrupt.
     *
     * source: sendDataCmd buffer (uint32_t type)
     * dest: MTDR register (32bit)
     * src offset: increase 4bytes to next sendDataCmd[] item after each transfer
     * dst offset: always write to MTDR REG, no address offset
     * minor bytes: one data command item, 4Bytes
     * major loops: number of data command items
     * adjustment: decrease sendDataCmd size for adjustment each major loop
     * modulo: disable modulo
     */

    memset(&stcd, 0, sizeof(stcd));
    // source and dest address, all 4Bytes(32bit) data width
    edmaCfg.srcAddr = (uint32_t)sendDataCmd;
    edmaCfg.srcTransferSize = kEDMATransferSize_4Bytes;
    edmaCfg.destAddr = (uint32_t)&LPI2C0_MTDR;
    edmaCfg.destTransferSize = kEDMATransferSize_4Bytes;
    // offset applied to current address to form next transfer address
    edmaCfg.srcOffset = 4;
    edmaCfg.destOffset = 0;
    edmaCfg.srcLastAddrAdjust = -sizeof(sendDataCmd);
    edmaCfg.destLastAddrAdjust = 0;
    edmaCfg.srcModulo = kEDMAModuloDisable;
    edmaCfg.destModulo = kEDMAModuloDisable;
    // 4bytes per DMA request
    edmaCfg.minorLoopCount = 4;
    edmaCfg.majorLoopCount = sizeof(sendDataCmd)/sizeof(uint32_t);
    // update TCD, disable interrupt on major loop done
    EDMA_HAL_STCDSetBasicTransfer(DMA0, &stcd, &edmaCfg, false, true);
    EDMA_HAL_PushSTCDToHTCD(DMA0, kEDMAChannel0, &stcd);
    // enable DMA in async mode
    EDMA_HAL_SetAsyncRequestInStopModeCmd(DMA0, kEDMAChannel0, true);

    /**
     * Configure the DMA for LPI2C Rx
     *
     * Transfer 1Bytes from MRDR register to recvData buffer
     * in one minor loop, which means one DMA req per 1bytes.
     * Set the major loop count to the recvData array size.
     * Enable interrupt.
     *
     * source: MRDR register (8bit)
     * dest: recvData buffer (uint8_t type)
     * src offset: always read from MRDR REG, no address offset
     * dst offset: increase 1bytes to next recvData[] item after each transfer
     * minor bytes: one data command item, 4Bytes
     * major loops: number of data command items
     * adjustment: decrease recvData size for adjustment each major loop
     * modulo: disable modulo
     */
    memset(&stcd, 0, sizeof(stcd));
    // source and dest address, all 1Bytes(8bit) data width
    edmaCfg.srcAddr = (uint32_t)&LPI2C0_MRDR;
    edmaCfg.srcTransferSize = kEDMATransferSize_1Bytes;
    edmaCfg.destAddr = (uint32_t)recvData;
    edmaCfg.destTransferSize = kEDMATransferSize_1Bytes;
    // offset applied to current address to form next transfer address
    edmaCfg.srcOffset = 0;
    edmaCfg.destOffset = 1;
    edmaCfg.srcLastAddrAdjust = 0;
    edmaCfg.destLastAddrAdjust = -sizeof(recvData);
    edmaCfg.srcModulo = kEDMAModuloDisable;
    edmaCfg.destModulo = kEDMAModuloDisable;
    // 1bytes per DMA request
    edmaCfg.minorLoopCount = 1;
    edmaCfg.majorLoopCount = sizeof(recvData);
    // update TCD, enable interrupt on major loop done
    EDMA_HAL_STCDSetBasicTransfer(DMA0, &stcd, &edmaCfg, true, true);
    EDMA_HAL_PushSTCDToHTCD(DMA0, kEDMAChannel1, &stcd);
    EDMA_HAL_SetAsyncRequestInStopModeCmd(DMA0, kEDMAChannel1, true);
    // enable IRQ in NVIC
    NVIC_EnableIRQ(DMA1_IRQn);

}

static void lptmr_init(void)
{
    lptmr_working_mode_user_config_t lptmr_mode_config;
    lptmr_prescaler_user_config_t prescaler_config;

    // enable LPTMR clock
    SIM_BWR_SCGC5_LPTMR(SIM, 0x1);

    // reset the LPTMR to initial status
    LPTMR_HAL_Init(LPTMR0);

    //Set Timer Working Mode
    lptmr_mode_config.freeRunningEnable = false;
    lptmr_mode_config.pinSelect = kLptmrPinSelectInput1;
    lptmr_mode_config.timerModeSelect = kLptmrTimerModeTimeCounter;
    LPTMR_HAL_SetTimerWorkingMode(LPTMR0, lptmr_mode_config);
    
    // Set Prescaler Mode
    prescaler_config.prescalerBypass = true;
    // LPO 1KHz clock source
    prescaler_config.prescalerClockSelect = kLptmrPrescalerClock1;
    LPTMR_HAL_SetPrescalerMode(LPTMR0, prescaler_config);

    //Set Compare Value, 1s
    LPTMR_HAL_SetCompareValue(LPTMR0, 1000);    
    
    //Enables the LPTMR interrupt
    NVIC_EnableIRQ(LPTMR0_IRQn);
    LPTMR_HAL_SetIntCmd(LPTMR0, true);
    
    //Enable LPTMR
    LPTMR_HAL_Enable(LPTMR0);
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
 * @brief example of LPI2C working in VLPS with async DMA
 *
 * The basic idea of this use case is demonstrate the
 * LPI2C working in VLPS mode without CPU interaction
 * to save power consumption. The working flow is defined as:
 * LPTMR (every 1s) -> EDMA (enable channel0 request) ->
 * LPI2C0 Tx (send sequential request for rom data) ->
 * LPI2C0 Rx (recv the data from AT240C2 EEPROM)
 *
 */
int main(void)
{
    uint32_t i;

    // init debug uart
    dbguart_init();

    printf("Reading data from EEPROM every 1s\r\n");

    // init LPI2C0 for master
    lpi2c_init();
    // Fill data into the EEPROM
    if (at24c02_filldata())
    {
        return -1;
    }
    // use LPTMR to start DMA for LPI2C Tx every 500ms
    lptmr_init();
    // init eDMA and DMAMUX for I2C Rx/Tx
    dma_init();

    while (1)
    {
        // enter VLPS
        while (!gDmaDone)
        {
            enter_vlps();
        }
        gDmaDone = false;

        for (i = 0; i < sizeof(recvData); i ++)
        {
            if (i % 16 == 0)
            {
                printf("\r\n");
            }
            // output data
            printf("%02x, ", recvData[i]);
        }
    }
}

/*!
* @Brief DMA1 channel interrupt handler
*/
void DMA1_IRQHandler(void)
{
    EDMA_HAL_ClearDoneStatusFlag(DMA0, kEDMAChannel1);
    EDMA_HAL_ClearIntStatusFlag(DMA0, kEDMAChannel1);
    gDmaDone = true;
}

/*!
* @Brief LPTMR0 IRQ handler to enable DMA channel0 request
*        which would start the LPI2C Tx
*/
void LPTMR0_IRQHandler(void)
{
    LPTMR_HAL_ClearIntFlag(LPTMR0);
    EDMA_HAL_SetDmaRequestCmd(DMA0, kEDMAChannel0, true);
    EDMA_HAL_SetDmaRequestCmd(DMA0, kEDMAChannel1, true);
}
