/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_debug_console.h"

#include "mcg_hal_unit_test.h"
#include "fsl_mcg_hal.h"
#include "fsl_clock_manager.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define DEBUG_ASSERT(x) \
    if (!x) { \
        while(1){}  \
    }

// Function delay to make sure FLL is stable.
static void test_delay(void)
{
    uint32_t freq = CLOCK_SYS_GetCoreClockFreq();
    freq /= 3000U;
    while(freq--) {}
}

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    uint8_t frdiv;
    uint8_t mode_step;
#if FSL_FEATURE_MCG_HAS_PLL
    uint8_t prdiv, vdiv;
    uint32_t desirePllFreq;
#endif
    uint32_t simDiv1;
    uint32_t i;
    for (i = 0; i < PORT_INSTANCE_COUNT; i++)
    {
        CLOCK_SYS_EnablePortClock(i);
    }
  
    BOARD_InitOsc0();

     /****************************************************
                    MCG mode switch test.
     ****************************************************/
#if FSL_FEATURE_MCG_HAS_PLL
     /* If there is PLL, then the test step is:
      * 1.  FEI  ->  FEE
      * 2.  FEE  ->  FEI
      * 3.  FEI  ->  FBE
      * 4.  FBE  ->  BLPE
      * 5.  BLPE ->  PBE
      * 6.  PBE  ->  PEE
      * 7.  PEE  ->  PBE
      * 8.  PBE  ->  FBE
      * 9.  FBE  ->  FBI
      * 10. FBI  ->  BLPI
      * 11. BLPI ->  FBI
      * 12. FBI  ->  FBE
      * 13. FBE  ->  PBE
      * 14. PBE  ->  BLPE
      * 15. BLPE ->  FBE
      * 16. FBE  ->  FEE
      * 17. FEE  ->  FBI
      * 18. FBI  ->  FEE
      * 19. FEE  ->  FBE
      * 20. FBE  ->  FEI
      * 21. FEI  ->  FBI
      * 22. FBI  ->  FEI
      */
    /*-----------------------------------------------------------------------
      Desired PLL frequency:

                 OSC0_XTAL_FREQ
      ----------------------------------- * FSL_FEATURE_MCG_PLL_VDIV_BASE
                 OSC0_XTAL_FREQ
         -----------------------------
          FSL_FEATURE_MCG_PLL_REF_MIN
     -----------------------------------------------------------------------*/
    desirePllFreq = (OSC0_XTAL_FREQ / (OSC0_XTAL_FREQ/FSL_FEATURE_MCG_PLL_REF_MIN))
                  * FSL_FEATURE_MCG_PLL_VDIV_BASE;

#if FSL_FEATURE_MCG_HAS_PLL_INTERNAL_DIV
    desirePllFreq /= 2U;
#endif

    simDiv1 = 0x01330000; //OUTDIV1=0, OUTDIV2=1, OUTDIV3=3, OUTDIV4=3

    // Get valid PRDIV/VDIV, always use OSC0 as clock source.
    DEBUG_ASSERT(desirePllFreq == CLOCK_HAL_CalculatePllDiv(g_xtal0ClkFreq,
                                                            desirePllFreq,
                                                            &prdiv,
                                                            &vdiv));

    if (OSC0_XTAL_FREQ != 12000000LU) {
      /* The 12MHz cannot be divided to a range suitable for FEE */
      DEBUG_ASSERT(kStatus_MCG_Success == CLOCK_HAL_GetAvailableFrdiv((osc_range_t)MCG_RD_C2_RANGE(MCG),
                                                                     (mcg_oscsel_select_t)0U,
                                                                     g_xtal0ClkFreq,
                                                                     &frdiv));
    } else {
      /* The 12MHz cannot be divided to a range suitable for FEE */
      DEBUG_ASSERT(kStatus_MCG_Success == CLOCK_HAL_GetAvailableFrdiv((osc_range_t)MCG_RD_C2_RANGE(MCG),
                                                                     (mcg_oscsel_select_t)0U,
                                                                     48000000UL,
                                                                     &frdiv));
    }
