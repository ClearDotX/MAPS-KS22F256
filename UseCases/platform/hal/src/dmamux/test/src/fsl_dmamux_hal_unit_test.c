#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "fsl_dmamux_hal.h"
#include "fsl_device_registers.h"
#include "fsl_clock_manager.h"
#include "board.h"
#include <assert.h>


#define ERROR_DEBUG() printf("DMAMUX HAL UNITTEST FAILED: function[%s] line[%d]\r\n", __func__, __LINE__)

extern DMAMUX_Type * const g_dmamuxBase[];

int32_t dmamux_hal_test_enable_channel(uint8_t module, uint8_t channel)
{
    DMAMUX_Type * baseAddr = g_dmamuxBase[module];
    DMAMUX_HAL_SetChannelCmd(baseAddr, channel,true);

    if (!DMAMUX_BRD_CHCFG_ENBL(baseAddr, channel))
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t dmamux_hal_test_disable_channel(uint8_t module, uint8_t channel)
{
    DMAMUX_Type * baseAddr = g_dmamuxBase[module];
    DMAMUX_HAL_SetChannelCmd(baseAddr, channel,false);

    if (DMAMUX_BRD_CHCFG_ENBL(baseAddr, channel))
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

#if FSL_FEATURE_DMAMUX_HAS_TRIG
int32_t dmamux_hal_test_enable_period_trigger(uint8_t module, uint8_t channel)
{
    DMAMUX_Type * baseAddr = g_dmamuxBase[module];
    DMAMUX_HAL_SetPeriodTriggerCmd(baseAddr, channel,true);

    if (!DMAMUX_BRD_CHCFG_TRIG(baseAddr, channel))
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t dmamux_hal_test_disable_period_trigger(uint8_t module, uint8_t channel)
{
    DMAMUX_Type * baseAddr = g_dmamuxBase[module];
    DMAMUX_HAL_SetPeriodTriggerCmd(baseAddr, channel,false);

    if (DMAMUX_BRD_CHCFG_TRIG(baseAddr, channel))
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}
#endif

int32_t dmamux_hal_test_set_trigger_source(uint8_t module, uint8_t channel, uint8_t source)
{
    DMAMUX_Type * baseAddr = g_dmamuxBase[module];
    DMAMUX_HAL_SetTriggerSource(baseAddr, channel, source);

    if (DMAMUX_BRD_CHCFG_SOURCE(baseAddr, channel) != source)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

void init_hardware(void)
{
    hardware_init();
    dbg_uart_init();
}

int32_t main(void)
{
    int32_t status = 0, i, j, k;

    init_hardware();

    printf("\r\nDMAMUX hal driver unit test start!\r\n");

    /* Enable DMAMUX clock*/
    for (i = 0; i < DMAMUX_INSTANCE_COUNT; i++)
    {
        CLOCK_SYS_EnableDmamuxClock(i);
        
        for (j = 0; j < FSL_FEATURE_DMAMUX_MODULE_CHANNEL; j++)
        {
            status += dmamux_hal_test_enable_channel(i, j);
            status += dmamux_hal_test_disable_channel(i, j);
#if FSL_FEATURE_DMAMUX_HAS_TRIG
            status += dmamux_hal_test_enable_period_trigger(i, j);
            status += dmamux_hal_test_disable_period_trigger(i, j);
#endif
            for (k = 0; k < 64; k++)
            {
                dmamux_hal_test_set_trigger_source(i, j, k); 
            }
        }

        CLOCK_SYS_DisableDmamuxClock(i);
    }


    if (status == 0)
    {
        printf("\r\nDMAMUX hal driver Unit Test Succeed!\r\n");
    }
    else
    {
        printf("\r\nDMAMUX hal driver unit test Failed!\r\n");
    }
}
