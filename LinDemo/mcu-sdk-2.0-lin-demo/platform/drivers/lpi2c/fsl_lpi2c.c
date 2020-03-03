/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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

#include "fsl_lpi2c.h"
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Common sets of flags used by the driver. */
enum _lpi2c_flag_constants
{
    /*! All flags which are cleared by the driver upon starting a transfer. */
    kMasterClearFlags = kLPI2C_MasterEndOfPacketFlag | kLPI2C_MasterStopDetectFlag | kLPI2C_MasterNackDetectFlag |
                        kLPI2C_MasterArbitrationLostFlag | kLPI2C_MasterFifoErrFlag | kLPI2C_MasterPinLowTimeoutFlag |
                        kLPI2C_MasterDataMatchFlag,

    /*! IRQ sources enabled by the non-blocking transactional API. */
    kMasterIrqFlags = kLPI2C_MasterArbitrationLostFlag | kLPI2C_MasterTxReadyFlag | kLPI2C_MasterRxReadyFlag |
                      kLPI2C_MasterStopDetectFlag | kLPI2C_MasterNackDetectFlag | kLPI2C_MasterPinLowTimeoutFlag |
                      kLPI2C_MasterFifoErrFlag,

    /*! Errors to check for. */
    kMasterErrorFlags = kLPI2C_MasterNackDetectFlag | kLPI2C_MasterArbitrationLostFlag | kLPI2C_MasterFifoErrFlag |
                        kLPI2C_MasterPinLowTimeoutFlag,

    /*! All flags which are cleared by the driver upon starting a transfer. */
    kSlaveClearFlags = kLPI2C_SlaveRepeatedStartDetectFlag | kLPI2C_SlaveStopDetectFlag | kLPI2C_SlaveBitErrFlag |
                       kLPI2C_SlaveFifoErrFlag,

    /*! IRQ sources enabled by the non-blocking transactional API. */
    kSlaveIrqFlags = kLPI2C_SlaveTxReadyFlag | kLPI2C_SlaveRxReadyFlag | kLPI2C_SlaveStopDetectFlag |
                     kLPI2C_SlaveRepeatedStartDetectFlag | kLPI2C_SlaveFifoErrFlag | kLPI2C_SlaveBitErrFlag |
                     kLPI2C_SlaveTransmitAckFlag | kLPI2C_SlaveAddressValidFlag,

    /*! Errors to check for. */
    kSlaveErrorFlags = kLPI2C_SlaveFifoErrFlag | kLPI2C_SlaveBitErrFlag,
};

/* ! @brief LPI2C master fifo commands. */
enum _lpi2c_master_fifo_cmd
{
    kTxDataCmd    = LPI2C_MTDR_CMD(0x0U),  /*!< Transmit DATA[7:0] */
    kRxDataCmd    = LPI2C_MTDR_CMD(0X1U),  /*!< Receive (DATA[7:0] + 1) bytes */
    kStopCmd      = LPI2C_MTDR_CMD(0x2U),  /*!< Generate STOP condition */
    kStartCmd     = LPI2C_MTDR_CMD(0x4U),  /*!< Generate(repeated) START and transmit address in DATA[[7:0] */
};

/*!
 * @brief Default watermark values.
 *
 * The default watermarks are set to zero.
 */
enum _lpi2c_default_watermarks
{
    kDefaultTxWatermark = 0,
    kDefaultRxWatermark = 0,
};

/*! @brief States for the state machine used by transactional APIs. */
enum _lpi2c_transfer_states
{
    kIdleState = 0,
    kSendCommandState,
    kIssueReadCommandState,
    kTransferDataState,
    kStopState,
    kWaitForCompletionState,
};

/*! @brief Typedef for interrupt handler. */
typedef void (*lpi2c_isr_t)(LPI2C_Type *base, void *handle);

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static uint32_t LPI2C_GetInstance(LPI2C_Type *base);

static uint32_t LPI2C_GetCyclesForWidth(uint32_t sourceClock_Hz,
                                        uint32_t width_ns,
                                        uint32_t maxCycles,
                                        uint32_t prescaler);

static status_t LPI2C_MasterCheckAndClearError(LPI2C_Type *base, uint32_t status);

static status_t LPI2C_MasterWaitForTxReady(LPI2C_Type *base);

static status_t LPI2C_CheckForBusyBus(LPI2C_Type *base);

static status_t LPI2C_RunTransferStateMachine(lpi2c_master_handle_t *handle, bool *isDone);

static void LPI2C_InitTransferStateMachine(lpi2c_master_handle_t *handle);

static status_t LPI2C_SlaveCheckAndClearError(LPI2C_Type *base, uint32_t flags);

static void LPI2C_CommonIRQHandler(LPI2C_Type *base, uint32_t instance);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Array to map LPI2C instance number to base pointer. */
static LPI2C_Type *const kLpi2cBases[] = LPI2C_BASE_PTRS;

/*! @brief Array to map LPI2C instance number to IRQ number. */
static IRQn_Type const kLpi2cIrqs[] = LPI2C_IRQS;

/*! @brief Array to map LPI2C instance number to clock gate enum. */
static clock_ip_name_t const kLpi2cClocks[] = LPI2C_CLOCKS;

/*! @brief Pointer to master IRQ handler for each instance. */
static lpi2c_isr_t s_lpi2cMasterIsr;

/*! @brief Pointers to master handles for each instance. */
static lpi2c_master_handle_t *s_lpi2cMasterHandle[FSL_FEATURE_SOC_LPI2C_COUNT];

/*! @brief Pointer to slave IRQ handler for each instance. */
static lpi2c_isr_t s_lpi2cSlaveIsr;

/*! @brief Pointers to slave handles for each instance. */
static lpi2c_slave_handle_t *s_lpi2cSlaveHandle[FSL_FEATURE_SOC_LPI2C_COUNT];

/*******************************************************************************
 * Code
 ******************************************************************************/

uint32_t LPI2C_GetInstance(LPI2C_Type *base)
{
    uint32_t instance;
    for (instance = 0; instance < FSL_FEATURE_SOC_LPI2C_COUNT; ++instance)
    {
        if (kLpi2cBases[instance] == base)
        {
            return instance;
        }
    }

    assert(false);
    return 0;
}

