/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this
 * list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice,
 * this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_sdhc.h"

/*******************************************************************************
 * Definitons
 ******************************************************************************/

/* Clock setting */
#define SDHC_MAX_DVS (16U)      /* Max SD clock divisor from base clock */
#define SDHC_INITIAL_DVS (1U)   /* Initial value of SD clock divisor */
#define SDHC_INITIAL_CLKFS (2U) /* Initial value of SD clock frequence selector */
#define SDHC_NEXT_DVS(x) ((x) += 1U)
#define SDHC_PREV_DVS(x) ((x) -= 1U)
#define SDHC_MAX_CLKFS (256U)
#define SDHC_NEXT_CLKFS(x) ((x) <<= 1U)
#define SDHC_PREV_CLKFS(x) ((x) >>= 1U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Gets the instance.
 *
 * @param base SDHC peripheral base address.
 * @return Instance number.
 */
static uint32_t getInstance(SDHC_Type *base);

/*!
 * @brief Gets clock name
 *
 * @param base SDHC peripheral base address.
 * @return Clock name.
 */
static clock_ip_name_t getClockName(SDHC_Type *base);

/*!
 * @brief Gets the IRQ number.
 *
 * @param base SDHC peripheral base address.
 * @return Irq number.
 */
static IRQn_Type getIRQNumber(SDHC_Type *base);

/*!
 * @brief Starts transfer according to current transfer state
 *
 * @param base SDHC peripheral base address.
 * @param command Command to be sent.
 * @param data Data to be transferred.
 */
static void SDHC_StartTransfer(SDHC_Type *base, sdhc_command_t *command, sdhc_data_t *data);

/*!
 * @brief Receives command response
 *
 * @param base SDHC peripheral base address.
 * @param command Command to be sent.
 */
static void SDHC_ReceiveCommandResponse(SDHC_Type *base, sdhc_command_t *command);

/*!
 * @brief Reads DATAPORT when buffer enable bit is set.
 *
 * @param base SDHC peripheral base address.
 * @param transferredWords The number of data words have been transferred last time transaction.
 * @return The number of total data words have been transferred after this time transaction.
 */
static uint32_t SDHC_ReadDataPort(SDHC_Type *base, sdhc_data_t *data, uint32_t transferredWords);

/*!
 * @brief Reads data by using DATAPORT polling way.
 *
 * @param base SDHC peripheral base address.
 * @param data Data to be transferred.
 * @retval kStatus_Fail Read DATAPORT failed.
 * @retval kStatus_Success Read DATAPORT successfully.
 */
static status_t SDHC_ReadByDataPortPolling(SDHC_Type *base, sdhc_data_t *data);

/*!
 * @brief Writes DATAPORT when buffer enable bit is set.
 *
 * @param base SDHC peripheral base address.
 * @param transferredWords The number of data words have been transferred last time.
 * @return The number of total data words have been transferred after this time transaction.
 */
static uint32_t SDHC_WriteDataPort(SDHC_Type *base, sdhc_data_t *data, uint32_t transferredWords);

/*!
 * @brief Writes data by using DATAPORT polling way.
 *
 * @param base SDHC peripheral base address.
 * @param data Data to be transferred.
 * @retval kStatus_Fail Write DATAPORT failed.
 * @retval kStatus_Success Write DATAPORT successfully.
 */
static status_t SDHC_WriteByDataPortPolling(SDHC_Type *base, sdhc_data_t *data);

/*!
 * @brief Sends command by using polling way.
 *
 * @param handle SDHC handle.
 * @retval kStatus_Fail Send command failed.
 * @retval kStatus_Success Send command successfully.
 */
static status_t SDHC_SendCommandPolling(sdhc_handle_t *handle);

/*!
 * @brief Sends command by using interrupt way.
 *
 * @param handle SDHC handle.
 * @retval kStatus_Timeout Send command timeout.
 * @retval kStatus_Fail Send command failed.
 * @retval kStatus_Success Send command successfully.
 */
static status_t SDHC_SendCommandIRQ(sdhc_handle_t *handle);

/*!
 * @brief Transfers data by DATAPORT and polling way.
 *
 * @param handle SDHC handle.
 * @retval kStatus_Fail Transfer data failed.
 * @retval kStatus_Success Transfer data successfully.
 */
static status_t SDHC_TransferByDataPortPolling(sdhc_handle_t *handle);

/*!
 * @brief Transfers data by using DATAPORT and interrupt way.
 *
 * @param handle SDHC handle.
 * @retval kStatus_Timeout Transfer data timeout.
 * @retval kStatus_Fail Transfer data failed.
 * @retval kStatus_Success Transfer data successfully.
 */
static status_t SDHC_TransferByDataPortIRQ(sdhc_handle_t *handle);

/*!
 * @brief Transfers data by ADMA2 and polling way.
 *
 * @param handle SDHC handle.
 * @retval kStatus_Fail Transfer data failed.
 * @retval kStatus_Success Transfer data successfully.
 */
static status_t SDHC_TransferByAdma2Polling(sdhc_handle_t *handle);

/*!
 * @brief Transfers data by using ADMA2 and interrupt way.
 *
 * @param handle SDHC handle.
 * @retval kStatus_Timeout Transfer data timeout.
 * @retval kStatus_Fail Transfer data failed.
 * @retval kStatus_Success Transfer data successfully.
 */
static status_t SDHC_TransferByAdma2IRQ(sdhc_handle_t *handle);

/*******************************************************************************
 * Variables
 ******************************************************************************/

sdhc_handle_t *g_sdhc[FSL_FEATURE_SOC_SDHC_COUNT];

/*******************************************************************************
 * Code
 ******************************************************************************/

static uint32_t getInstance(SDHC_Type *base)
{
    uint8_t instance;

    switch ((uint32_t)base)
    {
#if defined(SDHC)
        case (uint32_t)SDHC:
            instance = 0;
            break;
#endif
        default:
            break;
    }

    return instance;
}

static clock_ip_name_t getClockName(SDHC_Type *base)
{
    clock_ip_name_t gate;

    switch ((uint32_t)base)
    {
#if defined(SDHC)
        case (uint32_t)SDHC:
            gate = kCLOCK_Sdhc0;
            break;
#endif
        default:
            break;
    }

    return gate;
}

static IRQn_Type getIRQNumber(SDHC_Type *base)
{
    IRQn_Type sdhc_irqs[] = SDHC_IRQS;

    return sdhc_irqs[getInstance(base)];
}

