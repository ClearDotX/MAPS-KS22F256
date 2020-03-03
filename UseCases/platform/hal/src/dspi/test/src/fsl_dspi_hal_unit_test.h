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
#ifndef __FSL_DSPI_HAL_UNIT_TEST_H__
#define __FSL_DSPI_HAL_UNIT_TEST_H__

#include "fsl_dspi_hal.h"
#include "fsl_device_registers.h"
#include <assert.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Structure for all DSPI status flags */
typedef union _dspi_test_status
{
    uint32_t all;
    struct _dspi_test_status_bits
    {
        unsigned testStatusDspiEnableDisable : 1; /*!< module enable/disable test*/
        unsigned testStatusDspiBaudRateDiv : 1; /*!< baud rate divisors*/
        unsigned testStatusDspiMasterSlave : 1; /*!< master/slave config*/
        unsigned testStatusDspiContSck : 1; /*!< continuous SCK operation*/
        unsigned testStatusDspiFreeze :1 ; /*!< Freeze operation (halt in debug mode)*/
        unsigned testStatusDspiModTimingFmt : 1; /*!< modified timing format*/
        unsigned testStatusDspiPcsStrobe : 1; /*!< PCS strobe operation*/
        unsigned testStatusDspiRxFifoOvrwrite : 1; /*!< Rx FIFO overwrite*/
        unsigned testStatusDspiPcsPolarity : 1; /*!< pcs polarity*/
        unsigned testStatusDspiDozeMode : 1; /*!< Doze mode */
        unsigned testStatusDspiSamplePoint : 1; /*!< Sample point test*/
        unsigned testStatusDspiStopStart : 1; /*!< stop and start transfer*/
        unsigned testStatusDspiTransferCount : 1; /*!< transfer count preset and get test*/
        unsigned testStatusDspiDataFormat : 1; /*!< data format test*/
        unsigned testStatusDspiInterruptConfig : 1; /*<! interrupt config test*/
        unsigned testStatusDspiFifoConfig : 1; /*!< FIFO disable/enable config*/
        unsigned testStatusDspiTxFifoCounter : 1; /*!< Tx FIFO counter test flag*/
        unsigned testStatusDspiRxFifoCounter : 1; /*!< Rx FIFO counter test flag*/
        unsigned testStatusDspiTxFifoTest : 1; /*!< Tx FIFO fill/read test*/
        unsigned testStatusDspiStatusFlagSet : 1; /*<! status flag set test*/
        unsigned testStatusDspiStatusFlagClr : 1; /*<! status flag clear test*/
        unsigned testStatusDspiStatusFlagTfffClr : 1;/*<! special case TFFF status flag clear test*/
        unsigned testStatusDspiStatusFlagTfffSet : 1; /*<! special case TFFF status flag set test*/
        unsigned testStatusDspiFifoFlush : 1; /*!< FIFO flush test*/
        unsigned testStatusDspiDmaConfig : 1; /*!< DMA config test*/
        unsigned testStatusDspiBaudRateTest : 1; /*!< baud rate test*/
        unsigned testStatusDspiReset : 1; /*!< reset function*/
        unsigned testStatusDspiDelayCalc: 1; /*!< delay calculation function*/

    } B;
} dspi_test_status_t;


#endif /* __FSL_DSPI_HAL_UNIT_TEST_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

