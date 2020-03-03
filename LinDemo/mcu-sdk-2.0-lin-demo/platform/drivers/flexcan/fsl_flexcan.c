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

#include "fsl_flexcan.h"

/*******************************************************************************
 * Definitons
 ******************************************************************************/

#define FLEXCAN_TIME_QUANTA_NUM (10)

/*! @brief FlexCAN Internal State. */
enum _flexcan_state
{
    KFLEXCAN_StateIdle     = 0x0, /*!< MB is in idle state.*/
    KFLEXCAN_StateRxData   = 0x1, /*!< MB is in receiving data state.*/
    KFLEXCAN_StateRxRemote = 0x2, /*!< MB is in receiving remote reply state.*/
    KFLEXCAN_StateTxData   = 0x3, /*!< MB is in transmit data state.*/
    KFLEXCAN_StateTxRemote = 0x4, /*!< MB is in transmit remote request state.*/
    KFLEXCAN_StateRxFifo   = 0x5, /*!< MB is in receiving data from fifo state.*/
};

/*! @brief FlexCAN message buffer CODE for Rx buffers */
enum _flexcan_mb_code_rx
{
    KFLEXCAN_RxMbInactive     = 0x0, /*!< MB is not active.*/
    KFLEXCAN_RxMbFull         = 0x2, /*!< MB is full.*/
    KFLEXCAN_RxMbEmpty        = 0x4, /*!< MB is active and empty.*/
    KFLEXCAN_RxMbOverrun      = 0x6, /*!< MB is overwritten into a full buffer.*/
    KFLEXCAN_RxMbBusy         = 0x8, /*!< FlexCAN is updating the contents of the MB.*/
                                     /*!  The CPU must not access the MB.*/
    KFLEXCAN_RxMbRanswer      = 0xA, /*!< A frame was configured to recognize a Remote Request Frame*/
                                     /*!  and transmit a Response Frame in return.*/
    KFLEXCAN_RxMbNotUsed      = 0xF  /*!< Not used*/
};