uint32_t LPI2C_GetCyclesForWidth(uint32_t sourceClock_Hz, uint32_t width_ns, uint32_t maxCycles, uint32_t prescaler)
{
    uint32_t busCycle_ns = 1000000 / (sourceClock_Hz / prescaler / 1000);
    uint32_t cycles      = 0;
    uint32_t value;

    /* Search for the cycle count just below the desired glitch width. */
    while ((((cycles + 1) * busCycle_ns) < width_ns) && (cycles + 1 < maxCycles))
    {
        ++cycles;
    }

    /*
     * If we end up with zero cycles, then set the filter to a single cycle unless the
     * bus clock is greater than 10x the desired glitch width.
     */
    if ((cycles == 0) && (busCycle_ns <= (width_ns * 10)))
    {
        cycles = 1;
    }

    return cycles;
}

void LPI2C_MasterGetDefaultConfig(lpi2c_master_config_t *masterConfig)
{
    masterConfig->enableMaster            = true;
    masterConfig->debugEnable             = false;
    masterConfig->ignoreAck               = false;
    masterConfig->pinConfig               = kLPI2C_2PinOpenDrain;
    masterConfig->baudRate_Hz             = 100000U;
    masterConfig->busIdleTimeout_ns       = 0;
    masterConfig->pinLowTimeout_ns        = 0;
    masterConfig->sdaGlitchFilterWidth_ns = 0;
    masterConfig->sclGlitchFilterWidth_ns = 0;
    masterConfig->hostRequest.enable      = false;
    masterConfig->hostRequest.source      = kLPI2C_HostRequestExternalPin;
    masterConfig->hostRequest.polarity    = kLPI2C_HostRequestPinActiveHigh;
}

void LPI2C_MasterInit(LPI2C_Type *base, const lpi2c_master_config_t *masterConfig, uint32_t sourceClock_Hz)
{
    uint32_t prescaler;
    uint32_t cycles;
    uint32_t cfgr2;
    uint32_t value;

    /* Ungate the clock. */
    CLOCK_EnableClock(kLpi2cClocks[LPI2C_GetInstance(base)]);

    /* Reset peripheral before configuring it. */
    LPI2C_MasterReset(base);

    base->MCR = LPI2C_MCR_DBGEN(masterConfig->debugEnable) | LPI2C_MCR_DOZEN(masterConfig->enableDoze);

    /* host request */
    value = base->MCFGR0;
    value &= (~(LPI2C_MCFGR0_HREN_MASK | LPI2C_MCFGR0_HRPOL_MASK | LPI2C_MCFGR0_HRSEL_MASK));
    value |= LPI2C_MCFGR0_HREN(masterConfig->hostRequest.enable) |
             LPI2C_MCFGR0_HRPOL(masterConfig->hostRequest.polarity) |
             LPI2C_MCFGR0_HRSEL(masterConfig->hostRequest.source);
    base->MCFGR0 = value;

    /* pin config and ignore ack */
    value = base->MCFGR1;
    value &= ~(LPI2C_MCFGR1_PINCFG_MASK | LPI2C_MCFGR1_IGNACK_MASK);
    value |= LPI2C_MCFGR1_PINCFG(masterConfig->pinConfig);
    value |= LPI2C_MCFGR1_IGNACK(masterConfig->ignoreAck);
    base->MCFGR1 = value;

    LPI2C_MasterSetWatermarks(base, kDefaultTxWatermark, kDefaultRxWatermark);

    LPI2C_MasterSetBaudRate(base, sourceClock_Hz, masterConfig->baudRate_Hz);

    /* Configure glitch filters and bus idle and pin low timeouts. */
    prescaler = (base->MCFGR1 & LPI2C_MCFGR1_PRESCALE_MASK) >> LPI2C_MCFGR1_PRESCALE_SHIFT;
    cfgr2     = base->MCFGR2;
    if (masterConfig->busIdleTimeout_ns)
    {
        cycles = LPI2C_GetCyclesForWidth(sourceClock_Hz, masterConfig->busIdleTimeout_ns,
                                         (LPI2C_MCFGR2_BUSIDLE_MASK >> LPI2C_MCFGR2_BUSIDLE_SHIFT), prescaler);
        cfgr2 &= ~LPI2C_MCFGR2_BUSIDLE_MASK;
        cfgr2 |= LPI2C_MCFGR2_BUSIDLE(cycles);
    }
    if (masterConfig->sdaGlitchFilterWidth_ns)
    {
        cycles = LPI2C_GetCyclesForWidth(sourceClock_Hz, masterConfig->sdaGlitchFilterWidth_ns,
                                         (LPI2C_MCFGR2_FILTSDA_MASK >> LPI2C_MCFGR2_FILTSDA_SHIFT), 1);
        cfgr2 &= ~LPI2C_MCFGR2_FILTSDA_MASK;
        cfgr2 |= LPI2C_MCFGR2_FILTSDA(cycles);
    }
    if (masterConfig->sclGlitchFilterWidth_ns)
    {
        cycles = LPI2C_GetCyclesForWidth(sourceClock_Hz, masterConfig->sclGlitchFilterWidth_ns,
                                         (LPI2C_MCFGR2_FILTSCL_MASK >> LPI2C_MCFGR2_FILTSCL_SHIFT), 1);
        cfgr2 &= ~LPI2C_MCFGR2_FILTSCL_MASK;
        cfgr2 |= LPI2C_MCFGR2_FILTSCL(cycles);
    }
    base->MCFGR2 = cfgr2;
    if (masterConfig->pinLowTimeout_ns)
    {
        cycles = LPI2C_GetCyclesForWidth(sourceClock_Hz, masterConfig->pinLowTimeout_ns / 256,
                                         (LPI2C_MCFGR2_BUSIDLE_MASK >> LPI2C_MCFGR2_BUSIDLE_SHIFT), prescaler);
        base->MCFGR3 = (base->MCFGR3 & ~LPI2C_MCFGR3_PINLOW_MASK) | LPI2C_MCFGR3_PINLOW(cycles);
    }

    LPI2C_MasterSetEnable(base, masterConfig->enableMaster);
}

void LPI2C_MasterDeinit(LPI2C_Type *base)
{
    /* Restore to reset state. */
    LPI2C_MasterReset(base);

    /* Gate clock. */
    CLOCK_DisableClock(kLpi2cClocks[LPI2C_GetInstance(base)]);
}

void LPI2C_MasterConfigureDataMatch(LPI2C_Type *base, const lpi2c_data_match_config_t *config)
{
    /* Disable master mode. */
    bool wasEnabled = (base->MCR & LPI2C_MCR_MEN_MASK) >> LPI2C_MCR_MEN_SHIFT;
    LPI2C_MasterSetEnable(base, false);

    base->MCFGR1 = (base->MCFGR1 & ~LPI2C_MCFGR1_MATCFG_MASK) | LPI2C_MCFGR1_MATCFG(config->matchMode);
    base->MCFGR0 = (base->MCFGR0 & ~LPI2C_MCFGR0_RDMO_MASK) | LPI2C_MCFGR0_RDMO(config->rxDataMatchOnly);
    base->MDMR   = LPI2C_MDMR_MATCH0(config->match0) | LPI2C_MDMR_MATCH1(config->match1);

    /* Restore master mode. */
    if (wasEnabled)
    {
        LPI2C_MasterSetEnable(base, true);
    }
}