#if FSL_FEATURE_MCG_USE_OSCSEL
    MCG_WR_C7_OSCSEL(MCG, 0U);
#endif

    SIM_WR_CLKDIV1(SIM, simDiv1);
     mode_step = 1;
     while (mode_step < 23)
     {
         // DEBUG_ASSERT
         switch (mode_step)
         {
             case 1:
             case 3:
             case 21:
                 DEBUG_ASSERT(kMcgModeFEI == CLOCK_HAL_GetMcgMode(MCG));
                 break;
             case 2:
             case 17:
             case 19:
                 DEBUG_ASSERT(kMcgModeFEE == CLOCK_HAL_GetMcgMode(MCG));
                 break;
             case 18:
             case 10:
             case 12:
             case 22:
                 DEBUG_ASSERT(kMcgModeFBI == CLOCK_HAL_GetMcgMode(MCG));
                 break;
             case 9:
             case 16:
             case 20:
             case 13:
             case 4:
                 DEBUG_ASSERT(kMcgModeFBE == CLOCK_HAL_GetMcgMode(MCG));
                 break;
             case 11:
                 DEBUG_ASSERT(kMcgModeBLPI == CLOCK_HAL_GetMcgMode(MCG));
                 break;
             case 5:
             case 15:
                 DEBUG_ASSERT(kMcgModeBLPE == CLOCK_HAL_GetMcgMode(MCG));
                 break;
             case 6:
             case 8:
                 DEBUG_ASSERT(kMcgModePBE == CLOCK_HAL_GetMcgMode(MCG));
                 break;
             case 7:
                 DEBUG_ASSERT(kMcgModePEE == CLOCK_HAL_GetMcgMode(MCG));
                 break;
             default:
                 break;
         }
         // switch to next */
         switch (mode_step)
         {
             case 1:
             case 16:
             case 18:
                 CLOCK_HAL_SetFeeMode(MCG, kMcgOscselOsc, frdiv, kMcgDmx32Default,
                         kMcgDcoRangeSelLow, test_delay);
                 break;
             case 2:
             case 20:
             case 22:
                 CLOCK_HAL_SetFeiMode(MCG, kMcgDcoRangeSelLow, test_delay);
                 break;
             case 9:
             case 11:
             case 17:
             case 21:
                 CLOCK_HAL_SetFbiMode(MCG, kMcgDcoRangeSelLow, kMcgIrcFast, 0U,
                         test_delay);
                 break;
             case 3:
             case 12:
             case 8:
             case 15:
             case 19:
                 CLOCK_HAL_SetFbeMode(MCG, kMcgOscselOsc, frdiv, kMcgDmx32Default,
                         kMcgDcoRangeSelLow, test_delay);
                 break;
             case 4:
             case 14:
                 CLOCK_HAL_SetBlpeMode(MCG, kMcgOscselOsc);
                 break;
             case 10:
                 CLOCK_HAL_SetBlpiMode(MCG, 0U, kMcgIrcFast);
                 break;
             case 5:
             case 7:
             case 13:
                 CLOCK_HAL_SetPbeMode(MCG, kMcgOscselOsc, kMcgPllClkSelPll0, prdiv, vdiv);
                 break;
             case 6:
                 CLOCK_HAL_SetPeeMode(MCG);
                 break;
             default:
                 break;
         }
         mode_step ++;
     }
#else
     /* If there is **NOT** PLL, then the test step is:
      * 1.  FEI  ->  FEE
      * 2.  FEE  ->  FEI
      * 3.  FEI  ->  FBE
      * 4.  FBE  ->  BLPE
      * 5.  BLPE ->  FBE
      * 6.  FBE  ->  FBI
      * 7.  FBI  ->  BLPI
      * 8.  BLPI ->  FBI
      * 9.  FBI  ->  FBE
      * 10. FBE  ->  FEE
      * 11. FEE  ->  FBI
      * 12. FBI  ->  FEE
      * 13. FEE  ->  FBE
      * 14. FBE  ->  FEI
      * 15. FEI  ->  FBI
      * 16. FBI  ->  FEI
      */
    DEBUG_ASSERT(kStatus_MCG_Success == CLOCK_HAL_GetAvailableFrdiv((osc_range_t)MCG_RD_C2_RANGE(MCG),
                                                                     (mcg_oscsel_select_t)0U,
                                                                     g_xtal0ClkFreq,
                                                                     &frdiv));
