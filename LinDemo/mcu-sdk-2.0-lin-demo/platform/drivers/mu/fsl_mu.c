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

#include "fsl_mu.h"

/******************************************************************************
 * Code
 *****************************************************************************/
void MU_SendMsg(MU_Type *base, uint32_t regIndex, uint32_t msg)
{
    assert(regIndex < MU_TR_COUNT);

    /* Wait TX register to be empty. */
    while (!(base->SR & (kMU_Tx0EmptyFlag >> regIndex)))
    {
    }

    base->TR[regIndex] = msg;
}

uint32_t MU_ReceiveMsg(MU_Type *base, uint32_t regIndex)
{
    assert(regIndex < MU_TR_COUNT);

    /* Wait RX register to be full. */
    while (!(base->SR & (kMU_Rx0FullFlag >> regIndex)))
    {
    }

    return base->RR[regIndex];
}

void MU_SetFlags(MU_Type *base, uint32_t flags)
{
    /* Wait for update finished. */
    while (base->SR & MU_SR_FUP_MASK)
    {
    }

    MU_SetFlagsNonBlocking(base, flags);
}

status_t MU_TriggerInterrupts(MU_Type *base, uint32_t mask)
{
    uint32_t reg = base->CR;

    /* Previous interrupt has been accepted. */
    if (!(reg & mask))
    {
        /* All interrupts have been accepted, trigger now. */
        reg = (reg & ~MU_CR_GIRn_NMI_MASK) | mask;
        base->CR = reg;
        return kStatus_Success;
    }
    else
    {
        return kStatus_Fail;
    }
}

void MU_BootCoreB(MU_Type *base, mu_core_boot_mode_t mode)
{
    uint32_t reg = base->CR;

    reg = (reg & ~(MU_CR_GIRn_NMI_MASK | MU_CR_BRSTH_MASK | MU_CR_BBOOT_MASK)) | MU_CR_BBOOT(mode);

    base->CR = reg;
}