void LPI2C_MasterSetBaudRate(LPI2C_Type *base, uint32_t sourceClock_Hz, uint32_t baudRate_Hz)
{
    uint32_t prescale   = 0;
    uint32_t bestPre    = 0;
    uint32_t bestclkHi  = 0;
    uint32_t bestError  = 0xffffffffu;
    uint32_t clockInKHz = sourceClock_Hz / 1000u;
    uint32_t kbps       = baudRate_Hz / 1000u;
    uint32_t value;
    int i;
    bool wasEnabled;

    /* Disable master mode. */
    wasEnabled = (base->MCR & LPI2C_MCR_MEN_MASK) >> LPI2C_MCR_MEN_SHIFT;
    LPI2C_MasterSetEnable(base, false);

    /*
     * Baud rate = (sourceClockInHZ/2^prescale)/(CLKLO+1+CLKHI+1)
     * Assume CLKLO = 2*CLKHI, SETHOLD = CLKHI, DATAVD = CLKHI/2.
     */
    for (prescale = 1; (prescale <= 128) && (bestError != 0); prescale = 2 * prescale)
    {
        uint32_t clkHiCycle;
        for (clkHiCycle = 1; clkHiCycle < 32; clkHiCycle++)
        {
            uint32_t computedRate = (clockInKHz / prescale) / (3 * clkHiCycle + 2);
            uint32_t absError     = kbps > computedRate ? kbps - computedRate : computedRate - kbps;
            if (absError < bestError)
            {
                bestPre   = prescale;
                bestclkHi = clkHiCycle;
                bestError = absError;

                /*
                 * If the error is 0, then we can stop searching
                 * because we won't find a better match.
                 */
                if (absError == 0)
                {
                    break;
                }
            }
        }
    }

    /* Standard, fast, fast mode plus and ultra-fast transfers. */
    value = LPI2C_MCCR0_CLKHI(bestclkHi);
    if (bestclkHi < 2)
    {
        value |= LPI2C_MCCR0_CLKLO(3) | LPI2C_MCCR0_SETHOLD(2) | LPI2C_MCCR0_DATAVD(1);
    }
    else
    {
        value |= LPI2C_MCCR0_CLKLO(2 * bestclkHi) | LPI2C_MCCR0_SETHOLD(bestclkHi) | LPI2C_MCCR0_DATAVD(bestclkHi / 2);
    }
    base->MCCR0 = value;

    for (i = 0; i < 8; i++)
    {
        if (bestPre == (1U << i))
        {
            bestPre = i;
            break;
        }
    }
    base->MCFGR1 = (base->MCFGR1 & ~LPI2C_MCFGR1_PRESCALE_MASK) | LPI2C_MCFGR1_PRESCALE(bestPre);

    /* Restore master mode. */
    if (wasEnabled)
    {
        LPI2C_MasterSetEnable(base, true);
    }
}

/*!
 * @brief Convert provided flags to status code, and clear any errors if present.
 */
status_t LPI2C_MasterCheckAndClearError(LPI2C_Type *base, uint32_t status)
{
    status_t result = kStatus_Success;

    /*
     * Check for error. These errors cause a stop to automatically be sent. We must
     * clear the errors before a new transfer can start.
     */
    status &= kMasterErrorFlags;
    if (status)
    {
        /* Select the correct error code. Ordered by severity, with bus issues first. */
        if (status & kLPI2C_MasterPinLowTimeoutFlag)
        {
            result = kStatus_LPI2C_PinLowTimeout;
        }
        if (status & kLPI2C_MasterArbitrationLostFlag)
        {
            result = kStatus_LPI2C_ArbitrationLost;
        }
        if (status & kLPI2C_MasterNackDetectFlag)
        {
            result = kStatus_LPI2C_Nak;
        }
        if (status & kLPI2C_MasterFifoErrFlag)
        {
            result = kStatus_LPI2C_FifoError;
        }

        /* Clear the flags. */
        LPI2C_MasterClearStatusFlags(base, status);

        /* Reset fifos. These flags clear automatically. */
        base->MCR |= LPI2C_MCR_RRF_MASK | LPI2C_MCR_RTF_MASK;
    }

    return result;
}

/*!
 * @brief Wait until there is room in the tx fifo.
 */
status_t LPI2C_MasterWaitForTxReady(LPI2C_Type *base)
{
    uint32_t status;
    size_t txCount;
    size_t txFifoSize = FSL_FEATURE_LPI2C_FIFO_SIZEn(LPI2C_GetInstance(base));
    do
    {
        status_t result;

        /* Get the number of words in the tx fifo and compute empty slots. */
        LPI2C_MasterGetFifoCounts(base, NULL, &txCount);
        txCount = txFifoSize - txCount;

        /* Check for error flags. */
        status = LPI2C_MasterGetStatusFlags(base);
        result = LPI2C_MasterCheckAndClearError(base, status);
        if (result)
        {
            return result;
        }
    } while (!txCount);

    return kStatus_Success;
}

/*!
 * @brief Make sure the bus isn't already busy.
 *
 * A busy bus is allowed if we are the one driving it.
 */
status_t LPI2C_CheckForBusyBus(LPI2C_Type *base)
{
    uint32_t status = LPI2C_MasterGetStatusFlags(base);
    if ((status & kLPI2C_MasterBusBusyFlag) && !(status & kLPI2C_MasterBusyFlag))
    {
        return kStatus_LPI2C_Busy;
    }

    return kStatus_Success;
}

status_t LPI2C_MasterStart(LPI2C_Type *base, uint8_t address, lpi2c_direction_t dir)
{
    /* Return an error if the bus is already in use not by us. */
    status_t result = LPI2C_CheckForBusyBus(base);
    if (result)
    {
        return result;
    }

    /* Clear all flags. */
    LPI2C_MasterClearStatusFlags(base, kMasterClearFlags);

    /* Issue start command. */
    assert(base->MSR & LPI2C_MSR_TDF_MASK);
    base->MTDR = kStartCmd | (((uint32_t)address << 1U) | (uint32_t)dir);

    return kStatus_Success;
}

