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

#include "fsl_smartcard_phy_ncn8025.h"
#if (defined(FSL_FEATURE_SOC_EMVSIM_COUNT) && (FSL_FEATURE_SOC_EMVSIM_COUNT))
#include "fsl_smartcard_emvsim.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
* Prototypes
******************************************************************************/
static uint32_t smartcard_phy_ncn8025_InterfaceClockInit(void *base,
                                                         smartcard_interface_config_t const *config,
                                                         uint32_t sourceClockHz);
static void smartcard_phy_ncn8025_InterfaceClockDeinit(void *base, smartcard_interface_config_t const *config);
#if !(defined(FSL_FEATURE_SOC_EMVSIM_COUNT) && (FSL_FEATURE_SOC_EMVSIM_COUNT))
extern void smartcard_uart_TimerStart(uint8_t channel, uint32_t time);
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
* Code
******************************************************************************/
/*!
 * @brief This function initializes clock module used for card clock generation
 */
static uint32_t smartcard_phy_ncn8025_InterfaceClockInit(void *base,
                                                         smartcard_interface_config_t const *config,
                                                         uint32_t sourceClockHz)
{
    assert((NULL != config));
#if defined(FSL_FEATURE_SOC_EMVSIM_COUNT) && (FSL_FEATURE_SOC_EMVSIM_COUNT)
    assert(config->clockModule < FSL_FEATURE_SOC_EMVSIM_COUNT);

    uint32_t emvsimClkMhz = 0;
    uint8_t emvsimPRSCValue;

    /* Retrieve EMV SIM clock */
    emvsimClkMhz = sourceClockHz / 1000000;
    /* Calculate MOD value */
    emvsimPRSCValue = (emvsimClkMhz * 1000) / (config->smartCardClock / 1000);
    /* Set clock prescaler */
    ((EMVSIM_Type *)base)->CLKCFG =
        (((EMVSIM_Type *)base)->CLKCFG & ~EMVSIM_CLKCFG_CLK_PRSC_MASK) | EMVSIM_CLKCFG_CLK_PRSC(emvsimPRSCValue);
    /* Enable smart card clock */
    ((EMVSIM_Type *)base)->PCSR |= EMVSIM_PCSR_SCEN_MASK;

    return config->smartCardClock;
#elif defined(FSL_FEATURE_SOC_FTM_COUNT) && (FSL_FEATURE_SOC_FTM_COUNT)
    assert(config->clockModule < FSL_FEATURE_SOC_FTM_COUNT);

    uint32_t periph_clk_mhz = 0;
    uint16_t ftmModValue;
    uint32_t ftm_base[] = FTM_BASE_ADDRS;
    FTM_Type *ftmBase = (FTM_Type *)ftm_base[config->clockModule];

    /* Retrieve FTM system clock */
    periph_clk_mhz = sourceClockHz / 1000000;
    /* Calculate MOD value */
    ftmModValue = ((periph_clk_mhz * 1000 / 2) / (config->smartCardClock / 1000)) - 1;
    /* un-gate FTM peripheral clock */
    switch ((uint32_t)ftmBase)
    {
        case (uint32_t)FTM0:
            CLOCK_EnableClock(kCLOCK_Ftm0);
            break;
#if FSL_FEATURE_SOC_FTM_COUNT > 1
        case (uint32_t)FTM1:
            CLOCK_EnableClock(kCLOCK_Ftm1);
            break;
#endif
#if FSL_FEATURE_SOC_FTM_COUNT > 2
        case (uint32_t)FTM2:
            CLOCK_EnableClock(kCLOCK_Ftm2);
            break;
#endif
#if FSL_FEATURE_SOC_FTM_COUNT > 3
        case (uint32_t)FTM3:
            CLOCK_EnableClock(kCLOCK_Ftm3);
            break;
#endif
        default:
            break;
    }
    /* Initialize FTM driver */
    /* Reset ftm driver into default context */
    ftmBase->SC = 0;
    ftmBase->CNT = 0;
    ftmBase->MOD = 0;
    ftmBase->CNTIN = 0;
    ftmBase->STATUS = 0;
    ftmBase->MODE = 0x00000004;
    ftmBase->SYNC = 0;
    ftmBase->OUTINIT = 0;
    ftmBase->OUTMASK = 0;
    ftmBase->COMBINE = 0;
    ftmBase->DEADTIME = 0;
    ftmBase->EXTTRIG = 0;
    ftmBase->POL = 0;
    ftmBase->FMS = 0;
    ftmBase->FILTER = 0;
    ftmBase->FLTCTRL = 0;
    ftmBase->CONF = 0;
    ftmBase->FLTPOL = 0;
    ftmBase->SYNCONF = 0;
    ftmBase->INVCTRL = 0;
    ftmBase->SWOCTRL = 0;
    ftmBase->PWMLOAD = 0;
    /* Reset FTM prescaler to 'Divide by 1', i.e., to be same clock as peripheral clock  */
    ftmBase->SC &= ~FTM_SC_PS_MASK;
    /* Disable FTM counter firstly */
    ftmBase->SC &= ~FTM_SC_CLKS_MASK;
    /* Set initial counter value */
    ftmBase->CNTIN = 0;
    /* Set MOD value */
    ftmBase->MOD = ftmModValue;
    /*  Set counter to operates in Up-counting mode */
    ftmBase->SC &= ~FTM_SC_CPWMS_MASK;
    /* Configure mode to output compare, toggle output on match */
    ftmBase->CONTROLS[config->clockModuleChannel].CnSC |= FTM_CnSC_ELSA_MASK;
    ftmBase->CONTROLS[config->clockModuleChannel].CnSC &= ~FTM_CnSC_ELSB_MASK;
    ftmBase->CONTROLS[config->clockModuleChannel].CnSC |= FTM_CnSC_MSA_MASK;
    ftmBase->CONTROLS[config->clockModuleChannel].CnSC &= ~FTM_CnSC_MSB_MASK;
    /* Configure a match value to toggle output at */
    ftmBase->CONTROLS[config->clockModuleChannel].CnV = 1;
    /* Set clock source to start the counter : System clock */
    ftmBase->SC = FTM_SC_CLKS(1);
    /* Re-calculate the actually configured smartcard clock and return to caller */
    return (uint32_t)(((periph_clk_mhz * 1000 / 2) / (ftmBase->MOD + 1)) * 1000);
#else
    return 0;
#endif
}

