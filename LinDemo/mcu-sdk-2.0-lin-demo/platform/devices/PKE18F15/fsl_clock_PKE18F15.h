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

#ifndef _FSL_CLOCK_H_
#define _FSL_CLOCK_H_

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "fsl_device_registers.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Clock ip name array for DMAMUX. */
#define DMAMUX_CLOCKS {kCLOCK_Dmamux0}

/*! @brief Clock ip name array for RTC. */
#define RTC_CLOCKS {kCLOCK_Rtc0}

/*! @brief Clock ip name array for RTCOSC. */
#define RTCOSC_CLOCKS {kCLOCK_RtcOsc0}

/*! @brief Clock ip name array for SCG. */
#define SCG_CLOCKS {kCLOCK_Scg0}

/*! @brief Clock ip name array for PORT. */
#define PORT_CLOCKS {kCLOCK_PortA, kCLOCK_PortB, kCLOCK_PortC, kCLOCK_PortD, kCLOCK_PortE}

/*! @brief Clock ip name array for LPI2C. */
#define LPI2C_CLOCKS {kCLOCK_Lpi2c0, kCLOCK_Lpi2c1}

/*! @brief Clock ip name array for FLEXIO. */
#define FLEXIO_CLOCKS {kCLOCK_Flexio0}

/*! @brief Clock ip name array for TSI. */
#define TSI_CLOCKS {kCLOCK_Tsi0}

/*! @brief Clock ip name array for EMVSIM. */
#define EMVSIM_CLOCKS {kCLOCK_Emvsim0}

/*! @brief Clock ip name array for EDMA. */
#define EDMA_CLOCKS {kCLOCK_Dma0}

/*! @brief Clock ip name array for LPUART. */
#define LPUART_CLOCKS {kCLOCK_Lpuart0, kCLOCK_Lpuart1, kCLOCK_Lpuart2, kCLOCK_Lpuart3}

/*! @brief Clock ip name array for DAC. */
#define DAC_CLOCKS {kCLOCK_Dac0}

/*! @brief Clock ip name array for LPTMR. */
#define LPTMR_CLOCKS {kCLOCK_Lptmr0}

/*! @brief Clock ip name array for ADC16. */
#define ADC16_CLOCKS {kCLOCK_Adc0, kCLOCK_Adc1, kCLOCK_Adc2}

/*! @brief Clock ip name array for ATX. */
#define ATX_CLOCKS {kCLOCK_Atx0}

/*! @brief Clock ip name array for LPSPI. */
#define LPSPI_CLOCKS {kCLOCK_Lpspi0, kCLOCK_Lpspi1, kCLOCK_Lpspi2}

/*! @brief Clock ip name array for TRGMUX. */
#define TRGMUX_CLOCKS {kCLOCK_Trgmux0}

/*! @brief Clock ip name array for LPIT. */
#define LPIT_CLOCKS {kCLOCK_Lpit0}

/*! @brief Clock ip name array for CRC. */
#define CRC_CLOCKS {kCLOCK_Crc0}

/*! @brief Clock ip name array for CMP. */
#define CMP_CLOCKS {kCLOCK_Cmp0, kCLOCK_Cmp1, kCLOCK_Cmp2}

/*! @brief Clock ip name array for FLASH. */
#define FLASH_CLOCKS {kCLOCK_Flash0}

/*! @brief Clock ip name array for MPU. */
#define MPU_CLOCKS {kCLOCK_Mpu0}

/*! @brief Clock ip name array for SIM. */
#define SIM_CLOCKS {kCLOCK_Sim0}

/*! @brief Clock ip name array for WDOG. */
#define WDOG_CLOCKS {kCLOCK_Wdog0}

/*! @brief Clock ip name array for DUMMY. */
#define DUMMY_CLOCKS {kCLOCK_Dummy101}

/*! @brief Clock ip name array for EWM. */
#define EWM_CLOCKS {kCLOCK_Ewm0}

/*! @brief Clock ip name array for FLEXCAN. */
#define FLEXCAN_CLOCKS {kCLOCK_Flexcan0, kCLOCK_Flexcan1, kCLOCK_Flexcan2}

