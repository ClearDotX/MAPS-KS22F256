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

#include <stdio.h>
#include "fsl_sai_hal.h"
#include "fsl_os_abstraction.h"
#include "fsl_clock_manager.h"
#include "fsl_debug_console.h"
#include "board.h"

#define ERROR_DEBUG() printf("SAI HAL TEST ERROR: function[%s] line[%d]\r\n", __func__, __LINE__)

/******************************************************************************
*Codes
******************************************************************************/
extern I2S_Type * const g_saiBase[];

/* HAL layer tests. */
int32_t main(void)
{
    uint32_t i = 0, j = 0;
    uint32_t error = 0;
    uint32_t instances = 1;
    hardware_init();
    dbg_uart_init();
    for (i = 0; i < instances; i++)
    {
        CLOCK_SYS_EnableSaiClock(i);
#if FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER
        SAI_HAL_SetMclkDiv(g_saiBase[i], 48000 * 384, 120000000);
        if ((I2S_BRD_MDR_DIVIDE(g_saiBase[i]) != 0x270) && (I2S_BRD_MDR_FRACT(g_saiBase[i]) != 0x5F))
        {
            ERROR_DEBUG();
            error ++;
        }
#endif
        /* Enable TE bit */
        SAI_HAL_TxEnable(g_saiBase[i]);
        if (I2S_BRD_TCSR_TE(g_saiBase[i]) != 1)
        {
            ERROR_DEBUG();
            error ++;
        }
        SAI_HAL_TxDisable(g_saiBase[i]);
        if(I2S_BRD_TCSR_TE(g_saiBase[i]))
        {
            ERROR_DEBUG();
            error ++;
        }
#if (FSL_FEATURE_SAI_FIFO_COUNT > 1)
        SAI_HAL_TxSetIntCmd(g_saiBase[i], kSaiIntrequestFIFORequest,true);
        if(I2S_BRD_TCSR_FRIE(g_saiBase[i]) != 1)
        {
            ERROR_DEBUG();
            error ++;
        }
        SAI_HAL_TxSetIntCmd(g_saiBase[i],kSaiIntrequestFIFORequest,false);
        if(I2S_BRD_TCSR_FRIE(g_saiBase[i]))
        {
            ERROR_DEBUG();
            error ++;            
        }
        SAI_HAL_TxSetDmaCmd(g_saiBase[i], kSaiDmaReqFIFORequest, true);
        if(I2S_BRD_TCSR_FRDE(g_saiBase[i]) != 1)
        {
            ERROR_DEBUG();
            error ++;
        }
        SAI_HAL_TxSetDmaCmd(g_saiBase[i],kSaiDmaReqFIFORequest, false);
        if(I2S_BRD_TCSR_FRDE(g_saiBase[i]))
        {
            ERROR_DEBUG();
            error ++;            
        }
#endif
#if (FSL_FEATURE_SAI_FIFO_COUNT > 1)
        /* Tx Watermark setting test */
        for(j = 0; j < FSL_FEATURE_SAI_FIFO_COUNT; j ++)
        {
            SAI_HAL_TxSetWatermark(g_saiBase[i],j);
            if (I2S_BRD_TCR1_TFW(g_saiBase[i]) != j)
            {
                ERROR_DEBUG();
                error ++;
            }
        }
#endif
        /* Async/sync mode setting */
        SAI_HAL_TxSetSyncMode(g_saiBase[i], kSaiModeAsync);
        if (I2S_BRD_TCR2_SYNC(g_saiBase[i]) != 0)
        {
            ERROR_DEBUG();
            error ++; 
        }
        /* Tx mode setting */
        SAI_HAL_TxSetRunModeCmd(g_saiBase[i], kSaiRunModeDebug,true);
        if (I2S_BRD_TCSR_DBGE(g_saiBase[i]) != 1)		
        {
            ERROR_DEBUG();
            error++;
        }
        /* Slave/master mode setting */
        SAI_HAL_TxSetMasterSlave(g_saiBase[i], kSaiMaster);
        if (!(I2S_BRD_TCR2_BCD(g_saiBase[i]) && I2S_BRD_TCR4_FSD(g_saiBase[i])))
        {
            ERROR_DEBUG();
            error ++;
        }
        /* Use which data channel */
        SAI_HAL_TxSetDataChn(g_saiBase[i],0);
        if (I2S_BRD_TCR3_TCE(g_saiBase[i]) != 1)
        {
            error ++;
            ERROR_DEBUG();
        }
        /* Bus type setting */
        SAI_HAL_TxSetProtocol(g_saiBase[i],kSaiBusI2SType);
        if (!(I2S_BRD_TCR2_BCP(g_saiBase[i]) && I2S_BRD_TCR4_MF(g_saiBase[i]) && I2S_BRD_TCR4_FSE(g_saiBase[i])))
        {
            ERROR_DEBUG();
            error ++;
        }
        /* Bit clock divider setting */
        for(j = 2; j <= 512; j += 2)
        {
            SAI_HAL_TxSetBclkDiv(g_saiBase[i], j);
            if (I2S_BRD_TCR2_DIV(g_saiBase[i]) != (j/2 -1))
            {
                ERROR_DEBUG();
                error ++;
            }
        }
        /* swap tx bit clock */
        SAI_HAL_TxSetSwapBclkCmd(g_saiBase[i], true);
        if (I2S_BRD_TCR2_BCS(g_saiBase[i]) != 1)
        {
            ERROR_DEBUG();
            error ++;
        }
        /* Choose bit clock source */
        SAI_HAL_TxSetBclkSrc(g_saiBase[i], kSaiBclkSourceMclkDiv);
        if (I2S_BRD_TCR2_MSEL(g_saiBase[i]) != 1)
        {
            ERROR_DEBUG();
            error ++;
        }
        /* RX test */
        /* Enable RE bit */
        SAI_HAL_RxEnable(g_saiBase[i]);
        if (I2S_BRD_RCSR_RE(g_saiBase[i]) != 1)
        {
            ERROR_DEBUG();
            error ++;
        }
        SAI_HAL_RxDisable(g_saiBase[i]);
        if(I2S_BRD_RCSR_RE(g_saiBase[i]))
        {
            ERROR_DEBUG();
            error ++;
        }
#if (FSL_FEATURE_SAI_FIFO_COUNT > 1)
        SAI_HAL_RxSetIntCmd(g_saiBase[i], kSaiIntrequestFIFORequest,true);
        if(I2S_BRD_RCSR_FRIE(g_saiBase[i]) != 1)
        {
            ERROR_DEBUG();
            error ++;
        }
        SAI_HAL_RxSetIntCmd(g_saiBase[i],kSaiIntrequestFIFORequest, false);
        if(I2S_BRD_RCSR_FRIE(g_saiBase[i]))
        {
            ERROR_DEBUG();
            error ++;            
        }
        SAI_HAL_RxSetDmaCmd(g_saiBase[i], kSaiDmaReqFIFORequest,true);
        if(I2S_BRD_RCSR_FRDE(g_saiBase[i]) != 1)
        {
            ERROR_DEBUG();
            error ++;
        }
        SAI_HAL_RxSetDmaCmd(g_saiBase[i],kSaiDmaReqFIFORequest,false);
        if(I2S_BRD_RCSR_FRDE(g_saiBase[i]))
        {
            ERROR_DEBUG();
            error ++;            
        }
#endif
#if (FSL_FEATURE_SAI_FIFO_COUNT > 1)
        /* Rx Watermark setting test */
        for(j = 0; j < FSL_FEATURE_SAI_FIFO_COUNT; j ++)
        {
            SAI_HAL_RxSetWatermark(g_saiBase[i],j);
            if (I2S_BRD_RCR1_RFW(g_saiBase[i]) != j)
            {
                ERROR_DEBUG();
                error ++;
            }
        }
#endif
        /* Async/sync mode setting */
        SAI_HAL_RxSetSyncMode(g_saiBase[i], kSaiModeAsync);
        if (I2S_BRD_RCR2_SYNC(g_saiBase[i]) != 0)
        {
            ERROR_DEBUG();
            error ++; 
        }
        /* Tx mode setting */
        SAI_HAL_RxSetRunModeCmd(g_saiBase[i], kSaiRunModeDebug,true);
        if (I2S_BRD_RCSR_DBGE(g_saiBase[i]) != 1)		
        {
            ERROR_DEBUG();
            error ++;
        }
        /* Slave/master mode setting */
        SAI_HAL_RxSetMasterSlave(g_saiBase[i], kSaiSlave);
        if (I2S_BRD_RCR2_BCD(g_saiBase[i]) || I2S_BRD_RCR4_FSD(g_saiBase[i]))
        {
            ERROR_DEBUG();
            error ++;
        }
        /* Use which data channel */
        SAI_HAL_RxSetDataChn(g_saiBase[i],0);
        if (I2S_BRD_RCR3_RCE(g_saiBase[i]) != 1)
        {
            ERROR_DEBUG();
            error ++;
        }
        /* Bus type setting */
        SAI_HAL_RxSetProtocol(g_saiBase[i],kSaiBusI2SType);
        if (!(I2S_BRD_RCR2_BCP(g_saiBase[i]) && I2S_BRD_RCR4_MF(g_saiBase[i]) && I2S_BRD_RCR4_FSE(g_saiBase[i])))
        {
            ERROR_DEBUG();
            error ++;
        }
        /* Bit clock divider setting */
        for(j = 2; j <= 512; j += 2)
        {
            SAI_HAL_RxSetBclkDiv(g_saiBase[i], j);
            if (I2S_BRD_RCR2_DIV(g_saiBase[i]) != (j/2 -1))
            {
                ERROR_DEBUG();
                error ++;
            }
        }
        /* swap tx bit clock */
        SAI_HAL_RxSetSwapBclkCmd(g_saiBase[i], true);
        if (I2S_BRD_RCR2_BCS(g_saiBase[i]) != 1)
        {
            ERROR_DEBUG();
            error ++;
        }
        /* Choose bit clock source */
        SAI_HAL_RxSetBclkSrc(g_saiBase[i], kSaiBclkSourceMclkDiv);
        if (I2S_BRD_RCR2_MSEL(g_saiBase[i]) != 1)
        {
            ERROR_DEBUG();
            error ++;
        }
       
    }

    if(error == 0)
    {
        printf("SAI HAL Unit Test Succeed!\r\n");
    }
    printf("Test completed!\r\n");
    return error;
}

/******************************************************************************
*EOF
******************************************************************************/