status_t LPI2C_MasterStop(LPI2C_Type *base)
{
    /* Wait until there is room in the fifo. */
    status_t result = LPI2C_MasterWaitForTxReady(base);
    if (result)
    {
        return result;
    }

    /* Send the STOP signal */
    base->MTDR = kStopCmd;

    /*
     * Wait for the stop detected flag to set, indicating the transfer has completed on the bus.
     * Also check for errors while waiting.
     */
    while (result == kStatus_Success)
    {
        uint32_t status = LPI2C_MasterGetStatusFlags(base);

        /* Check for error flags. */
        result = LPI2C_MasterCheckAndClearError(base, status);

        /* Check if the stop was sent successfully. */
        if (status & kLPI2C_MasterStopDetectFlag)
        {
            LPI2C_MasterClearStatusFlags(base, kLPI2C_MasterStopDetectFlag);
            break;
        }
    }

    return result;
}

/* XXX handle receive sizes > 256 */
status_t LPI2C_MasterReceive(LPI2C_Type *base, void *rxBuff, size_t rxSize)
{
    status_t result;
    uint8_t *buf;

    assert(rxBuff);

    /* Handle empty read. */
    if (!rxSize)
    {
        return kStatus_Success;
    }

    /* Wait until there is room in the command fifo. */
    result = LPI2C_MasterWaitForTxReady(base);
    if (result)
    {
        return result;
    }

    /* Issue command to receive data. */
    base->MTDR = kRxDataCmd | LPI2C_MTDR_DATA(rxSize - 1);

    /* Receive data */
    buf = (uint8_t *)rxBuff;
    while (rxSize--)
    {
        /*
         * Read LPI2C receive fifo register. The register includes a flag to indicate whether
         * the FIFO is empty, so we can both get the data and check if we need to keep reading
         * using a single register read.
         */
        uint32_t value;
        do
        {
            /* Check for errors. */
            result = LPI2C_MasterCheckAndClearError(base, LPI2C_MasterGetStatusFlags(base));
            if (result)
            {
                return result;
            }

            value = base->MRDR;
        } while (value & LPI2C_MRDR_RXEMPTY_MASK);

        *buf++ = value & LPI2C_MRDR_DATA_MASK;
    }

    return kStatus_Success;
}

status_t LPI2C_MasterSend(LPI2C_Type *base, const void *txBuff, size_t txSize)
{
    const uint8_t *buf = (const uint8_t *)txBuff;

    assert(txBuff);

    /* Send data buffer */
    while (txSize--)
    {
        /* Wait until there is room in the fifo. This also checks for errors. */
        status_t result = LPI2C_MasterWaitForTxReady(base);
        if (result)
        {
            return result;
        }

        /* Write byte into LPI2C master data register. */
        base->MTDR = *buf++;
    }

    return kStatus_Success;
}

void LPI2C_MasterCreateHandle(LPI2C_Type *base,
                              lpi2c_master_handle_t *handle,
                              lpi2c_master_transfer_callback_t callback,
                              void *userData)
{
    uint32_t instance;

    assert(handle);

    /* Clear out the handle. */
    memset(handle, 0, sizeof(*handle));

    /* Look up instance number */
    instance = LPI2C_GetInstance(base);

    /* Save base and instance. */
    handle->base               = base;
    handle->instance           = instance;
    handle->completionCallback = callback;
    handle->userData           = userData;

    /* Save this handle for IRQ use. */
    s_lpi2cMasterHandle[instance] = handle;

    /* Set irq handler. */
    s_lpi2cMasterIsr = (lpi2c_isr_t)LPI2C_MasterHandleInterrupt;

    /* Clear internal IRQ enables and enable NVIC IRQ. */
    LPI2C_MasterDisableInterrupts(base, kMasterIrqFlags);
    EnableIRQ(kLpi2cIrqs[instance]);
}

/*!
 * @brief Execute states until FIFOs are exhausted.
 */
status_t LPI2C_RunTransferStateMachine(lpi2c_master_handle_t *handle, bool *isDone)
{
    uint32_t status;
    status_t result;
    lpi2c_master_transfer_t *xfer;
    size_t txCount;
    size_t rxCount;
    size_t txFifoSize = FSL_FEATURE_LPI2C_FIFO_SIZEn(LPI2C_GetInstance(handle->base));

    /* Set default isDone return value. */
    *isDone = false;

    /* Check for errors. */
    status = LPI2C_MasterGetStatusFlags(handle->base);
    result = LPI2C_MasterCheckAndClearError(handle->base, status);
    if (result)
    {
        return result;
    }

    /* Get pointer to private data. */
    xfer = &handle->transfer;

    /* Get fifo counts and compute room in tx fifo. */
    LPI2C_MasterGetFifoCounts(handle->base, &rxCount, &txCount);
    txCount = txFifoSize - txCount;

    do
    {
        /* Execute the state. */
        switch (handle->state)
        {
            case kSendCommandState:
            {
                /* Make sure there is room in the tx fifo for the next command. */
                if (!txCount--)
                {
                    return result;
                }

                /* Issue command. buf is a uint8_t* pointing at the uint16 command array. */
                handle->base->MTDR = *(uint16_t *)handle->buf;
                handle->buf += sizeof(uint16_t);

                /* Count down until all commands are sent. */
                if (--handle->remainingBytes == 0)
                {
                    /* Choose next state and set up buffer pointer and count. */
                    if (xfer->dataSize)
                    {
                        /* Either a send or receive transfer is next. */
                        handle->state          = kTransferDataState;
                        handle->buf            = xfer->data;
                        handle->remainingBytes = xfer->dataSize;
                    }
                    else
                    {
                        /* No transfer, so move to stop state. */
                        handle->state = kStopState;
                    }
                }
                break;
            }

            case kIssueReadCommandState:
                /* Make sure there is room in the tx fifo for the read command. */
                if (!txCount--)
                {
                    return result;
                }

                handle->base->MTDR = kRxDataCmd | LPI2C_MTDR_DATA(xfer->dataSize - 1);

                /* Move to transfer state. */
                handle->state = kTransferDataState;
                break;

            case kTransferDataState:
                if (xfer->direction == kLPI2C_Write)
                {
                    /* Make sure there is room in the tx fifo. */
                    if (!txCount--)
                    {
                        return result;
                    }

                    /* Put byte to send in fifo. */
                    handle->base->MTDR = *(handle->buf)++;
                }
                else
                {
                    /* XXX handle receive sizes > 256, use kIssueReadCommandState */
                    /* Make sure there is data in the rx fifo. */
                    if (!rxCount--)
                    {
                        return result;
                    }

                    /* Read byte from fifo. */
                    *(handle->buf)++ = handle->base->MRDR & LPI2C_MRDR_DATA_MASK;
                }

                /* Move to stop when the transfer is done. */
                if (--handle->remainingBytes == 0)
                {
                    handle->state = kStopState;
                }
                break;

            case kStopState:
                /* Only issue a stop transition if the caller requested it. */
                if ((xfer->flags & kLPI2C_TransferNoStopFlag) == 0)
                {
                    /* Make sure there is room in the tx fifo for the stop command. */
                    if (!txCount--)
                    {
                        return result;
                    }

                    handle->base->MTDR = kStopCmd;
                }
                else
                {
                    /* Caller doesn't want to send a stop, so we're done now. */
                    *isDone = true;
                }
                handle->state = kWaitForCompletionState;
                break;

            case kWaitForCompletionState:
                /* We stay in this state until the stop state is detected. */
                if (status & kLPI2C_MasterStopDetectFlag)
                {
                    *isDone = true;
                }
                return result;

            default:
                assert(false);
                break;
        }
    } while (result == kStatus_Success);

    return result;
}