/*! @brief FlexCAN message buffer CODE FOR Tx buffers*/
enum _flexcan_mb_code_tx
{
    KFLEXCAN_TxMbInactive     = 0x8, /*!< MB is not active.*/
    KFLEXCAN_TxMbAbort        = 0x9, /*!< MB is aborted.*/
    KFLEXCAN_TxMbDataOrRemote = 0xC, /*!< MB is a TX Data Frame(when MB RTR = 0) or*/
                                     /*!< MB is a TX Remote Request Frame (when MB RTR = 1).*/
    KFLEXCAN_TxMbTanswer      = 0xE, /*!< MB is a TX Response Request Frame from.*/
                                     /*!  an incoming Remote Request Frame.*/
    KFLEXCAN_TxMbNotUsed      = 0xF  /*!< Not used*/
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Get the FlexCAN instance from peripheral base address.
 *
 * @param base FlexCAN peripheral base address.
 * @return FlexCAN instance.
 */
uint32_t FLEXCAN_GetInstance(CAN_Type *base);

/*!
 * @brief Enter FlexCAN Fraze Mode.
 *
 * This function makes the FlexCAN work under Fraze Mode.
 *
 * @param base FlexCAN peripheral base address.
 */
static void FLEXCAN_EnterFrazeMode(CAN_Type *base);

/*!
 * @brief Exit FlexCAN Fraze Mode.
 *
 * This function makes the FlexCAN leave Fraze Mode.
 *
 * @param base FlexCAN peripheral base address.
 */
static void FLEXCAN_ExitFrazeMode(CAN_Type *base);

#if defined(DEBUG)
/*!
 * @brief Check if Message Buffer is occupied by Rx FIFO.
 *
 * This function check if Message Buffer is occupied by Rx FIFO.
 *
 * @param base FlexCAN peripheral base address.
 * @param mbIdx The FlexCAN Message Buffer index.
 */
static bool FLEXCAN_IsMbOccupied(CAN_Type *base, uint8_t mbIdx);
#endif

/*!
 * @brief Check if Message Buffer interrupt is enabled.
 *
 * This function check if Message Buffer interrupt is enabled.
 *
 * @param base FlexCAN peripheral base address.
 * @param mbIdx The FlexCAN Message Buffer index.
 */
static bool FLEXCAN_IsMbIntEnabled(CAN_Type *base, uint8_t mbIdx);

/*!
 * @brief Set Baud Rate of FlexCAN.
 *
 * This function set the baud rate of FlexCAN.
 *
 * @param base FlexCAN peripheral base address.
 * @param baudRate_Bps Baud Rate in Bps.
 * @param sourceClock_Hz Source Clock in Hz.
 */
static void FLEXCAN_SetBaudRate(CAN_Type *base, uint32_t baudRate_Bps, uint32_t sourceClock_Hz);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Array of FlexCAN handle. */
static flexcan_handle_t *s_flexcanHandle[FSL_FEATURE_SOC_FLEXCAN_COUNT];

/* Array of FlexCAN peripheral base address. */
static CAN_Type *const s_flexcanBase[] = CAN_BASE_PTRS;

/* Array of FlexCAN Message Buffer IRQ number. */
static const IRQn_Type s_flexcanMbIRQ[] = CAN_ORed_Message_buffer_IRQS;

/* Array of FlexCAN clock name. */
const clock_ip_name_t s_flexcanClock[] = FLEXCAN_CLOCKS;

/*******************************************************************************
 * Code
 ******************************************************************************/

uint32_t FLEXCAN_GetInstance(CAN_Type *base)
{
    uint32_t instance = 0;

    while ((s_flexcanBase[instance] != base) && (instance < (sizeof(s_flexcanBase) / sizeof(s_flexcanBase[0]))))
    {
        instance++;
    }

    assert(instance < FSL_FEATURE_SOC_FLEXCAN_COUNT);

    return instance;
}

static void FLEXCAN_EnterFrazeMode(CAN_Type *base)
{
    /* Set Freeze, Halt bits. */
    base->MCR |= CAN_MCR_FRZ_MASK | CAN_MCR_HALT_MASK;

    /* Wait until the FlexCAN Module enter freeze mode. */
    while (!(base->MCR & CAN_MCR_FRZACK_MASK)) {};
}

static void FLEXCAN_ExitFrazeMode(CAN_Type *base)
{
    /* Clear Freeze, Halt bits. */
    base->MCR &= ~(CAN_MCR_FRZ_MASK | CAN_MCR_HALT_MASK);

    /* Wait until the FlexCAN Module exit freeze mode. */
    while (base->MCR & CAN_MCR_FRZACK_MASK) {};
}

#if defined(DEBUG)
static bool FLEXCAN_IsMbOccupied(CAN_Type *base, uint8_t mbIdx)
{
    uint8_t lastOccupiedMb;

    /* Is Rx FIFO enabled? */
    if (base->MCR & CAN_MCR_RFEN_MASK)
    {
        /* Get RFFN value. */
        lastOccupiedMb = ((base->CTRL2 & CAN_CTRL2_RFFN_MASK) >> CAN_CTRL2_RFFN_SHIFT);
        /* Calculate the number of last Message Buffer occupied by Rx FIFO. */
        lastOccupiedMb = ((lastOccupiedMb + 1) * 2) + 5;

        if (mbIdx <= lastOccupiedMb)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
#endif

static bool FLEXCAN_IsMbIntEnabled(CAN_Type *base, uint8_t mbIdx)
{
    /* Assertion. */
    assert(mbIdx < FSL_FEATURE_FLEXCAN_HAS_MESSAGE_BUFFER_MAX_NUMBER);

#if (FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER > 0)
    if (mbIdx < 32)
    {
#endif
        if (base->IMASK1 & ((uint32_t)(1 << mbIdx)))
        {
            return true;
        }
        else
        {
            return false;
        }
#if (FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER > 0)
    }
    else
    {
        if (base->IMASK2 & ((uint32_t)(1 << (mbIdx - 32))))
            return true;
        else
            return false;
    }
#endif
}

static void FLEXCAN_SetBaudRate(CAN_Type *base, uint32_t baudRate_Bps, uint32_t sourceClock_Hz)
{
    flexcan_timing_config_t timingConfig;
    uint32_t priDiv = baudRate_Bps * FLEXCAN_TIME_QUANTA_NUM;

    /* Assertion: Desired baud rate is too high. */
    assert(baudRate_Bps <= 1000000U);
    /* Assertion: Source clock should greater than baud rate * FLEXCAN_TIME_QUANTA_NUM. */
    assert(priDiv <= sourceClock_Hz);

    if (0 == priDiv)
    {
        priDiv = 1;
    }

    priDiv = (sourceClock_Hz / priDiv) - 1;

    /* Desired baud rate is too low. */
    if (priDiv > 0xFF)
    {
        priDiv = 0xFF;
    }

    /* FlexCAN timing setting formula:
     * FLEXCAN_TIME_QUANTA_NUM = 1 + (PSEG1 + 1) + (PSEG2 + 1) + (PROPSEG + 1);
     */
    timingConfig.preDivider = priDiv;
    timingConfig.phaseSeg1  = 3;
    timingConfig.phaseSeg2  = 2;
    timingConfig.propSeg    = 1;
    timingConfig.rJumpwidth = 1;

    /* Update actual timing characteristic. */
    FLEXCAN_SetTimingConfig(base, &timingConfig);
}

void FLEXCAN_Init(CAN_Type *base, const flexcan_config_t *config, uint32_t srcClock_Hz)
{
    bool moduleEnabled;
    uint32_t mcrTemp;

    /* Assertion. */
    assert(config);
    assert((config->maxMbNum > 0) && (config->maxMbNum <= FSL_FEATURE_FLEXCAN_HAS_MESSAGE_BUFFER_MAX_NUMBER));

    /* Enable FlexCAN clock. */
    CLOCK_EnableClock(s_flexcanClock[FLEXCAN_GetInstance(base)]);

    /* Reset to known status.*/
    FLEXCAN_Reset(base);

    /* Check if FlexCAN Module already Enabled before calling Module_Config. */
    if (base->MCR & CAN_MCR_MDIS_MASK)
    {
        moduleEnabled = false;
    }
    else
    {
        moduleEnabled = true;
        FLEXCAN_Enable(base, false);
    }

    /* Protocol-Engine clock source selection, This bit must be set
     * when FlexCAN Module in Disable Mode.
     */
    if (KFLEXCAN_ClkSrcOsc == config->clkSrc)
    {
        base->CTRL1 &= ~CAN_CTRL1_CLKSRC_MASK;
    }
    else
    {
        base->CTRL1 |= CAN_CTRL1_CLKSRC_MASK;
    }

    /* Enable FlexCAN Module before configuration. */
    FLEXCAN_Enable(base, true);

    /* Enter Fraze Mode. */
    FLEXCAN_EnterFrazeMode(base);

    /* Save current MCR value. */
    mcrTemp = base->MCR;

    /* Set the maximum number of Message Buffers */
    mcrTemp = (mcrTemp & ~CAN_MCR_MAXMB_MASK) | CAN_MCR_MAXMB(config->maxMbNum - 1);

    /* Enable Loop Back Mode? */
    if (config->enableLoopBack)
    {
        base->CTRL1 |= CAN_CTRL1_LPB_MASK;
    }
    else
    {
        base->CTRL1 &= ~CAN_CTRL1_LPB_MASK;
    }

    /* Enable Self Wake Up Mode? */
    if (config->enableSelfWakeup)
    {
        mcrTemp |= CAN_MCR_SLFWAK_MASK;
    }
    else
    {
        mcrTemp &= ~CAN_MCR_SLFWAK_MASK;
    }

    /* Enable Individual Rx Masking? */
    if (config->enableIndividMask)
    {
        mcrTemp |= CAN_MCR_IRMQ_MASK;
    }
    else
    {
        mcrTemp &= ~CAN_MCR_IRMQ_MASK;
    }

    /* Enable Individual Rx Masking? */
    if (config->enableIndividMask)
    {
        mcrTemp |= CAN_MCR_IRMQ_MASK;
    }
    else
    {
        mcrTemp &= ~CAN_MCR_IRMQ_MASK;
    }

#if (defined(FSL_FEATURE_FLEXCAN_HAS_DOZE_MODE_SUPPORT) && FSL_FEATURE_FLEXCAN_HAS_DOZE_MODE_SUPPORT)
    /* Enable Doze Mode? */
    if (config->enableDoze)
    {
        mcrTemp |= CAN_MCR_DOZE_MASK;
    }
    else
    {
        mcrTemp &= ~CAN_MCR_DOZE_MASK;
    }
#endif

    /* Save MCR Configuation. */
    base->MCR = mcrTemp;

    /* Baud Rate Configuration.*/
    FLEXCAN_SetBaudRate(base, config->baudRate, srcClock_Hz);

    if (!moduleEnabled)
    {
        /* Disable FlexCAN Module after Config finish. */
        FLEXCAN_Enable(base, false);
    }
}

void FLEXCAN_Deinit(CAN_Type *base)
{
    /* Reset all Register Contents. */
    FLEXCAN_Reset(base);

    /* Disable FlexCAN module. */
    FLEXCAN_Enable(base, false);

    /* Disable FlexCAN clock. */
    CLOCK_DisableClock(s_flexcanClock[FLEXCAN_GetInstance(base)]);
}

void FLEXCAN_GetDefaultConfig(flexcan_config_t *config)
{
    /* Assertion. */
    assert(config);

    /* Initialize FlexCAN Module config struct with default value. */
    config->clkSrc = KFLEXCAN_ClkSrcOsc;
    config->baudRate = 125000U;
    config->maxMbNum = 16;
    config->enableLoopBack = false;
    config->enableSelfWakeup = false;
    config->enableIndividMask = false;
#if (defined(FSL_FEATURE_FLEXCAN_HAS_DOZE_MODE_SUPPORT) && FSL_FEATURE_FLEXCAN_HAS_DOZE_MODE_SUPPORT)
    config->enableDoze = false;
#endif
}

void FLEXCAN_Reset(CAN_Type *base)
{
    uint8_t i;
    bool moduleEnabled;

    /* The module must should be first exit from low power
     * mode, and then soft reset can be applied.
     */
    if (base->MCR & CAN_MCR_MDIS_MASK)
    {
        moduleEnabled = false;
        FLEXCAN_Enable(base, true);
    }
    else
    {
        moduleEnabled = true;
    }
#if (FSL_FEATURE_FLEXCAN_HAS_DOZE_MODE_SUPPORT != 0)
    /* De-assert DOZE Enable Bit. */
    base->MCR &= ~CAN_MCR_DOZE_MASK;
#endif

    /* Wait until FlexCAN exit from any Low Power Mode. */
    while (base->MCR & CAN_MCR_LPMACK_MASK) {};

    /* Assert Soft Reset Signal. */
    base->MCR |= CAN_MCR_SOFTRST_MASK;
    /* Wait until FlexCAN reset completes. */
    while (base->MCR & CAN_MCR_SOFTRST_MASK) {};

    /* Reset MCR rigister. */
#if (defined(FSL_FEATURE_FLEXCAN_HAS_GLITCH_FILTER) && FSL_FEATURE_FLEXCAN_HAS_GLITCH_FILTER)
    base->MCR |= CAN_MCR_WRNEN_MASK  |
                 CAN_MCR_WAKSRC_MASK |
                 CAN_MCR_MAXMB(FSL_FEATURE_FLEXCAN_HAS_MESSAGE_BUFFER_MAX_NUMBER - 1);
#else
    base->MCR |= CAN_MCR_WRNEN_MASK  |
                 CAN_MCR_MAXMB(FSL_FEATURE_FLEXCAN_HAS_MESSAGE_BUFFER_MAX_NUMBER - 1);
#endif

    /* Reset CTRL1 and CTRL2 rigister. */
    base->CTRL1 = CAN_CTRL1_SMP_MASK;
    base->CTRL2 = CAN_CTRL2_TASD(0x16) |
                  CAN_CTRL2_RRS_MASK   |
                  CAN_CTRL2_EACEN_MASK;

    /* Clean all individual Rx Mask of Message Buffers. */
    for (i = 0; i < FSL_FEATURE_FLEXCAN_HAS_MESSAGE_BUFFER_MAX_NUMBER; i++)
    {
        base->RXIMR[i] = 0x3FFFFFFF;
    }

    /* Clean Global Mask of Message Buffers. */
    base->RXMGMASK = 0x3FFFFFFF;
    /* Clean Global Mask of Message Buffer 14. */
    base->RX14MASK = 0x3FFFFFFF;
    /* Clean Global Mask of Message Buffer 15. */
    base->RX15MASK = 0x3FFFFFFF;
    /* Clean Global Mask of Rx FIFO. */
    base->RXFGMASK = 0x3FFFFFFF;

    /* Clean all Message Buffer CS fields. */
    for (i = 0; i < FSL_FEATURE_FLEXCAN_HAS_MESSAGE_BUFFER_MAX_NUMBER; i++)
    {
        base->MB[i].CS = 0x0;
    }

    if (!moduleEnabled)
    {
        FLEXCAN_Enable(base, false);
    }
}

void FLEXCAN_SetTimingConfig(CAN_Type *base, const flexcan_timing_config_t *config)
{
    /* Assertion. */
    assert(config);

    /* Enter Fraze Mode. */
    FLEXCAN_EnterFrazeMode(base);

    /* Cleaning previous Timing Setting. */
    base->CTRL1 &= ~(CAN_CTRL1_PRESDIV_MASK |
                     CAN_CTRL1_RJW_MASK |
                     CAN_CTRL1_PSEG1_MASK |
                     CAN_CTRL1_PSEG2_MASK |
                     CAN_CTRL1_PROPSEG_MASK);

    /* Updating Timing Setting according to configuration structure. */
    base->CTRL1 |= (CAN_CTRL1_PRESDIV(config->preDivider) |
                    CAN_CTRL1_RJW(config->rJumpwidth) |
                    CAN_CTRL1_PSEG1(config->phaseSeg1) |
                    CAN_CTRL1_PSEG2(config->phaseSeg2) |
                    CAN_CTRL1_PROPSEG(config->propSeg));

    /* Exit Fraze Mode. */
    FLEXCAN_ExitFrazeMode(base);
}

void FlEXCAN_SetRxMbGlobalMask(CAN_Type *base, uint32_t mask)
{
    /* Enter Fraze Mode. */
    FLEXCAN_EnterFrazeMode(base);

    /* Setting Global Mask value. */
    base->RXMGMASK = mask;
    base->RX14MASK = mask;
    base->RX15MASK = mask;

    /* Exit Fraze Mode. */
    FLEXCAN_ExitFrazeMode(base);
}

void FlEXCAN_SetRxFifoGlobalMask(CAN_Type *base, uint32_t mask)
{
    /* Enter Fraze Mode. */
    FLEXCAN_EnterFrazeMode(base);

    /* Setting Rx FIFO Global Mask value. */
    base->RXFGMASK = mask;

    /* Exit Fraze Mode. */
    FLEXCAN_ExitFrazeMode(base);
}

void FlEXCAN_SetRxIndividualMask(CAN_Type *base, uint8_t maskIdx, uint32_t mask)
{
    assert(maskIdx <= (base->MCR & CAN_MCR_MAXMB_MASK));

    /* Enter Fraze Mode. */
    FLEXCAN_EnterFrazeMode(base);

    /* Setting Rx Individual Mask value. */
    base->RXIMR[maskIdx] = mask;

    /* Exit Fraze Mode. */
    FLEXCAN_ExitFrazeMode(base);
}

void FLEXCAN_SetTxMbConfig(CAN_Type *base, uint8_t mbIdx)
{
    /* Assertion. */
    assert(mbIdx <= (base->MCR & CAN_MCR_MAXMB_MASK));
    assert(!FLEXCAN_IsMbOccupied(base, mbIdx));

    /* Inactivate Message Buffer. */
    base->MB[mbIdx].CS = 0;

    /* Clean Message Buffer content. */
    base->MB[mbIdx].ID = 0x0;
    base->MB[mbIdx].WORD0 = 0x0;
    base->MB[mbIdx].WORD1 = 0x0;
}

void FLEXCAN_SetRxMbConfig(CAN_Type *base, uint8_t mbIdx, const flexcan_rx_mb_config_t *config)
{
    /* Assertion. */
    assert(mbIdx <= (base->MCR & CAN_MCR_MAXMB_MASK));
    assert(!FLEXCAN_IsMbOccupied(base, mbIdx));
    assert(config);

    uint32_t cs_temp = 0;

    /* Inactivate Message Buffer. */
    base->MB[mbIdx].CS = 0;

    /* Clean Message Buffer content. */
    base->MB[mbIdx].ID = 0x0;
    base->MB[mbIdx].WORD0 = 0x0;
    base->MB[mbIdx].WORD1 = 0x0;

    /* Setup Message Buffer ID. */
    base->MB[mbIdx].ID = config->id;

    /* Setup Message Buffer format. */
    if (config->format == KFLEXCAN_FrameFormatExtend)
    {
        cs_temp |= CAN_CS_IDE_MASK;
    }

    /* Setup Message Buffer type. */
    if (config->type == KFLEXCAN_FrameTypeRemote)
    {
        cs_temp |= CAN_CS_RTR_MASK;
    }

    /* Activate Rx Message Buffer. */
    cs_temp |= CAN_CS_CODE(KFLEXCAN_RxMbEmpty);
    base->MB[mbIdx].CS = cs_temp;
}

void FlEXCAN_SetRxFifoConfig(CAN_Type *base, const flexcan_rx_fifo_config_t *config)
{
    /* Assertion. */
    assert(config);
    assert(config->idFilterNum <= 128);

    volatile uint32_t *idFilterRegion = (volatile uint32_t *)(&base->MB[6].CS);
    uint8_t setup_mb, i, rffn = 0;

    /* Get the setup_mb value. */
    setup_mb = (base->MCR & CAN_MCR_MAXMB_MASK) >> CAN_MCR_MAXMB_SHIFT;
    setup_mb = (setup_mb < FSL_FEATURE_FLEXCAN_HAS_MESSAGE_BUFFER_MAX_NUMBER) ?
                setup_mb : FSL_FEATURE_FLEXCAN_HAS_MESSAGE_BUFFER_MAX_NUMBER;

    /* Enter Fraze Mode. */
    FLEXCAN_EnterFrazeMode(base);

    /* Determine RFFN value. */
    for (i = 0; i <= 0xF; i++)
    {
        if ((8 * (i + 1)) > config->idFilterNum)
        {
            rffn = i;
            assert(((setup_mb - 8) - (2 * rffn)) > 0);

            base->CTRL2 = (base->CTRL2 & ~CAN_CTRL2_RFFN_MASK) | CAN_CTRL2_RFFN(rffn);
            break;
        }
    }

    /* Clean ID filter table occuyied Message Buffer Region. */
    rffn = (rffn + 1) * 8;
    for (i = 0; i < rffn; i++)
    {
        idFilterRegion[i] = 0x0;
    }

    /* Disable unused Rx FIFO Filter. */
    for (i = config->idFilterNum; i < rffn; i++)
    {
        idFilterRegion[i] = 0xFFFFFFFF;
    }

    /* Copy ID filter table to Message Buffer Region. */
    for (i = 0; i < config->idFilterNum; i++)
    {
        idFilterRegion[i] = config->idFilterTable[i];
    }

    /* Setup ID Fitlter Type. */
    switch (config->idFilterType)
    {
        case KFLEXCAN_RxFifoFilterTypeA:
            base->MCR = (base->MCR & ~CAN_MCR_IDAM_MASK) | CAN_MCR_IDAM(0x0);
            break;
        case KFLEXCAN_RxFifoFilterTypeB:
            base->MCR = (base->MCR & ~CAN_MCR_IDAM_MASK) | CAN_MCR_IDAM(0x1);
            break;
        case KFLEXCAN_RxFifoFilterTypeC:
            base->MCR = (base->MCR & ~CAN_MCR_IDAM_MASK) | CAN_MCR_IDAM(0x2);
            break;
        case KFLEXCAN_RxFifoFilterTypeD:
            /* All frames rejected. */
            base->MCR = (base->MCR & ~CAN_MCR_IDAM_MASK) | CAN_MCR_IDAM(0x3);
            break;
        default:
            break;
    }

    /* Setting Message Reception Priority. */
    if (config->priority  == KFLEXCAN_RxFifoPrioHigh)
    {
        /* Matching starts from Rx FIFO and continues on Mailboxes. */
        base->CTRL2 &= ~CAN_CTRL2_MRP_MASK;
    }
    else
    {
        /* Matching starts from Mailboxes and continues on Rx FIFO. */
        base->CTRL2 |= CAN_CTRL2_MRP_MASK;
    }

    /* Enable Rx Message FIFO. */
    base->MCR |= CAN_MCR_RFEN_MASK;

    /* Exit Fraze Mode. */
    FLEXCAN_ExitFrazeMode(base);
}

#if (defined(FSL_FEATURE_FLEXCAN_HAS_RX_FIFO_DMA) && FSL_FEATURE_FLEXCAN_HAS_RX_FIFO_DMA)
void FLEXCAN_EnableRxFifoDMA(CAN_Type *base, bool enable)
{
    if (enable)
    {
        /* Enter Fraze Mode. */
        FLEXCAN_EnterFrazeMode(base);

        /* Enable FlexCAN DMA. */
        base->MCR |= CAN_MCR_DMA_MASK;

        /* Exit Fraze Mode. */
        FLEXCAN_ExitFrazeMode(base);
    }
    else
    {
        /* Enter Fraze Mode. */
        FLEXCAN_EnterFrazeMode(base);

        /* Disable FlexCAN DMA. */
        base->MCR &= ~CAN_MCR_DMA_MASK;

        /* Exit Fraze Mode. */
        FLEXCAN_ExitFrazeMode(base);
    }
}
#endif /* FSL_FEATURE_FLEXCAN_HAS_RX_FIFO_DMA */

status_t FLEXCAN_WriteTxMb(CAN_Type *base, uint8_t mbIdx, const flexcan_frame_t *txFrame)
{
    /* Assertion. */
    assert(mbIdx <= (base->MCR & CAN_MCR_MAXMB_MASK));
    assert(!FLEXCAN_IsMbOccupied(base, mbIdx));
    assert(txFrame);
    assert(txFrame->length <= 8);

    uint32_t cs_temp = 0;

    /* Check if Message Buffer is activated. */
    if (CAN_CS_CODE(KFLEXCAN_TxMbDataOrRemote) != (base->MB[mbIdx].CS & CAN_CS_CODE_MASK))
    {
        /* Inactive Tx Message Buffer. */
        base->MB[mbIdx].CS = (base->MB[mbIdx].CS & ~CAN_CS_CODE_MASK) | CAN_CS_CODE(KFLEXCAN_TxMbInactive);

        /* Fill Message ID field. */
        base->MB[mbIdx].ID = txFrame->id;

        /* Fill Message Format field. */
        if (KFLEXCAN_FrameFormatExtend == txFrame->format)
        {
            cs_temp |= CAN_CS_SRR_MASK | CAN_CS_IDE_MASK;
        }

        /* Fill Message Type field. */
        if (txFrame->type == KFLEXCAN_FrameTypeRemote)
        {
            cs_temp |= CAN_CS_RTR_MASK;
        }

        cs_temp |= CAN_CS_CODE(KFLEXCAN_TxMbDataOrRemote) |
                   CAN_CS_DLC(txFrame->length);

        /* Load Message Payload. */
        base->MB[mbIdx].WORD0 = txFrame->dataWord0;
        base->MB[mbIdx].WORD1 = txFrame->dataWord1;

        /* Activate Tx Message Buffer. */
        base->MB[mbIdx].CS = cs_temp;

        return kStatus_Success;
    }
    else
    {
        /* Tx Message Buffer is activated, return immediately. */
        return kStatus_Fail;
    }
}

status_t FLEXCAN_ReadRxMb(CAN_Type *base, uint8_t mbIdx, flexcan_frame_t *rxFrame)
{
    /* Assertion. */
    assert(mbIdx <= (base->MCR & CAN_MCR_MAXMB_MASK));
    assert(!FLEXCAN_IsMbOccupied(base, mbIdx));
    assert(rxFrame);

    volatile uint32_t timer;
    uint32_t cs_temp;
    uint8_t rx_code;

    /* Read CS field of Rx Message Buffer to lock Message Buffer. */
    cs_temp = base->MB[mbIdx].CS;
    /* Get Rx Message Buffer Code field. */
    rx_code = (cs_temp & CAN_CS_CODE_MASK) >> CAN_CS_CODE_SHIFT;

    /* Check to see if Rx Message Buffer is busy. */
    if ((rx_code == KFLEXCAN_RxMbFull) || (rx_code == KFLEXCAN_RxMbOverrun))
    {
        /* Store Message ID. */
        rxFrame->id = base->MB[mbIdx].ID & (CAN_ID_EXT_MASK | CAN_ID_STD_MASK);

        /* Get the message ID and format. */
        if (cs_temp & CAN_CS_IDE_MASK)
        {
            /* Solve Extend ID. */
            rxFrame->format = KFLEXCAN_FrameFormatExtend;
        }
        else
        {
            /* Solve Standard ID. */
            rxFrame->format = KFLEXCAN_FrameFormatStandard;
        }

        /* Get the message type. */
        if (cs_temp & CAN_CS_RTR_MASK)
        {
            rxFrame->type = KFLEXCAN_FrameTypeRemote;
        }
        else
        {
            rxFrame->type = KFLEXCAN_FrameTypeData;
        }

        /* Get the message length. */
        rxFrame->length = (cs_temp & CAN_CS_DLC_MASK) >> CAN_CS_DLC_SHIFT;

        /* Store Message Payload. */
        rxFrame->dataWord0 = base->MB[mbIdx].WORD0;
        rxFrame->dataWord1 = base->MB[mbIdx].WORD1;

        /* Read free-running timer to unlock Rx Message Buffer. */
        timer = base->TIMER;

        /* Set Rx Message Buffer to Empty. */
        base->MB[mbIdx].CS = (base->MB[mbIdx].CS & ~CAN_CS_CODE_MASK) | CAN_CS_CODE(KFLEXCAN_RxMbEmpty);

        if (rx_code == KFLEXCAN_RxMbFull)
        {
            return kStatus_Success;
        }
        else
        {
            return kStatus_FLEXCAN_RxOverflow;
        }
    }
    else
    {
        /* Read free-running timer to unlock Rx Message Buffer. */
        timer = base->TIMER;

        /* To avoid GCC Warning. */
        timer = timer;

        return kStatus_Fail;
    }
}

status_t FlEXCAN_ReadRxFifo(CAN_Type *base, flexcan_frame_t *rxFrame)
{
    /* Assertion. */
    assert(rxFrame);

    volatile uint32_t timer;
    uint32_t cs_temp;

    /* Check if Rx FIFO is Enabled. */
    if (base->MCR & CAN_MCR_RFEN_MASK)
    {
        /* Read CS field of Rx Message Buffer to lock Message Buffer. */
        cs_temp = base->MB[0].CS;

        /* Read data from Rx FIFO output port. */
        /* Store Message ID. */
        rxFrame->id = base->MB[0].ID & (CAN_ID_EXT_MASK | CAN_ID_STD_MASK);

        /* Get the message ID and format. */
        if (cs_temp & CAN_CS_IDE_MASK)
        {
            /* Solve Extend ID. */
            rxFrame->format = KFLEXCAN_FrameFormatExtend;
        }
        else
        {
            /* Solve Standard ID. */
            rxFrame->format = KFLEXCAN_FrameFormatStandard;
        }

        /* Get the message type. */
        if (cs_temp & CAN_CS_RTR_MASK)
        {
            rxFrame->type = KFLEXCAN_FrameTypeRemote;
        }
        else
        {
            rxFrame->type = KFLEXCAN_FrameTypeData;
        }

        /* Get the message length. */
        rxFrame->length = (cs_temp & CAN_CS_DLC_MASK) >> CAN_CS_DLC_SHIFT;

        /* Store Message Payload. */
        rxFrame->dataWord0 = base->MB[0].WORD0;
        rxFrame->dataWord1 = base->MB[0].WORD1;

        /* Read free-running timer to unlock Rx Message Buffer. */
        timer = base->TIMER;

        /* To avoid GCC Warning. */
        timer = timer;

        /* Store ID Filter Hit Index. */
        rxFrame->idhit = (uint8_t)(base->RXFIR & CAN_RXFIR_IDHIT_MASK);

        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

void FLEXCAN_CreateHandle(CAN_Type *base, flexcan_handle_t *handle, flexcan_transfer_callback_t callback, void *userData)
{
    assert(handle);

    uint8_t instance, i;

    /* Clean FlexCAN transfer handle. */
    memset(handle, 0, sizeof(flexcan_handle_t));

    /* Get instance from peripheral base address. */
    instance = FLEXCAN_GetInstance(base);

    /* Save the context in global variables to support the double weak mechanism. */
    s_flexcanHandle[instance] = handle;

    /* Register Callback function. */
    handle->callback = callback;
    handle->userData = userData;

    /* We Enable Error & Status interrupt here, because this interrupt just
     * report current status of FlexCAN module through Callback function.
     * It is insignificance without a available callback function.
     */
    if (handle->callback != NULL)
    {
        FLEXCAN_EnableInterrupts(base, KFLEXCAN_BusOffInterruptEnable    |
                                               KFLEXCAN_ErrorInterruptEnable     |
                                               KFLEXCAN_RxWarningInterruptEnable |
                                               KFLEXCAN_TxWarningInterruptEnable |
                                               KFLEXCAN_WakeUpInterruptEnable);
    }
    else
    {
        FLEXCAN_DisableInterrupts(base, KFLEXCAN_BusOffInterruptEnable    |
                                                KFLEXCAN_ErrorInterruptEnable     |
                                                KFLEXCAN_RxWarningInterruptEnable |
                                                KFLEXCAN_TxWarningInterruptEnable |
                                                KFLEXCAN_WakeUpInterruptEnable);
    }

    /* Enable interrupt in NVIC. */
    for (i = 0; i < FSL_FEATURE_FLEXCAN_INTERRUPT_COUNT; i++)
    {
        EnableIRQ((IRQn_Type)(s_flexcanMbIRQ[instance] + i));
    }
}

status_t FLEXCAN_SendNonBlocking(CAN_Type *base, flexcan_handle_t *handle, flexcan_mb_transfer_t *xfer)
{
    /* Assertion. */
    assert(handle);
    assert(xfer);
    assert(xfer->mbIdx <= (base->MCR & CAN_MCR_MAXMB_MASK));
    assert(!FLEXCAN_IsMbOccupied(base, xfer->mbIdx));

    /* Check if Message Buffer is idle. */
    if (KFLEXCAN_StateIdle == handle->mbState[xfer->mbIdx])
    {
        /* Distinguish transmit type. */
        if (KFLEXCAN_FrameTypeRemote == xfer->frame->type)
        {
            handle->mbState[xfer->mbIdx] = KFLEXCAN_StateTxRemote;

            /* Register user Frame buffer to receive remote Frame. */
            handle->mbFrameBuf[xfer->mbIdx] = xfer->frame;
        }
        else
        {
            handle->mbState[xfer->mbIdx] = KFLEXCAN_StateTxData;
        }

        if (kStatus_Success == FLEXCAN_WriteTxMb(base, xfer->mbIdx, xfer->frame))
        {
            /* Enable Message Buffer Interrupt. */
            FLEXCAN_EnableMbInterrupts(base, 1 << xfer->mbIdx);

            return kStatus_Success;
        }
        else
        {
            handle->mbState[xfer->mbIdx] = KFLEXCAN_StateIdle;
            return kStatus_Fail;
        }
    }
    else
    {
        return kStatus_FLEXCAN_TxBusy;
    }
}

status_t FLEXCAN_ReceiveNonBlocking(CAN_Type *base, flexcan_handle_t *handle, flexcan_mb_transfer_t *xfer)
{
    /* Assertion. */
    assert(handle);
    assert(xfer);
    assert(xfer->mbIdx <= (base->MCR & CAN_MCR_MAXMB_MASK));
    assert(!FLEXCAN_IsMbOccupied(base, xfer->mbIdx));

    /* Check if Message Buffer is idle. */
    if (KFLEXCAN_StateIdle == handle->mbState[xfer->mbIdx])
    {
        handle->mbState[xfer->mbIdx] = KFLEXCAN_StateRxData;

        /* Register Message Buffer. */
        handle->mbFrameBuf[xfer->mbIdx] = xfer->frame;

        /* Enable Message Buffer Interrupt. */
        FLEXCAN_EnableMbInterrupts(base, 1 << xfer->mbIdx);

        return kStatus_Success;
    }
    else
    {
        return kStatus_FLEXCAN_RxBusy;
    }
}

status_t FLEXCAN_ReceiveFifoNonBlocking(CAN_Type *base, flexcan_handle_t *handle, flexcan_fifo_transfer_t *xfer)
{
    /* Assertion. */
    assert(handle);
    assert(xfer);

    /* Check if Message Buffer is idle. */
    if (KFLEXCAN_StateIdle == handle->rxFifoState)
    {
        handle->rxFifoState = KFLEXCAN_StateRxFifo;

        /* Register Message Buffer. */
        handle->rxFifoFrameBuf = xfer->frame;

        /* Enable Message Buffer Interrupt. */
        FLEXCAN_EnableMbInterrupts(base, KFLEXCAN_RxFifoOverflowFlag |
                                         KFLEXCAN_RxFifoWarningFlag  |
                                         KFLEXCAN_RxFifoFrameAvlFlag);

        return kStatus_Success;
    }
    else
    {
        return kStatus_FLEXCAN_RxFifoBusy;
    }
}

void FLEXCAN_AbortSend(CAN_Type *base, flexcan_handle_t *handle, uint8_t mbIdx)
{
    /* Assertion. */
    assert(handle);
    assert(mbIdx <= (base->MCR & CAN_MCR_MAXMB_MASK));
    assert(!FLEXCAN_IsMbOccupied(base, mbIdx));

    /* Disable Message Buffer Interrupt. */
    FLEXCAN_DisableMbInterrupts(base, 1<< mbIdx);

    /* Un-register handle. */
    handle->mbFrameBuf[mbIdx] = 0x0;

    /* Clean Message Buffer. */
    FLEXCAN_SetTxMbConfig(base, mbIdx);

    handle->mbState[mbIdx] = KFLEXCAN_StateIdle;
}

void FLEXCAN_AbortReceive(CAN_Type *base, flexcan_handle_t *handle, uint8_t mbIdx)
{
    /* Assertion. */
    assert(handle);
    assert(mbIdx <= (base->MCR & CAN_MCR_MAXMB_MASK));
    assert(!FLEXCAN_IsMbOccupied(base, mbIdx));

    /* Disable Message Buffer Interrupt. */
    FLEXCAN_DisableMbInterrupts(base, 1 << mbIdx);

    /* Un-register handle. */
    handle->mbFrameBuf[mbIdx] = 0x0;
    handle->mbState[mbIdx] = KFLEXCAN_StateIdle;
}

void FLEXCAN_AbortReceiveFifo(CAN_Type *base, flexcan_handle_t *handle)
{
    /* Assertion. */
    assert(handle);

    /* Check if Rx FIFO is enabled. */
    if (base->MCR & CAN_MCR_RFEN_MASK)
    {
        /* Disable Rx Message FIFO Interrupts. */
        FLEXCAN_DisableMbInterrupts(base, KFLEXCAN_RxFifoOverflowFlag |
                                          KFLEXCAN_RxFifoWarningFlag  |
                                          KFLEXCAN_RxFifoFrameAvlFlag);

        /* Un-register handle. */
        handle->rxFifoFrameBuf = 0x0;
    }

    handle->rxFifoState = KFLEXCAN_StateIdle;
}

void FLEXCAN_HandleIRQ(CAN_Type *base, flexcan_handle_t *handle)
{
    /* Assertion. */
    assert(handle);

    status_t status = kStatus_FLEXCAN_UnHandled;
    uint32_t result;

    /* Store Current FlexCAN Module Error and Status. */
    result = base->ESR1;

    do
    {
        /* Solve FlexCAN Error and Status Interrupt. */
        if (result & (KFLEXCAN_TxWarningIntFlag | KFLEXCAN_RxWarningIntFlag | KFLEXCAN_BusOffIntFlag | KFLEXCAN_ErrorIntFlag | KFLEXCAN_WakeUpIntFlag))
        {
            status = kStatus_FLEXCAN_ErrorStatus;

            /* Clear FlexCAN Error and Status Interrupt. */
            FLEXCAN_ClearStatusFlags(base, KFLEXCAN_TxWarningIntFlag |
                                           KFLEXCAN_RxWarningIntFlag |
                                           KFLEXCAN_BusOffIntFlag    |
                                           KFLEXCAN_ErrorIntFlag     |
                                           KFLEXCAN_WakeUpIntFlag);
        }
        /* Solve FlexCAN Rx FIFO & Message Buffer Interrupt. */
        else
        {
            /* For this implementation, we solve the Message with lowest MB index first. */
            for (result = 0; result < FSL_FEATURE_FLEXCAN_HAS_MESSAGE_BUFFER_MAX_NUMBER; result++)
            {
                /* Get the lowest unhandled Message Buffer */
                if ((FLEXCAN_GetMbStatusFlags(base, 1 << result)) && (FLEXCAN_IsMbIntEnabled(base, result)))
                {
                    break;
                }
            }

            /* Does not find Message to deal with. */
            if (result == FSL_FEATURE_FLEXCAN_HAS_MESSAGE_BUFFER_MAX_NUMBER)
            {
                break;
            }

            /* Solve Rx FIFO interrupt. */
            if ((KFLEXCAN_StateIdle != handle->rxFifoState) && ((1 << result) <= KFLEXCAN_RxFifoOverflowFlag))
            {
                switch (1 << result)
                {
                    case KFLEXCAN_RxFifoOverflowFlag:
                        status = kStatus_FLEXCAN_RxFifoOverflow;
                        break;

                    case KFLEXCAN_RxFifoWarningFlag:
                        status = kStatus_FLEXCAN_RxFifoWarning;
                        break;

                    case KFLEXCAN_RxFifoFrameAvlFlag:
                        status = FlEXCAN_ReadRxFifo(base, handle->rxFifoFrameBuf);
                        if (kStatus_Success == status)
                        {
                            status = kStatus_FLEXCAN_RxFifoIdle;
                        }
                        FLEXCAN_AbortReceiveFifo(base, handle);
                        break;

                    default:
                        status = kStatus_FLEXCAN_UnHandled;
                        break;
                }
            }
            else
            {
                /* Get current State of Message Buffer. */
                switch (handle->mbState[result])
                {
                    /* Solve Rx Data Frame. */
                    case KFLEXCAN_StateRxData:
                        status = FLEXCAN_ReadRxMb(base, result, handle->mbFrameBuf[result]);
                        if (kStatus_Success == status)
                        {
                            status = kStatus_FLEXCAN_RxIdle;
                        }
                        FLEXCAN_AbortReceive(base, handle, result);
                        break;

                    /* Solve Rx Remote Frame. */
                    case KFLEXCAN_StateRxRemote:
                        status = FLEXCAN_ReadRxMb(base, result, handle->mbFrameBuf[result]);
                        if (kStatus_Success == status)
                        {
                            status = kStatus_FLEXCAN_RxIdle;
                        }
                        FLEXCAN_AbortReceive(base, handle, result);
                        break;

                    /* Solve Tx Data Frame. */
                    case KFLEXCAN_StateTxData:
                        status = kStatus_FLEXCAN_TxIdle;
                        FLEXCAN_AbortSend(base, handle, result);
                        break;

                    /* Solve Tx Remote Frame. */
                    case KFLEXCAN_StateTxRemote:
                        handle->mbState[result] = KFLEXCAN_StateRxRemote;
                        status = kStatus_FLEXCAN_TxSwitchToRx;
                        break;

                    default:
                        status = kStatus_FLEXCAN_UnHandled;
                        break;
                }
            }

            /* Clear resolved Message Buffer IRQ. */
            FLEXCAN_ClearMbStatusFlags(base, 1 << result);
        }

        /* Calling Callback Function if has one. */
        if (handle->callback != NULL)
        {
            handle->callback(base, handle, status, result, handle->userData);
        }

        /* Reset return status */
        status = kStatus_FLEXCAN_UnHandled;

        /* Store Current FlexCAN Module Error and Status. */
        result = base->ESR1;
    }
#if (defined(FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER)) && (FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER > 0)
    while ((0 != FLEXCAN_GetMbStatusFlags(base, 0xFFFFFFFFFFFFFFFF)) ||
           (0 != (result & (KFLEXCAN_TxWarningIntFlag |
                            KFLEXCAN_RxWarningIntFlag |
                            KFLEXCAN_BusOffIntFlag    |
                            KFLEXCAN_ErrorIntFlag     |
                            KFLEXCAN_WakeUpIntFlag))));
#else
    while ((0 != FLEXCAN_GetMbStatusFlags(base, 0xFFFFFFFF)) ||
           (0 != (result & (KFLEXCAN_TxWarningIntFlag |
                            KFLEXCAN_RxWarningIntFlag |
                            KFLEXCAN_BusOffIntFlag    |
                            KFLEXCAN_ErrorIntFlag     |
                            KFLEXCAN_WakeUpIntFlag))));
#endif
}

#if (FSL_FEATURE_SOC_FLEXCAN_COUNT > 0)
void CAN0_ORed_Message_buffer_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[0]);

    FLEXCAN_HandleIRQ(CAN0, s_flexcanHandle[0]);
}

void CAN0_Bus_Off_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[0]);

    FLEXCAN_HandleIRQ(CAN0, s_flexcanHandle[0]);
}

void CAN0_Error_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[0]);

    FLEXCAN_HandleIRQ(CAN0, s_flexcanHandle[0]);
}

