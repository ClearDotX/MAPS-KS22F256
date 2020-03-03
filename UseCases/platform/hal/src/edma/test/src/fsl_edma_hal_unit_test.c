#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "fsl_debug_console.h"
#include "fsl_edma_hal.h"
#include "fsl_device_registers.h"
#include "fsl_clock_manager.h"
#include "board.h"
#include <assert.h>

#define ERROR_DEBUG() printf("EDMA HAL TEST ERROR: function[%s] line[%d]\r\n", __func__, __LINE__)

extern DMA_Type * const g_edmaBase[];
extern DMAMUX_Type * const g_dmamuxBase[];

//int32_t edma_hal_test_init(uint32_t instance, edma_config_t *init);

//int32_t edma_hal_test_cancel_transfer(uint32_t instance)

//int32_t edma_hal_test_error_cancel_transfer(uint32_t instance)

int32_t edma_hal_test_set_minor_loop_mapping(uint32_t instance, bool isEnabled)
{
    assert(instance < DMA_INSTANCE_COUNT);
    DMA_Type * baseAddr = g_edmaBase[instance];
    DMA_BWR_CR_EMLM(baseAddr, isEnabled);
    EDMA_HAL_SetMinorLoopMappingCmd(baseAddr, isEnabled);
    if ((bool)DMA_BRD_CR_EMLM(baseAddr) != isEnabled)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

#if (FSL_FEATURE_EDMA_CHANNEL_GROUP_COUNT > 0x1U)
int32_t edma_hal_test_set_group_priority(uint32_t instance, edma_group_priority_t groupPriority)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_SetGroupPriority(baseAddr, groupPriority);
    if (groupPriority == kEDMAGroup0PriorityLowGroup1PriorityHigh)
    {
        if ((DMA_BRD_CR_GRP0PRI(baseAddr) != 0x0U) || (DMA_BRD_CR_GRP1PRI(baseAddr) != 0x1U))
        {
            ERROR_DEBUG();
            return -1;
        }
    } 
    else
    {
        if ((DMA_BRD_CR_GRP0PRI(baseAddr) != 0x1U) || (DMA_BRD_CR_GRP1PRI(baseAddr) != 0x0U))
        {
            ERROR_DEBUG();
            return -1;
        }
    }

    return 0;
}

int32_t edma_hal_test_set_fixed_priority_group_arbitration(uint32_t instance)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_SetGroupArbitrationMode(baseAddr,kEDMAChnArbitrationFixedPriority);
    if (DMA_BRD_CR_ERGA(baseAddr) != 0x0U)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_set_roundrobin_group_arbitration(uint32_t instance)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_SetGroupArbitrationMode(baseAddr,kEDMAChnArbitrationRoundrobin);
    if (DMA_BRD_CR_ERGA(baseAddr) != 0x1U)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}
#endif

int32_t edma_hal_test_set_continuous_mode(uint32_t instance, bool isContinuous)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_SetContinuousLinkCmd(baseAddr, isContinuous);
    if ((bool)DMA_BRD_CR_CLM(baseAddr) != isContinuous)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_halt(uint32_t instance)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_SetHaltCmd(baseAddr,true);
    if ( DMA_BRD_CR_HALT(baseAddr) != 0x1U)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_clear_halt(uint32_t instance)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_SetHaltCmd(baseAddr, false);
    if ( DMA_BRD_CR_HALT(baseAddr) != 0x0U)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_set_halt_on_error(uint32_t instance, bool isHaltOnError)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_SetHaltOnErrorCmd(baseAddr, isHaltOnError);
    if ((bool)DMA_BRD_CR_HOE(baseAddr) != isHaltOnError)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_set_fixed_priority_channel_arbitration(uint32_t instance)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_SetChannelArbitrationMode(baseAddr,kEDMAChnArbitrationFixedPriority);
    if (DMA_BRD_CR_ERCA(baseAddr) != 0x0U)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_set_roundrobin_channel_arbitration(uint32_t instance)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_SetChannelArbitrationMode(baseAddr,kEDMAChnArbitrationRoundrobin);
    if (DMA_BRD_CR_ERCA(baseAddr) != 0x1U)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_set_debug_mode(uint32_t instance, bool isEnabled)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_SetDebugCmd(baseAddr, isEnabled);
    if ((bool)DMA_BRD_CR_EDBG(baseAddr) != isEnabled)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

//uint32_t edma_hal_test_get_error_status(uint32_t instance)

int32_t edma_hal_test_disable_all_enabled_error_interrupt(uint32_t instance)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_SetErrorIntCmd(baseAddr,false,kEDMAAllChannel);
    if (DMA_RD_EEI(baseAddr) != 0x0U)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_enable_all_channel_error_interrupt(uint32_t instance)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    uint32_t mask;
    EDMA_HAL_SetErrorIntCmd(baseAddr, true, kEDMAAllChannel);