/*!
 * @brief This function de-initialize clock module used for card clock generation
 */
static void smartcard_phy_ncn8025_InterfaceClockDeinit(void *base, smartcard_interface_config_t const *config)
{
#if defined(FSL_FEATURE_SOC_EMVSIM_COUNT) && (FSL_FEATURE_SOC_EMVSIM_COUNT)
    assert((config->clockModule < FSL_FEATURE_SOC_EMVSIM_COUNT) && (NULL != base));

    /* Disable smart card clock */
    ((EMVSIM_Type *)base)->PCSR &= ~EMVSIM_PCSR_SCEN_MASK;
#elif defined(FSL_FEATURE_SOC_FTM_COUNT) && (FSL_FEATURE_SOC_FTM_COUNT)
    assert(config->clockModule < FSL_FEATURE_SOC_FTM_COUNT);

    uint32_t ftm_base[] = FTM_BASE_ADDRS;
    FTM_Type *ftmBase = (FTM_Type *)ftm_base[config->clockModule];
    /* gate FTM peripheral clock */
    switch ((uint32_t)ftmBase)
    {
        case (uint32_t)FTM0:
            CLOCK_DisableClock(kCLOCK_Ftm0);
            break;
#if FSL_FEATURE_SOC_FTM_COUNT > 1
        case (uint32_t)FTM1:
            CLOCK_DisableClock(kCLOCK_Ftm1);
            break;
#endif
#if FSL_FEATURE_SOC_FTM_COUNT > 2
        case (uint32_t)FTM2:
            CLOCK_DisableClock(kCLOCK_Ftm2);
            break;
#endif
#if FSL_FEATURE_SOC_FTM_COUNT > 3
        case (uint32_t)FTM3:
            CLOCK_DisableClock(kCLOCK_Ftm3);
            break;
#endif
        default:
            break;
    }
#endif
}