/*!
 * @brief Prepares the transfer state machine and fills in the command buffer.
 */
void LPI2C_InitTransferStateMachine(lpi2c_master_handle_t *handle)
{
    lpi2c_master_transfer_t *xfer = &handle->transfer;

    /* Handle no start option. */
    if (xfer->flags & kLPI2C_TransferNoStartFlag)
    {
        if (xfer->direction == kLPI2C_Read)
        {
            /* Need to issue read command first. */
            handle->state = kIssueReadCommandState;
        }
        else
        {
            /* Start immediately in the data transfer state. */
            handle->state = kTransferDataState;
        }

        handle->buf            = xfer->data;
        handle->remainingBytes = xfer->dataSize;
    }
    else
    {
        uint16_t *cmd     = (uint16_t *)&handle->commandBuffer;
        uint32_t cmdCount = 0;

        /*
         * Initial direction depends on whether a subaddress was provided, and of course the actual
         * data transfer direction.
         */
        lpi2c_direction_t direction = xfer->subaddressSize ? kLPI2C_Write : xfer->direction;

        /* Start command. */
        cmd[cmdCount++] = kStartCmd | (((uint16_t)xfer->slaveAddress << 1U) | (uint16_t)direction);

        /* Subaddress, MSB first. */
        if (xfer->subaddressSize)
        {
            uint32_t subaddressRemaining = xfer->subaddressSize;
            while (subaddressRemaining--)
            {
                uint8_t subaddressByte = (xfer->subaddress >> (8 * subaddressRemaining)) & 0xff;
                cmd[cmdCount++] = subaddressByte;
            }
        }

        /* Reads need special handling. */
        if (xfer->dataSize && xfer->direction == kLPI2C_Read)
        {
            /* Need to send repeated start if switching directions to read. */
            if (direction == kLPI2C_Write)
            {
                cmd[cmdCount++] = kStartCmd | (((uint16_t)xfer->slaveAddress << 1U) | kLPI2C_Read);
            }

            /* Read command. */
            cmd[cmdCount++] = kRxDataCmd | LPI2C_MTDR_DATA(xfer->dataSize - 1);
        }

        /* Set up state machine for transferring the commands. */
        handle->state          = kSendCommandState;
        handle->remainingBytes = cmdCount;
        handle->buf            = (uint8_t *)&handle->commandBuffer;
    }
}

status_t LPI2C_MasterTransferNonBlocking(LPI2C_Type *base, lpi2c_master_handle_t *handle, lpi2c_master_transfer_t *xfer)
{
    status_t result;
    bool isDone;

    assert(handle);
    assert(xfer);
    assert(xfer->subaddressSize <= sizeof(xfer->subaddress));

    /* Return busy if another transaction is in progress. */
    if (handle->state != kIdleState)
    {
        return kStatus_LPI2C_Busy;
    }

    /* Return an error if the bus is already in use not by us. */
    result = LPI2C_CheckForBusyBus(handle->base);
    if (result)
    {
        return result;
    }

    /* Disable LPI2C IRQ sources while we configure stuff. */
    LPI2C_MasterDisableInterrupts(handle->base, kMasterIrqFlags);

    /* Save transfer into handle. */
    handle->transfer = *xfer;

    /* Generate commands to send. */
    LPI2C_InitTransferStateMachine(handle);

    /* Clear all flags. */
    LPI2C_MasterClearStatusFlags(handle->base, kMasterClearFlags);

    /* Prime the tx fifo by stepping the state machine once. */
    result = LPI2C_RunTransferStateMachine(handle, &isDone);

    if (!result)
    {
        /* Enable LPI2C internal IRQ sources. NVIC IRQ was enabled in CreateHandle() */
        LPI2C_MasterEnableInterrupts(handle->base, kMasterIrqFlags);
    }

    return result;
}

status_t LPI2C_MasterGetTransferStatus(LPI2C_Type *base, lpi2c_master_handle_t *handle, size_t *transferCount)
{
    assert(handle);

    /* Catch when there is not an active transfer. */
    if (handle->state == kIdleState)
    {
        if (transferCount)
        {
            *transferCount = 0;
        }
        return kStatus_LPI2C_Idle;
    }

    if (transferCount)
    {
        uint8_t state;
        uint16_t remainingBytes;
        uint32_t dataSize;

        /*
         * Cache some fields with IRQs disabled. This ensures all field values
         * are synchronized with each other during an ongoing transfer.
         */
        uint32_t irqs = LPI2C_MasterGetEnabledInterrupts(base);
        LPI2C_MasterDisableInterrupts(base, irqs);
        state          = handle->state;
        remainingBytes = handle->remainingBytes;
        dataSize       = handle->transfer.dataSize;
        LPI2C_MasterEnableInterrupts(base, irqs);

        /* Get transfer count based on current transfer state. */
        switch (state)
        {
            case kIdleState:
            case kSendCommandState:
            case kIssueReadCommandState: /* XXX return correct value for this state when >256 reads are supported */
                *transferCount = 0;
                break;

            case kTransferDataState:
                *transferCount = dataSize - remainingBytes;
                break;

            case kStopState:
            case kWaitForCompletionState:
            default:
                *transferCount = dataSize;
                break;
        }
    }

    return kStatus_Success;
}

status_t LPI2C_MasterAbortTransfer(LPI2C_Type *base, lpi2c_master_handle_t *handle)
{
    /*
     * Disable internal IRQ enables. We don't need to re-enable because we're either aborting
     * or there wasn't an active transfer, in which case IRQs were already disabled.
     */
    LPI2C_MasterDisableInterrupts(base, kMasterIrqFlags);

    /* Catch when there is not an active transfer. */
    if (handle->state == kIdleState)
    {
        return kStatus_LPI2C_Idle;
    }

    /* Reset fifos. */
    base->MCR |= LPI2C_MCR_RRF_MASK | LPI2C_MCR_RTF_MASK;

    /* Send a stop command to finalize the transfer. */
    base->MTDR = kStopCmd;

    /* Reset handle. */
    handle->state = kIdleState;

    return kStatus_Success;
}