/*! @brief Clock ip name array for FLEXTMR. */
#define FLEXTMR_CLOCKS {kCLOCK_Flextmr0, kCLOCK_Flextmr1, kCLOCK_Flextmr2, kCLOCK_Flextmr3}

/*! @brief Clock ip name array for PDB. */
#define PDB_CLOCKS {kCLOCK_Pdb0, kCLOCK_Pdb1, kCLOCK_Pdb2}

/*!
 * @brief LPO clock frequency.
 */
#define LPO_CLK_FREQ 128000U

/*! @brief Clock name used to get clock frequency. */
typedef enum _clock_name
{
    /* ----------------------------- System layer clock -------------------------------*/
    kCLOCK_CoreSysClk,        /*!< Core/system clock                                   */
    kCLOCK_PlatClk,           /*!< Platform clock                                      */
    kCLOCK_BusClk,            /*!< Bus clock                                           */
    kCLOCK_FlexBusClk,        /*!< FlexBus clock                                       */
    kCLOCK_FlashClk,          /*!< Flash clock                                         */

    /* ------------------------------------ SCG clock ---------------------------------*/
    kCLOCK_ScgSysOscClk,      /*!< SCG system OSC clock. (SYSOSC)                      */
    kCLOCK_ScgSircClk,        /*!< SCG SIRC clock.                                     */
    kCLOCK_ScgFircClk,        /*!< SCG FIRC clock.                                     */
    kCLOCK_ScgSysPllClk,      /*!< SCG system PLL clock. (SYSPLL)                      */

    kCLOCK_ScgSysOscAsyncDiv1Clk, /*!< SOSCDIV1_CLK.                                   */
    kCLOCK_ScgSysOscAsyncDiv2Clk, /*!< SOSCDIV2_CLK.                                   */
    kCLOCK_ScgSysOscAsyncDiv3Clk, /*!< SOSCDIV3_CLK.                                   */

    kCLOCK_ScgSircAsyncDiv1Clk,   /*!< SIRCDIV1_CLK.                                   */
    kCLOCK_ScgSircAsyncDiv2Clk,   /*!< SIRCDIV2_CLK.                                   */
    kCLOCK_ScgSircAsyncDiv3Clk,   /*!< SIRCDIV3_CLK.                                   */

    kCLOCK_ScgFircAsyncDiv1Clk,   /*!< FIRCDIV1_CLK.                                   */
    kCLOCK_ScgFircAsyncDiv2Clk,   /*!< FIRCDIV2_CLK.                                   */
    kCLOCK_ScgFircAsyncDiv3Clk,   /*!< FIRCDIV3_CLK.                                   */

    kCLOCK_ScgSysPllAsyncDiv1Clk, /*!< SPLLDIV1_CLK.                                   */
    kCLOCK_ScgSysPllAsyncDiv2Clk, /*!< SPLLDIV2_CLK.                                   */
    kCLOCK_ScgSysPllAsyncDiv3Clk, /*!< SPLLDIV3_CLK.                                   */

    /* --------------------------------- Other clock ----------------------------------*/
    kCLOCK_LpoClk,            /*!< LPO clock                                           */
    kCLOCK_Osc32kClk,         /*!< External OSC 32K clock (OSC32KCLK)                  */
    kCLOCK_ErClk,             /*!< ERCLK. The external reference clock from SCG.       */
    kCLOCK_Osc0ErClk = kCLOCK_ErClk,     /*!< for compatible with other MCG platforms. */
    kCLOCK_Er32kClk  = kCLOCK_Osc32kClk  /*!< For compatible with other MCG platforms. */
} clock_name_t;

/*!
 * @brief Clock source for peripherals that support various clock selections.
 */
typedef enum _clock_ip_src
{
    kCLOCK_IpSrcNoneOrExt   = 0U,   /*!< Clock is off or external clock is used. */
    kCLOCK_IpSrcSysOscAsync = 1U,   /*!< System Oscillator async clock.          */
    kCLOCK_IpSrcSircAsync   = 2U,   /*!< Slow IRC async clock.                   */
    kCLOCK_IpSrcFircAsync   = 3U,   /*!< Fast IRC async clock.                   */
    kCLOCK_IpSrcSysPllAsync = 6U    /*!< System PLL async clock.                 */
} clock_ip_src_t;