void SMARTCARD_PHY_NCN8025_GetDefaultConfig(smartcard_interface_config_t *config)
{
    assert((NULL != config));

    config->clockToResetDelay = SMARTCARD_INIT_DELAY_CLOCK_CYCLES;
    config->vcc = kSMARTCARD_VoltageClassB3_3V;
}

status_t SMARTCARD_PHY_NCN8025_Init(void *base, smartcard_interface_config_t const *config, uint32_t sourceClockHz)
{
    if ((NULL == config) || (0 == sourceClockHz))
    {
        return kStatus_SMARTCARD_InvalidInput;
    }

    /* Configure GPIO(CMDVCC, RST, INT, VSEL0, VSEL1) pins */
    uint32_t gpio_base[] = GPIO_BASE_ADDRS;
    IRQn_Type port_irq[] = PORT_IRQS;
    /* Set VSEL pins to low level context */
    ((GPIO_Type *)gpio_base[config->vsel0Port])->PCOR |= (1u << config->vsel0Pin);
    ((GPIO_Type *)gpio_base[config->vsel1Port])->PCOR |= (1u << config->vsel1Pin);
    /* Set VSEL pins to output pins */
    ((GPIO_Type *)gpio_base[config->vsel0Port])->PDDR |= (1u << config->vsel0Pin);
    ((GPIO_Type *)gpio_base[config->vsel1Port])->PDDR |= (1u << config->vsel1Pin);
#if defined(FSL_FEATURE_SOC_EMVSIM_COUNT) && (FSL_FEATURE_SOC_EMVSIM_COUNT)
    /* Set CMD_VCC pin to logic level '1', to allow card detection interrupt from NCN8025 */
    ((EMVSIM_Type *)base)->PCSR |= EMVSIM_PCSR_SVCC_EN_MASK;
    ((EMVSIM_Type *)base)->PCSR &= ~EMVSIM_PCSR_VCCENP_MASK;
#else
    /* Set RST pin to zero context and CMDVCC to high context */
    ((GPIO_Type *)gpio_base[config->resetPort])->PCOR |= (1u << config->resetPin);
    ((GPIO_Type *)gpio_base[config->controlPort])->PSOR |= (1u << config->controlPin);
    /* Set CMDVCC, RESET pins as output pins */
    ((GPIO_Type *)gpio_base[config->resetPort])->PDDR |= (1u << config->resetPin);
    ((GPIO_Type *)gpio_base[config->controlPort])->PDDR |= (1u << config->controlPin);

#endif
    /* Initialize INT pin */
    ((GPIO_Type *)gpio_base[config->irqPort])->PDDR &= ~(1u << config->irqPin);
    /* Enable Port IRQ for smartcard presence detection */
    NVIC_EnableIRQ(port_irq[config->irqPort]);
    /* Smartcard clock initialization */
    if (config->smartCardClock != smartcard_phy_ncn8025_InterfaceClockInit(base, config, sourceClockHz))
    {
        return kStatus_SMARTCARD_OtherError;
    }

    return kStatus_SMARTCARD_Success;
}

void SMARTCARD_PHY_NCN8025_Deinit(void *base, smartcard_interface_config_t *config)
{
    assert((NULL != config));

    IRQn_Type port_irq[] = PORT_IRQS;
    NVIC_EnableIRQ(port_irq[config->irqPort]);
    /* Stop smartcard clock */
    smartcard_phy_ncn8025_InterfaceClockDeinit(base, config);
}

