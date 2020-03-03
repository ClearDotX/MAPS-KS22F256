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

#include "fsl_clock_MKL28Z7.h"
#include "fsl_scg.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define PCC_PCS_VAL(reg)  ((reg & PCC_CLKCFG_PCS_MASK)  >> PCC_CLKCFG_PCS_SHIFT)
#define PCC_FRAC_VAL(reg) ((reg & PCC_CLKCFG_FRAC_MASK) >> PCC_CLKCFG_FRAC_SHIFT)
#define PCC_PCD_VAL(reg)  ((reg & PCC_CLKCFG_PCD_MASK)  >> PCC_CLKCFG_PCD_SHIFT)

/*******************************************************************************
 * Code
 ******************************************************************************/
static uint32_t CLOCK_GetErClkFreq(void)
{
    if (SCG->SOSCCSR & SCG_SOSCCSR_SOSCEN_MASK)
    {
        return g_xtal0Freq;
    }
    else
    {
        return 0U;
    }
}

uint32_t CLOCK_GetFreq(clock_name_t clockName)
{
    uint32_t freq;

    switch (clockName)
    {
        case kCLOCK_CoreSysClk:
        case kCLOCK_PlatClk:
            freq = CLOCK_GetSysClkFreq(kSCG_SysClkCore);
            break;
        case kCLOCK_BusClk:
        case kCLOCK_FlashClk:
            freq = CLOCK_GetSysClkFreq(kSCG_SysClkSlow);
            break;
        case kCLOCK_ScgSysOscClk:
            freq = CLOCK_GetSysOscFreq();
            break;
        case kCLOCK_ScgSircClk:
            freq = CLOCK_GetSircFreq();
            break;
        case kCLOCK_ScgFircClk:
            freq = CLOCK_GetFircFreq();
            break;
        case kCLOCK_ScgSysPllClk:
            freq = CLOCK_GetSysPllFreq();
            break;
        case kCLOCK_ScgSysOscAsyncDiv1Clk:
            freq = CLOCK_GetSysOscAsyncFreq(kSCG_AsyncDiv1Clk);
            break;
        case kCLOCK_ScgSysOscAsyncDiv2Clk:
            freq = CLOCK_GetSysOscAsyncFreq(kSCG_AsyncDiv2Clk);
            break;
        case kCLOCK_ScgSysOscAsyncDiv3Clk:
            freq = CLOCK_GetSysOscAsyncFreq(kSCG_AsyncDiv3Clk);
            break;
        case kCLOCK_ScgSircAsyncDiv1Clk:
            freq = CLOCK_GetSircAsyncFreq(kSCG_AsyncDiv1Clk);
            break;
        case kCLOCK_ScgSircAsyncDiv2Clk:
            freq = CLOCK_GetSircAsyncFreq(kSCG_AsyncDiv2Clk);
            break;
        case kCLOCK_ScgSircAsyncDiv3Clk:
            freq = CLOCK_GetSircAsyncFreq(kSCG_AsyncDiv3Clk);
            break;
        case kCLOCK_ScgFircAsyncDiv1Clk:
            freq = CLOCK_GetFircAsyncFreq(kSCG_AsyncDiv1Clk);
            break;
        case kCLOCK_ScgFircAsyncDiv2Clk:
            freq = CLOCK_GetFircAsyncFreq(kSCG_AsyncDiv2Clk);
            break;
        case kCLOCK_ScgFircAsyncDiv3Clk:
            freq = CLOCK_GetFircAsyncFreq(kSCG_AsyncDiv3Clk);
            break;
        case kCLOCK_ScgSysPllAsyncDiv1Clk:
            freq = CLOCK_GetSysPllAsyncFreq(kSCG_AsyncDiv1Clk);
            break;
        case kCLOCK_ScgSysPllAsyncDiv2Clk:
            freq = CLOCK_GetSysPllAsyncFreq(kSCG_AsyncDiv2Clk);
            break;
        case kCLOCK_ScgSysPllAsyncDiv3Clk:
            freq = CLOCK_GetSysPllAsyncFreq(kSCG_AsyncDiv3Clk);
            break;
        case kCLOCK_LpoClk:
            freq = LPO_CLK_FREQ;
            break;
        case kCLOCK_Osc32kClk:
            freq = (CLOCK_GetErClkFreq() == 32768U) ? 32768U : 0U;
            break;
        case kCLOCK_ErClk:
            freq = CLOCK_GetErClkFreq();
            break;
        default:
            freq = 0U;
            break;
    }
    return freq;
}

uint32_t CLOCK_GetIpFreq(clock_ip_name_t name)
{
    uint32_t reg = (*(volatile uint32_t *)name);

    scg_async_clk_t asycClk;
    uint32_t freq;

    assert(reg & PCC_CLKCFG_PR_MASK);

    /* USB uses SCG DIV1 clock, others use SCG DIV3 clock. */
    if (kCLOCK_Usbfs0 == name)
    {
        asycClk = kSCG_AsyncDiv1Clk;
    }
    else
    {
        asycClk = kSCG_AsyncDiv3Clk;
    }

    switch (PCC_PCS_VAL(reg))
    {
        case kCLOCK_IpSrcSysOscAsync:
            freq = CLOCK_GetSysOscAsyncFreq(asycClk);
            break;
        case kCLOCK_IpSrcSircAsync:
            freq = CLOCK_GetSircAsyncFreq(asycClk);
            break;
        case kCLOCK_IpSrcFircAsync:
            freq = CLOCK_GetFircAsyncFreq(asycClk);
            break;
        case kCLOCK_IpSrcSysPllAsync:
            freq = CLOCK_GetSysPllAsyncFreq(asycClk);
            break;
        default:
            freq = 0U;
            break;
    }

    if (reg & (PCC_CLKCFG_PCD_MASK | PCC_CLKCFG_FRAC_MASK))
    {
        return freq;
    }
    else
    {
        return freq * (PCC_FRAC_VAL(reg) + 1U) / (PCC_PCD_VAL(reg) + 1U);
    }
}