void CAN0_Tx_Warning_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[0]);

    FLEXCAN_HandleIRQ(CAN0, s_flexcanHandle[0]);
}

void CAN0_Rx_Warning_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[0]);

    FLEXCAN_HandleIRQ(CAN0, s_flexcanHandle[0]);
}

void CAN0_Wake_Up_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[0]);

    FLEXCAN_HandleIRQ(CAN0, s_flexcanHandle[0]);
}
#endif

#if (FSL_FEATURE_SOC_FLEXCAN_COUNT > 1)
void CAN1_ORed_Message_buffer_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[1]);

    FLEXCAN_HandleIRQ(CAN1, s_flexcanHandle[1]);
}

void CAN1_Bus_Off_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[1]);

    FLEXCAN_HandleIRQ(CAN1, s_flexcanHandle[1]);
}

void CAN1_Error_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[1]);

    FLEXCAN_HandleIRQ(CAN1, s_flexcanHandle[1]);
}

void CAN1_Tx_Warning_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[1]);

    FLEXCAN_HandleIRQ(CAN1, s_flexcanHandle[1]);
}

void CAN1_Rx_Warning_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[1]);

    FLEXCAN_HandleIRQ(CAN1, s_flexcanHandle[1]);
}

void CAN1_Wake_Up_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[1]);

    FLEXCAN_HandleIRQ(CAN1, s_flexcanHandle[1]);
}
#endif