void SDHC_Init(SDHC_Type *base, const sdhc_config_t *config)
{
    uint32_t proctl;
    uint32_t wml;

    assert(config);
#if !defined FSL_SDHC_ENABLE_ADMA1
    assert(config->dmaMode != kSDHC_DmaModeAdma1);
#endif /* FSL_SDHC_ENABLE_ADMA1 */

    /* Enables SDHC clock. */
    CLOCK_EnableClock(getClockName(base));

    /* Resets SDHC. */
    SDHC_Reset(base, kSDHC_ResetAll, 100);

    proctl = base->PROCTL;
    wml = base->WML;

    proctl &= ~(SDHC_PROCTL_D3CD_MASK | SDHC_PROCTL_EMODE_MASK | SDHC_PROCTL_DMAS_MASK);

    /* Sets DAT3 as card detection pin */
    if (config->cardDetectDat3)
    {
        proctl |= SDHC_PROCTL_D3CD_MASK;
    }

    /* Endian mode and DMA mode */
    proctl |= (SDHC_PROCTL_EMODE(config->endianMode) | SDHC_PROCTL_DMAS(config->dmaMode));

    /* Watermark level */
    wml &= ~(SDHC_WML_RDWML_MASK | SDHC_WML_WRWML_MASK);
    wml |= (SDHC_WML_RDWML(config->readWatermarkLevel) | SDHC_WML_WRWML(config->writeWatermarkLevel));

    base->WML = wml;
    base->PROCTL = proctl;
}

void SDHC_DeInit(SDHC_Type *base)
{
    switch ((uint32_t)base)
    {
#if defined(SDHC)
        case (uint32_t)SDHC:
            CLOCK_DisableClock(kCLOCK_Sdhc0);
            break;
#endif
        default:
            break;
    }
}

bool SDHC_Reset(SDHC_Type *base, uint32_t mask, uint32_t timeout)
{
    mask = (mask & (SDHC_SYSCTL_RSTA_MASK | SDHC_SYSCTL_RSTC_MASK | SDHC_SYSCTL_RSTD_MASK));

    base->SYSCTL |= mask;
    /* Delay some time to wait reset success. */
    while ((base->SYSCTL & mask))
    {
        if (!timeout)
        {
            break;
        }
        timeout--;
    }
    return ((!timeout) ? false : true);
}

void SDHC_GetCapability(SDHC_Type *base, sdhc_capability_t *capability)
{
    uint32_t htCapability;
    uint32_t hostVer;
    uint32_t maxBlockLength;

    assert(capability);

    hostVer = base->HOSTVER;
    htCapability = base->HTCAPBLT;

    /* Gets the capability of SDHC. */
    capability->specVersion = ((hostVer & SDHC_HOSTVER_SVN_MASK) >> SDHC_HOSTVER_SVN_SHIFT);
    capability->vendorVersion = ((hostVer & SDHC_HOSTVER_VVN_MASK) >> SDHC_HOSTVER_VVN_SHIFT);

    maxBlockLength = ((htCapability & SDHC_HTCAPBLT_MBL_MASK) >> SDHC_HTCAPBLT_MBL_SHIFT);
    capability->maxBlockLength = (512U << maxBlockLength);

    capability->flags = (htCapability & (kSDHC_SupportAdmaFlag | kSDHC_SupportHighSpeedFlag | kSDHC_SupportDmaFlag |
                                         kSDHC_SupportSuspendResumeFlag | kSDHC_SupportV330Flag));
#if defined FSL_FEATURE_SDHC_HAS_V300_SUPPORT && FSL_FEATURE_SDHC_HAS_V300_SUPPORT
    capability->flags |= (htCapability & kSDHC_SupportV300Flag);
#endif
#if defined FSL_FEATURE_SDHC_HAS_V180_SUPPORT && FSL_FEATURE_SDHC_HAS_V180_SUPPORT
    capability->flags |= (htCapability & kSDHC_SupportV180Flag);
#endif

    /* Other attributes not in HTCAPBLT register. */
    capability->maxBlockCount = SDHC_MAX_BLOCK_COUNT;
    /* eSDHC on all kinetis boards will support 4/8 bit data bus width. */
    capability->flags |= (kSDHC_Support4BitFlag | kSDHC_Support8BitFlag);
}

uint32_t SDHC_SetSdClock(SDHC_Type *base, uint32_t sourceFrequency, uint32_t sdClock)
{
    uint32_t divisor;
    uint32_t prescaler;
    uint32_t sysctl;
    uint32_t nearestFrequence = 0;

    assert(sdClock && (sdClock < sourceFrequency));

    divisor = SDHC_INITIAL_DVS;
    prescaler = SDHC_INITIAL_CLKFS;

    /* Disables SD clock. */
    base->SYSCTL &= ~SDHC_SYSCTL_SDCLKEN_MASK;

    if (sdClock > 0U)
    {
        while ((sourceFrequency / prescaler / SDHC_MAX_DVS > sdClock) && (prescaler < SDHC_MAX_CLKFS))
        {
            SDHC_NEXT_CLKFS(prescaler);
        }
        while ((sourceFrequency / prescaler / divisor > sdClock) && (divisor < SDHC_MAX_DVS))
        {
            SDHC_NEXT_DVS(divisor);
        }

        nearestFrequence = sourceFrequency / prescaler / divisor;
        SDHC_PREV_CLKFS(prescaler);
        SDHC_PREV_DVS(divisor);
        sysctl = base->SYSCTL;

        /* Sets the SD clock frequenceuency divisor, SD clock frequenceuency select, data timeout counter value. */
        sysctl &= ~(SDHC_SYSCTL_DVS_MASK | SDHC_SYSCTL_SDCLKFS_MASK | SDHC_SYSCTL_DTOCV_MASK);
        sysctl |= (SDHC_SYSCTL_DVS(divisor) | SDHC_SYSCTL_SDCLKFS(prescaler) | SDHC_SYSCTL_DTOCV(0xEU));

        base->SYSCTL = sysctl;

        /* Checks whether the SD clock is stable or not. */
        while (!(base->PRSSTAT & SDHC_PRSSTAT_SDSTB_MASK))
        {
        }

        /* Enables the SD clock. It should be disabled before changing the SD clock frequenceuency. */
        base->SYSCTL |= SDHC_SYSCTL_SDCLKEN_MASK;
    }

    return nearestFrequence;
}

