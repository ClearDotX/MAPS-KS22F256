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

#ifndef _FSL_SCG_H_
#define _FSL_SCG_H_

#include "fsl_common.h"

/*!
 * @addtogroup scg
 * @{
 */

/******************************************************************************
 * Definitions
 *****************************************************************************/
#define FSL_SCG_DRIVER_VERSION (MAKE_VERSION(2, 0, 0)) /*!< Driver version 2.0.0. */

/* XTAL0 clock frequency, it is defined in board level. */
extern uint32_t g_xtal0Freq;

/*!
 * @brief SCG status return codes.
 */
enum _scg_status
{
    kStatus_SCG_Busy        = MAKE_STATUS(kStatusGroup_SCG, 1), /*!< Clock is busy.  */
    kStatus_SCG_InvalidSrc  = MAKE_STATUS(kStatusGroup_SCG, 2)  /*!< Invalid source. */
};

/*!
 * @brief SCG system clock type.
 */
typedef enum _scg_sys_clk
{
    kSCG_SysClkCore,  /*!< Core clock.        */
#if (defined(FSL_FEATURE_SCG_HAS_DIVBUS) && FSL_FEATURE_SCG_HAS_DIVBUS)
    kSCG_SysClkBus,   /*!< Bus clock.         */
#endif
#if (defined(FSL_FEATURE_SCG_HAS_DIVPLAT) && FSL_FEATURE_SCG_HAS_DIVPLAT)
    kSCG_SysClkPlat,  /*!< Platform clock.    */
#endif
    kSCG_SysClkSlow   /*!< System slow clock. */
} scg_sys_clk_t;

/*!
 * @brief SCG system clock source.
 */
typedef enum _scg_sys_clk_src
{
    kSCG_SysClkSrcSysOsc = 1U,  /*!< System OSC. */
    kSCG_SysClkSrcSirc   = 2U,  /*!< Slow IRC.   */
    kSCG_SysClkSrcFirc   = 3U,  /*!< Fast IRC.   */
    kSCG_SysClkSrcSysPll = 6U   /*!< System PLL. */
} scg_sys_clk_src_t;

/*!
 * @brief SCG system clock divider value.
 */
typedef enum _scg_sys_clk_div
{
    kSCG_SysClkDivBy1   = 0U,   /*!< Divided by 1.  */
    kSCG_SysClkDivBy2   = 1U,   /*!< Divided by 2.  */
    kSCG_SysClkDivBy3   = 2U,   /*!< Divided by 3.  */
    kSCG_SysClkDivBy4   = 3U,   /*!< Divided by 4.  */
    kSCG_SysClkDivBy5   = 4U,   /*!< Divided by 5.  */
    kSCG_SysClkDivBy6   = 5U,   /*!< Divided by 6.  */
    kSCG_SysClkDivBy7   = 6U,   /*!< Divided by 7.  */
    kSCG_SysClkDivBy8   = 7U,   /*!< Divided by 8.  */
    kSCG_SysClkDivBy9   = 8U,   /*!< Divided by 9.  */
    kSCG_SysClkDivBy10  = 9U,   /*!< Divided by 10. */
    kSCG_SysClkDivBy11  = 10U,  /*!< Divided by 11. */
    kSCG_SysClkDivBy12  = 11U,  /*!< Divided by 12. */
    kSCG_SysClkDivBy13  = 12U,  /*!< Divided by 13. */
    kSCG_SysClkDivBy14  = 13U,  /*!< Divided by 14. */
    kSCG_SysClkDivBy15  = 14U,  /*!< Divided by 15. */
    kSCG_SysClkDivBy16  = 15U   /*!< Divided by 16. */
} scg_sys_clk_div_t;

/*!
 * @brief SCG system clock configuration.
 */