#if (FSL_FEATURE_SOC_FLEXCAN_COUNT > 2)
void CAN2_ORed_Message_buffer_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[2]);

    FLEXCAN_HandleIRQ(CAN2, s_flexcanHandle[2]);
}

void CAN2_Bus_Off_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[2]);

    FLEXCAN_HandleIRQ(CAN2, s_flexcanHandle[2]);
}

void CAN2_Error_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[2]);

    FLEXCAN_HandleIRQ(CAN2, s_flexcanHandle[2]);
}

void CAN2_Tx_Warning_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[2]);

    FLEXCAN_HandleIRQ(CAN2, s_flexcanHandle[2]);
}

void CAN2_Rx_Warning_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[2]);

    FLEXCAN_HandleIRQ(CAN2, s_flexcanHandle[2]);
}

void CAN2_Wake_Up_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[2]);

    FLEXCAN_HandleIRQ(CAN2, s_flexcanHandle[2]);
}
#endif

#if (FSL_FEATURE_SOC_FLEXCAN_COUNT > 3)
void CAN3_ORed_Message_buffer_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[3]);

    FLEXCAN_HandleIRQ(CAN3, s_flexcanHandle[3]);
}

void CAN3_Bus_Off_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[3]);

    FLEXCAN_HandleIRQ(CAN3, s_flexcanHandle[3]);
}