#if (FSL_FEATURE_EDMA_MODULE_CHANNEL == 32)
    mask = 0xFFFFFFFF;
#else
    mask = (uint32_t)((1U << FSL_FEATURE_EDMA_MODULE_CHANNEL) -1U);
#endif
    if ((DMA_RD_EEI(baseAddr) & mask) != mask)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_disable_all_channel_dma_request(uint32_t instance)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_SetDmaRequestCmd(baseAddr,kEDMAAllChannel,false);
    if (DMA_RD_ERQ(baseAddr) != 0x0U)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_enable_all_channel_dma_request(uint32_t instance)
{
    uint32_t mask;
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_SetDmaRequestCmd(baseAddr,kEDMAAllChannel,true);
#if (FSL_FEATURE_EDMA_MODULE_CHANNEL == 32)
    mask = 0xFFFFFFFF;
#else
    mask = (uint32_t)((1U << FSL_FEATURE_EDMA_MODULE_CHANNEL) -1U);
#endif
    if ((DMA_RD_ERQ(baseAddr) & mask) != mask)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_clear_all_channel_done_status(uint32_t instance)
{
    uint32_t i;
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_ClearDoneStatusFlag(baseAddr, kEDMAAllChannel);
    for (i = 0; i < FSL_FEATURE_EDMA_MODULE_CHANNEL; i++)
    {
        if (DMA_BRD_CSR_DONE(baseAddr, i) != 0x0U)
        {
            ERROR_DEBUG();
            return -1;
        }
    
    }

    return 0;
}

//int32_t edma_hal_test_trigger_all_channel_start_bit(uint32_t instance)

int32_t edma_hal_test_clear_all_channel_error_status(uint32_t instance)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_ClearErrorIntStatusFlag(baseAddr, kEDMAAllChannel);
    edma_error_status_all_t status = EDMA_HAL_GetErrorStatus(baseAddr);
    if (status.orOfAllError!= 0x0U)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_clear_all_channel_interrupt_request(uint32_t instance)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_ClearIntStatusFlag(baseAddr, kEDMAAllChannel);
    if (DMA_RD_INT(baseAddr) != 0x0U)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

//uint32_t edma_hal_test_get_all_channel_interrupt_request_status(uint32_t instance)

//uint32_t edma_hal_test_get_all_channel_error_status(uint32_t instance)

//uint32_t edma_hal_test_get_all_channel_dma_request_status(uint32_t instance)

//bool edma_hal_test_check_dma_request_enable_status(uint32_t instance, uint32_t channel)

int32_t edma_hal_test_disable_error_interrupt(uint32_t instance, edma_channel_indicator_t channel)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_SetErrorIntCmd(baseAddr, false, channel);
    if ((DMA_RD_EEI(baseAddr) >> channel) & 1U != 0x0U)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_enable_error_interrupt(uint32_t instance, edma_channel_indicator_t channel)
{
    uint32_t temp;
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_SetErrorIntCmd(baseAddr, true,channel);
    if ((DMA_RD_EEI(baseAddr) & (0x1U << channel)) != (0x1U << channel))
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_disable_dma_request(uint32_t instance, edma_channel_indicator_t channel)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_SetDmaRequestCmd(baseAddr,channel,false);
    if (DMA_RD_ERQ(baseAddr) & (0x1U << channel) != 0x0U)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_enable_dma_request(uint32_t instance, edma_channel_indicator_t channel)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_SetDmaRequestCmd(baseAddr,channel,true);
    if ((DMA_RD_ERQ(baseAddr) & (0x1U << channel)) != (0x1U << channel))
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_clear_done_status(uint32_t instance, edma_channel_indicator_t channel)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_ClearDoneStatusFlag(baseAddr, channel);
    if (DMA_BRD_CSR_DONE(baseAddr, channel) != 0x0U)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

//int32_t edma_hal_test_trigger_start_bit(uint32_t instance, uint32_t channel)

int32_t edma_hal_test_clear_error_status(uint32_t instance, edma_channel_indicator_t channel)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_ClearErrorIntStatusFlag(baseAddr,channel);
    if ((EDMA_HAL_GetErrorIntStatusFlag(baseAddr)& (0x1U << channel)) != 0x0U)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_clear_interrupt_request(uint32_t instance, edma_channel_indicator_t channel)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_ClearIntStatusFlag(baseAddr, channel);
    if (EDMA_HAL_GetIntStatusFlag(baseAddr, channel) != 0x0U)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

#if (FSL_FEATURE_EDMA_ASYNCHRO_REQUEST_CHANNEL_COUNT > 0x0U)
int32_t edma_hal_test_set_asynchronous_request_in_stop_mode(
                    uint32_t instance, uint32_t channel, bool isEnabled)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_SetAsyncRequestInStopModeCmd(baseAddr,channel,isEnabled);

    assert(instance < DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_EDMA_MODULE_CHANNEL);
    if(isEnabled) 
    {
        if ((DMA_RD_EARS(baseAddr) & (0x1U << channel)) != (0x1U << channel))
        {
            ERROR_DEBUG();
            return -1;
        }
    }
    else
    {
        if ((DMA_RD_EARS(baseAddr) & (0x1U << channel)) != 0x0U)
        {
            ERROR_DEBUG();
            return -1;
        }
    }
    return 0;
}
#endif

int32_t edma_hal_test_set_channel_preemp_ability(
                uint32_t instance, uint32_t channel, bool isDisabled)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_SetChannelPreemptMode(baseAddr,channel,isDisabled, isDisabled);
    if ((bool)DMA_BRD_DCHPRIn_DPA(baseAddr, channel) != isDisabled)
    {
        ERROR_DEBUG();
        return -1;
    }
    if ((bool)DMA_BRD_DCHPRIn_ECP(baseAddr, channel) != isDisabled)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}


