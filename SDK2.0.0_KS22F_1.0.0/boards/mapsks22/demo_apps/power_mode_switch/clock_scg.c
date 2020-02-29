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

#include "fsl_common.h"
#include "fsl_scg.h"
#include <assert.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*
 * In SCG based example, SCG clock sources are configured as:
 *
 * SCG_SOSC: Disabled.
 * SCG_SIRC: 8MHz.
 * SCG_FIRC: 48MHz.
 * SCG_SPLL: 72MHz.
 *
 * VLPR:
 *  - Clock source: SIRC
 * RUN:
 *  - Clock source: FIRC
 * HSRUN:
 *  - Clock source: SPLL
 */

/*
 * SIRC clock setting.
 * SIRC clock           : 8MHz
 * SIRCDIV1_CLK output  : Disable
 * SIRCDIV2_CLK output  : Disable
 * SIRCDIV3_CLK output  : 4MHz
 */
const scg_sirc_config_t s_scgSircConfig =
{
    .enableMode  = kSCG_SircEnable | kSCG_SircEnableInLowPower,
    .div1        = kSCG_AsyncClkDisable,
    .div2        = kSCG_AsyncClkDisable,
    .div3        = kSCG_AsyncClkDivBy2,
    .range       = kSCG_SircRangeHigh
};

/*
 * FIRC clock setting.
 * FIRC clock           : 48MHz
 * FIRCDIV1_CLK output  : 48MHz
 * FIRCDIV2_CLK output  : Disable
 * FIRCDIV3_CLK output  : 48MHz
 */
const scg_firc_config_t s_scgFircConfig =
{
    .enableMode  = kSCG_FircEnable,
    .div3        = kSCG_AsyncClkDivBy1,
    .div2        = kSCG_AsyncClkDisable,
    .div1        = kSCG_AsyncClkDivBy1,
    .range       = kSCG_FircRange48M,
    .trimConfig  = NULL
};

/*
 * SYSPLL clock setting.
 * SYSPLL clock       : 72MHz
 * SYSPLLDIV1 output  : 72MHz
 * SYSPLLDIV2 output  : Disable
 * SYSPLLDIV3 output  : 36MHz
 */
const scg_spll_config_t s_scgSysPllConfig =
{
    .enableMode   = kSCG_SysPllEnable,
    .monitorMode  = kSCG_SysPllMonitorDisable,
    .div1         = kSCG_AsyncClkDivBy1,
    .div2         = kSCG_AsyncClkDisable,
    .div3         = kSCG_AsyncClkDivBy2,
    .src          = kSCG_SysPllSrcFirc,
    .prediv       = 5U,
    .mult         = 2U
};

/*
 * System clock configuration while using SIRC in RUN mode.
 * Core clock : 8MHz
 * Slow clock : 4MHz
 */
const scg_sys_clk_config_t s_sysClkConfigSircInRun =
{
    .divSlow = kSCG_SysClkDivBy2,
#if (defined(FSL_FEATURE_SCG_HAS_DIVBUS) && FSL_FEATURE_SCG_HAS_DIVBUS)
    .divBus  = kSCG_SysClkDivBy1,
#else
    .reseved1 = 0U,
#endif
    .reseved2 = 0U,
#if (defined(FSL_FEATURE_SCG_HAS_DIVPLAT) && FSL_FEATURE_SCG_HAS_DIVPLAT)
    .divPlat = kSCG_SysClkDivBy1,
#else
    .reseved3 = 0U,
#endif
    .divCore = kSCG_SysClkDivBy1,
    .reseved4 = 0U,
    .src     = kSCG_SysClkSrcSirc,
    .reseved5 = 0U
};

/*
 * System clock configuration while using SIRC in VLPR mode.
 * Core clock : 4MHz
 * Slow clock : 1MHz
 */
const scg_sys_clk_config_t s_sysClkConfigSircInVlpr =
{
    .divSlow = kSCG_SysClkDivBy4,
#if (defined(FSL_FEATURE_SCG_HAS_DIVBUS) && FSL_FEATURE_SCG_HAS_DIVBUS)
    .divBus  = kSCG_SysClkDivBy1,
#else
    .reseved1 = 0U,
#endif
    .reseved2 = 0U,
#if (defined(FSL_FEATURE_SCG_HAS_DIVPLAT) && FSL_FEATURE_SCG_HAS_DIVPLAT)
    .divPlat = kSCG_SysClkDivBy1,
#else
    .reseved3 = 0U,
#endif
    .divCore = kSCG_SysClkDivBy2,
    .reseved4 = 0U,
    .src     = kSCG_SysClkSrcSirc,
    .reseved5 = 0U
};