bool SDHC_SetCardActive(SDHC_Type *base, uint32_t timeout)
{
    base->SYSCTL |= SDHC_SYSCTL_INITA_MASK;

    /* Delays some time to wait card become active state. */
    while (!(base->SYSCTL & SDHC_SYSCTL_INITA_MASK))
    {
        if (!timeout)
        {
            break;
        }
        timeout--;
    }
    return ((!timeout) ? false : true);
}

void SDHC_SetDataWidth(SDHC_Type *base, sdhc_data_width_t dataWidth)
{
    uint32_t proctl;

    proctl = base->PROCTL;

    proctl &= ~SDHC_PROCTL_DTW_MASK;
    proctl |= SDHC_PROCTL_DTW(dataWidth);

    base->PROCTL = proctl;
}

void SDHC_SetTransferConfig(SDHC_Type *base, const sdhc_transfer_config_t *config)
{
    uint32_t blkattr;

    assert(config);

    blkattr = base->BLKATTR;

    blkattr &= ~(SDHC_BLKATTR_BLKSIZE_MASK | SDHC_BLKATTR_BLKCNT_MASK);
    blkattr |= (SDHC_BLKATTR_BLKSIZE(config->dataBlockSize) | SDHC_BLKATTR_BLKCNT(config->dataBlockCount));

    base->BLKATTR = blkattr;

    base->CMDARG = config->commandArgument;

    base->XFERTYP = (((config->commandIndex << SDHC_XFERTYP_CMDINX_SHIFT) & SDHC_XFERTYP_CMDINX_MASK) |
                     (config->flags & (SDHC_XFERTYP_DMAEN_MASK | SDHC_XFERTYP_MSBSEL_MASK | SDHC_XFERTYP_DPSEL_MASK |
                                       SDHC_XFERTYP_CMDTYP_MASK | SDHC_XFERTYP_BCEN_MASK | SDHC_XFERTYP_CICEN_MASK |
                                       SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP_MASK | SDHC_XFERTYP_DTDSEL_MASK |
                                       SDHC_XFERTYP_AC12EN_MASK)));
}

void SDHC_EnableSdioControl(SDHC_Type *base, uint32_t mask, bool enable)
{
    uint32_t proctl = base->PROCTL;
    uint32_t vendor = base->VENDOR;

    if (enable)
    {
        if (mask & kSDHC_StopAtBlockGapFlag)
        {
            proctl |= SDHC_PROCTL_SABGREQ_MASK;
        }

        if (mask & kSDHC_ReadWaitControlFlag)
        {
            proctl |= SDHC_PROCTL_RWCTL_MASK;
        }

        if (mask & kSDHC_InterruptAtBlockGapFlag)
        {
            proctl |= SDHC_PROCTL_IABG_MASK;
        }

        if (mask & kSDHC_ExactBlockNumberReadFlag)
        {
            vendor |= SDHC_VENDOR_EXBLKNU_MASK;
        }
    }
    else
    {
        if (mask & kSDHC_StopAtBlockGapFlag)
        {
            proctl &= ~SDHC_PROCTL_SABGREQ_MASK;
        }

        if (mask & kSDHC_ReadWaitControlFlag)
        {
            proctl &= ~SDHC_PROCTL_RWCTL_MASK;
        }

        if (mask & kSDHC_InterruptAtBlockGapFlag)
        {
            proctl &= ~SDHC_PROCTL_IABG_MASK;
        }

        if (mask & kSDHC_ExactBlockNumberReadFlag)
        {
            vendor &= ~SDHC_VENDOR_EXBLKNU_MASK;
        }
    }

    base->PROCTL = proctl;
    base->VENDOR = vendor;
}

void SDHC_SetMmcBootConfig(SDHC_Type *base, const sdhc_boot_config_t *config)
{
    uint32_t mmcboot;

    assert(config);

    mmcboot = base->MMCBOOT;

    mmcboot |= (SDHC_MMCBOOT_DTOCVACK(config->ackTimeoutCount) | SDHC_MMCBOOT_BOOTMODE(config->bootMode) |
                SDHC_MMCBOOT_BOOTBLKCNT(config->blockCount));
    if (config->enableBootAck)
    {
        mmcboot |= SDHC_MMCBOOT_BOOTACK_MASK;
    }
    if (config->enableBoot)
    {
        mmcboot |= SDHC_MMCBOOT_BOOTEN_MASK;
    }
    if (config->enableAutoStopAtBlockGap)
    {
        mmcboot |= SDHC_MMCBOOT_AUTOSABGEN_MASK;
    }

    base->MMCBOOT = mmcboot;
}

#if defined FSL_SDHC_ENABLE_ADMA1
status_t SDHC_SetAdma1TableConfig(
    SDHC_Type *base, uint32_t *table, uint32_t tableWords, uint32_t *data, uint32_t dataBytes)
{
    uint32_t *startAddress;
    uint32_t entries;
    uint32_t i;
    sdhc_adma1_descriptor_t *adma1EntryAddress;

    assert(table);
    assert(tableWords);
    assert(data);
    assert(dataBytes);

    startAddress = data;

    entries = ((dataBytes / SDHC_ADMA1_DESCRIPTOR_MAX_LENGTH_PER_ENTRY) + 1U);
    /* ADMA1 needs two descritors to finish a transfer */
    entries <<= 1U;
    if (entries > (tableWords / sizeof(sdhc_adma1_descriptor_t)))
    {
        return kStatus_OutOfRange;
    }

    adma1EntryAddress = (sdhc_adma1_descriptor_t *)(table);

    for (i = 0U; i < entries; i += 2U)
    {
        /* Each descriptor for ADMA1 is 32-bit in length */
        if ((dataBytes - sizeof(uint32_t) * (startAddress - data)) <= SDHC_ADMA1_DESCRIPTOR_MAX_LENGTH_PER_ENTRY)
        {
            /* The last piece of data, setting end flag in descriptor */
            adma1EntryAddress[i] = ((uint32_t)(dataBytes - sizeof(uint32_t) * (startAddress - data))
                                    << SDHC_ADMA1_DESCRIPTOR_LENGTH_SHIFT);
            adma1EntryAddress[i] |= kSDHC_Adma1DescriptorTypeSetLength;
            adma1EntryAddress[i + 1U] = ((uint32_t)(startAddress) << SDHC_ADMA1_DESCRIPTOR_ADDRESS_SHIFT);
            adma1EntryAddress[i + 1U] |= (SDHC_ADMA1_DESCRIPTOR_TYPE_TRANSFER | SDHC_ADMA1_DESCRIPTOR_END_MASK);
        }
        else
        {
            adma1EntryAddress[i] =
                ((uint32_t)SDHC_ADMA1_DESCRIPTOR_MAX_LENGTH_PER_ENTRY << SDHC_ADMA1_DESCRIPTOR_LENGTH_SHIFT);
            adma1EntryAddress[i] |= kSDHC_Adma1DescriptorTypeSetLength;
            adma1EntryAddress[i + 1U] = ((uint32_t)(startAddress) << SDHC_ADMA1_DESCRIPTOR_ADDRESS_SHIFT);
            adma1EntryAddress[i + 1U] |= kSDHC_Adma1DescriptorTypeTransfer;
            startAddress += SDHC_ADMA1_DESCRIPTOR_MAX_LENGTH_PER_ENTRY / sizeof(uint32_t);
        }
    }

    /* When use ADMA, disables simple DMA */
    base->DSADDR = 0U;
    base->ADSADDR = (uint32_t)table;
    return kStatus_Success;
}
#endif /* FSL_SDHC_ENABLE_ADMA1 */