typedef struct _scg_sys_clk_config
{
    uint32_t divSlow  : 4;  /*!< Slow clock divider, see @ref scg_sys_clk_div_t. */
#if (defined(FSL_FEATURE_SCG_HAS_DIVBUS) && FSL_FEATURE_SCG_HAS_DIVBUS)
    uint32_t divBus   : 4;  /*!< Bus clock divider, see @ref scg_sys_clk_div_t.  */
#else
    uint32_t reseved1 : 4;
#endif
    uint32_t reseved2 : 4;

#if (defined(FSL_FEATURE_SCG_HAS_DIVPLAT) && FSL_FEATURE_SCG_HAS_DIVPLAT)
    uint32_t divPlat  : 4;  /*!< Platform clock divider, see @ref scg_sys_clk_div_t.*/
#else
    uint32_t reseved3 : 4;
#endif

    uint32_t divCore  : 4;  /*!< Core clock divider, see @ref scg_sys_clk_div_t. */

    uint32_t reseved4 : 4;

    uint32_t src      : 4;  /*!< System clock source, see @ref scg_sys_clk_src_t. */

    uint32_t reseved5 : 4;
} scg_sys_clk_config_t;

/*!
 * @brief SCG clock out configuration (CLKOUTSEL).
 */
typedef enum _clock_clkout_src
{
    kClockClkoutSelScgSlow = 0U,  /*!< SCG slow clock. */
    kClockClkoutSelSysOsc  = 1U,  /*!< System OSC.     */
    kClockClkoutSelSirc    = 2U,  /*!< Slow IRC.       */
    kClockClkoutSelFirc    = 3U,  /*!< Fast IRC.       */
    kClockClkoutSelSysPll  = 6U   /*!< System PLL.     */
} clock_clkout_src_t;

/*!
 * @brief SCG asynchronous clock type.
 */
typedef enum _scg_async_clk
{
    kSCG_AsyncDiv1Clk, /*!< The async clock by DIV1, e.g. SOSCDIV1_CLK, SIRCDIV1_CLK. */
    kSCG_AsyncDiv2Clk, /*!< The async clock by DIV2, e.g. SOSCDIV2_CLK, SIRCDIV2_CLK. */
    kSCG_AsyncDiv3Clk  /*!< The async clock by DIV3, e.g. SOSCDIV3_CLK, SIRCDIV3_CLK. */
} scg_async_clk_t;

/*!
 * @brief SCG asynchronous clock divider value.
 */
typedef enum scg_async_clk_div
{
    kSCG_AsyncClkDisable  = 0U,  /*!< Clock output is disabled.  */
    kSCG_AsyncClkDivBy1   = 1U,  /*!< Divided by 1.              */
    kSCG_AsyncClkDivBy2   = 2U,  /*!< Divided by 2.              */
    kSCG_AsyncClkDivBy4   = 3U,  /*!< Divided by 4.              */
    kSCG_AsyncClkDivBy8   = 4U,  /*!< Divided by 8.              */
    kSCG_AsyncClkDivBy16  = 5U,  /*!< Divided by 16.             */
    kSCG_AsyncClkDivBy32  = 6U,  /*!< Divided by 32.             */
    kSCG_AsyncClkDivBy64  = 7U   /*!< Divided by 64.             */
} scg_async_clk_div_t;

/*!
 * @brief SCG system OSC monitor mode.
 */
typedef enum _scg_sosc_monitor_mode
{
    kSCG_SysOscMonitorDisable = 0U,                         /*!< Monitor disable.                          */
    kSCG_SysOscMonitorInt     = SCG_SOSCCSR_SOSCCM_MASK,    /*!< Interrupt when system OSC error detected. */
    kSCG_SysOscMonitorReset   = SCG_SOSCCSR_SOSCCM_MASK  |
                                SCG_SOSCCSR_SOSCCMRE_MASK   /*!< Reset when system OSC error detected.     */
} scg_sosc_monitor_mode_t;