/*!
 * @brief Peripheral clock name difinition used for clock gate, clock source
 * and clock divider setting. It is defined as the corresponding register address.
 */
typedef enum _clock_ip_name
{
    kCLOCK_IpInvalid = 0U,

    /* PCC 0 */
    kCLOCK_Dma0      = 0x40065020U,
    kCLOCK_Mpu0      = 0x40065034U,
    kCLOCK_Flash0    = 0x40065080U,
    kCLOCK_Dmamux0   = 0x40065084U,
    kCLOCK_Flexcan0  = 0x40065090U,
    kCLOCK_Flexcan1  = 0x40065094U,
    kCLOCK_Flextmr3  = 0x40065098U,
    kCLOCK_Adc1      = 0x4006509CU,
    kCLOCK_Flexcan2  = 0x400650ACU,
    kCLOCK_Lpspi0    = 0x400650B0U,
    kCLOCK_Lpspi1    = 0x400650B4U,
    kCLOCK_Lpspi2    = 0x400650B8U,
    kCLOCK_Pdb1      = 0x400650C4U,
    kCLOCK_Crc0      = 0x400650C8U,
    kCLOCK_Pdb2      = 0x400650CCU,
    kCLOCK_Pdb0      = 0x400650D8U,
    kCLOCK_Lpit0     = 0x400650DCU,
    kCLOCK_Flextmr0  = 0x400650E0U,
    kCLOCK_Flextmr1  = 0x400650E4U,
    kCLOCK_Flextmr2  = 0x400650E8U,
    kCLOCK_Adc0      = 0x400650ECU,
    kCLOCK_Adc2      = 0x400650F0U,
    kCLOCK_Rtc0      = 0x400650F4U,
    kCLOCK_Dac0      = 0x400650FCU,
    kCLOCK_Lptmr0    = 0x40065100U,
    kCLOCK_Tsi0      = 0x40065114U,
    kCLOCK_Atx0      = 0x40065118U,
    kCLOCK_Sim0      = 0x40065120U,
    kCLOCK_PortA     = 0x40065124U,
    kCLOCK_PortB     = 0x40065128U,
    kCLOCK_PortC     = 0x4006512CU,
    kCLOCK_PortD     = 0x40065130U,
    kCLOCK_PortE     = 0x40065134U,
    kCLOCK_Wdog0     = 0x40065148U,
    kCLOCK_Flexio0   = 0x40065168U,
    kCLOCK_Emvsim0   = 0x40065178U,
    kCLOCK_RtcOsc0   = 0x40065180U,
    kCLOCK_Ewm0      = 0x40065184U,
    kCLOCK_Trgmux0   = 0x4006518CU,
    kCLOCK_Scg0      = 0x40065190U,
    kCLOCK_Dummy101  = 0x40065194U,
    kCLOCK_Lpi2c0    = 0x40065198U,
    kCLOCK_Lpi2c1    = 0x4006519CU,
    kCLOCK_Lpuart0   = 0x400651A8U,
    kCLOCK_Lpuart1   = 0x400651ACU,
    kCLOCK_Lpuart2   = 0x400651B0U,
    kCLOCK_Lpuart3   = 0x400651B4U,
    kCLOCK_Cmp0      = 0x400651CCU,
    kCLOCK_Cmp1      = 0x400651D0U,
    kCLOCK_Cmp2      = 0x400651D4U,
} clock_ip_name_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief Enable the clock for specific IP.
 *
 * @param name  Which clock to enable, see \ref clock_ip_name_t.
 */
static inline void CLOCK_EnableClock(clock_ip_name_t name)
{
    assert((*(volatile uint32_t *)name) & PCC_CLKCFG_PR_MASK);

    (*(volatile uint32_t *)name) |= PCC_CLKCFG_CGC_MASK;
}

/*!
 * @brief Disable the clock for specific IP.
 *
 * @param name  Which clock to disable, see \ref clock_ip_name_t.
 */
