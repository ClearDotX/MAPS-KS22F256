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

#include "fsl_gpio.h"

static PORT_Type *s_portBase[] = PORT_BASE_PTRS;

#if defined(FSL_FEATURE_SOC_GPIO_COUNT) && FSL_FEATURE_SOC_GPIO_COUNT

/*******************************************************************************
 * Variables
 ******************************************************************************/
const static GPIO_Type *s_gpioBase[] = GPIO_BASE_PTRS;

/*******************************************************************************
* Prototypes
******************************************************************************/

/*!
* @brief Gets the GPIO instance according to the GPIO base
*
* @param base    GPIO peripheral base pointer(PTA, PTB, PTC, etc.)
* @retval GPIO instance
*/
static uint32_t GPIO_GetInstance(GPIO_Type *base);

/*******************************************************************************
 * Code
 ******************************************************************************/

static uint32_t GPIO_GetInstance(GPIO_Type *base)
{
    uint8_t instance = 0;

    while ((s_gpioBase[instance] != base) && (instance < FSL_FEATURE_SOC_GPIO_COUNT))
    {
        instance++;
    }

    assert(instance < FSL_FEATURE_SOC_GPIO_COUNT);

    return instance;
}

void GPIO_PinInit(GPIO_Type *base, uint32_t pin, const gpio_pin_config_t *config)
{
    assert(config);

    if (config->pinDirection == kGPIO_DigitalInput)
    {
        base->PDDR &= ~(1U << pin);
    }
    else
    {
        GPIO_WritePinsOutput(base, 1 << pin, config->outputLogic);
        base->PDDR |= (1U << pin);
    }
}

uint32_t GPIO_GetInterruptFlags(GPIO_Type *base)
{
    uint8_t instance;
    PORT_Type *portBase;
    instance = GPIO_GetInstance(base);
    portBase = s_portBase[instance];
    return portBase->ISFR;
}

void GPIO_ClearPinsInterruptFlags(GPIO_Type *base, uint32_t mask)
{
    uint8_t instance;
    PORT_Type *portBase;
    instance = GPIO_GetInstance(base);
    portBase = s_portBase[instance];
    portBase->ISFR = mask;
}

#endif /* FSL_FEATURE_SOC_GPIO_COUNT */

#if defined(FSL_FEATURE_SOC_FGPIO_COUNT) && FSL_FEATURE_SOC_FGPIO_COUNT

/*******************************************************************************
 * Variables
 ******************************************************************************/
const static FGPIO_Type *s_fgpioBase[] = FGPIO_BASE_PTRS;

/*******************************************************************************
* Prototypes
******************************************************************************/
/*!
* @brief Gets the FGPIO instance according to the GPIO base
*
* @param base    FGPIO peripheral base pointer(PTA, PTB, PTC, etc.)
* @retval FGPIO instance
*/
static uint32_t FGPIO_GetInstance(FGPIO_Type *base);

/*******************************************************************************
 * Code
 ******************************************************************************/

static uint32_t FGPIO_GetInstance(FGPIO_Type *base)
{
    uint8_t instance = 0;

    while ((s_fgpioBase[instance] != base) && (instance < FSL_FEATURE_SOC_FGPIO_COUNT))
    {
        instance++;
    }

    assert(instance < FSL_FEATURE_SOC_FGPIO_COUNT);

    return instance;
}

void FGPIO_PinInit(FGPIO_Type *base, uint32_t pin, const gpio_pin_config_t *config)
{
    assert(config);

    if (config->pinDirection == kGPIO_DigitalInput)
    {
        base->PDDR &= ~(1U << pin);
    }
    else
    {
        FGPIO_WritePinsOutput(base, 1 << pin, config->outputLogic);
        base->PDDR |= (1U << pin);
    }
}

uint32_t FGPIO_GetInterruptFlags(FGPIO_Type *base)
{
    uint8_t instance;
    instance = FGPIO_GetInstance(base);
    PORT_Type *portBase;
    portBase = s_portBase[instance];
    return portBase->ISFR;
}

void FGPIO_ClearPinsInterruptFlags(FGPIO_Type *base, uint32_t mask)
{
    uint8_t instance;
    instance = FGPIO_GetInstance(base);
    PORT_Type *portBase;
    portBase = s_portBase[instance];
    portBase->ISFR = mask;
}

#endif /* FSL_FEATURE_SOC_FGPIO_COUNT */