#if (defined(FSL_FEATURE_SCG_HAS_OSC_SCXP) && FSL_FEATURE_SCG_HAS_OSC_SCXP)
/*! @brief Oscillator capacitor load setting.*/
enum _scg_sosc_cap_load
{
    kSCG_SysOscCap2P  = SCG_SOSCCFG_SC2P_MASK,  /*!< 2  pF capacitor load */
    kSCG_SysOscCap4P  = SCG_SOSCCFG_SC4P_MASK,  /*!< 4  pF capacitor load */
    kSCG_SysOscCap8P  = SCG_SOSCCFG_SC8P_MASK,  /*!< 8  pF capacitor load */
    kSCG_SysOscCap16P = SCG_SOSCCFG_SC16P_MASK  /*!< 16 pF capacitor load */
};
#endif /* FSL_FEATURE_SCG_HAS_OSC_SCXP */

/*! @brief OSC work mode. */
typedef enum _scg_sosc_mode
{
    kSCG_SysOscModeExt         = 0U,                       /*!< Use external clock.   */
    kSCG_SysOscModeOscLowPower = SCG_SOSCCFG_EREFS_MASK,   /*!< Oscillator low power. */
    kSCG_SysOscModeOscHighGain = SCG_SOSCCFG_EREFS_MASK |
                                 SCG_SOSCCFG_HGO_MASK      /*!< Oscillator high gain. */
} scg_sosc_mode_t;

/*! @brief OSC enable mode. */
enum _scg_sosc_enable_mode
{
    kSCG_SysOscEnable           = SCG_SOSCCSR_SOSCEN_MASK,       /*!< Enable OSC clock. */
    kSCG_SysOscEnableInStop     = SCG_SOSCCSR_SOSCSTEN_MASK,     /*!< Enable OSC in stop mode. */
    kSCG_SysOscEnableInLowPower = SCG_SOSCCSR_SOSCLPEN_MASK,     /*!< Enable OSC in low power mode. */
    kSCG_SysOscEnableErClk      = SCG_SOSCCSR_SOSCERCLKEN_MASK   /*!< Enable OSCERCLK. */
};

/*!
 * @brief SCG system OSC configuration.
 */
typedef struct _scg_sosc_config
{
    uint32_t freq;               /*!< System OSC frequency.                    */
    scg_sosc_monitor_mode_t monitorMode; /*!< Clock monitor mode selected.     */
    uint8_t enableMode;          /*!< Enable mode, OR'ed value of _scg_sosc_enable_mode.  */

    scg_async_clk_div_t div1;    /*!< SOSCDIV1 value.                          */
    scg_async_clk_div_t div2;    /*!< SOSCDIV2 value.                          */
    scg_async_clk_div_t div3;    /*!< SOSCDIV3 value.                          */

#if (defined(FSL_FEATURE_SCG_HAS_OSC_SCXP) && FSL_FEATURE_SCG_HAS_OSC_SCXP)
    uint8_t capLoad;             /*!< Capacitor load, OR'ed value of _scg_sosc_cap_load. */
#endif /* FSL_FEATURE_SCG_HAS_OSC_SCXP */
    scg_sosc_mode_t workMode;    /*!< OSC work mode.                           */
} scg_sosc_config_t;

/*!
 * @brief SCG slow IRC clock frequency range.
 */
typedef enum _scg_sirc_range
{
    kSCG_SircRangeLow,  /*!< Slow IRC low range clock (2 MHz).  */
    kSCG_SircRangeHigh  /*!< Slow IRC high range clock (8 MHz). */
} scg_sirc_range_t;

/*! @brief SIRC enable mode. */
enum _scg_sirc_enable_mode
{
    kSCG_SircEnable           = SCG_SIRCCSR_SIRCEN_MASK,    /*!< Enable SIRC clock.             */
    kSCG_SircEnableInStop     = SCG_SIRCCSR_SIRCSTEN_MASK,  /*!< Enable SIRC in stop mode.      */
    kSCG_SircEnableInLowPower = SCG_SIRCCSR_SIRCLPEN_MASK   /*!< Enable SIRC in low power mode. */
};

/*!
 * @brief SCG slow IRC clock configuration.
 */
