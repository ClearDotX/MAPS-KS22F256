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

#ifndef _FSL_MCGLITE_H_
#define _FSL_MCGLITE_H_

#include "fsl_osc.h"
#include "fsl_common.h"

#if (defined(OSC) && !(defined(OSC0)))
#define OSC0 OSC
#endif

/*! @addtogroup mcglite*/
/*! @{*/

/*******************************************************************************
* Definitions
******************************************************************************/
#define FSL_MCGLITE_DRIVER_VERSION (MAKE_VERSION(2, 0, 0)) /*!< Version 2.0.0. */

/* XTAL0 clock frequency. */
extern uint32_t g_xtal0Freq;

/*! @brief OSC work mode. */
typedef enum _osc_mode
{
    kOSC_ModeExt         = 0U,                   /*!< Use external clock.   */
    kOSC_ModeOscLowPower = MCG_C2_EREFS0_MASK,   /*!< Oscillator low power. */
#if ((defined(FSL_FEATURE_MCGLITE_HAS_HGO0) && FSL_FEATURE_MCGLITE_HAS_HGO0) && \
     (defined(FSL_FEATURE_MCGLITE_HAS_RANGE0) && FSL_FEATURE_MCGLITE_HAS_RANGE0))
    kOSC_ModeOscHighGain = MCG_C2_EREFS0_MASK |
                           MCG_C2_HGO0_MASK,     /*!< Oscillator high gain. */
#endif
} osc_mode_t;

/*!
 * @brief OSC Initialization Configuration Structure
 *
 * Defines the configuration data structure to initialize the OSC.
 * When porting to a new board, please set the following members
 * according to board setting:
 * 1. freq: The external frequency.
 * 2. workMode: The OSC module mode.
 */
typedef struct _osc_config
{
    uint32_t       freq;        /*!< External clock frequency.    */
    uint8_t        capLoad;     /*!< Capacitor load setting.      */
    osc_mode_t     workMode;    /*!< OSC work mode setting.       */
    oscer_config_t oscerConfig; /*!< Configuration for OSCERCLK.  */
} osc_config_t;

/*! @brief MCG_Lite clock source selection. */
typedef enum _mcglite_clkout_src
{
    kMCGLITE_ClkSrcHirc, /*!< MCGOUTCLK source is HIRC */
    kMCGLITE_ClkSrcLirc, /*!< MCGOUTCLK source is LIRC */
    kMCGLITE_ClkSrcExt,  /*!< MCGOUTCLK source is external clock source */
    kMCGLITE_ClkSrcReserved
} mcglite_clkout_src_t;

/*! @brief MCG_Lite LIRC select. */
typedef enum _mcglite_lirc_mode
{
    kMCGLITE_Lirc2M, /*!< slow internal reference(LIRC) 2MHz clock selected */
    kMCGLITE_Lirc8M, /*!< slow internal reference(LIRC) 8MHz clock selected */
} mcglite_lirc_mode_t;

/*! @brief MCG_Lite divider factor selection for clock source*/
typedef enum _mcglite_lirc_div
{
    kMCGLITE_LircDivBy1 = 0U, /*!< divider is 1    */
    kMCGLITE_LircDivBy2,      /*!< divider is 2    */
    kMCGLITE_LircDivBy4,      /*!< divider is 4    */
    kMCGLITE_LircDivBy8,      /*!< divider is 8    */
    kMCGLITE_LircDivBy16,     /*!< divider is 16   */
    kMCGLITE_LircDivBy32,     /*!< divider is 32   */
    kMCGLITE_LircDivBy64,     /*!< divider is 64   */
    kMCGLITE_LircDivBy128     /*!< divider is 128  */
} mcglite_lirc_div_t;

