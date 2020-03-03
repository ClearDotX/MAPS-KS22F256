/* dac_hal_test.c */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "fsl_clock_manager.h"
#include "fsl_dac_hal.h"
#include "board.h"

static uint32_t DAC_HAL_TEST_ConfigConverter(DAC_Type * base);
static uint32_t DAC_HAL_TEST_ConfigBuffer(DAC_Type * base);
static uint32_t DAC_HAL_TEST_SetBuffValue(DAC_Type * base);
static uint32_t DAC_HAL_TEST_SetBuffCurIdx(DAC_Type * base);

static DAC_Type * gDacBasePtr[] = DAC_BASE_PTRS;

int main(void)
{
    uint32_t idx;
    uint32_t errCounter = 0U;
    uint32_t totalErrCounter = 0U;
    
    hardware_init();
    dbg_uart_init();
    
    printf("\r\nDAC HAL Test: Start...\r\n");
    
    for (idx = 0U; idx < DAC_INSTANCE_COUNT; idx++)
    {
        /* Enable the clock. */
        CLOCK_SYS_EnableDacClock(idx);
        errCounter = 0U;
        errCounter += DAC_HAL_TEST_ConfigConverter(gDacBasePtr[idx]);
        errCounter += DAC_HAL_TEST_ConfigBuffer(gDacBasePtr[idx]);
        errCounter += DAC_HAL_TEST_SetBuffValue(gDacBasePtr[idx]);
        errCounter += DAC_HAL_TEST_SetBuffCurIdx(gDacBasePtr[idx]); 
        printf("DAC %ld HAL Test error counter: %ld\r\n", idx, errCounter);
        totalErrCounter += errCounter;
        CLOCK_SYS_DisableDacClock(idx);
    }
    
    printf("DAC HAL Test: End\r\n");

    /* Print log for auto run detection. */
    printf("DAC HAL Test ");
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

static uint32_t DAC_HAL_TEST_ConfigConverter(DAC_Type * base)
{
    uint32_t errCounter = 0;
        dac_converter_config_t ConfigStruct;
    
    /* Black. */
    ConfigStruct.dacRefVoltSrc = kDacRefVoltSrcOfVref2;
    ConfigStruct.lowPowerEnable = true;
    DAC_HAL_ConfigConverter(base, &ConfigStruct);
    
    if (1U != DAC_BRD_C0_DACRFS(base) )
    {
        errCounter++;
    }
    if (1U != DAC_BRD_C0_LPEN(base) )
    {
        errCounter++;
    }

    /* White. */
    ConfigStruct.dacRefVoltSrc = kDacRefVoltSrcOfVref1;
    ConfigStruct.lowPowerEnable = false;
    DAC_HAL_ConfigConverter(base, &ConfigStruct);
    
    if (0U != DAC_BRD_C0_DACRFS(base) )
    {
        errCounter++;
    }
    if (0U != DAC_BRD_C0_LPEN(base) )
    {
        errCounter++;
    }

    DAC_HAL_Init(base);

    return errCounter;
}

static uint32_t DAC_HAL_TEST_ConfigBuffer(DAC_Type * base)
{
    uint32_t errCounter = 0;
    dac_buffer_config_t ConfigStruct;

    /* Black*/
    ConfigStruct.bufferEnable = true;
    ConfigStruct.triggerMode = kDacTriggerBySoftware;
#if FSL_FEATURE_DAC_HAS_WATERMARK_SELECTION
    ConfigStruct.idxWatermarkIntEnable = true;        
    ConfigStruct.watermarkMode = kDacBuffWatermarkFromUpperAs4Word;      
#endif /* FSL_FEATURE_DAC_HAS_WATERMARK_SELECTION */
    ConfigStruct.idxStartIntEnable = true;
    ConfigStruct.idxUpperIntEnable = true;       
    ConfigStruct.dmaEnable = true; 
#if DAC_C1_DACBFMD_WIDTH!=0
    ConfigStruct.buffWorkMode = kDacBuffWorkAsOneTimeScanMode; 
#endif /* DAC_C1_DACBFMD_WIDTH */
    ConfigStruct.upperIdx = (DAC_DATL_COUNT-1U);
    DAC_HAL_ConfigBuffer(base, &ConfigStruct);
    
    if (1U != DAC_BRD_C1_DACBFEN(base) )
    {
        errCounter++;
    }
    if (1U != DAC_BRD_C0_DACTRGSEL(base) )
    {
        errCounter++;
    } 
#if FSL_FEATURE_DAC_HAS_WATERMARK_SELECTION
    if (1U != DAC_BRD_C0_DACBWIEN(base) )
    {
        errCounter++;
    }
    if ((uint8_t)kDacBuffWatermarkFromUpperAs4Word != DAC_BRD_C1_DACBFWM(base) )
    {
        errCounter++;
    }
#endif /* FSL_FEATURE_DAC_HAS_WATERMARK_SELECTION */
    if (1U != DAC_BRD_C0_DACBTIEN(base) )
    {
        errCounter++;
    }
    if (1U != DAC_BRD_C0_DACBBIEN(base) )
    {
        errCounter++;
    }
    if (1U != DAC_BRD_C1_DMAEN(base) )
    {
        errCounter++;
    }
#if DAC_C1_DACBFMD_WIDTH!=0
    if ((uint8_t)(kDacBuffWorkAsOneTimeScanMode) != DAC_BRD_C1_DACBFMD(base) )
    {
        errCounter++;
    }
#endif
    if ((DAC_DATL_COUNT-1U) != DAC_BRD_C2_DACBFUP(base) )
    {
        errCounter++;
    }

    /* White. */
    ConfigStruct.bufferEnable = false;
    ConfigStruct.triggerMode = kDacTriggerByHardware;
#if FSL_FEATURE_DAC_HAS_WATERMARK_SELECTION
    ConfigStruct.idxWatermarkIntEnable = false;        
    ConfigStruct.watermarkMode = kDacBuffWatermarkFromUpperAs1Word;      
#endif /* FSL_FEATURE_DAC_HAS_WATERMARK_SELECTION */
    ConfigStruct.idxStartIntEnable = false;
    ConfigStruct.idxUpperIntEnable = false;       
    ConfigStruct.dmaEnable = false; 
    ConfigStruct.buffWorkMode = kDacBuffWorkAsNormalMode;   
    ConfigStruct.upperIdx = 0U;
    DAC_HAL_ConfigBuffer(base, &ConfigStruct);
    
    if (0U != DAC_BRD_C1_DACBFEN(base) )
    {
        errCounter++;
    }
    if (0U != DAC_BRD_C0_DACTRGSEL(base) )
    {
        errCounter++;
    } 
#if FSL_FEATURE_DAC_HAS_WATERMARK_SELECTION
    if (0U != DAC_BRD_C0_DACBWIEN(base) )
    {
        errCounter++;
    }
    if ((uint8_t)kDacBuffWatermarkFromUpperAs1Word != DAC_BRD_C1_DACBFWM(base) )
    {
        errCounter++;
    }
#endif /* FSL_FEATURE_DAC_HAS_WATERMARK_SELECTION */
    if (0U != DAC_BRD_C0_DACBTIEN(base) )
    {
        errCounter++;
    }
    if (0U != DAC_BRD_C0_DACBBIEN(base) )
    {
        errCounter++;
    }
    if (0U != DAC_BRD_C1_DMAEN(base) )
    {
        errCounter++;
    }
#if DAC_C1_DACBFMD_WIDTH!=0
    if ((uint8_t)(kDacBuffWorkAsNormalMode) != DAC_BRD_C1_DACBFMD(base) )
    {
        errCounter++;
    }
#endif
    if (0U != DAC_BRD_C2_DACBFUP(base) )
    {
        errCounter++;
    }


    DAC_HAL_Init(base);

    return errCounter;
}

static uint32_t DAC_HAL_TEST_SetBuffValue(DAC_Type * base)
{
    uint32_t errCounter = 0;
    uint32_t i;
    
    /* Black*/
    for (i = 0U; i < DAC_DATL_COUNT; i++)
    {
        DAC_HAL_SetBuffValue(base, i, 0xFFF);
        if ( (0xFF != DAC_RD_DATL(base, i)) || (0x0F !=DAC_RD_DATH(base, i)) )
        {
            errCounter++;
        }
    }

    /* White. */
    for (i = 0U; i < DAC_DATL_COUNT; i++)
    {
        DAC_HAL_SetBuffValue(base, i, 0);
        if ( (0U!= DAC_RD_DATL(base, i)) || (0U!=DAC_RD_DATH(base, i)) )
        {
            errCounter++;
        }
    }

    DAC_HAL_Init(base);

    return errCounter;
}

static uint32_t DAC_HAL_TEST_SetBuffCurIdx(DAC_Type * base)
{
    uint32_t errCounter = 0U;
  
    DAC_HAL_Init(base);
    DAC_BWR_C1_DACBFEN(base, 1U);
    /*Black*/
    DAC_HAL_SetBuffCurIdx(base, DAC_DATL_COUNT-1U);
    //DAC_WR_C2(base, 0xFFU);
    if ( (DAC_DATL_COUNT-1U) != DAC_BRD_C2_DACBFRP(base))
    {
        errCounter++;
    }

    /*White*/
    DAC_HAL_SetBuffCurIdx(base, 0U);
    //DAC_WR_C2(base, 0U);
    if (0U != DAC_BRD_C2_DACBFRP(base))
    {
        errCounter++;
    }

    DAC_HAL_Init(base);
    return errCounter;
}
/* EOF. */




/* EOF. */