typedef struct _scg_sirc_config
{
    uint32_t enableMode;         /*!< Enable mode, OR'ed value of _scg_sirc_enable_mode. */

    scg_async_clk_div_t div1;    /*!< SIRCDIV1 value.                          */
    scg_async_clk_div_t div2;    /*!< SIRCDIV2 value.                          */
    scg_async_clk_div_t div3;    /*!< SIRCDIV3 value.                          */

    scg_sirc_range_t range;      /*!< Slow IRC frequency range.                */
} scg_sirc_config_t;

/*!
 * @brief SCG fast IRC trim mode.
 */
typedef enum _scg_firc_trim_mode
{
    kSCG_FircTrimNonUpdate = SCG_FIRCCSR_FIRCTREN_MASK,
    /*!< FIRC trim enable but not enable trim value update. In this mode, the
     trim value is fixed to the initialized value which is defined by
     trimCoar and trimFine in configure structure \ref scg_firc_trim_config_t.*/

    kSCG_FircTrimUpdate    = SCG_FIRCCSR_FIRCTREN_MASK
                           | SCG_FIRCCSR_FIRCTRUP_MASK
    /*!< FIRC trim enable and trim value update enable. In this mode, the trim
     value is auto update. */

} scg_firc_trim_mode_t;

/*!
 * @brief SCG fast IRC trim predivide value for system OSC.
 */
typedef enum _scg_firc_trim_div
{
    kSCG_FircTrimDivBy1,    /*!< Divided by 1.    */
    kSCG_FircTrimDivBy128,  /*!< Divided by 128.  */
    kSCG_FircTrimDivBy256,  /*!< Divided by 256.  */
    kSCG_FircTrimDivBy512,  /*!< Divided by 512.  */
    kSCG_FircTrimDivBy1024, /*!< Divided by 1024. */
    kSCG_FircTrimDivBy2048  /*!< Divided by 2048. */
} scg_firc_trim_div_t;

/*!
 * @brief SCG fast IRC trim source.
 */
typedef enum _scg_firc_trim_src
{
    kSCG_FircTrimSrcUsb0,   /*!< USB0 start of frame (1kHz). */
    kSCG_FircTrimSrcUsb1,   /*!< USB1 start of frame (1kHz). */
    kSCG_FircTrimSrcSysOsc, /*!< System OSC.                 */
    kSCG_FircTrimSrcRtcOsc  /*!< RTC OSC (32.768 kHz).       */
} scg_firc_trim_src_t;

/*!
 * @brief SCG fast IRC clock trim configuration.
 */
typedef struct _scg_firc_trim_config
{
    scg_firc_trim_mode_t trimMode; /*!< FIRC trim mode.                       */

    scg_firc_trim_src_t trimSrc;   /*!< Trim source.                          */
    scg_firc_trim_div_t trimDiv;   /*!< Trim predivide value for system OSC.  */

    uint8_t trimCoar;              /*!< Trim coarse value.                    */
    uint8_t trimFine;              /*!< Trim fine value.                      */
} scg_firc_trim_config_t;

/*!
 * @brief SCG fast IRC clock frequency range.
 */
typedef enum _scg_firc_range
{
    kSCG_FircRange48M, /*!< Fast IRC is trimmed to 48MHz.  */
    kSCG_FircRange52M, /*!< Fast IRC is trimmed to 52MHz.  */
    kSCG_FircRange56M, /*!< Fast IRC is trimmed to 56MHz.  */
    kSCG_FircRange60M  /*!< Fast IRC is trimmed to 60MHz.  */
} scg_firc_range_t;

/*! @brief FIRC enable mode. */
enum _scg_firc_enable_mode
{
    kSCG_FircEnable           = SCG_FIRCCSR_FIRCEN_MASK,    /*!< Enable FIRC clock.             */
    kSCG_FircEnableInStop     = SCG_FIRCCSR_FIRCSTEN_MASK,  /*!< Enable FIRC in stop mode.      */
    kSCG_FircEnableInLowPower = SCG_FIRCCSR_FIRCLPEN_MASK,  /*!< Enable FIRC in low power mode. */
    kSCG_FircDisableRegulator = SCG_FIRCCSR_FIRCREGOFF_MASK /*!< Disable regulator.             */
};