/*
 * System clock configuration while using FIRC in RUN mode.
 * Core clock : 48MHz
 * Slow clock : 24MHz
 */
const scg_sys_clk_config_t s_sysClkConfigFircInRun =
{
    .divSlow = kSCG_SysClkDivBy2,
#if (defined(FSL_FEATURE_SCG_HAS_DIVBUS) && FSL_FEATURE_SCG_HAS_DIVBUS)
    .divBus  = kSCG_SysClkDivBy2,
#else
    .reseved1 = 0U,
#endif
    .reseved2 = 0U,
#if (defined(FSL_FEATURE_SCG_HAS_DIVPLAT) && FSL_FEATURE_SCG_HAS_DIVPLAT)
    .divPlat = kSCG_SysClkDivBy1,
#else
    .reseved3 = 0U,
#endif
    .divCore = kSCG_SysClkDivBy1,
    .reseved4 = 0U,
    .src     = kSCG_SysClkSrcFirc,
    .reseved5 = 0U
};

#if (defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE)
/*
 * System clock configuration while using SYSPLL in HSRUN mode.
 * Core clock : 72MHz
 * Slow clock : 24MHz
 */
const scg_sys_clk_config_t s_sysClkConfigSysPllInHsrun =
{
    .divSlow = kSCG_SysClkDivBy3,
#if (defined(FSL_FEATURE_SCG_HAS_DIVBUS) && FSL_FEATURE_SCG_HAS_DIVBUS)
    .divBus  = kSCG_SysClkDivBy2,
#else
    .reseved1 = 0U,
#endif
    .reseved2 = 0U,
#if (defined(FSL_FEATURE_SCG_HAS_DIVPLAT) && FSL_FEATURE_SCG_HAS_DIVPLAT)
    .divPlat = kSCG_SysClkDivBy1,
#else
    .reseved3 = 0U,
#endif
    .divCore = kSCG_SysClkDivBy1,
    .reseved4 = 0U,
    .src     = kSCG_SysClkSrcSysPll,
    .reseved5 = 0U
};
#endif /* FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE */

/*
 * Initialize SCG setting after system boot up.
 */
void APP_InitClock(void)
{
    scg_sys_clk_config_t sysClkConfig;

    /*
     * Setup SIRC and FIRC:
     * On some platforms, SIRC is used by default after reset, while on some
     * other platforms, FIRC is used after reset. So at the begining, the
     * workflow is different.
     */
    CLOCK_GetCurSysClkConfig(&sysClkConfig);

    if (kSCG_SysClkSrcFirc == sysClkConfig.src) /* FIRC used by default. */
    {
        CLOCK_InitSirc(&s_scgSircConfig);
        CLOCK_SetRunModeSysClkConfig(&s_sysClkConfigSircInRun);
        CLOCK_InitFirc(&s_scgFircConfig);
        CLOCK_SetRunModeSysClkConfig(&s_sysClkConfigFircInRun);
    }
    else /* SIRC used by default. */
    {
        CLOCK_InitFirc(&s_scgFircConfig);
        CLOCK_SetRunModeSysClkConfig(&s_sysClkConfigFircInRun);
        CLOCK_InitSirc(&s_scgSircConfig);
    }

    CLOCK_InitSysPll(&s_scgSysPllConfig);

    CLOCK_SetVlprModeSysClkConfig(&s_sysClkConfigSircInVlpr);
#if (defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE)
    CLOCK_SetHsrunModeSysClkConfig(&s_sysClkConfigSysPllInHsrun);
#endif
}

#if (defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE)
/*
 * Set the clock configuration for HSRUN mode.
 */
void APP_SetClockHsrun(void)
{
    while (!CLOCK_IsSysPllValid())
    {
    }
}

/*
 * Set the clock configuration for RUN mode from HSRUN mode.
 */
void APP_SetClockRunFromHsrun(void)
{
}
#endif /* FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE */

/*
 * Set the clock configuration for RUN mode from VLPR mode.
 */
void APP_SetClockRunFromVlpr(void)
{
    while (!CLOCK_IsFircValid())
    {
    }
}

/*
 * Set the clock configuration for VLPR mode.
 */
void APP_SetClockVlpr(void)
{
}
