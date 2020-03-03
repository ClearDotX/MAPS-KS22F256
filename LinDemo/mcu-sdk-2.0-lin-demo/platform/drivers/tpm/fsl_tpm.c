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

#include "fsl_tpm.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Gets the instance from the base address
 *
 * @param base TPM peripheral base address
 *
 * @return The TPM instance
 */
static uint32_t TPM_GetInstance(TPM_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Pointers to TPM bases for each instance. */
static const TPM_Type *s_tpmBases[] = TPM_BASE_PTRS;

/*! @brief Pointers to TPM clocks for each instance. */
static const clock_ip_name_t s_tpmClocks[] = TPM_CLOCKS;

/*******************************************************************************
 * Code
 ******************************************************************************/
static uint32_t TPM_GetInstance(TPM_Type *base)
{
    uint8_t instance = 0;

    while ((s_tpmBases[instance] != base) && (instance < FSL_FEATURE_SOC_TPM_COUNT))
    {
        instance++;
    }

    assert(instance < FSL_FEATURE_SOC_TPM_COUNT);

    return instance;
}

void TPM_Init(TPM_Type *base, const tpm_config_t *config)
{
    assert(config);

    /* Enable the module clock */
    CLOCK_EnableClock(s_tpmClocks[TPM_GetInstance(base)]);

    /* Set the clock prescale factor */
    base->SC = TPM_SC_PS(config->prescale);

    /* Setup the counter operation */
    base->CONF = TPM_CONF_DOZEEN(config->enableDoze) | TPM_CONF_DBGMODE(config->enableDebugMode) |
                 TPM_CONF_GTBEEN(config->useGlobalTimeBase) | TPM_CONF_CROT(config->enableReloadOnTrigger) |
                 TPM_CONF_CSOT(config->enableStartOnTrigger) | TPM_CONF_CSOO(config->enableStopOnOverflow) |
#if defined(FSL_FEATURE_TPM_HAS_PAUSE_COUNTER_ON_TRIGGER) && FSL_FEATURE_TPM_HAS_PAUSE_COUNTER_ON_TRIGGER
                 TPM_CONF_CPOT(config->enablePauseOnTrigger) |
#endif
#if defined(FSL_FEATURE_TPM_HAS_EXTERNAL_TRIGGER_SELECTION) && FSL_FEATURE_TPM_HAS_EXTERNAL_TRIGGER_SELECTION
                 TPM_CONF_TRGSRC(config->triggerSource) |
#endif
                 TPM_CONF_TRGSEL(config->triggerSelect);
}

void TPM_Deinit(TPM_Type *base)
{
    /* Stop the counter */
    base->SC &= ~TPM_SC_CMOD_MASK;
    /* Gate the TPM clock */
    CLOCK_DisableClock(s_tpmClocks[TPM_GetInstance(base)]);
}

void TPM_GetDefaultConfig(tpm_config_t *config)
{
    assert(config);

    /* TPM clock divide by 1 */
    config->prescale = kTPM_Prescale_Divide_1;
    /* Use internal TPM counter as timebase */
    config->useGlobalTimeBase = false;
    /* TPM counter continues in doze mode */
    config->enableDoze = false;
    /* TPM counter pauses when in debug mode */
    config->enableDebugMode = false;
    /* TPM counter will not be reloaded on input trigger */
    config->enableReloadOnTrigger = false;
    /* TPM counter continues running after overflow */
    config->enableStopOnOverflow = false;
    /* TPM counter starts immediately once it is enabled */
    config->enableStartOnTrigger = false;
#if defined(FSL_FEATURE_TPM_HAS_PAUSE_COUNTER_ON_TRIGGER) && FSL_FEATURE_TPM_HAS_PAUSE_COUNTER_ON_TRIGGER
    config->enablePauseOnTrigger = false;
#endif
    /* Choose trigger select 0 as input trigger for controlling counter operation */
    config->triggerSelect = kTPM_Trigger_Select_0;
#if defined(FSL_FEATURE_TPM_HAS_EXTERNAL_TRIGGER_SELECTION) && FSL_FEATURE_TPM_HAS_EXTERNAL_TRIGGER_SELECTION
    /* Choose external trigger source to control counter operation */
    config->triggerSource = kTPM_TriggerSource_External;
#endif
}

status_t TPM_SetupPwm(TPM_Type *base,
                      const tpm_chnl_pwm_signal_param_t *chnlParams,
                      uint8_t numOfChnls,
                      tpm_pwm_mode_t mode,
                      uint32_t pwmFreqHz,
                      uint32_t sourceClockHz)
{
    assert(chnlParams);

    uint32_t mod;
    uint32_t tpmClock = (sourceClockHz / (1U << (base->SC & TPM_SC_PS_MASK)));
    uint16_t cnv;
    uint8_t i;

    switch (mode)
    {
        case kTPM_EdgeAlignedPwm:
            base->SC &= ~TPM_SC_CPWMS_MASK;
            mod = (tpmClock / pwmFreqHz) - 1;
            break;
        case kTPM_CenterAlignedPwm:
            base->SC |= TPM_SC_CPWMS_MASK;
            mod = tpmClock / (pwmFreqHz * 2);
            break;
        default:
            return kStatus_Fail;
    }

    /* Return an error in case we overflow the registers, probably would require changing
     * clock source to get the desired frequency */
    if (mod > 65535U)
    {
        return kStatus_Fail;
    }
    /* Set the PWM period */
    base->MOD = mod;

    /* Setup each TPM channel */
    for (i = 0; i < numOfChnls; i++)
    {
        /* Return error if requested dutycycle is greater than the max allowed */
        if (chnlParams->dutyCyclePercent > 100)
        {
            return kStatus_Fail;
        }

        if (chnlParams->dutyCyclePercent == 0)
        {
            /* Signal stays low */
            cnv = 0;
        }
        else
        {
            cnv = (mod * chnlParams->dutyCyclePercent) / 100;
            /* For 100% duty cycle */
            if (cnv >= mod)
            {
                cnv = mod + 1;
            }
        }

        base->CONTROLS[chnlParams->chnlNumber].CnV = cnv;

        /* When switching mode, disable channel first  */
        base->CONTROLS[chnlParams->chnlNumber].CnSC &=
            ~(TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK);

        /* Set the requested PWM mode, PWM output requires mode select to be set to 2  */
        base->CONTROLS[chnlParams->chnlNumber].CnSC |=
            ((chnlParams->level << TPM_CnSC_ELSA_SHIFT) | (2U << TPM_CnSC_MSA_SHIFT));

        /* Wait till mode change is acknowledged */
        while (!(base->CONTROLS[chnlParams->chnlNumber].CnSC &
                 (TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK)))
        {
        }
        chnlParams++;
    }

    return kStatus_Success;
}

void TPM_UpdatePwmDutycycle(TPM_Type *base,
                            tpm_chnl_t chnlNumber,
                            tpm_pwm_mode_t currentPwmMode,
                            uint8_t dutyCyclePercent)
{
    uint16_t cnv, mod;

    mod = base->MOD;
    cnv = (mod * dutyCyclePercent) / 100;
    /* For 100% duty cycle */
    if (cnv >= mod)
    {
        cnv = mod + 1;
    }

    base->CONTROLS[chnlNumber].CnV = cnv;
}

void TPM_UpdateChnlEdgeLevelSelect(TPM_Type *base, tpm_chnl_t chnlNumber, uint8_t level)
{
    uint32_t reg = base->CONTROLS[chnlNumber].CnSC;

    /* When switching mode, disable channel first  */
    base->CONTROLS[chnlNumber].CnSC &=
        ~(TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK);

    /* Clear the field and write the new level value */
    reg &= ~(TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK);
    reg |= ((uint32_t)level << TPM_CnSC_ELSA_SHIFT) & (TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK);

    base->CONTROLS[chnlNumber].CnSC = reg;

    /* Wait till mode change is acknowledged */
    while (!(base->CONTROLS[chnlNumber].CnSC &
             (TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK)))
    {
    }
}

void TPM_SetupInputCapture(TPM_Type *base, tpm_chnl_t chnlNumber, tpm_input_capture_edge_t captureMode)
{
    /* When switching mode, disable channel first  */
    base->CONTROLS[chnlNumber].CnSC &=
        ~(TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK);

    /* Set the requested input capture mode */
    base->CONTROLS[chnlNumber].CnSC |= captureMode;

    /* Wait till mode change is acknowledged */
    while (!(base->CONTROLS[chnlNumber].CnSC &
             (TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK)))
    {
    }
}

void TPM_SetupOutputCompare(TPM_Type *base,
                            tpm_chnl_t chnlNumber,
                            tpm_output_compare_mode_t compareMode,
                            uint32_t compareValue)
{
    /* Setup the compare value */
    base->CONTROLS[chnlNumber].CnV = compareValue;

    /* When switching mode, disable channel first  */
    base->CONTROLS[chnlNumber].CnSC &=
        ~(TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK);

    /* Setup the channel output behaviour when a match occurs with the compare value */
    base->CONTROLS[chnlNumber].CnSC |= compareMode;

    /* Wait till mode change is acknowledged */
    while (!(base->CONTROLS[chnlNumber].CnSC &
             (TPM_CnSC_MSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSB_MASK)))
    {
    }
}

void TPM_EnableInterrupts(TPM_Type *base, uint32_t mask)
{
    uint32_t chnlInterrupts = (mask & 0xFF);
    uint8_t chnlNumber = 0;

    /* Enable the timer overflow interrupt */
    if (mask & kTPM_TimeOverflowInterruptEnable)
    {
        base->SC |= TPM_SC_TOIE_MASK;
    }

    /* Enable the channel interrupts */
    while (chnlInterrupts)
    {
        if (chnlInterrupts & 0x1)
        {
            base->CONTROLS[chnlNumber].CnSC |= TPM_CnSC_CHIE_MASK;
        }
        chnlNumber++;
        chnlInterrupts = chnlInterrupts >> 1U;
    }
}

void TPM_DisableInterrupts(TPM_Type *base, uint32_t mask)
{
    uint32_t chnlInterrupts = (mask & 0xFF);
    uint8_t chnlNumber = 0;

    /* Disable the timer overflow interrupt */
    if (mask & kTPM_TimeOverflowInterruptEnable)
    {
        base->SC &= ~TPM_SC_TOIE_MASK;
    }

    /* Disable the channel interrupts */
    while (chnlInterrupts)
    {
        if (chnlInterrupts & 0x1)
        {
            base->CONTROLS[chnlNumber].CnSC &= ~TPM_CnSC_CHIE_MASK;
        }
        chnlNumber++;
        chnlInterrupts = chnlInterrupts >> 1U;
    }
}

uint32_t TPM_GetEnabledInterrupts(TPM_Type *base)
{
    uint32_t enabledInterrupts = 0;
    uint8_t chnlCount = 0;
    uint8_t allTpmsChnlCnt[] = FSL_FEATURE_TPM_CHANNEL_COUNTx;

    /* Check if timer overflow interrupt is enabled */
    if (base->SC & TPM_SC_TOIE_MASK)
    {
        enabledInterrupts |= kTPM_TimeOverflowInterruptEnable;
    }

    /* Get the number of channels for this TPM instance */
    chnlCount = allTpmsChnlCnt[TPM_GetInstance(base)];

    /* Check if the channel interrupts are enabled */
    while (chnlCount > 0)
    {
        chnlCount--;
        if (base->CONTROLS[chnlCount].CnSC & TPM_CnSC_CHIE_MASK)
        {
            enabledInterrupts |= (1U << chnlCount);
        }
    }

    return enabledInterrupts;
}

uint32_t TPM_GetStatusFlags(TPM_Type *base)
{
    uint32_t statusFlags = 0;

    /* Check timer flag */
    if (base->SC & TPM_SC_TOF_MASK)
    {
        statusFlags |= kTPM_TimeOverflowFlag;
    }

    statusFlags |= base->STATUS;

    return statusFlags;
}

void TPM_ClearStatusFlags(TPM_Type *base, uint32_t mask)
{
    /* Clear the timer overflow flag */
    if (mask & kTPM_TimeOverflowFlag)
    {
        base->SC |= TPM_SC_TOF_MASK;
    }

    /* Clear the channel status flags */
    base->STATUS = (mask & 0xFF);
}
