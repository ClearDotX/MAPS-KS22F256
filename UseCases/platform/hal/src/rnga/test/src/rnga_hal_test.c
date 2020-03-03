/*
 * Copyright (c) 2014, Freescale Semiconductor, Inc.
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

/* rnga_hal_test.c */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "fsl_rnga_hal.h"
#include "fsl_clock_manager.h"
#include <board.h>

extern RNG_Type * const g_rngaBase[];

static uint32_t ErrCounter;

static uint32_t RNGA_HAL_TEST_CR(RNG_Type * baseAddr);

static uint32_t RNGA_HAL_TEST_SR(RNG_Type * baseAddr);

static void time_delay(uint32_t t);

int main(void)
{
    uint32_t instance = 0;
    
    hardware_init();
    dbg_uart_init();

    printf("\r\nRNGA HAL Driver Unit Test Start...\r\n");

    /* Enable the clock. */
    CLOCK_SYS_EnableRngaClock(instance);
    ErrCounter = 0U;

    ErrCounter += RNGA_HAL_TEST_CR(g_rngaBase[instance]);
    ErrCounter += RNGA_HAL_TEST_SR(g_rngaBase[instance]);

    /* Disable the clock. */
    CLOCK_SYS_DisableRngaClock(instance);
        
    /* Show result. */
    printf("RNGA HAL Test for instance %2ld : %ld Err. \r\n", instance, ErrCounter);

    if (0U == ErrCounter)
    {
        printf("RNGA HAL Driver Unit Test Succeed!\r\n");
    }
    else
    {
        printf("RNGA HAL Driver Unit Test Failed!\r\n");
    }
    return 0;
}

static uint32_t RNGA_HAL_TEST_CR(RNG_Type * baseAddr)
{
    uint32_t ret = 0U;

        /* GO */
    RNGA_HAL_Enable(baseAddr);
    if (1U != RNG_BRD_CR_GO(baseAddr))
    {
    ret++;
    }

    /* GO, this bit is sticky, so keep set */
    RNGA_HAL_Disable(baseAddr);
    if (1U != RNG_BRD_CR_GO(baseAddr))
    {
        ret++;
    }

    /* High Assurance */
    RNGA_HAL_SetHighAssuranceCmd(baseAddr, true);
    if (1U != RNG_BRD_CR_HA(baseAddr))
    {
        ret++;
    }

    /* High Assurance, this bit is sticky, so keep set */
    RNGA_HAL_SetHighAssuranceCmd(baseAddr, false);
    if (1U != RNG_BRD_CR_HA(baseAddr))
    {
        ret++;
    }

    /* Interrupt Mask */
    RNGA_HAL_SetIntMaskCmd(baseAddr, true);
    if (1U != RNG_BRD_CR_INTM(baseAddr))
    {
        ret++;
    }

    /* Interrupt Mask */
    RNGA_HAL_SetIntMaskCmd(baseAddr, false);
    if (0U != RNG_BRD_CR_INTM(baseAddr))
    {
        ret++;
    }

    /* Sleep Mode */
    RNGA_HAL_SetWorkModeCmd(baseAddr, kRNGAModeSleep);
    if (1U != RNG_BRD_CR_SLP(baseAddr))
    {
        ret++;
    }

    /* Normal Mode */
    RNGA_HAL_SetWorkModeCmd(baseAddr, kRNGAModeNormal);
    if (0U != RNG_BRD_CR_SLP(baseAddr))
    {
        ret++;
    }

    return ret;
}

static uint32_t RNGA_HAL_TEST_SR(RNG_Type * baseAddr)
{
    uint32_t ret = 0U;
    uint32_t data;

    /* set GO and HA bit */
    RNG_WR_CR(baseAddr, 0x3);

    /* SECV */
    if (false != RNGA_HAL_GetSecurityViolationCmd(baseAddr))
    {
        ret++;
    }

    /* LRS */
    if (false != RNGA_HAL_GetLastReadStatusCmd(baseAddr))
    {
        ret++;
    }

    /* ORU */
    if (false != RNGA_HAL_GetOutputRegUnderflowCmd(baseAddr))
    {
        ret++;
    }

    /* Error Interrupt */
    if (false != RNGA_HAL_GetErrorIntCmd(baseAddr))
    {
        ret++;
    }

    /* Normal mode */
    if (kRNGAModeNormal != RNGA_HAL_GetWorkMode(baseAddr))
    {
        ret++;
    }

    time_delay(1);

    /* OREG_LVL */
    if (1U != RNGA_HAL_GetOutputRegLevel(baseAddr))
    {
        ret++;
    }

    if (kStatus_RNGA_Success != RNGA_HAL_GetRandomData(baseAddr, &data))
    {
        ret++;
    }

    /* Set to sleep mode */
    RNGA_HAL_SetWorkModeCmd(baseAddr, kRNGAModeSleep);

    /* Read out the remained random data from output register */
    data = RNGA_HAL_ReadRandomData(baseAddr);
    /* Read again to simulate underflow condition */
    data = RNGA_HAL_ReadRandomData(baseAddr);

    /* ORU, should be set before first read */
    if (true != RNGA_HAL_GetOutputRegUnderflowCmd(baseAddr))
    {
        ret++;
    }

    /* ORU, should be cleared after first read */
    if (false != RNGA_HAL_GetOutputRegUnderflowCmd(baseAddr))
    {
        ret++;
    }

    /* Error Interrupt, still be set after read */
    if (true != RNGA_HAL_GetErrorIntCmd(baseAddr))
    {
        ret++;
    }

    /* Clear interrupt  */
    RNGA_HAL_ClearIntFlag(baseAddr, true);

    /* Error Interrupt, should be cleared after setting CR->CLRI bit*/
    if (false != RNGA_HAL_GetErrorIntCmd(baseAddr))
    {
        ret++;
    }

    /* Sleep mode */
    if (kRNGAModeSleep != RNGA_HAL_GetWorkMode(baseAddr))
    {
        ret++;
    }

    /* Read again to simulate underflow condition */
    data = RNGA_HAL_ReadRandomData(baseAddr);

    /* LRS, should be set before first read */
    if (true != RNGA_HAL_GetLastReadStatusCmd(baseAddr))
    {
        ret++;
    }

    /* LRS, still be set after first read */
    if (true != RNGA_HAL_GetLastReadStatusCmd(baseAddr))
    {
        ret++;
    }
        
    /* SECV */
    if (true != RNGA_HAL_GetSecurityViolationCmd(baseAddr))
    {
        ret++;
    }
    return ret;
}

void time_delay(uint32_t t)
{
    uint32_t i;
    volatile uint32_t j;

    for (i = 0; i < t; i++)
    {
        for (j = 0; j < 1000; j++)
        {
            ;
        }
    }
}
