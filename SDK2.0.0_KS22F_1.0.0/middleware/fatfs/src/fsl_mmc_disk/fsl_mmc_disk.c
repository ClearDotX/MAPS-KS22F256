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
 * DISCLAIMED. IN NO EVENT SL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "fsl_sdhc.h"
#include "fsl_card.h"
#include "fsl_mmc_disk.h"
#include "diskio.h"
#include "sdhc_config.h"

/*******************************************************************************
 * Definitons
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief User defined transfer function.
 *
 * @param base SDHC peripheral base address.
 * @param content SDHC transfer state.
 * @retval kStatus_InvalidArgument Argument is invalid.
 * @retval kStatus_SDHC_PrepareAdmaDescriptorFailed Prepare ADMA descriptor failed.
 * @retval kStatus_SDHC_SendCommandFailed Send command failed.
 * @retval kStatus_SDHC_TransferDataFailed Transfer data failed.
 * @retval kStatus_Success Success.
 */
static status_t sdhc_transfer_function(SDHC_Type *base, sdhc_transfer_t *content);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* State in SDHC driver. */
static uint32_t g_sdhcAdmaTable[SDHC_ADMA_TABLE_WORDS];
static sdhc_handle_t g_sdhcHandle;

/* State in Card driver. */
static mmc_card_t g_mmc;

/*******************************************************************************
 * Code
 ******************************************************************************/

#if _USE_WRITE
DRESULT mmc_disk_write(uint8_t physicalDrive, const uint8_t *buffer, uint32_t sector, uint8_t count)
{
    if (physicalDrive != MMCDISK)
    {
        return RES_PARERR;
    }

    if (kStatus_Success != MMC_WriteBlocks(&g_mmc, buffer, sector, count))
    {
        return RES_ERROR;
    }
    return RES_OK;
}
#endif

DRESULT mmc_disk_read(uint8_t physicalDrive, uint8_t *buffer, uint32_t sector, uint8_t count)
{
    if (physicalDrive != MMCDISK)
    {
        return RES_PARERR;
    }

    if (kStatus_Success != MMC_ReadBlocks(&g_mmc, buffer, sector, count))
    {
        return RES_ERROR;
    }
    return RES_OK;
}

#if _USE_IOCTL
DRESULT mmc_disk_ioctl(uint8_t physicalDrive, uint8_t command, void *buffer)
{
    DRESULT res = RES_OK;

    if (physicalDrive != MMCDISK)
    {
        return RES_PARERR;
    }

    switch (command)
    {
        case GET_SECTOR_COUNT:
            if (buffer)
            {
                *(uint32_t *)buffer = g_mmc.userPartitionBlocks;
            }
            else
            {
                res = RES_PARERR;
            }
            break;
        case GET_SECTOR_SIZE:
            if (buffer)
            {
                *(uint32_t *)buffer = g_mmc.blockSize;
            }
            else
            {
                res = RES_PARERR;
            }
            break;
        case GET_BLOCK_SIZE:
            if (buffer)
            {
                *(uint32_t *)buffer = g_mmc.eraseGroupBlocks;
            }
            else
            {
                res = RES_PARERR;
            }
            break;
        case CTRL_SYNC:
            res = RES_OK;
            break;
        default:
            res = RES_PARERR;
            break;
    }

    return res;
}
#endif

DSTATUS mmc_disk_status(uint8_t physicalDrive)
{
    if (physicalDrive != MMCDISK)
    {
        return STA_NOINIT;
    }

    return RES_OK;
}

/* User defined transfer function. */
static status_t sdhc_transfer_function(SDHC_Type *base, sdhc_transfer_t *content)
{
    return SDHC_TransferNonBlocking(BOARD_SDHC_BASEADDR, &g_sdhcHandle, g_sdhcAdmaTable, SDHC_ADMA_TABLE_WORDS,
                                    content);
}

/* SDHC interrupt handler. */
void SDHC_IRQHandler(void)
{
    SDHC_HandleIRQ(BOARD_SDHC_BASEADDR, &g_sdhcHandle);
}

DSTATUS mmc_disk_initialize(uint8_t physicalDrive)
{
    sdhc_config_t *sdhcConfig = &(g_mmc.sdhcConfig);
    sdhc_transfer_callback_t sdhcCallback = {0};

    if (physicalDrive != MMCDISK)
    {
        return STA_NOINIT;
    }

    memset(&g_mmc, 0U, sizeof(g_mmc));
    memset(&g_sdhcAdmaTable, 0U, sizeof(g_sdhcAdmaTable));
    memset(&g_sdhcHandle, 0U, sizeof(g_sdhcHandle));

/* Initializes SDHC. */
#if defined SDHC_CARD_DETECT_USING_GPIO
    sdhcConfig->cardDetectDat3 = false;
#else
    sdhcConfig->cardDetectDat3 = true;
#endif
    sdhcConfig->endianMode = SDHC_ENDIAN_MODE;
    sdhcConfig->dmaMode = SDHC_DMA_MODE;
    sdhcConfig->readWatermarkLevel = SDHC_READ_WATERMARK_LEVEL;
    sdhcConfig->writeWatermarkLevel = SDHC_WRITE_WATERMARK_LEVEL;
    SDHC_Init(BOARD_SDHC_BASEADDR, sdhcConfig);

    /* Sets callback in SDHC driver. */
    init_event_timer();
    sdhcCallback.EventCreate = event_create;
    sdhcCallback.EventWait = event_wait;
    sdhcCallback.EventNotify = event_notify;
    sdhcCallback.EventDelete = event_delete;
    /* Creates handle for SDHC driver */
    SDHC_CreateHandle(BOARD_SDHC_BASEADDR, &g_sdhcHandle, &sdhcCallback);

    /* Fills state in card driver. */
    g_mmc.sdhcBase = BOARD_SDHC_BASEADDR;
    g_mmc.sdhcSourceClock_Hz = CLOCK_GetFreq(BOARD_SDHC_CLKSRC);
    g_mmc.sdhcTransfer = sdhc_transfer_function;

    if (kStatus_Success != MMC_Init(&g_mmc))
    {
        MMC_DeInit(&g_mmc);
        SDHC_DeInit(BOARD_SDHC_BASEADDR);
        memset(&g_mmc, 0U, sizeof(g_mmc));
        memset(&g_sdhcAdmaTable, 0U, sizeof(g_sdhcAdmaTable));
        memset(&g_sdhcHandle, 0U, sizeof(g_sdhcHandle));
        return STA_NOINIT;
    }
    return RES_OK;
}