void LPI2C_MasterHandleInterrupt(LPI2C_Type *base, lpi2c_master_handle_t *handle)
{
    bool isDone;
    status_t result;

    /* Don't do anything if we don't have a valid handle. */
    if (!handle)
    {
        return;
    }

    if (handle->state == kIdleState)
    {
        return;
    }

    result = LPI2C_RunTransferStateMachine(handle, &isDone);

    if (isDone || result != kStatus_Success)
    {
        /* XXX need to handle data that may be in rx fifo below watermark level? */

        /* XXX handle error, terminate xfer */

        /* Disable internal IRQ enables. */
        LPI2C_MasterDisableInterrupts(handle->base, kMasterIrqFlags);

        /* Invoke callback. */
        if (handle->completionCallback)
        {
            handle->completionCallback(handle->base, handle, result, handle->userData);
        }

        /* Set handle to idle state. */
        handle->state = kIdleState;
    }
}

void LPI2C_SlaveGetDefaultConfig(lpi2c_slave_config_t *slaveConfig)
{
    slaveConfig->enableSlave               = true;
    slaveConfig->address0                  = 0U;
    slaveConfig->address1                  = 0U;
    slaveConfig->addressMatchMode          = kLPI2C_MatchAddress0;
    slaveConfig->filterDozeEnable          = true;
    slaveConfig->filterEnable              = true;
    slaveConfig->enableGeneralCall         = false;
    slaveConfig->sclStall.enableAck        = false;
    slaveConfig->sclStall.enableTx         = true;
    slaveConfig->sclStall.enableRx         = true;
    slaveConfig->sclStall.enableAddress    = true;
    slaveConfig->ignoreAck                 = false;
    slaveConfig->enableReceivedAddressRead = false;
    slaveConfig->sdaGlitchFilterWidth_ns   = 0; // TODO determine default width values
    slaveConfig->sclGlitchFilterWidth_ns   = 0;
    slaveConfig->dataValidDelay_ns         = 0;
    slaveConfig->clockHoldTime_ns          = 0;
}

void LPI2C_SlaveInit(LPI2C_Type *base, const lpi2c_slave_config_t *slaveConfig, uint32_t sourceClock_Hz)
{
    /* Ungate the clock. */
    CLOCK_EnableClock(kLpi2cClocks[LPI2C_GetInstance(base)]);

    /* Restore to reset conditions. */
    LPI2C_SlaveReset(base);

    /* Configure peripheral. */
    base->SAMR = LPI2C_SAMR_ADDR0(slaveConfig->address0) | LPI2C_SAMR_ADDR1(slaveConfig->address1);

    base->SCFGR1 =
        LPI2C_SCFGR1_ADDRCFG(slaveConfig->addressMatchMode) | LPI2C_SCFGR1_IGNACK(slaveConfig->ignoreAck) |
        LPI2C_SCFGR1_RXCFG(slaveConfig->enableReceivedAddressRead) | LPI2C_SCFGR1_GCEN(slaveConfig->enableGeneralCall) |
        LPI2C_SCFGR1_ACKSTALL(slaveConfig->sclStall.enableAck) | LPI2C_SCFGR1_TXDSTALL(slaveConfig->sclStall.enableTx) |
        LPI2C_SCFGR1_RXSTALL(slaveConfig->sclStall.enableRx) |
        LPI2C_SCFGR1_ADRSTALL(slaveConfig->sclStall.enableAddress);

    base->SCFGR2 =
        LPI2C_SCFGR2_FILTSDA(LPI2C_GetCyclesForWidth(sourceClock_Hz, slaveConfig->sdaGlitchFilterWidth_ns,
                                                     (LPI2C_SCFGR2_FILTSDA_MASK >> LPI2C_SCFGR2_FILTSDA_SHIFT), 1)) |
        LPI2C_SCFGR2_FILTSCL(LPI2C_GetCyclesForWidth(sourceClock_Hz, slaveConfig->sclGlitchFilterWidth_ns,
                                                     (LPI2C_SCFGR2_FILTSCL_MASK >> LPI2C_SCFGR2_FILTSCL_SHIFT), 1)) |
        LPI2C_SCFGR2_DATAVD(LPI2C_GetCyclesForWidth(sourceClock_Hz, slaveConfig->dataValidDelay_ns,
                                                    (LPI2C_SCFGR2_DATAVD_MASK >> LPI2C_SCFGR2_DATAVD_SHIFT), 1)) |
        LPI2C_SCFGR2_CLKHOLD(LPI2C_GetCyclesForWidth(sourceClock_Hz, slaveConfig->clockHoldTime_ns,
                                                     (LPI2C_SCFGR2_CLKHOLD_MASK >> LPI2C_SCFGR2_CLKHOLD_SHIFT), 1));

    /* Save SCR to last so we don't enable slave until it is configured */
    base->SCR = LPI2C_SCR_FILTDZ(slaveConfig->filterDozeEnable) | LPI2C_SCR_FILTEN(slaveConfig->filterEnable) |
                LPI2C_SCR_SEN(slaveConfig->enableSlave);
}

void LPI2C_SlaveDeinit(LPI2C_Type *base)
{
    LPI2C_SlaveReset(base);

    /* Gate the clock. */
    CLOCK_DisableClock(kLpi2cClocks[LPI2C_GetInstance(base)]);
}

status_t LPI2C_SlaveCheckAndClearError(LPI2C_Type *base, uint32_t flags)
{
    status_t result = kStatus_Success;

    flags &= kSlaveErrorFlags;
    if (flags)
    {
        if (flags & kLPI2C_SlaveBitErrFlag)
        {
            result = kStatus_LPI2C_BitError;
        }
        else if (flags & kLPI2C_SlaveFifoErrFlag)
        {
            result = kStatus_LPI2C_FifoError;
        }

        /* Clear the errors. */
        LPI2C_SlaveClearStatusFlags(base, flags);
    }

    return result;
}