int32_t edma_hal_test_set_channel_priority(
                uint32_t instance, uint32_t channel, uint32_t priority)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_SetChannelPriority(baseAddr, (edma_channel_indicator_t)channel, 
        (edma_channel_priority_t)priority);
    if (DMA_BRD_DCHPRIn_CHPRI(baseAddr, channel) != priority)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_htcd_configure_source_address(
                uint32_t instance, uint32_t channel, uint32_t address)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_HTCDSetSrcAddr(baseAddr, channel,address);

    if (DMA_RD_SADDR(baseAddr, channel) != address)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_htcd_configure_source_offset(
                uint32_t instance, uint32_t channel, int16_t offset)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_HTCDSetSrcOffset(baseAddr,channel,offset);
    if (DMA_RD_SOFF(baseAddr, channel) != offset)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_htcd_configure_attribute(
                uint32_t instance, uint32_t channel, edma_modulo_t srcmodulo,
                edma_modulo_t destmodulo, edma_transfer_size_t srcsize,
                edma_transfer_size_t destsize)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_HTCDSetAttribute(baseAddr,channel,srcmodulo,destmodulo,
        srcsize,destsize);
    if (DMA_BRD_ATTR_SMOD(baseAddr, channel) != srcmodulo)
    {
        ERROR_DEBUG();
        return -1;
    }
    if (DMA_BRD_ATTR_SSIZE(baseAddr, channel) != srcsize)
    {
        ERROR_DEBUG();
        return -1;
    }
    if (DMA_BRD_ATTR_DMOD(baseAddr, channel) != destmodulo)
    {
        ERROR_DEBUG();
        return -1;
    }    
    if (DMA_BRD_ATTR_DSIZE(baseAddr, channel) != destsize)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}


int32_t edma_hal_test_htcd_configure_nbytes_minorloop_disabled(
                uint32_t instance, uint32_t channel, uint32_t nbytes)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_HTCDSetNbytes(baseAddr,channel,nbytes);
    if (EDMA_HAL_HTCDGetNbytes(baseAddr,channel)!= nbytes)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

//uint32_t edma_hal_test_htcd_get_nbytes_configuration(uint32_t instance, uint32_t channel);
//TODO
//int32_t edma_hal_test_htcd_configure_minorloop_offset(
//       uint32_t instance, uint32_t channel, edma_minorloop_offset_config_t config)

int32_t edma_hal_test_htcd_configure_source_last_adjustment(
                uint32_t instance, uint32_t channel, int32_t size)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_HTCDSetSrcLastAdjust(baseAddr, channel, size);
    if (DMA_RD_SLAST(baseAddr, channel) != size)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_htcd_configure_dest_address(
                uint32_t instance, uint32_t channel, uint32_t address)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_HTCDSetDestAddr(baseAddr, channel,address);
    if (DMA_RD_DADDR(baseAddr, channel) != address)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_htcd_configure_dest_offset(
                uint32_t instance, uint32_t channel, int16_t offset)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_HTCDSetDestOffset(baseAddr,channel,offset);
    if (DMA_RD_DOFF(baseAddr, channel) != offset)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_htcd_configure_dest_last_adjustment_or_scatter_address(
        uint32_t instance, uint32_t channel, uint32_t address)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_HTCDSetDestLastAdjust(baseAddr,channel,address);
    if (DMA_RD_DLAST_SGA(baseAddr, channel) != address)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_htcd_configure_bandwidth(
        uint32_t instance, uint32_t channel, edma_bandwidth_config_t bandwidth)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_HTCDSetBandwidth(baseAddr,channel,bandwidth);
    if (DMA_BRD_CSR_BWC(baseAddr, channel) != bandwidth)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_htcd_configure_majorlink(
        uint32_t instance, uint32_t channel, uint32_t majorchannel, bool isEnabled)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_HTCDSetChannelMajorLink(baseAddr,channel,majorchannel,isEnabled);
    if (DMA_BRD_CSR_MAJORLINKCH(baseAddr, channel)!= majorchannel)
    {
        ERROR_DEBUG();
        return -1;
    }
    if ((bool)DMA_BRD_CSR_MAJORELINK(baseAddr, channel) != isEnabled)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