status_t SMARTCARD_PHY_NCN8025_Activate(void *base, smartcard_context_t *context, smartcard_reset_type_t resetType)
{
    if ((NULL == context))
    {
        return kStatus_SMARTCARD_InvalidInput;
    }

    context->timersState.initCharTimerExpired = false;
    context->resetType = resetType;
    uint32_t gpio_base[] = GPIO_BASE_ADDRS;

/* Set data convention format into default direct mode */
#if defined(FSL_FEATURE_SOC_EMVSIM_COUNT) && (FSL_FEATURE_SOC_EMVSIM_COUNT)
    EMVSIM_Type *emvsimBase = (EMVSIM_Type *)base;
    context->cardParams.Fi = 372;
    context->cardParams.currentD = 1;
    emvsimBase->CTRL &= ~EMVSIM_CTRL_IC_MASK;
    emvsimBase->DIVISOR = (context->cardParams.Fi / context->cardParams.currentD) & 0x1FFU;
#else
    ((UART_Type *)base)->S2 &= ~UART_S2_RXINV_MASK;
    ((UART_Type *)base)->S2 &= ~UART_S2_MSBF_MASK;
    ((UART_Type *)base)->C3 &= ~UART_C3_TXINV_MASK;
#endif

    if (resetType == kSMARTCARD_ColdReset)
    { /* Ensure that RST is LOW and CMD is high here so that PHY goes in normal mode */
#if defined(FSL_FEATURE_SOC_EMVSIM_COUNT) && (FSL_FEATURE_SOC_EMVSIM_COUNT)
        emvsimBase->PCSR |= EMVSIM_PCSR_SVCC_EN_MASK;
        emvsimBase->PCSR &= ~EMVSIM_PCSR_VCCENP_MASK;
        emvsimBase->PCSR &= ~EMVSIM_PCSR_SRST_MASK;
#else
        ((GPIO_Type *)gpio_base[context->interfaceConfig.resetPort])->PCOR |= (1u << context->interfaceConfig.resetPin);
        ((GPIO_Type *)gpio_base[context->interfaceConfig.controlPort])->PSOR |=
            (1u << context->interfaceConfig.controlPin);
#endif
        if (context->interfaceConfig.vcc == kSMARTCARD_VoltageClassA5_0V)
        { /* vcc = 5v: vsel0=0,vsel1= 1 */
            ((GPIO_Type *)gpio_base[context->interfaceConfig.vsel1Port])->PSOR |=
                (1u << context->interfaceConfig.vsel1Pin);
            ((GPIO_Type *)gpio_base[context->interfaceConfig.vsel0Port])->PCOR |=
                (1u << context->interfaceConfig.vsel0Pin);
        }
        else if (context->interfaceConfig.vcc == kSMARTCARD_VoltageClassB3_3V)
        { /* vcc = 3.3v: vsel0=x,vsel1= 0 */
            ((GPIO_Type *)gpio_base[context->interfaceConfig.vsel1Port])->PCOR |=
                (1u << context->interfaceConfig.vsel1Pin);
            ((GPIO_Type *)gpio_base[context->interfaceConfig.vsel0Port])->PCOR |=
                (1u << context->interfaceConfig.vsel0Pin);
        }
        else
        { /* vcc = 1.8v: vsel0=1,vsel1= 1 */
            ((GPIO_Type *)gpio_base[context->interfaceConfig.vsel1Port])->PSOR |=
                (1u << context->interfaceConfig.vsel1Pin);
            ((GPIO_Type *)gpio_base[context->interfaceConfig.vsel0Port])->PSOR |=
                (1u << context->interfaceConfig.vsel0Pin);
        }
/* Set PHY to start Activation sequence by pulling CMDVCC low */
#if defined(FSL_FEATURE_SOC_EMVSIM_COUNT) && (FSL_FEATURE_SOC_EMVSIM_COUNT)
        emvsimBase->PCSR |= EMVSIM_PCSR_VCCENP_MASK;
#else
        ((GPIO_Type *)gpio_base[context->interfaceConfig.controlPort])->PCOR |=
            (1u << context->interfaceConfig.controlPin);
#endif
    }
    else if (resetType == kSMARTCARD_WarmReset)
    { /* Ensure that card is not already active */
        if (!context->cardParams.active)
        { /* Card is not active;hence return */
            return kStatus_SMARTCARD_Success;
        }
/* Pull RESET low to start warm Activation sequence */
#if defined(FSL_FEATURE_SOC_EMVSIM_COUNT) && (FSL_FEATURE_SOC_EMVSIM_COUNT)
        emvsimBase->PCSR &= ~EMVSIM_PCSR_SRST_MASK;
#else
        ((GPIO_Type *)gpio_base[context->interfaceConfig.resetPort])->PCOR |= (1u << context->interfaceConfig.resetPin);
#endif
    }
/* Wait for sometime as specified by EMV before pulling RST High */
/* As per EMV delay <= 42000 Clock cycles
 * as per PHY delay >= 1us
 */
#if defined(FSL_FEATURE_SOC_EMVSIM_COUNT) && (FSL_FEATURE_SOC_EMVSIM_COUNT)
    /* Disable ICM mode, receiver and ANACK generation */
    emvsimBase->CTRL = (emvsimBase->CTRL & ~(EMVSIM_CTRL_ANACK_MASK | EMVSIM_CTRL_ICM_MASK | EMVSIM_CTRL_RCV_EN_MASK));
    /* Clear any pending counter status flag  */
    emvsimBase->TX_STATUS = EMVSIM_TX_STATUS_GPCNT1_TO_MASK;
    /* Disable counter interrupt */
    emvsimBase->INT_MASK |= EMVSIM_INT_MASK_GPCNT1_IM_MASK;
    /* Set counter value */
    emvsimBase->GPCNT1_VAL = context->interfaceConfig.clockToResetDelay;
    /* Select the clock for GPCNT */
    emvsimBase->CLKCFG =
        (emvsimBase->CLKCFG & ~EMVSIM_CLKCFG_GPCNT1_CLK_SEL_MASK) | EMVSIM_CLKCFG_GPCNT1_CLK_SEL(kEMVSIM_GPCCardClock);
    /* Trigger the counter */
    emvsimBase->CTRL |= EMVSIM_CTRL_RCV_EN_MASK;
    /* In polling mode */
    while ((!(emvsimBase->TX_STATUS & EMVSIM_TX_STATUS_GPCNT1_TO_MASK)))
    {
    }
    /* Disable the counter */
    emvsimBase->CLKCFG &= ~EMVSIM_CLKCFG_GPCNT1_CLK_SEL_MASK;
    /* Disable trigger */
    emvsimBase->CTRL &= ~EMVSIM_CTRL_RCV_EN_MASK;
    /* Clear counter time-out status bit */
    emvsimBase->TX_STATUS = EMVSIM_TX_STATUS_GPCNT1_TO_MASK;
#else
    uint32_t temp = (uint32_t)((float)(1 + (float)(((float)(1000 * context->interfaceConfig.clockToResetDelay)) /
                                                   ((float)context->interfaceConfig.smartCardClock))));
    context->timeDelay(temp);
#endif

/* Pull reset HIGH Now to mark the end of Activation sequence */
#if defined(FSL_FEATURE_SOC_EMVSIM_COUNT) && (FSL_FEATURE_SOC_EMVSIM_COUNT)
    emvsimBase->PCSR |= EMVSIM_PCSR_SRST_MASK;
#else
    ((GPIO_Type *)gpio_base[context->interfaceConfig.resetPort])->PSOR |= (1u << context->interfaceConfig.resetPin);
#endif

#if defined(FSL_FEATURE_SOC_EMVSIM_COUNT) && (FSL_FEATURE_SOC_EMVSIM_COUNT)
    /* Down counter trigger, and clear any pending counter status flag */
    emvsimBase->TX_STATUS = EMVSIM_TX_STATUS_GPCNT1_TO_MASK | EMVSIM_TX_STATUS_GPCNT0_TO_MASK;
    /* Set counter value for TS detection delay */
    emvsimBase->GPCNT0_VAL = (SMARTCARD_INIT_DELAY_CLOCK_CYCLES + SMARTCARD_INIT_DELAY_CLOCK_CYCLES_ADJUSTMENT);
    /* Pre-load counter value for ATR duration delay */
    emvsimBase->GPCNT1_VAL = (SMARTCARD_EMV_ATR_DURATION_ETU + SMARTCARD_ATR_DURATION_ADJUSTMENT);
    /* Select the clock for GPCNT for both TS detection and early start of ATR duration counter */
    emvsimBase->CLKCFG =
        (emvsimBase->CLKCFG & ~EMVSIM_CLKCFG_GPCNT0_CLK_SEL_MASK) | EMVSIM_CLKCFG_GPCNT0_CLK_SEL(kEMVSIM_GPCCardClock);
    /* Set receiver to ICM mode */
    emvsimBase->CTRL |= EMVSIM_CTRL_ICM_MASK;
    /* Enable counter interrupt for TS detection */
    emvsimBase->INT_MASK &= ~EMVSIM_INT_MASK_GPCNT0_IM_MASK;
    /* Clear any pending status flags */
    emvsimBase->RX_STATUS = 0xFFFFFFFF;
    /* Flush RX FIFO */
    emvsimBase->CTRL |= EMVSIM_CTRL_FLSH_RX_MASK;
    /* Enable receiver */
    emvsimBase->CTRL |= EMVSIM_CTRL_RCV_EN_MASK;
#else
    /* Enable external timer for TS detection time-out */
    smartcard_uart_TimerStart(context->interfaceConfig.tsTimerId,
                              (SMARTCARD_INIT_DELAY_CLOCK_CYCLES + SMARTCARD_INIT_DELAY_CLOCK_CYCLES_ADJUSTMENT) *
                                  (CLOCK_GetFreq(kCLOCK_CoreSysClk) / context->interfaceConfig.smartCardClock));
#endif
    /*  Here the card was activated */
    context->cardParams.active = true;

    return kStatus_SMARTCARD_Success;
}

