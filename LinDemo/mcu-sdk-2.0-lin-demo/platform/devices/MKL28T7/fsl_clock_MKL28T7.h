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
#define DMAMUX_CLOCKS {kCLOCK_Dmamux0, kCLOCK_Dmamux1}

/*! @brief Clock ip name array for RTC. */
#define RTC_CLOCKS {kCLOCK_Rtc0}

/*! @brief Clock ip name array for SCG. */
#define SCG_CLOCKS {kCLOCK_Scg0}

/*! @brief Clock ip name array for SAI. */
#define SAI_CLOCKS {kCLOCK_Sai0}

/*! @brief Clock ip name array for PORT. */
#define PORT_CLOCKS {kCLOCK_PortA, kCLOCK_PortB, kCLOCK_PortC, kCLOCK_PortD, kCLOCK_PortE, kCLOCK_PortM}

/*! @brief Clock ip name array for LPI2C. */
#define LPI2C_CLOCKS {kCLOCK_Lpi2c0, kCLOCK_Lpi2c1, kCLOCK_Lpi2c2}

/*! @brief Clock ip name array for FLEXIO. */
#define FLEXIO_CLOCKS {kCLOCK_Flexio0}

/*! @brief Clock ip name array for TSI. */
#define TSI_CLOCKS {kCLOCK_Tsi0}

/*! @brief Clock ip name array for EMVSIM. */
#define EMVSIM_CLOCKS {kCLOCK_Emvsim0}

/*! @brief Clock ip name array for PCC. */
#define PCC_CLOCKS {kCLOCK_Pcc0, kCLOCK_Pcc1}

/*! @brief Clock ip name array for EDMA. */
#define EDMA_CLOCKS {kCLOCK_Dma0, kCLOCK_Dma1}

/*! @brief Clock ip name array for LPUART. */
#define LPUART_CLOCKS {kCLOCK_Lpuart0, kCLOCK_Lpuart1, kCLOCK_Lpuart2}

/*! @brief Clock ip name array for DAC. */
#define DAC_CLOCKS {kCLOCK_Dac0}

/*! @brief Clock ip name array for LPTMR. */
#define LPTMR_CLOCKS {kCLOCK_Lptmr0, kCLOCK_Lptmr1}

/*! @brief Clock ip name array for ADC16. */
#define ADC16_CLOCKS {kCLOCK_Adc0}

/*! @brief Clock ip name array for ATX. */
#define ATX_CLOCKS {kCLOCK_Atx0}

/*! @brief Clock ip name array for INTMUX. */
#define INTMUX_CLOCKS {kCLOCK_Intmux0, kCLOCK_Intmux1}

/*! @brief Clock ip name array for TRNG. */
#define TRNG_CLOCKS {kCLOCK_Trng0}

/*! @brief Clock ip name array for MMCAU. */
#define MMCAU_CLOCKS {kCLOCK_Mmcau0}

/*! @brief Clock ip name array for LPSPI. */
#define LPSPI_CLOCKS {kCLOCK_Lpspi0, kCLOCK_Lpspi1, kCLOCK_Lpspi2}

/*! @brief Clock ip name array for VREF. */
#define VREF_CLOCKS {kCLOCK_Vref0}

/*! @brief Clock ip name array for TRGMUX. */
#define TRGMUX_CLOCKS {kCLOCK_Trgmux0, kCLOCK_Trgmux1}

/*! @brief Clock ip name array for TSTMR. */
#define TSTMR_CLOCKS {kCLOCK_Tstmr0}

/*! @brief Clock ip name array for MMDVSQ. */
#define MMDVSQ_CLOCKS {kCLOCK_Mmdvsq0}

/*! @brief Clock ip name array for TPM. */
#define TPM_CLOCKS {kCLOCK_Tpm0, kCLOCK_Tpm1, kCLOCK_Tpm2}

/*! @brief Clock ip name array for LPIT. */
#define LPIT_CLOCKS {kCLOCK_Lpit0, kCLOCK_Lpit1}

/*! @brief Clock ip name array for CRC. */
#define CRC_CLOCKS {kCLOCK_Crc0}

/*! @brief Clock ip name array for CMP. */
#define CMP_CLOCKS {kCLOCK_Cmp0, kCLOCK_Cmp1}

/*! @brief Clock ip name array for XRDC. */
#define XRDC_CLOCKS {kCLOCK_Xrdc0}

/*! @brief Clock ip name array for FLASH. */
#define FLASH_CLOCKS {kCLOCK_Flash0}

/*! @brief Clock ip name array for MU. */
#define MU_CLOCKS {kCLOCK_Mu0A, kCLOCK_Mu0B}

/*! @brief Clock ip name array for SEMA. */
#define SEMA_CLOCKS {kCLOCK_Sema420, kCLOCK_Sema421}

/*!
 * @brief LPO clock frequency.
 */
#define LPO_CLK_FREQ 1000U

