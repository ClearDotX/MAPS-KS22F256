/* adc16_hal_test.c */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "fsl_adc16_hal.h"
#include "fsl_clock_manager.h"
#include <board.h>

static ADC_Type * gAdcBaseAddr[] = ADC_BASE_PTRS;


static uint32_t ADC16_HAL_TEST_ConfigConverter(ADC_Type *base);
static uint32_t ADC16_HAL_TEST_ConfigHwCompare(ADC_Type *base);
#if FSL_FEATURE_ADC16_HAS_HW_AVERAGE
static uint32_t ADC16_HAL_TEST_ConfigHwAverage(ADC_Type *base);
#endif /* FSL_FEATURE_ADC16_HAS_HW_AVERAGE */
#if FSL_FEATURE_ADC16_HAS_MUX_SELECT
static uint32_t ADC16_HAL_TEST_SetChnMuxMode(ADC_Type *base);
#endif /* FSL_FEATURE_ADC16_HAS_MUX_SELECT */
#if FSL_FEATURE_ADC16_HAS_CALIBRATION
static uint32_t ADC_HAL_TEST_SetAutoCalibration(ADC_Type *base);
#endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */

int main(void)
{
    uint32_t idx;
    uint32_t errCounter, totalErrCounter;

    hardware_init();
    dbg_uart_init();

    totalErrCounter = 0U;

    printf("\r\nADC16 HAL Test: Start...\r\n");
    

    for (idx = 0U; idx < ADC_INSTANCE_COUNT; idx++)
    {
        /* Enable the clock. */
        CLOCK_SYS_EnableAdcClock(idx);
        errCounter = 0U;

        errCounter += ADC16_HAL_TEST_ConfigConverter(gAdcBaseAddr[idx]);
        errCounter += ADC16_HAL_TEST_ConfigHwCompare(gAdcBaseAddr[idx]);
        errCounter += ADC16_HAL_TEST_ConfigHwAverage(gAdcBaseAddr[idx]);
        errCounter += ADC16_HAL_TEST_SetChnMuxMode(gAdcBaseAddr[idx]);
        errCounter += ADC_HAL_TEST_SetAutoCalibration(gAdcBaseAddr[idx]);

        /* Enable the clock. */
        CLOCK_SYS_DisableAdcClock(idx);
        /* Show result. */
        printf("ADC16 HAL Test for instance %2ld : %ld Err. \r\n", idx, errCounter);
        totalErrCounter += errCounter;
    }
    
    printf("ADC16 HAL Test: End\r\n");

    /* Print log for auto run detection. */
    printf("ADC16 HAL Test ");
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

static uint32_t ADC16_HAL_TEST_ConfigConverter(ADC_Type *base)
{
    uint32_t errCounter = 0U;
    adc16_converter_config_t configStruct;

    ADC16_HAL_Init(base);

    /* Black. */
    configStruct.lowPowerEnable = true;
    configStruct.clkDividerMode = kAdc16ClkDividerOf8;
    configStruct.longSampleTimeEnable = true;
    configStruct.resolution = kAdc16ResolutionBitOf12or13;
    configStruct.clkSrc = kAdc16ClkSrcOfAsynClk;
    configStruct.asyncClkEnable = true;
    configStruct.highSpeedEnable = true;
    configStruct.longSampleCycleMode = kAdc16LongSampleCycleOf6;
    configStruct.hwTriggerEnable = true;
    configStruct.refVoltSrc = kAdc16RefVoltSrcOfValt;
    configStruct.continuousConvEnable = true;
#if FSL_FEATURE_ADC16_HAS_DMA
    configStruct.dmaEnable = true;
#endif /* FSL_FEATURE_ADC16_HAS_DMA */
    ADC16_HAL_ConfigConverter(base, &configStruct);
    if (1U != ADC_BRD_CFG1_ADLPC(base))
    {
        errCounter++;
    }
    if ((uint32_t)(kAdc16ClkDividerOf8) != ADC_BRD_CFG1_ADIV(base))
    {
        errCounter++;
    }
    if (1U != ADC_BRD_CFG1_ADLSMP(base))
    {
        errCounter++;
    }
    if ((uint32_t)(kAdc16ResolutionBitOf12or13) != ADC_BRD_CFG1_MODE(base))
    {
        errCounter++;
    }
    if ((uint32_t)(kAdc16ClkSrcOfAsynClk) != ADC_BRD_CFG1_ADICLK(base))
    {
        errCounter++;
    }
    if (1U != ADC_BRD_CFG2_ADACKEN(base))
    {
        errCounter++;
    }
    if (1U != ADC_BRD_CFG2_ADHSC(base))
    {
        errCounter++;
    }
    if ((uint32_t)(kAdc16LongSampleCycleOf6) != ADC_BRD_CFG2_ADLSTS(base))
    {
        errCounter++;
    }
    if (1U != ADC_BRD_SC2_ADTRG(base))
    {
        errCounter++;
    }
    if ((uint32_t)(kAdc16RefVoltSrcOfValt) != ADC_BRD_SC2_REFSEL(base))
    {
        errCounter++;
    }
    if (1U != ADC_BRD_SC3_ADCO(base))
    {
        errCounter++;
    }
#if FSL_FEATURE_ADC16_HAS_DMA
    if (1U != ADC_BRD_SC2_DMAEN(base))
    {
        errCounter++;
    }
#endif /* FSL_FEATURE_ADC16_HAS_DMA */

    /* White. */
    configStruct.lowPowerEnable = false;
    configStruct.clkDividerMode = kAdc16ClkDividerOf1;
    configStruct.longSampleTimeEnable = false;
    configStruct.resolution = kAdc16ResolutionBitOf8or9;
    configStruct.clkSrc = kAdc16ClkSrcOfBusClk;
    configStruct.asyncClkEnable = false;
    configStruct.highSpeedEnable = false;
    configStruct.longSampleCycleMode = kAdc16LongSampleCycleOf24;
    configStruct.hwTriggerEnable = false;
    configStruct.refVoltSrc = kAdc16RefVoltSrcOfVref;
    configStruct.continuousConvEnable = false;
#if FSL_FEATURE_ADC16_HAS_DMA
    configStruct.dmaEnable = false;
#endif /* FSL_FEATURE_ADC16_HAS_DMA */
    ADC16_HAL_ConfigConverter(base, &configStruct);
    if (0U != ADC_BRD_CFG1_ADLPC(base))
    {
        errCounter++;
    }
    if ((uint32_t)(kAdc16ClkDividerOf1) != ADC_BRD_CFG1_ADIV(base))
    {
        errCounter++;
    }
    if (0U != ADC_BRD_CFG1_ADLSMP(base))
    {
        errCounter++;
    }
    if ((uint32_t)(kAdc16ResolutionBitOf8or9) != ADC_BRD_CFG1_MODE(base))
    {
        errCounter++;
    }
    if ((uint32_t)(kAdc16ClkSrcOfBusClk) != ADC_BRD_CFG1_ADICLK(base))
    {
        errCounter++;
    }
    if (0U != ADC_BRD_CFG2_ADACKEN(base))
    {
        errCounter++;
    }
    if (0U != ADC_BRD_CFG2_ADHSC(base))
    {
        errCounter++;
    }
    if ((uint32_t)(kAdc16LongSampleCycleOf24) != ADC_BRD_CFG2_ADLSTS(base))
    {
        errCounter++;
    }
    if (0U != ADC_BRD_SC2_ADTRG(base))
    {
        errCounter++;
    }
    if ((uint32_t)(kAdc16RefVoltSrcOfVref) != ADC_BRD_SC2_REFSEL(base))
    {
        errCounter++;
    }
    if (0U != ADC_BRD_SC3_ADCO(base))
    {
        errCounter++;
    }
#if FSL_FEATURE_ADC16_HAS_DMA
    if (0U != ADC_BRD_SC2_DMAEN(base))
    {
        errCounter++;
    }
#endif /* FSL_FEATURE_ADC16_HAS_DMA */

    ADC16_HAL_Init(base);

    return errCounter;
}

static uint32_t ADC16_HAL_TEST_ConfigHwCompare(ADC_Type *base)
{
    uint32_t errCounter = 0U;
    adc16_hw_cmp_config_t configStruct;

    ADC16_HAL_Init(base);

    /* Black. */
    configStruct.hwCmpEnable = true;
    configStruct.hwCmpGreaterThanEnable = true;
    configStruct.hwCmpRangeEnable = true;
    configStruct.cmpValue1 = 0xFFFU;
    configStruct.cmpValue2 = 0xFFFU;
    ADC16_HAL_ConfigHwCompare(base, &configStruct);
    if (1U != ADC_BRD_SC2_ACFE(base))
    {
        errCounter++;
    }
    if (1U != ADC_BRD_SC2_ACFGT(base))
    {
        errCounter++;
    }
    if (1U != ADC_BRD_SC2_ACREN(base))
    {
        errCounter++;
    }
    if (0xFFFU != ADC_BRD_CV1_CV(base))
    {
        errCounter++;
    }
    if (0xFFFU != ADC_BRD_CV2_CV(base))
    {
        errCounter++;
    }

    /* White. */
    configStruct.hwCmpEnable = false;
    configStruct.hwCmpGreaterThanEnable = false;
    configStruct.hwCmpRangeEnable = false;
    configStruct.cmpValue1 = 0U;
    configStruct.cmpValue2 = 0U;
    ADC16_HAL_ConfigHwCompare(base, &configStruct);
    if (0U != ADC_BRD_SC2_ACFE(base))
    {
        errCounter++;
    }
    if (0U != ADC_BRD_SC2_ACFGT(base))
    {
        errCounter++;
    }
    if (0U != ADC_BRD_SC2_ACREN(base))
    {
        errCounter++;
    }
    if (0U != ADC_BRD_CV1_CV(base))
    {
        errCounter++;
    }
    if (0U != ADC_BRD_CV2_CV(base))
    {
        errCounter++;
    }

    ADC16_HAL_Init(base);
    return errCounter;
}

#if FSL_FEATURE_ADC16_HAS_HW_AVERAGE
static uint32_t ADC16_HAL_TEST_ConfigHwAverage(ADC_Type *base)
{
    uint32_t errCounter = 0U;
    adc16_hw_average_config_t configStruct;

    ADC16_HAL_Init(base);

    /* Black. */
    configStruct.hwAverageEnable = true;
    configStruct.hwAverageCountMode = kAdc16HwAverageCountOf32;
    ADC16_HAL_ConfigHwAverage(base, &configStruct);
    if (1U != ADC_BRD_SC3_AVGE(base))
    {
        errCounter++;
    }
    if ((uint32_t)kAdc16HwAverageCountOf32 != ADC_BRD_SC3_AVGS(base))
    {
        errCounter++;
    }

    /* White. */
    configStruct.hwAverageEnable = false;
    configStruct.hwAverageCountMode = kAdc16HwAverageCountOf4;
    ADC16_HAL_ConfigHwAverage(base, &configStruct);
    if (0U != ADC_BRD_SC3_AVGE(base))
    {
        errCounter++;
    }
    if ((uint32_t)kAdc16HwAverageCountOf4 != ADC_BRD_SC3_AVGS(base))
    {
        errCounter++;
    }

    ADC16_HAL_Init(base);

    return errCounter;
}
#endif /* FSL_FEATURE_ADC16_HAS_HW_AVERAGE */

#if FSL_FEATURE_ADC16_HAS_MUX_SELECT
static uint32_t ADC16_HAL_TEST_SetChnMuxMode(ADC_Type *base)
{
    uint32_t errCounter = 0U;

    ADC16_HAL_Init(base);
    /* Black. */
    ADC16_HAL_SetChnMuxMode(base, kAdc16ChnMuxOfB);
    if ((uint32_t)kAdc16ChnMuxOfB != ADC_BRD_CFG2_MUXSEL(base))
    {
        errCounter++;
    }

    /* White. */
    ADC16_HAL_SetChnMuxMode(base, kAdc16ChnMuxOfA);
    if ((uint32_t)kAdc16ChnMuxOfA != ADC_BRD_CFG2_MUXSEL(base))
    {
        errCounter++;
    }

    return errCounter;
}
#endif /* FSL_FEATURE_ADC16_HAS_MUX_SELECT */

#if FSL_FEATURE_ADC16_HAS_CALIBRATION
static uint32_t ADC_HAL_TEST_SetAutoCalibration(ADC_Type *base)
{
    uint32_t errCounter = 0U;

    ADC16_HAL_Init(base);
    ADC16_HAL_SetPlusSideGainValue(base, 0xFFFFU);
    if (0xFFFFU != ADC_BRD_PG_PG(base))
    {
        errCounter++;
    }
    ADC16_HAL_SetPlusSideGainValue(base, 0U);
    if (0U != ADC_BRD_PG_PG(base))
    {
        errCounter++;
    }

#if FSL_FEATURE_ADC16_HAS_DIFF_MODE
    ADC16_HAL_SetMinusSideGainValue(base, 0xFFFFU);
    if (0xFFFFU != ADC_BRD_MG_MG(base))
    {
        errCounter++;
    }
    ADC16_HAL_SetMinusSideGainValue(base, 0U);
    if (0U != ADC_BRD_MG_MG(base))
    {
        errCounter++;
    }
#endif /* FSL_FEATURE_ADC16_HAS_DIFF_MODE */

#if FSL_FEATURE_ADC16_HAS_OFFSET_CORRECTION
    ADC16_HAL_SetOffsetValue(base, 0xFFFFU);
    if (0xFFFF != ADC16_HAL_GetOffsetValue(base))
    {
        errCounter++;
    }
    ADC16_HAL_SetOffsetValue(base, 0U);
    if (0U != ADC16_HAL_GetOffsetValue(base))
    {
        errCounter++;
    }
#endif /* FSL_FEATURE_ADC16_HAS_OFFSET_CORRECTION */

    ADC16_HAL_Init(base);
    return errCounter;
}
#endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */

/* EOF. */