//uint32_t edma_hal_test_htcd_get_majorlink_channel( 
//        uint32_t instance, uint32_t channel)


int32_t edma_hal_test_htcd_set_scatter_gather_process(
        uint32_t instance, uint32_t channel, bool isEnabled)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_HTCDSetScatterGatherCmd(baseAddr,channel,isEnabled);
    if (((bool)(DMA_RD_CSR_ESG(baseAddr,channel) != 0U)) != isEnabled)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

//bool edma_hal_test_htcd_is_gather_scatter_enabled(
//        uint32_t instance, uint32_t channel)

int32_t edma_hal_test_htcd_set_disable_dma_request_after_tcd_done(
        uint32_t instance, uint32_t channel, bool isDisabled)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_HTCDSetDisableDmaRequestAfterTCDDoneCmd(baseAddr,channel,isDisabled);
    if ((bool)DMA_BRD_CSR_DREQ(baseAddr, channel) != isDisabled)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}  

int32_t edma_hal_test_htcd_set_half_complete_interrupt(
        uint32_t instance, uint32_t channel, bool isEnabled)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_HTCDSetHalfCompleteIntCmd(baseAddr, channel, isEnabled);
    if ((bool)DMA_BRD_CSR_INTHALF(baseAddr, channel) != isEnabled)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_htcd_set_complete_interrupt(
        uint32_t instance, uint32_t channel, bool isEnabled)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_HTCDSetIntCmd(baseAddr,channel, isEnabled);
    if ((bool)DMA_BRD_CSR_INTMAJOR(baseAddr, channel) != isEnabled)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

//int32_t edma_hal_test_htcd_trigger_channel_start(
//        uint32_t instance, uint32_t channel)

//bool edma_hal_test_htcd_is_channel_active(
//        uint32_t instance, uint32_t channel)

//bool edma_hal_test_htcd_is_channel_done(
//        uint32_t instance, uint32_t channel)

int32_t edma_hal_test_htcd_set_minor_link(
        uint32_t instance, uint32_t channel, uint32_t minorchannel, bool isEnabled)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_HTCDSetChannelMinorLink(baseAddr,channel,minorchannel, isEnabled);
    if ((bool)DMA_BRD_BITER_ELINKYES_ELINK(baseAddr, channel) != isEnabled)
    {
        ERROR_DEBUG();
        return -1;
    }
    if (DMA_BRD_BITER_ELINKYES_LINKCH(baseAddr, channel) != minorchannel)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_htcd_configure_majorcount(
        uint32_t instance, uint32_t channel, uint32_t count)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_HTCDSetMajorCount(baseAddr,channel,count);
    EDMA_HAL_HTCDSetNbytes(baseAddr,channel,1);
    if ((EDMA_HAL_HTCDGetFinishedBytes(baseAddr, channel) != 0)
        && (EDMA_HAL_HTCDGetUnfinishedBytes(baseAddr,channel) != count))

    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

//uint32_t edma_hal_test_htcd_get_current_major_count(uint32_t instance, uint32_t channel);
//uint32_t edma_hal_test_htcd_get_begin_major_count(uint32_t instance, uint32_t channel);
//uint32_t edma_hal_test_htcd_get_unfinished_bytes(uint32_t instance, uint32_t channel);
//uint32_t edma_hal_test_htcd_get_finished_bytes(uint32_t instance, uint32_t channel);

