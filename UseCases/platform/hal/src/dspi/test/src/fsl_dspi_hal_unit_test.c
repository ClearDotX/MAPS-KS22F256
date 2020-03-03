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

#include "fsl_dspi_hal_unit_test.h"
#include "fsl_clock_manager.h"
#include "fsl_debug_console.h"
#include "board.h"
#include <stdio.h>
#include <string.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* save off various instances of this struct  */
static dspi_test_status_t s_dspiTestStatus[SPI_INSTANCE_COUNT];

/*! @brief Table of base pointers for SPI instances. */
SPI_Type * const g_dspiBase[SPI_INSTANCE_COUNT] = SPI_BASE_PTRS;

/*! @brief Table of SPI FIFO sizes per instance. */
const uint32_t g_dspiFifoSize[SPI_INSTANCE_COUNT] = FSL_FEATURE_DSPI_FIFO_SIZEx;
/*******************************************************************************
 * Code
 ******************************************************************************/

int main ()
{
    uint32_t instance = 0;
    uint32_t i, j;
    uint32_t fifoSizeCount;
    uint32_t errorFlag = 0;

    /* Initialize standard SDK demo application pins */
    hardware_init();
    for (i = 0; i < SPI_INSTANCE_COUNT; i++)
    {
        configure_spi_pins(i);
    }

    /* Call this function to initialize the console UART.  This function
       enables the use of STDIO functions (PRINTF, scanf, etc.) */
    dbg_uart_init();

    PRINTF("\r\nDSPI Hal unit test \n\r");

    /**************************************************************************/
    /* The following initializes the hardware such as clock gates and clock*/
    /* configuration*/
    /**************************************************************************/

    /* ungate the clocks to the dspi module*/
    for (i = 0; i < SPI_INSTANCE_COUNT; i++)
    {
        CLOCK_SYS_EnableSpiClock(i);
    }

    for (instance = 0; instance < SPI_INSTANCE_COUNT; instance++)
    {
        /* Clear the test status struct for this (and each) instance.*/
        memset(&s_dspiTestStatus[instance], 0, sizeof(dspi_test_status_t));

        SPI_Type *base = g_dspiBase[instance];

        /* configure the fifo size based on the dspi module instance*/
        fifoSizeCount = g_dspiFifoSize[instance];

        /**********************************************/
        /* Test the module enable/disable*/
        /**********************************************/
        DSPI_HAL_Enable(base);
        if (SPI_RD_MCR_MDIS(base) != 0)
        {
            s_dspiTestStatus[instance].B.testStatusDspiEnableDisable += 1;
        }

        DSPI_HAL_Disable(base);
        if (SPI_RD_MCR_MDIS(base) != 1)
        {
            s_dspiTestStatus[instance].B.testStatusDspiEnableDisable += 1;
        }

        /**********************************************/
        /* Test the DSPI_HAL_SetMasterSlaveMode */
        /**********************************************/
        DSPI_HAL_SetMasterSlaveMode(base, kDspiMaster);
        if (SPI_RD_MCR_MSTR(base) != 1)
        {
            s_dspiTestStatus[instance].B.testStatusDspiMasterSlave += 1;
        }

        DSPI_HAL_SetMasterSlaveMode(base, kDspiSlave);
        if (SPI_RD_MCR_MSTR(base) != 0)
        {
            s_dspiTestStatus[instance].B.testStatusDspiMasterSlave += 1;
        }

        /**********************************************/
        /* Test the baud rate divisors (master mode only)*/
        /**********************************************/
        /* first set to master mode as baud rate registers only work in master mode*/
        DSPI_HAL_SetMasterSlaveMode(base, kDspiMaster);

        dspi_baud_rate_divisors_t brDivisors;

        brDivisors.doubleBaudRate = 0x1;
        brDivisors.prescaleDivisor = 0x3;
        brDivisors.baudRateDivisor = 0xF;

        DSPI_HAL_SetBaudDivisors(base, kDspiCtar0, &brDivisors);

        /* read back divisor values to see if they were programmed correctly */
        if ((SPI_RD_CTAR_DBR(base, kDspiCtar0) != 0x1) ||
            (SPI_RD_CTAR_PBR(base, kDspiCtar0) != 0x3) ||
            (SPI_RD_CTAR_BR(base, kDspiCtar0) != 0xF))
        {
            s_dspiTestStatus[instance].B.testStatusDspiBaudRateDiv = 1;
        }

        /**********************************************/
        /* Test the dspi baud rate function (master mode only)*/
        /* Master mode set in previous test*/
        /**********************************************/
        /* this test specifically uses a bus cock of 60Mhz for it's calculation to ensure a*/
        /* baud match match.  */
        uint32_t sourceClkHz = 60000000;
        uint32_t baudRates[10] = {261, 523, 1220, 5859, 78125, 187500, 625000, 1875000, 15000000,
                                30000000};
        uint32_t reCalculatedBaudRate, dbr, pbr, br;

        /* spot check a few baud rates*/
        for (i = 0; i < 10; i++)
        {
            if (DSPI_HAL_SetBaudRate(base, kDspiCtar0, baudRates[i], sourceClkHz) != baudRates[i])
            {
                s_dspiTestStatus[instance].B.testStatusDspiBaudRateTest = +1;
            }

            /* now re-calculate the baud rate based on the dividers programmed*/
            dbr = SPI_RD_CTAR_DBR(base, kDspiCtar0);
            pbr = SPI_RD_CTAR_PBR(base, kDspiCtar0);
            br = SPI_RD_CTAR_BR(base, kDspiCtar0);
            reCalculatedBaudRate = ((sourceClkHz * (dbr + 1)) / (s_baudratePrescaler[pbr] *
                                (s_baudrateScaler[br])));

            if (reCalculatedBaudRate != baudRates[i])
            {
                s_dspiTestStatus[instance].B.testStatusDspiBaudRateTest = +1;
            }
        }

        /* now, select a few baud rates that are not exact matches and ensure they "round down"*/
        /* to the closest baud rate (and to ensure the calculated baud rate doesn't exceed*/
        /* the desired baud rate)*/
        uint32_t desiredBaudRates[11] = {100, 270, 1000, 1500, 6000, 79000, 200000, 720000, 1900000,
                                         18000000, 35000000};
        uint32_t roundDownBaudRates[11] = {0, 261, 915, 1464, 5859, 78125, 187500, 625000, 1875000,
                                           15000000, 30000000};
        for (i = 0; i < 11; i++)
        {
            if (DSPI_HAL_SetBaudRate(base, kDspiCtar0, desiredBaudRates[i], sourceClkHz)
                != roundDownBaudRates[i])
            {
                s_dspiTestStatus[instance].B.testStatusDspiBaudRateTest = +1;
            }
        }


        /**********************************************/
        /* Test the DSPI_HAL_SetContinuousSckCmd */
        /**********************************************/
        DSPI_HAL_SetContinuousSckCmd(base, true);
        if (SPI_RD_MCR_CONT_SCKE(base) != 1)
        {
            s_dspiTestStatus[instance].B.testStatusDspiContSck += 1;
        }

        DSPI_HAL_SetContinuousSckCmd(base, false);
        if (SPI_RD_MCR_CONT_SCKE(base) != 0)
        {
            s_dspiTestStatus[instance].B.testStatusDspiContSck += 1;
        }

        /**********************************************/
        /* Test the DSPI_HAL_SetHaltInDebugmodeCmd */
        /**********************************************/
        SPI_BWR_MCR_FRZ(base, 1);
        if (SPI_RD_MCR_FRZ(base) != 1)
        {
            s_dspiTestStatus[instance].B.testStatusDspiFreeze += 1;
        }

        SPI_BWR_MCR_FRZ(base, 0);
        if (SPI_RD_MCR_FRZ(base) != 0)
        {
            s_dspiTestStatus[instance].B.testStatusDspiFreeze += 1;
        }

        /**********************************************/
        /* Test the DSPI_HAL_SetModifiedTimingFormatCmd */
        /**********************************************/
        SPI_BWR_MCR_MTFE(base, 1);
        if (SPI_RD_MCR_MTFE(base) != 1)
        {
            s_dspiTestStatus[instance].B.testStatusDspiModTimingFmt += 1;
        }

        SPI_BWR_MCR_MTFE(base, 0);
        if (SPI_RD_MCR_MTFE(base) != 0)
        {
            s_dspiTestStatus[instance].B.testStatusDspiModTimingFmt += 1;
        }

#if FSL_FEATURE_DSPI_HAS_CHIP_SELECT_STROBE
        /**********************************************/
        /* Test the DSPI_HAL_SetPcsStrobeCmd */
        /**********************************************/
        DSPI_HAL_SetPcsStrobeCmd(base, true);
        if (SPI_RD_MCR_PCSSE(base) != 1)
        {
            s_dspiTestStatus[instance].B.testStatusDspiPcsStrobe += 1;
        }

        DSPI_HAL_SetPcsStrobeCmd(base, false);
        if (SPI_RD_MCR_PCSSE(base) != 0)
        {
            s_dspiTestStatus[instance].B.testStatusDspiPcsStrobe += 1;
        }
#endif
        /**********************************************/
        /* Test the DSPI_HAL_SetRxFifoOverwriteCmd */
        /**********************************************/
        DSPI_HAL_SetRxFifoOverwriteCmd(base, true);
        if (SPI_RD_MCR_ROOE(base) != 1)
        {
            s_dspiTestStatus[instance].B.testStatusDspiRxFifoOvrwrite += 1;
        }

        DSPI_HAL_SetRxFifoOverwriteCmd(base, false);
        if (SPI_RD_MCR_ROOE(base) != 0)
        {
            s_dspiTestStatus[instance].B.testStatusDspiRxFifoOvrwrite += 1;
        }

        /**********************************************/
        /* Test the DSPI_HAL_SetPcsPolarityMode */
        /**********************************************/
        DSPI_HAL_SetPcsPolarityMode(base, kDspiPcs0, kDspiPcs_ActiveHigh);
        DSPI_HAL_SetPcsPolarityMode(base, kDspiPcs1, kDspiPcs_ActiveHigh);
        DSPI_HAL_SetPcsPolarityMode(base, kDspiPcs2, kDspiPcs_ActiveHigh);
        DSPI_HAL_SetPcsPolarityMode(base, kDspiPcs3, kDspiPcs_ActiveHigh);
        DSPI_HAL_SetPcsPolarityMode(base, kDspiPcs4, kDspiPcs_ActiveHigh);
#if (FSL_FEATURE_DSPI_CHIP_SELECT_COUNT == 6)
        DSPI_HAL_SetPcsPolarityMode(base, kDspiPcs5, kDspiPcs_ActiveHigh);
#endif
        if (SPI_RD_MCR_PCSIS(base) != 0)
        {
            s_dspiTestStatus[instance].B.testStatusDspiPcsPolarity += 1;
        }

        DSPI_HAL_SetPcsPolarityMode(base, kDspiPcs0, kDspiPcs_ActiveLow);
        DSPI_HAL_SetPcsPolarityMode(base, kDspiPcs1, kDspiPcs_ActiveLow);
        DSPI_HAL_SetPcsPolarityMode(base, kDspiPcs2, kDspiPcs_ActiveLow);
        DSPI_HAL_SetPcsPolarityMode(base, kDspiPcs3, kDspiPcs_ActiveLow);
        DSPI_HAL_SetPcsPolarityMode(base, kDspiPcs4, kDspiPcs_ActiveLow);
#if (FSL_FEATURE_DSPI_CHIP_SELECT_COUNT == 6)
        DSPI_HAL_SetPcsPolarityMode(base, kDspiPcs5, kDspiPcs_ActiveLow);
#endif

#if (FSL_FEATURE_DSPI_CHIP_SELECT_COUNT == 6)
        if (SPI_RD_MCR_PCSIS(base) != 0x3F)
#else
        if (SPI_RD_MCR_PCSIS(base) != 0x1F)
#endif
        {
            s_dspiTestStatus[instance].B.testStatusDspiPcsPolarity += 1;
        }

        /**********************************************/
        /* Test the DSPI_HAL_SetDozemodeCmd */
        /**********************************************/
        SPI_BWR_MCR_DOZE(base, 1);
        if (SPI_RD_MCR_DOZE(base) != 1)
        {
            s_dspiTestStatus[instance].B.testStatusDspiDozeMode += 1;
        }

        SPI_BWR_MCR_DOZE(base, 0);
        if (SPI_RD_MCR_DOZE(base) != 0)
        {
            s_dspiTestStatus[instance].B.testStatusDspiDozeMode += 1;
        }

        /**********************************************/
        /* Test the DSPI_HAL_SetDatainSamplepointMode */
        /**********************************************/
        DSPI_HAL_SetDatainSamplepointMode(base, kDspiSckToSin_2Clock);
        if (SPI_RD_MCR_SMPL_PT(base) != kDspiSckToSin_2Clock)
        {
            s_dspiTestStatus[instance].B.testStatusDspiSamplePoint += 1;
        }

        DSPI_HAL_SetDatainSamplepointMode(base, kDspiSckToSin_1Clock);
        if (SPI_RD_MCR_SMPL_PT(base) != kDspiSckToSin_1Clock)
        {
            s_dspiTestStatus[instance].B.testStatusDspiSamplePoint += 1;
        }

        DSPI_HAL_SetDatainSamplepointMode(base, kDspiSckToSin_0Clock);
        if (SPI_RD_MCR_SMPL_PT(base) != kDspiSckToSin_0Clock)
        {
            s_dspiTestStatus[instance].B.testStatusDspiSamplePoint += 1;
        }

        /**********************************************/
        /* Test the dpsi stop and start transfer functions*/
        /**********************************************/
        DSPI_HAL_StopTransfer(base);
        if (SPI_RD_MCR_HALT(base) != 1)
        {
            s_dspiTestStatus[instance].B.testStatusDspiStopStart += 1;
        }

        DSPI_HAL_StartTransfer(base);
        if (SPI_RD_MCR_HALT(base) != 0)
        {
            s_dspiTestStatus[instance].B.testStatusDspiStopStart += 1;
        }

        /**********************************************/
        /* Test the dpsi preset and set transfer count functions*/
        /**********************************************/
        /* first enable the module*/
        DSPI_HAL_Enable(base);

        DSPI_HAL_PresetTransferCount(base, 0xFFFF);
        if (DSPI_HAL_GetTransferCount(base) != 0xFFFF)
        {
            s_dspiTestStatus[instance].B.testStatusDspiTransferCount += 1;
        }

        DSPI_HAL_PresetTransferCount(base, 0xAA55);
        if (DSPI_HAL_GetTransferCount(base) != 0xAA55)
        {
            s_dspiTestStatus[instance].B.testStatusDspiTransferCount += 1;
        }

        DSPI_HAL_PresetTransferCount(base, 0x0000);
        if (DSPI_HAL_GetTransferCount(base) != 0x0000)
        {
            s_dspiTestStatus[instance].B.testStatusDspiTransferCount += 1;
        }

        /* lastly disable the module*/
        DSPI_HAL_Disable(base);

        /**********************************************/
        /* Test the dspi set data format function*/
        /**********************************************/
        dspi_data_format_config_t dataFormat;

        dataFormat.bitsPerFrame = 16; /* only for slave mode, master mode gets changed later*/
        dataFormat.clkPolarity = kDspiClockPolarity_ActiveLow;
        dataFormat.clkPhase = kDspiClockPhase_SecondEdge;
        dataFormat.direction = kDspiLsbFirst; /* LSBFE not applicable in slave mode*/

        /* test in slave mode*/
        DSPI_HAL_SetMasterSlaveMode(base, kDspiSlave);

        /* first, ensure that this function returned successfully*/
        if (DSPI_HAL_SetDataFormat(base, kDspiCtar0, &dataFormat) !=
            kStatus_DSPI_Success)
        {
            s_dspiTestStatus[instance].B.testStatusDspiDataFormat += 1;
        }

        /* save off the first read to a temp variable for debug purposes */
        uint32_t temp = SPI_RD_CTAR_SLAVE_FMSZ(base, kDspiCtar0);

        if (temp != (dataFormat.bitsPerFrame - 1))
        {
            s_dspiTestStatus[instance].B.testStatusDspiDataFormat += 1;
        }

        /* read individual bit values to ensure they've been programmed correctly*/
        if ((SPI_RD_CTAR_SLAVE_CPOL(base, kDspiCtar0) != dataFormat.clkPolarity) ||
            (SPI_RD_CTAR_SLAVE_CPHA(base, kDspiCtar0) != dataFormat.clkPhase))
        {
            s_dspiTestStatus[instance].B.testStatusDspiDataFormat += 1;
        }

        /* test in master mode*/
        DSPI_HAL_SetMasterSlaveMode(base, kDspiMaster);

        dataFormat.bitsPerFrame = 8;

        /* first, ensure that this function returned successfully*/
        if (DSPI_HAL_SetDataFormat(base, kDspiCtar1, &dataFormat) !=
            kStatus_DSPI_Success)
        {
            s_dspiTestStatus[instance].B.testStatusDspiDataFormat += 1;
        }

        /* read individual bit values to ensure they've been programmed correctly*/
        if ((SPI_RD_CTAR_FMSZ(base, kDspiCtar1) != (dataFormat.bitsPerFrame - 1)) ||
            (SPI_RD_CTAR_CPOL(base, kDspiCtar1) != dataFormat.clkPolarity) ||
            (SPI_RD_CTAR_CPHA(base, kDspiCtar1) != dataFormat.clkPhase) ||
            (SPI_RD_CTAR_LSBFE(base, kDspiCtar1) != dataFormat.direction))
        {
            s_dspiTestStatus[instance].B.testStatusDspiDataFormat += 1;
        }

        /**********************************************/
        /* Test the interrupt functions*/
        /**********************************************/
        /* set the interrupt enable*/
        DSPI_HAL_SetIntMode(base, kDspiTxComplete, true);
        if (DSPI_HAL_GetIntMode(base, kDspiTxComplete) != 1)
        {
            s_dspiTestStatus[instance].B.testStatusDspiInterruptConfig += 1;
        }
        DSPI_HAL_SetIntMode(base, kDspiEndOfQueue, true);
        if (DSPI_HAL_GetIntMode(base, kDspiEndOfQueue) != 1)
        {
            s_dspiTestStatus[instance].B.testStatusDspiInterruptConfig += 1;
        }
        DSPI_HAL_SetIntMode(base, kDspiTxFifoUnderflow, true);
        if (DSPI_HAL_GetIntMode(base, kDspiTxFifoUnderflow) != 1)
        {
            s_dspiTestStatus[instance].B.testStatusDspiInterruptConfig += 1;
        }
        DSPI_HAL_SetIntMode(base, kDspiTxFifoFillRequest, true);
        if (DSPI_HAL_GetIntMode(base, kDspiTxFifoFillRequest) != 1)
        {
            s_dspiTestStatus[instance].B.testStatusDspiInterruptConfig += 1;
        }
        DSPI_HAL_SetIntMode(base, kDspiRxFifoOverflow, true);
        if (DSPI_HAL_GetIntMode(base, kDspiRxFifoOverflow) != 1)
        {
            s_dspiTestStatus[instance].B.testStatusDspiInterruptConfig += 1;
        }
        DSPI_HAL_SetIntMode(base, kDspiRxFifoDrainRequest, true);
        if (DSPI_HAL_GetIntMode(base, kDspiRxFifoDrainRequest) != 1)
        {
            s_dspiTestStatus[instance].B.testStatusDspiInterruptConfig += 1;
        }

        /* clear the interrupt enable*/
        DSPI_HAL_SetIntMode(base, kDspiTxComplete, false);
        if (DSPI_HAL_GetIntMode(base, kDspiTxComplete) != 0)
        {
            s_dspiTestStatus[instance].B.testStatusDspiInterruptConfig += 1;
        }
        DSPI_HAL_SetIntMode(base, kDspiEndOfQueue, false);
        if (DSPI_HAL_GetIntMode(base, kDspiEndOfQueue) != 0)
        {
            s_dspiTestStatus[instance].B.testStatusDspiInterruptConfig += 1;
        }
        DSPI_HAL_SetIntMode(base, kDspiTxFifoUnderflow, false);
        if (DSPI_HAL_GetIntMode(base, kDspiTxFifoUnderflow) != 0)
        {
            s_dspiTestStatus[instance].B.testStatusDspiInterruptConfig += 1;
        }
        DSPI_HAL_SetIntMode(base, kDspiTxFifoFillRequest, false);
        if (DSPI_HAL_GetIntMode(base, kDspiTxFifoFillRequest) != 0)
        {
            s_dspiTestStatus[instance].B.testStatusDspiInterruptConfig += 1;
        }
        DSPI_HAL_SetIntMode(base, kDspiRxFifoOverflow, false);
        if (DSPI_HAL_GetIntMode(base, kDspiRxFifoOverflow) != 0)
        {
            s_dspiTestStatus[instance].B.testStatusDspiInterruptConfig += 1;
        }
        DSPI_HAL_SetIntMode(base, kDspiRxFifoDrainRequest, false);
        if (DSPI_HAL_GetIntMode(base, kDspiRxFifoDrainRequest) != 0)
        {
            s_dspiTestStatus[instance].B.testStatusDspiInterruptConfig += 1;
        }

        /**********************************************/
        /* Test the DSPI_HAL_SetFifoCmd */
        /**********************************************/
        /* need to first enable the dspi to write to the DSI_TXF and DIS_RXF*/
        DSPI_HAL_Enable(base);

        DSPI_HAL_SetFifoCmd(base, false, false); /* disable fifos*/
        if ((SPI_RD_MCR_DIS_TXF(base) != 1) || (SPI_RD_MCR_DIS_RXF(base) != 1))
        {
            s_dspiTestStatus[instance].B.testStatusDspiFifoConfig = 1;
        }

        DSPI_HAL_SetFifoCmd(base, true, true); /* enable fifos*/
        if ((SPI_RD_MCR_DIS_TXF(base) != 0) || (SPI_RD_MCR_DIS_RXF(base) != 0))
        {
            s_dspiTestStatus[instance].B.testStatusDspiFifoConfig = 1;
        }

        /**********************************************/
        /* Test the filling the TX Fifo and the TX Fifo Counter and Fifo entry data*/
        /* Note: this test proceeds the status flag test as status flags will set*/
        /*       after a transfer is started*/
        /**********************************************/
        /* first, set for master mode and halt (stop) transfers to allow us to fill the tx fifo*/
        DSPI_HAL_SetMasterSlaveMode(base, kDspiMaster);
        DSPI_HAL_StopTransfer(base);

        /* fill up a src buffer with data (4 16-bit words)*/
        uint16_t src[4];
        for (i = 0; i < fifoSizeCount; i++)
        {
            src[i] = 0xFFF0 + i;
        }

        /* fill in members of the dspi_command_data_config_t struct*/
        dspi_command_config_t commandConfig;
        commandConfig.isChipSelectContinuous = true;
        commandConfig.whichCtar = kDspiCtar0;
        commandConfig.whichPcs = kDspiPcs1;
        commandConfig.clearTransferCount = false;
        commandConfig.isEndOfQueue = false;

        /* tx fifo counter should be empty (zero)*/
        if (SPI_RD_SR_TXCTR(base) != 0)
        {
            s_dspiTestStatus[instance].B.testStatusDspiTxFifoCounter = 1;
        }

        /* now fill the tx fifo*/
        for (i = 0; i < fifoSizeCount; i++)
        {
            if (i == 0) /* first data word*/
            {
                commandConfig.clearTransferCount = true; /* set to true for first word*/

                /* if fifoSizeCount is 1, then set the end of queue flag  */
                if(fifoSizeCount == 1)
                {
                    commandConfig.isEndOfQueue = true;
                }

                DSPI_HAL_WriteDataMastermode(base, &commandConfig, src[i]);
                commandConfig.clearTransferCount = false; /* set back to false after first word*/

                /* tx fifo counter should be increased by one*/
                if (SPI_RD_SR_TXCTR(base) != 1)
                {
                    s_dspiTestStatus[instance].B.testStatusDspiTxFifoCounter = 1;
                }
            }
            else if (i == 3) /* last data word*/
            {
                /* test the DSPI_HAL_WriteCmdDataMastermode function here */
                uint32_t tempWord;
                commandConfig.isEndOfQueue = true;

                /* First, build up the 32-bit word then write it to the PUSHR */
                tempWord = SPI_PUSHR_CONT(commandConfig.isChipSelectContinuous) |
                           SPI_PUSHR_CTAS(commandConfig.whichCtar) |
                           SPI_PUSHR_PCS(commandConfig.whichPcs) |
                           SPI_PUSHR_EOQ(commandConfig.isEndOfQueue) |
                           SPI_PUSHR_CTCNT(commandConfig.clearTransferCount) |
                           SPI_PUSHR_TXDATA(src[i]);

                DSPI_HAL_WriteCmdDataMastermode(base, tempWord);

                /* tx fifo counter should be full (four)*/
                if (SPI_RD_SR_TXCTR(base) != 4)
                {
                    s_dspiTestStatus[instance].B.testStatusDspiTxFifoCounter = 1;
                }
            }
            else
            {
                DSPI_HAL_WriteDataMastermode(base, &commandConfig, src[i]);

                /* tx fifo counter should be increased by one*/
                if (SPI_RD_SR_TXCTR(base) != (1+i))
                {
                    s_dspiTestStatus[instance].B.testStatusDspiTxFifoCounter = 1;
                }
            }
        }

        /* read back the fifo contents of the transmit fifo registers (TXFRn)*/
        for (i = 0; i < fifoSizeCount; i++)
        {
            if (i == 0) /* first data word*/
            {
                /* if fifoSizeCount is 1, then set the end of queue flag  */
                if(fifoSizeCount == 1)
                {
                    /* 0x8C020000 => CONT, EOQ, CTCNT, and PCS[1] are set*/
                    if (SPI_RD_TXFR0(base) != (0x8C020000 | src[i]))
                    {
                        s_dspiTestStatus[instance].B.testStatusDspiTxFifoTest = 1;
                    }
                }
                else
                {
                    /* 0x84020000 => CONT, CTCNT, and PCS[1] are set*/
                    if (SPI_RD_TXFR0(base) != (0x84020000 | src[i]))
                    {
                        s_dspiTestStatus[instance].B.testStatusDspiTxFifoTest = 1;
                    }
                }
            }
            else if (i == 3) /* last data word*/
            {
                /* 0x88020000 => CONT, EOQ, and PCS[1] are set*/
                if (SPI_RD_TXFR3(base) != (0x88020000 | src[i]))
                {
                    s_dspiTestStatus[instance].B.testStatusDspiTxFifoTest = 1;
                }
            }
            else if (i == 2)
            {
                /* 0x80020000 => CONT and PCS[1] are set*/
                if (SPI_RD_TXFR2(base) != (0x80020000 | src[i]))
                {
                    s_dspiTestStatus[instance].B.testStatusDspiTxFifoTest = 1;
                }
            }
            else /* i == 1 */
            {
                /* 0x80020000 => CONT and PCS[1] are set*/
                if (SPI_RD_TXFR1(base) != (0x80020000 | src[i]))
                {
                    s_dspiTestStatus[instance].B.testStatusDspiTxFifoTest = 1;
                }
            }
        }

        /**********************************************/
        /* Test the status flag function (and RX fifo counter and pop next pointer test)*/
        /**********************************************/
        /* set the baud rate to the slowest value from the baud rate test above*/
        DSPI_HAL_SetBaudRate(base, kDspiCtar0, baudRates[0], sourceClkHz);
        /* the following status flags should be set once a transfer has started*/
        DSPI_HAL_StartTransfer(base);

        /* immediately check the run status flag*/
        if (DSPI_HAL_GetStatusFlag(base, kDspiTxAndRxStatus) != 1)
        {
            s_dspiTestStatus[instance].B.testStatusDspiStatusFlagSet += 1;
        }

        /* now wait for transfer to complete by waiting till transfer count gets to */
        /* "fifoSizeCount" words*/
        while (DSPI_HAL_GetTransferCount(base) != fifoSizeCount) {}

        /* first start with the RX fifo counter and POPNXTPRT flags, even though nothing was */
        /* connected to the rx line the receive fifo will still shift in data even if */
        /* nothing is connected.  It should be equal to fifoSizeCount.*/
        if (SPI_RD_SR_RXCTR(base) != fifoSizeCount)
        {
            s_dspiTestStatus[instance].B.testStatusDspiRxFifoCounter += 1;
        }
        /* now read from the rx fifo */
        for (i = 0; i < fifoSizeCount; i++)
        {
            /* see if rx pop next pointer incremented*/
            if (SPI_RD_SR_POPNXTPTR(base) != i)
            {
                s_dspiTestStatus[instance].B.testStatusDspiRxFifoCounter += 1;
            }

            /* read from fifo*/
            DSPI_HAL_ReadData(base);
            /* see if rx fifo counter decremented*/
            if (SPI_RD_SR_RXCTR(base) !=
               ((fifoSizeCount - 1) -i))
            {
                s_dspiTestStatus[instance].B.testStatusDspiRxFifoCounter += 1;
            }
        }

        /* Check the various status flags to ensure they were properly set*/
        if (DSPI_HAL_GetStatusFlag(base, kDspiTxComplete) != 1)
        {
            s_dspiTestStatus[instance].B.testStatusDspiStatusFlagSet += 1;
        }
        if (DSPI_HAL_GetStatusFlag(base, kDspiEndOfQueue) != 1)
        {
            s_dspiTestStatus[instance].B.testStatusDspiStatusFlagSet += 1;
        }
        if (DSPI_HAL_GetStatusFlag(base, kDspiTxFifoFillRequest) != 1)
        {
            s_dspiTestStatus[instance].B.testStatusDspiStatusFlagSet += 1;
        }
        if (DSPI_HAL_GetStatusFlag(base, kDspiRxFifoDrainRequest) != 1)
        {
            s_dspiTestStatus[instance].B.testStatusDspiStatusFlagSet += 1;
        }

        /* clear the status flags*/
        DSPI_HAL_ClearStatusFlag(base, kDspiTxComplete);
        if (DSPI_HAL_GetStatusFlag(base, kDspiTxComplete) != 0)
        {
            s_dspiTestStatus[instance].B.testStatusDspiStatusFlagClr += 1;
        }
        DSPI_HAL_ClearStatusFlag(base, kDspiEndOfQueue);
        if (DSPI_HAL_GetStatusFlag(base, kDspiEndOfQueue) != 0)
        {
            s_dspiTestStatus[instance].B.testStatusDspiStatusFlagClr += 1;
        }
        DSPI_HAL_ClearStatusFlag(base, kDspiRxFifoDrainRequest);
        if (DSPI_HAL_GetStatusFlag(base, kDspiRxFifoDrainRequest) != 0)
        {
            s_dspiTestStatus[instance].B.testStatusDspiStatusFlagClr += 1;
        }

        /* TFFF (tx fifo fil1 request, is a special case. Normally, this status flag operates in*/
        /* conjunction with the DMA.  However, without DMA, this bit will only clear if:*/
        /* 1. Verify TFFF is set (TX fifo not full)*/
        /* 2. The TX fifo is full*/
        /* 3. We manually clear it (w1c), if tx fifo is full, this bit should stay cleared*/

        /* Step 0. First, halt spi transfers, though the fifo will fill quicker than the */
        /*         the transmit shift register can shift out data, but for now halt transfer*/
        /*         We call this step 0 since it is not an actual needed step.*/
        DSPI_HAL_StopTransfer(base);

        /* Step 1. Verify TFFF is set*/
        if (DSPI_HAL_GetStatusFlag(base, kDspiTxFifoFillRequest) != 1)
        {
            s_dspiTestStatus[instance].B.testStatusDspiStatusFlagSet += 1;
        }

        /* Step 2. Fill TX fifo. This routine will keep filling the TX fifo and then manually
         *         clear TFFF and if it stays clear, exits, else it keeps filling the fifo
         *         We'll set a limit of 20 loops such that if we hit the limit, we'll know something
         *         didn't work since the spi fifos are not configured for such large fifo depths
         */
        for ( i = 0; i < 20; i++)
        {
            /* note, commandConfig defined above, don't care what the conents are*/
            DSPI_HAL_WriteDataMastermode(base, &commandConfig, i);

            DSPI_HAL_ClearStatusFlag(base, kDspiTxFifoFillRequest);
            if (DSPI_HAL_GetStatusFlag(base, kDspiTxFifoFillRequest) == 0)
            {
                break;
            }
         }

        /* see if we hit the limit*/
        if (i == 20)
        {
            s_dspiTestStatus[instance].B.testStatusDspiStatusFlagTfffClr += 1;
        }
        /* double check to make sure TFFF is cleared*/
        if (DSPI_HAL_GetStatusFlag(base, kDspiTxFifoFillRequest) != 0)
        {
            s_dspiTestStatus[instance].B.testStatusDspiStatusFlagTfffClr += 1;
        }

        /* now, for one final test of the TFFF bit, let's see if it sets when the transfer */
        /* is started*/
        DSPI_HAL_StartTransfer(base);
        /* wait for tx to complete*/
        while(DSPI_HAL_GetStatusFlag(base, kDspiTxComplete) != 1) { }

        if (DSPI_HAL_GetStatusFlag(base, kDspiTxFifoFillRequest) != 1)
        {
            s_dspiTestStatus[instance].B.testStatusDspiStatusFlagTfffSet += 1;
        }

        /**********************************************/
        /* Test the TX and RX Fifo Flush*/
        /**********************************************/
        /* flush the tx fifo*/
        DSPI_HAL_SetFlushFifoCmd(base, true, true);

        /* read back the fifo contents of the transmit fifo registers (TXFRn)*/
        /* should be cleared*/
        for (i = 0; i < fifoSizeCount; i++)
        {
            if (i == 0)
            {
                if ((SPI_RD_TXFR0(base) != 0x0) ||
                    (SPI_RD_RXFR0(base) != 0x0))
                {
                    s_dspiTestStatus[instance].B.testStatusDspiFifoFlush += 1;
                }
            }
            if (i == 1)
            {
                if ((SPI_RD_TXFR1(base) != 0x0) ||
                    (SPI_RD_RXFR1(base) != 0x0))
                {
                    s_dspiTestStatus[instance].B.testStatusDspiFifoFlush += 1;
                }
            }
            if (i == 2)
            {
                if ((SPI_RD_TXFR2(base) != 0x0) ||
                    (SPI_RD_RXFR2(base) != 0x0))
                {
                    s_dspiTestStatus[instance].B.testStatusDspiFifoFlush += 1;
                }
            }
            if (i == 3)
            {
                if ((SPI_RD_TXFR3(base) != 0x0) ||
                    (SPI_RD_RXFR3(base) != 0x0))
                {
                    s_dspiTestStatus[instance].B.testStatusDspiFifoFlush += 1;
                }
            }
        }

        /**********************************************/
        /* Test the DMA Configure functions*/
        /**********************************************/
        DSPI_HAL_SetTxFifoFillDmaIntMode(base, kDspiGenerateDmaReq, false);
        DSPI_HAL_SetRxFifoDrainDmaIntMode(base, kDspiGenerateIntReq, false);
        if ((SPI_RD_RSER_TFFF_DIRS(base) != 1) ||
            (SPI_RD_RSER_RFDF_DIRS(base) != 0))
        {
            s_dspiTestStatus[instance].B.testStatusDspiDmaConfig += 1;
        }

        DSPI_HAL_SetTxFifoFillDmaIntMode(base, kDspiGenerateIntReq, false);
        DSPI_HAL_SetRxFifoDrainDmaIntMode(base, kDspiGenerateDmaReq, false);
        if ((SPI_RD_RSER_TFFF_DIRS(base) != 0) ||
            (SPI_RD_RSER_RFDF_DIRS(base) != 1))
        {
            s_dspiTestStatus[instance].B.testStatusDspiDmaConfig += 1;
        }

        /**********************************************/
        /* Test the dspi init function*/
        /**********************************************/
        DSPI_HAL_Init(base);

        if ((SPI_RD_MCR(base) != (SPI_MCR_MDIS_MASK | SPI_MCR_HALT_MASK)) ||
            (SPI_RD_TCR(base) != 0) ||
            (SPI_RD_CTAR(base, kDspiCtar0) != 0x78000000) ||
            (SPI_RD_CTAR(base, kDspiCtar1) != 0x78000000) ||
            (SPI_RD_RSER(base) != 0))
        {
            s_dspiTestStatus[instance].B.testStatusDspiReset = 1;
        }

        /**********************************************/
        /* Test the dspi calculate delay function*/
        /**********************************************/
        DSPI_HAL_SetMasterSlaveMode(base, kDspiMaster);

        uint32_t failCount = 0;
        sourceClkHz = 60000000;
        uint32_t delayInNanoSec;
        uint32_t initialDelay, calcDelay;

        initialDelay = (1000000000/sourceClkHz) * 2;

        /* first test inital value of 0 and 1 */
        for (delayInNanoSec = 0; delayInNanoSec < 10; delayInNanoSec++)
        {
            calcDelay = DSPI_HAL_CalculateDelay(base, kDspiCtar0, kDspiPcsToSck, sourceClkHz,
                                                delayInNanoSec);
            if (calcDelay!= initialDelay)
            {
                failCount++;
            }
        }

        /* Now confirm other values of the delay */
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 16; j++)
            {
                delayInNanoSec = (1000000000/sourceClkHz)*s_delayPrescaler[i]*s_delayScaler[j];

                /* Init delay values */
                DSPI_HAL_SetDelay(base, kDspiCtar0, 0, 0, kDspiPcsToSck);
                calcDelay = DSPI_HAL_CalculateDelay(base, kDspiCtar0, kDspiPcsToSck, sourceClkHz,
                                                    delayInNanoSec);
                if (calcDelay != delayInNanoSec)
                {
                    failCount++;
                    PRINTF("calcDelay = %d \n\r", calcDelay);
                    PRINTF("delayInNanoSec = %d \n\r", delayInNanoSec);
                    PRINTF("i = %d, j = %d \n", i, j);
                }
                /* Check PCS to SCK delay values */
                if ((SPI_RD_CTAR_PCSSCK(base, kDspiCtar0) != i) ||
                    (SPI_RD_CTAR_CSSCK(base, kDspiCtar0) != j))
                {
                    failCount++;
                }

                /* Check last SCK to PCS delay values */
                /* Init delay values */
                DSPI_HAL_SetDelay(base, kDspiCtar0, 0, 0, kDspiLastSckToPcs);
                calcDelay = DSPI_HAL_CalculateDelay(base, kDspiCtar0, kDspiLastSckToPcs,
                                                    sourceClkHz, delayInNanoSec);
                if ((SPI_RD_CTAR_PASC(base, kDspiCtar0) != i) ||
                    (SPI_RD_CTAR_ASC(base, kDspiCtar0) != j))
                {
                    failCount++;
                }
                /* Check after transfer delay values */
                /* Init delay values */
                DSPI_HAL_SetDelay(base, kDspiCtar0, 0, 0, kDspiAfterTransfer);
                calcDelay = DSPI_HAL_CalculateDelay(base, kDspiCtar0, kDspiAfterTransfer,
                                                    sourceClkHz, delayInNanoSec);
                if ((SPI_RD_CTAR_PDT(base, kDspiCtar0) != i) ||
                    (SPI_RD_CTAR_DT(base, kDspiCtar0) != j))
                {
                    failCount++;
                }
            }
        }

        /* Pass in various values to DSPI_HAL_CalculateDelay and see if we get a close match */
        for (i = 10; i < 10000001; i*=10)
        {
            calcDelay = DSPI_HAL_CalculateDelay(base, kDspiCtar0, kDspiPcsToSck,
                                                sourceClkHz, i);
            /* Each calculated value should be equal or larger to the requested value, hence
             * check each one to see if it is a close match per known calculated values.
             * For the final value, we will max out the calculation so see if we get the max
             * value.  The driver will have to notify user that value doesn't meet expectations.
             */
            if (i == 10)
            {
                if (calcDelay != 32)
                {
                    failCount++;
                }
            }
            if (i == 100)
            {
                if (calcDelay != 128)
                {
                    failCount++;
                }
            }
            if (i == 1000)
            {
                if (calcDelay != 1024)
                {
                    failCount++;
                }
            }
            if (i == 10000)
            {
                if (calcDelay != 10240)
                {
                    failCount++;
                }
            }
            if (i == 100000)
            {
                if (calcDelay != 114688)
                {
                    failCount++;
                }
            }
            if (i == 1000000)
            {
                if (calcDelay != 1048576)
                {
                    failCount++;
                }
            }
            if (i == 10000000)
            {
                if (calcDelay != 7340032)
                {
                    failCount++;
                }
            }
        }

        if (failCount)
        {
           s_dspiTestStatus[instance].B.testStatusDspiDelayCalc = 1;
        }


    }  /* end of for-loop hal tests*/


    /**************************************************************************/
    /* Error reporting*/
    /**************************************************************************/
    for (instance = 0; instance < SPI_INSTANCE_COUNT; instance++)
    {
        if (s_dspiTestStatus[instance].all)
        {
            PRINTF("\r\n**failures seen on instance %d\n", instance);

            if (s_dspiTestStatus[instance].B.testStatusDspiEnableDisable == 1)
            {
                PRINTF("\r  DspiEnableDisable failed \n ");
            }
            if (s_dspiTestStatus[instance].B.testStatusDspiBaudRateDiv == 1)
            {
                PRINTF("\r  DspiBaudRateDiv failed \n ");
            }
            if (s_dspiTestStatus[instance].B.testStatusDspiMasterSlave == 1)
            {
                PRINTF("\r  DspiMasterSlave failed \n ");
            }
            if (s_dspiTestStatus[instance].B.testStatusDspiContSck == 1)
            {
                PRINTF("\r  DspiContSck failed \n ");
            }
            if (s_dspiTestStatus[instance].B.testStatusDspiPcsStrobe == 1)
            {
                PRINTF("\r  DspiPcsStrobe failed \n ");
            }
            if (s_dspiTestStatus[instance].B.testStatusDspiRxFifoOvrwrite == 1)
            {
                PRINTF("\r  DspiRxFifoOvrwrite failed \n ");
            }
            if (s_dspiTestStatus[instance].B.testStatusDspiPcsPolarity == 1)
            {
                PRINTF("\r  DspiPcsPolarity failed \n ");
            }
            if (s_dspiTestStatus[instance].B.testStatusDspiSamplePoint == 1)
            {
                PRINTF("\r  DspiSamplePoint failed \n ");
            }
            if (s_dspiTestStatus[instance].B.testStatusDspiStopStart == 1)
            {
                PRINTF("\r  DspiStopStart failed \n ");
            }
            if (s_dspiTestStatus[instance].B.testStatusDspiTransferCount == 1)
            {
                PRINTF("\r  DspiTransferCount failed \n ");
            }
            if (s_dspiTestStatus[instance].B.testStatusDspiDataFormat == 1)
            {
                PRINTF("\r  DspiDataFormat failed \n ");
            }
            if (s_dspiTestStatus[instance].B.testStatusDspiInterruptConfig == 1)
            {
                PRINTF("\r  DspiInterruptConfig failed \n ");
            }
            if (s_dspiTestStatus[instance].B.testStatusDspiFifoConfig == 1)
            {
                PRINTF("\r  DspiFifoConfig failed \n ");
            }
            if (s_dspiTestStatus[instance].B.testStatusDspiTxFifoTest == 1)
            {
                PRINTF("\r  DspiTxFifoTest failed \n ");
            }
            if (s_dspiTestStatus[instance].B.testStatusDspiStatusFlagSet == 1)
            {
                PRINTF("\r  DspiStatusFlagSet failed \n ");
            }
            if (s_dspiTestStatus[instance].B.testStatusDspiStatusFlagClr == 1)
            {
                PRINTF("\r  DspiStatusFlagClr failed \n ");
            }
            if (s_dspiTestStatus[instance].B.testStatusDspiFifoFlush == 1)
            {
                PRINTF("\r  DspiFifoFlush failed \n ");
            }
            if (s_dspiTestStatus[instance].B.testStatusDspiDmaConfig == 1)
            {
                PRINTF("\r  DspiDmaConfig failed \n ");
            }
            if (s_dspiTestStatus[instance].B.testStatusDspiBaudRateTest == 1)
            {
                PRINTF("\r  DspiBaudRateTest failed \n ");
            }
            if (s_dspiTestStatus[instance].B.testStatusDspiReset == 1)
            {
                PRINTF("\r  DspiReset failed \n ");
            }
            if (s_dspiTestStatus[instance].B.testStatusDspiDelayCalc == 1)
            {
                PRINTF("\r  DspiDelayCalc failed \n ");
            }

            errorFlag = 1;
        }
        else
        {
            PRINTF("\r\nAll Tests PASSED, instance %d\n", instance);
        }
    }

    /* trap here if any failures were reported (useful for cases where the uart is not used) */
    if (errorFlag == 1)
    {
        while(1) { }
    }

    return 0;

}

/*******************************************************************************
 * EOF
 ******************************************************************************/