void CAN3_Error_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[3]);

    FLEXCAN_HandleIRQ(CAN3, s_flexcanHandle[3]);
}

void CAN3_Tx_Warning_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[3]);

    FLEXCAN_HandleIRQ(CAN3, s_flexcanHandle[3]);
}

void CAN3_Rx_Warning_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[3]);

    FLEXCAN_HandleIRQ(CAN3, s_flexcanHandle[3]);
}

void CAN3_Wake_Up_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[3]);

    FLEXCAN_HandleIRQ(CAN3, s_flexcanHandle[3]);
}
#endif

#if (FSL_FEATURE_SOC_FLEXCAN_COUNT > 4)
void CAN4_ORed_Message_buffer_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[4]);

    FLEXCAN_HandleIRQ(CAN4, s_flexcanHandle[4]);
}

void CAN4_Bus_Off_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[4]);

    FLEXCAN_HandleIRQ(CAN4, s_flexcanHandle[4]);
}

void CAN4_Error_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[4]);

    FLEXCAN_HandleIRQ(CAN4, s_flexcanHandle[4]);
}

void CAN4_Tx_Warning_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[4]);

    FLEXCAN_HandleIRQ(CAN4, s_flexcanHandle[4]);
}

void CAN4_Rx_Warning_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[4]);

    FLEXCAN_HandleIRQ(CAN4, s_flexcanHandle[4]);
}

void CAN4_Wake_Up_DriverIRQHandler(void)
{
    assert(s_flexcanHandle[4]);

    FLEXCAN_HandleIRQ(CAN4, s_flexcanHandle[4]);
}
#endif