status_t LPI2C_SlaveSend(LPI2C_Type *base, const void *txBuff, size_t txSize, size_t *actualTxSize)
{
    uint8_t *buf     = (uint8_t *)txBuff;
    size_t remaining = txSize;

    assert(txBuff);

    while (remaining)
    {
        uint32_t flags;
        status_t result;

        /* Wait until we can transmit. */
        do
        {
            /* Check for errors */
            flags  = LPI2C_SlaveGetStatusFlags(base);
            result = LPI2C_SlaveCheckAndClearError(base, flags);
            if (result)
            {
                if (actualTxSize)
                {
                    *actualTxSize = txSize - remaining;
                }
                return result;
            }
        } while (
            !(flags & (kLPI2C_SlaveTxReadyFlag | kLPI2C_SlaveStopDetectFlag | kLPI2C_SlaveRepeatedStartDetectFlag)));

        /* Send a byte. */
        if (flags & kLPI2C_SlaveTxReadyFlag)
        {
            base->STDR = *buf++;
            --remaining;
        }

        /* Exit loop if we see a stop or restart */
        if (flags & (kLPI2C_SlaveStopDetectFlag | kLPI2C_SlaveRepeatedStartDetectFlag))
        {
            LPI2C_SlaveClearStatusFlags(base, kLPI2C_SlaveStopDetectFlag | kLPI2C_SlaveRepeatedStartDetectFlag);
            break;
        }
    }

    if (actualTxSize)
    {
        *actualTxSize = txSize - remaining;
    }

    return kStatus_Success;
}

status_t LPI2C_SlaveReceive(LPI2C_Type *base, void *rxBuff, size_t rxSize, size_t *actualRxSize)
{
    uint8_t *buf     = (uint8_t *)rxBuff;
    size_t remaining = rxSize;

    assert(rxBuff);

    while (remaining)
    {
        uint32_t flags;
        status_t result;

        /* Wait until we can receive. */
        do
        {
            /* Check for errors */
            flags  = LPI2C_SlaveGetStatusFlags(base);
            result = LPI2C_SlaveCheckAndClearError(base, flags);
            if (result)
            {
                if (actualRxSize)
                {
                    *actualRxSize = rxSize - remaining;
                }
                return result;
            }
        } while (
            !(flags & (kLPI2C_SlaveRxReadyFlag | kLPI2C_SlaveStopDetectFlag | kLPI2C_SlaveRepeatedStartDetectFlag)));

        /* Receive a byte. */
        if (flags & kLPI2C_SlaveRxReadyFlag)
        {
            *buf++ = base->SRDR & LPI2C_SRDR_DATA_MASK;
            --remaining;
        }

        /* Exit loop if we see a stop or restart */
        if (flags & (kLPI2C_SlaveStopDetectFlag | kLPI2C_SlaveRepeatedStartDetectFlag))
        {
            LPI2C_SlaveClearStatusFlags(base, kLPI2C_SlaveStopDetectFlag | kLPI2C_SlaveRepeatedStartDetectFlag);
            break;
        }
    }

    if (actualRxSize)
    {
        *actualRxSize = rxSize - remaining;
    }

    return kStatus_Success;
}

void LPI2C_SlaveCreateHandle(LPI2C_Type *base,
                             lpi2c_slave_handle_t *handle,
                             lpi2c_slave_transfer_callback_t callback,
                             void *userData)
{
    uint32_t instance;

    assert(handle);

    /* Clear out the handle. */
    memset(handle, 0, sizeof(*handle));

    /* Look up instance number */
    instance = LPI2C_GetInstance(base);

    /* Save base and instance. */
    handle->base     = base;
    handle->instance = instance;
    handle->callback = callback;
    handle->userData = userData;

    /* Save this handle for IRQ use. */
    s_lpi2cSlaveHandle[instance] = handle;

    /* Set irq handler. */
    s_lpi2cSlaveIsr = (lpi2c_isr_t)LPI2C_SlaveHandleInterrupt;

    /* Clear internal IRQ enables and enable NVIC IRQ. */
    LPI2C_SlaveDisableInterrupts(base, kMasterIrqFlags);
    EnableIRQ(kLpi2cIrqs[instance]);

    /* Nack by default. */
    base->STAR = LPI2C_STAR_TXNACK_MASK;
}

status_t LPI2C_SlaveTransferNonBlocking(LPI2C_Type *base, lpi2c_slave_handle_t *handle, uint32_t eventMask)
{
    uint32_t status;

    assert(handle);

    /* Return busy if another transaction is in progress. */
    if (handle->isBusy)
    {
        return kStatus_LPI2C_Busy;
    }

    /* Return an error if the bus is already in use not by us. */
    status = LPI2C_SlaveGetStatusFlags(handle->base);
    if ((status & kLPI2C_SlaveBusBusyFlag) && !(status & kLPI2C_SlaveBusyFlag))
    {
        return kStatus_LPI2C_Busy;
    }

    /* Disable LPI2C IRQ sources while we configure stuff. */
    LPI2C_SlaveDisableInterrupts(handle->base, kSlaveIrqFlags);

    /* Clear transfer in handle. */
    memset(&handle->transfer, 0, sizeof(handle->transfer));

    /* Record that we're busy. */
    handle->isBusy = true;

    /* Set up event mask. tx and rx are always enabled. */
    handle->eventMask = eventMask | kLPI2C_SlaveTransmitEvent | kLPI2C_SlaveReceiveEvent;

    /* Ack by default. */
    handle->base->STAR = 0;

    /* Clear all flags. */
    LPI2C_SlaveClearStatusFlags(handle->base, kSlaveClearFlags);

    /* Enable LPI2C internal IRQ sources. NVIC IRQ was enabled in CreateHandle() */
    LPI2C_SlaveEnableInterrupts(handle->base, kSlaveIrqFlags);

    return kStatus_Success;
}

status_t LPI2C_SlaveGetTransferStatus(LPI2C_Type *base, lpi2c_slave_handle_t *handle, size_t *transferCount)
{
    assert(handle);

    /* Catch when there is not an active transfer. */
    if (!handle->isBusy)
    {
        if (transferCount)
        {
            *transferCount = 0;
        }
        return kStatus_LPI2C_Idle;
    }

    /* For an active transfer, just return the count from the handle. */
    if (transferCount)
    {
        *transferCount = handle->transferredCount;
    }

    return kStatus_Success;
}

status_t LPI2C_SlaveAbortTransfer(LPI2C_Type *base, lpi2c_slave_handle_t *handle)
{
    uint32_t status;

    assert(handle);

    /* Return idle if no transaction is in progress. */
    if (!handle->isBusy)
    {
        return kStatus_LPI2C_Idle;
    }

    /* Disable LPI2C IRQ sources. */
    LPI2C_SlaveDisableInterrupts(handle->base, kSlaveIrqFlags);

    /* Nack by default. */
    handle->base->STAR = LPI2C_STAR_TXNACK_MASK;

    /* Reset transfer info. */
    memset(&handle->transfer, 0, sizeof(handle->transfer));

    /* We're no longer busy. */
    handle->isBusy = false;

    return kStatus_Success;
}

