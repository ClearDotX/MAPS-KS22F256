/* flexio_i2c_hal_test.c */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "fsl_flexio_hal.h"
#include "fsl_clock_manager.h"
#include "board.h"

FLEXIO_Type * gFlexioBasePtr[] = FLEXIO_BASE_PTRS;

static void hardware_init_flexio_clock_source(void);
static uint32_t FLEXIO_HAL_TEST_ConfigMisc(FLEXIO_Type * base);
static uint32_t FLEXIO_HAL_TEST_ConfigTimer(FLEXIO_Type * base, uint32_t timerIdx);
static uint32_t FLEXIO_HAL_TEST_ConfigShifter(FLEXIO_Type * base, uint32_t shifterIdx);

int main(void)
{
    uint32_t instance;
    uint32_t errCounter = 0U;
    uint32_t totalErrCounter = 0U;
    uint32_t timerCount, timerIdx;
    uint32_t shifterCount, shifterIdx;


    hardware_init();
    dbg_uart_init();

    hardware_init_flexio_clock_source();
    printf("\r\nflexio HAL Test: Start...\r\n");

    //CLOCK_SYS_EnableFlexioClock(0U);

    for (instance = 0U; instance < FLEXIO_INSTANCE_COUNT; instance++)
    {
        errCounter = 0U;
        CLOCK_SYS_EnableFlexioClock(instance);

        /* Show the information about the flexio. */
        printf("flexio %ld info:\r\n", instance);
        printf(" - MajorVersionNumber: %ld\r\n", FLEXIO_HAL_GetMajorVersionNumber(gFlexioBasePtr[instance]) );
        printf(" - MinorVersionNumber: %ld\r\n", FLEXIO_HAL_GetMinorVersionNumber(gFlexioBasePtr[instance]) );
        printf(" - FeatureNumber: %ld\r\n", FLEXIO_HAL_GetFeatureNumber(gFlexioBasePtr[instance]) );
        printf(" - TriggerNumber: %ld\r\n", FLEXIO_HAL_GetTriggerNumber(gFlexioBasePtr[instance]) );
        printf(" - PinNumber: %ld\r\n", FLEXIO_HAL_GetPinNumber(gFlexioBasePtr[instance]) );
        timerCount = FLEXIO_HAL_GetTimerNumber(gFlexioBasePtr[instance]);
        printf(" - TimerNumber: %ld\r\n", timerCount);
        shifterCount = FLEXIO_HAL_GetShifterNumber(gFlexioBasePtr[instance]);
        printf(" - ShifterNumber: %ld\r\n", shifterCount);

        /* Test the register's setting. */
        errCounter += FLEXIO_HAL_TEST_ConfigMisc(gFlexioBasePtr[instance]);
        for (timerIdx = 0U; timerIdx < timerCount; timerIdx++)
        {
            errCounter += FLEXIO_HAL_TEST_ConfigTimer(gFlexioBasePtr[instance], timerIdx);
        }
        for (shifterIdx = 0U; shifterIdx < shifterCount; shifterIdx++)
        {
            errCounter += FLEXIO_HAL_TEST_ConfigShifter(gFlexioBasePtr[instance], shifterIdx);
        }
        printf("Error: %ld\r\n", errCounter);
        CLOCK_SYS_DisableFlexioClock(instance);
        totalErrCounter += errCounter;
    }

    printf("flexio HAL Test: End\r\n");

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

static void hardware_init_flexio_clock_source(void)
{
    uint32_t tmp = SIM->SOPT2 & ~SIM_SOPT2_FLEXIOSRC_MASK;
    tmp |= SIM_SOPT2_FLEXIOSRC(1U); /* IRC48M clock. */
    SIM->SOPT2 = tmp;
}

static uint32_t FLEXIO_HAL_TEST_ConfigMisc(FLEXIO_Type * base)
{
    uint32_t errCounter = 0U;

    /* FLEXIO_HAL_SetDozeModeCmd. */
    FLEXIO_HAL_SetDozeModeCmd(base, true);
    if (1U != FLEXIO_BRD_CTRL_DOZEN(base) )
    {
        errCounter++;
    }
    FLEXIO_HAL_SetDozeModeCmd(base, false);
    if (0U != FLEXIO_BRD_CTRL_DOZEN(base) )
    {
        errCounter++;
    }

    /* FLEXIO_HAL_SetDebugModeCmd */
    FLEXIO_HAL_SetDebugModeCmd(base, true);
    if (1U != FLEXIO_BRD_CTRL_DBGE(base) )
    {
        errCounter++;
    }
    FLEXIO_HAL_SetDebugModeCmd(base, false);
    if (0U != FLEXIO_BRD_CTRL_DBGE(base) )
    {
        errCounter++;
    }

    /* FLEXIO_HAL_SetFastAccessCmd */
    FLEXIO_HAL_SetFastAccessCmd(base, true);
    if (1U != FLEXIO_BRD_CTRL_FASTACC(base) )
    {
        errCounter++;
    }
    FLEXIO_HAL_SetFastAccessCmd(base, false);
    if (0U != FLEXIO_BRD_CTRL_FASTACC(base) )
    {
        errCounter++;
    }

    /* FLEXIO_HAL_SetFlexioEnableCmd */
    FLEXIO_HAL_SetFlexioEnableCmd(base, true);
    if (1U != FLEXIO_BRD_CTRL_FLEXEN(base) )
    {
        errCounter++;
    }
    FLEXIO_HAL_SetFlexioEnableCmd(base, false);
    if (0U != FLEXIO_BRD_CTRL_FLEXEN(base) )
    {
        errCounter++;
    }
    FLEXIO_HAL_SetFlexioEnableCmd(base, true);

    return errCounter;
}
static uint32_t FLEXIO_HAL_TEST_ConfigTimer(FLEXIO_Type * base, uint32_t timerIdx)
{
    uint32_t errCounter = 0U;
    flexio_timer_config_t timerConfigStruct;

    /* FLEXIO_HAL_SetTimerStatusIntCmd */
    FLEXIO_HAL_SetTimerStatusIntCmd(base, 1U<<timerIdx, true);
    if (0U == ((1U<<timerIdx)&FLEXIO_BRD_TIMIEN_TEIE(base) ) )
    {
        errCounter++;
    }
    FLEXIO_HAL_SetTimerStatusIntCmd(base, 1U<<timerIdx, false);
    if (0U != ((1U<<timerIdx)&FLEXIO_BRD_TIMIEN_TEIE(base) ) )
    {
        errCounter++;
    }

    /* FLEXIO_HAL_ConfigureTimer */
    /* black. */
    timerConfigStruct.trgsel = 0xFU;
    timerConfigStruct.trgpol = kFlexioTimerTriggerPolarityActiveLow;
    timerConfigStruct.trgsrc = kFlexioTimerTriggerSourceInternal;
    timerConfigStruct.pincfg = kFlexioPinConfigOutput;
    timerConfigStruct.pinsel = 0x7U;
    timerConfigStruct.pinpol = kFlexioPinActiveLow;
    timerConfigStruct.timod  = kFlexioTimerModeSingle16Bit;
    timerConfigStruct.timout = kFlexioTimerOutputZeroAffectedByReset;
    timerConfigStruct.timdec = kFlexioTimerDecSrcOnTriggerInputShiftTriggerInput;
    timerConfigStruct.timrst = kFlexioTimerResetOnTimerTriggerBothEdge;
    timerConfigStruct.timdis = kFlexioTimerDisableOnTriggerFallingEdge;
    timerConfigStruct.timena = kFlexioTimerEnableOnTriggerBothEdge;
    timerConfigStruct.tstop  = kFlexioTimerStopBitEnableOnTimerCompareDisable;
    timerConfigStruct.tstart = kFlexioTimerStartBitEnabled;
    timerConfigStruct.timcmp = 0xFFFFU;
    FLEXIO_HAL_ConfigureTimer(base, timerIdx, &timerConfigStruct);
    if (0xFU != FLEXIO_BRD_TIMCTL_TRGSEL(base, timerIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioTimerTriggerPolarityActiveLow != FLEXIO_BRD_TIMCTL_TRGPOL(base, timerIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioTimerTriggerSourceInternal != FLEXIO_BRD_TIMCTL_TRGSRC(base, timerIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioPinConfigOutput != FLEXIO_BRD_TIMCTL_PINCFG(base, timerIdx))
    {
        errCounter++;
    }
    if (0x7U != FLEXIO_BRD_TIMCTL_PINSEL(base, timerIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioPinActiveLow != FLEXIO_BRD_TIMCTL_PINPOL(base, timerIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioTimerModeSingle16Bit != FLEXIO_BRD_TIMCTL_TIMOD(base, timerIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioTimerOutputZeroAffectedByReset != FLEXIO_BRD_TIMCFG_TIMOUT(base, timerIdx)) 
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioTimerDecSrcOnTriggerInputShiftTriggerInput != FLEXIO_BRD_TIMCFG_TIMDEC(base, timerIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioTimerResetOnTimerTriggerBothEdge != FLEXIO_BRD_TIMCFG_TIMRST(base, timerIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioTimerDisableOnTriggerFallingEdge != FLEXIO_BRD_TIMCFG_TIMDIS(base, timerIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioTimerEnableOnTriggerBothEdge != FLEXIO_BRD_TIMCFG_TIMENA(base, timerIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioTimerStopBitEnableOnTimerCompareDisable != FLEXIO_BRD_TIMCFG_TSTOP(base, timerIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioTimerStartBitEnabled != FLEXIO_BRD_TIMCFG_TSTART(base, timerIdx))
    {
        errCounter++;
    }
    if (0xFFFFU != FLEXIO_BRD_TIMCMP_CMP(base, timerIdx))
    {
        errCounter++;
    }

    /* white. */
    timerConfigStruct.trgsel = 0U;
    timerConfigStruct.trgpol = kFlexioTimerTriggerPolarityActiveHigh;
    timerConfigStruct.trgsrc = kFlexioTimerTriggerSourceExternal;
    timerConfigStruct.pincfg = kFlexioPinConfigOutputDisabled;
    timerConfigStruct.pinsel = 0U;
    timerConfigStruct.pinpol = kFlexioPinActiveHigh;
    timerConfigStruct.timod  = kFlexioTimerModeDisabled;
    timerConfigStruct.timout = kFlexioTimerOutputOneNotAffectedByReset;
    timerConfigStruct.timdec = kFlexioTimerDecSrcOnFlexIOClockShiftTimerOutput;
    timerConfigStruct.timrst = kFlexioTimerResetNever;
    timerConfigStruct.timdis = kFlexioTimerDisableNever;
    timerConfigStruct.timena = kFlexioTimerEnabledAlways;
    timerConfigStruct.tstop  = kFlexioTimerStopBitDisabled;
    timerConfigStruct.tstart = kFlexioTimerStartBitDisabled;
    timerConfigStruct.timcmp = 0U;
    FLEXIO_HAL_ConfigureTimer(base, timerIdx, &timerConfigStruct);
    if (0U != FLEXIO_BRD_TIMCTL_TRGSEL(base, timerIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioTimerTriggerPolarityActiveHigh != FLEXIO_BRD_TIMCTL_TRGPOL(base, timerIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioTimerTriggerSourceExternal != FLEXIO_BRD_TIMCTL_TRGSRC(base, timerIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioPinConfigOutputDisabled != FLEXIO_BRD_TIMCTL_PINCFG(base, timerIdx))
    {
        errCounter++;
    }
    if (0U != FLEXIO_BRD_TIMCTL_PINSEL(base, timerIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioPinActiveHigh != FLEXIO_BRD_TIMCTL_PINPOL(base, timerIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioTimerModeDisabled != FLEXIO_BRD_TIMCTL_TIMOD(base, timerIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioTimerOutputOneNotAffectedByReset != FLEXIO_BRD_TIMCFG_TIMOUT(base, timerIdx)) 
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioTimerDecSrcOnFlexIOClockShiftTimerOutput != FLEXIO_BRD_TIMCFG_TIMDEC(base, timerIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioTimerResetNever != FLEXIO_BRD_TIMCFG_TIMRST(base, timerIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioTimerDisableNever != FLEXIO_BRD_TIMCFG_TIMDIS(base, timerIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioTimerEnabledAlways != FLEXIO_BRD_TIMCFG_TIMENA(base, timerIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioTimerStopBitDisabled != FLEXIO_BRD_TIMCFG_TSTOP(base, timerIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioTimerStartBitDisabled != FLEXIO_BRD_TIMCFG_TSTART(base, timerIdx))
    {
        errCounter++;
    }
    if (0U != FLEXIO_BRD_TIMCMP_CMP(base, timerIdx))
    {
        errCounter++;
    }

    return errCounter;
}

static uint32_t FLEXIO_HAL_TEST_ConfigShifter(FLEXIO_Type * base, uint32_t shifterIdx)
{
    uint32_t errCounter = 0U;
    flexio_shifter_config_t shifterConfigStruct;

    /* FLEXIO_HAL_SetShifterStatusIntCmd */
    FLEXIO_HAL_SetShifterStatusIntCmd(base, 1U<<shifterIdx, true);
    if ( 0U == (1U<<shifterIdx)&&FLEXIO_HAL_GetShifterStatusIntCmd(base))
    {
        errCounter++;
    }
    FLEXIO_HAL_SetShifterStatusIntCmd(base, 1U<<shifterIdx, false);
    if ( 0U != (1U<<shifterIdx)&&FLEXIO_HAL_GetShifterStatusIntCmd(base))
    {
        errCounter++;
    }

    /* FLEXIO_HAL_SetShifterErrorIntCmd */
    FLEXIO_HAL_SetShifterErrorIntCmd(base, 1U<<shifterIdx, true);
    if (0U == (1U<<shifterIdx)&FLEXIO_HAL_GetShifterErrorInt(base))
    {
        errCounter++;
    }
    FLEXIO_HAL_SetShifterErrorIntCmd(base, 1U<<shifterIdx, false);
    if (0U != (1U<<shifterIdx)&FLEXIO_HAL_GetShifterErrorInt(base))
    {
        errCounter++;
    }

    /* FLEXIO_HAL_ConfigureShifter */
    /* Black. */
    shifterConfigStruct.timsel = 0x3U;
    shifterConfigStruct.timpol = kFlexioShifterTimerPolarityOnNegitive;
    shifterConfigStruct.pincfg = kFlexioPinConfigOutput;
    shifterConfigStruct.pinsel = 0x7U;
    shifterConfigStruct.pinpol = kFlexioPinActiveLow;
    shifterConfigStruct.smode  = kFlexioShifterModeMatchContinuous;
    shifterConfigStruct.insrc  = kFlexioShifterInputFromNextShifterOutput;
    shifterConfigStruct.sstop  = kFlexioShifterStopBitHigh;
    shifterConfigStruct.sstart = kFlexioShifterStartBitHigh;
    FLEXIO_HAL_ConfigureShifter(base, shifterIdx, &shifterConfigStruct);
    if (0x3U != FLEXIO_BRD_SHIFTCTL_TIMSEL(base, shifterIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioShifterTimerPolarityOnNegitive != FLEXIO_BRD_SHIFTCTL_TIMPOL(base, shifterIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioPinConfigOutput != FLEXIO_BRD_SHIFTCTL_PINCFG(base, shifterIdx))
    {
        errCounter++;
    }
    if (0x7U != FLEXIO_BRD_SHIFTCTL_PINSEL(base, shifterIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioPinActiveLow != FLEXIO_BRD_SHIFTCTL_PINPOL(base, shifterIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioShifterModeMatchContinuous != FLEXIO_BRD_SHIFTCTL_SMOD(base, shifterIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioShifterInputFromNextShifterOutput != FLEXIO_BRD_SHIFTCFG_INSRC(base, shifterIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioShifterStopBitHigh != FLEXIO_BRD_SHIFTCFG_SSTOP(base, shifterIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioShifterStartBitHigh != FLEXIO_BRD_SHIFTCFG_SSTART(base, shifterIdx))
    {
        errCounter++;
    }

    /* White. */
    shifterConfigStruct.timsel = 0U;
    shifterConfigStruct.timpol = kFlexioShifterTimerPolarityOnPositive;
    shifterConfigStruct.pincfg = kFlexioPinConfigOutputDisabled;
    shifterConfigStruct.pinsel = 0U;
    shifterConfigStruct.pinpol = kFlexioPinActiveHigh;
    shifterConfigStruct.smode  = kFlexioShifterDisabled;
    shifterConfigStruct.insrc  = kFlexioShifterInputFromPin;
    shifterConfigStruct.sstop  = kFlexioShifterStopBitDisable;
    shifterConfigStruct.sstart = kFlexioShifterStartBitDisabledLoadDataOnEnable;
    FLEXIO_HAL_ConfigureShifter(base, shifterIdx, &shifterConfigStruct);
    if (0U != FLEXIO_BRD_SHIFTCTL_TIMSEL(base, shifterIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioShifterTimerPolarityOnPositive != FLEXIO_BRD_SHIFTCTL_TIMPOL(base, shifterIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioPinConfigOutputDisabled != FLEXIO_BRD_SHIFTCTL_PINCFG(base, shifterIdx))
    {
        errCounter++;
    }
    if (0U != FLEXIO_BRD_SHIFTCTL_PINSEL(base, shifterIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioPinActiveHigh != FLEXIO_BRD_SHIFTCTL_PINPOL(base, shifterIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioShifterDisabled != FLEXIO_BRD_SHIFTCTL_SMOD(base, shifterIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioShifterInputFromPin != FLEXIO_BRD_SHIFTCFG_INSRC(base, shifterIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioShifterStopBitDisable != FLEXIO_BRD_SHIFTCFG_SSTOP(base, shifterIdx))
    {
        errCounter++;
    }
    if ((uint32_t)kFlexioShifterStartBitDisabledLoadDataOnEnable != FLEXIO_BRD_SHIFTCFG_SSTART(base, shifterIdx))
    {
        errCounter++;
    }
    
    return errCounter;
}

/* EOF. */


