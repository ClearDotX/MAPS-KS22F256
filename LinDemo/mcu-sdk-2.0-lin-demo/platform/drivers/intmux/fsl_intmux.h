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

#ifndef _FSL_INTMUX_H_
#define _FSL_INTMUX_H_

#include "fsl_common.h"

/*!
 * @addtogroup intmux_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* clang-format off */ 
#define FSL_INTMUX_DRIVER_VERSION   (MAKE_VERSION(2, 0, 0))    /*!< Version 2.0.0. */
/* clang-format on */

/*! @brief INTMUX channel logic mode */
typedef enum _intmux_channel_logic_mode
{
    kINTMUX_ChannelLogicOR = 0x0U, /*!< Logic OR all enabled interrupt inputs */
    kINTMUX_ChannelLogicAND,       /*!< Logic AND all enabled interrupt inputs */
} intmux_channel_logic_mode_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name INTMUX Initialization and De-initialization
 * @{
 */

/*!
 * @brief Initializes INTMUX module.
 *
 * This function initialize the INTMUX module, enable the INTMUX clock.
 *
 * @param base INTMUX peripheral base address.
 *
 */
void INTMUX_Init(INTMUX_Type *base);

/*!
 * @brief Deinitializes an INTMUX instance for operation.
 *
 * This function deinitializes the INTMUX module, disable the INTMUX clock.
 *
 * @param base INTMUX peripheral base address.
 */
void INTMUX_Deinit(INTMUX_Type *base);

/* @} */
/*!
 * @name INTMUX Operation
 * @{
 */

/*!
 * @brief Reset INTMUX channel.
 *
 * Set all register value in one channel to reset value.
 *
 * @param base INTMUX peripheral base address.
 * @param channel The INTMUX channel number.
 */
static inline void INTMUX_ResetChannel(INTMUX_Type *base, uint32_t channel)
{
    assert(channel < FSL_FEATURE_INTMUX_CHANNEL_COUNT);

    base->CHANNEL[channel].CHn_CSR |= INTMUX_CHn_CSR_RST_MASK;
}

/*!
 * @brief Set INTMUX channel configuration.
 *
 * @param base INTMUX peripheral base address.
 * @param channel The INTMUX channel number.
 * @param source The INTMUX channel interrupt source. If need to set INTMUX source IRQ[n],
                 the Bit[n] of source need to be set.
 * @param logic The intmux channel logic mode:
 *              kINTMUX_ChannelLogicOR means any source pending will trigger interrupt.
 *              kINTMUX_ChannelLogicAND means all source pending will trigger interrupt.
 */
void INTMUX_SetChannelConfig(INTMUX_Type *base, uint32_t channel, uint32_t source, intmux_channel_logic_mode_t logic);

/* @} */
/*!
 * @name INTMUX Status Operation
 * @{
 */

/*!
 * @brief Get INTMUX pending interrupt source for specific channel.
 *
 * @param base INTMUX peripheral base address.
 * @param channel The INTMUX channel number.
 * @return The mask of pending interrupt bits. Bit[n] set means intmux source n is pending.
 */
static inline uint32_t INTMUX_GetChannelPendingSource(INTMUX_Type *base, uint32_t channel)
{
    assert(channel < FSL_FEATURE_INTMUX_CHANNEL_COUNT);

    return base->CHANNEL[channel].CHn_IPR_31_0;
}

/* @} */

#if defined(__cplusplus)
}
#endif

/* @} */

#endif /* _FSL_INTMUX_H_ */