#if FSL_FEATURE_MCG_USE_OSCSEL
    MCG_WR_C7_OSCSEL(MCG, 0U);
#endif

     mode_step = 1;
     while (mode_step < 17)
     {
         // DEBUG_ASSERT
         switch (mode_step)
         {
             case 1:
             case 3:
             case 15:
                 DEBUG_ASSERT(kMcgModeFEI == CLOCK_HAL_GetMcgMode(MCG));
                 break;
             case 2:
             case 11:
             case 13:
                 DEBUG_ASSERT(kMcgModeFEE == CLOCK_HAL_GetMcgMode(MCG));
                 break;
             case 7:
             case 9:
             case 12:
             case 16:
                 DEBUG_ASSERT(kMcgModeFBI == CLOCK_HAL_GetMcgMode(MCG));
                 break;
             case 4:
             case 6:
             case 10:
             case 14:
                 DEBUG_ASSERT(kMcgModeFBE == CLOCK_HAL_GetMcgMode(MCG));
                 break;
             case 8:
                 DEBUG_ASSERT(kMcgModeBLPI == CLOCK_HAL_GetMcgMode(MCG));
                 break;
             case 5:
                 DEBUG_ASSERT(kMcgModeBLPE == CLOCK_HAL_GetMcgMode(MCG));
                 break;
             default:
                 break;
         }
         // switch to next */
         switch (mode_step)
         {
             case 1:
             case 10:
             case 12:
                 CLOCK_HAL_SetFeeMode(MCG, kMcgOscselOsc, frdiv, kMcgDmx32Default,
                         kMcgDcoRangeSelLow, test_delay);
                 break;
             case 2:
             case 14:
             case 16:
                 CLOCK_HAL_SetFeiMode(MCG, kMcgDcoRangeSelLow, test_delay);
                 break;
             case 6:
             case 8:
             case 11:
             case 15:
                 CLOCK_HAL_SetFbiMode(MCG, kMcgDcoRangeSelLow, kMcgIrcFast, 0U,
                         test_delay);
                 break;
             case 3:
             case 5:
             case 9:
             case 13:
                 CLOCK_HAL_SetFbeMode(MCG, kMcgOscselOsc, frdiv, kMcgDmx32Default,
                         kMcgDcoRangeSelLow, test_delay);
                 break;
             case 4:
                 CLOCK_HAL_SetBlpeMode(MCG, kMcgOscselOsc);
                 break;
             case 7:
                 CLOCK_HAL_SetBlpiMode(MCG, 0U, kMcgIrcFast);
                 break;
             default:
                 break;
         }
         mode_step ++;
     }

#endif

    hardware_init();
    printf("MCG frequency test.\r\n");

    printf("MCGOUT clock: %dHz.\r\n", CLOCK_HAL_GetOutClk(MCG));

    printf("FLL clock: %dHz.\r\n", CLOCK_HAL_GetFllClk(MCG));

#if FSL_FEATURE_MCG_HAS_PLL
    printf("PLL0 clock: %dHz.\r\n", CLOCK_HAL_GetPll0Clk(MCG));
#endif

#if FSL_FEATURE_MCG_HAS_PLL1
    printf("PLL1 clock: %dHz.\r\n", CLOCK_HAL_GetPll1Clk(MCG));
#endif

    printf("MCGIRC clock: %dHz.\r\n", CLOCK_HAL_GetInternalRefClk(MCG));

     printf("\r\n-------- MCG unit test Succeed ----------\r\n");

     while(1){}
     return 0;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

  
