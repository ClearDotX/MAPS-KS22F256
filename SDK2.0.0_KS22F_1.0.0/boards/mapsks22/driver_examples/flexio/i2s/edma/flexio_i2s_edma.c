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

#include "board.h"
#include "music.h"
#include "fsl_dmamux.h"
#include "fsl_flexio_i2s_edma.h"
#include "fsl_debug_console.h"

#include "fsl_wm8960.h"
#include "fsl_sai.h"
#include "fsl_device_registers.h"
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* SAI and I2C instance and clock */
#define DEMO_SAI I2S0
#define DEMO_I2C LPI2C0
#define DEMO_FLEXIO_BASE FLEXIO
#define DEMO_SAI_CLKSRC kCLOCK_CoreSysClk
#define DEMO_I2C_CLKSRC kCLOCK_McgIrc48MClk
#define DEMO_FLEXIO_CLKSRC kCLOCK_McgIrc48MClk
#define DEMO_CODEC_WM8960 (1)
#define EXAMPLE_DMA (DMA0)
#define EXAMPLE_CHANNEL (0)
#define EXAMPLE_DMA_SOURCE (38)
#define BCLK_PIN (4)
#define FRAME_SYNC_PIN (6)
#define TX_DATA_PIN (5)
#define OVER_SAMPLE_RATE (384U)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void callback(FLEXIO_I2S_Type *i2sBase, flexio_i2s_edma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
flexio_i2s_edma_handle_t txHandle = {0};
edma_handle_t dmaHandle = {0};
#if defined(DEMO_CODEC_WM8960)
wm8960_handle_t codecHandle = {0};
#else
sgtl_handle_t codecHandle = {0};
#endif

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
lpi2c_master_handle_t i2cHandle = {0};
#else
i2c_master_handle_t i2cHandle = {0};
#endif
volatile bool isFinished = false;
FLEXIO_I2S_Type base;
/*******************************************************************************
 * Code
 ******************************************************************************/

