/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
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
#include <stdbool.h>
#include <stdio.h>
#include "fsl_cmp_hal.h"
#include "fsl_clock_manager.h"
#include <board.h>

static uint32_t CMP_HAL_TEST_ConfigComparator(CMP_Type * base);
static uint32_t CMP_HAL_TEST_ConfigDacChn(CMP_Type * base);
static uint32_t CMP_HAL_TEST_ConfigSampleFilter(CMP_Type * base);

/*! @brief Table of base addresses for CMP instances. */
static CMP_Type * gCmpBasePtr[] = CMP_BASE_PTRS;

int main(void)
{
    uint32_t idx;
    uint32_t errCounter = 0U;
    uint32_t totalErrCounter = 0U;
    
    hardware_init();
    dbg_uart_init();
    
    printf("\r\nCMP HAL Test: Start...\r\n");
    
    for (idx = 0U; idx < CMP_INSTANCE_COUNT; idx++)
    {
        CLOCK_SYS_EnableCmpClock(idx);
        
        errCounter = 0U;
        errCounter += CMP_HAL_TEST_ConfigComparator(gCmpBasePtr[idx]);
        errCounter += CMP_HAL_TEST_ConfigDacChn(gCmpBasePtr[idx]);
        errCounter += CMP_HAL_TEST_ConfigSampleFilter(gCmpBasePtr[idx]);
        
        printf("CMP %ld HAL Test error counter: %ld\r\n", idx, errCounter);
        totalErrCounter += errCounter;
        CLOCK_SYS_DisableCmpClock(idx);
    }
    
    printf("CMP HAL Test: End\r\n");

    /* Print log for auto run detection. */
    printf("CMP HAL Test ");
    if (0U == totalErrCounter)
    {
        printf("Succeed\r\n");
    }
    else
    {
        printf("Error\r\n");
    }

    return 0;
}

static uint32_t CMP_HAL_TEST_ConfigComparator(CMP_Type * base)
{

    uint32_t errCounter = 0;
    cmp_comparator_config_t ConfigStruct;

    /* Black. */
    ConfigStruct.hystersisMode = kCmpHystersisOfLevel3;
    ConfigStruct.pinoutEnable = true;
    ConfigStruct.pinoutUnfilteredEnable = true;

    ConfigStruct.invertEnable = true;

    ConfigStruct.highSpeedEnable = true;
#if FSL_FEATURE_CMP_HAS_DMA
    ConfigStruct.dmaEnable = true;
#endif 
    ConfigStruct.risingIntEnable = true;
    ConfigStruct.fallingIntEnable = true;

    ConfigStruct.plusChnMux = kCmpInputChn7;
    ConfigStruct.minusChnMux = kCmpInputChn0;
    
#if FSL_FEATURE_CMP_HAS_TRIGGER_MODE
    ConfigStruct.triggerEnable = true;
#endif
    CMP_HAL_ConfigComparator(base, &ConfigStruct);
    
    if (3U != CMP_BRD_CR0_HYSTCTR(base) )
    {
        errCounter++;
    }
    if (1U != CMP_BRD_CR1_OPE(base) )
    {
        errCounter++;
    }
    if (1U != CMP_BRD_CR1_COS(base) )
    {
        errCounter++;
    }
    if (1U != CMP_BRD_CR1_INV(base) )
    {
        errCounter++;
    }
    if (1U != CMP_BRD_CR1_PMODE(base) )
    {
        errCounter++;
    }
#if FSL_FEATURE_CMP_HAS_DMA
    if (1U != CMP_BRD_SCR_DMAEN(base) )
    {
        errCounter++;
    }
#endif      
    if (1U != CMP_BRD_SCR_IER(base) )
    {
        errCounter++;
    }
    if (1U != CMP_BRD_SCR_IEF(base) )
    {
        errCounter++;
    }
    if (7U != CMP_BRD_MUXCR_PSEL(base) )
    {
        errCounter++;
    }
    if (0U != CMP_BRD_MUXCR_MSEL(base) )
    {
        errCounter++;
    }
#if FSL_FEATURE_CMP_HAS_TRIGGER_MODE
    if (1U != CMP_BRD_CR1_TRIGM(base) )
    {
        errCounter++;
    }
#endif      
    
    /* White. */
    ConfigStruct.hystersisMode = kCmpHystersisOfLevel0;
    ConfigStruct.pinoutEnable = false;
    ConfigStruct.pinoutUnfilteredEnable = false;

    ConfigStruct.invertEnable = false;

    ConfigStruct.highSpeedEnable = false;
#if FSL_FEATURE_CMP_HAS_DMA
    ConfigStruct.dmaEnable = false;
#endif 
    ConfigStruct.risingIntEnable = false;
    ConfigStruct.fallingIntEnable = false;

    ConfigStruct.plusChnMux = kCmpInputChn0;
    ConfigStruct.minusChnMux = kCmpInputChn7;
    
#if FSL_FEATURE_CMP_HAS_TRIGGER_MODE
    ConfigStruct.triggerEnable = false;
#endif
    CMP_HAL_ConfigComparator(base, &ConfigStruct);
    
    if (0U != CMP_BRD_CR0_HYSTCTR(base) )
    {
        errCounter++;
    }
    if (0U != CMP_BRD_CR1_OPE(base) )
    {
        errCounter++;
    }
    if (0U != CMP_BRD_CR1_COS(base) )
    {
        errCounter++;
    }
    if (0U != CMP_BRD_CR1_INV(base) )
    {
        errCounter++;
    }
    if (0U != CMP_BRD_CR1_PMODE(base) )
    {
        errCounter++;
    }
#if FSL_FEATURE_CMP_HAS_DMA
    if (0U != CMP_BRD_SCR_DMAEN(base) )
    {
        errCounter++;
    }
#endif      
    if (0U != CMP_BRD_SCR_IER(base) )
    {
        errCounter++;
    }
    if (0U != CMP_BRD_SCR_IEF(base) )
    {
        errCounter++;
    }
    if (0U != CMP_BRD_MUXCR_PSEL(base) )
    {
        errCounter++;
    }
    if (7U != CMP_BRD_MUXCR_MSEL(base) )
    {
        errCounter++;
    }
#if FSL_FEATURE_CMP_HAS_TRIGGER_MODE
    if (0U != CMP_BRD_CR1_TRIGM(base) )
    {
        errCounter++;
    }
#endif

    CMP_HAL_Init(base);

    return errCounter;
}