void LPI2C_SlaveHandleInterrupt(LPI2C_Type *base, lpi2c_slave_handle_t *handle)
{
    uint32_t flags;
    lpi2c_slave_transfer_t *xfer;

    /* Check for a valid handle in case of a spurious interrupt. */
    if (!handle)
    {
        return;
    }

    xfer = &handle->transfer;

    /* Get status flags. */
    flags = LPI2C_SlaveGetStatusFlags(handle->base);

    if (flags & (kLPI2C_SlaveBitErrFlag | kLPI2C_SlaveFifoErrFlag))
    {
        xfer->event            = kLPI2C_SlaveCompletionEvent;
        xfer->completionStatus = LPI2C_SlaveCheckAndClearError(handle->base, flags);

        if ((handle->eventMask & kLPI2C_SlaveCompletionEvent) && handle->callback)
        {
            handle->callback(handle->base, xfer, handle->userData);
        }
        return;
    }
    if (flags & (kLPI2C_SlaveRepeatedStartDetectFlag | kLPI2C_SlaveStopDetectFlag))
    {
        xfer->event = (flags & kLPI2C_SlaveRepeatedStartDetectFlag) ? kLPI2C_SlaveRepeatedStartEvent :
                                                                      kLPI2C_SlaveCompletionEvent;
        xfer->receivedAddress  = 0;
        xfer->completionStatus = kStatus_Success;
        xfer->transferredCount = handle->transferredCount;
        if (handle->wasTransmit)
        {
            /*
             * Subtract one from the transmit count to offset the fact that LPI2C asserts the
             * tx flag before it sees the nack from the master-receiver, thus causing one more
             * count that the master actually receives.
             */
            --xfer->transferredCount;
            handle->wasTransmit = false;
        }

        /* Clear the flag. */
        LPI2C_SlaveClearStatusFlags(handle->base,
                                    flags & (kLPI2C_SlaveRepeatedStartDetectFlag | kLPI2C_SlaveStopDetectFlag));

        /* Revert to sending an Ack by default, in case we sent a Nack for receive. */
        handle->base->STAR = 0;

        if ((handle->eventMask & xfer->event) && handle->callback)
        {
            handle->callback(handle->base, xfer, handle->userData);
        }

        /* Clean up transfer info on completion, after the callback has been invoked. */
        memset(&handle->transfer, 0, sizeof(handle->transfer));
    }
    if (flags & kLPI2C_SlaveAddressValidFlag)
    {
        xfer->event           = kLPI2C_SlaveAddressMatchEvent;
        xfer->receivedAddress = handle->base->SASR & LPI2C_SASR_RADDR_MASK;

        if ((handle->eventMask & kLPI2C_SlaveAddressMatchEvent) && handle->callback)
        {
            handle->callback(handle->base, xfer, handle->userData);
        }
    }
    if (flags & kLPI2C_SlaveTransmitAckFlag)
    {
        xfer->event = kLPI2C_SlaveTransmitAckEvent;

        if ((handle->eventMask & kLPI2C_SlaveTransmitAckEvent) && handle->callback)
        {
            handle->callback(handle->base, xfer, handle->userData);
        }
    }

    /* Handle transmit and receive. */
    if (flags & kLPI2C_SlaveTxReadyFlag)
    {
        handle->wasTransmit = true;

        /* If we're out of data, invoke callback to get more. */
        if (!xfer->data || !xfer->dataSize)
        {
            xfer->event = kLPI2C_SlaveTransmitEvent;
            if (handle->callback)
            {
                handle->callback(handle->base, xfer, handle->userData);
            }

            /* Clear the transferred count now that we have a new buffer. */
            handle->transferredCount = 0;
        }

        /* Transmit a byte. */
        if (xfer->data && xfer->dataSize)
        {
            handle->base->STDR = *xfer->data++;
            --xfer->dataSize;
            ++handle->transferredCount;
        }
    }
    else if (flags & kLPI2C_SlaveRxReadyFlag)
    {
        /* If we're out of room in the buffer, invoke callback to get another. */
        if (!xfer->data || !xfer->dataSize)
        {
            xfer->event = kLPI2C_SlaveReceiveEvent;
            if (handle->callback)
            {
                handle->callback(handle->base, xfer, handle->userData);
            }

            /* Clear the transferred count now that we have a new buffer. */
            handle->transferredCount = 0;
        }

        /* Receive a byte. */
        if (xfer->data && xfer->dataSize)
        {
            *xfer->data++ = handle->base->SRDR;
            --xfer->dataSize;
            ++handle->transferredCount;
        }
        else
        {
            /* We don't have any room to receive more data, so send a nack. */
            handle->base->STAR = LPI2C_STAR_TXNACK_MASK;
        }
    }
}

/*!
 * @brief Shared IRQ handler that can call both master and slave ISRs.
 *
 * The master and slave ISRs are called through function pointers in order to decouple
 * this code from the ISR functions. Without this, the linker would always pull in both
 * ISRs and every function they call, even if only the functional API was used.
 */
void LPI2C_CommonIRQHandler(LPI2C_Type *base, uint32_t instance)
{
    /* Check for master IRQ. */
    if ((base->MCR & LPI2C_MCR_MEN_MASK) && s_lpi2cMasterIsr)
    {
        /* Master mode. */
        s_lpi2cMasterIsr(base, s_lpi2cMasterHandle[instance]);
    }

    /* Check for slave IRQ. */
    if ((base->SCR & LPI2C_SCR_SEN_MASK) && s_lpi2cSlaveIsr)
    {
        /* Slave mode. */
        s_lpi2cSlaveIsr(base, s_lpi2cSlaveHandle[instance]);
    }
}

/* Implementation of LPI2C0 handler named in startup code. */
void LPI2C0_DriverIRQHandler(void)
{
    LPI2C_CommonIRQHandler(LPI2C0, 0);
}

#if (FSL_FEATURE_SOC_LPI2C_COUNT > 1U)
/* Implementation of LPI2C1 handler named in startup code. */
void LPI2C1_DriverIRQHandler(void)
{
    LPI2C_CommonIRQHandler(LPI2C1, 1);
}
#endif

#if (FSL_FEATURE_SOC_LPI2C_COUNT > 2U)
/* Implementation of LPI2C2 handler named in startup code. */
void LPI2C2_DriverIRQHandler(void)
{
    LPI2C_CommonIRQHandler(LPI2C2, 2);
}
#endif