status_t SDHC_SetAdma2TableConfig(
    SDHC_Type *base, uint32_t *table, uint32_t tableWords, const uint32_t *data, uint32_t dataBytes)
{
    const uint32_t *startAddress;
    uint32_t entries;
    uint32_t i;
    sdhc_adma2_descriptor_t *adma2EntryAddress;

    assert(table);
    assert(tableWords);
    assert(data);
    assert(dataBytes);

    startAddress = data;

    entries = ((dataBytes / SDHC_ADMA2_DESCRIPTOR_MAX_LENGTH_PER_ENTRY) + 1U);
    if (entries > (tableWords / sizeof(sdhc_adma2_descriptor_t)))
    {
        return kStatus_OutOfRange;
    }

    adma2EntryAddress = (sdhc_adma2_descriptor_t *)(table);
    for (i = 0U; i < entries; i++)
    {
        /* Each descriptor for ADMA2 is 64-bit in length */
        if ((dataBytes - sizeof(uint32_t) * (startAddress - data)) <= SDHC_ADMA2_DESCRIPTOR_MAX_LENGTH_PER_ENTRY)
        {
            /* The last piece of data, setting end flag in descriptor */
            adma2EntryAddress[i].address = startAddress;
            adma2EntryAddress[i].attribute =
                ((SDHC_ADMA2_DESCRIPTOR_LENGTH_MASK & (dataBytes - sizeof(uint32_t) * (startAddress - data)))
                 << SDHC_ADMA2_DESCRIPTOR_LENGTH_SHIFT);
            adma2EntryAddress[i].attribute |= (kSDHC_Adma2DescriptorTypeTransfer | kSDHC_Adma2DescriptorEndFlag);
        }
        else
        {
            adma2EntryAddress[i].address = startAddress;
            adma2EntryAddress[i].attribute =
                ((SDHC_ADMA2_DESCRIPTOR_LENGTH_MASK & SDHC_ADMA2_DESCRIPTOR_MAX_LENGTH_PER_ENTRY)
                 << SDHC_ADMA2_DESCRIPTOR_LENGTH_SHIFT);
            adma2EntryAddress[i].attribute |= kSDHC_Adma2DescriptorTypeTransfer;
            startAddress += (SDHC_ADMA2_DESCRIPTOR_MAX_LENGTH_PER_ENTRY / sizeof(uint32_t));
        }
    }

    /* When use ADMA, disable simple DMA */
    base->DSADDR = 0U;
    base->ADSADDR = (uint32_t)table;
    return kStatus_Success;
}

static void SDHC_StartTransfer(SDHC_Type *base, sdhc_command_t *command, sdhc_data_t *data)
{
    uint32_t flags = 0U;
    sdhc_transfer_config_t sdhcTransferConfig;
    sdhc_dma_mode_t dmaMode;

    assert(command);

    /* Defines the flag corresponding to each response type. */
    switch (command->responseType)
    {
        case kSDHC_ResponseTypeNone:
            break;
        case kSDHC_ResponseTypeR1: /* Response 1 */
            flags |= (kSDHC_ResponseLength48Flag | kSDHC_EnableCrcCheckFlag | kSDHC_EnableIndexCheckFlag);
            break;
        case kSDHC_ResponseTypeR1b: /* Response 1 with busy */
            flags |= (kSDHC_ResponseLength48BusyFlag | kSDHC_EnableCrcCheckFlag | kSDHC_EnableIndexCheckFlag);
            break;
        case kSDHC_ResponseTypeR2: /* Response 2 */
            flags |= (kSDHC_ResponseLength136Flag | kSDHC_EnableCrcCheckFlag);
            break;
        case kSDHC_ResponseTypeR3: /* Response 3 */
            flags |= (kSDHC_ResponseLength48Flag);
            break;
        case kSDHC_ResponseTypeR4: /* Response 4 */
            flags |= (kSDHC_ResponseLength48Flag);
            break;
        case kSDHC_ResponseTypeR5: /* Response 5 */
            flags |= (kSDHC_ResponseLength48Flag | kSDHC_EnableCrcCheckFlag);
            break;
        case kSDHC_ResponseTypeR5b: /* Response 5 with busy */
            flags |= (kSDHC_ResponseLength48BusyFlag | kSDHC_EnableCrcCheckFlag | kSDHC_EnableIndexCheckFlag);
            break;
        case kSDHC_ResponseTypeR6: /* Response 6 */
            flags |= (kSDHC_ResponseLength48Flag | kSDHC_EnableCrcCheckFlag | kSDHC_EnableIndexCheckFlag);
            break;
        case kSDHC_ResponseTypeR7: /* Response 7 */
            flags |= (kSDHC_ResponseLength48Flag | kSDHC_EnableCrcCheckFlag | kSDHC_EnableIndexCheckFlag);
            break;
        default:
            break;
    }

    if (command->isAbortCommand)
    {
        flags |= kSDHC_CommandTypeAbortFlag;
    }

    if (data)
    {
        flags |= kSDHC_DataPresentFlag;

        dmaMode = (sdhc_dma_mode_t)((base->PROCTL & SDHC_PROCTL_DMAS_MASK) >> SDHC_PROCTL_DMAS_SHIFT);
        if (dmaMode != kSDHC_DmaModeNo)
        {
            flags |= kSDHC_EnableDmaFlag;
        }

        if (data->rxData)
        {
            flags |= kSDHC_DataReadFlag;
        }

        if (data->blockCount > 1U)
        {
            flags |= (kSDHC_MultipleBlockFlag | kSDHC_EnableBlockCountFlag);

            if (data->enableAutoCommand12)
            {
                /* Enables Auto command 12. */
                flags |= kSDHC_EnableAutoCommand12Flag;
            }
        }

        if (data->blockCount > SDHC_MAX_BLOCK_COUNT)
        {
            sdhcTransferConfig.dataBlockSize = data->blockSize;
            sdhcTransferConfig.dataBlockCount = SDHC_MAX_BLOCK_COUNT;

            flags &= ~(uint32_t)kSDHC_EnableBlockCountFlag;
        }
        else
        {
            sdhcTransferConfig.dataBlockSize = data->blockSize;
            sdhcTransferConfig.dataBlockCount = data->blockCount;
        }
    }
    else
    {
        sdhcTransferConfig.dataBlockSize = 0U;
        sdhcTransferConfig.dataBlockCount = 0U;
    }

    sdhcTransferConfig.commandArgument = command->argument;
    sdhcTransferConfig.commandIndex = command->index;
    sdhcTransferConfig.flags = flags;

    SDHC_SetTransferConfig(base, &sdhcTransferConfig);
}