status_t SMARTCARD_PHY_NCN8025_Deactivate(void *base, smartcard_context_t *context)
{
    if ((NULL == context))
    {
        return kStatus_SMARTCARD_InvalidInput;
    }

#if !(defined(FSL_FEATURE_SOC_EMVSIM_COUNT) && (FSL_FEATURE_SOC_EMVSIM_COUNT))
    uint32_t gpio_base[] = GPIO_BASE_ADDRS;
#endif
/* Tell PHY to start Deactivation sequence by pulling CMD high and reset low */
#if defined(FSL_FEATURE_SOC_EMVSIM_COUNT) && (FSL_FEATURE_SOC_EMVSIM_COUNT)
    ((EMVSIM_Type *)base)->PCSR |= EMVSIM_PCSR_SVCC_EN_MASK;
    ((EMVSIM_Type *)base)->PCSR &= ~EMVSIM_PCSR_VCCENP_MASK;
    ((EMVSIM_Type *)base)->PCSR &= ~EMVSIM_PCSR_SRST_MASK;
#else
    ((GPIO_Type *)gpio_base[context->interfaceConfig.controlPort])->PSOR |= (1u << context->interfaceConfig.controlPin);
    ((GPIO_Type *)gpio_base[context->interfaceConfig.resetPort])->PCOR |= (1u << context->interfaceConfig.resetPin);
#endif
    /* Here the card was deactivated */
    context->cardParams.active = false;

    return kStatus_SMARTCARD_Success;
}

