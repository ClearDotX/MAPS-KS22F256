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
#include "fsl_mcglite.h"
#include "clock_config.h"
#include <assert.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Clock configuration structure. */
typedef struct _clock_config
{
    mcglite_config_t mcgliteConfig; /*!< MCG configuration.      */
    sim_clock_config_t simConfig;   /*!< SIM configuration.      */
#if (defined(FSL_FEATURE_SOC_OSC_COUNT) && FSL_FEATURE_SOC_OSC_COUNT)
    osc_config_t oscConfig; /*!< OSC configuration.      */
#endif
} clock_config_t;

#if (defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE)
extern const clock_config_t g_defaultClockConfigHsrun;
#endif
extern const clock_config_t g_defaultClockConfigRun;
extern const clock_config_t g_defaultClockConfigVlpr;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

#if (defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE)
/*
 * Set the clock configuration for HSRUN mode.
 */
void APP_SetClockHsrun(void)
{
    CLOCK_SetSimSafeDivs();
    CLOCK_SetMcgliteConfig(&g_defaultClockConfigHsrun.mcgliteConfig);
    CLOCK_SetSimConfig(&g_defaultClockConfigHsrun.simConfig);
}

/*
 * Set the clock configuration for RUN mode from HSRUN mode.
 */
void APP_SetClockRunFromHsrun(void)
{
    CLOCK_SetSimSafeDivs();
    CLOCK_SetMcgliteConfig(&g_defaultClockConfigRun.mcgliteConfig);
    CLOCK_SetSimConfig(&g_defaultClockConfigRun.simConfig);
}
#endif /* FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE */

/*
 * Set the clock configuration for RUN mode from VLPR mode.
 */
void APP_SetClockRunFromVlpr(void)
{
    CLOCK_SetSimSafeDivs();
    CLOCK_SetMcgliteConfig(&g_defaultClockConfigRun.mcgliteConfig);
    CLOCK_SetSimConfig(&g_defaultClockConfigRun.simConfig);
}

/*
 * Set the clock configuration for VLPR mode.
 */
void APP_SetClockVlpr(void)
{
    CLOCK_SetSimSafeDivs();
    CLOCK_SetMcgliteConfig(&g_defaultClockConfigVlpr.mcgliteConfig);
    CLOCK_SetSimConfig(&g_defaultClockConfigVlpr.simConfig);
}
