/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
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
#include "fsl_clock_manager.h"

/*
 * README:
 * This file should provide these APIs:
 * 1. APIs to get the frequency of output clocks in Reference Manual ->
 * Chapter Clock Distribution -> Figure Clocking diagram.
 * 2. APIs for IP modules listed in Reference Manual -> Chapter Clock Distribution
 * -> Module clocks.
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
uint32_t g_usbClkInFreq[USB_EXT_CLK_COUNT];        /* USB_CLKIN        */
uint32_t g_ftmClkFreq[FTM_EXT_CLK_COUNT];          /* FTM_CLK          */

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
                        0U,
                        simConfig->outdiv4);

    CLOCK_HAL_SetPllfllSel(SIM, simConfig->pllFllSel);

    CLOCK_HAL_SetExternalRefClock32kSrc(SIM, simConfig->er32kSrc);
}

clock_manager_error_code_t CLOCK_SYS_SetConfiguration(clock_manager_user_config_t const* config)
{
    assert(NULL != config);

    /* Set outdiv for safe output clock frequency. */

    CLOCK_HAL_SetOutDiv(SIM, 1U, 1U, 0U, 7U);

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
        case kOsc32kClock:
            *frequency = CLOCK_SYS_GetExternalRefClock32kFreq();
            break;
        case kOsc0ErClock:
            *frequency = CLOCK_SYS_GetOsc0ExternalRefClockFreq();
            break;
        case kOsc0ErClockUndiv:
            *frequency = CLOCK_SYS_GetOsc0ExternalRefClockUndivFreq();
            break;
        case kRtcoutClock:
            *frequency = CLOCK_SYS_GetRtcOutFreq();
            break;
        case kMcgFfClock:
            *frequency = CLOCK_SYS_GetFixedFreqClockFreq();
            break;
        case kMcgFllClock:
            *frequency = CLOCK_HAL_GetFllClk(MCG);
            break;
        case kMcgOutClock:
            *frequency = CLOCK_HAL_GetOutClk(MCG);
            break;
        case kMcgIrClock:
            *frequency = CLOCK_HAL_GetInternalRefClk(MCG);
            break;
        case kIrc48mClock:
            *frequency = CPU_INTERNAL_IRC_48M; 
            break;
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

uint32_t CLOCK_SYS_GetCoreClockFreq(void)
{
    return CLOCK_HAL_GetOutClk(MCG) / (CLOCK_HAL_GetOutDiv1(SIM) + 1);
}

uint32_t CLOCK_SYS_GetSystemClockFreq(void)
{
    return CLOCK_HAL_GetOutClk(MCG) / (CLOCK_HAL_GetOutDiv1(SIM) + 1);
}

uint32_t CLOCK_SYS_GetBusClockFreq(void)
{
    return CLOCK_HAL_GetOutClk(MCG) / (CLOCK_HAL_GetOutDiv2(SIM) + 1);
}

uint32_t CLOCK_SYS_GetFlashClockFreq(void)
{
    return CLOCK_HAL_GetOutClk(MCG) / (CLOCK_HAL_GetOutDiv4(SIM) + 1);
}

uint32_t CLOCK_SYS_GetPllFllClockFreq(void)
{
    uint32_t freq;
    clock_pllfll_sel_t src;

    src = CLOCK_HAL_GetPllfllSel(SIM);

    switch (src)
    {
        case kClockPllFllSelFll:
            freq = CLOCK_HAL_GetFllClk(MCG);
            break;
        case kClockPllFllSelIrc48M:
            freq = CPU_INTERNAL_IRC_48M;
            break;
        default:
            freq = 0U;
            break;
    }
    return freq;
}

uint32_t CLOCK_SYS_GetRtcOutFreq(void)
{
    if (kClockRtcoutSrc1Hz == CLOCK_SYS_GetRtcOutSrc())
    {
        return g_xtalRtcClkFreq >> 15U;
    }
    else
    {
        return g_xtalRtcClkFreq;
    }
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
        case kClockEr32kSrcRtc:      /* RTC 32k clock  */
            freq = g_xtalRtcClkFreq;
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

uint32_t CLOCK_SYS_GetOsc0ExternalRefClockFreq(void)
{
    return CLOCK_SYS_GetOsc0ExternalRefClockUndivFreq()
        >> OSC_HAL_GetExternalRefClkDiv(g_oscBase[0]);
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

uint32_t CLOCK_SYS_GetTraceFreq(uint32_t instance)
{
    clock_trace_src_t src = CLOCK_HAL_GetTraceClkSrc(SIM);

    if (kClockTraceSrcMcgoutClk == src)
    {
        return CLOCK_HAL_GetOutClk(MCG);
    }
    else
    {
        return CLOCK_SYS_GetCoreClockFreq();
    }
}

uint32_t CLOCK_SYS_GetPortFilterFreq(uint32_t instance, clock_port_filter_src_t src)
{
    if (kClockPortFilterSrcBusClk == src)
    {
        return CLOCK_SYS_GetBusClockFreq();
    }
    else
    {
        return CLOCK_SYS_GetLpoClockFreq();
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
        case kClockLptmrSrcOsc0erClkUndiv:  /* OSC0ERCLKUDIV clock */
            freq = CLOCK_SYS_GetOsc0ExternalRefClockUndivFreq();
            break;
        default:
            freq = 0U;
            break;
    }

    return freq;
}

uint32_t CLOCK_SYS_GetUsbfsFreq(uint32_t instance)
{
    uint8_t usbdiv, usbfrac;
    uint32_t freq;
    clock_usbfs_src_t src;

    src = CLOCK_HAL_GetUsbfsSrc(SIM, instance);

    if (kClockUsbfsSrcExt == src)
    {
        return g_usbClkInFreq[0];       /* USB_CLKIN */
    }
    else
    {
        freq = CLOCK_SYS_GetPllFllClockFreq();

        CLOCK_HAL_GetUsbfsDiv(SIM, &usbdiv, &usbfrac);
        return freq * (usbfrac + 1) / (usbdiv + 1);
    }
}

uint32_t CLOCK_SYS_GetLpuartFreq(uint32_t instance)
{
    clock_lpuart_src_t src;
    uint32_t freq;

    src = CLOCK_HAL_GetLpuartSrc(SIM, instance);
    switch (src)
    {
        case kClockLpuartSrcPllFllSel:
            freq = CLOCK_SYS_GetPllFllClockFreq();
            break;
        case kClockLpuartSrcOsc0erClk:
            freq = CLOCK_SYS_GetOsc0ExternalRefClockFreq();
            break;
        case kClockLpuartSrcMcgIrClk:
            freq = CLOCK_HAL_GetInternalRefClk(MCG);
            break;
        default:
            freq = 0U;
            break;
    }
    return freq;
}

uint32_t CLOCK_SYS_GetUartFreq(uint32_t instance)
{
    uint32_t freq;

    switch (instance)
    {
        case 0:
        case 1:
            freq = CLOCK_SYS_GetSystemClockFreq();
            break;
        case 2:
            freq = CLOCK_SYS_GetBusClockFreq();
            break;
        default:
            freq = 0U;
            break;
    }

    return freq;
}

uint32_t CLOCK_SYS_GetSaiFreq(uint32_t instance, clock_sai_src_t saiSrc)
{
    uint32_t freq;
    switch (saiSrc)
    {
        case kClockSaiSrcPllFllSel:
            freq = CLOCK_SYS_GetPllFllClockFreq();
            break;
        case kClockSaiSrcOsc0erClk:
            freq = CLOCK_SYS_GetOsc0ExternalRefClockFreq();
            break;
        case kClockSaiSrcSysClk:
            freq = CLOCK_SYS_GetSystemClockFreq();
            break;
        default:
            freq = 0U;
            break;
    }

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

/* PORT instance table. */
static const sim_clock_gate_name_t portGateTable[] =
{
    kSimClockGatePortA,
    kSimClockGatePortB,
    kSimClockGatePortC,
    kSimClockGatePortD,
    kSimClockGatePortE
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

/* ADC instance table. */
static const sim_clock_gate_name_t adcGateTable[] =
{
    kSimClockGateAdc0,
    kSimClockGateAdc1
};

void CLOCK_SYS_EnableAdcClock(uint32_t instance)
{
    assert(instance < sizeof(adcGateTable)/sizeof(adcGateTable[0]));

    SIM_HAL_EnableClock(SIM, adcGateTable[instance]);
}

void CLOCK_SYS_DisableAdcClock(uint32_t instance)
{
    assert(instance < sizeof(adcGateTable)/sizeof(adcGateTable[0]));

    SIM_HAL_DisableClock(SIM, adcGateTable[instance]);
}

bool CLOCK_SYS_GetAdcGateCmd(uint32_t instance)
{
    assert(instance < sizeof(adcGateTable)/sizeof(adcGateTable[0]));

    return SIM_HAL_GetGateCmd(SIM, adcGateTable[instance]);
}

/* FTM instance table. */
static const sim_clock_gate_name_t ftmGateTable[] =
{
    kSimClockGateFtm0,
    kSimClockGateFtm1,
    kSimClockGateFtm2,
};

void CLOCK_SYS_EnableFtmClock(uint32_t instance)
{
    assert(instance < sizeof(ftmGateTable)/sizeof(ftmGateTable[0]));

    SIM_HAL_EnableClock(SIM, ftmGateTable[instance]);
}

void CLOCK_SYS_DisableFtmClock(uint32_t instance)
{
    assert(instance < sizeof(ftmGateTable)/sizeof(ftmGateTable[0]));

    SIM_HAL_DisableClock(SIM, ftmGateTable[instance]);
}

bool CLOCK_SYS_GetFtmGateCmd(uint32_t instance)
{
    assert(instance < sizeof(ftmGateTable)/sizeof(ftmGateTable[0]));

    return SIM_HAL_GetGateCmd(SIM, ftmGateTable[instance]);
}

/* SPI instance table. */
static const sim_clock_gate_name_t spiGateTable[] =
{
    kSimClockGateSpi0,
    kSimClockGateSpi1,
};

void CLOCK_SYS_EnableSpiClock(uint32_t instance)
{
    assert(instance < sizeof(spiGateTable)/sizeof(spiGateTable[0]));

    SIM_HAL_EnableClock(SIM, spiGateTable[instance]);
}

void CLOCK_SYS_DisableSpiClock(uint32_t instance)
{
    assert(instance < sizeof(spiGateTable)/sizeof(spiGateTable[0]));

    SIM_HAL_DisableClock(SIM, spiGateTable[instance]);
}

bool CLOCK_SYS_GetSpiGateCmd(uint32_t instance)
{
    assert(instance < sizeof(spiGateTable)/sizeof(spiGateTable[0]));

    return SIM_HAL_GetGateCmd(SIM, spiGateTable[instance]);
}

/* I2C instance table. */
static const sim_clock_gate_name_t i2cGateTable[] =
{
    kSimClockGateI2c0,
    kSimClockGateI2c1,
};

void CLOCK_SYS_EnableI2cClock(uint32_t instance)
{
    assert(instance < sizeof(i2cGateTable)/sizeof(i2cGateTable[0]));

    SIM_HAL_EnableClock(SIM, i2cGateTable[instance]);
}

void CLOCK_SYS_DisableI2cClock(uint32_t instance)
{
    assert(instance < sizeof(i2cGateTable)/sizeof(i2cGateTable[0]));

    SIM_HAL_DisableClock(SIM, i2cGateTable[instance]);
}

bool CLOCK_SYS_GetI2cGateCmd(uint32_t instance)
{
    assert(instance < sizeof(i2cGateTable)/sizeof(i2cGateTable[0]));

    return SIM_HAL_GetGateCmd(SIM, i2cGateTable[instance]);
}

/* Uart instance table. */
static const sim_clock_gate_name_t uartGateTable[] =
{
    kSimClockGateUart0,
    kSimClockGateUart1,
    kSimClockGateUart2,
};

void CLOCK_SYS_EnableUartClock(uint32_t instance)
{
    assert(instance < sizeof(uartGateTable)/sizeof(uartGateTable[0]));

    SIM_HAL_EnableClock(SIM, uartGateTable[instance]);
}

void CLOCK_SYS_DisableUartClock(uint32_t instance)
{
    assert(instance < sizeof(uartGateTable)/sizeof(uartGateTable[0]));

    SIM_HAL_DisableClock(SIM, uartGateTable[instance]);
}

bool CLOCK_SYS_GetUartGateCmd(uint32_t instance)
{
    assert(instance < sizeof(uartGateTable)/sizeof(uartGateTable[0]));

    return SIM_HAL_GetGateCmd(SIM, uartGateTable[instance]);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
