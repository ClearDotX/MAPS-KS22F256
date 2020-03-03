/* pdb_hal_test.c */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "fsl_clock_manager.h"
#include "fsl_pdb_hal.h"
#include "board.h"

static uint32_t PDB_HAL_TEST_ConfigTimer(PDB_Type * base);
static uint32_t PDB_HAL_TEST_ConfigAdcPreTrigger(PDB_Type * base);
static uint32_t PDB_HAL_TEST_ConfigDacTrigger(PDB_Type * base);
static uint32_t PDB_HAL_TEST_ConfigCmpPulseOut(PDB_Type * base);

static PDB_Type * gPdbBasePtr[] = PDB_BASE_PTRS;

int main(void)
{
    uint32_t idx;
    uint32_t errCounter = 0U;
    uint32_t totalErrCounter = 0U;
    
    hardware_init();
    dbg_uart_init();
    
    printf("\r\nPDB HAL Test: Start...\r\n");
    
    for (idx = 0U; idx < PDB_INSTANCE_COUNT; idx++)
    {
        /* Enable the clock. */
        CLOCK_SYS_EnablePdbClock(idx);
        errCounter = 0U;

        PDB_HAL_Enable(gPdbBasePtr[idx]);
        if (1U != PDB_BRD_SC_PDBEN(gPdbBasePtr[idx]) )
        {
            errCounter++;
        }
        errCounter += PDB_HAL_TEST_ConfigTimer(gPdbBasePtr[idx]);
        errCounter += PDB_HAL_TEST_ConfigAdcPreTrigger(gPdbBasePtr[idx]);
        errCounter += PDB_HAL_TEST_ConfigDacTrigger(gPdbBasePtr[idx]);
        errCounter += PDB_HAL_TEST_ConfigCmpPulseOut(gPdbBasePtr[idx]);
        PDB_HAL_Disable(gPdbBasePtr[idx]);
        if (0U != PDB_BRD_SC_PDBEN(gPdbBasePtr[idx]) )
        {
            errCounter++;
        }
        printf("PDB %ld HAL Test error counter: %ld\r\n", idx, errCounter);
        totalErrCounter += errCounter;
    }
    
    printf("PDB HAL Test: End\r\n");

    /* Print log for auto run detection. */
    printf("PDB HAL Test ");
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

static uint32_t PDB_HAL_TEST_ConfigTimer(PDB_Type * base)
{
    uint32_t errCounter = 0;
    pdb_timer_config_t ConfigStruct;
    
    /* Black. */
    ConfigStruct.loadValueMode = kPdbLoadValueAtModuloCounterOrNextTrigger;
    ConfigStruct.seqErrIntEnable = true;
    ConfigStruct.clkPreDiv = kPdbClkPreDivBy128;
    ConfigStruct.clkPreMultFactor = kPdbClkPreMultFactorAs40;
    ConfigStruct.triggerInput = kPdbSoftTrigger;
    ConfigStruct.continuousModeEnable = true;
    ConfigStruct.dmaEnable = true;
    ConfigStruct.intEnable = true;
    PDB_HAL_ConfigTimer(base, &ConfigStruct);
    
    if ((uint32_t)kPdbLoadValueAtModuloCounterOrNextTrigger != PDB_BRD_SC_LDMOD(base) )
    {
        errCounter++;
    }
    if (1U != PDB_BRD_SC_PDBEIE(base) )
    {
        errCounter++;
    }
    if ((uint32_t)kPdbClkPreDivBy128 != PDB_BRD_SC_PRESCALER(base) )
    {
        errCounter++;
    }
        
    if ((uint32_t)kPdbClkPreMultFactorAs40 != PDB_BRD_SC_MULT(base) )
    {
        errCounter++;
    }
    if ((uint32_t)kPdbSoftTrigger != PDB_BRD_SC_TRGSEL(base) )
    {
        errCounter++;
    }
    if (1U != PDB_BRD_SC_CONT(base) )
    {
        errCounter++;
    }
    if (1U != PDB_BRD_SC_DMAEN(base) )
    {
        errCounter++;
    }
    if (1U != PDB_BRD_SC_PDBIE(base) )
    {
        errCounter++;
    }

    /* White. */
    ConfigStruct.loadValueMode = kPdbLoadValueImmediately;
    ConfigStruct.seqErrIntEnable = false;
    ConfigStruct.clkPreDiv = kPdbClkPreDivBy1;
    ConfigStruct.clkPreMultFactor = kPdbClkPreMultFactorAs1;
    ConfigStruct.triggerInput = kPdbTrigger0;
    ConfigStruct.continuousModeEnable = false;
    ConfigStruct.dmaEnable = false;
    ConfigStruct.intEnable = false;
    PDB_HAL_ConfigTimer(base, &ConfigStruct);
    
    if (0U != PDB_BRD_SC_LDMOD(base) )
    {
        errCounter++;
    }
    if (0U != PDB_BRD_SC_PDBEIE(base) )
    {
        errCounter++;
    }
        if (0U != PDB_BRD_SC_PRESCALER(base) )
    {
        errCounter++;
    }
        
    if (0U != PDB_BRD_SC_MULT(base) )
    {
        errCounter++;
    }
        if (0U != PDB_BRD_SC_TRGSEL(base) )
    {
        errCounter++;
    }
    if (0U != PDB_BRD_SC_CONT(base) )
    {
        errCounter++;
    }
    if (0U != PDB_BRD_SC_DMAEN(base) )
    {
        errCounter++;
    }
    if (0U != PDB_BRD_SC_PDBIE(base) )
    {
        errCounter++;
    }
    
    /* Black. */
    PDB_HAL_SetTimerModulusValue(base, 0xFF);
    PDB_HAL_SetLoadValuesCmd(base);
    if (0xFF != PDB_BRD_MOD_MOD(base) )
    {
        errCounter++;
    }
        
    /* White. */
    PDB_HAL_SetTimerModulusValue(base, 0U);
    PDB_HAL_SetLoadValuesCmd(base);
    if (0U != PDB_BRD_MOD_MOD(base) )
    {
        errCounter++;
    }

    /* Black. */
    PDB_HAL_SetValueForTimerInterrupt(base, 0xFF);
    PDB_HAL_SetLoadValuesCmd(base);
    if (0xFF != PDB_BRD_IDLY_IDLY(base) )
    {
        errCounter++;
    }
        
    /* White. */
    PDB_HAL_SetValueForTimerInterrupt(base, 0U);
    PDB_HAL_SetLoadValuesCmd(base);
    if (0U != PDB_BRD_IDLY_IDLY(base) )
    {
        errCounter++;
    }

    return errCounter;
}

static uint32_t PDB_HAL_TEST_ConfigAdcPreTrigger(PDB_Type * base)
{
    uint32_t preChn, chn;
    uint32_t errCounter = 0;
    
    for(chn = 0;chn < PDB_C1_COUNT; chn++)
    {
        for(preChn = 0; preChn < FSL_FEATURE_PDB_ADC_PRE_CHANNEL_COUNT; preChn++)
        {
            /* Black. */
            PDB_HAL_SetAdcPreTriggerBackToBackEnable(base, chn, 1U << preChn, true); 
            if( (1U << preChn) != (PDB_BRD_C1_BB(base, chn) & (1U << preChn) ) )
            {
                errCounter++;
            }
            /* White. */
            PDB_HAL_SetAdcPreTriggerBackToBackEnable(base, chn, 1U << preChn, false);
            if ( 0U != (PDB_BRD_C1_BB(base, chn) & (1U << preChn) ) )
            {
                errCounter++;
            }

            /* Black. */
            PDB_HAL_SetAdcPreTriggerOutputEnable(base, chn, 1U << preChn, true);
            if( (1U << preChn) != (PDB_BRD_C1_TOS(base, chn) & (1U << preChn) ) )
            {
                errCounter++;
            } 
            /* While. */
            PDB_HAL_SetAdcPreTriggerOutputEnable(base, chn, 1U << preChn, false);
            if( 0U != (PDB_BRD_C1_TOS(base, chn) & (1U << preChn) ) )
            {
                errCounter++;
            }

            /* Black. */
            PDB_HAL_SetAdcPreTriggerEnable(base, chn, 1U << preChn, true);
            if( (1U << preChn) != (PDB_BRD_C1_EN(base, chn) & (1U << preChn) ) )
            {
                errCounter++;
            }
            /* White. */
            PDB_HAL_SetAdcPreTriggerEnable(base, chn, 1U << preChn, false);
            if( 0U != (PDB_BRD_C1_EN(base, chn) & (1U << preChn) ) )
            {
                errCounter++;
            }
            /* Black. */
            PDB_HAL_SetAdcPreTriggerDelayValue(base, chn,  preChn, 0xFFFF);
            PDB_HAL_SetLoadValuesCmd(base);
            if( 0xFFFF != PDB_BRD_DLY_DLY(base, chn, preChn))
            {
                errCounter++;
            }
            /* White. */
            PDB_HAL_SetAdcPreTriggerDelayValue(base, chn,  preChn, 0U);
            PDB_HAL_SetLoadValuesCmd(base);
            if( 0U != PDB_BRD_DLY_DLY(base, chn, preChn))
            {
                errCounter++;
            } 
        }
    }
    return errCounter;
}

static uint32_t PDB_HAL_TEST_ConfigDacTrigger(PDB_Type * base)
{
    uint32_t errCounter = 0;
    uint32_t dacChn;
    
    for(dacChn = 0; dacChn < PDB_INTC_COUNT; dacChn++)
    {
        /*Black*/
        PDB_HAL_SetDacExtTriggerInputEnable(base, dacChn, true);
        if( 1U != PDB_BRD_INTC_EXT(base, dacChn) )
        {
            errCounter++;
        }
        PDB_HAL_SetDacExtTriggerInputEnable(base, dacChn, false);
        if( 0U != PDB_BRD_INTC_EXT(base, dacChn) )
        {
            errCounter++;
        }

        PDB_HAL_SetDacIntervalTriggerEnable(base, dacChn, true);    
        if( 1U != PDB_BRD_INTC_TOE(base, dacChn))
        {
            errCounter++;
        }
        PDB_HAL_SetDacIntervalTriggerEnable(base, dacChn, false);    
        if( 0U != PDB_BRD_INTC_TOE(base, dacChn))
        {
            errCounter++;
        }

        PDB_HAL_SetDacIntervalValue(base, dacChn, 0xFF);
        PDB_HAL_SetLoadValuesCmd(base);
        if( 0xFF != PDB_BRD_INT_INT(base, dacChn))
        {
            errCounter++;
        }
        PDB_HAL_SetDacIntervalValue(base, dacChn, 0U);
        PDB_HAL_SetLoadValuesCmd(base);
        if( 0U != PDB_BRD_INT_INT(base, dacChn))
        {
            errCounter++;
        }
    }
    return errCounter;
}

static uint32_t PDB_HAL_TEST_ConfigCmpPulseOut(PDB_Type * base)
{
    uint32_t errCounter = 0;
    uint32_t pulseChn;
    for(uint32_t pulseChn = 0; pulseChn < PDB_PODLY_COUNT; pulseChn++)
    {
        PDB_HAL_SetCmpPulseOutEnable(base, 1U << pulseChn, true);
        if( (1U <<pulseChn) != (PDB_BRD_POEN_POEN(base) & (1U <<pulseChn) ) )
        {
            errCounter++;
        } 
        PDB_HAL_SetCmpPulseOutEnable(base, 1U << pulseChn, false);
        if( 0U != (PDB_BRD_POEN_POEN(base) & (1U <<pulseChn) ) )
        {
            errCounter++;
        } 

        PDB_HAL_SetCmpPulseOutDelayForHigh(base, pulseChn, 0xFFFF);
        PDB_HAL_SetLoadValuesCmd(base);
        if( 0xFFFF != PDB_BRD_PODLY_DLY1(base, pulseChn))
        {
            errCounter++;
        }
        PDB_HAL_SetCmpPulseOutDelayForHigh(base, pulseChn, 0U);
        PDB_HAL_SetLoadValuesCmd(base);
        if( 0U != PDB_BRD_PODLY_DLY1(base, pulseChn))
        {
            errCounter++;
        }

        PDB_HAL_SetCmpPulseOutDelayForLow(base, pulseChn, 0xFFFF);
        PDB_HAL_SetLoadValuesCmd(base);
        if( 0xFFFF != PDB_BRD_PODLY_DLY2(base, pulseChn))
        {
            errCounter++;
        }
        PDB_HAL_SetCmpPulseOutDelayForLow(base, pulseChn, 0U);
        PDB_HAL_SetLoadValuesCmd(base);
        if( 0U != PDB_BRD_PODLY_DLY2(base, pulseChn))
        {
            errCounter++;
        }
    }
    return errCounter++;
}

/* EOF. */