int32_t edma_hal_test_stcd_configure_source_address(
        edma_software_tcd_t *stcd, uint32_t address)
{
    EDMA_HAL_STCDSetSrcAddr(stcd, address);
    if (stcd->SADDR != address)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_stcd_configure_source_offset(
                edma_software_tcd_t *stcd, uint32_t offset)
{
    EDMA_HAL_STCDSetSrcOffset(stcd, offset);
    if (stcd->SOFF != offset)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_stcd_configure_attribute(
                edma_software_tcd_t *stcd, edma_modulo_t srcmodulo, edma_modulo_t destmodulo,
                edma_transfer_size_t srcsize, edma_transfer_size_t destsize)
{
    EDMA_HAL_STCDSetAttribute(stcd,srcmodulo,destmodulo,
        srcsize,destsize);
    if ((stcd->ATTR & DMA_ATTR_SMOD_MASK) != (srcmodulo << DMA_ATTR_SMOD_SHIFT))
    {
        ERROR_DEBUG();
        return -1;
    }
    if ((stcd->ATTR & DMA_ATTR_SSIZE_MASK) != (srcsize << DMA_ATTR_SSIZE_SHIFT))
    {
        ERROR_DEBUG();
        return -1;
    }
    if ((stcd->ATTR & DMA_ATTR_DMOD_MASK) != (destmodulo << DMA_ATTR_DMOD_SHIFT))
    {
        ERROR_DEBUG();
        return -1;
    }
    if ((stcd->ATTR & DMA_ATTR_DSIZE_MASK) != (destsize << DMA_ATTR_DSIZE_SHIFT))
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}


int32_t edma_hal_test_stcd_configure_nbytes(
                uint32_t instance, edma_software_tcd_t *stcd, uint32_t nbytes)
{
    DMA_Type * baseAddr = g_edmaBase[instance];
    EDMA_HAL_STCDSetNbytes(baseAddr,stcd,nbytes);
    if ((stcd->NBYTES & DMA_NBYTES_MLNO_NBYTES_MASK) != (nbytes << DMA_NBYTES_MLNO_NBYTES_SHIFT))
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

//int32_t edma_hal_test_stcd_configure_minorloop_offset(
//        edma_software_tcd_t *stcd, edma_minorloop_offset_config_t *config)

int32_t edma_hal_test_stcd_configure_source_last_adjustment(
                edma_software_tcd_t *stcd, int32_t size)
{
    EDMA_HAL_STCDSetSrcLastAdjust(stcd, size);
    if ((stcd->SLAST & DMA_SLAST_SLAST_MASK) != (size << DMA_SLAST_SLAST_SHIFT))
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_stcd_configure_dest_address(
                edma_software_tcd_t *stcd, uint32_t address)
{
    stcd->DADDR = address;
    if (stcd->DADDR != address)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_stcd_configure_dest_offset(
                edma_software_tcd_t *stcd, uint32_t offset)
{
    EDMA_HAL_STCDSetDestOffset(stcd,offset);
    if (stcd->DOFF != offset)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_stcd_configure_dest_last_adjustment_or_scatter_address(
        edma_software_tcd_t *stcd, uint32_t address)
{
    EDMA_HAL_STCDSetDestLastAdjust(stcd, address);
    if (stcd->DLAST_SGA != address)
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_stcd_configure_bandwidth(
        edma_software_tcd_t *stcd, edma_bandwidth_config_t bandwidth)
{
    EDMA_HAL_STCDSetBandwidth(stcd, bandwidth);
    if ((stcd->CSR & DMA_CSR_BWC_MASK) != (bandwidth << DMA_CSR_BWC_SHIFT))
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_stcd_set_majorlink(edma_software_tcd_t *stcd, uint32_t majorchannel, bool isEnabled)
{
    EDMA_HAL_STCDSetChannelMajorLink(stcd, majorchannel, isEnabled);
    if ((stcd->CSR & DMA_CSR_MAJORELINK_MASK) != (isEnabled << DMA_CSR_MAJORELINK_SHIFT))
    {
        ERROR_DEBUG();
        return -1;
    }
    if ((stcd->CSR & DMA_CSR_MAJORLINKCH_MASK) != (majorchannel << DMA_CSR_MAJORLINKCH_SHIFT))
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_stcd_set_scatter_gather_process(
        edma_software_tcd_t *stcd, bool isEnabled)
{
    EDMA_HAL_STCDSetScatterGatherCmd(stcd, isEnabled);
    if ((stcd->CSR & DMA_CSR_ESG_MASK) != (isEnabled << DMA_CSR_ESG_SHIFT))
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_stcd_set_disable_dma_request_after_tcd_done(
        edma_software_tcd_t *stcd, bool isDisabled)
{
    EDMA_HAL_STCDSetDisableDmaRequestAfterTCDDoneCmd(stcd, isDisabled);
    if ((stcd->CSR & DMA_CSR_DREQ_MASK) != (isDisabled << DMA_CSR_DREQ_SHIFT))
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}  

int32_t edma_hal_test_stcd_set_half_complete_interrupt(
        edma_software_tcd_t *stcd, bool isEnabled)
{
    EDMA_HAL_STCDSetHalfCompleteIntCmd(stcd, isEnabled);
    if ((stcd->CSR & DMA_CSR_INTHALF_MASK) != (isEnabled << DMA_CSR_INTHALF_SHIFT))
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_stcd_set_complete_interrupt(
        edma_software_tcd_t *stcd, bool isEnabled)
{
    EDMA_HAL_STCDSetIntCmd(stcd , isEnabled);
    if ((stcd->CSR & DMA_CSR_INTMAJOR_MASK) != (isEnabled << DMA_CSR_INTMAJOR_SHIFT))
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

//int32_t edma_hal_test_stcd_trigger_channel_start(
//        edma_software_tcd_t *stcd)

int32_t edma_hal_test_stcd_set_minor_link(
        edma_software_tcd_t *stcd, uint32_t minorchannel, bool isEnabled)
{
    EDMA_HAL_STCDSetChannelMinorLink(stcd,minorchannel,isEnabled);
    if ((stcd->BITER & DMA_BITER_ELINKYES_ELINK_MASK) != (isEnabled << DMA_BITER_ELINKYES_ELINK_SHIFT))
    {
        ERROR_DEBUG();
        return -1;
    }
    if ((stcd->CITER & DMA_CITER_ELINKYES_ELINK_MASK) != (isEnabled << DMA_CITER_ELINKYES_ELINK_SHIFT))
    {
        ERROR_DEBUG();
        return -1;
    }
    if ((stcd->BITER & DMA_BITER_ELINKYES_LINKCH_MASK) != (minorchannel << DMA_BITER_ELINKYES_LINKCH_SHIFT))
    {
        ERROR_DEBUG();
        return -1;
    }
    if ((stcd->CITER & DMA_CITER_ELINKYES_LINKCH_MASK) != (minorchannel << DMA_CITER_ELINKYES_LINKCH_SHIFT))
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}



int32_t edma_hal_test_stcd_configure_majorcount_minorlink_disabled(
        edma_software_tcd_t *stcd, uint32_t count)
{
    EDMA_HAL_STCDSetMajorCount(stcd,count);
    if ((stcd->BITER & DMA_BITER_ELINKNO_BITER_MASK) != (count << DMA_BITER_ELINKNO_BITER_SHIFT))
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_stcd_configure_current_majorcount_minorlink_disabled(
        edma_software_tcd_t *stcd, uint32_t count)
{
    EDMA_HAL_STCDSetMajorCount(stcd,count);
    if ((stcd->CITER & DMA_CITER_ELINKNO_CITER_MASK) != (count << DMA_CITER_ELINKNO_CITER_SHIFT))
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_stcd_configure_majorcount_minorlink_enabled(
        edma_software_tcd_t *stcd, uint32_t count)
{
    EDMA_HAL_STCDSetMajorCount(stcd,count);
    if ((stcd->BITER & DMA_BITER_ELINKYES_BITER_MASK) != (count << DMA_BITER_ELINKYES_BITER_SHIFT))
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}

int32_t edma_hal_test_stcd_configure_current_majorcount_minorlink_enabled(
        edma_software_tcd_t *stcd, uint32_t count)
{
    EDMA_HAL_STCDSetMajorCount(stcd,count);
    if ((stcd->CITER & DMA_CITER_ELINKYES_CITER_MASK) != (count << DMA_CITER_ELINKYES_CITER_SHIFT))
    {
        ERROR_DEBUG();
        return -1;
    }

    return 0;
}


int32_t main(void)
{
    hardware_init();
    dbg_uart_init();
    int32_t status = 0;
    uint32_t i, j, k; 
    edma_software_tcd_t *stcd, tcd; 

    stcd = &tcd;

    printf("\r\nEDMA hal driver unit test start!\r\n");

    for (i  = 0; i < DMA_INSTANCE_COUNT; i++)
    {
        CLOCK_SYS_EnableDmaClock(i);
        status += edma_hal_test_set_minor_loop_mapping(i, true);
        status += edma_hal_test_set_minor_loop_mapping(i, false);
#if (FSL_FEATURE_EDMA_CHANNEL_GROUP_COUNT > 0x1U)
        status += edma_hal_test_set_group_priority(i, kEDMAGroup0PriorityLowGroup1PriorityHigh);
        status += edma_hal_test_set_group_priority(i, kEDMAGroup0PriorityHighGroup1PriorityLow);
        status += edma_hal_test_set_fixed_priority_group_arbitration(i);
        status += edma_hal_test_set_roundrobin_group_arbitration(i);
#endif
        status += edma_hal_test_set_continuous_mode(i, true);
        status += edma_hal_test_set_continuous_mode(i, false);
        status += edma_hal_test_halt(i);
        status += edma_hal_test_clear_halt(i);
        status += edma_hal_test_set_halt_on_error(i, true);      
        status += edma_hal_test_set_halt_on_error(i, false);      
        status += edma_hal_test_set_fixed_priority_channel_arbitration(i);
        status += edma_hal_test_set_roundrobin_channel_arbitration(i);
        status += edma_hal_test_set_debug_mode(i, true);
        status += edma_hal_test_set_debug_mode(i, false);
        status += edma_hal_test_enable_all_channel_error_interrupt(i);
        status += edma_hal_test_disable_all_enabled_error_interrupt(i);
        status += edma_hal_test_disable_all_channel_dma_request(i);
        status += edma_hal_test_enable_all_channel_dma_request(i);
        status += edma_hal_test_disable_all_channel_dma_request(i);
        status += edma_hal_test_clear_all_channel_done_status(i);
        status += edma_hal_test_clear_all_channel_error_status(i);
        status += edma_hal_test_clear_all_channel_interrupt_request(i);

        for (j = 0; j < FSL_FEATURE_EDMA_MODULE_CHANNEL; j++)
        {
            status += edma_hal_test_enable_error_interrupt(i, (edma_channel_indicator_t)j);
            status += edma_hal_test_disable_error_interrupt(i, (edma_channel_indicator_t)j);
            status += edma_hal_test_enable_dma_request(i, (edma_channel_indicator_t)j);
            status += edma_hal_test_disable_dma_request(i, (edma_channel_indicator_t)j);
            status += edma_hal_test_clear_done_status(i, (edma_channel_indicator_t)j);
            status += edma_hal_test_clear_error_status(i, (edma_channel_indicator_t)j);
            status += edma_hal_test_clear_interrupt_request(i, (edma_channel_indicator_t)j);
#if (FSL_FEATURE_EDMA_ASYNCHRO_REQUEST_CHANNEL_COUNT > 0x0U)
            //TODO: async register is not albe to be read. Pending for reason.
            //status += edma_hal_test_set_asynchronous_request_in_stop_mode(i, j, true);
            //status += edma_hal_test_set_asynchronous_request_in_stop_mode(i, j, false);
#endif
            status += edma_hal_test_set_channel_preemp_ability(i, j, true);
            status += edma_hal_test_set_channel_preemp_ability(i, j, false);
            uint8_t priority_bit = FSL_FEATURE_EDMA_MODULE_CHANNEL > 16 ? 16: FSL_FEATURE_EDMA_MODULE_CHANNEL;
            for (k = 0; k < priority_bit; k++)
            {
                status += edma_hal_test_set_channel_priority(i, j, k);
            }
            status += edma_hal_test_htcd_configure_source_address(i, j, 0xFFFFU); 
            status += edma_hal_test_htcd_configure_dest_address(i, j, 0xFFFFU); 
            status += edma_hal_test_htcd_configure_source_offset(i, j, 0xFFU);
            status += edma_hal_test_htcd_configure_dest_offset(i, j, 0xFFU);
            for (k = 0; k < 32; k++)
            {
                status += edma_hal_test_htcd_configure_attribute(i, (edma_channel_indicator_t)j, 
                    (edma_modulo_t)k, (edma_modulo_t)k, kEDMATransferSize_1Bytes,kEDMATransferSize_1Bytes);
                status += edma_hal_test_htcd_configure_attribute(i, (edma_channel_indicator_t)j, 
                    (edma_modulo_t)k, (edma_modulo_t)k, kEDMATransferSize_2Bytes,kEDMATransferSize_2Bytes);
                status += edma_hal_test_htcd_configure_attribute(i, (edma_channel_indicator_t)j, 
                    (edma_modulo_t)k, (edma_modulo_t)k, kEDMATransferSize_4Bytes,kEDMATransferSize_4Bytes); 
                status += edma_hal_test_htcd_configure_attribute(i, (edma_channel_indicator_t)j, 
                    (edma_modulo_t)k, (edma_modulo_t)k, kEDMATransferSize_16Bytes,kEDMATransferSize_16Bytes);
                status += edma_hal_test_htcd_configure_attribute(i, (edma_channel_indicator_t)j, 
                    (edma_modulo_t)k, (edma_modulo_t)k, kEDMATransferSize_32Bytes,kEDMATransferSize_32Bytes);
            }
            status += edma_hal_test_htcd_configure_nbytes_minorloop_disabled(i, j, 0xFF);
            status += edma_hal_test_htcd_configure_source_last_adjustment(i, j, 0xFF);
            status += edma_hal_test_htcd_configure_dest_last_adjustment_or_scatter_address(i, j, 0xFF);
            status += edma_hal_test_htcd_configure_bandwidth(i, j, kEDMABandwidthStallNone);
            status += edma_hal_test_htcd_configure_bandwidth(i, j, kEDMABandwidthStall4Cycle);
            status += edma_hal_test_htcd_configure_bandwidth(i, j, kEDMABandwidthStall8Cycle);
            status += edma_hal_test_htcd_configure_majorlink(i, j, 0x3,true);
            status += edma_hal_test_htcd_configure_majorlink(i, j, 0x3,false);
            status += edma_hal_test_htcd_set_scatter_gather_process(i, j, true);
            status += edma_hal_test_htcd_set_scatter_gather_process(i, j, false);
            status += edma_hal_test_htcd_set_disable_dma_request_after_tcd_done(i, j, true);
            status += edma_hal_test_htcd_set_disable_dma_request_after_tcd_done(i, j, false); 
            status += edma_hal_test_htcd_set_half_complete_interrupt(i, j, true);
            status += edma_hal_test_htcd_set_half_complete_interrupt(i, j, false);
            status += edma_hal_test_htcd_set_complete_interrupt(i, j, true); 
            status += edma_hal_test_htcd_set_complete_interrupt(i, j, false); 
            status += edma_hal_test_htcd_set_minor_link(i, j, 0x3,true);
            status += edma_hal_test_htcd_set_minor_link(i, j, 0x3,false);
            status += edma_hal_test_htcd_configure_majorcount(i, j, 0xFU);
        }
    }
    /* Software TCD configuration.*/
    status += edma_hal_test_stcd_configure_source_address(stcd, 0xFFFFU);
    status += edma_hal_test_stcd_configure_dest_address(stcd, 0xFFFFU);
    status += edma_hal_test_stcd_configure_source_offset(stcd, 0xFF);
    status += edma_hal_test_stcd_configure_dest_offset(stcd, 0xFF);
    for ( k = 0; k < 32; k++)
    {
        status += edma_hal_test_stcd_configure_attribute(stcd, (edma_modulo_t)k, (edma_modulo_t)k, 
            kEDMATransferSize_1Bytes,kEDMATransferSize_1Bytes);
        status += edma_hal_test_stcd_configure_attribute(stcd, (edma_modulo_t)k, (edma_modulo_t)k, 
            kEDMATransferSize_2Bytes,kEDMATransferSize_2Bytes);
        status += edma_hal_test_stcd_configure_attribute(stcd, (edma_modulo_t)k, (edma_modulo_t)k, 
            kEDMATransferSize_4Bytes,kEDMATransferSize_4Bytes); 
        status += edma_hal_test_stcd_configure_attribute(stcd, (edma_modulo_t)k, (edma_modulo_t)k, 
            kEDMATransferSize_16Bytes,kEDMATransferSize_16Bytes);
        status += edma_hal_test_stcd_configure_attribute(stcd, (edma_modulo_t)k, (edma_modulo_t)k, 
            kEDMATransferSize_32Bytes,kEDMATransferSize_32Bytes);
    }
    status += edma_hal_test_stcd_configure_nbytes(0, stcd, 0xFFU);
    status += edma_hal_test_stcd_configure_source_last_adjustment(stcd, 0xFFU);
    status += edma_hal_test_stcd_configure_source_offset(stcd, 0xFF);
    status += edma_hal_test_stcd_configure_dest_offset(stcd, 0xFF);
    status += edma_hal_test_stcd_configure_dest_last_adjustment_or_scatter_address(stcd, 0xFF);
    status += edma_hal_test_stcd_configure_bandwidth(stcd, kEDMABandwidthStallNone);
    status += edma_hal_test_stcd_configure_bandwidth(stcd, kEDMABandwidthStall4Cycle);
    status += edma_hal_test_stcd_configure_bandwidth(stcd, kEDMABandwidthStall8Cycle);
    status += edma_hal_test_stcd_set_majorlink(stcd, 0x3,true);
    status += edma_hal_test_stcd_set_majorlink(stcd, 0x3,false);
    status += edma_hal_test_stcd_set_scatter_gather_process(stcd, true);
    status += edma_hal_test_stcd_set_scatter_gather_process(stcd, false);
    status += edma_hal_test_stcd_set_disable_dma_request_after_tcd_done(stcd, true);
    status += edma_hal_test_stcd_set_disable_dma_request_after_tcd_done(stcd, false);
    status += edma_hal_test_stcd_set_half_complete_interrupt(stcd, true);
    status += edma_hal_test_stcd_set_half_complete_interrupt(stcd, false);
    status += edma_hal_test_stcd_set_complete_interrupt(stcd, true);
    status += edma_hal_test_stcd_set_complete_interrupt(stcd, false);
    status += edma_hal_test_stcd_set_minor_link(stcd, 0x3u,true);
    status += edma_hal_test_stcd_set_minor_link(stcd, 0x3u,false);
    status += edma_hal_test_stcd_configure_majorcount_minorlink_disabled(stcd, 0xFFU);
    status += edma_hal_test_stcd_configure_current_majorcount_minorlink_disabled(stcd, 0xFFU);
    status += edma_hal_test_stcd_configure_current_majorcount_minorlink_enabled(stcd, 0xFFU);
    status += edma_hal_test_stcd_configure_majorcount_minorlink_disabled(stcd, 0xFFU);
    status += edma_hal_test_stcd_configure_majorcount_minorlink_enabled(stcd, 0xFFU);

    if (status == 0)
    {
        printf("EDMA hal driver unit test Finished successfully!\r\n");
    }
    else
    {
        printf("EDMA hal driver unit test Failed!\r\n");
    }
    for( i = 0; i < DMA_INSTANCE_COUNT; i++)
    {
        CLOCK_SYS_DisableDmaClock(i);
    }
}