/*!
 * @brief SCG fast IRC clock configuration.
 */
typedef struct _scg_firc_config_t
{
    uint32_t enableMode;         /*!< Enable mode, OR'ed value of _scg_firc_enable_mode. */

    scg_async_clk_div_t div1;    /*!< FIRCDIV1 value.                          */
    scg_async_clk_div_t div2;    /*!< FIRCDIV2 value.                          */
    scg_async_clk_div_t div3;    /*!< FIRCDIV3 value.                          */

    scg_firc_range_t range;      /*!< Fast IRC frequency range.                 */

    scg_firc_trim_config_t *trimConfig; /*!< Pointer to the FIRC trim configuration. */
} scg_firc_config_t;

/*!
 * @brief SCG system PLL clock source.
 */
typedef enum _scg_spll_src
{
    kSCG_SysPllSrcSysOsc, /*!< System PLL clock source is system OSC. */
    kSCG_SysPllSrcFirc    /*!< System PLL clock source is fast IRC.   */
} scg_spll_src_t;

/*!
 * @brief SCG system PLL monitor mode.
 */
typedef enum _scg_spll_monitor_mode
{
    kSCG_SysPllMonitorDisable = 0U,                         /*!< Monitor disable.                          */
    kSCG_SysPllMonitorInt     = SCG_SPLLCSR_SPLLCM_MASK,    /*!< Interrupt when system PLL error detected. */
    kSCG_SysPllMonitorReset   = SCG_SPLLCSR_SPLLCM_MASK
                              | SCG_SPLLCSR_SPLLCMRE_MASK   /*!< Reset when system PLL error detected.     */
} scg_spll_monitor_mode_t;

/*! @brief SPLL enable mode. */
enum _scg_spll_enable_mode
{
    kSCG_SysPllEnable        = SCG_SPLLCSR_SPLLEN_MASK,    /*!< Enable SPLL clock.             */
    kSCG_SysPllEnableInStop  = SCG_SPLLCSR_SPLLSTEN_MASK   /*!< Enable SPLL in stop mode.      */
};

/*!
 * @brief SCG system PLL configuration.
 */