static void callback(FLEXIO_I2S_Type *i2sBase, flexio_i2s_edma_handle_t *handle, status_t status, void *userData)
{
    isFinished = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    flexio_i2s_config_t config;
    uint32_t sourceClockHz = 0U;
    flexio_i2s_format_t format;
    flexio_i2s_transfer_t xfer;
    edma_config_t dmaConfig = {0};
#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    lpi2c_master_config_t i2cConfig = {0};
#else
    i2c_master_config_t i2cConfig = {0};
#endif
    uint32_t i2cSourceClock;
    uint32_t temp = 0;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    CLOCK_SetPllFllSelClock(3U); /* IRC48 MHz clock. */
    CLOCK_SetLpi2c0Clock(1);
    CLOCK_SetFlexio0Clock(1);

    PRINTF("FLEXIO I2S EDMA example started!\n\r");

    /* Set flexio i2s pin, shifter and timer */
    base.bclkPinIndex = BCLK_PIN;
    base.fsPinIndex = FRAME_SYNC_PIN;
    base.txPinIndex = TX_DATA_PIN;
    base.rxPinIndex = 0;
    base.txShifterIndex = 0;
    base.rxShifterIndex = 1;
    base.bclkTimerIndex = 0;
    base.fsTimerIndex = 1;
    base.flexioBase = FLEXIO;

#if defined(DEMO_CODEC_WM8960)
    /* Use SAI to do master clock if using WM8960 codec */
    sai_config_t saiConfig;
    SAI_TxGetDefaultConfig(&saiConfig);
    SAI_TxInit(DEMO_SAI, &saiConfig);

    /* Configure the audio format */
    sai_transfer_format_t saiFormat = {0};
    saiFormat.bitWidth = kSAI_WordWidth16bits;
    saiFormat.channel = 0U;
    saiFormat.sampleRate_Hz = kSAI_SampleRate16KHz;
#if defined(FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) && (FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER)
    saiFormat.masterClockHz = OVER_SAMPLE_RATE * saiFormat.sampleRate_Hz;
#else
    saiFormat.masterClockHz = CLOCK_GetFreq(DEMO_SAI_CLKSRC);
#endif
    saiFormat.protocol = saiConfig.protocol;

    uint32_t masterClockSrc = CLOCK_GetFreq(DEMO_SAI_CLKSRC);
    SAI_TxSetFormat(DEMO_SAI, &saiFormat, masterClockSrc, saiFormat.masterClockHz);
    SAI_TxEnable(DEMO_SAI, true);
#endif
    /* Create EDMA handle */
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(EXAMPLE_DMA, &dmaConfig);
    EDMA_CreateHandle(&dmaHandle, EXAMPLE_DMA, EXAMPLE_CHANNEL);

    DMAMUX_Init(DMAMUX0);
    DMAMUX_SetSource(DMAMUX0, EXAMPLE_CHANNEL, EXAMPLE_DMA_SOURCE);
    DMAMUX_EnableChannel(DMAMUX0, EXAMPLE_CHANNEL);

    /* Init SAI module */
    FLEXIO_I2S_GetDefaultConfig(&config);
    FLEXIO_I2S_Init(&base, &config);

    /* Configure the audio format */
    format.bitWidth = kFLEXIO_I2S_WordWidth16bits;
    format.sampleRate_Hz = kFLEXIO_I2S_SampleRate16KHz;

    /* Configure Sgtl5000 I2C */
    codecHandle.base = DEMO_I2C;
    codecHandle.i2cHandle = &i2cHandle;
    i2cSourceClock = CLOCK_GetFreq(DEMO_I2C_CLKSRC);

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    LPI2C_MasterGetDefaultConfig(&i2cConfig);
    LPI2C_MasterInit(DEMO_I2C, &i2cConfig, i2cSourceClock);
    LPI2C_MasterCreateHandle(DEMO_I2C, &i2cHandle, NULL, NULL);
#else
    I2C_MasterGetDefaultConfig(&i2cConfig);
    I2C_MasterInit(DEMO_I2C, &i2cConfig, i2cSourceClock);
#endif

#if defined(DEMO_CODEC_WM8960)
    WM8960_Init(&codecHandle, NULL);
    WM8960_SetProtocol(&codecHandle, kWM8960_BusI2S);
    WM8960_ConfigDataFormat(&codecHandle, OVER_SAMPLE_RATE * format.sampleRate_Hz, format.sampleRate_Hz,
                            format.bitWidth);
#else
    /* Use default settings for sgtl5000 */
    SGTL_Init(&codecHandle, NULL);
    /* Configure codec format */
    SGTL_ConfigDataFormat(&codecHandle, OVER_SAMPLE_RATE * format.sampleRate_Hz, format.sampleRate_Hz, format.bitWidth);
#endif

    FLEXIO_I2S_TxCreateHandleEDMA(&base, &txHandle, callback, NULL, &dmaHandle);

    sourceClockHz = CLOCK_GetFreq(DEMO_FLEXIO_CLKSRC);
    FLEXIO_I2S_SetTransferFormatEDMA(&base, &txHandle, &format, sourceClockHz);

    /*  xfer structure */
    temp = (uint32_t)music;
    xfer.data = (uint8_t *)temp;
    xfer.dataSize = MUSIC_LEN;
    FLEXIO_I2S_SendEDMA(&base, &txHandle, &xfer);
    /* Wait until finished */
    while (isFinished != true)
    {
    }

#if defined(DEMO_CODEC_WM8960)
    WM8960_Deinit(&codecHandle);
#else
    SGTL_Deinit(&codecHandle);
#endif

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    LPI2C_MasterDeinit(DEMO_I2C);
#else
    I2C_MasterDeinit(DEMO_I2C);
#endif
    PRINTF("\n\r FLEXIO I2S EDMA example finished!\n\r ");

    while (1)
    {
    }
}
