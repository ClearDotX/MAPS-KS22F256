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
 * DISCLAIMED. IN NO EVENT SDRVL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_port.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

void PORT_SetPinConfig(PORT_Type *base, uint32_t pin, const port_pin_config_t *config)
{
    assert(config);

    uint16_t PCRL = *((const uint16_t *)config);

    if (pin < 16)
    {
        base->GPCLR = ((1U << pin) << 16) | PCRL;
    }
    else
    {
        base->GPCHR = (1U << pin) | PCRL;
    }
}

void PORT_SetPinMux(PORT_Type *base, uint32_t pin, port_mux_t mux)
{
    base->PCR[pin] = (base->PCR[pin] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(mux);
}

#if defined(FSL_FEATURE_PORT_HAS_DIGITAL_FILTER) && FSL_FEATURE_PORT_HAS_DIGITAL_FILTER

void PORT_SetDigitalFilterConfig(PORT_Type *base, const port_digital_filter_config_t *config)
{
    assert(config);

    base->DFCR = PORT_DFCR_CS(config->clockSource);
    base->DFWR = PORT_DFWR_FILT(config->digitalFilterWidth);
}

#endif /* FSL_FEATURE_PORT_HAS_DIGITAL_FILTER */

void PORT_SetPinInterruptConfig(PORT_Type *base, uint32_t pin, const port_interrupt_t config)
{
    base->PCR[pin] = (base->PCR[pin] & ~PORT_PCR_IRQC_MASK) | PORT_PCR_IRQC(config);
}