static inline void CLOCK_DisableClock(clock_ip_name_t name)
{
    assert((*(volatile uint32_t *)name) & PCC_CLKCFG_PR_MASK);

    (*(volatile uint32_t *)name) &= ~PCC_CLKCFG_CGC_MASK;
}

/*!
 * @brief Check whether the clock is already enabled and configured by
 * any other core.
 *
 * @param name Which peripheral to check, see \ref clock_ip_name_t.
 * @return True if clock is already enabled, otherwise false.
 */
static inline bool CLOCK_IsEnabledByOtherCore(clock_ip_name_t name)
{
    assert((*(volatile uint32_t *)name) & PCC_CLKCFG_PR_MASK);

    return ((*(volatile uint32_t *)name) & PCC_CLKCFG_INUSE_MASK) ? true : false;
}

/*!
 * @brief Set the clock source for specific IP module.
 *
 * Set the clock source for specific IP, not all modules need to set the
 * clock source, should only use this function for the modules need source
 * setting.
 *
 * @param name Which peripheral to check, see \ref clock_ip_name_t.
 * @param src Clock source to set.
 */
static inline void CLOCK_SetIpSrc(clock_ip_name_t name, clock_ip_src_t src)
{
    uint32_t reg = (*(volatile uint32_t *)name);

    assert(reg & PCC_CLKCFG_PR_MASK);
    assert(!(reg & PCC_CLKCFG_INUSE_MASK)); /* Should not change if clock has been enabled by other core. */

    reg = (reg & ~PCC_CLKCFG_PCS_MASK) | PCC_CLKCFG_PCS(src);

    /*
     * If clock is already enabled, first disable it, then set the clock
     * source and re-enable it.
     */
    (*(volatile uint32_t *)name) = reg & ~PCC_CLKCFG_CGC_MASK;
    (*(volatile uint32_t *)name) = reg;
}

/*!
 * @brief Set the clock source and divider for specific IP module.
 *
 * Set the clock source and divider for specific IP, not all modules need to
 * set the clock source and divider, should only use this function for the
 * modules need source and divider setting.
 *
 * Divider output clock = Divider input clock x [(fracValue+1)/(divValue+1)]).
 *
 * @param name Which peripheral to check, see \ref clock_ip_name_t.
 * @param src Clock source to set.
 * @param divValue  The divider value.
 * @param fracValue The fraction multiply value.
 */
static inline void CLOCK_SetIpSrcDiv(clock_ip_name_t name, clock_ip_src_t src, uint8_t divValue, uint8_t fracValue)
{
    uint32_t reg = (*(volatile uint32_t *)name);

    assert(reg & PCC_CLKCFG_PR_MASK);
    assert(!(reg & PCC_CLKCFG_INUSE_MASK)); /* Should not change if clock has been enabled by other core. */

    reg = (reg & ~(PCC_CLKCFG_PCS_MASK  | PCC_CLKCFG_FRAC_MASK | PCC_CLKCFG_PCD_MASK))
               |   PCC_CLKCFG_PCS(src)
               |   PCC_CLKCFG_PCD(divValue)
               |   PCC_CLKCFG_FRAC(fracValue);

    /*
     * If clock is already enabled, first disable it, then set the clock
     * source and re-enable it.
     */
    (*(volatile uint32_t *)name) = reg & ~PCC_CLKCFG_CGC_MASK;
    (*(volatile uint32_t *)name) = reg;
}

/*!
 * @brief Gets the clock frequency for a specific clock name.
 *
 * This function checks the current clock configurations and then calculates
 * the clock frequency for a specific clock name defined in clock_name_t.
 *
 * @param clockName Clock names defined in clock_name_t
 * @return Clock frequency value in hertz
 */
uint32_t CLOCK_GetFreq(clock_name_t clockName);

/*!
 * @brief Gets the clock frequency for a specific IP module.
 *
 * This function gets the IP module clock frequency based on PCC registers. It is
 * only used for the IP modules which could select clock source by PCC[PCS].
 *
 * @param name Which peripheral to get, see \ref clock_ip_name_t.
 * @return Clock frequency value in hertz
 */
uint32_t CLOCK_GetIpFreq(clock_ip_name_t name);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _FSL_CLOCK_H_ */