status_t SMARTCARD_PHY_NCN8025_Control(void *base,
                                       smartcard_context_t *context,
                                       smartcard_interface_control_t control,
                                       uint32_t param)
{
    if ((NULL == context))
    {
        return kStatus_SMARTCARD_InvalidInput;
    }

#if !(defined(FSL_FEATURE_SOC_EMVSIM_COUNT) && (FSL_FEATURE_SOC_EMVSIM_COUNT))
    uint32_t gpio_base[] = GPIO_BASE_ADDRS;
#endif

    switch (control)
    {
        case kSMARTCARD_InterfaceSetVcc:
            /* Set card parameter to VCC level set by caller */
            context->interfaceConfig.vcc = (smartcard_card_voltage_class_t)param;
            break;
        case kSMARTCARD_InterfaceSetClockToResetDelay:
            /* Set interface clock to Reset delay set by caller */
            context->interfaceConfig.clockToResetDelay = param;
            break;
        case kSMARTCARD_InterfaceReadStatus:
#if defined(FSL_FEATURE_SOC_EMVSIM_COUNT) && (FSL_FEATURE_SOC_EMVSIM_COUNT)
            /* Expecting active low present detect */
            context->cardParams.present =
                ((emvsim_presence_detect_status_t)((((EMVSIM_Type *)base)->PCSR & EMVSIM_PCSR_SPDP_MASK) >>
                                                   EMVSIM_PCSR_SPDP_SHIFT) == kEMVSIM_DetectPinIsLow);
#else
            if (((GPIO_Type *)gpio_base[context->interfaceConfig.controlPort])->PDIR &
                (1u << context->interfaceConfig.controlPin))
            {
                if (((GPIO_Type *)gpio_base[context->interfaceConfig.irqPort])->PDIR &
                    (1u << context->interfaceConfig.irqPin))
                { /* CMDVCC is high => session is inactive and INT is high => card is present */
                    context->cardParams.present = true;
                    context->cardParams.active = false;
                    context->cardParams.faulty = false;
                    context->cardParams.status = SMARTCARD_NCN8025_STATUS_PRES;
                }
                else
                { /* CMDVCC is high => session is inactive and INT is low => card is absent */
                    context->cardParams.present = false;
                    context->cardParams.active = false;
                    context->cardParams.faulty = false;
                    context->cardParams.status = 0;
                }
            }
            else
            {
                if (((GPIO_Type *)gpio_base[context->interfaceConfig.irqPort])->PDIR &
                    (1u << context->interfaceConfig.irqPin))
                {   /* CMDVCC is low => session is active and INT is high => card is present */
                    context->cardParams.present = true;
                    context->cardParams.active = true;
                    context->cardParams.faulty = false;
                    context->cardParams.status = SMARTCARD_NCN8025_STATUS_PRES | SMARTCARD_NCN8025_STATUS_ACTIVE;
                }
                else
                {   /* CMDVCC is low => session is active and INT is low => card is absent/deactivated due to some fault
                     */
                    /* A fault has been detected (card has been deactivated) but The cause of the deactivation is not
                     * yet known.
                     * Lets determine the cause of fault by pulling CMD high
                     */
                    ((GPIO_Type *)gpio_base[context->interfaceConfig.controlPort])->PSOR |=
                        (1u << context->interfaceConfig.controlPin);

                    if (((GPIO_Type *)gpio_base[context->interfaceConfig.irqPort])->PDIR &
                        (1u << context->interfaceConfig.irqPin))
                    {   /* The fault detected was not a card removal (card is still present) */
                        /* If INT follows CMDVCCN, the fault is due to a supply voltage drop, a VCC over-current
                         * detection or overheating. */
                        context->cardParams.present = true;
                        context->cardParams.active = false;
                        context->cardParams.faulty = true;
                        context->cardParams.status = SMARTCARD_NCN8025_STATUS_PRES | SMARTCARD_NCN8025_STATUS_FAULTY |
                                                     SMARTCARD_NCN8025_STATUS_CARD_DEACTIVATED;
                    }
                    else
                    { /* The fault detected was the card removal
                       * Setting CMDVCCN allows checking if the deactivation is due to card removal.
                       * In this case the INT pin will stay low after CMDVCCN is high.
                       */
                        context->cardParams.present = false;
                        context->cardParams.active = false;
                        context->cardParams.faulty = false;
                        context->cardParams.status =
                            SMARTCARD_NCN8025_STATUS_CARD_REMOVED | SMARTCARD_NCN8025_STATUS_CARD_DEACTIVATED;
                    }
                }
            }
#endif
            break;
        default:
            break;
    }

    return kStatus_SMARTCARD_Success;
}

void SMARTCARD_PHY_NCN8025_IRQHandler(void *base, smartcard_context_t *context)
{
    if ((NULL == context))
    {
        return;
    }
    smartcard_interface_control_t interfaceControl;

    /* Read interface/card status */
    interfaceControl = kSMARTCARD_InterfaceReadStatus;
    SMARTCARD_PHY_NCN8025_Control(base, context, interfaceControl, 0);
    /* Invoke callback if there is one */
    if (NULL != context->interfaceCallback)
    {
        context->interfaceCallback(base, context->interfaceCallbackParam);
    }
}