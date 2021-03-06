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

#include "fsl_debug_console.h"
#include "fsl_dspi.h"
#include "fsl_flexio_spi_edma.h"
#include "fsl_dma_manager.h"
#include "board.h"

#include "fsl_device_registers.h"
#include "fsl_common.h"
#include "pin_mux.h"
#include <stdbool.h>
#include "clock_config.h"
#include "stdbool.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_FLEXIO_BASE       FLEXIO
#define FLEXIO_SPI_SOUT_PIN     0U
#define FLEXIO_SPI_SIN_PIN      1U
#define FLEXIO_SPI_CLK_PIN      2U
#define FLEXIO_SPI_PCS_PIN      3U
#define FLEXIO_CLOCK_FREQUENCY  CLOCK_GetFreq(kCLOCK_PllFllSelClk)
#define FLEXIO_DMA_REQUEST_BASE  kDmaRequestMux0FlexIOChannel0

#define DSPI_MASTER_CLK_SRC DSPI0_CLK_SRC
#define EXAMPLE_DSPI_MASTER_PCS_FOR_INIT kDSPI_Pcs0
#define EXAMPLE_DSPI_MASTER_PCS_FOR_TRANSFER kDSPI_MasterPcs0
#define EXAMPLE_DSPI_MASTER_BASEADDR SPI1
#define SLAVE_SPI_IRQ FLEXIO_IRQn
#define MASTER_SPI_IRQ SPI1_IRQn
#define TRANSFER_SIZE 256U        /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 500000U  /*! Transfer baudrate - 500k */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* DSPI user callback */
void DSPI_MasterUserCallback(SPI_Type *base, dspi_master_handle_t *handle, status_t status, void *userData);
void FLEXIO_SPI_SlaveUserCallback(FLEXIO_SPI_Type *base, flexio_spi_slave_edma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t masterRxData[TRANSFER_SIZE] = {0U};
uint8_t masterTxData[TRANSFER_SIZE] = {0U};
uint8_t slaveRxData[TRANSFER_SIZE] = {0U};
uint8_t slaveTxData[TRANSFER_SIZE] = {0U};

dspi_master_handle_t g_m_handle;
FLEXIO_SPI_Type spiDev;
flexio_spi_slave_edma_handle_t g_s_handle;

edma_handle_t txHandle;
edma_handle_t rxHandle;

volatile bool isTransferCompleted = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    CLOCK_SetFlexio0Clock(1);

    PRINTF("DSPI Master interrupt - FLEXIO SPI Slave edma example start.\r\n");
    PRINTF("This example use one dspi instance as master and one flexio spi as slave on one board.\r\n");
    PRINTF("Master uses interrupt and slave uses edma way.\r\n");
    PRINTF("Please make sure you make the correct line connection. Basically, the connection is: \r\n");
    PRINTF("DSPI_master -- FLEXIO_SPI_slave   \r\n");
    PRINTF("   CLK      --    CLK  \r\n");
    PRINTF("   PCS      --    PCS  \r\n");
    PRINTF("   SOUT     --    SIN  \r\n");
    PRINTF("   SIN      --    SOUT \r\n");

    uint32_t srcClock_Hz;
    uint32_t errorCount;
    uint32_t i;
    dspi_master_config_t masterConfig;
    flexio_spi_slave_config_t slaveConfig;
    dspi_transfer_t masterXfer;
    flexio_spi_transfer_t slaveXfer;
    dma_request_source_t dma_request_source_tx;
    dma_request_source_t dma_request_source_rx;

    /* Master config */
    masterConfig.whichCtar = kDSPI_Ctar0;
    masterConfig.ctarConfig.baudRate = TRANSFER_BAUDRATE;
    masterConfig.ctarConfig.bitsPerFrame = 8;
    masterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;
    masterConfig.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
    masterConfig.ctarConfig.direction = kDSPI_MsbFirst;
    masterConfig.ctarConfig.pcsToSckDelayInNanoSec = 1000000000U / TRANSFER_BAUDRATE;
    masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec = 1000000000U / TRANSFER_BAUDRATE;
    masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 1000000000U / TRANSFER_BAUDRATE;

    masterConfig.whichPcs = EXAMPLE_DSPI_MASTER_PCS_FOR_INIT;
    masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;

    masterConfig.enableContinuousSCK = false;
    masterConfig.enableRxFifoOverWrite = false;
    masterConfig.enableModifiedTimingFormat = false;
    masterConfig.samplePoint = kDSPI_SckToSin0Clock;

    srcClock_Hz = CLOCK_GetFreq(DSPI_MASTER_CLK_SRC);
    DSPI_MasterInit(EXAMPLE_DSPI_MASTER_BASEADDR, &masterConfig, srcClock_Hz);

    /* Slave config */
    FLEXIO_SPI_SlaveGetDefaultConfig(&slaveConfig);

    spiDev.flexioBase = BOARD_FLEXIO_BASE;
    spiDev.SDOPinIndex = FLEXIO_SPI_SOUT_PIN;
    spiDev.SDIPinIndex = FLEXIO_SPI_SIN_PIN;
    spiDev.SCKPinIndex = FLEXIO_SPI_CLK_PIN;
    spiDev.CSnPinIndex = FLEXIO_SPI_PCS_PIN;
    spiDev.shifterIndex[0] = 0U;
    spiDev.shifterIndex[1] = 1U;
    spiDev.timerIndex[0] = 0U;
    FLEXIO_SPI_SlaveInit(&spiDev, &slaveConfig);

    /* Set dspi slave interrupt priority higher. */
    NVIC_SetPriority(MASTER_SPI_IRQ, 1U);
    NVIC_SetPriority(SLAVE_SPI_IRQ, 0U);

    /* Set up the transfer data */
    for (i = 0U; i < TRANSFER_SIZE; i++)
    {
        masterTxData[i] = i % 256U;
        masterRxData[i] = 0U;

        slaveTxData[i] = ~masterTxData[i];
        slaveRxData[i] = 0U;
    }

    isTransferCompleted = false;

    /*Init DMA for example.*/
    DMAMGR_Init();

    dma_request_source_tx = (dma_request_source_t)(FLEXIO_DMA_REQUEST_BASE + spiDev.shifterIndex[0]);
    dma_request_source_rx = (dma_request_source_t)(FLEXIO_DMA_REQUEST_BASE + spiDev.shifterIndex[1]);

    /* Request DMA channels for TX & RX. */
    if (dma_request_source_tx < 0x200)
    {
        DMAMGR_RequestChannel((dma_request_source_t)dma_request_source_tx, 0, &txHandle);
        DMAMGR_RequestChannel((dma_request_source_t)dma_request_source_rx, 1, &rxHandle);
    }
    else
    {
        DMAMGR_RequestChannel((dma_request_source_t)dma_request_source_tx, 16, &txHandle);
        DMAMGR_RequestChannel((dma_request_source_t)dma_request_source_rx, 17, &rxHandle);
    }

    /* Set up slave first */
    FLEXIO_SPI_SlaveCreateHandleEDMA(&spiDev, &g_s_handle, FLEXIO_SPI_SlaveUserCallback, NULL, &txHandle, &rxHandle);

    /*Set slave transfer ready to receive/send data*/
    slaveXfer.txData = slaveTxData;
    slaveXfer.rxData = slaveRxData;
    slaveXfer.dataSize = TRANSFER_SIZE;
    slaveXfer.flags = kFLEXIO_SPI_8bitMsb;

    FLEXIO_SPI_SlaveTransferEDMA(&spiDev, &g_s_handle, &slaveXfer);

    /* Set up master transfer */
    DSPI_MasterCreateHandle(EXAMPLE_DSPI_MASTER_BASEADDR, &g_m_handle, DSPI_MasterUserCallback, NULL);

    /*Start master transfer*/
    masterXfer.txData = masterTxData;
    masterXfer.rxData = masterRxData;
    masterXfer.dataSize = TRANSFER_SIZE;
    masterXfer.configFlags = kDSPI_MasterCtar0 | EXAMPLE_DSPI_MASTER_PCS_FOR_TRANSFER;

    DSPI_MasterTransferNonBlocking(EXAMPLE_DSPI_MASTER_BASEADDR, &g_m_handle, &masterXfer);

    /* Wait slave received all data. */
    while (!isTransferCompleted)
    {
    }

    errorCount = 0U;
    for (i = 0U; i < TRANSFER_SIZE; i++)
    {
        if (masterTxData[i] != slaveRxData[i])
        {
            errorCount++;
        }

        if (slaveTxData[i] != masterRxData[i])
        {
            errorCount++;
        }
    }
    if (errorCount == 0U)
    {
        PRINTF(" \r\nDSPI master <-> FLEXIO SPI slave transfer all data matched! \r\n");
    }
    else
    {
        PRINTF(" \r\nError occured in DSPI master <-> FLEXIO SPI slave transfer ! \r\n");
    }

    DSPI_Deinit(EXAMPLE_DSPI_MASTER_BASEADDR);
    FLEXIO_SPI_SlaveDeInit(&spiDev);

    while (1)
    {
    }
}

void DSPI_MasterUserCallback(SPI_Type *base, dspi_master_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        __NOP();
    }
}

void FLEXIO_SPI_SlaveUserCallback(FLEXIO_SPI_Type *base, flexio_spi_slave_edma_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        __NOP();
    }

    if (status == kStatus_DSPI_Error)
    {
        __NOP();
    }

    isTransferCompleted = true;

    PRINTF("This is FLEXIO SPI slave call back . \r\n");
}