static void SDHC_ReceiveCommandResponse(SDHC_Type *base, sdhc_command_t *command)
{
    uint32_t i;

    assert(command);

    if (command->responseType != kSDHC_ResponseTypeNone)
    {
        command->response[0U] = SDHC_GetCommandResponse(base, 0U);

        if (command->responseType == kSDHC_ResponseTypeR2)
        {
            command->response[1U] = SDHC_GetCommandResponse(base, 1U);
            command->response[2U] = SDHC_GetCommandResponse(base, 2U);
            command->response[3U] = SDHC_GetCommandResponse(base, 3U);

            i = 4U;
            /* R3-R2-R1-R0(lowest 8 bit is invalid bit) has the same format as R2 format in SD specification document
            after removed internal CRC7 and end bit. */
            do
            {
                command->response[i - 1U] <<= 8U;
                if (i > 1U)
                {
                    command->response[i - 1U] |= ((command->response[i - 2U] & 0xFF000000U) >> 24U);
                }
            } while (i--);
        }
    }
}

static uint32_t SDHC_ReadDataPort(SDHC_Type *base, sdhc_data_t *data, uint32_t transferredWords)
{
    uint32_t i;
    uint32_t totalWords;
    uint32_t wordsCanBeRead; /* The words can be got at this time. */
    uint32_t readWatermark = ((base->WML & SDHC_WML_RDWML_MASK) >> SDHC_WML_RDWML_SHIFT);

    assert(data);

    totalWords = ((data->blockCount * data->blockSize) / sizeof(uint32_t));

    /* If watermark level is equal or bigger than totalWords, transfers totalWords data. */
    if (readWatermark >= totalWords)
    {
        wordsCanBeRead = totalWords;
    }
    /* If watermark level is less than totalWords and left words to be sent is equal or bigger than readWatermark,
    transfers watermark level words. */
    else if ((readWatermark < totalWords) && ((totalWords - transferredWords) >= readWatermark))
    {
        wordsCanBeRead = readWatermark;
    }
    /* If watermark level is less than totalWords and left words to be sent is less than readWatermark, transfers left
    words. */
    else
    {
        wordsCanBeRead = (totalWords - transferredWords);
    }

    i = 0U;
    while (i < wordsCanBeRead)
    {
        data->rxData[transferredWords++] = SDHC_GetData(base);
        i++;
    }

    return transferredWords;
}

static status_t SDHC_ReadByDataPortPolling(SDHC_Type *base, sdhc_data_t *data)
{
    uint32_t totalWords;
    uint32_t transferredWords = 0U; /*!< Words transferred complete */

    assert(data);

    totalWords = ((data->blockCount * data->blockSize) / sizeof(uint32_t));

    while (transferredWords < totalWords)
    {
        while (!(SDHC_GetInterruptStatusFlags(base) & (kSDHC_DataErrorFlag | kSDHC_BufferReadReadyFlag)))
        {
        }

        if (SDHC_GetInterruptStatusFlags(base) & kSDHC_DataErrorFlag)
        {
            return kStatus_Fail;
        }

        transferredWords = SDHC_ReadDataPort(base, data, transferredWords);

        /* Clears buffer enable bit to trigger transfer. */
        SDHC_ClearInterruptStatusFlags(base, kSDHC_BufferReadReadyFlag);
    }

    return kStatus_Success;
}

static uint32_t SDHC_WriteDataPort(SDHC_Type *base, sdhc_data_t *data, uint32_t transferredWords)
{
    uint32_t i;
    uint32_t totalWords;
    uint32_t wordsCanBeWrote; /* Words can be wrote at this time. */
    uint32_t writeWatermark = ((base->WML & SDHC_WML_WRWML_MASK) >> SDHC_WML_WRWML_SHIFT);

    assert(data);

    totalWords = ((data->blockCount * data->blockSize) / sizeof(uint32_t));

    /* If watermark level is equal or bigger than totalWords, transfers totalWords data.*/
    if (writeWatermark >= totalWords)
    {
        wordsCanBeWrote = totalWords;
    }
    /* If watermark level is less than totalWords and left words to be sent is equal or bigger than watermark,
    transfers watermark level words. */
    else if ((writeWatermark < totalWords) && ((totalWords - transferredWords) >= writeWatermark))
    {
        wordsCanBeWrote = writeWatermark;
    }
    /* If watermark level is less than totalWords and left words to be sent is less than watermark, transfers left
    words. */
    else
    {
        wordsCanBeWrote = (totalWords - transferredWords);
    }

    i = 0U;
    while (i < wordsCanBeWrote)
    {
        SDHC_SetData(base, data->txData[transferredWords++]);
        i++;
    }

    return transferredWords;
}

static status_t SDHC_WriteByDataPortPolling(SDHC_Type *base, sdhc_data_t *data)
{
    uint32_t totalWords;
    uint32_t transferredWords = 0U; /*!< Words transferred */

    assert(data);

    totalWords = (data->blockCount * data->blockSize) / sizeof(uint32_t);

    while (transferredWords < totalWords)
    {
        while (!(SDHC_GetInterruptStatusFlags(base) & (kSDHC_DataErrorFlag | kSDHC_BufferWriteReadyFlag)))
        {
        }

        if (SDHC_GetInterruptStatusFlags(base) & kSDHC_DataErrorFlag)
        {
            return kStatus_Fail;
        }

        transferredWords = SDHC_WriteDataPort(base, data, transferredWords);

        /* Clears buffer enable bit to trigger transfer. */
        SDHC_ClearInterruptStatusFlags(base, kSDHC_BufferWriteReadyFlag);
    }

    /* Waits write data complete or data transfer error after the last writing operation. */
    while (!(SDHC_GetInterruptStatusFlags(base) & (kSDHC_DataCompleteFlag | kSDHC_DataErrorFlag)))
    {
    }

    if (SDHC_GetInterruptStatusFlags(base) & kSDHC_DataErrorFlag)
    {
        return kStatus_Fail;
    }

    return kStatus_Success;
}

