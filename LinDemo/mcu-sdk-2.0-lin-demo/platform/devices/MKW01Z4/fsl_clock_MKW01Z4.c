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

#include "fsl_clock_MKW01Z4.h"
#include "fsl_mcg.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SIM_CLKDIV1_OUTDIV1_VAL ((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> SIM_CLKDIV1_OUTDIV1_SHIFT)
#define SIM_CLKDIV1_OUTDIV4_VAL ((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV4_MASK) >> SIM_CLKDIV1_OUTDIV4_SHIFT)
#define SIM_SOPT2_PLLFLLSEL_VAL ((SIM->SOPT2   & SIM_SOPT2_PLLFLLSEL_MASK) >> SIM_SOPT2_PLLFLLSEL_SHIFT)
#define SIM_SOPT1_OSC32KSEL_VAL ((SIM->SOPT1   & SIM_SOPT1_OSC32KSEL_MASK) >> SIM_SOPT1_OSC32KSEL_SHIFT)

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* XTAL0 clock frequency. */
extern uint32_t g_xtal0Freq;
extern uint32_t g_xtal32Freq;

/*******************************************************************************
 * Code
 ******************************************************************************/
uint32_t CLOCK_GetOsc0ErClkFreq(void)
{
    if (OSC0->CR & OSC_CR_ERCLKEN_MASK)
    {
        return g_xtal0Freq;
    }
    else
    {
        return 0U;
    }
}

uint32_t CLOCK_GetEr32kFreq(void)
{
    uint32_t freq;

    switch (SIM_SOPT1_OSC32KSEL_VAL)
    {
        case 0U:      /* OSC 32k clock  */
            freq = (g_xtal0Freq == 32768U) ? 32768U : 0U;
            break;
        case 2U:      /* RTC 32k clock  */
            freq = g_xtal32Freq;
            break;
        case 3U:      /* LPO clock      */
            freq = LPO_CLK_FREQ;
            break;
        default:
            freq = 0U;
            break;
    }
    return freq;
}

uint32_t CLOCK_GetPllFllSelClkFreq(void)
{
    uint32_t freq;

    switch (SIM_SOPT2_PLLFLLSEL_VAL)
    {
        case 0U:      /* FLL. */
            freq = CLOCK_GetFllFreq();
            break;
        case 1U:      /* PLL. */
            freq = CLOCK_GetPll0Freq();
            freq >>= 1U;
            break;
        default:
            freq = 0U;
            break;
    }

    return freq;
}

uint32_t CLOCK_GetFreq(clock_name_t clockName)
{
    uint32_t freq;

    switch (clockName)
    {
        case kCLOCK_CoreSysClk:
        case kCLOCK_PlatClk:
            freq = CLOCK_GetOutClkFreq() / (SIM_CLKDIV1_OUTDIV1_VAL + 1);
            break;
        case kCLOCK_BusClk:
        case kCLOCK_FlashClk:
            freq = CLOCK_GetOutClkFreq() / (SIM_CLKDIV1_OUTDIV1_VAL + 1);
            freq /= (SIM_CLKDIV1_OUTDIV4_VAL + 1);
            break;
        case kCLOCK_PllFllSelClk:
            freq = CLOCK_GetPllFllSelClkFreq();
            break;
        case kCLOCK_Er32kClk:
            freq = CLOCK_GetEr32kFreq();
            break;
        case kCLOCK_McgFixedFreqClk:
            freq = CLOCK_GetFixedFreqClkFreq();
            break;
        case kCLOCK_McgInternalRefClk:
            freq = CLOCK_GetInternalRefClkFreq();
            break;
        case kCLOCK_McgFllClk:
            freq = CLOCK_GetFllFreq();
            break;
        case kCLOCK_McgPll0Clk:
            freq = CLOCK_GetPll0Freq();
            break;
        case kCLOCK_LpoClk:
            freq = LPO_CLK_FREQ;
            break;
        case kCLOCK_Osc0ErClk:
            freq = CLOCK_GetOsc0ErClkFreq();
            break;
        default:
            freq = 0U;
            break;
    }

    return freq;
}

void CLOCK_SetSimConfig(sim_clock_config_t const * config)
{
    SIM->CLKDIV1 = config->clkdiv1;
    CLOCK_SetPllFllSelClock(config->pllFllSel);
    CLOCK_SetEr32kClock(config->er32kSrc);
}