uint32_t CMP_HAL_TEST_ConfigDacChn(CMP_Type * base)
{
    uint32_t errCounter = 0;
    cmp_dac_config_t ConfigStruct;

    /* Black. */
    ConfigStruct.dacEnable = true;
    ConfigStruct.refVoltSrcMode = kCmpDacRefVoltSrcOf2;
    ConfigStruct.dacValue = 0x3F;
    CMP_HAL_ConfigDacChn(base,&ConfigStruct); 
    if (1U != CMP_BRD_DACCR_DACEN(base) )
    {
        errCounter++;
    }
    if (1U != CMP_BRD_DACCR_VRSEL(base) )
    {
        errCounter++;
    }
    if (0x3F != CMP_BRD_DACCR_VOSEL(base) )
    {
        errCounter++;
    }
        
    /* White. */
    ConfigStruct.dacEnable = false;
    ConfigStruct.refVoltSrcMode = kCmpDacRefVoltSrcOf1;
    ConfigStruct.dacValue = 0;
    CMP_HAL_ConfigDacChn(base,&ConfigStruct);
    if (0U != CMP_BRD_DACCR_DACEN(base) )
    {
        errCounter++;
    } 
    if (0U != CMP_BRD_DACCR_VRSEL(base) )
    {
        errCounter++;
    }
    if (0U != CMP_BRD_DACCR_VOSEL(base) )
    {
        errCounter++;
    }

    CMP_HAL_Init(base);

    return errCounter;
}

static uint32_t CMP_HAL_TEST_ConfigSampleFilter(CMP_Type * base)
{
    uint32_t errCounter = 0;
    cmp_sample_filter_config_t ConfigStruct;

    /* Black. */
    ConfigStruct.workMode = kCmpSampleWithFilteredMode;
    ConfigStruct.useExtSampleOrWindow = true;
    ConfigStruct.filterClkDiv = 0xFF;
    ConfigStruct.filterCount = kCmpFilterCountSampleOf7;

    CMP_HAL_ConfigSampleFilter( base,&ConfigStruct);

    if (1U != CMP_BRD_CR1_SE(base) )
    {
        errCounter++;
    }
    if (7U != CMP_BRD_CR0_FILTER_CNT(base) )
    {
        errCounter++;
    }

    /* White*/
    ConfigStruct.workMode = kCmpContinuousMode;
    ConfigStruct.useExtSampleOrWindow = false;
    ConfigStruct.filterClkDiv = 0;
    ConfigStruct.filterCount = kCmpFilterCountSampleOf0;
    CMP_HAL_ConfigSampleFilter( base,&ConfigStruct);

    if (0U != CMP_BRD_CR0_FILTER_CNT(base) )
    {
        errCounter++;
    }   
    CMP_HAL_Init(base);

    return errCounter;
}


/*******************************************************************************
 * EOF
 ******************************************************************************/