static status_t SDHC_SendCommandPolling(sdhc_handle_t *handle)
{
    assert(handle->transfer.command);

    /* Waits command complete or SDHC encounters error. */
    while (!(SDHC_GetInterruptStatusFlags(handle->base) & kSDHC_CommandFlag))
    {
    }

    if (SDHC_GetInterruptStatusFlags(handle->base) & kSDHC_CommandErrorFlag)
    {
        return kStatus_Fail;
    }

    /* Command completes successfully. Receives response */
    SDHC_ReceiveCommandResponse(handle->base, handle->transfer.command);

    return kStatus_Success;
}

static status_t SDHC_SendCommandIRQ(sdhc_handle_t *handle)
{
    bool eventStatus;

    assert(handle->EventWait);
    assert(handle->EventDelete);

    eventStatus = handle->EventWait(kSDHC_ApplicationEventCommand, FSL_SDHC_TIMEOUT_COMMAND);

    handle->EventDelete(kSDHC_ApplicationEventCommand);

    if (eventStatus == false)
    {
        /* If waits command failed, no need to receive data. */
        if (handle->transfer.data)
        {
            handle->EventDelete(kSDHC_ApplicationEventData);
        }

        return kStatus_Timeout;
    }

    if (handle->interruptFlags & kSDHC_CommandErrorFlag)
    {
        return kStatus_Fail;
    }
    else
    {
        /* Receives response */
        SDHC_ReceiveCommandResponse(handle->base, handle->transfer.command);
    }

    return kStatus_Success;
}

static status_t SDHC_TransferByDataPortPolling(sdhc_handle_t *handle)
{
    assert(handle->transfer.data);

    if (handle->transfer.data->rxData)
    {
        return SDHC_ReadByDataPortPolling(handle->base, handle->transfer.data);
    }
    else
    {
        return SDHC_WriteByDataPortPolling(handle->base, handle->transfer.data);
    }
}

static status_t SDHC_TransferByDataPortIRQ(sdhc_handle_t *handle)
{
    bool eventStatus;

    assert(handle->EventWait);
    assert(handle->EventDelete);

    eventStatus = handle->EventWait(kSDHC_ApplicationEventData, FSL_SDHC_TIMEOUT_DATA);

    handle->EventDelete(kSDHC_ApplicationEventData);

    if (eventStatus == false)
    {
        return kStatus_Timeout;
    }

    if (handle->interruptFlags & kSDHC_DataErrorFlag)
    {
        return kStatus_Fail;
    }

    return kStatus_Success;
}

static status_t SDHC_TransferByAdma2Polling(sdhc_handle_t *handle)
{
    uint32_t dataFlags;

    dataFlags = (kSDHC_DataCompleteFlag | kSDHC_DmaErrorFlag);

    while (!(SDHC_GetInterruptStatusFlags(handle->base) & dataFlags))
    {
    }

    if (SDHC_GetInterruptStatusFlags(handle->base) & kSDHC_DmaErrorFlag)
    {
        return kStatus_Fail;
    }

    return kStatus_Success;
}

static status_t SDHC_TransferByAdma2IRQ(sdhc_handle_t *handle)
{
    bool eventStatus;

    assert(handle->EventWait);
    assert(handle->EventDelete);

    eventStatus = handle->EventWait(kSDHC_ApplicationEventData, FSL_SDHC_TIMEOUT_DATA);

    handle->EventDelete(kSDHC_ApplicationEventData);

    if (eventStatus == false)
    {
        return kStatus_Timeout;
    }

    if (handle->interruptFlags & kSDHC_DmaErrorFlag)
    {
        return kStatus_Fail;
    }

    return kStatus_Success;
}

#if defined FSL_SDHC_ENABLE_ADMA1
#define SDHC_TransferByAdma1Polling(handle) SDHC_TransferByAdma2Polling(handle)

#define SDHC_TransferByAdma1Interrupt(handle) SDHC_TransferByAdma2IRQ(handle);
#endif /* FSL_SDHC_ENABLE_ADMA1 */

static void SDHC_SetTransactionInterrupt(SDHC_Type *base, bool usingInterruptSignal)
{
    uint32_t interruptEnabled; /* The Interrupt status flags to be enabled */

    sdhc_dma_mode_t dmaMode = (sdhc_dma_mode_t)((base->PROCTL & SDHC_PROCTL_DMAS_MASK) >> SDHC_PROCTL_DMAS_SHIFT);
    bool cardDetectDat3 = (bool)(base->PROCTL & SDHC_PROCTL_D3CD_MASK);

    /* Disables all interrupts */
    SDHC_DisableInterruptStatus(base, (uint32_t)kSDHC_AllInterruptFlags);
    SDHC_DisableInterruptSignal(base, (uint32_t)kSDHC_AllInterruptFlags);
    NVIC_DisableIRQ(getIRQNumber(base));

    interruptEnabled =
        (kSDHC_CommandIndexErrorFlag | kSDHC_CommandCrcErrorFlag | kSDHC_CommandEndBitErrorFlag |
         kSDHC_CommandTimeoutFlag | kSDHC_CommandCompleteFlag | kSDHC_DataTimeoutFlag | kSDHC_DataCrcErrorFlag |
         kSDHC_DataEndBitErrorFlag | kSDHC_DataCompleteFlag | kSDHC_AutoCommand12ErrorFlag);

    if (cardDetectDat3)
    {
        interruptEnabled |= (kSDHC_CardInsertionFlag | kSDHC_CardRemovalFlag);
    }

    switch (dmaMode)
    {
        case kSDHC_DmaModeAdma1:
        case kSDHC_DmaModeAdma2:
            interruptEnabled |= (kSDHC_DmaErrorFlag | kSDHC_DmaCompleteFlag);
            break;
        case kSDHC_DmaModeNo:
            interruptEnabled |= (kSDHC_BufferReadReadyFlag | kSDHC_BufferWriteReadyFlag);
            break;
        default:
            break;
    }

    SDHC_EnableInterruptStatus(base, interruptEnabled);

    if (usingInterruptSignal)
    {
        SDHC_EnableInterruptSignal(base, interruptEnabled);
        NVIC_EnableIRQ(getIRQNumber(base));
    }
}

