#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "board.h"
#include "fsl_dma_hal.h"
#include "fsl_device_registers.h"
#include "fsl_clock_manager.h"
#include <assert.h>

#define ERROR_DEBUG() printf("DMA HAL UNIT TEST ERROR: function[%s] line[%d]\r\n", __func__, __LINE__)
extern DMA_Type * const g_dmaBase[DMA_INSTANCE_COUNT];
extern DMAMUX_Type * const g_dmamuxBase[DMAMUX_INSTANCE_COUNT];

int32_t dma_hal_test_configure_source_address(
        uint32_t instance, uint32_t channel, uint32_t address)
{
    DMA_Type * baseAddr = g_dmaBase[instance];
    DMA_HAL_SetSourceAddr(baseAddr, channel, address);
    if (DMA_RD_SAR(baseAddr, channel) != address)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t dma_hal_test_configure_dest_address(
        uint32_t instance, uint32_t channel, uint32_t address)
{
    DMA_Type * baseAddr = g_dmaBase[instance];
    DMA_HAL_SetDestAddr(baseAddr, channel, address);
    if (DMA_RD_DAR(baseAddr, channel) != address)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t dma_hal_test_configure_count(
                    uint32_t instance, uint32_t channel, uint32_t count)
{
    DMA_Type * baseAddr = g_dmaBase[instance];
    DMA_HAL_SetTransferCount(baseAddr, channel, count);

    if (DMA_HAL_GetUnfinishedByte(baseAddr, channel) != count)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t dma_hal_test_enable_interrupt(uint32_t instance, uint8_t channel)
{
    DMA_Type * baseAddr = g_dmaBase[instance];
    DMA_HAL_SetIntCmd(baseAddr, channel, true);
    if (!DMA_BRD_DCR_EINT(baseAddr, channel))
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t dma_hal_test_disable_interrupt(uint32_t instance, uint8_t channel)
{
    DMA_Type * baseAddr = g_dmaBase[instance];
    DMA_HAL_SetIntCmd(baseAddr, channel, false);
    if (DMA_BRD_DCR_EINT(baseAddr, channel))
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t dma_hal_test_set_cycle_steal(
        uint32_t instance, uint8_t channel, bool isCycleSteal)
{
    DMA_Type * baseAddr = g_dmaBase[instance];
    DMA_HAL_SetCycleStealCmd(baseAddr, channel, isCycleSteal);
    if (DMA_BRD_DCR_CS(baseAddr, channel) != isCycleSteal)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t dma_hal_test_set_autoalign_ability(
        uint32_t instance, uint8_t channel, bool isEnabled)
{
    DMA_Type * baseAddr = g_dmaBase[instance];
    DMA_HAL_SetAutoAlignCmd(baseAddr, channel, isEnabled);
    if (DMA_BRD_DCR_AA(baseAddr, channel) != isEnabled)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t dma_hal_test_set_async_dma_request_ability(
        uint32_t instance, uint8_t channel, bool isEnabled)
{
    DMA_Type * baseAddr = g_dmaBase[instance];
    DMA_HAL_SetAsyncDmaRequestCmd(baseAddr, channel, isEnabled);
    if (DMA_BRD_DCR_EADREQ(baseAddr, channel) != isEnabled)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t dma_hal_test_set_source_increment(
        uint32_t instance, uint32_t channel, bool isEnabled)
{
    DMA_Type * baseAddr = g_dmaBase[instance];
    DMA_HAL_SetSourceIncrementCmd(baseAddr, channel, isEnabled);
    if (DMA_BRD_DCR_SINC(baseAddr, channel) != isEnabled)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t dma_hal_test_set_dest_increment(
        uint32_t instance, uint32_t channel, bool isEnabled)
{
    DMA_Type * baseAddr = g_dmaBase[instance];
    DMA_HAL_SetDestIncrementCmd(baseAddr, channel, isEnabled);
    if (DMA_BRD_DCR_DINC(baseAddr, channel) != isEnabled)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t dma_hal_test_configure_source_transfersize(
        uint32_t instance, uint32_t channel, dma_transfer_size_t transfersize)
{
    DMA_Type * baseAddr = g_dmaBase[instance];
    DMA_HAL_SetSourceTransferSize(baseAddr, channel, transfersize);
    if (DMA_BRD_DCR_SSIZE(baseAddr, channel) != (uint32_t)transfersize)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t dma_hal_test_configure_dest_transfersize(
        uint32_t instance, uint32_t channel, dma_transfer_size_t transfersize)
{
    DMA_Type * baseAddr = g_dmaBase[instance];
    DMA_HAL_SetDestTransferSize(baseAddr, channel, transfersize);
    if (DMA_BRD_DCR_DSIZE(baseAddr, channel) != (uint32_t)transfersize)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t dma_hal_test_configure_source_modulo(
        uint32_t instance, uint32_t channel, dma_modulo_t modulo)
{
    DMA_Type * baseAddr = g_dmaBase[instance];
    DMA_HAL_SetSourceModulo(baseAddr, channel, modulo);
    if (DMA_BRD_DCR_SMOD(baseAddr, channel) != (uint32_t)modulo)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t dma_hal_test_configure_dest_modulo(
        uint32_t instance, uint32_t channel, dma_modulo_t modulo)
{
    DMA_Type * baseAddr = g_dmaBase[instance];
    DMA_HAL_SetDestModulo(baseAddr, channel, modulo);
    if (DMA_BRD_DCR_DMOD(baseAddr, channel) != (uint32_t)modulo)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t dma_hal_test_set_dma_request(
        uint32_t instance, uint32_t channel, bool isEnabled)
{
    DMA_Type * baseAddr = g_dmaBase[instance];
    DMA_HAL_SetDmaRequestCmd(baseAddr, channel, isEnabled);
    if (DMA_BRD_DCR_ERQ(baseAddr, channel) != isEnabled)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t dma_hal_test_set_disable_dma_request_after_done(
        uint32_t instance, uint32_t channel, bool isDisabled)
{
    DMA_Type * baseAddr = g_dmaBase[instance];
    DMA_HAL_SetDisableRequestAfterDoneCmd(baseAddr, channel, isDisabled);
    if (DMA_BRD_DCR_D_REQ(baseAddr, channel) != isDisabled)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

//int32_t dma_hal_test_clear_status(uint32_t instance, uint8_t channel)

//dma_error_status_t dma_hal_test_get_status(
//        uint32_t instance, uint8_t channel)

void init_hardware(void)
{
    hardware_init();
    dbg_uart_init();
}

int32_t main(void)
{
    int32_t status = 0;
    uint32_t i, j; 
    init_hardware();


    printf("\r\nDMA hal driver unit test start!\r\n");

    for (i = 0; i < DMA_INSTANCE_COUNT; i++)
    {
        CLOCK_SYS_EnableDmaClock(i);
        for (j = 0; j < FSL_FEATURE_DMA_DMAMUX_CHANNELS; j++)
        {
            status += dma_hal_test_configure_source_address(i, j, 0xFFFFU); 
            status += dma_hal_test_configure_dest_address(i, j, 0xFFFFU);
            status += dma_hal_test_configure_count(i, j, 0x10U);
            status += dma_hal_test_enable_interrupt(i, j);
            status += dma_hal_test_disable_interrupt(i, j);
            status += dma_hal_test_set_cycle_steal(i, j, true);
            status += dma_hal_test_set_cycle_steal(i, j, false);
            status += dma_hal_test_set_async_dma_request_ability(i, j, true); 
            status += dma_hal_test_set_async_dma_request_ability(i, j, false);
            status += dma_hal_test_set_source_increment(i, j, true);
            status += dma_hal_test_set_source_increment(i, j, false);
            status += dma_hal_test_set_dest_increment(i, j, true);
            status += dma_hal_test_set_dest_increment(i, j, false);
            status += dma_hal_test_configure_source_transfersize(i, j, kDmaTransfersize8bits);
            status += dma_hal_test_configure_source_transfersize(i, j, kDmaTransfersize16bits);
            status += dma_hal_test_configure_source_transfersize(i, j, kDmaTransfersize32bits);
            //status += dma_hal_configure_source_modulo(i, j, ); 
            //status += dma_hal_configure_dest_modulo(i, j, ); 
            status += dma_hal_test_set_dma_request(i, j, true);
            status += dma_hal_test_set_dma_request(i, j, false);
            status += dma_hal_test_set_disable_dma_request_after_done(i, j, true);
            status += dma_hal_test_set_disable_dma_request_after_done(i, j, false);
        }
        CLOCK_SYS_DisableDmaClock(i);
    }


    if (status == 0)
    {
        printf("\r\nDMA hal driver Unit Test Succeed!\r\n");
    }
    else
    {
        printf("\r\nDMA hal driver unit test Failed!\r\n");
    }
}

