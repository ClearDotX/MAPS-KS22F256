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

#include "fsl_intmux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
 
/*!
 * @brief Get instance number for INTMUX.
 *
 * @param base INTMUX peripheral base address.
 */
static uint32_t INTMUX_GetInstance(INTMUX_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Array to map INTMUX instance number to base pointer. */
static INTMUX_Type *const s_intmuxBase[] = INTMUX_BASE_PTRS;

/*! @brief Array to map INTMUX instance number to clock name. */
static const clock_ip_name_t s_intmuxClockName[] = INTMUX_CLOCKS;

/*******************************************************************************
 * Code
 ******************************************************************************/
static uint32_t INTMUX_GetInstance(INTMUX_Type *base)
{
    uint8_t instance = 0;

    while ((s_intmuxBase[instance] != base) && (instance < FSL_FEATURE_SOC_INTMUX_COUNT))
    {
        instance++;
    }
    assert(instance < FSL_FEATURE_SOC_INTMUX_COUNT);

    return instance;
}

void INTMUX_Init(INTMUX_Type *base)
{
    CLOCK_EnableClock(s_intmuxClockName[INTMUX_GetInstance(base)]);
}

void INTMUX_Deinit(INTMUX_Type *base)
{
    CLOCK_DisableClock(s_intmuxClockName[INTMUX_GetInstance(base)]);
}

void INTMUX_SetChannelConfig(INTMUX_Type *base, uint32_t channel, uint32_t source, intmux_channel_logic_mode_t logic)
{
    assert(channel < FSL_FEATURE_INTMUX_CHANNEL_COUNT);

    base->CHANNEL[channel].CHn_CSR = INTMUX_CHn_CSR_AND(logic);
    base->CHANNEL[channel].CHn_IER_31_0 = source;
}