void SDHC_CreateHandle(SDHC_Type *base, sdhc_handle_t *handle, uint32_t *admaTable, uint32_t admaTableWords)
{
    assert(admaTable);
    assert(admaTableWords);

    /* Zero the handle. */
    memset(handle, 0, sizeof(*handle));

    /* Saves the handle in global variables to support the double weak mechanism. */
    g_sdhc[getInstance(base)] = handle;

    /* Saves ADMA descriptor table address and length. */
    handle->admaTable = admaTable;
    handle->admaTableWords = admaTableWords;
}

status_t SDHC_TransferBlocking(sdhc_handle_t *handle, sdhc_transfer_t *transfer)
{
    status_t error = kStatus_Success;
    sdhc_dma_mode_t dmaMode;
    uint32_t dataBytes;

    assert(transfer);

    handle->transfer = *transfer;

    /* DATA-PORT is 32-bit align, ADMA2 4 bytes align, ADMA1 is 4096 bytes align */
    if ((!(transfer->command)) || ((handle->transfer.data) && (handle->transfer.data->blockSize % 4U)))
    {
        return kStatus_InvalidArgument;
    }

    /* Disables interrupt signal. */
    SDHC_SetTransactionInterrupt(handle->base, false);

    dmaMode = (sdhc_dma_mode_t)((handle->base->PROCTL & SDHC_PROCTL_DMAS_MASK) >> SDHC_PROCTL_DMAS_SHIFT);
    if ((handle->transfer.data) && (dmaMode != kSDHC_DmaModeNo))
    {
        dataBytes = (handle->transfer.data->blockCount * handle->transfer.data->blockSize);
        switch (dmaMode)
        {
#if defined FSL_SDHC_ENABLE_ADMA1
            case kSDHC_DmaModeAdma1:
                error = SDHC_SetAdma1TableConfig(handle->base, handle->admaTable, handle->admaTableWords,
                                                 (handle->transfer.data->rxData ? handle->transfer.data->rxData : handle->transfer.data->txData), dataBytes);
                break;
#endif
            case kSDHC_DmaModeAdma2:
                error = SDHC_SetAdma2TableConfig(handle->base, handle->admaTable, handle->admaTableWords,
                                                (handle->transfer.data->rxData ? handle->transfer.data->rxData : handle->transfer.data->txData), dataBytes);
                break;
            default:
                break;
        }

        if (error != kStatus_Success)
        {
            return kStatus_SDHC_PrepareAdmaDescriptorFailed;
        }
    }

    SDHC_StartTransfer(handle->base, handle->transfer.command, handle->transfer.data);

    if (kStatus_Success != SDHC_SendCommandPolling(handle))
    {
        return kStatus_SDHC_SendCommandFailed;
    }

    /* Waits until command line is not busy. */
    while ((SDHC_GetPresentStatusFlags(handle->base) & kSDHC_CommandInhibitFlag))
    {
    }

    /* Receives data. */
    if (handle->transfer.data)
    {
        dmaMode = (sdhc_dma_mode_t)((handle->base->PROCTL & SDHC_PROCTL_DMAS_MASK) >> SDHC_PROCTL_DMAS_SHIFT);

        switch (dmaMode)
        {
            case kSDHC_DmaModeNo:
                error = SDHC_TransferByDataPortPolling(handle);
                break;
#if defined FSL_SDHC_ENABLE_ADMA1
            case kSDHC_DmaModeAdma1:
                error = SDHC_TransferByAdma1Polling(handle);
                break;
#endif /* FSL_SDHC_ENABLE_ADMA1 */
            case kSDHC_DmaModeAdma2:
                error = SDHC_TransferByAdma2Polling(handle);
                break;
            default:
                error = kStatus_InvalidArgument;
                break;
        }

        if (error != kStatus_Success)
        {
            return kStatus_SDHC_TransferDataFailed;
        }
    }

    /* If command's response type is R1b/R5b, the data line may still busy after receving the response even if command
    has no data. Waits until data line is not busy. */
    while ((SDHC_GetPresentStatusFlags(handle->base) & kSDHC_DataInhibitFlag))
    {
    }

    handle->interruptFlags |= SDHC_GetInterruptStatusFlags(handle->base);
    SDHC_ClearInterruptStatusFlags(handle->base, kSDHC_AllInterruptFlags);

    return kStatus_Success;
}

status_t SDHC_TransferNoneBlocking(sdhc_handle_t *handle, sdhc_transfer_t *transfer)
{
    sdhc_dma_mode_t dmaMode;
    status_t error = kStatus_Success;
    uint32_t dataBytes;

    assert(handle->EventCreate);
    assert(transfer);

    handle->transfer = *transfer;

    /* DATA-PORT is 32-bit align, ADMA2 4 bytes align, ADMA1 is 4096 bytes align */
    if ((!(transfer->command)) || ((handle->transfer.data) && (handle->transfer.data->blockSize % 4U)))
    {
        return kStatus_InvalidArgument;
    }

    /* Enables interrupt signal. */
    SDHC_SetTransactionInterrupt(handle->base, true);

    dmaMode = (sdhc_dma_mode_t)((handle->base->PROCTL & SDHC_PROCTL_DMAS_MASK) >> SDHC_PROCTL_DMAS_SHIFT);
    if ((handle->transfer.data) && (dmaMode != kSDHC_DmaModeNo))
    {
        dataBytes = (handle->transfer.data->blockCount * handle->transfer.data->blockSize);
        switch (dmaMode)
        {
#if defined FSL_SDHC_ENABLE_ADMA1
            case kSDHC_DmaModeAdma1:
                error = SDHC_SetAdma1TableConfig(handle->base, handle->admaTable, handle->admaTableWords,
                                                 (handle->transfer.data->rxData ? handle->transfer.data->rxData : handle->transfer.data->txData), dataBytes);
                break;
#endif
            case kSDHC_DmaModeAdma2:
                error = SDHC_SetAdma2TableConfig(handle->base, handle->admaTable, handle->admaTableWords,
                                                 (handle->transfer.data->rxData ? handle->transfer.data->rxData : handle->transfer.data->txData), dataBytes);
                break;
            default:
                break;
        }
        if (error != kStatus_Success)
        {
            return kStatus_SDHC_PrepareAdmaDescriptorFailed;
        }
    }
    
    if (dmaMode == kSDHC_DmaModeNo)
    {
        handle->transferredWords = 0U;
    }

    /* Creates event for command and data. */
    if (false == handle->EventCreate(kSDHC_ApplicationEventCommand))
    {
        return kStatus_SDHC_CreateEventFailed;
    }
    if (handle->transfer.data)
    {
        if (false == handle->EventCreate(kSDHC_ApplicationEventData))
        {
            return kStatus_SDHC_CreateEventFailed;
        }
    }

    handle->interruptFlags = 0U;

    SDHC_StartTransfer(handle->base, handle->transfer.command, handle->transfer.data);

    if (kStatus_Success != SDHC_SendCommandIRQ(handle))
    {
        return kStatus_SDHC_SendCommandFailed;
    }

    /* Waits until command line is not busy. */
    while ((SDHC_GetPresentStatusFlags(handle->base) & kSDHC_CommandInhibitFlag))
    {
    }

    if (handle->transfer.data)
    {
        switch (dmaMode)
        {
            case kSDHC_DmaModeNo:
                error = SDHC_TransferByDataPortIRQ(handle);
                break;
#if defined FSL_SDHC_ENABLE_ADMA1
            case kSDHC_DmaModeAdma1:
                error = SDHC_TransferByAdma1Interrupt(handle);
                break;
#endif /* FSL_SDHC_ENABLE_ADMA1 */
            case kSDHC_DmaModeAdma2:
                error = SDHC_TransferByAdma2IRQ(handle);
                break;
            default:
                error = kStatus_InvalidArgument;
                break;
        }

        if (error != kStatus_Success)
        {
            return kStatus_SDHC_TransferDataFailed;
        }
    }

    /* If command's response type is R1b/R5b, the data line may still busy after receving the response even if command
    has no data. Waits until data line is not busy. */
    while ((SDHC_GetPresentStatusFlags(handle->base) & kSDHC_DataInhibitFlag))
    {
    }

    return kStatus_Success;
}

