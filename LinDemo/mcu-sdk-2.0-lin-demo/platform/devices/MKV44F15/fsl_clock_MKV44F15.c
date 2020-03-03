/*
 * Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
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

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "fsl_device_registers.h"
#include "fsl_mcg_hal.h"
#include "fsl_sim_hal.h"
#include "fsl_osc_hal.h"
#include "fsl_clock_manager.h"
#include "fsl_mcg_hal_modes.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Table of base addresses for instances. */
extern SIM_Type * const g_simBase[];
extern MCG_Type * const g_mcgBase[];

uint32_t g_ftmClkFreq[FTM_EXT_CLK_COUNT];          /* FTM_CLK0         */

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_FllStableDelay
 * Description   : This funtion is used to delay for FLL stable.
 * According to datasheet, every time the FLL reference source or reference
 * divider is changed, trim value is changed, DMX32 bit is changed, DRS bits
 * are changed, or changing from FLL disabled (BLPE, BLPI) to FLL enabled
 * (FEI, FEE, FBE, FBI), there should be 1ms delay for FLL stable. Please
 * check datasheet for t(fll_aquire).
 *
 *END**************************************************************************/
static void CLOCK_SYS_FllStableDelay(void)
{
    uint32_t coreClk = CLOCK_SYS_GetCoreClockFreq();

    coreClk /= 3000U;

    // Delay 1 ms.
    while (coreClk--)
    {
        __asm ("nop");
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_SetSimConfigration
 * Description   : This funtion sets the SIM registers for clock transitiom.
 *
 *END**************************************************************************/
static void CLOCK_SYS_SetSimConfigration(sim_config_t const *simConfig)
{
    CLOCK_HAL_SetOutDiv(SIM,
                        simConfig->outdiv1,
                        simConfig->outdiv2,
                        simConfig->outdiv3,
                        simConfig->outdiv4);

    CLOCK_HAL_SetExternalRefClock32kSrc(SIM, simConfig->er32kSrc);
}

uint32_t CLOCK_SYS_GetDmaFreq(uint32_t instance)
{
    uint32_t freq = 0;
    CLOCK_SYS_GetFreq(kSystemClock, &freq);
    return freq;
}

uint32_t CLOCK_SYS_GetDmamuxFreq(uint32_t instance)
{
    uint32_t freq = 0;
    CLOCK_SYS_GetFreq(kBusClock, &freq);
    return freq;
}

uint32_t CLOCK_SYS_GetPortFreq(uint32_t instance)
{
    uint32_t freq = 0;
    CLOCK_SYS_GetFreq(kLpoClock, &freq);
    return freq;
}
/* PORT instance table. */
static const sim_clock_gate_name_t portGateTable[] =
{
    kSimClockGatePortA,
    kSimClockGatePortB,
    kSimClockGatePortC,
    kSimClockGatePortD,
    kSimClockGatePortE
};

/*! @brief CLOCK name config table for KV46*/
const clock_name_config_t kClockNameConfigTable [] =  {
    {false, kSystemClock,     kClockDividerOutdiv1},    /* Core clock divider */
    {false, kSystemClock,     kClockDividerOutdiv1},    /* System clock divider */
    {false, kSystemClock,     kClockDividerOutdiv1},    /* Not used for KV4x */
    {false, kSystemClock,     kClockDividerOutdiv4},    /* Bus clock divider */
    {false, kSystemClock,     kClockDividerOutdiv1},    /* Not used for KV4x*/
    {false, kSystemClock,     kClockDividerOutdiv4},    /* Flash clock divider */
    {false, kSystemClock,     kClockDividerOutdiv2}     /* Fast peripheral clock divider */
};

void CLOCK_SYS_EnablePortClock(uint32_t instance)
{
    assert(instance < sizeof(portGateTable)/sizeof(portGateTable[0]));

    SIM_HAL_EnableClock(SIM, portGateTable[instance]);
}

void CLOCK_SYS_DisablePortClock(uint32_t instance)
{
    assert(instance < sizeof(portGateTable)/sizeof(portGateTable[0]));

    SIM_HAL_DisableClock(SIM, portGateTable[instance]);
}

bool CLOCK_SYS_GetPortGateCmd(uint32_t instance)
{
    assert(instance < sizeof(portGateTable)/sizeof(portGateTable[0]));

    return SIM_HAL_GetGateCmd(SIM, portGateTable[instance]);
}

clock_manager_error_code_t CLOCK_SYS_GetSysClkFreq(clock_names_t clockName,
                                                        uint32_t *frequency)
{
    const clock_name_config_t *table = &kClockNameConfigTable[clockName];

    /* check if we need to use a reference clock*/
    if (table->useOtherRefClock)
    {
        /* get other specified ref clock*/
        if ( kClockManagerSuccess != CLOCK_SYS_GetFreq(table->otherRefClockName,
                                                                    frequency) )
        {
            return kClockManagerNoSuchClockName;
        }
        else
        {
          return kClockManagerSuccess;
        }
    }
    else
    {
        /* get default ref clock */
       if ((table->dividerName)==kClockDividerOutdiv1)
       {
          *frequency =(CLOCK_HAL_GetOutClk(g_mcgBase[0])/(CLOCK_HAL_GetOutDiv1(SIM) + 1));
       }
       else  if ((table->dividerName)==kClockDividerOutdiv2)
       {
          *frequency =(CLOCK_HAL_GetOutClk(g_mcgBase[0])/(CLOCK_HAL_GetOutDiv2(SIM) + 1));
       }
       else  if ((table->dividerName)==kClockDividerOutdiv4)
       {
          *frequency =(CLOCK_HAL_GetOutClk(g_mcgBase[0])/(CLOCK_HAL_GetOutDiv4(SIM) + 1));
       }
        return kClockManagerSuccess;
    }
}

clock_manager_error_code_t CLOCK_SYS_SetConfiguration(clock_manager_user_config_t const* config)
{
    assert(NULL != config);

    /* Set outdiv for safe output clock frequency. */
    CLOCK_HAL_SetOutDiv(SIM, 0U, 1U, 2U, 5U);

    /* Set MCG mode. */
    CLOCK_SYS_SetMcgMode(&config->mcgConfig, CLOCK_SYS_FllStableDelay);

    /* Set SIM setting. */
    CLOCK_SYS_SetSimConfigration(&config->simConfig);

    /* Set OSCERCLK setting. */
    CLOCK_SYS_SetOscerConfigration(0, &config->oscerConfig);
    SystemCoreClock = CLOCK_SYS_GetCoreClockFreq();

    return kClockManagerSuccess;
}

clock_manager_error_code_t CLOCK_SYS_GetFreq(clock_names_t clockName,
                                                 uint32_t *frequency)
{
    clock_manager_error_code_t returnCode = kClockManagerSuccess;

    switch (clockName)
    {
        case kCoreClock:
        case kSystemClock:
            *frequency = CLOCK_SYS_GetCoreClockFreq();
            break;
        case kPlatformClock:
            *frequency = CLOCK_SYS_GetSystemClockFreq();
            break;
        case kBusClock:
            *frequency = CLOCK_SYS_GetBusClockFreq();
            break;
        case kFlashClock:
            *frequency = CLOCK_SYS_GetFlashClockFreq();
            break;
        case kFastPeripheralClock:
            returnCode = CLOCK_SYS_GetSysClkFreq(clockName, frequency);
        break;    
        case kOsc0ErClock:
            *frequency = CLOCK_SYS_GetOsc0ExternalRefClockFreq();
            break;
        case kOsc0ErClockUndiv:
            *frequency = CLOCK_SYS_GetOsc0ExternalRefClockUndivFreq();
            break;
        case kMcgFfClock:
            *frequency = CLOCK_SYS_GetFixedFreqClockFreq();
            break;
        case kMcgFllClock:
            *frequency = CLOCK_HAL_GetFllClk(MCG);
            break;
        case kMcgPll0Clock:
            *frequency = CLOCK_HAL_GetPll0Clk(MCG);
            break;
        case kMcgOutClock:
            *frequency = CLOCK_HAL_GetOutClk(MCG);
            break;
        case kMcgIrClock:
            *frequency = CLOCK_HAL_GetInternalRefClk(MCG);
            break;
#if FSL_FEATURE_MCG_HAS_IRC_48M            
        case kIrc48mClock:
            *frequency = CPU_INTERNAL_IRC_48M; 
            break;
#endif            
        case kLpoClock:
            *frequency = CLOCK_SYS_GetLpoClockFreq();
            break;
        case kSystickClock:
            *frequency = CLOCK_SYS_GetSystickFreq();
            break;
        default:
            *frequency = 0U;
            returnCode = kClockManagerNoSuchClockName;
            break;
    }

    return returnCode;
}

uint32_t CLOCK_SYS_GetFlashClockFreq(void)
{
    return CLOCK_HAL_GetOutClk(MCG) / (CLOCK_HAL_GetOutDiv4(SIM) + 1);
}

uint32_t CLOCK_SYS_GetCoreClockFreq(void)
{
    return CLOCK_HAL_GetOutClk(MCG) / (CLOCK_HAL_GetOutDiv1(SIM) + 1);
}

uint32_t CLOCK_SYS_GetSystemClockFreq(void)
{
    return CLOCK_HAL_GetOutClk(MCG) / (CLOCK_HAL_GetOutDiv1(SIM) + 1);
}

uint32_t CLOCK_SYS_GetFastPeripheralClockFreq(void)
{
    return CLOCK_HAL_GetOutClk(MCG) / (CLOCK_HAL_GetOutDiv2(SIM) + 1);
}

uint32_t CLOCK_SYS_GetBusClockFreq(void)
{
    return CLOCK_HAL_GetOutClk(MCG) / (CLOCK_HAL_GetOutDiv4(SIM) + 1);
}

uint32_t CLOCK_SYS_GetExternalRefClock32kFreq(void)
{
    clock_er32k_src_t src;
    uint32_t freq;

    src = CLOCK_HAL_GetExternalRefClock32kSrc(SIM);

    switch (src)
    {
        case kClockEr32kSrcOsc0:      /* OSC 32k clock  */
            freq = (32768U == g_xtal0ClkFreq) ? 32768U : 0U;
            break;
        case kClockEr32kSrcLpo:         /* LPO clock      */
            freq = CLOCK_SYS_GetLpoClockFreq();
            break;
        default:
            freq = 0U;
            break;
    }

    return freq;
}

uint32_t CLOCK_SYS_GetOsc0ExternalRefClockFreq(void)
{
    return CLOCK_SYS_GetOsc0ExternalRefClockUndivFreq()
        >> OSC_HAL_GetExternalRefClkDiv(g_oscBase[0]);
}

uint32_t CLOCK_SYS_GetOsc0ExternalRefClockUndivFreq(void)
{
    if (OSC_HAL_GetExternalRefClkCmd(g_oscBase[0]))
    {
        return g_xtal0ClkFreq;
    }
    else
    {
        return 0U;
    }
}

uint32_t CLOCK_SYS_GetLptmrFreq(uint32_t instance, clock_lptmr_src_t lptmrSrc)
{
    uint32_t freq;

    switch (lptmrSrc)
    {
        case kClockLptmrSrcMcgIrClk:        /* MCG out clock  */
            freq = CLOCK_HAL_GetInternalRefClk(MCG);
            break;
        case kClockLptmrSrcLpoClk:             /* LPO clock     */
            freq = CLOCK_SYS_GetLpoClockFreq();
            break;
        case kClockLptmrSrcEr32kClk:        /* ERCLK32K clock */
            freq = CLOCK_SYS_GetExternalRefClock32kFreq();
            break;
        case kClockLptmrSrcOsc0erClkUndiv:        /* OSCERCLK_UNDIV clock */
            freq = CLOCK_SYS_GetOsc0ExternalRefClockUndivFreq();
            break;
        default:
            freq = 0U;
            break;
    }

    return freq;
}

uint32_t CLOCK_SYS_GetEwmFreq(uint32_t instance)
{
    uint32_t freq = 0;
    CLOCK_SYS_GetFreq(kLpoClock, &freq);
    return freq;
}

uint32_t CLOCK_SYS_GetFtfFreq(uint32_t instance)
{
    uint32_t freq = 0;
    CLOCK_SYS_GetFreq(kFlashClock, &freq);
    return freq;
}

uint32_t CLOCK_SYS_GetCrcFreq(uint32_t instance)
{
    uint32_t freq = 0;
    CLOCK_SYS_GetFreq(kSystemClock, &freq);
    return freq;
}

uint32_t CLOCK_SYS_GetAdcFreq(uint32_t instance)
{
    uint32_t freq = 0;
    CLOCK_SYS_GetFreq(kMcgIrClock, &freq);
    return freq;
}

uint32_t CLOCK_SYS_GetCmpFreq(uint32_t instance)
{
    uint32_t freq = 0;
    CLOCK_SYS_GetFreq(kBusClock, &freq);
    return freq;
}

uint32_t CLOCK_SYS_GetPdbFreq(uint32_t instance)
{
    uint32_t freq = 0;
    CLOCK_SYS_GetFreq(kFastPeripheralClock, &freq);
    return freq;
}

uint32_t CLOCK_SYS_GetPwmFreq(uint32_t instance)
{
    uint32_t freq = 0;
    CLOCK_SYS_GetFreq(kFastPeripheralClock, &freq);
    return freq;
}

uint32_t CLOCK_SYS_GetFtmExternalFreq(uint32_t instance)
{
    sim_ftm_clk_sel_t sel = SIM_HAL_GetFtmExternalClkPinMode(SIM, instance);

    if (kSimFtmClkSel0 == sel)
    {
        return g_ftmClkFreq[0];
    }
    else
    {
        return g_ftmClkFreq[1];
    }
}
uint32_t CLOCK_SYS_GetPitFreq(uint32_t instance)
{
    uint32_t freq = 0;
    CLOCK_SYS_GetFreq(kBusClock, &freq);
    return freq;
}

uint32_t CLOCK_SYS_GetSpiFreq(uint32_t instance)
{
    uint32_t freq = 0;
    CLOCK_SYS_GetFreq(kFastPeripheralClock, &freq);
    return freq;
}

uint32_t CLOCK_SYS_GetI2cFreq(uint32_t instance)
{
    uint32_t freq = 0;
    CLOCK_SYS_GetFreq(kBusClock, &freq);
    return freq;
}

uint32_t CLOCK_SYS_GetUartFreq(uint32_t instance)
{
    uint32_t freq = 0;
    CLOCK_SYS_GetFreq(kFastPeripheralClock, &freq);
    return freq;
}

uint32_t CLOCK_SYS_GetGpioFreq(uint32_t instance)
{
    uint32_t freq = 0;
    CLOCK_SYS_GetFreq(kSystemClock, &freq);
    return freq;
}

uint32_t CLOCK_SYS_GetEncFreq(uint32_t instance)
{
    uint32_t freq = 0;
    CLOCK_SYS_GetFreq(kFastPeripheralClock, &freq);
    return freq;
}

uint32_t CLOCK_SYS_GetWdogFreq(uint32_t instance, clock_wdog_src_t wdogSrc)
{
    if (kClockWdogSrcLpoClk == wdogSrc)
    {
        return CLOCK_SYS_GetLpoClockFreq();
    }
    else
    {
        return CLOCK_SYS_GetBusClockFreq();
    }
}

uint32_t CLOCK_SYS_GetXbarFreq(uint32_t instance)
{
    uint32_t freq = 0;
    CLOCK_SYS_GetFreq(kBusClock, &freq);
    return freq;
}

uint32_t CLOCK_SYS_GetAoiFreq(uint32_t instance)
{
    uint32_t freq = 0;
    CLOCK_SYS_GetFreq(kBusClock, &freq);
    return freq;
}

uint32_t CLOCK_SYS_GetFlexcanFreq(uint8_t instance, clock_flexcan_src_t flexcanSrc)
{
    uint32_t freq = 0;
    if (kClockFlexcanSrcOsc0erClk == flexcanSrc)
    {
        return CLOCK_SYS_GetOsc0ExternalRefClockFreq();
    }
    else
    { 
        CLOCK_SYS_GetFreq(kFastPeripheralClock, &freq);
        return freq;
    }
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