typedef struct _scg_spll_config_t
{
    uint8_t enableMode;          /*!< Enable mode, OR'ed value of _scg_spll_enable_mode */
    scg_spll_monitor_mode_t monitorMode; /*!< Clock monitor mode selected.     */

    scg_async_clk_div_t div1;    /*!< SPLLDIV1 value.                          */
    scg_async_clk_div_t div2;    /*!< SPLLDIV2 value.                          */
    scg_async_clk_div_t div3;    /*!< SPLLDIV3 value.                          */

    scg_spll_src_t src;          /*!< Clock source.                            */
    uint8_t prediv;              /*!< PLL reference clock divider.             */
    uint8_t mult;                /*!< System PLL multiplier.                   */
} scg_spll_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @name MCU System Clock.
 * @{
 */

/*!
 * @brief Get SCG system clock frequency.
 *
 * This function gets the SCG system clock frequency, these clocks are used for
 * core, platform, external and bus clock domains.
 *
 * @param type     Which type of clock to get, core clock or slow clock.
 * @return  Clock frequency.
 */
uint32_t CLOCK_GetSysClkFreq(scg_sys_clk_t type);

/*!
 * @brief Set the system clock configuration for VLPR mode.
 *
 * This function sets the system clock configuration for VLPR mode.
 *
 * @param config Poionter to the configuration.
 */
static void CLOCK_SetVlprModeSysClkConfig(const scg_sys_clk_config_t *config)
{
    assert(config);

    SCG->VCCR = *(const uint32_t *)config;
}

/*!
 * @brief Set the system clock configuration for RUN mode.
 *
 * This function sets the system clock configuration for RUN mode.
 *
 * @param config Poionter to the configuration.
 */
static void CLOCK_SetRunModeSysClkConfig(const scg_sys_clk_config_t *config)
{
    assert(config);

    SCG->RCCR = *(const uint32_t *)config;
}

/*!
 * @brief Set the system clock configuration for HSRUN mode.
 *
 * This function sets the system clock configuration for HSRUN mode.
 *
 * @param config Poionter to the configuration.
 */
static void CLOCK_SetHsrunModeSysClkConfig(const scg_sys_clk_config_t *config)
{
    assert(config);

    SCG->HCCR = *(const uint32_t *)config;
}

/*!
 * @brief Get the system clock configuration in current power mode.
 *
 * This function gets the system configuration in current power mode.
 *
 * @param config Poionter to the configuration.
 */
static inline void CLOCK_GetCurSysClkConfig(scg_sys_clk_config_t *config)
{
    assert(config);

    *(uint32_t *)config = SCG->CSR;
}

/*!
 * @brief Set clock out selection.
 *
 * This function sets the clock out selection (CLKOUTSEL).
 *
 * @param setting The selection to set.
 * @return  Current clock out selection.
 */
static inline void CLOCK_SetClkOutSel(clock_clkout_src_t setting)
{
    SCG->CLKOUTCNFG = SCG_CLKOUTCNFG_CLKOUTSEL(setting);
}
/* @} */

/*!
 * @name SCG System OSC Clock.
 * @{
 */

/*!
 * @brief Initialize SCG system OSC.
 *
 * This function enables the SCG system OSC clock according to the
 * configuration.
 *
 * @param config   Pointer to the configuration structure.
 * @retval kStatus_Success System OSC is initialized.
 * @retval kStatus_SCG_Busy System OSC has been enabled and used by system clock.
 * @retval kStatus_ReadOnly System OSC control register is locked.
 *
 * @note This function can not detect whether system OSC has been enabled and
 * used by some IPs.
 */
status_t CLOCK_InitSysOsc(const scg_sosc_config_t *config);

/*!
 * @brief De-initialize SCG system OSC.
 *
 * This function disables the SCG system OSC clock.
 *
 * @retval kStatus_Success System OSC is deinitialized.
 * @retval kStatus_SCG_Busy System OSC is used by system clock.
 * @retval kStatus_ReadOnly System OSC control register is locked.
 *
 * @note This function can not detect whether system OSC is used by some IPs.
 */
status_t CLOCK_DeinitSysOsc(void);

/*!
 * @brief Get SCG system OSC clock frequency (SYSOSC).
 *
 * @return  Clock frequency, if clock is invalid, return 0.
 */
uint32_t CLOCK_GetSysOscFreq(void);

/*!
 * @brief Get SCG asynchronous clock frequency from system OSC.
 *
 * @param type     The asynchronous clock type.
 * @return  Clock frequency, if clock is invalid, return 0.
 */
uint32_t CLOCK_GetSysOscAsyncFreq(scg_async_clk_t type);

/*!
 * @brief Check whether system OSC clock error occurs.
 *
 * @return  True if error occurs, false if not.
 */
static inline bool CLOCK_IsSysOscErr(void) { return (bool)(SCG->SOSCCSR & SCG_SOSCCSR_SOSCERR_MASK); }

/*!
 * @brief Clear system OSC clock error.
 */
static inline void CLOCK_ClearSysOscErr(void) { SCG->SOSCCSR |= SCG_SOSCCSR_SOSCERR_MASK; }

/*!
 * @brief Set the system OSC monitor mode.
 *
 * This function sets the system OSC monitor mode. The mode could be disabled,
 * or generate interrupt when error disabled, or reset when error detected.
 *
 * @param mode Monitor mode to set.
 */
static inline void CLOCK_SetSysOscMonitorMode(scg_sosc_monitor_mode_t mode)
{
    uint32_t reg = SCG->SOSCCSR;

    reg &= ~(SCG_SOSCCSR_SOSCCM_MASK | SCG_SOSCCSR_SOSCCMRE_MASK);

    reg |= (uint32_t)mode;

    SCG->SOSCCSR = reg;
}

/*!
 * @brief Check whether system OSC clock is valid.
 *
 * @return  True if clock is valid, false if not.
 */
static inline bool CLOCK_IsSysOscValid(void) { return (bool)(SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK); }
/* @} */

/*!
 * @name SCG Slow IRC Clock.
 * @{
 */

/*!
 * @brief Initialize SCG slow IRC clock.
 *
 * This function enables the SCG slow IRC clock according to the
 * configuration.
 *
 * @param config   Pointer to the configuration structure.
 * @retval kStatus_Success SIRC is initialized.
 * @retval kStatus_SCG_Busy SIRC has been enabled and used by system clock.
 * @retval kStatus_ReadOnly SIRC control register is locked.
 *
 * @note This function can not detect whether system OSC has been enabled and
 * used by some IPs.
 */
status_t CLOCK_InitSirc(const scg_sirc_config_t *config);

/*!
 * @brief De-initialize SCG slow IRC.
 *
 * This function disables the SCG slow IRC.
 *
 * @retval kStatus_Success SIRC is deinitialized.
 * @retval kStatus_SCG_Busy SIRC is used by system clock.
 * @retval kStatus_ReadOnly SIRC control register is locked.
 *
 * @note This function can not detect whether SIRC is used by some IPs.
 */
status_t CLOCK_DeinitSirc(void);

/*!
 * @brief Get SCG SIRC clock frequency.
 *
 * @return  Clock frequency, if clock is invalid, return 0.
 */
uint32_t CLOCK_GetSircFreq(void);

/*!
 * @brief Get SCG asynchronous clock frequency from SRIC.
 *
 * @param type     The asynchronous clock type.
 * @return  Clock frequency, if clock is invalid, return 0.
 */
uint32_t CLOCK_GetSircAsyncFreq(scg_async_clk_t type);

/*!
 * @brief Check whether SIRC clock is valid.
 *
 * @return  True if clock is valid, false if not.
 */
static inline bool CLOCK_IsSircValid(void) { return (bool)(SCG->SIRCCSR & SCG_SIRCCSR_SIRCVLD_MASK); }
/* @} */

/*!
 * @name SCG Fast IRC Clock.
 * @{
 */

/*!
 * @brief Initialize SCG fast IRC clock.
 *
 * This function enables the SCG fast IRC clock according to the configuration.
 *
 * @param config   Pointer to the configuration structure.
 * @retval kStatus_Success FIRC is initialized.
 * @retval kStatus_SCG_Busy FIRC has been enabled and used by system clock.
 * @retval kStatus_ReadOnly FIRC control register is locked.
 *
 * @note This function can not detect whether FIRC has been enabled and
 * used by some IPs.
 */
status_t CLOCK_InitFirc(const scg_firc_config_t *config);

/*!
 * @brief De-initialize SCG fast IRC.
 *
 * This function disables the SCG fast IRC.
 *
 * @retval kStatus_Success FIRC is deinitialized.
 * @retval kStatus_SCG_Busy FIRC is used by system clock.
 * @retval kStatus_ReadOnly FIRC control register is locked.
 *
 * @note This function can not detect whether FIRC is used by some IPs.
 */
status_t CLOCK_DeinitFirc(void);

/*!
 * @brief Get SCG FIRC clock frequency.
 *
 * @return  Clock frequency, if clock is invalid, return 0.
 */
uint32_t CLOCK_GetFircFreq(void);

/*!
 * @brief Get SCG asynchronous clock frequency from FRIC.
 *
 * @param type     The asynchronous clock type.
 * @return  Clock frequency, if clock is invalid, return 0.
 */
uint32_t CLOCK_GetFircAsyncFreq(scg_async_clk_t type);

/*!
 * @brief Check whether FIRC clock is valid.
 *
 * @return  True if clock is valid, false if not.
 */
static inline bool CLOCK_IsFircValid(void) { return (bool)(SCG->FIRCCSR & SCG_FIRCCSR_FIRCVLD_MASK); }
/* @} */

/*!
 * @name SCG System PLL Clock.
 * @{
 */

/*!
 * @brief Calculate the MULT and PREDIV for PLL.
 *
 * This function calculates the proper MULT and PREDIV to generate desired PLL
 * output frequency with input reference clock frequency. It returns the closest
 * frequency PLL could generate, the corresponding MULT/PREDIV are returned from
 * parameters. If desired frequency is not valid, this funtion returns 0.
 *
 * @param refFreq     The input reference clock frequency.
 * @param desireFreq  The desired output clock frequency.
 * @param mult        The value of MULT.
 * @param prediv      The value of PREDIV.
 * @return The PLL output frequency with the MULT and PREDIV, if could not generate
 * the desired frequency, this function returns 0U.
 */
uint32_t CLOCK_GetSysPllMultDiv(uint32_t refFreq, uint32_t desireFreq, uint8_t *mult, uint8_t *prediv);

/*!
 * @brief Initialize SCG system PLL.
 *
 * This function enables the SCG system PLL clock according to the
 * configuration. The system PLL could use system OSC or FIRC as
 * the clock source, please make sure the source clock is valid before
 * this function.
 *
 * @param config   Pointer to the configuration structure.
 * @retval kStatus_Success System PLL is initialized.
 * @retval kStatus_SCG_Busy System PLL has been enabled and used by system clock.
 * @retval kStatus_ReadOnly System PLL control register is locked.
 *
 * @note This function can not detect whether system PLL has been enabled and
 * used by some IPs.
 */
status_t CLOCK_InitSysPll(const scg_spll_config_t *config);

/*!
 * @brief De-initialize SCG system PLL.
 *
 * This function disables the SCG system PLL.
 *
 * @retval kStatus_Success system PLL is deinitialized.
 * @retval kStatus_SCG_Busy system PLL is used by system clock.
 * @retval kStatus_ReadOnly System PLL control register is locked.
 *
 * @note This function can not detect whether system PLL is used by some IPs.
 */
status_t CLOCK_DeinitSysPll(void);

/*!
 * @brief Get SCG system PLL clock frequency.
 *
 * @return  Clock frequency, if clock is invalid, return 0.
 */
uint32_t CLOCK_GetSysPllFreq(void);

/*!
 * @brief Get SCG asynchronous clock frequency from system PLL.
 *
 * @param type     The asynchronous clock type.
 * @return  Clock frequency, if clock is invalid, return 0.
 */
uint32_t CLOCK_GetSysPllAsyncFreq(scg_async_clk_t type);

/*!
 * @brief Check whether system PLL clock error occurs.
 *
 * @return  True if error occurs, false if not.
 */
static inline bool CLOCK_IsSysPllErr(void) { return (bool)(SCG->SPLLCSR & SCG_SPLLCSR_SPLLERR_MASK); }

/*!
 * @brief Clear system PLL clock error.
 */
static inline void CLOCK_ClearSysPllErr(void) { SCG->SPLLCSR |= SCG_SPLLCSR_SPLLERR_MASK; }

/*!
 * @brief Set the system PLL monitor mode.
 *
 * This function sets the system PLL monitor mode. The mode could be disabled,
 * or generate interrupt when error disabled, or reset when error detected.
 *
 * @param mode Monitor mode to set.
 */
static inline void CLOCK_SetSysPllMonitorMode(scg_spll_monitor_mode_t mode)
{
    uint32_t reg = SCG->SPLLCSR;

    reg &= ~(SCG_SPLLCSR_SPLLCM_MASK | SCG_SPLLCSR_SPLLCMRE_MASK);

    reg |= (uint32_t)mode;

    SCG->SPLLCSR = reg;
}

/*!
 * @brief Check whether system PLL clock is valid.
 *
 * @return  True if clock is valid, false if not.
 */
static inline bool CLOCK_IsSysPllValid(void) { return (bool)(SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK); }
/* @} */

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */

#endif /* FSL_SCG_H */