status_t SDHC_DetectCardByDat3(SDHC_Type *base)
{
    if (!(SDHC_GetPresentStatusFlags(base) & kSDHC_CardInsertionFlag))
    {
        return kStatus_SDHC_NoCardInserted;
    }

    SDHC_SetCardActive(base, 100U);
    return kStatus_Success;
}

void SDHC_SetTransferCallback(sdhc_handle_t *handle, sdhc_callback_t *callback)
{
    handle->EventCreate = callback->EventCreate;
    handle->EventWait = callback->EventWait;
    handle->EventNotify = callback->EventNotify;
    handle->EventDelete = callback->EventDelete;
    
    handle->CallbackCardInterrupt = callback->CallbackCardInterrupt;
    handle->CallbackCardInserted = callback->CallbackCardInserted;
    handle->CallbackCardRemoved = callback->CallbackCardRemoved;
    handle->CallbackBlockGap = callback->CallbackBlockGap;
}

static void SDHC_HandleCardDetect(sdhc_handle_t *handle, uint32_t interruptFlags)
{
    assert(interruptFlags & kSDHC_CardDetectFlag);

    if (interruptFlags & kSDHC_CardInsertionFlag)
    {
        if (handle->CallbackCardInserted)
        {
            handle->CallbackCardInserted();
        }
    }
    else
    {
        if (handle->CallbackCardRemoved)
        {
            handle->CallbackCardRemoved();
        }
    }
}

static void SDHC_HandleCommand(sdhc_handle_t *handle, uint32_t interruptFlags)
{
    assert(handle->EventNotify);
    assert(interruptFlags & kSDHC_CommandFlag);

    handle->EventNotify(kSDHC_ApplicationEventCommand);
}

static void SDHC_HandleData(sdhc_handle_t *handle, uint32_t interruptFlags)
{
    sdhc_dma_mode_t dmaMode;

    assert(handle->EventNotify);
    assert(handle->transfer.data);
    assert(interruptFlags & kSDHC_DataFlag);

    if (interruptFlags & (kSDHC_DataErrorFlag | kSDHC_DmaErrorFlag))
    {
        handle->EventNotify(kSDHC_ApplicationEventData);
        return;
    }

    if (interruptFlags & kSDHC_BufferReadReadyFlag)
    {
        handle->transferredWords = SDHC_ReadDataPort(handle->base, handle->transfer.data, handle->transferredWords);
    }
    else if (interruptFlags & kSDHC_BufferWriteReadyFlag)
    {
        handle->transferredWords = SDHC_WriteDataPort(handle->base, handle->transfer.data, handle->transferredWords);
    }
    else if (interruptFlags & kSDHC_DataCompleteFlag)
    {
        handle->EventNotify(kSDHC_ApplicationEventData);
    }
    else if (interruptFlags & kSDHC_DmaCompleteFlag)
    {
        dmaMode = (sdhc_dma_mode_t)((handle->base->PROCTL & SDHC_PROCTL_DMAS_MASK) >> SDHC_PROCTL_DMAS_SHIFT);
        if (dmaMode != kSDHC_DmaModeNo)
        {
            return;
        }
    }
}

static void SDHC_HandleCardInterrupt(sdhc_handle_t *handle)
{
    if (handle->CallbackCardInterrupt)
    {
        handle->CallbackCardInterrupt();
    }
}

static void SDHC_HandleBlockGapEvent(sdhc_handle_t *handle)
{
    if (handle->CallbackBlockGap)
    {
        handle->CallbackBlockGap();
    }
}

void SDHC_HandleIRQ(sdhc_handle_t *handle)
{
    uint32_t interruptFlags;

    assert(handle);

    interruptFlags = SDHC_GetInterruptStatusFlags(handle->base);
    handle->interruptFlags = interruptFlags;

    if (interruptFlags & kSDHC_CardDetectFlag)
    {
        SDHC_HandleCardDetect(handle, (interruptFlags & kSDHC_CardDetectFlag));
    }
    if (interruptFlags & kSDHC_CommandFlag)
    {
        SDHC_HandleCommand(handle, (interruptFlags & kSDHC_CommandFlag));
    }
    if (interruptFlags & kSDHC_DataFlag)
    {
        SDHC_HandleData(handle, (interruptFlags & kSDHC_DataFlag));
    }
    if (interruptFlags & kSDHC_CardInterruptFlag)
    {
        SDHC_HandleCardInterrupt(handle);
    }
    if (interruptFlags & kSDHC_BlockGapEventFlag)
    {
        SDHC_HandleBlockGapEvent(handle);
    }

    SDHC_ClearInterruptStatusFlags(handle->base, interruptFlags);
}