/*! @brief MCG_Lite clock mode definitions */
typedef enum _mcglite_mode
{
    kMCGLITE_ModeHirc48M, /*!< clock mode is HIRC 48M  */
    kMCGLITE_ModeLirc8M,  /*!< clock mode is LIRC 8M   */
    kMCGLITE_ModeLirc2M,  /*!< clock mode is LIRC 2M   */
    kMCGLITE_ModeExt,     /*!< clock mode is EXT       */
    kMCGLITE_ModeError    /*!< Unknown mode            */
} mcglite_mode_t;

/*! @brief MCG internal reference clock (MCGIRCLK) enable mode definition. */
enum _mcglite_irclk_enable_mode
{
    kMCGLITE_IrclkEnable       = MCG_C1_IRCLKEN_MASK,  /*!< MCGIRCLK enable.              */
    kMCGLITE_IrclkEnableInStop = MCG_C1_IREFSTEN_MASK  /*!< MCGIRCLK enable in stop mode. */
};

/*! @brief MCG_Lite configure structure for mode change. */
typedef struct _mcglite_config
{
    mcglite_clkout_src_t outSrc;  /*!< MCGOUT clock select.                */
    uint8_t irclkEnableMode;      /*!< MCGIRCLK enable mode, see _mcglite_irclk_enable_mode. */
    mcglite_lirc_mode_t ircs;     /*!< MCG_C2[IRCS].                       */
    mcglite_lirc_div_t fcrdiv;    /*!< MCG_SC[FCRDIV].                     */
    mcglite_lirc_div_t lircDiv2;  /*!< MCG_MC[LIRC_DIV2].                  */
    bool hircEnableInNotHircMode; /*!< HIRC enable when not in HIRC mode.  */
} mcglite_config_t;

/*******************************************************************************
* API
******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @name Get clock frequency.
 * @{
 */

/*!
 * @brief Get the MCG_Lite output clock(MCGOUTCLK) frequency.
 *
 * This function gets the MCG_Lite output clock frequency (Hz) based on current
 * MCG_Lite register value.
 *
 * @return The frequency of MCGOUTCLK.
 */
uint32_t CLOCK_GetOutClkFreq(void);

/*!
 * @brief Get the MCG internal reference clock(MCGIRCLK) frequency.
 *
 * This function gets the MCG_Lite internal reference clock frequency (Hz) based
 * on current MCG register value.
 *
 * @return The frequency of MCGIRCLK.
 */
uint32_t CLOCK_GetInternalRefClkFreq(void);

/*!
* @brief Gets the current MCGPCLK frequency.
*
* This function gets the MCGPCLK frequency (Hertz) based on current MCG_Lite
* register settings.
*
* @return The frequency of MCGPCLK.
*/
uint32_t CLOCK_GetPeriphClkFreq(void);

/*! @}*/

/*!
 * @name MCG_Lite mode.
 * @{
 */

/*!
 * @brief Gets the current MCG_Lite mode.
 *
 * This function checks the MCG_Lite registers and determine current MCG_Lite mode.
 *
 * @return Current MCG_Lite mode or error code.
 */
mcglite_mode_t CLOCK_GetMode(void);

/*!
 * @brief Sets the MCG_Lite configuration.
 *
 * This function configures the MCG_Lite, include output clock source, MCGIRCLK
 * setting, HIRC setting and so on, see \ref mcglite_config_t for details.
 *
 * @param  targetConfig Pointer to the target MCG_Lite mode configuration structure.
 * @return Error code.
 */
status_t CLOCK_SetMcgliteConfig(mcglite_config_t const *targetConfig);

/*! @}*/

/*!
 * @name OSC configuration
 * @{
 */

/*!
 * @brief Initialize OSC0.
 *
 * This function initializes OSC0 according to board configuration.
 *
 * @param  config Pointer to the OSC0 configuration structure.
 */
void CLOCK_InitOsc0(osc_config_t const *config);

/*!
 * @brief Deinitialize OSC0.
 *
 * This function deinitializes OSC0.
 */
void CLOCK_DeinitOsc0(void);

/*! @}*/

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*! @}*/

#endif /* _FSL_MCGLITE_H_*/