/*! @brief Clock name used to get clock frequency. */
typedef enum _clock_name
{

    /* ----------------------------- System layer clock -------------------------------*/
    kCLOCK_CoreSysClk = 0U, /*!< Core/system clock                                   */
    kCLOCK_PlatClk = 1U,    /*!< Platform clock                                      */
    kCLOCK_BusClk = 2U,     /*!< Bus clock                                           */
    kCLOCK_FlexBusClk = 3U, /*!< FlexBus clock                                       */
    kCLOCK_FlashClk = 4U,   /*!< Flash clock                                         */

    /* ------------------------------------ SCG clock ---------------------------------*/
    kCLOCK_ScgSysOscClk = 5U, /*!< SCG system OSC clock. (SYSOSC)                      */
    kCLOCK_ScgSircClk = 6U,   /*!< SCG SIRC clock.                                     */
    kCLOCK_ScgFircClk = 7U,   /*!< SCG FIRC clock.                                     */
    kCLOCK_ScgSysPllClk = 8U, /*!< SCG system PLL clock. (SYSPLL)                      */

    kCLOCK_ScgSysOscAsyncDiv1Clk = 9U,  /*!< SOSCDIV1_CLK.                                   */
    kCLOCK_ScgSysOscAsyncDiv2Clk = 10U, /*!< SOSCDIV2_CLK.                                   */
    kCLOCK_ScgSysOscAsyncDiv3Clk = 11U, /*!< SOSCDIV3_CLK.                                   */

    kCLOCK_ScgSircAsyncDiv1Clk = 12U, /*!< SIRCDIV1_CLK.                                   */
    kCLOCK_ScgSircAsyncDiv2Clk = 13U, /*!< SIRCDIV2_CLK.                                   */
    kCLOCK_ScgSircAsyncDiv3Clk = 14U, /*!< SIRCDIV3_CLK.                                   */

    kCLOCK_ScgFircAsyncDiv1Clk = 15U, /*!< FIRCDIV1_CLK.                                   */
    kCLOCK_ScgFircAsyncDiv2Clk = 16U, /*!< FIRCDIV2_CLK.                                   */
    kCLOCK_ScgFircAsyncDiv3Clk = 17U, /*!< FIRCDIV3_CLK.                                   */

    kCLOCK_ScgSysPllAsyncDiv1Clk = 18U, /*!< SPLLDIV1_CLK.                                   */
    kCLOCK_ScgSysPllAsyncDiv2Clk = 19U, /*!< SPLLDIV2_CLK.                                   */
    kCLOCK_ScgSysPllAsyncDiv3Clk = 20U, /*!< SPLLDIV3_CLK.                                   */

    /* --------------------------------- Other clock ----------------------------------*/
    kCLOCK_LpoClk = 21U,    /*!< LPO clock                                           */
    kCLOCK_Osc32kClk = 22U, /*!< External OSC 32K clock (OSC32KCLK)                  */
    kCLOCK_ErClk = 23U,     /*!< ERCLK. The external reference clock from SCG.       */
    kCLOCK_Osc0ErClk = 22U, /*!< for compatible with other MCG platforms. */
    kCLOCK_Er32kClk = 23U,  /*!< For compatible with other MCG platforms. */
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
    kCLOCK_Dma0    = 0x4007A020U,
    kCLOCK_Xrdc0   = 0x4007A050U,
    kCLOCK_Sema420 = 0x4007A06CU,
    kCLOCK_Flash0  = 0x4007A080U,
    kCLOCK_Dmamux0 = 0x4007A084U,
    kCLOCK_Mu0A    = 0x4007A08CU,
    kCLOCK_Intmux0 = 0x4007A090U,
    kCLOCK_Tpm2    = 0x4007A0B8U,
    kCLOCK_Lpit0   = 0x4007A0C0U,
    kCLOCK_Lptmr0  = 0x4007A0D0U,
    kCLOCK_Rtc0    = 0x4007A0E0U,
    kCLOCK_Lpspi2  = 0x4007A0F8U,
    kCLOCK_Lpi2c2  = 0x4007A108U,
    kCLOCK_Lpuart2 = 0x4007A118U,
    kCLOCK_Sai0    = 0x4007A130U,
    kCLOCK_Emvsim0 = 0x4007A138U,
    kCLOCK_Usbfs0  = 0x4007A154U,
    kCLOCK_PortA   = 0x4007A168U,
    kCLOCK_PortB   = 0x4007A16CU,
    kCLOCK_PortC   = 0x4007A170U,
    kCLOCK_PortD   = 0x4007A174U,
    kCLOCK_PortE   = 0x4007A178U,
    kCLOCK_Tsi0    = 0x4007A188U,
    kCLOCK_Adc0    = 0x4007A198U,
    kCLOCK_Dac0    = 0x4007A1A8U,
    kCLOCK_Cmp0    = 0x4007A1B8U,
    kCLOCK_Vref0   = 0x4007A1C8U,
    kCLOCK_Atx0    = 0x4007A1CCU,
    kCLOCK_Crc0    = 0x4007A1E0U,

    /* PCC 1 */
    kCLOCK_Dma1    = 0x400FA020U,
    kCLOCK_Sema421 = 0x400FA06CU,
    kCLOCK_Dmamux1 = 0x400FA084U,
    kCLOCK_Mu0B    = 0x400FA08CU,
    kCLOCK_Intmux1 = 0x400FA090U,
    kCLOCK_Trng0   = 0x400FA094U,
    kCLOCK_Tpm0    = 0x400FA0B0U,
    kCLOCK_Tpm1    = 0x400FA0B4U,
    kCLOCK_Lpit1   = 0x400FA0C4U,
    kCLOCK_Lptmr1  = 0x400FA0D4U,
    kCLOCK_Lpspi0  = 0x400FA0F0U,
    kCLOCK_Lpspi1  = 0x400FA0F4U,
    kCLOCK_Lpi2c0  = 0x400FA100U,
    kCLOCK_Lpi2c1  = 0x400FA104U,
    kCLOCK_Lpuart0 = 0x400FA110U,
    kCLOCK_Lpuart1 = 0x400FA114U,
    kCLOCK_Flexio0 = 0x400FA128U,
    kCLOCK_PortM   = 0x400FA180U,
    kCLOCK_Cmp1    = 0x400FA1BCU,
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
